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
 *        \brief   Stream wrapper providing stateful input streams.
 *
 *        \details -
 *
 *********************************************************************************************************************/
#ifndef LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_STATEFUL_INPUT_STREAM_H_
#define LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_STATEFUL_INPUT_STREAM_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/charconv/type_helper.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/core/string_view.h"
#include "amsr/iostream/input_stream.h"
#include "amsr/iostream/stateful_stream.h"
#include "amsr/iostream/types.h"
#include "vac/language/byte.h"

namespace amsr {
namespace stream {

/*!
 * \brief Template class wrapping streams and providing a stateful stream handling / API.
 * \unit amsr::stream::StatefulInputStream
 * \vprivate Product Private
 */
class StatefulInputStream final : public StatefulStream<InputStream> {
 public:
  /*!
   * \brief Inherited constructor.
   */
  using StatefulStream<InputStream>::StatefulStream;

  // StatefulInputStream is not copy constructible because a stream is not copy constructible.
  StatefulInputStream(StatefulInputStream const& stream) = delete;

  // StatefulInputStream is not copy assignable because a stream is not copy assignable.
  StatefulInputStream& operator=(StatefulInputStream const& stream) & = delete;

  /*!
   * \brief Move constructs instance of class.
   * \param[in] stream Instance of StatefulInputStream
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  StatefulInputStream(StatefulInputStream&& stream) noexcept = default;

  /*!
   * \brief Move assigns an instance of class.
   * \param[in] stream Instance of StatefulInputStream
   * \return The assigned to stream.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  StatefulInputStream& operator=(StatefulInputStream&& stream) & noexcept = default;

  /*!
   * \brief Destructor
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  ~StatefulInputStream() noexcept final = default;  // VCA_IOSTREAM_DESTRUCTOR

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
   * empty or read size exceeds the numeric limits of std::size_t.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<amsr::core::Span<vac::language::byte>> Read(amsr::core::Span<vac::language::byte> buffer) noexcept;

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
   * empty or read size exceeds the numeric limits of std::size_t.
   * \error StreamErrc::kIoError A physical I/O error has occurred caused by \c errno EIO.
   * \error StreamErrc::kUnexpected Unexpected \c errno which is not mapped to any category.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<amsr::core::Span<char>> Read(amsr::core::Span<char> buffer) noexcept;

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
  amsr::core::Result<std::uint64_t> Read(std::uint64_t count,
                                         vac::language::FunctionRef<void(amsr::core::StringView)> callback);

  /*!
   * \brief Reads from the stream until \c delimiter is encountered.
   * \details Returns the delimiter found or EOF. The views found in between are provided via \c callback.
   * The data is only guaranteed to be available until the next callback / call to a non-const member function of the
   * stream. The found delimiter is not consumed, meaning that for repeated calls to ReadUntil the file position needs
   * to be advanced by at least 1.
   * \param[in] delimiter A set of delimiters that stop reading.
   * \param[in] callback The callback that
   * is called for every view read.
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
  amsr::core::Result<std::int64_t> ReadUntil(amsr::core::StringView delimiter,
                                             vac::language::FunctionRef<void(amsr::core::StringView)> callback);

  /*!
   * \brief Get a character from the underlying stream object (e.g. a file or a string).
   * \return The character or EOF.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not readable.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Get() noexcept;

  /*!
   * \brief Makes the most recently extracted character available again.
   * \return Result of type void.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not readable.
   * \threadsafe FALSE
   * \error StreamErrc::kInvalidPosition In case current position is at the beginning of file/string.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<void> UnGet() noexcept;

  /*!
   * \brief Peeks the next character in the stream.
   * \return The peeked character.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not readable.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Peek() noexcept;

  /*!
   * \brief Checks if end-of-file is reached.
   * \return Result of type bool which is \c true, if yes, \c false, if no.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<bool> IsEof() const noexcept;

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
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::uint64_t> Find(amsr::core::StringView set) noexcept;

  /*!
   * \brief Finds the first character in the stream from the current offset (inclusive) that is not contained in set.
   * \details Moves the stream offset to the first mismatch or to EOF.
   * \param[in] set Set of characters.
   * \return Offset of the first mismatch.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not readable.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::uint64_t> FindNot(amsr::core::StringView set) noexcept;

 private:
  /*! Friend test declaration. */
  FRIEND_TEST(UT__StatefulInputStream__Test, GetStream);
  /*! Friend test declaration. */
  FRIEND_TEST(UT__StatefulInputStream__Test, OperatorStar);
  /*! Friend test declaration. */
  FRIEND_TEST(UT__StatefulInputStream__Test, OperatorArrow);
};

}  // namespace stream
}  // namespace amsr

#endif  // LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_STATEFUL_INPUT_STREAM_H_
