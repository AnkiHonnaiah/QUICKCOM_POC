/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in The license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  array_vec.h
 *        \brief  A vector backed by a fixed size array
 *      \details  Enables tracking of size and supports non-default types
 *
 *         \unit  LogAndTrace::Common::ArrayVec
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_ARRAY_VEC_H_
#define LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_ARRAY_VEC_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <cstdint>
#include <iterator>
#include <limits>
#include <type_traits>
#include <utility>

#include "amsr/core/array.h"
#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "vac/iterators/range.h"

#include "amsr/log/internal/log_error_domain.h"
#include "amsr/log/internal/ref.h"
#include "amsr/log/internal/utils.h"

namespace amsr {
namespace log {
namespace internal {
/*!
 * \brief   A data structure with a fixed maximum capacity that can be used similar to a ara::core::Vector.
 * \tparam  T         Type inside the array.
 *                    Must implement the CopyConstructible, MoveConstructible and Destructible named requirements.
 * \tparam  Capacity  Maximum capacity.
 *                    Must be a non-zero positive integer.
 */
// VECTOR NC Metric-OO.WMC.One: MD_LOG_Metric.OO.WMC.One
template <typename T, std::size_t Capacity>
class ArrayVec final {
  using StorageElement = typename std::aligned_storage<sizeof(T), alignof(T)>::type;

  /*!
   * \brief The internal storage type
   */
  using Storage = core::Array<StorageElement, Capacity>;

  /*!
   * \brief The type without cv qualifiers
   */
  using Decayed = std::decay_t<T>;

 public:
  /*!
   * \brief ype of the size field
   */
  using size_type = std::size_t;
  /*!
   * \brief Type of the values in The vector
   */
  using value_type = T;
  /*!
   * \brief Type of the values in The vector but const-qualified
   */
  using const_value_type = std::add_const_t<value_type>;
  /*!
   * \brief Reference type for contained elements
   */
  using reference = value_type&;
  /*!
   * \brief Const reference type for contained elements
   */
  using const_reference = const_value_type&;
  /*!
   * \brief Typedef for a pointer
   */
  using pointer = value_type*;
  /*!
   * \brief Typedef for a const pointer
   */
  using const_pointer = const_value_type*;
  /*!
   * \brief Iterator type for mutable iteration
   */
  using iterator = pointer;
  /*!
   * \brief Iterator type for constant iteration
   */
  using const_iterator = const_pointer;

  /*!
   * \brief Default constructor for the vector
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr ArrayVec() noexcept = default;
  /*!
   * \brief Copy constructor
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr ArrayVec(ArrayVec const& other) noexcept = default;
  /*!
   * \brief Move constructor
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr ArrayVec(ArrayVec&& other) noexcept = default;
  /*!
   * \brief Copy assignment
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator=(ArrayVec const& other) & noexcept -> ArrayVec& = default;
  /*!
   * \brief Move assignment
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator=(ArrayVec&& other) & noexcept -> ArrayVec& = default;
  /*!
   * \brief Destructor
   * \spec
   *   requires true;
   * \endspec
   */
  ~ArrayVec() noexcept { this->clear(); }

  // VECTOR NC AutosarC++17_10-M0.3.1: MD_LOG_AutosarC++17_10-M0.3.1_false_positive_use_after_free
  /*!
   * \brief Gets the current size of the vector
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr auto size() const noexcept -> size_type { return this->size_; }

  /*!
   * \brief Gets the maximum size of the vector
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_LOG_AutosarC++17_10-M9.3.3_stable_api
  constexpr auto capacity() const noexcept -> size_type { return Capacity; }

  /*!
   * \brief Returns true if the vector is empty
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr bool empty() const noexcept { return this->size() == 0; }

  /*!
   * \brief Returns true if the vector is full
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr bool full() const noexcept { return this->size() == this->capacity(); }

  // VECTOR NC AutosarC++17_10-M5.0.16: MD_LOG_AutosarC++17_10-M5.0.16_FalsePositive
  /*!
   * \brief   Inserts a value at the end of the vector
   * \tparam  Value   The type to push to the ArrayVec.
   *                  The value_type in the ArrayVec must be constructible from Value.
   * \param   value   The value to add to the vector
   * \return  The empty Result, or the error that occurred
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename Value = value_type>
  auto Push(Value&& value) noexcept -> Result<void> {
    Result<void> result{Error<void>(LogErrc::kBufferTooSmall, "ArrayVec::PushBack: Storage is full")};
    if (!this->full()) {
      size_type const size{this->size()};
      new (&this->storage_[size]) value_type(std::forward<value_type>(value));
      this->size_ += 1;
      result.EmplaceValue();
    }
    return result;
  }

  // VECTOR NC AutosarC++17_10-M0.3.1: MD_LOG_AutosarC++17_10-M0.3.1_false_positive_use_after_free
  /*!
   * \brief Gets a pointer to the continuous memory
   * \return The pointer to the first element
   * \spec
   *   requires true;
   * \endspec
   */
  auto data() & noexcept -> pointer {
    // VECTOR NL AutosarC++17_10-A5.2.4: MD_LOG_AutosarC++17_10-A5.2.4_cast_from_aligned_storage
    return reinterpret_cast<pointer>(this->storage_.data());
  }

  /*!
   * \copydoc data()
   */
  constexpr auto data() const& noexcept -> const_pointer {
    // VECTOR NL AutosarC++17_10-A5.2.4: MD_LOG_AutosarC++17_10-A5.2.4_cast_from_aligned_storage
    return reinterpret_cast<const_pointer>(this->storage_.data());
  }

