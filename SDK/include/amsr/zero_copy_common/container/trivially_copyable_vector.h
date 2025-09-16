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
/*!        \file
 *        \brief Implementation of a trivially copyable vector.
 *         \unit ZeroCopyCommon::TriviallyCopyableContainer::TriviallyCopyableVector
 *   \complexity The high complexity of the class TriviallyCopyableVector from the equally named unit is caused by a
 *               high max call-tree complexity and max level weighted complexity which is caused by inserting/emplacing
 *               elements to the TriviallyCopyableVector. These operations require many different steps to be done and
 *               the API contract follows the standard library definition for the std::vector.
 *               In summary, the design is already chosen to not duplicate any functional code parts, but preferring to
 *               use existing functionality from other APIs.
 *               Therefore, the risk is considered low.
 *
 *
 *********************************************************************************************************************/
#ifndef LIB_TRIVIALLY_COPYABLE_CONTAINER_INCLUDE_AMSR_ZERO_COPY_COMMON_CONTAINER_TRIVIALLY_COPYABLE_VECTOR_H_
#define LIB_TRIVIALLY_COPYABLE_CONTAINER_INCLUDE_AMSR_ZERO_COPY_COMMON_CONTAINER_TRIVIALLY_COPYABLE_VECTOR_H_

#include <cstddef>
#include <cstring>
#include <iterator>
#include <limits>
#include <type_traits>

#include "amsr/core/abort.h"
#include "amsr/core/array.h"

namespace amsr {
namespace zero_copy_common {
namespace container {

/*!
 * \brief   A vector that manages elements in contiguous memory and that is trivially copyable.
 * \details Contiguous memory is allocated by statically configuring the vector size using template parameters.
 *          All memory is contained within the trivially copyable vector object using an array with static size.
 *          Elements can be added and removed within the statically configured maximum capacity.
 *          As far as sensible APIs are modeled after std::vector.
 *          If the static maximum_capacity is exceeded, program execution is aborted. See (parameter) pre-conditions.
 *          Exception safety of most methods is given by the trivially copyable property of T (copying, moving and
 *          deconstructing values will never throw an exception, therefore methods will not abort due to exceptions).
 *          Methods that might terminate because of T throwing exceptions are documented.
 *
 * \tparam  T Element type stored in the vector.
 *            Must be trivially copyable and trivially destructible. Must be copy assignable and copy constructible.
 *            The trivially copyable property should be held further than C++ specifies for the Zero-Copy use-case.
 *            It should be possible to share the object over process boundaries (as this is one of the main use-cases).
 *            Therefore, no form of pointer should be used in T, as it might become invalid after performing an
 *            'inter-process mem-copy'. Note that this includes e.g. pointers to global constant memory.
 * \tparam  kMaxCapacity Maximum capacity of the vector. Must be >= 1. Must be < std::numeric_limits<size_t>::max() / 2.
 *
 * \trace   DSGN-ZeroCopyCommon-TriviallyCopyableVector
 * \vpublic
 */
// VECTOR NC Metric-OO.WMC.One: MD_ZEROCOPYCOMMON_Metric-OO.WMC.One_containersModeledAfterStandardLibrary
template <class T, std::size_t kMaxCapacity>
class TriviallyCopyableVector final {
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

  /*!
   * \brief Typedef for a typetrait checking if the given Iterator type is an input iterator.
   * \tparam Iterator Type to be checked to be an input iterator.
   */
  template <typename Iterator>
  using IsInputIterator =
      std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>;

 public:
  /*!
   * \brief Typedef for the vector size.
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  using size_type = std::size_t;

  /*!
   * \brief Typedef for the contained element type.
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  using value_type = T;

  /*!
   * \brief Typedef for references to the contained elements.
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  using reference = value_type&;

  /*!
   * \brief Typedef for constant references to contained elements.
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  using const_reference = value_type const&;

  /*!
   * \brief Typedef for a pointer to contained elements.
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  using pointer = value_type*;

  /*!
   * \brief Typedef for a constant pointer to contained elements.
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  using const_pointer = value_type const*;

  /*!
   * \brief Typedef for iterators.
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  using iterator = pointer;

  /*!
   * \brief Typedef for const iterators.
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  using const_iterator = const_pointer;

  /*!
   * \brief Typedef for reverse iterators.
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  using reverse_iterator = std::reverse_iterator<iterator>;

  /*!
   * \brief Typedef for reverse const iterators.
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  /*!
   * \brief Construct an empty vector.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   *
   * \vpublic
   */
  TriviallyCopyableVector() noexcept = default;

  /*!
   * \brief Construct a vector with count copies of the given value.
   * \param[in] count The initial element count of the vector. Must be <= kMaxCapacity.
   * \param[in] value The default value to be copied into the initial elements.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  TriviallyCopyableVector(size_type count, value_type const& value) noexcept { assign(count, value); }

  /*!
   * \brief Construct a vector with copies of the given iterator range.
   * \details The copied range of elements is [first, last), i.e. the last element is not copied.
   *
   * \tparam InputIt Iterator type of the range.
   * \param[in] first Iterator of the first value to be copied into the vector.
   * \param[in] last Iterator of the last value (exclusive) to be copied into the vector.
   * \context ANY
   * \pre The given range [first, last) must be a valid iterator range created from the same object.
   *      The caller is responsible to hold this contract, as validity of iterators can not be fully checked.
   *      Violating this precondition will result in undefined behavior.
   * \pre The number of elements in the given range [first, last) must be <= kMaxCapacity of the vector.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  template <class InputIt, std::enable_if_t<IsInputIterator<InputIt>::value, bool> = true>
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  TriviallyCopyableVector(InputIt first, InputIt last) noexcept {
    assign(first, last);
  }

  /*!
   * \brief Construct a copy of another vector of same maximum size.
   * \param[in] other Vector to be copied.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   *
   * \internal
   * To keep the type trivially copyable, a default copy constructor is required.
   * This default constructor must take another object of exact same type as argument,
   * i. e. its argument other must have the same type T and the same size limit kMaxCapacity.
   * This is in contrast to the overloaded function with parameter of any size.
   * \endinternal
   */
  TriviallyCopyableVector(TriviallyCopyableVector const& other) noexcept = default;

