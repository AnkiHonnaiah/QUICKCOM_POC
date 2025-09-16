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
/*!        \file   helper_functions.h
 *        \brief   Common helper functions used internally by many of the classes in the TLS library
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_COMMON_HELPER_FUNCTIONS_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_COMMON_HELPER_FUNCTIONS_H_

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cwchar>
#include <iterator>
#include <memory>
#include <string>
#include <type_traits>
#include "amsr/core/abort.h"
#include "ara/core/vector.h"
#include "tls/internal/common/common_general_types.h"
#include "tls/internal/common/tls_ara_logger.h"
#include "tls/internal/messages/alert_message.h"
#include "tls/public/config/psk_config.h"
#include "tls/public/tls_comm_callback_interfaces.h"
#include "tls/public/tls_comm_error_definitions.h"
#include "tls/public/tls_comm_general_types.h"
#include "tls_crypto_adapter/internal/crypto_adapter_error_definitions.h"
#include "tls_crypto_adapter/internal/performance_crypto_adapter.h"
#include "tls_crypto_adapter/internal/remote_crypto_adapter.h"
#include "vac/language/location.h"

namespace amsr {
namespace tls {
namespace internal {
namespace common {

/*!
 * \brief Reports an error to a supplied context.
 * \details Writes an error into the context object with supplied error code and error message.
 * \param[in,out] flow_context Context object to write the error to.
 * \param[in] flow_context_error_string Error message to write to the context object.
 * \param[in] flow_context_error_code Error code to write to the context object.
 * \context ANY
 * \pre cxt has no error when the function is called.
 * \reentrant FALSE
 * \vprivate This method is component private.
 */
void ReportError(common::TlsCommFlowContext &flow_context, char const *flow_context_error_string,
                 TlsCommErrorCode flow_context_error_code) noexcept;

/*!
 * \brief Clears error in a provided TlsCommFlowContext.
 * \param[in,out] flow_context Reference to context object.
 * \context ANY
 * \pre-
 * \reentrant FALSE
 * \vprivate This method is component private.
 */
void ClearFlowContextError(common::TlsCommFlowContext &flow_context) noexcept;

/*!
 * \brief Restores a given TlsCommFlowContext to its default state.
 * \param[in,out] flow_context Reference to context object.
 * \context ANY
 * \pre-
 * \reentrant FALSE
 * \vprivate This method is component private.
 */
void ResetFlowContext(common::TlsCommFlowContext &flow_context) noexcept;

/*!
 * \brief Writes an error to the context and logs the error.
 * \details Writes an error into the context with supplied error code and error string. Also logs an error using the
 * supplied module, function name and line number.

 * \param[in] module The module used as the source for the log output.
 * \param[in] log_message The error string that should be used in the log output.
 * \param[in] file_name Filename of the source that called this function.
 * \param[in] function_name Name of the function where the error occurred.
 * \param[in] line_number Source code line number where the error occurred.
 * \param[in,out] flow_context The context object to write the error to.
 * \param[in] flow_context_error_code The error code that should be written to the context object and used in the log
 * output.
 * \context ANY
 * \pre cxt has no error when the function is called
 * \reentrant FALSE
 * \vprivate This method is component private.
 */
void ReportAndLogError(TlsAraLogger::EnTlsLogModule module, LogMessage log_message, FileName const file_name,
                       FunctionName const function_name, LineNumber const line_number, TlsCommFlowContext &flow_context,
                       TlsCommErrorCode flow_context_error_code) noexcept;

/*!
 * \brief Converts the given value to string.
 * \details Uses std::to_string() to convert to string.
 * \tparam T  The type of the value to convert to string from.
 * \param[in] value Parameter to be converted to string.
 * \return The passed value converted to string.
 * \context ANY
 * \reentrant FALSE
 */
template <typename T>
amsr::core::String ToString(T const &value) noexcept {
  return amsr::core::String{std::to_string(value)};
}

