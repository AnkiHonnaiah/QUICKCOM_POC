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
 *        \brief  Implements calls to Posix functions.
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_POSIX_ABSTRACTION_H_
#define LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_POSIX_ABSTRACTION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/iostream/filestream/fct_pointers.h"
#include "amsr/iostream/filestream/file_permission.h"
#include "amsr/iostream/filestream/open_mode.h"
#include "amsr/iostream/filestream/posix_abstraction_interface.h"
#include "amsr/iostream/filestream/std_io_file_descriptor.h"
#include "amsr/iostream/types.h"
#include "vac/language/byte.h"

namespace amsr {
namespace stream {
namespace filestream {

/*!
 * References a function to open a file.
 * \vprivate Component Private
 */
extern OpenFctPtr const open_file_fct;

/*!
 * References a function to close a file.
 * \vprivate Component Private
 */
extern CloseFctPtr const close_file_fct;

/*!
 * References a function to manipulate a file descriptor.
 * \vprivate Component Private
 */
extern FcntlFctPtr const file_control_fct;

/*!
 * References a function to read from a file.
 * \vprivate Component Private
 */
extern ReadFctPtr const file_read_fct;

/*!
 * References a function to write to a file.
 * \vprivate Component Private
 */
extern WriteFctPtr const file_write_fct;

/*!
 * References a function to synchronize a file's in-core state with the storage device.
 * \vprivate Component Private
 */
extern FdatasyncFctPtr const file_datasync_fct;

/*!
 * References a function to reposition a stream.
 * \vprivate Component Private
 */
extern LseekFctPtr const file_seek_fct;

/*!
 * \brief Implements calls to Posix functions.
 * \details -
 * \vprivate Component Private
 */
class PosixAbstraction : public PosixAbstractionInterface {
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
  PosixAbstraction() noexcept = default;

 protected:
  // file access should not be shared
  PosixAbstraction(PosixAbstraction const&) = delete;

  // file access should not be shared
  PosixAbstraction& operator=(PosixAbstraction const&) & = delete;

  /*!
   * \brief Move constructs instance of class.
   * \param[in] abstraction Instance of PosixAbstraction
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  PosixAbstraction(PosixAbstraction&& abstraction) noexcept = default;

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
  PosixAbstraction& operator=(PosixAbstraction&& abstraction) & noexcept = default;

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
  ~PosixAbstraction() noexcept override;

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
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  bool Open(amsr::core::String const& file_name, int flags, FilePermission permission) noexcept override;

  /*!
   * \brief Opens a file stream to STDOUT / STDERR.
   * \param[in] std_file_descriptor File handle to open (STDOUT_FILENO / STDERR_FILENO).
   * \return Whether operation succeeded.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  bool OpenStdIo(StdIoFileDescriptor std_file_descriptor) noexcept override;

  /*!
   * \brief Closes a file.
   * \details Calling close on an already closed file, does not have any effect (no error).
   * \return Whether operation succeeded.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  bool Close() noexcept override;

  /*!
   * \brief Reads from file.
   * \param[out] buffer Memory to save the read bytes to.
   * \return The number of characters read.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  std::int64_t Read(amsr::core::Span<char> buffer) const noexcept override;

  /*!
   * \brief Reads from file.
   * \param[out] buffer Memory to save the read bytes to.
   * \return The number of characters read.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  std::int64_t Read(amsr::core::Span<vac::language::byte> buffer) const noexcept override;

  /*!
   * \brief Writes to file.
   * \param[in] data Span to write.
   * \return The number of characters written.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  std::int64_t Write(amsr::core::Span<char const> data) const noexcept override;

  /*!
   * \brief Writes to file.
   * \param[in] data Span to write.
   * \return The number of characters written.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  std::int64_t Write(amsr::core::Span<vac::language::byte const> data) const noexcept override;

  /*!
   * \brief Synchronize the data of a file (realtime).
   * \return Whether operation succeeded.
   * \context ANY
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  bool Fdatasync() const noexcept override;

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
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  std::int64_t Lseek(std::int64_t offset, SeekDirection whence) const noexcept override;

  /*!
   * \brief Get Posix error based on \c errno.
   * \return Value of \c errno.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  std::int64_t GetError() const noexcept override;

 private:
  /*!
   * \brief Clears the Posix error based on \c errno.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  static void ClearError() noexcept;

  /*! Friend test declaration. */
  FRIEND_TEST(UT__PosixAbstraction__Other, Close__Error);
  /*! Friend test declaration. */
  FRIEND_TEST(UT__PosixAbstraction__Open, MoveConstructor);
  /*! Friend test declaration. */
  FRIEND_TEST(UT__PosixAbstraction__Open, MoveAssignment);

  /*! Descriptor of the file to handle. */
  std::int32_t file_descriptor_{-1};
};

}  // namespace filestream
}  // namespace stream
}  // namespace amsr

#endif  // LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_POSIX_ABSTRACTION_H_
