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
/**        \file deser_variant.h
 *        \brief Contains static functions to deserialize variant / union.
 *        \unit SomeIpProtocol::Serdes::Deserialization
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_VARIANT_H_
#define LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_VARIANT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include <tuple>
#include <utility>
#include "ara/core/variant.h"
#include "deser_forward.h"
#include "someip-protocol/internal/deserialization/reader.h"
#include "someip-protocol/internal/deserialization/types.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace deserialization {

namespace detail {

template <typename TpPack, std::size_t ReverseIndex, typename... ItemConfs>
class DeserializeVariantTypeRecurse;

/*!
 * \brief Specialized template class to deserialize recursively a variant.
 * \tparam TpPack Transformation properties.
 * \tparam ReverseIndex Index to check if read type selector field value is within the bounds.
 * \tparam ItemConf Length field configuration and/or endianness of the sub-elements of the first variant.
 * \tparam ItemConfTail Length field configuration and/or endianness of the sub-elements of the remaining variants.
 */
template <typename TpPack, std::size_t ReverseIndex, typename... ItemConf, typename... ItemConfTail>
class DeserializeVariantTypeRecurse<TpPack, ReverseIndex, ConfPack<ItemConf...>, ItemConfTail...> {
 public:
  /*!
   * \brief The type of the selector field.
   */
  using SelectorType =
      typename UintRead<Tp<TpPack>::kSizeOfUnionTypeSelectorField, typename Tp<TpPack>::ByteOrder>::type;
  // Type erasure and a vtable could be used to make the lookup a constant time
  // operation
  /*!
   * \brief Check if the read type index is within the number of configured variants and deserializes the variant if
   *        so.
   * \details This function calls it itself with the next variant in case the current variant is not the one to
   *          deserialized.
   * \tparam Ts Types of the underlying variants.
   * \param[out] v Variable the variant shall be deserialized into.
   * \param[in,out] r Buffer Reader holding the view on the serialized data.
   * \param[in] type_index Index of the variant to be used.
   * \return true if deserialization was successful, false otherwise.
   * \pre -
   * \context Reactor|App
   * \reentrant
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - If the current index matches the index read from the type selector field
   *   - If the buffer contains at least the number of bytes matching the type of the variant
   *     - Deserialize the variant
   * - else call Deserialize for (ReverseIndex - 1)
   * \endinternal
   */
  template <typename... Ts>
  static Result Deserialize(ara::core::Variant<Ts...>& v, Reader& r, SelectorType const type_index) noexcept {
    static constexpr std::size_t kActualIndex{sizeof...(Ts) - ReverseIndex - 1};
    Result result{false};
    if (type_index == kActualIndex) {
      using T = std::tuple_element_t<kActualIndex, std::tuple<Ts...>>;
      T t{};

      constexpr std::size_t static_size{SomeIpProtocolGetStaticSize<TpPack, ItemConf...>(SizeToken<T>{})};
      result = r.VerifySize(static_size);

      if (result == true) {
        result = SomeIpProtocolDeserialize<TpPack, ItemConf...>(r, t);
      }

      if (result) {
        // clang-format off
        v.template emplace<kActualIndex>(std::move(t));  // VCA_SOMEIPPROTOCOL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
        // clang-format on
      }
    } else {  // VCA_SOMEIPPROTOCOL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
              // clang-format off
      // VECTOR NL AutosarC++17_10-M5.19.1:MD_SOMEIPPROTOCOL_AutosarC++17_10-M5.19.1_OverflowOrUnderflowOfConstExpr
      result = DeserializeVariantTypeRecurse<TpPack, ReverseIndex - 1, ItemConfTail...>::Deserialize(v, r, type_index);  // VCA_SOMEIPPROTOCOL_WITHIN_SPEC
      //clang-format on
    }
    return result;
  }
};

/*!
 * \brief Recursion tail, if the read type selector field value does not match the configured variants.
 * \tparam TpPack Transformation properties.
 */
