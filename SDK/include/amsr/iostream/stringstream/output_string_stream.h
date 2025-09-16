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
 *        \brief  Implements an output string stream.
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_STRINGSTREAM_OUTPUT_STRING_STREAM_H_
#define LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_STRINGSTREAM_OUTPUT_STRING_STREAM_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cmath>
#include <cstring>  // for std memcpy
#include <limits>
#include <string>

#include "amsr/charconv/charconv_error_domain.h"
#include "amsr/charconv/internal/safe_conversion.h"
#include "amsr/charconv/to_textual.h"
#include "amsr/charconv/type_helper.h"
#include "amsr/core/array.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/core/string.h"
#include "amsr/core/string_view.h"
#include "amsr/iostream/binary_prefix.h"
#include "amsr/iostream/hex_prefix.h"
#include "amsr/iostream/io_format.h"
#include "amsr/iostream/output_stream.h"
#include "amsr/iostream/stream_error_domain.h"
#include "amsr/iostream/stringstream/string_stream_helper.h"
#include "amsr/iostream/utility.h"
#include "vac/container/c_string_view.h"
#include "vac/container/string_literals.h"
#include "vac/language/byte.h"
#include "vac/language/function_ref.h"

namespace amsr {
namespace stream {
namespace stringstream {

// VECTOR NC Metric-OO.WMC.One: MD_IoStream_Metric-OO.WMC.One_interface
/*!
 * \brief Implements an output string stream.
 * \unit amsr::stream::stringstream::OutputStringStream
 * \vprivate Product Private
 */
class OutputStringStream : public OutputStream {
 public:
  /*!
   * \brief Constructs instance of class.
   * \param[in] span A span.
   * \context ANY
   * \pre Aborts if \c span is empty.
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  explicit OutputStringStream(amsr::core::Span<char> span) noexcept;

  // OutputStringStream is not copy constructible because base class OutputStream is not copy constructible.
  OutputStringStream(OutputStringStream const& output_string_stream) = delete;

  // OutputStringStream is not copy assignable because base class OutputStream is not copy assignable.
  OutputStringStream& operator=(OutputStringStream const& output_string_stream) & = delete;

  /*!
   * \brief Move constructs instance of class.
   * \param[in] output_string_stream Instance of OutputStringStream
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  OutputStringStream(OutputStringStream&& output_string_stream) noexcept = default;

  /*!
   * \brief Move assigns instance of class.
   * \param[in] output_string_stream Instance of OutputStringStream.
   * \return Moved OutputStringStream.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  OutputStringStream& operator=(OutputStringStream&& output_string_stream) & noexcept = default;

  /*!
   * \brief Destructs instance of class
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  ~OutputStringStream() noexcept override = default;  // VCA_IOSTREAM_DESTRUCTOR

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
   * \brief Write a character to the stream.
   * \param[in] data A character.
   * \return The number of bytes written
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Stream buffer too small for \c data.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(char data) noexcept override;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type bool.
   * \return The number of bytes written.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Stream buffer too small for \c data or unable to write number.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \steady UNKNOWN
   * \vprivate Product Private
   */
  amsr::core::Result<std::int64_t> Write(bool const& data) noexcept override;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type std::int8_t.
   * \return The number of bytes written.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Stream buffer too small for \c data or unable to write number.
   * \error StreamErrc::kInvalidArgument Value out of range of result type.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(std::int8_t const& data) noexcept override;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type std::int8_t.
   * \param[in] base Base value of the arithmetic value to be written
   * \return The number of bytes written.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Stream buffer too small for \c data or unable to write number.
   * \error StreamErrc::kInvalidArgument Value out of range of result type.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(std::int8_t const& data, Base base) noexcept override;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type std::int16_t.
   * \return The number of bytes written.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Stream buffer too small for \c data or unable to write number.
   * \error StreamErrc::kInvalidArgument Value out of range of result type.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(std::int16_t const& data) noexcept override;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type std::int16_t.
   * \param[in] base Base value of the arithmetic value to be written
   * \return The number of bytes written.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Stream buffer too small for \c data or unable to write number.
   * \error StreamErrc::kInvalidArgument Value out of range of result type.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(std::int16_t const& data, Base base) noexcept override;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type std::int32_t.
   * \return The number of bytes written.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Stream buffer too small for \c data or unable to write number.
   * \error StreamErrc::kInvalidArgument Value out of range of result type.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(std::int32_t const& data) noexcept override;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type std::int32_t.
   * \param[in] base Base value of the arithmetic value to be written
   * \return The number of bytes written.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Stream buffer too small for \c data or unable to write number.
   * \error StreamErrc::kInvalidArgument Value out of range of result type.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(std::int32_t const& data, Base base) noexcept override;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type std::int64_t.
   * \return The number of bytes written.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Stream buffer too small for \c data or unable to write number.
   * \error StreamErrc::kInvalidArgument Value out of range of result type.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(std::int64_t const& data) noexcept override;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type std::int64_t.
   * \param[in] base Base value of the arithmetic value to be written
   * \return The number of bytes written.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Stream buffer too small for \c data or unable to write number.
   * \error StreamErrc::kInvalidArgument Value out of range of result type.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(std::int64_t const& data, Base base) noexcept override;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type std::uint8_t.
   * \return The number of bytes written.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Stream buffer too small for \c data or unable to write number.
   * \error StreamErrc::kInvalidArgument Value out of range of result type.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(std::uint8_t const& data) noexcept override;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type std::uint8_t.
   * \param[in] base Base value of the arithmetic value to be written
   * \return The number of bytes written.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Stream buffer too small for \c data or unable to write number.
   * \error StreamErrc::kInvalidArgument Value out of range of result type.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(std::uint8_t const& data, Base base) noexcept override;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type std::uint16_t.
   * \return The number of bytes written.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Stream buffer too small for \c data or unable to write number.
   * \error StreamErrc::kInvalidArgument Value out of range of result type.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(std::uint16_t const& data) noexcept override;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type std::uint16_t.
   * \param[in] base Base value of the arithmetic value to be written
   * \return The number of bytes written.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Stream buffer too small for \c data or unable to write number.
   * \error StreamErrc::kInvalidArgument Value out of range of result type.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(std::uint16_t const& data, Base base) noexcept override;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type std::uint32_t.
   * \return The number of bytes written.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Stream buffer too small for \c data or unable to write number.
   * \error StreamErrc::kInvalidArgument Value out of range of result type.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(std::uint32_t const& data) noexcept override;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type std::uint32_t.
   * \param[in] base Base value of the arithmetic value to be written
   * \return The number of bytes written.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Stream buffer too small for \c data or unable to write number.
   * \error StreamErrc::kInvalidArgument Value out of range of result type.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(std::uint32_t const& data, Base base) noexcept override;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type std::uint64_t.
   * \return The number of bytes written.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Stream buffer too small for \c data or unable to write number.
   * \error StreamErrc::kInvalidArgument Value out of range of result type.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(std::uint64_t const& data) noexcept override;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type std::uint64_t.
   * \param[in] base Base value of the arithmetic value to be written
   * \return The number of bytes written.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Stream buffer too small for \c data or unable to write number.
   * \error StreamErrc::kInvalidArgument Value out of range of result type.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(std::uint64_t const& data, Base base) noexcept override;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type float.
   * \return The number of bytes written.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Stream buffer too small for \c data or unable to write number.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(float const& data) noexcept override;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data An arithmetic value of type double.
   * \return The number of bytes written.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Stream buffer too small for \c data or unable to write number.
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
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Stream buffer too small for \c data.
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
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Stream buffer too small for \c data.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(vac::container::CStringView data) noexcept override;

