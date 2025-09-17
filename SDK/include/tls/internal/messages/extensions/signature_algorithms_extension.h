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
/*!        \file  signature_algorithms_extension.h
 *        \brief  Deserializer/Serializer for supported signature algorithms extension type.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_EXTENSIONS_SIGNATURE_ALGORITHMS_EXTENSION_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_EXTENSIONS_SIGNATURE_ALGORITHMS_EXTENSION_H_

#include <cstdint>
#include <set>
#include <utility>
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "ara/core/vector.h"

namespace amsr {
namespace tls {
namespace internal {
namespace messages {
namespace extensions {

/*!
 * \brief Signature Algorithms extension class.
 */
class SignatureAlgorithmsExtension final {
 public:
  /*!
   * \brief Deserializes an SignatureAlgorithmsExtension from the passed buffer.
   * \param[in] buffer The buffer to deserialize from.
   * \return Result containing an SignatureAlgorithmsExtension or an error.
   * \error TlsErrorDomain::TlsCommErrorCode::kTlsErrDeserialize if the passed buffer has an invalid size,
   *                                                             if the size of the contained list is invalid,
   *                                                             if the list contains duplicate values.
   * \context Any
   * \pre -
   * \reentrant TRUE
   * \vprivate This method is component private.
   */
  static amsr::core::Result<SignatureAlgorithmsExtension> Deserialize(
      amsr::core::Span<std::uint8_t const> const &buffer) noexcept;

  /*!
   * \brief Default constructible.
   */
  SignatureAlgorithmsExtension() noexcept = default;

  /*!
   * \brief Destructor.
   */
  ~SignatureAlgorithmsExtension() noexcept = default;

  /*!l
   * \brief Not copy constructible.
   */
  SignatureAlgorithmsExtension(SignatureAlgorithmsExtension const &) = delete;

  /*!
   * \brief Move constructor.
   */
  SignatureAlgorithmsExtension(SignatureAlgorithmsExtension &&) noexcept = default;

  /*!
   * \brief Not copy assignable.
   */
  SignatureAlgorithmsExtension &operator=(SignatureAlgorithmsExtension const &) = delete;

  /*!
   * \brief Not move assignable.
   */
  SignatureAlgorithmsExtension &operator=(SignatureAlgorithmsExtension &&) = delete;

  /*!
   * \brief Serializes an SignatureAlgorithmsExtension at the tail of the buffer.
   * \param[in, out] buffer The buffer to write into.
   * \context Any
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  static void Serialize(ara::core::Vector<std::uint8_t> &buffer) noexcept;

 private:
  /*!
   * \brief Checks if the buffer's size is ok.
   * \param[in] buffer The buffer to check.
   * \return Empty result or an error.
   * \error TlsErrorDomain::TlsCommErrorCode::kTlsErrDeserialize if the passed buffer has an invalid size.
   * \context Any
   * \pre -
   * \reentrant TRUE
   */
  static amsr::core::Result<void> CheckBufferSize(amsr::core::Span<std::uint8_t const> const &buffer) noexcept;
  /*!
   * \brief Deserializes the list's size in bytes value from the buffer.
   * \param[in] buffer The buffer to deserialize from.
   * \return Result with the list size or an error.
   * \error TlsErrorDomain::TlsCommErrorCode::kTlsErrDeserialize if the size value is 0 or malformed.
   * \context Any
   * \pre -
   * \reentrant TRUE
   */
  static amsr::core::Result<std::uint16_t> DeserializeListSize(
      amsr::core::Span<std::uint8_t const> const &buffer) noexcept;
  /*!
   * \brief Deserializes the list of signature algorithms values from the buffer.
   * \param[in] list_size The size of list in bytes.
   * \param[in] buffer The buffer to deserialize from.
   * \return Result with the list or an error.
   * \error TlsErrorDomain::TlsCommErrorCode::kTlsErrDeserialize if the list contains duplicates.
   * \context Any
   * \pre -
   * \reentrant TRUE
   */
  static amsr::core::Result<std::set<std::pair<std::uint8_t, std::uint8_t>>> DeserializeList(
      std::uint16_t list_size, amsr::core::Span<std::uint8_t const> buffer) noexcept;
  /*!
   * \brief Validates that the list of curves contains any supported curve(s).
   * \param[in] list The list to validate.
   * \return Empty result or an error.
   * \error TlsErrorDomain::TlsCommErrorCode::kTlsErrDeserialize if the list contains no supported value(s).
   * \context Any
   * \pre -
   * \reentrant TRUE
   */
  static amsr::core::Result<void> ValidateList(std::set<std::pair<std::uint8_t, std::uint8_t>> const &list) noexcept;
};

}  // namespace extensions
}  // namespace messages
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_EXTENSIONS_SIGNATURE_ALGORITHMS_EXTENSION_H_
