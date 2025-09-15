#[=======================================================================[
Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.

This software is copyright protected and proprietary to Vector Informatik GmbH.
Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
All other rights remain with Vector Informatik GmbH.

File description:
-----------------
This is a CMake toolchain file.
#]=======================================================================]

#[=======================================================================[.rst:
gcc11_linux_x86_64
------------------

Toolchain file for GCC/G++ compiler version 11 for Linux x86_64 architectures.
#]=======================================================================]

set(CMAKE_SYSTEM_VERSION   1)
set(CMAKE_SYSTEM_NAME      Linux)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_C_COMPILER   gcc-11)
set(CMAKE_CXX_COMPILER g++-11)

set(CMAKE_AR     ar     CACHE FILEPATH "archiver")
set(CMAKE_RANLIB ranlib CACHE FILEPATH "ranlib")

set(CMAKE_NM      nm      CACHE FILEPATH "nm")
set(CMAKE_OBJCOPY objcopy CACHE FILEPATH "objcopy")
set(CMAKE_OBJDUMP objdump CACHE FILEPATH "objdump")

set(CMAKE_C_COMPILER_AR       gcc-ar-11     CACHE FILEPATH "archiver")
set(CMAKE_C_COMPILER_RANLIB   gcc-ranlib-11 CACHE FILEPATH "ranlib")
set(CMAKE_CXX_COMPILER_AR     gcc-ar-11     CACHE FILEPATH "archiver")
set(CMAKE_CXX_COMPILER_RANLIB gcc-ranlib-11 CACHE FILEPATH "ranlib")
