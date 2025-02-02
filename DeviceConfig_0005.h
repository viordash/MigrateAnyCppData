#pragma once

#include "MigrateAnyData/MigrateAnyData.h"

namespace MigrateDeviceConfig {
	namespace V00000005 {
		inline int GetSizeOfCurrentData();
		inline void MigrateUp(void *pCurr, void *pPrev);
		inline void MigrateDown(void *pCurr, void *pPrev);

		const TDataMigrate DataMigrate = {0x00000005, MigrateUp, MigrateDown, GetSizeOfCurrentData};
		typedef V00000004::Snapshot::TDeviceConfig TDeviceConfig_prev, *PTDeviceConfig_prev;

		namespace Snapshot {
			typedef struct {
				char Name[16];
				uint16_t RestartCount;
				uint32_t CommunicationTimeout;
				bool EnableIR;
				bool DisableKeypad;
			} TDeviceConfig, *PTDeviceConfig;
		} // namespace Snapshot

		inline int GetSizeOfCurrentData() {
			return sizeof(Snapshot::TDeviceConfig);
		}

		inline void MigrateUp(void *pCurr, void *pPrev) {
			auto pCurrConfig = (Snapshot::PTDeviceConfig)pCurr;
			auto pPrevConfig = (PTDeviceConfig_prev)pPrev;

			strncpy(pCurrConfig->Name, pPrevConfig->Name, sizeof(pCurrConfig->Name) - 1);
			memcpy(&pCurrConfig->RestartCount, &pPrevConfig->RestartCount, sizeof(pCurrConfig->RestartCount));
			memcpy(&pCurrConfig->CommunicationTimeout, &pPrevConfig->CommunicationTimeout, sizeof(pCurrConfig->CommunicationTimeout));
			memcpy(&pCurrConfig->EnableIR, &pPrevConfig->EnableIR, sizeof(pCurrConfig->EnableIR));
			pCurrConfig->DisableKeypad = false;
			fprintf(stdout, "DeviceConfig. MigrateUp to %04X\n", DataMigrate.Version);
		}

		inline void MigrateDown(void *pCurr, void *pPrev) {
			auto pCurrConfig = (Snapshot::PTDeviceConfig)pCurr;
			auto pPrevConfig = (PTDeviceConfig_prev)pPrev;

			strncpy(pPrevConfig->Name, pCurrConfig->Name, sizeof(pPrevConfig->Name) - 1);
			memcpy(&pPrevConfig->RestartCount, &pCurrConfig->RestartCount, sizeof(pPrevConfig->RestartCount));
			memcpy(&pPrevConfig->CommunicationTimeout, &pCurrConfig->CommunicationTimeout, sizeof(pPrevConfig->CommunicationTimeout));
			memcpy(&pPrevConfig->EnableIR, &pCurrConfig->EnableIR, sizeof(pPrevConfig->EnableIR));
			fprintf(stdout, "DeviceConfig. MigrateDown from %04X\n", DataMigrate.Version);
		}
	} // namespace V00000005
} // namespace MigrateDeviceConfig
