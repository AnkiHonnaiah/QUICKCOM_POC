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
 *        \brief  Implementation of a read-only wrapper around a trivially copyable map.
 *        \unit   ZeroCopyCommon::TriviallyCopyableContainer::TriviallyCopyableMap
 *
 *********************************************************************************************************************/
#ifndef LIB_TRIVIALLY_COPYABLE_CONTAINER_INCLUDE_AMSR_ZERO_COPY_COMMON_CONTAINER_TRIVIALLY_COPYABLE_MAP_READER_H_
#define LIB_TRIVIALLY_COPYABLE_CONTAINER_INCLUDE_AMSR_ZERO_COPY_COMMON_CONTAINER_TRIVIALLY_COPYABLE_MAP_READER_H_

#include <cstddef>
#include <cstring>
#include <iterator>
#include <limits>
#include <type_traits>

#include "amsr/core/abort.h"
#include "amsr/zero_copy_common/container/trivially_copyable_container_error_domain.h"
#include "amsr/zero_copy_common/container/trivially_copyable_map.h"
#include "ara/core/result.h"

namespace amsr {
namespace zero_copy_common {
namespace container {

// VECTOR NC Metric-OO.WMC.One: MD_ZEROCOPYCOMMON_Metric-OO.WMC.One_containersModeledAfterStandardLibrary
/*!
 * \brief A read-only wrapper around a trivially copyable map instance that ensures safe bounds
 * \details A trivially copyable map might be shared between processes of different qualification levels.
 *          This class is constructed as a wrapper around a potentially untrusted instance of a map,
 *          i.e. it is created based on a map instance that might contain corrupted data.
 *          The wrapper allows read-only access to this underlying instance and ensures that no out of bounds
 *          memory access happens. Concretely, the map size & node links are checked so no out of bounds access happens.
 *          WARNING: A wrapper instance is tightly bound to its underlying map. It is the users responsibility
 *                   to ensure the wrapper is not used after the underlying map has changed/is freed.
 *
 * \trace DSGN-ZeroCopyCommon-TriviallyCopyableMap-ReadOnlyWrapper
 *
 * \tparam Key Key type stored in the map. Must be trivially copyable and trivially destructible.
 * \tparam T Value type stored in the map. Must be trivially copyable and trivially destructible.
 * \tparam kCapacity Maximum capacity of the map. Must be >= 1. Must be < std::numeric_limits<size_t>::max() / 2.
 *
 * \vpublic
 */
template <typename Key, typename T, size_t kCapacity>
class TriviallyCopyableMapReader final {
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

 public:
  /*!
   * \brief Typedef for a typetrait checking if the given Iterator type is an input iterator.
   * \tparam Iterator Type to be checked to be an input iterator.
   */
  template <typename Iterator>
  using IsInputIterator =
      std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>;

  /*!
   * \brief Typedef for the vector size.
   * \vpublic
   */
  using size_type = typename TriviallyCopyableMap<Key, T, kCapacity>::size_type;

  /*!
   * \brief Typedef for the contained element type.
   * \vpublic
   */
  using value_type = typename TriviallyCopyableMap<Key, T, kCapacity>::value_type;

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
   * \brief Typedef for constant references to contained elements.
   * \vpublic
   */
  using const_reference = typename TriviallyCopyableMap<Key, T, kCapacity>::const_reference;

  /*!
   * \brief Typedef for a constant pointer to contained elements.
   * \vpublic
   */
  using const_pointer = typename TriviallyCopyableMap<Key, T, kCapacity>::const_pointer;

  /*!
   * \brief Typedef for const iterators.
   * \vpublic
   */
  using const_iterator = ConstTriviallyCopyableMapIterator<Key, T, kCapacity, TriviallyCopyableMapReader>;

  /*!
   * \brief Type of indices to other nodes.
   * \vprivate
   */
  using Index = std::size_t;

  /*!
   * \brief Value of an empty index.
   * \vprivate
   */
  static constexpr Index kEmptyIndex{std::numeric_limits<Index>::max()};