  /*!
   * \brief Construct a copy of another vector.
   * \param[in] other Vector to be copied. Size of other must be <= kMaxCapacity of the constructed vector.
   * \tparam kOtherSize Static maximum size of the vector to be copied.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  template <size_type kOtherSize>
  explicit TriviallyCopyableVector(TriviallyCopyableVector<T, kOtherSize> const& other) noexcept {
    Copy(other);
  }

  /*!
   * \brief Move a vector into a new vector of same size (same as copy constructor).
   * \param[in] other Vector to be copied.
   * \pre -
   * \context ANY
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   *
   * \internal
   * To keep the type trivially copyable, a default move constructor is required.
   * This default constructor must take another object of exact same type as argument,
   * i. e. its argument other must have the same type T and the same size limit kMaxCapacity.
   * This is in contrast to the overloaded function with parameter of any size.
   * \endinternal
   */
  TriviallyCopyableVector(TriviallyCopyableVector<T, kMaxCapacity>&& other) noexcept = default;

  /*!
   * \brief Move a vector into another vector (same as copy constructor).
   * \tparam kOtherSize Static maximum size of the vector to be copied.
   * \param[in] other Vector to be moved from. Size of other must be <= kMaxCapacity of the constructed vector.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  template <size_type kOtherSize>
  explicit TriviallyCopyableVector(TriviallyCopyableVector<T, kOtherSize>&& other) noexcept {
    Copy(std::move(other));
  }

  /*!
   * \brief Default destructor does nothing.
   * \vpublic
   */
  ~TriviallyCopyableVector() noexcept = default;

  /*!
   * \brief Replace the vector contents with a copy of another vector of same size.
   * \param[in] other Vector to be copied from.
   * \return Reference to the assigned-to vector.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   *
   * \internal
   * To keep the type trivially copyable, a default copy assignment is required.
   * This default assignment must take another object of exact same type as argument,
   * i. e. its argument other must have the same type T and the same size limit kMaxCapacity.
   * This is in contrast to the overloaded function with parameter of any size.
   * \endinternal
   */
  auto operator=(TriviallyCopyableVector<T, kMaxCapacity> const& other) & noexcept
      -> TriviallyCopyableVector<T, kMaxCapacity>& = default;

  /*!
   * \brief Replace the vector contents with a copy of another vector.
   * \param[in] other The vector to be copied from. other.size() must be <= max_size() of the target vector.
   * \return Reference to the assigned-to vector.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  template <size_type kOtherSize>
  auto operator=(TriviallyCopyableVector<T, kOtherSize> const& other) & noexcept
      -> TriviallyCopyableVector<T, kMaxCapacity>& {
    Copy(other);
    return *this;
  }

  /*!
   * \brief Replace the vector contents by moving the elements of another vector of same size.
   * \param[in] other Vector to be moved from.
   * \return Reference to the assigned-to vector.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   *
   * \internal
   * To keep the type trivially copyable, a default move assignment is required.
   * This default assignment must take another object of exact same type as argument,
   * i. e. its argument other must have the same type T and the same size limit max_capacity.
   * This is in contrast to the overloaded function with parameter of any size.
   * \endinternal
   */
  auto operator=(TriviallyCopyableVector<T, kMaxCapacity>&& other) & noexcept
      -> TriviallyCopyableVector<T, kMaxCapacity>& = default;

  /*!
   * \brief Replace the vector contents by moving the elements of another vector.
   * \tparam kOtherSize Static maximum size of the vector to be copied.
   * \param[in] other The vector to be moved from. other.size() must be <= max_size() of the target vector.
   * \return Reference to the assigned-to vector.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  template <size_type kOtherSize>
  auto operator=(TriviallyCopyableVector<T, kOtherSize>&& other) & noexcept
      -> TriviallyCopyableVector<T, kMaxCapacity>& {
    Copy(std::move(other));
    return *this;
  }

  /*!
   * \brief Replace the vector contents with copies of the given initializer list.
   * \param[in] elements The elements to be copied. elements.size() must be <= max_size() of the target vector.
   * \return Reference to the assigned-to vector.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto operator=(std::initializer_list<T> elements) & noexcept -> TriviallyCopyableVector<T, kMaxCapacity>& {
    assign(elements.begin(), elements.end());
    return *this;
  }

  /*!
   * \brief Replace the contents of the vector with count copies of the given value.
   * \param[in] count Count of elements to be assigned to the vector. Must be <= max_size() of the vector.
   * \param[in] value The value copied into the vector.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  void assign(size_type count, value_type const& value) noexcept {
    AssertSizeWithinCapacity(count);

    SetSize(count);
    for (size_type i{0}; i < count; ++i) {
      elements_.at(i) = value;
    }
  }

  /*!
   * \brief Replace the contents of the vector with copies of the given iterator range.
   * \details The copied range of elements is [first, last), i.e. the last element is not copied.
   *
   * \tparam InputIt Iterator type of the range.
   * \param[in] first Iterator of the first value to be copied into the vector.
   * \param[in] last Iterator of the last value (exclusive) to be copied into the vector.
   * \context ANY
   * \pre The given range [first, last) must be a valid iterator range created from the same object.
   *      The caller is responsible to hold this contract, as validity of iterators can not be fully checked.
   *      Violating this precondition will result in undefined behavior.
   * \pre The given range [first, last) must not reference this vector instance.
   * \pre The number of elements in the given range [first, last) must be <= max_size() of the vector.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   *
   * \internal
   * - Check plausibility of all input parameters.
   * - Copy iterator elements into vector.
   * - Set vector size to the number of inserted elements.
   * \endinternal
   */
  template <class InputIt, std::enable_if_t<IsInputIterator<InputIt>::value, bool> = true>
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  void assign(InputIt first, InputIt last) noexcept {
    size_type const target_size{
        AssertIteratorRangePositiveDistance(first, last)};  // VCA_ZEROCOPYCOMMON_LNG_01_VECTOR_VALID_ARGUMENT
    AssertIteratorRangeNotFromThisVector(first, last);
    AssertSizeWithinCapacity(target_size);

    InputIt current_iterator{first};
    for (size_type current_index{0}; current_index < target_size; ++current_index) {
      T const value{*current_iterator};
      //@ assert (current_index >= 0) && (current_index < kMaxCapacity);
      // VECTOR NL AutosarC++17_10-M5.0.16: MD_ZEROCOPYCOMMON_M5.0.16_vectorDataArrayAccessCheckedByVCA
      elements_[current_index] = value;
      // VECTOR NC AutosarC++17_10-M5.0.16: MD_ZEROCOPYCOMMON_M5.0.16_vectorPointerArithmeticOnIterators
      ++current_iterator;
    }
    SetSize(target_size);
  }

