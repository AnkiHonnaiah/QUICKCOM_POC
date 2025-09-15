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
/*!
 * \file crypto_adapter_interface.h
 *
 * \brief Contains the Crypto Adapter Interface
 *
 * \details The file includes several interfaces that are being used by the TLS library
 *          to make use of Crypto Adapter Library functionality for message encryption/decryption.
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_CRYPTO_ADAPTER_INTERFACE_H_
#define LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_CRYPTO_ADAPTER_INTERFACE_H_

#include <cstddef>
#include <memory>
#include <utility>

#include "amsr/core/result.h"
#include "amsr/core/string.h"
#include "amsr/core/string_view.h"
#include "ara/crypto/keys/key_storage_provider.h"
#include "ara/crypto/x509/certificate.h"
#include "ara/crypto/x509/entry_point.h"
#include "ara/crypto/x509/x509_dn.h"
#include "crypto_adapter_error_definitions.h"
#include "crypto_adapter_types.h"

namespace amsr {
namespace tls_crypto_adapter {
namespace internal {

/*!
 * \brief Interface for a generic hash function context.
 */
class HashInterface {
 public:
  /*! \brief Default constructor.*/
  HashInterface() = default;

  /*! \brief Not copy constructible. */
  HashInterface(HashInterface const &) = delete;

  /*! \brief Not move constructible. */
  HashInterface(HashInterface &&) = delete;

  /*! \brief Not copy assignable. */
  HashInterface &operator=(HashInterface &&) = delete;

  /*! \brief Not move assignable. */
  HashInterface &operator=(HashInterface const &) = delete;

  /*! \brief Virtual default destructor.*/
  virtual ~HashInterface() = default;

  /*!
   * \brief Start a new hashing process.
   * \context Internal
   * \pre Update() and Finish() have not been called before making a call to Start() on the same instance
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   * \return An error code providing information about success or failure of the operation.
   */
  virtual CryptoAdapterErrorCode Start() noexcept = 0;

  /*!
   * \brief Update the current hashing process.
   * \param[in] data The input data for which the hash shall be computed.
   * \context Internal
   * \pre Start() must be called and Finish must NOT be called prior to calling Update() on the same instance
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   * \return An error code providing information about success or failure of the operation.
   */
  virtual CryptoAdapterErrorCode Update(ReadOnlyBufferView data) noexcept = 0;

  /*!
   * \brief Finishes the current hashing process.
   * \param[in] buffer_size The size of the buffer to store the result in
   * \context Internal
   * \pre Start() and Update() must be called in that order prior to calling Finish() on the same instance
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   * \return A read only view of the destination buffer area that contains the result or an error code.
   */
  virtual amsr::core::Result<ReadOnlyBufferView, CryptoAdapterErrorCode> Finish(size_t buffer_size) noexcept = 0;
};

/*!
 * \brief Interface for a MAC generator context.
 */
class MacGeneratorInterface {
 public:
  /*! \brief Default constructor. */
  MacGeneratorInterface() = default;

  /*! \brief Virtual default destructor. */
  virtual ~MacGeneratorInterface() = default;

  /*! \brief Not copy constructible */
  MacGeneratorInterface(MacGeneratorInterface const &) = delete;

  /*! \brief Not move constructible */
  MacGeneratorInterface(MacGeneratorInterface &&) = delete;

  /*! \brief Not copy assignable */
  MacGeneratorInterface &operator=(MacGeneratorInterface const &) = delete;

  /*! \brief Not move assignable */
  MacGeneratorInterface &operator=(MacGeneratorInterface &&) = delete;

  /*!
   * \brief Starts a new MAC generation process.
   * \context Internal
   * \pre Update() and Finish() have not been called before making a call to Start() on the same instance
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   * \return An error code providing information about success or failure of the operation.
   */
  virtual CryptoAdapterErrorCode Start() noexcept = 0;

  /*!
   * \brief Updates the current MAC generation process.
   * \param[in] data A chunk of input data for the MAC generation.
   * \context Internal
   * \pre Start() must be called and Finish must NOT be called prior to calling Update() on the same instance
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   * \return An error code providing information about success or failure of the operation.
   */
  virtual CryptoAdapterErrorCode Update(ReadOnlyBufferView data) noexcept = 0;

