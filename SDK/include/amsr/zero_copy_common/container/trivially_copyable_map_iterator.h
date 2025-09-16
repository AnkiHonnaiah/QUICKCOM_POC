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
/*!       \file
 *        \brief  Implementation of a trivially copyable map iterator.
 *        \unit   ZeroCopyCommon::TriviallyCopyableContainer::TriviallyCopyableMap
 *
 *********************************************************************************************************************/
#ifndef LIB_TRIVIALLY_COPYABLE_CONTAINER_INCLUDE_AMSR_ZERO_COPY_COMMON_CONTAINER_TRIVIALLY_COPYABLE_MAP_ITERATOR_H_
#define LIB_TRIVIALLY_COPYABLE_CONTAINER_INCLUDE_AMSR_ZERO_COPY_COMMON_CONTAINER_TRIVIALLY_COPYABLE_MAP_ITERATOR_H_

namespace amsr {
namespace zero_copy_common {
namespace container {

/*!
 * \brief Prototype for TriviallyCopyableMap.
 */
template <typename Key, typename T, std::size_t kCapacity>
class TriviallyCopyableMap;

/*!
 * \brief Prototype for TriviallyCopyableMapManagementNode.
 */
struct TriviallyCopyableMapManagementNode;

/*!
 * \brief Prototype for ReadableKeyValueStorage.
 */
template <typename Key, typename T>
class ReadableKeyValueStorage;

/*!
 * \brief Prototype for WritableKeyValueStorage.
 */
template <typename Key, typename T>
class WritableKeyValueStorage;

// VECTOR NC AutosarC++17_10-A12.4.1: MD_ZEROCOPYCOMMON_A12.4.1_mapNoVirtualDtor
/*!
 * \brief Iterator base class for the TriviallyCopyableMapIterators.
 * \tparam kCapacity Maximum capacity of the map.
 */
template <std::size_t kCapacity>
class TriviallyCopyableMapIteratorBase {
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

 protected:
  /*!
   * \brief Constructor of Iterator Base to set the index.
   * \param[in] index Element index where the iterator points to.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  explicit TriviallyCopyableMapIteratorBase(Index index) : index_(index) {}

  /*!
   * \brief Find min element from the right subtree.
   * \param[in] management Reference to the management structure for search.
   * \return Element from right subtree with minimum key or kEmptyIndex if the right subtree is empty.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  auto FindMinRight(amsr::core::Array<TriviallyCopyableMapManagementNode, kCapacity> const& management) const noexcept
      -> Index {
    Index min_right_index{index_};
    // VCA_ZEROCOPYCOMMON_SPC_15_STD_FUNCTIONS
    if (management.at(min_right_index).right_ == kEmptyIndex) {
      min_right_index = kEmptyIndex;
    } else {
      min_right_index = management.at(min_right_index).right_;
      // VCA_ZEROCOPYCOMMON_SPC_15_STD_FUNCTIONS
      while (management.at(min_right_index).left_ != kEmptyIndex) {
        min_right_index = management.at(min_right_index).left_;
      }
    }
    return min_right_index;
  }

  /*!
   * \brief Find max element from the left subtree.
   * \param[in] management Reference to the management structure for search.
   * \return Element from left subtree with maximum key, or kEmptyIndex if the left subtree is emtpy.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  auto FindMaxLeft(amsr::core::Array<TriviallyCopyableMapManagementNode, kCapacity> const& management) const noexcept
      -> Index {
    Index max_left_index{index_};
    // VCA_ZEROCOPYCOMMON_SPC_15_STD_FUNCTIONS
    if (management.at(max_left_index).left_ == kEmptyIndex) {
      max_left_index = kEmptyIndex;
    } else {
      max_left_index = management.at(max_left_index).left_;
      // VCA_ZEROCOPYCOMMON_SPC_15_STD_FUNCTIONS
      while (management.at(max_left_index).right_ != kEmptyIndex) {
        max_left_index = management.at(max_left_index).right_;
      }
    }
    return max_left_index;
  }

  /*!
   * \brief Find a parent node with immediate large key value.
   * \param[in] management Reference to the management structure for search.
   * \return Parent with larger key value or kEmptyIndex in case such a parent is not found.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  auto FindLargerParent(
      amsr::core::Array<TriviallyCopyableMapManagementNode, kCapacity> const& management) const noexcept -> Index {
    Index drag{index_};
    Index larger_parent{index_};
    // VCA_ZEROCOPYCOMMON_SPC_15_STD_FUNCTIONS
    while ((management.at(larger_parent).parent_ != kEmptyIndex) && (management.at(larger_parent).left_ != drag)) {
      drag = larger_parent;
      larger_parent = management.at(larger_parent).parent_;
    }
    if ((management.at(larger_parent).parent_ == kEmptyIndex) && (management.at(larger_parent).left_ != drag)) {
      larger_parent = kEmptyIndex;
    }
    return larger_parent;
  }

  /*!
   * \brief Find a parent node with immediate smaller key value.
   * \param[in] management Reference to the management structure for search.
   * \return Parent with smaller key value.
   *         The return value is undefined if no parent with smaller key value is found.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  auto FindSmallerParent(
      amsr::core::Array<TriviallyCopyableMapManagementNode, kCapacity> const& management) const noexcept -> Index {
    Index drag{index_};
    Index smaller_parent{index_};
    // VCA_ZEROCOPYCOMMON_SPC_15_STD_FUNCTIONS
    while ((management.at(smaller_parent).parent_ != kEmptyIndex) && (management.at(smaller_parent).right_ != drag)) {
      drag = smaller_parent;
      smaller_parent = management.at(smaller_parent).parent_;
    }
    return smaller_parent;
  }

  /*!
   * \brief Index to the element the iterator currently points to.
   */
  // VECTOR NL VectorC++-V11.0.2: MD_ZEROCOPYCOMMON_V11.0.2_mapClassMemberShallBePrivate
  Index index_{kEmptyIndex};
};

/*!
 * \brief Iterator for this TriviallyCopyableMap.
 * \tparam Key Type for key.
 * \tparam T Type of the value.
 * \tparam kCapacity Maximum capacity of the map.
 */
template <typename Key, typename T, std::size_t kCapacity>
class TriviallyCopyableMapIterator final : TriviallyCopyableMapIteratorBase<kCapacity> {
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
  using value_type = ReadableKeyValueStorage<key_type, mapped_type>;

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
   * \brief Difference type.
   * \vpublic
   */
  using difference_type = std::ptrdiff_t;