  /*!
   * \brief Replace the contents of the vector with copies of the given initializer list.
   * \param[in] elements The elements to be copied. elements.size() must be <= max_size() of the target vector.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  void assign(std::initializer_list<T> elements) noexcept { assign(elements.begin(), elements.end()); }

  /*!
   * \brief Return the element at the given position with bounds checking.
   * \param[in] pos The position of the element to return. Must be <= size().
   * \return Reference to the requested element.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto at(size_type pos) noexcept -> reference {
    AssertIndexWithinSize(pos);

    return elements_[pos];
  }

  /*!
   * \brief Return the element at the given position with bounds checking (const version).
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

    return elements_[pos];
  }

  /*!
   * \brief Return the element at the given position without bounds checking.
   * \details Accessing elements out of bounds (pos >= size()) results in undefined behavior.
   * \param[in] pos The position of the element to return.
   * \return Reference to the requested element.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto operator[](size_type pos) noexcept -> reference {
    // VECTOR NL AutosarC++17_10-M5.0.16: MD_ZEROCOPYCOMMON_M5.0.16_vectorUncheckedUserElementAccess
    return elements_[pos];
  }

  /*!
   * \brief Return the element at the given position without bounds checking (const version).
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
  auto operator[](size_type pos) const noexcept -> const_reference { return elements_[pos]; }

  /*!
   * \brief Return a reference to the first element in the vector.
   * \return Reference to first element in the vector.
   * \context ANY
   * \pre Vector is not empty.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto front() noexcept -> reference {
    size_type const current_size{AssertVectorNotEmptyAndGetSize()};
    static_cast<void>(current_size);
    return elements_[0];
  }

  /*!
   * \brief Return a reference to the first element in the vector (const version).
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
    size_type const current_size{AssertVectorNotEmptyAndGetSize()};
    static_cast<void>(current_size);
    return elements_[0];
  }

  /*!
   * \brief Return a reference to the last element in the vector.
   * \return Reference to first element in the vector.
   * \context ANY
   * \pre Vector is not empty.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto back() noexcept -> reference {
    size_type const current_size{AssertVectorNotEmptyAndGetSize()};
    return elements_[current_size - 1];
  }

  /*!
   * \brief Return a reference to the last element in the vector (const version).
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
  auto back() const noexcept -> const_reference {
    size_type const current_size{AssertVectorNotEmptyAndGetSize()};

    size_type const index{current_size - 1};
    //@ assert index >= 0 && index < kMaxCapacity;
    // VECTOR NL AutosarC++17_10-M5.0.16: MD_ZEROCOPYCOMMON_M5.0.16_vectorDataArrayAccessCheckedByVCA
    return elements_[index];
  }

  /*!
   * \brief Provide access to the contiguous array where elements are stored.
   * \return Pointer to the contiguous array where elements are stored.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto data() noexcept -> pointer { return elements_.data(); }

  /*!
   * \brief Provide access to the contiguous array where elements are stored (const version).
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
  auto data() const noexcept -> const_pointer { return elements_.data(); }

  /*!
   * \brief Return an iterator to the first element of the vector.
   * \return Iterator to the first element.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto begin() noexcept -> iterator { return iterator{&(elements_[0])}; }

  /*!
   * \brief Return an iterator to the first element of the vector (const version).
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
  auto begin() const noexcept -> const_iterator { return const_iterator{&(elements_[0])}; }

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
  auto cbegin() const noexcept -> const_iterator { return const_iterator{&(elements_[0])}; }

  /*!
   * \brief Return an iterator to the element following the last element of the vector.
   * \return Iterator to one past the last element.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto end() noexcept -> iterator { return iterator{&(elements_[GetSize()])}; }

  /*!
   * \brief Return an iterator to the element following the last element of the vector (const version).
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
  auto end() const noexcept -> const_iterator { return const_iterator{&(elements_[GetSize()])}; }

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
  auto cend() const noexcept -> const_iterator { return const_iterator{&(elements_[GetSize()])}; }

  /*!
   * \brief Return a reverse iterator to the first element of the reversed vector.
   * \return Reverse iterator to the first element in the reversed vector.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto rbegin() noexcept -> reverse_iterator { return reverse_iterator{end()}; }

  /*!
   * \brief Return a reverse iterator to the first element of the reversed vector (const version).
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
   * \return Reverse iterator one past the last element in the reversed vector.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto rend() noexcept -> reverse_iterator { return reverse_iterator{begin()}; }

  /*!
   * \brief Return a reverse iterator to the element following the last element of the reversed vector
   *        (const version).
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
   * \return The static maximum number of elements this vector type can hold, i.e. return kMaxCapacity.
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

  // The reserve method is not provided as the memory region has static size.

  /*!
   * \brief Return the number of elements the vector has allocated memory for in its fixed-size internal buffer.
   * \details As this vector never re-allocates memory, this method always returns the same as max_size().
   * \return The static number of elements this vector has allocated memory for, i. e. return kMaxCapacity.
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

  // The shrink_to_fit method is not provided as the memory region has static size.

  /*!
   * \brief Delete all elements from the vector.
   * \details The size will be 0 after this operation.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   *
   * \internal
   * As elements T are trivially destructible there is no need to call destructors, therefore,
   * this method will never throw or abort program execution.
   * \endinternal
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  void clear() noexcept { SetSize(0); }

  /*!
   * \brief Insert a copy of the given value before pos.
   * \param[in] pos Position before which the value should be inserted. Must be a valid iterator into this vector.
   * \param[in] value Value to be copied into the vector.
   * \return Iterator pointing to the inserted element.
   * \context ANY
   * \pre The vector must not be full, i.e. size() < max_size().
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto insert(const_iterator pos, value_type const& value) noexcept -> iterator { return insert(pos, 1, value); }

  /*!
   * \brief Insert count copies of the given value before pos.
   * \param[in] pos Position before which the values should be inserted. Must be a valid iterator into this vector.
   * \param[in] count Number of elements to be inserted into the vector.
   *              Count must be smaller than the remaining vector size, i.e. count <= (max_size() - size()).
   * \param[in] value Value to be copied into the vector.
   * \return Iterator pointing to the first inserted element.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   *
   * \internal
   * - Check plausibility of all input parameters.
   * - Shift existing elements count slots to the right.
   * - Insert count copies of value into now free slots.
   * - Update size of the vector.
   * - Return iterator pointing to the first inserted position (same as given pos).
   * \endinternal
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto insert(const_iterator pos, size_type count, value_type const& value) noexcept -> iterator {
    AssertSizeWithinCapacity(count);  // Prevents possible integer overflow in next line.
    size_type const target_size{GetSize() + count};
    AssertSizeWithinCapacity(target_size);
    size_type const first_insert_index{AssertValidIteratorIntoThisAndGetIndex(pos)};
    size_type const last_insert_index{first_insert_index + count};

    ShiftElementsRight(first_insert_index, count);

    for (size_type current_index{first_insert_index}; current_index < last_insert_index; ++current_index) {
      elements_[current_index] = value;
    }

    SetSize(target_size);

    return IteratorForIndex(first_insert_index);
  }

  /*!
   * \brief Insert copies of the given iterator range [first, last) into the vector.
   * \details The copied range of elements is [first, last), i.e. the last element is not copied.
   *
   * \tparam InputIt Iterator type of the inserted elements.
   * \param[in] pos Position before which the values should be inserted. Must be a valid iterator into this vector.
   * \param[in] first First element of the range of copied elements (inclusive).
   * \param[in] last Last element of the range of copied elements (exclusive).
   * \return Iterator pointing to the first inserted element.
   *
   * \pre The given range [first, last) must be a valid iterator range created from the same object.
   *      The caller is responsible to hold this contract, as validity of iterators can not be checked.
   *      Violating this precondition will result in undefined behavior.
   * \pre The given range [first, last) must not reference this vector instance.
   * \pre The remaining size (max_size() - size()) of the vector must be greater than the length of the iterator range.
   * \context ANY
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   *
   * \internal
   * - Check plausibility of all input parameters.
   * - Shift existing elements count slots to the right.
   * - Insert copies of the values in the iterator-range into now free slots.
   * - Update size of the vector.
   * - Return iterator pointing to the first inserted position (same as given pos).
   * \endinternal
   */
  template <class InputIt>
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto insert(const_iterator pos, InputIt first, InputIt last) noexcept
      -> std::enable_if_t<IsInputIterator<InputIt>::value, iterator> {
    size_type const count{
        AssertIteratorRangePositiveDistance(first, last)};  // VCA_ZEROCOPYCOMMON_LNG_01_VECTOR_VALID_ARGUMENT
    AssertIteratorRangeNotFromThisVector(first, last);
    AssertSizeWithinCapacity(count);  // Prevents possible integer overflow in next line.
    size_type const target_size{GetSize() + count};
    AssertSizeWithinCapacity(target_size);
    size_type const first_insert_index{AssertValidIteratorIntoThisAndGetIndex(pos)};
    size_type const last_insert_index{first_insert_index + count};

    ShiftElementsRight(first_insert_index, count);

    InputIt current_input_element{first};
    for (size_type current_index{first_insert_index}; current_index < last_insert_index; ++current_index) {
      elements_[current_index] = *current_input_element;
      // VECTOR NL AutosarC++17_10-M5.0.16: MD_ZEROCOPYCOMMON_M5.0.16_vectorPointerArithmeticOnIterators
      ++current_input_element;
    }

    SetSize(target_size);

    return IteratorForIndex(first_insert_index);
  }

