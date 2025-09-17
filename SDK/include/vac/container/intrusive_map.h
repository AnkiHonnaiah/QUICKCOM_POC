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
/*!        \file  intrusive_map.h
 *        \brief  The header file of intrusive map is an implementation of a Key-value-storage where the
 *                Key is stored inside the value object.
 *         \unit  VaCommonLib::ContainerLibrary::IntrusiveContainers::IntrusiveMap
 *
 *      \details  Implement the tree-like functions, for example, find parent node, find left/right nodes
 *                erase node, and insert node.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_CONTAINER_INTRUSIVE_MAP_H_
#define LIB_VAC_INCLUDE_VAC_CONTAINER_INTRUSIVE_MAP_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cassert>
#include <iterator>
#include <stdexcept>
#include <utility>

#include "vac/testing/test_adapter.h"

namespace vac {
namespace container {

// VECTOR Next Construct Metric-OO.WMC.One: MD_VAC_Metric-OO.WMC.One
/*!
 * \brief  Type for nodes of the intrusive map.
 *         This serves as a pair of Key and value to be inserted in the map as a node.
 * \tparam Key Type for Key in the pair for node to be inserted in map.
 *         Limitations for Key:
 *         Must be a type which fulfill C++ Named Requirements: Compare
 * \tparam T Type of the value in the pair for node to be inserted in map.
 */
template <typename Key, typename T>
class IntrusiveMapNode {
 public:
  /*!
   * \brief Type of contained Key.
   */
  using key_type = Key;

  // VECTOR Next Construct AutosarC++17_10-A3.9.1: MD_VAC_A3.9.1_baseTypeRequiredBySpecification
  /*!
   * \brief Type of compare result.
   */
  using CompareType = std::int32_t;

  /*!
   * \brief Constructor for a node without a map.
   * \spec
   *   requires true;
   * \endspec
   */
  IntrusiveMapNode() noexcept = default;

  /*!
   * \brief Default copy constructor deleted.
   */
  IntrusiveMapNode(IntrusiveMapNode const&) = delete;

  /*!
   * \brief  Default copy assignment operator deleted.
   */
  IntrusiveMapNode& operator=(IntrusiveMapNode const&) & = delete;

  /*!
   * \brief Default move constructor deleted.
   */
  IntrusiveMapNode(IntrusiveMapNode&&) = delete;

  /*!
   * \brief  Default move assignment operator deleted.
   */
  IntrusiveMapNode& operator=(IntrusiveMapNode&&) & = delete;

  /*!
   * \brief  Compare the keys of this element to another Key.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return Negative value if new Key is less than the other current node Key, zero if the both keys are equal,
   *         positive value if new Key is greater than the other current node Key.
   */
  virtual CompareType KeyCompare(key_type) const noexcept = 0;

  /*!
   * \brief  Compare the keys of this element to the Key obtained from another element.
   *         This function is necessary as the IntrusiveMap class needs to compare IntrusiveMapNode objects. However, as
   *         it does not know how the IntrusiveMapNode stores its Key, it cannot extract the Key for comparison using
   *         KeyCompare(key_type).
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return Negative value if new Key is less than the other current node Key, zero if the both keys are equal,
   *         positive value if new Key is greater than the other current node Key.
   */
  virtual CompareType KeyCompare(T const&) const noexcept = 0;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief Destructor that removes the node from a map, if it is contained in one.
   * \pre   -
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~IntrusiveMapNode() noexcept {  // VCA_VAC_DESTRUCTOR_OUT_OF_BOUNDS
    EraseFromMap();                       // VCA_VAC_VALID_NONSTATIC_METHOD
  }

