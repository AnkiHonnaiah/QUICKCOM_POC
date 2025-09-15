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
/*!        \file ser_someip_header.h
 *        \brief Serializer for serializing SOME/IP header.
 *        \unit SomeIpProtocol::Messages::SerializeSomeIpMessageHeader
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_PROTOCOL_MESSAGES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_SOMEIP_HEADER_H_
#define LIB_SOMEIP_PROTOCOL_MESSAGES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_SOMEIP_HEADER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <limits>
#include <string>
#include "ara/log/logging.h"
#include "someip-protocol/internal/logging/someip_protocol_log_builder.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/serialization/types.h"
#include "someip-protocol/internal/serialization/writer.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace serialization {

/*!
 * \brief Serialize the SOME/IP message header.
 * \param[in, out] writer Writer holding the buffer to which data is serialized.
 * \param[in] header SOME/IP header struct containing the header information.
 * \pre -
 * \context Reactor|App
 * \threadsafe TRUE
 * \reentrant FALSE
 * \synchronous TRUE
 * \trace SPEC-10144515
 * \trace SPEC-10144519
 * \vprivate Product private
 *
 * \internal
 * - Serialize the header.
 * \endinternal
 * \trace SPEC-10144302, SPEC-10144304, SPEC-10144308, SPEC-10144335, SPEC-4980077
 */
// VECTOR AV NC Architecture-ApiAnalysis Parameter_Of_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
// VECTOR NL AutosarC++17_10-A0.1.3: MD_SOMEIPPROTOCOL_AutosarC++17_10-A0.1.3_function_not_called_false_positive
static inline void SerializeSomeIpMessageHeaderObject(Writer& writer, SomeIpMessageHeader const& header) {
  /*!
   * \brief Type alias for endianness 'BigEndian'.
   */
  using BigEndian = ::amsr::someip_protocol::internal::BigEndian;
  // clang-format off
  writer.writePrimitive<ServiceId, BigEndian>(header.service_id_); // VCA_SOMEIPPROTOCOL_INSUFFICIENT_SIZE, VCA_SOMEIPPROTOCOL_WRITE_REFERENCE
  writer.writePrimitive<MethodId, BigEndian>(header.method_id_); // VCA_SOMEIPPROTOCOL_WRITE_REFERENCE
  // VECTOR NC AutosarC++17_10-M0.3.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.3.1_CastMayTruncateValue_CheckPerformed
  // VECTOR NC AutosarC++17_10-A4.7.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-A4.7.1_CastMayTruncateValue_CheckPerformed
  writer.writePrimitive<LengthField, BigEndian>( // VCA_SOMEIPPROTOCOL_INSUFFICIENT_SIZE, VCA_SOMEIPPROTOCOL_WRITE_REFERENCE
       header.length_); // VCA_SOMEIPPROTOCOL_WRITE_REFERENCE
  writer.writePrimitive<ClientId, BigEndian>(header.client_id_); // VCA_SOMEIPPROTOCOL_INSUFFICIENT_SIZE, VCA_SOMEIPPROTOCOL_WRITE_REFERENCE
  writer.writePrimitive<SessionId, BigEndian>(header.session_id_); // VCA_SOMEIPPROTOCOL_INSUFFICIENT_SIZE, VCA_SOMEIPPROTOCOL_WRITE_REFERENCE
  writer.writePrimitive<ProtocolVersion, BigEndian>(header.protocol_version_); // VCA_SOMEIPPROTOCOL_INSUFFICIENT_SIZE, VCA_SOMEIPPROTOCOL_WRITE_REFERENCE
  writer.writePrimitive<InterfaceVersion, BigEndian>(header.interface_version_); // VCA_SOMEIPPROTOCOL_INSUFFICIENT_SIZE, VCA_SOMEIPPROTOCOL_WRITE_REFERENCE

  MessageType const message_type{static_cast<MessageType>(header.message_type_)};
  writer.writePrimitive<MessageType, BigEndian>(message_type); // VCA_SOMEIPPROTOCOL_INSUFFICIENT_SIZE, VCA_SOMEIPPROTOCOL_WRITE_REFERENCE

  ReturnCode const return_code{static_cast<ReturnCode>(header.return_code_)};
  writer.writePrimitive<ReturnCode, BigEndian>(return_code); // VCA_SOMEIPPROTOCOL_INSUFFICIENT_SIZE, VCA_SOMEIPPROTOCOL_WRITE_REFERENCE
  // clang-format on
}

/*!
 * \brief Serialize the given SOME/IP message header, using a calculated length value.
 * \param[in, out] writer Writer holding the buffer to which data is serialized.
 * \param[in] header SOME/IP header struct containing the header information.
 * \param[in] data_length The length of the data within the SOME/IP packet.
 * \pre The length of serialized data should be no more than the upper limit of the LengthField value.
 * \context Reactor|App
 * \threadsafe TRUE
 * \reentrant FALSE
 * \synchronous TRUE
 * \trace SPEC-10144515
 * \trace SPEC-10144519
 * \vprivate Product private
 *
 * \internal
 * - Check if data_length + kMinimumPayload length fits in the lengthfield.
 * - If it doesn't fit
 *   - log fatal and abort.
 * - Serialize the header.
 * \endinternal
 * \trace SPEC-10144302, SPEC-10144304, SPEC-10144308, SPEC-10144335, SPEC-4980077
 */
// VECTOR AV NC Architecture-ApiAnalysis Parameter_Of_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
// VECTOR NL AutosarC++17_10-A0.1.3: MD_SOMEIPPROTOCOL_AutosarC++17_10-A0.1.3_function_not_called_false_positive
static inline void SerializeSomeIpMessageHeader(Writer& writer, SomeIpMessageHeader const& header,
                                                std::size_t const data_length) {
  // clang-format off
  // VECTOR NC AutosarC++17_10-M0.1.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.1.2_dead_branch
  if (data_length >
      (std::numeric_limits<LengthField>::max() - amsr::someip_protocol::internal::kMinimumPayloadLength)) {
    logging::SomeipProtocolLogBuilder::LogFatalAndAbort(ara::core::StringView{"Violation: Length field overflow during serialization."}, // VCA_SOMEIPPROTOCOL_VALID_POINTER_PARAMETERS
                                                        ara::core::StringView{AMSR_FILE_LINE});
  } // VCA_SOMEIPPROTOCOL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
  // clang-format on

  // Create New Header including the correct length field to be serialized
  SomeIpMessageHeader to_be_serialized_header{header};
  to_be_serialized_header.length_ =
      amsr::someip_protocol::internal::kMinimumPayloadLength + static_cast<LengthField>(data_length);
  SerializeSomeIpMessageHeaderObject(writer, to_be_serialized_header);
}

}  // namespace serialization
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_PROTOCOL_MESSAGES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_SOMEIP_HEADER_H_
