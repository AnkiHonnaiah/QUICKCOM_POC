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
/**        \file deser_wrapper.h
 *         \brief Deserialization dispatcher.
 *         \unit SomeIpProtocol::Serdes::Deserialization
 *         \complexity High metrics are expected for complex data structures due to the nature of the recursive design.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_WRAPPER_H_
#define LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_WRAPPER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <string>
#include <type_traits>
#include <utility>
#include "ara/core/array.h"
#include "ara/core/map.h"
#include "ara/core/optional.h"
#include "ara/core/string.h"
#include "ara/core/variant.h"
#include "ara/core/vector.h"
#include "deser_array.h"
#include "deser_forward.h"
#include "deser_lengthfield.h"
#include "deser_map.h"
#include "deser_sizing.h"
#include "deser_string.h"
#include "deser_variant.h"
#include "deser_vector.h"
#include "someip-protocol/internal/deserialization/reader.h"
#include "someip-protocol/internal/deserialization/types.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace deserialization {

namespace detail {

/*!
 * \brief Calls the deserialization function for a struct with a length field.
 * \tparam TpPack Transformation properties.
 * \tparam LengthConf Length field configuration for the length field itself.
 * \tparam UserStruct Type of struct.
 * \param[in,out] r Buffer Reader holding the view on the serialized data.
 * \param[out] s Struct the data shall be deserialized into.
 * \return true if deserialization was successful, false otherwise.
 * \pre VerifySize has been called for the sizeOf(length field)
 *      or the static size of the struct if no length field is present.
 * \context Reactor|App
 * \reentrant TRUE for different reader objects.
 * \trace SPEC-4981322
 *
 * \internal
 * - If struct has a length field
 *   - Deserialize Length field via length field deserializer
 *   - Verify the static size of the struct members
 * - Dispatch deserialization to generated data type deserializer.
 * \endinternal
 */
// VECTOR AV NC Architecture-ApiAnalysis Local_Var_Of_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
template <typename TpPack, typename LengthConf, typename UserStruct>
auto SomeIpProtocolDeserialize(Reader& r, UserStruct& s) noexcept
    -> std::enable_if_t<(!IsPrimitiveType<UserStruct>::value) && (!IsEnumType<UserStruct>::value) &&
                            (!traits::IsBasicString<UserStruct>::value),
                        Result> {
  bool result{false};
  return DeserializeWithLengthField<LengthConf>(r, [&result, &s](Reader& subReader) {
    // Has the struct a length field?
    // VECTOR NL AutosarC++17_10-M0.1.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.1.2_dead_branch
    if (LengthConf::value != 0U) {
      // If the struct has a length field then till now only the length field of the struct is consumed, so static size
      // of subelements must be consumed now.
      // VECTOR AV NC Architecture-ApiAnalysis Local_Var_Of_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
      constexpr std::size_t static_size{
          SomeIpProtocolGetStaticSize<TpPack, LengthSize<0, typename LengthConf::endian>>(SizeToken<UserStruct>{})};
      result = subReader.VerifySize(static_size);
    } else {
      result = true;
    }

    // If the struct has no length field then till now static size of subelements is already consumed.
    if (result == true) {
      // VCA_SOMEIPPROTOCOL_WITHIN_SPEC, VCA_SOMEIPPROTOCOL_READER_REFERENCE
      result = SomeIpProtocolDeserialize<TpPack>(subReader, s);
    }

    return result;
  });
}

/*!
 * \brief Calls the deserialization function for a primitive.
 * \tparam TpPack Transformation properties.
 * \tparam Conf Endianness.
 * \tparam Primitive Type of primitive.
 * \param[in,out] r Buffer Reader holding the view on the serialized data.
 * \param[out] t Primitive, the data shall be deserialized into.
 * \return Always true.
 * \pre VerifySize has been called for the sizeOf(Primitive).
 * \context Reactor|App
 * \reentrant TRUE for different reader objects.
 *
 * \internal
 * - Read primitive from byte stream into t.
 * \endinternal
 */
template <typename TpPack, typename Conf, typename Primitive>
auto SomeIpProtocolDeserialize(Reader& r, Primitive& t) noexcept
    -> std::enable_if_t<IsPrimitiveType<Primitive>::value, Result> {
  r.ReadPrimitive<std::decay_t<Primitive>, Conf>(t);  // VCA_SOMEIPPROTOCOL_READER_REFERENCE
  return true;
}

/*!
 * \brief Calls the deserialization function for a enum.
 * \tparam TpPack Transformation Property.
 * \tparam Conf Endianness.
 * \tparam EnumType Type of enum.
 * \param[in,out] r Buffer Reader holding the view on the serialized data.
 * \param[out] t Enum, the data shall be deserialized into.
 * \return Always true.
 * \pre VerifySize has been called for the sizeOf(EnumType).
 * \context Reactor|App
 * \reentrant TRUE for different reader objects.
 *
 * \internal
 * - Read enum from byte stream into t.
 * \endinternal
 */
