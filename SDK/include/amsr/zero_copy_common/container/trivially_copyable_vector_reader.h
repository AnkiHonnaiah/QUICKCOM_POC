/*!********************************************************************************************************************
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
/*!       \file
 *        \brief  Implementation of a read-only wrapper around a trivially copyable vector.
 *        \unit   ZeroCopyCommon::TriviallyCopyableContainer::TriviallyCopyableVector
 *
 *********************************************************************************************************************/
#ifndef LIB_TRIVIALLY_COPYABLE_CONTAINER_INCLUDE_AMSR_ZERO_COPY_COMMON_CONTAINER_TRIVIALLY_COPYABLE_VECTOR_READER_H_
#define LIB_TRIVIALLY_COPYABLE_CONTAINER_INCLUDE_AMSR_ZERO_COPY_COMMON_CONTAINER_TRIVIALLY_COPYABLE_VECTOR_READER_H_

#include <cstddef>
#include <cstring>
#include <iterator>
#include <limits>
#include <type_traits>

#include "amsr/core/abort.h"
#include "amsr/zero_copy_common/container/trivially_copyable_container_error_domain.h"
#include "amsr/zero_copy_common/container/trivially_copyable_vector.h"
#include "ara/core/result.h"

namespace amsr {
namespace zero_copy_common {
namespace container {

/*!
 * \brief   A read-only wrapper around a trivially copyable vector instance that ensures safe bounds.
 * \details A trivially copyable vector might be shared between processes of different qualification levels.
 *          This class is constructed as a wrapper around a potentially untrusted instance of a vector,
 *          i.e. it is created based on a vector instance that might contain corrupted data.
 *          The wrapper allows read-only access to this underlying instance and ensures that no out of bounds
 *          memory access happens. Concretely, the vector size is checked so no out of bounds access occurs.
 *          WARNING: A wrapper instance is tightly bound to its underlying vector. It is the users responsibility
 *                   to ensure the wrapper is not used after the underlying vector has changed/is freed.
 *
 * \tparam  T Element type stored in the vector.
 *            Must be trivially copyable and trivially destructible. Must be copy assignable and copy constructible.
 *            The trivially copyable property should be held further than C++ specifies for the Zero-Copy use-case.
 *            It should be possible to share the object over process boundaries (as this is one of the main use-cases).
 *            Therefore, no form of pointer should be used in T, as it might become invalid after performing an
 *            'inter-process mem-copy'. Note that this includes e.g. pointers to global constant memory.
 * \tparam  kMaxCapacity Maximum capacity of the vector. Must be >= 1. Must be < std::numeric_limits<size_t>::max() / 2.
 *
 * \trace   DSGN-ZeroCopyCommon-TriviallyCopyableVector-ReadOnlyWrapper
 * \vpublic
 */
// VECTOR NC Metric-OO.WMC.One: MD_ZEROCOPYCOMMON_Metric-OO.WMC.One_containersModeledAfterStandardLibrary
template <class T, size_t kMaxCapacity>
class TriviallyCopyableVectorReader final {
  // Trivially copyable elements are required for the container itself to be trivially copyable.
  // General copy properties are required to insert elements into the vector.
  static_assert(std::is_trivially_copyable<T>::value, "Contained elements must be trivially copyable.");
  static_assert(std::is_copy_assignable<T>::value, "Contained elements must be copy assignable.");
  static_assert(std::is_copy_constructible<T>::value, "Contained elements must be copy constructible.");
  // Trivially destructible elements are required for fast deletion of elements.
  static_assert(std::is_trivially_destructible<T>::value, "Contained elements must be trivially destructible.");

  // Having a data size of at least one allows simpler and more defensive implementations of accessor methods.
  static_assert(kMaxCapacity >= 1, "The capacity must be at least one, otherwise no elements can be stored.");
  // We often perform additions of two values of type size_type.
  // To avoid overflows we usually check both parameters to be <= kMaxCapacity.
  // By limiting the kMaxCapacity range, we can ensure that no overflow can occur at any point in our code.
  static_assert((std::numeric_limits<size_t>::max() / 2) >= kMaxCapacity,
                "kMaxCapacity must be small enough to avoid integer overflows.");

