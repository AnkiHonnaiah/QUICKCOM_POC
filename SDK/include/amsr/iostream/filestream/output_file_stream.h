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
 *        \brief  Implements an output file stream.
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_OUTPUT_FILE_STREAM_H_
#define LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_OUTPUT_FILE_STREAM_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include <memory>
#include <string>

#include "amsr/core/array.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/core/string_view.h"
#include "amsr/iostream/filestream/file_stream.h"
#include "amsr/iostream/filestream/output_file_based_stream.h"
#include "amsr/iostream/hex_prefix.h"
#include "amsr/iostream/output_stream.h"
#include "amsr/iostream/stream_error_domain.h"
#include "amsr/iostream/stringstream/output_string_stream.h"
#include "vac/language/byte.h"

namespace amsr {
namespace stream {
namespace filestream {

/*!
 * \brief Implements an output file stream.
 * \unit amsr::stream::filestream::OutputFileStream
 * \vprivate Product Private
 */
class OutputFileStream : public OutputFileBasedStream {
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
  OutputFileStream() noexcept;

  /*!
   * \brief Constructs an output filestream using an external buffer.
   * \param[in] buffer Externally owned buffer.
   * \context ANY
   * \pre Aborts if \c buffer is empty (checked in FileStream::FileStream).
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  explicit OutputFileStream(amsr::core::Span<char> buffer) noexcept;

  /*!
   * \brief Constructs instance of class.
   * \param[in] file_stream_ptr File stream.
   * \context ANY
   * \pre Aborts if \c file_stream_ptr is a nullptr (handled in OutputFileBasedStream::OutputFileBasedStream).
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  explicit OutputFileStream(std::unique_ptr<FileStreamInterface> file_stream_ptr) noexcept;

  // OutputFileStream is not copy constructible because FileStreams are not copy constructible.
  OutputFileStream(OutputFileStream const& output_file_stream) = delete;

  // OutputFileStream is not copy assignable because FileStreams are not copy assignable.
  OutputFileStream& operator=(OutputFileStream const& output_file_stream) & = delete;

  /*!
   * \brief Move constructs instance of class.
   * \param[in] output_file_stream Instance of OutputFileStream
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  OutputFileStream(OutputFileStream&& output_file_stream) noexcept = default;

  /*!
   * \brief Move assigns instance of class.
   * \param[in] output_file_stream Instance of OutputFileStream
   * \return Moved OutputFileStream
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  OutputFileStream& operator=(OutputFileStream&& output_file_stream) & noexcept = default;

  /*!
   * \brief Destructs instance.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  ~OutputFileStream() noexcept override = default;  // VCA_IOSTREAM_DESTRUCTOR

  /*!
   * \brief Opens the associated file of the stream with predefined flags kCreate and kWrite and mode 0755.
   * \param[in] file_name File name of the file.
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
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<void> Open(amsr::core::StringView file_name) const noexcept;

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
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<void> Open(amsr::core::StringView file_name, OpenModeType flags) const noexcept;

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
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<void> Open(amsr::core::StringView file_name, OpenModeType flags,
                                FilePermission permission) const noexcept;
};

}  // namespace filestream
}  // namespace stream
}  // namespace amsr

#endif  // LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_OUTPUT_FILE_STREAM_H_
