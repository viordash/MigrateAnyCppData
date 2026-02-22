#include "../MigrateAnyData/MigrateAnyData.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace ZeroSizeTest {
	namespace StoredVersion {
		inline int GetSizeOfCurrentData() {
			return 0;
		}
		inline void MigrateUp(void *pCurr, void *pPrev) {
		}
		inline void MigrateDown(void *pCurr, void *pPrev) {
		}

		const TDataMigrate DataMigrate = {0x00000001, MigrateUp, MigrateDown, GetSizeOfCurrentData};
	} // namespace StoredVersion

	namespace CurrentVersion {
		inline int GetSizeOfCurrentData() {
			return sizeof(uint32_t);
		}
		inline void MigrateUp(void *pCurr, void *pPrev) {
		}
		inline void MigrateDown(void *pCurr, void *pPrev) {
		}

		const TDataMigrate DataMigrate = {0x00000002, MigrateUp, MigrateDown, GetSizeOfCurrentData};
	} // namespace CurrentVersion
} // namespace ZeroSizeTest

const TDataMigrate ZeroSizeTestMigrate_Data[] = {
	{INITIAL_VERSION, 0, 0, 0},
	ZeroSizeTest::StoredVersion::DataMigrate,
	ZeroSizeTest::CurrentVersion::DataMigrate,
};

const TDataMigrateItems ZeroSizeDataMigrateItems = {
	ZeroSizeTestMigrate_Data,
	sizeof(ZeroSizeTestMigrate_Data) / sizeof(ZeroSizeTestMigrate_Data[0])
};

int main(const int argc, const char *argv[]) {
	uint8_t storedData[4] = {1, 2, 3, 4};
	uint32_t currentData = 0;

	auto onMigrateDataItem = [](void *parent, void *pItem) -> bool {
		auto current = (uint32_t *)parent;
		memcpy(current, pItem, sizeof(uint32_t));
		return true;
	};

	TMigrateResult result = MigrateData::Run(
		ZeroSizeTest::StoredVersion::DataMigrate.Version,
		ZeroSizeTest::CurrentVersion::DataMigrate.Version,
		&ZeroSizeDataMigrateItems,
		storedData,
		sizeof(storedData),
		&currentData,
		onMigrateDataItem);

	if (result != MigrateRes_Error) {
		fprintf(stderr, "Expected MigrateRes_Error when stored record size is zero\n");
		return EXIT_FAILURE;
	}

	printf("Test passed: zero record size correctly returns error\n");
	return EXIT_SUCCESS;
}
