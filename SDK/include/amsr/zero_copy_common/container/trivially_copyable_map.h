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
/*!        \file
 *        \brief Implementation of a trivially copyable map.
 *         \unit ZeroCopyCommon::TriviallyCopyableContainer::TriviallyCopyableMap
 *   \complexity The high complexity of the class TriviallyCopyableMap from the equally named unit is caused by a high
 *               call-tree complexity. The high number of internal function calls is required to support different API
 *               signatures for the same action by not duplicating any code.
 *               In summary, the complexity is increased by using many internal functions which are useful to reduce
 *               code duplication and it reflects the behavior of the standard library containers which have many
 *               different APIs for the same actions with different parameters.
 *               Therefore, the risk is considered low.
 *
 *********************************************************************************************************************/
#ifndef LIB_TRIVIALLY_COPYABLE_CONTAINER_INCLUDE_AMSR_ZERO_COPY_COMMON_CONTAINER_TRIVIALLY_COPYABLE_MAP_H_
#define LIB_TRIVIALLY_COPYABLE_CONTAINER_INCLUDE_AMSR_ZERO_COPY_COMMON_CONTAINER_TRIVIALLY_COPYABLE_MAP_H_

#include <cassert>
#include <cstring>
#include <iostream>
#include <iterator>
#include <limits>
#include <utility>

#include "amsr/core/abort.h"
#include "amsr/core/array.h"
#include "trivially_copyable_map_iterator.h"

namespace amsr {
namespace zero_copy_common {
namespace container {

template <typename Key, typename T, std::size_t kCapacity>
class TriviallyCopyableMap;

/*!
 * \brief Type for management nodes of the trivially copyable map.
 *        This stores the parent, left and right indices for the neighbors inside the map.
 */
struct TriviallyCopyableMapManagementNode {
  /*!
   * \brief Type of indices to other nodes.
   */
  using Index = std::size_t;

  /*!
   * \brief Value of an empty index.
   */
  static constexpr Index kEmptyIndex{std::numeric_limits<Index>::max()};

  /*!
   * \brief Index to the left child node.
   */
  Index left_{kEmptyIndex};

  /*!
   * \brief Index to the right child node.
   */
  Index right_{kEmptyIndex};

  /*!
   * \brief Index to the parent node.
   */
  Index parent_{kEmptyIndex};
};

// VECTOR NC AutosarC++17_10-A12.4.1: MD_ZEROCOPYCOMMON_A12.4.1_mapNoVirtualDtor
/*!
 * \brief Class for key value nodes of the trivially copyable map.
 *        This serves as a pair of key and value to be inserted in the map as a node.
 * \tparam key Type for key.
 * \tparam T Type of the value.
 */
template <typename Key, typename T>
class ReadableKeyValueStorage {
 public:
  /*!
   * \brief Return a read-only reference to the key for the user.
   * \return Const reference to the key.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto key() const -> Key const& { return key_; }

  /*!
   * \brief Return a writable reference to the value for the user.
   * \return Reference to the value.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto value() -> T& { return value_; }

  /*!
   * \brief Return a writable reference to the value for the user.
   * \return Reference to the value.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto value() const -> T const& { return value_; }

 protected:
  /*!
   * \brief Key storage.
   */
  // VECTOR NL VectorC++-V11.0.2: MD_ZEROCOPYCOMMON_V11.0.2_mapClassMemberShallBePrivate
  Key key_{};

  /*!
   * \brief Data storage.
   */
  // VECTOR NL VectorC++-V11.0.2: MD_ZEROCOPYCOMMON_V11.0.2_mapClassMemberShallBePrivate
  T value_{};
};

/*!
 * \brief Class for key value nodes of the trivially copyable map.
 *        This serves as a pair of key and value to be inserted in the map as a node.
 * \tparam Key Type for key.
 * \tparam T Type of the value.
 */
template <typename Key, typename T>
class WritableKeyValueStorage final : public ReadableKeyValueStorage<Key, T> {
  using ReadableKeyValueStorage<Key, T>::key_;

 public:
  /*!
   * \brief Set the key for a map element.
   * \param[in] key Key which gets placed inside the storage.
   * \return Const reference to the key.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  void SetKey(Key const& key) { key_ = key; }
};

// VECTOR NC Metric-OO.WMC.One: MD_ZEROCOPYCOMMON_Metric-OO.WMC.One_containersModeledAfterStandardLibrary
/*!
 * \brief Class to implement an TriviallyCopyableMap.
 * \details Contiguous memory is allocated by statically configuring the map size using template parameters.
 *          All memory is contained within the trivially copyable map object using an array with static size.
 *          Elements can be added and removed within the statically configured maximum capacity.
 *          As far as sensible APIs are modeled after std::map.
 *          If the static maximum_capacity is exceeded, program execution is aborted. See (parameter) pre-conditions.
 *          Exception safety of most methods is given by the trivially copyable property of T (copying, moving and
 *          deconstructing values will never throw an exception, therefore methods will not abort due to exceptions).
 *          Methods that might terminate because of T throwing exceptions are documented.
 *
 * \tparam  Key Key type stored in the map.
 *            Must be trivially copyable and trivially destructible. Must be copy assignable and copy constructible.
 *            The trivially copyable property should be held further than C++ specifies for the Zero-Copy use-case.
 *            It should be possible to share the object over process boundaries (as this is one of the main use-cases).
 *            Therefore, no form of pointer should be used in T, as it might become invalid after performing an
 *            'inter-process mem-copy'. Note that this includes e.g. pointers to global constant memory.
 * \tparam  T Value type stored in the map.
 *            Must be trivially copyable and trivially destructible. Must be copy assignable and copy constructible.
 *            The trivially copyable property should be held further than C++ specifies for the Zero-Copy use-case.
 *            It should be possible to share the object over process boundaries (as this is one of the main use-cases).
 *            Therefore, no form of pointer should be used in T, as it might become invalid after performing an
 *            'inter-process mem-copy'. Note that this includes e.g. pointers to global constant memory.
 * \tparam  kCapacity Maximum capacity of the map. Must be >= 1. Must be < std::numeric_limits<size_t>::max() / 2.
 *
 * \trace DSGN-ZeroCopyCommon-TriviallyCopyableMap
 */
template <typename Key, typename T, std::size_t kCapacity>
class TriviallyCopyableMap final {
 public:
  /*!
   * \brief Type of contained key.
   * \vpublic
   */
  using key_type = Key;

