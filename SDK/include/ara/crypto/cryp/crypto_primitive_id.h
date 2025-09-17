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
/*!        \file  ara/crypto/cryp/crypto_primitive_id.h
 *        \brief  Common interface for identification of all Crypto Primitives and their keys & parameters.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_CRYPTO_PRIMITIVE_ID_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_CRYPTO_PRIMITIVE_ID_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/string_view.h"
#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/custom_disposable.h"
#include "ara/crypto/common/std_api.h"

namespace ara {
namespace crypto {
namespace cryp {

/*! Forward declaration of the Crypto Provider interface. */
class CryptoProvider;

/*!
 * \brief Common interface for identification of all Crypto Primitives and their keys & parameters.
 * \vpublic
 */
class CryptoPrimitiveId : public CustomDisposable {
 public:
  /*!
   * \brief Type definition of vendor specific binary Crypto Primitive ID.
   * \vpublic
   */
  using AlgId = CryptoAlgId;

  /*!
   * \brief Enumeration of categories of all supported crypto primitives.
   * \vpublic
   */
  enum class Category : std::uint8_t {
    kUnknown = 0,              // A value reserved for erroneous situations
    kGenericSymmetricKey = 1,  // Generic set of symmetric key primitives (Symmetric Key interface). This category is
                               // applicable to key objects only!
    kGenericAsymmetricDlp =
        2,  // Generic set of public / private key primitives based on the Discrete Logarithm Problem (DLP), i.e.
            // interfaces: PublicKey / PrivateKey. This category is applicable to key objects only!
    kGenericAsymmetricIfp =
        3,  // Generic set of public / private key primitives based on the Integer Factoring Problem (IFP), i.e.
            // interfaces: PublicKey / PrivateKey. This category is applicable to key objects only!
    kHashFunction = 4,  // Keyless hash function primitives (HashFunctionCtx interface)
    kKeyDerivationFunction =
        5,  // Keyless key derivation function (KDF) primitives (KeyDerivationFunctionCtx interface).
    kSymmetricBlockCipher = 6,   // Symmetric symmetric block cipher primitives (SymmetricBlockCipherCtx interface).
    kSymmetricStreamCipher = 7,  // Symmetric stream cipher primitives (StreamCipher Ctx interface).
    kSymmetricAuthentication =
        8,  // Symmetric message authentication code (MAC) primitives (MessageAuthnCodeCtx interface).
    kAuthenticStreamCipher = 9,  // Symmetric authenticated stream cipher primitives (AuthnStreamCipherCtx interface).
    kKeyDiversification = 10,    // Symmetric key diversifier primitives (KeyDiversifierCtx interface).
    kSymmetricKeyWrap = 11,      // Symmetric symmetric key wrapping primitives (SymmetricKeyWrapCtx interface).
    kRandomGenerator = 12,       // Random number generator (RNG) primitives (RandomGeneratorCtx interface).
    kKeyAgreementDlp = 13,      // Asymmetric key agreement primitives, based on the DLP (KeyAgreePrivateCtx interface).
    kDigitalSignatureDlp = 14,  // Asymmetric signature primitives, based on the DLP. Signature calculation and
                                // verification interfaces: SignerPrivateCtx / VerifierPublicCtx.
    kSignatureEncoderIfp =
        15,  // Asymmetric signature encoding primitives with message recovery, based on the IFP. Signature
             // calculation and message recovery interfaces: Sig EncodePrivateCtx / MsgRecoveryPublicCtx.
    kAsymmetricCipherIfp = 16,  // Asymmetric cipher primitives, based on the IFP. Encryption / decryption
                                // interfaces:
                                // EncryptPublicCtx / DecryptPrivateCtx.
    kKeyEncapsulationIfp = 17   // Asymmetric key encapsulation primitives, based on the IFP. Encapsulation /
                                // Decapsulation interfaces: KeyEncapsulatePublicCtx / KeyDecapsulatePrivateCtx
  };

  /*!
   * \brief Gets a unified name of the primitive.
   * \details The crypto primitive name can be fully or partially specified.
   * \return The unified name of the crypto primitive.
   * \vpublic
   */
  virtual ara::core::StringView const GetPrimitiveName() const noexcept = 0;

  /*!
   * \brief Gets vendor specific ID of the primitive.
   * \return The binary Crypto Primitive ID.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual AlgId GetPrimitiveId() const noexcept = 0;

  /*!
   * \brief Gets the category of the primitive.
   * \return The category of the primitive.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual Category GetCategory() const noexcept = 0;

  /*!
   * \brief Gets a reference to Crypto Provider of this primitive.
   * \return A reference to Crypto Provider instance that provides this primitive.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual CryptoProvider& MyProvider() const noexcept = 0;
};

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_CRYPTO_PRIMITIVE_ID_H_
