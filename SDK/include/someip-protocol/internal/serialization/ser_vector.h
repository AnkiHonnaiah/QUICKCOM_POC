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
/*!        \file ser_vector.h
 *        \brief Defines the serialization functions for serializing a vector.
 *        \unit SomeIpProtocol::Serdes::Serialization
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_VECTOR_H_
#define LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_VECTOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/vector.h"
#include "ara/log/logging.h"
#include "ser_forward.h"
#include "someip-protocol/internal/serialization/types.h"
#include "someip-protocol/internal/serialization/writer.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace serialization {

/*!
 * \brief Serializes a vector.
 * \details Specialization for when no max. size limitation (arraySize) configured for the vector.
 * \tparam TpPack Transformation properties.
 * \tparam ArraySizeConf Array size item configuration: max. number of allowed vector elements.
 * \tparam ItemConf Configuration parameters required to serialize an element.
 * \tparam T Item type.
 * \tparam Alloc Vector allocator.
 * \param[in] v Vector to serialize.
 * \param[in,out] w Writer holding the buffer to which data is serialized.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 *
 * \internal
 * - For each element in the vector
 *   - call SomeIpProtocolSerialize.
 * \endinternal
 */
template <typename TpPack, typename ArraySizeConf, typename... ItemConf, typename T, typename Alloc>
// VECTOR NL AutosarC++17_10-A2.11.4: MD_SOMEIPPROTOCOL_AutosarC++17_10-A2.11.4_NameOfObjectOrFunctionReused
static auto serializeVector(ara::core::Vector<T, Alloc> const& v, Writer& w) noexcept
    -> std::enable_if_t<(!TypeTraitIsVectorArraySizeLimitActive<ArraySizeConf>()) &&
                            (!TypeTraitIsOptimizableContainer<T, ItemConf...>()),
                        void> {
  for (T const& value : v) {
    SomeIpProtocolSerialize<TpPack, ItemConf...>(w, value);  // VCA_SOMEIPPROTOCOL_WRITER_REFERENCE
  }
}

/*!
 * \brief Serializes a vector.
 * \details Specialization for when no max. size limitation (arraySize) configured for the vector,
 *          and element types with the same endianness.
 * \tparam TpPack Transformation properties.
 * \tparam ArraySizeConf Array size item configuration: max. number of allowed vector elements.
 * \tparam ItemConf Configuration parameters required to serialize an element.
 * \tparam T Item type.
 * \tparam Alloc Vector allocator.
 * \param[in] v Vector to serialize.
 * \param[in,out] w Writer holding the buffer to which data is serialized.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 *
 * \internal
 * - Write vector to buffer.
 * \endinternal
 */
template <typename TpPack, typename ArraySizeConf, typename... ItemConf, typename T, typename Alloc>
static auto serializeVector(ara::core::Vector<T, Alloc> const& v, Writer& w) noexcept
    -> std::enable_if_t<(!TypeTraitIsVectorArraySizeLimitActive<ArraySizeConf>()) &&
                            TypeTraitIsOptimizableContainer<T, ItemConf...>(),
                        void> {
  w.writeVector(v, v.size());  // VCA_SOMEIPPROTOCOL_WRITE_REFERENCE
}

/*!
 * \brief Serializes a vector.
 * \details Specialization for when max. size limitation (arraySize) is configured for the vector.
 * \tparam TpPack Transformation properties.
 * \tparam ArraySizeConf Array size item configuration: max. number of allowed vector elements.
 * \tparam ItemConf Configuration parameters required to serialize an element.
 * \tparam T Item type.
 * \tparam Alloc Vector allocator.
 * \param[in] v Vector to serialize.
 * \param[in,out] w Writer holding the buffer to which data is serialized.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 *
 * \internal
 * - Set the number of elements to serialize to the number of elements in the vector.
 * - Fetch maximum number of elements from configuration.
 * - If number of elements in vector is greater than the configured maximum amount of elements
 *   - calculate number of unexpected elements.
 *   - log a warning regarding skipped elements.
 *   - set number of elements to serialize to the configured maximum number of elements.
 * - Loop over the number of elements to serialize in the vector
 *   - call SomeIpProtocolSerialize with the current element.
 * \endinternal
 */