  /*!
   * \brief Insert a copy of the given values before pos.
   * \param[in] pos Position before which the values should be inserted. Must be a valid iterator into this vector.
   * \param[in] values Initializer list of values to be copied into the vector.
   *               The list length must be smaller than the remaining vector size, i.e. length <= (max_size() - size()).
   * \return Iterator pointing to the first inserted element.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto insert(const_iterator pos, std::initializer_list<T> values) noexcept -> iterator {
    return insert(pos, values.begin(), values.end());
  }

  /*!
   * \brief Insert a new element to the vector before pos by constructing it in place.
   * \details If the constructor of T raises an exception, this method aborts execution.
   * \tparam Args The types of the arguments forwarded to the constructor of T.
   * \param[in] pos Position before which the values should be inserted. Must be a valid iterator into this vector.
   * \param[in] args The arguments forwarded to the constructor of T.
   * \return Iterator pointing to the inserted element.
   * \context ANY
   * \pre The vector must not be full, i.e. size() < max_size().
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   *
   * \internal
   * - Check plausibility of all input parameters.
   * - Shift existing elements one slot to the right.
   * - Placement new with forwarded constructor arguments in target slot.
   * - Increase size of the vector by one.
   * - Return iterator pointing to the first inserted position (same as given pos).
   * \endinternal
   */
  template <class... Args>
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto emplace(const_iterator pos, Args&&... args) noexcept -> iterator {
    size_type const target_size{GetSize() + 1};
    size_type const pos_index{AssertValidIteratorIntoThisAndGetIndex(pos)};
    AssertSizeWithinCapacity(target_size);

    ShiftElementsRight(pos_index, 1);

    // Perform placement new into the existing memory slot.
    // We are not interested in the returned pointer, as the element is accessible through the elements_ array.
    pointer const pos_pointer{&elements_[pos_index]};
    static_cast<void>(new (pos_pointer) T(std::forward<Args>(args)...));

    SetSize(target_size);

    return IteratorForIndex(pos_index);
  }