  /*!
   * \brief Write a span of characters to the string stream.
   * \param[in] data The span to write.
   * \return The number of bytes written.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Stream buffer too small for \c data.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(amsr::core::Span<char const> data) noexcept override;

  /*!
   * \brief Write a span of bytes to the string stream.
   * \param[in] data The span to write.
   * \return The number of bytes written.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Stream buffer too small for \c data.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(amsr::core::Span<vac::language::byte const> data) noexcept override;

  /*!
   * \brief Write a number of characters to the string stream by a provided callback.
   * \details The callback is called maximum once.
   * \warning The callback might not be called at all.
   * \param[in] max_count Maximum number of characters to be written. If size-of-internal-buffer is smaller than
   * max_count, maximum size-of-internal-buffer bytes can be written.
   * \param[in] callback The callback that gets access to the internal buffer. The return value indicates the number of
   * bytes written into the buffer.
   * \return The actual number of bytes written.
   * \context ANY
   * \pre Callback has to be checked for FFI with respect to memory.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::uint64_t> Write(
      std::uint64_t max_count,
      vac::language::FunctionRef<std::uint64_t(amsr::core::Span<char>)> callback) noexcept override;

  /*!
   * \brief Write data to the string stream by a provided callback.
   * \details The callback is only called once. Maximum size-of-internal-buffer bytes can be written.
   * \param[in] callback The callback that gets access to the internal buffer. The return value indicates the number of
   * bytes written into the buffer.
   * \return The actual number of bytes written, or abort if size of data to be written is bigger than buffer.
   * \context ANY
   * \pre Callback has to be checked for FFI with respect to memory.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::uint64_t> Write(
      vac::language::FunctionRef<std::uint64_t(amsr::core::Span<char>)> callback) noexcept override;

  /*!
   * \brief Writes a byte array in hexadecimal or binary base to the stream.
   * \param[in] data A span for a the byte array.
   * \param[in] base Base value of the arithmetic value to be written
   * \return The number of bytes written.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Stream buffer too small for \c data or unable to write number.
   * \error StreamErrc::kInvalidArgument Base is kDecimal.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<std::int64_t> Write(amsr::core::Span<std::uint8_t const> data, Base base) noexcept override;

  /*!
   * \brief Returns a string view with the data written so far.
   * \returns On success a string view with the data written so far.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \trace CREQ-Iostream-OutputStringStream-AsStringView
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::StringView AsStringView() const;

  /*!
   * \brief Inserts character into a string stream.
   * \param[in] data The character to be inserted.
   * \return Result of type void.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Stream buffer too small for character.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<void> Put(char data) noexcept override;

  /*!
   * \brief Resets the stream to start position.
   * \details This function is needed by vajson.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \trace CREQ-Iostream-OutputStringStream-Reset
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  void Reset();

  /*!
   * \brief Synchronizes written data.
   * \details The current implementation does not need synchronization for OutputStringStreams, so result always has
   * a value.
   * \return Result with value void.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<void> Sync() noexcept override;

  /*!
   * \brief Flushes written data.
   * \details The current implementation does not need flushing for OutputStringStreams, so result always has
   * a value.
   * \return Result with value void.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Result<void> Flush() noexcept override;

  /*!
   * \brief Get the number of remaining characters in buffer.
   * \return The number of characters left.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  amsr::core::Span<char>::index_type GetRemainingBufferSize() const noexcept override;

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
   * \brief Write an integral value to the stream.
   * \tparam Number An integral data type.
   * \param[in] data An integral value.
   * \param[in] base The numbers base (binary, dec or hex)
   * \return The number of bytes written.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Stream buffer too small for \c data or unable to write number.
   * \error StreamErrc::kInvalidArgument Value out of range of result type.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * - If value should be prefixed:
   *   - Call WritePrefixedHex if base is hex.
   * - Else:
   *   - Write data by calling to_textual.
   * \endinternal
   */
  template <typename Number,
            std::enable_if_t<std::is_integral<Number>::value && (!std::is_same<Number, bool>::value), std::int32_t> = 0>
  amsr::core::Result<std::int64_t> Write(Number data, charconv::NumberBase base) noexcept {
    amsr::core::Result<std::int64_t> result{amsr::core::Result<std::int64_t>::FromValue(0)};
    if (ShowBasePrefix() && (base == charconv::NumberBase::kHex)) {  // VCA_IOSTREAM_NO_NULL_POINTER
      result = WritePrefixedHex(data);
    } else if (ShowBasePrefix() && (base == charconv::NumberBase::kBinary)) {  // VCA_IOSTREAM_NO_NULL_POINTER
      result = WritePrefixedBinary(data);
    } else {
      result = WriteByCallingToTextual(data, base);
    }

    return result;
  }

