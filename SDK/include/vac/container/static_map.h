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
/*!        \file  static_map.h
 *        \brief  Contains StaticMap class.
 *         \unit  VaCommonLib::ContainerLibrary::StaticContainers::StaticMap
 *
 *      \details  The StaticMap class can be used to store items derived from StaticMap::Node.
 *                Before adding nodes the number of supported nodes has to be specified.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_CONTAINER_STATIC_MAP_H_
#define LIB_VAC_INCLUDE_VAC_CONTAINER_STATIC_MAP_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <iterator>
#include <memory>
#include <utility>
#include <vector>

#include "vac/container/intrusive_map.h"
#include "vac/memory/object_pool.h"

namespace vac {
namespace container {

/*!
 * \brief   Class to implement a StaticMap.
 *          Map Nodes must inherit from StaticMap::Node.
 *          Before adding nodes the number of supported nodes has to be reserved.
 * \tparam  Key The key type. Must implement Key::operator<(Key) used to check whether two Keys are the same.
 * \tparam  T The value type. Must fulfill: C++ named requirements: CopyConstructible, Erasable.
 * \remarks The execution sequence of key and the value type is not specified by C++11. Therefore do not expect a
 *          specific execution order of the key or the value constructor. Also do not use key and value objects that
 *          have dependencies to each other.
 * \trace   CREQ-VaCommonLib-StaticMap
 */
template <typename Key, typename T>
class StaticMap final {
 public:
  /*!
   * \brief The type implementing the pair.
   */
  using value_type = std::pair<Key const, T>;

  /*!
   * \brief The key type of this map.
   */
  using key_type = Key;

  /*!
   * \brief The mapped type of this map.
   */
  using mapped_type = T;

  /*!
   * \brief Type for nodes of the static Map.
   *        Key type for key in the pair
   *        T type of the value in the pair.
   */
  class Node final : public vac::container::IntrusiveMapNode<Key, Node> {
   public:
    /*!
     * \brief Type of compare result.
     */
    using CompareType = std::int32_t;

    /*!
     * \brief Constructor for a node without argument.
     */
    Node() = delete;

    /*!
     * \brief Copy constructor.
     */
    Node(Node const&) = delete;

    /*!
     * \brief     Move constructor.
     * \param[in] other_node Node to be moved.
     */
    Node(Node&& other_node) = delete;

    /*!
     * \brief  Deleted copy assignment.
     * \return A reference to the assigned to object.
     */
    Node& operator=(Node const&) & = delete;

    /*!
     * \brief  Deleted move assignment.
     * \return A reference to the assigned to object.
     */
    Node& operator=(Node&&) & = delete;

    // VECTOR NL VectorC++-V2.7.6: MD_VAC_V2-7-6_cCommentShouldBeDoxygen
    /*VCAST_DONT_INSTRUMENT_START*/
    // VECTOR NL VectorC++-V2.7.6: MD_VAC_V2-7-6_cCommentShouldBeDoxygen
    /*VCAST_INSTRUMENTATION_ISSUE_REFERENCE: TAR-24033*/
    // VECTOR Next Construct VectorC++-V3.9.2: MD_VAC_V3-9-2_parametersBasedOnBuiltInTypes
    /*!
     * \brief     Forwarding constructor.
     * \details   Terminates if the constructor of T throws.
     * \tparam    Args Types of the arguments for the constructor of the Key type
     * \param[in] args Arguments for the constructor of the Key type.
     * \pre       -
     */
    template <typename... Args>
    explicit Node(Args&&... args) noexcept  // NOLINT(runtime/explicit)
        : vac::container::IntrusiveMapNode<Key, Node>(),
          storage_(std::forward<Args>(args)...) {}  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    // VECTOR NL VectorC++-V2.7.6: MD_VAC_V2-7-6_cCommentShouldBeDoxygen
    /*VCAST_DONT_INSTRUMENT_END*/

    // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
    /*!
     * \brief Destructor.
     * \spec
     *   requires true;
     * \endspec
     */
    ~Node() noexcept final = default;  // VCA_VAC_VALID_NONSTATIC_METHOD, VCA_VAC_SAFE_DEALLOCATION

    /*!
     * \brief  Compares the new key value with the node current node value.
     * \param  new_key Value to be compared.
     * \return A negative value if new key is less than the other current node key, zero if the both keys are equal,
     *         positive value if new key is greater than the other current node key.
     * \pre    -
     * \spec
     *   requires true;
     * \endspec
     */
    CompareType KeyCompare(StaticMap::key_type new_key) const noexcept final {
      CompareType retval{0};
      if (storage_.first < new_key) {
        retval = 1;
      } else if (new_key < storage_.first) {
        retval = -1;
      } else {
        retval = 0;
      }
      return retval;
    }

    /*!
     * \brief  Compares the new key value with the node current node value.
     * \param  key_node Node containing the key to be compared to.
     * \return A negative value if new key is less than the other current node key, zero if the both keys are equal,
     *         positive value if new key is greater than the other current node key.
     * \pre    -
     * \spec
     *   requires true;
     * \endspec
     */
    CompareType KeyCompare(Node const& key_node) const noexcept final {
      return this->KeyCompare(key_node.storage_.first);
    }

    /*!
     * \brief  Get a reference to the stored key and value.
     * \pre    -
     * \spec
     *   requires true;
     * \endspec
     * \return A reference to the stored key and value.
     */
    value_type& GetStorage() noexcept { return storage_; }

    /*!
     * \brief  Get a const reference to the stored key and value.
     * \pre    -
     * \spec
     *   requires true;
     * \endspec
     * \return A const reference to the stored key and value.
     */
    value_type const& GetStorage() const noexcept { return storage_; }

   private:
    /*!
     * \brief Pair to store the key and value.
     */
    value_type storage_;
  };

  /*!
   * \brief The memory management type used to allocate Node objects.
   * \trace CREQ-VaCommonLib-StaticMap
   */
  using StorageType = vac::memory::ObjectPool<Node, std::allocator<Node>>;

  /*!
   * \brief The size type used in this implementation.
   */
  using size_type = typename StorageType::size_type;

  /*!
   * \brief The type of the map used to store elements.
   */
  using MapType = vac::container::IntrusiveMap<Key, Node>;

  /*!
   * \brief Typedef for the iterator type of this map.
   * \trace CREQ-VaCommonLib-StaticMap
   */
  class iterator final {
   public:
    /*! \brief Category. */
    using iterator_category = std::bidirectional_iterator_tag;
    /*! \brief Value type. */
    using value_type = StaticMap::value_type;
    /*! \brief Difference type. */
    using difference_type = std::ptrdiff_t;
    /*! \brief Pointer type. */
    using pointer = value_type*;
    /*! \brief Reference type. */
    using reference = value_type&;

    /*! \brief Const reference type. */
    using const_reference = value_type const&;
    /*! \brief Const pointer type. */
    using const_pointer = value_type const*;

    /*!
     * \brief Typedef for the iterator of the container.
     */
    using base_iterator = typename MapType::iterator;

    // VECTOR Next Construct AutosarC++17_10-A12.1.5: MD_VAC_A12.1.5_useDelegatingConstructor
    /*!
     * \brief Construct an iterator from a base_iterator.
     * \param it The base iterator from which to construct the object.
     * \pre   -
     * \spec
     *   requires true;
     * \endspec
     */
    explicit iterator(base_iterator const& it) noexcept : it_(it) {}

    /*!
     * \brief     Construct an iterator from a base_iterator.
     * \param[in] it The base iterator from which to construct the object.
     * \pre       -
     * \spec
     *   requires true;
     * \endspec
     */
    explicit iterator(base_iterator&& it) noexcept : it_(std::move(it)) {}

