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
/*!          \file  cipher_text.h
 *          \brief  Models a Cipher Text object. (See RFC)
 *        \details  This file contains the base class describing a Cipher text and the TLS/DTLS specific
 *        implementations of a cipher text.
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_CIPHER_TEXT_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_CIPHER_TEXT_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>

#include "tls/internal/record_protocol/record_protocol_common_types.h"
#include "tls/internal/record_protocol/record_protocol_containers.h"

namespace amsr {
namespace tls {
namespace internal {
namespace record_protocol {
/*!
 * \brief CipherText base class
 * \trace SPEC-6493112, SPEC-6562097
 * \vprivate This class is component private.
 */
class CipherText {
 protected:
  /*! \brief Default move constructible restricted to objects created by derived class. */
  CipherText(CipherText &&) noexcept = default;

 public:
  /*!
   * \brief Constructor for CipherText
   * \param[in] contentType The content type for this CipherText
   * \param[in] protocolVersion The protocol version for this CipherText
   * \param[in] payload The payload this CipherText takes ownership for
   * \reentrant TRUE
   */
  CipherText(ContentType contentType, ProtocolVersion protocolVersion, Payload payload);
  /*! \brief Virtual destructor with default implementation */
  virtual ~CipherText() = default;

  CipherText(CipherText const &) = delete;
  CipherText &operator=(CipherText const &) = delete;
  CipherText &operator=(CipherText &&) = delete;

  /*!
   * \brief Creates a CompressedText object wrapped in a unique pointer from the current state of this CipherText
   * \details The ownership for the payload is transferred to the new CompressedText object
   * \return A unique pointer wrapping the new CompressedText object
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Vector component internal API
   */
  virtual CompressedTextUptr CreateCompressedText() noexcept = 0;

  /*!
   * \brief Creates the input required for MAC operations
   * \return A serialized container with data used as input for MAC operations
   * \pre Mac should not be part of the cipher_text when called
   * \context ANY
   * \reentrant FALSE
   * \synchronous TRUE
   */
  virtual MacInput CreateMacInput() const noexcept = 0;

  /*!
   * \brief Creates a Nonce required for decryption operations with general AEAD ciphers
   * \details See the RFC 5246 and 6347 for how the 'Nonce' is constructed
   * \param[in] sp Const reference to the current security parameters
   * \return A serialized container representing a 'Nonce'
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \synchronous TRUE
   */
  Nonce CreateNonce(SecurityParameters const &sp) const noexcept;

  /*!
   * \brief Creates the Additional data required for decryption operations with general AEAD ciphers
   * \details See the RFC 5246 and 6347 for how the 'additional data' is constructed
   * \param[in] sp Const reference to the current security parameters
   * \param[in] authentication_tag_length Length of the authentication tag used by the AEAD encryption algorithm when
   * encrypting this CipherText.
   * \return A serialized container with 'additional data'
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \synchronous TRUE
   */
  virtual AdditionalData CreateAdditionalData(SecurityParameters const &sp,
                                              std::size_t const authentication_tag_length) const noexcept = 0;

  /*!
   * \brief Get the payload that the cipher text owns and can be modified
   * \return A reference of the payload
   * \pre -
   * \context ANY
   * \reentrant TRUE
   * \vprivate This method is component private.
   */
  Payload &GetPayloadRef() { return payload_; }

  /*!
   * \brief Get the payload that the cipher text owns but cannot be modified
   * \return A constant reference of the payload
   * \pre -
   * \context ANY
   * \reentrant TRUE
   * \vprivate This method is component private.
   */
  Payload const &GetPayloadRefConst() const { return payload_; }

  /*!
   * \brief Serializes this ciphertext for sending.
   * \return A buffer with the serialized object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \synchronous TRUE
   */
  virtual SerializedRecord Serialize() const noexcept = 0;

  /*! \brief  The content type for this CipherText */
  ContentType const contentType_{};

  /*! \brief  The Protocol Version for this CipherText */
  ProtocolVersion const protocolVersion_{};

 private:
  /*! \brief  The payload this CipherText owns */
  Payload payload_{};
};

/*!
 * \brief CipherText specific for TLS
 * \trace SPEC-6493112
 * \vprivate This class is component private.
 */
class CipherTextTls final : public CipherText {
 public:
  /*!
   * \brief Constructor for TLS specific CipherText
   * \param[in] contentType The content type for this CipherText
   * \param[in] protocolVersion The ProtocolVersion for this CipherText
   * \param[in] payload The payload to take ownership for
   * \param[in] read_seq The implicit read sequence counter from Record Protocol
   * \param[in] write_seq The implicit write sequence counter from Record Protocol
   * \reentrant TRUE
   */
  CipherTextTls(ContentType contentType, ProtocolVersion protocolVersion, Payload payload, std::uint64_t read_seq,
                std::uint64_t write_seq);