  /*!
   * \brief Erase the element at the given iterator position from the vector.
   * \param[in] pos Position of the element to be removed.
   *                Must be a valid iterator pointing to an element in the vector.
   * \return Iterator following the erased element.
   * \return The updated end() iterator if the following element is at/beyond the end.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto erase(const_iterator pos) noexcept -> iterator {
    const_iterator const first_pos{pos};
    const_iterator const last_pos{++pos};
    return erase(first_pos, last_pos);
  }

  /*!
   * \brief Erase the given iterator range [first, last) from the vector.
   * \param[in] first First iterator of the erased range. Must be a valid iterator into the vector.
   * \param[in] last Last iterator of the erased range. Must be a valid iterator into the vector.
   * \return Iterator following the erased elements.
   * \return Iterator end() if last == end.
   * \return Iterator end() if the range is empty.
   * \context ANY
   * \pre Last must be after first, i.e. last >= first, making [first, last) a valid iterator range into this vector.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   *
   * \internal
   * - Check plausibility of all input parameters.
   * - If range is empty:
   *   - Return trivial end() result.
   * - Otherwise:
   *   - Shift remaining vector elements to the left over deleted range.
   *   - Adjust vector size.
   *   - Return iterator pointing to the first left-shifted element.
   * \endinternal
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  auto erase(const_iterator first, const_iterator last) -> iterator {
    size_type const first_index{AssertValidIteratorIntoThisAndGetIndex(first)};
    size_type const last_index{AssertValidIteratorIntoThisAndGetIndex(last)};
    if (first_index > last_index) {
      amsr::core::Abort("Given iterator range is not valid as first > last.");
    }
    size_type const delete_count{last_index - first_index};
    size_type const current_size{GetSize()};

    iterator result{nullptr};
    if (delete_count == 0) {
      result = end();
    } else if (delete_count >= current_size) {
      clear();
      result = end();
    } else {
      ShiftElementsLeft(last_index, delete_count);

      size_type const target_size{current_size - delete_count};
      // VCA_ZEROCOPYCOMMON_ANO_01_VECTOR_TARGET_SIZE_IN_VALID_RANGE
      //@ assert (target_size > 0) && (target_size <= kMaxCapacity);
      SetSize(target_size);

      result = IteratorForIndex(first_index);
    }

    return result;
  }

  /*!
   * \brief Append a copy of the given value to the end of the vector.
   * \param[in] value The value to append.
   * \context ANY
   * \pre The vector must not be full, i.e. size() < max_size().
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  void push_back(value_type const& value) noexcept {
    size_type const current_size{GetSize()};
    size_type const target_size{current_size + 1};
    AssertSizeWithinCapacity(target_size);

    size_type const index{target_size - 1};
    //@ assert (index >= 0) && (index < kMaxCapacity);
    // VECTOR NL AutosarC++17_10-M5.0.16: MD_ZEROCOPYCOMMON_M5.0.16_vectorDataArrayAccessCheckedByVCA
    elements_[index] = value;
    SetSize(target_size);
  }

  /*!
   * \brief Append the given value to the end of the vector by moving it.
   * \param[in] value The value to append.
   * \pre The vector must not be full, i.e. size() < max_size().
   * \context ANY
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  void push_back(value_type&& value) noexcept {
    size_type const current_size{GetSize()};
    size_type const target_size{current_size + 1};
    AssertSizeWithinCapacity(target_size);

    elements_[current_size] = std::move(value);
    SetSize(target_size);
  }

  /*!
   * \brief Append a new element to the vector by constructing it in place.
   * \details If the constructor of T raises an exception, this method aborts execution.
   * \tparam Args The types of the arguments forwarded to the constructor of T.
   * \param[in] args The arguments forwarded to the constructor of T.
   * \context ANY
   * \pre The vector must not be full, i.e. size() < max_size().
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  template <class... Args>
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  void emplace_back(Args&&... args) noexcept {
    static_cast<void>(emplace(end(), std::forward<Args>(args)...));
  }

  /*!
   * \brief Remove the last element of the vector.
   * \context ANY
   * \pre The vector must not be empty, i.e. empty() == false.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  void pop_back() noexcept {
    size_type const current_size{AssertVectorNotEmptyAndGetSize()};
    SetSize(current_size - 1);
  }

  /*!
   * \brief Resize the vector to contain count elements.
   * \details If count < size(), elements at the end are deleted.
   *          If count >= size() default constructed elements are added.
   * \param[in] count The number of elements to be contained in the vector. Must be <= max_size().
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  void resize(size_type count) { resize(count, T()); }

  /*!
   * \brief Resize the vector to contain count elements.
   * \details If count < size(), elements at the end are deleted.
   *          If count >= size() copies of the given value are added to the end.
   * \param[in] count The number of elements to be contained in the vector. Must be <= max_size().
   * \param[in] value The value to be copied to extend the vector if required.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   *
   * \internal
   * - Check plausibility of all input parameters.
   * - If count is bigger than current size.
   *   - Fill new slots with the given value.
   * - Adjust vector size.
   * \endinternal
   */
  // NOLINTNEXTLINE(readability-identifier-naming) - Modeled according to C++ Standard
  void resize(size_type count, value_type const& value) noexcept {
    size_type const target_size{count};
    AssertSizeWithinCapacity(target_size);

    if (target_size > GetSize()) {
      for (size_type i{GetSize()}; i < target_size; ++i) {
        elements_[i] = value;
      }
    }

    SetSize(target_size);
  }