  /*!
   * \brief Finishes the current MAC generation process.
   * \param[in] buffer_size The size of the buffer to store the result.
   * \context Internal
   * \pre Start() and Update() must be called in that order prior to calling Finish() on the same instance
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   * \return A read only view onto the destination buffer area that contains the result or an error code.
   */
  virtual amsr::core::Result<ReadOnlyBufferView, CryptoAdapterErrorCode> Finish(size_t buffer_size) noexcept = 0;

  /*!
   * \brief Performs MAC generation.
   * \param[in] data A chunk of input data for the MAC generation.
   * \param[in] buffer_size The size of the buffer to store the result.
   * \context Internal
   * \pre -
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   * \return A read only view onto the destination buffer area that contains the result or an error code.
   */
  virtual amsr::core::Result<ReadOnlyBufferView> Generate(ReadOnlyBufferView data,
                                                          std::size_t buffer_size) noexcept = 0;
};

/*!
 * \brief Interface for a MAC verifier context.
 */
class MacVerifierInterface {
 public:
  /*! \brief Default constructor.*/
  MacVerifierInterface() = default;

  /*! \brief Virtual default destructor.*/
  virtual ~MacVerifierInterface() = default;

  /*! \brief Not copy constructible */
  MacVerifierInterface(MacVerifierInterface const &) = delete;

  /*! \brief Not move constructible */
  MacVerifierInterface(MacVerifierInterface &&) = delete;

  /*! \brief Not copy assignable */
  MacVerifierInterface &operator=(MacVerifierInterface const &) = delete;

  /*! \brief Not move assignable */
  MacVerifierInterface &operator=(MacVerifierInterface &&) = delete;

  /*!
   * \brief Starts a MAC verification process.
   * \context Internal
   * \pre Update() and Finish() have not been called before making a call to Start() on the same instance
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   * \return An error code providing information about success or failure of the operation.
   */
  virtual CryptoAdapterErrorCode Start() noexcept = 0;

  /*!
   * \brief Updates the current MAC verification process.
   * \param[in] data A chunk of input data for the MAC verification.
   * \context Internal
   * \pre Start() must be called and Finish() must not be called prior to calling Update() on the same instance
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   * \return An error code providing information about success or failure of the operation.
   */
  virtual CryptoAdapterErrorCode Update(ReadOnlyBufferView data) noexcept = 0;

  /*!
   * \brief Finishes the current MAC verification process.
   * \param[in] verification_tag The tag to verify against.
   * \context Internal
   * \pre Start() and Update() must be called in that order prior to calling Finish() on the same instance
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   * \return True if verification passed, false if verification failed or an error code.
   */
  virtual amsr::core::Result<bool, CryptoAdapterErrorCode> Finish(ReadOnlyBufferView verification_tag) noexcept = 0;

  /*!
   * \brief Performs MAC verification.
   * \param[in] data A chunk of input data for the MAC verification.
   * \param[in] verification_tag The tag to verify against.
   * \context Internal
   * \pre -
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   * \return True if verification passed, false if verification failed or an error code.
   */
  virtual amsr::core::Result<bool> Verify(ReadOnlyBufferView data, ReadOnlyBufferView verification_tag) noexcept = 0;
};

/*!
 * \brief Interface for a Random Number Generator (RNG) context.
 */
class RngInterface {
 public:
  /*! \brief Default constructor.*/
  RngInterface() = default;

  /*! \brief Default destructor.*/
  virtual ~RngInterface() = default;

  /*! \brief Not copy constructible */
  RngInterface(RngInterface const &) = delete;

  /*! \brief Not move constructible */
  RngInterface(RngInterface &&) = delete;

  /*! \brief Not copy assignable */
  RngInterface &operator=(RngInterface const &) = delete;

  /*! \brief Not move assignable */
  RngInterface &operator=(RngInterface &&) = delete;

  /*!
   * \brief Generates random data.
   * \param[in] destination Destination buffer view to store the generated random data. The amount of generated random
   * data is determined by the size of the buffer view.
   * \context Internal
   * \pre -
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   * \return An error code providing information about success or failure of the operation.
   */
  virtual CryptoAdapterErrorCode Generate(WriteableBufferView destination) noexcept = 0;
};

/*!
 * \brief Interface for a Pseudo Random Function (PRF) context.
 */