 public:
  /*!
   * \brief Typedef for the vector size.
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  using size_type = typename TriviallyCopyableVector<T, kMaxCapacity>::size_type;

  /*!
   * \brief Typedef for the contained element type.
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  using value_type = typename TriviallyCopyableVector<T, kMaxCapacity>::value_type;

  /*!
   * \brief Typedef for constant references to contained elements.
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  using const_reference = typename TriviallyCopyableVector<T, kMaxCapacity>::const_reference;

  /*!
   * \brief Typedef for a constant pointer to contained elements.
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  using const_pointer = typename TriviallyCopyableVector<T, kMaxCapacity>::const_pointer;

  /*!
   * \brief Typedef for const iterators.
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  using const_iterator = typename TriviallyCopyableVector<T, kMaxCapacity>::const_iterator;

  /*!
   * \brief Typedef for reverse const iterators.
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  using const_reverse_iterator = typename TriviallyCopyableVector<T, kMaxCapacity>::const_reverse_iterator;

  /*!
   * \brief Create a safe read-only wrapper around the given vector instance.
   * \param[in] vector The vector being wrapped. The given vector must outlive the created read-only wrapper.
   * \return Safe wrapper instance around the given vector.
   * \error TriviallyCopyableContainerErrc::kContainerCorrupted The given vector is corrupted,
   *        i.e. its size() > kMaxCapacity, which could lead to invalid memory accesses.
   * \context ANY
   * \pre -
   * \threadsafe TRUE, if the given vector object instance is used exclusively by this thread.
   * \reentrant TRUE, if different vector object instances are used for each call.
   * \synchronous TRUE
   * \vpublic
   */
  static auto FromTriviallyCopyableVector(TriviallyCopyableVector<T, kMaxCapacity> const& vector)
      -> ara::core::Result<TriviallyCopyableVectorReader> {
    using Result = ara::core::Result<TriviallyCopyableVectorReader<T, kMaxCapacity>>;
    Result result{TriviallyCopyableContainerErrc::kDefaultError};

    size_type const size{vector.ManagementData()};
    amsr::core::Array<T, kMaxCapacity> const& elements{vector.ElementData()};

    if (size <= kMaxCapacity) {
      result.EmplaceValue(TriviallyCopyableVectorReader(elements, size));
    } else {
      result.EmplaceError(TriviallyCopyableContainerErrc::kContainerCorrupted);
    }

    return result;
  }

  // Default constructor deleted, since member elements_ must always be valid to avoid 'dangling' wrappers.
  TriviallyCopyableVectorReader() noexcept = delete;

  /*!
   * \brief Create a copy of another reader-wrapper.
   * \details This will leave two intact references to the original vector.
   *          Be careful that the wrapped vector outlives the wrappers instances.
   * \param[in] other The wrapper to be copied.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  TriviallyCopyableVectorReader(TriviallyCopyableVectorReader const& other) noexcept = default;

  /*!
   * \brief Move the given other vector into a new reader instance.
   * \details This will leave two intact references to the original vector.
   *          Be careful that the wrapped vector outlives the wrappers instances.
   * \param[in] other The wrapper to be copied.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  TriviallyCopyableVectorReader(TriviallyCopyableVectorReader&& other) noexcept = default;

  // Assignment operators deleted, since a read-wrapper instance shall always belong to the vector it originated from.
  // This helps to avoid programming mistakes where lifetimes are violated.
  auto operator=(TriviallyCopyableVectorReader const& other) noexcept -> TriviallyCopyableVectorReader& = delete;
  auto operator=(TriviallyCopyableVectorReader&& other) noexcept -> TriviallyCopyableVectorReader& = delete;

  /*!
   * \brief Destruct the reader wrapper.
   */
  ~TriviallyCopyableVectorReader() noexcept = default;

