/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2020 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!          \file    null_null.h
 *          \brief    This file contains the class declaration for the bulk encryption with "NullNull".
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_BULK_ENCRYPTION_NULL_NULL_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_BULK_ENCRYPTION_NULL_NULL_H_

#include "tls/internal/record_protocol/bulk_encryption/bulk_encryption_interface.h"
#include "tls/internal/record_protocol/record_protocol_common_types.h"

namespace amsr {
namespace tls {
namespace internal {
namespace record_protocol {
namespace bulk_encryption {

/*!
 * \brief Maximum number of extra bytes used by the "TlsNullWithNullNull" cipher suite.
 */
static constexpr std::uint16_t kTlsNullWithNullNullRecordExpansion{0u};

/*!
 * \brief This classification of bulk encryption provides no authentication and no encryption.
 * \details Authentication: NULL
 *          Encryption: NULL
 *          Hash: NULL
 * \vprivate This class is component private.
 */
class NullNull final : public BulkEncryptionInterface {
 public:
  /*! \brief Deafult Constructor. */
  NullNull() = default;
  /*! \brief Default Destructor. */
  ~NullNull() final = default;
  /*! \brief Not copy constructible. */
  NullNull(NullNull const &) = delete;
  /*! \brief Not copy assignable. */
  NullNull &operator=(NullNull const &) = delete;
  /*! \brief Not move constructible. */
  NullNull(NullNull &&) = delete;
  /*! \brief Not move assignable. */
  NullNull &operator=(NullNull &&) = delete;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::bulk_encryption::BulkEncryptionInterface::InitializeEncryptor
   * \vprivate This method is component private.
   */
  amsr::core::Result<void> InitializeEncryptor(SecurityParameters const &sp) noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::bulk_encryption::BulkEncryptionInterface::InitializeDecryptor
   * \vprivate This method is component private.
   */
  amsr::core::Result<void> InitializeDecryptor(SecurityParameters const &sp) noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::bulk_encryption::BulkEncryptionInterface::Decrypt
   * \details The returning compressed text will take ownership over the payload in the passed cipher text.
   * \trace SPEC-6562035
   * \vprivate This method is component private.
   */
  CompressedTextUptr Decrypt(CipherTextUptr &&cipher_text, SecurityParameters const &sp) noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::bulk_encryption::BulkEncryptionInterface::Encrypt
   * \details The returning cipher text will take ownership over the payload in the passed compressed text.
   * \trace SPEC-6493113, SPEC-6493114, SPEC-6562035
   * \vprivate This method is component private.
   */
  CipherTextUptr Encrypt(CompressedTextUptr &&comp_text, SecurityParameters const &sp) noexcept final;
};

}  // namespace bulk_encryption
}  // namespace record_protocol
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_BULK_ENCRYPTION_NULL_NULL_H_
