#pragma once

#include "MigrateAnyData/MigrateAnyData.h"

namespace MigrateDeviceConfig {
	namespace Initial {
		inline int GetSizeOfCurrentData();
		inline void MigrateUp(void *pCurr, void *pPrev);
		inline void MigrateDown(void *pCurr, void *pPrev);

		const TDataMigrate DataMigrate = {0x00000001, MigrateUp, MigrateDown, GetSizeOfCurrentData};

		namespace Snapshot {
			typedef struct {
				char Name[8];
			} TDeviceConfig, *PTDeviceConfig;
		} // namespace Snapshot

		inline int GetSizeOfCurrentData() {
			return sizeof(Snapshot::TDeviceConfig);
		}

		inline void MigrateUp(void *pCurr, void *pPrev) {
			auto pConfig = (Snapshot::PTDeviceConfig)pCurr;
			memset(pConfig, 0, sizeof(*pConfig));
			strcpy(pConfig->Name, "Awesome");
			fprintf(stdout, "DeviceConfig. Initial migrate to %04X\n", DataMigrate.Version);
		}

		inline void MigrateDown(void *pCurr, void *pPrev) {
		}

	} // namespace Initial
} // namespace MigrateDeviceConfig
