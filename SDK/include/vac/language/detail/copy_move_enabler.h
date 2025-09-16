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
/*!        \file  vac/language/detail/copy_move_enabler.h
 *        \brief  Helper template for enabling/disabling copy/move constructor/assignment.
 *         \unit  VaCommonLib::MemoryManagement::Either
 *
 *      \details  Conditionally deletes copy/move constructor/assignment based on template arguments.
 *                Used as a private member in classes which then uses default declarations
 *                for corresponding methods.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_LANGUAGE_DETAIL_COPY_MOVE_ENABLER_H_
#define LIB_VAC_INCLUDE_VAC_LANGUAGE_DETAIL_COPY_MOVE_ENABLER_H_

namespace vac {
namespace language {
namespace detail {

// VECTOR Disable AutosarC++17_10-A12.4.1: MD_VAC_A12.4.1_noVirtualDtor
/*!
 *  \brief   Conditionally disable copy/move constructor.
 *  \tparam  IsCopyConstructible The bool to check if it is copy constructible.
 *  \tparam  IsMoveConstructible The bool to check if it is move constructible.
 */
template <bool IsCopyConstructible, bool IsMoveConstructible>
class CopyMoveConstructorEnabler {};

/*!
 *  \brief   Conditionally disable copy/move assignment.
 *  \tparam  IsCopyAssignable The bool to check if it is copy assignable.
 *  \tparam  IsMoveAssignable The bool to check if it is move assignable.
 */
template <bool IsCopyAssignable, bool IsMoveAssignable>
class CopyMoveAssignmentEnabler {};

// VECTOR Next Construct AutosarC++17_10-A10.1.1: MD_VAC_A10.1.1_multipleInheritance
// VECTOR Disable AutosarC++17_10-M10.2.1: MD_VAC_M10.2.1_uniqueEntityNamesInMultipleInheritance
/*!
 * \brief   Conditionally disables copy/move constructor/assignment.
 * \tparam  IsCopyConstructible The bool to check if it is copy constructible.
 * \tparam  IsMoveConstructible The bool to check if it is move constructible.
 * \tparam  IsCopyAssignable The bool to check if it is copy assignable.
 * \tparam  IsMoveAssignable The bool to check if it is move assignable.
 */
template <bool IsCopyConstructible, bool IsMoveConstructible, bool IsCopyAssignable, bool IsMoveAssignable>
class CopyMoveEnabler : private CopyMoveConstructorEnabler<IsCopyConstructible, IsMoveConstructible>,
                        private CopyMoveAssignmentEnabler<IsCopyAssignable, IsMoveAssignable> {};

// VECTOR Disable AutosarC++17_10-A12.8.6:MD_VAC_A12.8.6_copyMoveConstructorAssignmentShallBeProtectedOrDeleted
// VECTOR Disable AutosarC++17_10-A12.4.2: MD_VAC_A12.4.2_noVirtualDtor
/*! \brief Disables copy constructor. */
template <>
class CopyMoveConstructorEnabler<false, true> {
 public:
  /*!
   * \brief  Default constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  constexpr CopyMoveConstructorEnabler() = default;

  /*! \brief  Default copy constructor deleted. */
  constexpr CopyMoveConstructorEnabler(CopyMoveConstructorEnabler const&) = delete;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief  Default move constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  constexpr CopyMoveConstructorEnabler(CopyMoveConstructorEnabler&&) noexcept = default;

  /*!
   * \brief   Default copy assignment.
   * \return  A reference to the assigned-to object.
   * \spec
   *  requires true;
   * \endspec
   */
  CopyMoveConstructorEnabler& operator=(CopyMoveConstructorEnabler const&) & noexcept = default;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief   Default move assignment.
   * \return  A reference to the assigned-to object.
   * \spec
   *  requires true;
   * \endspec
   */
  CopyMoveConstructorEnabler& operator=(CopyMoveConstructorEnabler&&) & noexcept = default;

  /*!
   * \brief  Default destructor.
   * \spec
   *  requires true;
   * \endspec
   */
  ~CopyMoveConstructorEnabler() noexcept = default;
};

/*! \brief  Disables move constructor. */
template <>
class CopyMoveConstructorEnabler<true, false> {
 public:
  /*!
   * \brief  Default constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  constexpr CopyMoveConstructorEnabler() = default;

  /*!
   * \brief  Default copy constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  constexpr CopyMoveConstructorEnabler(CopyMoveConstructorEnabler const&) noexcept = default;

  /*! \brief  Default move constructor deleted. */
  constexpr CopyMoveConstructorEnabler(CopyMoveConstructorEnabler&&) = delete;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief   Default copy assignment.
   * \return  A reference to the assigned-to object.
   * \spec
   *  requires true;
   * \endspec
   */
  CopyMoveConstructorEnabler& operator=(CopyMoveConstructorEnabler const&) & noexcept = default;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief   Default move assignment.
   * \return  A reference to the assigned-to object.
   * \spec
   *  requires true;
   * \endspec
   */
  CopyMoveConstructorEnabler& operator=(CopyMoveConstructorEnabler&&) & noexcept = default;

  /*!
   * \brief  Default destructor.
   * \spec
   *  requires true;
   * \endspec
   */
  ~CopyMoveConstructorEnabler() noexcept = default;
};