  /*!
   * \brief Iterator category tag.
   * \vpublic
   */
  using iterator_category = std::bidirectional_iterator_tag;

  /*!
   * \brief Type of indices to other nodes.
   * \vprivate
   */
  using Index = std::size_t;

  /*!
   * \brief Using the base class index.
   */
  using TriviallyCopyableMapIteratorBase<kCapacity>::index_;

  /*!
   * \brief Value of an empty index.
   * \vprivate
   */
  static constexpr Index kEmptyIndex{std::numeric_limits<Index>::max()};

  /*!
   * \brief Constructor for an iterator.
   * \param[in] index The map element index from which to construct the iterator.
   * \param[in] management A reference to the management segment.
   * \param[in] data A reference to the data segment.
   * \param[in] map A reference to the trivially copyable map to reach their functions.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate
   */
  explicit TriviallyCopyableMapIterator(Index index,
                                        std::array<TriviallyCopyableMapManagementNode, kCapacity>& management,
                                        std::array<WritableKeyValueStorage<Key, T>, kCapacity>& data,
                                        TriviallyCopyableMap<Key, T, kCapacity>& map) noexcept
      : TriviallyCopyableMapIteratorBase<kCapacity>(index), management_(management), data_(data), map_(map) {}

  /*!
   * \brief Copy constructor for an iterator.
   * \param[in] other Iterator to be copied.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate
   */
  TriviallyCopyableMapIterator(TriviallyCopyableMapIterator const& other) noexcept = default;

  /*!
   * \brief Move constructor for an iterator.
   * \param[in] other Iterator to be copied.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate
   */
  TriviallyCopyableMapIterator(TriviallyCopyableMapIterator&& other) noexcept = default;

