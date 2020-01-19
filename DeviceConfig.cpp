
#include "DeviceConfig.h"
#include "MigrateAnyData/MigrateAnyData.h"
#include "MigrateDeviceConfig.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

typedef uint32_t TVersion;
const char *DeviceConfigFileName = "DeviceConfig.dat";

typedef struct {
	TVersion Version;
	TDeviceConfig Config;
} TStoredDeviceConfig;

static TVersion GetStoredConfigVersion();
static size_t GetStoredConfigSize();
static bool ReadStoredConfig(uint8_t *buffer, size_t bufferSize);

bool ReadDeviceConfig(PTDeviceConfig pConfig) {
	TVersion version = GetStoredConfigVersion();
	size_t storedSize = GetStoredConfigSize();
	uint8_t *storedData = NULL;
	if (storedSize > sizeof(TVersion)) {
		storedSize -= sizeof(TVersion);
		storedData = new uint8_t[storedSize];
		if (storedData == NULL) {
			fprintf(stderr, "ReadDeviceConfig. not enough memory\n");
			version = INITIAL_VERSION;
		}
		if (!ReadStoredConfig(storedData, storedSize)) {
			fprintf(stderr, "ReadDeviceConfig. read error\n");
			version = INITIAL_VERSION;
		}
	}

	TMigrateResult migrateResult
		= MigrateData::Run(version, DEVICE_CONFIG_VERSION, &DeviceConfigMigrations, storedData, storedSize, pConfig, [](void *parent, void *pItem) -> bool {
			  memcpy(parent, pItem, sizeof(*pConfig));
			  return true;
		  });
	if (migrateResult == MigrateRes_Migrate) {
		return true;
	} else if (migrateResult == MigrateRes_Skipped && storedData != NULL) {
		memcpy(pConfig, storedData, sizeof(*pConfig));
	}

	if (version > INITIAL_VERSION) {
		return true;
	}
	return false;
}

void StoreDeviceConfig(PTDeviceConfig pConfig) {
	FILE *fData;
	fData = fopen(DeviceConfigFileName, "wb");
	if (fData == NULL) {
		return;
	}
	TVersion version = DEVICE_CONFIG_VERSION;
	fwrite(&version, sizeof(TVersion), 1, fData);
	fwrite(pConfig, sizeof(*pConfig), 1, fData);
	fclose(fData);
}

static TVersion GetStoredConfigVersion() {
	FILE *fData;
	TVersion version;
	fData = fopen(DeviceConfigFileName, "rb");
	if (fData == NULL) {
		return INITIAL_VERSION;
	}
	if (fread(&version, sizeof(TVersion), 1, fData) != 1) {
		version = INITIAL_VERSION;
	}
	fclose(fData);
	return version;
}

static size_t GetStoredConfigSize() {
	FILE *fData;
	struct stat buff;
	if ((fData = fopen(DeviceConfigFileName, "rb")) == NULL) {
		return 0;
	}
	fstat(fileno(fData), &buff);
	fclose(fData);
	return buff.st_size;
}

static bool ReadStoredConfig(uint8_t *buffer, size_t bufferSize) {
	FILE *fData;
	fData = fopen(DeviceConfigFileName, "rb");
	if (fData == NULL) {
		return false;
	}
	bool res = true;
	if (res && fseek(fData, sizeof(TVersion), SEEK_SET) != EXIT_SUCCESS) {
		res = false;
	}
	if (res && fread(buffer, bufferSize, 1, fData) != 1) {
		res = false;
	}

	fclose(fData);
	return res;
}