class PrfInterface {
 public:
  /*! \brief Default constructor.*/
  PrfInterface() = default;

  /*! \brief Default destructor.*/
  virtual ~PrfInterface() = default;

  /*! \brief Not copy constructible */
  PrfInterface(PrfInterface const &) = delete;

  /*! \brief Not move constructible */
  PrfInterface(PrfInterface &&) = delete;

  /*! \brief Not copy assignable */
  PrfInterface &operator=(PrfInterface const &) = delete;

  /*! \brief Not move assignable */
  PrfInterface &operator=(PrfInterface &&) = delete;

  /*!
   * \brief Generates random data.
   * \param[in] label PRF label.
   * \param[in] seed PRF seeding data
   * \param[in] destination Destination buffer view to store the generated data. The amount of generated
   * data is determined by the size of the buffer view.
   * \context Internal
   * \pre -
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   * \return An error code providing information about success or failure of the operation.
   */
  virtual CryptoAdapterErrorCode Generate(amsr::core::StringView label, ReadOnlyBufferView seed,
                                          WriteableBufferView destination) const noexcept = 0;
};

/*! \brief Interface for an Encryptor context. */
class EncryptorInterface {
 public:
  /*! \brief Default constructor.*/
  EncryptorInterface() = default;

  /*! \brief Default destructor.*/
  virtual ~EncryptorInterface() = default;

  /*! \brief Not copy constructible */
  EncryptorInterface(EncryptorInterface const &) = delete;

  /*! \brief Not move constructible */
  EncryptorInterface(EncryptorInterface &&) = delete;

  /*! \brief Not copy assignable */
  EncryptorInterface &operator=(EncryptorInterface const &) = delete;

  /*! \brief Not move assignable */
  EncryptorInterface &operator=(EncryptorInterface &&) = delete;

  /*!
   * \brief Encrypts the payload.
   * \param[in] nonce The nonce used for encryption.
   * \param[in] additional_data The additional data used by authentication stream ciphers.
   * \param[in] payload The payload to encrypt.
   * \return A read only buffer view for the encrypted payload.
   * \error EnCryptoAdapterErrors::kInvalidIvSize if the nonce has an invalid size in bytes.
   * \error EnCryptoAdapterErrors::kRuntimeError if a cryptographic operation failed.
   * \vprivate This method is component private.
   */
  virtual amsr::core::Result<ReadOnlyBufferView> Encrypt(ReadOnlyBufferView nonce, ReadOnlyBufferView additional_data,
                                                         ReadOnlyBufferView payload) noexcept = 0;

 protected:
  /*! Maximum size of the encryption data buffer. */
  static constexpr uint32_t const kMaxEncryptionBufferSize{2U << 14U};
};

/*!
 * \brief Interface for a decryptor context.
 */
class DecryptorInterface {
 public:
  /*! \brief Default constructor.*/
  DecryptorInterface() = default;

  /*! \brief Default destructor.*/
  virtual ~DecryptorInterface() = default;

  /*! \brief Not copy constructible */
  DecryptorInterface(DecryptorInterface const &) = delete;

  /*! \brief Not move constructible */
  DecryptorInterface(DecryptorInterface &&) = delete;

  /*! \brief Not copy assignable */
  DecryptorInterface &operator=(DecryptorInterface const &) = delete;

  /*! \brief Not move assignable */
  DecryptorInterface &operator=(DecryptorInterface &&) = delete;

  /*!
   * \brief Decrypts the payload.
   * \param[in] nonce The nonce used for decryption.
   * \param[in] additional_data The additional data used by authentication stream ciphers.
   * \param[in] payload The payload to decrypt.
   * \return A read only buffer view for the decrypted payload.
   * \error EnCryptoAdapterErrors::kInvalidIvSize if the nonce has an invalid size in bytes.
   * \error EnCryptoAdapterErrors::kRuntimeError if a cryptographic operation failed.
   * \vprivate This method is component private.
   */
  virtual amsr::core::Result<ReadOnlyBufferView> Decrypt(ReadOnlyBufferView nonce, ReadOnlyBufferView additional_data,
                                                         ReadOnlyBufferView payload) noexcept = 0;

 protected:
  /*! Maximum size of the decryption data buffer. */
  static constexpr uint32_t const kMaxDecryptionBufferSize{2U << 14U};