  // The swap method is omitted as the usual std::vector 'pointer swap semantics' cannot be reproduced.

  /*!
   * \brief Provide access to the contiguous array where elements are stored.
   * \return Reference to the statically sized array where elements are stored.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate
   */
  auto ElementData() const noexcept -> amsr::core::Array<T, kMaxCapacity> const& { return elements_; }

  /*!
   * \brief Provide access to the management structure of this vector (i.e. grants access to the vectors size).
   * \return Constant reference to the vectors size.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate
   */
  auto ManagementData() const noexcept -> size_type const& { return size_; }

  /*!
   * \brief Shift the elements starting at index distance cells to the left.
   * \details Elements in the range [index, end) are shifted left inside the underlying data array.
   *          Shift means, that the values in the range are copied to indices distance to the left.
   *          The vectors size is NOT changed.
   *          Does nothing if the given index is not contained in the vector, i.e. if no elements are affected.
   * \param[in] index Index of first element to be shifted.
   * \param[in] distance Distance the elements will be shifted.
   * \context ANY
   * \pre The shift must not go beyond the vector bounds, i.e. distance <= index.
   *      Precondition must be met if any elements are affected by the shift.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate
   */
  void ShiftElementsLeft(size_type index, size_type distance) noexcept {
    size_type const current_size{GetSize()};
    bool const are_elements_affected_by_shift{index < current_size};
    if (are_elements_affected_by_shift) {
      if (distance > index) {
        amsr::core::Abort("LeftShift not possible, as some or all destination indices are out of bounds.");
      }

      // We shift all elements in the range [source_start_index, size_ - 1] left.
      // The destination range starts at destination_start_index.
      // The destination is at lower indices within the elements_ array, therefore access is within bounds.
      size_type const source_start_index{index};
      size_type const destination_start_index{index - distance};

      size_type const num_shifted_elements{current_size - source_start_index};

      // Note: Loop can be replaced by memmove in the future if there are performance issues.
      for (size_type i{0}; i < num_shifted_elements; ++i) {
        elements_[destination_start_index + i] = elements_[source_start_index + i];
      }
    }
  }

  /*!
   * \brief Shift the elements starting at index distance cells to the right.
   * \details Elements in the range [index, end) are shifted right inside the underlying data array.
   *          Shift means, that the values in the range are copied to indices distance to the right.
   *          The vectors size is NOT changed.
   *          Only elements in the target range (distance elements right of the source) are affected by the operation.
   *          Does nothing if the given index is not contained in the vector, i.e. if no elements are affected.
   * \param[in] index Index of first element to be shifted.
   * \param[in] distance Distance the elements will be shifted.
   * \context ANY
   * \pre The capacity of the vector is large enough to accommodate the shift,
   *      i.e. (GetSize() + distance) <= max_size(). Precondition must be met if any elements are affected by the shift.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate
   */
  void ShiftElementsRight(size_type index, size_type distance) noexcept {
    size_type const current_size{GetSize()};
    if (index < current_size) {
      // We shift the last element distance slots forward.
      // This equals our precondition: (GetSize() + distance) <= max_size()
      if (distance > kMaxCapacity) {  // Required to prevent possible integer overflow.
        amsr::core::Abort("RightShift not possible, as some or all destination indices are out of bounds.");
      }
      //@ assert (current_size <= kMaxCapacity) && (distance <= kMaxCapacity);
      // VECTOR NC AutosarC++17_10-A4.7.1: MD_ZEROCOPYCOMMON_A4.7.1_integerOverflowInVectorShiftCheckedByVCA
      // VECTOR NC AutosarC++17_10-M0.3.1: MD_ZEROCOPYCOMMON_M0.3.1_underAndOverflowInVectorAlreadyJustified_falsePositive
      size_type const required_size{current_size + distance};
      if (required_size > kMaxCapacity) {
        amsr::core::Abort("RightShift not possible, as some or all destination indices are out of bounds.");
      }

      // We shift the source range [index, source_end_index] ending at source_end_index
      // into the destination range [index + distance, destination_end_index].
      // The ends of both ranges are well-defined: shift the last vector element distance slots to the right.
      size_type const source_end_index{current_size - 1};
      //@ assert required_size >= 1;
      // VECTOR NC AutosarC++17_10-A4.7.1: MD_ZEROCOPYCOMMON_A4.7.1_integerUnderflowInVectorShiftCheckedByVCA
      // VECTOR NC AutosarC++17_10-M0.3.1: MD_ZEROCOPYCOMMON_M0.3.1_underAndOverflowInVectorAlreadyJustified_falsePositive
      size_type const destination_end_index{required_size - 1};  // distance right of source_index

      // Both range-ends are within bounds (VCA tool assisted).
      //@ assert (source_end_index >= 0) && (destination_end_index >= 0);
      //@ assert (source_end_index < kMaxCapacity) && (destination_end_index < kMaxCapacity);

      size_type const elements_to_be_shifted{current_size - index};
      // Note: Loop can be replaced by memmove in the future if there are performance issues.
      for (size_type i{0}; i < elements_to_be_shifted; ++i) {
        size_type const source_index{source_end_index - i};
        // VCA_ZEROCOPYCOMMON_ANO_01_VECTOR_RIGHT_SHIFT_DESTINATION_POSITIVE
        //@ assert destination_end_index >= i;
        size_type const destination_index{destination_end_index - i};

        elements_[destination_index] = elements_[source_index];
      }
    }
  }