  /*!
   * \brief Return the element at the given position with bounds checking.
   * \param[in] pos The position of the element to return. Must be <= size().
   * \return Constant reference to the requested element.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto at(size_type pos) const noexcept -> const_reference {
    AssertIndexWithinSize(pos);

    //@ assert (pos >= 0) && (pos < kMaxCapacity);
    // VECTOR NL AutosarC++17_10-M5.0.16: MD_ZEROCOPYCOMMON_M5.0.16_vectorDataArrayAccessCheckedByVCA
    return GetElements()[pos];  // VCA_ZEROCOPYCOMMON_LNG_02_GETELEMENTS_REFERENCE_VALID
  }

  /*!
   * \brief Return the element at the given position without bounds checking.
   * \details Accessing elements out of bounds (pos >= size()) results in undefined behavior.
   * \param[in] pos The position of the element to return.
   * \return Constant reference to the requested element.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto operator[](size_type pos) const noexcept -> const_reference { return GetElements()[pos]; }

  /*!
   * \brief Return a reference to the first element in the vector.
   * \return Constant reference to first element in the vector.
   * \context ANY
   * \pre Vector is not empty.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto front() const noexcept -> const_reference {
    static_cast<void>(AssertVectorNotEmptyAndGetSize());

    return GetElements()[0];  // VCA_ZEROCOPYCOMMON_LNG_02_GETELEMENTS_REFERENCE_VALID
  }

  /*!
   * \brief Return a reference to the last element in the vector.
   * \return Constant reference to last element in the vector.
   * \context ANY
   * \pre Vector is not empty.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto back() const noexcept -> const_reference {
    size_type const current_size{AssertVectorNotEmptyAndGetSize()};

    return GetElements()[current_size - 1];  // VCA_ZEROCOPYCOMMON_LNG_02_GETELEMENTS_REFERENCE_VALID
  }

  /*!
   * \brief Provide access to the contiguous array where elements are stored.
   * \return Constant pointer to the contiguous array where elements are stored.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto data() const noexcept -> const_pointer { return GetElements().data(); }

  /*!
   * \brief Return an iterator to the first element of the vector.
   * \return Const iterator to the first element.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto begin() const noexcept -> const_iterator { return &(GetElements()[0]); }

  /*!
   * \brief Return a const iterator to the first element of the vector.
   * \return Const iterator to the first element.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto cbegin() const noexcept -> const_iterator { return &(GetElements()[0]); }

  /*!
   * \brief Return an iterator to the element following the last element of the vector.
   * \return Const iterator to one past the last element.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto end() const noexcept -> const_iterator { return &(GetElements()[GetSize()]); }

  /*!
   * \brief Return a const iterator to the element following the last element of the vector.
   * \return Const iterator to one past the last element.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto cend() const noexcept -> const_iterator { return &(GetElements()[GetSize()]); }

  /*!
   * \brief Return a reverse iterator to the first element of the reversed vector.
   * \return Const reverse iterator to the first element in the reversed vector.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto rbegin() const noexcept -> const_reverse_iterator { return const_reverse_iterator{end()}; }

  /*!
   * \brief Return a constant reverse iterator to the first element of the reversed vector.
   * \return Const reverse iterator to the first element in the reversed vector.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto crbegin() const noexcept -> const_reverse_iterator { return const_reverse_iterator{cend()}; }

  /*!
   * \brief Return a reverse iterator to the element following the last element of the reversed vector.
   * \return Const reverse iterator one past the last element in the reversed vector.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto rend() const noexcept -> const_reverse_iterator { return const_reverse_iterator{begin()}; }

  /*!
   * \brief Return a const reverse iterator to the element following the last element of the reversed vector.
   * \return Const reverse iterator one past the last element in the reversed vector.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto crend() const noexcept -> const_reverse_iterator { return const_reverse_iterator{cbegin()}; }

  /*!
   * \brief Check if the vector contains no elements.
   * \return True if the vector is empty.
   * \return False otherwise.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto empty() const noexcept -> bool { return GetSize() == 0; }

  /*!
   * \brief Return the current number of elements in the vector.
   * \return Current vector size.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto size() const noexcept -> size_type { return GetSize(); }

  /*!
   * \brief Return the maximum number of elements this specific vector type can hold.
   * \details For a specialization of the vector (with concrete template parameters), the maximal technical
   *          size that can be held is its kMaxCapacity template parameter. This is because the memory region can
   *          never be extended beyond that.
   *          Another specialization (with different kMaxCapacity template parameter) will have a different limit.
   * \returns The static maximum number of elements this vector type can hold, i.e. returns kMaxCapacity.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  static constexpr auto max_size() noexcept -> size_type { return kMaxCapacity; }

  /*!
   * \brief Return the number of elements the vector has allocated memory for in its fixed-size internal buffer.
   * \details As this vector never re-allocates memory, this method always returns the same as max_size().
   * \return The static number of elements this vector has allocated memory for, i. e. returns kMaxCapacity.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  static constexpr auto capacity() noexcept -> size_type { return kMaxCapacity; }

 private:
  /*!
   * \brief Pointer referencing the data of the original vector.
   */
  amsr::core::Array<T, kMaxCapacity> const& elements_;

  /*!
   * \brief Current size of the read-wrapper (checked at creation).
   */
  size_type const size_;

  /*!
   * \brief Construct a vector with the given members.
   */
  TriviallyCopyableVectorReader(amsr::core::Array<T, kMaxCapacity> const& elements, size_type size)
      : elements_{elements}, size_{size} {
    //@ assert size <= kMaxCapacity;
  }

