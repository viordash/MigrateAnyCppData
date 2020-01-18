#include "../MigrateAnyData/MigrateAnyData.h"
#include "TestsCommon.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(const int argc, const char *argv[]) {
	TestData::V00000002::Snapshot::TTestSettings_curr storedSettings = {};
	storedSettings.Data0 = 110;
	storedSettings.Data1 = 111;

	auto onMigrateDataItem = [](void *parent, void *pItem) -> bool { //
		auto currentSettings = (TestData::V00000003::Snapshot::TTestSettings_curr *)parent;
		memcpy(currentSettings, pItem, TestData::V00000003::DataMigrate.GetSizeOfCurrentData());
		return true;
	};

	TestData::V00000003::Snapshot::TTestSettings_curr settingsV00000003 = {};
	if (MigrateData::Run(TestData::V00000002::DataMigrate.Version, TestData::V00000003::DataMigrate.Version, &DataMigrateItems, &storedSettings,
						 sizeof(storedSettings), &settingsV00000003, onMigrateDataItem)
		!= MigrateRes_Migrate) {
		return EXIT_FAILURE;
	}

	if (settingsV00000003.Data0 != 110) {
		return EXIT_FAILURE;
	}
	if (settingsV00000003.Data1 != 111) {
		return EXIT_FAILURE;
	}
	if (settingsV00000003.Data2 != 3) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}