 private:
  /*!
   * \brief Current number of elements contained in the vector.
   */
  size_type size_{0};

  /*!
   * \brief Storage for elements contained in the vector.
   */
  amsr::core::Array<T, kMaxCapacity> elements_{};

  /*!
   * \brief Copy the given vector into this vector (replacing its old content).
   * \param[in] other Vector to be copied. Size of other must be <= max_size() of this vector.
   * \tparam kOtherSize Static maximum size of the vector to be copied.
   * \context ANY
   * \pre -
   */
  template <size_type kOtherSize>
  void Copy(TriviallyCopyableVector<T, kOtherSize> const& other) noexcept {
    size_type const target_size{other.size()};
    AssertSizeWithinCapacity(target_size);

    std::memcpy(data(), other.data(), target_size * sizeof(T));
    SetSize(target_size);
  }

  /*!
   * \brief Set the current size_ of the vector.
   * \param[in] target_size Target size of the vector.
   * \internal
   * All writes to size_ should go through this method, as it enables to check the size_ invariant using VCA.
   * \endinternal
   */
  void SetSize(size_type target_size) noexcept {
    // All assignments to the member size_ are handled via this setter (verified by manual code analysis).
    // Using the VCA silent analysis we can verify that size_ <= kMaxCapacity must hold.

    //@ assert target_size <= kMaxCapacity;
    size_ = target_size;
  }

