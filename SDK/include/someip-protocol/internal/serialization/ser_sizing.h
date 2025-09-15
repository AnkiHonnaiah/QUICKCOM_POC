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
/*!        \file ser_sizing.h
 *        \brief Defines GetBufferSize() for all supported primitives and containers.
          \details Types with conditional length fields require an instantiation of the LengthSize
                   tag type. Containers use a parameter pack named ItemConf which must contain
                   all configuration required to determine the size of an item.
 *        \unit SomeIpProtocol::Serdes::Serialization
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_SIZING_H_
#define LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_SIZING_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <cassert>
#include <map>
#include <string>
#include <type_traits>
#include <utility>
#include "amsr/core/abort.h"
#include "ara/core/array.h"
#include "ara/core/optional.h"
#include "ara/core/variant.h"
#include "ara/core/vector.h"
#include "ser_forward.h"
#include "ser_sizing_variant.h"
#include "someip-protocol/internal/is_static_size.h"
#include "someip-protocol/internal/serialization/infinity_size_t.h"
#include "someip-protocol/internal/serialization/types.h"
#include "someip-protocol/internal/serialization/utf8_to_utf16.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace serialization {

namespace detail {

// ----------------------------GetBufferSize Start---------------------------------
/*!
 * \brief Returns the length field size.
 * \tparam LengthConf Length information of the data.
 * \return The length field size. (May be zero.)
 */
template <typename LengthConf>
constexpr InfSizeT GetLengthFieldSize() noexcept {
  return InfSizeT{LengthConf::value};
}

/*!
 * \brief Size for primitives.
 * \tparam IterCfg Type of computation to be done by the recursion.
 * \tparam TpPack Transformation properties.
 * \tparam Endian Endianness.
 * \tparam Primitive Type of primitive.
 * \return Returns the calculated size. Auto is required due to usage of trailing return type.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 */
template <typename IterCfg, typename TpPack, typename Endian, typename Primitive>
// VECTOR NL AutosarC++17_10-A2.11.4: MD_SOMEIPPROTOCOL_AutosarC++17_10-A2.11.4_NameOfObjectOrFunctionReused
constexpr auto GetBufferSize(Primitive const&) noexcept
    -> std::enable_if_t<IsPrimitiveType<Primitive>::value, InfSizeT> {
  return InfSizeT{sizeof(Primitive)};
}

/*!
 * \brief Size for bool.
 * \tparam TpPack Transformation properties.
 * \tparam Endian Endianness.
 * \return Returns the calculated size. Auto is required due to usage of trailing return type.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 *
 * \internal
 * - The value of sizeof(bool) is implementation defined and might differ from 1.
 * - But for SOME/IP the serialized size of a bool is 1 byte.
 * \endinternal
 */
template <typename IterCfg, typename TpPack, typename Endian>
// VECTOR NL AutosarC++17_10-A2.11.4: MD_SOMEIPPROTOCOL_AutosarC++17_10-A2.11.4_NameOfObjectOrFunctionReused
auto constexpr GetBufferSize(bool const&) noexcept -> InfSizeT {
  return InfSizeT{sizeof(std::uint8_t)};
}

/*!
 * \brief Calculate the static size for Enum data type.
 * \tparam IterCfg Type of computation to be done by the recursion.
 * \tparam TpPack Transformation Property.
 * \tparam Endian Endianness information
 * \tparam EnumType Type of enum.
 * \return Returns the calculated size. Auto is required due to usage of trailing return type.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 *
 * \internal
 * - Return size of the underlying type of the enum.
 * \endinternal
 */
template <typename IterCfg, typename TpPack, typename Endian, typename EnumType>
constexpr auto GetBufferSize(EnumType const&) noexcept -> std::enable_if_t<IsEnumType<EnumType>::value, InfSizeT> {
  /*!
   * \brief The underlying type of the enum class.
   */
  using UnderlyingType = typename std::underlying_type<EnumType>::type;
  return InfSizeT{sizeof(UnderlyingType)};
}

/*!
 * \brief Catch-all for user generated data types.
 * \details This is less specialized than other templates, and will only be chosen when no other signature matches.
 * \tparam IterCfg Type of computation to be done by the recursion.
 * \tparam TpPack Transformation properties.
 * \tparam LengthConf Length field configuration for the length field itself.
 * \tparam UserStruct Type of struct.
 * \param[in] s Structure to serialize.
 * \return Returns the calculated size. Auto is required due to usage of trailing return type.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 *
 * \internal
 * - Return sum of length field size and required buffer size for the user struct.
 * \endinternal
 */
template <typename IterCfg, typename TpPack, typename LengthConf, typename UserStruct>
constexpr auto GetBufferSize(UserStruct const& s) noexcept
    -> std::enable_if_t<(!IsPrimitiveType<UserStruct>::value) && (!IsEnumType<UserStruct>::value) &&
                            (!traits::IsBasicString<UserStruct>::value),
                        InfSizeT> {
  InfSizeT ret{GetLengthFieldSize<LengthConf>()};
  // Continue the recursion from an autogenerated struct entry point (hence no config parameters).
  if (IterCfg::IsInstanceSizeIteration()) {
    ret += GetRequiredBufferSize<TpPack>(s);  // VCA_SOMEIPPROTOCOL_REQUIRED_BUFFER_SIZE
  } else {
    ret += GetMaximumBufferSize<TpPack>(UserStruct{});
  }
  return ret;
}

