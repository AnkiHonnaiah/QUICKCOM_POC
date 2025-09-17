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
/*!        \file  block_device.h
 *        \brief  BlockDevice Class
 *        \unit   osabstraction::block_device
 *
 *   \complexity  Unit is QM, no action required.
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_BLOCKDEVICE_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_BLOCK_DEVICE_BLOCK_DEVICE_H_
#define LIB_BLOCKDEVICE_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_BLOCK_DEVICE_BLOCK_DEVICE_H_

#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/filesystem/path.h"
#include "osabstraction/io/file_descriptor.h"

namespace amsr {
namespace block_device {

/*!
 * \brief Access modes for opening block devices.
 * \vprivate Vector product internal API
 */
enum class AccessMode : std::uint8_t {
  /*!
   * \brief Open the device for reading.
   */
  kReadOnly,
  /*!
   * \brief Open the device for writing.
   */
  kWriteOnly,
  /*!
   * \brief Open the device for reading and writing.
   */
  kReadWrite
};

/*!
 * \brief Provides access to block devices.
 * \vprivate Vector product internal API
 */
class BlockDevice {
 public:
  /*!
   * \brief Used to specify flags for opening block devices.
   * \vprivate Vector product internal API
   */
  using OpenFlags = std::uint8_t;
  /*!
   * \brief Avoid caching when accessing the block device.
   * \details When this flag is set, all buffers used in Read() and Write() calls must be aligned to the logical block
   *          size. Calling ::fork() or osabstraction::process::OsProcess::CreateProcess() concurrently to Read() or
              Write() results in undefined behavior if this flag is set.
   *          This flag only makes a best effort to avoid caching but does not guarantee absence of caching.
   * \vprivate Vector product internal API
   */
  static constexpr OpenFlags kAvoidCache{0x1};
  /*!
   * \brief Ensure that all data has been written to the device when Write() returns successfully.
   * \details The behavior is as if Flush() was called after every Write(). On some operating systems, this flag may not
   *          flush hardware caches, so the device may still hold the data in non-persistent storage only after Write()
   *          returns.
   * \vprivate Vector product internal API
   */
  static constexpr OpenFlags kSync{0x2};

  /*!
   * \brief Opens a block device.
   *
   * \param[in]       path          Pathname of the block device.
   * \param[in]       mode          The access mode in which the block device shall be opened.
   * \param[in]       flags         Bitwise or of flags.
   *
   * \error           osabstraction::OsabErrc::kInsufficientPrivileges  Permission denied
   * \error           osabstraction::OsabErrc::kDoesNotExist            Path does not refer to an existing block device
   * \error           osabstraction::OsabErrc::kSystemEnvironmentError  Device is not usable
   * \error           osabstraction::OsabErrc::kResource                Insufficient resources
   * \error           osabstraction::OsabErrc::kUnexpected              Some unexpected error occurred
   *
   * \context         ANY
   *
   * \pre             Block device is not open.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-BlockDevice-Open
   *
   * \vprivate Vector product internal API
   */
  auto Open(amsr::filesystem::Path const& path, AccessMode mode, OpenFlags flags) noexcept -> amsr::core::Result<void>;

  /*!
   * \brief Closes the block device.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-BlockDevice-Close
   *
   * \vprivate Vector product internal API
   */
  void Close() noexcept;

  /*!
   * \brief Checks whether the block device is open.
   *
   * \return          true if the the device is open, false otherwise.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate Vector product internal API
   */
  auto IsOpen() const noexcept -> bool;

  /*!
   * \brief Reads blocks from the block device.
   *
   * \param[in]       block_number  Logical block number of the first block to read. Block numbers start at 0.
   * \param[out]      data          View to a buffer where the read data will be written to. The length of the span
   *                                determines the number of bytes to read and must be a multiple of the logical block
   *                                size. If the kAvoidCache flag was set when the device was opened, the span must also
   *                                be aligned to a multiple of the logical block size.
   *
   * \error           osabstraction::OsabErrc::kSize                    Attempted to read beyond end of device
   * \error           osabstraction::OsabErrc::kSystemEnvironmentError  An IO error occurred
   * \error           osabstraction::OsabErrc::kResource                Insufficient resources
   * \error           osabstraction::OsabErrc::kUnexpected              Some unexpected error occurred
   *
   * \context         ANY
   *
   * \pre             Block device is open.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-BlockDevice-Read
   *
   * \vprivate Vector product internal API
   */
  auto Read(std::uint64_t block_number, amsr::core::Span<std::uint8_t> data) const noexcept -> amsr::core::Result<void>;

