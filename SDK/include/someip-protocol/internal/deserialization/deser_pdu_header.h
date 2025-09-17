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
/*!        \file deser_pdu_header.h
 *        \brief Deserializer for PDU header.
 *        \unit SomeIpProtocol::Messages::DeserializePduMessageHeader
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_PROTOCOL_MESSAGES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_PDU_HEADER_H_
#define LIB_SOMEIP_PROTOCOL_MESSAGES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_PDU_HEADER_H_

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
 * \brief Deserializes the PDU message header from the byte stream.
 * \param[in,out] reader Reader holding the view on the serialized data.
 * \param[out] header PDU message header which has to be deserialized.
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
 * - If the serialized byte stream contains at least kPduHeaderSize bytes,
 *   - Deserialize the PDU header.
 * - Otherwise the deserialization fails.
 * \endinternal
 */
// VECTOR NL AutosarC++17_10-A0.1.3: MD_SOMEIPPROTOCOL_AutosarC++17_10-A0.1.3_function_not_called_false_positive
static inline Result DeserializePduMessageHeader(Reader& reader, PduMessageHeader& header) {
  Result const result{reader.VerifySize(kPduHeaderSize)};
  if (result) {
    reader.ReadPrimitive<PduId, BigEndian>(header.pdu_id_);
    reader.ReadPrimitive<LengthField, BigEndian>(header.length_);
  }

  return result;
}

}  // namespace deserialization
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_PROTOCOL_MESSAGES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_PDU_HEADER_H_