  /*!
   * \brief Copy assignment operator for an TriviallyCopyableMap iterator.
   * \param[in] other Iterator to be copied.
   * \return Reference to the current iterator object.
   * \context ANY
   * \pre Iterator which is copied must be valid (hold valid references).
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto operator=(TriviallyCopyableMapIterator const& other) & noexcept -> TriviallyCopyableMapIterator& {
    index_ = other.index_;
    management_ = other.management_;  // VCA_ZEROCOPYCOMMON_SLC_10_POINTS_TO_VALID_REFERENCE
    map_ = other.map_;                // VCA_ZEROCOPYCOMMON_SLC_10_POINTS_TO_VALID_REFERENCE
    data_ = other.data_;              // VCA_ZEROCOPYCOMMON_SLC_10_POINTS_TO_VALID_REFERENCE
    return *this;
  };

  /*!
   * \brief Move assignment operator for an TriviallyCopyableMap iterator.
   * \param[in]  other Iterator to be moved.
   * \return Reference to the current iterator object.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto operator=(TriviallyCopyableMapIterator&& other) & noexcept -> TriviallyCopyableMapIterator& = default;

  /*!
   * \brief Default destructor TriviallyCopyableMap iterator.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  ~TriviallyCopyableMapIterator() noexcept = default;

  /*!
   * \brief Compare two iterators for equality.
   * \param[in] other Iterator to compare to.
   * \return True if both iterators point to the same map node.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  bool operator==(TriviallyCopyableMapIterator const& other) const noexcept {
    return ((index_ == other.index_) && (&map_ == &other.map_));
  }

  /*!
   * \brief Compare two iterators for inequality.
   * \param[in] other Iterator to compare to.
   * \return True if both iterators point to different map nodes.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  bool operator!=(TriviallyCopyableMapIterator const& other) const noexcept {
    return ((index_ != other.index_) || (&map_ != &other.map_));
  }

  /*!
   * \brief Return a reference to the contained element.
   * \return A reference to the element type.
   * \context ANY
   * \pre The iterator must not equal the end() iterator.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto operator*() noexcept -> reference {
    if (index_ == kEmptyIndex) {
      amsr::core::Abort(
          "Iterator points to the past-the-last element, which results in undefined behavior for dereferencing.");
    }
    return data_[index_];  // VCA_ZEROCOPYCOMMON_LNG_01_VALID_REFERENCE_ARGUMENT
  }

  /*!
   * \brief Return a pointer to the contained element.
   * \return A pointer to the element type.
   * \context ANY
   * \pre The iterator must not equal the end() iterator.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto operator->() noexcept -> pointer {
    if (index_ == kEmptyIndex) {
      amsr::core::Abort(
          "Iterator points to the past-the-last element, which results in undefined behavior for dereferencing.");
    }
    return &data_[index_];  // VCA_ZEROCOPYCOMMON_LNG_01_VALID_REFERENCE_ARGUMENT
  }

  /*!
   * \brief Pre-increment the underlying iterator.
   * \return A reference to this incremented iterator.
   * \context ANY
   * \pre The end iterator is not incrementable and the behavior is undefined if ++container.end() is evaluated.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto operator++() noexcept -> TriviallyCopyableMapIterator& {
    Index temp_index{index_};
    if (temp_index != kEmptyIndex) {
      temp_index = this->FindMinRight(GetManagement());
      if (temp_index == kEmptyIndex) {
        temp_index = this->FindLargerParent(GetManagement());
      }
      index_ = temp_index;
    } else {
      temp_index = map_.GetMinimumIndex();  // VCA_ZEROCOPYCOMMON_LNG_03_VALID_REFERENCE
    }
    return *this;
  }

  /*!
   * \brief Post-increment the underlying iterator.
   * \return A new iterator which is not incremented yet.
   * \context ANY
   * \pre The end iterator is not incrementable and the behavior is undefined if ++container.end() is evaluated.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto operator++(std::int32_t) noexcept -> TriviallyCopyableMapIterator {
    Index temp_index{index_};
    TriviallyCopyableMapIterator const iterator{index_, GetManagement(), GetData(), GetMap()};
    if (temp_index != kEmptyIndex) {
      temp_index = this->FindMinRight(GetManagement());
      if (temp_index == kEmptyIndex) {
        temp_index = this->FindLargerParent(GetManagement());
      }
      index_ = temp_index;
    } else {
      temp_index = map_.GetMinimumIndex();  // VCA_ZEROCOPYCOMMON_LNG_03_VALID_REFERENCE
    }
    return iterator;
  }

  /*!
   * \brief Pre-decrement the underlying iterator.
   * \return A reference to this decremented iterator.
   * \context ANY
   * \pre The begin iterator is not decrementable and the behavior is undefined if --container.begin() is evaluated.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto operator--() noexcept -> TriviallyCopyableMapIterator& {
    Index temp_index{index_};
    if (temp_index != kEmptyIndex) {
      temp_index = this->FindMaxLeft(GetManagement());
      if (temp_index == kEmptyIndex) {
        temp_index = this->FindSmallerParent(GetManagement());
      }
    } else {
      temp_index = map_.GetMaximumIndex();  // VCA_ZEROCOPYCOMMON_LNG_03_VALID_REFERENCE
    }
    index_ = temp_index;
    return *this;
  }

  /*!
   * \brief Post-decrement the underlying iterator.
   * \return A new iterator which is not decremented yet.
   * \context ANY
   * \pre The begin iterator is not decrementable and the behavior is undefined if --container.begin() is evaluated.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto operator--(std::int32_t) noexcept -> TriviallyCopyableMapIterator {
    Index temp_index{index_};
    TriviallyCopyableMapIterator const iterator{index_, GetManagement(), GetData(), GetMap()};
    if (temp_index != kEmptyIndex) {
      temp_index = this->FindMaxLeft(GetManagement());
      if (temp_index == kEmptyIndex) {
        temp_index = this->FindSmallerParent(GetManagement());
      }
    } else {
      temp_index = map_.GetMaximumIndex();  // VCA_ZEROCOPYCOMMON_LNG_03_VALID_REFERENCE
    }
    index_ = temp_index;
    return iterator;
  }

 private:
  /*!
   * \brief Get the current management_ reference of the map iterator.
   * \return Reference to management_ of the map iterator.
   * \internal
   * Guarantees the VCA contract that the reference is valid.
   * \endinternal
   */
  auto GetManagement() const noexcept -> amsr::core::Array<TriviallyCopyableMapManagementNode, kCapacity>& {
    amsr::core::Array<TriviallyCopyableMapManagementNode, kCapacity>& management_local{management_};

    // VCA_ZEROCOPYCOMMON_LNG_03_VALID_REFERENCE
    return management_local;
  }

