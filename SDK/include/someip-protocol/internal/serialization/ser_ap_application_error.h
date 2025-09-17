/*!********************************************************************************************************************
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
/*!        \file ser_ap_application_error.h
 *        \brief Serializer for SOME/IP application error.
 *        \unit SomeIpProtocol::Serdes::ApApplicationError
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_AP_APPLICATION_ERROR_H_
#define LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_AP_APPLICATION_ERROR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <string>
#include "ara/core/array.h"
#include "ser_forward.h"
#include "ser_sizing.h"
#include "ser_wrapper.h"
#include "someip-protocol/internal/ap_application_error.h"
#include "someip-protocol/internal/serialization/types.h"
#include "someip-protocol/internal/serialization/writer.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace serialization {

/*!
 * \brief Serialize the SomeIp application error.
 * \details The user message will always be empty.
 * \tparam TpPack Transformation properties parameter pack.
 * \param[in,out] w Writer holding the buffer to which data is serialized.
 * \param[out] s Struct, the serialized value shall be taken from.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 *
 * \internal
 * - Serialize error domain value.
 * - Serialize error code.
 * - Serialize support data.
 * - Serialize an empty string as user message.
 * \endinternal
 */
template <typename TpPack>
void SomeIpProtocolSerialize(Writer& w, serialization::ApApplicationErrorStruct const& s) noexcept {
  // clang-format off
  SomeIpProtocolSerialize<TpPack, typename Tp<TpPack>::ByteOrder>(w, s.error_domain_value);  // VCA_SOMEIPPROTOCOL_WRITER_REFERENCE
  SomeIpProtocolSerialize<TpPack, typename Tp<TpPack>::ByteOrder>(w, s.error_code);  // VCA_SOMEIPPROTOCOL_WRITER_REFERENCE
  SomeIpProtocolSerialize<TpPack, typename Tp<TpPack>::ByteOrder>(w, s.support_data);  // VCA_SOMEIPPROTOCOL_WRITER_REFERENCE
  amsr::core::ErrorDomain::StringType const empty_user_message{""};
  SomeIpProtocolSerialize<TpPack, LengthSize<Tp<TpPack>::kSizeOfStringLengthField, typename Tp<TpPack>::ByteOrder>>( // VCA_SOMEIPPROTOCOL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
      w, ara::core::String{empty_user_message}); // VCA_SOMEIPPROTOCOL_STRING_CONSTRUCTOR
  // clang-format on
}

/*!
 * \brief Check if the struct is static or not.
 * \tparam TpPack Transformation properties parameter pack.
 * \return True if it is static size, false if it is not.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 */
template <typename TpPack>
constexpr bool IsStaticSize(SizeToken<serialization::ApApplicationErrorStruct>) noexcept {
  return true;
}

/*!
 * \brief Get the required buffer size of SomeIp application error.
 * \details With R19-11 the user message was removed from the ApApplicationError, so the size is not included in
 * the calculation.
 * \tparam TpPack Transformation properties parameter pack.
 * \param[in] s SomeIp application error instance.
 * \return The required buffer size of SomeIp application error.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 *
 * \internal
 * - Return the sum of required buffer sizes of all elements except 'user message' in ApApplicationErrorStruct.
 * \endinternal
 */
// VECTOR AV NC Architecture-ApiAnalysis Return_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
template <typename TpPack>
constexpr std::size_t GetRequiredBufferSize(serialization::ApApplicationErrorStruct const& s) noexcept {
  // user message will always be serialized as an empty message
  amsr::core::ErrorDomain::StringType const empty_user_message{""};
  // clang-format off
  return serialization::GetRequiredBufferSize<TpPack, typename serialization::Tp<TpPack>::ByteOrder>( // VCA_SOMEIPPROTOCOL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
             s.error_domain_value) +
         serialization::GetRequiredBufferSize<TpPack, typename serialization::Tp<TpPack>::ByteOrder>(s.error_code) +  // VCA_SOMEIPPROTOCOL_REQUIRED_BUFFER_SIZE
         serialization::GetRequiredBufferSize<TpPack, typename serialization::Tp<TpPack>::ByteOrder>(s.support_data) +  // VCA_SOMEIPPROTOCOL_REQUIRED_BUFFER_SIZE
         serialization::GetRequiredBufferSize<TpPack,
                                              internal::LengthSize<serialization::Tp<TpPack>::kSizeOfStringLengthField,
                                                                   typename serialization::Tp<TpPack>::ByteOrder>>(
             ara::core::String{empty_user_message}); // VCA_SOMEIPPROTOCOL_STRING_CONSTRUCTOR
  // clang-format on
}

