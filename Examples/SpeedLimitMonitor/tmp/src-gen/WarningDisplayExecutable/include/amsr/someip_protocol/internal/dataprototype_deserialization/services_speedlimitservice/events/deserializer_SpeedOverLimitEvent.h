/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2025 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  WarningDisplayExecutable/include/amsr/someip_protocol/internal/dataprototype_deserialization/services_speedlimitservice/events/deserializer_SpeedOverLimitEvent.h
 *        \brief  SOME/IP protocol deserializer implementation for data prototype '/ServiceInterfaces/PkgSpeedLimitService/Latest/SpeedLimitService/SpeedOverLimitEvent
 *
 *      \details
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  GENERATOR INFORMATION
 *  -------------------------------------------------------------------------------------------------------------------
 *    Generator Name: amsr_someipprotocol
 *         Commit ID: ad694a8ff64fb52ed9a0b9551cf05c10d499927a
 *********************************************************************************************************************/

#ifndef WARNINGDISPLAYEXECUTABLE_INCLUDE_AMSR_SOMEIP_PROTOCOL_INTERNAL_DATAPROTOTYPE_DESERIALIZATION_SERVICES_SPEEDLIMITSERVICE_EVENTS_deserializer_SpeedOverLimitEvent_h_
#define WARNINGDISPLAYEXECUTABLE_INCLUDE_AMSR_SOMEIP_PROTOCOL_INTERNAL_DATAPROTOTYPE_DESERIALIZATION_SERVICES_SPEEDLIMITSERVICE_EVENTS_deserializer_SpeedOverLimitEvent_h_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "datatypes/impl_type_overlimitenum.h"
#include "someip-protocol/internal/deserialization/reader.h"
#include "someip-protocol/internal/deserialization/types.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace dataprototype_deserializer {
namespace services_speedlimitservice {
namespace events {

/*!
 * \brief   Deserializer for service event /ServiceInterfaces/PkgSpeedLimitService/Latest/SpeedLimitService/SpeedOverLimitEvent
 *          of service interface /ServiceInterfaces/PkgSpeedLimitService/Latest/SpeedLimitService.
 * \details Top-Level data type: /Datatypes/OverLimitEnum
 *          Effective transformation properties of the DataPrototype:
 *          - ByteOrder:                    MOST-SIGNIFICANT-BYTE-FIRST (big-endian)
 *          - sizeOfArrayLengthField:       4
 *          - sizeOfVectorLengthField:      4
 *          - sizeOfMapLengthField:         4
 *          - sizeOfStringLengthField:      4
 *          - sizeOfStructLengthField:      0
 *          - sizeOfUnionLengthField:       4
 *          - sizeOfUnionTypeSelectorField: 4
 *          - isBomActive:                  true
 *          - isNullTerminationActive:      true
 *          - isDynamicLengthFieldSize:     false
 *          - stringEncoding:               Utf8Encoding
 * \trace SPEC-4980064
 * \vprivate Product private
 */
class DeserializerSpeedOverLimitEvent {
 public:
  /*!
   * \brief         Deserialize the data prototype service event /ServiceInterfaces/PkgSpeedLimitService/Latest/SpeedLimitService/SpeedOverLimitEvent.
   * \param[in,out] reader  Reference to the byte stream reader.
   * \param[out]    data    Reference to data object of top-level data type
   *                        /Datatypes/OverLimitEnum
   *                        in which the deserialized value will be written.
   * \return        True if the deserialization is successful. False otherwise.
   * \pre           -
   * \context       Reactor|App
   * \threadsafe    FALSE
   * \reentrant     TRUE for different reader objects.
   * \synchronous   TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NC AutosarC++17_10-M3.2.4: MD_SOMEIPPROTOCOL_AutosarC++17_10-M3.2.4_without_definition_false_positive
  static deserialization::Result Deserialize(deserialization::Reader &reader, ::datatypes::OverLimitEnum& data) noexcept;
};

}  // namespace events
}  // namespace services_speedlimitservice
}  // namespace dataprototype_deserializer
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // WARNINGDISPLAYEXECUTABLE_INCLUDE_AMSR_SOMEIP_PROTOCOL_INTERNAL_DATAPROTOTYPE_DESERIALIZATION_SERVICES_SPEEDLIMITSERVICE_EVENTS_deserializer_SpeedOverLimitEvent_h_

