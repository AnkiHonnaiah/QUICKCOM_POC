/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file
 *        \brief  Interface for calls to Posix functions.
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_POSIX_ABSTRACTION_INTERFACE_H_
#define LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_POSIX_ABSTRACTION_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/core/string.h"
#include "amsr/iostream/filestream/file_permission.h"
#include "amsr/iostream/filestream/open_mode.h"
#include "amsr/iostream/filestream/std_io_file_descriptor.h"
#include "amsr/iostream/types.h"
#include "vac/language/byte.h"

namespace amsr {
namespace stream {
namespace filestream {

/*!
 * \brief Interface for calls to Posix functions.
 * \details -
 * \vprivate Component Private
 */
class PosixAbstractionInterface {
 public:
  /*!
   * \brief Constructs instance of class.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  PosixAbstractionInterface() noexcept = default;

 protected:
  // file access should not be shared
  PosixAbstractionInterface(PosixAbstractionInterface const&) = delete;

  // file access should not be shared
  PosixAbstractionInterface& operator=(PosixAbstractionInterface const&) & = delete;

  /*!
   * \brief Move constructs instance of class.
   * \param[in] abstraction Instance of PosixAbstractionInterface
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  PosixAbstractionInterface(PosixAbstractionInterface&& abstraction) noexcept = default;

  /*!
   * \brief Move assigns instance of class.
   * \param[in] abstraction Instance of PosixAbstractionInterface
   * \return Moved file handle
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  PosixAbstractionInterface& operator=(PosixAbstractionInterface&& abstraction) & noexcept = default;

 public:
  /*!
   * \brief Destructs instance.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  virtual ~PosixAbstractionInterface() noexcept = default;

  // VECTOR NC AutosarC++17_10-A3.9.1: MD_IoStream_A3.9.1_posix_api
  /*!
   * \brief Opens the file given in file_name with the given flags and permission.
   * \param[in] file_name File name of file to open.
   * \param[in] flags Combination of file status flags and file access modes.
   * \param[in] permission Only used when the file is created. Defines the permission bits. See
   * https://www.gnu.org/software/libc/manual/html_node/Permission-Bits.html for explanation.
   * \return Whether operation succeeded.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  virtual bool Open(amsr::core::String const& file_name, int flags, FilePermission permission) noexcept = 0;

  /*!
   * \brief Opens a file stream to STDOUT / STDERR.
   * \param[in] std_file_descriptor File handle to open (STDOUT_FILENO / STDERR_FILENO).
   * \return Whether operation succeeded.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  virtual bool OpenStdIo(StdIoFileDescriptor std_file_descriptor) noexcept = 0;

  /*!
   * \brief Closes a file.
   * \details Calling close on an already closed file, does not have any effect (no error).
   * \return Whether operation succeeded.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  virtual bool Close() noexcept = 0;

  /*!
   * \brief Reads from file
   * \param[out] buffer Memory to save the read bytes to.
   * \return The number of characters read.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  virtual std::int64_t Read(amsr::core::Span<char> buffer) const noexcept = 0;

  /*!
   * \brief Reads from file.
   * \param[out] buffer Memory to save the read bytes to.
   * \return The number of characters read.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  virtual std::int64_t Read(amsr::core::Span<vac::language::byte> buffer) const noexcept = 0;

  /*!
   * \brief Writes to file.
   * \param[in] data Span to write.
   * \return The number of characters written.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  virtual std::int64_t Write(amsr::core::Span<char const> data) const noexcept = 0;

  /*!
   * \brief Writes to file.
   * \param[in] data Span to write.
   * \return The number of characters written.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  virtual std::int64_t Write(amsr::core::Span<vac::language::byte const> data) const noexcept = 0;

  /*!
   * \brief Synchronizes changes to a file.
   * \return Whether operation succeeded.
   * \context ANY
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  virtual bool Fdatasync() const noexcept = 0;

  /*!
   * \brief Move the read/write file offset.
   * \param[in] offset Offset to change the file position with.
   * \param[in] whence Specifies how the offset is interpreted:
   * - kBeg: offset is a count of characters from the beginning of the file.
   * - kEnd: offset is a count of characters from the end of the file.
   * - kCur: offset is a count of characters from the current file position.
   * \return The updated file position in the file, measured in bytes from the beginning of the file.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  virtual std::int64_t Lseek(std::int64_t offset, SeekDirection whence) const noexcept = 0;

  /*!
   * \brief Get Posix error based on \c errno.
   * \return Value of \c errno.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  virtual std::int64_t GetError() const noexcept = 0;
};

}  // namespace filestream
}  // namespace stream
}  // namespace amsr

#endif  // LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_POSIX_ABSTRACTION_INTERFACE_H_
