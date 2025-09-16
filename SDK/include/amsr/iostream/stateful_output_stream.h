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
 *        \brief   Stream wrapper providing stateful output streams.
 *
 *        \details -
 *
 *********************************************************************************************************************/
#ifndef LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_STATEFUL_OUTPUT_STREAM_H_
#define LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_STATEFUL_OUTPUT_STREAM_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/charconv/type_helper.h"
#include "amsr/core/abort.h"
#include "amsr/core/span.h"
#include "amsr/core/string.h"
#include "amsr/core/string_view.h"
#include "amsr/iostream/output_stream.h"
#include "amsr/iostream/stateful_stream.h"
#include "amsr/iostream/types.h"
#include "vac/container/c_string_view.h"
#include "vac/language/byte.h"
#include "vac/language/function_ref.h"
#include "vac/testing/test_adapter.h"

namespace amsr {
namespace stream {

// VECTOR NC Metric-OO.WMC.One: MD_IoStream_Metric-OO.WMC.One_interface
/*!
 * \brief OutputStream wrapper providing a stateful API.
 * \unit amsr::stream::StatefulOutputStream
 * \vprivate Product Private
 */
class StatefulOutputStream final : public StatefulStream<OutputStream> {
 public:
  /*!
   * \brief Inherited constructor.
   */
  using StatefulStream<OutputStream>::StatefulStream;

  // StatefulOutputStream is not copy constructible because a stream is not copy constructible.
  StatefulOutputStream(StatefulOutputStream const& stream) = delete;

  // StatefulOutputStream is not copy assignable because a stream is not copy assignable.
  StatefulOutputStream& operator=(StatefulOutputStream const& stream) & = delete;

