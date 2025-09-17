/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!       \file   amsr/asn1/builder/transition.h
 *        \brief  Structure for transition table entry.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_TRANSITION_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_TRANSITION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/asn1/asn1_parser.h"

namespace amsr {
namespace asn1 {
namespace builder {

/*!
 * \brief Flag for element presence.
 */
enum class ElementPresence : std::uint8_t {
  kOptional, /*! Element may be present */
  kPresent,  /*! Element should be present */
  kAbsent    /*! Element should not be present */
};

/*!
 * \brief Element identifier.
 */
struct ElementIdentifier {
  /*!
   * \brief Identifier for data element.
   */
  std::uint64_t id{};
  /*! \brief Flag for element presence. */
  ElementPresence presence_flag{};
  /*! \brief Represents final state. */
  bool is_final{};
};

/*!
 * \brief Transition input value.
 */
struct ElementInput {
  /*! \brief Input class type */
  ::amsr::asn1::Asn1Parser::Asn1Class class_type{};
  /*! \brief Input tag value */
  std::uint64_t tag{};
};

/*!
 * \brief Transition info for state.
 */
class Transition final {
 public:
  /*!
   * \brief Constructor for state.
   * \param[in] current_element Current element id.
   * \param[in] input Input class and tag.
   * \param[in] next_element Next element id.
   */
  explicit Transition(ElementIdentifier const& current_element, ElementInput const& input,
                      ElementIdentifier const& next_element = {})
      : current_element_{current_element}, input_{input}, next_element_{next_element} {}

  /*!
   * \brief Comparison operator.
   * \param[in] other Object to be compared.
   * \return \c true If this value is same as other, \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  bool operator==(Transition const& other) const {
    return (current_element_.id == other.current_element_.id) && (input_.class_type == other.input_.class_type) &&
           (input_.tag == other.input_.tag);
  }

  /*!
   * \brief Less than operator.
   * \param[in] other Object to be compared.
   * \return \c true If this value is less than other, \c false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  bool operator<(Transition const& other) const {
    bool is_less{current_element_.id < other.current_element_.id};
    if (current_element_.id == other.current_element_.id) {
      is_less = input_.class_type < other.input_.class_type;
      if (input_.class_type == other.input_.class_type) {
        is_less = input_.tag < other.input_.tag;
      }
    }
    return is_less;
  }

  /*!
   * \brief Getter for current element id.
   * \return Value of Current Element.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  ElementIdentifier const& CurentElement() const noexcept { return current_element_; }

  /*!
   * \brief Getter for next element id.
   * \return Value of Next Element.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  ElementIdentifier const& NextElement() const noexcept { return next_element_; }

 private:
  /*! \brief Current element id. */
  ElementIdentifier current_element_;
  /*! \brief Transition input value. */
  ElementInput input_;
  /*! \brief Next element id. */
  ElementIdentifier next_element_;
};

}  // namespace builder
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_TRANSITION_H_