namespace helper_functions {

/*!
 * \brief     Returns the underlying value of an enumerator.
 *
 * \tparam    EnumType An enumeration type.
 *
 * \param[in] enumerator The enumerator to convert.
 * \return    The underlying value of the enumerator.
 *
 * \pre       -
 * \context   ANY
 * \reentrant TRUE
 *
 * \see       std::to_underlying from C++23.
 */
template <typename EnumType, std::enable_if_t<std::is_enum<EnumType>::value, bool> = true>
constexpr auto ToUnderlying(EnumType const enumerator) noexcept -> std::underlying_type_t<EnumType> {
  return static_cast<std::underlying_type_t<EnumType>>(enumerator);
}

/*!
 * \brief     Computes the size of a given container in bytes.
 *
 * \tparam    ContainerType A container-like type. Must provide a size member function and a value_type member type.
 *
 * \param[in] container The container to measure.
 * \return    The size of the container in bytes.
 *
 * \pre       -
 * \context   ANY
 * \reentrant TRUE
 */
template <typename ContainerType>
constexpr auto GetSizeInBytes(ContainerType const &container) noexcept -> std::size_t {
  return container.size() * sizeof(typename ContainerType::value_type);
}

/*!
 * \brief Returns a given byte of a multi byte value that is not an unsigned long long
 * \tparam ByteOffset  The offset of the byte to return. byte_offset == 0 denotes the LSB
 * \tparam T  The unsigned type of the value to extract a specific byte from
 * \param[in] value  The value to get the needed byte from
 * \return The byte from value denoted by ByteOffset
 * \reentrant TRUE
 * \vprivate This method is component private.
 */
template <std::size_t ByteOffset, typename T>
constexpr auto GetByte(T const &value) noexcept ->
    typename std::enable_if<(sizeof(T) < sizeof(std::uint64_t)), std::uint8_t>::type {
  static_assert(std::is_unsigned<T>::value, "T has to be unsigned.");
  static_assert(ByteOffset < sizeof(T), "MultiByteType byte_offset out of range");
  std::uint8_t byte_to_return{0U};
  constexpr std::uint32_t pos{static_cast<std::uint32_t>(ByteOffset * 8UL)};
  std::uint32_t const bit_pattern{static_cast<std::uint32_t>(value)};
  std::uint32_t const shifted_value{bit_pattern >> pos};
  std::uint32_t const lsb{shifted_value & 0xFFu};
  byte_to_return = static_cast<std::uint8_t>(lsb);
  return byte_to_return;
}

/*!
 * \brief Returns a given byte of a multi byte value that is an unsigned long long
 * \tparam ByteOffset  The offset of the byte to return. byte_offset == 0 denotes the LSB
 * \tparam T  The unsigned type of the value to extract a specific byte from
 * \param[in] value  The value to get the needed byte from
 * \return The byte from value denoted by ByteOffset
 * \reentrant TRUE
 * \vprivate This method is component private.
 */
template <std::size_t ByteOffset, typename T>
constexpr auto GetByte(T const &value) noexcept ->
    typename std::enable_if<sizeof(T) == sizeof(std::uint64_t), std::uint8_t>::type {
  static_assert(std::is_unsigned<T>::value, "T has to be unsigned.");
  static_assert(ByteOffset < sizeof(T), "MultiByteType byte_offset out of range");
  std::uint8_t byte_to_return{0U};
  constexpr std::uint64_t pos{ByteOffset * 8ULL};
  std::uint64_t const bit_pattern{value};
  std::uint64_t const shifted_value{bit_pattern >> pos};
  std::uint64_t const lsb{shifted_value & 0xFFu};
  byte_to_return = static_cast<std::uint8_t>(lsb);
  return byte_to_return;
}

/*!
 * \brief Serialize a multibyte value by N bytes and push the serialized value back into the in-out buffer
 * \details Serializes and pushes back from Hi to Lo for network order, big endianess. E.g if the ByteCount is
 * 3 for a uint64_t the Hi byte will be the highest byte from the Lo byte -> 00'00'00'00'00'[FF'FF'FF]
 * \tparam ByteCount  The number of bytes to serialize from the passed value
 * \tparam T  The unsigned type of the value to serialize from
 * \tparam ContainerType the type of container to push back into.
 * \param[in] value  The value to get the needed bytes from
 * \param[in, out] buffer  The in-out buffer to push back serialized values to
 * \return Void
 * \reentrant FALSE
 * \vprivate This method is component private.
 */
template <std::size_t ByteCount, typename T, typename ContainerType>
constexpr auto SerializeUnsignedMultibyteAndPushBack(T const &value, ContainerType &buffer) noexcept -> void {
  static_assert(ByteCount <= sizeof(T), "ByteCount cannot be more than sizeof given multibyte type T");
  switch (ByteCount) {
    case 1:
      buffer.push_back(GetByte<0, std::uint16_t>(static_cast<std::uint16_t>(value)));
      break;
    case 2:
      buffer.push_back(GetByte<1, std::uint16_t>(static_cast<std::uint16_t>(value)));
      buffer.push_back(GetByte<0, std::uint16_t>(static_cast<std::uint16_t>(value)));
      break;
    case 3:
      buffer.push_back(GetByte<2, std::uint32_t>(static_cast<std::uint32_t>(value)));
      buffer.push_back(GetByte<1, std::uint32_t>(static_cast<std::uint32_t>(value)));
      buffer.push_back(GetByte<0, std::uint32_t>(static_cast<std::uint32_t>(value)));
      break;
    case 4:
      buffer.push_back(GetByte<3, std::uint32_t>(static_cast<std::uint32_t>(value)));
      buffer.push_back(GetByte<2, std::uint32_t>(static_cast<std::uint32_t>(value)));
      buffer.push_back(GetByte<1, std::uint32_t>(static_cast<std::uint32_t>(value)));
      buffer.push_back(GetByte<0, std::uint32_t>(static_cast<std::uint32_t>(value)));
      break;
    case 5:
      buffer.push_back(GetByte<4, std::uint64_t>(value));
      buffer.push_back(GetByte<3, std::uint64_t>(value));
      buffer.push_back(GetByte<2, std::uint64_t>(value));
      buffer.push_back(GetByte<1, std::uint64_t>(value));
      buffer.push_back(GetByte<0, std::uint64_t>(value));
      break;
    case 6:
      buffer.push_back(GetByte<5, std::uint64_t>(value));
      buffer.push_back(GetByte<4, std::uint64_t>(value));
      buffer.push_back(GetByte<3, std::uint64_t>(value));
      buffer.push_back(GetByte<2, std::uint64_t>(value));
      buffer.push_back(GetByte<1, std::uint64_t>(value));
      buffer.push_back(GetByte<0, std::uint64_t>(value));
      break;
    case 7:
      buffer.push_back(GetByte<6, std::uint64_t>(value));
      buffer.push_back(GetByte<5, std::uint64_t>(value));
      buffer.push_back(GetByte<4, std::uint64_t>(value));
      buffer.push_back(GetByte<3, std::uint64_t>(value));
      buffer.push_back(GetByte<2, std::uint64_t>(value));
      buffer.push_back(GetByte<1, std::uint64_t>(value));
      buffer.push_back(GetByte<0, std::uint64_t>(value));
      break;
    case 8:
      buffer.push_back(GetByte<7, std::uint64_t>(value));
      buffer.push_back(GetByte<6, std::uint64_t>(value));
      buffer.push_back(GetByte<5, std::uint64_t>(value));
      buffer.push_back(GetByte<4, std::uint64_t>(value));
      buffer.push_back(GetByte<3, std::uint64_t>(value));
      buffer.push_back(GetByte<2, std::uint64_t>(value));
      buffer.push_back(GetByte<1, std::uint64_t>(value));
      buffer.push_back(GetByte<0, std::uint64_t>(value));
      break;
    default:
      // This should never be reached, covered by static_assert.
      break;
  }
}

/*!
 * \brief         Serializes an enumeration value to an output buffer.
 *
 * \note          This function uses the size of the enumeration type as
 *                the number of bytes that must be emitted.
 *
 * \tparam        EnumType      An arbitrary enumeration type.
 * \tparam        ContainerType A container type that provides a push_back member function.
 *
 * \param[in]     enumerator The enumeration value.
 * \param[in,out] buffer     The output buffer.
 *
 * \pre           -
 * \context       ANY
 * \reentrant     FALSE
 */
template <typename EnumType, typename ContainerType, std::enable_if_t<std::is_enum<EnumType>::value, bool> = true>
constexpr void SerializeEnumAndPushBack(EnumType const enumerator, ContainerType &buffer) noexcept {
  SerializeUnsignedMultibyteAndPushBack<sizeof(EnumType)>(ToUnderlying(enumerator), buffer);
}

/*!
 * \brief Perform a check for a given range [index, index+count] if it is within the boundaries of a buffer.
 * \param[in] buffer_size  The actual size of the buffer
 * \param[in] index  The starting index for the range in the buffer
 * \param[in] count  The count for the range
 * \return True if the range is within the boundary, False if it not
 * \reentrant TRUE
 * \vprivate This method is component private.
 */
bool CheckBufferBoundary(std::size_t const buffer_size, std::size_t const index, std::size_t const count) noexcept;

/*!
 * \brief Deserialize bytes from a given valid range and store the values in an unsigned multibyte destination variable
 * \details If the provided iterator range is invalid amsr::core::Abort will be called
 * \pre The iterator range shall NOT exceed the byte storing capacity of type T and it shall NOT be negative
 * \tparam IterType  The type of the iterator
 * \tparam T  The type of the destination multibyte variable
 * \param[in] begin  The beginning of the iterator range
 * \param[in] end  The end for the iterator range
 * \param[in, out] out The unsigned multibyte destination variable
 * \return void if enabled
 * \reentrant FALSE
 * \vprivate This method is component private.
 */
template <class IterType, typename T>
auto DeserializeToValue(IterType begin, IterType end, T &out) noexcept ->
    typename std::enable_if<std::is_same<typename std::iterator_traits<IterType>::value_type, std::uint8_t>::value &&
                                std::is_unsigned<T>::value,
                            void>::type {
  TlsAraLogger &logger_ref{TlsAraLogger::Instance()};

  std::ptrdiff_t const distance{std::distance(begin, end)};

  if ((distance > static_cast<ptrdiff_t>(sizeof(T))) || (distance < 0)) {
    logger_ref.LogFatalAndAbort(TlsAraLogger::EnTlsLogModule::kCentral, static_cast<LogMessage>("Invalid byte range."),
                                {AMSR_FILE}, {static_cast<char const *>(__func__)}, {__LINE__});
  }

  out = 0U;

  // The returned function object is of no interest here
  static_cast<void>(std::for_each(begin, end, [&out](std::uint8_t value) noexcept {
    T const bits_in_a_byte{static_cast<T>(8U)};
    // VECTOR Next Construct AutosarC++17_10-A4.7.1: MD_libseccom_A4.7.1_overflow
    // VECTOR Next Construct AutosarC++17_10-M0.3.1: MD_libseccom_M0.3.1_overflow
    out = static_cast<T>(out << bits_in_a_byte);
    out = static_cast<T>(out | static_cast<T>(value));
  }));
}

/*!
 * \brief Maps incoming Crypto Adapter error code to a tls comm error code
 * \param[in] crypto_adapter_error_code code to map
 * \return TlsCommErrorCode The corresponding Tls error code
 * \context ANY
 * \pre -
 * \reentrant TRUE
 * \vprivate This method is component private.
 */
TlsCommErrorCode MapErrorCode(
    tls_crypto_adapter::internal::EnCryptoAdapterErrors const crypto_adapter_error_code) noexcept;

/*!
 * \brief Log the alert message to be sent based on the related log level
 * \details If the alert message description is of type CloseNotify or ProtocolVersion or UserCanceled, it will be
 * logged as an info, otherwise will be logged as a warning.
 * \param[in,out] cxt The TlsCommFlowContext where context information about the operation can be
 * recorded.
 * \param[in] message A reference to the alert message which is going to be logged
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vprivate This method is component private.
 */
void LogAlertMessage(common::TlsCommFlowContext const &cxt, messages::AlertMessage const &message) noexcept;

/*!
 * \brief           Validate the TLS configuration
 * \param[in]       config
 *                  The configuration structure to validate.
 * \param[in]       ptr_callback_holder
 *                  A pointer to the callback holder, cannot be nullptr.
 * \param[in]       psk_config
 *                  A pointer to the PSK configuration.
 * \return          A result with a value if the configuration is valid, with TlsCommErrorCode::kTlsErrInvalidArgument
 *                  error (and an appropriate message) otherwise.
 */
ara::core::Result<void> ValidateConfiguration(TlsCommConfigurationInformation const &config,
                                              TlsCommCallbackInterface const *ptr_callback_holder,
                                              config::PskConfig const *psk_config) noexcept;

}  // namespace helper_functions
}  // namespace common
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_COMMON_HELPER_FUNCTIONS_H_