  /*!
   * \brief Erase the node from a map.
   * \pre   -
   * \spec
   *   requires true;
   * \endspec
   */
  void EraseFromMap() noexcept {
    // Both child nodes present.
    if ((left_ != nullptr) && (right_ != nullptr)) {
      // Find the node with maximum Key value in the left sub tree.
      IntrusiveMapNode<Key, T>* const temp{FindMaxLeft()};  // VCA_VAC_VALID_NONSTATIC_METHOD

      // Erase temp from the map.
      temp->EraseNodeWithOneOrNoChild();  // VCA_VAC_VALID_NONSTATIC_METHOD

      // Adjust the pointers of the node and parent node.
      temp->SetLeft(left_);      // VCA_VAC_VALID_NONSTATIC_METHOD
      temp->SetRight(right_);    // VCA_VAC_VALID_NONSTATIC_METHOD
      temp->SetParent(parent_);  // VCA_VAC_VALID_NONSTATIC_METHOD

      if (parent_->Right() == this) {
        parent_->SetRight(temp);  // VCA_VAC_VALID_NONSTATIC_METHOD
      }
      if (parent_->Left() == this) {
        parent_->SetLeft(temp);  // VCA_VAC_VALID_NONSTATIC_METHOD
      }

      // Adjust the left and right parent pointers.
      if (left_ != nullptr) {
        left_->parent_ = temp;  // VCA_VAC_CHECKED_ACCESS_TO_VALID_POINTER
      }
      // The sorted map node (except for the rightest node) will always contains the right hand child.
      // VECTOR NL AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_pointerIsNullAtDereference
      right_->parent_ = temp;  // VCA_VAC_VALID_MEMBER_VARIABLE_ACCESS, VCA_VAC_VALID_ASSIGNING_NOT_ACCESSING

      // Reset links to parent and child.
      left_ = nullptr;    // VCA_VAC_VALID_MEMBER_VARIABLE_ACCESS
      right_ = nullptr;   // VCA_VAC_VALID_MEMBER_VARIABLE_ACCESS
      parent_ = nullptr;  // VCA_VAC_VALID_MEMBER_VARIABLE_ACCESS

    } else {
      // If node has one or no child
      EraseNodeWithOneOrNoChild();  // VCA_VAC_VALID_NONSTATIC_METHOD
    }
  }

  /*!
   * \brief  Get the contained element.
   * \return The contained element.
   * \spec
   *   requires true;
   * \endspec
   */
  T* GetSelf() noexcept {
    static_assert(std::is_base_of<IntrusiveMapNode<Key, T>, T>::value, "T must derive from IntrusiveMapNode");
    // VECTOR NC AutosarC++17_10-M5.2.3: MD_VAC_M5.2.3_castFromPolymorphicBaseClassToDerivedClass
    // VECTOR NL VectorC++-V5.2.3: MD_VAC_V5-2-3_castFromPolymorphicBaseClassToDerivedClass
    return static_cast<T*>(this);
  }

  /*!
   * \brief  Get the contained element.
   * \return The contained element.
   * \spec
   *   requires true;
   * \endspec
   */
  T const* GetSelf() const noexcept {
    static_assert(std::is_base_of<IntrusiveMapNode<Key, T>, T>::value, "T must derive from IntrusiveMapNode");
    // VECTOR NC AutosarC++17_10-M5.2.3: MD_VAC_M5.2.3_castFromPolymorphicBaseClassToDerivedClass
    // VECTOR NL VectorC++-V5.2.3: MD_VAC_V5-2-3_castFromPolymorphicBaseClassToDerivedClass
    return static_cast<T const*>(this);
  }

  /*!
   * \brief  Returns the pointer to left node.
   * \return Pointer to the left child node.
   * \spec
   *   requires true;
   * \endspec
   */
  IntrusiveMapNode* Left() noexcept { return left_; }

  /*!
   * \brief  Returns const pointer to left node.
   * \return Const pointer to the left child node.
   * \spec
   *   requires true;
   * \endspec
   */
  IntrusiveMapNode const* Left() const noexcept { return left_; }

  /*!
   * \brief  Returns the pointer to right node.
   * \return Pointer to the right child node.
   * \spec
   *   requires true;
   * \endspec
   */
  IntrusiveMapNode* Right() noexcept { return right_; }

  /*!
   * \brief  Returns const pointer to right node.
   * \return Const pointer to the right child node.
   * \spec
   *   requires true;
   * \endspec
   */
  IntrusiveMapNode const* Right() const noexcept { return right_; }

  /*!
   * \brief  Returns the pointer to parent node.
   * \return Pointer to the parent node.
   * \spec
   *   requires true;
   * \endspec
   */
  IntrusiveMapNode* Parent() noexcept { return parent_; }

  /*!
   * \brief  Returns const pointer to parent node.
   * \return Const pointer to the parent node.
   * \spec
   *   requires true;
   * \endspec
   */
  IntrusiveMapNode const* Parent() const noexcept { return parent_; }

  /*!
   * \brief     Set the left element.
   * \param[in] new_elem Pointer to an IntrusiveMapNode.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   */
  void SetLeft(IntrusiveMapNode* new_elem) noexcept {
    left_ = new_elem;  // VCA_VAC_VALID_MEMBER_VARIABLE_ACCESS
  }

  /*!
   * \brief     Set the right element.
   * \param[in] new_elem Pointer to an IntrusiveMapNode.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   */
  void SetRight(IntrusiveMapNode* new_elem) noexcept {
    right_ = new_elem;  // VCA_VAC_VALID_MEMBER_VARIABLE_ACCESS
  }

