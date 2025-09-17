#[=======================================================================[
Copyright (c) by Vector Informatik GmbH. All rights reserved.

This software is copyright protected and proprietary to Vector Informatik GmbH.
Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
All other rights remain with Vector Informatik GmbH.

Attention:
----------
The ${target} property settings in this file only take effect if this file is
set as an expected OUTPUT of dvacgf_file_gen().

File description:
-----------------
This is a generated CMake build-configuration file, modifications get lost.
#]=======================================================================]

set(amsrAraCoreApi r20_11 PARENT_SCOPE)
set(amsrAraComApi r20_11 PARENT_SCOPE)

# Add generated source file(s) to target property SOURCES
target_sources(${target} PRIVATE
  "${CMAKE_CURRENT_LIST_DIR}/src/amsr/socal/internal/runtime_gen.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/src/services/speedlimitservice/speedlimitservice.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/src/services/speedlimitservice/speedlimitservice_skeleton.cpp"
)

# Mark source file(s) as generated
set_source_files_properties(
  "${CMAKE_CURRENT_LIST_DIR}/src/amsr/socal/internal/runtime_gen.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/src/services/speedlimitservice/speedlimitservice.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/src/services/speedlimitservice/speedlimitservice_skeleton.cpp"
  PROPERTIES GENERATED 1
)

# Add generated header file(s) to target property SOURCES
target_sources(${target} PRIVATE
  "${CMAKE_CURRENT_LIST_DIR}/include/amsr/socal/definitions.h"
  "${CMAKE_CURRENT_LIST_DIR}/include/amsr/socal/internal/lifecycle_manager.h"
  "${CMAKE_CURRENT_LIST_DIR}/include/services/speedlimitservice/SpeedLimitService.h"
  "${CMAKE_CURRENT_LIST_DIR}/include/services/speedlimitservice/SpeedLimitService_datatypes.h"
  "${CMAKE_CURRENT_LIST_DIR}/include/services/speedlimitservice/SpeedLimitService_skeleton_backend_interface.h"
  "${CMAKE_CURRENT_LIST_DIR}/include/services/speedlimitservice/SpeedLimitService_types.h"
  "${CMAKE_CURRENT_LIST_DIR}/include/services/speedlimitservice/speedlimitservice_common.h"
  "${CMAKE_CURRENT_LIST_DIR}/include/services/speedlimitservice/speedlimitservice_skeleton.h"
)

# Mark header file(s) as generated
set_source_files_properties(
  "${CMAKE_CURRENT_LIST_DIR}/include/amsr/socal/definitions.h"
  "${CMAKE_CURRENT_LIST_DIR}/include/amsr/socal/internal/lifecycle_manager.h"
  "${CMAKE_CURRENT_LIST_DIR}/include/services/speedlimitservice/SpeedLimitService.h"
  "${CMAKE_CURRENT_LIST_DIR}/include/services/speedlimitservice/SpeedLimitService_datatypes.h"
  "${CMAKE_CURRENT_LIST_DIR}/include/services/speedlimitservice/SpeedLimitService_skeleton_backend_interface.h"
  "${CMAKE_CURRENT_LIST_DIR}/include/services/speedlimitservice/SpeedLimitService_types.h"
  "${CMAKE_CURRENT_LIST_DIR}/include/services/speedlimitservice/speedlimitservice_common.h"
  "${CMAKE_CURRENT_LIST_DIR}/include/services/speedlimitservice/speedlimitservice_skeleton.h"
  PROPERTIES GENERATED 1
)

# Add all generated files that may be installed to target property SOURCES
target_sources(${target} PRIVATE
  "${CMAKE_CURRENT_LIST_DIR}/etc/socal_config.json"
  "${CMAKE_CURRENT_LIST_DIR}/etc/socal_config.json.validation"
)

# Add all generated files that may be installed to target property INTERFACE_SOURCES
target_sources(${target} PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/etc/socal_config.json>"
  "$<INSTALL_INTERFACE:etc/socal_config.json>"
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/etc/socal_config.json.validation>"
  "$<INSTALL_INTERFACE:etc/socal_config.json.validation>"
)

# Mark installable file(s) as generated
set_source_files_properties(
  "${CMAKE_CURRENT_LIST_DIR}/etc/socal_config.json"
  "${CMAKE_CURRENT_LIST_DIR}/etc/socal_config.json.validation"
  PROPERTIES GENERATED 1
)

# Add include directory of generated header file(s)
target_include_directories(${target} PRIVATE # No public interface for executables
  "${CMAKE_CURRENT_LIST_DIR}/include"
)

