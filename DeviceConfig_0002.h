#pragma once

#include "MigrateAnyData/MigrateAnyData.h"

namespace MigrateDeviceConfig {
	namespace V00000002 {
		inline int GetSizeOfCurrentData();
		inline void MigrateUp(void *pCurr, void *pPrev);
		inline void MigrateDown(void *pCurr, void *pPrev);

		const TDataMigrate DataMigrate = {0x00000002, MigrateUp, MigrateDown, GetSizeOfCurrentData};
		typedef Initial::Snapshot::TDeviceConfig TDeviceConfig_prev, *PTDeviceConfig_prev;

		namespace Snapshot {
			typedef struct {
				char Name[16];
			} TDeviceConfig, *PTDeviceConfig;
		} // namespace Snapshot

		inline int GetSizeOfCurrentData() {
			return sizeof(Snapshot::TDeviceConfig);
		}

		inline void MigrateUp(void *pCurr, void *pPrev) {
			auto pCurrConfig = (Snapshot::PTDeviceConfig)pCurr;
			auto pPrevConfig = (PTDeviceConfig_prev)pPrev;

			strncpy(pCurrConfig->Name, pPrevConfig->Name, sizeof(pCurrConfig->Name) - 1);
			fprintf(stdout, "DeviceConfig. MigrateUp to %04X\n", DataMigrate.Version);
		}

		inline void MigrateDown(void *pCurr, void *pPrev) {
			auto pCurrConfig = (Snapshot::PTDeviceConfig)pCurr;
			auto pPrevConfig = (PTDeviceConfig_prev)pPrev;

			strncpy(pPrevConfig->Name, pCurrConfig->Name, sizeof(pPrevConfig->Name) - 1);
			fprintf(stdout, "DeviceConfig. MigrateDown from %04X\n", DataMigrate.Version);
		}
	} // namespace V00000002
} // namespace MigrateDeviceConfig
