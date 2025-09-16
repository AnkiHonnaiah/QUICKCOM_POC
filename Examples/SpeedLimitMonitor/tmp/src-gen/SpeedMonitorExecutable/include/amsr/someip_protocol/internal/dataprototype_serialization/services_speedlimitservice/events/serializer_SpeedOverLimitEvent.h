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
/**        \file  SpeedMonitorExecutable/include/amsr/someip_protocol/internal/dataprototype_serialization/services_speedlimitservice/events/serializer_SpeedOverLimitEvent.h
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

#ifndef SPEEDMONITOREXECUTABLE_INCLUDE_AMSR_SOMEIP_PROTOCOL_INTERNAL_DATAPROTOTYPE_SERIALIZATION_SERVICES_SPEEDLIMITSERVICE_EVENTS_serializer_SpeedOverLimitEvent_h_
#define SPEEDMONITOREXECUTABLE_INCLUDE_AMSR_SOMEIP_PROTOCOL_INTERNAL_DATAPROTOTYPE_SERIALIZATION_SERVICES_SPEEDLIMITSERVICE_EVENTS_serializer_SpeedOverLimitEvent_h_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "datatypes/impl_type_overlimitenum.h"
#include "someip-protocol/internal/serialization/ser_forward.h"
#include "someip-protocol/internal/serialization/ser_sizing.h"
#include "someip-protocol/internal/serialization/types.h"
#include "someip-protocol/internal/serialization/writer.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace dataprototype_serializer {
namespace services_speedlimitservice {
namespace events {

/*!
 * \brief   Serializer for service event /ServiceInterfaces/PkgSpeedLimitService/Latest/SpeedLimitService/SpeedOverLimitEvent
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
class SerializerSpeedOverLimitEvent {
 public:
  /*!
   * \brief       Returns the required buffer size for the data prototype service event /ServiceInterfaces/PkgSpeedLimitService/Latest/SpeedLimitService/SpeedOverLimitEvent.
   * \param[in]   data Reference to data object of top-level data type /Datatypes/OverLimitEnum.
   * \return      Calculated buffer size for serialization.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr static std::size_t GetRequiredBufferSize(::datatypes::OverLimitEnum const& data) noexcept {
    return serialization::GetRequiredBufferSize<
      TpPackDataPrototype,
      // Byte-order of primitive datatype (/Datatypes/OverLimitEnum)
      typename serialization::Tp<TpPackDataPrototype>::ByteOrder

      >(data);
  }

  /*!
   * \brief       Returns the maximum buffer size for the data prototype service event /ServiceInterfaces/PkgSpeedLimitService/Latest/SpeedLimitService/SpeedOverLimitEvent.
   * \return      Calculated buffer size for serialization.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr static amsr::someip_protocol::internal::serialization::InfSizeT GetMaximumBufferSize() noexcept {
    return serialization::GetMaximumBufferSize<
      ::datatypes::OverLimitEnum,
      TpPackDataPrototype,
      // Byte-order of primitive datatype (/Datatypes/OverLimitEnum)
      typename serialization::Tp<TpPackDataPrototype>::ByteOrder

      >();
  }


  /*!
   * \brief         Serialize the data prototype service event /ServiceInterfaces/PkgSpeedLimitService/Latest/SpeedLimitService/SpeedOverLimitEvent.
   * \param[in,out] writer Reference to the byte stream writer.
   * \param[in]     data   Reference to data object of top-level data type /Datatypes/OverLimitEnum,
   *                       whose value will be serialized into the writer.
   * \pre           -
   * \context       App
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NC AutosarC++17_10-M3.2.4: MD_SOMEIPPROTOCOL_AutosarC++17_10-M3.2.4_without_definition_false_positive
  static void Serialize(serialization::Writer &writer, ::datatypes::OverLimitEnum const& data)  noexcept;

 private:
  /*!
   * \brief Transformation properties of the data prototype.
   */
  using TpPackDataPrototype = internal::TpPack<
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
      serialization::DynamicLengthFieldSizeInactive,
      Utf8Encoding>;

};

}  // namespace events
}  // namespace services_speedlimitservice
}  // namespace dataprototype_serializer
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // SPEEDMONITOREXECUTABLE_INCLUDE_AMSR_SOMEIP_PROTOCOL_INTERNAL_DATAPROTOTYPE_SERIALIZATION_SERVICES_SPEEDLIMITSERVICE_EVENTS_serializer_SpeedOverLimitEvent_h_

