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
/**        \file deser_vector.h
 *        \brief Contains static functions to deserialize dynamic length containers (vector).
 *        \unit SomeIpProtocol::Serdes::Deserialization
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_VECTOR_H_
#define LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_VECTOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <type_traits>
#include "ara/core/vector.h"
#include "deser_forward.h"
#include "someip-protocol/internal/deserialization/reader.h"
#include "someip-protocol/internal/deserialization/types.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace deserialization {

namespace detail {
/*!
 * \brief   Type-trait condition for maximum container size configuration.
 * \details arraySize ARXML option set to any value > 0.
 * \tparam  ArraySizeConf Array size item configuration: Maximum number of allowed vector elements.
 * \return  True if maximum container size limitation is activated. False otherwise.
 * \pre     -
 * \context Reactor|App
 * \spec
 *   requires true;
 * \endspec
 */
template <typename ArraySizeConf>
static constexpr bool TypeTraitIsVectorArraySizeLimitActive() noexcept(true) {
  return !std::is_same<ArraySizeConf, ArraySizeInactive>::value;
}

}  // namespace detail

/*!
 * \brief         Calls the deserialization function for a generic vector.
 * \details       Template specialization for the following case:
 *                - Non-optimized deserialization for the vector datatype is applied
 *                AND
 *                - Max size limitation (arraySize) is not configured for the vector.
 * \tparam        TpPack Transformation properties.
 * \tparam        T Value type of vector.
 * \tparam        ArraySizeConf Array size item configuration: max. number of allowed vector elements.
 * \tparam        ItemConf Item configuration for the vector elements holding (length field size and endianness).
 * \tparam        Alloc Vector allocator.
 * \param[in,out] r Reader holding the view on the serialized data.
 * \param[out]    v Vector, the data shall be deserialized into.
 * \return        True if deserialization was successful. False otherwise.
 * \pre           -
 * \context       Reactor|App
 * \spec
 *   requires true;
 * \endspec
 */
template <typename TpPack, typename T, typename ArraySizeConf, typename... ItemConf, typename Alloc>
static auto DeserializeVector(ara::core::Vector<T, Alloc>& v, Reader& r) noexcept
    -> std::enable_if_t<(!TypeTraitIsOptimizableContainer<T, ItemConf...>()) &&
                            (!detail::TypeTraitIsVectorArraySizeLimitActive<ArraySizeConf>()),
                        Result> {
  Result result{true};
  constexpr std::size_t static_size{SomeIpProtocolGetStaticSize<TpPack, ItemConf...>(SizeToken<T>{})};
  while (r.VerifySize(static_size)) {
    v.emplace_back();  // VCA_SOMEIPPROTOCOL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
    // clang-format off
    result = SomeIpProtocolDeserialize<TpPack, ItemConf...>(r, v.back());  // VCA_SOMEIPPROTOCOL_READER_REFERENCE, VCA_SOMEIPPROTOCOL_DESER_VECTOR
    // clang-format on
    if (!result) {
      break;
    }
  }

  // Check that all the elements are deserialized, if not then we must return false.
  result = result && (r.Size() == 0U);

  return result;
}

/*!
 * \brief         Calls the deserialization function for a generic vector.
 * \details       Template specialization for the following case:
 *                - non-optimized deserialization for the vector datatype is applied
 *                AND
 *                - Maximum size limitation (arraySize) is configured for the vector.
 * \tparam        TpPack Transformation properties.
 * \tparam        T Value type of vector.
 * \tparam        ArraySizeConf Array size item configuration: max. number of allowed vector elements.
 * \tparam        ItemConf Item configuration for the vector elements holding (length field size and endianness info).
 * \tparam        Alloc Vector allocator.
 * \param[in,out] r Reader holding the view on the serialized data.
 * \param[out]    v Vector, the data shall be deserialized into.
 * \return        true if deserialization was successful, false otherwise.
 * \pre           -
 * \context       Reactor|App
 * \spec
 *   requires true;
 * \endspec
 */
// VECTOR NC AutosarC++17_10-A15.4.2: MD_SOMEIPPROTOCOL_A15.4.2_noexceptUnsupportedInAraCoreFunction
// VECTOR NC AutosarC++17_10-A15.5.3: MD_SOMEIPPROTOCOL_A15.5.3_noexceptUnsupportedInAraCoreFunction
template <typename TpPack, typename T, typename ArraySizeConf, typename... ItemConf, typename Alloc>
static auto DeserializeVector(ara::core::Vector<T, Alloc>& v, Reader& r) noexcept
    -> std::enable_if_t<(!TypeTraitIsOptimizableContainer<T, ItemConf...>()) &&
                            detail::TypeTraitIsVectorArraySizeLimitActive<ArraySizeConf>(),
                        Result> {
  Result result{true};
  constexpr std::size_t static_size{SomeIpProtocolGetStaticSize<TpPack, ItemConf...>(SizeToken<T>{})};
  Result static_size_verfication_result{r.VerifySize(static_size)};
  v.reserve(ArraySizeConf::value);  // VCA_SOMEIPPROTOCOL_VECTOR_RESERVE
  while ((v.size() < ArraySizeConf::value) && static_size_verfication_result) {
    v.emplace_back();  // VCA_SOMEIPPROTOCOL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
    // clang-format off
    result = SomeIpProtocolDeserialize<TpPack, ItemConf...>(r, v.back()); // VCA_SOMEIPPROTOCOL_READER_REFERENCE
    // clang-format on
    // VECTOR NL AutosarC++17_10-M0.1.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.1.2_dead_branch
    if (!result) {
      break;
    }
    static_size_verfication_result = r.VerifySize(static_size);
  }

  // Skip unexpected array elements
  if (r.Size() > 0U) {
    // In case more bytes are remaining and we deserialized less than max. possible elements at least verify that
    // the static size of the unexpected elements is contained.
    // In case of dynamic value types no full verification of the unexpected elements is done.
    if (v.size() <= ArraySizeConf::value) {
      // VECTOR NL AutosarC++17_10-M0.1.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.1.2_dead_branch
      result = result && static_size_verfication_result;
    }
    // VECTOR NL AutosarC++17_10-M0.1.2: MD_SOMEIPPROTOCOL_AutosarC++17_10-M0.1.2_dead_branch
    if (result == true) {
      result = r.Skip(r.Size());
    }
  }

  return result;
}

