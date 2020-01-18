cmake_minimum_required(VERSION 3.0.0)
project(MigrateAny VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 14)

include(CTest)

add_executable(MigrateAny main.cpp 
        MigrateAnyData/MigrateAnyData.cpp)

add_executable(TestMigrateDown
        Tests/MigrateDown.cpp
        Tests/TestsCommon.cpp
        MigrateAnyData/MigrateAnyData.cpp)

add_executable(TestMigrationDown_RandomAccess
        Tests/MigrationDown_RandomAccess.cpp
        Tests/TestsCommon.cpp
        MigrateAnyData/MigrateAnyData.cpp)

add_executable(TestMigrateUp
        Tests/MigrationUp.cpp
        Tests/TestsCommon.cpp
        MigrateAnyData/MigrateAnyData.cpp)

add_executable(TestMigrationUp_RandomAccess
        Tests/MigrationUp_RandomAccess.cpp
        Tests/TestsCommon.cpp
        MigrateAnyData/MigrateAnyData.cpp)

add_executable(TestError_Stored_Version__Do_Initial_And_MigrateUp
        Tests/Error_Stored_Version__Do_Initial_And_MigrateUp.cpp
        Tests/TestsCommon.cpp
        MigrateAnyData/MigrateAnyData.cpp)

add_executable(TestError_Current_Version_Return_Error
        Tests/Error_Current_Version_Return_Error.cpp
        Tests/TestsCommon.cpp
        MigrateAnyData/MigrateAnyData.cpp)

add_executable(TestError_In_Args
        Tests/Error_In_Args.cpp
        Tests/TestsCommon.cpp
        MigrateAnyData/MigrateAnyData.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

enable_testing()

add_test(TestMigrateDown TestMigrateDown)
add_test(TestMigrationDown_RandomAccess TestMigrationDown_RandomAccess)
add_test(TestMigrateUp TestMigrateUp)
add_test(TestMigrationUp_RandomAccess TestMigrationUp_RandomAccess)
add_test(TestError_Stored_Version__Do_Initial_And_MigrateUp TestError_Stored_Version__Do_Initial_And_MigrateUp)
add_test(TestError_Current_Version_Return_Error TestError_Current_Version_Return_Error)
add_test(TestError_In_Args TestError_In_Args)