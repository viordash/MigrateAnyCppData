#include "../MigrateAnyData/MigrateAnyData.h"
#include "TestsCommon.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(const int argc, const char *argv[]) {
	TestData::V00000003::Snapshot::TTestSettings_curr storedSettings = {};
	storedSettings.Data0 = 100;
	storedSettings.Data1 = 101;
	storedSettings.Data2 = 102;

	TestData::V00000002::Snapshot::TTestSettings_curr settingsV00000002 = {};
	if (MigrateData::Run(TestData::V00000003::DataMigrate.Version, TestData::V00000002::DataMigrate.Version, &DataMigrateItems, &storedSettings,
						 sizeof(storedSettings), &settingsV00000002,
						 [](void *parent, void *pItem) -> bool {
							 memcpy(parent, pItem, TestData::V00000002::DataMigrate.GetSizeOfCurrentData());
							 return true;
						 })
		!= MigrateRes_Migrate) {
		return EXIT_FAILURE;
	}
	if (settingsV00000002.Data0 != 100) {
		return EXIT_FAILURE;
	}
	if (settingsV00000002.Data1 != 101) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}