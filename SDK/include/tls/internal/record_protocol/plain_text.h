/*********************************************************************************************************************
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
/*!          \file  plain_text.h
 *          \brief  Models a Plain Text object. (See RFC)
 *        \details  This file contains the base class describing a Plain text and the TLS/DTLS specific
 *                  implementations of a plain text.
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_PLAIN_TEXT_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_PLAIN_TEXT_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "tls/internal/record_protocol/record_protocol_common_types.h"
#include "tls/internal/record_protocol/record_protocol_containers.h"

namespace amsr {
namespace tls {
namespace internal {
namespace record_protocol {

/*!
 * \brief PlainText base class
 * \vprivate This class is component private.
 */
class PlainText {
 protected:
  /*! \brief Default move constructible restricted to objects created by derived class */
  PlainText(PlainText &&) noexcept = default;

 public:
  /*!
   * \brief Constructor for PlainText
   * \param[in] contentType The content type for this PlainText
   * \param[in] protocolVersion The protocol version for this PlainText
   * \param[in] payload The payload this PlainText takes ownership for
   * \reentrant TRUE
   */
  PlainText(ContentType contentType, ProtocolVersion protocolVersion, Payload payload);

  /*!
   * \brief Creates a CompressedText object wrapped in a unique pointer from the current state of this PlainText
   * \details The ownership for the payload is transferred to the new CompressedText object
   * \return A unique pointer wrapping the new CompressedText object
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \synchronous TRUE
   */
  virtual CompressedTextUptr CreateCompressedText() noexcept = 0;

  /*! \brief Virtual destructor with default implementation */
  virtual ~PlainText() = default;

  PlainText(PlainText const &) = delete;
  PlainText &operator=(PlainText const &) = delete;
  PlainText &operator=(PlainText &&) = delete;

  /*!
   * \brief Get the payload that the plain text owns and can be modified
   * \return A reference of the payload
   * \pre -
   * \context ANY
   * \reentrant TRUE
   * \vprivate This method is component private.
   */
  Payload &GetPayloadRef() noexcept { return payload_; }

  /*!
   * \brief Get the payload that the plain text owns but cannot be modified
   * \return A constant reference of the payload
   * \pre -
   * \context ANY
   * \reentrant TRUE
   * \vprivate This method is component private.
   */
  Payload const &GetPayloadRefConst() const { return payload_; }

  /*! \brief  The content type for this PlainText */
  ContentType const contentType_{};

  /*! \brief  The Protocol Version for this PlainText */
  ProtocolVersion const protocolVersion_{};

 private:
  /*! \brief  The payload this PlainText owns */
  Payload payload_{};
};

/*!
 * \brief PlainText specific for TLS
 * \trace SPEC-6561834
 * \vprivate This class is component private.
 */
class PlainTextTls final : public PlainText {
 public:
  /*!
   * \brief Constructor for TLS specific PlainText
   * \param[in] contentType The content type for this PlainText object
   * \param[in] protocolVersion The protocol version for this PlainText object
   * \param[in] payload The payload to take ownership for
   * \param[in] read_seq The implicit read sequence counter from Record Protocol
   * \param[in] write_seq The implicit write sequence counter from Record Protocol
   * \reentrant TRUE
   */
  PlainTextTls(ContentType contentType, ProtocolVersion protocolVersion, Payload payload, std::uint64_t read_seq,
               std::uint64_t write_seq);

  /*!
   * \brief Explicit constructor for PlainTextTls with CompressedTextTls
   * \details The constructed PlainTextTls object will take ownership of the payload from the passed CompressedText
   * \param[in, out] compressedText The TLS specific CompressedText
   * \reentrant FALSE
   */
  explicit PlainTextTls(CompressedTextTls &compressedText) noexcept;

  /*!
   * \brief Creates a TLS CompressedText object wrapped in a unique pointer from the current state of this TLS PlainText
   * \details The ownership for the payload is transferred to the new TLS CompressedText object
   * \return A unique pointer wrapping the new TLS CompressedText object
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate This method is component private.
   */
  CompressedTextUptr CreateCompressedText() noexcept final;

  /*! \brief The implicit read sequence counter */
  std::uint64_t const implicit_read_seq_{0U};

  /*! \brief The implicit write sequence counter */
  std::uint64_t const implicit_write_seq_{0U};
};

/*!
 * \brief PlainText specific for DTLS
 * \trace SPEC-6562097
 * \trace SPEC-6562006
 * \vprivate This class is component private.
 */
class PlainTextDtls final : public PlainText {
 public:
  /*!
   * \brief Constructor for DTLS specific PlainText
   * \param[in] contentType The content type for this PlainText object
   * \param[in] protocolVersion The protocol version for this PlainText object
   * \param[in] payload The payload to take ownership for
   * \param[in] epoch The current epoch from Record Protocol
   * \param[in] explicit_seq The current explicit sequence counter from Record Protocol
   * \reentrant TRUE
   */
  PlainTextDtls(ContentType contentType, ProtocolVersion protocolVersion, Payload payload, std::uint16_t epoch,
                std::uint64_t explicit_seq);

  /*!
   * \brief Explicit constructor for PlainTextDtls with CompressedTextDtls
   * \details The constructed PlainTextDtls object will take ownership of the payload from the passed CompressedText
   * \param[in, out] compressedText The DTLS specific CompressedText
   * \reentrant FALSE
   */
  explicit PlainTextDtls(CompressedTextDtls &compressedText) noexcept;

  /*!
   * \brief Creates a DTLS CompressedText object wrapped in a unique pointer from the current state of this DTLS
   * PlainText
   * \details The ownership for the payload is transferred to the new DTLS CompressedText object
   * \return A unique pointer wrapping the new DTLS CompressedText object
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate This method is component private.
   */
  CompressedTextUptr CreateCompressedText() noexcept final;

  /*! \brief The current epoch */
  uint16_t const epoch_{0U};

  /*! \brief The current explicit sequence counter */
  uint64_t const explicit_seq_{0U};
};

}  // namespace record_protocol
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_PLAIN_TEXT_H_
