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
/*!        \file  ser_sizing_variant.h
 *        \brief  Defines classes for recursive validation and performs computation of buffer sizes for variant type
 *                serialization.
 *        \unit   SomeIpProtocol::Serdes::Serialization
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_SIZING_VARIANT_H_
#define LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_SIZING_VARIANT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include <tuple>
#include <utility>
#include "someip-protocol/internal/serialization/infinity_size_t.h"
#include "someip-protocol/internal/serialization/types.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace serialization {
namespace detail {

/*!
 * \brief Recursive validation.
 * \tparam IterCfg Type of computation to be done by the recursion.
 * \tparam TpPack Transformation properties.
 * \tparam ReverseIndex Reversed Index.
 * \tparam ItemConf Length field configuration and/or endianness of the sub-elements of the variants.
 */
template <typename IterCfg, typename TpPack, std::size_t ReverseIndex, typename... ItemConfs>
class GetBufferSizeVariantTypeRecurse;

/*!
 * \brief  Recursive validation.
 * \tparam IterCfg Type of computation to be done by the recursion.
 * \tparam TpPack Transformation properties.
 * \tparam ReverseIndex Reversed Index.
 * \tparam ItemConf Configuration parameters required to serialize the currently checked type.
 * \tparam ItemConfTail Configuration parameter packs for the remaining types.
 */
template <typename IterCfg, typename TpPack, std::size_t ReverseIndex, typename... ItemConf, typename... ItemConfTail>
class GetBufferSizeVariantTypeRecurse<IterCfg, TpPack, ReverseIndex, ConfPack<ItemConf...>, ItemConfTail...> {
 public:
  /*!
   * \brief Recursive validation.
   * \details Type erasure and index could be used to make the lookup a constant time operation.
   * \tparam Ts Types that the variant can hold.
   * \param[in] v Variant to serialize.
   * \param[in] typeIndex Index of the variant type to serialize.
   * \return Buffer size required for serialization.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - Get index of current recursion.
   * - If current recursion index is equal to the index of the variant to serialize
   *   - fetch the required buffer size of the data in the variant and store it.
   * - Otherwise, recursively call GetBufferSizeVariantTypeRecurse with the next reversed index.
   * - Return the stored size.
   * \endinternal
   */
  template <typename... Ts>
  static constexpr InfSizeT Get(ara::core::Variant<Ts...> const& v, std::uint16_t const typeIndex) noexcept {
    InfSizeT res{0U};
    constexpr std::size_t ActualIndex{sizeof...(Ts) - ReverseIndex - 1};
    if (IterCfg::IsInstanceSizeIteration()) {
      if (typeIndex == ActualIndex) {
        // clang-format off
        res = InfSizeT{GetBufferSizeRoot<IterCfg, TpPack, ItemConf...>(ara::core::get<ActualIndex>(v))}; // VCA_SOMEIPPROTOCOL_REQUIRED_BUFFER_SIZE
    } else {
        // VECTOR NL AutosarC++17_10-M5.19.1:MD_SOMEIPPROTOCOL_AutosarC++17_10-M5.19.1_OverflowOrUnderflowOfConstExpr
        res = GetBufferSizeVariantTypeRecurse<IterCfg, TpPack, ReverseIndex - 1, ItemConfTail...>::Get(v, typeIndex); // VCA_SOMEIPPROTOCOL_REQUIRED_BUFFER_SIZE
        // clang-format on
      }
    } else {
      // clang-format off
      InfSizeT const value{GetBufferSizeRoot<IterCfg, TpPack, ItemConf...>(decltype(ara::core::get<ActualIndex>(v)){})};  // VCA_SOMEIPPROTOCOL_REQUIRED_BUFFER_SIZE
      // VECTOR NL AutosarC++17_10-M5.19.1:MD_SOMEIPPROTOCOL_AutosarC++17_10-M5.19.1_OverflowOrUnderflowOfConstExpr
      res = Max(value, GetBufferSizeVariantTypeRecurse<IterCfg, TpPack, ReverseIndex - 1, ItemConfTail...>::Get(v, 0u)); // VCA_SOMEIPPROTOCOL_REQUIRED_BUFFER_SIZE
      // clang-format on
    }
    return res;
  }
};

/*!
 * \brief Recursion tail.
 * \tparam IterCfg Type of computation to be done by the recursion.
 * \tparam TpPack Transformation properties.
 */
template <typename IterCfg, typename TpPack>
class GetBufferSizeVariantTypeRecurse<IterCfg, TpPack, std::numeric_limits<std::size_t>::max()> {
 public:
  /*!
   * \brief Recursion tail.
   * \tparam Ts Types that the variant can hold.
   * \return Always return zero.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  template <typename... Ts>
  static constexpr InfSizeT Get(ara::core::Variant<Ts...> const&, std::uint16_t const) noexcept {
    return InfSizeT{0U};
  }
};

/*!
 * \brief Validation for Variant.
 * \tparam IterCfg Type of computation to be done by the recursion.
 * \tparam TpPack Transformation properties.
 * \tparam ItemConf Configuration for the item of the variant
 * \tparam Ts Types that the variant can hold.
 * \param[in] v Variant to serialize.
 * \return Sum of the type selector size and the recursion tail size.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 *
 * \internal
 * - Return the sum of the size of the union type selector field and the required buffer size of the variant.
 * \endinternal
 */
template <typename IterCfg, typename TpPack, typename... ItemConf, typename... Ts>
constexpr InfSizeT GetBufferSizeVariantImpl(ara::core::Variant<Ts...> const& v) noexcept {
  return Tp<TpPack>::kSizeOfUnionTypeSelectorField +
         // clang-format off
         GetBufferSizeVariantTypeRecurse<IterCfg, TpPack, sizeof...(Ts) - 1, ItemConf...>::Get( // VCA_SOMEIPPROTOCOL_REQUIRED_BUFFER_SIZE
             v, static_cast<std::uint16_t>(v.index())); // VCA_SOMEIPPROTOCOL_REQUIRED_BUFFER_SIZE
  // clang-format on
}

}  // namespace detail
}  // namespace serialization
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_SIZING_VARIANT_H_
