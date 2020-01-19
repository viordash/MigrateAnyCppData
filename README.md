Модуль для версионной миграции структурируемых данных с C/C++, при обновлении программного обеспечения.
Поддерживается миграция вверх и вниз. Миграция вниз через промежуточную версию.
После каждого изменения формата версионных данных создается новая миграция, которая будет вызвана при переходе на эту версию.

Порядок использования:
- миграция, это h файл с набором действий по переносу данных. Также хранит "слепок" (namespace Snapshot) текущего, для этой версии миграции, формата данных.
- каждый миграция заключена в свой namespace, *namespace MigrateDeviceConfig { namespace Initial { ... } namespace V00000002 { ... } }* 
- привязки к типу мигрируемых данных нет, трансформация и перенос происходит в методах MigrateUp и MigrateDown
- массив миграций, с элементами типа TDataMigrate, это сценарий для трансформации данных с "чистого" состояния или любого из промежуточных, до текущего состояния

		const TDataMigrate Migrate_DeviceConfig[] = {
			{INITIAL_VERSION, 0, 0, 0},
			MigrateDeviceConfig::Initial::DataMigrate,	 //
			MigrateDeviceConfig::V00000002::DataMigrate, //
			MigrateDeviceConfig::V00000003::DataMigrate, //
			MigrateDeviceConfig::V00000004::DataMigrate, //
		};

- нулевым элементом обязательно должен быть {INITIAL_VERSION(0), 0, 0, 0}.
- в каждой миграции необходимо описать последовательность переноса данных между текущей и предыдущей версиями. 

		//DeviceConfig_0004.h MigrateUp
		strncpy(pCurrConfig->Name, pPrevConfig->Name, sizeof(pCurrConfig->Name) - 1);
		memcpy(&pCurrConfig->RestartCount, &pPrevConfig->RestartCount, sizeof(pCurrConfig->RestartCount));
		memcpy(&pCurrConfig->CommunicationTimeout, &pPrevConfig->CommunicationTimeout, sizeof(pCurrConfig->CommunicationTimeout));
		pCurrConfig->EnableIR = true;
		
		//DeviceConfig_0004.h MigrateDown
		strncpy(pPrevConfig->Name, pCurrConfig->Name, sizeof(pPrevConfig->Name) - 1);
		memcpy(&pPrevConfig->RestartCount, &pCurrConfig->RestartCount, sizeof(pPrevConfig->RestartCount));
		memcpy(&pPrevConfig->CommunicationTimeout, &pCurrConfig->CommunicationTimeout, sizeof(pPrevConfig->CommunicationTimeout));
		memset(pPrevConfig->spare, 0, sizeof(pPrevConfig->spare));	
		
- callback MigrateData::Run TOnMigrateDataItem вызывается после осуществления миграции, и предназначен для того чтобы скопировать мигрированные данные на целевые.
- при совпадении версий, миграция пропускается. В этом случае из метода MigrateData::Run возвращается результат MigrateRes_Skipped, и необходимо вручную скопировать сохраненные данные в целевые.
- можно применять для миграции массива табличных данных. Миграция происходит для каждого элемента поочереди, поэлементное сохранение в целевой набор данных, необходимо производить в callback TOnMigrateDataItem.


------------
Пример использования в main.cpp / DeviceConfig.cpp / DeviceConfig.h.
В структуре TDeviceConfig хранятся настройки устройства.
Методы:
 - bool ReadDeviceConfig(PTDeviceConfig pConfig);  //чтение настроек
 - void StoreDeviceConfig(PTDeviceConfig pConfig); //сохранение настроек
 
Файл с конфигом хранит также и номер текущей версии.
В данном примере имеются уже 5 миграций (DEVICE_CONFIG_VERSION), и сценарий их возникновения таков:
- **DeviceConfig_Initial.h**. Первоначальная версия, конфиг хранит только поле Name[8].
- **DeviceConfig_0002.h**. Поле Name увеличено до 16 символов.
- **DeviceConfig_0003.h**. Добавлены поля CommunicationTimeout и spare[4].
- **DeviceConfig_0004.h**. Удалено поле spare[4]. Добавлены поле EnableIR.
- **DeviceConfig_0005.h**. Добавлены поле DisableKeypad.