  /*!
   * \brief Copies a memory segment of a certain size starting at the current write position of the output stream.
   * \tparam T a byte type (char or vac::byte)
   * \param[in] data The span to copy from.
   * \return The number of bytes written.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Stream buffer too small for \c data.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * #01 If data is empty return 0.
   * #05 If std::memcpy can be used:
   *     #10 If internal buffer is large enough:
   *         #20 Write span to stream.
   *         #30 Update current position and set return value.
   *     #40 Otherwise return error.
   * \endinternal
   */
  template <typename T, std::enable_if_t<(std::is_same<vac::language::byte const, T>::value) ||
                                             (std::is_same<char const, T>::value),
                                         std::int32_t> = 0>
  amsr::core::Result<std::int64_t> WriteByMemCpy(amsr::core::Span<T> data) noexcept {
    using vac::container::literals::operator""_sv;
    amsr::core::Result<std::int64_t> result{StreamErrc::kInvalidArgument};
    amsr::core::Span<char const>::size_type const size{data.size()};
    amsr::core::Span<char> const destination{cur_pos_.pos, last_.pos};
    if (data.empty()) {  // #01
      result.EmplaceValue(static_cast<std::int64_t>(0));
    } else if (FulfillsMemcpyPrerequisites<char>(destination, data.begin(), data.end(), size)) {  // #05
      if (IsRangeLargeEnough(cur_pos_, last_, size)) {                                            // 10
        std::memcpy(cur_pos_.pos, data.data(), size);  // VCA_IOSTREAM_STD_MEMCPY

        // #30
        std::advance(cur_pos_.pos,
                     amsr::charconv::ConvertOrAbort<std::ptrdiff_t>(size));  // within bounds because of range check
        result.EmplaceValue(amsr::charconv::ConvertOrAbort<std::int64_t>(size));
      } else {  // #40
        result.EmplaceError(
            StreamErrc::kInsufficientResources,
            amsr::core::String("OutputStringStream::Write: Internal buffer is too small for data to be written or data "
                               "cannot be copied to internal buffer.")
                .c_str());
      }
    } else {
      // intentionally left empty
    }

    return result;
  }