  /*! Maximum size of the plaintext fragments. */
  static constexpr uint32_t const kMaxPlaintextFragmentSize{2U << 14U};
};

/*!
 * \brief An interface for signature generation.
 */
class SignatureGeneratorInterface {
 public:
  /*! \brief Virtual default destructor. */
  virtual ~SignatureGeneratorInterface() noexcept = default;

  /*! \brief Default constructor. */
  SignatureGeneratorInterface() noexcept = default;

  /*! \brief No copy construction. */
  SignatureGeneratorInterface(SignatureGeneratorInterface const &) = delete;

  /*! \brief No move construction. */
  SignatureGeneratorInterface(SignatureGeneratorInterface &&) = delete;

  /*! \brief Not copy assignable. */
  SignatureGeneratorInterface &operator=(SignatureGeneratorInterface const &) = delete;

  /*! \brief Not move assignable. */
  SignatureGeneratorInterface &operator=(SignatureGeneratorInterface &&) = delete;

  /*!
   * \brief Generates a digital signature from raw data.
   * \param[in] raw_data Data needs to be singed.
   * \context Handshake
   * \pre -
   * \reentrant FALSE
   * \return Generated signature.
   */
  virtual amsr::core::Result<Signature, CryptoAdapterErrorCode> GenerateSignature(RawData const &raw_data) const = 0;
};

/*!
 * \brief An interface for signature verification.
 */
class SignatureVerifierInterface {
 public:
  /*! \brief Virtual default destructor. */
  virtual ~SignatureVerifierInterface() noexcept = default;

  /*! \brief Default constructor. */
  SignatureVerifierInterface() noexcept = default;

  /*! \brief No copy construction. */
  SignatureVerifierInterface(SignatureVerifierInterface const &) = delete;

  /*! \brief No move construction. */
  SignatureVerifierInterface(SignatureVerifierInterface &&) = delete;

  /*! \brief Not copy assignable. */
  SignatureVerifierInterface &operator=(SignatureVerifierInterface const &) = delete;

  /*! \brief Not move assignable. */
  SignatureVerifierInterface &operator=(SignatureVerifierInterface &&) = delete;

  /*!
   * \brief Verifies a signature.
   * \param[in] data Data used for the generation of the signature.
   * \param[in] signature Signature generated from the 'data'.
   * \context Handshake
   * \pre -
   * \reentrant FALSE
   * \return Generated signature.
   */
  virtual amsr::core::Result<bool, CryptoAdapterErrorCode> VerifySignature(ara::core::Vector<std::uint8_t> data,
                                                                           Signature signature) const noexcept = 0;
};

/*!
 * \brief An interface for Certificate provider context.
 */
class CertificateProviderInterface {
 public:
  /*! \brief Virtual default destructor. */
  virtual ~CertificateProviderInterface() noexcept = default;

  /*! \brief Default constructor. */
  CertificateProviderInterface() = default;

  /*! \brief No copy construction. */
  CertificateProviderInterface(CertificateProviderInterface const &) = delete;

  /*! \brief No move construction. */
  CertificateProviderInterface(CertificateProviderInterface &&) = delete;

  /*! \brief Not copy assignable. */
  CertificateProviderInterface &operator=(CertificateProviderInterface const &) = delete;

  /*! \brief Not move assignable. */
  CertificateProviderInterface &operator=(CertificateProviderInterface &&) = delete;

  /*!
   * \brief Loads certificate matching label and exports it to binary format.
   * \param[in] label Certificate label.
   * \param[out] target binary certificate target container.
   * \pre -
   * \reentrant FALSE
   * \return Result containing size of written data or error.
   */
  virtual amsr::core::Result<std::size_t, CryptoAdapterErrorCode> GetBinaryCertificate(
      amsr::core::String const &label, ara::core::Vector<std::uint8_t> &target) const noexcept = 0;

  /*!
   * \brief Parses a serialized representation of the certificate chain.
   * \param[out] parsed_certs output vector of parsed certificates.
   * \param[in] cert_chain binary certificate chain to be parsed.
   * \return Nothing if successful; an error code otherwise.
   * \pre -
   * \context Handshake
   * \reentrant FALSE
   * \error EnCryptoAdapterErrors::kRuntimeError
   * \vprivate This function is component private.
   */
  virtual amsr::core::Result<void> ParseCertificateChain(
      ara::core::Vector<ara::crypto::x509::Certificate::Uptr> &parsed_certs,
      ara::crypto::ReadOnlyMemRegion const &cert_chain) const noexcept = 0;