  /*!
   * \brief Get the current data_ reference of the map iterator.
   * \return Reference to data_ of the map iterator.
   * \internal
   * Guarantees the VCA contract that the reference is valid.
   * \endinternal
   */
  auto GetData() const noexcept -> amsr::core::Array<WritableKeyValueStorage<key_type, mapped_type>, kCapacity>& {
    amsr::core::Array<WritableKeyValueStorage<key_type, mapped_type>, kCapacity>& data_local{data_};

    // VCA_ZEROCOPYCOMMON_LNG_03_VALID_REFERENCE
    return data_local;
  }

  /*!
   * \brief Get the current map_ reference of the map iterator.
   * \return Reference to map_ of the map iterator.
   * \internal
   * Guarantees the VCA contract that the reference is valid.
   * \endinternal
   */
  auto GetMap() const noexcept -> TriviallyCopyableMap<Key, T, kCapacity>& {
    TriviallyCopyableMap<Key, T, kCapacity>& map_local{map_};

    // VCA_ZEROCOPYCOMMON_LNG_03_VALID_REFERENCE
    return map_local;
  }

  /*!
   * \brief Reference to the map management.
   */
  amsr::core::Array<TriviallyCopyableMapManagementNode, kCapacity>& management_{};

  /*!
   * \brief Reference to the map data.
   */
  amsr::core::Array<WritableKeyValueStorage<key_type, mapped_type>, kCapacity>& data_{};

  /*!
   * \brief Reference to the underlying map.
   */
  TriviallyCopyableMap<Key, T, kCapacity>& map_{};
};

/*!
 * \brief Const iterator for this TriviallyCopyableMap.
 * \tparam Key Type for key.
 * \tparam T Type of the value.
 * \tparam kCapacity Maximum capacity of the map.
 * \tparam Map Type of the map, either a map or a reader.
 */
template <typename Key, typename T, std::size_t kCapacity, typename Map>
class ConstTriviallyCopyableMapIterator final : TriviallyCopyableMapIteratorBase<kCapacity> {
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
   * \brief Type of reference.
   * \vpublic
   */
  using reference = value_type const&;

  /*!
   * \brief Type of const reference.
   * \vpublic
   */
  using const_reference = value_type const&;

  /*!
   * \brief Type of pointer.
   * \vpublic
   */
  using pointer = value_type const*;