// VECTOR AV NC Architecture-ApiAnalysis Local_Var_Of_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
template <typename TpPack, typename ArraySizeConf, typename... ItemConf, typename T, typename Alloc>
static auto serializeVector(ara::core::Vector<T, Alloc> const& v, Writer& w) noexcept
    -> std::enable_if_t<TypeTraitIsVectorArraySizeLimitActive<ArraySizeConf>() &&
                            (!TypeTraitIsOptimizableContainer<T, ItemConf...>()),
                        void> {
  std::size_t nr_elements_to_serialize{v.size()};
  std::size_t const max_nr_elements{ArraySizeConf::value};

  if (nr_elements_to_serialize > max_nr_elements) {
    std::size_t const unexpected_elements{nr_elements_to_serialize - max_nr_elements};
    using vac::container::literals::operator""_sv;
    constexpr amsr::core::StringView kSomeIpLoggerContextId{"vcso"_sv};
    constexpr amsr::core::StringView kSomeIpLoggerContextDescription{"Logger for SOME/IP libraries"_sv};
    ara::log::Logger& logger{ara::log::CreateLogger(  // VCA_SOMEIPPROTOCOL_UNDEFINED_FUNCTION
        core::StringView(kSomeIpLoggerContextId), core::StringView(kSomeIpLoggerContextDescription))};
    logger.LogWarn(
        [unexpected_elements](ara::log::LogStream& s) {
          s << "Skipping serialization of " << unexpected_elements
            << " unexpected array elements. Maximum expected number of array elements: " << max_nr_elements;
        },
        static_cast<char const*>(__func__), __LINE__);

    nr_elements_to_serialize = max_nr_elements;
  }

  for (std::size_t element_index{0U}; element_index < nr_elements_to_serialize; ++element_index) {
    // clang-format off
    SomeIpProtocolSerialize<TpPack, ItemConf...>(w, v[element_index]);  // VCA_SOMEIPPROTOCOL_WRITER_REFERENCE
    // clang-format on
  }
}

/*!
 * \brief Serializes a vector.
 * \details Specialization for when max. size limitation (arraySize) is configured for the vector,
 *          and element types with the same endianness.
 * \tparam TpPack Transformation properties.
 * \tparam ArraySizeConf Array size item configuration: max. number of allowed vector elements.
 * \tparam ItemConf Configuration parameters required to serialize an element.
 * \tparam T Item type.
 * \tparam Alloc Vector allocator.
 * \param[in] v Vector to serialize.
 * \param[in,out] w Writer holding the buffer to which data is serialized.
 * \pre -
 * \context Reactor|App
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 *
 * \internal
 * - Set the number of elements to serialize to the number of elements in the vector.
 * - Fetch maximum number of elements from configuration.
 * - If number of elements in vector is greater than the configured maximum amount of elements
 *   - calculate number of unexpected elements.
 *   - log a warning regarding skipped elements.
 *   - set number of elements to serialize to the configured maximum number of elements.
 * - Write the set number of elements of the vector to the buffer.
 * \endinternal
 */
// VECTOR AV NC Architecture-ApiAnalysis Local_Var_Of_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
template <typename TpPack, typename ArraySizeConf, typename... ItemConf, typename T, typename Alloc>
static auto serializeVector(ara::core::Vector<T, Alloc> const& v, Writer& w) noexcept
    -> std::enable_if_t<(TypeTraitIsVectorArraySizeLimitActive<ArraySizeConf>() &&
                         TypeTraitIsOptimizableContainer<T, ItemConf...>()),
                        void> {
  // VECTOR AV NC Architecture-ApiAnalysis Local_Var_Of_Type: MD_SOMEIPPROTOCOL_ArchCheck_size_t_Divergence
  std::size_t nr_elements_to_serialize{v.size()};
  std::size_t const max_nr_elements{ArraySizeConf::value};

  if (nr_elements_to_serialize > max_nr_elements) {
    std::size_t const unexpected_elements{nr_elements_to_serialize - max_nr_elements};
    using vac::container::literals::operator""_sv;
    constexpr amsr::core::StringView kSomeIpLoggerContextId{"vcso"_sv};
    constexpr amsr::core::StringView kSomeIpLoggerContextDescription{"Logger for SOME/IP libraries"_sv};
    ara::log::Logger& logger{ara::log::CreateLogger(  // VCA_SOMEIPPROTOCOL_UNDEFINED_FUNCTION
        core::StringView(kSomeIpLoggerContextId), core::StringView(kSomeIpLoggerContextDescription))};
    logger.LogWarn(
        [unexpected_elements](ara::log::LogStream& s) {
          s << "Skipping serialization of " << unexpected_elements
            << " unexpected array elements. Maximum expected number of array elements: " << max_nr_elements;
        },
        static_cast<char const*>(__func__), __LINE__);
    nr_elements_to_serialize = max_nr_elements;
  }
  w.writeVector(v, nr_elements_to_serialize);  // VCA_SOMEIPPROTOCOL_WRITE_REFERENCE
}

}  // namespace serialization
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_SERIALIZATION_SER_VECTOR_H_
