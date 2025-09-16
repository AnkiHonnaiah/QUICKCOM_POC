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
 *        \brief  Implements a file-based output stream.
 *      \details
 *
 *********************************************************************************************************************/
#ifndef LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_OUTPUT_FILE_BASED_STREAM_H_
#define LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_OUTPUT_FILE_BASED_STREAM_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <limits>
#include <memory>
#include <string>

#include "amsr/charconv/internal/safe_conversion.h"
#include "amsr/charconv/to_textual.h"
#include "amsr/charconv/to_textual_bool.h"
#include "amsr/core/array.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/core/string_view.h"
#include "amsr/iostream/filestream/file_stream.h"
#include "amsr/iostream/filestream/file_stream_interface.h"
#include "amsr/iostream/filestream/std_io_file_descriptor.h"
#include "amsr/iostream/hex_prefix.h"
#include "amsr/iostream/output_stream.h"
#include "amsr/iostream/stream_error_domain.h"
#include "amsr/iostream/stringstream/output_string_stream.h"
#include "vac/container/c_string_view.h"
#include "vac/language/byte.h"

namespace amsr {
namespace stream {
namespace filestream {

// VECTOR NC Metric-OO.WMC.One: MD_IoStream_Metric-OO.WMC.One_interface
/*!
 * \brief Implements a file-based output stream.
 * \unit amsr::stream::filestream::OutputFileBasedStream
 * \vprivate Product Private
 */
class OutputFileBasedStream : public OutputStream {
 public:
  /*!
   * \brief Constructs instance of class.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  OutputFileBasedStream() noexcept;

  /*!
   * \brief Constructs a file-based output stream using an external buffer.
   * \param[in] buffer Externally owned buffer.
   * \context ANY
   * \pre Aborts if \c buffer is empty (checked in FileStream::FileStream).
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  explicit OutputFileBasedStream(amsr::core::Span<char> buffer) noexcept;

  /*!
   * \brief Constructs instance of class.
   * \param[in] file_stream_ptr File stream.
   * \context ANY
   * \pre Aborts if \c file_stream_ptr is a nullptr.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  explicit OutputFileBasedStream(std::unique_ptr<FileStreamInterface> file_stream_ptr) noexcept;

 protected:
  // OutputFileBasedStream is not copy constructible because FileStreams are not copy constructible.
  OutputFileBasedStream(OutputFileBasedStream const& stream) = delete;

  // OutputFileBasedStream is not copy assignable because FileStreams are not copy assignable.
  OutputFileBasedStream& operator=(OutputFileBasedStream const& stream) & = delete;

  /*!
   * \brief Move constructs instance of class.
   * \param[in] stream Instance of OutputFileBasedStream
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  OutputFileBasedStream(OutputFileBasedStream&& stream) noexcept = default;

  /*!
   * \brief Move assigns instance of class.
   * \param[in] stream Instance of OutputFileBasedStream
   * \return Moved OutputFileBasedStream
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  OutputFileBasedStream& operator=(OutputFileBasedStream&& stream) & noexcept =
      default;  // VCA_IOSTREAM_STD_UNIQUE_PTR_MOVE_ASSIGNMENT_CONTRACT

 public:
  /*!
   * \brief Destructs instance.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  ~OutputFileBasedStream() noexcept override;

  /*!
   * \brief Opens the associated file of the stream with the given flags.
   * \param[in] file_name File name of file to open.
   * \param[in] flags Bitwise-inclusive OR combination of following list (exactly one of the first three values (file
   * access modes) shall be specified): kRead, kWrite, kReadWrite, kAppend, kCreate, kExcl, kSync, kTruncate.
   * \return Result with value void or error code.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno EMFILE,
   * ENFILE or ENOSPC, ENOBUFS.
   * \error StreamErrc::kInvalidArgument One argument or the combination of arguments is invalid. Underlying cause is
   * one of \c errno EEXIST, EISDIR, ELOOP, ENAMETOOLONG, ENOTDIR, EOVERFLOW, EROFS, EINVAL, EFBIG, EACCES or ENXIO.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kNoSuchFileOrDirectory Caused by \c errno ENOENT.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  amsr::core::Result<void> OpenStream(amsr::core::StringView file_name, OpenModeType flags) const noexcept;

  /*!
   * \brief Opens the associated file of the stream with the given flags and modes.
   * \param[in] file_name File name of file to open.
   * \param[in] flags Bitwise-inclusive OR combination of following list (exactly one of the first three values (file
   * access modes) shall be specified): kRead, kWrite, kReadWrite, kAppend, kCreate, kExcl, kSync, kTruncate.
   * \param[in] permission Only used when the file is created. Defines the permission bits. See
   * https://www.gnu.org/software/libc/manual/html_node/Permission-Bits.html for explanation.
   * \return Result with value void or posix error code.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno EMFILE,
   * ENFILE, ENOSPC or ENOBUFS.
   * \error StreamErrc::kInvalidArgument One argument or the combination of arguments is invalid. Underlying cause is
   * one of \c errno EEXIST, EISDIR, ELOOP, ENAMETOOLONG, ENOTDIR, EOVERFLOW, EROFS, EINVAL, EFBIG, EACCES or ENXIO.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kNoSuchFileOrDirectory Caused by \c errno ENOENT.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  amsr::core::Result<void> OpenStream(amsr::core::StringView file_name, OpenModeType flags,
                                      FilePermission permission) const noexcept;

  /*!
   * \brief Opens a file stream to STDOUT / STDERR.
   * \param[in] file_descriptor File handle to open (STDOUT_FILENO / STDERR_FILENO).
   * \return Valid result if operation succeeded, otherwise error code.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is \c errno EMFILE.
   * \error StreamErrc::kInvalidArgument File descriptor limit reached caused by \c errno EINVAL, or invalid
   * return value caused by \c errno EOVERFLOW.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  amsr::core::Result<void> OpenStdStream(StdIoFileDescriptor const file_descriptor) const noexcept;

  /*!
   * \brief Return open status of associated file.
   * \return True if file is open, false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  bool IsOpen() const noexcept override;

  /*!
   * \brief Closes the file associated with the stream.
   * \details Closing an already closed file stream also results in a result with value void.
   * \return Result with value void.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<void> Close() const noexcept;

  /*!
   * \brief Get the current file position.
   * \return The current file position, measured in bytes from the beginning of the file.
   * \context ANY
   * \pre Aborts if stream is closed (checked by FileStream::Tell).
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::uint64_t> Tell() const noexcept override;

  /*!
   * \brief Changes the file position of the file.
   * \param[in] offset Offset to change the file position with.
   * \param[in] whence Specifies how the offset is interpreted:
   * - kBeg: offset is a count of characters from the beginning of the file.
   * - kEnd: offset is a count of characters from the end of the file.
   * - kCur: offset is a count of characters from the current file position.
   * \return The updated file position in the file, measured in bytes from the beginning of the file.
   * \context ANY
   * \pre Aborts if the file stream is closed or not seekable (both checked by FileStream::Seek).
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC.
   * \error StreamErrc::kInvalidArgument Offset exceeding numeric limits caused by \c errno EOVERFLOW or whence
   * argument invalid caused by \c errno EINVAL, or file size exceeds file size limit caused by \c errno
   * EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::uint64_t> Seek(std::int64_t offset, SeekDirection whence) noexcept override;

  /*!
   * \brief Transfers all modified in-core data of the file referred to by the file descriptor to the disk.
   * \return Result with value void.
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (checked by FileStream::Flush).
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS, ENOSPC or ENOMEM.
   * \error StreamErrc::kInvalidArgument File descriptor does not allow this operation caused by \c errno EINVAL,
   * or file position is invalid caused by \c errno EOVERFLOW, or file size exceeds file size limit caused by
   * \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<void> Sync() noexcept override;

  /*!
   * \brief Flushes buffer to file.
   * \details Can be called with a read-only file stream. In this case it returns an empty result.
   * \return Result with value if successful.
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by FileStream::Flush).
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<void> Flush() noexcept override;

  /*!
   * \brief Write a single character to the file stream.
   * \param[in] data The character.
   * \return The number of bytes written.
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by FileStream::EnterWriteMode).
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(char data) noexcept override;

  /*!
   * \brief Write an arithmetic value to the file stream.
   * \param[in] data An arithmetic value of type bool.
   * \return The number of bytes written
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by FileStream::EnterWriteMode).
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(bool const& data) noexcept override;

  /*!
   * \brief Write an arithmetic value to the file stream.
   * \param[in] data An arithmetic value of type std::int8_t.
   * \return The number of bytes written
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by FileStream::EnterWriteMode).
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string buffer too small for data.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(std::int8_t const& data) noexcept override;
  /*!
   * \brief Write an arithmetic value to the file stream.
   * \param[in] data An arithmetic value of type std::int8_t.
   * \param[in] base Base value of the arithmetic value to be written
   * \return The number of bytes written
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by FileStream::EnterWriteMode).
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string buffer too small for data.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(std::int8_t const& data, Base base) noexcept override;

  /*!
   * \brief Write an arithmetic value to the file stream.
   * \param[in] data An arithmetic value of type std::int16_t.
   * \return The number of bytes written
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by FileStream::EnterWriteMode).
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string buffer too small for data.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(std::int16_t const& data) noexcept override;

  /*!
   * \brief Write an arithmetic value to the file stream.
   * \param[in] data An arithmetic value of type std::int16_t.
   * \param[in] base Base value of the arithmetic value to be written.
   * \return The number of bytes written
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by FileStream::EnterWriteMode).
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string buffer too small for data.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(std::int16_t const& data, Base base) noexcept override;

  /*!
   * \brief Write an arithmetic value to the file stream.
   * \param[in] data An arithmetic value of type std::int32_t.
   * \return The number of bytes written
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by FileStream::EnterWriteMode)
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string buffer too small for data.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(std::int32_t const& data) noexcept override;
  /*!
   * \brief Write an arithmetic value to the file stream.
   * \param[in] data An arithmetic value of type std::int32_t.
   * \param[in] base Base value of the arithmetic value to be written
   * \return The number of bytes written
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by FileStream::EnterWriteMode).
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string buffer too small for data.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(std::int32_t const& data, Base base) noexcept override;

  /*!
   * \brief Write an arithmetic value to the file stream.
   * \param[in] data An arithmetic value of type std::int64_t.
   * \return The number of bytes written
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by FileStream::EnterWriteMode).
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string buffer too small for data.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(std::int64_t const& data) noexcept override;

  /*!
   * \brief Write an arithmetic value to the file stream.
   * \param[in] data An arithmetic value of type std::int64_t.
   * \param[in] base Base value of the arithmetic value to be written
   * \return The number of bytes written
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by FileStream::EnterWriteMode).
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string buffer too small for data.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(std::int64_t const& data, Base base) noexcept override;

  /*!
   * \brief Write an arithmetic value to the file stream.
   * \param[in] data An arithmetic value of type std::uint8_t.
   * \return The number of bytes written
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by FileStream::EnterWriteMode)
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string buffer too small for data.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(std::uint8_t const& data) noexcept override;
  /*!
   * \brief Write an arithmetic value to the file stream.
   * \param[in] data An arithmetic value of type std::uint8_t.
   * \param[in] base Base value of the arithmetic value to be written
   * \return The number of bytes written
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by FileStream::EnterWriteMode).
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string buffer too small for data.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(std::uint8_t const& data, Base base) noexcept override;

  /*!
   * \brief Write an arithmetic value to the file stream.
   * \param[in] data An arithmetic value of type std::uint16_t.
   * \return The number of bytes written
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by FileStream::EnterWriteMode).
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string buffer too small for data.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(std::uint16_t const& data) noexcept override;
  /*!
   * \brief Write an arithmetic value to the file stream.
   * \param[in] data An arithmetic value of type std::uint16_t.
   * \param[in] base Base value of the arithmetic value to be written
   * \return The number of bytes written
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by FileStream::EnterWriteMode).
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string buffer too small for data.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(std::uint16_t const& data, Base base) noexcept override;
  /*!
   * \brief Write an arithmetic value to the file stream.
   * \param[in] data An arithmetic value of type std::uint32_t.
   * \return The number of bytes written
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by FileStream::EnterWriteMode)
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string buffer too small for data.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(std::uint32_t const& data) noexcept override;

  /*!
   * \brief Write an arithmetic value to the file stream.
   * \param[in] data An arithmetic value of type std::uint32_t.
   * \param[in] base Base value of the arithmetic value to be written
   * \return The number of bytes written
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by FileStream::EnterWriteMode).
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string buffer too small for data.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(std::uint32_t const& data, Base base) noexcept override;

  /*!
   * \brief Write an arithmetic value to the file stream.
   * \param[in] data An arithmetic value of type std::uint64_t.
   * \return The number of bytes written
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by FileStream::EnterWriteMode).
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string buffer too small for data.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(std::uint64_t const& data) noexcept override;

  /*!
   * \brief Write an arithmetic value to the file stream.
   * \param[in] data An arithmetic value of type std::uint64_t.
   * \param[in] base Base value of the arithmetic value to be written
   * \return The number of bytes written
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by FileStream::EnterWriteMode)
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string buffer too small for data.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(std::uint64_t const& data, Base base) noexcept override;

  /*!
   * \brief Write an arithmetic value to the file stream.
   * \param[in] data An arithmetic value of float.
   * \return The number of bytes written
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by FileStream::EnterWriteMode).
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(float const& data) noexcept override;

  /*!
   * \brief Write an arithmetic value to the file stream.
   * \param[in] data An arithmetic value of type double.
   * \return The number of bytes written
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by FileStream::EnterWriteMode).
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(double const& data) noexcept override;

  /*!
   * \brief Write a basic string view to the stream.
   * \param[in] data A basic string view.
   * \return The number of bytes written.
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by FileStream::EnterWriteMode).
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(amsr::core::StringView data) noexcept override;

  /*!
   * \brief Write a basic C string view to the stream.
   * \param[in] data A basic C string view.
   * \return The number of bytes written.
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by FileStream::EnterWriteMode).
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(vac::container::CStringView data) noexcept override;

  /*!
   * \brief Write a span of characters to the string stream.
   * \param[in] data The span to write.
   * \return The number of bytes written
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by FileStream::EnterWriteMode).
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG, or input and
   * output buffer overlap.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(amsr::core::Span<char const> data) noexcept override;

  /*!
   * \brief Write a span of characters in hexadecimal or binary base to the string stream.
   * \param[in] data The span to write.
   * \param[in] base Base value of the arithmetic value to be written.
   * \return The number of bytes written
   * \context ANY
   * \pre Aborts if the file stream is closed (checked by FileStream::Write) or not writable (checked by
   * FileStream::EnterWriteMode).
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC; or string stream buffer is too small for data.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type or base is kDecimal.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(amsr::core::Span<std::uint8_t const> data, Base base) noexcept override;

  /*!
   * \brief Write a span of bytes to the string stream.
   * \param[in] data The span to write.
   * \return The number of written bytes.
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by FileStream::EnterWriteMode).
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG, or input and
   * output buffer overlap.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(amsr::core::Span<vac::language::byte const> data) noexcept override;

  /*!
   * \brief Write a number of characters to the file stream by a provided callback.
   * \details The callback might be called multiple times with different views into the stream.
   * \warning The callback might be called less often than requested (even zero times).
   * \param[in] max_count Maximum number of characters to be written.
   * \param[in] callback The callback that gets access to the internal buffer. The return value indicates the number of
   * bytes written into the buffer. In case the return value is equal in size to the provided buffer (and max_count not
   * reached) the stream will try to call again.
   * \return The actual number of bytes written.
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by FileStream::EnterWriteMode). Callback has
   * to be checked for FFI with respect to memory.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::uint64_t> Write(
      std::uint64_t max_count,
      vac::language::FunctionRef<std::uint64_t(amsr::core::Span<char>)> callback) noexcept override;

  /*!
   * \brief Write data to the file stream by a provided callback.
   * \details The callback might be called multiple times with different views into the stream.
   * \warning The callback might be called less often than requested (even zero times).
   * \param[in] callback The callback that gets access to the internal buffer. The return value indicates the number of
   * bytes written into the buffer. In case the return value is equal in size to the provided buffer the stream will try
   * to call again.
   * \return The actual number of bytes written.
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by FileStream::EnterWriteMode). Callback has
   * to be checked for FFI with respect to memory.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::uint64_t> Write(
      vac::language::FunctionRef<std::uint64_t(amsr::core::Span<char>)> callback) noexcept override;

  /*!
   * \brief Write a single character to the file stream.
   * \param[in] data The character.
   * \return The number of bytes written.
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by FileStream::EnterWriteMode).
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<void> Put(char data) noexcept override;

  /*!
   * \brief Returns the size of a file.
   * \return The file size.
   * \context ANY
   * \pre Aborts if the file stream is closed (checked by FileStream::GetFileSize).
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  std::uint64_t GetFileSize() const noexcept;

  /*!
   * \brief Get the number of remaining space in buffer.
   * \details For file streams the buffer is flushed when the last element is written.
   * \return The number of space left.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Span<char>::index_type GetRemainingBufferSize() const noexcept override;

 private:
  /*!
   * \brief Write an integral value to the file stream.
   * \tparam Number An integral data type.
   * \param[in] data An integral value.
   * \param[in] base Base value of the arithmetic value to be written.
   * \return The number of bytes written
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by FileStream::EnterWriteMode).
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string buffer too small for data.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * - Abort if file is not open.
   * - Write data with WriteByCallingToTextual, WritePrefixedBinary or WritePrefixedHex.
   * \endinternal
   */
  template <typename Number,
            std::enable_if_t<std::is_integral<Number>::value && (!std::is_same<Number, bool>::value), std::int32_t> = 0>
  amsr::core::Result<std::int64_t> Write(Number data, Base base) noexcept {
    amsr::core::Result<std::int64_t> result{amsr::core::Result<std::int64_t>::FromValue(0)};
    bool const show_base_prefix{ShowBasePrefix()};  // VCA_IOSTREAM_NO_NULL_POINTER
    if ((base == amsr::stream::Base::kDecimal) || (!show_base_prefix)) {
      result = WriteByCallingToTextual(data, base);
    } else if (base == amsr::stream::Base::kBinary) {
      result = WritePrefixedBinary(data);
    } else {
      result = WritePrefixedHex(data);
    }

    return result;
  }

