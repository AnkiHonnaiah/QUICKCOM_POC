/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!       \file   amsr/asn1/builder/asn1_builder_state.h
 *        \brief  ASN.1 Builder state.
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_ASN1_BUILDER_STATE_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_ASN1_BUILDER_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/asn1/asn1_builder.h"
#include "amsr/asn1/asn1_error_domain.h"
#include "amsr/asn1/builder/constraint.h"

namespace amsr {
namespace asn1 {
namespace builder {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_VAASN1_AutosarC++17_10-M7.3.6_using_declaration
/*!
 * \brief String literal for StringView.
 */
using vac::container::literals::operator"" _sv;

/*!
 * \brief Representation of base state for ASN.1 builder.
 * \vprivate Product Private
 */
template <class YT>
class Asn1BuilderState : public amsr::asn1::Asn1Builder {
 public:
  /*!
   * \brief Type alias to access a builder's yielded type in a static way. This avoids clauses like
   *  \c SequenceOfBuilder&lt;builder::FooBuilder, structure::Foo&gt;
   */
  using YieldedType = YT;
  /*!
   * \brief Alias declaration.
   * \vprivate Product Private
   */
  using Asn1Class = ::amsr::asn1::Asn1Parser::Asn1Class;

  /*!
   * \brief Alias declaration.
   * \vprivate Product Private
   */
  using UniversalTag = ::amsr::asn1::Asn1Parser::UniversalElementTag;

  /*!
   * \brief Constructor.
   * \vprivate Product Private
   */
  using Asn1Builder::Asn1Builder;

  /*!
   * \brief Yields a representation of the decoded ASN.1 structure.
   * \error kRuntimeFault If the internal structure is not consistent with the corresponding ASN.1 schema.
   * \vpublic
   */
  virtual auto Yield() const noexcept -> amsr::core::Result<YT> {
    amsr::core::Result<YT> result{amsr::core::Result<YT>::FromError(
        Asn1Errc::kRuntimeFault, "Yield called for a builder that has not yet completed decoding."_sv)};
    if (IsCompleted()) {
      result = std::move(DoYield());
    }
    return result;
  }

 protected:
  /*!
   * \brief Recursively collects data from leaf nodes (such as primitive and SEQUENCE OF builders) and yields
   *    the data structure to be built by this builder.
   * \details All the checks regarding values returned by builders are handled in this class' \c Yield method.
   *    \c DoYield is intended for subclasses to be overwritten.
   * \return The yielded data structure
   * \vprivate Product Private
   */
  virtual auto DoYield() const noexcept -> amsr::core::Result<YT> = 0;
};

}  // namespace builder
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_BUILDER_ASN1_BUILDER_STATE_H_
