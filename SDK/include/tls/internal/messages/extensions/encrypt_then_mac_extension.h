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
/*!       \file   encrypt_then_mac_extension.h
 *        \brief  Parser for encrypt then mac extension type.
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_EXTENSIONS_ENCRYPT_THEN_MAC_EXTENSION_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_EXTENSIONS_ENCRYPT_THEN_MAC_EXTENSION_H_

#include <cstdint>

#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "ara/core/vector.h"

namespace amsr {
namespace tls {
namespace internal {
namespace messages {
namespace extensions {

/*!
 * \brief Encrypt then mac extension type serializer and deserializer.
 */
class EncryptThenMacExtension final {
 public:
  /*!
   * \brief Deserialize an instance of the class from a serialized data buffer.
   * \param[in] buffer The buffer to read from.
   * \return Result with the class instance or an error.
   * \error tls::TlsCommErrorCode::kTlsErrDeserialize if the buffers size is not exactly 0 bytes.
   */
  static amsr::core::Result<EncryptThenMacExtension> Deserialize(
      amsr::core::Span<std::uint8_t const> const &buffer) noexcept;

  /*!
   * \brief Destructor.
   */
  ~EncryptThenMacExtension() noexcept = default;

  /*!
   * \brief Not copy constructible.
   */
  EncryptThenMacExtension(EncryptThenMacExtension const &) = delete;

  /*!
   * \brief Move constructor.
   */
  EncryptThenMacExtension(EncryptThenMacExtension &&) noexcept = default;

  /*!
   * \brief Not copy assignable.
   */
  EncryptThenMacExtension &operator=(EncryptThenMacExtension const &) = delete;

  /*!
   * \brief Not move assignable.
   */
  EncryptThenMacExtension &operator=(EncryptThenMacExtension &&) = delete;

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
   * \error tls::TlsCommErrorCode::kTlsErrDeserialize if the buffers size is not exactly 2 bytes.
   */
  static amsr::core::Result<void> CheckBufferSize(amsr::core::Span<std::uint8_t const> const &buffer) noexcept;
};

}  // namespace extensions
}  // namespace messages
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_EXTENSIONS_ENCRYPT_THEN_MAC_EXTENSION_H_
