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
/*!        \file  bulk_encryption_interface.h
 *        \brief  This file contains the interface class for bulk encryption
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_BULK_ENCRYPTION_BULK_ENCRYPTION_INTERFACE_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_BULK_ENCRYPTION_BULK_ENCRYPTION_INTERFACE_H_

#include "tls/internal/record_protocol/record_protocol_common_types.h"

namespace amsr {
namespace tls {
namespace internal {
namespace record_protocol {
namespace bulk_encryption {

/*!
 * \brief BulkEncryptionInterface
 * \vprivate This class is component private.
 */
class BulkEncryptionInterface {
 public:
  /*! \brief Virtual destructor. */
  virtual ~BulkEncryptionInterface() = default;
  /*! \brief Default constructor. */
  BulkEncryptionInterface() = default;
  /*! \brief Default copy constructor deleted. */
  BulkEncryptionInterface(BulkEncryptionInterface const &) = delete;
  /*! \brief Default move constructor deleted. */
  BulkEncryptionInterface(BulkEncryptionInterface &&) = delete;
  /*! \brief Default copy assignment operator deleted. */
  void operator=(BulkEncryptionInterface const &) = delete;
  /*! \brief Default move assignment operator deleted. */
  void operator=(BulkEncryptionInterface &&) = delete;

  /*!
   * \brief Initializes the underlying encryption context.
   * \param[in] sp The security parameters which contains the encryption key.
   * \error TlsCommErrorCode::kTlsCryptoAdapterFailure for any crypto adapter error.
   * \return Nothing on success.
   * \context ANY|!InitPhase
   * \reentrant FALSE
   * \synchronous TRUE
   */
  virtual amsr::core::Result<void> InitializeEncryptor(SecurityParameters const &sp) noexcept = 0;

  /*!
   * \brief Initializes the underlying decryption context.
   * \param[in] sp The security parameters which contains the encryption key.
   * \error TlsCommErrorCode::kTlsCryptoAdapterFailure for any crypto adapter error.
   * \return Nothing on success.
   * \context ANY|!InitPhase
   * \reentrant FALSE
   * \synchronous TRUE
   */
  virtual amsr::core::Result<void> InitializeDecryptor(SecurityParameters const &sp) noexcept = 0;

  /*!
   * \brief Decrypt a Cipher text
   * \param[in] cipher_text The cipher text to be decrypted
   * \param[in] sp Const reference to current security parameters
   * \return Returns a unique pointer with a CompressedText after successful decryption, otherwise nullptr
   * \context ANY|!InitPhase
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace SPEC-6493125
   */
  virtual CompressedTextUptr Decrypt(CipherTextUptr &&cipher_text, SecurityParameters const &sp) noexcept = 0;

  /*!
   * \brief Encrypt a Compressed text
   * \param[in] comp_text The compressed text to be encrypted
   * \param[in] sp Const reference to current security parameters
   * \return Returns a unique pointer with a CipherText after successful encryption, otherwise nullptr
   * \context ANY|!InitPhase
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace SPEC-6493112, SPEC-6493113, SPEC-6493114, SPEC-6493119, SPEC-6493124, SPEC-6562035, SPEC-6562038
   */
  virtual CipherTextUptr Encrypt(CompressedTextUptr &&comp_text, SecurityParameters const &sp) noexcept = 0;
};

}  // namespace bulk_encryption
}  // namespace record_protocol
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_BULK_ENCRYPTION_BULK_ENCRYPTION_INTERFACE_H_