/*!
 * \brief         Calls the deserialization function for a arithmetic vector without requiring endianness conversion.
 * \details       Template specialization for the following case:
 *                - optimized deserialization for the vector datatype is applied
 *                AND
 *                - Maximum size limitation (arraySize) is not configured for the vector.
 * \tparam        TpPack Transformation properties.
 * \tparam        T Type of vector.
 * \tparam        ArraySizeConf Array size item configuration: max. number of allowed vector elements.
 * \tparam        ItemConf Item configuration for the vector elements holding (length field size and endianness).
 * \tparam        Alloc Vector allocator.
 * \param[in,out] r Reader holding the view on the serialized data.
 * \param[out]    v Vector, the data shall be deserialized into.
 * \return        true if deserialization was successful, false otherwise.
 * \pre           -
 * \context       Reactor|App
 * \spec
 *   requires true;
 * \endspec
 */
template <typename TpPack, typename T, typename ArraySizeConf, typename... ItemConf, typename Alloc>
static auto DeserializeVector(ara::core::Vector<T, Alloc>& v, Reader& r) noexcept
    -> std::enable_if_t<TypeTraitIsOptimizableContainer<T, ItemConf...>() &&
                            (!detail::TypeTraitIsVectorArraySizeLimitActive<ArraySizeConf>()),
                        Result> {
  std::size_t const item_size{sizeof(T)};
  std::size_t const item_count{r.Size() / item_size};
  std::size_t const static_size{item_size * item_count};
  Result const result{r.VerifySize(static_size)};

  if (result) {
    r.ReadVector(v);
  }

  // Check that all the elements are deserialized, if not then we must return false.
  return result && (r.Size() == 0U);
}

/*!
 * \brief         Calls the deserialization function for a arithmetic vector without requiring
 *                endianness conversion.
 * \details       Template specialization for the following case:
 *                - optimized deserialization for the vector datatype is applied
 *                AND
 *                - maximum size limitation (arraySize) is configured for the vector.
 * \tparam        TpPack Transformation properties.
 * \tparam        T Type of vector.
 * \tparam        ArraySizeConf Array size item configuration: max. number of allowed vector elements.
 * \tparam        ItemConf Item configuration for the vector elements holding (length field size and endianness).
 * \tparam        Alloc Vector allocator.
 * \param[in,out] r Reader holding the view on the serialized data.
 * \param[out]    v Vector, the data shall be deserialized into.
 * \return        true if deserialization was successful, false otherwise.
 * \pre           -
 * \context       Reactor|App
 * \reentrant TRUE for different reader objects.
 * \spec
 *   requires true;
 * \endspec
 *
 * \internal
 * - Calculate the number of elements that could possibly fit into the byte stream
 * - Consume the number of bytes corresponding to the number of elements
 * - Deserialize the vector
 * - Return false if any error occurs or there are still bytes present in the reader
 * \endinternal
 */
// VECTOR NC AutosarC++17_10-A15.4.2: MD_SOMEIPPROTOCOL_A15.4.2_noexceptUnsupportedInAraCoreFunction
// VECTOR NC AutosarC++17_10-A15.5.3: MD_SOMEIPPROTOCOL_A15.5.3_noexceptUnsupportedInAraCoreFunction
template <typename TpPack, typename T, typename ArraySizeConf, typename... ItemConf, typename Alloc>
static auto DeserializeVector(ara::core::Vector<T, Alloc>& v, Reader& r) noexcept
    -> std::enable_if_t<(TypeTraitIsOptimizableContainer<T, ItemConf...>() &&
                         detail::TypeTraitIsVectorArraySizeLimitActive<ArraySizeConf>()),
                        Result> {
  std::size_t const item_size{sizeof(T)};
  std::size_t const item_count{r.Size() / item_size};
  std::size_t const static_size{item_size * item_count};
  Result const result{r.VerifySize(static_size)};

  // In normal use-case this check should never fail.
  if (result) {
    v.reserve(ArraySizeConf::value);  // VCA_SOMEIPPROTOCOL_VECTOR_RESERVE
    r.ReadVector<ArraySizeConf>(v);
  }

  // Check that all the elements are deserialized, if not then we must return false.
  return result && (r.Size() == 0U);
}

}  // namespace deserialization
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_VECTOR_H_
