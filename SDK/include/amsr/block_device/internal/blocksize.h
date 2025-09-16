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
/*!        \file  blocksize.h
 *        \brief  Determining block sizes of block devices.
 *        \unit   osabstraction::block_device
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_BLOCKDEVICE_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_BLOCK_DEVICE_INTERNAL_BLOCKSIZE_H_
#define LIB_BLOCKDEVICE_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_BLOCK_DEVICE_INTERNAL_BLOCKSIZE_H_

#include "amsr/core/result.h"
#include "osabstraction/io/native_types.h"

namespace amsr {
namespace block_device {
namespace internal {

/*!
 * \brief Size information about a block device.
 */
struct BlockDeviceSizes {
  /*!
   * \brief The logical block size of a block device.
   */
  std::uint64_t logical_block_size;

  /*!
   * \brief The physical block size of a block device.
   */
  std::uint64_t physical_block_size;

  /*!
   * \brief The total size of a block device (in bytes).
   */
  std::uint64_t device_size;
};

/*!
 * \brief Determines the sizes of a block device.
 *
 * \param[in]       handle        An open handle to a block device.
 *
 * \return          A struct containing information about the size of the block device.
 * \error           osabstraction::OsabErrc::kSystemEnvironmentError  Cannot get size information about block device.
 * \error           osabstraction::OsabErrc::kUnexpected              An unexpected error occurred.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \trace DSGN-Osab-BlockDevice-Sizes
 */
auto GetBlockDeviceSizes(osabstraction::io::NativeHandle handle) -> amsr::core::Result<BlockDeviceSizes>;

}  // namespace internal
}  // namespace block_device
}  // namespace amsr

#endif  // LIB_BLOCKDEVICE_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_BLOCK_DEVICE_INTERNAL_BLOCKSIZE_H_
