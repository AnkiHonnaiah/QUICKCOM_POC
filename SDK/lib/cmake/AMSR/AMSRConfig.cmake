#[=========================================================================================[
Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.

This software is copyright protected and proprietary to Vector Informatik GmbH.
Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
All other rights remain with Vector Informatik GmbH.
#]=========================================================================================]

#[=========================================================================================[.rst
AMSRConfig
----------

This CMake package script provides the `AMSR` build-targets as imported targets.

Usage
^^^^^

Examples:

When you try to locate the `AMSR` targets, you should specify which components you want to use.
If none is given, no imported target will be created and you won't be able to link to `AMSR` libraries.

.. code-block:: cmake

   # No specific version required, find the components you need
   find_package(AMSR COMPONENTS ...)

.. hint::

   You don't need to deal with 'AMSRs' dependencies when linking your targets against `AMSR`
   libraries, they will all be configured automatically, even if you use `AMSR` static libraries.

.. code-block:: cmake

   # or if the project uses semantic versioning
   find_package(AMSR <MAJOR>.<MINOR> COMPONENTS ...)

.. note::

   You can enforce a specific version if the project uses semantic versioning, either <MAJOR>.<MINOR> or
   only <MAJOR>. If nothing is specified, the version won't be checked (i.e. any version will be accepted).

On macOS by default CMake will search for frameworks. If you want to use static libraries and have installed
both `AMSR` frameworks and `AMSR` static libraries, you must set CMAKE_FIND_FRAMEWORK to
"NEVER" or "LAST". `Please refer to CMake documentation for more details
<https://cmake.org/cmake/help/latest/variable/CMAKE_FIND_FRAMEWORK.html>`_.

Additionally, keep in mind that `AMSR` frameworks are only available as "Release" libraries
unlike `dylibs` which are available for both `Release` and `Debug`.

If `AMSR` is not installed in a standard path, you can use the 'AMSR_DIR' CMake variable to
tell CMake where AMSR's config file is located (PREFIX/lib/cmake/AMSR for a library based
installation, and PREFIX/AMSR.framework/Resources/CMake on macOS for a framework-based installation).

The `AMSR` targets are the same for all build configurations and will automatically provide
correct settings based on your currently active build configuration. The `AMSR` target names
also do not change when using dynamic or static `AMSR` libraries.

.. hint::

   This script defines for each component a CMake variable:
   - <UPPER_COMPONENT_NAME>_FOUND: TRUE if either the target of the component is found
   - AMSR_FOUND: TRUE if all the required components are found

.. note::

   When linking against a `AMSR` target, you do NOT need to specify indirect dependencies!

The following imported targets are part of this CMake install package:

.. code-block:: cmake

   set(importTargetNames
       amsr::applicationbase
       amsr::aracrypto_ara
       amsr::aracrypto_common
       amsr::charconv_common
       amsr::charconv_vector
       amsr::charconv
       amsr::comintegrator
       amsr::comtrace
       amsr::crc
       amsr::crc_internal
       amsr::e2e_proprietary_interface
       amsr::e2e_proprietary_default
       amsr::e2e
       amsr::e2e_internal
       amsr::iointegritystream
       amsr::ipcbinding
       amsr::ipcbinding_xf_common
       amsr::ipcbinding_xf
       amsr::ipcbinding_core
       amsr::ipcbinding_generic
       amsr::ipcmulticast_common
       amsr::ipcmulticast_ipv6_impl
       amsr::ipcmulticast_daemon_impl
       amsr::ipcmulticast
       amsr::ipcservicediscovery
       amsr::ipcservicediscovery_centralized
       amsr::ipcservicediscovery_common
       amsr::ipcservicediscovery_external
       amsr::iostream
       amsr::osa_common
       amsr::osa_filesystem
       amsr::osa_ptp_cfg
       amsr::osa_ptp
       amsr::osa_net
       amsr::osa_syslog
       amsr::osa_blockdevice
       amsr::osa_resourceusage
       amsr::osa_logsource
       amsr::osa_utils
       amsr::vac
       amsr::log_common
       amsr::log_ipc_common
       amsr::log
       amsr::log_ara_logging
       amsr::trace_extension_point
       amsr::loguti
       amsr::msr4base
       amsr::cryptolibes
       amsr::crypto_client
       amsr::crypto_ipc
       amsr::crypto_server
       amsr::crypto_server_common
       amsr::crypto_server_lib_no_idsm
       amsr::crypto_x509provider
       amsr::crypto_x509customextensionparser
       amsr::seccom_tls_provider_common
       amsr::libseccom_external
       amsr::seccom_tls_provider_null
       amsr::seccom_tls
       amsr::socal_external
       amsr::socal
       amsr::com_r18_03
       amsr::com_r20_11
       amsr::com_default
       amsr::socal_generic
       amsr::softwarecryptoprovider_lib
       amsr::softwarecryptoprovider
       amsr::someipbinding
       amsr::someipbinding_core
       amsr::someipbinding_transformation_layer
       amsr::someipbinding_lite
       amsr::someipbinding_generic_layer
       amsr::someipd_null_message_authentication
       amsr::someipd_null_pdu_aggregation
       amsr::someipd_default_statistics_handler
       amsr::someipd_dummy_iam
       amsr::someipd_extension_points_null_tracing
       amsr::someipd_extension_points_null_communication
       amsr::someipd_extension_points_null_extensions_handler
       amsr::someipd_statistics_handler_interface
       amsr::someipd_someip_tp
       amsr::someipd_daemon_core
       amsr::someipd_daemon_lite
       amsr::someipd_extension_points_interfaces
       amsr::someipdaemonclient
       amsr::someipdaemonclient_external
       amsr::someipprotocol_basic_serdes
       amsr::someipprotocol_logging
       amsr::someipprotocol_messages
       amsr::someipprotocol_serdes
       amsr::someipprotocol_tlv
       amsr::someipprotocol
       amsr::vathread
       amsr::vaasn1
       amsr::vajson
       amsr::zerocopybinding
       amsr::zerocopybinding_xf
       amsr::zerocopybinding_core
       amsr::zerocopybinding_common
       amsr::zerocopybinding_generic
       amsr::zerocopycommon_common
       amsr::zerocopycommon_logic
       amsr::zerocopycommon_memcon
       amsr::zerocopycommon
       amsr::zerocopycommon_tcc
       amsr::zerocopycommon_external)

