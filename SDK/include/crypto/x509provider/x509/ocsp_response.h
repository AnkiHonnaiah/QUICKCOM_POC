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
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  crypto/x509provider/x509/ocsp_response.h
 *        \brief  Provides X.509 OCSP response.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_OCSP_RESPONSE_H_
#define LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_OCSP_RESPONSE_H_

/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/structure/basic_ocsp_response.h"
#include "amsr/asn1/structure/ocsp_response.h"
#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "ara/core/string.h"
#include "ara/core/string_view.h"
#include "ara/crypto/x509/ocsp_response.h"
#include "vac/testing/test_adapter.h"

#include "crypto/x509provider/x509/certificate_store_entry.h"
#include "crypto/x509provider/x509/x509_dn.h"

namespace crypto {
namespace x509provider {
namespace x509 {

/*!
 * \brief OcspResponse implementation.
 * \vprivate Product Private
 */
class OcspResponse : public ara::crypto::x509::OcspResponse {
 public:
  /*!
   * \brief Type alias for OCSP cert status enumeration.
   */
  using OcspCertStatus = ara::crypto::x509::OcspCertStatus;

  /*!
   * \brief Type alias for unique smart pointer to this class.
   */
  using Uptr = std::unique_ptr<OcspResponse, ara::crypto::CustomDeleter>;

  /*!
   * \brief Copy constructor.
   * \vprivate Component Private
   */
  OcspResponse(OcspResponse const&) = delete;

  /*!
   * \brief Deleted copy assignment.
   * \return -
   * \vprivate Component Private
   */
  auto operator=(OcspResponse const&) & -> OcspResponse& = delete;

  /*!
   * \brief Default destructor
   * \vprivate Component Private
   */
  ~OcspResponse() noexcept override = default;

  /*!
   * \brief Release allocated memory and other resources.
   * \details May be used only if the object's memory was allocated using the new operator.
           The object must not be used once the method is called.
   * \context ANY
   * \pre The object's memory is allocated with the new operator.
       The memory is not released.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void Release() noexcept override { delete this; };

  /*!
   * \brief Creator function for an OCSP reponse from a DER encoded buffer
   * \param[in] default_crypto_provider Default crypto provider
   * \param[in] ocsp_response_der DER encoded OCSP response to be parsed
   * \return newly created OcspResponse object
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kUnexpectedValue If parsing the encoded data fails
   * \vprivate Component Private
   */
  static auto Create(std::shared_ptr<ara::crypto::cryp::CryptoProvider> default_crypto_provider,
                     ara::crypto::ReadOnlyMemRegion ocsp_response_der) noexcept -> ara::core::Result<Uptr>;

  /*!
   * \brief Serialize itself publicly.
   * \param[out] output  The preallocated output buffer (it can be empty if only the required
   *             size of the output buffer is interested).
   * \param[in] format_id  The Crypto Provider specific identifier of the output format.
   * \return Size required for storing of the output object.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kInsufficientCapacity
   *         if (output.empty() == false), but its capacity is less than required
   * \error ara::crypto::SecurityErrc::kUnsupportedFormat
   *         if the specified format ID is not supported for this object type
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  // VECTOR NC VectorC++-V8.3.1: MD_CRYPTO_VectorC++-V8.3.1_AUTOSAR
  auto ExportPublicly(
      // VECTOR NL AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
      ara::crypto::WritableMemRegion output = ara::crypto::WritableMemRegion(),
      // VECTOR NL AutosarC++17_10-M8.3.1: MD_CRYPTO_AutosarC++17_10-M8.3.1_namespace
      ara::crypto::Serializable::FormatId format_id = ara::crypto::Serializable::kFormatDefault) const noexcept
      -> ara::core::Result<std::size_t> override;

  /*!
   * \brief Get version of the OCSP response format.
   * \return OCSP response format version.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto Version() const noexcept -> std::uint32_t override;

  /*!
   * \brief Gets the hash algorithm id corresponding to the hash algorithm defined in the certificate identified by
   *        given serial number.
   * \param[in] cert_serial_number Certificate serial number
   * \return Hash algorithm name if the serial number is found
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrc::kInvalidArgument If no matching entry was found with serial number == \c cert_serial_number
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetHashAlgorithmId(ara::crypto::ReadOnlyMemRegion cert_serial_number) const noexcept
      -> amsr::core::Result<ara::crypto::CryptoAlgId>;

  /*!
   * \brief Indicates whether this response's \c responderID value is a name or a key.
   * \return true if the \c responderID field is defined by name, false if by key.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error SecurityErrc::kRuntimeFault If neither option is true. The responderID field is not optional.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto IsResponderIdByName() const noexcept -> ara::core::Result<bool>;

  /*!
   * \brief Get the responder dn
   * \return The responder DN
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  auto GetResponderDn() const noexcept -> amsr::core::Optional<std::reference_wrapper<X509DN const>>;

  /*!
   * \brief Get the responder key id.
   * \param[in,out] responder_key_id a buffer for holding the responder key id
   * \return The size of the responder key id
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error SecurityErrc::kRuntimeFault If the ocsp response doesn't contain one
   * \error SecurityErrc::kInsufficientCapacity if the buffer is too small to hold the responder key id
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  virtual auto GetResponderKeyId(ara::crypto::WritableMemRegion responder_key_id) const noexcept
      -> amsr::core::Result<std::size_t>;

  /*!
   * \brief Get the crypto provider.
   * \return The crypto provider.
   */
  auto GetCryptoProvider() const noexcept -> std::shared_ptr<ara::crypto::cryp::CryptoProvider>;

