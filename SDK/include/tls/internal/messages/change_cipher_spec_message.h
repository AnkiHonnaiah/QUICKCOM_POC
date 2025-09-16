/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2019 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!       \file   change_cipher_spec_message.h
 *        \brief  Class describing a TLS/DTLS Change Cipher Spec message
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_CHANGE_CIPHER_SPEC_MESSAGE_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_CHANGE_CIPHER_SPEC_MESSAGE_H_

#include <cstdint>
#include "ara/core/vector.h"
#include "tls/internal/common/common_general_types.h"

namespace amsr {
namespace tls {
namespace internal {
namespace messages {

/*!
 * \brief Class describing a TLS/DTLS Change Cipher Spec message
 * \trace SPEC-6493262
 * \vprivate This class is component private.
 */
class ChangeCipherSpecMessage final {
 public:
  /*!
   * \brief Create a new ChangeCipherSpecMessage object representing a Change Cipher Spec Message
   */
  ChangeCipherSpecMessage() noexcept = default;

  /*!
   * \brief Not copy constructible.
   */
  ChangeCipherSpecMessage(ChangeCipherSpecMessage const&) = delete;
  /*!
   * \brief Not move constructible.
   */
  ChangeCipherSpecMessage(ChangeCipherSpecMessage&&) = delete;
  /*!
   * \brief Not copy assignable.
   */
  ChangeCipherSpecMessage& operator=(ChangeCipherSpecMessage const&) = delete;
  /*!
   * \brief Not move assignable.
   */
  ChangeCipherSpecMessage& operator=(ChangeCipherSpecMessage&&) = delete;

  /*!
   * \brief Destructor
   */
  ~ChangeCipherSpecMessage() = default;

  /*!
   * \brief Serializes this message object to the end of a byte vector.
   * \details The contents of a ChangeCipherSpecMessage is one byte of value 0x01. This value will be returned in a
   * vector from this function.
   * \return The serialized message.
   * \context ANY
   * \pre -
   * \reentrant TRUE
   * \vprivate This method is component private.
   * \trace DSGN-SecCom-HandleChangeCipherSpecProtocolMessages, SPEC-6493262
   */
  static ara::core::Vector<std::uint8_t> Serialize() noexcept;

  /*!
   * \brief Deserializes an input byte vector (buffer) into this message object.
   * \details Reads the input vector<uint8_t> containing a serialized version of this object byte by byte starting from
   * the first byte in the supplied vector and fills in the fields of this message object. Will return immediately with
   * return value false and an error code in the context if an error in the serialized data is found. NOTE: For
   * ChangeCipherSpecMessage there is no field of interest so this function only check that the incoming data conforms
   * to the format for ChangeCipherSpecMessage from the RFC it never actually saves any data since it is not needed. The
   * name of the function is still Deserialize since it conforms with the pattern for all the other message classes.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in] buffer The input byte vector with data to do serialization on.
   * \context ANY
   * \pre -
   * \error TlsCommErrorCode::kTlsErrDeserialize is reported if buffer size does not match or first byte does
   * not have value one
   * \reentrant FALSE
   * \vprivate This method is component private.
   * \trace DSGN-SecCom-HandleChangeCipherSpecProtocolMessages, SPEC-6493262
   *
   */
  static void Deserialize(common::TlsCommFlowContext& cxt, ara::core::Vector<uint8_t> const& buffer) noexcept;
};

}  // namespace messages
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_CHANGE_CIPHER_SPEC_MESSAGE_H_
