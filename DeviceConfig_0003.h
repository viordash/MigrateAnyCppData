#pragma once

#include "MigrateAnyData/MigrateAnyData.h"

namespace MigrateDeviceConfig {
	namespace V00000003 {
		inline int GetSizeOfCurrentData();
		inline void MigrateUp(void *pCurr, void *pPrev);
		inline void MigrateDown(void *pCurr, void *pPrev);

		const TDataMigrate DataMigrate = {0x00000003, MigrateUp, MigrateDown, GetSizeOfCurrentData};
		typedef V00000002::Snapshot::TDeviceConfig TDeviceConfig_prev, *PTDeviceConfig_prev;

		namespace Snapshot {
			typedef struct {
				char Name[16];
				uint16_t RestartCount;
				uint32_t CommunicationTimeout;
				uint32_t spare[4];
			} TDeviceConfig, *PTDeviceConfig;
		} // namespace Snapshot

		inline int GetSizeOfCurrentData() {
			return sizeof(Snapshot::TDeviceConfig);
		}

		inline void MigrateUp(void *pCurr, void *pPrev) {
			auto pCurrConfig = (Snapshot::PTDeviceConfig)pCurr;
			auto pPrevConfig = (PTDeviceConfig_prev)pPrev;

			strncpy(pCurrConfig->Name, pPrevConfig->Name, sizeof(pCurrConfig->Name) - 1);
			pCurrConfig->RestartCount = 0;
			pCurrConfig->CommunicationTimeout = 1500;
			memset(pCurrConfig->spare, 0, sizeof(pCurrConfig->spare));
			fprintf(stdout, "DeviceConfig. MigrateUp to %04X\n", DataMigrate.Version);
		}

		inline void MigrateDown(void *pCurr, void *pPrev) {
			auto pCurrConfig = (Snapshot::PTDeviceConfig)pCurr;
			auto pPrevConfig = (PTDeviceConfig_prev)pPrev;

			strncpy(pPrevConfig->Name, pCurrConfig->Name, sizeof(pPrevConfig->Name) - 1);
			fprintf(stdout, "DeviceConfig. MigrateDown from %04X\n", DataMigrate.Version);
		}
	} // namespace V00000003
} // namespace MigrateDeviceConfig