  /*!
   * \brief Returns the item at the index
   * \param index The index of the requested item
   * \return The reference to the item if it exists, else nothing
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator[](size_type index) & noexcept -> Optional<Ref<value_type>> {
    Optional<Ref<value_type>> result{};
    if (index < this->size()) {
      // VCA_LOGAPI_VALID_REFERENCE_ARGUMENT
      result.emplace(*this->GetAt(index));
    }
    return result;
  }

  /*!
   * \copydoc operator[](size_type index)
   */
  auto operator[](size_type index) const& noexcept -> Optional<Ref<const_value_type>> {
    Optional<Ref<const_value_type>> result{};
    if (index < this->size()) {
      // VCA_LOGAPI_VALID_REFERENCE_ARGUMENT
      result.emplace(*this->GetAt(index));
    }
    return result;
  }

  /*!
   * \brief Removes all stored values
   * \spec
   *   requires true;
   * \endspec
   */
  void clear() noexcept {
    for (size_type const idx : vac::iterators::range<size_type>(0, this->size())) {
      this->Destroy(idx);
    }
    this->size_ = 0;
  }

  /*!
   * \brief Returns the specified item from the vector
   * \param index The index to pop. Defaults to the last.
   * \return The specified or last item if it exists, else nothing
   * \spec
   *   requires true;
   * \endspec
   */
  auto Pop(std::size_t index = std::numeric_limits<std::size_t>::max()) noexcept -> Optional<value_type> {
    Optional<value_type> result{};
    // VECTOR NC AutosarC++17_10-M0.1.2: MD_LOG_AutosarC++17_10-M0.1.2_false_positive_no_sideeffect
    if ((this->size() > 0) && ((index < this->size()) || (index == std::numeric_limits<std::size_t>::max()))) {
      std::size_t const last_idx{this->size() - 1};
      if (index != std::numeric_limits<std::size_t>::max()) {
        // We cannot easily use std::remove, so we rotate by 1
        iterator const tb_removed{this->GetAt(index)};
        // VCA_LOGAPI_CHECKED_CONTRACT
        Drop(std::rotate(tb_removed, std::next(tb_removed), this->end()));
      }
      // VCA_LOGAPI_VALID_REFERENCE_ARGUMENT
      result.emplace(std::move(*this->GetAt(last_idx)));
      this->Destroy(last_idx);
      this->size_ = last_idx;
    }
    return result;
  }

  /*!
   * \brief Erases an element in the vector
   * \param elem The element to erase
   * \details This changes the order of elements inside the vector
   * \return The iterator to the following element or end
   * \spec
   *   requires true;
   * \endspec
   */
  auto erase(iterator elem) noexcept -> iterator {
    iterator ret_val{this->end()};
    if (elem != ret_val) {
      ret_val = std::next(elem);
      size_type const last_idx{this->size() - 1};
      iterator e{GetAt(last_idx)};
      std::swap(elem, e);
      this->Destroy(last_idx);
      this->size_ = last_idx;
    }
    return ret_val;
  }

  /*!
   * \brief Construct an iterator to the beginning of the vector
   * \return The iterator to the beginning of the vector
   * \spec
   *   requires true;
   * \endspec
   */
  auto begin() noexcept -> iterator { return this->GetAt(0); }

  /*!
   * \brief Construct an iterator behind the end of the vector
   * \return The iterator behind the end of the vector
   * \spec
   *   requires true;
   * \endspec
   */
  auto end() noexcept -> iterator { return this->GetAt(this->size()); }

  /*!
   * \copydoc begin()
   */
  constexpr auto begin() const noexcept -> const_iterator { return this->GetAt(0); }

  /*!
   * \copydoc end()
   */
  constexpr auto end() const noexcept -> const_iterator { return this->GetAt(this->size()); }

  /*!
   * \copydoc begin()
   */
  constexpr auto cbegin() const noexcept -> const_iterator { return this->GetAt(0); }

  /*!
   * \copydoc end()
   */
  constexpr auto cend() const noexcept -> const_iterator { return this->GetAt(this->size()); }

 private:
  /*!
   * \brief Gets the item at the passed index
   * \param index The index containing the item
   * \details The passed index must be less than the current size
   * \return The item as a pointer
   */
  auto GetAt(size_type index) & noexcept -> pointer {
    return std::next(this->data(), static_cast<std::ptrdiff_t>(index));
  }

  /*!
   * \brief Gets the item at the passed index
   * \param index The index containing the item
   * \details The passed index must be less than the current size
   * \return The item as a pointer
   */
  constexpr auto GetAt(size_type index) const& noexcept -> const_pointer {
    return std::next(this->data(), static_cast<std::ptrdiff_t>(index));
  }

  // VECTOR NC AutosarC++17_10-M0.1.8: MD_LOG_AutosarC++17_10-M0.1.8_false_positive_no_sideeffect
  /*!
   * \brief Destroys the item at the passed index
   * \param index The index which contains the item to be destroyed
   * \details The passed index must be less than the current size
   */
  void Destroy(size_type index) noexcept(std::is_nothrow_destructible<Decayed>::value) { this->GetAt(index)->~T(); }

  /*!
   * \brief The backing array that contains the storage
   */
  Storage storage_{};
  /*!
   * \brief The contained size
   */
  size_type size_{0};
};
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_ARRAY_VEC_H_