/*! \brief  Disables both copy and move constructor. */
template <>
class CopyMoveConstructorEnabler<false, false> {
 public:
  /*!
   * \brief  Default constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  constexpr CopyMoveConstructorEnabler() = default;

  /*! \brief  Default copy constructor deleted. */
  constexpr CopyMoveConstructorEnabler(CopyMoveConstructorEnabler const&) = delete;

  /*! \brief  Default move constructor deleted. */
  constexpr CopyMoveConstructorEnabler(CopyMoveConstructorEnabler&&) = delete;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief   Default copy assignment.
   * \return  A reference to the assigned-to object.
   * \spec
   *  requires true;
   * \endspec
   */
  CopyMoveConstructorEnabler& operator=(CopyMoveConstructorEnabler const&) & noexcept = default;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief   Default move assignment.
   * \return  A reference to the assigned-to object.
   * \spec
   *  requires true;
   * \endspec
   */
  CopyMoveConstructorEnabler& operator=(CopyMoveConstructorEnabler&&) & noexcept = default;

  /*!
   * \brief  Default destructor.
   * \spec
   *  requires true;
   * \endspec
   */
  ~CopyMoveConstructorEnabler() noexcept = default;
};

/*! \brief  Disables copy assignment. */
template <>
class CopyMoveAssignmentEnabler<false, true> {
 public:
  // VECTOR Disable AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate

  /*!
   * \brief  Default constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  constexpr CopyMoveAssignmentEnabler() = default;

  /*!
   * \brief  Default copy constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  constexpr CopyMoveAssignmentEnabler(CopyMoveAssignmentEnabler const&) noexcept = default;

  /*!
   * \brief  Default move constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  constexpr CopyMoveAssignmentEnabler(CopyMoveAssignmentEnabler&&) noexcept = default;

  /*!
   * \brief  Default copy assignment operator deleted.
   * \return
   */
  CopyMoveAssignmentEnabler& operator=(CopyMoveAssignmentEnabler const&) & = delete;

  /*!
   * \brief  Default move assignment.
   * \return A reference to the assigned-to object.
   * \spec
   *  requires true;
   * \endspec
   */
  CopyMoveAssignmentEnabler& operator=(CopyMoveAssignmentEnabler&&) & noexcept = default;

  /*!
   * \brief  Default destructor.
   * \spec
   *  requires true;
   * \endspec
   */
  ~CopyMoveAssignmentEnabler() noexcept = default;
  // VECTOR Enable AutosarC++17_10-A15.5.1
};

/*! \brief  Disables move assignment. */
template <>
class CopyMoveAssignmentEnabler<true, false> {
 public:
  /*!
   * \brief  Default constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  constexpr CopyMoveAssignmentEnabler() = default;

  /*!
   * \brief   Default copy constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  constexpr CopyMoveAssignmentEnabler(CopyMoveAssignmentEnabler const&) noexcept = default;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief   Default move constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  constexpr CopyMoveAssignmentEnabler(CopyMoveAssignmentEnabler&&) noexcept = default;

  /*!
   * \brief  Default copy assignment.
   * \return A reference to the assigned-to object.
   * \spec
   *  requires true;
   * \endspec
   */
  CopyMoveAssignmentEnabler& operator=(CopyMoveAssignmentEnabler const&) & noexcept = default;

  /*!
   * \brief  Default move assignment operator deleted.
   * \return
   */
  CopyMoveAssignmentEnabler& operator=(CopyMoveAssignmentEnabler&&) & = delete;

  /*!
   * \brief  Default destructor.
   * \spec
   *  requires true;
   * \endspec
   */
  ~CopyMoveAssignmentEnabler() noexcept = default;
};

/*! \brief  Disables copy and move assignment. */
template <>
class CopyMoveAssignmentEnabler<false, false> {
 public:
  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief  Default constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  constexpr CopyMoveAssignmentEnabler() = default;

  /*!
   * \brief  Default copy constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  constexpr CopyMoveAssignmentEnabler(CopyMoveAssignmentEnabler const&) noexcept = default;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief  Default move constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  constexpr CopyMoveAssignmentEnabler(CopyMoveAssignmentEnabler&&) noexcept = default;
  /*!
   * \brief  Default copy assignment operator deleted.
   * \return
   */
  CopyMoveAssignmentEnabler& operator=(CopyMoveAssignmentEnabler const&) & = delete;

  /*!
   * \brief  Default move assignment operator deleted.
   * \return
   */
  CopyMoveAssignmentEnabler& operator=(CopyMoveAssignmentEnabler&&) & = delete;

  /*!
   * \brief  Default destructor.
   * \spec
   *  requires true;
   * \endspec
   */
  ~CopyMoveAssignmentEnabler() noexcept = default;
};
// VECTOR Enable AutosarC++17_10-A12.8.6
// VECTOR Enable AutosarC++17_10-A12.4.1: MD_VAC_A12.4.1_noVirtualDtor
// VECTOR Enable AutosarC++17_10-M10.2.1: MD_VAC_M10.2.1_uniqueEntityNamesInMultipleInheritance
// VECTOR Enable AutosarC++17_10-A12.4.2

}  // namespace detail
}  // namespace language
}  // namespace vac

#endif  // LIB_VAC_INCLUDE_VAC_LANGUAGE_DETAIL_COPY_MOVE_ENABLER_H_
