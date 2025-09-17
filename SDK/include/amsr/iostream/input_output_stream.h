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
 *        \brief  Interface class for all input-output streams.
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_INPUT_OUTPUT_STREAM_H_
#define LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_INPUT_OUTPUT_STREAM_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/charconv/type_helper.h"
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "amsr/iostream/input_stream.h"
#include "amsr/iostream/output_stream.h"
#include "amsr/iostream/types.h"
#include "vac/container/c_string_view.h"
#include "vac/language/byte.h"
#include "vac/language/function_ref.h"

namespace amsr {
namespace stream {

// VECTOR NC Metric-OO.WMC.One: MD_IoStream_Metric-OO.WMC.One_interface
/*!
 * \brief Interface class for input-output streams which inherits from InputStream interface.
 * \unit amsr::stream::InputOutputStream
 * \vprivate Product Private
 */
class InputOutputStream : public InputStream {
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
  InputOutputStream() noexcept = default;

  /*!
   * \brief Destructs instance.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  ~InputOutputStream() noexcept override = default;

  // InputOutputStream is not copy constructible because a stream is not copy constructible.
  InputOutputStream(InputOutputStream const&) = delete;

  // InputOutputStream is not copy assignable because a stream is not copy assignable.
  InputOutputStream& operator=(InputOutputStream const&) & = delete;

 protected:
  /*!
   * \brief Move constructs instance of class.
   * \param[in] stream Instance of InputOutputStream.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  InputOutputStream(InputOutputStream&& stream) noexcept = default;

  /*!
   * \brief Move assigns instance of class.
   * \param[in] stream Instance of InputOutputStream.
   * \return Moved InputOutputFileStream
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  InputOutputStream& operator=(InputOutputStream&& stream) & noexcept = default;

 public:
  /*!
   * \brief Get the current file position.
   * \return The current file position, measured in bytes from the beginning of the file.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed.
   * \threadsafe FALSE
   * \error StreamErrc::kInvalidPosition Position in stream cannot be determined.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::uint64_t> Tell() const noexcept override = 0;

  /*!
   * \brief Changes the file position of the file.
   * \param[in] offset Offset to change the file position with.
   * \param[in] whence Specifies how the offset is interpreted:
   * - kBeg: offset is a count of characters from the beginning of the file.
   * - kEnd: offset is a count of characters from the end of the file.
   * - kCur: offset is a count of characters from the current file position.
   * \return The updated file position in the file, measured in bytes from the beginning of the file.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not seekable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC.
   * \error StreamErrc::kInvalidArgument Offset exceeding numeric limits (can be caused by \c errno EOVERFLOW) or
   * whence argument invalid (can be caused by \c errno EINVAL), or file size exceeds file size limit caused by
   * \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::uint64_t> Seek(std::int64_t offset, SeekDirection whence) noexcept override = 0;

  /*!
   * \brief Read from the underlying 'stream'-object (e.g. a file or a string) up to the size of the given buffer.
   * \param[out] buffer Memory to safe the read content to.
   * \return A span of the part of the given buffer with the read content or a posix error.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not readable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOMEM or ENOBUFS.
   * \error StreamErrc::kInvalidArgument File position is invalid caused by \c errno EOVERFLOW. Or buffer is
   * empty or read size exceeds the numeric limits of std::size_t, or buffer cannot be copied because of invalid
   * parameters.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<amsr::core::Span<vac::language::byte>> Read(
      amsr::core::Span<vac::language::byte> buffer) noexcept override = 0;

  /*!
   * \brief Read from the underlying 'stream'-object (e.g. a file or a string) up to the size of the given buffer.
   * \param[out] buffer Memory to safe the read content to.
   * \return A span of the part of the given buffer with the read content or a posix error.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not readable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOMEM or ENOBUFS.
   * \error StreamErrc::kInvalidArgument File position is invalid caused by \c errno EOVERFLOW. Or buffer is
   * empty or read size exceeds the numeric limits of std::size_t, or buffer cannot be copied because of invalid
   * parameters.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<amsr::core::Span<char>> Read(amsr::core::Span<char> buffer) noexcept override = 0;

  /*!
   * \brief Reads up to \c count bytes from the stream without additional copy.
   * \details Returns a view on the internal buffer. This view may be smaller than the requested amount of data. The
   * data is only guaranteed to be available until the next call to a non-const member function of the stream.
   * \param[in] count Maximum number of bytes to be read.
   * \return Up to count bytes inside the internal buffer (zero bytes if EOF is reached).
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not readable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOMEM or ENOBUFS.
   * \error StreamErrc::kInvalidArgument File position is invalid caused by \c errno EOVERFLOW. Or read size
   * exceeds the numeric limits of std::size_t.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<amsr::core::Span<char const>> Read(std::uint64_t count) noexcept override = 0;

  /*!
   * \brief Reads up to \c count bytes from the stream without additional copy and provides them in a callback.
   * \details Returns the number of read characters. This number may be smaller than the requested amount of data.
   * The data provided is only guaranteed to be available until the next callback/call to a non-const member function of
   * the stream.
   * \param[in] count Maximum number of bytes to be read.
   * \param[in] callback The callback that is called on every buffer view.
   * \return Number of read characters.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not readable. Callback has to be checked for
   * FFI with respect to memory.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOMEM or ENOBUFS.
   * \error StreamErrc::kInvalidArgument File position is invalid caused by \c errno EOVERFLOW. Or read size
   * exceeds the numeric limits of std::size_t.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::uint64_t> Read(
      std::uint64_t count, vac::language::FunctionRef<void(amsr::core::StringView)> callback) noexcept override = 0;

  /*!
   * \brief Reads from the stream until \c pred returns \c false for a character.
   * \details Returns a view on the internal buffer. This view may not contain all matching characters. To retrieve all
   * matching characters, the method should be called repeatedly until it returns zero bytes. The data is only
   * guaranteed to be available until the next call to a non-const member function of the stream.
   * \param[in] pred Predicate function to determine if a character shall still be read.
   * \return A number of bytes inside the internal buffer (zero bytes if \c pred returns false or if EOF is reached).
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not readable. Callback has to be checked for
   * FFI with respect to memory.
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
      vac::language::FunctionRef<bool(char)> pred) noexcept override = 0;

  /*!
   * \brief Reads from the stream until \c delimiter is encountered.
   * \details Returns the delimiter found or EOF. The views found in between are provided via \c callback.
   * The data is only guaranteed to be available until the next callback / call to a non-const member function of the
   * stream. The found delimiter is not consumed, meaning that for repeated calls to ReadUntil the file position needs
   * to be advanced by at least 1.
   * \param[in] delimiter A set of delimiters that stop reading.
   * \param[in] callback The callback that is called for every view read.
   * \return The delimiter that was found or if it was not EOF.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not readable. Callback has to be checked for
   * FFI with respect to memory.
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
      vac::language::FunctionRef<void(amsr::core::StringView)> callback) noexcept override = 0;

  /*!
   * \brief Finds the first character from the current offset (inclusive) in the stream matching the predicate.
   * \details Moves the stream offset to the first match or to EOF.
   * \param[in] pred Function determining if a character is a match.
   * \return Offset of the found character.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not readable. Callback has to be checked for
   * FFI with respect to memory.
   * \threadsafe FALSE
   * \error StreamErrc::kEndOfFile If the character was not found.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::uint64_t> Find(vac::language::FunctionRef<bool(char)> pred) noexcept override = 0;

  /*!
   * \brief Finds the first character from the current offset (inclusive) in the stream that is contained in the set.
   * \details Moves the stream offset to the first match or to EOF.
   * \param[in] set Set of characters to look for.
   * \return Offset of the found character.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not readable.
   * \threadsafe FALSE
   * \error StreamErrc::kEndOfFile If the character was not found.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::uint64_t> Find(amsr::core::StringView set) noexcept override = 0;

  /*!
   * \brief Finds the first character in the stream from the current offset (inclusive) that is not contained in set.
   * \details Moves the stream offset to the first mismatch or to EOF.
   * \param[in] set Set of characters.
   * \return Offset of the first mismatch.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not readable.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::uint64_t> FindNot(amsr::core::StringView set) noexcept override = 0;

  /*!
   * \brief Get a character from the underlying stream object (e.g. a file or a string).
   * \return The character or EOF.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not readable.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Get() noexcept override = 0;

  /*!
   * \brief Makes the most recently extracted character available again.
   * \return Result of type void.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not readable.
   * \threadsafe FALSE
   * \error StreamErrc::kInvalidPosition In case current position is at the beginning of file/string.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<void> UnGet() noexcept override = 0;

  /*!
   * \brief Peeks the next character in the stream.
   * \return The peeked character.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not readable.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Peek() noexcept override = 0;

  /*!
   * \brief Checks if end-of-file is reached.
   * \return Result of type bool which is \c true, if yes, \c false, if no.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  bool IsEof() const noexcept override = 0;

  /*!
   * \brief Return open status of associated file.
   * \details For string streams always true is returned.
   * \return True if file is open, false otherwise. For string streams always true is returned.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  bool IsOpen() const noexcept override = 0;

  /*!
   * \brief Write a character to the stream.
   * \param[in] data A character.
   * \return The number of bytes written.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string stream buffer is too small.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::int64_t> Write(char data) noexcept = 0;

  /*!
   * \brief Write a basic string view to the stream.
   * \param[in] data A basic string view.
   * \return The number of bytes written
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string stream buffer is too small.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::int64_t> Write(amsr::core::StringView data) noexcept = 0;

  /*!
   * \brief Write a basic C string view to the stream.
   * \param[in] data A basic C string view.
   * \return The number of bytes written
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string stream buffer is too small.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::int64_t> Write(vac::container::CStringView data) noexcept = 0;

  /*!
   * \brief Write a span of characters to the stream.
   * \param[in] data The span to write.
   * \return The number of bytes written.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string stream buffer is too small.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG, or input and
   * output buffer overlap.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::int64_t> Write(amsr::core::Span<char const> data) noexcept = 0;

  /*!
   * \brief Write a span of bytes to the stream.
   * \param[in] data The span to write.
   * \return The number of bytes written.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string stream buffer is too small.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG, or input and
   * output buffer overlap.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::int64_t> Write(amsr::core::Span<vac::language::byte const> data) noexcept = 0;

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
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable. Callback has to be checked for
   * FFI with respect to memory.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC, or string stream buffer is too small.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::uint64_t> Write(
      std::uint64_t max_count, vac::language::FunctionRef<std::uint64_t(amsr::core::Span<char>)> callback) noexcept = 0;

  /*!
   * \brief Write data to the file stream by a provided callback.
   * \details The callback might be called multiple times with different views into the stream.
   * \warning The callback might be called less often than requested (even zero times).
   * \param[in] callback The callback that gets access to the internal buffer. The return value indicates the number of
   * bytes written into the buffer. In case the return value is equal in size to the provided buffer the stream will try
   * to call again.
   * \return The actual number of bytes written.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable. Callback has to be checked for
   * FFI with respect to memory.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC; or string stream buffer is too small.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::uint64_t> Write(
      vac::language::FunctionRef<std::uint64_t(amsr::core::Span<char>)> callback) noexcept = 0;

  /*!
   * \brief Writes a byte array in hexadecimal or binary base to the stream.
   * \param[in] data A span for a the byte array.
   * \param[in] base Base value of the arithmetic value to be written.
   * \return The number of bytes written.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC; or string stream buffer is too small for data.
   * \error StreamErrc::kInvalidArgument File size exceeds file size limit caused by \c errno EFBIG; or value
   * out of range of result type or base is kDecimal.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::int64_t> Write(amsr::core::Span<std::uint8_t const> data, Base base) noexcept = 0;

  /*!
   * \brief Synchronizes written data.
   * \return Result with value void.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS, ENOSPC or ENOMEM.
   * \error StreamErrc::kInvalidArgument File descriptor does not allow this operation caused by \c errno EINVAL, or
   * file position is invalid caused by \c errno EOVERFLOW, or file size exceeds file size limit caused by \c errno
   * EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<void> Sync() noexcept = 0;

  /*!
   * \brief Flushes internal write buffer to file.
   * \return Result with value void.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
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
  virtual amsr::core::Result<void> Flush() noexcept = 0;

  /*!
   * \brief Get the number of remaining characters in buffer.
   * \details For file streams the buffer is flushed when the last element is written.
   * \return The number of characters left.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Span<char>::index_type GetRemainingBufferSize() const noexcept = 0;

 protected:
  /*!
   * \brief Do not use Write(char*).
   * \details Without this overload, a function with a char* parameter will call the overload Write(uint8_t). To prevent
   * this unintended behavior, this overload is defined as a protected member function to cause a compile error.
   * \return Dummy value.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::int64_t> Write(char const* /*data*/) noexcept = 0;
};

}  // namespace stream
}  // namespace amsr

#endif  // LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_INPUT_OUTPUT_STREAM_H_
