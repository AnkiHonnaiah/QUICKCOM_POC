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
/*!        \file
 *         \brief  Public data types.
 *         \unit   SecCom::LibTLS::TlsProvider
 *
 *********************************************************************************************************************/
#ifndef LIB_TLS_PROVIDER_COMMON_INCLUDE_AMSR_TLS_DATA_TYPES_H_
#define LIB_TLS_PROVIDER_COMMON_INCLUDE_AMSR_TLS_DATA_TYPES_H_
/*!********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

namespace amsr {
namespace tls {

/*!
 * \vprivate Product private.
 * \brief TLS protocol version.
 */
enum class TLSProtocolVersion : std::uint16_t {
  kTls1Point2, /*! Version 1.2 */
  kTls1Point3  /*! Version 1.3 */
};

/*!
 * \vprivate Product private.
 * \brief Cipher suites.
 */
enum class CipherSuite : std::uint16_t {
  kTlsAes256GcmSha384 = 0x1302U /*! "TLS_AES_256_GCM_SHA384" */
};

/*!
 * \vprivate Product private.
 * \brief States of a TLS provider.
 */
enum class ProviderState : std::uint8_t { kDisconnected, kConnected, kAlertDisconnect };

}  // namespace tls
}  // namespace amsr

#endif  // LIB_TLS_PROVIDER_COMMON_INCLUDE_AMSR_TLS_DATA_TYPES_H_