  /*!
   * \brief Load the root certificate and set as the root of trust.
   * \param[in] root_label label used to load the certificate.
   * \return Result contains a pointer to the root certificate if it can be set as the root of trust.
   * \pre -
   * \context Handshake
   * \reentrant FALSE
   * \error EnCryptoAdapterErrors::kRuntimeError
   * \vprivate This function is component private.
   */
  virtual amsr::core::Result<ara::crypto::x509::Certificate::Uptr, CryptoAdapterErrorCode>
  LoadRootCertificateAndSetAsRootOfTrust(amsr::core::String const &root_label) const noexcept = 0;

  /*!
   * \brief     Attempts to load a single certificate.
   *
   * \param[in] label The label used to load the certificate.
   * \return    The loaded certificate if successful; an error code otherwise.
   *
   * \pre       -
   * \context   Handshake
   * \reentrant FALSE
   */
  virtual auto LoadCertificate(ara::core::StringView label) const noexcept
      -> ara::core::Result<ara::crypto::x509::Certificate::Uptr, CryptoAdapterErrorCode> = 0;

  /*!
   * \brief Verify a certificate chain.
   * \param[in] cert_chain Certificate chain to be verified.
   * \return Nothing if successful; an error code otherwise.
   * \pre The root certificate is set as root of trust.
   * \context Handshake
   * \reentrant FALSE
   * \error EnCryptoAdapterErrors::kRuntimeError
   * \vprivate This function is component private.
   */
  virtual amsr::core::Result<void> VerifyCertificateChain(
      ara::core::Vector<ara::crypto::x509::Certificate::Uptr> const &cert_chain) const noexcept = 0;

  /*!
   * \brief Gets the public key of the leaf certificate.
   * \param[in] cert the certificate to export the public key from.
   * \return Result contains a pointer to the public key of the certificate.
   * \pre The certificate shall be verified.
   * \context Handshake
   * \reentrant FALSE
   * \error EnCryptoAdapterErrors::kRuntimeError
   * \vprivate This function is component private.
   */
  virtual ara::core::Result<ara::crypto::cryp::PublicKey::Uptrc, CryptoAdapterErrorCode> GetCertificatePublicKey(
      ara::crypto::x509::Certificate const &cert) const noexcept = 0;

  /*!
   * \brief     Attempts to decode a distinguished name.
   *
   * \param[in] distinguished_name The encoded distinguished name.
   * \param[in] format_id          The ID of the format. Use ara::crypto::Serializable::kFormatDefault to auto-detect.
   * \return    The decoded distinguished name if successful; an error code otherwise.
   *
   * \pre       -
   * \context   Handshake
   * \reentrant FALSE
   */
  virtual auto DecodeDn(ara::crypto::ReadOnlyMemRegion distinguished_name,
                        ara::crypto::Serializable::FormatId format_id) noexcept
      -> ara::core::Result<ara::crypto::x509::X509DN::Uptrc> = 0;
};

/*!
 * \brief Interface for Crypto Adapter.
 */
class CryptoAdapterInterface {
 public:
  /*! \brief Default constructor. */
  CryptoAdapterInterface() = default;

  /*! \brief Not copy constructible. */
  CryptoAdapterInterface(CryptoAdapterInterface const &) = delete;

  /*! \brief Not move constructible. */
  CryptoAdapterInterface(CryptoAdapterInterface &&) = delete;

  /*! \brief Not copy assignable. */
  CryptoAdapterInterface &operator=(CryptoAdapterInterface const &) = delete;

  /*! \brief Not move assignable. */
  CryptoAdapterInterface &operator=(CryptoAdapterInterface &&) = delete;

  /*! \brief Hash interface unique pointer type definition. */
  using HashInterfaceUptr = std::unique_ptr<HashInterface>;

  /*! \brief Mac Generator interface unique pointer type definition. */
  using MacGeneratorInterfaceUptr = std::unique_ptr<MacGeneratorInterface>;

  /*! \brief Mac Verifier interface unique pointer type definition. */
  using MacVerifierInterfaceUptr = std::unique_ptr<MacVerifierInterface>;