  /*!
   * \brief Create a safe read-only wrapper around the given map instance.
   * \param[in] map The map being wrapped. The passed map must outlive the created read-only wrapper.
   * \return Safe wrapper instance around the given map.
   * \error TriviallyCopyableContainerErrc::kContainerCorrupted The given map is corrupted,
   *        i.e. its size() > kCapacity, which could lead to invalid memory accesses.
   * \context ANY
   * \pre -
   * \threadsafe TRUE, if the passed map object instance is used exclusively by this thread.
   * \reentrant TRUE, if different map object instances are used for each call.
   * \synchronous TRUE
   * \vpublic
   */
  static auto FromTriviallyCopyableMap(TriviallyCopyableMap<Key, T, kCapacity> const& map)
      -> ara::core::Result<TriviallyCopyableMapReader> {
    using Result = ara::core::Result<TriviallyCopyableMapReader<Key, T, kCapacity>>;
    Result result{TriviallyCopyableContainerErrc::kDefaultError};

    typename TriviallyCopyableMap<Key, T, kCapacity>::header_segment_type const header{map.HeaderSegment()};
    typename TriviallyCopyableMap<Key, T, kCapacity>::management_segment_type const management{map.ManagementSegment()};
    typename TriviallyCopyableMap<Key, T, kCapacity>::data_segment_type const& data{map.DataSegment()};

    if (CheckMap(header, management, header.size_)) {
      result.EmplaceValue(TriviallyCopyableMapReader(header, management, data));
    } else {
      result.EmplaceError(TriviallyCopyableContainerErrc::kContainerCorrupted);
    }

    return result;
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

  /*!
   * \brief Return a reference to the mapped value with the key.
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
  auto operator[](Key const& key) const noexcept -> mapped_type const& {
    std::pair<Index, bool> const search_result{SearchNode(key)};
    if (!search_result.second) {
      amsr::core::Abort("The passed key doesn't exist!");
    }
    return data_[search_result.first].value();  // VCA_ZEROCOPYCOMMON_LNG_02_REFERENCE_VALID
  }

  /*!
   * \brief Return a const_iterator pointing to the element with the lowest key of this map.
   * \return A const_iterator to the first element of this TriviallyCopyableMap
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
    // VCA_ZEROCOPYCOMMON_LNG_01_VALID_REFERENCE_ARGUMENT
    return const_iterator(first_index, management_, data_, *this);
  }

  /*!
   * \brief Return a const_iterator pointing to the element with the lowest key of this map.
   * \return A const_iterator to the first element of this TriviallyCopyableMap
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
  auto end() const noexcept -> const_iterator {
    // VCA_ZEROCOPYCOMMON_LNG_01_VALID_REFERENCE_ARGUMENT
    return const_iterator(kEmptyIndex, management_, data_, *this);
  }

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
   * \brief Return the number of elements with key that compares equivalent to the specified argument. (1 or 0)
   * \param[in] key key value of the elements to count
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
    // VCA_ZEROCOPYCOMMON_LNG_01_VALID_REFERENCE_ARGUMENT
    return const_iterator(result_index, management_, data_, *this);
  }

  /*!
   * \brief Compare the keys to key. Returns a range containing all elements with the given key in the container.
   * \param[in] key The key to compare the elements to
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
   * \brief Return a const_iterator pointing to the first element that is not less than (greater or equal to) key.
   * \param[in] key The key to compare the elements to
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
    // VCA_ZEROCOPYCOMMON_LNG_01_VALID_REFERENCE_ARGUMENT
    const_iterator it{search_result.first, management_, data_, *this};
    if (!search_result.second) {
      if (data_[search_result.first].key() < key) {
        ++it;
      }
    }
    return it;
  }

  /*!
   * \brief Return a const_iterator pointing to the first element that is not greater than key.
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
    // VCA_ZEROCOPYCOMMON_LNG_01_VALID_REFERENCE_ARGUMENT
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

 private:
  /*!
   * \brief Construct a map with the given members.
   */
  TriviallyCopyableMapReader(typename TriviallyCopyableMap<Key, T, kCapacity>::header_segment_type const header,
                             typename TriviallyCopyableMap<Key, T, kCapacity>::management_segment_type const management,
                             typename TriviallyCopyableMap<Key, T, kCapacity>::data_segment_type const& data)
      : header_{header}, management_{management}, data_{data} {}

  /*!
   * \brief Check the map for correctness. All node indices are inbound, there are no cycles, number of nodes equals
   *        size.
   * \param header Reference to the header segment.
   * \param management Reference to the management segment.
   * \param size Reference to the size of the map.
   * \return False, if the map is corrupted.
   */
  static auto CheckMap(typename TriviallyCopyableMap<Key, T, kCapacity>::header_segment_type const& header,
                       typename TriviallyCopyableMap<Key, T, kCapacity>::management_segment_type const& management,
                       size_t const& size) -> bool {
    bool result{true};
    size_t iterations{0};

    if ((size == 0) && (header.root_ == kEmptyIndex)) {
      result = true;
    } else {
      Index const current_index{header.root_};
      Index const parent_index{kEmptyIndex};
      result = IsNodeCorrect(management, current_index, parent_index, iterations, size);
    }

    if (iterations != size) {
      result = false;
    }

    return result;
  }

  // VECTOR NC AutosarC++17_10-A7.5.2: MD_ZEROCOPYCOMMON_A7.5.2_recursiveFunctionCall
  // VECTOR NC AutosarC++17_10-A8.4.4: MD_ZEROCOPYCOMMON_A8.4.4_mapOutputParameterUsed
  /*!
   * \brief Check each node for correctness considering out-of-bounds, cycles & node links.
   * \param management Reference to the map's management data.
   * \param current_index Reference to the index of the node to check.
   * \param parent_index Reference to the parent index of the current node to check.
   * \param iterations Reference to the iterations already proceeded.
   * \param size Reference to the map size.
   * \return False, if the node is corrupted.
   */
  static auto IsNodeCorrect(typename TriviallyCopyableMap<Key, T, kCapacity>::management_segment_type const& management,
                            Index const& current_index, Index const& parent_index, size_t& iterations,
                            size_t const& size) -> bool {
    bool result{true};
    ++iterations;

    // VCA_ZEROCOPYCOMMON_SPC_15_STD_FUNCTIONS
    if ((current_index >= kCapacity) || (iterations > size) || (management.at(current_index).parent_ != parent_index)) {
      result = false;
    } else {
      // VCA_ZEROCOPYCOMMON_SPC_15_STD_FUNCTIONS
      if (management.at(current_index).left_ != kEmptyIndex) {
        // VCA_ZEROCOPYCOMMON_LNG_01_VALID_REFERENCE_ARGUMENT, VCA_ZEROCOPYCOMMON_SPC_15_STD_FUNCTIONS
        result = IsNodeCorrect(management, management.at(current_index).left_, current_index, iterations, size);
      }
      // VCA_ZEROCOPYCOMMON_SPC_15_STD_FUNCTIONS
      if (result && (management.at(current_index).right_ != kEmptyIndex)) {
        // VCA_ZEROCOPYCOMMON_LNG_01_VALID_REFERENCE_ARGUMENT, VCA_ZEROCOPYCOMMON_SPC_15_STD_FUNCTIONS
        result = IsNodeCorrect(management, management.at(current_index).right_, current_index, iterations, size);
      }
    }
    return result;
  }

  /*!
   * \brief Search for a key in the map.
   * \param[in] key The key of the element to search for
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
      // VCA_ZEROCOPYCOMMON_SPC_15_STD_FUNCTIONS
      if (key < data_.at(temp).key()) {
        temp = management_.at(temp).left_;
      } else if (key > data_.at(temp).key()) {
        temp = management_.at(temp).right_;
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
   * \brief Get the index of the successor node (by key) of a given position.
   * \param index local root index
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
      // VCA_ZEROCOPYCOMMON_SPC_15_STD_FUNCTIONS
      while ((management_.at(result).left_ != kEmptyIndex)) {
        result = management_.at(result).left_;
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
    // VCA_ZEROCOPYCOMMON_SPC_15_STD_FUNCTIONS
    while (management_.at(result).right_ != kEmptyIndex) {
      result = management_.at(result).right_;
    }
    return result;
  }

  /*!
   * \brief Inplace header segment of the TriviallyCopyableMap.
   */
  typename TriviallyCopyableMap<Key, T, kCapacity>::header_segment_type const header_{};

  /*!
   * \brief Management segment with size of template parameter.
   */
  typename TriviallyCopyableMap<Key, T, kCapacity>::management_segment_type const management_{};

  /*!
   * \brief Data segment with size of template parameter containing key + data for each node.
   */
  typename TriviallyCopyableMap<Key, T, kCapacity>::data_segment_type const& data_{};
};

/*!
 * \brief Create a safe read-only wrapper around the given map.
 * \tparam Key Key type stored in the map.
 * \tparam T Value type stored in the map.
 * \tparam kCapacity Maximum capacity of the map.
 * \param map The map being wrapped. The passed map must outlive the created read-only wrapper.
 * \context ANY
 * \return Safe wrapper instance around the given map.
 * \error TriviallyCopyableContainerErrc::kContainerCorrupted The given vector is corrupted,
 *        i.e. its size() > kMaxCapacity, which could lead to invalid memory accesses.
 * \pre -
 * \threadsafe TRUE, if the passed map object instance is used exclusively by this thread.
 * \reentrant TRUE, if different map object instances are used for each call.
 * \synchronous TRUE
 * \vpublic
 */
template <typename Key, typename T, size_t kCapacity>
// VECTOR NL AutosarC++17_10-A2.11.4: MD_ZEROCOPYCOMMON_A2.11.4_nameOrObjectReused
auto ReadOnlyWrapper(TriviallyCopyableMap<Key, T, kCapacity> const& map) noexcept
    -> ara::core::Result<TriviallyCopyableMapReader<Key, T, kCapacity>> {
  return TriviallyCopyableMapReader<Key, T, kCapacity>::FromTriviallyCopyableMap(map);
}

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
bool operator==(TriviallyCopyableMapReader<Key, T, kCapacityLeft> const& lhs,
                TriviallyCopyableMapReader<Key, T, kCapacityRight> const& rhs) noexcept {
  bool compare_result{true};
  if (lhs.size() == rhs.size()) {
    typename TriviallyCopyableMapReader<Key, T, kCapacityLeft>::const_iterator lhs_iterator{lhs.cbegin()};
    typename TriviallyCopyableMapReader<Key, T, kCapacityRight>::const_iterator rhs_iterator{rhs.cbegin()};
    while (lhs_iterator != lhs.cend()) {
      if (((*lhs_iterator).key() != (*rhs_iterator).key()) || ((*lhs_iterator).value() != (*rhs_iterator).value())) {
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

#endif  // LIB_TRIVIALLY_COPYABLE_CONTAINER_INCLUDE_AMSR_ZERO_COPY_COMMON_CONTAINER_TRIVIALLY_COPYABLE_MAP_READER_H_