/*!
 * \brief Size of non-static array.
 * \details Non-static size items requires looping to calculate array size.
 * \tparam IterCfg Type of computation to be done by the recursion.
 * \tparam TpPack Transformation properties.
 * \tparam LengthConf Length field configuration for the length field itself.
 * \tparam ItemConf Item configuration for the array elements holding (length field size and endianness info).
 * \tparam T Type of the array.
 * \tparam N Size of the array.
 * \param[in] arr Array to serialize.
 * \return Returns the calculated size. Auto is required due to usage of trailing return type.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 *
 * \internal
 * - Loop over array and sum up the required buffer size of each element.
 * - Return total required buffer size.
 * \endinternal
 */
template <typename IterCfg, typename TpPack, typename LengthConf, typename... ItemConf, typename T, std::size_t N>
constexpr auto GetBufferSize(ara::core::Array<T, N> const& arr) noexcept
    -> std::enable_if_t<!IsStaticSizeForConf<TpPack, ItemConf...>(SizeToken<T>{}), InfSizeT> {
  constexpr InfSizeT length_field_size{GetLengthFieldSize<LengthConf>()};
  InfSizeT res{length_field_size};

  if (IterCfg::IsInstanceSizeIteration()) {
    for (T const& item : arr) {
      res += GetBufferSizeRoot<IterCfg, TpPack, ItemConf...>(item);  // VCA_SOMEIPPROTOCOL_REQUIRED_BUFFER_SIZE
    }
  } else {
    res += N * GetBufferSizeRoot<IterCfg, TpPack, ItemConf...>(arr[0]);
  }
  return res;
}

/*!
 * \brief Size of static array.
 * \details Size of array with static size items is a simple multiplication.
 * \tparam IterCfg Type of computation to be done by the recursion.
 * \tparam TpPack Transformation properties.
 * \tparam LengthConf Length field configuration for the length field itself.
 * \tparam ItemConf Item configuration for the array elements holding (length field size and endianness info).
 * \tparam T Type of the array.
 * \tparam N Size of the array.
 * \param[in] t Array to serialize.
 * \return Returns the calculated size. Auto is required due to usage of trailing return type.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 *
 * \internal
 * - Return N multiplied by the required buffer size for one element.
 * \endinternal
 */
template <typename IterCfg, typename TpPack, typename LengthConf, typename... ItemConf, typename T, std::size_t N>
constexpr auto GetBufferSize(ara::core::Array<T, N> const& t) noexcept
    -> std::enable_if_t<IsStaticSizeForConf<TpPack, ItemConf...>(SizeToken<T>{}), InfSizeT> {
  return (N * GetBufferSizeRoot<IterCfg, TpPack, ItemConf...>(t[0])) +  // VCA_SOMEIPPROTOCOL_REQUIRED_BUFFER_SIZE
         GetLengthFieldSize<LengthConf>();
}

// ----------------------------------Vector Start---------------------------------

/*!
 * \brief Size of vector with no ArraySize configured and non-static sized items.
 * \tparam IterCfg Type of computation to be done by the recursion.
 * \tparam TpPack Transformation properties.
 * \tparam LengthConf Length field configuration for the length field itself.
 * \tparam ArraySizeConf Array size item configuration: max. number of allowed vector elements.
 * \tparam ItemConf Item configuration for the vector elements holding (length field size
 *                  and endianness info).
 * \tparam T Type of vector.
 * \tparam Alloc Vector allocator.
 * \param[in] v Vector to serialize.
 * \return Returns the calculated size. Auto is required due to usage of trailing return type.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 *
 * \internal
 * - Fetch and store the size of the length field.
 * - Loop over vector
 *   - Add the required buffer size for each element to the stored size.
 * - Return the total size required.
 * \endinternal
 */
template <typename IterCfg, typename TpPack, typename LengthConf, typename ArraySizeConf, typename... ItemConf,
          typename T, typename Alloc>
constexpr auto GetBufferSize(ara::core::Vector<T, Alloc> const& v) noexcept
    -> std::enable_if_t<(!IsStaticSizeForConf<TpPack, ItemConf...>(SizeToken<T>{})) &&
                            (!TypeTraitIsVectorArraySizeLimitActive<ArraySizeConf>()),
                        InfSizeT> {
  constexpr InfSizeT length_field_size{GetLengthFieldSize<LengthConf>()};
  InfSizeT res{length_field_size};
  if (IterCfg::IsInstanceSizeIteration()) {
    for (T const& item : v) {
      // VCA_SOMEIPPROTOCOL_REQUIRED_BUFFER_SIZE
      res += GetBufferSizeRoot<IterCfg, TpPack, ItemConf...>(item);
    }
  } else {
    res = InfSizeT::Infinity();
  }
  return res;
}

/*!
 * \brief Size of vector with ArraySize configured and non-static sized items.
 * \tparam IterCfg Type of computation to be done by the recursion.
 * \tparam TpPack Transformation properties.
 * \tparam LengthConf Length field configuration for the length field itself.
 * \tparam ArraySizeConf Array size item configuration: max. number of allowed vector elements.
 * \tparam ItemConf Item configuration for the vector elements holding (length field size
 *                  and endianness info).
 * \tparam T Type of vector.
 * \tparam Alloc Vector allocator.
 * \param[in] v Vector to serialize.
 * \return Returns the calculated size. Auto is required due to usage of trailing return type.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 *
 * \internal
 * - Fetch and store the size of the length field.
 * - Loop over the full vector or until max number of element is reached(defined by ArraySizeConf).
 *   - Add the required buffer size for each element looped over to the stored size.
 * - Return the total size required.
 * \endinternal
 */
// VECTOR AV NC Architecture-ApiAnalysis Local_Var_Of_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
template <typename IterCfg, typename TpPack, typename LengthConf, typename ArraySizeConf, typename... ItemConf,
          typename T, typename Alloc>