#]=========================================================================================]

####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was Package.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

####################################################################################
list(APPEND CMAKE_MODULE_PATH "${PACKAGE_PREFIX_DIR}/cmake")

# We use if(...IN_LIST...), make sure it is available
if(CMAKE_VERSION VERSION_LESS 3.3)
  set(${CMAKE_FIND_PACKAGE_NAME}_NOT_FOUND_MESSAGE "AMSR requires CMake 3.3 or later")
  set(${CMAKE_FIND_PACKAGE_NAME}_FOUND FALSE)
  return()
endif()

cmake_minimum_required(VERSION 3.3...3.27)

# Is this a Apple specific framework package?
set(${CMAKE_FIND_PACKAGE_NAME}_IS_FRAMEWORK_INSTALL FALSE)

# Set AMSR_FOUND to TRUE by default, may be overwritten below...
set(${CMAKE_FIND_PACKAGE_NAME}_FOUND TRUE)

# If no COMPONENTS are specified then no imported target will be created
if(NOT ${CMAKE_FIND_PACKAGE_NAME}_FIND_COMPONENTS)
  set(${CMAKE_FIND_PACKAGE_NAME}_NOT_FOUND_MESSAGE "AMSR found, but without using: COMPONENTS or REQUIRED")
  set(${CMAKE_FIND_PACKAGE_NAME}_FOUND FALSE)
  return()
endif()

# All exported component targets are contained in the CMake package config files for which they were typed...
set(configPackageFileList AMSRDevelopment.cmake)
foreach(configPackageFile IN LISTS configPackageFileList)
  set(configPackageFilePath "${CMAKE_CURRENT_LIST_DIR}/${configPackageFile}")
  if(EXISTS "${configPackageFilePath}")
    include("${configPackageFilePath}")
  else()
    message(VERBOSE "CMake target import file not present: ${configPackageFilePath}")
  endif()
endforeach()

# Check all required component(s) are available
foreach(componentName IN LISTS ${CMAKE_FIND_PACKAGE_NAME}_FIND_COMPONENTS)
  string(TOUPPER "${componentName}" upperComponentName)
  string(REPLACE "::" "_" upperComponentName "${upperComponentName}")
  if(TARGET "${componentName}")
    set(${upperComponentName}_FOUND TRUE)
    get_target_property(targetType "${componentName}" TYPE)
    if(targetType STREQUAL "STATIC_LIBRARY" AND ${CMAKE_FIND_PACKAGE_NAME}_IS_FRAMEWORK_INSTALL)
      message(WARNING "Static frameworks are not supported by AMSR. Clear AMSR_DIR cache entry"
              " and change CMAKE_FIND_FRAMEWORK before calling find_package(AMSR COMPONENTS ...)")
    endif()
  else()
    set(${CMAKE_FIND_PACKAGE_NAME}_NOT_FOUND_MESSAGE "AMSR missing required component: ${componentName}")
    set(${CMAKE_FIND_PACKAGE_NAME}_FOUND FALSE)
    set(${upperComponentName}_FOUND FALSE)
    return()
  endif()
endforeach()