  /*!
   * \brief Get the current size_ of the vector.
   * \return Current size_ of the vector.
   * \internal
   * Guarantees the VCA contract GetSize() <= kMaxCapacity.
   * \endinternal
   */
  auto GetSize() const noexcept -> size_type {
    size_type const result{size_};
    // We know that this holds, as we test in SetSize (see its comment for further information).

    // VCA_ZEROCOPYCOMMON_ANO_01_VECTOR_SIZE_MEMBER_HOLDS_INVARIANT
    //@ assert result <= kMaxCapacity;
    return result;
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

  /*!
   * \brief Abort if the size > kMaxCapacity.
   * \param[in] size Size to be checked.
   * \context ANY
   * \pre -
   */
  // VECTOR NL AutosarC++17_10-M0.1.8: MD_ZEROCOPYCOMMON_M0.1.8_noSideEffectInAssertFunction
  static void AssertSizeWithinCapacity(size_type size) noexcept {
    if (size > kMaxCapacity) {
      amsr::core::Abort("Static vector capacity exceeded.");
    }
  }

  /*!
   * \brief Abort if the vector is empty, return its current size otherwise.
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
   * \brief Abort if the given iterator is no valid iterator into this vector, return its index otherwise.
   * \details The end() iterator is considered valid.
   * \param[in] itr Iterator to be checked.
   * \return The index the given iterator points to.
   * \context ANY
   * \pre -
   */
  auto AssertValidIteratorIntoThisAndGetIndex(const_iterator itr) const noexcept -> size_type {
    std::ptrdiff_t const iterator_index{itr - begin()};

    if ((iterator_index < 0L) || (static_cast<size_type>(iterator_index) > GetSize())) {
      amsr::core::Abort("Given iterator is not valid for this vector.");
    }

    // Guaranteed to be positive.
    return static_cast<size_type>(iterator_index);
  }

  /*!
   * \brief Return the positive size of the iterator range [first, last).
   * \details Aborts if the std::distance between first and last is negative.
   * \tparam InputIt Type of the given iterator pair.
   * \param[in] first First element in the range (inclusive).
   * \param[in] last Last element in the range (exclusive).
   * \return Size of the iterator range.
   * \context ANY
   * \pre The range [first, last) must be a valid range originating from one object.
   *      This precondition cannot be checked and undefined behavior results if the caller violates it.
   */
  template <typename InputIt>
  static auto AssertIteratorRangePositiveDistance(InputIt first, InputIt last) noexcept
      -> std::enable_if_t<IsInputIterator<InputIt>::value, size_type> {
    std::ptrdiff_t const range_size{std::distance(first, last)};
    if (range_size < 0) {
      amsr::core::Abort("Given iterator range is not valid as first > last.");
    }

    // Guaranteed to be positive.
    return static_cast<size_type>(range_size);
  }

  /*!
   * \brief Abort if the given range [first_pointer, last_pointer) points into the elements_ array.
   * \details Preconditions must be checked by calling function (internal helper method).
   * \param[in] first_pointer First pointer of the range.
   * \param[in] last_pointer Last pointer of the range.
   * \context ANY
   * \pre The range [first_pointer, lastPointer) must be a positive range, i.e. first_pointer <= last_pointer.
   */
  // VECTOR NL AutosarC++17_10-M0.1.8: MD_ZEROCOPYCOMMON_M0.1.8_noSideEffectInAssertFunction
  void AssertPointerRangeNotFromThisVector(const_pointer first_pointer, const_pointer last_pointer) {
    const_pointer const first_element_pointer{&elements_[0]};
    const_pointer const last_element_pointer{&elements_[kMaxCapacity - 1]};

    // VECTOR NL AutosarC++17_10-M5.0.18: MD_ZEROCOPYCOMMON_M5.0.18_vectorPointerComparisonDetectInvalidIterators
    bool const range_behind_this{first_pointer > last_element_pointer};
    // VECTOR NL AutosarC++17_10-M5.0.18: MD_ZEROCOPYCOMMON_M5.0.18_vectorPointerComparisonDetectInvalidIterators
    bool const range_before_this{last_pointer < first_element_pointer};
    if ((!range_before_this) && (!range_behind_this)) {
      amsr::core::Abort("Iterator range must not point to this vector.");
    }
  }

  /*!
   * \brief Abort if the given iterator range [begin, end) references this vector object instance (this data_ array).
   * \details This effectively captures the 'abort if the iterator range references into this object instance' intent.
   * \tparam InputIt Generic iterator types.
   * \param[in] first Begin of the range to be checked.
   * \param[in] last End of the range to be checked.
   * \context ANY
   * \pre -
   * \internal
   * Technical Note: This functionality is implemented using multiple function overloads, as it is used
   *                 from within template functions that take any iterator type as parameter.
   * \endinternal
   */
  template <typename InputIt>
  // VECTOR NL AutosarC++17_10-M0.1.8: MD_ZEROCOPYCOMMON_M0.1.8_noSideEffectInAssertFunction
  static void AssertIteratorRangeNotFromThisVector(InputIt first, InputIt last) noexcept {
    // If this overload is chosen, the iterator type given is NOT any of the TriviallyCopyableVector's iterator types.
    // Therefore, we interpret the iterator range as not referencing this vector.
    // No abort is required in this overload. Ignore the input arguments (required for correct overload resolution).
    static_cast<void>(first);
    static_cast<void>(last);
  }

  /*!
   * \brief Abort if the given iterator range [begin, end) references this vector object instance (this data_ array).
   * \details This effectively captures the 'abort if the iterator range references into this object instance' intent.
   * \param[in] first Begin of the range to be checked.
   * \param[in] last End of the range to be checked.
   * \context ANY
   * \pre -
   * \internal
   * Technical Note: This functionality is implemented using multiple function overloads, as it is used
   *                 from within template functions that take any iterator type as parameter.
   * \endinternal
   */
  // VECTOR NL AutosarC++17_10-M0.1.8: MD_ZEROCOPYCOMMON_M0.1.8_noSideEffectInAssertFunction
  void AssertIteratorRangeNotFromThisVector(iterator first, iterator last) {
    AssertPointerRangeNotFromThisVector(first, last);
  }

  /*!
   * \copydoc AssertIteratorRangeNotFromThisVector(iterator,iterator)
   */
  // VECTOR NL AutosarC++17_10-M0.1.8: MD_ZEROCOPYCOMMON_M0.1.8_noSideEffectInAssertFunction
  void AssertIteratorRangeNotFromThisVector(const_iterator first, const_iterator last) {
    AssertPointerRangeNotFromThisVector(first, last);
  }

  /*!
   * \copydoc AssertIteratorRangeNotFromThisVector(iterator,iterator)
   */
  // VECTOR NL AutosarC++17_10-M0.1.8: MD_ZEROCOPYCOMMON_M0.1.8_noSideEffectInAssertFunction
  void AssertIteratorRangeNotFromThisVector(reverse_iterator first, reverse_iterator last) {
    // Need to 'invert' the underlying pointers, as the called method expects first_pointer <= last_pointer.
    AssertPointerRangeNotFromThisVector(last.base(), first.base());
  }

  /*!
   * \copydoc AssertIteratorRangeNotFromThisVector(iterator,iterator)
   */
  // VECTOR NL AutosarC++17_10-M0.1.8: MD_ZEROCOPYCOMMON_M0.1.8_noSideEffectInAssertFunction
  void AssertIteratorRangeNotFromThisVector(const_reverse_iterator first, const_reverse_iterator last) {
    // Need to 'invert' the underlying pointers, as the called method expects first_pointer <= last_pointer.
    AssertPointerRangeNotFromThisVector(last.base(), first.base());
  }

  /*!
   * \brief Get a non-const iterator from a valid index into this vector.
   * \details Preconditions must be checked by calling function (internal helper method).
   * \param[in] index Index to be converted to a non-const iterator. Must be < size().
   * \return Non const it pointing to the same element as the given index.
   * \context ANY
   * \pre -
   */
  auto IteratorForIndex(size_type index) noexcept -> iterator { return &elements_[index]; }
};

/*!
 * \brief Compare two trivially copyable vectors element-wise.
 * \tparam T Type stored in both vectors.
 * \tparam kSizeLeft Maximum size of the left vector.
 * \tparam kSizeRight Maximum size of the right vector.
 * \param[in] lhs Left vector instance.
 * \param[in] rhs Right vector instance.
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
auto operator==(TriviallyCopyableVector<T, kSizeLeft> const& lhs,
                TriviallyCopyableVector<T, kSizeRight> const& rhs) noexcept -> bool {
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

}  // namespace container
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_TRIVIALLY_COPYABLE_CONTAINER_INCLUDE_AMSR_ZERO_COPY_COMMON_CONTAINER_TRIVIALLY_COPYABLE_VECTOR_H_