constexpr auto GetBufferSize(ara::core::Vector<T, Alloc> const& v) noexcept
    -> std::enable_if_t<(!IsStaticSizeForConf<TpPack, ItemConf...>(SizeToken<T>{})) &&
                            TypeTraitIsVectorArraySizeLimitActive<ArraySizeConf>(),
                        InfSizeT> {
  constexpr InfSizeT length_field_size{GetLengthFieldSize<LengthConf>()};
  InfSizeT res{length_field_size};

  if (IterCfg::IsInstanceSizeIteration()) {
    // VECTOR AV NC Architecture-ApiAnalysis Local_Var_Of_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
    std::size_t const nr_elements{std::min(v.size(), ArraySizeConf::value)};
    for (std::size_t element_index{0U}; element_index < nr_elements; ++element_index) {
      // VCA_SOMEIPPROTOCOL_REQUIRED_BUFFER_SIZE
      res += GetBufferSizeRoot<IterCfg, TpPack, ItemConf...>(v[element_index]);
    }
  } else {
    res += ArraySizeConf::value * GetBufferSizeRoot<IterCfg, TpPack, ItemConf...>(
                                      decltype(v[0]){});  // VCA_SOMEIPPROTOCOL_REQUIRED_BUFFER_SIZE
  }
  return res;
}

/*!
 * \brief Size of vector with no ArraySize configured and static sized items.
 * \tparam IterCfg Type of computation to be done by the recursion.
 * \tparam TpPack Transformation properties.
 * \tparam LengthConf Length field configuration for the length field itself.
 * \tparam ArraySizeConf Array size item configuration: max. number of allowed vector elements.
 * \tparam ItemConf Item configuration for the vector elements holding (length field size
 *                  and endianness info).
 * \tparam T Type of vector.
 * \tparam Alloc Vector allocator.
 * \param[in] v Vector to serialize.
 * \return Returns the calculated size. Auto is required due to usage of trailing return type.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 *
 * \internal
 * - Fetch and store the size of the length field.
 * - If vector is not empty
 *   - Add number of element in vector multiplied by required buffer size for one element to the stored size.
 * - Return the total size required.
 * \endinternal
 */
template <typename IterCfg, typename TpPack, typename LengthConf, typename ArraySizeConf, typename... ItemConf,
          typename T, typename Alloc>
constexpr auto GetBufferSize(ara::core::Vector<T, Alloc> const& v) noexcept
    -> std::enable_if_t<IsStaticSizeForConf<TpPack, ItemConf...>(SizeToken<T>{}) &&
                            (!TypeTraitIsVectorArraySizeLimitActive<ArraySizeConf>()),
                        InfSizeT> {
  constexpr InfSizeT length_field_size{GetLengthFieldSize<LengthConf>()};
  InfSizeT result{length_field_size};
  if (IterCfg::IsInstanceSizeIteration()) {
    // clang-format off
    if (!v.empty()) {  // VCA_SOMEIPPROTOCOL_CALL_NONSTATIC_METHOD_FOR_POSSIBLY_INVALID_OBJECT
      result += v.size() * GetBufferSizeRoot<IterCfg, TpPack, ItemConf...>(v[0]);  // VCA_SOMEIPPROTOCOL_REQUIRED_BUFFER_SIZE
    }
    // clang-format on
  } else {
    result = InfSizeT::Infinity();
  }
  return result;
}

/*!
 * \brief Size of vector with ArraySize configured and static sized items.
 * \tparam IterCfg Type of computation to be done by the recursion.
 * \tparam TpPack Transformation properties.
 * \tparam LengthConf Length field configuration for the length field itself.
 * \tparam ArraySizeConf Array size item configuration: max. number of allowed vector elements.
 * \tparam ItemConf Item configuration for the vector elements holding (length field size
 *                  and endianness info).
 * \tparam T Type of vector.
 * \tparam Alloc Vector allocator.
 * \param[in] v Vector to serialize.
 * \return Returns the calculated size. Auto is required due to usage of trailing return type.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 *
 * \internal
 * - Fetch and store size of length field.
 * - If vector is not empty
 *   - Calculate number of used elements by comparing the size of the vector with the configured ArraySizeConf,
 *     whichever is smallest is used.
 *   - Add number of used elements multiplied by the required buffer size for one element to the stored size.
 * - Return the total size required.
 * \endinternal
 */
// VECTOR AV NC Architecture-ApiAnalysis Local_Var_Of_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
template <typename IterCfg, typename TpPack, typename LengthConf, typename ArraySizeConf, typename... ItemConf,
          typename T, typename Alloc>
constexpr auto GetBufferSize(ara::core::Vector<T, Alloc> const& v) noexcept
    -> std::enable_if_t<IsStaticSizeForConf<TpPack, ItemConf...>(SizeToken<T>{}) &&
                            TypeTraitIsVectorArraySizeLimitActive<ArraySizeConf>(),
                        InfSizeT> {
  constexpr InfSizeT length_field_size{GetLengthFieldSize<LengthConf>()};
  InfSizeT result{length_field_size};
  if (IterCfg::IsInstanceSizeIteration()) {
    // clang-format off
    if (!v.empty()) {  // VCA_SOMEIPPROTOCOL_CALL_NONSTATIC_METHOD_FOR_POSSIBLY_INVALID_OBJECT
      // clang-format on
      // VECTOR AV NC Architecture-ApiAnalysis Local_Var_Of_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
      std::size_t const nr_elements{std::min(v.size(), ArraySizeConf::value)};
      result += nr_elements *
                GetBufferSizeRoot<IterCfg, TpPack, ItemConf...>(v[0]);  // VCA_SOMEIPPROTOCOL_REQUIRED_BUFFER_SIZE
    }
  } else {
    result += ArraySizeConf::value * GetBufferSizeRoot<IterCfg, TpPack, ItemConf...>(
                                         decltype(v[0]){});  // VCA_SOMEIPPROTOCOL_REQUIRED_BUFFER_SIZE
  }
  return result;
}