  /*!
   * \brief Get the current size_ of the vector.
   * \return Current size_ of the vector.
   * \internal
   * Guarantees the VCA contract GetSize() <= max_size().
   * \endinternal
   */
  auto GetSize() const noexcept -> size_type {
    size_type const result{size_};
    // We know that this holds, as we only set it in the constructor - which is checked by VCA.

    // VCA_ZEROCOPYCOMMON_ANO_01_VECTOR_READER_SIZE_MEMBER_HOLDS_INVARIANT
    //@ assert result <= kMaxCapacity;
    return result;
  }

  /*!
   * \brief Get the current elements_ reference of the vector.
   * \return Reference to elements_ of the vector.
   * \internal
   * Guarantees the VCA contract that the reference is valid.
   * \endinternal
   */
  auto GetElements() const noexcept -> amsr::core::Array<T, kMaxCapacity> const& {
    amsr::core::Array<T, kMaxCapacity> const& elements_local{elements_};

    // VCA_ZEROCOPYCOMMON_ANO_01_VECTOR_READER_REFERENCES_VALID_ELEMENT_ARRAY
    return elements_local;
  }

  /*!
   * \brief Abort if the vector is empty, returns its current size otherwise.
   * \return The size of the non-empty vector.
   * \context ANY
   * \pre -
   */
  auto AssertVectorNotEmptyAndGetSize() const noexcept -> size_type {
    size_type const current_size{GetSize()};
    if (current_size == 0) {
      amsr::core::Abort("Vector is empty, which would cause undefined behavior for this function call.");
    }

    return current_size;
  }

  /*!
   * \brief Abort if the index is out of bounds.
   * \param[in] index Index to be checked.
   * \context ANY
   * \pre -
   */
  // VECTOR NL AutosarC++17_10-M0.1.8: MD_ZEROCOPYCOMMON_M0.1.8_noSideEffectInAssertFunction
  void AssertIndexWithinSize(size_type index) const noexcept {
    if ((index >= kMaxCapacity) || (index >= GetSize())) {
      amsr::core::Abort("Accessed index is out of the vector's bounds.");
    }
  }
};

/*!
 * \brief Compare two trivially copyable vector read wrappers element-wise.
 * \tparam T Type stored in both vectors.
 * \tparam kSizeLeft Maximum size of the left vector.
 * \tparam kSizeRight Maximum size of the right vector.
 * \param[in] lhs Left vector reader instance.
 * \param[in] rhs Right vector reader instance.
 * \return True, if lhs is element-wise equal to rhs.
 * \return False, otherwise.
 * \context ANY
 * \pre -
 * \threadsafe TRUE, if lhs and rhs object instances are used exclusively by this thread during the call.
 * \reentrant TRUE, if different lhs and rhs object instances are used for each call.
 * \synchronous TRUE
 * \vpublic
 */
template <typename T, size_t kSizeLeft, size_t kSizeRight>
auto operator==(TriviallyCopyableVectorReader<T, kSizeLeft> const& lhs,
                TriviallyCopyableVectorReader<T, kSizeRight> const& rhs) noexcept -> bool {
  bool result{};
  if (lhs.size() != rhs.size()) {
    result = false;
  } else if (lhs.data() == rhs.data()) {
    result = true;
  } else {
    result = true;
    for (std::size_t i{0}; i < lhs.size(); ++i) {
      if (lhs[i] != rhs[i]) {
        result = false;
        break;
      }
    }
  }

  return result;
}

/*!
 * \brief Create a safe read-only wrapper around the given vector.
 * \tparam T Type stored in the vector.
 * \tparam kMaximumSize Maximum size of the vector.
 * \param[in] vector The vector being wrapped. The given vector must outlive the created read-only wrapper.
 * \context ANY
 * \return Safe wrapper instance around the given vector.
 * \error TriviallyCopyableContainerErrc::kContainerCorrupted The given vector is corrupted,
 *        i.e. its size() > kMaxCapacity, which could lead to invalid memory accesses.
 * \pre -
 * \threadsafe TRUE, if the given vector object instance is used exclusively by this thread.
 * \reentrant TRUE, if different vector object instances are used for each call.
 * \synchronous TRUE
 * \vpublic
 */
template <typename T, size_t kMaximumSize>
auto ReadOnlyWrapper(TriviallyCopyableVector<T, kMaximumSize> const& vector) noexcept
    -> ara::core::Result<TriviallyCopyableVectorReader<T, kMaximumSize>> {
  return TriviallyCopyableVectorReader<T, kMaximumSize>::FromTriviallyCopyableVector(vector);
}

}  // namespace container
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_TRIVIALLY_COPYABLE_CONTAINER_INCLUDE_AMSR_ZERO_COPY_COMMON_CONTAINER_TRIVIALLY_COPYABLE_VECTOR_READER_H_
