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
/*!        \file ser_variant.h
 *        \brief Defines the serialization function for the type index and value of a variant.
 *        \unit SomeIpProtocol::Serdes::Serialization
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_VARIANT_H_
#define LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_VARIANT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include <string>
#include <tuple>
#include "ser_forward.h"
#include "someip-protocol/internal/logging/someip_protocol_log_builder.h"
#include "someip-protocol/internal/serialization/types.h"
#include "someip-protocol/internal/serialization/writer.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace serialization {
namespace detail {
/*!
 * \brief Base template for serializing variant.
 * \tparam TpPack Transformation properties.
 * \tparam Index Recursion Index.
 * \tparam MaxIndex End of iteration Index.
 * \tparam ItemConf Length field configuration and/or endianness of the sub-elements of the variants.
 */
template <typename TpPack, std::size_t Index, std::size_t MaxIndex, typename... ItemConfs>
class SerializeVariantTypeRecurse;

/*!
 * \brief Recursive serialization of a variant by looking at one type at a time.
 * \tparam TpPack Transformation properties.
 * \tparam Index Recursion Index.
 * \tparam MaxIndex End of iteration Index.
 * \tparam ItemConf Configuration parameters required to serialize the currently checked type.
 * \tparam ItemConfTail Configuration parameter packs for the remaining types.
 */
template <typename TpPack, std::size_t Index, std::size_t MaxIndex, typename... ItemConf, typename... ItemConfTail>
class SerializeVariantTypeRecurse<TpPack, Index, MaxIndex, ConfPack<ItemConf...>, ItemConfTail...> {
 public:
  /*!
   * \brief Serializes a variant by looking at one type at a time.
   * \tparam Ts Types that the variant can hold.
   * \param[in] v Variant to serialize.
   * \param[in,out] w Writer holding the buffer to which data is serialized.
   * \param[in] typeIndex Index of the variant type to serialize, i.e. variant.index() + 1.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \internal
   * - Get index of current recursion.
   * - If current recursion index is equal to the index of the variant to serialize
   *   - Call SomeIpProtocolSerialize with the data in the variant.
   * - Otherwise, recursively call SerializeVariantTypeRecurse with the next reversed index.
   * \endinternal
   */
  template <typename... Ts>
  static void serialize(ara::core::Variant<Ts...> const& v, Writer& w, std::uint16_t const typeIndex) noexcept {
    if (typeIndex == Index) {
      // clang-format off
      SomeIpProtocolSerialize<TpPack, ItemConf...>(w, ara::core::get<Index>(v)); // VCA_SOMEIPPROTOCOL_WRITER_REFERENCE
    } else {
      // VECTOR NL AutosarC++17_10-M5.19.1:MD_SOMEIPPROTOCOL_AutosarC++17_10-M5.19.1_OverflowOrUnderflowOfConstExpr
      SerializeVariantTypeRecurse<TpPack, Index + 1, MaxIndex, ItemConfTail...>::serialize(v, w, typeIndex);  // VCA_SOMEIPPROTOCOL_SER_VARIANT_REFERENCE
      // clang-format on
    }
  }
};

/*!
 * \brief Template specialization when Index is invalid.
 * \tparam TpPack Transformation properties.
 * \tparam MaxIndex End of iteration Index.
 */
template <typename TpPack, std::size_t MaxIndex>
class SerializeVariantTypeRecurse<TpPack, MaxIndex, MaxIndex> {
 public:
  /*!
   * \brief Recursion tail, if the type selector field value held by the Variant does not match any defined
   * alternatives. It will always log fatal and abort.
   * \tparam Ts Types that the variant can hold.
   * \param[in] v Variant to serialize.
   * \param[in] w Writer holding the buffer to which data is serialized.
   * \param[in] typeIndex Index of the variant type to serialize.
   * \pre -
   * \context Reactor|App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  // clang-format off
  template <typename... Ts>
  static void serialize(ara::core::Variant<Ts...> const& v, Writer const& w, std::uint16_t const typeIndex) noexcept { // COV_SOMEIPPROTOCOL_VARIANT
    static_cast<void>(v);
    static_cast<void>(w);
    static_cast<void>(typeIndex);
    // clang-format off
    logging::SomeipProtocolLogBuilder::LogFatalAndAbort(ara::core::StringView{"Violation: type selector field value held by the Variant does not match any defined alternative."}, // VCA_SOMEIPPROTOCOL_VALID_POINTER_PARAMETERS
                                                        ara::core::StringView{AMSR_FILE_LINE}); // COV_SOMEIPPROTOCOL_VARIANT
  } // VCA_SOMEIPPROTOCOL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
  // clang-format on
};

/*!
 * \brief Serialize a specific variant of the union according to the given type selector field.
 * \tparam TpPack Transformation properties.
 * \tparam ItemConf Configuration parameters required to serialize the currently checked type.
 * \tparam Ts Types that the variant can hold.
 * \param[in] v Variant to serialize.
 * \param[in,out] w Writer holding the buffer to which data is serialized.
 * \param[in] typeIndex Index of the variant type to serialize.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 */
template <typename TpPack, typename... ItemConf, typename... Ts>
void serializeVariantType(ara::core::Variant<Ts...> const& v, Writer& w, std::uint16_t const typeIndex) noexcept {
  // clang-format off
  SerializeVariantTypeRecurse<TpPack, 0, sizeof...(Ts), ItemConf...>::serialize(v, w, typeIndex); // VCA_SOMEIPPROTOCOL_SER_VARIANT_REFERENCE
  // clang-format on
}
}  // namespace detail
/*!
 * \brief Serialize SOME/IP union.
 * \tparam TpPack Transformation properties.
 * \tparam ItemConf Configuration parameters required to serialize the currently checked type.
 * \tparam Ts Types that the variant can hold.
 * \param[in] v Variant to serialize.
 * \param[in,out] w Writer holding the buffer to which data is serialized.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 *
 * \internal
 * - Serialize type selector field.
 * - Serialize variant.
 * \endinternal
 */
template <typename TpPack, typename... ItemConf, typename... Ts>
void serializeVariant(ara::core::Variant<Ts...> const& v, Writer& w) noexcept {
  /*!
   * \brief The endianness of the type selector field value.
   */
  using Endian = typename Tp<TpPack>::ByteOrder;
  /*!
   * \brief The type of the type selector field.
   */
  using TypeSelectorFieldType = typename UintWrite<Tp<TpPack>::kSizeOfUnionTypeSelectorField, Endian>::type;

  TypeSelectorFieldType const type_selector_val{static_cast<TypeSelectorFieldType>(v.index() + 1U)};
  w.writeUintOfSize<Tp<TpPack>::kSizeOfUnionTypeSelectorField, Endian>(type_selector_val);
  // clang-format off
  detail::serializeVariantType<TpPack, ItemConf...>(v, w, static_cast<std::uint16_t>(v.index())); // VCA_SOMEIPPROTOCOL_SER_VARIANT_REFERENCE
  // clang-format on
}

}  // namespace serialization
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr
#endif  // LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_VARIANT_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_SOMEIPPROTOCOL_VARIANT
//   \ACCEPT  XX
//   \REASON  The justified function is a check to detect misconfigured variant indecies.
//            The function does not provide ASIL relevant functionality.
// COV_JUSTIFICATION_END