// ----------------------------------Vector End---------------------------------

// ------------------------------Vector<bool> Begin-----------------------------
/*!
 * \brief Size of vector<bool> with no ArraySize configured and are static sized items.
 * \tparam IterCfg Type of computation to be done by the recursion.
 * \tparam TpPack Transformation properties.
 * \tparam LengthConf Length field configuration for the length field itself.
 * \tparam ArraySizeConf Array size item configuration: max. number of allowed vector elements.
 * \tparam ItemConf Item configuration for the vector elements holding (length field size
 *                  and endianness info).
 * \tparam Alloc Vector allocator.
 * \param[in] v Vector to serialize.
 * \return Returns the calculated size. Auto is required due to usage of trailing return type.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 *
 * \internal
 * - The value of sizeof(bool) is implementation defined and might differ from 1.
 * - But for SOME/IP the serialized size of a bool is 1 byte.
 * - Fetch and store the size of the length field.
 * - If vector is not empty
 *   - Add number of element in vector multiplied by required buffer size for bool to the stored size.
 * - Return the total size required.
 * \endinternal
 */
// VECTOR NC AutosarC++17_10-A18.1.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-A18.1.2_use_of_vector_bool
template <typename IterCfg, typename TpPack, typename LengthConf, typename ArraySizeConf, typename... ItemConf,
          typename Alloc>
constexpr auto GetBufferSize(ara::core::Vector<bool, Alloc> const& v) noexcept
    -> std::enable_if_t<!TypeTraitIsVectorArraySizeLimitActive<ArraySizeConf>(), InfSizeT> {
  constexpr InfSizeT length_field_size{GetLengthFieldSize<LengthConf>()};
  InfSizeT result{length_field_size};
  if (IterCfg::IsInstanceSizeIteration()) {
    // clang-format off
    if (!v.empty()) {  // VCA_SOMEIPPROTOCOL_CALL_NONSTATIC_METHOD_FOR_POSSIBLY_INVALID_OBJECT
      // clang-format on
      result += v.size() * sizeof(std::uint8_t);
    }
  } else {
    result = InfSizeT::Infinity();
  }
  return result;
}

/*!
 * \brief Size of vector<bool> with ArraySize configured and are static sized items.
 * \tparam IterCfg Type of computation to be done by the recursion.
 * \tparam TpPack Transformation properties.
 * \tparam LengthConf Length field configuration for the length field itself.
 * \tparam ArraySizeConf Array size item configuration: max. number of allowed vector elements.
 * \tparam ItemConf Item configuration for the vector elements holding (length field size
 *                  and endianness info).
 * \tparam Alloc Vector allocator.
 * \param[in] v Vector to serialize.
 * \return Returns the calculated size. Auto is required due to usage of trailing return type.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 *
 * \internal
 * - The value of sizeof(bool) is implementation defined and might differ from 1.
 * - But for SOME/IP the serialized size of a bool is 1 byte.
 * - Fetch and store size of length field.
 * - If vector is not empty
 *   - Calculate number of used elements by comparing the size of the vector with the configured ArraySizeConf,
 *     whichever is smallest is used.
 *   - Add number of used elements multiplied by the required buffer size for bool to the stored size.
 * - Return the total size required.
 * \endinternal
 */
// VECTOR NC AutosarC++17_10-A18.1.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-A18.1.2_use_of_vector_bool
template <typename IterCfg, typename TpPack, typename LengthConf, typename ArraySizeConf, typename... ItemConf,
          typename Alloc>
constexpr auto GetBufferSize(ara::core::Vector<bool, Alloc> const& v) noexcept
    -> std::enable_if_t<TypeTraitIsVectorArraySizeLimitActive<ArraySizeConf>(), InfSizeT> {
  constexpr InfSizeT length_field_size{GetLengthFieldSize<LengthConf>()};
  InfSizeT result{length_field_size};
  if (IterCfg::IsInstanceSizeIteration()) {
    // clang-format off
    if (!v.empty()) {  // VCA_SOMEIPPROTOCOL_CALL_NONSTATIC_METHOD_FOR_POSSIBLY_INVALID_OBJECT
      // clang-format on
      std::size_t const nr_elements{std::min(v.size(), ArraySizeConf::value)};
      result += nr_elements * sizeof(std::uint8_t);
    }
  } else {
    result += ArraySizeConf::value * sizeof(std::uint8_t);
  }
  return result;
}

// -------------------------------Vector<bool> End------------------------------

/*!
 * \brief Base template for unpacking ConfPack for key and value.
 * \tparam IterCfg Type of computation to be done by the recursion.
 * \tparam TpPack Transformation properties.
 * \tparam KeyConf Configuration for the type of used keys.
 * \tparam ValueConf Configuration for the type of values.
 */
template <typename IterCfg, typename TpPack, typename KeyConf, typename ValueConf>
class GetBufferSizeForMap;

/*!
 * \brief Unpacks ConfPack for key and value of a map.
 * \tparam IterCfg Type of computation to be done by the recursion.
 * \tparam TpPack Transformation properties.
 * \tparam KeyConf Configuration for the type of used keys.
 * \tparam ValueConf Configuration for the type of values.
 */
