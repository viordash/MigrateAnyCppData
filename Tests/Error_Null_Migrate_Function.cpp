#include "../MigrateAnyData/MigrateAnyData.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace NullMigrateFuncTest {
	namespace StoredVersion {
		inline int GetSizeOfCurrentData() {
			return sizeof(uint8_t);
		}
		inline void MigrateUp(void *pCurr, void *pPrev) {
		}
		inline void MigrateDown(void *pCurr, void *pPrev) {
		}

		const TDataMigrate DataMigrate = {0x00000001, MigrateUp, MigrateDown, GetSizeOfCurrentData};
	} // namespace StoredVersion

	namespace CurrentVersionNullUp {
		inline int GetSizeOfCurrentData() {
			return sizeof(uint32_t);
		}
		inline void MigrateDown(void *pCurr, void *pPrev) {
		}

		const TDataMigrate DataMigrate = {0x00000002, NULL, MigrateDown, GetSizeOfCurrentData};
	} // namespace CurrentVersionNullUp

	namespace CurrentVersionNullDown {
		inline int GetSizeOfCurrentData() {
			return sizeof(uint32_t);
		}
		inline void MigrateUp(void *pCurr, void *pPrev) {
		}

		const TDataMigrate DataMigrate = {0x00000003, MigrateUp, NULL, GetSizeOfCurrentData};
	} // namespace CurrentVersionNullDown
} // namespace NullMigrateFuncTest

const TDataMigrate NullUpTestMigrate_Data[] = {
	{INITIAL_VERSION, 0, 0, 0},
	NullMigrateFuncTest::StoredVersion::DataMigrate,
	NullMigrateFuncTest::CurrentVersionNullUp::DataMigrate,
};

const TDataMigrateItems NullUpDataMigrateItems = {
	NullUpTestMigrate_Data,
	sizeof(NullUpTestMigrate_Data) / sizeof(NullUpTestMigrate_Data[0])
};

const TDataMigrate NullDownTestMigrate_Data[] = {
	{INITIAL_VERSION, 0, 0, 0},
	NullMigrateFuncTest::StoredVersion::DataMigrate,
	NullMigrateFuncTest::CurrentVersionNullDown::DataMigrate,
};

const TDataMigrateItems NullDownDataMigrateItems = {
	NullDownTestMigrate_Data,
	sizeof(NullDownTestMigrate_Data) / sizeof(NullDownTestMigrate_Data[0])
};

int main(const int argc, const char *argv[]) {
	uint8_t storedData = 42;
	uint32_t currentData = 0;

	auto onMigrateDataItem = [](void *parent, void *pItem) -> bool {
		auto current = (uint32_t *)parent;
		memcpy(current, pItem, sizeof(uint32_t));
		return true;
	};

	// Test 1: MigrateUp is NULL during upgrade
	TMigrateResult result = MigrateData::Run(
		NullMigrateFuncTest::StoredVersion::DataMigrate.Version,
		NullMigrateFuncTest::CurrentVersionNullUp::DataMigrate.Version,
		&NullUpDataMigrateItems,
		&storedData,
		sizeof(storedData),
		&currentData,
		onMigrateDataItem);

	if (result != MigrateRes_Error) {
		fprintf(stderr, "Test 1 failed: Expected MigrateRes_Error when MigrateUp is NULL\n");
		return EXIT_FAILURE;
	}
	printf("Test 1 passed: null MigrateUp correctly returns error\n");

	// Test 2: MigrateDown is NULL during downgrade
	uint32_t storedDataV3 = 123;
	uint8_t currentDataV1 = 0;

	auto onMigrateDataItemV1 = [](void *parent, void *pItem) -> bool {
		auto current = (uint8_t *)parent;
		memcpy(current, pItem, sizeof(uint8_t));
		return true;
	};

	result = MigrateData::Run(
		NullMigrateFuncTest::CurrentVersionNullDown::DataMigrate.Version,
		NullMigrateFuncTest::StoredVersion::DataMigrate.Version,
		&NullDownDataMigrateItems,
		&storedDataV3,
		sizeof(storedDataV3),
		&currentDataV1,
		onMigrateDataItemV1);

	if (result != MigrateRes_Error) {
		fprintf(stderr, "Test 2 failed: Expected MigrateRes_Error when MigrateDown is NULL\n");
		return EXIT_FAILURE;
	}
	printf("Test 2 passed: null MigrateDown correctly returns error\n");

	printf("All tests passed\n");
	return EXIT_SUCCESS;
}