    /*!
     * \brief  Dereference iterator to map element.
     * \pre    -
     * \spec
     *   requires true;
     * \endspec
     * \return The map element that current iterator points to.
     */
    reference operator*() noexcept { return it_.GetMapNode()->GetSelf()->GetStorage(); }  // VCA_VAC_VALID_RETURN_VALUE

    /*!
     * \brief  Dereference iterator to map element.
     * \pre    -
     * \spec
     *   requires true;
     * \endspec
     * \return The map element that current iterator points to.
     */
    const_reference operator*() const noexcept {
      return it_.GetMapNode()->GetSelf()->GetStorage();  // VCA_VAC_VALID_RETURN_VALUE
    }

    /*!
     * \brief  Dereference iterator to map element.
     * \pre    -
     * \spec
     *   requires true;
     * \endspec
     * \return The map element that current iterator points to.
     */
    pointer operator->() noexcept { return &(it_.GetMapNode()->GetSelf()->GetStorage()); }

    /*!
     * \brief  Dereference iterator to map element.
     * \pre    -
     * \spec
     *   requires true;
     * \endspec
     * \return The map element that current iterator points to.
     */
    const_pointer operator->() const noexcept { return &(it_.GetMapNode()->GetSelf()->GetStorage()); }

    /*!
     * \brief  Increment the iterator by one element.
     * \pre    -
     * \spec
     *   requires true;
     * \endspec
     * \return IntrusiveMapIterator to the element with immediate higher key value.
     */
    iterator& operator++() noexcept {
      ++it_;
      return *this;
    }

    /*!
     * \brief  Decrement the iterator by one element.
     * \pre    -
     * \spec
     *   requires true;
     * \endspec
     * \return IntrusiveMapIterator to the element with an immediate lower key value.
     */
    iterator& operator--() noexcept {
      --it_;
      return *this;
    }

    /*!
     * \brief  Compare two iterators for equality.
     * \param  other Iterator to compare to.
     * \return True if both iterators point to the same map node.
     * \pre    -
     * \spec
     *   requires true;
     * \endspec
     */
    bool operator==(iterator const& other) const noexcept { return it_ == other.it_; }

    /*!
     * \brief  Compare two iterators for inequality.
     * \param  other Iterator to compare to.
     * \return True if both iterators point to different map nodes.
     * \pre    -
     * \spec
     *   requires true;
     * \endspec
     */
    bool operator!=(iterator const& other) const noexcept { return it_ != other.it_; }

    /*!
     * \brief  Access to the underlying containers' iterator.
     * \pre    -
     * \spec
     *   requires true;
     * \endspec
     * \return A copy of the underlying iterator.
     */
    base_iterator GetBaseIterator() const noexcept { return it_; }

   private:
    /*!
     * \brief The underlying containers' iterator.
     */
    base_iterator it_;
  };

  /*!
   * \brief Typedef for the const iterator type of this map.
   */
  class const_iterator final {
   public:
    /*! \brief Category. */
    using iterator_category = std::bidirectional_iterator_tag;
    /*! \brief Value type. */
    using value_type = StaticMap::value_type;
    /*! \brief Difference type. */
    using difference_type = std::ptrdiff_t;
    /*! \brief Pointer type. */
    using pointer = value_type const*;
    /*! \brief Reference type. */
    using reference = value_type const&;

    /*!
     * \brief Typedef for the const iterator of the container.
     */
    using base_iterator = typename MapType::const_iterator;

    // VECTOR Next Construct AutosarC++17_10-A12.1.5: MD_VAC_A12.1.5_useDelegatingConstructor
    /*!
     * \brief Constructor for an iterator from a base_iterator.
     * \param it The base iterator from which to construct the object.
     * \pre   -
     * \spec
     *   requires true;
     * \endspec
     */
    explicit const_iterator(base_iterator const& it) noexcept : it_(it) {}

    /*!
     * \brief Constructor of an iterator from a base_iterator.
     * \param it The base iterator from which to construct the object.
     * \pre   -
     * \spec
     *   requires true;
     * \endspec
     */
    explicit const_iterator(base_iterator&& it) noexcept : it_(std::move(it)) {}

