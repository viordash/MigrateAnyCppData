#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MigrateDeviceConfig.h"

#define DEVICE_CONFIG_VERSION ((uint32_t)0x00000005)

bool ReadDeviceConfig(CurrentVersion::PTDeviceConfig pConfig);
void StoreDeviceConfig(CurrentVersion::PTDeviceConfig pConfig);
