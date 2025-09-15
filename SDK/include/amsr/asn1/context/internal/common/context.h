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
/*!        \file  amsr/asn1/context/internal/common/context.h
 *        \brief  Common general templates for context markers. Do not include as client, but contexts.h instead.
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_COMMON_CONTEXT_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_COMMON_CONTEXT_H_

#include "amsr/asn1/context/internal/common/tags.h"
#include "amsr/asn1/context/internal/common/traits.h"
#include "amsr/core/variant.h"
#include "ara/core/vector.h"

namespace amsr {
namespace asn1 {
namespace context {

/*!
 * \brief Base class for different kinds of schema context.
 * \tparam T The context's determinant. In the basic, user-visible case, this will usually be a structure type.
 *      In internal usage, this may be a type that identifies a type's property, or any ASN.1 declaration. The
 *      determinant must identify a place in an ASN.1 schema that can be decoded, encoded or validated.
 */
template <typename T>
class ContextBase {
 public:
  /*!
   * \brief Internally, this can be a determinant, but metacontexts ensure structure nature of their determinants
   *        when accessed via this type name. Hence, this type exposes the structural type that the context concerns.
   *        A \c Context<Boolean> has \c Boolean and an \c Optional<Explicit<Integer>> has \c Integer for this name.
   */
  using StructureType = T;

  /*!
   * \brief Default constructor
   */
  ContextBase() noexcept = delete;
  /*!
   * \brief Defaulted copy constructor
   */
  ContextBase(ContextBase const&) noexcept = delete;
  /*!
   * \brief Defaulted move constructor
   */
  ContextBase(ContextBase&&) noexcept = delete;
  /*!
   * \brief Defaulted copy assignment operator
   */
  auto operator=(ContextBase const&) & noexcept -> ContextBase& = delete;
  /*!
   * \brief Defaulted move assignment operator
   */
  auto operator=(ContextBase&&) & noexcept -> ContextBase& = delete;
  /*!
   * \brief Defaulted destructor
   */
  virtual ~ContextBase() noexcept = default;

  /*!
   * \brief Access the context's tag class (universal, context-specific, application, private)
   * \return Default value for a tag's class: UNIVERSAL
   */
  static constexpr auto TagClass() noexcept -> Class { return Class::kUniversal; }

  /*!
   * \brief Access the context's tag number
   * \return Default value for a tag number: SEQUENCE
   */
  static constexpr auto TagNumber() noexcept -> Tag { return static_cast<Tag>(UniversalTag::kSequence); }

  /*!
   * \brief Access the context's constructedness flag
   * \return Default value for constructedness: constructed
   */
  static constexpr auto IsConstructed() noexcept -> bool { return true; }

  /*!
   * \brief Access the context's OPTIONAL/PRESENT flag
   * \return Default value for optionality: PRESENT (false)
   */
  static constexpr auto IsOptional() noexcept -> bool {
    static_assert(!IsAmsrCoreOptional<StructureType>{},
                  "A context that is not context::Optional has an amsr::core::Optional structure type. To model "
                  "optional contexts, use context::Optional<T> without amsr::core::Optional.");
    return false;
  }

  /*!
   * \brief Access the context's DEFAULT value if present
   * \return Default value for DEFAULT values: an empty optional
   */
  static auto DefaultValue() noexcept -> amsr::core::Optional<StructureType> { return amsr::core::nullopt; }
};

/*!
 * \brief The default context type. Most user-visible context will probably be derived from this template.
 * \tparam T The context determinant (See ContextBase)
 */
template <typename T, typename Enable = void>
class Context : public ContextBase<T> {};

/*!
 * \brief Exclude a particular nesting order
 * \details This Context template is a default wrapper around structure types. Nesting it inside other contexts has
 *          no meaning.
 * \tparam T See primary template
 */
template <typename T>
class Context<T, typename std::enable_if<HasStructureType<T>::value>::type> {
  static_assert((sizeof(T) == 0), "Context<T> cannot use other context-like types as T.");
};

}  // namespace context
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_COMMON_CONTEXT_H_