  /*!
   * \brief     Set the parent element.
   * \param[in] parent Pointer to an IntrusiveMapNode.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   */
  void SetParent(IntrusiveMapNode* parent) noexcept {
    parent_ = parent;  // VCA_VAC_VALID_MEMBER_VARIABLE_ACCESS
  }

  /*!
   * \brief  Find min element from the right subtree.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return Element from right subtree with minimum Key or nullptr if the right subtree is empty.
   */
  IntrusiveMapNode const* FindMinRight() const noexcept {
    IntrusiveMapNode const* min_right{this};
    if (min_right->Right() == nullptr) {
      min_right = nullptr;
    } else {
      min_right = min_right->Right();
      while (min_right->Left() != nullptr) {
        min_right = min_right->Left();
      }
    }
    return min_right;
  }

  /*!
   * \brief  Find max element from the left subtree.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return Element from left subtree with maximum Key, or nullptr if the left subtree is emtpy.
   */
  IntrusiveMapNode const* FindMaxLeft() const noexcept {
    IntrusiveMapNode const* max_left{this};
    if (max_left->Left() == nullptr) {
      max_left = nullptr;
    } else {
      max_left = max_left->Left();
      while (max_left->Right() != nullptr) {
        max_left = max_left->Right();
      }
    }
    return max_left;
  }

  /*!
   * \brief  Find a parent node with immediate large Key value.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return Parent with larger Key value or nullptr in case such a parent is not found.
   */
  IntrusiveMapNode const* FindLargerParent() const noexcept {
    IntrusiveMapNode const* drag{this};
    IntrusiveMapNode const* larger_parent{this};
    // VECTOR Next Construct AutosarC++17_10-M5.14.1: MD_VAC_M5.14.1_operandHasNoSideEffect
    while ((larger_parent != nullptr) && (larger_parent->Left() != drag)) {
      // While loop terminates when reaching the end() element or when coming in to the parent from a left child.
      drag = larger_parent;
      larger_parent = larger_parent->Parent();
    }  // At this point, larger_parent is either the larger parent or end().
    return larger_parent;
  }

  /*!
   * \brief  Find a parent node with immediate smaller Key value.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return Parent with smaller Key value or nullptr in case such a parent is not found.
   */
  IntrusiveMapNode const* FindSmallerParent() const noexcept {
    IntrusiveMapNode const* drag{this};
    IntrusiveMapNode const* smaller_parent{this};
    // VECTOR Next Construct AutosarC++17_10-M5.14.1: MD_VAC_M5.14.1_operandHasNoSideEffect
    while ((smaller_parent != nullptr) && (smaller_parent->Right() != drag)) {
      // While loop terminates when reaching the end() element or when coming in to the parent from a right child
      drag = smaller_parent;
      smaller_parent = smaller_parent->Parent();
    }  // At this point, larger_parent is either the larger parent or end().
    return smaller_parent;
  }

  /*!
   * \brief  Find max element from the left subtree.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return Element from left subtree with maximum Key.
   */
  IntrusiveMapNode* FindMaxLeft() noexcept {
    // VECTOR Next Construct AutosarC++17_10-A5.2.3: MD_VAC_A5.2.3_constCastReducesCodeDuplication
    // VCA_VAC_VALID_NONSTATIC_METHOD
    return const_cast<IntrusiveMapNode*>(static_cast<IntrusiveMapNode const*>(this)->FindMaxLeft());
  }

  /*!
   * \brief  Find min element from the right subtree.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return Element from right subtree with minimum Key.
   */
  IntrusiveMapNode* FindMinRight() noexcept {
    // VECTOR Next Construct AutosarC++17_10-A5.2.3: MD_VAC_A5.2.3_constCastReducesCodeDuplication
    // VCA_VAC_VALID_NONSTATIC_METHOD
    return const_cast<IntrusiveMapNode*>(static_cast<IntrusiveMapNode const*>(this)->FindMinRight());
  }

  /*!
   * \brief  Find a parent node with immediate large Key value.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return Parent with larger Key value or nullptr in case such a parent is not found.
   */
  IntrusiveMapNode* FindLargerParent() noexcept {
    // VECTOR Next Construct AutosarC++17_10-A5.2.3: MD_VAC_A5.2.3_constCastReducesCodeDuplication
    // VCA_VAC_VALID_NONSTATIC_METHOD
    return const_cast<IntrusiveMapNode*>(static_cast<IntrusiveMapNode const*>(this)->FindLargerParent());
  }

