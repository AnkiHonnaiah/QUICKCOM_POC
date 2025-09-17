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
/**        \file  WarningDisplayExecutable/src/amsr/someip_protocol/internal/dataprototype_deserialization/services_speedlimitservice/events/deserializer_SpeedOverLimitEvent.cpp
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

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someip-protocol/internal/deserialization/deser_wrapper.h"
#include "amsr/someip_protocol/internal/dataprototype_deserialization/services_speedlimitservice/events/deserializer_SpeedOverLimitEvent.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace dataprototype_deserializer {
namespace services_speedlimitservice {
namespace events {

deserialization::Result DeserializerSpeedOverLimitEvent::Deserialize(deserialization::Reader &reader, ::datatypes::OverLimitEnum& data) noexcept {
  // Transformation properties parameter pack for data prototype /ServiceInterfaces/PkgSpeedLimitService/Latest/SpeedLimitService/SpeedOverLimitEvent
    using TpPackAlias = internal::TpPack<
        BigEndian,
        internal::SizeOfArrayLengthField<4>,
        internal::SizeOfVectorLengthField<4>,
        internal::SizeOfMapLengthField<4>,
        internal::SizeOfStringLengthField<4>,
        internal::SizeOfStructLengthField<0>,
        internal::SizeOfUnionLengthField<4>,
        internal::SizeOfUnionTypeSelectorField<4>,
        internal::StringBomActive,
        internal::StringNullTerminationActive,
        Utf8Encoding>;


  // Verify static size
  constexpr std::size_t static_size{deserialization::SomeIpProtocolGetStaticSize<
      TpPackAlias,
        // Byte-order of primitive datatype (/Datatypes/OverLimitEnum)
      typename deserialization::Tp<TpPackAlias>::ByteOrder

      >(deserialization::SizeToken<::datatypes::OverLimitEnum>{})};

  deserialization::Result result{reader.VerifySize(static_size)};
  if (result) {
    // Deserialize byte stream
    result = deserialization::SomeIpProtocolDeserialize<
      TpPackAlias,
      // Byte-order of primitive datatype (/Datatypes/OverLimitEnum)
      typename deserialization::Tp<TpPackAlias>::ByteOrder

      >(reader, data);
  }

  return result;
}

}  // namespace events
}  // namespace services_speedlimitservice
}  // namespace dataprototype_deserializer
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

