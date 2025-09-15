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
/*!        \file ser_pdu_header.h
 *        \brief Serializer for PDU header.
 *        \unit SomeIpProtocol::Messages::SerializePduMessageHeader
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_PROTOCOL_MESSAGES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_PDU_HEADER_H_
#define LIB_SOMEIP_PROTOCOL_MESSAGES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_PDU_HEADER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include <string>
#include "ara/log/logging.h"
#include "someip-protocol/internal/logging/someip_protocol_log_builder.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/serialization/types.h"
#include "someip-protocol/internal/serialization/writer.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace serialization {

/*!
 * \brief Serialize the PDU message header.
 * \param[in, out] writer Writer holding the buffer to which data is serialized.
 * \param[in] header PDU header struct containing the header information.
 * \param[in] data_length The length of the data within the PDU packet.
 * \pre The length of serialized data should be no more than the upper limit of the LengthField value.
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vprivate Product private
 *
 * \internal
 * - Check if data_length fits in the lengthfield.
 * - If it doesn't fit
 *   - log fatal and abort.
 * - Otherwise, Serialize the header.
 * \endinternal
 */
// VECTOR NL AutosarC++17_10-A0.1.3: MD_SOMEIPPROTOCOL_AutosarC++17_10-A0.1.3_function_not_called_false_positive
static inline void SerializePduMessageHeader(Writer& writer, PduMessageHeader const& header,
                                             std::size_t const data_length) {
  // clang-format off
  // VECTOR NL AutosarC++17_10-M0.1.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.1.2_dead_branch
  if (data_length > std::numeric_limits<LengthField>::max()) {
    logging::SomeipProtocolLogBuilder::LogFatalAndAbort(ara::core::StringView{"Violation: Length field overflow during serialization."}, // VCA_SOMEIPPROTOCOL_VALID_POINTER_PARAMETERS
                                                        ara::core::StringView{AMSR_FILE_LINE});
    // clang-format on
  }
  writer.writePrimitive<PduId, BigEndian>(header.pdu_id_);  // VCA_SOMEIPPROTOCOL_WRITE_REFERENCE
  // VECTOR NC AutosarC++17_10-M0.3.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.3.1_CastMayTruncateValue_CheckPerformed
  // VECTOR NC AutosarC++17_10-A4.7.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-A4.7.1_CastMayTruncateValue_CheckPerformed
  // clang-format off
  writer.writePrimitive<LengthField, BigEndian>(static_cast<LengthField>(data_length)); // VCA_SOMEIPPROTOCOL_WRITE_REFERENCE
  // clang-format on
}

}  // namespace serialization
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_PROTOCOL_MESSAGES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_PDU_HEADER_H_
