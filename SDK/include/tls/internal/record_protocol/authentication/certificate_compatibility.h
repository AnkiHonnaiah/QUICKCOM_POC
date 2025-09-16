/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  certificate_compatibility.h
 *        \brief  Provides functions related to certificate compatibility validation.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_AUTHENTICATION_CERTIFICATE_COMPATIBILITY_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_AUTHENTICATION_CERTIFICATE_COMPATIBILITY_H_

/*!********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "ara/core/vector.h"
#include "ara/crypto/x509/certificate.h"
#include "tls/internal/messages/certificate_request_message.h"
#include "tls_crypto_adapter/internal/crypto_adapter_interface.h"

namespace amsr {
namespace tls {
namespace internal {
namespace record_protocol {
namespace authentication {

/*!
 * \brief     Indicates whether a certificate request is compatible with a collection of client certificates.
 *
 * \details   This function intentionally only performs a subset of checks required by RFC 5246. We are supposed to
 *            validate the types and signature algorithms of our certificates in response to a certificate request.
 *            Currently, we just check that the certificate request contains types and signature algorithms known to
 *            be supported by Crypto. This may change in the future. However, this function does perform limited
 *            validation of the certificate authorities.
 *
 * \param[in] certificate_provider        The certificate provider to use.
 * \param[in] certificate_request_message The certificate request message to check.
 * \param[in] certificates                The collection of client certificates.
 * \return    True if the certificate request message is compatible; false otherwise.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 */
auto IsCompatible(tls_crypto_adapter::internal::CertificateProviderInterface& certificate_provider,
                  messages::CertificateRequestMessage const& certificate_request_message,
                  ara::core::Vector<ara::crypto::x509::Certificate::Uptr> const& certificates) noexcept -> bool;

}  // namespace authentication
}  // namespace record_protocol
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_AUTHENTICATION_CERTIFICATE_COMPATIBILITY_H_