template <typename IterCfg, typename TpPack, typename... KeyConf, typename... ValueConf>
class GetBufferSizeForMap<IterCfg, TpPack, ConfPack<KeyConf...>, ConfPack<ValueConf...>> {
 public:
  /*!
   * \brief Size of map with static sized items.
   * \tparam K Type of used keys.
   * \tparam V Type of values.
   * \tparam C Comparison function.
   * \tparam C Comparison functor type.
   * \tparam Alloc Allocator type.
   * \tparam Alloc Type of allocator.
   * \param[in] map Map to serialize.
   * \return Returns the calculated size. Auto is required due to usage of trailing return type.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - Initialize a variable to store required buffer size to zero.
   * - Loop over map
   *   - for each entry add the sum of required buffer size of the key and the value to the stored size.
   * - Return the total size required.
   * \endinternal
   */
  template <typename K, typename V, typename C, typename Alloc>
  static constexpr auto get(ara::core::Map<K, V, C, Alloc> const& map) noexcept
      -> std::enable_if_t<(!IsStaticSizeForConf<TpPack, KeyConf...>(SizeToken<K>{})) ||
                              (!IsStaticSizeForConf<TpPack, ValueConf...>(SizeToken<V>{})),
                          InfSizeT> {
    InfSizeT res{0U};
    if (IterCfg::IsInstanceSizeIteration()) {
      for (std::pair<K, V> const& p : map) {  // VCA_SOMEIPPROTOCOL_MAP_REFERENCE
        res += GetBufferSizeRoot<IterCfg, TpPack, KeyConf...>(p.first) +
               GetBufferSizeRoot<IterCfg, TpPack, ValueConf...>(p.second);
      }  // VCA_SOMEIPPROTOCOL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
    } else {
      res = InfSizeT::Infinity();
    }
    return res;
  }

  /*!
   * \brief Size of map with items which are static size.
   * \tparam K Type of used keys.
   * \tparam V Type of values.
   * \tparam C Comparison function.
   * \tparam Alloc Type of allocator.
   * \param[in] map Map to serialize.
   * \return Returns the calculated size. Auto is required due to usage of trailing return type.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - Initialize a variable to store required buffer size to zero.
   * - If map is not empty
   *   - add number of map entries multiplied by required buffer size of the key and value to the stored size.
   * - Return the total size required.
   * \endinternal
   */
  template <typename K, typename V, typename C, typename Alloc>
  static constexpr auto get(ara::core::Map<K, V, C, Alloc> const& map) noexcept
      -> std::enable_if_t<IsStaticSizeForConf<TpPack, KeyConf...>(SizeToken<K>{}) &&
                              IsStaticSizeForConf<TpPack, ValueConf...>(SizeToken<V>{}),
                          InfSizeT> {
    InfSizeT result{0U};
    if (IterCfg::IsInstanceSizeIteration()) {
      // clang-format off
      if (!map.empty()) {  // VCA_SOMEIPPROTOCOL_CALL_NONSTATIC_METHOD_FOR_POSSIBLY_INVALID_OBJECT
        // clang-format on
        std::pair<K, V> const& p{*map.begin()};  // VCA_SOMEIPPROTOCOL_MAP_REFERENCE
        result = map.size() * (GetBufferSizeRoot<IterCfg, TpPack, KeyConf...>(p.first) +
                               GetBufferSizeRoot<IterCfg, TpPack, ValueConf...>(p.second));
      }
    } else {
      result = InfSizeT::Infinity();
    }
    return result;
  }
};

/*!
 * \brief Size of a map.
 * \tparam IterCfg Type of computation to be done by the recursion.
 * \tparam TpPack Transformation properties.
 * \tparam LengthConf Length field configuration for the length field itself.
 * \tparam KeyConf Configuration for the type of used keys.
 * \tparam ValueConf Configuration for the type of values.
 * \tparam Map Type of the map.
 * \param[in] map Map to serialize.
 * \return Returns the calculated size.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 */
template <typename IterCfg, typename TpPack, typename LengthConf, typename KeyConf, typename ValueConf, typename Map>
constexpr auto GetBufferSize(Map const& map) noexcept -> std::enable_if_t<IsMap<Map>::value, InfSizeT> {
  return IterCfg::IsInstanceSizeIteration()
             ? (InfSizeT{GetLengthFieldSize<LengthConf>() +
                         GetBufferSizeForMap<IterCfg, TpPack, KeyConf, ValueConf>::get(map)})
             : InfSizeT::Infinity();
}

/*!
 * \brief Size of a string(UTF-8/UTF-16).
 * \tparam IterCfg Type of computation to be done by the recursion.
 * \tparam TpPack Transformation properties.
 * \tparam LengthConf Length field configuration for the length field itself.
 * \tparam String Type of string.
 * \param[in] s String to serialize.
 * \return Returns the calculated size.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 *
 * \internal
 * - Fetch and store the sum of the size of the length field and the size of the string.
 * - If BOM is active
 *   - add the size of the BOM to the stored size.
 * - If null termination is active
 *   - add the size of null termination to the stored size.
 * - Return the total size required.
 * \endinternal
 */
template <typename IterCfg, typename TpPack, typename LengthConf, typename String,
          std::enable_if_t<traits::IsBasicString<String>::value>* = nullptr>