  /*!
   * \brief  Find a parent node with immediate smaller Key value.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return Parent with smaller Key value or nullptr in case such a parent is not found.
   */
  IntrusiveMapNode* FindSmallerParent() noexcept {
    // VECTOR Next Line AutosarC++17_10-A5.2.3: MD_VAC_A5.2.3_constCastReducesCodeDuplication
    return const_cast<IntrusiveMapNode*>(static_cast<IntrusiveMapNode const*>(this)->FindSmallerParent());
  }

 private:
  // VECTOR Next Construct AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_pointerIsNullAtDereference
  /*! \brief Helper function to update the left and right parent pointers
   * \spec
   *   requires true;
   * \endspec
   */
  void UpdateParentPointers() noexcept {
    if (this->HasParent()) {
      if (this == parent_->right_) {
        parent_->right_ = right_;  // VCA_VAC_CHECKED_ACCESS_TO_VALID_POINTER
        if (left_ != nullptr) {
          parent_->right_ = left_;  // VCA_VAC_CHECKED_ACCESS_TO_VALID_POINTER
        }
      }
      if (this == parent_->left_) {
        parent_->left_ = right_;  // VCA_VAC_CHECKED_ACCESS_TO_VALID_POINTER
        if (left_ != nullptr) {
          parent_->left_ = left_;  // VCA_VAC_CHECKED_ACCESS_TO_VALID_POINTER
        }
      }
    }
  }

  /*! \brief Helper function to adjust the left and right child pointers
   * \spec
   *   requires true;
   * \endspec
   */
  void UpdateChildPointers() noexcept {
    if (left_ != nullptr) {
      left_->parent_ = parent_;  // VCA_VAC_CHECKED_ACCESS_TO_VALID_POINTER
    }
    if (right_ != nullptr) {
      right_->parent_ = parent_;  // VCA_VAC_CHECKED_ACCESS_TO_VALID_POINTER
    }
  }

  /*!
   * \brief Erase the node from a map that has either no or exactly one child.
   * \spec
   *   requires true;
   * \endspec
   */
  void EraseNodeWithOneOrNoChild() noexcept {
    // Deleting a node that has only one or no child.
    assert(((left_ == nullptr) || (right_ == nullptr)));  // COV_MSR_INV_STATE_ASSERT
    UpdateParentPointers();                               // VCA_VAC_VALID_NONSTATIC_METHOD
    UpdateChildPointers();                                // VCA_VAC_VALID_NONSTATIC_METHOD
    // Adjust the node pointers
    left_ = nullptr;    // VCA_VAC_VALID_MEMBER_VARIABLE_ACCESS
    right_ = nullptr;   // VCA_VAC_VALID_MEMBER_VARIABLE_ACCESS
    parent_ = nullptr;  // VCA_VAC_VALID_MEMBER_VARIABLE_ACCESS
  }

  /*!
   * \brief  Determines if the 'this' node has a parent node.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return True if a parent node exists, else false.
   */
  bool HasParent() const noexcept { return parent_ != nullptr; }

  /*!
   * \brief Pointer to the left child node.
   */
  IntrusiveMapNode* left_{nullptr};

  /*!
   * \brief Pointer to the right child node.
   */
  IntrusiveMapNode* right_{nullptr};

  /*!
   * \brief Pointer to the parent node.
   */
  IntrusiveMapNode* parent_{nullptr};
};

/*!
 * \brief Type for end node of the intrusive map.
 * \tparam Key Type for Key in the pair.
 * \tparam T Type of the value in the pair.
 */
template <typename Key, typename T>
class EndNode final : public IntrusiveMapNode<Key, T> {  // VCA_VAC_SAFE_DEALLOCATION
 public:
  /*!
   * \brief Constructor.
   */
  using IntrusiveMapNode<Key, T>::IntrusiveMapNode;

  /*!
   * \brief Type of compare result.
   */
  using CompareType = typename IntrusiveMapNode<Key, T>::CompareType;

  /*!
   * \brief  Compares the  Key value with the node current node value.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return Always returns 1 because this is the root node with only left subtree so the Key is considered to be
   *         always greater then any other Key in the map.
   */
  CompareType KeyCompare(Key) const noexcept final { return 1; }

  /*!
   * \brief  Compares the  Key value with the node current node value.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return Always returns 1 because this is the root node with only left subtree so the Key is considered to be
   *         always greater then any other Key in the map.
   */
  CompareType KeyCompare(T const&) const noexcept final { return 1; }
};

/*!
 * \brief Iterator for this IntrusiveMap.
 * \tparam Key Type for Key in the pair.
 *         Key must be a type which fulfill C++ Named Requirements: Compare.
 * \tparam T Type of the value in the pair.
 * \trace CREQ-VaCommonLib-IntrusiveMap
 */