  /*!
   * \brief Writes a boolean value starting at the current write position of the output stream.
   * \param[in] data A boolean value.
   * \return The number of bytes written
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by FileStream::EnterWriteMode).
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * #20 Convert integral number to textual representation
   * #30 Write to stream if conversion succeeded
   * \endinternal
   */
  amsr::core::Result<std::int64_t> WriteByCallingToTextual(bool const& data) noexcept {
    // #20
    constexpr std::size_t const kFalseLiteralSize{5};
    amsr::core::Array<char, kFalseLiteralSize> buffer_arr{};
    amsr::core::Span<char> const buffer{buffer_arr};
    // this operation never fails since buffer is large enough
    charconv::ToTextual<bool> to_textual{buffer, data};
    amsr::core::Result<amsr::core::Span<char>> const conversion_result{to_textual.Convert()};
    AssertHasValue(conversion_result, __FILE__, __LINE__, "Internal error: Conversion should never fail.");

    // #30
    amsr::core::Span<char const> const span{conversion_result.Value()};
    return Write(span);  // VCA_IOSTREAM_NO_NULL_POINTER, VCA_IOSTREAM_UNDET_POINTS_TO_SET_VALID_PTR_NO_PRECOND
  }

  /*!
   * \brief Writes an integral value starting at the current write position of the output stream.
   * \tparam Number An integral data type.
   * \param[in] data An integral value.
   * \param[in] base Base value of the arithmetic value to be written.
   * \return The number of bytes written
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by FileStream::EnterWriteMode).
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * #20 Convert integral number to textual representation
   * #30 Write to stream if conversion succeeded
   * \endinternal
   */
  template <typename Number,
            std::enable_if_t<std::is_integral<Number>::value && (!std::is_same<Number, bool>::value), std::int32_t> = 0>
  amsr::core::Result<std::int64_t> WriteByCallingToTextual(Number data, Base base) noexcept {
    // #20
    constexpr std::uint8_t uint8_max{std::numeric_limits<std::uint8_t>::max()};  // maximum width
    amsr::core::Array<char, uint8_max> buffer_arr{};
    amsr::core::Span<char> const buffer{buffer_arr};
    charconv::NumberBase const number_base{ToCharconvBase(base)};  // VCA_IOSTREAM_NO_NULL_POINTER
    // VCA_IOSTREAM_NO_NULL_POINTER, VCA_IOSTREAM_NO_OVERLAP
    charconv::ExtendedFormat const format{GetCharconvFormat()};

    // this operation never fails since buffer is large enough
    charconv::ToTextual<Number> to_textual{buffer, data};
    amsr::core::Result<amsr::core::Span<char>> const conversion_result{to_textual.Convert(number_base, format)};
    AssertHasValue(conversion_result, __FILE__, __LINE__, "Internal error: Conversion should never fail.");

    // #30
    amsr::core::Span<char const> const span{conversion_result.Value()};
    return Write(span);  // VCA_IOSTREAM_NO_NULL_POINTER, VCA_IOSTREAM_UNDET_POINTS_TO_SET_VALID_PTR_NO_PRECOND
  }