constexpr InfSizeT GetBufferSize(String const& s) noexcept {
  InfSizeT result{GetLengthFieldSize<LengthConf>()};
  // Check if BOM is active
  // VECTOR NL AutosarC++17_10-M0.1.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.1.2_dead_branch
  if (Tp<TpPack>::StringIsBomActive::value) {
    std::size_t length_string_bom{3U};  // Default UTF-8 Encoding
    if (Tp<TpPack>::StringEncoding::value == EStringEncoding::kUTF_16) {
      length_string_bom = 2U;
    }
    result += length_string_bom;
  }

  // Check if NullTermination is active
  // VECTOR NL AutosarC++17_10-M0.1.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.1.2_dead_branch
  if (Tp<TpPack>::StringIsNullTerminationActive::value) {
    std::size_t length_string_null_termination{1U};  // Default String Encoding.
    if (Tp<TpPack>::StringEncoding::value == EStringEncoding::kUTF_16) {
      length_string_null_termination = sizeof(char16_t);
    }

    result += length_string_null_termination;
  }

  if (IterCfg::IsInstanceSizeIteration()) {
    if (Tp<TpPack>::StringEncoding::value == EStringEncoding::kUTF_16) {
      result += Utf8ToUtf16<LittleEndian>::GetUtf16Length(s);
    } else {
      result += s.size();
    }
  } else {
    result = InfSizeT::Infinity();
  }
  return result;
}

/*!
 * \brief Size of a variant.
 * \tparam IterCfg Type of computation to be done by the recursion.
 * \tparam TpPack Transformation properties.
 * \tparam LengthConf Length field configuration for the length field itself.
 * \tparam ItemConf Configuration for the item of the variant
 * \tparam T Type of variant.
 * \param[in] v Variant to serialize.
 * \return Returns the calculated size.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 *
 * \internal
 * - Initialize a variable to store required buffer size to zero.
 * - If variant is not invalid
 *   - add the sum of the length field and the required buffer size for the variant to the stored size.
 * - Return the total size required.
 * \endinternal
 */
template <typename IterCfg, typename TpPack, typename LengthConf, typename... ItemConf, typename... Ts>
constexpr InfSizeT GetBufferSize(ara::core::Variant<Ts...> const& v) noexcept {
  InfSizeT required_size{0U};
  if ((!IterCfg::IsInstanceSizeIteration()) || (!v.valueless_by_exception())) {
    constexpr InfSizeT length_field_size{GetLengthFieldSize<LengthConf>()};
    // VCA_SOMEIPPROTOCOL_REQUIRED_BUFFER_SIZE
    required_size = length_field_size + GetBufferSizeVariantImpl<IterCfg, TpPack, ItemConf...>(v);
  }
  return required_size;
}

// VECTOR NC AutosarC++17_10-M7.1.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-M7.1.2_ParameterDeclaredAsConst
/*!
 * \brief Size of an Optional.
 * \details This is NOT how TLV treat Optionals. Instead this implementation is here to allow compilation, and
 * NOT usage, of modeled data types with optional members. These are used for signal-based update bits. The
 * Signal2ServiceSerDes generator will create serializers/deserializers for this use-case. However, the SomeIpProtocol
 * generator will also generate serializers/deserializers. These will never be used, but still have to compile.
 * \tparam IterCfg Type of computation to be done by the recursion.
 * \tparam TpPack Transformation properties.
 * \tparam ItemConf Configuration for the item of the Optional
 * \tparam T Contained type.
 * \param[in] o Optional to serialize.
 * \return Returns the calculated size.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 *
 * \internal
 * - Initialize a variable to store required buffer size to zero.
 * - If instance-based and the Optional has value
 *   - add the size of the stored object.
 * - Else
 *   - add the size of a default constructed object.
 * - Return the total size required.
 * \endinternal
 */
template <typename IterCfg, typename TpPack, typename... ItemConf, typename T>
constexpr auto GetBufferSize(ara::core::Optional<T> const& o) noexcept -> InfSizeT {
  static_cast<void>(o);
  amsr::core::Abort(
      "Non-TLV optional is not supported for SOME/IP. This code should compile, to allow for signal-based update "
      "bits.");
  return InfSizeT{};
}

// ----------------------------GetBufferSize End---------------------------------
// ----------------------------GetTlvFieldBufferSize Start ---------------------------------

/*!
 * \brief Base template for the Tlv field required buffer size.
 * \tparam IterCfg Type of computation to be done by the recursion.
 * \tparam TpPack Transformation properties.
 * \tparam Head Either length size config or endianness.
 * \tparam T Type of the member.
 * \tparam hasLengthField True if the field should have a length field.
 * \tparam ItemConfs Configuration of the members.
 */
template <typename IterCfg, typename TpPack, typename Head, typename T, bool hasLengthField, typename... ItemConfs>
class GetTlvFieldBufferSizeImpl;

/*!
 * \brief Specialization for primitive or enum type with provided config as a Tlv field.
 * \tparam IterCfg Type of computation to be done by the recursion.
 * \tparam TpPack Transformation properties.
 * \tparam Head Either length size config or endianness.
 * \tparam T Type of the member.
 * \tparam ItemConfs Configuration of the members.
 */
template <typename IterCfg, typename TpPack, typename Head, typename T, typename... ItemConfs>
class GetTlvFieldBufferSizeImpl<IterCfg, TpPack, Head, T, false, ItemConfs...> {
 public:
  /*!
   * \brief Returns the required buffer size to serialize a primitive or enum type with provided config as a Tlv field.
   * \param[in] t Data to be serialized.
   * \return Required buffer size for TLV serialization.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - Initialize a variable to store required buffer size to zero.
   * - Fetch the required buffer size for the data.
   * - If the fetched size is greater than 0
   *   - add the sum of the fetched size and the size of the tlv tag to the stored size.
   * - Return the total size required.
   * \endinternal
   */
  static constexpr InfSizeT Get(T const& t) noexcept {
    constexpr std::size_t kTlvTagSize{2U};
    InfSizeT const kTlvValueSize{GetBufferSizeRoot<IterCfg, TpPack, Head, ItemConfs...>(t)};
    InfSizeT const result{kTlvTagSize + kTlvValueSize};
    return result;
  }
};

