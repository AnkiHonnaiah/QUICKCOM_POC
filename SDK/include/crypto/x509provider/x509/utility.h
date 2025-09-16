/**********************************************************************************************************************
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
/*!        \file  crypto/x509provider/x509/utility.h
 *        \brief  Common utility functions used throughout the x509 provider.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_UTILITY_H_
#define LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_UTILITY_H_
/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/core/result.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/common/serializable.h"
#include "crypto/softwareprovider/cryp/x509_public_key_info.h"

namespace crypto {
namespace x509provider {
namespace x509 {
namespace utility {

/*!
 * \brief       Copy the content of data to \c out if capacity is sufficient
 * \param[in]   data
 *              Data to be copied
 * \param[in]   output
 *              Output view
 * \return      The size of data
 * \context     ANY
 * \pre         -
 * \threadsafe  TRUE
 * \error       SecurityErrorDomain::kInsufficientCapacity
 *              if (output.empty() == false), but it's capacity is less than required
 * \reentrant   FALSE
 * \synchronous TRUE
 * \vprivate    Component Private
 */
auto CopyIfSufficientCapacity(ara::crypto::ReadOnlyMemRegion data, ara::crypto::WritableMemRegion output) noexcept
    -> amsr::core::Result<std::size_t>;

/*!
 * \brief       Export the given data publicly.
 * \param[out]  data
 *              Data to be exported
 * \param[out]  output
 *              The pre-allocated output buffer, can be empty if only the required size of the output buffer is needed
 * \param[in]   format_id
 *              The Crypto Provider specific identifier of the output format
 * \return      Size required for storing of the output object
 * \context     ANY
 * \pre         -
 * \threadsafe  TRUE
 * \error       SecurityErrorDomain::kInsufficientCapacity
 *              If (output.empty() == false), but it's capacity is less than required
 * \error       SecurityErrorDomain::kUnknownIdentifier
 *              If an unknown format ID was specified
 * \error       SecurityErrorDomain::kUnsupportedFormat
 *              If the specified format ID is not supported for this object type
 * \reentrant   FALSE
 * \synchronous TRUE
 * \vprivate    Component Private
 */
auto ExportPublicly(ara::crypto::ReadOnlyMemRegion data, ara::crypto::WritableMemRegion output,
                    ara::crypto::Serializable::FormatId format_id) noexcept -> ara::core::Result<std::size_t>;

/*!
 * \brief       Get the algorithm identifier of the verifier corresponding to the given signature algorithm identifier.
 * \param[in]   algorithm_id
 *              Signature algorithm identifier
 * \return      An algorithm identifier of the verifier corresponding to the given signature algorithm or
 *              kAlgIdNone if given \c algorithm_id does not belong to a known signature algorithm
 * \context     ANY
 * \pre         -
 * \threadsafe  TRUE
 * \reentrant   TRUE
 * \synchronous TRUE
 * \vprivate    Component Private
 */
auto GetCorrespondingVerifier(ara::crypto::CryptoAlgId algorithm_id) noexcept -> ara::crypto::CryptoAlgId;

/*!
 * \brief       Convert the given OID string to the corresponding signature CryptoAlgId.
 * \param[in]   oid_string An OID string identifying a signature algorithm
 * \return      An algorithm identifier of the signature algorithm corresponding to the OID or
 *              kAlgIdNone if no match was found
 * \context     ANY
 * \pre         -
 * \threadsafe  TRUE
 * \reentrant   TRUE
 * \synchronous TRUE
 * \vprivate    Product Private
 */
auto ToSignatureAlgorithmId(amsr::core::StringView oid_string) noexcept -> ara::crypto::CryptoAlgId;

/*!
 * \brief       Verify a signature.
 * \param[in]   crypto_provider
 *              The crypto provide in which realm to create the X509PublicKeyInfo object
 * \param[in]   public_key
 *              Public key to be used for signature verification
 * \param[in]   to_be_signed
 *              The input data used for signature generation
 * \param[in]   signature
 *              The expected signature value
 * \param[in]   signature_algorithm
 *              The expected signature value
 * \return      True if signature verified correctly, false otherwise
 * \context     ANY
 * \pre         -
 * \threadsafe  FALSE
 * \error       ara::crypto::SecurityErrc::kRuntimeFault
 *              If an error occurred during verification process
 * \reentrant   FALSE
 * \synchronous TRUE
 * \vprivate    Component Private
 */
auto VerifySignature(ara::crypto::cryp::CryptoProvider& crypto_provider, ara::crypto::cryp::PublicKey const& public_key,
                     ara::crypto::ReadOnlyMemRegion to_be_signed, ara::crypto::ReadOnlyMemRegion signature,
                     ara::crypto::CryptoAlgId signature_algorithm) noexcept -> amsr::core::Result<bool>;

/*!
 * \brief       Get the OID name of the related domain parameters
 * \param[in]   alg_id
 *              Algorithm ID the related domain parameters shall be retrieved for
 * \return      The domain parameters OID string
 * \context     ANY
 * \pre         -
 * \threadsafe  FALSE
 * \error       ara::crypto::SecurityErrc::kUnknownIdentifier
 *              If no related domain parameters for given \c alg_id known
 * \reentrant   FALSE
 * \synchronous TRUE
 * \vprivate    Component Private
 */
auto GetRelatedDomainParametersOidName(ara::crypto::CryptoAlgId alg_id) noexcept -> amsr::core::String;

}  // namespace utility
}  // namespace x509
}  // namespace x509provider
}  // namespace crypto

#endif  //  LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_UTILITY_H_