  /*!
   * \brief Type of contained data.
   * \vpublic
   */
  using mapped_type = T;

  /*!
   * \brief Type of contained value.
   * \vpublic
   */
  using value_type = ReadableKeyValueStorage<Key, T>;

  /*!
   * \brief Type of a pair containing key and value.
   * \vpublic
   */
  using pair_type = std::pair<Key const, T>;

  /*!
   * \brief Type of size.
   * \vpublic
   */
  using size_type = std::size_t;

  /*!
   * \brief Type of reference.
   * \vpublic
   */
  using reference = value_type&;

  /*!
   * \brief Type of const reference.
   * \vpublic
   */
  using const_reference = value_type const&;

  /*!
   * \brief Type of pointer.
   * \vpublic
   */
  using pointer = value_type*;

  /*!
   * \brief Type of const pointer.
   * \vpublic
   */
  using const_pointer = value_type const*;

  /*!
   * \brief Typedef for the iterator type of this map.
   * \vpublic
   */
  using iterator = TriviallyCopyableMapIterator<Key, T, kCapacity>;

  /*!
   * \brief Typedef for the const_iterator type of this map.
   * \vpublic
   */
  using const_iterator = ConstTriviallyCopyableMapIterator<Key, T, kCapacity, TriviallyCopyableMap>;

  /*!
   * \brief Typedef for the reverse_iterator type of this map.
   * \vpublic
   */
  using reverse_iterator = std::reverse_iterator<iterator>;

  /*!
   * \brief Typedef for the const_reverse_iterator type of this map.
   * \vpublic
   */
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  /*!
   * \brief Type of indices to other nodes.
   * \vprivate
   */
  using Index = std::size_t;

  /*!
   * \brief Struct definition of the header segment.
   * \vprivate
   */
  struct header {
    /*!
     * \brief Current size of the map.
     */
    size_type size_{0};

    /*!
     * \brief Index of the root node.
     */
    Index root_{kEmptyIndex};

    /*!
     * \brief Index of the next free node.
     */
    Index free_{0};
  };

  /*!
   * \brief Type of the header segment.
   * \vprivate
   */
  using header_segment_type = header;

  /*!
   * \brief Type of the management segment.
   * \vprivate
   */
  using management_segment_type = amsr::core::Array<TriviallyCopyableMapManagementNode, kCapacity>;

  /*!
   * \brief Type of the data segment.
   * \vprivate
   */
  using data_segment_type = amsr::core::Array<WritableKeyValueStorage<Key, T>, kCapacity>;

  /*!
   * \brief Value of an empty index.
   * \vprivate
   */
  static constexpr Index kEmptyIndex{std::numeric_limits<Index>::max()};

  // Trivially copyable elements are required for the container itself to be trivially copyable.
  // General copy properties are required to insert elements into the vector.
  static_assert(std::is_trivially_copyable<Key>::value, " Key must be trivially copyable.");
  static_assert(std::is_copy_assignable<Key>::value, "Key must be copy assignable");
  static_assert(std::is_copy_constructible<Key>::value, "Key must be copy constructible");
  static_assert(std::is_trivially_destructible<Key>::value, "Key must be trivially destructible.");

  static_assert(std::is_trivially_copyable<T>::value, "Value must be trivially copyable.");
  static_assert(std::is_copy_assignable<T>::value, "Value must be copy assignable");
  static_assert(std::is_copy_constructible<T>::value, "Value must be copy constructible");
  static_assert(std::is_trivially_destructible<T>::value, "Value must be trivially destructible.");

  // Having a data size of at least one allows simpler and more defensive implementations of accessor methods.
  static_assert(kCapacity >= 1, "The capacity must be at least one, otherwise no elements can be stored.");
  // We often perform additions of two values of type size_type.
  // To avoid overflows we usually check both parameters to be <= kCapacity.
  // By limiting the kCapacity range, we can ensure that no overflow can occur at any point in our code.
  static_assert((std::numeric_limits<size_t>::max() / 2) >= kCapacity,
                "kCapacity must be small enough to avoid integer overflows.");

  /*!
   * \brief Typedef for a typetrait checking if the given Iterator type is an input iterator.
   * \tparam Iterator Type to be checked to be an input iterator.
   */
  template <typename Iterator>
  using IsInputIterator =
      std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>;