/*!
 * \brief Specialization for non-primitive type that is not an optional with provided config as a Tlv field.
 * \tparam IterCfg Type of computation to be done by the recursion.
 * \tparam TpPack Transformation properties.
 * \tparam Head Either length size config or endianness.
 * \tparam ItemConfs Configuration of the members.
 * \tparam T Type of the member.
 */
template <typename IterCfg, typename TpPack, typename Head, typename T, typename... ItemConfs>
class GetTlvFieldBufferSizeImpl<IterCfg, TpPack, Head, T, true, ItemConfs...> {
 public:
  /*!
   * \brief Returns the required buffer size to serialize a non-primitive type that is not an optional with provided
   * config as a Tlv field.
   * \param[in] t Data to be serialized.
   * \return Required buffer size for TLV serialization.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - Initialize a variable to store required buffer size to zero.
   * - Fetch the size of the length field and the required buffer size for the data.
   * - If the fetched size is greater than 0
   *   - add the sum of the fetched size and the size of the tlv tag to the stored size..
   * - Return the total size required.
   * \endinternal
   */
  static constexpr InfSizeT Get(T const& t) noexcept {
    constexpr std::size_t kTlvTagSize{2U};
    constexpr InfSizeT kLengthFieldSize{GetLengthFieldSize<Head>()};
    InfSizeT const kTlvValueSize{kLengthFieldSize + GetBufferSizeRoot<IterCfg, TpPack, LengthSize<0>, ItemConfs...>(t)};
    InfSizeT const result{kTlvTagSize + kTlvValueSize};
    return result;
  }
};

/*!
 * \brief Specialization for Optional with provided config as a Tlv field.
 * \tparam IterCfg Type of computation to be done by the recursion.
 * \tparam TpPack Transformation properties.
 * \tparam Head Either length size config or endianness.
 * \tparam ItemConfs Configuration of the members.
 * \tparam T Type of the member.
 */
template <typename IterCfg, typename TpPack, typename Head, typename T, typename... ItemConfs>
class GetTlvFieldBufferSizeImpl<IterCfg, TpPack, Head, ara::core::Optional<T>, true, ItemConfs...> {
 public:
  /*!
   * \brief Returns the required buffer size to serialize an Optional with provided config as a Tlv field.
   * \param[in] t Data to be serialized.
   * \return Required buffer size for TLV serialization.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - Initialize a variable to store required buffer size to zero.
   * - If optional has a value
   *   - add required buffer size (with tlv field) for the object to the stored size.
   * - Return the total size required.
   * \endinternal
   */
  static constexpr InfSizeT Get(ara::core::Optional<T> const& t) noexcept {
    InfSizeT result{0U};
    if (IterCfg::IsInstanceSizeIteration()) {
      if (t) {
        result = GetTlvFieldBufferSizeRoot<IterCfg, TpPack, Head, ItemConfs...>(*t);
      }
    } else {
      result = GetTlvFieldBufferSizeRoot<IterCfg, TpPack, Head, ItemConfs...>(T{});
    }
    return result;
  }
};

// ----------------------------GetTlvFieldBufferSize End ---------------------------------

/*!
 * \brief Returns the required buffer size to serialize a type with provided config for non-static size types.
 * \tparam IterCfg Type of computation to be done by the recursion.
 * \tparam TpPack Transformation properties.
 * \tparam Head Either length size config or endianness.
 * \tparam Confs Configuration of the members.
 * \tparam T Type of the member.
 * \param[in] t Data to be serialized.
 * \return Calculated buffer size for serialization.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 */
template <typename IterCfg, typename TpPack, typename Head, typename... Confs, typename T>
constexpr auto GetBufferSizeRoot(T const& t) noexcept
    -> std::enable_if_t<!IsStaticSize<TpPack, Head, Confs...>(SizeToken<T>{}), InfSizeT> {
  return detail::GetBufferSize<IterCfg, TpPack, Head, Confs...>(t);  // VCA_SOMEIPPROTOCOL_REQUIRED_BUFFER_SIZE
}

/*!
 * \brief Returns the required buffer size to serialize a type with provided config for static size types.
 * \tparam IterCfg Type of computation to be done by the recursion.
 * \tparam TpPack Transformation properties.
 * \tparam Head Either length size config or endianness.
 * \tparam Confs Configuration of the members.
 * \tparam T Type of the member.
 * \param[in] t Data to be serialized.
 * \return Calculated buffer size for serialization.
 * \pre -
 * \context ANY
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 */
template <typename IterCfg, typename TpPack, typename Head, typename... Confs, typename T>
constexpr auto GetBufferSizeRoot(T const& t) noexcept
    -> std::enable_if_t<IsStaticSize<TpPack, Head, Confs...>(SizeToken<T>{}), InfSizeT> {
  return detail::GetBufferSize<IterCfg, TpPack, Head, Confs...>(t);  // VCA_SOMEIPPROTOCOL_REQUIRED_BUFFER_SIZE
}

/*!
 * \brief Returns the required buffer size to serialize a type with provided config as a Tlv field.
 * \tparam IterCfg Type of computation to be done by the recursion.
 * \tparam TpPack Transformation properties.
 * \tparam Head Either length size config or endianness.
 * \tparam Confs Configuration of the members.
 * \tparam T Type of the member.
 * \param[in] t Data to be serialized.
 * \return Required buffer size for TLV serialization.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 */
