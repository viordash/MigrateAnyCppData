#include "MigrateAnyData.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TMigrateResult MigrateData::Run(uint32_t storedVersion, uint32_t currentVersion, const TDataMigrateItems *pMigrateData, void *pStoredData, uint32_t storedSize,
								void *parent, TOnMigrateDataItem onMigrateDataItem) {
	if (storedVersion == currentVersion) {
		return MigrateRes_Skipped;
	}
	if (pStoredData == NULL && storedVersion > INITIAL_VERSION) {
		return MigrateRes_Error;
	}

	PTDataMigrate pStored = Find(storedVersion, pMigrateData);
	if (pStored == NULL) {
		pStored = Find(INITIAL_VERSION, pMigrateData);
		if (pStored == NULL) {
			fprintf(stderr, "MigrateData. no migration found for stored");
			return MigrateRes_Error;
		}
		fprintf(stderr, "MigrateData. Restart from initial");
	}

	PTDataMigrate pCurrent = Find(currentVersion, pMigrateData);
	if (pCurrent == NULL) {
		fprintf(stderr, "MigrateData. no migration found for current");
		return MigrateRes_Error;
	}
	size_t recordsCount;
	size_t currentRecordSize = pCurrent->GetSizeOfCurrentData();
	size_t storedRecordSize;

	if (pStored->Version > INITIAL_VERSION && storedSize != 0) {
		storedRecordSize = pStored->GetSizeOfCurrentData();
		if (storedRecordSize == 0) {
			fprintf(stderr, "MigrateData. stored record size is zero");
			return MigrateRes_Error;
		}
		if (storedSize % storedRecordSize != 0) {
			fprintf(stderr, "MigrateData. stored data does not match the migration");
			return MigrateRes_Error;
		}
		recordsCount = storedSize / storedRecordSize;
	} else {
		recordsCount = 1;
		storedRecordSize = 0;
	}

	size_t totalSize = recordsCount * currentRecordSize;
	if (currentRecordSize != 0 && totalSize / currentRecordSize != recordsCount) {
		fprintf(stderr, "MigrateData. integer overflow in buffer size calculation");
		return MigrateRes_Error;
	}

	uint8_t *currentData = new uint8_t[totalSize];

	bool success = false;
	uint8_t *pStoredObj = (uint8_t *)pStoredData;
	uint8_t *pCurrentObj = currentData;
	uint8_t *bufferEnd = currentData + totalSize;
	while (pCurrentObj < bufferEnd) {
		if (MigrateUp(pStored, pStoredObj, pCurrent, pCurrentObj) || MigrateDown(pStored, pStoredObj, pCurrent, pCurrentObj)) {
			if (!onMigrateDataItem(parent, pCurrentObj)) {
				success = false;
				break;
			}
			success = true;
		} else {
			fprintf(stderr, "MigrateData. undefined behaviour");
			success = false;
			break;
		}
		pStoredObj += storedRecordSize;
		pCurrentObj += currentRecordSize;
	}
	delete[] currentData;
	return success ? MigrateRes_Migrate : MigrateRes_Error;
}

bool MigrateData::MigrateUp(PTDataMigrate pStored, void *pStoredData, PTDataMigrate pCurrent, void *pCurrentData) {
	uint8_t *tempNextAfterStored = NULL;
	while (pCurrent > pStored) {
		PTDataMigrate pNextVersionAfterStored = pStored + 1;
		uint8_t *temp = tempNextAfterStored;
		tempNextAfterStored = new uint8_t[pNextVersionAfterStored->GetSizeOfCurrentData()];

		pNextVersionAfterStored->MigrateUp(tempNextAfterStored, pStoredData);
		delete[] temp;

		pStoredData = tempNextAfterStored;
		pStored++;
	}
	if (tempNextAfterStored != NULL) {
		memcpy(pCurrentData, tempNextAfterStored, pStored->GetSizeOfCurrentData());
		delete[] tempNextAfterStored;
		return true;
	}
	return false;
}

bool MigrateData::MigrateDown(PTDataMigrate pStored, void *pStoredData, PTDataMigrate pCurrent, void *pCurrentData) {
	uint8_t *tempPrevBeforeStored = NULL;
	while (pCurrent < pStored) {
		PTDataMigrate pPrevVersionBeforeStored = pStored - 1;
		uint8_t *temp = tempPrevBeforeStored;
		tempPrevBeforeStored = new uint8_t[pPrevVersionBeforeStored->GetSizeOfCurrentData()];

		pStored->MigrateDown(pStoredData, tempPrevBeforeStored);
		delete[] temp;

		pStoredData = tempPrevBeforeStored;
		pStored--;
	}
	if (tempPrevBeforeStored != NULL) {
		memcpy(pCurrentData, tempPrevBeforeStored, pStored->GetSizeOfCurrentData());
		delete[] tempPrevBeforeStored;
		return true;
	}
	return false;
}

PTDataMigrate MigrateData::Find(uint32_t version, const TDataMigrateItems *pMigrateData) {
	for (int i = 0; i < pMigrateData->Count; i++) {
		PTDataMigrate pDataMigrate = (PTDataMigrate)&pMigrateData->Items[i];
		if (pDataMigrate->Version == version) {
			return pDataMigrate;
		}
	}
	return NULL;
}
