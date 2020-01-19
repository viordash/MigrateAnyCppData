#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "DeviceConfig.h"

TDeviceConfig DeviceConfig;

int main(const int argc, const char *argv[]) {
	if (ReadDeviceConfig(&DeviceConfig)) {
		fprintf(stdout, "DeviceConfig read successfully, Name:%s, RestartCount:%d, CommunicationTimeout:%d\n", DeviceConfig.Name, DeviceConfig.RestartCount,
				DeviceConfig.CommunicationTimeout);
	}

	DeviceConfig.RestartCount++;
	StoreDeviceConfig(&DeviceConfig);
	return EXIT_SUCCESS;
}