  /*!
   * \brief Returns the DER-encoded tbsResponseData field (which is the byte vector to be signed).
   * \return The DER-encoded ASN.1 representation of the OCSP response data to be signed (including T and L)
   * \error SecurityErrc::kRuntimeFault If retrieval of internal data structures failed.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error SecurityErrc::kRuntimeFault TBS response data could not be encoded.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetTbsResponseData() const noexcept -> amsr::core::Result<ara::core::Vector<std::uint8_t>>;

  /*!
   * \brief Returns signature field of the (basic) OCSP response.
   * \return The signature of the OCSP response
   * \error SecurityErrc::kRuntimeFault If retrieval of internal data structures failed.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetSignature() const noexcept -> ara::crypto::ReadOnlyMemRegion;

  /*!
   * \brief Look up the CertStatus of a SingleResponse identified by CertID.
   * \param[in] client_cert The client certificate (the subject of the single response)
   * \param[in] issuer_cert The issuer certificate (that issued <tt>client_cert</tt>)
   * \return The CertStatus as per this OCSP response for a given certificate
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error SecurityErrc::kRuntimeFault If retrieval of internal data structures failed.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetCertStatusForCertId(Certificate const& client_cert, Certificate const& issuer_cert) const noexcept
      -> ara::core::Result<OcspCertStatus>;

  /*!
   * \brief Hash a public key.
   * \param hash_ctx The pre-created, unstarted hashing context
   * \param pub_key The public key to hash
   * \return The digest of the passed public key
   * \error SecurityErrc::kRuntimeFault If hashing fails.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto HashPublicKey(ara::crypto::cryp::HashFunctionCtx::Uptr hash_ctx,
                            ara::crypto::cryp::PublicKey::Uptrc pub_key) noexcept
      -> ara::core::Result<ara::core::Vector<std::uint8_t>>;

 protected:
  /*!
   * \brief Default move constructor.
   * \vprivate Component Private
   */
  OcspResponse(OcspResponse&&) noexcept = default;

  /*!
   * \brief Default move assignment.
   * \return reference to this
   * \vprivate Component Private
   */
  auto operator=(OcspResponse&&) & noexcept -> OcspResponse& = default;

 private:
  /*!
   * \brief Constructs an OCSP response instance
   * \param[in] default_crypto_provider Default crypto provider.
   * \vprivate Component Private
   */
  explicit OcspResponse(std::shared_ptr<ara::crypto::cryp::CryptoProvider> default_crypto_provider) noexcept;

  /*!
   * \brief Hash a memory region given a certain algorithm.
   * \param[in] to_be_hashed The memory region to be hashed
   * \param[in] hash_alg_id The hashing algorithm to use
   * \return The hashed data
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error SecurityErrc::kRuntimeFault If there was a failure with the internal crypto provider calls.
   * \reentrant FALSE
   * \synchronous TRUE
   */
  auto HashMemRegion(ara::crypto::ReadOnlyMemRegion to_be_hashed,
                     ara::crypto::cryp::CryptoProvider::AlgId hash_alg_id) const
      -> ara::core::Result<ara::core::Vector<std::uint8_t>>;

  /*!
   * \brief Get the cert status for certificate with given identifying hash triple.
   * \param[in] name_hash Hash of the certificate's issuer distinguished name
   * \param[in] key_hash Hash of the certificate's public key
   * \param[in] serial_number Serial number of the certificate
   * \return The corresponding OCSP cert status
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error SecurityErrc::kInvalidArgument If no matching entry was found
   * \reentrant FALSE
   * \synchronous TRUE
   */
  auto GetCertStatusForCertId(ara::crypto::ReadOnlyMemRegion name_hash, ara::crypto::ReadOnlyMemRegion key_hash,
                              ara::crypto::ReadOnlyMemRegion serial_number) const noexcept
      -> amsr::core::Result<OcspCertStatus>;

  std::shared_ptr<ara::crypto::cryp::CryptoProvider> default_crypto_provider_; /*!< Default crypto provider */
  amsr::asn1::structure::OcspResponse asn1_ocsp_response_;                     /*!< OcspResponse ASN.1 object */
  amsr::core::Optional<X509DN::Uptr>
      responder_dn_; /*!< Optional pointer to an X509DN instance representing the responder name */
  ara::core::Vector<std::uint8_t> der_encoded_; /*!< DER encoded binary representation of this object */
  crypto::common::util::Logger logger_;         /*!< Logging instance */

  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__x509provider__x509__OcspResponse, HashMemRegion__Error);
};

}  // namespace x509
}  // namespace x509provider
}  // namespace crypto

#endif  //  LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_OCSP_RESPONSE_H_