template <typename TpPack>
class DeserializeVariantTypeRecurse<TpPack, std::numeric_limits<std::size_t>::max()> {
 public:
  /*!
   * \brief The type of the selector field.
   */
  using SelectorType =
      typename UintRead<Tp<TpPack>::kSizeOfUnionTypeSelectorField, typename Tp<TpPack>::ByteOrder>::type;
  /*!
   * \brief Recursion tail if the read type selector field index is not within the configured variants range.
   * \tparam Ts Types of the underlying variants.
   * \param[out] v Variable the variant shall be deserialized into.
   * \param[in,out] r Buffer Reader holding the view on the serialized data.
   * \param[in] type_index Index of the variant to be used.
   * \return true if deserialization was successful, false otherwise.
   * \pre -
   * \context Reactor|App
   * \reentrant TRUE.
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename... Ts>
  static Result Deserialize(ara::core::Variant<Ts...> const& v, Reader const& r,
                            SelectorType const type_index) noexcept {
    static_cast<void>(v);
    static_cast<void>(r);
    static_cast<void>(type_index);
    // Recursion tail. No more action required.
    return false;
  }
};

/*!
 * \brief Deserialize a specific variant of the union according to the given type selector field.
 * \tparam TpPack Transformation properties.
 * \tparam ItemConf Length field configuration and/or endianness of the sub-elements of the variants.
 * \tparam Ts Types of the underlying variants.
 * \param[out] v Variable the variant shall be deserialized into.
 * \param[in,out] r Buffer Reader holding the view on the serialized data.
 * \param[in] type_index Variant to be used.
 * \return true if deserialization was successful, false otherwise.
 * \pre -
 * \context Reactor|App
 * \reentrant TRUE for different reader objects.
 * \spec
 *   requires true;
 * \endspec
 */
template <typename TpPack, typename... ItemConf, typename... Ts>
Result DeserializeVariantType(ara::core::Variant<Ts...>& v, Reader& r,
                              typename UintRead<Tp<TpPack>::kSizeOfUnionTypeSelectorField,
                                                typename Tp<TpPack>::ByteOrder>::type const type_index) noexcept {
  return DeserializeVariantTypeRecurse<TpPack, sizeof...(Ts) - 1, ItemConf...>::Deserialize(v, r, type_index);
}

}  // namespace detail

/*!
 * \brief Deserialize SOME/IP union.
 * \details Variant is deserialized according to the read type selector field.
 * \tparam TpPack Transformation properties.
 * \tparam ItemConf Length field configuration and/or endianness of the sub-elements of the variants.
 * \tparam Ts Type of the underlying variants.
 * \param[out] v - Variable the variant shall be deserialized into.
 * \param[in,out] r - Buffer Reader holding the view on the serialized data.
 * \return true if deserialization was successful, false otherwise.
 * \pre -
 * \context Reactor|App
 * \reentrant TRUE for different reader objects.
 * \spec
 *   requires true;
 * \endspec
 *
 * \internal
 * - If serialized buffer contains at least size of type selector field
 *   - Read type selector field
 *   - Deserialize variant according to the selector field value
 * \endinternal
 */
template <typename TpPack, typename... ItemConf, typename... Ts>
Result DeserializeVariant(ara::core::Variant<Ts...>& v, Reader& r) noexcept {
  using Endian_DeVariant = typename Tp<TpPack>::ByteOrder;
  /*!
   * \brief The type of the selector field.
   */
  using SelectorType = typename UintRead<Tp<TpPack>::kSizeOfUnionTypeSelectorField, Endian_DeVariant>::type;

  // Check if the type selector can be consumed.
  Result result{r.VerifySize(Tp<TpPack>::kSizeOfUnionTypeSelectorField)};
  if (result) {
    SelectorType opt_index{};
    r.ReadUintOfSize<Tp<TpPack>::kSizeOfUnionTypeSelectorField, Endian_DeVariant>(opt_index);
    if (opt_index > 0) {
      result = detail::DeserializeVariantType<TpPack, ItemConf...>(v, r, static_cast<SelectorType>(opt_index - 1));
    }
  }
  return result;
}

}  // namespace deserialization
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr
#endif  // LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_DESERIALIZATION_DESER_VARIANT_H_