    // VECTOR Next Construct VectorC++-V12.1.4: MD_VAC_V12-1-4_constructorShallBeDeclaredExplicit
    /*!
     * \brief Constructor of a const_iterator from an iterator.
     * \param it The iterator from which to construct the const_iterator.
     * \pre   -
     * \spec
     *   requires true;
     * \endspec
     */
    const_iterator(iterator const& it) noexcept : it_(it.GetBaseIterator()) {}  // NOLINT(runtime/explicit)

    /*!
     * \brief  Dereference iterator to map element.
     * \pre    -
     * \spec
     *   requires true;
     * \endspec
     * \return The map element that current iterator points to.
     */
    reference operator*() const noexcept {
      return it_.GetMapNode()->GetSelf()->GetStorage();  // VCA_VAC_VALID_RETURN_VALUE
    }

    /*!
     * \brief  Dereference iterator to map element.
     * \pre    -
     * \spec
     *   requires true;
     * \endspec
     * \return The map element that current iterator points to.
     */
    pointer operator->() const noexcept { return &(it_.GetMapNode()->GetSelf()->GetStorage()); }

    /*!
     * \brief  Increment the iterator by one element.
     * \pre    -
     * \spec
     *   requires true;
     * \endspec
     * \return IntrusiveMapIterator to the element with immediate higher key value.
     */
    const_iterator& operator++() noexcept {
      ++it_;
      return *this;
    }

    /*!
     * \brief  Decrement the iterator by one element.
     * \pre    -
     * \spec
     *   requires true;
     * \endspec
     * \return IntrusiveMapIterator to the element with an immediate lower key value.
     */
    const_iterator& operator--() noexcept {
      --it_;
      return *this;
    }

    /*!
     * \brief  Compare two iterators for equality.
     * \param  other Iterator to compare to.
     * \return True if both iterators point to the same map node.
     * \pre    -
     * \spec
     *   requires true;
     * \endspec
     */
    bool operator==(const_iterator const& other) const noexcept {  // Comment to force line break. See TAR-22835.
      return it_ == other.it_;
    }

    /*!
     * \brief  Compare two iterators for inequality.
     * \param  other Iterator to compare to.
     * \return True if both iterators point to different map nodes.
     * \pre    -
     * \spec
     *   requires true;
     * \endspec
     */
    bool operator!=(const_iterator const& other) const noexcept {  // Comment to force line break. See TAR-22835.
      return it_ != other.it_;
    }

    /*!
     * \brief  Access to the underlying containers' iterator.
     * \pre    -
     * \spec
     *   requires true;
     * \endspec
     * \return A copy to the underlying iterator.
     */
    base_iterator GetBaseIterator() const noexcept { return it_; }

   private:
    /*!
     * \brief The underlying containers' iterator.
     */
    base_iterator it_;
  };

  /*!
   * \brief Constructor to create an empty Static map.
   * \spec
   *   requires true;
   * \endspec
   */
  StaticMap() noexcept = default;

  /*!
   * \brief Deleted copy constructor.
   */
  StaticMap(StaticMap const&) = delete;
  /*!
   * \brief  Deleted copy assignment.
   * \return A reference to the assigned-to object.
   */
  StaticMap operator=(StaticMap const&) & = delete;
  /*!
   * \brief Deleted move constructor.
   */
  StaticMap(StaticMap&&) = delete;
  /*!
   * \brief  Deleted move assignment.
   * \return A reference to the assigned-to object.
   */
  StaticMap operator=(StaticMap&&) & = delete;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief Destructor.
   * \pre   -
   * \spec
   *   requires true;
   * \endspec
   */
  ~StaticMap() noexcept { clear(); }

  /*!
   * \brief   Update the memory allocation.
   * \details The current implementation only allows a single allocation.
   *          All further reservations will only limit the visible memory. All subsequent calls to reserve()
   *          where new_capacity is greater than the initial new_capacity will be ignored.
   * \param   new_capacity The number of nodes to reserve space for.
   * \pre     -
   * \spec
   *   requires true;
   * \endspec
   * \trace   CREQ-VaCommonLib-StaticMap
   */
  void reserve(size_type new_capacity) noexcept { storage_.reserve(new_capacity); }

