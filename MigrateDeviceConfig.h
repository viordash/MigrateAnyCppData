#pragma once

#include "DeviceConfig.h"
#include "DeviceConfig_Initial.h"
#include "DeviceConfig_0002.h"
#include "DeviceConfig_0003.h"
#include "DeviceConfig_0004.h"
#include "DeviceConfig_0005.h"
#include "MigrateAnyData/MigrateAnyData.h"

const TDataMigrate Migrate_DeviceConfig[] = {
	{INITIAL_VERSION, 0, 0, 0},
	MigrateDeviceConfig::Initial::DataMigrate,	 //
	MigrateDeviceConfig::V00000002::DataMigrate, //
	MigrateDeviceConfig::V00000003::DataMigrate, //
	MigrateDeviceConfig::V00000004::DataMigrate, //
	MigrateDeviceConfig::V00000005::DataMigrate, //
};

const TDataMigrateItems DeviceConfigMigrations = {
	Migrate_DeviceConfig,										   //
	sizeof(Migrate_DeviceConfig) / sizeof(Migrate_DeviceConfig[0]) //
};
