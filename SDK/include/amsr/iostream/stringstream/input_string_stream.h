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
 *        \brief  Implements an input string stream.
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_STRINGSTREAM_INPUT_STRING_STREAM_H_
#define LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_STRINGSTREAM_INPUT_STRING_STREAM_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/charconv/internal/safe_conversion.h"
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "amsr/iostream/input_stream.h"
#include "amsr/iostream/stringstream/string_stream_helper.h"
#include "vac/language/byte.h"

namespace amsr {
namespace stream {
namespace stringstream {

// VECTOR NC Metric-OO.WMC.One: MD_IoStream_Metric-OO.WMC.One_input_string_stream
/*!
 * \brief Implements an input string stream.
 * \unit amsr::stream::stringstream::InputStringStream
 * \vprivate Product Private
 */
class InputStringStream : public amsr::stream::InputStream {
 public:
  /*!
   * \brief Constructs instance of class.
   * \details An empty span \c span is allowed because it is necessary for some use cases.
   * \param[in] span A span used as a memory block.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  explicit InputStringStream(amsr::core::Span<char const> span) noexcept;

  // InputStringStream is not copy constructible because base class InputStream is not copy constructible.
  InputStringStream(InputStringStream const& input_string_stream) = delete;

  // InputStringStream is not copy assignable because base class InputStream is not copy assignable.
  InputStringStream& operator=(InputStringStream const& input_string_stream) & = delete;

  /*!
   * \brief Move constructs instance of class.
   * \param[in] input_string_stream Instance of InputStringStream
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  InputStringStream(InputStringStream&& input_string_stream) noexcept = default;

  /*!
   * \brief Move assigns instance of class.
   * \param[in] input_string_stream Instance of InputStringStream
   * \return Moved InputStringStream
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  InputStringStream& operator=(InputStringStream&& input_string_stream) & noexcept = default;

  /*!
   * \brief Destructs instance.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  ~InputStringStream() noexcept override = default;  // VCA_IOSTREAM_DESTRUCTOR

  /*!
   * \brief Returns the current string position.
   * \return The current string position, measured in bytes from the beginning of the string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInvalidPosition Position in stream cannot be determined.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::uint64_t> Tell() const noexcept override;

  /*!
   * \brief Changes the position of the string stream.
   * \param[in] offset Offset to change the position with.
   * \param[in] whence Specifies how the offset is interpreted:
   * - kBeg: offset is a count of characters from the beginning of the string.
   * - kEnd: offset is a count of characters from the end of the string.
   * - kCur: offset is a count of characters from the current string position.
   * \return The updated position in the string, measured in bytes from the beginning of the string.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInvalidArgument The whence argument is not valid, or the resulting offset is not
   * valid.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::uint64_t> Seek(std::int64_t offset, SeekDirection whence) noexcept override;

  /*!
   * \brief Reads up to size of buffer bytes into the span buffer.
   * \param[out] buffer A span to store the read bytes.
   * \return Result with number of bytes really read (could be smaller than the size of buffer in some cases).
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInvalidArgument If buffer is empty.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<amsr::core::Span<vac::language::byte>> Read(
      amsr::core::Span<vac::language::byte> buffer) noexcept override;

  /*!
   * \brief Reads up to size of buffer chars into the span buffer.
   * \param[out] buffer A span to store the read bytes.
   * \return Result with number of chars really read (could be smaller than the size of buffer in some cases).
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInvalidArgument If buffer is empty.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<amsr::core::Span<char>> Read(amsr::core::Span<char> buffer) noexcept override;

  /*!
   * \brief Reads up to \c count bytes from the stream without additional copy.
   * \details Returns a view on the internal buffer. This view may be smaller than the requested amount of data. The
   * data is only guaranteed to be available until the next call to a non-const member function of the stream.
   * \param[in] count Maximum number of bytes to be read.
   * \return A span of at most size \c count (empty span if EOF is reached).
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<amsr::core::Span<char const>> Read(std::uint64_t count) noexcept final;

  /*!
   * \brief Reads up to \c count bytes from the file without additional copy and provides them in a callback.
   * \details Returns the number of read characters. This number may be smaller than the requested amount of data. The
   * data provided is only guaranteed to be available until the next callback/call to a non-const member function of the
   * stream.
   * \param[in] count Maximum number of bytes to be read.
   * \param[in] callback The callback that is called on every buffer view.
   * \return Number of read characters.
   * \context ANY
   * \pre Callback has to be checked for FFI with respect to memory.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::uint64_t> Read(
      std::uint64_t count, vac::language::FunctionRef<void(amsr::core::StringView)> callback) noexcept final;

  /*!
   * \brief Reads two times size of buffer bytes from the internal buffer which are interpreted as hex-encoded byte
   * array.
   * \param[out] buffer Holds the decoded byte array.
   * \param[in] error_on_odd_char_num Whether an odd number of characters left from the current iterator position in the
   * internal buffer will result in an error. Otherwise the remaining buffer is treated as if preceded by a 0x30-byte
   * (Ascii zero-character).
   * \param[in] error_on_empty_input Whether no characters left from the current iterator position in the internal
   * buffer will result in an error. Otherwise a view of an empty buffer will be returned.
   * \return A span of the decoded data.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInvalidArgument If \c error_on_odd_char_num is true and the internal buffer has odd number of
   * elements left, if \c error_on_empty_input is true and the internal buffer has zero elements left, if output buffer
   * is too small or hex data cannot be decoded.
   * \reentrant FALSE
   * \spec requires true;
   * \endspec
   * \trace CREQ-Iostream-InputStringStream-ReadEncodedByteArray
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<amsr::core::Span<std::uint8_t>> ReadEncodedByteArray(
      amsr::core::Span<std::uint8_t> buffer, bool error_on_odd_char_num = true,
      bool error_on_empty_input = true) const noexcept;

  /*!
   * \brief Reads from the stream until \c pred returns \c false for a character.
   * \details Returns a view on the internal buffer. This view may not contain all matching characters. To retrieve all
   * matching characters, the method should be called repeatedly until it returns zero bytes. The
   * data are only guaranteed to be available until the next call to a non-const member function of the stream happens.
   * \param[in] pred Predicate function to determine if a character shall still be read.
   * \return A number of bytes inside the internal buffer (zero bytes if \c pred returns false or if EOF is reached).
   * \context ANY
   * \pre Callback has to be checked for FFI with respect to memory.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<amsr::core::Span<char const>> ReadWhile(
      vac::language::FunctionRef<bool(char)> pred) noexcept final;

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
   * \pre Callback has to be checked for FFI with respect to memory.
   * \threadsafe FALSE
   * \error StreamErrc::kInvalidArgument No delimiters given.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> ReadUntil(
      amsr::core::StringView delimiter,
      vac::language::FunctionRef<void(amsr::core::StringView)> callback) noexcept final;

  /*!
   * \brief Finds the first character from the current offset (inclusive) in the stream matching the predicate.
   * \details Moves the stream offset to the first match or to EOF.
   * \param[in] pred Function determining if a character is a match.
   * \return Offset of the found character.
   * \context ANY
   * \pre Callback has to be checked for FFI with respect to memory.
   * \threadsafe FALSE
   * \error StreamErrc::kEndOfFile If the character was not found.
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::uint64_t> Find(vac::language::FunctionRef<bool(char)> pred) noexcept final;

  /*!
   * \brief Finds the first character from the current offset (inclusive) in the stream that is contained in the set.
   * \details Moves the stream offset to the first match or to EOF.
   * \param[in] set Set of characters to look for.
   * \return Offset of the found character.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kEndOfFile If the character was not found.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::uint64_t> Find(amsr::core::StringView set) noexcept final;

  /*!
   * \brief Finds the first character in the stream from the current offset (inclusive) that is not contained in set.
   * \details Moves the stream offset to the first mismatch or to EOF.
   * \param[in] set Set of characters.
   * \return Offset of the first mismatch.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInvalidArgument No delimiters given.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::uint64_t> FindNot(amsr::core::StringView set) noexcept final;

  /*!
   * \brief Gets one character from the current read position.
   * \return The character or EOF.
   * \context ANY
   * \pre -
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
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInvalidPosition In case current position is \c first_.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<void> UnGet() noexcept override;

  /*!
   * \brief Peeks one character from the current read position without altering the position.
   * \return The peeked character.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Peek() noexcept override;

  /*!
   * \brief Checks if EOF is reached.
   * \return Result of type bool which is \c true, if yes, \c false, if no. Result always has a value.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  bool IsEof() const noexcept override;

  /*!
   * \brief Return always true.
   * \return For string streams always true is returned.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  bool IsOpen() const noexcept override;

 private:
  /*!
   * \brief Reads up to size of buffer bytes into the span buffer.
   * \param[out] buffer A span to store the read bytes.
   * \return Result with number of bytes really read (could be smaller than the size of buffer in some cases).
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInvalidArgument If buffer is empty.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \steady UNKNOWN
   * \vprivate Product Private
   *
   * \internal
   * #10 Adjust number of bytes to read if remaining internal buffer is smaller than buffer.
   * #15 If std::memcpy can be used:
   *     #20 Copy data to buffer.
   *     #30 Set return value and update current position.
   *  #40 Otherwise, set error.
   * \endinternal
   */
  template <typename T>
  auto ReadFromInternalBuffer(amsr::core::Span<T> buffer) noexcept -> amsr::core::Result<amsr::core::Span<T>> {
    amsr::core::Result<amsr::core::Span<T>> result{StreamErrc::kUnexpected};

    // #10 Adjust number of bytes to read if remaining internal buffer is smaller than buf.
    std::size_t read_size{buffer.size()};
    if (!IsRangeLargeEnough(cur_pos_, last_, read_size)) {
      read_size = amsr::charconv::ConvertOrAbort<std::size_t>(std::distance(cur_pos_.pos, last_.pos));
    }

    if (FulfillsMemcpyPrerequisites<T>(buffer, cur_pos_.pos, last_.pos, read_size)) {  // #15
      // #20
      std::memcpy(buffer.data(), cur_pos_.pos, read_size);  // VCA_IOSTREAM_STD_MEMCPY

      // #30
      result.EmplaceValue(buffer.subspan(0, read_size));
      std::advance(cur_pos_.pos, read_size);  // within bounds because of range check
    } else {                                  // #40
      result.EmplaceError(
          StreamErrc::kInvalidArgument,
          amsr::core::String("InputStringStream::Read(amsr::core::Span<T>): Data cannot be copied to output buffer.")
              .c_str());
    }

    return result;
  }

  /*! Friend test declaration. */
  FRIEND_TEST(UT__InputStringStream, ConvertToNumber__InvalidData);

  /*! start of memory block */
  ConstBufferIterator first_;

  /*! current position for next write operation */
  ConstBufferIterator cur_pos_;

  /*! end of memory block (excluded) */
  ConstBufferIterator last_;
};

}  // namespace stringstream
}  // namespace stream
}  // namespace amsr

#endif  // LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_STRINGSTREAM_INPUT_STRING_STREAM_H_
