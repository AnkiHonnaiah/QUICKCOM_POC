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
/**        \file deser_someip_header.h
 *        \brief Deserializer for SOME/IP header byte stream.
 *        \unit SomeIpProtocol::Messages::DeserializeSomeIpMessageHeader
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_PROTOCOL_MESSAGES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_SOMEIP_HEADER_H_
#define LIB_SOMEIP_PROTOCOL_MESSAGES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_SOMEIP_HEADER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someip-protocol/internal/deserialization/reader.h"
#include "someip-protocol/internal/deserialization/types.h"
#include "someip-protocol/internal/message.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace deserialization {

/*!
 * \brief Deserializes the SOME/IP message header from the byte stream.
 * \param[in,out] reader Reader holding the view on the serialized data.
 * \param[out] header SOME/IP message header which has to be deserialized.
 * \return true if deserialization was successful, false otherwise.
 * \pre -
 * \context Reactor|App
 * \reentrant TRUE for different reader objects.
 * \spec
 *   requires true;
 * \endspec
 * \vprivate Product private
 *
 * \internal
 * - If the serialized byte stream contains at least kHeaderSize bytes.
 *   - Deserialize header.
 * - Otherwise deserializations fails.
 * \endinternal
 * \trace SPEC-10144302, SPEC-10144304, SPEC-10144308
 */
// VECTOR NC AutosarC++17_10-A0.1.3: MD_SOMEIPPROTOCOL_AutosarC++17_10-A0.1.3_function_not_called_false_positive
// VECTOR NC AutosarC++17_10-A8.4.4: MD_SOMEIPPROTOCOL_AutosarC++17_10-A8.4.4_UsingReturnValue
static inline Result DeserializeSomeIpMessageHeader(Reader& reader, SomeIpMessageHeader& header) {
  Result const result{reader.VerifySize(kHeaderSize)};
  if (result) {
    reader.ReadPrimitive<ServiceId, BigEndian>(header.service_id_);
    reader.ReadPrimitive<MethodId, BigEndian>(header.method_id_);
    reader.ReadPrimitive<LengthField, BigEndian>(header.length_);
    reader.ReadPrimitive<ClientId, BigEndian>(header.client_id_);
    reader.ReadPrimitive<SessionId, BigEndian>(header.session_id_);
    reader.ReadPrimitive<ProtocolVersion, BigEndian>(header.protocol_version_);
    reader.ReadPrimitive<InterfaceVersion, BigEndian>(header.interface_version_);

    MessageType message_type{static_cast<MessageType>(SomeIpMessageType::kRequest)};
    reader.ReadPrimitive<MessageType, BigEndian>(message_type);
    // VECTOR NL AutosarC++17_10-A7.2.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-A7.2.1_MissingRangeCheck
    header.message_type_ = static_cast<SomeIpMessageType>(message_type);

    ReturnCode return_code{static_cast<ReturnCode>(SomeIpReturnCode::kOk)};
    reader.ReadPrimitive<ReturnCode, BigEndian>(return_code);
    // VECTOR NL AutosarC++17_10-A7.2.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-A7.2.1_MissingRangeCheck
    header.return_code_ = static_cast<SomeIpReturnCode>(return_code);
  }

  return result;
}

}  // namespace deserialization
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_PROTOCOL_MESSAGES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_SOMEIP_HEADER_H_