  /*!
   * \brief Writes a boolean value starting at the current write position of the output stream.
   * \param[in] data A boolean value.
   * \return The number of bytes written.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Stream buffer too small for \c data or unable to write number.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * #10 Convert boolean value to textual representation.
   * #20 Update stream position and set return value if successful.
   * \endinternal
   */
  amsr::core::Result<std::int64_t> WriteByCallingToTextual(bool const& data) noexcept {
    using vac::container::literals::operator""_sv;
    BufferIterator const cur_pos_old{cur_pos_};
    amsr::core::Span<char> const buffer{cur_pos_.pos, static_cast<std::size_t>(std::distance(cur_pos_.pos, last_.pos))};

    // #10
    amsr::core::Result<std::int64_t> result{amsr::core::Result<std::int64_t>::FromError(
        StreamErrc::kInsufficientResources, "OutputStringStream::WriteByCallingToTextual"_sv.c_str())};
    charconv::ToTextual<bool> to_textual{buffer, data};
    amsr::core::Result<amsr::core::Span<char>> const conversion_result{to_textual.Convert()};

    if (conversion_result.HasValue()) {                // #20
      cur_pos_.pos = conversion_result.Value().end();  // within bounds because result of ToTextual (ASIL)
      result.EmplaceValue(std::distance(cur_pos_old.pos, cur_pos_.pos));
    }

    return result;
  }