// VECTOR NC AutosarC++17_10-A8.4.4: MD_SOMEIPPROTOCOL_AutosarC++17_10-A8.4.4_UsingReturnValue
template <typename TpPack, typename Conf, typename EnumType>
auto SomeIpProtocolDeserialize(Reader& r, EnumType& t) noexcept
    -> std::enable_if_t<IsEnumType<EnumType>::value, Result> {
  using UnderlyingType = typename std::underlying_type<EnumType>::type;
  UnderlyingType enumValue{};
  r.ReadPrimitive<UnderlyingType, Conf>(enumValue);
  // VECTOR NL AutosarC++17_10-A7.2.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-A7.2.1_MissingRangeCheck
  t = static_cast<EnumType>(enumValue);
  return true;
}

/*!
 * \brief Calls the deserialization function for an array.
 * \tparam TpPack Transformation properties.
 * \tparam LengthConf Length field configuration for the length field itself.
 * \tparam ItemConf Item configuration for the array elements holding (length field size and endianness info).
 * \tparam T Type of the array.
 * \tparam N Size of the array.
 * \param[in,out] r Buffer Reader holding the view on the serialized data.
 * \param[out] v Array, the data shall be deserialized into.
 * \return true if deserialization was successful, false otherwise.
 * \pre VerifySize has been called for the length field to be deserialized.
 * \context Reactor|App
 * \reentrant TRUE for different reader objects.
 *
 * \internal
 * - If array has a length field
 *   - Deserialize Length field via length field deserializer
 *   - Verify the static size of the array members
 * - Dispatch deserialization task to length field deserializer providing array deserializer as lambda function.
 * \endinternal
 */
template <typename TpPack, typename LengthConf, typename... ItemConf, typename T, std::size_t N>
Result SomeIpProtocolDeserialize(Reader& r, ara::core::Array<T, N>& v) noexcept {
  bool result{true};
  return DeserializeWithLengthField<LengthConf>(r, [&result, &v](Reader& subReader) {
    if (LengthConf::value != 0U) {
      // If the array has a length field then till now only the length field is consumed, so static size
      // of members must be consumed now.
      // VECTOR AV NC Architecture-ApiAnalysis Local_Var_Of_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
      constexpr std::size_t static_size{(SomeIpProtocolGetStaticSize<TpPack, ItemConf...>(SizeToken<T>{})) * N};
      result = subReader.VerifySize(static_size);
    }
    if (result) {
      result = DeserializeArray<TpPack, ItemConf...>(v, subReader);  // VCA_SOMEIPPROTOCOL_READER_REFERENCE
    }
    return result;
  });
}

/*!
 * \brief Calls the deserialization function for a string.
 * \tparam TpPack Transformation properties.
 * \tparam LengthConf Length field configuration for the length field itself.
 * \tparam String String type.
 * \param[in,out] r Buffer Reader holding the view on the serialized data.
 * \param[out] v String, the data shall be deserialized into.
 * \return true if deserialization was successful, false otherwise.
 * \pre VerifySize has been called for the length field to be deserialized.
 * \context Reactor|App
 * \reentrant TRUE for different reader objects.
 *
 * \internal
 * - Dispatch deserialization task to length field deserializer providing string deserializer as lambda function.
 * \endinternal
 */
template <typename TpPack, typename LengthConf, typename String,
          std::enable_if_t<traits::IsBasicString<String>::value>* = nullptr>
Result SomeIpProtocolDeserialize(Reader& r, String& v) noexcept {
  v.clear();  // VCA_SOMEIPPROTOCOL_WITHIN_SPEC
  return DeserializeWithLengthField<LengthConf>(
      r, [&v](Reader& subReader) { return DeserializeString<TpPack>(v, subReader); });
}

/*!
 * \brief Calls the deserialization function for a SOME/IP union.
 * \tparam TpPack Transformation properties.
 * \tparam LengthConf Length field configuration for the length field itself.
 * \tparam ItemConf Configuration of the sub-elements of the variant.
 * \tparam Ts Type of the underlying variants.
 * \param[in,out] r Buffer Reader holding the view on the serialized data.
 * \param[out] v String, the data shall be deserialized into.
 * \return true if deserialization was successful, false otherwise.
 * \pre VerifySize has been called for the length field to be deserialized.
 * \context Reactor|App
 * \reentrant TRUE for different reader objects.
 *
 * \internal
 * - Dispatch deserialization task to length field deserializer providing variant deserializer as lambda function.
 * \endinternal
 */
template <typename TpPack, typename LengthConf, typename... ItemConf, typename... Ts>
Result SomeIpProtocolDeserialize(Reader& r, ara::core::Variant<Ts...>& v) noexcept {
  constexpr std::size_t VarTypeSize{Tp<TpPack>::kSizeOfUnionTypeSelectorField};
  return DeserializeWithLengthField<LengthConf, VarTypeSize>(
      r, [&v](Reader& subReader) { return DeserializeVariant<TpPack, ItemConf...>(v, subReader); });
}