  /*!
   * \brief Construct an empty map with a maximum size.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  TriviallyCopyableMap() noexcept { clear(); };

  /*!
   * \brief Destruct a map.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  ~TriviallyCopyableMap() noexcept = default;

  /*!
   * \brief Copy constructor of a map with same capacity.
   * \param[in] other Map to be copied.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  TriviallyCopyableMap(TriviallyCopyableMap const& other) noexcept = default;

  /*!
   * \brief Move constructor of a map with same capacity.
   * \param[in] other Map to be moved.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  TriviallyCopyableMap(TriviallyCopyableMap&& other) noexcept = default;

  // VECTOR NC AutosarC++17_10-A8.5.4: MD_ZEROCOPYCOMMON_A8.5.4_mapInitializerListInCtorOnlyIfContainer_falsePositive
  // VECTOR NC VectorC++-V8.5.4: MD_ZEROCOPYCOMMON_V8.5.4_mapCtorShallNotAcceptStdInitializerList
  /*!
   * \brief Construct the container with the contents of the initializer list.
   * \details If multiple elements in the range have keys that compare equivalent,
   *          it is unspecified which element is inserted.
   * \param[in] ilist Initializer list to insert the values from
   * \context ANY
   * \pre Initializer list may not be greater than the map capacity, otherwise the ctor aborts.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  explicit TriviallyCopyableMap(std::initializer_list<pair_type> ilist) noexcept {
    if (ilist.size() > kCapacity) {
      amsr::core::Abort("Trivially copyable map capacity exceeded.");
    }
    clear();
    insert(ilist.begin(), ilist.end());
  }

  /*!
   * \brief Construct the container with the contents in the range from first to last.
   * \details If multiple elements in the range have keys that compare equivalent,
   *          it is unspecified which element is inserted.
   * \param[in] first Iterator pointing to the first element
   * \param[in] last Iterator pointing to one element after the last element
   * \context ANY
   * \pre Iterator must point to the same object and the range must be <= kCapacity
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  template <class InputIt, std::enable_if_t<IsInputIterator<InputIt>::value, bool> = true>
  explicit TriviallyCopyableMap(InputIt first, InputIt last) noexcept {
    if (static_cast<std::size_t>(std::distance(first, last)) > kCapacity) {
      amsr::core::Abort("Trivially copyable map capacity exceeded.");
    }
    clear();
    insert(first, last);
  }

  /*!
   * \brief Replace the map contents with a copy of another map of same size.
   * \param[in] other Map to be copied from.
   * \return Reference to map.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto operator=(TriviallyCopyableMap const& other) & noexcept -> TriviallyCopyableMap& = default;

  /*!
   * \brief Move another map with same size to this map.
   * \param[in] other Map to be moved.
   * \return Reference to map.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto operator=(TriviallyCopyableMap&& other) & noexcept -> TriviallyCopyableMap& = default;

  /*!
   * \brief Return a reference to the mapped value to the key.
   * \param[in] key The key of the element to find.
   * \return Reference to the mapped value.
   * \context ANY
   * \pre key must exist in the map and can be checked with find(), otherwise the function aborts.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto at(Key const& key) noexcept -> mapped_type& {
    std::pair<Index, bool> const search_result{SearchNode(key)};  // VCA_ZEROCOPYCOMMON_LNG_02_REFERENCE_VALID
    if (!search_result.second) {
      amsr::core::Abort("The passed key doesn't exist!");
    }
    return data_[search_result.first].value();  // VCA_ZEROCOPYCOMMON_LNG_02_REFERENCE_VALID
  }

  /*!
   * \brief Return a const reference to the mapped value to the key.
   * \param[in] key The key of the element to find.
   * \return Const reference to the mapped value.
   * \context ANY
   * \pre key must exist in the map and can be checked with find(), otherwise the function aborts.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto at(Key const& key) const noexcept -> mapped_type const& {
    std::pair<Index, bool> const search_result{SearchNode(key)};
    if (!search_result.second) {
      amsr::core::Abort("The passed key doesn't exist!");
    }
    return data_[search_result.first].value();  // VCA_ZEROCOPYCOMMON_LNG_02_REFERENCE_VALID
  }

  // VECTOR NC AutosarC++17_10-A13.5.1: MD_ZEROCOPYCOMMON_A13.5.1_mapOperatorIsNotSupportedForConst
  /*!
   * \brief Return a reference to the mapped value with the key. If key does not exist it will be created.
   * \param[in] key The key of the element to find.
   * \return Reference to the mapped value.
   * \context ANY
   * \pre If the element does not exist, the size() of the map must be less than the capacity to create a new element.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto operator[](Key const& key) noexcept -> mapped_type& {
    std::pair<iterator, bool> insert_result{insert({key, 0})};
    return insert_result.first->value();  // VCA_ZEROCOPYCOMMON_LNG_02_REFERENCE_VALID
  }

  // VECTOR NC AutosarC++17_10-A13.5.1: MD_ZEROCOPYCOMMON_A13.5.1_mapOperatorIsNotSupportedForConst
  /*!
   * \brief Return a reference to the mapped value with the key. If key does not exist it will be created.
   * \param[in] key The key of the element to find.
   * \return Reference to the mapped value.
   * \context ANY
   * \pre If the element does not exist, the size() of the map must be less than the capacity to create a new element.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto operator[](Key&& key) noexcept -> mapped_type& {
    std::pair<iterator, bool> insert_result{insert({std::move(key), 0})};
    return insert_result.first->value();  // VCA_ZEROCOPYCOMMON_LNG_02_REFERENCE_VALID
  }

  /*!
   * \brief Return an iterator pointing to the element with the lowest key of this map.
   * \return An iterator to the first element of this TriviallyCopyableMap
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto begin() noexcept -> iterator {
    Index const first_index{GetMinimumKeyIndex(header_.root_)};
    return iterator(first_index, management_, data_, *this);
  }

  /*!
   * \brief Return an const_iterator pointing to the element with the lowest key of this map.
   * \return An const_iterator to the first element of this TriviallyCopyableMap
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto begin() const noexcept -> const_iterator {
    Index const first_index{GetMinimumKeyIndex(header_.root_)};
    return const_iterator(first_index, management_, data_, *this);
  }

  /*!
   * \brief Return an const_iterator pointing to the element with the lowest key of this map.
   * \return An const_iterator to the first element of this TriviallyCopyableMap
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto cbegin() const noexcept -> const_iterator { return begin(); }

  /*!
   * \brief Return a reverse_iterator pointing to the element with the maximum key of this map.
   * \return A reverse_iterator to the last element of this TriviallyCopyableMap
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto rbegin() noexcept -> std::reverse_iterator<iterator> { return std::reverse_iterator<iterator>{end()}; }

  /*!
   * \brief Return a const reverse_iterator pointing to the element with the maximum key of this map.
   * \return A const reverse_iterator to the last element of this TriviallyCopyableMap
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto rbegin() const noexcept -> std::reverse_iterator<const_iterator> {
    return std::reverse_iterator<const_iterator>{end()};
  }

  /*!
   * \brief Return a const reverse_iterator pointing to the element with the maximum key of this map.
   * \return A const reverse_iterator to the last element of this TriviallyCopyableMap
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto crbegin() const noexcept -> std::reverse_iterator<const_iterator> { return rbegin(); }

  /*!
   * \brief Return an end iterator pointing a non-existing element.
   * \return An end() iterator pointing to an invalid element.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto end() noexcept -> iterator { return iterator(kEmptyIndex, management_, data_, *this); }

  /*!
   * \brief Return an end const_iterator pointing a non-existing element.
   * \return An end() const_iterator pointing to an invalid element.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto end() const noexcept -> const_iterator { return const_iterator(kEmptyIndex, management_, data_, *this); }

  /*!
   * \brief Return an end const_iterator pointing a non-existing element.
   * \return An end() const_iterator pointing to an invalid element.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto cend() const noexcept -> const_iterator { return end(); }

  /*!
   * \brief Return a reverse_iterator pointing to the reverse end element.
   * \return A reverse_iterator to the last in reverse order element of this TriviallyCopyableMap
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto rend() noexcept -> std::reverse_iterator<iterator> { return std::reverse_iterator<iterator>{begin()}; }

  /*!
   * \brief Return a const reverse_iterator pointing to the reverse end element.
   * \return A const reverse_iterator to the last element in reverse order of this TriviallyCopyableMap
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto rend() const noexcept -> std::reverse_iterator<const_iterator> {
    return std::reverse_iterator<const_iterator>{begin()};
  }

  /*!
   * \brief Return a const reverse_iterator pointing to the reverse end element.
   * \return A const reverse_iterator to the last element in reverse order of this TriviallyCopyableMap
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto crend() const noexcept -> std::reverse_iterator<const_iterator> { return rend(); }

  /*!
   * \brief Check if the container has no elements.
   * \return true if the container is empty, false otherwise
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto empty() const noexcept -> bool { return (header_.size_ == 0); }

  /*!
   * \brief Return the number of currently used elements in the container.
   * \return The number of elements in the container.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto size() const noexcept -> size_type { return header_.size_; }

  /*!
   * \brief Return the maximum number of elements the container is able to hold (kCapacity).
   * \return Maximum number of elements.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  static constexpr auto max_size() noexcept -> size_type { return kCapacity; }

  /*!
   * \brief Erase all elements from the container. After this call, size() returns zero.
   * \details Invalidates any references, pointers, or iterators referring to contained elements. Any past-the-end
   *            iterator remains valid.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  void clear() noexcept {
    // Clear header information
    header_.free_ = 0;
    header_.root_ = kEmptyIndex;
    header_.size_ = 0;
    // build up free list again
    for (std::size_t idx{0}; idx < max_size(); ++idx) {
      management_[idx].parent_ = kEmptyIndex;
      management_[idx].left_ = kEmptyIndex;
      management_[idx].right_ = idx + 1;
    }
    management_[max_size() - 1].right_ = kEmptyIndex;
  }

  /*!
   * \brief Insert an element into the container, if the container does not already contain an element with an
   *            equivalent key.
   * \param[in] value Element value to insert
   * \return Returns a pair consisting of an iterator to the inserted element (or to the element that prevented the
   *            insertion) and a bool denoting whether the insertion took place.
   * \context ANY
   * \pre The remaining size (Capacity - size()) must be greater than 0, otherwise the function aborts.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   * \internal
   * - if the key already exists
   *    - return iterator on existing element + boolean flag (true)
   * - else
   *    - get the next free node
   *    - if there is no free node
   *       - ABORT
   *    - else
   *       - insert kay and value and increment size by one
   *       - if the map was empty before
   *          - set root index to new element & set parent
   *       - else
   *          - set parent to searched node and identify if the element is the left or right child
   * \endinternal
   */
  auto insert(pair_type const& value) noexcept -> std::pair<iterator, bool> {
    bool key_exists{false};
    Index index_result{kEmptyIndex};
    std::pair<Index, bool> const search_result{SearchNode(value.first)};
    if (search_result.second) {
      key_exists = true;
      index_result = search_result.first;
    } else {
      index_result = GetNextFreeNode();
      // VCA_ZEROCOPYCOMMON_ANO_01_HOLDS_ONLY_VALID_INDICES
      //@ assert index_result < kCapacity;
      ++header_.size_;
      data_[index_result].SetKey(value.first);
      data_[index_result].value() = value.second;
      if (search_result.first == kEmptyIndex) {
        header_.root_ = index_result;
        management_[index_result].parent_ = search_result.first;
      } else {
        // VCA_ZEROCOPYCOMMON_ANO_01_HOLDS_ONLY_VALID_INDICES
        //@ assert search_result.first < kCapacity;
        management_[index_result].parent_ = search_result.first;
        if (value.first < data_[search_result.first].key()) {
          management_[search_result.first].left_ = index_result;
        } else {
          management_[search_result.first].right_ = index_result;
        }
      }
    }
    return std::make_pair(iterator(index_result, management_, data_, *this), key_exists);
  }