  /*! \brief Prf interface unique pointer type definition. */
  using PrfInterfaceUptr = std::unique_ptr<PrfInterface>;

  /*! \brief Encryption interface unique pointer type definition. */
  using EncryptorInterfaceUptr = std::unique_ptr<EncryptorInterface>;

  /*! \brief Decryption interface unique pointer type definition. */
  using DecryptorInterfaceUptr = std::unique_ptr<DecryptorInterface>;

  /*! \brief Random generator interface unique pointer type definition. */
  using RngInterfaceUptr = std::unique_ptr<RngInterface>;

  /*! \brief Signature Generator Interface unique pointer type definition. */
  using SignatureGeneratorInterfaceUptr = std::unique_ptr<SignatureGeneratorInterface>;

  /*! \brief Signature Verifier Interface unique pointer type definition. */
  using SignatureVerifierInterfaceUptr = std::unique_ptr<SignatureVerifierInterface>;

  /*! \brief Diffie-Hellman Public key has a size of 32-octet(bytes) and stored in a fixed size data type. */
  using ECDHPublicKeyByteString = amsr::core::Array<std::uint8_t, 32U>;

  /*! * \brief Type alias for ECDH key pair. */
  using ECDHPrivatePublicKeyPair = std::pair<ara::crypto::cryp::PrivateKey::Uptrc, ECDHPublicKeyByteString>;

  /*! \brief Virtual default destructor. */
  virtual ~CryptoAdapterInterface() = default;

  /*!
   * \brief Creates a hash function context, a factory method.
   * \param[in] algorithm The hash algorithm to be used.
   * \context Internal
   * \pre -
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   * \return A unique pointer to the newly created hash function context or an error code.
   */
  virtual amsr::core::Result<HashInterfaceUptr, CryptoAdapterErrorCode> CreateHash(
      EnHashAlgorithm algorithm) const noexcept = 0;

  /*!
   * \brief Creates a MAC generator context, a factory method.
   * \param[in] algorithm The MAC algorithm to be used.
   * \param[in] key The key material to be used for MAC generation.
   * \context Internal
   * \pre -
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   * \return A unique pointer to the newly created MAC generator context or an error code.
   */
  virtual amsr::core::Result<MacGeneratorInterfaceUptr, CryptoAdapterErrorCode> CreateMacGenerator(
      EnMacAlgorithm algorithm, ReadOnlyBufferView key) const noexcept = 0;

  /*!
   * \brief Creates the MAC verifier context, a factory method.
   * \param[in] algorithm The MAC algorithm to be used.
   * \param[in] key The key material to be used for MAC verification.
   * \context Internal
   * \pre -
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   * \return A unique pointer to the newly created MAC verifier context or an error code.
   */
  virtual amsr::core::Result<MacVerifierInterfaceUptr, CryptoAdapterErrorCode> CreateMacVerifier(
      EnMacAlgorithm algorithm, ReadOnlyBufferView key) const noexcept = 0;

  /*!
   * \brief Creates the PRF context, a factory method.
   * \param[in] algorithm The PRF algorithm to be used.
   * \param[in] secret The secret to be used with the PRF (usually the TLS master secret). For each key a new PRF needs
   * to be created.
   * \context Internal
   * \pre -
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   * \return A unique pointer to the newly created PRF context or an error code.
   * \trace SPEC-6562818
   */
  virtual amsr::core::Result<PrfInterfaceUptr, CryptoAdapterErrorCode> CreatePrf(
      EnPrfAlgorithm algorithm, ReadOnlyBufferView secret) const noexcept = 0;

  /*!
   * \brief Creates the Encryptor context, a factory method.
   * \param[in] algorithm The cipher algorithm to be used.
   * \param[in] key The key material to be used for encryption.
   * \context Internal
   * \pre -
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   * \return A unique pointer to the newly created encryptor context or an error code.
   */
  virtual amsr::core::Result<EncryptorInterfaceUptr, CryptoAdapterErrorCode> CreateEncryptor(
      EnCipherAlgorithm algorithm, ReadOnlyBufferView key) const noexcept = 0;

