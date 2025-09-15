/**********************************************************************************************************************
 *  COPYRIGHT
 *
 -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *
 -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *
 -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  openflags.h
 *        \brief  Getting OS-specific open flags.
 *        \unit   osabstraction::block_device
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_BLOCKDEVICE_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_BLOCK_DEVICE_INTERNAL_OPENFLAGS_H_
#define LIB_BLOCKDEVICE_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_BLOCK_DEVICE_INTERNAL_OPENFLAGS_H_

#include "amsr/block_device/block_device.h"

namespace amsr {
namespace block_device {
namespace internal {

// VECTOR Next Construct AutosarC++17_10-A3.9.1: MD_OSA_A3.9.1_PosixApi
/*!
 * \brief Determines operating system specific flags corresponding to the given flags.
 *
 * \param[in]       flags            Flags for opening a block device.
 *
 * \return          Flags to be passed to open.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 */
auto GetOpenFlags(BlockDevice::OpenFlags flags) -> int;

}  // namespace internal
}  // namespace block_device
}  // namespace amsr

#endif  // LIB_BLOCKDEVICE_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_BLOCK_DEVICE_INTERNAL_OPENFLAGS_H_
