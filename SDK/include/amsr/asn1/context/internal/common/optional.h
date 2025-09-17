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
/*!        \file  amsr/asn1/context/internal/common/optional.h
 *        \brief  Special template for OPTIONAL contexts
 *
 *********************************************************************************************************************/
#ifndef LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_COMMON_OPTIONAL_H_
#define LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_COMMON_OPTIONAL_H_

#include "amsr/asn1/context/internal/common/context.h"
#include "amsr/asn1/context/internal/common/nature_metafunctions.h"
#include "amsr/asn1/context/internal/common/tags.h"

namespace amsr {
namespace asn1 {
namespace context {

/*!
 * \brief A template used for marking a context OPTIONAL.
 * \details This is compatible with both context types and structure types. This class inherits from a class
 *          evaluated by a metafunction which, for structure types, will wrap a \c Context<T> around \c T which
 *          guarantees the OPTIONAL context to inherit default values from \c Context<T> which may be explicitly
 *          specialized, for example in the case of primitives.
 * \tparam T The context or determinant to mark as optional
 */
template <typename T>
class Optional : public EnsureContextNature<T>::Type {
  // NOTE: It looks like this class template isn't necessary at first glance. However, it allows for reducing optional
  //  contexts to their unmodified ones. If this didn't exist, there'd need to be a Context<core::Optional<T>> for every
  //  explicitly specialized Context<T>.
 public:
  /*!
   * \brief Default constructor
   */
  Optional() noexcept = delete;
  /*!
   * \brief Defaulted copy constructor
   */
  Optional(Optional const&) noexcept = delete;
  /*!
   * \brief Defaulted move constructor
   */
  Optional(Optional&&) noexcept = delete;
  /*!
   * \brief Defaulted copy assignment operator
   */
  auto operator=(Optional const&) & noexcept -> Optional& = delete;
  /*!
   * \brief Defaulted move assignment operator
   */
  auto operator=(Optional&&) & noexcept -> Optional& = delete;
  /*!
   * \brief Defaulted destructor
   */
  ~Optional() noexcept override = default;

  /*!
   * \brief Recursively searched structure type
   * \details Similarly to the metafunction in the base class, this iterates through a nested metacontext and finds
   *        the "lowest T". For instance, a \c Optional<Tagged<Boolean>> will have
   *        \c amsr::core::Optional<Boolean> for StructureType.
   */
  using StructureType = amsr::core::Optional<typename EnsureStructureNature<T>::Type>;

  /*!
   * \brief Access the context's OPTIONAL/PRESENT flag
   * \return \c true
   */
  static constexpr auto IsOptional() noexcept -> bool { return true; }
};

}  // namespace context
}  // namespace asn1
}  // namespace amsr

#endif  // LIB_VAASN1_INCLUDE_AMSR_ASN1_CONTEXT_COMMON_OPTIONAL_H_
