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
/**       \file     serializers/vac/variant.h
 *        \brief    Provides a serializer for the amsr::core::Variant type.
 *
 *        \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_SERIALIZERS_VAC_VARIANT_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_SERIALIZERS_VAC_VARIANT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/core/variant.h"

namespace amsr {
namespace json {
/*!
 * \brief A generic variant visitor.
 * \tparam Serializer Type of serializer. Must be noexcept move constructible and noexcept move assignable.
 *
 * \details Serializes the children according to their implemented serializers.
 *
 * \vprivate component private
 *
 * \trace DSGN-JSON-Writer-Serializable-Data-Structures
 */
template <typename Serializer>
class VariantVisitor {
  static_assert(std::is_nothrow_move_constructible<Serializer>::value, "Serializer must be noexcept movable");
  static_assert(std::is_nothrow_move_assignable<Serializer>::value, "Serializer must be noexcept movable");

 public:
  /*!
   * \brief Constructs the visitor.
   * \param serializer to use.
   *
   * \details Visitor must only be used once.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate component private
   *
   * \spec
   * requires true;
   * \endspec
   */
  explicit VariantVisitor(Serializer&& serializer) noexcept : serializer_(std::move(serializer)) {}

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Call operator.
   * \tparam T Type of the variant.
   * \param value to serialize.
   * \return The succeeding serializer.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate component private
   *
   * \spec
   * requires true;
   * \endspec
   */
  template <typename T>
  auto operator()(T const& value) noexcept -> typename Serializer::Next {
    return std::move(this->serializer_) << value;
  }

 private:
  /*!
   * \brief Serializer of the variant.
   */
  Serializer serializer_;
};

// VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
/*!
 * \brief Serializes a variant of serializable elements.
 * \tparam Serializer Type of serializer.
 * \tparam Types of values in the variant.
 * \param s Serializer instance to write into.
 * \param variant to serialize.
 * \return The succeeding serializer.
 *
 * \context ANY
 * \pre All alternatives contained in the variant are noexcept movable
 * \threadsafe FALSE
 * \reentrant FALSE
 * \vpublic
 *
 * \trace DSGN-JSON-Writer-Serializable-Data-Structures
 */
template <typename Serializer, typename... Types>
auto operator<<(Serializer s, amsr::core::Variant<Types...>& variant) noexcept -> typename Serializer::Next {
  VariantVisitor<Serializer> ser{std::move(s)};
  // VCA_VAJSON_EXTERNAL_CALL
  return amsr::core::visit(std::move(ser), variant);
}
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_WRITER_SERIALIZERS_VAC_VARIANT_H_