  /*!
   * \brief  Determine whether the map is currently empty.
   * \return True if the static map is empty. False if the map has at least one element.
   * \spec
   *   requires true;
   * \endspec
   */
  bool empty() const noexcept { return map_.empty(); }

  /*!
   * \brief  The number of objects currently allocated in this static map.
   * \return The number of objects currently allocated in this static map.
   * \spec
   *   requires true;
   * \endspec
   */
  size_type size() const noexcept { return storage_.size(); }

  /*!
   * \brief  Determine whether the map is currently full.
   * \return True if the static map is full. False if the map has at least one free place.
   * \spec
   *   requires true;
   * \endspec
   */
  bool full() const noexcept { return storage_.full(); }

  /*!
   * \brief  Return an iterator to the first element of the map.
   * \return An iterator to the first element.
   * \spec
   *   requires true;
   * \endspec
   */
  iterator begin() noexcept { return iterator(map_.begin()); }

  /*!
   * \brief  Get iterator to end element.
   * \return Iterator to end element.
   * \spec
   *   requires true;
   * \endspec
   */
  iterator end() noexcept { return iterator(map_.end()); }

  /*!
   * \brief  Return an iterator to the first element of the map.
   * \return An iterator to the first element.
   * \spec
   *   requires true;
   * \endspec
   */
  const_iterator begin() const noexcept { return const_iterator(map_.cbegin()); }

  /*!
   * \brief  Get iterator to end element.
   * \return Iterator to end element.
   * \spec
   *   requires true;
   * \endspec
   */
  const_iterator end() const noexcept { return const_iterator(map_.cend()); }

  /*!
   * \brief  Return an iterator to the first element of the map.
   * \return An iterator to the first element.
   * \spec
   *   requires true;
   * \endspec
   */
  const_iterator cbegin() const noexcept { return this->begin(); }

  /*!
   * \brief  Get iterator to end element.
   * \return Iterator to end element.
   * \spec
   *   requires true;
   * \endspec
   */
  const_iterator cend() const noexcept { return this->end(); }

  /*!
   * \brief   Remove an element from map.
   * \details Terminates if the destructor of the erased element throws.
   * \param   erase_key Key to be erased.
   * \return  Number of elements removed.
   * \pre     -
   * \trace   CREQ-VaCommonLib-StaticMap
   */
  std::size_t erase(Key const& erase_key) noexcept {
    std::size_t erased_count{0};
    iterator const itr{find(erase_key)};

    if (itr != end()) {
      Node* const elem_to_destroy{itr.GetBaseIterator().GetMapNode()->GetSelf()};
      erased_count = map_.erase(erase_key);
      storage_.destroy(elem_to_destroy);
    }
    return erased_count;
  }

  /*!
   * \brief Remove an element from map.
   * \param elem Node to be erased.
   * \pre   -
   * \spec
   *   requires true;
   * \endspec
   */
  void erase(iterator elem) noexcept {
    if (elem != end()) {
      Node* const node{elem.GetBaseIterator().GetMapNode()->GetSelf()};
      node->EraseFromMap();  // VCA_VAC_VALID_NONSTATIC_METHOD
      storage_.destroy(node);
    }
  }

  /*!
   * \brief Remove all elements from the map.
   * \pre   -
   * \spec
   *   requires true;
   * \endspec
   */
  void clear() noexcept {
    while (!empty()) {
      erase(begin());
    }
  }

  /*!
   * \brief  Find node in map with the input key.
   * \param  find_key Key to to searched for.
   * \return Iterator to a node matching the key or a end_ node.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   */
  iterator find(Key const& find_key) noexcept { return iterator(map_.find(find_key)); }

