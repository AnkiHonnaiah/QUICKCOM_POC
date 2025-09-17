/*!********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  hello_message_extensions.h
 *        \brief  This file contains the declarations and definitions of SupportedExtensions, ExtensionTypesList and
 *                HelloMessageExtensions.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_EXTENSIONS_HELLO_MESSAGE_EXTENSIONS_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_EXTENSIONS_HELLO_MESSAGE_EXTENSIONS_H_

#include <set>
#include <utility>

#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "tls/internal/messages/extensions/ec_extension.h"
#include "tls/internal/messages/extensions/ec_point_format_extension.h"
#include "tls/internal/messages/extensions/encrypt_then_mac_extension.h"
#include "tls/internal/messages/extensions/record_size_limit.h"
#include "tls/internal/messages/extensions/signature_algorithms_extension.h"
#include "tls/internal/messages/extensions/tls_extensions.h"

namespace amsr {
namespace tls {
namespace internal {
namespace messages {
namespace extensions {

/*!
 * \brief Contains all optional TLS extensions supported by LibTLS.
 */
struct SupportedExtensions {
  /*!
   * \brief The optional Elliptic curves (formerly known as supported groups) extension
   */
  amsr::core::Optional<EcExtension> ec{amsr::core::nullopt};
  /*!
   * \brief The optional ECPointFormat extension
   */
  amsr::core::Optional<EcPointFormatExtension> ec_point_format{amsr::core::nullopt};
  /*!
   * \brief The optional RecordSizeLimit extension
   */
  amsr::core::Optional<RecordSizeLimitExtension> record_size_limit{amsr::core::nullopt};
  /*!
   * \brief The optional EncryptThenMac extension
   */
  amsr::core::Optional<EncryptThenMacExtension> encrypt_then_mac{amsr::core::nullopt};
  /*!
   * \brief The optional SignatureAlgorithms extension
   */
  amsr::core::Optional<SignatureAlgorithmsExtension> signature_algorithms{amsr::core::nullopt};
};

/*!
 * \brief A list to keep track of extensions contained in a Hello message.
 */
struct ExtensionTypesList {
  /*!
   * \brief A set of received TLS extensions.
   */
  std::set<TlsExtensionType> extension_types{};
};

/*!
 * \brief This class provides API's to deserialize and serialized supported TLS extensions.
 */
class HelloMessageExtensions final {
 public:
  /*!
   * \brief Deserializes TLS extensions from a given buffer.
   * \param[in] extensions Serialized TLS extensions.
   * \return Supported TLS extensions (if any were deserialized) and a list of all extensions contained in the buffer.
   * \error tls::TlsCommErrorCode::kTlsErrInvalidArgument if a deserialized value is invalid.
   * \error tls::TlsCommErrorCode::kTlsErrDeserialize if the extension type is already present in the list,
   *                                                  if the extension failed to be deserialized,
   *                                                  if the size of the next extension is not 0 and not enough for a
   *                                                  complete extension.
   * \trace DSGN-SecCom-HelloMessageExtensions
   */
  static amsr::core::Result<std::pair<SupportedExtensions, ExtensionTypesList>> Deserialize(
      amsr::core::Span<std::uint8_t const> extensions) noexcept;

  /*!
   * \brief Serializes supported extensions.
   * \param[in] extensions The supported extensions to serialize.
   * \return A buffer with serialized extensions, or an empty buffer if no extensions were present.
   */
  static ara::core::Vector<std::uint8_t> Serialize(SupportedExtensions const& extensions) noexcept;

 private:
  /*!
   * \brief Deserializes extensions from a given range.
   * \param[in] extensions The data range to deserialize.
   * \param[in, out] supported_extensions A reference to supported extensions.
   * \param[in, out] extension_types_list A reference to an TLS extensions list.
   * \return An empty result.
   * \error tls::TlsCommErrorCode::kTlsErrInvalidArgument if a deserialized value is invalid.
   * \error tls::TlsCommErrorCode::kTlsErrDeserialize if the extension type is already present in the list,
   *                                                  if the extension failed to be deserialized,
   *                                                  if the size of the next extension is not 0 and not enough for a
   *                                                  complete extension.
   */
  static amsr::core::Result<void> DeserializeExtensions(amsr::core::Span<std::uint8_t const> extensions,
                                                        SupportedExtensions& supported_extensions,
                                                        ExtensionTypesList& extension_types_list) noexcept;

  /*!
   * \brief Deserializes one extension from a given range.
   * \param[in] extension_data The data range to deserialize.
   * \param[in, out] supported_extensions A reference to supported extensions, only updated if the deserialized
   * extension is supported.
   * \param[in] tls_extension_type The type of this extension.
   * \return An empty result.
   * \error tls::TlsCommErrorCode::kTlsErrInvalidArgument if a deserialized value is invalid.
   * \error tls::TlsCommErrorCode::kTlsErrDeserialize if the extension failed to be deserialized.
   */
  static amsr::core::Result<void> DeserializeExtension(amsr::core::Span<std::uint8_t const> extension_data,
                                                       SupportedExtensions& supported_extensions,
                                                       TlsExtensionType tls_extension_type) noexcept;

  /*!
   * \brief Deserializes the extension data field size value.
   * \param[in] extension_field_length The range of bytes containing the value.
   * \return The size value of the extension data field.
   */
  static std::size_t DeserializeExtensionFieldLength(
      amsr::core::Span<std::uint8_t const> extension_field_length) noexcept;
  /*!
   * \brief Deserialized the extension type value of the extension.
   * \param[in] extension_type The range of bytes containing the value.
   * \return The type of the extension.
   */
  static TlsExtensionType DeserializeExtensionType(amsr::core::Span<std::uint8_t const> extension_type) noexcept;

  /*!
   * \brief Adds an extension type to the list of received extension types.
   * \param[in, out] list The list to update.
   * \param[in] extension_type The received extension type.
   * \return Empty result.
   * \error tls::TlsCommErrorCode::kTlsErrDeserialize if the extension type is already present in the list.
   */
  static amsr::core::Result<void> AddToList(ExtensionTypesList& list, TlsExtensionType const& extension_type) noexcept;

  /*!
   * \brief Advances to the next extension to deserialize or to the end.
   * \param[in] extensions The current range of extensions.
   * \param[in] tls_extension_field_length The number of bytes indicating the size of the current extension.
   * \return A range for the next extension or the end.
   * \error tls::TlsCommErrorCode::kTlsErrDeserialize if the size of the next extension is not 0 and not enough for a
   * complete extension.
   */
  static amsr::core::Result<amsr::core::Span<std::uint8_t const>> Advance(
      amsr::core::Span<std::uint8_t const> const& extensions, std::size_t tls_extension_field_length) noexcept;

  /*!
   * \brief Serializes a supported extension if the optional contains a value.
   * \tparam Optional The optional type containing an extension.
   * \param[in] opt The optional extension to serialize.
   * \param[in, out] destination The destination buffer for appending the serialized extension.
   */
  template <class Optional>
  static void SerializeIfPresent(Optional const& opt, ara::core::Vector<std::uint8_t>& destination) noexcept;
};

}  // namespace extensions
}  // namespace messages
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_EXTENSIONS_HELLO_MESSAGE_EXTENSIONS_H_
