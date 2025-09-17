/**********************************************************************************************************************
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
/*!        \file  cipher_suite_factory.h
 *         \brief This file contains the declaration of CipherSuiteFactory.
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_CIPHER_SUITE_FACTORY_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_CIPHER_SUITE_FACTORY_H_

#include "tls/internal/record_protocol/cipher_suite.h"
#include "tls/public/config/psk_config.h"
#include "tls_crypto_adapter/internal/crypto_adapter_interface.h"

namespace amsr {
namespace tls {
namespace internal {
namespace record_protocol {

/*!
 * \brief CipherSuiteFactory creates CipherSuite instances.
 */
class CipherSuiteFactory final {
 public:
  /*!
   * \brief Constructor.
   * \param[in] config The current config.
   * \param[in] crypto_adapter The current crypto adapter.
   * \param[in] psk_config The pre-shared key configuration.
   */
  explicit CipherSuiteFactory(tls::TlsCommConfigurationInformation const &config,
                              tls_crypto_adapter::internal::CryptoAdapterInterface &crypto_adapter,
                              config::PskConfig const &psk_config) noexcept;

  /*!
   * \brief Creates a new CipherSuite for a supported cipher suite value.
   * \param[in] cipher_suite The cipher suite to create.
   * \return A new CipherSuite instance.
   */
  CipherSuite CreateCipherSuite(SupportedCipherSuites const cipher_suite) const noexcept;

 private:
  /*!
   * \brief Creates a new CipherSuite for TLS_NULL_WITH_NULL_NULL
   * \return A new CipherSuite instance.
   */
  static CipherSuite CreateNullWithNullNull() noexcept;
  /*!
   * \brief Creates a new CipherSuite for TLS_PSK_WITH_NULL_SHA256
   * \return A new CipherSuite instance.
   */
  CipherSuite CreatePSKWithNullSha256() const noexcept;
  /*!
   * \brief Creates a new CipherSuite for TLS_PSK_WITH_AES_128_GCM_SHA256
   * \return A new CipherSuite instance.
   */
  CipherSuite CreatePSKWithAes128GcmSha256() const noexcept;
  /*!
   * \brief Creates a new CipherSuite for TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256
   * \return A new CipherSuite instance.
   */
  CipherSuite CreateEcdheEcdsaWithAes128GcmSha256() const noexcept;
  /*!
   * \brief Creates a new CipherSuite for TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384
   * \return A new CipherSuite instance.
   */
  CipherSuite CreateEcdheEcdsaWithAes256GcmSha384() const noexcept;
  /*!
   * \brief Creates a new CipherSuite for TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256
   * \return A new CipherSuite instance.
   */
  CipherSuite CreateEcdheEcdsaWithAes128CbcSha256() const noexcept;
  /*!
   * \brief Creates a new CipherSuite for TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384
   * \return A new CipherSuite instance.
   */
  CipherSuite CreateEcdheEcdsaWithAes256CbcSha384() const noexcept;
  /*!
   * \brief Creates a new CipherSuite for TLS_ECDHE_ECDSA_WITH_NULL_SHA1
   * \return A new CipherSuite instance.
   */
  CipherSuite CreateEcdheEcdsaWithNullSha1() const noexcept;

  /*!
   * \brief A TLS configuration.
   */
  tls::TlsCommConfigurationInformation const config_;
  /*!
   * \brief The current crypto adapter.
   */
  tls_crypto_adapter::internal::CryptoAdapterInterface &crypto_adapter_;
  /*!
   * \brief The pre-shared key configuration.
   */
  config::PskConfig const &psk_config_;
};

}  // namespace record_protocol
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_CIPHER_SUITE_FACTORY_H_
