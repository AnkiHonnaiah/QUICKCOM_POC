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
/**        \file deser_ap_application_error.h
 *        \brief Deserializer for SOME/IP application error
 *        \unit SomeIpProtocol::Serdes::ApApplicationError
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_AP_APPLICATION_ERROR_H_
#define LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_AP_APPLICATION_ERROR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include "deser_wrapper.h"
#include "someip-protocol/internal/ap_application_error.h"
#include "someip-protocol/internal/deserialization/reader.h"
#include "types.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace deserialization {

/*!
 * \brief Deserializes the SOME/IP message header from the byte stream.
 * \param[in,out] reader Reader holding the view on the serialized data.
 * \param[out] app_error application error which has to be deserialized.
 * \return true if deserialization was successful, false otherwise.
 * \pre -
 * \context Reactor|App
 * \reentrant TRUE for different reader objects.
 * \trace SPEC-8053379
 * \spec
 *   requires true;
 * \endspec
 *
 * \internal
 * - If static size of the length field is valid, return result of deserialized ApApplicationError.
 * - Return false otherwise.
 * \endinternal
 */
inline Result DeserializeApApplicationError(Reader& reader, serialization::ApApplicationError& app_error) {
  // Transformation properties for deserialization of a SOME/IP application error payload
  using TpPack_ApError =
      internal::TpPack<deserialization::BigEndian, internal::SizeOfArrayLengthField<0U>,
                       internal::SizeOfVectorLengthField<0U>, internal::SizeOfMapLengthField<0U>,
                       internal::SizeOfStringLengthField<2U>, internal::SizeOfStructLengthField<2U>,
                       internal::SizeOfUnionLengthField<4U>, internal::SizeOfUnionTypeSelectorField<1U>,
                       internal::StringBomActive, internal::StringNullTerminationActive, internal::Utf8Encoding>;

  // Validate static size
  deserialization::Result result{reader.VerifySize(
      deserialization::Tp<TpPack_ApError>::kSizeOfUnionLengthField)};  // Size of the length field of the union

  // Deserialize byte stream
  if (result == true) {
    result = SomeIpProtocolDeserialize<
        TpPack_ApError,
        // Config of variant/union length field
        internal::LengthSize<deserialization::Tp<TpPack_ApError>::kSizeOfUnionLengthField,
                             typename deserialization::Tp<TpPack_ApError>::ByteOrder>,
        // Config of struct inside the variant
        internal::ConfPack<internal::LengthSize<deserialization::Tp<TpPack_ApError>::kSizeOfStructLengthField,
                                                typename deserialization::Tp<TpPack_ApError>::ByteOrder>>>(reader,
                                                                                                           app_error);
  }
  return result;
}

/*!
 * \brief Deserializes the user message from the byte stream.
 * \details With R19-11 the user message was removed from the ApApplicationError. The current R19-03 based
 *          implementation supports parsing of the user message, but the user message is always set to an empty string.
 * \param[in,out] r Reader holding the view on the serialized data.
 * \param[out] user_message User message, the deserialized value shall be stored into.
 * \return true if the user message size is correct, false otherwise.
 * \pre -
 * \context Reactor|App
 * \reentrant TRUE for different reader objects.
 * \spec
 *   requires true;
 * \endspec
 *
 * \internal
 * - Read and verify the value of the string length field.
 * - Skip the user message according to the read length field.
 * - Return true if the user message size is correct, false otherwise.
 * \endinternal
 */
template <typename TpPack>
// VECTOR NL AutosarC++17_10-A8.4.4: MD_SOMEIPPROTOCOL_AutosarC++17_10-A8.4.4_UsingReturnValue
inline Result DeserializeUserMessage(Reader& r, amsr::core::ErrorDomain::StringType& user_message) noexcept {
  // Parse string length field
  typename UintRead<Tp<TpPack>::kSizeOfStringLengthField, typename Tp<TpPack>::ByteOrder>::type string_length_field{0U};
  r.ReadUintOfSize<Tp<TpPack>::kSizeOfStringLengthField, typename Tp<TpPack>::ByteOrder>(string_length_field);

  Result const result{static_cast<Result>(r.VerifySize(string_length_field))};

  // Consume the user message, but always return an empty string.
  static_cast<void>(r.Skip(string_length_field));
  user_message = "";

  return result;
}

/*!
 * \brief Deserializes the SomeIp message header from the byte stream.
 * \tparam TpPack  Transformation properties parameter pack.
 * \param[in,out] r Reader holding the view on the serialized data.
 * \param[out] s Struct, the deserialized value shall be stored into.
 * \return true if deserialization was successful, false otherwise.
 * \pre It has been verified, that the buffer contains at least the number of bytes provided by
 *      SomeIpProtocolGetStaticSiz(SizeToken<ApApplicationErrorStruct>).
 * \context Reactor|App
 * \reentrant TRUE for different reader objects.
 *
 * \internal
 * - Deserialize error domain, error code, support data & user message
 * - Return false, if any error occured, otherwise return true.
 * \endinternal
 */
template <typename TpPack>
// VECTOR NL AutosarC++17_10-A13.3.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-A13.3.1_overload_w_forwarding_reference
Result SomeIpProtocolDeserialize(Reader& r,
                                 amsr::someip_protocol::internal::serialization::ApApplicationErrorStruct& s) noexcept {
  // for the primitive types, 'SomeIpProtocolDeserialize' always returns true. So ignore the return values
  static_cast<void>(SomeIpProtocolDeserialize<TpPack, typename Tp<TpPack>::ByteOrder>(r, s.error_domain_value));
  static_cast<void>(SomeIpProtocolDeserialize<TpPack, typename Tp<TpPack>::ByteOrder>(r, s.error_code));
  static_cast<void>(SomeIpProtocolDeserialize<TpPack, typename Tp<TpPack>::ByteOrder>(r, s.support_data));
  return DeserializeUserMessage<TpPack>(r, s.user_message);
}

/*!
 * \brief Calculates the static size for ApApplicationErrorStruct.
 * \tparam TpPack Transformation properties parameter pack.
 * \return Returns the static size of the struct in bytes.
 * \pre -
 * \context Reactor|App
 * \reentrant TRUE for different reader objects.
 * \spec
 *   requires true;
 * \endspec
 *
 * \internal
 * - Return sum of size of {error dome value, error code, support data, string length field}
 * \endinternal
 */
// VECTOR AV NC Architecture-ApiAnalysis Return_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
template <typename TpPack>
constexpr std::size_t SomeIpProtocolGetStaticSize(
    SizeToken<amsr::someip_protocol::internal::serialization::ApApplicationErrorStruct>) noexcept {
  return {/* size of error_domain_value */ sizeof(amsr::core::ErrorDomain::IdType) +
          /* size of error_code */ sizeof(amsr::core::ErrorDomain::CodeType) +
          /* size of support_data */ sizeof(amsr::core::ErrorDomain::SupportDataType::CodeType) +
          deserialization::Tp<TpPack>::kSizeOfStringLengthField};
}
}  // namespace deserialization
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_AP_APPLICATION_ERROR_H_
