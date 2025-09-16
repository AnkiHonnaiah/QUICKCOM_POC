/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2020 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!          \file  compressed_text.h
 *          \brief  Models a Compressed Text object. (See RFC)
 *        \details  This file contains the base class describing a Compressed text and the TLS/DTLS specific
 *                  implementations of a compressed text.
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_COMPRESSED_TEXT_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_COMPRESSED_TEXT_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "tls/internal/record_protocol/plain_text.h"
#include "tls/internal/record_protocol/record_protocol_common_types.h"
#include "tls/internal/record_protocol/record_protocol_containers.h"
#include "tls/internal/record_protocol/security_parameters.h"

namespace amsr {
namespace tls {
namespace internal {
namespace record_protocol {

/*!
 * \brief CompressedText base class
 * \vprivate This class is component private.
 */
class CompressedText {
 protected:
  /*! \brief Default move constructible restricted to objects created by derived class */
  CompressedText(CompressedText &&) noexcept = default;

 public:
  /*!
   * \brief Constructor for CompressedText
   * \param[in] contentType The content type for this CompressedText
   * \param[in] protocolVersion The protocol version for this CompressedText
   * \param[in] payload The payload this CompressedText takes ownership for
   * \reentrant TRUE
   */
  CompressedText(ContentType contentType, ProtocolVersion protocolVersion, Payload payload);

  /*! \brief Virtual destructor with default implementation */
  virtual ~CompressedText() = default;

  CompressedText(CompressedText const &) = delete;
  CompressedText &operator=(CompressedText const &) = delete;
  CompressedText &operator=(CompressedText &&) = delete;

  /*!
   * \brief Creates a PlainText object wrapped in a unique pointer from the current state of this CompressedText
   * \details The ownership for the payload is transferred to the new PlainText object
   * \return A unique pointer wrapping the new PlainText object
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Vector component internal API
   */
  virtual PlainTextUptr CreatePlainText() noexcept = 0;

  /*!
   * \brief Creates a CipherText object wrapped in a unique pointer from the current state of this CompressedText
   * \details The ownership for the payload is transferred to the new CipherText object
   * \return A unique pointer wrapping the new CipherText object
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \synchronous TRUE
   */
  virtual CipherTextUptr CreateCipherText() noexcept = 0;

  /*!
   * \brief Creates the input required for MAC operations
   * \return A serialized container with data used as input for MAC operations
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \synchronous TRUE
   */
  virtual MacInput CreateMacInput() const noexcept = 0;

  /*!
   * \brief Creates a Nonce required for encryption operations with general AEAD ciphers
   * \details See the RFC 5246 and 6347 for how the 'Nonce' is constructed
   * \param[in] sp Const reference to the current security parameters
   * \return A serialized container representing a 'Nonce'
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \synchronous TRUE
   */
  virtual Nonce CreateNonce(SecurityParameters const &sp) const noexcept = 0;

  /*!
   * \brief Creates the Additional data required for encryption operations with general AEAD ciphers
   * \details See the RFC 5246 and 6347 for how the 'additional data' is constructed
   * \return A serialized container with 'additional data'
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace SPEC-6493123
   */
  virtual AdditionalData CreateAdditionalData() const noexcept = 0;

  /*!
   * \brief Get the payload that the compressed text owns and can be modified
   * \return A reference of the payload
   * \pre -
   * \context ANY
   * \reentrant TRUE
   * \vprivate This method is component private.
   */
  Payload &GetPayloadRef() { return payload_; }

  /*!
   * \brief Get the payload that the compressed text owns but cannot be modified
   * \return A constant reference of the payload
   * \pre -
   * \context ANY
   * \reentrant TRUE
   * \vprivate This method is component private.
   */
  Payload const &GetPayloadRefConst() const { return payload_; }

  /*! \brief  The content type for this CompressedText */
  ContentType const contentType_{};

  /*! \brief  The Protocol Version for this CompressedText */
  ProtocolVersion const protocolVersion_{};

 private:
  /*! \brief  The payload this CompressedText owns */
  Payload payload_{};
};

/*!
 * \brief CompressedText specific for TLS
 * \vprivate This method is component private.
 */
class CompressedTextTls final : public CompressedText {
 public:
  /*!
   * \brief Constructor for TLS specific CompressedText
   * \param[in] contentType The content type for this CipherText
   * \param[in] protocolVersion The ProtocolVersion for this CipherText
   * \param[in] payload The payload to take ownership for
   * \param[in] read_seq The implicit read sequence counter from Record Protocol
   * \param[in] write_seq The implicit write sequence counter from Record Protocol
   * \reentrant TRUE
   */
  CompressedTextTls(ContentType contentType, ProtocolVersion protocolVersion, Payload payload, std::uint64_t read_seq,
                    std::uint64_t write_seq);

