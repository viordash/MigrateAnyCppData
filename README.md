[![CMake on multiple platforms](https://github.com/viordash/MigrateAnyCppData/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/viordash/MigrateAnyCppData/actions/workflows/cmake-multi-platform.yml)

Library for versioned migration of structured data in C/C++. This library allows you to change these structures without losing any stored data during software updates. The library supports up and down migration between versions. Down migration is performed using intermediate version.

![1717238881056](https://github.com/user-attachments/assets/d0a6b693-46d0-4a3c-9a47-c6e657b859ce)

### How to use:
- add MigrateAnyData/MigrateAnyData.cpp and MigrateAnyData/MigrateAnyData.h to the working project.
- in the module with the intended use of migration, for example call it DeviceConfig, add the storage where the version number and the data are stored. For example, a normal file with the first 4 bytes containing the version number and then the data itself.
- in the DeviceConfig module call MigrateData::Run and pass arguments:
	- storedVersion, a version of the data in the storage. If the storage is empty, then INITIAL_VERSION.
	- currentVersion, current version of the configuration (incremental constant DEVICE_CONFIG_VERSION).
	- pMigrateData, pointer to the migration array (DeviceConfigMigrations).
	- pStoredData, an array of data from the storage (storedData).
	- storedSize, the size of the data array (storedSize).
	- parent, a pointer to a structure with a working configuration (pConfig).
	- onMigrateDataItem, callback function, to copy migrated data into the working configuration. Called after all migrations have ended.
- further actions depend on the result of the method, TMigrateResult:
	- MigrateRes_Migrate, the migration was successful, the working configuration contains the actual data.
	- MigrateRes_Skipped: No migration is needed, as the versions match. Instead, we need to copy the data array from the storage into the working configuration.
	- MigrateRes_Error, error during migration. Additional information in stderr.

### Migrations:
- the migration array (DeviceConfigMigrations) contains the TDataMigrateItems structure. It specifies the list of actions for migration and also the size of the list.

```cpp
const TDataMigrateItems DeviceConfigMigrations = {
	Migrate_DeviceConfig,										   //
	sizeof(Migrate_DeviceConfig) / sizeof(Migrate_DeviceConfig[0]) //
};
```
- the migration list element is a script for transforming data from one version to another.

```cpp
const TDataMigrate Migrate_DeviceConfig[] = {
	{INITIAL_VERSION, 0, 0, 0},
	MigrateDeviceConfig::Initial::DataMigrate,	 //
	MigrateDeviceConfig::V00000002::DataMigrate, //
	MigrateDeviceConfig::V00000003::DataMigrate, //
	MigrateDeviceConfig::V00000004::DataMigrate, //
	MigrateDeviceConfig::V00000005::DataMigrate, //
};
```
- the zero element must be {INITIAL_VERSION(0), 0, 0, 0, 0}.
- migration script is a 'h' file with a set of migration commands. It stores the snapshot (namespace Snapshot) of the current data structure for current version of migration.

```cpp
namespace Snapshot {
	typedef struct {
		char Name[16];
		uint16_t RestartCount;
		uint32_t CommunicationTimeout;
		bool EnableIR;
		bool DisableKeypad;
	} TDeviceConfig, *PTDeviceConfig;
} // namespace Snapshot
```
- link to the scenario of previous version is made through a reference to type in the scenario of previous version (PTDeviceConfig_prev). There is no such link in INITIAL_VERSION script, only initial parameters are set.
- each scenario is in its own namespace, *namespace MigrateDeviceConfig { namespace Initial { ... } namespace V00000005 { ... } }* 
- each scenario in the MigrateUp method performs actions to upgrade to a new version.

```cpp
//DeviceConfig_0005.h MigrateUp
auto pCurrConfig = (Snapshot::PTDeviceConfig)pCurr;
auto pPrevConfig = (PTDeviceConfig_prev)pPrev;
strncpy(pCurrConfig->Name, pPrevConfig->Name, sizeof(pCurrConfig->Name) - 1);
memcpy(&pCurrConfig->RestartCount, &pPrevConfig->RestartCount, sizeof(pCurrConfig->RestartCount));
memcpy(&pCurrConfig->CommunicationTimeout, &pPrevConfig->CommunicationTimeout, sizeof(pCurrConfig->CommunicationTimeout));
memcpy(&pCurrConfig->EnableIR, &pPrevConfig->EnableIR, sizeof(pCurrConfig->EnableIR));
pCurrConfig->DisableKeypad = false;
```	
- each scenario in the MigrateDown method contains commands to roll back to the previous version. A full rollback can only be performed with an intermediate version, which will have a scenario for the new version, but DEVICE_CONFIG_VERSION will be less.

```cpp
//DeviceConfig_0005.h MigrateDown
auto pCurrConfig = (Snapshot::PTDeviceConfig)pCurr;
auto pPrevConfig = (PTDeviceConfig_prev)pPrev;		
strncpy(pPrevConfig->Name, pCurrConfig->Name, sizeof(pPrevConfig->Name) - 1);
memcpy(&pPrevConfig->RestartCount, &pCurrConfig->RestartCount, sizeof(pPrevConfig->RestartCount));
memcpy(&pPrevConfig->CommunicationTimeout, &pCurrConfig->CommunicationTimeout, sizeof(pPrevConfig->CommunicationTimeout));
memcpy(&pPrevConfig->EnableIR, &pCurrConfig->EnableIR, sizeof(pPrevConfig->EnableIR));
```
- it can be used to migrate an array of tabular data. Migration occurs for each element in turn, element-by-element saving to the target dataset must be performed in the callback TOnMigrateDataItem.

Minimum list of changes when adding a new version of version data:
 - create 'h' file of migration scenario. It can be simplified by copying from the latest version.
	- rename the namespace (V00000006).
	- specify the version number (const TDataMigrate DataMigrate = {0x00000006, ...) 
	- renaming namespace of link to previous version, PTDeviceConfig_prev (V00000005).
	- create a "snapshot" of current version data in namespace Snapshot.
	- implement body of MigrateUp and MigrateDown methods.
- add an entry to the end of the Migrate_DeviceConfig array (MigrateDeviceConfig::V00000006::DataMigrate). 

Version names in namespace (V00000006) can be changed to calendar format for convenience, such as V20200118.

------------
Usage example located in main.cpp / DeviceConfig.cpp / DeviceConfig.h.
The TDeviceConfig structure stores the device settings.
Methods:
```cpp
bool ReadDeviceConfig(PTDeviceConfig pConfig);  //read config
void StoreDeviceConfig(PTDeviceConfig pConfig); //store config
 ```
The config file ("DeviceConfig.dat") also stores the current version number.
In this example there are already 5 migrations (DEVICE_CONFIG_VERSION) and the scenario of their occurrence is:
- **DeviceConfig_Initial.h**. Initial version, the config stores only the Name[8] field.
- **DeviceConfig_0002.h**. Name field has been increased to 16 characters.
- **DeviceConfig_0003.h**. CommunicationTimeout and spare[4] fields were added.
- **DeviceConfig_0004.h**. The spare[4] field has been removed. The EnableIR field has been added.
- **DeviceConfig_0005.h**. Added DisableKeypad field.