template <typename IterCfg, typename TpPack, typename Head, typename... Confs, typename T>
constexpr InfSizeT GetTlvFieldBufferSizeRoot(T const& t) noexcept {
  bool constexpr is_complex_type{(!IsPrimitiveType<T>::value) && (!IsEnumType<T>::value)};
  return detail::GetTlvFieldBufferSizeImpl<IterCfg, TpPack, Head, T, is_complex_type, Confs...>::Get(t);
}

/*!
 * \brief Configures GetBufferSize/GetTlvFieldBufferSize recursions to check the size of the actual passed argument.
 */
class InstanceSizeIteration {
 public:
  /*!
   * \brief   Check if template is of class InstanceSizeIteration
   * \return  Always returns true, since this class is InstanceSizeIteration
   * \pre     -
   * \context Reactor|App
   */
  static constexpr bool IsInstanceSizeIteration() { return true; }
};

/*!
 * \brief Configures GetBufferSize/GetTlvFieldBufferSize recursions to check the maximum size of the type.
 */
class MaxSizeIteration {
 public:
  /*!
   * \brief   Check if template is of class InstanceSizeIteration
   * \return  Always returns false, since this class is not InstanceSizeIteration
   * \pre     -
   * \context Reactor|App
   */
  static constexpr bool IsInstanceSizeIteration() { return false; }
};

}  // namespace detail

/*!
 * \brief Returns the required buffer size to serialize a type with provided config
 * \tparam TpPack Transformation properties.
 * \tparam Head Either length size config or endianness.
 * \tparam Confs Configuration of the members.
 * \tparam T Type of the member.
 * \param[in] t Data to be serialized.
 * \return Calculated buffer size for serialization.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 */
template <typename TpPack, typename Head, typename... Confs, typename T>
constexpr std::size_t GetRequiredBufferSize(T const& t) noexcept {
  // clang-format off
  InfSizeT const v{detail::GetBufferSizeRoot<detail::InstanceSizeIteration, TpPack, Head, Confs...>(t)};  // VCA_SOMEIPPROTOCOL_REQUIRED_BUFFER_SIZE
  // clang-format on
  assert(!v.IsInfinity());
  return v.Value();
}

/*!
 * \brief Returns the required buffer size to serialize a type with provided config as a Tlv field.
 * \tparam TpPack Transformation properties.
 * \tparam Head Either length size config or endianness.
 * \tparam Confs Configuration of the members.
 * \tparam T Type of the member.
 * \param[in] t Data to be serialized.
 * \return Required buffer size for TLV serialization.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 */
template <typename TpPack, typename Head, typename... Confs, typename T>
constexpr std::size_t GetTlvFieldRequiredBufferSize(T const& t) noexcept {
  InfSizeT const v{detail::GetTlvFieldBufferSizeRoot<detail::InstanceSizeIteration, TpPack, Head, Confs...>(t)};
  assert(!v.IsInfinity());
  return v.Value();
}

/*!
 * \brief Returns the maximum buffer size to serialize a type with provided config (for argument-dependent lookup)
 * \tparam TpPack Transformation properties.
 * \tparam Head Either length size config or endianness.
 * \tparam Confs Configuration of the members.
 * \return Calculated buffer size for serialization if the model has maximum sizes for all elements.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 */
template <typename TpPack, typename Head, typename... Confs, typename T>
constexpr InfSizeT GetMaximumBufferSize(T const&) noexcept {
  return detail::GetBufferSizeRoot<detail::MaxSizeIteration, TpPack, Head, Confs...>(
      T{});  // VCA_SOMEIPPROTOCOL_REQUIRED_BUFFER_SIZE
}

/*!
 * \brief Returns the maximum buffer size to serialize a type with provided config
 * \tparam T Type of the member.
 * \tparam TpPack Transformation properties.
 * \tparam Head Either length size config or endianness.
 * \tparam Confs Configuration of the members.
 * \return Calculated buffer size for serialization if the model has maximum sizes for all elements.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 */
template <typename T, typename TpPack, typename Head, typename... Confs>
constexpr InfSizeT GetMaximumBufferSize() noexcept {
  return GetMaximumBufferSize<TpPack, Head, Confs...>(T{});
}

/*!
 * \brief       Returns the maximum buffer size to serialize a type with provided
                config as a Tlv field. (for argument-dependent lookup)
 * \tparam      T Type of the member.
 * \tparam      TpPack Transformation properties.
 * \tparam      Head Either length size config or endianness.
 * \tparam      Confs Configuration of the members.
 * \return      Required buffer size for TLV serialization if the model has maximum sizes for all elements.
 * \pre         -
 * \context     Reactor|App
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 */
template <typename TpPack, typename Head, typename... Confs, typename T>
constexpr InfSizeT GetTlvFieldMaximumBufferSize(T const&) noexcept {
  return detail::GetTlvFieldBufferSizeRoot<detail::MaxSizeIteration, TpPack, Head, Confs...>(T{});
}

/*!
 * \brief Returns the maximum buffer size to serialize a type with provided config as a Tlv field.
 * \tparam T Type of the member.
 * \tparam TpPack Transformation properties.
 * \tparam Head Either length size config or endianness.
 * \tparam Confs Configuration of the members.
 * \return Required buffer size for TLV serialization if the model has maximum sizes for all elements.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 */
template <typename T, typename TpPack, typename Head, typename... Confs>
constexpr InfSizeT GetTlvFieldMaximumBufferSize() noexcept {
  return GetTlvFieldMaximumBufferSize<TpPack, Head, Confs...>(T{});
}

}  // namespace serialization
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_SIZING_H_