  /*!
   * \brief Writes blocks to the block device.
   *
   * \param[in]       block_number  Logical block number of the first block to write. Block numbers start at 0.
   * \param[out]      data          View to the data that will be written to the device. The length of the span must be
   *                                a multiple of the logical block size. If the kAvoidCache flag was set when the
   *                                device was opened, the span must also be aligned to a multiple of the logical block
   *                                size.
   *
   * \error           osabstraction::OsabErrc::kSize                    Attempted to write beyond end of device
   * \error           osabstraction::OsabErrc::kSystemEnvironmentError  An IO error occurred
   * \error           osabstraction::OsabErrc::kResource                Insufficient resources
   * \error           osabstraction::OsabErrc::kUnexpected              Some unexpected error occurred
   *
   * \context         ANY
   *
   * \pre             Block device is open.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-BlockDevice-Write
   *
   * \vprivate Vector product internal API
   */
  auto Write(std::uint64_t block_number, amsr::core::Span<std::uint8_t const> data) const noexcept
      -> amsr::core::Result<void>;

  /*!
   * \brief Flushes cached data to be written to the device.
   *
   * \details When Flush returns successfully, all data has been written to the device. If this function returns an
   *          error, data written to the device may have been lost. On some operating systems, this function may not
   *          flush hardware caches, so the device may still hold the data in non-persistent storage only after Flush()
   *          returns.
   *
   * \error           osabstraction::OsabErrc::kSystemEnvironmentError  An IO error occurred
   * \error           osabstraction::OsabErrc::kUnexpected              Some unexpected error occurred.
   *
   * \context         ANY
   *
   * \pre             Block device is open.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-BlockDevice-Flush
   *
   * \vprivate Vector product internal API
   */
  auto Flush() const noexcept -> amsr::core::Result<void>;

  /*!
   * \brief Gets the size of a logical block in bytes.
   *
   * \details The logical block size is the smallest chunk that can be transferred to and from the device.
   *
   * \return          The logical block size.
   *
   * \context         ANY
   *
   * \pre             Block device is open.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           CREQ-Osab-BlockDevice-LogicalBlockSize
   * \trace           DSGN-Osab-BlockDevice-Sizes
   *
   * \vprivate Vector product internal API
   */
  auto GetLogicalBlockSize() const noexcept -> std::uint64_t;

  /*!
   * \brief Gets the size of a physical block in bytes.
   *
   * \details The physical block size is the smallest chunk that can be efficiently transferred to and from the device.
   *          For optimal performance calls to Write should be made with data being a multiple of the physical block
   *          size. It can be assumed that the physical block size is a multiple of the logical block size.
   *
   * \return          The physical block size.
   *
   * \context         ANY
   *
   * \pre             Block device is open.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           CREQ-Osab-BlockDevice-PhysicalBlockSize
   * \trace           DSGN-Osab-BlockDevice-Sizes
   *
   * \vprivate Vector product internal API
   */
  auto GetPhysicalBlockSize() const noexcept -> std::uint64_t;

  /*!
   * \brief Gets the size of a logical blocks on the device.
   *
   * \return          The number of logical blocks on the device.
   *
   * \context         ANY
   *
   * \pre             Block device is open.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           CREQ-Osab-BlockDevice-Size
   * \trace           DSGN-Osab-BlockDevice-Sizes
   *
   * \vprivate Vector product internal API
   */
  auto GetNumberOfLogicalBlocks() const noexcept -> std::uint64_t;

 private:
  /*!
   * \brief File descriptor to the open block device.
   */
  osabstraction::io::FileDescriptor file_descriptor_{};

  /*!
   * \brief The logical block size of the open block device.
   */
  std::uint64_t logical_block_size_{};

  /*!
   * \brief The physical block size of the open block device.
   */
  std::uint64_t physical_block_size_{};

  /*!
   * \brief The number of logical blocks on the open block device.
   */
  std::uint64_t device_blocks_{};
};

}  // namespace block_device
}  // namespace amsr

#endif  // LIB_BLOCKDEVICE_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_BLOCK_DEVICE_BLOCK_DEVICE_H_
