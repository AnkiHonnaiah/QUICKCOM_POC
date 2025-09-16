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
/*!       \file   finished_message.h
 *        \brief  Class describing a TLS/DTLS Finished message
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_FINISHED_MESSAGE_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_FINISHED_MESSAGE_H_

#include <cstdint>
#include "ara/core/vector.h"
#include "tls/internal/handshakestatemachine/common_handshake_types.h"
#include "tls/internal/messages/handshake_message.h"

namespace amsr {
namespace tls {
namespace internal {
namespace messages {

/*!
 * \brief The FinishedMessage class
 * \trace SPEC-6561966, SPEC-6561967
 * \vprivate This class is component private.
 */
class FinishedMessage final : public HandshakeMessage {
 public:
  /*!
   * \brief Create a new FinishedMessage object representing a finished Message
   * \param[in]  kMessageSeqFinished FinishedMessage sequence number.
   * \reentrant TRUE
   */
  explicit FinishedMessage(std::uint16_t kMessageSeqFinished) noexcept
      : HandshakeMessage(kMessageSeqFinished, handshakestatemachine::HandshakeMessageHandle::kFinished) {
    included_in_finish_calc_ = true;
  }

  /*! \brief Destructor */
  ~FinishedMessage() final = default;

  /*!
   * \brief Not copy constructible.
   */
  FinishedMessage(FinishedMessage const&) = delete;
  /*!
   * \brief Not move constructible.
   */
  FinishedMessage(FinishedMessage&&) = delete;
  /*!
   * \brief Not copy assignable.
   */
  FinishedMessage& operator=(FinishedMessage const&) = delete;
  /*!
   * \brief Not move assignable.
   */
  FinishedMessage& operator=(FinishedMessage&&) = delete;

  /*!
   * \brief Serializes this message object to the end of a byte vector.
   * \details The contents of a FinishedMessage is one byte for HandshakeType, three bytes for
   * DataLength and the VerifyData.
   * In DTLS two bytes SequenceNumber, three bytes for FragmentationOffset and three bytes for FragmentationLength
   * are added before the VerifyData.
   * These values will be pushed back to the end of the supplied vector.
   * \param[in,out] cxt Used for tracing internal logic flow and error reporting
   * \return Serialized message with type of vector<uint8_t>
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   * \trace SPEC-6561967
   */
  ara::core::Vector<std::uint8_t> Serialize(common::TlsCommFlowContext& cxt) const noexcept final;

  /*!
   * \brief Deserializes an input byte vector (buffer) into this message object.
   * \details Reads the input vector<uint8_t> containing a serialized version of this object byte by byte starting from
   * the first byte in the supplied vector, first it checks if the message header is correct or not and after it fills
   * in the VerifyData of this message object. Report an error code in the context if an error in the serialized data is
   * found.
   * \param[in,out] cxt Used for tracing internal logic flow and error reporting
   * \param[in] buffer The input byte vector with data to do deserialization on.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   * \trace SPEC-6561967
   */
  void Deserialize(common::TlsCommFlowContext& cxt, ara::core::Vector<std::uint8_t> const& buffer) noexcept final;

  /*!
   * \brief Set the VerifyData variable of this message object.
   * \details checks the size of the supplied vector input and assigns its value to the VerifyData vector
   * if it has the right size. otherwise, it returns an error to the context.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in] verify_data The input byte vector with data to be written to VerifyData.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   * \error TlsCommErrorCode::kTlsErrUnexpectedInternalParam is reported if size of verify_data_ is incorrect
   * \trace SPEC-6561967
   */
  void SetVerifyData(common::TlsCommFlowContext& cxt, ara::core::Vector<std::uint8_t> const& verify_data) noexcept;

  /*!
   * \brief Get the VerifyData variable of this message object.
   * \return VerifyData with type of vector<uint8_t>
   * \context ANY
   * \pre -
   * \reentrant TRUE
   * \vprivate This method is component private.
   * \trace SPEC-6561967
   */
  ara::core::Vector<std::uint8_t> const& GetVerifyData() const noexcept { return verify_data_; };

  /*!
   * \brief Reset FinishedMessage object data to default values
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void Cleanup() noexcept;

  /*! \brief Constant with HandshakeType(20) = Finished */
  static std::uint8_t const kHandshakeType{
      static_cast<std::uint8_t>(handshakestatemachine::HandshakeMessageHandle::kFinished)};

 private:
  /*! Verify data */
  ara::core::Vector<std::uint8_t> verify_data_{};
  /*! \brief Friend test declarations */
  FRIEND_TEST(Cdd__MessageManager, Cleanup);
  /*! \brief Friend test declarations */
  FRIEND_TEST(UT__MessageManager, Cleanup);
};

}  // namespace messages
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_FINISHED_MESSAGE_H_