  /*!
   * \brief Writes a floating point value starting at the current write position of the output stream.
   * \tparam FP A floating point data type.
   * \param[in] data A floating point value.
   * \return The number of bytes written
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by FileStream::EnterWriteMode).
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * #20 Convert floating point number to textual representation
   * #30 Write to buffer if conversion succeeded
   * \endinternal
   */
  template <typename FP, std::enable_if_t<std::is_floating_point<FP>::value, std::int32_t> = 0>
  amsr::core::Result<std::int64_t> WriteByCallingToTextual(FP const data) noexcept {
    // #20
    amsr::core::Array<char, amsr::charconv::kFPMaxLength> buffer_arr{};
    amsr::core::Span<char> const buffer{buffer_arr};
    charconv::ToTextual<FP> to_textual{buffer, data};
    amsr::charconv::FPFormat const format{GetCharconvFloatingPointFormat()};  // VCA_IOSTREAM_NO_NULL_POINTER
    // this operation never fails since buffer is large enough
    amsr::core::Result<amsr::core::Span<char>> const conversion_result{to_textual.Convert(format)};
    AssertHasValue(conversion_result, __FILE__, __LINE__, "Internal error: Conversion should never fail.");

    // #30
    amsr::core::Span<char const> const span{conversion_result.Value()};
    return Write(span);  // VCA_IOSTREAM_NO_NULL_POINTER
  }