template <typename Key, typename T>
class IntrusiveMapIterator final {
 public:
  /*! \brief Category. */
  using iterator_category = std::bidirectional_iterator_tag;
  /*! \brief Value type. */
  using value_type = T;
  /*! \brief Difference type. */
  using difference_type = std::ptrdiff_t;
  /*! \brief Pointer type. */
  using pointer = T*;
  /*! \brief Reference type. */
  using reference = T&;

  /*!
   * \brief     Constructor for an iterator.
   * \param[in] node The Map from which to construct the iterator.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   */
  explicit IntrusiveMapIterator(IntrusiveMapNode<Key, T>* node) noexcept : node_(node) {}

  /*!
   * \brief  Return a reference to the contained element.
   * \return A reference to the element type.
   * \spec
   *   requires true;
   * \endspec
   */
  T& operator*() noexcept { return *(this->GetMapNode()->GetSelf()); }

  /*!
   * \brief  Return a reference to the contained element.
   * \return A const reference to the element type.
   * \spec
   *   requires true;
   * \endspec
   */
  T const& operator*() const noexcept { return *(this->GetMapNode()->GetSelf()); }

  /*!
   * \brief  Get the map node .
   * \return The contained pointer to the map node.
   * \spec
   *   requires true;
   * \endspec
   */
  IntrusiveMapNode<Key, T>* GetMapNode() noexcept { return node_; }

  /*!
   * \brief  Get the map node .
   * \return The contained pointer to the map node.
   * \spec
   *   requires true;
   * \endspec
   */
  IntrusiveMapNode<Key, T> const* GetMapNode() const noexcept { return node_; }

  /*!
   * \brief  Compare two iterators for equality.
   * \param  other Iterator to compare to.
   * \return True if both iterators point to the same map node.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   */
  bool operator==(IntrusiveMapIterator const& other) const noexcept { return node_ == other.node_; }

  /*!
   * \brief  Compare two iterators for inequality.
   * \param  other Iterator to compare to.
   * \return True if both iterators point to different map nodes.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   */
  bool operator!=(IntrusiveMapIterator const& other) const noexcept { return node_ != other.node_; }

  /*!
   * \brief  Increment the iterator by one element.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return IntrusiveMapIterator to the element with immediate higher Key value.
   */
  IntrusiveMapIterator& operator++() noexcept {
    IntrusiveMapNode<Key, T>* temp_elem{node_};
    if (node_ != nullptr) {
      temp_elem = node_->FindMinRight();  // VCA_VAC_VALID_NONSTATIC_METHOD

      // If Min right node not found.
      if (temp_elem == nullptr) {
        temp_elem = node_->FindLargerParent();  // VCA_VAC_VALID_NONSTATIC_METHOD
      }
    }
    if (temp_elem != nullptr) {
      // Avoid leaving the tree, remain on the end() element.
      node_ = temp_elem;
    }
    return *this;
  }

  /*!
   * \brief  Decrement the iterator by one element.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return IntrusiveMapIterator to the element with an immediate lower Key value.
   */
  IntrusiveMapIterator& operator--() noexcept {
    IntrusiveMapNode<Key, T>* temp_elem{node_};
    if (node_ != nullptr) {
      temp_elem = node_->FindMaxLeft();

      // If Max Left node not found.
      if (temp_elem == nullptr) {
        temp_elem = node_->FindSmallerParent();
      }
    }
    node_ = temp_elem;
    return *this;
  }

 private:
  /*!
   * \brief Pointer to the map element.
   */
  IntrusiveMapNode<Key, T>* node_;
};

/*!
 * \brief Const Iterator for this IntrusiveMap.
 * \tparam Key Type for Key in the pair.
 *         Key must be a type which fulfill C++ Named Requirements: Compare.
 * \tparam T Type of the value in the pair.
 */
template <typename Key, typename T>
class ConstIntrusiveMapIterator final {
 public:
  /*! \brief Category. */
  using iterator_category = std::bidirectional_iterator_tag;
  /*! \brief Value type. */
  using value_type = T;
  /*! \brief Difference type. */
  using difference_type = std::ptrdiff_t;
  /*! \brief Pointer type. */
  using pointer = T const*;
  /*! \brief Reference type. */
  using reference = T const&;

  // VECTOR Next Construct AutosarC++17_10-A12.1.5: MD_VAC_A12.1.5_useDelegatingConstructor
  /*!
   * \brief     Constructor for an iterator.
   * \param[in] node The Map from which to construct the iterator.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   */
  explicit ConstIntrusiveMapIterator(IntrusiveMapNode<Key, T> const* node) noexcept : node_(node) {}

