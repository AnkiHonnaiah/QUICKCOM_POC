#[================================================================================================[
Copyright (c) 2024 by null. All rights reserved.

This software is copyright protected and proprietary to null.
null grants to you only those rights as set out in the license conditions.
All other rights remain with null.

File description:
-----------------
This is a CMake setup file for critical build variables used during an Integration Build.

#]================================================================================================]

# Setup the cmake variable SDK_DIR.
# This variable shall be set to the root of the Platform SDK which is created
# when building the Microsar Adaptive Platform
set (SDK_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../../SDK")
message(STATUS ${SDK_DIR})

# Setup the cmake variable MODEL_GENERATION_RESULT_DIR.
# This variable shall be set to the root of the source tree created when generating
# the information using Microsar Adaptive generators.
set (MODEL_GENERATION_RESULT_DIR "${CMAKE_CURRENT_LIST_DIR}/../../tmp/src-gen")

# Setup the cmake variable CMAKE_TOOLCHAIN_FILE.
# This variable shall be set to the file path of the toolchain definition cmake file.
set (CMAKE_TOOLCHAIN_FILE "${SDK_DIR}/CMake/Toolchain/gcc11_linux_x86_64.cmake")

# Setup the prefix for the installation commands.
# This varaible shall be set to the root of the deployment package.
set (CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_LIST_DIR}/../../deploy" CACHE PATH "..." FORCE)

# Verifiy the paths which has been setup above.

# The path to the pre-built software development package based on the BSW Package
if(NOT EXISTS "${SDK_DIR}")
    message(FATAL_ERROR "Platform SDK is missing, please make sure that a correct value is set for the SDK_DIR variable.")
endif()
message(STATUS "APPLICATION LOG: Configured SDK_DIR = ${SDK_DIR}")

if(NOT EXISTS "${MODEL_GENERATION_RESULT_DIR}")
    message(FATAL_ERROR "Model Generation directory is missing, please make sure that a correct value is set for the MODEL_GENERATION_RESULT_DIR variable.")
endif()
message(STATUS "APPLICATION LOG: Configured MODEL_GENERATION_RESULT_DIR = ${MODEL_GENERATION_RESULT_DIR}")

if(NOT EXISTS "${CMAKE_TOOLCHAIN_FILE}")
    message(FATAL_ERROR "The TOOLCHAIN file doesn't exist, please make sure that a correct value is set for the CMAKE_TOOLCHAIN_FILE variable.")
endif()
message(STATUS "APPLICATION LOG: The following TOOLCHAIN file is configured for this project: ${CMAKE_TOOLCHAIN_FILE}")

message(STATUS "APPLICATION LOG: The following INSTALL PREFIX is configured for this project: ${CMAKE_INSTALL_PREFIX}")