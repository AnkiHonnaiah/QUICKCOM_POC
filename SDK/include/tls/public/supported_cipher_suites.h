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
/*!        \file  supported_cipher_suites.h
 *        \brief  This file contains the SupportedCipherSuites enum defining which cipher suites are supported by
 *                libTLS.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_PUBLIC_SUPPORTED_CIPHER_SUITES_H_
#define LIB_LIBTLS_INCLUDE_TLS_PUBLIC_SUPPORTED_CIPHER_SUITES_H_

#include <cstdint>

namespace amsr {
namespace tls {

/*!
 * \brief Supported cipher suites.
 */
enum class SupportedCipherSuites : std::uint16_t {
  /*!
   * \brief TLS null with null null
   * \details
   *    Defined in RFC 5246 Sec. A.5.
   *    - protocol: Transport Layer Security
   *    - key exchange: none
   *    - authentication: none
   *    - encryption: none
   *    - message authentication: none
   */
  TLS_NULL_WITH_NULL_NULL = 0x0000U,

  /*!
   * \brief TLS PSK with null SHA256
   * \details
   *    Defined in RFC 5487.
   *    - protocol: Transport Layer Security
   *    - key exchange: Pre-Shared Key
   *    - authentication: Pre-Shared Key
   *    - encryption: none
   *    - message authentication: Secure Hash Algorithm 256
   *
   * \trace SPEC-6181151
   * \trace SPEC-6562824
   * \trace SPEC-6562850
   */
  TLS_PSK_WITH_NULL_SHA256 = 0x00B0U,

  /*!
   * \brief TLS PSK with AES 128 GCM SHA256
   * \details
   *    Defined in RFC 5487.
   *    - protocol: Transport Layer Security
   *    - key exchange: Pre-Shared Key
   *    - authentication: Pre-Shared Key
   *    - encryption: Advanced Encryption Standard with 128bit key in Galois/Counter Mode
   *    - message authentication: Secure Hash Algorithm 256
   *
   * \trace SPEC-6181147
   * \trace SPEC-6562836
   * \trace SPEC-6562842
   */
  TLS_PSK_WITH_AES_128_GCM_SHA256 = 0x00A8U,

  /*!
   * \brief TLS ECDHE ECDSA with null SHA1
   * \details
   *    Defined in RFC 4492 Sec. 6.
   *    - protocol: Transport Layer Security
   *    - key exchange: Elliptic Curve Diffie-Hellman Ephemeral
   *    - authentication: Elliptic Curve Digital Signature Algorithm
   *    - encryption: none
   *    - message authentication: Secure Hash Algorithm 1
   */
  TLS_ECDHE_ECDSA_WITH_NULL_SHA1 = 0xC006U,

  /*!
   * \brief TLS ECDHE ECDSA with AES 128 GCM SHA256
   * \details
   *    Defined in RFC 5289 Sec. 3.2.
   *    - protocol: Transport Layer Security
   *    - key exchange: Elliptic Curve Diffie-Hellman Ephemeral
   *    - authentication: Elliptic Curve Digital Signature Algorithm
   *    - encryption: Advanced Encryption Standard with 128bit key in Galois/Counter Mode
   *    - message authentication: Secure Hash Algorithm 256
   */
  TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256 = 0xC02BU,

  /*!
   * \brief TLS ECDHE ECDSA with AES 256 GCM SHA384
   * \details
   *    Defined in RFC 5289 Sec. 3.2.
   *    - protocol: Transport Layer Security
   *    - key exchange: Elliptic Curve Diffie-Hellman Ephemeral
   *    - authentication: Elliptic Curve Digital Signature Algorithm
   *    - encryption: Advanced Encryption Standard with 256bit key in Galois/Counter Mode
   *    - message authentication: Secure Hash Algorithm 384
   */
  TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384 = 0xC02CU,

  /*!
   * \brief TLS ECDHE ECDSA with AES 128 CBC SHA256
   * \details
   *    Defined in RFC 5289 Sec. 3.1.
   *    - protocol: Transport Layer Security
   *    - key exchange: Elliptic Curve Diffie-Hellman Ephemeral
   *    - authentication: Elliptic Curve Digital Signature Algorithm
   *    - encryption: Advanced Encryption Standard with 128bit key in Cipher Block Chaining mode
   *    - message authentication: Secure Hash Algorithm 256
   */
  TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256 = 0xC023U,

  /*!
   * \brief TLS ECDHE ECDSA with AES 256 CBC SHA384
   * \details
   *    Defined in RFC 5289 Sec. 3.1.
   *    - protocol: Transport Layer Security
   *    - key exchange: Elliptic Curve Diffie-Hellman Ephemeral
   *    - authentication: Elliptic Curve Digital Signature Algorithm
   *    - encryption: Advanced Encryption Standard with 256bit key in Cipher Block Chaining mode
   *    - message authentication: Secure Hash Algorithm 384
   */
  TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384 = 0xC024U
};

}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_PUBLIC_SUPPORTED_CIPHER_SUITES_H_