  // VECTOR Next Construct VectorC++-V12.1.4: MD_VAC_V12-1-4_constructorShallBeDeclaredExplicit
  /*!
   * \brief Constructor for const_iterator from an iterator.
   * \param it The iterator from which to construct the const_iterator.
   * \pre   -
   * \spec
   *   requires true;
   * \endspec
   */
  ConstIntrusiveMapIterator(IntrusiveMapIterator<Key, T> const& it) noexcept  // NOLINT(runtime/explicit)
      : node_(it.GetMapNode()) {}

  /*!
   * \brief  Return a reference to the contained element.
   * \return A const reference to the element type.
   * \spec
   *   requires true;
   * \endspec
   */
  reference operator*() const noexcept { return *(this->GetMapNode()->GetSelf()); }

  /*!
   * \brief  Get the map node .
   * \return The contained pointer to the map node.
   * \spec
   *   requires true;
   * \endspec
   */
  IntrusiveMapNode<Key, T> const* GetMapNode() const noexcept { return node_; }

  /*!
   * \brief  Compare two iterators for equality.
   * \param  other Iterator to compare to.
   * \return True if both iterators point to the same map node.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   */
  bool operator==(ConstIntrusiveMapIterator const& other) const noexcept { return node_ == other.node_; }

  /*!
   * \brief  Compare two iterators for inequality.
   * \param  other Iterator to compare to.
   * \return True if both iterators point to different map nodes.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   */
  bool operator!=(ConstIntrusiveMapIterator const& other) const noexcept { return node_ != other.node_; }

  /*!
   * \brief  Increment the iterator by one element.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return IntrusiveMapIterator to the element with immediate higher Key value.
   */
  ConstIntrusiveMapIterator& operator++() noexcept {
    IntrusiveMapNode<Key, T> const* temp_elem{node_};
    if (node_ != nullptr) {
      temp_elem = node_->FindMinRight();

      // If Min right node not found.
      if (temp_elem == nullptr) {
        temp_elem = node_->FindLargerParent();
      }
    }
    node_ = temp_elem;
    return *this;
  }

  /*!
   * \brief  Decrement the iterator by one element.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return IntrusiveMapIterator to the element with an immediate lower Key value.
   */
  ConstIntrusiveMapIterator& operator--() noexcept {
    IntrusiveMapNode<Key, T> const* temp_elem{node_};
    if (node_ != nullptr) {
      temp_elem = node_->FindMaxLeft();

      // If Max Left node not found.
      if (temp_elem == nullptr) {
        temp_elem = node_->FindSmallerParent();
      }
    }
    node_ = temp_elem;
    return *this;
  }

 private:
  /*!
   * \brief Pointer to the map element.
   */
  IntrusiveMapNode<Key, T> const* node_;
};

/*!
 * \brief Class to implement an IntrusiveMap.
 *        Contains pair of Key and value to be inserted in the map as a node.
 * \tparam Key Type for Key in the pair for node to be inserted in map.
 *         Limitations for Key:
 *         Must be a type which fulfill C++ Named Requirements: Compare.
 * \tparam T Type of the value in the pair for node to be inserted in map.
 * \trace CREQ-VaCommonLib-IntrusiveMap
 */
template <typename Key, typename T>
class IntrusiveMap final {
 public:
  /*!
   * \brief Typedef for the contained element.
   */
  using value_type = T;

  /*!
   * \brief Typedef for the iterator type of this map.
   */
  using iterator = IntrusiveMapIterator<Key, T>;

  /*!
   * \brief Typedef for the iterator type of this map.
   */
  using const_iterator = ConstIntrusiveMapIterator<Key, T>;

  /*!
   * \brief Type of compare result.
   */
  using CompareType = typename IntrusiveMapNode<Key, T>::CompareType;

  /*!
   * \brief   Type returned by insert.
   * \details Contains an iterator and a bool that flags whether the insertion was successful.
   */
  using InsertType = std::pair<iterator, bool>;

  /*!
   * \brief Constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  IntrusiveMap() noexcept { map_.SetParent(nullptr); }

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief Destructor.
   * \spec
   *   requires true;
   * \endspec
   * \pre   -
   */
  ~IntrusiveMap() noexcept {
    // Release all elements from the map starting from the smallest Key member.
    while (!empty()) {
      erase(begin());
    }
  }

  /*!
   * \brief Deleted copy constructor.
   */
  IntrusiveMap(IntrusiveMap const&) = delete;
  /*!
   * \brief Deleted move constructor.
   */
  IntrusiveMap(IntrusiveMap&&) = delete;
  /*!
   * \brief  Deleted copy assignent.
   */
  IntrusiveMap& operator=(IntrusiveMap const&) & = delete;
  /*!
   * \brief  Deleted move assignent.
   */
  IntrusiveMap& operator=(IntrusiveMap&&) & = delete;