  /*!
   * \brief Move constructs instance of class.
   * \param[in] stream Instance of StatefulOutputStream
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  StatefulOutputStream(StatefulOutputStream&& stream) noexcept = default;

  /*!
   * \brief Move assigns instance of class.
   * \param[in] stream Instance of StatefulOutputStream
   * \return The moved stream.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  StatefulOutputStream& operator=(StatefulOutputStream&& stream) & noexcept = default;

  /*!
   * \brief Destructs instance.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  ~StatefulOutputStream() noexcept final = default;  // VCA_IOSTREAM_DESTRUCTOR

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data A number of type bool.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  void Write(bool const& data) noexcept;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data A number of type std::int8_t.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  void Write(std::int8_t const& data) noexcept;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data A number of type std::int8_t.
   * \param[in] base Base value of the arithmetic value to be written
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  void Write(std::int8_t const& data, Base base) noexcept;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data A number of type std::int16_t.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  void Write(std::int16_t const& data) noexcept;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data A number of type std::int16_t.
   * \param[in] base Base value of the arithmetic value to be written
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  void Write(std::int16_t const& data, Base base) noexcept;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data A number of type std::int32_t.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  void Write(std::int32_t const& data) noexcept;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data A number of type std::int32_t.
   * \param[in] base Base value of the arithmetic value to be written
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  void Write(std::int32_t const& data, Base base) noexcept;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data A number of type std::int64_t.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  void Write(std::int64_t const& data) noexcept;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data A number of type std::int64_t.
   * \param[in] base Base value of the arithmetic value to be written
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  void Write(std::int64_t const& data, Base base) noexcept;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data A number of type std::uint8_t.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  void Write(std::uint8_t const& data) noexcept;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data A number of type std::uint8_t.
   * \param[in] base Base value of the arithmetic value to be written
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  void Write(std::uint8_t const& data, Base base) noexcept;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data A number of type std::uint16_t.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  void Write(std::uint16_t const& data) noexcept;
  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data A number of type std::uint16_t.
   * \param[in] base Base value of the arithmetic value to be written
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  void Write(std::uint16_t const& data, Base base) noexcept;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data A number of type std::uint32_t.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  void Write(std::uint32_t const& data) noexcept;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data A number of type std::uint32_t.
   * \param[in] base Base value of the arithmetic value to be written
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  void Write(std::uint32_t const& data, Base base) noexcept;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data A number of type std::uint32_t.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  void Write(std::uint64_t const& data) noexcept;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data A number of type std::uint32_t.
   * \param[in] base Base value of the arithmetic value to be written
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  void Write(std::uint64_t const& data, Base base) noexcept;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data A number of type float.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  void Write(float const& data) noexcept;

  /*!
   * \brief Write an arithmetic value to the stream.
   * \param[in] data A number of type double.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  void Write(double const& data) noexcept;

  /*!
   * \brief Write a character to the stream.
   * \param[in] data A character.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  void Write(char data) noexcept;

  /*!
   * \brief Write a basic string view to the stream.
   * \param[in] data A basic string view.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  void Write(amsr::core::StringView data) noexcept;

  /*!
   * \brief Write a basic C string view to the stream.
   * \param[in] data A basic C string view.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  void Write(vac::container::CStringView data) noexcept;

  /*!
   * \brief Write a span of characters to the stream.
   * \param[in] data The span to write.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  void Write(amsr::core::Span<char const> data) noexcept;

  /*!
   * \brief Write a span of bytes to the stream.
   * \param[in] data The span to write.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  void Write(amsr::core::Span<vac::language::byte const> data) noexcept;

  /*!
   * \brief Write a number of characters to the file stream by a provided callback.
   * \details The callback might be called multiple times with different views into the stream.
   * \warning The callback might be called less often than requested (even zero times).
   * \param[in] max_count The maximum number of characters to be written.
   * \param[in] callback The callback that gets access to the internal buffer. The return value indicates the number of
   * bytes written into the buffer.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable. Callback has to be checked for
   * FFI with respect to memory.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  void Write(std::uint64_t max_count,
             vac::language::FunctionRef<std::uint64_t(amsr::core::Span<char>)> callback) noexcept;

  /*!
   * \brief Write data to the stream by a provided callback.
   * \details The callback might be called multiple times with different views into the stream.
   * \warning The callback might be called less often than requested (even zero times).
   * \param[in] callback The callback that gets access to the internal buffer. The return value indicates the number of
   * bytes written into the buffer.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable. Callback has to be checked for
   * FFI with respect to memory.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  void Write(vac::language::FunctionRef<std::uint64_t(amsr::core::Span<char>)> callback) noexcept;

  /*!
   * \brief Writes a byte array in hexadecimal or binary base to the stream.
   * \param[in] data A span for a the byte array.
   * \param[in] base Base value of the arithmetic value to be written.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  void Write(amsr::core::Span<std::uint8_t> data, Base base) noexcept;

  /*!
   * \brief Inserts character into the underlying stream (e.g. a file or a string).
   * \param[in] data The character to be inserted.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  void Put(char data) noexcept;

  /*!
   * \brief Flushes buffer to file.
   * \context ANY
   * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  void Flush() noexcept;

  /*!
   * \brief Allow definition of manipulators.
   * \param[in,out] manipulator Manipulator of a stateful output stream.
   * \return Manipulated stateful output stream.
   * \context ANY
   * \pre In case of a file stream: Might abort if the file stream is closed or not writable.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  StatefulOutputStream& operator<<(StatefulOutputStream& (*manipulator)(StatefulOutputStream&)) noexcept;

 private:
  /*!
   * \brief Do not use Write(char const*).
   * \details Without this overload, a function with a char* parameter will call the overload Write(uint8_t). To prevent
   * this unintended behavior, this overload is defined as a private member function to cause a compile error.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Component Private
   * \steady UNKNOWN
   */
  static void Write(char const* /*data*/) noexcept;

  /*! Friend test declaration. */
  FRIEND_TEST(UT__StatefulOutputStream__Other, WriteCharPointer__Dummy);

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_IoStream_A11.3.1_friend_class
  /*! Friend test declaration. */
  template <typename T>
  FRIEND_TEST(CT__IoStream__StatefulOutputStreamCReq, WriteCharConstPointer);

