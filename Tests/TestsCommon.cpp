#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "TestsCommon.h"

const TDataMigrate TestMigrate_Data[5] = {
	{INITIAL_VERSION, 0, 0, 0},
	TestData::Initial::DataMigrate,	  //
	TestData::V00000002::DataMigrate, //
	TestData::V00000003::DataMigrate, //
	TestData::V00000004::DataMigrate, //
};

const TDataMigrateItems DataMigrateItems = {
	TestMigrate_Data,									   //
	sizeof(TestMigrate_Data) / sizeof(TestMigrate_Data[0]) //
};