  /*!
   * \brief  Find node in map with the input key.
   * \param  find_key Key to to searched for.
   * \return Iterator to a node matching the key or a end_ node.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   */
  const_iterator find(Key const& find_key) const noexcept { return const_iterator(map_.find(find_key)); }

  /*!
   * \brief   Insert a new element into the map.
   * \details Terminates if the constructor of the new element throws.
   *          insert(value_type&) requires std::copy_constructible<T> to be true.
   * \param   item Element to be inserted in the map.
   * \return  Pair consisting of an iterator to the inserted element (or to the element that prevented the insertion)
   *          and a bool denoting whether the insertion took place.
   * \pre     There must be capacity for an additional element, otherwise ara::core::Abort is called.
   * \trace   CREQ-VaCommonLib-StaticMap
   */
  std::pair<iterator, bool> insert(value_type const& item) noexcept {
    std::pair<iterator, bool> ret_value{std::make_pair(end(), false)};
    // Check if key is already contained in map.
    iterator existing{map_.find(item.first)};
    if (existing != end()) {
      // When key already in use return pair with iterator to the element that prevented the insertion
      // and second set to false.
      ret_value = std::make_pair(existing, false);
    } else {
      // When key is not used create new element in storage and insert into map.
      // VECTOR NL AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_possibleLeakObjectPoolCreate
      typename StorageType::pointer const element_ptr{storage_.create(item)};
      std::pair<typename MapType::iterator, bool> pair{map_.insert(element_ptr)};

      // Since the existence of the key is checked before and map_.insert returns only false if the key exists,
      // this can never be false in a single-threaded context.
      assert(pair.second);  // COV_MSR_INV_STATE_ASSERT

      // Return pair of iterator and flag (when flag is false insert failed).
      ret_value = std::make_pair(iterator(pair.first), pair.second);
    }
    return ret_value;
  }

  // VECTOR Next Construct VectorC++-V3.9.2: MD_VAC_V3-9-2_parametersBasedOnBuiltInTypes
  /*!
   * \brief     Inserts a new element into the map constructed in-place with the given args.
   * \details   Careful use of emplace allows the new element to be constructed in the storage used by the current
   *            instance while avoiding unnecessary copy or move operations. The constructor of the new element is
   *            called with exactly the same arguments as supplied to emplace, forwarded via std::forward<Args>(args)...
   *            to the create method of the underlying storage. The element may be constructed even if there already is
   *            an element with a matching key in the container, in this case the newly constructed element will be
   *            destroyed immediately after construction. Note that the function will terminate if the constructor of
   *            the new element throws.
   * \tparam    Args Types for the arguments to forward to the constructor of the element.
   * \param[in] args Arguments to forward to the constructor of the element.
   * \return    Pair consisting of an iterator to the inserted element (or to the element that prevented the insertion)
   *            and a bool denoting whether the insertion took place (true: inserted; false: not inserted).
   * \pre       There must be capacity for an additional element, otherwise ara::core::Abort is called.
   */
  template <typename... Args>
  auto emplace(Args&&... args) noexcept -> std::pair<iterator, bool> {
    // VECTOR NL AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_possibleLeakObjectPoolCreate
    typename StorageType::pointer const element_ptr{storage_.create(std::forward<Args>(args)...)};
    // Insert the element to the map.
    std::pair<typename MapType::iterator, bool> pair{map_.insert(element_ptr)};
    // When the key is already used the pair's second value is false.
    if (!pair.second) {
      // When insert to underlying map failed immediately destroy the create element.
      storage_.destroy(element_ptr);
    }

    // Return pair of iterator and flag (when flag is false insert failed).
    return std::make_pair(iterator(pair.first), pair.second);
  }

 private:
  /*!
   * \brief Instance of objectpool storage.
   */
  StorageType storage_;

  /*!
   * \brief Map used to store nodes.
   */
  MapType map_;
};

}  // namespace container
}  // namespace vac

#endif  // LIB_VAC_INCLUDE_VAC_CONTAINER_STATIC_MAP_H_
