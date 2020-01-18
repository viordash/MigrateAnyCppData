#include "../MigrateAnyData/MigrateAnyData.h"
#include "TestsCommon.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(const int argc, const char *argv[]) {
	TestData::V00000004::Snapshot::TTestSettings_curr storedSettings = {};
	storedSettings.Data0 = 100;
	storedSettings.Data1 = 101;
	storedSettings.Data2 = 102;
	strcpy(storedSettings.Data3, "103");

	TestData::Initial::Snapshot::TTestSettings_curr settingsInitial = {};
	if (MigrateData::Run(TestData::V00000004::DataMigrate.Version, TestData::Initial::DataMigrate.Version, &DataMigrateItems, &storedSettings,
						 sizeof(storedSettings), &settingsInitial,
						 [](void *parent, void *pItem) -> bool {
							 memcpy(parent, pItem, TestData::Initial::DataMigrate.GetSizeOfCurrentData());
							 return true;
						 })
		!= MigrateRes_Migrate) {
		return EXIT_FAILURE;
	}
	if (settingsInitial.Data0 != 100) {
		return EXIT_FAILURE;
	}

	TestData::V00000002::Snapshot::TTestSettings_curr settingsV00000002 = {};
	if (MigrateData::Run(TestData::V00000004::DataMigrate.Version, TestData::V00000002::DataMigrate.Version, &DataMigrateItems, &storedSettings,
						 sizeof(storedSettings), &settingsV00000002,
						 [](void *parent, void *pItem) -> bool {
							 memcpy(parent, pItem, TestData::V00000002::DataMigrate.GetSizeOfCurrentData());
							 return true;
						 })
		!= TMigrateResult::MigrateRes_Migrate) {
		return EXIT_FAILURE;
	}
	if (settingsV00000002.Data0 != 100) {
		return EXIT_FAILURE;
	}
	if (settingsV00000002.Data1 != 101) {
		return EXIT_FAILURE;
	}

	TestData::V00000003::Snapshot::TTestSettings_curr settingsV00000003 = {};
	if (MigrateData::Run(TestData::V00000004::DataMigrate.Version, TestData::V00000003::DataMigrate.Version, &DataMigrateItems, &storedSettings,
						 sizeof(storedSettings), &settingsV00000003,
						 [](void *parent, void *pItem) -> bool {
							 memcpy(parent, pItem, TestData::V00000003::DataMigrate.GetSizeOfCurrentData());
							 return true;
						 })
		!= TMigrateResult::MigrateRes_Migrate) {
		return EXIT_FAILURE;
	}
	if (settingsV00000003.Data0 != 100) {
		return EXIT_FAILURE;
	}
	if (settingsV00000003.Data1 != 101) {
		return EXIT_FAILURE;
	}
	if (settingsV00000003.Data2 != 102) {
		return EXIT_FAILURE;
	}

	TestData::V00000004::Snapshot::TTestSettings_curr settingsV00000004 = {};
	if (MigrateData::Run(TestData::V00000004::DataMigrate.Version, TestData::V00000004::DataMigrate.Version, &DataMigrateItems, &storedSettings,
						 sizeof(storedSettings), &settingsV00000004, [](void *parent, void *pItem) -> bool { return false; })
		!= TMigrateResult::MigrateRes_Skipped) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}