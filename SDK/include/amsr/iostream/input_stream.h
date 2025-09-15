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
 *        \brief  Interface class for all input streams.
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_INPUT_STREAM_H_
#define LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_INPUT_STREAM_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "amsr/iostream/io_format.h"
#include "amsr/iostream/types.h"
#include "vac/language/byte.h"
#include "vac/language/function_ref.h"

namespace amsr {
namespace stream {

/*!
 * \brief Interface class for all input streams to abstract from the underlying type of 'stream'.
 * \unit amsr::stream::InputStream
 * \vprivate Product Private
 */
class InputStream : public IOFormat {
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
  InputStream() noexcept = default;

  // InputStream is not copy constructible because a stream is not copy constructible.
  InputStream(InputStream const&) = delete;

  // InputStream is not copy assignable.
  InputStream& operator=(InputStream const&) = delete;

 protected:
  /*!
   * \brief Move constructs instance of class.
   * \param[in] stream Instance of InputStream.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  InputStream(InputStream&& stream) noexcept = default;

  /*!
   * \brief Move assigns instance of class.
   * \param[in] stream Instance of InputStream.
   * \return Moved InputOutputFileStream
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  InputStream& operator=(InputStream&& stream) & noexcept = default;

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
  ~InputStream() noexcept override = default;

  /*!
   * \brief Get the current position.
   * \return The current position, measured in bytes from the beginning of the file/string.
   * \context ANY
   * \pre In case of a file stream: Aborts if stream is closed.
   * \threadsafe FALSE
   * \error StreamErrc::kInvalidPosition Position in stream cannot be determined.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::uint64_t> Tell() const noexcept = 0;

  /*!
   * \brief Changes the position of the underlying stream.
   * \param[in] offset Offset to change the position with.
   * \param[in] whence Specifies how the offset is interpreted:
   * - kBeg: offset is a count of characters from the beginning of the file/string.
   * - kEnd: offset is a count of characters from the end of the file/string.
   * - kCur: offset is a count of characters from the current file/string position.
   * \return The updated position in the file/string, measured in bytes from the beginning of the file/string.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not seekable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOBUFS or ENOSPC.
   * \error StreamErrc::kInvalidArgument Offset exceeding numeric limits (can be caused by \c errno EOVERFLOW) or
   * whence argument invalid (can be caused by \c errno EINVAL), or file size exceeds file size limit caused
   * by \c errno EFBIG.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::uint64_t> Seek(std::int64_t offset, SeekDirection whence) noexcept = 0;

  /*!
   * \brief Read from the underlying 'stream'-object (e.g. a file or a string) up to the size of the given buffer.
   * \param[out] buffer Memory to safe the read content to.
   * \return A span of the part of the given buffer with the read content or a posix error.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not readable.
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOMEM or ENOBUFS.
   * \error StreamErrc::kInvalidArgument File position is invalid caused by \c errno EOVERFLOW. Or empty buffer,
   * or read size exceeds the numeric limits of std::size_t, or buffer cannot be copied because of invalid parameters.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<amsr::core::Span<vac::language::byte>> Read(
      amsr::core::Span<vac::language::byte> buffer) noexcept = 0;

  /*!
   * \brief Read from the underlying 'stream'-object (e.g. a file or a string) up to the size of the given buffer.
   * \param[out] buffer Memory to safe the read content to.
   * \return A span of the part of the given buffer with the read content or a posix error.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not readable.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno
   * ENOMEM or ENOBUFS.
   * \error StreamErrc::kInvalidArgument File position is invalid caused by \c errno EOVERFLOW. Or empty buffer,
   * or read size exceeds the numeric limits of std::size_t, or buffer cannot be copied because of invalid parameters.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<amsr::core::Span<char>> Read(amsr::core::Span<char> buffer) noexcept = 0;

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
  virtual amsr::core::Result<amsr::core::Span<char const>> Read(std::uint64_t count) noexcept = 0;

  /*!
   * \brief Reads up to \c count bytes from the stream without additional copy and provides them in a callback.
   * \details Returns the number of read characters. This number may be smaller than the requested amount of data. The
   * data provided is only guaranteed to be available until the next callback/call to a non-const member function of the
   * stream.
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
  virtual amsr::core::Result<std::uint64_t> Read(
      std::uint64_t count, vac::language::FunctionRef<void(amsr::core::StringView)> callback) noexcept = 0;

  /*!
   * \brief Reads from the stream until \c pred returns \c false for a character.
   * \details Returns a view on the internal buffer. This view may not contain all matching characters. To retrieve all
   * matching characters, the method should be called repeatedly until it returns zero bytes. The
   * data are only guaranteed to be available until the next call to a non-const member function of the stream happens.
   * \param[in] pred Predicate function to determine if a character shall still be read.
   * \return A number of bytes inside the internal buffer (zero bytes if \c pred returns false or if EOF is reached).
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not readable. Callback has to be checked for
   * FFI with respect to memory.
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno ENOMEM or
   * ENOBUFS.
   * \error StreamErrc::kInvalidArgument File position is invalid caused by \c errno EOVERFLOW.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<amsr::core::Span<char const>> ReadWhile(
      vac::language::FunctionRef<bool(char)> pred) noexcept = 0;

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
   * \error StreamErrc::kInsufficientResources Resources are insufficient. Underlying cause is one of \c errno ENOMEM or
   * ENOBUFS.
   * \error StreamErrc::kInvalidArgument File position is invalid caused by \c errno EOVERFLOW. Or no delimiters given.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  virtual amsr::core::Result<std::int64_t> ReadUntil(
      amsr::core::StringView delimiter, vac::language::FunctionRef<void(amsr::core::StringView)> callback) noexcept = 0;

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
  virtual amsr::core::Result<std::uint64_t> Find(vac::language::FunctionRef<bool(char)> pred) noexcept = 0;

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
  virtual amsr::core::Result<std::uint64_t> Find(amsr::core::StringView set) noexcept = 0;

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
  virtual amsr::core::Result<std::uint64_t> FindNot(amsr::core::StringView set) noexcept = 0;

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
  virtual amsr::core::Result<std::int64_t> Get() noexcept = 0;

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
  virtual amsr::core::Result<void> UnGet() noexcept = 0;

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
  virtual amsr::core::Result<std::int64_t> Peek() noexcept = 0;

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
  virtual bool IsEof() const noexcept = 0;

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
  virtual bool IsOpen() const noexcept = 0;
};

}  // namespace stream
}  // namespace amsr

#endif  // LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_INPUT_STREAM_H_