  /*!
   * \brief Writes an integral value starting at the current write position of the output stream.
   * \tparam Number An integral data type.
   * \param[in] data An integral value.
   * \param[in] base Base value of the arithmetic value to be written.
   * \return The number of bytes written.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Stream buffer too small for \c data or unable to write number.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * #10 Convert integral number to textual representation.
   * #20 Update stream position and set return value if successful.
   * \endinternal
   */
  template <typename Number,
            std::enable_if_t<std::is_integral<Number>::value && (!std::is_same<Number, bool>::value), std::int32_t> = 0>
  amsr::core::Result<std::int64_t> WriteByCallingToTextual(Number data, charconv::NumberBase base) noexcept {
    using vac::container::literals::operator""_sv;
    BufferIterator const cur_pos_old{cur_pos_};
    amsr::core::Span<char> const buffer{cur_pos_.pos, static_cast<std::size_t>(std::distance(cur_pos_.pos, last_.pos))};

    // #10
    amsr::core::Result<std::int64_t> result{amsr::core::Result<std::int64_t>::FromError(
        StreamErrc::kInsufficientResources,
        "OutputStringStream::WriteByCallingToTextual: Internal buffer is too small for data to be written."_sv
            .c_str())};
    charconv::ToTextual<Number> to_textual{buffer, data};
    amsr::charconv::ExtendedFormat const format{GetCharconvFormat()};  // VCA_IOSTREAM_NO_NULL_POINTER
    amsr::core::Result<amsr::core::Span<char>> const conversion_result{to_textual.Convert(base, format)};

    if (conversion_result.HasValue()) {                // #20
      cur_pos_.pos = conversion_result.Value().end();  // within bounds because result of ToTextual (ASIL)
      result.EmplaceValue(std::distance(cur_pos_old.pos, cur_pos_.pos));
    }

    return result;
  }

  /*!
   * \brief Writes a floating point value starting at the current write position of the output stream.
   * \tparam FP A floating point data type.
   * \param[in] data A floating point value.
   * \return The number of bytes written
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Stream buffer too small for \c data or unable to write number.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * #10 Convert floating point number to textual representation
   * #20 Write to file if conversion succeeded
   * \endinternal
   */
  template <typename FP, std::enable_if_t<std::is_floating_point<FP>::value, std::int32_t> = 0>
  amsr::core::Result<std::int64_t> WriteByCallingToTextual(FP const data) noexcept {
    using vac::container::literals::operator""_sv;
    amsr::core::Result<std::int64_t> result{amsr::core::Result<std::int64_t>::FromError(
        StreamErrc::kInsufficientResources,
        "OutputStringStream::WriteByCallingToTextual: Internal buffer is too small for data to be written."_sv
            .c_str())};
    amsr::core::Span<char> const buffer{cur_pos_.pos, static_cast<std::size_t>(std::distance(cur_pos_.pos, last_.pos))};
    BufferIterator const cur_pos_old{cur_pos_};

    // 10 Convert floating point number to textual representation
    charconv::ToTextual<FP> to_textual{buffer, data};
    amsr::charconv::FPFormat const fp_format{GetCharconvFloatingPointFormat()};  // VCA_IOSTREAM_NO_NULL_POINTER
    amsr::core::Result<amsr::core::Span<char>> const conversion_result{to_textual.Convert(fp_format)};

    if (conversion_result.HasValue()) {
      // #20 Update stream position and set return value if successful
      cur_pos_.pos = conversion_result.Value().end();  // within bounds because result of ToTextual (ASIL)
      result.EmplaceValue(std::distance(cur_pos_old.pos, cur_pos_.pos));
    }
    return result;
  }

  /*!
   * \brief Gets the total amount of characters written.
   * \param[in] chars_written Amount of characters written.
   * \param[in] add_chars_written Amount of additional characters written.
   * \return chars_written + add_chars_written
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInvalidArgument Value out of range of result type.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * - If sum of characters written is not out of range return sum.
   * - Otherwise return error.
   * \endinternal
   */
  static amsr::core::Result<std::int64_t> GetTotalAmountOfCharsWritten(std::int64_t chars_written,
                                                                       std::int64_t add_chars_written) noexcept {
    amsr::core::Result<std::int64_t> result_data{StreamErrc::kInvalidArgument};
    if (chars_written <= (std::numeric_limits<std::int64_t>::max() - add_chars_written)) {
      std::int64_t const bytes_written{chars_written + add_chars_written};
      result_data.EmplaceValue(bytes_written);
    }
    return result_data;
  }

