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
/*!        \file  send_fragmenter.h
 *        \brief  Fragments data for sending
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_SEND_FRAGMENTER_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_SEND_FRAGMENTER_H_

/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include "tls/internal/record_protocol/payload_framer.h"
#include "tls/internal/record_protocol/record_protocol_common_types.h"
#include "tls/internal/record_protocol/record_protocol_containers.h"

namespace amsr {
namespace tls {
namespace internal {
namespace record_protocol {

/*!
 * \brief SendFragmenter base class.
 * \vprivate This class is component private.
 */
class SendFragmenter {
 public:
  /*! \brief Default constructor */
  SendFragmenter() noexcept = default;

  /*! \brief Not copy constructible */
  SendFragmenter(SendFragmenter const&) = delete;
  /*! \brief Not move constructible */
  SendFragmenter(SendFragmenter&&) = delete;
  /*! \brief Not copy assignable */
  SendFragmenter& operator=(SendFragmenter const&) = delete;
  /*! \brief Not move assignable */
  SendFragmenter& operator=(SendFragmenter&&) = delete;

  /*! \brief Default destructor */
  virtual ~SendFragmenter() noexcept = default;

  /*!
   * \brief Sets the data that this fragmenter should use as source. Also sets the overhead that the current cipher
   * suite and TLS/DTLS header adds to the data.
   * \details The overhead is used to calculate how big each fragment can be to still fullfill the requested record_size
   * after compression and encryption.
   * \param[in] data A view of the source data that this SendFragmenter should use to create PlainText objects
   * when GetNextPlainText() is called.
   * \param[in] record_size The size limit of the plaintext without taking record expansion into account.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \synchronous TRUE
   */
  virtual void SetDataAndRecordSize(ReadOnlyBufferView const data, std::size_t const record_size) noexcept = 0;

  /*!
   * \brief Gets the next PlainText with payload from the source data supplied in the SetDataAndRecordSize() call and
   * header information from the context and contentType function arguments.
   * \param[in] rpContext A const reference to the current RecordProtocolContext
   * \param[in] contentType The content type of the created PlainText
   * \return A PlainText object wrapped in a unique pointer OR nullptr if no payload data could be read. For
   * application data (contentType==ContentType::kApplicationData) a PlainText will be returned even when there
   * is no payload data.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \synchronous TRUE
   * \trace SPEC-6561834
   */
  virtual PlainTextUptr GetNextPlainText(RecordProtocolContext const& rpContext,
                                         ContentType const contentType) noexcept = 0;

 protected:
  // VECTOR NC VectorC++-V11.0.2: MD_libseccom_V11.0.2_mutableMemberShallBePrivate
  /*! \brief A PayloadFramer used to extract the Payload objects from the source data. */
  PayloadFramer payload_framer_{};
};

/*!
 * \brief TLS specific version of the SendFragmenter
 * \vprivate This class is component private.
 */
class SendFragmenterTls final : public SendFragmenter {
 public:
  /*!
   * \copydoc SendFragmenter::SetDataAndRecordSize
   * \vprivate This method is component private.
   */
  void SetDataAndRecordSize(ReadOnlyBufferView const data, std::size_t const record_size) noexcept final;
  /*!
   * \copydoc SendFragmenter::GetNextPlainText
   * \vprivate This method is component private.
   */
  PlainTextUptr GetNextPlainText(RecordProtocolContext const& rpContext, ContentType const contentType) noexcept final;
};

/*!
 * \brief DTLS specific version of the SendFragmenter
 * \vprivate This class is component private.
 */
class SendFragmenterDtls final : public SendFragmenter {
 public:
  /*!
   * \copydoc SendFragmenter::SetDataAndRecordSize
   * \vprivate This method is component private.
   */
  void SetDataAndRecordSize(ReadOnlyBufferView const data, std::size_t const record_size) noexcept final;
  /*!
   * \copydoc SendFragmenter::GetNextPlainText
   * \vprivate This method is component private.
   */
  PlainTextUptr GetNextPlainText(RecordProtocolContext const& rpContext, ContentType const contentType) noexcept final;
};

}  // namespace record_protocol
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_SEND_FRAGMENTER_H_
