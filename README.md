Модуль версионной миграции структурных данных в C/C++, при обновлениях программного обеспечения с изменением этих структур.
Поддерживается миграция вверх и вниз по версиям. Миграция вниз через промежуточную версию.

Использование библитотеки:
- подключить MigrateAnyData/MigrateAnyData.cpp и MigrateAnyData/MigrateAnyData.h к рабочему проекту
- в модуль, с предполагаемым использованием миграции, для примера назовем его DeviceConfig, добавить хранилище,  в котором храненится номер версии и сами данные. Например обычный файл, в первых 4-х байтах которого хранится номер версии, а затем уже сами данные.
- в модуле DeviceConfig вызываем MigrateData::Run и передаем аргументы:
	- storedVersion, версия данных в хранилище. Если хранилище пустое, то INITIAL_VERSION.
	- currentVersion, текущая версия конфигурации (инкрементальная константа DEVICE_CONFIG_VERSION).
	- pMigrateData, указатель на массив миграций (DeviceConfigMigrations).
	- pStoredData, массив с данными из хранилища (storedData).
	- storedSize, размер массива с данными (storedSize).
	- parent, указатель на структуру с рабочей конфигурацей (pConfig).
	- onMigrateDataItem, callback функция, для копирования мигрированных данных в рабочую конфигурацию. Вызывается по окончании всех миграций.
- дальнейшие действия зависят от результата работы метода, TMigrateResult:
	- MigrateRes_Migrate, миграция произведена успешно, в рабочей конфигурации находятся актуальные данные.
	- MigrateRes_Skipped, не было необходимости в миграции, версии соответствуют. Необходимо скопировать массив с данными из хранилища в рабочую конфигурации.
	- MigrateRes_Error, ошибка при миграции. Дополнительная информация в stderr.

Миграции:
- массив миграций (DeviceConfigMigrations) содержит структуру TDataMigrateItems. В ней указывается список действий для проведения миграции и также размер этого списка.

		const TDataMigrateItems DeviceConfigMigrations = {
			Migrate_DeviceConfig,										   //
			sizeof(Migrate_DeviceConfig) / sizeof(Migrate_DeviceConfig[0]) //
		};

- элемент списка миграции, это сценарий для трансформации данных с одной версии на другую. 

		const TDataMigrate Migrate_DeviceConfig[] = {
			{INITIAL_VERSION, 0, 0, 0},
			MigrateDeviceConfig::Initial::DataMigrate,	 //
			MigrateDeviceConfig::V00000002::DataMigrate, //
			MigrateDeviceConfig::V00000003::DataMigrate, //
			MigrateDeviceConfig::V00000004::DataMigrate, //
			MigrateDeviceConfig::V00000005::DataMigrate, //
		};

- нулевым элементом обязательно должен быть {INITIAL_VERSION(0), 0, 0, 0}.
- сценарий миграции, это h файл с набором команд по переносу данных. В нем хранится "отпечаток" (namespace Snapshot) структуры данных текущего для этой версии миграции.

		namespace Snapshot {
			typedef struct {
				char Name[16];
				uint16_t RestartCount;
				uint32_t CommunicationTimeout;
				bool EnableIR;
				bool DisableKeypad;
			} TDeviceConfig, *PTDeviceConfig;
		} // namespace Snapshot

- связь со сценарием прошлой версии производится через ссылку на тип в сценарии прошлой версии (PTDeviceConfig_prev). В сценарии для INITIAL_VERSION такой ссылки нет, производится только установка начальных параметров.
- каждый сценарий заключен в свой namespace, *namespace MigrateDeviceConfig { namespace Initial { ... } namespace V00000005 { ... } }* 
- каждый сценарий в методе MigrateUp производит действия для обновления на новую версию.

		//DeviceConfig_0005.h MigrateUp
		auto pCurrConfig = (Snapshot::PTDeviceConfig)pCurr;
		auto pPrevConfig = (PTDeviceConfig_prev)pPrev;
		strncpy(pCurrConfig->Name, pPrevConfig->Name, sizeof(pCurrConfig->Name) - 1);
		memcpy(&pCurrConfig->RestartCount, &pPrevConfig->RestartCount, sizeof(pCurrConfig->RestartCount));
		memcpy(&pCurrConfig->CommunicationTimeout, &pPrevConfig->CommunicationTimeout, sizeof(pCurrConfig->CommunicationTimeout));
		memcpy(&pCurrConfig->EnableIR, &pPrevConfig->EnableIR, sizeof(pCurrConfig->EnableIR));
		pCurrConfig->DisableKeypad = false;
		
- каждый сценарий в методе MigrateDown содержит команды для отката на предыдущую версию. Полноценный откат возможно произвести только с промежуточной версией, в которой будет сценарий новой версии, но DEVICE_CONFIG_VERSION будет меньше

		//DeviceConfig_0005.h MigrateDown
		auto pCurrConfig = (Snapshot::PTDeviceConfig)pCurr;
		auto pPrevConfig = (PTDeviceConfig_prev)pPrev;		
		strncpy(pPrevConfig->Name, pCurrConfig->Name, sizeof(pPrevConfig->Name) - 1);
		memcpy(&pPrevConfig->RestartCount, &pCurrConfig->RestartCount, sizeof(pPrevConfig->RestartCount));
		memcpy(&pPrevConfig->CommunicationTimeout, &pCurrConfig->CommunicationTimeout, sizeof(pPrevConfig->CommunicationTimeout));
		memcpy(&pPrevConfig->EnableIR, &pCurrConfig->EnableIR, sizeof(pPrevConfig->EnableIR));

- можно применять для миграции массива табличных данных. Миграция происходит для каждого элемента поочереди, поэлементное сохранение в целевой набор данных, необходимо производить в callback TOnMigrateDataItem.

Минимальный перечень изменений при добавлении новой версии версионных данных:
 - создание h файла сценария миграции. Можно упростить путем копипаста последней версии.
	- переименование namespace (V00000006).
	- указание номера версии (const TDataMigrate DataMigrate = {0x00000006, ...) 
	- переименование namespace для ссылки на предыдущую версию, PTDeviceConfig_prev (V00000005).
	- создания "отпечатка" текущих версионных данных в namespace Snapshot.
	- реализация тела методов MigrateUp и MigrateDown.
- добавление записи в конец списка Migrate_DeviceConfig (MigrateDeviceConfig::V00000006::DataMigrate, //). 

Названия версий в namespace (V00000006) для удобства можно изменить на календарный формат, например V20200118.

------------
Пример использования в main.cpp / DeviceConfig.cpp / DeviceConfig.h.
В структуре TDeviceConfig хранятся настройки устройства.
Методы:
 - bool ReadDeviceConfig(PTDeviceConfig pConfig);  //чтение настроек
 - void StoreDeviceConfig(PTDeviceConfig pConfig); //сохранение настроек
 
Файл с конфигом ("DeviceConfig.dat") хранит также и номер текущей версии.
В данном примере имеются уже 5 миграций (DEVICE_CONFIG_VERSION), и сценарий их возникновения таков:
- **DeviceConfig_Initial.h**. Первоначальная версия, конфиг хранит только поле Name[8].
- **DeviceConfig_0002.h**. Поле Name увеличено до 16 символов.
- **DeviceConfig_0003.h**. Добавлены поля CommunicationTimeout и spare[4].
- **DeviceConfig_0004.h**. Удалено поле spare[4]. Добавлены поле EnableIR.
- **DeviceConfig_0005.h**. Добавлены поле DisableKeypad.
