#include "../MigrateAnyData/MigrateAnyData.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char Name[16];
} TSettingsLarge, *PTSettingsLarge;

typedef struct {
	char Name[8];
} TSettingsSmall, *PTSettingsSmall;

int main(const int argc, const char *argv[]) {
	// Test 1: STRING_FIELD_MIGR_UP - string longer than target buffer
	{
		TSettingsLarge prev;
		strcpy(prev.Name, "VeryLongName123"); // 15 chars, longer than target 8

		TSettingsSmall curr;
		memset(&curr, 0xFF, sizeof(curr)); // Fill with garbage (non-zero)

		PTSettingsSmall pCurrSettings = &curr;
		PTSettingsLarge pPrevSettings = &prev;

		STRING_FIELD_MIGR_UP(Name);

		if (curr.Name[sizeof(curr.Name) - 1] != '\0') {
			fprintf(stderr, "Test 1 failed: string not null-terminated after STRING_FIELD_MIGR_UP\n");
			fprintf(stderr, "Last byte value: 0x%02X\n", (unsigned char)curr.Name[sizeof(curr.Name) - 1]);
			return EXIT_FAILURE;
		}

		if (strlen(curr.Name) >= sizeof(curr.Name)) {
			fprintf(stderr, "Test 1 failed: string length exceeds buffer\n");
			return EXIT_FAILURE;
		}

		printf("Test 1 passed: STRING_FIELD_MIGR_UP correctly null-terminates string\n");
	}

	// Test 2: STRING_FIELD_MIGR_DOWN - copy to larger buffer (should also null-terminate)
	{
		TSettingsSmall curr;
		strcpy(curr.Name, "1234567"); // 7 chars + null

		TSettingsLarge prev;
		memset(&prev, 0xFF, sizeof(prev)); // Fill with garbage

		PTSettingsSmall pCurrSettings = &curr;
		PTSettingsLarge pPrevSettings = &prev;

		STRING_FIELD_MIGR_DOWN(Name);

		if (prev.Name[sizeof(prev.Name) - 1] != '\0') {
			fprintf(stderr, "Test 2 failed: string not null-terminated after STRING_FIELD_MIGR_DOWN\n");
			fprintf(stderr, "Last byte value: 0x%02X\n", (unsigned char)prev.Name[sizeof(prev.Name) - 1]);
			return EXIT_FAILURE;
		}

		printf("Test 2 passed: STRING_FIELD_MIGR_DOWN correctly null-terminates string\n");
	}

	printf("All tests passed\n");
	return EXIT_SUCCESS;
}