  /*!
   * \brief  Remove an element from map.
   * \param  erase_key Key to be erased.
   * \return Number of elements removed.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   */
  std::size_t erase(Key const& erase_key) noexcept {
    std::size_t deleted_count{0};
    iterator itr{find(erase_key)};

    // VECTOR Next Construct AutosarC++17_10-M5.14.1: MD_VAC_M5.14.1_operandHasNoSideEffect
    if ((map_.Left() != nullptr) && (itr != end())) {
      IntrusiveMapNode<Key, T>* const elem{itr.GetMapNode()};
      // Erase the node.
      elem->EraseFromMap();  // VCA_VAC_VALID_NONSTATIC_METHOD
      deleted_count++;
    }
    return deleted_count;
  }

  /*!
   * \brief Remove an element from map.
   * \param elem Iterator to the element to be erased.
   * \spec
   *   requires true;
   * \endspec
   */
  void erase(iterator elem) noexcept {
    // VECTOR Next Construct AutosarC++17_10-M5.14.1: MD_VAC_M5.14.1_operandHasNoSideEffect
    if ((!empty()) && (elem != end())) {
      if (elem.GetMapNode() != nullptr) {
        elem.GetMapNode()->EraseFromMap();  // VCA_VAC_VALID_NONSTATIC_METHOD
      }
    }
  }

  /*!
   * \brief  Determine whether the map is currently empty.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return True if the map is empty. False if the map has at least one element.
   */
  bool empty() const noexcept { return (map_.Left() == nullptr); }

  /*!
   * \brief     Insert a new node (Key value pair) into the map at appropriate position.
   * \param[in] node Element to be inserted in the map.
   * \return    Pair containing iterator and bool.
   *            Iterator will point to inserted element or element that prevented insertion.
   *            Bool will be true if insertion succeeded and false otherwise.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   */
  InsertType insert(IntrusiveMapNode<Key, T>* node) noexcept {
    bool is_inserted{false};
    InsertType insert_result{std::make_pair(iterator{node}, is_inserted)};
    if (node != nullptr) {
      if (map_.Left() == nullptr) {
        map_.SetLeft(node);
        node->SetParent(&map_);
        is_inserted = true;
        insert_result = std::make_pair(iterator{node}, is_inserted);
      } else {
        T const* const self{node->GetSelf()};
        IntrusiveMapNode<Key, T>* const temp_node{SearchNode(*self)};  // VCA_VAC_VALID_ARGUMENT
        insert_result = InsertNode(node, self, temp_node);
      }
    }
    return insert_result;
  }

  /*!
   * \brief  Find node in map with the input Key.
   * \param  find_key Key to to searched for.
   * \return Iterator to a node matching the Key or a end_ node.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   */
  iterator find(Key const& find_key) noexcept {
    iterator ret_value{end()};
    IntrusiveMapNode<Key, T>* const node{SearchNode(find_key)};
    // VECTOR Next Construct AutosarC++17_10-M5.14.1: MD_VAC_M5.14.1_operandHasNoSideEffect
    // VCA_VAC_VALID_FUNC_CALL_RUN_TIME_CHECK, VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
    if ((node != nullptr) && (node->KeyCompare(find_key) == 0)) {
      ret_value = iterator(node);
    }
    return ret_value;
  }

  /*!
   * \brief  Find node in map with the input Key.
   * \param  find_key Key to to searched for.
   * \return Iterator to a node matching the Key or a end_ node.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   */
  const_iterator find(Key const& find_key) const noexcept {
    const_iterator ret_value{cend()};
    IntrusiveMapNode<Key, T> const* const node{SearchNode(find_key)};

    if (node != nullptr) {
      // VCA_VAC_VALID_FUNC_CALL_RUN_TIME_CHECK, VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
      if (node->KeyCompare(find_key) == 0) {
        ret_value = const_iterator(node);
      }
    }
    return ret_value;
  }

  /*!
   * \brief  Get iterator to smallest Key node.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return Iterator to Intrusive map.
   */
  iterator begin() noexcept {
    iterator ret_value{end()};
    if (!empty()) {
      IntrusiveMapNode<Key, T>* node{map_.Left()};
      while (node->Left() != nullptr) {
        node = node->Left();
      }
      ret_value = iterator(node);
    }
    return ret_value;
  }

