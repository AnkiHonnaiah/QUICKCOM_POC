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
/*!        \file  reverse.h
 *        \brief  Wrappers reversing an iterable's iterators.
 *         \unit  VaCommonLib::Iterators::Reverse
 *
 *      \details  This class enables backward iteration on compatible classes.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_ITERATORS_REVERSE_H_
#define LIB_VAC_INCLUDE_VAC_ITERATORS_REVERSE_H_

namespace vac {
namespace iterators {

/*!
 * \brief  Type that reverses an iterable type with a bidirectional iterator for use in a range based for loop.
 * \tparam Iterable Type of the iterable.
 *         Limitations:
 *         - Iterable::reverse_iterator must be public and fulfill C++ named requirement: LegacyBidirectionalIterator.
 *         - Iterable::rbegin() and Iterable::rend() must have return type Iterable::reverse_iterator.
 * \trace  CREQ-VaCommonLib-WrapperIterator
 */
template <typename Iterable>
class Reverse final {
 public:
  /*!
   * \brief     Construct from an iterator.
   * \param[in] it The iterator of Reverse to be constructed from.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   */
  explicit Reverse(Iterable& it) noexcept : iterable_{it} {}

  /*!
   * \brief  Gets an iterator to the end of the passed iterable.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return An iterator to the end of the passed iterable.
   */
  typename Iterable::reverse_iterator begin() noexcept {
    return iterable_.rbegin();  // VCA_VAC_VALID_NONSTATIC_METHOD
  }

  /*!
   * \brief  Gets an iterator beyond the start of the passed iterable.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return An iterator beyond the start of the passed iterable.
   */
  typename Iterable::reverse_iterator end() noexcept {
    return iterable_.rend();  // VCA_VAC_VALID_NONSTATIC_METHOD
  }

 private:
  /*!
   * \brief The iterable to be reversed.
   */
  Iterable& iterable_;
};

/*!
 * \brief  Type that reverses a bidirectional iterator for use in a range based for loop.
 * \tparam Iterable Type of the iterable.
 *         Limitations:
 *         - Iterable::const_reverse_iterator must be public and fulfill C++ named requirement:
 *           LegacyBidirectionalIterator.
 *         - Iterable::rbegin() and Iterable::rend() must have return type Iterable::const_reverse_iterator.
 */
template <typename Iterable>
class ConstReverse final {
 public:
  /*!
   * \brief Construct from an iterator.
   * \param it The iterator of ConstReverse to be constructed from.
   * \pre   -
   * \spec
   *   requires true;
   * \endspec
   */
  explicit ConstReverse(Iterable const& it) noexcept : iterable_{it} {}

  /*!
   * \brief  Gets an iterator to the end of the passed iterable.
   * \return An iterator to the end of the passed iterable.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   */
  typename Iterable::const_reverse_iterator begin() const noexcept {
    return iterable_.rbegin();  // VCA_VAC_VALID_NONSTATIC_METHOD
  }

  /*!
   * \brief  Gets an iterator beyond the start of the passed iterable.
   * \return An iterator beyond the start of the passed iterable.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   */
  typename Iterable::const_reverse_iterator end() const noexcept {
    return iterable_.rend();  // VCA_VAC_VALID_NONSTATIC_METHOD
  }

 private:
  /*!
   * \brief The iterable to be reversed.
   */
  Iterable const& iterable_;
};

// VECTOR Next Construct AutosarC++17_10-M17.0.3: MD_VAC_M17.0.3_standardLibraryFunctionNameOverride
/*!
 * \brief     Helper function to reverse an iterable.
 * \tparam    Iterable Type of the iterable.
 *            Limitations:
 *             - Iterable::reverse_iterator must be public and fulfill C++ named requirement:
 *               LegacyBidirectionalIterator.
 *             - Iterable::rbegin() and Iterable::rend() must have return type Iterable::reverse_iterator.
 * \param[in] it The iterable to reverse.
 * \return    The reversed iterable.
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 */
template <typename Iterable>
auto reverse(Iterable& it) noexcept -> Reverse<Iterable> {
  return Reverse<Iterable>{it};
}

// VECTOR Next Construct AutosarC++17_10-M17.0.3: MD_VAC_M17.0.3_standardLibraryFunctionNameOverride
/*!
 * \brief  Helper function to reverse a const iterable.
 * \tparam Iterable Type of the iterable.
 *         Limitations:
 *         - Iterable::const_reverse_iterator must be public and fulfill C++ named requirement:
 *           LegacyBidirectionalIterator.
 *         - Iterable::rbegin() and Iterable::rend() must have return type Iterable::const_reverse_iterator.
 * \param  it The const iterable to reverse.
 * \return The const reversed iterable.
 * \pre    -
 * \spec
 *   requires true;
 * \endspec
 */
template <typename Iterable>
auto reverse(Iterable const& it) noexcept -> ConstReverse<Iterable> {
  return ConstReverse<Iterable>{it};
}
}  // namespace iterators
}  // namespace vac

#endif  // LIB_VAC_INCLUDE_VAC_ITERATORS_REVERSE_H_