  /*!
   * \brief Explicit constructor for TLS specific CompressedText with PlainText
   * \details This constructor does not compress the passed plain text, it simply copies fields and takes ownership
   * over its payload which is assumed to be compressed.
   * \param[in, out] plainText Reference to TLS specific PlainText
   * \reentrant FALSE
   */
  explicit CompressedTextTls(PlainTextTls &plainText);

  /*!
   * \brief Constructor for TLS specific CompressedText with CipherText
   * \details This constructor does not decompress the passed cipher text, it simply copies fields and takes ownership
   * over its payload which is assumed to be decompressed.
   * \param[in, out] cipherText Reference to TLS specific CipherText
   * \reentrant FALSE
   */
  explicit CompressedTextTls(CipherTextTls &cipherText);

  /*!
   * \brief Creates a unique pointer wrapping a TLS PlainText object
   * \details The new TLS PlainText takes ownership for the payload
   * \return Unique pointer wrapping a TLS PlainText
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate This method is component private.
   */
  PlainTextUptr CreatePlainText() noexcept final;

  /*!
   * \brief Creates a unique pointer wrapping a TLS CipherText object
   * \details The new TLS CipherText takes ownership for the payload
   * \return Unique pointer wrapping a TLS CipherText
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate This method is component private.
   */
  CipherTextUptr CreateCipherText() noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::CompressedText::CreateMacInput
   * \trace SPEC-6561834
   * \vprivate This method is component private.
   */
  MacInput CreateMacInput() const noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::CompressedText::CreateNonce
   * \vprivate This method is component private.
   */
  Nonce CreateNonce(SecurityParameters const &sp) const noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::CompressedText::CreateAdditionalData
   * \vprivate This method is component private.
   */
  AdditionalData CreateAdditionalData() const noexcept final;

  /*! \brief Reference to the implicit read sequence counter */
  std::uint64_t const implicit_read_seq_{0U};

  /*! \brief Reference to the implicit write sequence counter */
  std::uint64_t const implicit_write_seq_{0U};
};

/*!
 * \brief CompressedText specific for DTLS
 * \trace SPEC-6562097
 * \vprivate This class is component private.
 */
class CompressedTextDtls final : public CompressedText {
 public:
  /*!
   * \brief Constructor for DTLS specific CompressedText
   * \param[in] contentType The content type for this CipherText
   * \param[in] protocolVersion The ProtocolVersion for this CipherText
   * \param[in] payload The payload to take ownership for
   * \param[in] epoch Reference to the epoch from Record Protocol
   * \param[in] explicit_seq Reference to the explicit sequence counter from Record Protocol
   * \reentrant TRUE
   */
  CompressedTextDtls(ContentType contentType, ProtocolVersion protocolVersion, Payload payload, std::uint16_t epoch,
                     std::uint64_t explicit_seq);

  /*!
   * \brief Explicit constructor for DTLS specific CompressedText with PlainText
   * \details This constructor does not compress the passed plain text, it simply copies fields and takes ownership
   * over its payload which is assumed to be compressed.
   * \param[in, out] plainText Reference to DTLS specific PlainText
   * \reentrant FALSE
   */
  explicit CompressedTextDtls(PlainTextDtls &plainText);

  /*!
   * \brief Constructor for DTLS specific CompressedText with CipherText
   * \details This constructor does not decompress the passed cipher text, it simply copies fields and takes ownership
   * over its payload which is assumed to be decompressed.
   * \param[in, out] cipherText Reference to DTLS specific CipherText
   * \reentrant FALSE
   */
  explicit CompressedTextDtls(CipherTextDtls &cipherText);

  /*!
   * \brief Creates a unique pointer wrapping a DTLS PlainText object
   * \details The new DTLS PlainText takes ownership for the payload
   * \return Unique pointer wrapping a DTLS PlainText
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate This method is component private.
   */
  PlainTextUptr CreatePlainText() noexcept final;

  /*!
   * \brief Creates a unique pointer wrapping a DTLS CipherText object
   * \details The new DTLS CipherText takes ownership for the payload
   * \return Unique pointer wrapping a DTLS CipherText
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate This method is component private.
   */
  CipherTextUptr CreateCipherText() noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::CompressedText::CreateMacInput
   * \vprivate This method is component private.
   * \trace SPEC-6562006
   */
  MacInput CreateMacInput() const noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::CompressedText::CreateNonce
   * \vprivate This method is component private.
   */
  Nonce CreateNonce(SecurityParameters const &sp) const noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::CompressedText::CreateAdditionalData
   * \vprivate This method is component private.
   */
  AdditionalData CreateAdditionalData() const noexcept final;

  /*! \brief Reference to the epoch */
  uint16_t const epoch_{0U};

  /*! \brief Reference to the explicit sequence counter */
  uint64_t const explicit_seq_{0U};
};

}  // namespace record_protocol
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_COMPRESSED_TEXT_H_
