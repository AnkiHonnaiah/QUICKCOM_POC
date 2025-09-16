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
 *        \brief  InputOutputFileStream class to read from and write to a file.
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_INPUT_OUTPUT_FILE_STREAM_H_
#define LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_INPUT_OUTPUT_FILE_STREAM_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <string>

#include "amsr/core/span.h"
#include "amsr/core/string_view.h"
#include "amsr/iostream/filestream/file_stream.h"
#include "amsr/iostream/filestream/file_stream_interface.h"
#include "amsr/iostream/input_output_stream.h"
#include "amsr/iostream/io_format.h"
#include "vac/container/c_string_view.h"
#include "vac/language/byte.h"

namespace amsr {
namespace stream {
namespace filestream {

// VECTOR NC Metric-OO.WMC.One: MD_IoStream_Metric-OO.WMC.One_interface
/*!
 * \brief Class to read from and write to a file.
 * \unit amsr::stream::filestream::InputOutputFileStream
 * \vprivate Product Private
 */
class InputOutputFileStream : public amsr::stream::InputOutputStream {
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
  InputOutputFileStream() noexcept;

  /*!
   * \brief Constructs an input output filestream using an external buffer.
   * \param[in] buffer Externally owned buffer.
   * \context ANY
   * \pre Aborts if \c buffer is empty (checked in FileStream::FileStream).
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  explicit InputOutputFileStream(amsr::core::Span<char> buffer) noexcept;

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
  explicit InputOutputFileStream(std::unique_ptr<FileStreamInterface> file_stream_ptr) noexcept;

  // InputOutputFileStream not copy constructible because base class InputStream is not copy constructible.
  InputOutputFileStream(InputOutputFileStream const& input_output_file_stream) = delete;

  // InputOutputFileStream not copy assginable because base class InputStream is not copy assignable.
  InputOutputFileStream& operator=(InputOutputFileStream const& input_output_file_stream) & = delete;

  /*!
   * \brief Move constructs instance of class.
   * \param[in] input_output_file_stream Instance of InputOutputFileStream.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  InputOutputFileStream(InputOutputFileStream&& input_output_file_stream) noexcept = default;

  /*!
   * \brief Move assign instance of class.
   * \param[in] input_output_file_stream Instance of InputOutputFileStream.
   * \return Moved InputOutputFileStream
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  InputOutputFileStream& operator=(InputOutputFileStream&& input_output_file_stream) & noexcept =
      default;  // VCA_IOSTREAM_STD_UNIQUE_PTR_MOVE_ASSIGNMENT_CONTRACT

  /*!
   * \brief Destructs instance.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  ~InputOutputFileStream() noexcept override;

  /*!
   * \brief Opens the associated file of the stream with predefined flags kReadWrite and kCreate.
   * \param[in] file_name File name of the file.
   * \return Result with value void.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of
   * \c errno EMFILE, ENFILE or ENOSPC, ENOBUFS.
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
   * \brief Closes the associated with the stream.
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
   * \brief Read retrieves the content of the associated file and fills the given buffer up to the size of it.
   * \param[out] buffer Memory to save the read bytes to.
   * \return A span of the given memory which is either the same size, if enough data was retrieved from the file, or
   * smaller, if end of file was reached.
   * \context ANY
   * \pre Aborts if the file stream is closed or not readable (both checked by FileStream::Read).
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOMEM or ENOBUFS.
   * \error StreamErrc::kInvalidArgument File position is invalid caused by \c errno EOVERFLOW. Or empty buffer,
   * or read size exceeds the numeric limits of std::size_t, or buffer cannot be copied because of invalid parameters.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<amsr::core::Span<vac::language::byte>> Read(
      amsr::core::Span<vac::language::byte> buffer) noexcept override;

  /*!
   * \brief Read retrieves the content of the associated file and fills the given buffer up to the size of it.
   * \param[out] buffer Memory to save the read characters to.
   * \return A span of the given memory which is either the same size, if enough data was retrieved from the file, or
   * smaller, if end of file was reached.
   * \context ANY
   * \pre Aborts if the file stream is closed or not readable (both checked by FileStream::Read).
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOMEM or ENOBUFS.
   * \error StreamErrc::kInvalidArgument File position is invalid caused by \c errno EOVERFLOW. Or empty buffer,
   * or read size exceeds the numeric limits of std::size_t, or buffer cannot be copied because of invalid parameters.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<amsr::core::Span<char>> Read(amsr::core::Span<char> buffer) noexcept override;

  /*!
   * \brief Reads up to \c count bytes from the file without additional copy.
   * \details Returns a view on the internal buffer. This view may be smaller than the requested amount of data. The
   * data is only guaranteed to be available until the next call to a non-const member function of the stream.
   * \param[in] count Maximum number of bytes to be read.
   * \return A span of at most size \c count (empty span if EOF is reached).
   * \context ANY
   * \pre Aborts if the file stream is closed or not readable (both checked by FileStream::Read).
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOMEM or ENOBUFS.
   * \error StreamErrc::kInvalidArgument File position is invalid caused by \c errno EOVERFLOW.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<amsr::core::Span<char const>> Read(std::uint64_t count) noexcept override;

  /*!
   * \brief Reads up to \c count bytes from the file without additional copy and provides them in a callback.
   * \details Returns the number of read characters. This number may be smaller than the requested amount of data. The
   * data provided is only guaranteed to be available until the next callback/call to a non-const member function of the
   * stream.
   * \param[in] count Maximum number of bytes to be read.
   * \param[in] callback The callback that is called on every buffer view.
   * \return Number of read characters.
   * \context ANY
   * \pre Aborts if the file stream is closed (checked by FileStream::IsEof) or not readable.
   * (checked by FileStream::Read). Callback has to be checked for FFI with respect to memory.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOMEM or ENOBUFS.
   * \error StreamErrc::kInvalidArgument File position is invalid caused by \c errno EOVERFLOW.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::uint64_t> Read(
      std::uint64_t count, vac::language::FunctionRef<void(amsr::core::StringView)> callback) noexcept override;

  /*!
   * \brief Reads from the file until \c pred returns \c false for a character.
   * \details Returns a view on the internal buffer. This view may not contain all matching characters. To retrieve all
   * matching characters, the method should be called repeatedly until it returns zero bytes. The
   * data are only guaranteed to be available until the next call to a non-const member function of the stream happens.
   * The function can be used as ReadWhile as well as ReadUntil, depending on the definition of the predicate.
   * \param[in] pred Predicate function to determine if a character shall still be read.
   * \return A view on the internal buffer, ending when \c pred returns false or if EOF is reached.
   * \context ANY
   * \pre Aborts if the file stream is closed or not readable (both checked by FileStream::ReadWhile). Callback has to
   * be checked for FFI with respect to memory.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOMEM or ENOBUFS.
   * \error StreamErrc::kInvalidArgument File position is invalid caused by \c errno EOVERFLOW.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<amsr::core::Span<char const>> ReadWhile(
      vac::language::FunctionRef<bool(char)> pred) noexcept override;

  /*!
   * \brief Reads from the file until any \c delimiter is found.
   * \details Returns the delimiter found or EOF. The views found in between are provided via \c callback.
   * The data is only guaranteed to be available until the next callback / call to a non-const member function of the
   * stream. The found delimiter is not consumed, meaning that for repeated calls to ReadUntil the file position needs
   * to be advanced by at least 1.
   * \param[in] delimiter A set of delimiters that stop reading.
   * \param[in] callback The callback that is called for every view read.
   * \return The delimiter that was found or if it was not EOF.
   * \context ANY
   * \pre Aborts if the file stream is closed or not readable (both checked by FileStream::Read). Callback has to be
   * checked for FFI with respect to memory.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOMEM or ENOBUFS.
   * \error StreamErrc::kInvalidArgument File position is invalid caused by \c errno EOVERFLOW. Or no delimiters given.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> ReadUntil(
      amsr::core::StringView delimiter,
      vac::language::FunctionRef<void(amsr::core::StringView)> callback) noexcept override;

  /*!
   * \brief Finds the first character from the current offset (inclusive) in the stream matching the predicate.
   * \details Moves the stream offset to the first match or to EOF.
   * \param[in] pred Function determining if a character is a match.
   * \return Offset of the found character.
   * \context ANY
   * \pre Aborts if the file stream is closed or not readable (both checked by FileStream::Find). Callback has to be
   * checked for FFI with respect to memory.
   * \threadsafe FALSE
   * \error StreamErrc::kEndOfFile If the character was not found.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::uint64_t> Find(vac::language::FunctionRef<bool(char)> pred) noexcept override;

  /*!
   * \brief Finds the first character in the stream from the current offset (inclusive) that is contained in set.
   * \details Moves the stream offset to the first match or to EOF.
   * \param[in] set Set of characters to search for.
   * \return Offset of the found character.
   * \context ANY
   * \pre Aborts if the file stream is closed or not readable (both checked by
   * FileStream::Find(vac::language::FunctionRef<bool(char)>)).
   * \threadsafe FALSE
   * \error StreamErrc::kEndOfFile If the character was not found.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::uint64_t> Find(amsr::core::StringView set) noexcept override;

  /*!
   * \brief Finds the first character in the stream from the current offset (inclusive) that is not contained in set.
   * \details Moves the stream offset to the first mismatch or to EOF.
   * \param[in] set Set of characters.
   * \return Offset of the first mismatch.
   * \context ANY
   * \pre Aborts if the file stream is closed or not readable (both checked by
   * FileStream::Find(vac::language::FunctionRef<bool(char)>)).
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::uint64_t> FindNot(amsr::core::StringView set) noexcept override;

  /*!
   * \brief Gets one character from the current read position of the associated file.
   * \return The character or EOF.
   * \context ANY
   * \pre Aborts if the file stream is closed or not readable (both checked by FileStream::Get).
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Get() noexcept override;

  /*!
   * \brief Makes the most recently extracted character available again.
   * \return Result of type void.
   * \context ANY
   * \pre Aborts if the file stream is closed or not readable (both checked by FileStream::UnGet).
   * \threadsafe FALSE
   * \error StreamErrc::kInvalidPosition In case current position is at the beginning of file.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<void> UnGet() noexcept override;

  /*!
   * \brief Peeks at the next character in the file without changing the current position.
   * - Reads one byte out of the file.
   * - Resets the position in the file.
   * \return The peeked byte.
   * \context ANY
   * \pre Aborts if the file stream is closed or not readable (both checked by FileStream::Read).
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Peek() noexcept override;

  /*!
   * \brief Checks if end-of-file is reached.
   * \return \c true, if yes or if no file is opened, \c false, if no.
   * \context ANY
   * \pre Aborts if the file stream is closed (checked by FileStream::IsEof).
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  bool IsEof() const noexcept override;

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
   * \brief Write a span of characters to the file stream.
   * \param[in] data The span to write.
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
  amsr::core::Result<std::int64_t> Write(amsr::core::Span<char const> data) noexcept override;

  /*!
   * \brief Write a span of bytes to the string stream.
   * \param[in] data The span to write.
   * \return The number of written bytes.
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by EnterWriteMode).
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
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno ENOBUFS
   * or ENOSPC.
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
   * \pre Aborts if the file stream is closed or not writable (both checked by EnterWriteMode). Callback has to be
   * checked for FFI with respect to memory.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno ENOBUFS
   * or ENOSPC.
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
   * \brief Writes a span of byte-sized values in hexadecimal or binary base to the stream.
   * \param[in] data A span of values of type std::int8_t.
   * \param[in] base Base value of the arithmetic value to be written.
   * \return The number of bytes written.
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
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(amsr::core::Span<std::uint8_t const> data, Base base) noexcept override;

  /*!
   * \brief Write a character to the stream.
   * \param[in] data A character to write.
   * \return The number of bytes written.
   * \context ANY
   * \pre Aborts if the file stream is closed or not writable (both checked by EnterWriteMode).
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
  /*! underlying file stream */
  std::unique_ptr<FileStreamInterface> fstream_;

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
  FRIEND_TEST(UT__InputOutputFileStream, WriteCharConstPointer__Success);

  /*! Friend test declaration. */
  FRIEND_TEST(CT__IoStream__InputOutputFileStreamCReq, WriteCharConstPointer);

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoStream_A11.3.1_friend_class
  /*! Friend test declaration. */
  template <typename T>
  FRIEND_TEST(CT__IoStream__StatefulOutputFileStreamTypesCReq, WriteCharConstPointer);
};

}  // namespace filestream
}  // namespace stream
}  // namespace amsr

#endif  // LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_INPUT_OUTPUT_FILE_STREAM_H_