  /*!
   * \brief Insert value in the position as close as possible.
   * \param[in] position Iterator to position to insert the value
   * \param[in] value Element value to insert
   * \return Returns an iterator to the inserted element, or to the element that prevented the insertion.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto insert(iterator position, pair_type const& value) noexcept -> iterator {
    static_cast<void>(position);
    return insert(value).first;
  }

  /*!
   * \brief Insert value in the position as close as possible.
   * \param[in] position Const_iterator to position to insert the value
   * \param[in] value Element value to insert
   * \return Returns an iterator to the inserted element, or to the element that prevented the insertion.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto insert(const_iterator position, pair_type const& value) noexcept -> iterator {
    static_cast<void>(position);
    return insert(value).first;
  }

  /*!
   * \brief Insert elements from range [first, last) which ist a TriviallyCopyableMapIterator.
   * \details If multiple elements in the range have keys that compare equivalent, it is unspecified which element
   *            is inserted.
   * \param[in] first Iterator to first element of the range of copied elements.
   * \param[in] last Iterator after last element of the range of copied elements.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  template <std::size_t Capacity>
  void insert(TriviallyCopyableMapIterator<Key, T, Capacity> first,
              TriviallyCopyableMapIterator<Key, T, Capacity> last) noexcept {
    while (first != last) {
      static_cast<void>(insert({first->key(), first->value()}));  // VCA_ZEROCOPYCOMMON_LNG_01_VALID_REFERENCE_ARGUMENT
      ++first;
    }
  }

  /*!
   * \brief Insert elements from range [first, last) which ist a TriviallyCopyableMapIterator.
   * \details If multiple elements in the range have keys that compare equivalent, it is unspecified which element
   *            is inserted.
   * \param[in] first Iterator to first element of the range of copied elements.
   * \param[in] last Iterator after last element of the range of copied elements.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  template <std::size_t Capacity>
  void insert(
      ConstTriviallyCopyableMapIterator<Key, T, Capacity, TriviallyCopyableMap<Key, T, Capacity>> first,
      ConstTriviallyCopyableMapIterator<Key, T, Capacity, TriviallyCopyableMap<Key, T, Capacity>> last) noexcept {
    while (first != last) {
      static_cast<void>(insert({first->key(), first->value()}));  // VCA_ZEROCOPYCOMMON_LNG_01_VALID_REFERENCE_ARGUMENT
      ++first;
    }
  }

  /*!
   * \brief Insert elements from range [first, last).
   * \details If multiple elements in the range have keys that compare equivalent, it is unspecified which element
   *            is inserted.
   * \tparam InputIt Iterator type of the inserted elements.
   * \param[in] first Iterator to first element of the range of copied elements
   * \param[in] last Iterator after last element of the range of copied elements.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  template <class InputIt, std::enable_if_t<IsInputIterator<InputIt>::value, bool> = true>
  void insert(InputIt first, InputIt last) noexcept {
    while (first != last) {
      static_cast<void>(insert({first->first, first->second}));  // VCA_ZEROCOPYCOMMON_LNG_01_VALID_REFERENCE_ARGUMENT
      // VECTOR NL AutosarC++17_10-M5.0.16: MD_ZEROCOPYCOMMON_M5.0.16_vectorPointerArithmeticOnIterators
      ++first;
    }
  }

  /*!
   * \brief Insert elements from initializer list initializer_list.
   * \details If multiple elements in the range have keys that compare equivalent, it is unspecified which element
   *            is inserted.
   * \param[in] ilist Initializer list to insert the values from
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  void insert(std::initializer_list<pair_type> ilist) noexcept { insert(ilist.begin(), ilist.end()); }

  /*!
   * \brief Insert a new element into the container constructed in-place with the given args if there is no element
   *            with the key in the container.
   * \details No iterators or references are invalidated.
   * \tparam Args The types of the arguments forwarded to the constructor of T.
   * \param[in] args arguments to forward to the constructor of the element
   * \return Returns a pair consisting of an iterator to the inserted element, or the already-existing element if no
   *            insertion happened, and a bool denoting whether the insertion took place.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  template <class... Args>
  auto emplace(Args&&... args) noexcept -> std::pair<iterator, bool> {
    std::pair<iterator, bool> const result{insert(std::pair<key_type, mapped_type>(std::forward<Args>(args)...))};
    return result;
  }

  /*!
   * \brief Insert a new element to the container as close as possible to the position just before hint.
   * \details No iterators or references are invalidated.
   * \tparam Args The types of the arguments forwarded to the constructor of T.
   * \param[in] hint Iterator to the position before which the new element will be inserted
   * \param[in] args Arguments to forward to the constructor of the element
   * \return Returns a pair consisting of an iterator to the inserted element, or the already-existing element if no
   *            insertion happened, and a bool denoting whether the insertion took place.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  template <class... Args>
  auto emplace_hint(const_iterator hint, Args&&... args) noexcept -> iterator {
    static_cast<void>(hint);
    std::pair<iterator, bool> const result{insert(std::pair<key_type, mapped_type>(std::forward<Args>(args)...))};
    return result.first;
  }

  /*!
   * \brief Remove the element at pos.
   * \details References and iterators to the erased elements are invalidated.
   * \param[in] pos Iterator to an element in the vector
   * \return Iterator following the removed element.
   * \context ANY
   * \pre The iterator pos must be valid and dereferenceable. (must be inside the map)
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto erase(iterator pos) noexcept -> iterator {
    // VCA_ZEROCOPYCOMMON_LNG_03_POSITION_ITERATOR_INVALID_OBJECT
    key_type const remove_key{pos->key()};
    ++pos;
    static_cast<void>(erase(remove_key));
    return pos;
  }

  /*!
   * \brief Remove the element (if one exists) with the key equivalent to key.
   * \details References and iterators to the erased elements are invalidated.
   * \param[in] key The Key of the elements to remove
   * \return Number of elements removed (0 or 1).
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto erase(Key const& key) noexcept -> size_type {
    std::pair<Index, bool> const search_result{SearchNode(key)};
    size_type result{0};
    if (search_result.second) {
      result++;
      header_.size_--;
      if ((management_[search_result.first].right_ != kEmptyIndex) &&
          (management_[search_result.first].left_ != kEmptyIndex)) {
        // node has a left and a right child
        EraseNodeWithTwoChilds(search_result.first);
      } else if ((management_[search_result.first].right_ == kEmptyIndex) &&
                 (management_[search_result.first].left_ == kEmptyIndex)) {
        // node has no child
        EraseNodeWithNoChild(search_result.first);
      } else {
        // node has one child
        EraseNodeWithOneChild(search_result.first);
      }
      AddNodeToFreeList(search_result.first);
    } else {
      // key not found
    }
    return result;
  }

  /*!
   * \brief Remove the elements in the range [first; last), which must be a valid range in *this.
   * \details References and iterators to the erased elements are invalidated.
   * \param[in] first Iterator to the first element which will be removed
   * \param[in] last Iterator to one element after the last element which will be removed
   * \return Iterator following the last removed element.
   * \context ANY
   * \pre The iterators first and last must be valid and dereferenceable. (last is not inluded for removal)
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto erase(const_iterator first, const_iterator last) noexcept -> iterator {
    Index result_index{kEmptyIndex};
    while (first != last) {
      // VCA_ZEROCOPYCOMMON_LNG_03_POSITION_ITERATOR_INVALID_OBJECT
      key_type const remove_key{first->key()};
      ++first;
      static_cast<void>(erase(remove_key));
    }
    if (last != cend()) {
      result_index = SearchNode(first->key()).first;  // VCA_ZEROCOPYCOMMON_LNG_02_REFERENCE_VALID
    } else {
      result_index = kEmptyIndex;
    }
    return iterator(result_index, management_, data_, *this);
  }

  /*!
   * \brief Exchange the contents of the container with those of other.
   * \details All iterators and references remain valid. The past-the-end iterator is invalidated.
   * \param[in,out] other Container to exchange the contents with
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  void swap(TriviallyCopyableMap& other) noexcept {
    TriviallyCopyableMap buf{};
    std::memcpy(&buf, &other, sizeof(other));
    std::memcpy(&other, this, sizeof(TriviallyCopyableMap));
    std::memcpy(this, &buf, sizeof(TriviallyCopyableMap));
  }

  /*!
   * \brief Return the number of elements with key that compares equivalent to the specified argument. (1 or 0)
   * \param[in] key The Key of the elements to count
   * \return Number of elements with key that compares equivalent to key, which is either 1 or 0.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto count(Key const& key) const noexcept -> size_type {
    size_type count_result{0};
    if (SearchNode(key).second) {
      count_result = 1;
    }
    return count_result;
  }

  /*!
   * \brief Find an element with key equivalent to key.
   * \param[in] key The key of the element to search for
   * \return Iterator to an element with key equivalent to key. If no such element is found, end() is returned.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto find(Key const& key) noexcept -> iterator {
    std::pair<Index, bool> const search_result{SearchNode(key)};
    Index result_index{kEmptyIndex};
    if (search_result.second) {
      result_index = search_result.first;
    }
    return iterator(result_index, management_, data_, *this);
  }

  /*!
   * \brief Find an element with key equivalent to key.
   * \param[in] key The key of the element to search for.
   * \return Const_iterator to an element with key equivalent to key. If no such element is found, end() is returned.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto find(Key const& key) const noexcept -> const_iterator {
    std::pair<Index, bool> const search_result{SearchNode(key)};
    Index result_index{kEmptyIndex};
    if (search_result.second) {
      result_index = search_result.first;
    }
    return const_iterator(result_index, management_, data_, *this);
  }

  /*!
   * \brief Compare the keys to key. Returns a range containing all elements with the given key in the container.
   * \param[in] key The key to compare the elements to
   * \return std::pair containing a pair of iterators defining the wanted range: the first pointing to the first element
   *            that is not less than key and the second pointing to the first element greater than key.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto equal_range(Key const& key) noexcept -> std::pair<iterator, iterator> {
    return std::make_pair(find(key), ++find(key));
  }

  /*!
   * \brief Compare the keys to key. Returns a range containing all elements with the given key in the container.
   * \param[in] key The key to compare the elements to.
   * \return std::pair containing a pair of const_iterators defining the wanted range: the first pointing to the first
   *            element that is not less than key and the second pointing to the first element greater than key.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto equal_range(Key const& key) const noexcept -> std::pair<const_iterator, const_iterator> {
    return std::make_pair(find(key), ++find(key));
  }

  /*!
   * \brief Return an iterator pointing to the first element that is not less than (i.e. greater or equal to) key.
   * \param[in] key key value to compare the elements to
   * \return Iterator pointing to the first element that is not less than key. If no such element is found,
   *            a past-the-end iterator (see end()) is returned.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto lower_bound(Key const& key) noexcept -> iterator {
    std::pair<Index, bool> const search_result{SearchNode(key)};
    iterator it{search_result.first, management_, data_, *this};
    if (!search_result.second) {
      if (data_[search_result.first].key() < key) {
        ++it;
      }
    }
    return it;
  }

  /*!
   * \brief Return an const_iterator pointing to the first element that is not less than (greater or equal to) key.
   * \param[in] key key value to compare the elements to
   * \return Const_iterator pointing to the first element that is not less than key. If no such element is found,
   *            a past-the-end iterator (see end()) is returned.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto lower_bound(Key const& key) const noexcept -> const_iterator {
    std::pair<Index, bool> const search_result{SearchNode(key)};
    const_iterator it{search_result.first, management_, data_, *this};
    if (!search_result.second) {
      if (data_[search_result.first].key() < key) {
        ++it;
      }
    }
    return it;
  }

  /*!
   * \brief Return an iterator pointing to the first element that is greater than key.
   * \param[in] key The key to compare the elements to
   * \return Iterator pointing to the first element that is greater than key. If no such element is found,
   *            a past-the-end iterator (see end()) is returned.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto upper_bound(Key const& key) noexcept -> iterator {
    std::pair<Index, bool> const search_result{SearchNode(key)};
    iterator it{search_result.first, management_, data_, *this};
    return ++it;
  }

  /*!
   * \brief Return an const_iterator pointing to the first element that is not greater than key.
   * \param[in] key The key to compare the elements to
   * \return Const_iterator pointing to the first element that is greater than key. If no such element is found,
   *            a past-the-end iterator (see end()) is returned.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto upper_bound(Key const& key) const noexcept -> const_iterator {
    std::pair<Index, bool> const search_result{SearchNode(key)};
    const_iterator it{search_result.first, management_, data_, *this};
    return ++it;
  }

  /*!
   * \brief Identify the max. key contained by the map.
   * \return The index of the node with the maximum key.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate
   */
  auto GetMaximumIndex() const noexcept -> Index { return GetMaximumKeyIndex(header_.root_); }

