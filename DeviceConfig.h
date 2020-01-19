#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEVICE_CONFIG_VERSION ((uint32_t)0x00000005)

typedef struct {
	char Name[16];
	uint16_t RestartCount;
	uint32_t CommunicationTimeout;
	bool EnableIR;
	bool DisableKeypad;
} TDeviceConfig, *PTDeviceConfig;

bool ReadDeviceConfig(PTDeviceConfig pConfig);
void StoreDeviceConfig(PTDeviceConfig pConfig);
