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
/**     \file       ref.h
 *      \brief
 *      \details
 *
 *      \unit       LogAndTrace::Common::Ref
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_REF_H_
#define LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_REF_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <utility>

namespace amsr {
namespace log {
namespace internal {
/*!
 * \brief An alternative to the std::reference_wrapper without as much writing/reading overhead.
 * \tparam T The type whose reference should be stored.
 */
template <typename T>
class Ref final {
  /*!
   * \brief The stored reference
   */
  T* reference_;

 public:
  /*!
   * \brief Create a wrapped reference
   *
   * \param reference The reference to store
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NC VectorC++-V12.1.4: MD_LOG_AutosarC++17_10-A12.1.4_implicit_conversion
  // VCA_LOGAPI_VALID_OBJECT
  constexpr Ref(T& reference) noexcept : reference_{&reference} {}  // NOLINT(hicpp-explicit-conversions)

  /*!
   * \brief Implicit conversion to a reference
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NC AutosarC++17_10-A13.5.2: MD_LOG_AutosarC++17_10-A13.5.2_implicit_conversion
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_LOG_AutosarC++17_10-M9.3.3_non_const_method
  operator T&() noexcept { return *this->reference_; }  // NOLINT(hicpp-explicit-conversions)

  /*!
   * \brief Implicit conversion to a constant reference
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NC AutosarC++17_10-A13.5.2: MD_LOG_AutosarC++17_10-A13.5.2_implicit_conversion
  operator T const&() const noexcept { return *this->reference_; }  // NOLINT(hicpp-explicit-conversions)

  // VECTOR NC AutosarC++17_10-A8.2.1: MD_LOG_AutosarC++17_10-A8.2.1_false_positive_operator
  // VECTOR NC AutosarC++17_10-A13.5.2: MD_LOG_AutosarC++17_10-A13.5.2_implicit_conversion
  /*!
   * \brief Implicit conversion to a constant Ref
   * \spec
   *   requires true;
   * \endspec
   */
  template <class C = T, typename = std::enable_if_t<std::is_same<C, T>::value && (not std::is_const<C>::value)>>
  constexpr operator Ref<C const>() const noexcept {  // NOLINT(hicpp-explicit-conversions)
    return *this->reference_;
  }

  /*!
   * \brief Returns the reference.
   * \spec
   *   requires true;
   * \endspec
   */
  auto Get() const& noexcept -> T const& { return *this->reference_; }

  /*!
   * \brief Returns the reference.
   * \spec
   *   requires true;
   * \endspec
   */
  auto Get() & noexcept -> T& { return *this->reference_; }

  /*!
   * \brief Returns the reference as a const pointer.
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr auto AsPtr() const noexcept -> T const* { return this->reference_; }

  /*!
   * \brief Returns the reference as a pointer.
   * \spec
   *   requires true;
   * \endspec
   */
  auto AsPtr() noexcept -> T* { return this->reference_; }

  /*!
   * \brief Implements arrow operator
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr auto operator->() const noexcept -> T const* { return this->AsPtr(); }

  /*!
   * \brief Implements arrow operator
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator->() noexcept -> T* { return this->AsPtr(); }

  /*!
   * \brief Implements dereference operator
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator*() const& noexcept -> T const& { return this->Get(); }

  /*!
   * \brief Implements dereference operator
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator*() & noexcept -> T& { return this->Get(); }

  /*!
   * \brief Implements dereference operator
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr auto operator*() const&& noexcept -> T const&& { return *std::move(*this).reference_; }
};

/*!
 * \brief Compares two Refs for equality
 *
 * \tparam T  The underlying type to compare
 * \param lhs The left reference to compare
 * \param rhs The right reference to compare
 * \return    True if both point to the same object
 * \spec
 *   requires true;
 * \endspec
 */
template <typename T>
auto operator==(Ref<T> const& lhs, Ref<T> const& rhs) noexcept -> bool {
  return *lhs == *rhs;
}

/*!
 * \brief Compares two Refs for inequality
 *
 * \tparam T  The underlying type to compare
 * \param lhs The left reference to compare
 * \param rhs The right reference to compare
 * \return    False if both point to the same object
 * \spec
 *   requires true;
 * \endspec
 */
template <typename T>
auto operator!=(Ref<T> const& lhs, Ref<T> const& rhs) noexcept -> bool {
  return *lhs != *rhs;
}

/*!
 * \brief Create a wrapped reference
 *
 * \tparam T        The type of the stored reference
 * \param reference The stored reference
 * \return          A wrapped reference
 * \spec
 *   requires true;
 * \endspec
 */
template <typename T>
auto MakeRef(T& reference) noexcept -> Ref<T> {
  return Ref<T>{reference};
}

/*!
 * \brief Create a wrapped reference
 *
 * \tparam T        The type of the stored reference
 * \param reference The stored reference
 * \return          A wrapped reference
 * \spec
 *   requires true;
 * \endspec
 */
template <typename T>
auto MakeCRef(T const& reference) noexcept -> Ref<T const> {
  return Ref<T const>{reference};
}
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_REF_H_