  /*!
   * \brief Identify the max. key contained by the map.
   * \return The index of the node with the minimum key.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate
   */
  auto GetMinimumIndex() const noexcept -> Index { return GetMinimumKeyIndex(header_.root_); }

  /*!
   * \brief Provide access to the header segment where size, root node & free list root node are stored.
   * \return Reference to the underlying header struct.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate
   */
  auto HeaderSegment() const noexcept -> struct header const& { return header_; }

  /*!
   * \brief Provide access to the management segment where node links are stored.
   * \return Reference to the underlying management array.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate
   */
  auto ManagementSegment() const noexcept -> amsr::core::Array<TriviallyCopyableMapManagementNode, kCapacity> const& {
    return management_;
  }

  /*!
   * \brief Provide access to the data segment where key and data are stored.
   * \return Reference to the underlying data array.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate
   */
  auto DataSegment() const noexcept
      -> amsr::core::Array<WritableKeyValueStorage<key_type, mapped_type>, kCapacity> const& {
    return data_;
  }

 private:
  /*!
   * \brief Search for a key in the map.
   * \param[in] key key value of the element to search for
   * \return pair containing the index of the searched key or one before if the index was not found and additionally
   *            a boolean flag if a node with this key exists or not. (index < kCapacity || index == kEmptyIndex)
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  auto SearchNode(Key const& key) const noexcept -> std::pair<Index, bool> const {
    Index temp{header_.root_};
    Index prev{kEmptyIndex};
    bool found{false};
    while (temp != kEmptyIndex) {
      prev = temp;
      if (key < data_[temp].key()) {
        temp = management_[temp].left_;
      } else if (key > data_[temp].key()) {
        temp = management_[temp].right_;
      } else {
        found = true;
        break;
      }
    }
    // VCA_ZEROCOPYCOMMON_ANO_01_HOLDS_ONLY_VALID_INDICES
    //@ assert (prev < kCapacity) || (prev == kEmptyIndex);
    return std::make_pair(prev, found);
  }

  /*!
   * \brief Return the next free node and updates the free list.
   * \return Index of the next free node in free list
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  auto GetNextFreeNode() noexcept -> Index {
    Index const index_result{header_.free_};
    if (index_result != kEmptyIndex) {
      header_.free_ = management_[index_result].right_;
      ResetManagementNode(index_result);
    } else {
      amsr::core::Abort("Trivially copyable map capacity exceeded.");
    }
    return index_result;
  }

  /*!
   * \brief Insert a node by index to the free list.
   * \param[in] index node index to add to the free list
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  void AddNodeToFreeList(Index const& index) noexcept {
    ResetManagementNode(index);
    if (header_.free_ != kEmptyIndex) {
      management_[index].right_ = header_.free_;
    }
    header_.free_ = index;
  }

  /*!
   * \brief Reset the management information of a node.
   * \param[in] index node index
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  void ResetManagementNode(Index const& index) noexcept {
    // VCA_ZEROCOPYCOMMON_ANO_01_HOLDS_ONLY_VALID_INDICES
    //@ assert index < kCapacity;
    TriviallyCopyableMapManagementNode& node{management_[index]};
    node.right_ = kEmptyIndex;
    node.left_ = kEmptyIndex;
    node.parent_ = kEmptyIndex;
  }

  /*!
   * \brief Erase a node which has no child.
   * \param[in] index node index
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  void EraseNodeWithNoChild(Index const& index) noexcept {
    Index& parent{management_[index].parent_};
    if (parent == kEmptyIndex) {
      header_.root_ = kEmptyIndex;
    } else {
      if (management_[parent].right_ == index) {
        management_[parent].right_ = kEmptyIndex;
      } else {
        management_[parent].left_ = kEmptyIndex;
      }
    }
  }

  /*!
   * \brief Erase a node which has only one child (either left or right).
   * \param[in] index node index
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  void EraseNodeWithOneChild(Index const& index) noexcept {
    Index& parent{management_[index].parent_};
    Index child{kEmptyIndex};

    if (management_[index].right_ == kEmptyIndex) {
      child = management_[index].left_;
    } else {
      child = management_[index].right_;
    }

    if (parent == kEmptyIndex) {
      header_.root_ = child;
      management_[child].parent_ = kEmptyIndex;
    } else {
      if (management_[parent].right_ == index) {
        management_[parent].right_ = child;
      } else {
        management_[parent].left_ = child;
      }
      management_[child].parent_ = parent;
    }
  }

  /*!
   * \brief Erase a node which has two child nodes, left and right.
   * \param[in] index Node index
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  void EraseNodeWithTwoChilds(Index const& index) noexcept {
    Index& parent{management_[index].parent_};
    // VCA_ZEROCOPYCOMMON_LNG_01_VALID_REFERENCE_ARGUMENT
    Index const successor_index{GetMinimumKeyIndex(management_[index].right_)};

    if (management_[successor_index].right_ == kEmptyIndex) {
      EraseNodeWithNoChild(successor_index);
    } else {
      EraseNodeWithOneChild(successor_index);
    }

    if (parent == kEmptyIndex) {
      header_.root_ = successor_index;
    } else {
      if (management_[parent].right_ == index) {
        management_[parent].right_ = successor_index;
      } else {
        management_[parent].left_ = successor_index;
      }
    }
    // Set management in successor node
    management_[successor_index].parent_ = parent;
    management_[successor_index].right_ = management_[index].right_;
    management_[successor_index].left_ = management_[index].left_;
    // Set parent in child nodes
    Index const right_child{management_[successor_index].right_};
    if (right_child != kEmptyIndex) {
      management_[right_child].parent_ = successor_index;
    }
    Index const left_child{management_[successor_index].left_};
    // When a node with two child nodes is erased we always use the successor node (right side) to replace it.
    // This means that the left side is unmodified and left child of the erased node must be always a valid node
    // inside this function. Therefore, a check "left_child != kEmptyIndex" is unnecessary.
    management_[left_child].parent_ = successor_index;
  }

  /*!
   * \brief Get the index of the successor node (by key) of a given position.
   * \param[in] index Local root index
   * \return successor node is returned or kEmptyIndex on failure
   * \context ANY
   * \pre Invalid index (>= kCapacity or kEmptyIndex) must not be passed to the function.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  auto GetMinimumKeyIndex(Index const& index) const noexcept -> Index {
    Index result{index};
    if (index == kEmptyIndex) {
      result = kEmptyIndex;
    } else {
      while ((management_[result].left_ != kEmptyIndex)) {
        result = management_[result].left_;
      }
    }
    return result;
  }

  /*!
   * \brief Get the index of the preceding node (by key) of a given position.
   * \param[in] index Local root index
   * \return Index of the preceding node
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  auto GetMaximumKeyIndex(Index const& index) const noexcept -> Index {
    Index result{index};
    while (management_[result].right_ != kEmptyIndex) {
      result = management_[result].right_;
    }
    return result;
  }

  /*!
   * \brief Inplace header segment of the TriviallyCopyableMap.
   */
  struct header header_ {};