/*!
 * \brief Get the maximum buffer size of SomeIp application error.
 * \details With R19-11 the user message was removed from the ApApplicationError, so the size is not included in
 * the calculation.
 * \tparam TpPack Transformation properties parameter pack.
 * \return The required buffer size of SomeIp application error.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 *
 * \internal
 * - Return the sum of required buffer sizes of all elements except 'user message' in ApApplicationErrorStruct.
 * \endinternal
 */
// VECTOR AV NC Architecture-ApiAnalysis Return_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
template <typename TpPack>
constexpr InfSizeT GetMaximumBufferSize(serialization::ApApplicationErrorStruct const&) noexcept {
  InfSizeT size{};
  serialization::ApApplicationErrorStruct const s{};
  // clang-format off
  size += serialization::GetMaximumBufferSize<TpPack, typename serialization::Tp<TpPack>::ByteOrder>( // VCA_SOMEIPPROTOCOL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
             s.error_domain_value);
  size += serialization::GetMaximumBufferSize<TpPack, typename serialization::Tp<TpPack>::ByteOrder>(s.error_code);
  size += serialization::GetMaximumBufferSize<TpPack, typename serialization::Tp<TpPack>::ByteOrder>(s.support_data);
  // user message will always be serialized as an empty message
  size += serialization::GetMaximumBufferSize<TpPack,
                                              internal::LengthSize<serialization::Tp<TpPack>::kSizeOfStringLengthField,
                                                                   typename serialization::Tp<TpPack>::ByteOrder>>(
             ara::core::String{}); // VCA_SOMEIPPROTOCOL_STRING_CONSTRUCTOR
  // clang-format on
  return size;
}

/*!
 * \brief Serializer for 'ApApplicationError'.
 */
class ApApplicationErrorSerializer {
 public:
  /*!
   * \brief Serialize the SOME/IP application error.
   * \param[in,out] writer Writer holding the buffer to which data is serialized.
   * \param[in] app_error Application error which has to be serialized.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - Serialize the SOME/IP application error.
   * \endinternal
   */
  static void Serialize(Writer& writer, serialization::ApApplicationError const& app_error) {
    // Serialize the Application Error
    SomeIpProtocolSerialize<
        TpPackAlias,
        // Config of variant/union length field
        LengthSize<Tp<TpPackAlias>::kSizeOfUnionLengthField, typename Tp<TpPackAlias>::ByteOrder>,
        // Config of struct inside the variant
        ConfPack<LengthSize<Tp<TpPackAlias>::kSizeOfStructLengthField, typename Tp<TpPackAlias>::ByteOrder>>>(
        writer, app_error);
  }

  /*!
   * \brief Wrapper of the function outside the class.
   * \param[in] s SomeIp application error instance.
   * \return The required buffer size of SomeIp application error.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - Return the required buffer size of the SOME/IP application error.
   * \endinternal
   */
  // VECTOR AV NC Architecture-ApiAnalysis Return_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
  static std::size_t GetRequiredBufferSize(serialization::ApApplicationError const& s) {
    return serialization::template GetRequiredBufferSize<
        TpPackAlias, LengthSize<Tp<TpPackAlias>::kSizeOfUnionLengthField, typename Tp<TpPackAlias>::ByteOrder>,
        ConfPack<LengthSize<Tp<TpPackAlias>::kSizeOfStructLengthField, typename Tp<TpPackAlias>::ByteOrder>>>(s);
  }

  /*!
   * \brief Wrapper of the function outside the class.
   * \return The required buffer size of SomeIp application error.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - Return the required buffer size of the SOME/IP application error.
   * \endinternal
   */
  // VECTOR AV NC Architecture-ApiAnalysis Return_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
  static constexpr InfSizeT GetMaximumBufferSize() {
    return serialization::template GetMaximumBufferSize<
        serialization::ApApplicationError, TpPackAlias,
        LengthSize<Tp<TpPackAlias>::kSizeOfUnionLengthField, typename Tp<TpPackAlias>::ByteOrder>,
        ConfPack<LengthSize<Tp<TpPackAlias>::kSizeOfStructLengthField, typename Tp<TpPackAlias>::ByteOrder>>>();
  }

 private:
  /*!
   * \brief Transformation properties for serialization of a SOME/IP application error.
   */
  using TpPackAlias = TpPack<BigEndian, SizeOfArrayLengthField<0U>, SizeOfVectorLengthField<0U>,
                             SizeOfMapLengthField<0U>, SizeOfStringLengthField<2U>, SizeOfStructLengthField<2U>,
                             SizeOfUnionLengthField<4U>, SizeOfUnionTypeSelectorField<1U>, StringBomActive,
                             StringNullTerminationActive, DynamicLengthFieldSizeInactive, internal::Utf8Encoding>;
};

}  // namespace serialization
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_AP_APPLICATION_ERROR_H_