  /*!
   * \brief Creates the Decryptor context, a factory method.
   * \param[in] algorithm The cipher algorithm to be used.
   * \param[in] key The key material to be used for decryption.
   * \context Internal
   * \pre -
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   * \return A unique pointer to the newly created decryptor context or an error code.
   */
  virtual amsr::core::Result<DecryptorInterfaceUptr, CryptoAdapterErrorCode> CreateDecryptor(
      EnCipherAlgorithm algorithm, ReadOnlyBufferView key) const noexcept = 0;

  /*!
   * \brief Creates the RNG context, a factory method.
   * \context Internal
   * \pre -
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   * \return A unique pointer to the newly created RNG context or an error code.
   */
  virtual amsr::core::Result<RngInterfaceUptr, CryptoAdapterErrorCode> CreateRng() const noexcept = 0;

  /*!
   * \brief Loads a pre-shard key from the KeyStorageProvider for our CryptoProvider.
   * \param[in] uuid The pre-shared key UUID.
   * \return A trusted container with the loaded key, or any crypto error.
   * \vprivate This function is Vector component internal API.
   * \pre -
   * \context ANY
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  virtual amsr::core::Result<ara::crypto::cryp::SymmetricKey::Uptrc,
                             tls_crypto_adapter::internal::CryptoAdapterErrorCode>
  LoadPreSharedKey(ara::crypto::Uuid const &uuid) = 0;

  /*!
   * \brief Loads a pre-master key from the KeyStorageProvider for our CryptoProvider when the algorithm is
   *        Diffie-Hellman.
   * \param[in] own_private_key Our own private key which is used for generating pre-master secret.
   * \param[in] partner_public_key The client's public key which is used for generating pre-master secret.
   * \return Pre-master secret or the related crypto error code.
   * \vprivate This function is Vector component internal API.
   * \pre -
   * \context ANY
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  virtual amsr::core::Result<ara::crypto::cryp::SymmetricKey::Uptrc, CryptoAdapterErrorCode> DerivePreMasterSecretECDHE(
      ara::crypto::cryp::PrivateKey::Uptrc own_private_key,
      ara::crypto::ReadOnlyMemRegion partner_public_key) noexcept = 0;

  /*!
   * \brief Generates the master secret.
   * \param[in] pre_master_secret is used to generate master secret key
   * \param[in] seed Seeding data.
   * \param[out] destination Reference to the destination buffer to fill with the generated master secret.
   * \param[in] algorithm The hash algorithm to be used.
   * \return On success returns "kCryptoAdapterErrOk", on failure returns "kCryptoProviderException".
   * \context Internal
   * \pre -
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   * \return An error code providing information about success or failure of the operation.
   */
  virtual amsr::core::Result<void, CryptoAdapterErrorCode> GenerateMasterSecret(
      ara::crypto::cryp::SymmetricKey::Uptrc pre_master_secret, ReadOnlyBufferView seed,
      MasterSecretContainer &destination, EnPrfAlgorithm algorithm) noexcept = 0;

  /*!
   * \brief Gets a shared pointer to the key storage provider.
   * \context Handshake
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   * \return A shared pointer to the key storage provider.
   */
  virtual ara::crypto::keys::KeyStorageProvider::Sptr GetKsp() const noexcept = 0;

  /*!
   * \brief Create an instance for a X25519KeyPairGenerator.
   * \context Handshake
   * \pre -
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   * \return The generator returned by its interface type.
   * \error EnCryptoAdapterErrors::kRuntimeError if CryptoProvider::GeneratePrivateKey fails.
   */
  virtual amsr::core::Result<CryptoAdapterInterface::ECDHPrivatePublicKeyPair, CryptoAdapterErrorCode>
  GenerateX25519KeyPair() const noexcept = 0;

  /*!
   * \brief Factory function for creating an object of Ed25519SignatureVerifier type.
   * \param[in] public_key A unique pointer to a constant PublicKey.
   * \context ANY
   * \pre -
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   * \vprivate This function is Vector component internal API
   * \return A base pointer to the created Ed25519SignatureVerifier object.
   */
  virtual amsr::core::Result<SignatureVerifierInterfaceUptr, CryptoAdapterErrorCode> CreateSignatureVerifier(
      ara::crypto::cryp::PublicKey::Uptrc public_key) const noexcept = 0;
};

}  // namespace internal
}  // namespace tls_crypto_adapter
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_INTERNAL_CRYPTO_ADAPTER_INTERFACE_H_
