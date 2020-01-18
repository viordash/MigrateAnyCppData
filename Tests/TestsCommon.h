#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <memory.h>
#include "../MigrateAnyData/MigrateAnyData.h"



namespace TestData {
	namespace Initial {
		inline int GetSizeOfCurrentData();
		inline void MigrateUp(void *pCurr, void *pPrev);
		inline void MigrateDown(void *pCurr, void *pPrev);

		const TDataMigrate DataMigrate = {0x00000001, MigrateUp, MigrateDown, GetSizeOfCurrentData};

		namespace Snapshot {
			typedef struct {
				uint8_t Data0;
			} TTestSettings_curr, *PTTestSettings_curr;
		} // namespace Snapshot

		inline int GetSizeOfCurrentData() {
			return sizeof(Snapshot::TTestSettings_curr);
		}

		inline void MigrateUp(void *pCurr, void *pPrev) {
			Snapshot::PTTestSettings_curr pCurrSettings = (Snapshot::PTTestSettings_curr)pCurr;
			memset(pCurrSettings, 0, sizeof(*pCurrSettings));
			printf("TestSettings. Initial migrate to v%08X\n", DataMigrate.Version);
		}

		inline void MigrateDown(void *pCurr, void *pPrev) {
		}

	} // namespace Initial

	namespace V00000002 {
		inline int GetSizeOfCurrentData();
		inline void MigrateUp(void *pCurr, void *pPrev);
		inline void MigrateDown(void *pCurr, void *pPrev);

		const TDataMigrate DataMigrate = {0x00000002, MigrateUp, MigrateDown, GetSizeOfCurrentData};
		typedef Initial::Snapshot::TTestSettings_curr TTestSettings_prev, *PTTestSettings_prev;

		namespace Snapshot {
			typedef struct {
				uint8_t Data0;
				uint16_t Data1;
			} TTestSettings_curr, *PTTestSettings_curr;
		} // namespace Snapshot

		inline int GetSizeOfCurrentData() {
			return sizeof(Snapshot::TTestSettings_curr);
		}

		inline void MigrateUp(void *pCurr, void *pPrev) {
			Snapshot::PTTestSettings_curr pCurrSettings = (Snapshot::PTTestSettings_curr)pCurr;
			PTTestSettings_prev pPrevSettings = (PTTestSettings_prev)pPrev;
			pCurrSettings->Data0 = pPrevSettings->Data0;
			pCurrSettings->Data1 = 2;
			printf("TestSettings. MigrateUp to v%08X\n", DataMigrate.Version);
		}

		inline void MigrateDown(void *pCurr, void *pPrev) {
			Snapshot::PTTestSettings_curr pCurrSettings = (Snapshot::PTTestSettings_curr)pCurr;
			PTTestSettings_prev pPrevSettings = (PTTestSettings_prev)pPrev;
			pPrevSettings->Data0 = pCurrSettings->Data0;
			printf("TestSettings. MigrateDown from v%08X\n", DataMigrate.Version);
		}

	} // namespace V00000002

	namespace V00000003 {
		inline int GetSizeOfCurrentData();
		inline void MigrateUp(void *pCurr, void *pPrev);
		inline void MigrateDown(void *pCurr, void *pPrev);

		const TDataMigrate DataMigrate = {0x00000003, MigrateUp, MigrateDown, GetSizeOfCurrentData};
		typedef V00000002::Snapshot::TTestSettings_curr TTestSettings_prev, *PTTestSettings_prev;

		namespace Snapshot {
			typedef struct {
				uint8_t Data0;
				uint16_t Data1;
				uint32_t Data2;
			} TTestSettings_curr, *PTTestSettings_curr;
		} // namespace Snapshot

		inline int GetSizeOfCurrentData() {
			return sizeof(Snapshot::TTestSettings_curr);
		}

		inline void MigrateUp(void *pCurr, void *pPrev) {
			Snapshot::PTTestSettings_curr pCurrSettings = (Snapshot::PTTestSettings_curr)pCurr;
			PTTestSettings_prev pPrevSettings = (PTTestSettings_prev)pPrev;
			pCurrSettings->Data0 = pPrevSettings->Data0;
			pCurrSettings->Data1 = pPrevSettings->Data1;
			pCurrSettings->Data2 = 3;
			printf("TestSettings. MigrateUp to v%08X\n", DataMigrate.Version);
		}

		inline void MigrateDown(void *pCurr, void *pPrev) {
			Snapshot::PTTestSettings_curr pCurrSettings = (Snapshot::PTTestSettings_curr)pCurr;
			PTTestSettings_prev pPrevSettings = (PTTestSettings_prev)pPrev;
			pPrevSettings->Data0 = pCurrSettings->Data0;
			pPrevSettings->Data1 = pCurrSettings->Data1;
			printf("TestSettings. MigrateDown from v%08X\n", DataMigrate.Version);
		}

	} // namespace V00000003

	namespace V00000004 {
		inline int GetSizeOfCurrentData();
		inline void MigrateUp(void *pCurr, void *pPrev);
		inline void MigrateDown(void *pCurr, void *pPrev);

		const TDataMigrate DataMigrate = {0x00000004, MigrateUp, MigrateDown, GetSizeOfCurrentData};
		typedef V00000003::Snapshot::TTestSettings_curr TTestSettings_prev, *PTTestSettings_prev;

		namespace Snapshot {
			typedef struct {
				uint8_t Data0;
				uint16_t Data1;
				uint32_t Data2;
				char Data3[16];
			} TTestSettings_curr, *PTTestSettings_curr;
		} // namespace Snapshot

		inline int GetSizeOfCurrentData() {
			return sizeof(Snapshot::TTestSettings_curr);
		}

		inline void MigrateUp(void *pCurr, void *pPrev) {
			Snapshot::PTTestSettings_curr pCurrSettings = (Snapshot::PTTestSettings_curr)pCurr;
			PTTestSettings_prev pPrevSettings = (PTTestSettings_prev)pPrev;
			pCurrSettings->Data0 = pPrevSettings->Data0;
			pCurrSettings->Data1 = pPrevSettings->Data1;
			pCurrSettings->Data2 = pPrevSettings->Data2;
			strncpy(pCurrSettings->Data3, "test_V00000004", sizeof(pCurrSettings->Data3));
			printf("TestSettings. MigrateUp to v%08X\n", DataMigrate.Version);
		}

		inline void MigrateDown(void *pCurr, void *pPrev) {
			Snapshot::PTTestSettings_curr pCurrSettings = (Snapshot::PTTestSettings_curr)pCurr;
			PTTestSettings_prev pPrevSettings = (PTTestSettings_prev)pPrev;
			pPrevSettings->Data0 = pCurrSettings->Data0;
			pPrevSettings->Data1 = pCurrSettings->Data1;
			pPrevSettings->Data2 = pCurrSettings->Data2;
			printf("TestSettings. MigrateDown from v%08X\n", DataMigrate.Version);
		}
	} // namespace V00000004
} // namespace TestData


extern const TDataMigrate TestMigrate_Data[5];
extern const TDataMigrateItems DataMigrateItems;