  /*!
   * \brief Writes an integral value with prefix to the file stream.
   * \tparam Number An integral data type.
   * \param[in] data An integral value.
   * \param[in] base Base.
   * \return The number of bytes written
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by FileStream::EnterWriteMode).
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string buffer too small for data.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * #20 Write to temporary array.
   * #30 Assert if writing is succeeded.
   * #40 Write to file if writing to array succeeded.
   * \endinternal
   */
  template <typename Number, std::enable_if_t<std::is_integral<Number>::value, std::int32_t> = 0>
  amsr::core::Result<std::int64_t> WritePrefixed(Number data, Base base) noexcept {
    // #20
    constexpr std::uint8_t uint8_max{std::numeric_limits<std::uint8_t>::max()};  // maximum width
    constexpr std::uint8_t prefix_length{2};
    amsr::core::Array<char, uint8_max + prefix_length> buffer_arr{};  // as prefix is not part of width
    amsr::core::Span<char> const buffer_span{buffer_arr};

    amsr::stream::stringstream::OutputStringStream oss{buffer_span};
    char const fill_char{GetFillChar()};            // VCA_IOSTREAM_NO_NULL_POINTER
    LetterCase const letter_case{GetLetterCase()};  // VCA_IOSTREAM_NO_NULL_POINTER
    std::uint8_t const output_width{GetWidth()};    // VCA_IOSTREAM_NO_NULL_POINTER
    oss.SetFormat(ShowBase::kYes, output_width, fill_char, letter_case);
    amsr::core::Result<std::int64_t> bytes_written_result{oss.Write(data, base)};

    // #30
    // Write always succeeds because buffer is large enough to hold valid data.
    Assert(bytes_written_result.HasValue(), __FILE__, __LINE__, amsr::core::StringView{"Conversion failed."});
    std::int64_t const bytes_written{bytes_written_result.Value()};

    // #40
    // VCA_IOSTREAM_NO_NULL_POINTER, VCA_IOSTREAM_UNDETERMINED_POINTS_TO_SET_SUBSPAN
    return Write(buffer_span.subspan(0, amsr::charconv::ConvertOrAbort<std::size_t>(bytes_written)));
  }

