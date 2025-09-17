#[================================================================================================[
Copyright (c) 2024 by null. All rights reserved.

This software is copyright protected and proprietary to null.
null grants to you only those rights as set out in the license conditions.
All other rights remain with null.

File description:
-----------------
This is a CMake build configuration file which acts as the starting point for an Integration Build.
An integration build combines builds of application subsystems which in turn can consist of a set
of Executables.

#]================================================================================================]


# Setup the cmake version to use. Shall be > 3.22
cmake_minimum_required(VERSION 3.22)

# Include the file which defines critical paths used by this cmake build structure.
include(LocalPaths.cmake)

# CMake needs to know the CMake project config files of the platform sdk
list(APPEND CMAKE_PREFIX_PATH "${SDK_DIR}/lib/cmake")

# Top level project info
project("SpeedLimitApplication"
        VERSION "1.0.0"
        DESCRIPTION "description"
        HOMEPAGE_URL "vector.com")


# Project specific language settings
set(CMAKE_CXX_STANDARD "14" CACHE STRING "")
set(CMAKE_CXX_EXTENSIONS OFF CACHE BOOL "")
set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE BOOL "")


# Include the Executables which are part of the Application Project.
add_subdirectory(src/SpeedMonitorExecutable SpeedMonitorExecutable)

add_subdirectory(src/WarningDisplayExecutable WarningDisplayExecutable)