/*!
 * \brief Calls the deserialization function for a Map.
 * \tparam TpPack Transformation properties.
 * \tparam LengthConf Length field configuration for the length field itself.
 * \tparam ItemConf Configurations holding endianness and length information for keys and values.
 * \tparam K Type of used keys.
 * \tparam V Type of stored values.
 * \tparam C Comparison functor type.
 * \tparam Alloc Allocator type.
 * \param[in,out] r Buffer Reader holding the view on the serialized data.
 * \param[out] v Map, the data shall be deserialized into.
 * \return true if deserialization was successful, false otherwise.
 * \pre VerifySize has been called for the length field to be deserialized.
 * \context Reactor|App
 * \reentrant TRUE for different reader objects.
 *
 * \internal
 * - Dispatch deserialization task to length field deserializer providing map deserializer as lambda function.
 * \endinternal
 */
template <typename TpPack, typename LengthConf, typename... ItemConf, typename K, typename V, typename C,
          typename Alloc>
Result SomeIpProtocolDeserialize(Reader& r, ara::core::Map<K, V, C, Alloc>& v) noexcept {
  v.clear();  // VCA_SOMEIPPROTOCOL_WITHIN_SPEC
  return DeserializeWithLengthField<LengthConf>(
      r, [&v](Reader& subReader) { return DeserializeMap<TpPack, ItemConf...>(v, subReader); });
}

/*!
 * \brief Calls the deserialization function for a vector.
 * \tparam TpPack Transformation properties.
 * \tparam LengthConf Length field configuration for the length field itself.
 * \tparam ItemConf Item configuration for the vector elements holding
 *                  (length field size and endianness info).
 * \tparam T Type of vector.
 * \tparam Alloc Vector allocator.
 * \param[in,out] r Buffer Reader holding the view on the serialized data.
 * \param[out] v Vector, the data shall be deserialized into.
 * \return true if deserialization was successful, false otherwise.
 * \pre VerifySize has been called for the length field to be deserialized.
 * \context Reactor|App
 * \reentrant TRUE for different reader objects.
 *
 * \internal
 * - Dispatch deserialization task to length field deserializer providing vector deserializer as lambda function.
 * \endinternal
 */
template <typename TpPack, typename LengthConf, typename... ItemConf, typename T, typename Alloc>
Result SomeIpProtocolDeserialize(Reader& r, ara::core::Vector<T, Alloc>& v) noexcept {
  v.clear();  // VCA_SOMEIPPROTOCOL_WITHIN_SPEC
  return DeserializeWithLengthField<LengthConf>(r, [&v](Reader& subReader) {
    return DeserializeVector<TpPack, T, ItemConf...>(v, subReader);  // VCA_SOMEIPPROTOCOL_WITHIN_SPEC
  });
}

/*!
 * \brief Deserialize optional TLV members.
 * \tparam TpPack Transformation properties.
 * \tparam ItemConf Item configuration the optional element.
 * \tparam T Type of Optional.
 * \param[in,out] r Buffer Reader holding the view on the serialized data.
 * \param[out] v Vector, the data shall be deserialized into.
 * \return true if deserialization was successful, false otherwise.
 * \pre VerifySize has been called for the length field to be deserialized.
 * \context Reactor|App
 * \reentrant TRUE for different reader objects.
 *
 * \internal
 * - Emplace a value to optional.
 * - Call corresponding deserializer for the value.
 * \endinternal
 */
template <typename TpPack, typename... ItemConf, typename T>
Result SomeIpProtocolDeserialize(Reader& r, ara::core::Optional<T>& v) noexcept {
  // By the time we reach this function it is clear, the optional member is present.
  v.emplace();
  return SomeIpProtocolDeserialize<TpPack, ItemConf...>(r, *v);
}

}  // namespace detail

/*!
 * \brief Dispatches the deserialization job to the corresponding deserializer.
 * \details This is the entry function for the static code.
 * \tparam TpPack Transformation properties.
 * \tparam Head Either length size config or endianness.
 * \tparam Confs Configuration holding length field info and/or endianness info of the sub-elements.
 * \tparam Ts Type list required for variants only.
 * \param[in,out] r Buffer Reader holding the view on the serialized data.
 * \param[out] ts Argument list the deserialized data shall be stored into.
 * \return true if deserialization was successful, false otherwise.
 * \pre It has been verfied, that the next element to be deserialized
 *      be read from the reader by calling VerifySize.
 * \context Reactor|App
 * \reentrant TRUE for different reader objects.
 *
 * \internal
 * - Dispatch to the internal deserializers.
 * \endinternal
 */
template <typename TpPack, typename Head, typename... Confs, typename... Ts>
// VECTOR NL AutosarC++17_10-A13.3.1: MD_SOMEIPPROTOCOL_AutosarC++17_10-A13.3.1_overload_w_forwarding_reference
Result SomeIpProtocolDeserialize(Reader& r, Ts&&... ts) noexcept {
  // clang-format off
  return detail::SomeIpProtocolDeserialize<TpPack, Head, Confs...>(r, std::forward<Ts>(ts)...);  // VCA_SOMEIPPROTOCOL_READER_REFERENCE
  // clang-format on
}

}  // namespace deserialization
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr
#endif  // LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_WRAPPER_H_