  /*!
   * \brief Writes an integral value in hexadecimal format to the stream, prefixed with 0x.
   * \tparam Number An integral data type.
   * \param[in] data An integral value.
   * \return The number of bytes written
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by FileStream::EnterWriteMode).
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string buffer too small for data.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * #10 Write prefixed number.
   * \endinternal
   */
  template <typename Number, std::enable_if_t<std::is_integral<Number>::value, std::int32_t> = 0>
  amsr::core::Result<std::int64_t> WritePrefixedHex(Number data) noexcept {
    return WritePrefixed(data, Base::kHex);  // VCA_IOSTREAM_REF_PARAM
  }

  /*!
   * \brief Writes an integral value in binary format to the stream, prefixed with 0b.
   * \tparam Number An integral data type.
   * \param[in] data An integral value.
   * \return The number of bytes written.
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by FileStream::EnterWriteMode).
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string buffer too small for data.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * #10 Write prefixed number.
   * \endinternal
   */
  template <typename Number, std::enable_if_t<std::is_integral<Number>::value, std::int32_t> = 0>
  amsr::core::Result<std::int64_t> WritePrefixedBinary(Number data) noexcept {
    // #10
    return WritePrefixed(data, Base::kBinary);  // VCA_IOSTREAM_REF_PARAM
  }

