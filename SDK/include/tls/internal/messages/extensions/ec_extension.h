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
/*!        \file  ec_extension.h
 *        \brief  Deserializer/Serializer for supported elliptic curves extension type.
 *        \note   This extension is also known as "supported_groups"
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_EXTENSIONS_EC_EXTENSION_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_EXTENSIONS_EC_EXTENSION_H_

#include <cstdint>
#include <set>
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "ara/core/vector.h"

namespace amsr {
namespace tls {
namespace internal {
namespace messages {
namespace extensions {

/*!
 * \brief Elliptic Curves / Supported Groups extension class.
 */
class EcExtension final {
 public:
  /*!
   * \brief Deserializes an EcExtension from the passed buffer.
   * \param[in] buffer The buffer to deserialize from.
   * \return Result containing an EcExtension or an error.
   * \error TlsErrorDomain::TlsCommErrorCode::kTlsErrDeserialize if the passed buffer has an invalid size,
   *                                                             if the size of the contained list is invalid,
   *                                                             if the list contains duplicate values,
   * \context Any
   * \pre -
   * \reentrant TRUE
   * \vprivate This method is component private.
   */
  static auto Deserialize(amsr::core::Span<std::uint8_t const> const &buffer) noexcept
      -> amsr::core::Result<EcExtension>;

  /*!
   * \brief Creates an EcExtension from the supplied list of supported curves / groups.
   * \param[in] list A list of curves / groups.
   * \return Result containing an EcExtension or an error.
   * \context Any
   * \pre -
   * \reentrant TRUE
   * \vprivate This method is component private.
   */
  static auto Create(std::set<std::uint16_t> list) noexcept -> amsr::core::Result<EcExtension>;

  /*!
   * \brief Default constructible.
   */
  EcExtension() noexcept = delete;

  /*!
   * \brief Destructor.
   */
  ~EcExtension() noexcept = default;

  /*!l
   * \brief Not copy constructible.
   */
  EcExtension(EcExtension const &) = delete;

  /*!
   * \brief Move constructor.
   */
  EcExtension(EcExtension &&) noexcept = default;

  /*!
   * \brief Not copy assignable.
   */
  auto operator=(EcExtension const &) -> EcExtension & = delete;

  /*!
   * \brief Not move assignable.
   */
  auto operator=(EcExtension &&) -> EcExtension & = delete;

  /*!
   * \brief Serializes an EcExtension at the tail of the buffer.
   * \param[in, out] buffer The buffer to write into.
   * \context Any
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void Serialize(ara::core::Vector<std::uint8_t> &buffer) const noexcept;

  /*!
   * \brief Checks to see if the only supported curve, x25519, is included in the extension.
   * \return True if x25519 curve is found. False otherwise.
   * \context Any
   * \pre -
   * \reentrant TRUE
   */
  auto ContainsX25519() const noexcept -> bool;

 private:
  /*!
   * \brief Constructor
   * \note The constructor is private since the Create() or Deserialize() APIs should be used to create EcExtension
   * objects.
   * \param[in] list
   */
  explicit EcExtension(std::set<std::uint16_t> list) noexcept;

  /*!
   * \brief Checks if the buffer's size is ok.
   * \param[in] buffer The buffer to check.
   * \return Empty result or an error.
   * \error TlsErrorDomain::TlsCommErrorCode::kTlsErrDeserialize if the passed buffer has an invalid size.
   * \context Any
   * \pre -
   * \reentrant TRUE
   */
  static auto CheckBufferSize(amsr::core::Span<std::uint8_t const> const &buffer) noexcept -> amsr::core::Result<void>;

  /*!
   * \brief Deserializes the list's size in bytes value from the buffer.
   * \param[in] buffer The buffer to deserialize from.
   * \return Result with the list size or an error.
   * \error TlsErrorDomain::TlsCommErrorCode::kTlsErrDeserialize if the size value is 0 or malformed.
   * \context Any
   * \pre -
   * \reentrant TRUE
   */
  static auto DeserializeListSize(amsr::core::Span<std::uint8_t const> const &buffer) noexcept
      -> amsr::core::Result<std::uint16_t>;

  /*!
   * \brief Deserializes the list of curves values from the buffer.
   * \param[in] list_size The size of list in bytes.
   * \param[in] buffer The buffer to deserialize from.
   * \return Result with the list or an error.
   * \error TlsErrorDomain::TlsCommErrorCode::kTlsErrDeserialize if the list contains duplicates.
   * \context Any
   * \pre -
   * \reentrant TRUE
   */
  static auto DeserializeList(std::uint16_t list_size, amsr::core::Span<std::uint8_t const> buffer) noexcept
      -> amsr::core::Result<std::set<std::uint16_t>>;

  /*!
   * \brief Container for the curves / groups.
   * \note The only supported curve is x25519.
   */
  std::set<std::uint16_t> curves_;
};

}  // namespace extensions
}  // namespace messages
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_EXTENSIONS_EC_EXTENSION_H_
