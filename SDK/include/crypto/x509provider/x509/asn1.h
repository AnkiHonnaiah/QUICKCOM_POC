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
/*!        \file  asn1.h
 *        \brief  ASN.1 related helper functions.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_ASN1_H_
#define LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_ASN1_H_
/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/asn1_der_parser.h"
#include "amsr/asn1/builder/certification_request_builder.h"
#include "amsr/asn1/builder/common/name_builder_state.h"
#include "amsr/asn1/structure/certification_request.h"
#include "amsr/asn1/structure/extension.h"
#include "amsr/asn1/structure/general_name.h"
#include "amsr/asn1/structure/key_usage.h"
#include "amsr/asn1/structure/name.h"
#include "amsr/asn1/structure/subject_public_key_info.h"
#include "amsr/asn1/structure/validity.h"
#include "ara/core/result.h"
#include "ara/core/string_view.h"
#include "ara/crypto/common/base_id_types.h"

namespace crypto {
namespace x509provider {
namespace x509 {
namespace asn1 {

/*!
 * \brief       Get the (first) extension with matching OID from the ExtensionsInfo structure if present.
 * \tparam      T Expected type of the extension structure to be returned.
 * \param[in]   extensions
 *              Extensions structure to search
 * \param[in]   extension_oid
 *              Extension OID to look for
 * \threadsafe  TRUE iff parameter \c extensions_info is not accessed simultaneously in another thread
 * \context     ANY
 * \pre         -
 * \error       ara::crypto::SecurityErrc::kInvalidArgument
 *              If no extension with OID \c extension_oid exists in \c extension_info
 * \reentrant   TRUE
 * \synchronous TRUE
 * \vprivate    Component Private
 */
template <typename T>
auto GetExtension(ara::core::Vector<amsr::asn1::structure::Extension> const& extensions,
                  amsr::core::StringView extension_oid) noexcept -> amsr::core::Result<T const*> {
  amsr::core::Result<T const*> result{ara::crypto::SecurityErrc::kInvalidArgument};

  for (ara::core::Vector<amsr::asn1::structure::Extension>::const_reference extension : extensions) {
    if (extension.ExtnID() == extension_oid) {
      if (T const* ext{extension.ExtnValue().get_if<T>()}) {
        result.EmplaceValue(ext);
        break;
      }
    }
  }

  return result;
}

/*!
 * \brief       Extract the signature from ASN.1 bit string structure.
 * \param[in]   bit_string A ASN.1 bit string structure
 * \param[in]   signature_id Signature algorithm NID
 * \return      Vector of bytes containing the extracted signature
 * \context     ANY
 * \pre         -
 * \threadsafe  TRUE iff parameters \c bit_string & \c output are not accessed simultaneously in another thread
 * \error       ara::crypt::SecurityErrc::kInvalidArgument
 *              If given \c bit_string could not be parsed
 * \error       ara::crypt::SecurityErrc::kUnsupported
 *              If given \c signature_id is not supported
 * \reentrant   TRUE
 * \synchronous TRUE
 * \vprivate    Product Private
 */
auto ParseSignature(amsr::asn1::structure::BitString const& bit_string, ara::crypto::CryptoAlgId signature_id) noexcept
    -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

/*!
 * \brief       Extract the ECDSA signature from ASN.1 bit string structure.
 * \param[in]   bit_string A ASN.1 bit string structure containing an ECDSA signature
 * \return      Vector of bytes containing the extracted signature
 * \context     ANY
 * \pre         -
 * \threadsafe  TRUE iff parameters \c bit_string is not accessed simultaneously in another thread
 * \error       ara::crypt::SecurityErrc::kInvalidArgument
 *              If given \c bit_string could not be parsed as ECDSA signature
 * \reentrant   TRUE
 * \synchronous TRUE
 * \vprivate    Product Private
 */
auto ParseSignatureEcdsa(amsr::asn1::structure::BitString const& bit_string) noexcept
    -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

/*!
 * \brief       Convert the given data to 32-bit unsigned integer.
 * \details     Abundant bytes are ignored, missing bytes are treated as zeroes.
 * \param[in]   data
 *              Data to be converted
 * \return      32-bit unsigned integer representation of \c data
 * \context     ANY
 * \pre         -
 * \threadsafe  TRUE
 * \reentrant   TRUE
 * \synchronous TRUE
 * \vprivate    Component Private
 */
auto ParseAsn1Integer(ara::core::Vector<std::uint8_t> const& data) noexcept -> std::uint32_t;

/*!
 * \brief       Parse ASN.1 time in UTC format to a timestamp.
 * \param[in]   time ASN.1 time in UTC format
 * \return      A timestamp representing the parsed time point
 * \context     ANY
 * \pre         -
 * \threadsafe  TRUE
 * \error       ara::crypt::SecurityErrc::kInvalidArgument
 *              If given string view could not be parsed to a time point
 * \reentrant   TRUE
 * \synchronous TRUE
 * \vprivate    Component Private
 */
auto ParseTime(amsr::asn1::structure::UtcTime const& time) noexcept -> amsr::core::Result<time_t>;

/*!
 * \brief       Parse ASN.1 time in GeneralizedTime format to a timestamp.
 * \param[in]   time ASN.1 time in GeneralizedTime format
 * \return      A timestamp representing the parsed time point
 * \context     ANY
 * \pre         -
 * \threadsafe  TRUE
 * \error       ara::crypt::SecurityErrc::kInvalidArgument
 *              If given string view could not be parsed to a time point
 * \reentrant   TRUE
 * \synchronous TRUE
 * \vprivate    Component Private
 */
auto ParseTime(amsr::asn1::structure::GeneralizedTime const& time) noexcept -> amsr::core::Result<time_t>;

/*!
 * \brief       Parse ASN.1 time type variant to a timestamp.
 * \param[in]   time ASN.1 time type variant object
 * \return      A timestamp representing the parsed time point
 * \context     ANY
 * \pre         -
 * \threadsafe  TRUE
 * \error       ara::crypt::SecurityErrc::kInvalidArgument
 *              If given string view could not be parsed to a time point
 * \reentrant   TRUE
 * \synchronous TRUE
 * \vprivate    Component Private
 */
auto ParseTime(amsr::asn1::structure::Validity::Time time) noexcept -> amsr::core::Result<time_t>;

}  // namespace asn1
}  // namespace x509
}  // namespace x509provider
}  // namespace crypto

#endif  //  LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_ASN1_H_
