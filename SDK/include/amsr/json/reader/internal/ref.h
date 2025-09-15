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
/**       \file     ref.h
 *        \brief    An alternative to the std::reference_wrapper.
 *
 *        \details  Significant lesser writing/reading overhead than the STD implementation.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_REF_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_REF_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <utility>

namespace amsr {
namespace json {
namespace internal {
/*!
 * \brief A custom reference wrapper.
 * \tparam T Type of stored reference.
 *
 * \details -
 */
template <typename T>
class Ref {
 public:
  // VECTOR NC VectorC++-V12.1.4: MD_JSON_implicit_conversion
  /*!
   * \brief Constructs a wrapped reference.
   * \param reference to store.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr Ref(T& reference) noexcept : reference_{&reference} {}

  // VECTOR NC AutosarC++17_10-A15.4.4: MD_JSON_AutosarC++17_10-A15.4.4_explicit_noexcept_missing
  /*!
   * \brief Implicit conversion to a constant Ref.
   * \tparam C Helper type for SFINAE.
   * \return The Ref object as a constant Ref object.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename C,
            typename = std::enable_if_t<(std::is_same<C, T>::value) && (!std::is_const<C>::value), std::uint8_t>>
  constexpr operator Ref<C const>() const noexcept {  // VECTOR SL AutosarC++17_10-A13.5.2: MD_JSON_implicit_conversion
    return *this->reference_;
  }

  /*!
   * \brief Const member of pointer operator.
   * \return Const pointer to contained reference.
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr auto operator->() const noexcept -> T const* { return this->reference_; }

  /*!
   * \brief Member of pointer operator.
   * \return Pointer to contained reference.
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator->() noexcept -> T* { return this->reference_; }

  /*!
   * \brief Const dereference operator.
   * \return Const reference of contained reference.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator*() const& noexcept -> T const& { return *this->reference_; }

  /*!
   * \brief Dereference operator.
   * \return Reference of contained reference.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator*() & noexcept -> T& { return *this->reference_; }

 private:
  /*!
   * \brief Stored reference.
   */
  T* reference_;
};
}  // namespace internal
}  // namespace json
}  // namespace amsr

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_REF_H_
