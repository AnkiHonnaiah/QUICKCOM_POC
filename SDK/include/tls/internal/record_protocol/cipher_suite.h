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
/*!        \file  cipher_suite.h
 *        \brief  This file contains the definition of CipherSuite.
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_CIPHER_SUITE_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_CIPHER_SUITE_H_

#include <memory>
#include <utility>

#include "tls/internal/messages/key_exchange_algorithms/key_exchange_algorithm_interface.h"
#include "tls/internal/record_protocol/authentication/authentication_interface.h"
#include "tls/internal/record_protocol/bulk_encryption/bulk_encryption_interface.h"
#include "tls/public/supported_cipher_suites.h"

namespace amsr {
namespace tls {
namespace internal {
namespace record_protocol {

/*!
 * \brief The CipherSuite comprises a set for a key exchange algorithm, an optional
 *        authentication algorithm and the bulk encryption algorithm.
 * \details To create an instance, use the CipherSuiteFactory.
 * \vprivate This structure is component private.
 */
class CipherSuite final {
 public:
  /*!
   * \brief Constructor.
   * \param[in] cipher_suite The value for this instance.
   * \param[in,out] key_exchange The key exchange algorithm for this instance.
   * \param[in,out] authentication The authentication algorithm for this instance.
   * \param[in,out] bulk_encryption The bulk encryption algorithm for this instance.
   */
  CipherSuite(SupportedCipherSuites const cipher_suite,
              std::unique_ptr<messages::key_exchange_algorithms::KeyExchangeAlgorithmInterface> key_exchange,
              std::unique_ptr<authentication::AuthenticationInterface> authentication,
              std::unique_ptr<bulk_encryption::BulkEncryptionInterface> bulk_encryption) noexcept
      : value_(cipher_suite),
        key_exchange_(std::move(key_exchange)),
        authentication_(std::move(authentication)),
        bulk_encryption_(std::move(bulk_encryption)) {}

  CipherSuite(CipherSuite const&) = delete;

  /*!
   * \brief Default move constructor.
   * \param[in,out] other The other instance to construct from.
   */
  CipherSuite(CipherSuite&& other) noexcept = default;

  /*! Default destructor. */
  ~CipherSuite() noexcept = default;

  CipherSuite& operator=(CipherSuite const&) = delete;

  /*!
   * \brief Default move assignable.
   * \param[in,out] other The other instance to move assign from.
   * \return New instance.
   */
  CipherSuite& operator=(CipherSuite&& other) & noexcept = default;

  /*!
   * \brief Gets the underlying value of this cipher suite.
   * \return The value this CipherSuite represents.
   */
  SupportedCipherSuites Value() const noexcept { return value_; }

  /*!
   * \brief Gets access to the KeyExchange algorithm of this cipher suite.
   * \return The key exchange algorithm interface.
   */
  messages::key_exchange_algorithms::KeyExchangeAlgorithmInterface& KeyExchange() const noexcept {
    assert(key_exchange_ != nullptr);  // Must be valid if user wants to get access to this
    return *key_exchange_;
  }

  /*!
   * \brief Gets access to the Authentication algorithm of this cipher suite.
   * \return The authentication algorithm interface.
   */
  authentication::AuthenticationInterface& Authentication() const noexcept {
    assert(authentication_ != nullptr);  // Must be valid if user wants to get access to this
    return *authentication_;
  }

  /*!
   * \brief Gets access to the BulkEncryption algorithm of this cipher suite.
   * \return The bulk encryption algorithm interface.
   */
  bulk_encryption::BulkEncryptionInterface& BulkEncryption() const noexcept {
    assert(bulk_encryption_ != nullptr);  // Must be valid if user wants to get access to this
    return *bulk_encryption_;
  }

  /*!
   * \brief Check if PSK or Certificate cipher suite is used.
   * \return True if PSK based cipher suite is used, otherwise false.
   */
  constexpr bool IsPskBasedCipherSuiteUsed() const noexcept {
    return ((value_ == SupportedCipherSuites::TLS_PSK_WITH_NULL_SHA256) ||
            (value_ == SupportedCipherSuites::TLS_PSK_WITH_AES_128_GCM_SHA256));
  }

 private:
  /*!
   * \brief This cipher suite's underlying value.
   */
  SupportedCipherSuites value_;

  /*!
   * \brief Unique pointer to the key exchange algorithm interface.
   */
  std::unique_ptr<messages::key_exchange_algorithms::KeyExchangeAlgorithmInterface> key_exchange_;

  /*!
   * \brief Unique pointer to the authentication interface.
   */
  std::unique_ptr<authentication::AuthenticationInterface> authentication_;

  /*!
   * \brief Unique pointer to the bulk encryption interface.
   */
  std::unique_ptr<bulk_encryption::BulkEncryptionInterface> bulk_encryption_;
};

}  // namespace record_protocol
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_CIPHER_SUITE_H_