  /*!
   * \brief Type of const pointer.
   * \vpublic
   */
  using const_pointer = value_type const*;

  /*!
   * \brief Difference type.
   * \vpublic
   */
  using difference_type = std::ptrdiff_t;

  /*!
   * \brief Iterator category tag.
   * \vpublic
   */
  using iterator_category = std::bidirectional_iterator_tag;

  /*!
   * \brief Type of indices to other nodes.
   * \vprivate
   */
  using Index = std::size_t;

  /*!
   * \brief Using the base class index.
   */
  using TriviallyCopyableMapIteratorBase<kCapacity>::index_;

  /*!
   * \brief Value of an empty index.
   * \vprivate
   */
  static constexpr Index kEmptyIndex{std::numeric_limits<Index>::max()};

  /*!
   * \brief Constructor for an iterator.
   * \param index The map element index from which to construct the iterator.
   * \param[in] management A reference to the management segment.
   * \param[in] data A reference to the data segment.
   * \param[in] map A reference to the map to reach their functions.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate
   */
  explicit ConstTriviallyCopyableMapIterator(
      Index const index, std::array<TriviallyCopyableMapManagementNode, kCapacity> const& management,
      std::array<WritableKeyValueStorage<Key, T>, kCapacity> const& data, Map const& map) noexcept
      : TriviallyCopyableMapIteratorBase<kCapacity>(index), management_(management), data_(data), map_(map) {}

  /*!
   * \brief Compare two iterators for equality.
   * \param[in] other Iterator to compare to.
   * \return True if both iterators point to the same map node.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  bool operator==(ConstTriviallyCopyableMapIterator const& other) const noexcept {
    return ((index_ == other.index_) && (&map_ == &other.map_));
  }

  /*!
   * \brief Compare two iterators for inequality.
   * \param[in] other Iterator to compare to.
   * \return True if both iterators point to different map nodes.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  bool operator!=(ConstTriviallyCopyableMapIterator const& other) const noexcept {
    return ((index_ != other.index_) || (&map_ != &other.map_));
  }

  /*!
   * \brief Return a reference to the contained element.
   * \return A const reference to the element type.
   * \context ANY
   * \pre The iterator must not equal the end() iterator.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto operator*() const noexcept -> const_reference {
    if (index_ == kEmptyIndex) {
      amsr::core::Abort(
          "Iterator points to the past-the-last element, which results in undefined behavior for dereferencing.");
    }
    return data_.at(index_);  // VCA_ZEROCOPYCOMMON_LNG_01_VALID_REFERENCE_ARGUMENT
  }

  /*!
   * \brief Return a pointer to the contained element.
   * \return A pointer to the element type.
   * \context ANY
   * \pre The iterator must not equal the end() iterator.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto operator->() const noexcept -> const_pointer {
    if (index_ == kEmptyIndex) {
      amsr::core::Abort(
          "Iterator points to the past-the-last element, which results in undefined behavior for dereferencing.");
    }
    return &data_.at(index_);  // VCA_ZEROCOPYCOMMON_LNG_01_VALID_REFERENCE_ARGUMENT
  }

  /*!
   * \brief Pre-increment the underlying iterator.
   * \return A reference to this incremented iterator.
   * \context ANY
   * \pre The end iterator is not incrementable and the behavior is undefined if ++container.end() is evaluated.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto operator++() noexcept -> ConstTriviallyCopyableMapIterator& {
    Index temp_index{index_};
    if (temp_index != kEmptyIndex) {
      temp_index = this->FindMinRight(GetManagement());
      if (temp_index == kEmptyIndex) {
        temp_index = this->FindLargerParent(GetManagement());
      }
      index_ = temp_index;
    } else {
      temp_index = map_.GetMinimumIndex();  // VCA_ZEROCOPYCOMMON_LNG_03_VALID_REFERENCE
    }
    return *this;
  }

  /*!
   * \brief Post-increment the underlying iterator.
   * \return A new iterator which is not incremented yet.
   * \context ANY
   * \pre The end iterator is not incrementable and the behavior is undefined if ++container.end() is evaluated.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto operator++(std::int32_t) noexcept -> ConstTriviallyCopyableMapIterator {
    Index temp_index{index_};
    ConstTriviallyCopyableMapIterator const const_iterator{index_, GetManagement(), GetData(), GetMap()};
    if (temp_index != kEmptyIndex) {
      temp_index = this->FindMinRight(GetManagement());
      if (temp_index == kEmptyIndex) {
        temp_index = this->FindLargerParent(GetManagement());
      }
      index_ = temp_index;
    } else {
      temp_index = map_.GetMinimumIndex();  // VCA_ZEROCOPYCOMMON_LNG_03_VALID_REFERENCE
    }
    return const_iterator;
  }

  /*!
   * \brief Pre-decrement the underlying iterator.
   * \return A reference to this decremented iterator.
   * \context ANY
   * \pre The begin iterator is not decrementable and the behavior is undefined if --container.begin() is evaluated.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto operator--() noexcept -> ConstTriviallyCopyableMapIterator& {
    Index temp_index{index_};
    if (temp_index != kEmptyIndex) {
      temp_index = this->FindMaxLeft(GetManagement());
      if (temp_index == kEmptyIndex) {
        temp_index = this->FindSmallerParent(GetManagement());
      }
    } else {
      temp_index = map_.GetMaximumIndex();  // VCA_ZEROCOPYCOMMON_LNG_03_VALID_REFERENCE
    }
    index_ = temp_index;
    return *this;
  }

  /*!
   * \brief Post-decrement the underlying iterator.
   * \return A new iterator which is not decremented yet.
   * \context ANY
   * \pre The begin iterator is not decrementable and the behavior is undefined if --container.begin() is evaluated.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  auto operator--(std::int32_t) noexcept -> ConstTriviallyCopyableMapIterator {
    Index temp_index{index_};
    ConstTriviallyCopyableMapIterator const const_iterator{index_, GetManagement(), GetData(), GetMap()};
    if (temp_index != kEmptyIndex) {
      temp_index = this->FindMaxLeft(GetManagement());
      if (temp_index == kEmptyIndex) {
        temp_index = this->FindSmallerParent(GetManagement());
      }
    } else {
      temp_index = map_.GetMaximumIndex();  // VCA_ZEROCOPYCOMMON_LNG_03_VALID_REFERENCE
    }
    index_ = temp_index;
    return const_iterator;
  }

 private:
  /*!
   * \brief Get the current management_ reference of the map iterator.
   * \return Reference to management_ of the map iterator.
   * \internal
   * Guarantees the VCA contract that the reference is valid.
   * \endinternal
   */
  auto GetManagement() const noexcept -> amsr::core::Array<TriviallyCopyableMapManagementNode, kCapacity> const& {
    amsr::core::Array<TriviallyCopyableMapManagementNode, kCapacity> const& management_local{management_};

    // VCA_ZEROCOPYCOMMON_LNG_03_VALID_REFERENCE
    return management_local;
  }