  /*!
   * \brief  Get const iterator to smallest Key node.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return Const iterator to Intrusive map.
   */
  const_iterator cbegin() const noexcept {
    const_iterator ret_value{cend()};
    if (!empty()) {
      IntrusiveMapNode<Key, T> const* node{map_.Left()};
      while (node->Left() != nullptr) {
        node = node->Left();
      }
      ret_value = const_iterator(node);
    }
    return ret_value;
  }

  /*!
   * \brief  Get iterator to end element.
   * \return Iterator to end element.
   * \spec
   *   requires true;
   * \endspec
   */
  iterator end() noexcept { return iterator(&map_); }

  /*!
   * \brief  Get iterator to end element.
   * \return Iterator to end element.
   * \spec
   *   requires true;
   * \endspec
   */
  const_iterator cend() const noexcept { return const_iterator(&map_); }

 private:
  /*!
   * \brief     Helper method to insert a non-first node into the map at appropriate position.
   * \param[in] node Element to be inserted in the map.
   * \param[in] self Contained element by node.
   * \param[in] temp_node New node will be set left or right of this node
   * \return    Pair containing iterator and bool.
   *            Iterator will point to inserted element or element that prevented insertion.
   *            Bool will be true if insertion succeeded and false otherwise.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   */
  static InsertType InsertNode(IntrusiveMapNode<Key, T>* node, T const* const self,
                               IntrusiveMapNode<Key, T>* const temp_node) {
    bool is_inserted{false};
    if (temp_node != nullptr) {
      // VCA_VAC_VALID_FUNC_CALL_RUN_TIME_CHECK, VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
      CompareType const result{temp_node->KeyCompare(*self)};

      if (result > 0) {
        // assert to help VCA.
        assert(node != nullptr);    // COV_MSR_INV_STATE_ASSERT
        temp_node->SetRight(node);  // VCA_VAC_VALID_NONSTATIC_METHOD
        node->SetParent(temp_node);
        is_inserted = true;
      } else if (result < 0) {
        temp_node->SetLeft(node);  // VCA_VAC_VALID_NONSTATIC_METHOD
        // assert to help VCA.
        assert(node != nullptr);  // COV_MSR_INV_STATE_ASSERT
        node->SetParent(temp_node);
        is_inserted = true;
      } else {
        // Get the node that prevented the insertion.
        node = temp_node;
      }
    }
    return std::make_pair(iterator{node}, is_inserted);
  }

  /*!
   * \brief  Search for a node position in map with the input Key.
   * \tparam InputKey Class type for the input to search a node position with.
   * \param  find_key Key to searched for.
   * \return Iterator to a node matching the Key or a node just previous to the matching Key.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename InputKey>
  IntrusiveMapNode<Key, T> const* SearchNode(InputKey const& find_key) const noexcept {
    IntrusiveMapNode<Key, T> const* temp{map_.Left()};
    IntrusiveMapNode<Key, T> const* prev{nullptr};
    while (temp != nullptr) {
      prev = temp;
      // VCA_VAC_VALID_FUNC_CALL_RUN_TIME_CHECK, VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
      CompareType const result{temp->GetSelf()->KeyCompare(find_key)};
      if (result > 0) {
        temp = temp->Right();
      } else if (result < 0) {
        temp = temp->Left();
      } else {
        break;
      }
    }
    return prev;
  }

  // VECTOR Next Construct AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_constCastReducesCodeDuplication
  // VECTOR Next Construct AutosarC++17_10-A5.2.3: MD_VAC_A5.2.3_constCastReducesCodeDuplication
  /*!
   * \brief  Search for a node position in map with the input Key.
   * \tparam InputKey Class type for the input to search a node position with.
   * \param  find_key Key to to searched for.
   * \return Iterator to a node matching the Key or a node just previous to the matching Key.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename InputKey>
  IntrusiveMapNode<Key, T>* SearchNode(InputKey const& find_key) noexcept {
    return const_cast<IntrusiveMapNode<Key, T>*>(const_cast<IntrusiveMap const*>(this)->SearchNode(find_key));
  }

  /*!
   * \brief Pointer to the map.
   *        Map_ is the root node and the actual map starts from the map_.left. map_.right will always be nullptr.
   *        For an empty map the map_.left will be nullptr.
   */
  EndNode<Key, T> map_{};

  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__IntrusiveMap, IteratorOperationsIncrementRightSubTree);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__IntrusiveMap, IteratorOperationsIncrementLeftSubTree);
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__IntrusiveMap, InsertElementWithNullptr);
};

}  // namespace container
}  // namespace vac

#endif  // LIB_VAC_INCLUDE_VAC_CONTAINER_INTRUSIVE_MAP_H_
