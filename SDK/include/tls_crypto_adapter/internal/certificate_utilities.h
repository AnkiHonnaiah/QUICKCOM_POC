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
/*!        \file  certificate_utilities.h
 *        \brief  Provides utilities related to certificates.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_CERTIFICATE_UTILITIES_H_
#define LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_CERTIFICATE_UTILITIES_H_

#include <cstdint>  // uint8_t

#include "ara/core/result.h"
#include "ara/core/span.h"
#include "ara/core/string.h"
#include "ara/core/vector.h"
#include "ara/crypto/x509/certificate.h"
#include "ara/crypto/x509/x509_dn.h"
#include "tls_crypto_adapter/internal/crypto_adapter_interface.h"

namespace amsr {
namespace tls_crypto_adapter {
namespace internal {

/*!
 * \brief     Attempts to load a collection of certificates using the given certificate provider.
 *
 * \param[in] provider The certificate provider used to load the certificates.
 * \param[in] labels   The labels of the certificates that should be loaded.
 * \return    The loaded certificates if successful; an error code otherwise.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 */
auto LoadCertificates(CertificateProviderInterface const& provider,
                      ara::core::Span<ara::core::String const> labels) noexcept
    -> ara::core::Result<ara::core::Vector<ara::crypto::x509::Certificate::Uptr>>;

/*!
 * \brief     Attempts to serialize a single certificate.
 *
 * \param[in] certificate The certificate that should be serialized.
 * \return    The serialized certificate if successful; an error code otherwise.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 */
auto SerializeCertificate(ara::crypto::x509::Certificate const& certificate) noexcept
    -> ara::core::Result<ara::core::Vector<std::uint8_t>>;

/*!
 * \brief     Converts a distinguished name to a string.
 *
 * \param[in] distinguished_name The distinguished name to convert.
 * \return    The distinguished name as a string.
 *
 * \pre       -
 * \context   ANY
 * \reentrant FALSE
 */
auto ToString(ara::crypto::x509::X509DN const& distinguished_name) noexcept -> ara::core::Result<ara::core::String>;

}  // namespace internal
}  // namespace tls_crypto_adapter
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_CERTIFICATE_UTILITIES_H_