  /*!
   * \brief Explicit constructor for TLS specific CipherText
   * \details This constructor does not encrypt the passed compressed text, it simply copies fields and takes ownership
   * over its payload which is assumed to be encrypted.
   * \param[in, out] compressedText CompressedTextTls object containing data for initialization
   * \reentrant FALSE
   */
  explicit CipherTextTls(CompressedTextTls &compressedText) noexcept;

  /*! \brief Default move constructible */
  CipherTextTls(CipherTextTls &&) noexcept = default;

  CipherTextTls(CipherTextTls const &) = delete;
  CipherTextTls &operator=(CipherTextTls const &) = delete;
  CipherTextTls &operator=(CipherTextTls &&) = delete;

  /*! \brief default destructor */
  ~CipherTextTls() final = default;

  /*!
   * \brief Creates a TLS CompressedText object wrapped in a unique pointer from the current state of this TLS
   * CipherText
   * \details The ownership for the payload is transferred to the new TLS CompressedText object
   * \return A unique pointer wrapping the new TLS CompressedText object
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate This method is component private.
   */
  CompressedTextUptr CreateCompressedText() noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::CipherText::CreateMacInput
   * \vprivate This method is component private.
   */
  MacInput CreateMacInput() const noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::CipherText::CreateAdditionalData
   * \vprivate This method is component private.
   */
  AdditionalData CreateAdditionalData(SecurityParameters const &sp,
                                      std::size_t const authentication_tag_length) const noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::CipherText::Serialize
   * \vprivate This method is component private.
   */
  SerializedRecord Serialize() const noexcept final;

  /*! \brief The current implicit read sequence counter */
  std::uint64_t const implicit_read_seq_{0U};

  /*! \brief The current implicit write sequence counter */
  std::uint64_t const implicit_write_seq_{0U};
};

/*!
 * \brief CipherText specific for DTLS
 * \trace SPEC-6562097
 * \vprivate This class is component private.
 */
class CipherTextDtls final : public CipherText {
 public:
  /*!
   * \brief Constructor for DTLS specific CipherText
   * \param[in] contentType The content type for this CipherText
   * \param[in] protocolVersion The ProtocolVersion for this CipherText
   * \param[in] payload The payload to take ownership for
   * \param[in] epoch Reference to the epoch from Record Protocol
   * \param[in] explicit_seq Reference to the explicit sequence counter from Record Protocol
   * \reentrant TRUE
   */
  CipherTextDtls(ContentType contentType, ProtocolVersion protocolVersion, Payload payload, std::uint16_t epoch,
                 std::uint64_t explicit_seq);

  /*!
   * \brief Explicit constructor for DTLS specific CipherText
   * \details This constructor does not encrypt the passed compressed text, it simply copies fields and takes ownership
   * over its payload which is assumed to be encrypted.
   * \param[in, out] compressedText CompressedTextDtls object containing data for initialization
   * \reentrant FALSE
   */
  explicit CipherTextDtls(CompressedTextDtls &compressedText) noexcept;

  /*! \brief Default move constructible */
  CipherTextDtls(CipherTextDtls &&) noexcept = default;

  CipherTextDtls(CipherTextDtls const &) = delete;
  CipherTextDtls &operator=(CipherTextDtls const &) = delete;
  CipherTextDtls &operator=(CipherTextDtls &&) = delete;

  /*! \brief default destructor */
  ~CipherTextDtls() final = default;

  /*!
   * \brief Creates a TLS CompressedText object wrapped in a unique pointer from the current state of this TLS
   * CipherText
   * \details The ownership for the payload is transferred to the new TLS CompressedText object
   * \return A unique pointer wrapping the new TLS CompressedText object
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate This method is component private.
   */
  CompressedTextUptr CreateCompressedText() noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::CipherText::CreateMacInput
   * \trace SPEC-6562006
   * \vprivate This method is component private.
   */
  MacInput CreateMacInput() const noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::CipherText::CreateAdditionalData
   * \vprivate This method is component private.
   */
  AdditionalData CreateAdditionalData(SecurityParameters const &sp,
                                      std::size_t const authentication_tag_length) const noexcept final;

  /*!
   * \copydoc ::amsr::tls::internal::record_protocol::CipherText::Serialize
   * \vprivate This method is component private.
   */
  SerializedRecord Serialize() const noexcept final;

  /*! \brief The current epoch */
  std::uint16_t const epoch_{0U};

  /*! \brief The current explicit sequence counter */
  std::uint64_t const explicit_seq_{0U};
};

}  // namespace record_protocol
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_CIPHER_TEXT_H_