  /*! Friend test declaration. */
  FRIEND_TEST(UT__StatefulOutputStream__Other, OperatorStar);

  /*! Friend test declaration. */
  FRIEND_TEST(UT__StatefulOutputStream__Other, OperatorArrow);
};

/*!
 * \brief Stream operator that calls OutputStringStream::Write() with the data provided.
 * \details Will Abort if Write fails and print the error message contained in the Result returned from Write.
 * \tparam T Any type that has been implemented by Write.
 * \param[in] stream An StatefulOutputStream to be written to.
 * \param[in] data The data to write to oss.
 * \return The stream.
 * \context ANY
 * \pre In case of a file stream: Aborts if the file stream is closed or not writable (location of checks depends on D).
 * \threadsafe FALSE
 * \unit amsr::stream::StatefulOutputStream
 * \steady UNKNOWN
 * \vprivate Component Private
 */
template <typename D,
          std::enable_if_t<((std::is_arithmetic<D>::value || std::is_same<D, amsr::core::StringView>::value) ||
                            (std::is_same<D, char>::value || std::is_same<D, std::int8_t>::value)),
                           std::int32_t> = 0>
StatefulOutputStream& operator<<(StatefulOutputStream& stream, D data) noexcept {
  stream.Write(std::forward<D>(data));
  return stream;
}

/*!
 * \brief Stream operator specialization for ara::core::String.
 * \details This will NOT include the null character at the end in the write.
 * \param[in] stream An StatefulOutputStream to be written to.
 * \param[in] str The String to write to the stream.
 * \return The stream.
 * \context ANY
 * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
 * \threadsafe FALSE
 * \unit amsr::stream::StatefulOutputStream
 * \vprivate Component Private
 * \steady UNKNOWN
 */
StatefulOutputStream& operator<<(StatefulOutputStream& stream, amsr::core::String str) noexcept;

/*!
 * \brief Stream operator specialization for vac::container::CStringView.
 * \details This will NOT include the null character at the end in the write.
 * \param[in] stream An StatefulOutputStream to be written to.
 * \param[in] csv The CStringView to write to the stream.
 * \return The stream.
 * \context ANY
 * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
 * \threadsafe FALSE
 * \unit amsr::stream::StatefulOutputStream
 * \vprivate Component Private
 * \steady UNKNOWN
 */
StatefulOutputStream& operator<<(StatefulOutputStream& stream, vac::container::CStringView csv) noexcept;

/*!
 * \brief Provide manipulator \c Flush.
 * \param[in,out] stream Stateful output stream.
 * \return Flushed stateful output stream.
 * \context ANY
 * \pre In case of a file stream: Aborts if the file stream is closed or not writable.
 * \threadsafe FALSE
 * \reentrant FALSE
 * \spec requires true; \endspec
 * \unit amsr::stream::StatefulOutputStream
 * \vprivate Product Private
 * \steady UNKNOWN
 */
StatefulOutputStream& Flush(StatefulOutputStream& stream) noexcept;

/*!
 * \brief Provide manipulator \c endl.
 * \param[in,out] stream Stateful output stream.
 * \return Stateful output stream with end-of-line character and flushed.
 * \context ANY
 * \pre In case of a file stream: Might abort if file stream is closed or not writable.
 * \threadsafe FALSE
 * \reentrant FALSE
 * \spec requires true; \endspec
 * \unit amsr::stream::StatefulOutputStream
 * \vprivate Product Private
 * \steady UNKNOWN
 */
StatefulOutputStream& endl(StatefulOutputStream& stream) noexcept;

/*!
 * \brief Sets the base to Decimal for the underlying stream.
 * \param[in] stream Stateful output stream.
 * \return The stateful output stream.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \spec requires true; \endspec
 * \unit amsr::stream::StatefulOutputStream
 * \vprivate Product Private
 * \steady UNKNOWN
 */
StatefulOutputStream& Dec(StatefulOutputStream& stream) noexcept;

/*!
 * \brief Sets the base to Binary for the underlying stream.
 * \param[in] stream Stateful output stream.
 * \return The stateful output stream.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \spec requires true; \endspec
 * \unit amsr::stream::StatefulOutputStream
 * \vprivate Product Private
 * \steady UNKNOWN
 */
StatefulOutputStream& Binary(StatefulOutputStream& stream) noexcept;

/*!
 * \brief Sets the base to Hex for the underlying stream.
 * \param[in] stream Stateful output stream.
 * \return The stateful output stream.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \spec requires true; \endspec
 * \unit amsr::stream::StatefulOutputStream
 * \vprivate Product Private
 * \steady UNKNOWN
 */
StatefulOutputStream& Hex(StatefulOutputStream& stream) noexcept;

/*!
 * \brief Sets the base prefix to be shown in for the underlying stream.
 * \param[in] stream Stateful output stream.
 * \return The stateful output stream.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \spec requires true; \endspec
 * \unit amsr::stream::StatefulOutputStream
 * \vprivate Product Private
 * \steady UNKNOWN
 */
StatefulOutputStream& SetShowBase(StatefulOutputStream& stream) noexcept;

/*!
 * \brief Sets the base prefix to hide for the underlying stream.
 * \param[in] stream Stateful output stream.
 * \return The stateful output stream.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \spec requires true; \endspec
 * \unit amsr::stream::StatefulOutputStream
 * \vprivate Product Private
 * \steady UNKNOWN
 */
StatefulOutputStream& NoShowBase(StatefulOutputStream& stream) noexcept;

/*!
 * \brief Enables use of uppercase letters for alpha-numeric characters for the underlying stream.
 * \param[in] stream Stateful output stream.
 * \return The stateful output stream.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \spec requires true; \endspec
 * \unit amsr::stream::StatefulOutputStream
 * \vprivate Product Private
 * \steady UNKNOWN
 */
StatefulOutputStream& Uppercase(StatefulOutputStream& stream) noexcept;

/*!
 * \brief Enables use of lowercase letters for alpha-numeric characters for the underlying stream.
 * \param[in] stream Stateful output stream.
 * \return The stateful output stream.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \spec requires true; \endspec
 * \unit amsr::stream::StatefulOutputStream
 * \vprivate Product Private
 * \steady UNKNOWN
 */
StatefulOutputStream& NoUppercase(StatefulOutputStream& stream) noexcept;

/*!
 * \brief Sets the floating point format to Fixed for the underlying stream.
 * \param[in] stream Stateful output stream.
 * \return The stateful output stream.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \spec requires true; \endspec
 * \unit amsr::stream::StatefulOutputStream
 * \vprivate Product Private
 * \steady UNKNOWN
 */
StatefulOutputStream& Fixed(StatefulOutputStream& stream) noexcept;

/*!
 * \brief Sets the floating point format to Scientific for the underlying stream.
 * \param[in] stream Stateful output stream.
 * \return The stateful output stream.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \spec requires true; \endspec
 * \unit amsr::stream::StatefulOutputStream
 * \vprivate Product Private
 * \steady UNKNOWN
 */
StatefulOutputStream& Scientific(StatefulOutputStream& stream) noexcept;

/*!
 * \brief Sets the floating point format to General for the underlying stream.
 * \param[in] stream Stateful output stream.
 * \return The stateful output stream.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \spec requires true; \endspec
 * \unit amsr::stream::StatefulOutputStream
 * \vprivate Product Private
 * \steady UNKNOWN
 */
StatefulOutputStream& General(StatefulOutputStream& stream) noexcept;

}  // namespace stream
}  // namespace amsr

#endif  // LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_STATEFUL_OUTPUT_STREAM_H_
