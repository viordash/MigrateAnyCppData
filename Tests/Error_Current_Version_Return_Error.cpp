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
	if (MigrateData::Run(TestData::V00000002::DataMigrate.Version, 0xFFFFFFFF, &DataMigrateItems, &storedSettings, sizeof(storedSettings), &settingsV00000003,
						 onMigrateDataItem)
		!= MigrateRes_Error) {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}