  /*!
   * \brief Do not use Write(char const*).
   * \details Without this overload, a function with a char* parameter will call the overload Write(uint8_t). To prevent
   * this unintended behavior, this overload is defined as a private member function to cause a compile error.
   * \return Dummy value.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(char const* /*data*/) noexcept override;

  /*! Friend test declaration. */
  FRIEND_TEST(UT__OutputFileBasedStream__Other, WriteCharConstPointer__Success);

  /*! Friend test declaration. */
  FRIEND_TEST(UT__OutputFileBasedStream__Other, MoveConstructor);

  /*! Friend test declaration. */
  FRIEND_TEST(UT__OutputFileBasedStream__Other, MoveAssignment);

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoStream_A11.3.1_friend_class
  /*! Friend test declaration. */
  template <typename T>
  FRIEND_TEST(CT__IoStream__OutputFileStdStreamTypesCReq, WriteCharConstPointer);

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoStream_A11.3.1_friend_class
  /*! Friend test declaration. */
  template <typename T>
  FRIEND_TEST(CT__IoStream__StatefulOutputFileStreamTypesCReq, WriteCharConstPointer);

  /*! underlying file stream */
  std::unique_ptr<FileStreamInterface> fstream_;
};

}  // namespace filestream
}  // namespace stream
}  // namespace amsr

#endif  // LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_OUTPUT_FILE_BASED_STREAM_H_
