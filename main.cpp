#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "DeviceConfig.h"

CurrentVersion::TDeviceConfig DeviceConfig;

int main(const int argc, const char *argv[]) {
	if (ReadDeviceConfig(&DeviceConfig)) {
		fprintf(stdout, "DeviceConfig read successfully, Name:%s, RestartCount:%u, CommunicationTimeout:%u\n", DeviceConfig.Name, DeviceConfig.RestartCount,
				DeviceConfig.CommunicationTimeout);
	}

	DeviceConfig.RestartCount++;
	StoreDeviceConfig(&DeviceConfig);
	return EXIT_SUCCESS;
}