  /*!
   * \brief Writes an integral value in hexadecimal format to the stream, prefixed with 0x.
   * \tparam Int An integral data type.
   * \param[in] data An integral value.
   * \return The number of bytes written
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Stream buffer too small for \c data or unable to write number.
   * \error StreamErrc::kInvalidArgument Value out of range of result type.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * #10 Write prefix 0x.
   * #20 Write data as hex.
   * #40 Return total amount of characters written.
   * In case any of the operations above fail:
   * - Skip remaining steps.
   * - Return error.
   * \endinternal
   */
  template <typename Number, std::enable_if_t<std::is_integral<Number>::value, std::int32_t> = 0>
  amsr::core::Result<std::int64_t> WritePrefixedHex(Number data) noexcept {
    // #10
    amsr::core::StringView const view{HexPrefix::GetLiteral()};
    return Write(view)  // VCA_IOSTREAM_NO_NULL_POINTER
        .AndThen([this, &data](std::int64_t) {
          // #20
          return WriteByCallingToTextual(data, charconv::NumberBase::kHex);
        })
        .AndThen([](std::int64_t chars_written) {  // COV_IOSTREAM_OnlyValue_2
          return GetTotalAmountOfCharsWritten(chars_written,
                                              amsr::charconv::Convert<std::int64_t>(HexPrefix::kHexPrefixSize));
        });
  }

  /*!
   * \brief Writes an integral value in binary format to the stream, prefixed with 0b.
   * \tparam Int An integral data type.
   * \param[in] data An integral value.
   * \return The number of bytes written
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error StreamErrc::kInsufficientResources Stream buffer too small for \c data or unable to write number.
   * \error StreamErrc::kInvalidArgument Value out of range of result type.
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \steady UNKNOWN
   * \vprivate Component Private
   *
   * \internal
   * #10 Write prefix 0b.
   * #20 Write data as binary.
   * #40 Return total number of characters written.
   * In case any of the operations above fail:
   * - Skip remaining steps.
   * - Return error.
   * \endinternal
   */
  template <typename Number, std::enable_if_t<std::is_integral<Number>::value, std::int32_t> = 0>
  amsr::core::Result<std::int64_t> WritePrefixedBinary(Number data) noexcept {
    // #10
    amsr::core::StringView const view{BinaryPrefix::GetLiteral()};
    return Write(view)  // VCA_IOSTREAM_NO_NULL_POINTER
        .AndThen([this, &data](std::int64_t) {
          // #20
          return WriteByCallingToTextual(data, charconv::NumberBase::kBinary);
        })
        .AndThen([](std::int64_t chars_written) {  // COV_IOSTREAM_OnlyValue_2
          return GetTotalAmountOfCharsWritten(chars_written,
                                              amsr::charconv::Convert<std::int64_t>(BinaryPrefix::kBinaryPrefixSize));
        });
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
  FRIEND_TEST(UT__OutputStringStream__Other, GetTotalAmountOfCharsWritten);

  /*! Friend test declaration. */
  FRIEND_TEST(UT__OutputStringStream__Other, WriteCharConstPointer__Success);

  /*! Friend test declaration. */
  FRIEND_TEST(CT__IoStream__OutputStringStreamCReq, WriteCharConstPointer);

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoStream_A11.3.1_friend_class
  /*! Friend test declaration. */
  template <typename T>
  FRIEND_TEST(CT__IoStream__StatefulOutputFileStreamTypesCReq, WriteCharConstPointer);

  /*! start of memory block */
  BufferIterator first_;

  /*! current position for next write operation */
  BufferIterator cur_pos_;

  /*! end of memory block (excluded) */
  BufferIterator last_;
};

}  // namespace stringstream
}  // namespace stream
}  // namespace amsr

#endif  // LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_STRINGSTREAM_OUTPUT_STRING_STREAM_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_IOSTREAM_OnlyValue_2
//   \ACCEPT  xx
//   \REASON  In this part of the lambda expression, the called function GetTotalAmountOfCharsWritten always returns a
//   value as the error case cannot be reached (the number of characters written is always smaller than numeric_limits).
//   Therefore, the error case will never happen in this part. The value case is covered e.g. by
//   (UT__OutputStringStream__TypedInt, Write__PrefixedHex__Success)
// COV_JUSTIFICATION_END
