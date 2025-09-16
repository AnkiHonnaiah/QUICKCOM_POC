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
/*!        \file  reference_wrapper.h
 *        \brief  constexpr version of std::reference_wrapper.
 *         \unit  VaCommonLib::LanguageSupport::ReferenceWrapper
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_LANGUAGE_REFERENCE_WRAPPER_H_
#define LIB_VAC_INCLUDE_VAC_LANGUAGE_REFERENCE_WRAPPER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
namespace vac {
namespace language {

// VECTOR Next Construct AutosarC++17_10-A12.0.1: MD_VAC_A12.0.1_missingMoveConstructorAssignmentOperator
/*!
 * \brief  Constexpr version of std::reference_wrapper.
 * \tparam T Wrapped type.
 * \trace  CREQ-VaCommonLib-ReferenceWrapper
 */
template <typename T>
class reference_wrapper final {
 public:
  /*!
   * \brief Type.
   */
  using type = T;

  /*!
   * \brief     Constructor.
   * \param[in] x Reference to value.
   * \details   The value object should outlive the reference_wrapper object
   *            otherwise it could lead to unknown behavior.
   * \pre       -
   * \spec
   *  requires true;
   * \endspec
   */
  constexpr explicit reference_wrapper(T& x) noexcept : ptr_(&x) {}

  /*!
   * \brief Copy Constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  constexpr reference_wrapper(reference_wrapper const&) noexcept = default;

  /*!
   * \brief Default destructor.
   * \spec
   *  requires true;
   * \endspec
   */
  ~reference_wrapper() noexcept = default;

  /*!
   * \brief  Copy Assignment.
   * \param  other Other object.
   * \return A reference to the assigned-to object.
   * \spec
   *  requires true;
   * \endspec
   */
  auto operator=(reference_wrapper<T> const& other) & noexcept -> reference_wrapper& = default;

  /*!
   * \brief  Get the reference reference.
   * \pre    -
   * \return The reference reference.
   * \spec
   *  requires true;
   * \endspec
   */
  constexpr auto get() const noexcept -> T const& {
    // Can never be nullptr because the constructor only takes a reference.
    return *ptr_;  // VCA_VAC_VALID_RETURN_VALUE
  }

  /*!
   * \brief  Get the reference reference.
   * \pre    -
   * \return The reference reference.
   * \spec
   *  requires true;
   * \endspec
   */
  auto get() noexcept -> T& {
    // Can never be nullptr because the constructor only takes a reference.
    return *ptr_;  // VCA_VAC_VALID_RETURN_VALUE
  }

  /*!
   * \brief  Explicit conversion to reference.
   * \pre    -
   * \return The reference reference.
   * \spec
   *  requires true;
   * \endspec
   */
  explicit constexpr operator T const&() const noexcept { return get(); }  // VCA_VAC_VALID_RETURN_VALUE

  /*!
   * \brief  Explicit conversion to reference.
   * \pre    -
   * \return The reference reference.
   * \spec
   *  requires true;
   * \endspec
   */
  explicit operator T&() noexcept { return get(); }  // VCA_VAC_VALID_RETURN_VALUE

 private:
  /*!
   * \brief Reference.
   */
  T* ptr_;
};

}  // namespace language
}  // namespace vac

#endif  // LIB_VAC_INCLUDE_VAC_LANGUAGE_REFERENCE_WRAPPER_H_