  /*!
   * \brief Management segment with size of template parameter.
   */
  amsr::core::Array<TriviallyCopyableMapManagementNode, kCapacity>
      management_{};  // VCA_ZEROCOPYCOMMON_SLC_12_ACCESS_INBOUND

  /*!
   * \brief Data segment with size of template parameter containing key + data for each node.
   */
  amsr::core::Array<WritableKeyValueStorage<Key, T>, kCapacity> data_{};  // VCA_ZEROCOPYCOMMON_LNG_03_VALID_REFERENCE
};

/*!
 * \brief Compare two maps for equality of the key-value pairs.
 * \param[in] lhs First map to be compared.
 * \param[in] rhs Second map to be compared.
 * \return True, if the map elements are equal.
 * \context ANY
 * \pre -
 *
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
template <typename Key, typename T, std::size_t kCapacityLeft, std::size_t kCapacityRight>
bool operator==(TriviallyCopyableMap<Key, T, kCapacityLeft> const& lhs,
                TriviallyCopyableMap<Key, T, kCapacityRight> const& rhs) noexcept {
  bool compare_result{true};
  if (lhs.size() == rhs.size()) {
    typename TriviallyCopyableMap<Key, T, kCapacityLeft>::const_iterator lhs_iterator{lhs.cbegin()};
    typename TriviallyCopyableMap<Key, T, kCapacityRight>::const_iterator rhs_iterator{rhs.cbegin()};
    while (lhs_iterator != lhs.cend()) {
      if ((lhs_iterator->key() != rhs_iterator->key()) || (lhs_iterator->value() != rhs_iterator->value())) {
        compare_result = false;
        break;
      }
      ++lhs_iterator;
      ++rhs_iterator;
    }
  } else {
    compare_result = false;
  }
  return compare_result;
}

}  // namespace container
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_TRIVIALLY_COPYABLE_CONTAINER_INCLUDE_AMSR_ZERO_COPY_COMMON_CONTAINER_TRIVIALLY_COPYABLE_MAP_H_
