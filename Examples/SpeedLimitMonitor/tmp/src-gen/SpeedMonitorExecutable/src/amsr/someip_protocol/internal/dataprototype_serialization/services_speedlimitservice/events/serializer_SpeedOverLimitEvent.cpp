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
/**        \file  SpeedMonitorExecutable/src/amsr/someip_protocol/internal/dataprototype_serialization/services_speedlimitservice/events/serializer_SpeedOverLimitEvent.cpp
 *        \brief  SOME/IP protocol serializer implementation for data prototype '/ServiceInterfaces/PkgSpeedLimitService/Latest/SpeedLimitService/SpeedOverLimitEvent
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

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someip-protocol/internal/serialization/ser_wrapper.h"

#include "amsr/someip_protocol/internal/dataprototype_serialization/services_speedlimitservice/events/serializer_SpeedOverLimitEvent.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace dataprototype_serializer {
namespace services_speedlimitservice {
namespace events {

void SerializerSpeedOverLimitEvent::Serialize(serialization::Writer
&writer, ::datatypes::OverLimitEnum const& data) noexcept {
  // Serialize byte stream
  serialization::SomeIpProtocolSerialize<
      TpPackDataPrototype,
      // Byte-order of primitive datatype (/Datatypes/OverLimitEnum)
      typename serialization::Tp<TpPackDataPrototype>::ByteOrder

      >(writer, data);
}

}  // namespace events
}  // namespace services_speedlimitservice
}  // namespace dataprototype_serializer
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

