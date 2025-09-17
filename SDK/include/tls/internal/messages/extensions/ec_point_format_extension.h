/*!********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!       \file   ec_point_format_extension.h
 *        \brief  Deserializer for elliptic curve point format extension type.
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_EXTENSIONS_EC_POINT_FORMAT_EXTENSION_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_EXTENSIONS_EC_POINT_FORMAT_EXTENSION_H_

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
 * \brief Elliptic Curves point format extension class.
 */
class EcPointFormatExtension final {
 public:
  /*!
   * \brief Deserialize an EcPointFormatExtension instance from a serialized buffer.
   * \param[in] buffer The buffer to read from.
   * \return Result with the class instance or an error.
   * \error tls::TlsCommErrorCode::kTlsErrDeserialize if the buffer's size is not exactly 2 bytes,
   *                                                  if the list size is not exactly 1,
   *                                                  if the ec point format value is not exactly 0 (Uncompressed).
   */
  static amsr::core::Result<EcPointFormatExtension> Deserialize(
      amsr::core::Span<std::uint8_t const> const &buffer) noexcept;

  /*!
   * \brief Constructor.
   */
  EcPointFormatExtension() noexcept = default;

  /*!
   * \brief Destructor.
   */
  ~EcPointFormatExtension() noexcept = default;

  /*!l
   * \brief Not copy constructible.
   */
  EcPointFormatExtension(EcPointFormatExtension const &) = delete;

  /*!
   * \brief Move constructor.
   */
  EcPointFormatExtension(EcPointFormatExtension &&) noexcept = default;

  /*!
   * \brief Not copy assignable.
   */
  EcPointFormatExtension &operator=(EcPointFormatExtension const &) = delete;

  /*!
   * \brief Not move assignable.
   */
  EcPointFormatExtension &operator=(EcPointFormatExtension &&) = delete;
  /*!
   * \brief Serialize the member variables into a buffer.
   * \param[out] buffer The buffer to write into.
   */
  static void Serialize(ara::core::Vector<std::uint8_t> &buffer) noexcept;

 private:
  /*!
   * \brief Checks that the buffer's size has the expected size of the extension data.
   * \param[in] buffer A reference to the buffer.
   * \return Empty result or an error if the size was not as expected.
   */
  static amsr::core::Result<void> CheckBufferSize(amsr::core::Span<std::uint8_t const> const &buffer) noexcept;

  /*!
   * \brief Checks that the lists size is 1, only one item shall be present in the list.
   * \param[in] value The deserialized value.
   * \return Empty result or an error if the value was not as expected.
   */
  static amsr::core::Result<void> CheckListSize(std::uint8_t value) noexcept;

  /*!
   * \brief Checks that the value is 0, Uncompressed, for the ECPointFormat.
   * \param[in] value The deserialized value.
   * \return Empty result or an error if the value was not as expected.
   */
  static amsr::core::Result<void> CheckValue(std::uint8_t value) noexcept;
};

}  // namespace extensions
}  // namespace messages
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_EXTENSIONS_EC_POINT_FORMAT_EXTENSION_H_