  /*!
   * \brief Get the current data_ reference of the map iterator.
   * \return Reference to data_ of the map iterator.
   * \internal
   * Guarantees the VCA contract that the reference is valid.
   * \endinternal
   */
  auto GetData() const noexcept -> amsr::core::Array<WritableKeyValueStorage<key_type, mapped_type>, kCapacity> const& {
    amsr::core::Array<WritableKeyValueStorage<key_type, mapped_type>, kCapacity> const& data_local{data_};

    // VCA_ZEROCOPYCOMMON_LNG_03_VALID_REFERENCE
    return data_local;
  }

  /*!
   * \brief Get the current map_ reference of the map iterator.
   * \return Reference to map_ of the map iterator.
   * \internal
   * Guarantees the VCA contract that the reference is valid.
   * \endinternal
   */
  auto GetMap() const noexcept -> Map const& {
    Map const& map_local{map_};

    // VCA_ZEROCOPYCOMMON_LNG_03_VALID_REFERENCE
    return map_local;
  }

  /*!
   * \brief Reference to the map management.
   */
  amsr::core::Array<TriviallyCopyableMapManagementNode, kCapacity> const& management_{};

  /*!
   * \brief Reference to the map data.
   */
  amsr::core::Array<WritableKeyValueStorage<key_type, mapped_type>, kCapacity> const& data_{};

  /*!
   * \brief Reference to the underlying map.
   */
  Map const& map_{};
};

}  // namespace container
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_TRIVIALLY_COPYABLE_CONTAINER_INCLUDE_AMSR_ZERO_COPY_COMMON_CONTAINER_TRIVIALLY_COPYABLE_MAP_ITERATOR_H_
