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
/*!      \file  red_black_tree.h
 *      \brief  This file contains the implementation of the red black tree data structure.
 *       \unit  VaCommonLib::ContainerLibrary::DynamicContainers::Map
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_CONTAINER_RED_BLACK_TREE_H_
#define LIB_VAC_INCLUDE_VAC_CONTAINER_RED_BLACK_TREE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <algorithm>
#include <cassert>
#include <functional>
#include <type_traits>
#include "amsr/container_error_domain.h"
#include "ara/core/abort.h"
#include "ara/core/array.h"
#include "ara/core/memory_resource.h"
#include "ara/core/result.h"
#include "vac/language/rebind_allocator.h"

namespace vac {
namespace container {
namespace internal {

/*!
 * \brief The directions the children can be below the parent.
 */
static constexpr std::uint8_t const kLeft{0};
static constexpr std::uint8_t const kRight{1};

/*!
 * \brief The available node colors.
 */
static constexpr bool const kBlack{false};
static constexpr bool const kRed{true};

// Forward declaration of class RBNode.
template <typename Key, typename Value>
class RBNode;

// Forward declaration of class RBNode.
template <typename Key, typename Value, typename Compare, typename Alloc>
class RBTree;

/*!
 * \brief     Changes direction of \p dir. kLeft becomes kRight and kRight becomes kLeft.
 * \param[in] dir  The direction to change.
 * \return    The opposite direction of \p dir.
 */
constexpr std::uint8_t GetOppositeDirection(std::uint8_t dir) noexcept {
  return static_cast<std::uint8_t>(!static_cast<bool>(dir));
}

/*!
 * \brief   A class for storing the data in a red black tree.
 * \details RBNode stores a pointer to a RBData. Any data contained by a RBNode is stored in a RBData. This seperation
 *          allows having nodes without any data, necessary by the header node in RBTreeHeader.
 * \tparam  Key    First type stored in a std::pair. Stored as const.
 * \tparam  Value  Second type stored in a std::pair.
 */
template <typename Key, typename Value>
class RBData final {
 public:
  /*!
   * \brief Alias for the type of data stored.
   */
  using DataType = std::pair<Key const, Value>;

  /*!
   * \brief Default destructor.
   */
  ~RBData() = default;
  RBData() = delete;
  RBData(RBData const&) = delete;
  RBData(RBData&&) = delete;
  RBData& operator=(RBData const&) = delete;
  RBData& operator=(RBData&&) = delete;

  /*!
   * \brief     Constructor to create the data object.
   * \tparam    Args  The types of the arguments to construct the data object.
   * \param[in] args  The arguments to create a pair of Key and Value.
   */
  template <typename... Args>
  explicit RBData(Args&&... args) noexcept : data_(std::forward<Args>(args)...) {}  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT

  /*!
   * \brief  Get a reference to the contained data.
   * \return A reference to the contained data.
   */
  DataType& GetData() noexcept { return data_; }

 private:
  /*!
   * \brief The stored data object.
   */
  DataType data_;
};

// VECTOR Next Construct Metric-OO.WMC.One: MD_VAC_Metric-OO.WMC.One
/*!
 * \brief    A node within the red black tree.
 * \details  Each node has one parent link and two children, one left and one right and are colored red or black.
 * \tparam   Key    Type of the key in the tree. The tree is sorted based on Key. Stored as a const value.
 * \tparam   Value  Type of the mapped-to value object in the nodes in the tree.
 * \vprivate Component Private
 */
template <typename Key, typename Value>
class RBNode final {
 public:
  /*!
   * \brief Alias for the class containing the data.
   */
  using DataStructType = RBData<Key, Value>;

  /*!
   * \brief Type of data stored in RBData.
   */
  using DataType = typename DataStructType::DataType;

  // VECTOR NC AutosarC++17_10-A12.8.4: MD_VAC_A12.8.4_moveConstructorShallNotUseCopySemantics
  /*!
   * \brief         Move constructor for a RBNode.
   * \param[in,out] other  The node to move from.
   */
  RBNode(RBNode&& other) noexcept
      : color_{std::move(other.color_)},
        visited_{std::move(other.visited_)},
        parent_{std::move(other.parent_)},
        child_{std::move(other.child_)},
        data_{std::move(other.data_)} {
    other.parent_ = nullptr;
    other.child_ = {nullptr, nullptr};
    other.data_ = nullptr;
  }

  /*!
   * \brief Deleted copy constructor
   */
  RBNode(RBNode const& other) = delete;
  /*!
   * \brief Deleted copy assignment operator
   */
  RBNode& operator=(RBNode const&) = delete;

  /*!
   * \brief         Move assignment operator for a RBNode.
   * \param[in,out] other  The node to move from.
   * \return        this.
   * \vprivate      Component Private
   */
  RBNode& operator=(RBNode&& other) & noexcept {
    this->color_ = other.color_;
    this->visited_ = other.visited_;
    this->parent_ = other.parent_;
    this->child_ = other.child_;
    this->data_ = other.data_;
    other.parent_ = nullptr;
    other.child_ = {nullptr, nullptr};
    other.data_ = nullptr;

    return *this;
  }

  /*!
   * \brief         Construct a node with a pointer to data and a color.
   * \param[in,out] data   Pointer to a data container object.
   * \param[in]     color  The default color of the node.
   * \vprivate      Component Private
   */
  explicit RBNode(DataStructType* data, bool color = kBlack) noexcept
      // VCA_VAC_VALID_CONSTRUCTOR_CALLED
      : color_{color}, visited_{false}, parent_{nullptr}, child_{nullptr, nullptr}, data_{data} {};
  /*!
   * \brief    Default destructor.
   * \vprivate Component Private
   */
  ~RBNode() noexcept = default;

  /*!
   * \brief         Copy assignment of RBNode.
   * \tparam        ThisTreeType  Type of RBTree *this belongs to.
   * \param[in]     other      The node to copy.
   * \param[in,out] this_tree  The tree containing this.
   * \return        ara::core::Result<void> with value is no allocation errors occur, otherwise it contains the error
   *                amsr::ContainerErrorCode::kAllocationError.
   * \vprivate      Component Private
   */
  template <typename ThisTreeType>
  auto CopyNode(RBNode const* other, ThisTreeType&& this_tree) noexcept -> ara::core::Result<void> {
    using ThisTree = std::remove_reference_t<ThisTreeType>;
    using ThisDataAlloc = typename ThisTree::data_allocator_type;

    constexpr std::size_t nr_of_elements{1};

    // Deallocate the to-be overwritten
    if (this->GetDataPointer() != nullptr) {
      std::allocator_traits<ThisDataAlloc>::destroy(this_tree.GetDataAllocator(), this->GetDataPointer());
      std::allocator_traits<ThisDataAlloc>::deallocate(  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
          this_tree.GetDataAllocator(), this->GetDataPointer(), nr_of_elements);
      SetDataPointer(nullptr);
    }

    using alloc_traits = std::allocator_traits<typename ThisTree::data_allocator_type>;
    ara::core::Result<void> res{};

    typename ThisTree::NodeType::DataStructType* const new_data{
        alloc_traits::allocate(this_tree.GetDataAllocator(), nr_of_elements)};
    if (new_data != nullptr) {
      alloc_traits::construct(this_tree.GetDataAllocator(), new_data,  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
                              other->GetData());                       // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
      SetDataPointer(new_data);
      SetColor(other->GetColor());
    } else {
      res.EmplaceError(amsr::ContainerErrorCode::kAllocationError);
    }
    return res;
  }

  /*!
   * \brief    Checks if the the node is the root of a tree.
   * \details  The properties of a root is that it's parent is the header of the tree. The header's parent shall be the
   *           root. This means that if the grandparent is itself, the node is the root.
   * \return   True if the node is the root.
   * \vprivate Component Private
   */
  auto IsRoot() const noexcept -> bool {
    bool is_root{false};
    if (parent_ != nullptr) {
      is_root = parent_->GetParent() == this;
    }
    return is_root;
  }

  /*!
   * \brief    Checks if *this is the header node.
   * \details  All nodes will be the parent of their children, except for the header node.
   * \return   true if *this is the header, false otherwise.
   * \vprivate Component Private
   */
  auto IsHeader() const noexcept -> bool {
    bool is_header{false};
    if (this->GetChild(kLeft) != nullptr) {
      is_header = (this->GetChild(kLeft)->GetParent() != this) || (this->GetChild(kLeft)->IsRoot());
    }
    return is_header;
  }

  /*!
   * \brief    Getter for the parent node.
   * \return   A pointer to the parent.
   * \vprivate Component Private
   */
  auto GetParent() const noexcept -> RBNode* { return parent_; }

  // VECTOR NC AutosarC++17_10-M5.2.3: MD_VAC_M5.2.3_castFromPolymorphicBaseClassToDerivedClassPreconditioned
  // VECTOR NC VectorC++-V5.2.3: MD_VAC_M5.2.3_castFromPolymorphicBaseClassToDerivedClassPreconditioned
  /*!
   * \brief    Getter for the valid parent node.
   * \details  If the parent is nullptr or the header, the function aborts.
   * \return   A pointer to the parent.
   * \vprivate Component Private
   */
  auto GetValidParent() const noexcept -> RBNode* {
    auto const parent = this->GetParent();
    if ((parent == nullptr) || (this->IsRoot())) {
      // If the parent is nullptr, or the node is the root (i.e the parent of the parent is itself)
      // the parent is not valid, and the function aborts.
      ara::core::Abort("Parent is not valid");
    }
    // Otherwise, it is certain that the parent is not a base class object (since this only happens when the node is
    // the root) and we can safely cast to child type.
    return parent;
  }

  /*!
   * \brief     Get a pointer to the child in direction \p dir.
   * \param[in] dir  The direction of the wanted child.
   * \return    A pointer to the child in direction \p dir.
   * \vprivate  Component Private
   */
  auto GetChild(std::uint8_t dir) const noexcept -> RBNode* { return child_[dir]; }

  /*!
   * \brief     Get a pointer to the valid child in direction \p dir.
   * \details   If the requested child is nullptr, the function aborts.
   * \param[in] dir  The direction fo the wanted child.
   * \return    A pointer to the child in direction \p dir.
   * \vprivate  Component Private
   */
  auto GetValidChild(std::uint8_t dir) const noexcept -> RBNode* {
    RBNode* const child{this->GetChild(dir)};
    if (child == nullptr) {
      ara::core::Abort("Child is not valid");
    }
    assert(child != nullptr);  // COV_MSR_INV_STATE_ASSERT
    return child;
  }

  /*!
   * \brief    Get a pointer to the contained data container object.
   * \return   A pointer to the contained data container object.
   * \vprivate Component Private
   */
  auto GetDataPointer() const noexcept -> DataStructType* { return this->data_; }

  /*!
   * \brief         Set the pointer to a data container object.
   * \param[in,out] data  The pointer to replace the data container pointer with.
   * \vprivate      Component Private
   */
  void SetDataPointer(DataStructType* data) noexcept { this->data_ = data; }

  /*!
   * \brief    Getter for the contained data.
   * \details  Only the overridden function shall be used. This function must exists to avoid unnecessary casts between
   *           parent/child pointers.
   * \return   A reference to the data object.
   * \vprivate Component Private
   * \spec     requires true; \endspec
   */
  auto GetData() noexcept -> DataType& {
    if (data_ == nullptr) {
      ara::core::Abort("Cannot get data from header");
    }
    assert(data_ != nullptr);  // COV_MSR_INV_STATE_ASSERT
    return data_->GetData();   // VCA_VAC_VALID_RETURN_VALUE_RUN_TIME_CHECK
  }

  /*!
   * \brief    Getter for the contained data.
   * \details  Only the overridden function shall be used. This function must exists to avoid unnecessary casts between
   *           parent/child pointers.
   * \return   A reference to the data object.
   * \vprivate Component Private
   */
  auto GetData() const noexcept -> DataType const& {
    if (data_ == nullptr) {
      ara::core::Abort("Cannot get data from header");
    }
    assert(data_ != nullptr);  // COV_MSR_INV_STATE_ASSERT
    return data_->GetData();   // VCA_VAC_VALID_RETURN_VALUE_RUN_TIME_CHECK
  }

  /*!
   * \brief     Replace the node \p old_node with \p new_node in the tree.
   * \details   \p is_root is necessary since Replace is used is locations where two nodes are supposed to change
   *            value, and when replacing the second node with the first, the IsRoot() property is violated and the
   *            IsRoot() property before the first Replace() has to be passed. If simply replacing a node with another
   *            this parameter can be left out (which then will use the IsRoot() function to determine if old_node is
   *            root).
   * \param[in] old_node  The old node, to be replaced by this.
   * \param[in] dir       The direction below it's parent \p old_node is placed.
   * \param[in] is_root   Boolean to declare if \p old_node is root.
   * \vprivate  Component Private
   */
  void Replace(RBNode const* old_node, std::uint8_t dir, bool is_root = false) noexcept {
    // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
    assert(this != old_node->GetParent());   // COV_MSR_INV_STATE_ASSERT
    this->SetParent(old_node->GetParent());  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
    if (this->GetParent() != nullptr) {
      if ((!old_node->IsRoot()) && (!is_root)) {
        // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
        this->GetParent()->SetChild(dir, this);
      } else {
        // If parent is header, set parent of header to this.
        this->GetParent()->SetParent(this);  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
      }
    } else {
      // Do not update parent if parent is nullptr.
    }

    this->SetChild(kLeft, nullptr);   // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
    this->SetChild(kRight, nullptr);  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
    if ((old_node->GetChild(kLeft) != this) && (old_node->GetChild(kLeft) != nullptr)) {
      this->SetChild(kLeft, old_node->GetChild(kLeft));  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
      this->GetChild(kLeft)->SetParent(this);            // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
    }
    if ((old_node->GetChild(kRight) != this) && (old_node->GetChild(kRight) != nullptr)) {
      this->SetChild(kRight, old_node->GetChild(kRight));  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
      this->GetChild(kRight)->SetParent(this);             // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
    }
    this->SetColor(old_node->GetColor());  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
  }

  /*!
   * \brief     Replace the node \p old_node with \p new_node in the tree.
   * \param[in] old_node  The old node, to be replaced by this.
   * \vprivate  Component Private
   */
  void Replace(RBNode const* old_node) noexcept {
    Replace(old_node, old_node->GetSameSideIdx());  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
  }

  /*!
   * \brief         Swaps the relations and color between this node and \p other.
   * \details       Swap is not possible to call for RBNode objects. Will abort if called. This function must. exists
   *                to avoid unnecessary casts between parent/child pointers.
   * \param[in,out] other  The node to swap value with.
   * \vprivate      Component Private
   */
  void SwapLinks(RBNode* other) noexcept {
    assert(other != nullptr);  // COV_MSR_INV_STATE_ASSERT
    if (other->IsHeader() || this->IsHeader()) {
      ara::core::Abort("Cannot swap a header.");
    }

    // Copy the links and the color of the node to be swapped, to a temporary node.
    RBNode cp_node{nullptr};
    cp_node.SetParent(other->GetParent());
    cp_node.SetChild(kLeft, other->GetChild(kLeft));
    cp_node.SetChild(kRight, other->GetChild(kRight));
    cp_node.SetColor(other->GetColor());

    std::uint8_t const other_dir{other->GetSameSideIdx()};
    bool const other_is_root{other->IsRoot()};

    other->Replace(this);                               // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
    this->Replace(&cp_node, other_dir, other_is_root);  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
  }

  /*!
   * \brief     Setter for the parent of the node.
   * \param[in] p  Pointer to the node to be set as parent.
   * \vprivate  Component Private
   */
  void SetParent(RBNode* const p) noexcept {
    // VCA_VAC_VALID_PTR_ASSIGNING_NOT_DEREFERENCING
    // VCA_VAC_VALID_ASSIGNING_NOT_ACCESSING
    // VCA_VAC_VALID_ACCESS_ASSIGNMENT
    parent_ = p;
  }

  /*!
   * \brief     Setter for the child at direction \p dir.
   * \param[in] dir  The direction to set child \p n.
   * \param[in] n    Pointer to the node to be set as child.
   * \vprivate  Component Private
   */
  void SetChild(std::uint8_t dir, RBNode* const n) noexcept {
    // VCA_VAC_VALID_PTR_ASSIGNING_NOT_DEREFERENCING
    // VCA_VAC_VALID_ASSIGNING_NOT_ACCESSING
    // VCA_VAC_VALID_ACCESS_ASSIGNMENT
    child_[dir] = n;
  }

  /*!
   * \brief     Setter for the color of the node.
   * \param[in] new_color  The color to set.
   * \vprivate  Component Private
   */
  void SetColor(bool new_color) noexcept {
    // VCA_VAC_VALID_PTR_ASSIGNING_NOT_DEREFERENCING
    // VCA_VAC_VALID_ASSIGNING_NOT_ACCESSING
    // VCA_VAC_VALID_ACCESS_ASSIGNMENT
    color_ = new_color;
  }

  /*!
   * \brief    Getter for the color of the node.
   * \details  Use help function GetColor(RBNode<Key, Value>* n) instead to get color of nullptr as well.
   * \return   The color of the node (kBlack = false, kRed = true).
   * \vprivate Component Private
   */
  auto GetColor() const noexcept -> bool { return color_; };

  /*!
   * \brief    Change the color of the node.
   * \details  Since the color can only be red or black, this function will color the node black if it is red, and vice
   *           versa.
   * \vprivate Component Private
   */
  void Recolor() noexcept {
    // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
    this->SetColor(!this->GetColor());
  }

  /*!
   * \brief    Checks if the node is the left child of it's parent.
   * \return   True if the node is the left child of it's parent.
   * \vprivate Component Private
   */
  auto IsLeftChild() const noexcept -> bool { return (parent_ != nullptr) && (parent_->GetChild(kLeft) == this); }

  /*!
   * \brief    Checks if the node is the right child of it's parent.
   * \return   True if the node is the right child of it's parent.
   * \vprivate Component Private
   */
  auto IsRightChild() const noexcept -> bool { return (parent_ != nullptr) && (parent_->GetChild(kRight) == this); }

  /*!
   * \brief    Get the index for the same child-direction as the node.
   * \details  This function is intended to be used for indexing the child-array of a node. For example:
   *           node->GetValidParent()->GetChild(node->GetSameSideChild()) = new_node;.
   * \return   Return the index for which side of it's parent the node is located (kLeft = 0, kRight = 1).
   * \vprivate Component Private
   */
  auto GetSameSideIdx() const noexcept -> std::uint8_t { return this->IsRightChild() ? kRight : kLeft; }

  /*!
   * \brief    Get the index for the opposite child-direction as the node.
   * \details  This function is intended to be used for indexing the child-array of a node. For example:
   *           node->GetValidParent()->GetChild(node->GetOppositeSideChild()) = new_node;.
   * \return   Return the index for which side of it's parent the node's sibling is located (kLeft = 0, kRight = 1).
   * \vprivate Component Private
   */
  auto GetOppositeSideIdx() const noexcept -> std::uint8_t { return GetOppositeDirection(this->GetSameSideIdx()); }

  /*!
   * \brief    Gets the amount of (non-NULL) children of the node.
   * \return   The amount of non-null children.
   * \vprivate Component Private
   */
  auto GetNumberOfChildren() const noexcept -> std::size_t {
    return static_cast<std::size_t>(static_cast<std::size_t>(this->GetChild(kLeft) != nullptr) +
                                    static_cast<std::size_t>(this->GetChild(kRight) != nullptr));
  }

  /*!
   * \brief    Returns whether *this is a leaf node.
   * \return   bool True if *this is a leaf, false otherwise.
   * \vprivate Component Private
   */
  auto IsLeaf() const noexcept -> bool {
    return (this->GetChild(kLeft) == nullptr) && (this->GetChild(kRight) == nullptr);
  }

  /*!
   * \brief    Returns whether *this has been visited.
   * \return   true if *this has been visited, false otherwise.
   * \vprivate Component Private
   */
  auto IsVisited() const noexcept -> bool { return visited_; }

  /*!
   * \brief    Sets whether *this has been visited.
   * \vprivate Component Private
   */
  void SetVisited(bool const b) const noexcept {
    visited_ = b;  // VCA_VAC_VALID_ACCESS_ASSIGNMENT
  }

 private:
  /*!
   * \brief The color of the node.
   */
  bool color_;

  /*!
   * \brief Flag to ease iterating through the tree. Used to keep track of whether the node has been visited in the
   *        iteration.
   */
  mutable bool visited_;

  /*!
   * \brief Link to the parent node.
   */
  RBNode* parent_;

  /*!
   * \brief Links to the child nodes.
   */
  // TODO: Change to amsr::Array when HUN-4041 is merged.
  ara::core::Array<RBNode*, 2> child_;

  /*!
   * \brief Link to data storage.
   */
  DataStructType* data_;
};

/*!
 * \brief     Increment iterator.
 * \details   Based off "local_Rb_tree_increment" function in STL:
 *            https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/src/c%2B%2B98/tree.cc
 * \tparam    Key    The type of the key.
 * \tparam    Value  The type of the value.
 * \param[in] n  pointer to the current position of an iterator.
 * \return    Returns a pointer to the next node.
 * \vprivate  Component Private
 */
template <typename Key, typename Value>
auto IncrementIterator(RBNode<Key, Value> const* n) noexcept -> RBNode<Key, Value>* {
  // VECTOR NL AutosarC++17_10-A5.2.3: MD_VAC_A5.2.3_constCastReducesCodeDuplication
  RBNode<Key, Value>* node{const_cast<RBNode<Key, Value>*>(n)};
  if (node->GetChild(kRight) != nullptr) {
    // The leftmost position of node's right child.
    node = node->GetChild(kRight);
    while (node->GetChild(kLeft) != nullptr) {
      node = node->GetChild(kLeft);
    }
  } else {
    // No right child.
    RBNode<Key, Value>* y{node->GetParent()};
    assert(y != nullptr);  // COV_MSR_INV_STATE_ASSERT
    while (node == y->GetChild(kRight)) {
      node = y;
      y = y->GetParent();
    }
    // If node == root, y == &header_: node will be set to &header_.
    if (node->GetChild(kRight) != y) {
      node = y;
    }
  }
  return node;
}

/*!
 * \brief     Decrement iterator.
 * \details   Based off "local_Rb_tree_decrement" function in STL:
 *            https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/src/c%2B%2B98/tree.cc
 * \tparam    Key    The type of the key.
 * \tparam    Value  The type of the value.
 * \param[in] n  pointer to the current position of an iterator.
 * \return    Returns a pointer to the previous node.
 * \vprivate  Component Private
 */
template <typename Key, typename Value>
auto DecrementIterator(RBNode<Key, Value> const* n) noexcept -> RBNode<Key, Value>* {
  // VECTOR NL AutosarC++17_10-A5.2.3: MD_VAC_A5.2.3_constCastReducesCodeDuplication
  RBNode<Key, Value>* node{const_cast<RBNode<Key, Value>*>(n)};
  if (node->IsHeader()) {
    node = node->GetChild(kRight);
  } else if (node->GetChild(kLeft) != nullptr) {
    RBNode<Key, Value>* y{node->GetChild(kLeft)};
    while (nullptr != y->GetChild(kRight)) {
      y = y->GetChild(kRight);
    }
    node = y;
  } else {
    RBNode<Key, Value>* y{node->GetParent()};
    while (node == y->GetChild(kLeft)) {
      node = y;
      y = y->GetParent();
    }
    node = y;
  }
  return node;
}

// Forward declaration of RBTreeConstIterator.
template <typename Key, typename Value>
class RBTreeConstIterator;

/*!
 * \brief    Iterator type for traversing a RBTree.
 * \details  Fulfills C++ named requirements: LegacyBiDirectionalIterator.
 * \tparam   Key    The type of the key.
 * \tparam   Value  The type of the value.
 * \vprivate Component Private
 */
template <typename Key, typename Value>
class RBTreeIterator final {
  /*!
   * \brief    Type of node to iterate over.
   * \vprivate Component Private
   */
  using NodeType = RBNode<Key, Value>;

 public:
  /*!
   * \brief    Type of value contained in the node.
   * \vprivate Component Private
   */
  using value_type = std::pair<Key const, Value>;
  /*!
   * \brief    Iterator category.
   * \vprivate Component Private
   */
  using iterator_category = std::bidirectional_iterator_tag;
  /*!
   * \brief    Type to measure distance between nodes.
   * \vprivate Component Private
   */
  using difference_type = std::ptrdiff_t;
  /*!
   * \brief    Pointer type.
   * \vprivate Component Private
   */
  using pointer = value_type*;
  /*!
   * \brief    Reference type.
   * \vprivate Component Private
   */
  using reference = value_type&;

  /*!
   * \brief         Construct iterator from a NodeType.
   * \param[in,out] node  The node that the iterator points to.
   * \vprivate      Component Private
   */
  explicit RBTreeIterator(NodeType* node) noexcept : node_{node} {}

  /*!
   * \brief    Deleted constructor.
   * \vprivate Component Private
   */
  RBTreeIterator() noexcept = delete;
  /*!
   * \brief    Default destructor.
   * \vprivate Component Private
   */
  ~RBTreeIterator() noexcept = default;
  /*!
   * \brief    Default copy constructor.
   * \vprivate Component Private
   */
  RBTreeIterator(RBTreeIterator const&) noexcept = default;
  /*!
   * \brief    Default move constructor.
   * \vprivate Component Private
   */
  RBTreeIterator(RBTreeIterator&&) noexcept = default;
  /*!
   * \brief    Default move assignment.
   * \vprivate Component Private
   */
  RBTreeIterator& operator=(RBTreeIterator&&) & noexcept = default;
  /*!
   * \brief    Default copy assignment.
   * \vprivate Component Private
   */
  RBTreeIterator& operator=(RBTreeIterator const&) & noexcept = default;

  /*!
   * \brief    Increment iterator.
   * \vprivate Component Private
   */
  auto operator++(int) noexcept -> RBTreeIterator {
    RBTreeIterator const tmp{*this};
    node_ = IncrementIterator(node_);
    return tmp;
  }
  /*!
   * \brief    Increment iterator.
   * \vprivate Component Private
   */
  auto operator++() noexcept -> RBTreeIterator& {
    node_ = IncrementIterator(node_);
    return *this;
  }

  /*!
   * \brief    Decrement iterator.
   * \vprivate Component Private
   */
  auto operator--(int) noexcept -> RBTreeIterator {
    RBTreeIterator const tmp{*this};
    node_ = DecrementIterator(node_);
    return tmp;
  }
  /*!
   * \brief    Decrement iterator.
   * \vprivate Component Private
   */
  auto operator--() noexcept -> RBTreeIterator& {
    node_ = DecrementIterator(node_);
    return *this;
  }

  /*!
   * \brief     Comparison operator.
   * \details   Compares whether the iterators stores the same pointer.
   * \param[in] other  Iterator to be compared with.
   * \return    true if other points to the same pointer, false otherwise.
   * \vprivate  Component Private
   */
  auto operator==(RBTreeIterator const& other) const noexcept -> bool { return node_ == other.node_; }

  /*!
   * \brief     Comparison operator.
   * \details   Compares whether the iterators stores the different pointers.
   * \param[in] other  Iterator to be compared with.
   * \return    true if other points to a different pointer, false otherwise.
   * \vprivate  Component Private
   */
  auto operator!=(RBTreeIterator const& other) const noexcept -> bool { return !(*this == other); }

  // VECTOR NC AutosarC++17_10-M5.2.3: MD_VAC_M5.2.3_castFromPolymorphicBaseClassToDerivedClassPreconditioned
  // VECTOR NC VectorC++-V5.2.3: MD_VAC_M5.2.3_castFromPolymorphicBaseClassToDerivedClassPreconditioned
  /*!
   * \brief    Dereference operator.
   * \details  Calling this operator when node_ is the tree's header node is undefined behavior.
   * \return   Reference to the object in node_.
   * \vprivate Component Private
   */
  auto operator*() noexcept -> reference {
    return node_->GetData();  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
  }

  // VECTOR NC AutosarC++17_10-M5.2.3: MD_VAC_M5.2.3_castFromPolymorphicBaseClassToDerivedClassPreconditioned
  // VECTOR NC VectorC++-V5.2.3: MD_VAC_M5.2.3_castFromPolymorphicBaseClassToDerivedClassPreconditioned
  /*!
   * \brief    Pointer operator.
   * \return   Pointer to the object in node_.
   * \vprivate Component Private
   */
  auto operator->() noexcept -> pointer {
    return &node_->GetData();  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
  }

  /*!
   * \brief    Getter for the pointer to the tree node.
   * \details  Should only be used internally by RBTree.
   * \return   Pointer to the node in the tree.
   * \vprivate Component Private
   */
  auto GetNode() const noexcept -> NodeType* { return node_; }

  /*!
   * \brief    Cast to non-const iterator.
   * \return   An regular iterator to the same object as this.
   * \vprivate Component Private
   */
  auto NonConstCast() const noexcept -> RBTreeIterator<Key, Value> { return *this; }

 private:
  /*!
   * \brief Node iterator points to.
   */
  NodeType* node_;
};

/*!
 * \brief    Const iterator type for traversing a RBTree.
 * \details  Fulfills C++ named requirements: LegacyBiDirectionalIterator.
 * \tparam   Key    The type of the key.
 * \tparam   Value  The type of the value.
 * \vprivate Component Private
 */
template <typename Key, typename Value>
class RBTreeConstIterator final {
  /*!
   * \brief Type of node to iterate over.
   */
  using NodeType = RBNode<Key, Value>;

 public:
  /*!
   * \brief    Type of value contained in the node.
   * \vprivate Component Private
   */
  using value_type = std::pair<Key const, Value>;
  /*!
   * \brief    Iterator category.
   * \vprivate Component Private
   */
  using iterator_category = std::bidirectional_iterator_tag;
  /*!
   * \brief    Type to measure distance between nodes.
   * \vprivate Component Private
   */
  using difference_type = std::ptrdiff_t;
  /*!
   * \brief    Pointer type.
   * \vprivate Component Private
   */
  using pointer = value_type const*;
  /*!
   * \brief    Reference type.
   * \vprivate Component Private
   */
  using reference = value_type const&;

  /*!
   * \brief         Construct const iterator from a NodeType.
   * \param[in,out] node  The node that the iterator points to.
   * \vprivate      Component Private
   */
  explicit RBTreeConstIterator(NodeType const* node) noexcept : node_{node} {}

  /*!
   * \brief     Constructor to create a const iterator from a regular iterator.
   * \param[in] it  The iterator to convert to const iterator.
   * \vprivate  Component Private
   */
  explicit RBTreeConstIterator(RBTreeIterator<Key, Value> const& it) noexcept : RBTreeConstIterator(it.GetNode()) {}

  /*!
   * \brief    Deleted constructor.
   * \vprivate Component Private
   */
  RBTreeConstIterator() noexcept = delete;
  /*!
   * \brief    Default destructor.
   * \vprivate Component Private
   */
  ~RBTreeConstIterator() noexcept = default;
  /*!
   * \brief    Default copy constructor.
   * \vprivate Component Private
   */
  RBTreeConstIterator(RBTreeConstIterator const&) noexcept = default;
  /*!
   * \brief    Default move constructor.
   * \vprivate Component Private
   */
  RBTreeConstIterator(RBTreeConstIterator&&) noexcept = default;
  /*!
   * \brief    Default move constructor.
   * \vprivate Component Private
   */
  RBTreeConstIterator& operator=(RBTreeConstIterator&&) & noexcept = default;
  /*!
   * \brief    Default copy constructor.
   * \vprivate Component Private
   */
  RBTreeConstIterator& operator=(RBTreeConstIterator const&) & noexcept = default;

  /*!
   * \brief    Increment iterator.
   * \vprivate Component Private
   */
  auto operator++(int) noexcept -> RBTreeConstIterator {
    RBTreeConstIterator const tmp{*this};
    node_ = IncrementIterator(node_);
    return tmp;
  }
  /*!
   * \brief    Increment iterator.
   * \vprivate Component Private
   */
  auto operator++() noexcept -> RBTreeConstIterator& {
    node_ = IncrementIterator(node_);
    return *this;
  }

  /*!
   * \brief    Decrement iterator.
   * \vprivate Component Private
   */
  auto operator--(int) noexcept -> RBTreeConstIterator {
    RBTreeConstIterator const tmp{*this};
    node_ = DecrementIterator(node_);
    return tmp;
  }
  /*!
   * \brief    Decrement iterator.
   * \vprivate Component Private
   */
  auto operator--() noexcept -> RBTreeConstIterator& {
    node_ = DecrementIterator(node_);
    return *this;
  }

  /*!
   * \brief     Comparison operator.
   * \details   Compares whether the iterators stores the same pointer.
   * \param[in] other  Iterator to be compared with.
   * \return    true if other points to the same pointer, false otherwise.
   * \vprivate  Component Private
   */
  auto operator==(RBTreeConstIterator const& other) const noexcept -> bool { return node_ == other.node_; }

  /*!
   * \brief     Comparison operator.
   * \details   Compares whether the iterators stores the different pointers.
   * \param[in] other  Iterator to be compared with.
   * \return    true if other points to a different pointer, false otherwise.
   * \vprivate  Component Private
   */
  auto operator!=(RBTreeConstIterator const& other) const noexcept -> bool { return !(*this == other); }

  // VECTOR NC AutosarC++17_10-M5.2.3: MD_VAC_M5.2.3_castFromPolymorphicBaseClassToDerivedClassPreconditioned
  // VECTOR NC VectorC++-V5.2.3: MD_VAC_M5.2.3_castFromPolymorphicBaseClassToDerivedClassPreconditioned
  /*!
   * \brief    Dereference operator.
   * \details  Calling this operator when node_ is the tree's header node will abort.
   * \return   Reference to the object in node_.
   * \vprivate Component Private
   */
  auto operator*() const noexcept -> reference {
    return node_->GetData();  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
  }

  // VECTOR NC AutosarC++17_10-M5.2.3: MD_VAC_M5.2.3_castFromPolymorphicBaseClassToDerivedClassPreconditioned
  // VECTOR NC VectorC++-V5.2.3: MD_VAC_M5.2.3_castFromPolymorphicBaseClassToDerivedClassPreconditioned
  /*!
   * \brief    Pointer operator.
   * \details  Calling this operator when node_ is the tree's header node will abort.
   * \return   Pointer to the object in node_.
   * \vprivate Component Private
   */
  auto operator->() const noexcept -> pointer {
    return &node_->GetData();  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
  }

  /*!
   * \brief    Getter for the pointer to the tree node.
   * \details  Should only be used internally by RBTree.
   * \return   Pointer to the node in the tree.
   * \vprivate Component Private
   */
  auto GetNode() const noexcept -> NodeType const* { return node_; }

  // VECTOR NC AutosarC++17_10-A5.2.3: MD_VAC_A5.2.3_iteratorConstCast
  /*!
   * \brief    Cast to non-const iterator.
   * \return   An regular iterator to the same object as this.
   * \vprivate Component Private
   */
  auto NonConstCast() const noexcept -> RBTreeIterator<Key, Value> {
    return RBTreeIterator<Key, Value>(const_cast<NodeType*>(node_));
  }

 private:
  /*!
   * \brief Node iterator points to.
   */
  NodeType const* node_;
};

/*!
 * \brief     Equality operator to compare a const iterator and a regular iterator.
 * \tparam    Key    The type of the key in the RBTree.
 * \tparam    Value  The type of the value in the RBTree.
 * \param[in] c_it  The const iterator
 * \param[in] it    The regular iterator.
 * \return    True if the iterators compare equal, false otherwise.
 * \vprivate  Product Private
 */
template <typename Key, typename Value>
auto operator==(RBTreeConstIterator<Key, Value> const& c_it, RBTreeIterator<Key, Value> const& it) noexcept -> bool {
  return c_it.GetNode() == it.GetNode();
}

/*!
 * \brief     Equality operator to compare a const iterator and a regular iterator.
 * \tparam    Key    The type of the key in the RBTree.
 * \tparam    Value  The type of the value in the RBTree.
 * \param[in] c_it  The const iterator
 * \param[in] it    The regular iterator.
 * \return    True if the iterators compare equal, false otherwise.
 * \vprivate  Product Private
 */
template <typename Key, typename Value>
auto operator==(RBTreeIterator<Key, Value> const& it, RBTreeConstIterator<Key, Value> const& c_it) noexcept -> bool {
  return c_it == it;
}

/*!
 * \brief     Not equal operator to compare a const iterator and a regular iterator.
 * \tparam    Key    The type of the key in the RBTree.
 * \tparam    Value  The type of the value in the RBTree.
 * \param[in] c_it  The const iterator
 * \param[in] it    The regular iterator.
 * \return    True if the iterators compare not equal, false otherwise.
 * \vprivate  Product Private
 */
template <typename Key, typename Value>
auto operator!=(RBTreeConstIterator<Key, Value> const& c_it, RBTreeIterator<Key, Value> const& it) noexcept -> bool {
  return !(c_it.GetNode() == it.GetNode());
}

/*!
 * \brief     Not equal operator to compare a const iterator and a regular iterator.
 * \tparam    Key    The type of the key in the RBTree.
 * \tparam    Value  The type of the value in the RBTree.
 * \param[in] c_it  The const iterator
 * \param[in] it    The regular iterator.
 * \return    True if the iterators compare not equal, false otherwise.
 * \vprivate  Product Private
 */
template <typename Key, typename Value>
auto operator!=(RBTreeIterator<Key, Value> const& it, RBTreeConstIterator<Key, Value> const& c_it) noexcept -> bool {
  return !(c_it == it);
}

/*!
 * \brief    Class for the header of a red black tree.
 * \details  The header contains a RBNode of type \p Node. The parent of the header node links to the root node of the
 *           tree. The left child and right child links to the leftmost node and the rightmost node, respectively. The
 *           header also records the size of the tree, i.e., the number of nodes.
 * \tparam   Node  The type of the node in the tree.
 * \vprivate Component Private
 */
template <typename Node>
class RBTreeHeader {
 public:
  /*!
   * \brief    Type conveying size.
   * \vprivate Component Private
   */
  using size_type = std::size_t;

  /*!
   * \brief    Alias type for a pointer to a Node.
   * \vprivate Component Private
   */
  using pointer = Node*;

  /*!
   * \brief    Alias type for a pointer to const Node.
   * \vprivate Component Private
   */
  using const_pointer = Node const*;

  /*!
   * \brief    Default constructor.
   * \vprivate Component Private
   */
  RBTreeHeader() noexcept {  // VCA_VAC_VALID_DEREFERENCED_POINTER_ACCESS
    header_.SetColor(kRed);
    ResetHeader();
  }
  /*!
   * \brief    Move constructor.
   * \vprivate Component Private
   */
  RBTreeHeader(RBTreeHeader&& other) noexcept = delete;

  /*!
   * \brief    Deleted copy constructor.
   * \vprivate Component Private
   */
  RBTreeHeader(RBTreeHeader const& other) noexcept = delete;

  /*!
   * \brief    Deleted copy assign.
   * \vprivate Component Private
   */
  RBTreeHeader& operator=(RBTreeHeader const&) noexcept = delete;
  /*!
   * \brief    Deleted move assign.
   * \vprivate Component Private
   */
  RBTreeHeader& operator=(RBTreeHeader&& other) noexcept = delete;
  /*!
   * \brief    Default destructor.
   * \vprivate Component Private
   */
  virtual ~RBTreeHeader() noexcept = default;  // VCA_VAC_POSSIBLY_FREEING_STATIC_MEMORY

  /*!
   * \brief         Move the header from \p other to this.
   * \details       \p Other is still valid after the move, but in a unspecified state.
   * \param[in,out] other  The tree from which to move the header.
   * \vprivate      Component Private
   */
  void MoveHeader(RBTreeHeader&& other) noexcept {
    this->node_count_ = other.node_count_;  // VCA_VAC_VALID_MEMBER_VARIABLE_ACCESS
    // Remap the links from other
    this->header_.SetParent(other.header_.GetParent());
    pointer const left_child{other.header_.GetChild(kLeft)->IsHeader() ? this->GetHeader()
                                                                       : other.header_.GetChild(kLeft)};
    pointer const right_child{other.header_.GetChild(kRight)->IsHeader() ? this->GetHeader()
                                                                         : other.header_.GetChild(kRight)};
    this->header_.SetChild(kLeft, left_child);
    this->header_.SetChild(kRight, right_child);
    this->header_.SetColor(other.header_.GetColor());

    if (other.GetRoot() != nullptr) {
      other.GetRoot()->SetParent(&this->header_);  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
    }

    // Set the header of other to not point to anything.
    other.ResetHeader();
  }

  /*!
   * \brief         Swaps the links of two headers and corrects the link from the tree's root to the header.
   * \param[in,out] header1  First header that will swap links.
   * \param[in,out] header2  Second header that will swap links.
   * \vprivate      Component Private
   */
  static void SwapHeaders(pointer header1, pointer header2) noexcept {
    pointer const tmp_parent{header1->GetParent()};
    header1->SetParent(header2->GetParent());
    if (header1->GetParent() != nullptr) {
      header1->GetParent()->SetParent(header1);
    }
    header2->SetParent(tmp_parent);
    if (header2->GetParent() != nullptr) {  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
      header2->GetParent()->SetParent(header2);
    }

    ara::core::Array<pointer, 2> tmp_children{header2->GetChild(kLeft), header2->GetChild(kRight)};

    pointer tmp_child{header1->GetChild(kLeft)};
    tmp_child = tmp_child->IsHeader() ? header2 : tmp_child;
    header2->SetChild(kLeft, tmp_child);

    tmp_child = tmp_children[kLeft];
    tmp_child = tmp_child->IsHeader() ? header1 : tmp_child;
    header1->SetChild(kLeft, tmp_child);

    tmp_child = header1->GetChild(kRight);
    tmp_child = tmp_child->IsHeader() ? header2 : tmp_child;
    header2->SetChild(kRight, tmp_child);

    tmp_child = tmp_children[kRight];
    tmp_child = tmp_child->IsHeader() ? header1 : tmp_child;
    header1->SetChild(kRight, tmp_child);
  }

  // VECTOR NC AutosarC++17_10-M5.2.3: MD_VAC_M5.2.3_castFromPolymorphicBaseClassToDerivedClassPreconditioned
  // VECTOR NC VectorC++-V5.2.3: MD_VAC_M5.2.3_castFromPolymorphicBaseClassToDerivedClassPreconditioned
  /*!
   * \brief    Getter for the root node of the tree.
   * \details  The root is the parent of the header.
   * \return   Pointer to the root node.
   * \vprivate Component Private
   */
  auto GetRoot() const noexcept -> pointer { return header_.GetParent(); }
  /*!
   * \brief    Getter for the root node of the tree.
   * \details  The root is the parent of the header.
   * \return   Pointer to the root node.
   * \vprivate Component Private
   */
  auto GetRoot() noexcept -> pointer { return header_.GetParent(); }

  /*!
   * \brief    Getter for the root node of the tree. Aborts if the root is nullptr.
   * \details  The root is the parent of the header.
   * \return   Pointer to the root node.
   * \vprivate Component Private
   */
  auto GetValidRoot() const noexcept -> pointer {
    auto const root = this->GetRoot();
    if (root == nullptr) {
      ara::core::Abort("Root is not valid");
    }
    return root;
  }

  /*!
   * \brief    Set the root.
   * \vprivate Component Private
   */
  void SetRoot(pointer node) noexcept {
    header_.SetParent(node);
    if (node != nullptr) {
      node->SetParent(&header_);  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
    }
  }

  /*!
   * \brief    Set the size of the tree. Should only be done when copying/moving a RBTree.
   * \vprivate Component Private
   */
  void SetSize(size_type size) noexcept { this->node_count_ = size; }

  /*!
   * \brief    Return pointer to header node.
   * \return   Pointer to header node.
   * \vprivate Component Private
   */
  auto GetHeader() noexcept -> pointer { return &header_; }

  /*!
   * \brief    Return pointer to header node.
   * \return   Pointer to header node.
   * \vprivate Component Private
   */
  auto GetHeader() const noexcept -> const_pointer { return &header_; }

  /*!
   * \brief    Getter for node_count.
   * \return   Number of elements in the tree.
   * \vprivate Component Private
   */
  auto GetNodeCount() const noexcept -> size_type { return node_count_; }

 protected:
  /*!
   * \brief    Increments node count.
   * \vprivate Component Private
   */
  void IncrementNodeCount() noexcept { ++node_count_; }

  /*!
   * \brief    Decrements node count.
   * \vprivate Component Private
   */
  void DecrementNodeCount() noexcept { --node_count_; }

  /*!
   * \brief    Resets header to its initial state, when the tree is empty.
   * \vprivate Component Private
   */
  void ResetHeader(bool reset_size = true) noexcept {
    if (reset_size) {
      node_count_ = 0;  // VCA_VAC_VALID_MEMBER_VARIABLE_ACCESS
    }
    header_.SetParent(nullptr);
    header_.SetChild(kLeft, &header_);
    header_.SetChild(kRight, &header_);
    header_.SetColor(kRed);
  }

 private:
  /*!
   * \brief The header node.
   */
  Node header_{nullptr};
  /*!
   * \brief Counter of how many nodes there are in the tree.
   */
  size_type node_count_{0};  // VCA_VAC_VALID_MEMBER_VARIABLE_ACCESS
};

// VECTOR Next Construct Metric-OO.WMC.One: MD_VAC_Metric-OO.WMC.One
/*!
 * \brief    A class for sorting RBNodes via the red black tree data structure.
 * \details  A red black tree is a balanced binary search tree. std::pair<Key const, Value> must fulfill either of the
 *           C++ named requirements: MoveConstructible or CopyConstructible.
 * \tparam   Key        Type of the key in the tree. The tree is sorted based on Key. Stored as a const value.
 * \tparam   Value      Type of the mapped-to value object in the nodes in the tree.
 * \tparam   Compare    A callable struct that is used to sort the tree. An object of this type is stored in the class.
 *                      Must fulfill C++ Named Requirements: Compare.
 * \tparam   Allocator  The allocator used to allocate memory for the nodes. An object of this type is stored in the
 *                      class. Allocator must fulfill the C++ Named Requirements: Allocator.
 * \vprivate Component Private
 */
template <typename Key, typename Value, typename Compare = std::less<Key const>,
          typename Allocator = ara::core::PolymorphicAllocator<RBNode<Key, Value>>>
class RBTree final : public RBTreeHeader<RBNode<Key, Value>> {
 public:
  /*!
   * \brief    Alias for node base type.
   * \vprivate Component Private
   */
  using NodeType = RBNode<Key, Value>;

 private:
  /*!
   * \brief Alias for RBTreeHeader.
   */
  using Header = RBTreeHeader<NodeType>;

  /*!
   * \brief Type stored in RBNode.
   */
  using pair_type = std::pair<Key const, Value>;

 public:
  /*!
   * \brief    Default predicate used with std::equal in CompareTreeEqual.
   * \vprivate Component Private
   */
  class BinaryPredicate {
   public:
    /*!
     * \brief     Invocable operator comparing parameters with the operator==.
     * \param[in] lhs  Left hand side of comparison.
     * \param[in] rhs  Right hand side of comparison.
     * \return    True if lhs and rhs are equal according to operator==, false otherwise.
     */
    auto operator()(pair_type const& lhs, pair_type const& rhs) noexcept -> bool {
      return (lhs.first == rhs.first) && (lhs.second == rhs.second);
    }
  };

  /*!
   * \brief    Default comparator used with std::lexicographical_compare in CompareTreeLexicographically.
   * \vprivate Component Private
   */
  class LexComparator {
   public:
    /*!
     * \brief     Invocable operator comparing parameters with the operator<.
     * \param[in] p1  Left hand side of comparison.
     * \param[in] p2  Right hand side of comparison.
     * \return    bool is p1 lesser than p2 according to the operator<.
     * \vprivate  Component Private
     */
    auto operator()(pair_type const& p1, pair_type const& p2) noexcept -> bool { return Compare{}(p1.first, p2.first); }
  };

  /*!
   * \brief    Type of the allocator.
   * \vprivate Component Private
   */
  using allocator_type = Allocator;

  /*!
   * \brief    Type of the allocator traits.
   * \vprivate Component Private
   */
  using allocator_traits = std::allocator_traits<allocator_type>;

  /*!
   * \brief    Type of the allocator.
   * \vprivate Component Private
   */
  using data_allocator_type = typename allocator_traits::template rebind_alloc<RBData<Key, Value>>;

  /*!
   * \brief    Type of the allocator.
   * \vprivate Component Private
   */
  using data_allocator_traits = std::allocator_traits<data_allocator_type>;

  /*!
   * \brief    Type used for size.
   * \vprivate Component Private
   */
  using size_type = std::size_t;

  /*!
   * \brief    Alias for the value type.
   * \vprivate Component Private
   */
  using value_type = typename allocator_traits::value_type;

  /*!
   * \brief    Pointer to a data node.
   * \vprivate Component Private
   */
  using pointer = typename allocator_traits::pointer;

  /*!
   * \brief    Alias type for a pointer to an element.
   * \vprivate Component Private
   */
  using const_pointer = typename allocator_traits::const_pointer;

  /*!
   * \brief    Iterator alias.
   * \details  RBTree iterators are read-only. Writing outside API would make tree unbalanced.
   * \vprivate Component Private
   */
  using iterator = RBTreeIterator<Key, Value>;

  /*!
   * \brief    Iterator alias.
   * \vprivate Component Private
   */
  using const_iterator = RBTreeConstIterator<Key, Value>;

  /*!
   * \brief    Iterator alias.
   * \vprivate Component Private
   */
  using reverse_iterator = std::reverse_iterator<iterator>;

  /*!
   * \brief    Iterator alias.
   * \vprivate Component Private
   */
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  // VECTOR NC AutosarC++17_10-A12.1.5: MD_VAC_A12.1.5_useDelegatingConstructor
  /*!
   * \brief     Constructor for a RBTree.
   * \param[in] allocator  The allocator to use when creating nodes for the tree.
   * \param[in] compare    A callable struct used to sort the red black tree.
   * \vprivate  Component Private
   */
  explicit RBTree(allocator_type const& allocator = allocator_type(), Compare const& compare = Compare()) noexcept
      : Header{}, allocator_(allocator), data_allocator_{allocator}, compare_(compare){};

  /*!
   * \brief     Constructor for a RBTree.
   * \param[in] compare  A callable struct used to sort the red black tree.
   * \vprivate  Component Private
   */
  explicit RBTree(Compare const& compare) noexcept : RBTree(allocator_type(), compare){};

  /*!
   * \brief    Deleted copy constructor.
   * \vprivate Component Private
   */
  RBTree(RBTree const& other) noexcept = delete;

  // VECTOR NC AutosarC++17_10-A12.8.4: MD_VAC_A12.8.4_moveConstructorShallNotUseCopySemantics
  /*!
   * \brief    Move constructor.
   * \vprivate Component Private
   */
  RBTree(RBTree&& other) noexcept
      : Header(),
        allocator_{std::move(other.allocator_)},  // VCA_VAC_VALID_ARGUMENT
        data_allocator_{std::move(other.data_allocator_)},
        compare_{std::move(other.compare_)}  // VCA_VAC_VALID_ARGUMENT
  {                                          // VCA_VAC_VALID_DEREFERENCED_POINTER_ACCESS
    Header::MoveHeader(std::move(other));
  };

  /*!
   * \brief    Deleted copy assignment operator.
   * \vprivate Component Private
   */
  RBTree& operator=(RBTree const& other) noexcept = delete;
  /*!
   * \brief    Deleted move assignment operator.
   * \vprivate Component Private
   */
  RBTree& operator=(RBTree&& other) noexcept = delete;

  /*!
   * \brief    Destroys the tree by destroying all nodes in the tree.
   * \details  Inflicts the following requirement on Key and Value: C++ named requirement Destructible.
   * \vprivate Component Private
   */
  ~RBTree() noexcept final {  // VCA_VAC_POSSIBLY_FREEING_STATIC_MEMORY
    DestroyTree();
  }

  /*!
   * \brief         Move construct a new tree. If \p allocator == this->GetAllocator(), other is move constructed into
   *                *this. Otherwise new elements are allocted with \p allocator and move constructed from elements in
   *                *this.
   * \details       Inflicts the following requirements on Key and Value: If allocator_traits::is_always_equal::value
   *                is false: C++ named requirement CopyConstructible.
   * \tparam        K2     The key type of the moved nodes.
   * \tparam        V2     The value type of the moved nodes.
   * \tparam        Comp   A callable struct that is used to sort the red black tree.
   * \tparam        Alloc  The allocator used to allocate memory for the nodes.
   * \param[in,out] old_tree   The header of the tree to copy from.
   * \param[in]     allocator  The allocator to use to allocate memory for the moved nodes.
   * \return        A result containing the new tree or an error.
   * \vprivate      Component Private
   */
  template <typename K2, typename V2, typename Comp, typename Alloc>
  static auto CreateTree(RBTree<K2, V2, Comp, Alloc>&& old_tree, allocator_type const& allocator) noexcept
      -> ara::core::Result<RBTree<K2, V2, Comp, Alloc>> {
    ara::core::Result<RBTree> res{amsr::ContainerErrorCode::kAllocationError};
    if (old_tree.GetAllocator() != allocator) {
      constexpr bool kMoveStorage{allocator_traits::is_always_equal::value};
      res.EmplaceValue(allocator, old_tree.GetCompare());
      ara::core::Result<void> const assign_res{
          res.Value().MoveAssign(std::move(old_tree), std::integral_constant<bool, kMoveStorage>{})};
      if (!assign_res.HasValue()) {
        res.EmplaceError(assign_res.Error());
      }
    } else {
      // This move constructor to RBTree is private, must be called here.
      res.EmplaceValue(RBTree(std::move(old_tree), allocator, data_allocator_type{allocator}));
    }
    return res;
  }

  /*!
   * \brief     Create a new tree with copies of all nodes in \p old_tree.
   * \details   Inflicts the following requirements on Key and Value: C++ named requirement CopyConstructible.
   * \tparam    K2     The key type of the copied nodes.
   * \tparam    V2     The value type of the copied nodes.
   * \tparam    Comp   A callable struct that is used to sort the red black tree.
   * \tparam    Alloc  The allocator used to allocate memory for the nodes.
   * \param[in] old_tree   The header of the tree to copy from.
   * \param[in] allocator  The allocator to use to allocate memory for the copied nodes.
   * \return    A result containing the new tree or an error.
   * \vprivate  Component Private
   */
  template <typename K2, typename V2, typename Comp, typename Alloc,
            std::enable_if_t<std::is_copy_constructible<std::pair<K2 const, V2>>::value, bool> = true>
  static auto CreateTree(RBTree<K2, V2, Comp, Alloc> const& old_tree, allocator_type const& allocator) noexcept
      -> ara::core::Result<RBTree<K2, V2, Comp, Alloc>> {
    ara::core::Result<RBTree> res{RBTree(allocator, old_tree.GetCompare())};

    ara::core::Result<void> const assign_res{res.Value().AssignElements(old_tree)};

    if (!assign_res.HasValue()) {
      res.EmplaceError(assign_res.Error());
    }

    return res;
  }

  /*!
   * \brief     Create a new tree with copies of all nodes in \p old_tree.
   * \details   Specialization when no allocator is given. Inflicts the following requirements on Key and Value: C++
   *            named requirement CopyConstructible.
   * \tparam    K2     The key type of the copied nodes.
   * \tparam    V2     The value type of the copied nodes.
   * \tparam    Comp   A callable struct that is used to sort the red black tree.
   * \tparam    Alloc  The allocator used to allocate memory for the nodes.
   * \param[in] old_tree  The header of the tree to copy from.
   * \return    A result containing the new tree or an error.
   * \vprivate  Component Private
   */
  template <typename K2, typename V2, typename Comp, typename Alloc,
            std::enable_if_t<std::is_copy_constructible<std::pair<K2 const, V2>>::value, bool> = true>
  static auto CreateTree(RBTree<K2, V2, Comp, Alloc> const& old_tree) noexcept
      -> ara::core::Result<RBTree<K2, V2, Comp, Alloc>> {
    return CreateTree(old_tree,
                      std::allocator_traits<Alloc>::select_on_container_copy_construction(old_tree.GetAllocator()));
  }

  /*!
   * \brief     Create a new tree with the contents of the range [first, last)
   * \details   Complexity is N*log(N) if the range [first, last) is not sorted with Compare, otherwise linear.
   *            Inflicts the following requirement(s) on Key: Comparable via template parameter \p Compare (default
   *            std::less which inflicts the C++ named requirements LessThanComparable). Key and Value must fulfill C++
   *            named requirement CopyConstructible.
   * \tparam    InputIterator  The type of the iterators in the range.
   * \param[in] first  The start of the range
   * \param[in] last   The end of the range
   * \param[in] alloc  The allocator to use to allocate the nodes in the tree.
   * \param[in] comp   A callable struct that is used to sort the red black tree.
   * \return    An result with an error code if no tree could be constructed, the constructed tree otherwise.
   * \vprivate  Component Private
   */
  template <class InputIterator,
            std::enable_if_t<std::is_convertible<typename std::iterator_traits<InputIterator>::iterator_category,
                                                 std::input_iterator_tag>::value,
                             bool> = true>
  static auto CreateTree(InputIterator first, InputIterator last, allocator_type const& alloc = allocator_type(),
                         Compare const& comp = Compare()) noexcept
      -> ara::core::Result<RBTree<Key, Value, Compare, allocator_type>> {
    ara::core::Result<RBTree<Key, Value, Compare, allocator_type>> result{
        RBTree<Key, Value, Compare, allocator_type>(alloc, comp)};
    // VECTOR NL AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_iteratorLeftPointingToEnd
    for (; first != last; ++first) {
      ara::core::Result<iterator> const emplace_res{
          result.Value().EmplaceHint(result.Value().CEnd(), *first)};  // VCA_VAC_VALID_ARGUMENT
      if (!emplace_res.HasValue()) {
        result.EmplaceError(emplace_res.Error());
        break;
      }
    }
    return result;
  }

  /*!
   * \brief     Create a new tree with the contents of the range [first, last)
   * \details   Complexity is N*log(N) if the range [first, last) is not sorted with Compare, otherwise linear.
   *            Inflicts the following requirement(s) on \p Key: Comparable via template parameter \p Compare (default
   *            std::less which inflicts the C++ named requirements LessThanComparable).
   * \tparam    InputIterator  The type of the iterators in the range.
   * \param[in] first  The start of the range
   * \param[in] last   The end of the range
   * \param[in] comp   A callable struct that is used to sort the red black tree.
   * \return    An result with an error code if no tree could be constructed, the constructed tree otherwise.
   * \vprivate  Component Private
   */
  template <class InputIterator,
            std::enable_if_t<std::is_convertible<typename std::iterator_traits<InputIterator>::iterator_category,
                                                 std::input_iterator_tag>::value,
                             bool> = true>
  static auto CreateTree(InputIterator first, InputIterator last, Compare const& comp) noexcept
      -> ara::core::Result<RBTree<Key, Value, Compare, allocator_type>> {
    return CreateTree(first, last, allocator_type(), comp);
  }

  /*!
   * \brief     Create a new tree from an initializer list
   * \details   Inflicts the following requirement(s) on \p Key: Comparable via template parameter \p Compare (default
   *            std::less which inflicts the C++ named requirements LessThanComparable).
   * \param[in] init   The initializer list from which to create a tree.
   * \param[in] alloc  The allocator to use to allocate the nodes in the tree.
   * \param[in] comp   A callable struct that is used to sort the red black tree.
   * \return    An result with an error code if no tree could be constructed, the constructed tree otherwise.
   * \vprivate  Component Private
   */
  static auto CreateTree(std::initializer_list<pair_type> init, allocator_type const& alloc = allocator_type(),
                         Compare const& comp = Compare()) noexcept
      -> ara::core::Result<RBTree<Key, Value, Compare, allocator_type>> {
    return CreateTree(init.begin(), init.end(), alloc, comp);
  }

  /*!
   * \brief     Create a new tree from an initializer list
   * \details   Inflicts the following requirement(s) on \p Key: Comparable via template parameter \p Compare (default
   *            std::less which inflicts the C++ named requirements LessThanComparable).
   * \param[in] init  The initializer list from which to create a tree.
   * \param[in] comp  A callable struct that is used to sort the red black tree.
   * \return    An result with an error code if no tree could be constructed, the constructed tree otherwise.
   * \vprivate  Component Private
   */
  static auto CreateTree(std::initializer_list<pair_type> init, Compare const& comp) noexcept
      -> ara::core::Result<RBTree<Key, Value, Compare, allocator_type>> {
    using input_iterator = typename std::initializer_list<pair_type>::iterator;
    return CreateTree<input_iterator>(init.begin(), init.end(), allocator_type(), comp);
  }

  /*!
   * \brief     Copy the content of \p other to this.
   * \param[in] other  The tree to copy
   * \return    A result with an error if an error occurs, otherwise empty.
   * \vprivate  Component Private
   */
  auto Assign(RBTree const& other) noexcept -> ara::core::Result<void> {
    ConditionalCopyAllocator(other.GetAllocator(), typename allocator_traits::propagate_on_container_copy_assignment());
    return AssignElements(other);
  }

  // VECTOR NC AutosarC++17_10-A12.8.3: MD_VAC_A12.8.3_movedFromObjectReadAccessAfterElementMove
  /*!
   * \brief         Move the content of another tree to this.
   * \details       If the allocator propagates, the header is moved, otherwise each element is move assigned to this.
   *                Inflicts the following requirement(s) on Key and Value: If
   *                (allocator_traits::propagate_on_container_move_assignment::value ||
   *                allocator_traits::is_always_equal::value) is true C++ named requirement CopyContructible.
   * \param[in,out] other  The tree to move to this.
   * \return        A result with an error if the allocation fails, otherwise empty.
   * \vprivate      Component Private
   */
  auto Assign(RBTree&& other) noexcept -> ara::core::Result<void> {
    constexpr bool kMoveStorage{allocator_traits::propagate_on_container_move_assignment::value ||
                                allocator_traits::is_always_equal::value};
    return MoveAssign(std::move(other), std::integral_constant<bool, kMoveStorage>{});
  }

  /*!
   * \brief     Replace the content of this by the objects in the \p init list.
   * \tparam    InputIterator  The type of the iterators in the range.
   * \param[in] init  The content to replace with.
   * \return    A result with an error if an error occurs, otherwise empty.
   * \vprivate  Component Private
   */
  auto Assign(std::initializer_list<pair_type> init) noexcept -> ara::core::Result<void> {
    ara::core::Result<void> result{};
    this->DestroyTree();
    for (auto it = init.begin(); it != init.end(); it++) {
      ara::core::Result<iterator> const res{this->EmplaceHint(CEnd(), *it)};  // VCA_VAC_VALID_ARGUMENT
      if (!res.HasValue()) {
        result.EmplaceError(amsr::ContainerErrorCode::kAllocationError);
        break;
      }
    }
    return result;
  }

  /*!
   * \brief     Insert a node with an element constructed by \p args in the red black tree.
   * \details   Inflicts the following requirement(s) on \p Key: Comparable via template parameter \p Compare (default
   *            std::less which inflicts the C++ named requirements LessThanComparable).
   * \tparam    Args  A template parameter pack with the types of the arguments to construct value_type.
   * \param[in] args  The arguments to construct an object of type value_type.
   * \return    A iterator to the inserted element.
   * \vprivate  Component Private
   */
  template <typename... Args, typename = std::enable_if_t<std::is_constructible<pair_type, Args...>::value>>
  auto Emplace(Args&&... args) noexcept -> ara::core::Result<iterator> {
    ara::core::Result<iterator> result{amsr::ContainerErrorCode::kAllocationError};
    constexpr std::size_t nr_of_elements{1};
    pointer const new_node{allocator_traits::allocate(allocator_, nr_of_elements)};
    RBData<Key, Value>* new_data{data_allocator_traits::allocate(data_allocator_, nr_of_elements)};
    if ((new_node != nullptr) && (new_data != nullptr)) {
      data_allocator_traits::construct(data_allocator_, new_data,  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
                                       std::forward<Args>(args)...);
      allocator_traits::construct(allocator_, new_node, new_data);  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT

      InsertInTree(new_node);
      new_node->SetColor(kRed);
      InsertRebalance(new_node);
      Header::IncrementNodeCount();
      result.EmplaceValue(iterator{new_node});
    } else {
      // Allocation failed, deallocate both.
      if (new_node != nullptr) {
        allocator_traits::deallocate(allocator_, new_node, nr_of_elements);  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
      }
      if (new_data != nullptr) {
        data_allocator_traits::deallocate(data_allocator_, new_data,  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
                                          nr_of_elements);
      }
    }
    return result;
  }

  /*!
   * \brief     Insert a node with an element constructed by \p args in the red black tree, as close to \p hint as
   *            possible.
   * \details   In general the complexity is log(n), but if the element is inserted just before \p hint the complexity
   *            is amortized constant. Inflicts the following requirement(s) on \p Key: Comparable via template
   *            parameter \p Compare (default std::less which inflicts the C++ named requirements LessThanComparable).
   * \tparam    Args  A template parameter pack with the types of the arguments to construct value_type.
   * \param[in] pos   A iterator to the wanted location of the element in the tree.
   * \param[in] args  The arguments to construct an object of type value_type.
   * \return    A iterator to the inserted element.
   * \vprivate  Component Private
   */
  template <typename... Args, typename = std::enable_if_t<std::is_constructible<pair_type, Args...>::value>>
  auto EmplaceHint(const_iterator pos, Args&&... args) noexcept -> ara::core::Result<iterator> {
    ara::core::Result<iterator> result{amsr::ContainerErrorCode::kAllocationError};
    constexpr std::size_t nr_of_elements{1};

    pointer new_node{allocator_traits::allocate(allocator_, nr_of_elements)};
    RBData<Key, Value>* new_data{data_allocator_traits::allocate(data_allocator_, nr_of_elements)};
    bool const allocation_succeeded{(new_node != nullptr) && (new_data != nullptr)};
    if (allocation_succeeded) {
      data_allocator_traits::construct(data_allocator_,  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
                                       new_data, std::forward<Args>(args)...);
      // Construct the element to insert.
      allocator_traits::construct(allocator_, new_node, new_data);  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
      assert(new_node != nullptr);                                  // COV_MSR_INV_STATE_ASSERT
      new_node->SetColor(kRed);

      if (pos == CEnd()) {
        InsertNodeAtEnd(new_node);
      } else if (compare_(new_node->GetData().first, (*pos).first)) {  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
        InsertNodeLeftOfPos(pos, new_node);
      } else if (compare_((*pos).first, new_node->GetData().first)) {  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
        InsertNodeRightOfPos(pos, new_node);
      } else {
        // The hinted location compare equal to the key in new_node.
        InsertInTree(new_node);
      }

      InsertRebalance(new_node);
      Header::IncrementNodeCount();
      result.EmplaceValue(new_node);
    } else {
      if (new_node != nullptr) {
        allocator_traits::deallocate(allocator_, new_node, nr_of_elements);  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
      }
      if (new_data != nullptr) {
        data_allocator_traits::deallocate(data_allocator_, new_data,  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
                                          nr_of_elements);
      }
    }
    return result;
  }

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConst
  /*!
   * \brief     Searches the tree for a node containing \p data.
   * \details   If no match was found, the function returns End(). Inflicts the following requirement(s) on \p Key: -
   *            Comparable via template parameter \p Compare (default std::less which inflicts the C++ named
   *            requirements LessThanComparable).
   * \tparam    U  The key type of the object to search for.
   * \param[in] data  The key to search for.
   * \return    A pointer to the node containing \p data.
   * \return    An iterator to the element containing data if it exists, End() otherwise.
   * \vprivate  Component Private
   */
  template <typename U = Key>
  auto Search(U const& data) noexcept -> iterator {
    const_iterator const it{this->GenericSearch(data)};  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
    return it.NonConstCast();
  }

  /*!
   * \brief     Searches the tree for a node containing \p data.
   * \details   If no match was found, the function returns End(). Inflicts the following requirement(s) on \p Key: -
   *            Comparable via template parameter \p Compare (default std::less which inflicts the C++ named
   *            requirements LessThanComparable).
   * \tparam    U  The key type of the object to search for.
   * \param[in] data  The key to search for.
   * \return    A pointer to the node containing \p data.
   * \return    An const iterator to the element containing data if it exists, CEnd() otherwise.
   * \vprivate  Component Private
   */
  template <typename U = Key>
  auto Search(U const& data) const noexcept -> const_iterator {
    return GenericSearch(data);
  }

  /*!
   * \brief         Deletes a node from the tree.
   * \details       Inflicts the following requirement on \p Key and \p Value: C++ named requirement Destructible.
   * \tparam        IteratorType  The iterator type.
   * \param[in,out] it  Iterator to the node to delete. n Must be valid and dereferencable. If \p it points to End(),
   *                    the function aborts.
   * \return        An iterator to the element following the removed element.
   * \vprivate      Component Private
   */
  template <typename IteratorType = iterator>
  auto Delete(IteratorType it) noexcept -> iterator {
    if (it == CEnd()) {
      ara::core::Abort("vac::container::internal::RBTree::Delete: Deleting end is invalid.");
    }
    pointer const n{it.NonConstCast().GetNode()};
    ++it;

    if (n->GetNumberOfChildren() == 2) {
      pointer successor{nullptr};
      // Replace the data with the data from the leftmost successor in the right tree.
      successor = GetLeftmostSuccessor(n->GetValidChild(kRight));  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
      assert(successor != nullptr);                                // COV_MSR_INV_STATE_ASSERT
      if (successor == n->GetChild(kRight)) {
        successor->Replace(n);  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
      } else {
        successor->SwapLinks(n);  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
        // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
        n->GetParent()->SetChild(n->GetSameSideIdx(), nullptr);
      }
      ReplaceExtremeValue(successor, n);
    } else if (n->GetNumberOfChildren() == 1) {
      // Replace the node with it's child and color it black.
      pointer const child{(n->GetChild(kRight) != nullptr) ? n->GetChild(kRight) : n->GetChild(kLeft)};
      assert(child != nullptr);  // COV_MSR_INV_STATE_ASSERT
      child->Replace(n);
      ReplaceExtremeValue(child, n);
      child->SetColor(kBlack);
    } else if (n->IsRoot()) {
      // n is root without children.
      this->ResetHeader(false);
    } else {
      // n has no children.
      ReplaceExtremeValue(n->GetParent(), n);
      if (GetColor(n) == kBlack) {
        this->DeleteRebalance(n);
      }
      n->GetValidParent()->SetChild(n->GetSameSideIdx(), nullptr);  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
    }

    Header::DecrementNodeCount();

    DestroyNode(n);

    return it.NonConstCast();
  }

  /*!
   * \brief         Swaps the contents of *this and other.
   * \param[in,out] other  RBTree to swap contents with.
   * \vprivate      Component Private
   */
  void Swap(RBTree& other) noexcept {
    pointer const this_header{this->GetHeader()};
    pointer const other_header{other.GetHeader()};
    Header::SwapHeaders(this_header, other_header);

    std::swap(other.compare_, this->compare_);

    size_type const tmp_size{this->Size()};
    this->SetSize(other.Size());
    other.SetSize(tmp_size);
    this->SwapAllocators(other.allocator_, typename allocator_traits::propagate_on_container_swap());
  }

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConst
  /*!
   * \brief     Get the first element larger or equal to \p key.
   * \details   Inflicts the following requirement(s) on \p Key: Comparable via template parameter \p Compare (default
   *            std::less which inflicts the C++ named requirements LessThanComparable). Inflicts the following
   *            requirement on \p Key: - Must be able to compare object of type U and Key.
   * \tparam    U  Type comparable to Key.
   * \param[in] key  The key to find the lower bound for.
   * \return    Iterator pointing to the first element that is equal to or larger than the value of key.
   * \vprivate  Component Private
   */
  template <typename U = Key>
  auto LowerBound(U const& key) noexcept -> iterator {
    return GenericLowerBound<U>(key).NonConstCast();
  }

  /*!
   * \brief     Get the first element larger or equal to \p key.
   * \details   Inflicts the following requirement(s) on \p Key: Comparable via template parameter \p Compare (default
   *            std::less which inflicts the C++ named requirements LessThanComparable). Inflicts the following
   *            requirement on \p Key: - Must be able to compare object of type U and Key.
   * \tparam    U  Type comparable to Key.
   * \param[in] key  The key to find the lower bound for.
   * \return    Iterator pointing to the first element that is equal to or larger than the value of key.
   * \vprivate  Component Private
   */
  template <typename U = Key>
  auto LowerBound(U const& key) const noexcept -> const_iterator {
    return GenericLowerBound<U>(key);
  }

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConst
  /*!
   * \brief     Get the first element smaller to \p key.
   * \details   Inflicts the following requirement on \p Compare: - Must be able to compare object of type U and Key.
   * \tparam    U  Type comparable to Key.
   * \param[in] key  The key to find the lower bound for.
   * \return    iterator to the upper bound node. End() if no upper bound is found.
   * \vprivate  Component Private
   */
  template <typename U = Key>
  auto UpperBound(U const& key) noexcept -> iterator {
    return GenericUpperBound<U>(key).NonConstCast();
  }

  /*!
   * \brief     Get the first element smaller to \p key.
   * \details   Inflicts the following requirement on \p Compare: - Must be able to compare object of type U and Key.
   * \tparam    U  Type comparable to Key.
   * \param[in] key  The key to find the lower bound for.
   * \return    const iterator to the upper bound node. End() if no upper bound is found.
   * \vprivate  Component Private
   */
  template <typename U = Key>
  auto UpperBound(U const& key) const noexcept -> const_iterator {
    return GenericUpperBound<U>(key);
  }

  /*!
   * \brief    Destroys and deallocates all nodes. *this will be in an empty state afterwards.
   * \details  All iterators and pointers to any element within the tree is invalid after calling Clear(). Inflicts the
   *           following requirement on Key: C++ named requirement Destructible.
   * \vprivate Component Private
   */
  void Clear() noexcept {
    DestroyTree();
    Header::ResetHeader();
  }

  /*!
   * \brief     Compares all nodes of both trees iff they are of the same size.
   * \tparam    BinaryPred  Binary predicate
   * \param[in] other      Tree to compare *this with.
   * \param[in] predicate  The predicate to use to compare equality.
   * \return    True if both trees have the same size and each node compares equally.
   * \vprivate  Component Private
   */
  template <typename BinaryPred = BinaryPredicate>
  auto CompareTreeEqual(RBTree const& other, BinaryPred predicate = BinaryPred{}) const noexcept -> bool {
    return (Size() == other.Size()) && (std::equal(CBegin(), CEnd(), other.CBegin(), predicate));
  }

  /*!
   * \brief     Compares *this with other lexicographically.
   * \tparam    LexComp  Lexicographic comparator.
   * \param[in] other           Tree to compare *this with.
   * \param[in] lex_comparator  The comparator to compare two pair_types.
   * \return    bool Returns true if *this < other is true lexiographically . False otherwise.
   * \vprivate  Component Private
   */
  template <typename LexComp = LexComparator>
  auto CompareTreeLexicographically(RBTree const& other, LexComp lex_comparator = LexComp{}) const noexcept -> bool {
    return std::lexicographical_compare<const_iterator, const_iterator, LexComp>(CBegin(), CEnd(), other.CBegin(),
                                                                                 other.CEnd(), lex_comparator);
  }

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConst
  /*!
   * \brief     Get the range of all elements with key \p key.
   * \tparam    K  The type of the key.
   * \param[in] key  The key to identify the range of.
   * \return    A iterator pair with the start and end of the range of elements with key \p key.
   * \vprivate  Component Private
   */
  template <class K2>
  auto EqualRange(K2 const& key) noexcept -> std::pair<iterator, iterator> {
    return {LowerBound(key), UpperBound(key)};
  }
  /*!
   * \brief     Get the range of all elements with key \p key.
   * \tparam    K2  The type of the key.
   * \param[in] key  The key to identify the range of.
   * \return    A const iterator pair with the start and end of the range of elements with key \p key.
   * \vprivate  Component Private
   */
  template <class K2>
  auto EqualRange(K2 const& key) const noexcept -> std::pair<const_iterator, const_iterator> {
    return {LowerBound(key), UpperBound(key)};
  }

  /*!
   * \brief    Get the allocator.
   * \return   A reference to the allocator.
   * \vprivate Component Private
   */
  auto GetAllocator() noexcept -> allocator_type& { return allocator_; }

  /*!
   * \brief    Get the allocator.
   * \return   A const reference to the allocator.
   * \vprivate Component Private
   */
  auto GetAllocator() const noexcept -> allocator_type const& { return allocator_; }

  /*!
   * \brief    Get the allocator used for RBData.
   * \return   A reference to the allocator.
   * \vprivate Component Private
   */
  auto GetDataAllocator() noexcept -> data_allocator_type& { return data_allocator_; }

  /*!
   * \brief    Get the allocator used for RBData.
   * \return   A const reference to the allocator.
   * \vprivate Component Private
   */
  auto GetDataAllocator() const noexcept -> data_allocator_type const& { return data_allocator_; }

  /*!
   * \brief    Get the comparator.
   * \return   A const reference to the comparator.
   * \vprivate Component Private
   */
  auto GetCompare() const noexcept -> Compare const& { return compare_; }

  /*!
   * \brief    Returns the size.
   * \return   Number of nodes in the tree.
   * \vprivate Component Private
   */
  auto Size() const noexcept -> size_type { return Header::GetNodeCount(); }

  /*!
   * \brief     Get the number of elements with a key comparing equally to \p key.
   * \tparam    K  The type of the given key.
   * \param[in] x  The key to get the count of.
   * \return    The number of contained elements with key \p x
   * \vprivate  Component Private
   */
  template <class K2>
  auto Count(K2&& x) const noexcept -> size_type {
    return (Search(x) != CEnd()) ? size_type{1} : size_type{0};
  }

  /*!
   * \brief    Returns the theoretical maximum number of elements that can be stored in the RBTree.
   * \return   Theoretical maximum number of elements.
   * \vprivate Component Private
   */
  static auto MaxSize() noexcept -> size_type {
    return std::numeric_limits<size_type>::max() / (sizeof(typename NodeType::DataType) + sizeof(value_type));
  }

  /*!
   * \brief    Returns iterator to leftmost node.
   * \return   iterator to beginning of tree.
   * \vprivate Component Private
   */
  auto Begin() noexcept -> iterator { return iterator{Header::GetHeader()->GetChild(kLeft)}; }

  /*!
   * \brief    Returns iterator to leftmost node.
   * \return   iterator to beginning of tree.
   * \vprivate Component Private
   */
  auto Begin() const noexcept -> const_iterator { return const_iterator{Header::GetHeader()->GetChild(kLeft)}; }

  /*!
   * \brief    Returns iterator to the element beyond the last element.
   * \return   iterator to element beyond last element.
   * \vprivate Component Private
   */
  auto End() noexcept -> iterator { return iterator{Header::GetHeader()}; }

  /*!
   * \brief    Returns iterator to the element beyond the last element.
   * \return   iterator to element beyond last element.
   * \vprivate Component Private
   */
  auto End() const noexcept -> const_iterator { return const_iterator{Header::GetHeader()}; }

  /*!
   * \brief    Returns iterator to the element beyond the last element.
   * \return   const_iterator.
   * \vprivate Component Private
   */
  auto CBegin() noexcept -> const_iterator { return const_iterator{Header::GetHeader()->GetChild(kLeft)}; }

  /*!
   * \brief    Returns iterator to the element beyond the last element.
   * \return   const_iterator.
   * \vprivate Component Private
   */
  auto CBegin() const noexcept -> const_iterator { return const_iterator{Header::GetHeader()->GetChild(kLeft)}; }

  /*!
   * \brief    Returns iterator to the element beyond the last element.
   * \return   const_iterator.
   * \vprivate Component Private
   */
  auto CEnd() noexcept -> const_iterator { return const_iterator{Header::GetHeader()}; }

  /*!
   * \brief    Returns iterator to the element beyond the last element.
   * \return   const_iterator.
   * \vprivate Component Private
   */
  auto CEnd() const noexcept -> const_iterator { return const_iterator{Header::GetHeader()}; }

  /*!
   * \brief    Gets the root node of the tree.
   * \return   reverse_iterator.
   * \vprivate Component Private
   */
  auto RBegin() noexcept -> reverse_iterator { return reverse_iterator{End()}; }

  /*!
   * \brief    Gets the root node of the tree.
   * \return   reverse_iterator.
   * \vprivate Component Private
   */
  auto RBegin() const noexcept -> const_reverse_iterator { return const_reverse_iterator{End()}; }

  /*!
   * \brief    Gets the reverse end of the tree.
   * \return   reverse_iterator, End() if tree is empty.
   * \vprivate Component Private
   */
  auto REnd() noexcept -> reverse_iterator { return reverse_iterator{Begin()}; }

  /*!
   * \brief    Gets the reverse end of the tree.
   * \return   reverse_iterator, End() if tree is empty.
   * \vprivate Component Private
   */
  auto REnd() const noexcept -> const_reverse_iterator { return const_reverse_iterator{Begin()}; }

  /*!
   * \brief    Gets the root node of the tree.
   * \return   const_reverse_iterator.
   * \vprivate Component Private
   */
  auto CRBegin() const noexcept -> const_reverse_iterator { return const_reverse_iterator{CEnd()}; }

  /*!
   * \brief    Gets the root node of the tree.
   * \return   const_reverse_iterator.
   * \vprivate Component Private
   */
  auto CREnd() const noexcept -> const_reverse_iterator { return const_reverse_iterator{CBegin()}; }

  /*!
   * \brief    Get the leftmost node of the tree.
   * \return   The rightmost node of the tree.
   * \vprivate Component Private
   */
  auto Leftmost() const noexcept -> pointer { return this->GetHeader()->GetChild(kLeft); }

  /*!
   * \brief    Get the rightmost node of the tree.
   * \return   The rightmost node of the tree.
   * \vprivate Component Private
   */
  auto Rightmost() const noexcept -> pointer { return this->GetHeader()->GetChild(kRight); }

  /*!
   * \brief     Set the leftmost node of the tree.
   * \param[in] node  The node to set to leftmost.
   * \vprivate  Component Private
   */
  void SetLeftmost(pointer node) noexcept { this->GetHeader()->SetChild(kLeft, node); }

  /*!
   * \brief     Get the rightmost node of the tree.
   * \param[in] node  The node to set to rightmost.
   * \vprivate  Component Private
   */
  void SetRightmost(pointer node) noexcept { this->GetHeader()->SetChild(kRight, node); }

 private:
  // VECTOR Next Construct AutosarC++17_10-M0.1.8: MD_VAC_M0.1.8_functionHasNoExternalSideEffect
  // VECTOR Next Construct AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredStatic
  /*!
   * \brief   On const *this, Clear() should not be called.
   * \details Called from GenericAssignElementsLoop.
   */
  void CallClear() const {}

  /*!
   * \brief   On non-const *this, Clear() should be called.
   * \details Called from GenericAssignElementsLoop.
   */
  void CallClear() { Clear(); }

  /*!
   * \brief         Uses move semantics to assign \p other to *this.
   * \details       Second parameter regards whether (allocator_traits::propagate_on_move_assignment::value) ||
   *                (allocator_traits::is_always_equal::value) is true.
   * \param[in,out] other  The RBTree to assign to *this.
   * \return        Result containing value.
   * \vprivate      Component Private
   */
  auto MoveAssign(RBTree&& other, std::true_type) -> ara::core::Result<void> {
    this->allocator_ = std::move(other.GetAllocator());
    this->DestroyTree();
    this->MoveHeader(std::move(other));
    return ara::core::Result<void>{};
  }

  /*!
   * \brief         Checks equality of allocators. If they are equal, move semantics is used, otherwise the elements
   *                are re-allocated using the allocators of *this.
   * \details       Second parameter regards whether (allocator_traits::propagate_on_move_assignment::value) ||
   *                (allocator_traits::is_always_equal::value) is false.
   * \param[in,out] other  The RBTree to assign to *this.
   * \return        Result with value is no allocation errors occur, otherwise it contains the error
   *                amsr::ContainerErrorCode::kAllocationError.
   * \vprivate      Component Private
   */
  auto MoveAssign(RBTree&& other, std::false_type) -> ara::core::Result<void> {
    ara::core::Result<void> res;
    if (this->allocator_ == other.GetAllocator()) {
      this->allocator_ = std::move(other.GetAllocator());
      this->DestroyTree();
      this->MoveHeader(std::move(other));
    } else {
      res = AssignElements(std::move(other));
    }
    return res;
  }

  // VECTOR NC AutosarC++17_10-A12.1.5: MD_VAC_A12.1.5_useDelegatingConstructor
  /*!
   * \brief         Move constructor.
   * \details       This constructor should only be used when allocator_traits::is_always_equal::value is true or when
   *                the allocator of \p other is equal to \p alloc.
   * \param[in,out] other       RBTree from which to construct *this.
   * \param[in]     alloc       Allocator to to handle the memory of RBNode.
   * \param[in]     data_alloc  Allocator to handle the memory of RBData.
   * \vprivate      Component Private
   */
  RBTree(RBTree&& other, allocator_type const& alloc, data_allocator_type const& data_alloc) noexcept
      : Header(), allocator_(alloc), data_allocator_{data_alloc}, compare_{std::move(other.compare_)} {
    Header::MoveHeader(std::move(other));
  }

  /*!
   * \brief     Searches the tree for a node containing \p data.
   * \details   If no match was found, the function returns End(). Inflicts the following requirement(s) on \p Key: -
   *            Comparable via template parameter \p Compare (default std::less which inflicts the C++ named
   *            requirements LessThanComparable).
   * \tparam    U  The data type of the object to search for.
   * \param[in] data  The data to search for.
   * \return    An const_iterator to the element containing data if it exists, CEnd() otherwise.
   * \vprivate  Component Private
   */
  template <typename U = Key>
  auto GenericSearch(U const& data) const noexcept -> const_iterator {
    const_iterator const lb{GenericLowerBound<U>(data)};  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
    // lb is the largest element equal to, or larger than data, so if data isn't compare less than lb, they are equal.
    return ((lb != CEnd()) && (!this->compare_(data, lb->first))) ? lb : CEnd();
  }

  /*!
   * \brief         Calls swap(...) on the allocator of *this and other.
   * \param[in,out] other  The allocator to swap with.
   * \vprivate      Component Private
   */
  void SwapAllocators(allocator_type& other, std::true_type) noexcept { swap(allocator_, other); }

  // VECTOR NC VectorC++-V0.1.8: MD_VAC_M0.1.8_emptyVoidFunctionWithoutExternalSideEffect
  // VECTOR NC AutosarC++17_10-M0.1.8: MD_VAC_M0.1.8_emptyVoidFunctionWithoutExternalSideEffect
  /*!
   * \brief    Doesn't call swap(...) on the allocator of *this and other.
   * \vprivate Component Private
   */
  static void SwapAllocators(allocator_type const&, std::false_type) noexcept {}

  /*!
   * \brief     Copies other into the allocator of *this.
   * \param[in] other  Allocator to copy from.
   * \vprivate  Component Private
   */
  void ConditionalCopyAllocator(allocator_type const& other, std::true_type) noexcept {
    if ((!allocator_traits::is_always_equal::value) && (allocator_ != other)) {
      // replacement allocator cannot free existing storage
      this->DestroyTree();
    }
    this->allocator_ = allocator_traits::select_on_container_copy_construction(other);
  }

  // VECTOR NC VectorC++-V0.1.8: MD_VAC_M0.1.8_emptyVoidFunctionWithoutExternalSideEffect
  // VECTOR NC AutosarC++17_10-M0.1.8: MD_VAC_M0.1.8_emptyVoidFunctionWithoutExternalSideEffect
  /*!
   * \brief    Doesn't copy other into the allocator of *this.
   * \vprivate Component Private
   */
  static void ConditionalCopyAllocator(allocator_type const&, std::false_type) noexcept {}

  /*!
   * \brief         Assigns this to \p other using element wise move assignment.
   * \param[in,out] other  The tree to move from.
   * \return        An empty result if no error occurs, the error otherwise.
   * \vprivate      Component Private
   */
  auto AssignElements(RBTree&& other) noexcept -> ara::core::Result<void> {
    return GenericAssignElements<RBTree, true>(std::move(other));
  }

  /*!
   * \brief     Assigns this to \p other using element wise copy assignment.
   * \param[in] other  The tree to copy from.
   * \return    An empty result if no error occurs, the error otherwise.
   * \vprivate  Component Private
   */
  auto AssignElements(RBTree const& other) noexcept -> ara::core::Result<void> {
    return GenericAssignElements<RBTree const&, false>(other);
  }

  /*!
   * \brief    Erases all nodes with node.visited = false.
   * \details  Assumes that if a node n is not visited, all nodes in n's subtree is also not visited.
   * \vprivate Component Private
   */
  void EraseAllNonVisited() noexcept {
    iterator it{GetLeftmostSuccessor(Header::GetRoot())};

    while (CEnd() != it) {
      if (!it.GetNode()->IsVisited()) {
        pointer const cur_ptr{it.GetNode()};
        pointer const parent{cur_ptr->GetParent()};
        ReplaceExtremeValue(parent, cur_ptr);
        EraseAllNodes(cur_ptr);
        it = iterator{parent};
      } else {
        ++it;
      }
    }
  }

  /*!
   * \brief    Iterates through the tree and sets all visited flags to false.
   * \vprivate Component Private
   */
  void ResetVisitedFlags() noexcept {
    iterator iter{Begin()};

    while (iter != End()) {
      assert(iter.GetNode() != nullptr);  // COV_MSR_INV_STATE_ASSERT
      iter.GetNode()->SetVisited(false);
      ++iter;
    }
  }

  // VECTOR Next Construct AutosarC++17_10-A8.4.4: MD_VAC_A8.4.4_useReturnValueInsteadOfOutputParameter
  /*!
   * \brief         Moves into the child of both \p this_elem and \p other_elem in the direction of \p dir.
   * \details       If this_elem does not have a child in the direction of dir, a new node is allocated and added as
   *                that missing child.
   * \tparam        OtherNodePointer  Type of pointer in another tree, being assigned into *this.
   * \param[in,out] this_elem   Pointer to node in *this.
   * \param[in,out] other_elem  Pointer to node in another tree, being assigned into *this.
   * \param[in]     dir         Direction to the child to traverse.
   * \return        An empty result if no error occurred, otherwise a result with the error.
   * \vprivate      Component Private
   */
  auto TraverseChild(pointer& this_elem, pointer& other_elem, std::uint8_t dir) noexcept -> ara::core::Result<void> {
    ara::core::Result<void> res{};

    if (this_elem->GetChild(dir) == nullptr) {
      constexpr std::size_t nr_of_elements{1};
      pointer const new_node{allocator_traits::allocate(allocator_, nr_of_elements)};
      if (new_node == nullptr) {
        res.EmplaceError(amsr::ContainerErrorCode::kAllocationError);
      } else {
        allocator_traits::construct(allocator_, new_node, nullptr);  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
        assert(this_elem != nullptr);                                // COV_MSR_INV_STATE_ASSERT
        this_elem->SetChild(dir, new_node);                          // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
        new_node->SetParent(this_elem);
        Header::IncrementNodeCount();
      }
    }
    if (this_elem->GetChild(dir) != nullptr) {
      this_elem = this_elem->GetChild(dir);
    }
    other_elem = other_elem->GetChild(dir);
    return res;
  }

  /*!
   * \brief         Assigns other_elem to this_elem. Sets both elems to visited.
   * \tparam        TreeType  Type of the tree to move from
   * \tparam        UseMove   Boolean that is true if move semantics shall be used.
   * \param[in]     other       The tree to copy to this.
   * \param[in,out] this_elem   Pointer to node in *this.
   * \param[in,out] other_elem  Pointer to node in another tree, being assigned into *this.
   * \return        An empty result if no error occurred, otherwise a result with the error.
   * \vprivate      Component Private
   */
  template <typename TreeType, bool UseMove>
  auto VisitNode(TreeType const& other, pointer this_elem, pointer other_elem) noexcept -> ara::core::Result<void> {
    ara::core::Result<void> res{};
    assert(this_elem != nullptr);  // COV_MSR_INV_STATE_ASSERT
    ara::core::Result<void> const assign_res{this_elem->template CopyNode(other_elem, *this)};
    if (!assign_res.HasValue()) {
      // Forward error.
      res.EmplaceError(assign_res.Error());
    } else {
      assert(this_elem != nullptr);  // COV_MSR_INV_STATE_ASSERT
      this_elem->SetVisited(true);
      assert(other_elem != nullptr);  // COV_MSR_INV_STATE_ASSERT
      other_elem->SetVisited(true);   // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
      ReplaceExtremeValue(this_elem, other_elem, other.Leftmost(), other.Rightmost());
      if (other_elem->IsRoot()) {
        // this_elem can't be null here since any missing node is created either in TraverseChild or
        // above if *this is missing a root.
        this_elem->SetParent(Header::GetHeader());  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
      }
    }
    return res;
  }

  /*!
   * \brief         Copy assign a RBTree by copy assigning all possible elements from \p other.
   * \details       If \p other has more elements than this, the new elements will be allocated using the allocator of
   *                this. If this has more elements than \p other, the elements will be destroyed using the allocator
   *                of this.
   * \tparam        TreeType  Type of the tree to move from
   * \tparam        UseMove   Boolean that is true if move semantics shall be used.
   * \param[in,out] other  The tree to copy to this.
   * \return        An empty result if success, otherwise an allocation error code.
   * \vprivate      Component Private
   */
  template <typename TreeType, bool UseMove>
  auto GenericAssignElements(TreeType&& other) noexcept -> ara::core::Result<void> {
    ara::core::Result<void> res{};
    pointer this_elem{Header::GetRoot()};
    pointer const other_elem{other.GetRoot()};

    if (other_elem != nullptr) {
      // If *this is empty and other is not, create a root node.
      if (this_elem == nullptr) {
        constexpr std::size_t nr_of_elements{1};
        pointer const new_node{allocator_traits::allocate(allocator_, nr_of_elements)};
        if (new_node == nullptr) {
          // fix result
          res.EmplaceError(amsr::ContainerErrorCode::kAllocationError);
        } else {
          allocator_traits::construct(allocator_, new_node, nullptr);  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
          new_node->SetParent(Header::GetHeader());
          this->SetRoot(new_node);
          this->IncrementNodeCount();
          this->SetLeftmost(new_node);
          this->SetRightmost(new_node);
          this_elem = new_node;
        }
      }

      if (res.HasValue()) {
        res = GenericAssignElementsLoop<TreeType, UseMove>(std::forward<TreeType>(other), this_elem, other_elem);
      }

      if (res.HasValue()) {
        EraseAllNonVisited();
        ResetVisitedFlags();
      }
    }
    return res;
  }

  /*!
   * \brief         Iterates through all elements of other. Copy/move assigns elements from other into *this.
   * \details       If \p other has more elements than this, the new elements will be allocated using the allocator of
   *                this. If this has more elements than \p other, the elements will be destroyed using the allocator
   *                of this. If *this is lacking a node for a position, a new node is allocated and assigned to.
   * \tparam        TreeType  Type of the tree to assign from
   * \tparam        UseMove   Boolean that is true if move semantics shall be used.
   * \param[in,out] other       The tree to assign to this.
   * \param[in,out] this_elem   Root node of this.
   * \param[in,out] other_elem  Root node of other.
   * \return        An empty result if success, otherwise an allocation error code.
   * \vprivate      Component Private
   */
  template <typename TreeType, bool UseMove>
  auto GenericAssignElementsLoop(TreeType&& other, pointer this_elem, pointer other_elem) noexcept
      -> ara::core::Result<void> {
    ara::core::Result<void> res{};
    // Loop until the tree is fully traversed or an allocation fails.
    // Each node is set as visited when a node in other is assigned to a node in *this.
    // If no node exists in *this in a position where a node exists in other, a node is created for this position.
    while ((other_elem != other.GetHeader())) {
      if (!other_elem->IsVisited()) {
        if ((other_elem->GetChild(kLeft) != nullptr) && (!other_elem->GetChild(kLeft)->IsVisited())) {
          // this_elem can't be null here since any missing node is created either in TraverseChild or
          // above if *this is missing a root.
          res = TraverseChild(this_elem, other_elem, kLeft);
        } else if ((other_elem->GetChild(kRight) != nullptr) && (!other_elem->GetChild(kRight)->IsVisited())) {
          // this_elem can't be null here since any missing node is created either in TraverseChild or
          // above if *this is missing a root.
          res = TraverseChild(this_elem, other_elem, kRight);
        } else {
          res = VisitNode<TreeType, UseMove>(other, this_elem, other_elem);
        }
        if (!res.HasValue()) {
          break;
        }
      } else {
        // Everything in subtree is visited, go upwards.
        this_elem = this_elem->GetParent();
        other_elem = other_elem->GetParent();
      }
    }

    // Calls clear if move semantics are used.
    other.CallClear();

    return res;
  }

  /*!
   * \brief     Iterates through the tree and destroys each leaf until only the header remains.
   * \param[in] start_node  top of the subtree which to destroy.
   * \vprivate  Component Private
   */
  void EraseAllNodes(pointer start_node) noexcept {
    pointer const top{start_node->GetParent()};
    pointer cur_ptr{start_node};
    while (cur_ptr != top) {
      while (!cur_ptr->IsLeaf()) {
        if (cur_ptr->GetChild(kLeft) != nullptr) {
          cur_ptr = cur_ptr->GetChild(kLeft);
        } else {
          assert(cur_ptr->GetChild(kRight) != nullptr);  // COV_MSR_INV_STATE_ASSERT
          cur_ptr = cur_ptr->GetChild(kRight);
        }
      }
      pointer const parent{cur_ptr->GetParent()};
      assert(parent != nullptr);  // COV_MSR_INV_STATE_ASSERT
      parent->SetChild(cur_ptr->GetSameSideIdx(), nullptr);
      DestroyNode(cur_ptr);
      Header::DecrementNodeCount();
      cur_ptr = parent;
    }
  }

  /*!
   * \brief    Destroys and deallocates all nodes.
   * \vprivate Component Private
   */
  void DestroyTree() noexcept {
    if (Size() != 0) {
      EraseAllNodes(Header::GetRoot());
    }
    Header::ResetHeader(true);
  }

  /*!
   * \brief         Inserts a new node at the end of the tree, after the rightmost node.
   * \param[in,out] new_node  Node to be inserted.
   * \vprivate      Component Private
   */
  void InsertNodeAtEnd(pointer new_node) noexcept {
    assert(this->GetHeader()->GetChild(kRight) != nullptr);  // COV_MSR_INV_STATE_ASSERT
    pointer const rightmost{this->GetHeader()->GetChild(kRight)};
    assert(rightmost != nullptr);                               // COV_MSR_INV_STATE_ASSERT
    assert(new_node != nullptr);                                // COV_MSR_INV_STATE_ASSERT
    if ((Size() != 0) && (compare_(rightmost->GetData().first,  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
                                   new_node->GetData().first))) {
      AddNewNode(new_node, rightmost, kRight);
      this->SetRightmost(new_node);
    } else {
      InsertInTree(new_node);
    }
  }

  /*!
   * \brief         Insert a new node at the position left of \p pos.
   * \param[in]     pos       The position used to insert the new node.
   * \param[in,out] new_node  The new node to insert.
   * \vprivate      Component Private
   */
  void InsertNodeLeftOfPos(const_iterator pos, pointer new_node) noexcept {
    const_iterator before{pos};
    // Key of new_node is more left than pos.
    assert(new_node != nullptr);  // COV_MSR_INV_STATE_ASSERT
    if (before == Begin()) {
      AddNewNode(new_node, this->GetHeader()->GetChild(kLeft), kLeft);
    } else if (compare_((*--before).first, new_node->GetData().first)) {
      if (before.GetNode()->GetChild(kRight) == nullptr) {
        AddNewNode(new_node, before.NonConstCast().GetNode(), kRight);
      } else {
        AddNewNode(new_node, pos.NonConstCast().GetNode(), kLeft);
      }
    } else {
      InsertInTree(new_node);
    }
  }

  /*!
   * \brief         Insert a new node at the position right of \p pos.
   * \param[in]     pos       The position used to insert the new node.
   * \param[in,out] new_node  The new node to insert.
   * \vprivate      Component Private
   */
  void InsertNodeRightOfPos(const_iterator pos, pointer new_node) noexcept {
    const_iterator after{pos};
    // Key of new_node is more right than pos.
    // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
    if ((++after != CEnd()) && (compare_(new_node->GetData().first, (*after).first))) {
      if (after.GetNode()->GetChild(kLeft) == nullptr) {
        AddNewNode(new_node, after.NonConstCast().GetNode(), kLeft);
      } else {
        AddNewNode(new_node, pos.NonConstCast().GetNode(), kRight);
      }
    } else {
      InsertInTree(new_node);
    }
  }

  /*!
   * \brief     Get the leftmost node in the subtree where \p root is the root.
   * \param[in] root_node  The root of the subtree.
   * \return    A pointer to the leftmost node.
   * \vprivate  Component Private
   */
  static auto GetLeftmostSuccessor(pointer root_node) noexcept -> pointer {
    assert(root_node != nullptr);
    pointer cur{root_node};
    pointer out{cur};
    while (cur != nullptr) {
      out = cur;
      cur = cur->GetChild(kLeft);
    }
    return out;
  }

  /*!
   * \brief     Get the first element larger or equal to \p key.
   * \details   Inflicts the following requirement(s) on \p Key: Comparable via template parameter \p Compare (default
   *            std::less which inflicts the C++ named requirements LessThanComparable). Inflicts the following
   *            requirement on \p Key: - Must be able to compare object of type U and Key.
   * \tparam    U  The type of the object to find the lower bound of.
   * \param[in] key  The value to find the lower bound for.
   * \return    Iterator pointing to the first element that is equal to or larger than the value of \p key.
   * \vprivate  Component Private
   */
  template <typename U = Key>
  auto GenericLowerBound(U const& key) const noexcept -> const_iterator {
    const_pointer out{Header::GetHeader()->GetChild(kRight)};
    const_pointer cur{Header::GetRoot()};

    while ((cur != nullptr)) {
      if (!compare_(cur->GetData().first, key)) {
        // key less than or equal to child data
        out = cur;
        cur = cur->GetChild(kLeft);
      } else {
        if (cur == Header::GetHeader()->GetChild(kRight)) {
          out = Header::GetHeader();
        }
        cur = cur->GetChild(kRight);
      }
    }
    return out->IsHeader() ? const_iterator{Header::GetHeader()} : const_iterator{out};
  }

  /*!
   * \brief     Get the first element smaller to \p key.
   * \details   Inflicts the following requirement on \p Compare: - Must be able to compare object of type U and Key.
   * \tparam    U  The type of the object to find the upper bound of.
   * \param[in] key  Value to find the upper bound of.
   * \return    iterator to the upper bound node. End() if no upper bound is found.
   * \vprivate  Component Private
   */
  template <typename U = Key>
  auto GenericUpperBound(U const& key) const noexcept -> const_iterator {
    const_pointer out{Header::GetHeader()->GetChild(kRight)};
    const_pointer cur{Header::GetRoot()};

    while (cur != nullptr) {
      if (compare_(key, cur->GetData().first)) {
        out = cur;
        cur = cur->GetChild(kLeft);
      } else {
        if (cur == Header::GetRoot()->GetChild(kRight)) {
          out = Header::GetHeader();
        }
        cur = cur->GetChild(kRight);
      }
    }
    return out->IsHeader() ? const_iterator{Header::GetHeader()} : const_iterator{out};
  }

  /*!
   * \brief         Insert a node at the correct place in the binary tree.
   * \param[in,out] new_node  The node to insert in the tree.
   * \vprivate      Component Private
   */
  void InsertInTree(pointer new_node) noexcept {
    assert(new_node != nullptr);  // COV_MSR_INV_STATE_ASSERT
    // Begin search at root.
    pointer x{Header::GetRoot()};
    pointer y{Header::GetHeader()};

    std::uint8_t dir{kLeft};
    // Find a parent, y, to a null node x.
    while (x != nullptr) {
      y = x;
      dir = compare_(y->GetData().first, new_node->GetData().first);  // VCA_VAC_VALID_FUNC_CALL
      x = x->GetChild(dir);
    }

    pointer const parent{y};
    pointer const header_ptr{Header::GetHeader()};
    new_node->SetParent(parent);  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
    if (parent != header_ptr) {
      parent->SetChild(dir, new_node);  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
      if (parent == header_ptr->GetChild(dir)) {
        header_ptr->SetChild(dir, new_node);
      }
    } else {
      header_ptr->SetParent(new_node);
      header_ptr->SetChild(kRight, new_node);
      header_ptr->SetChild(kLeft, new_node);
    }
  }

  /*!
   * \brief         Rotates a subtree in direction given by \p direction.
   * \param[in,out] target_node  Pointer to the root of the subtree to be rotated.
   * \param[in]     direction    The direction to rotate in.
   * \pre           \p target_node must have a non-null child on the opposite side to \p direction.
   * \vprivate      Component Private
   */
  void Rotate(pointer target_node, std::uint8_t direction) noexcept {
    assert(target_node != nullptr);  // COV_MSR_INV_STATE_ASSERT
    //  Assert the preconditions are fulfilled.
    std::uint8_t const dir{direction};
    // The child in the opposite direction than the rotation will be the parent after the rotation.
    // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
    pointer const focused_child{target_node->GetValidChild(GetOppositeDirection(dir))};
    // Remap the parent of the child to it's grandparent.
    focused_child->SetParent(target_node->GetParent());  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
    if (target_node->IsRoot()) {
      // If the new parent of the child is NULL, the new root of the tree is the child.
      Header::SetRoot(focused_child);
    } else {
      // Remap the parent's child (on the side of the target node) to the child.
      // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
      focused_child->GetValidParent()->SetChild(target_node->GetSameSideIdx(), focused_child);
    }
    // Set the parent of the target node to the (previous) child.
    assert(target_node != nullptr);         // COV_MSR_INV_STATE_ASSERT
    target_node->SetParent(focused_child);  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
    // The inner child of the focused child (now the parent) is remapped to the inner child of the new child.
    target_node->SetChild(GetOppositeDirection(dir),  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
                          focused_child->GetChild(dir));
    if (target_node->GetChild(GetOppositeDirection(dir)) != nullptr) {
      //  If the inner child of the new child is not NULL, it's new parent is the new child.
      target_node
          ->GetValidChild(GetOppositeDirection(dir))  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
          ->SetParent(target_node);
    }
    // Remap the child of the new parent to the new child (old parent).
    focused_child->SetChild(dir, target_node);  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
  }

  /*!
   * \brief         Rebalance tree after insertion.
   * \details       Will at most run tree_height / 2 iterations and maximally 2 rotations could be run.
   * \param[in,out] new_node  The target node to recolor.
   * \vprivate      Component Private
   */
  void InsertRebalance(pointer new_node) noexcept {
    pointer target_node{new_node};
    assert(target_node != nullptr);  // COV_MSR_INV_STATE_ASSERT
    while ((!target_node->IsRoot()) && (target_node->GetValidParent()->GetColor() == kRed)) {
      assert(target_node->GetValidParent() != nullptr);  // COV_MSR_INV_STATE_ASSERT
      if (target_node->GetValidParent()->IsRoot()) {
        // Parent red and root.
        target_node->GetValidParent()->Recolor();  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
        break;
      } else {
        // Parent is not root, i.e grandparent exists.
        assert(target_node->GetValidParent()->GetParent() != nullptr);  // COV_MSR_INV_STATE_ASSERT
        // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
        pointer const grandparent{target_node->GetValidParent()->GetValidParent()};
        pointer const uncle{grandparent->GetChild(target_node->GetValidParent()->GetOppositeSideIdx())};

        if (GetColor(uncle) == kBlack) {
          // Uncle is black (and parent red).
          this->HandleParentRedUncleBlack(target_node);
          break;
        }
        // Uncle is red, i.e it is not null (since then it would be black).
        assert(uncle != nullptr);  // COV_MSR_INV_STATE_ASSERT
        // Parent red and uncle is red.
        target_node->GetValidParent()->Recolor();  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
        target_node->GetValidParent()->GetValidParent()->Recolor();
        uncle->Recolor();
        // new target node is the grandparent.
        target_node = target_node->GetValidParent()->GetValidParent();
      }
    }
  }

  /*!
   * \brief         Function to handle the special insert-case in InsertRebalance where the parent of \p target_node is
   *                red and the uncle is black.
   * \param[in,out] target_node  The node to be inserted.
   * \vprivate      Component Private
   */
  void HandleParentRedUncleBlack(pointer target_node) noexcept {
    // Grandparent is valid, otherwise this branch is unreachable.
    assert(target_node->GetValidParent()->GetParent() != nullptr);  // COV_MSR_INV_STATE_ASSERT
    if (((target_node->IsLeftChild()) && (!target_node->GetValidParent()->IsLeftChild())) ||
        ((!target_node->IsLeftChild()) && (target_node->GetValidParent()->IsLeftChild()))) {
      // target_node is inner child, i.e the direction of the target_node and the parent is different.
      this->Rotate(target_node->GetValidParent(), target_node->GetValidParent()->GetSameSideIdx());
      // Set target to the previous parent.
      target_node = target_node->GetValidChild(target_node->GetSameSideIdx());
    }
    target_node->GetValidParent()->SetColor(kBlack);  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
    // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
    target_node->GetValidParent()->GetValidParent()->SetColor(kRed);
    this->Rotate(target_node->GetValidParent()->GetValidParent(), target_node->GetValidParent()->GetOppositeSideIdx());
  }

  /*!
   * \brief         Rebalances the tree after a deletion.
   * \param[in,out] n  The node that has been deleted.
   * \vprivate      Component Private
   */
  void DeleteRebalance(pointer n) noexcept {
    assert(n->GetParent() != nullptr);  // COV_MSR_INV_STATE_ASSERT
    // Declare relationships for n.
    pointer parent{n->GetValidParent()};  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
    pointer sibling{nullptr};
    pointer c_nephew{nullptr};
    pointer d_nephew{nullptr};

    while (!n->IsRoot()) {
      // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
      assert(n->GetValidParent() != nullptr);  // COV_MSR_INV_STATE_ASSERT
      // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
      sibling = n->GetValidParent()->GetValidChild(n->GetOppositeSideIdx());
      d_nephew = sibling->GetChild(n->GetOppositeSideIdx());
      c_nephew = sibling->GetChild(n->GetSameSideIdx());
      parent = n->GetValidParent();  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD

      if (GetColor(sibling) == kRed) {
        // If the sibling is red, both the nephews and the grandparent must be black.
        this->HandleSiblingRed(n);
        break;
        // sibling is black.
      } else if (GetColor(d_nephew) == kRed) {
        this->HandleDistantNephewRed(n);
        break;
        // d_nephew is black.
      } else if (GetColor(c_nephew) == kRed) {
        this->HandleCloseNephewRed(n);
        break;
        // c_nephew is black.
      } else if (GetColor(parent) == kRed) {
        this->HandleParentRed(sibling, parent);
        break;
      } else {
        sibling->SetColor(kRed);
        n = n->GetValidParent();  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
      }
    }
  }

  /*!
   * \brief         Function to handle the special delete-case in DeleteRebalance where the sibling of \p n is red.
   * \param[in,out] n  The node to be deleted.
   * \vprivate      Component Private
   */
  void HandleSiblingRed(pointer n) noexcept {
    // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
    pointer sibling{n->GetValidParent()->GetValidChild(n->GetOppositeSideIdx())};
    pointer d_nephew{sibling->GetChild(n->GetOppositeSideIdx())};
    pointer c_nephew{sibling->GetChild(n->GetSameSideIdx())};
    pointer const parent{n->GetValidParent()};  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD

    // Rotate the parent is the same direction as n.
    this->Rotate(parent, n->GetSameSideIdx());
    parent->SetColor(kRed);
    sibling->SetColor(kBlack);
    // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
    sibling = n->GetValidParent()->GetValidChild(n->GetOppositeSideIdx());
    d_nephew = sibling->GetChild(n->GetOppositeSideIdx());
    c_nephew = sibling->GetChild(n->GetSameSideIdx());

    if (GetColor(d_nephew) == kRed) {
      this->HandleDistantNephewRed(n);
    } else if (GetColor(c_nephew) == kRed) {
      this->HandleCloseNephewRed(n);
    } else {
      this->HandleParentRed(sibling, parent);
    }
  }

  /*!
   * \brief         Function to handle the special delete-case in DeleteRebalance where the parent of \p sibling is
   *                red.
   * \details       Colors the sibling red and the parent black.
   * \param[in,out] sibling  The sibling of the target node.
   * \param[in,out] parent   The parent of the target node.
   * \vprivate      Component Private
   */
  static void HandleParentRed(pointer sibling, pointer parent) noexcept {
    assert(sibling != nullptr);  // COV_MSR_INV_STATE_ASSERT
    assert(parent != nullptr);   // COV_MSR_INV_STATE_ASSERT
    sibling->SetColor(kRed);
    parent->SetColor(kBlack);
  }

  /*!
   * \brief         Function to handle the special delete-case in DeleteRebalance where the close nephew of \p n is
   *                red.
   * \param[in,out] n  The node that should be deleted.
   * \vprivate      Component Private
   */
  void HandleCloseNephewRed(pointer n) noexcept {
    // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
    pointer const sibling{n->GetValidParent()->GetValidChild(n->GetOppositeSideIdx())};
    pointer const c_nephew{sibling->GetChild(n->GetSameSideIdx())};
    assert(sibling != nullptr);   // COV_MSR_INV_STATE_ASSERT
    assert(c_nephew != nullptr);  // COV_MSR_INV_STATE_ASSERT
    this->Rotate(sibling, n->GetOppositeSideIdx());
    assert(sibling != nullptr);   // COV_MSR_INV_STATE_ASSERT
    assert(c_nephew != nullptr);  // COV_MSR_INV_STATE_ASSERT
    sibling->SetColor(kRed);
    c_nephew->SetColor(kBlack);
    this->HandleDistantNephewRed(n);
  }

  /*!
   * \brief         Function to handle the special delete-case in DeleteRebalance where the distant nephew of \p n is
   *                red.
   * \param[in,out] n  The node that should be deleted.
   * \vprivate      Component Private
   */
  void HandleDistantNephewRed(pointer n) noexcept {
    // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
    pointer const sibling{n->GetValidParent()->GetValidChild(n->GetOppositeSideIdx())};
    pointer const d_nephew{sibling->GetChild(n->GetOppositeSideIdx())};
    // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
    pointer const parent{n->GetValidParent()};
    assert(sibling != nullptr);   // COV_MSR_INV_STATE_ASSERT
    assert(d_nephew != nullptr);  // COV_MSR_INV_STATE_ASSERT
    assert(parent != nullptr);    // COV_MSR_INV_STATE_ASSERT
    this->Rotate(parent, n->GetSameSideIdx());
    sibling->SetColor(GetColor(parent));
    parent->SetColor(kBlack);
    d_nephew->SetColor(kBlack);
  }

  /*!
   * \brief         Add a new node to the tree.
   * \param[in,out] new_node  The node to add.
   * \param[in]     parent    The parent to which to add \p new_node.
   * \param[in]     dir       The direction below \p parent that \p new_node should be added.
   * \vprivate      Component Private
   */
  void AddNewNode(pointer new_node, pointer parent, std::uint8_t dir) noexcept {
    new_node->SetParent(parent);                     // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
    new_node->GetParent()->SetChild(dir, new_node);  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
    if (parent == Header::GetHeader()->GetChild(dir)) {
      Header::GetHeader()->SetChild(dir, new_node);
    }
  }

  /*!
   * \brief     Updates the leftmost and rightmost if it compares equal to \p n.
   * \param[in] n      The node to potentially set to left/rightmost.
   * \param[in] prev   A pointer to a node which when compared equal to left/rightmost will allow \p n to be the new
   *                   left/rightmost node.
   * \param[in] first  Pointer to the current leftmost node.
   * \param[in] last   Pointer to the current rightmost node.
   * \vprivate  Component Private
   */
  void ReplaceExtremeValue(pointer n, const_pointer prev, const_pointer first, const_pointer last) noexcept {
    if (first == prev) {
      SetLeftmost(n);
    } else if (last == prev) {
      SetRightmost(n);
    } else {
      // Do nothing
    }
  }

  /*!
   * \brief     Updates the leftmost and rightmost if it compares equal to \p n.
   * \param[in] n     The node to potentially set to left/rightmost.
   * \param[in] prev  A pointer to a node which when compared equal to left/rightmost will allow \p n to be the new
   *                  left/rightmost node.
   * \vprivate  Component Private
   */
  void ReplaceExtremeValue(pointer n, const_pointer prev) noexcept {
    ReplaceExtremeValue(n, prev, Begin().GetNode(), std::prev(End()).GetNode());
  }

  /*!
   * \brief         Destroys and deallocates an RBData object.
   * \param[in,out] data_ptr  Pointer to the RBData object.
   * \vprivate      Component Private
   */
  void DestroyData(typename NodeType::DataStructType* data_ptr) const noexcept {
    data_allocator_type data_alloc{GetDataAllocator()};
    constexpr std::size_t nr_of_elements{1};
    data_allocator_traits::destroy(data_alloc, data_ptr);                     // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    data_allocator_traits::deallocate(data_alloc, data_ptr, nr_of_elements);  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
  }

  /*!
   * \brief         Destroys and deallocates an RBNode object.
   * \param[in,out] node  Pointer to the object to destroy.
   * \pre           node must not be the header, deleting the header is undefined behavior.
   * \vprivate      Component Private
   */
  void DestroyNode(pointer node) noexcept {
    constexpr std::size_t nr_of_elements{1};
    typename NodeType::DataStructType* const data_ptr{node->GetDataPointer()};
    if (data_ptr != nullptr) {
      DestroyData(data_ptr);
    }

    allocator_traits::destroy(allocator_, node);                     // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    allocator_traits::deallocate(allocator_, node, nr_of_elements);  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
  }

  /*!
   * \brief     Help function to get the color of a node.
   * \details   Each node contains it's own color, but a NULL node shall have the color black.
   * \tparam    Key    First type stored in a std::pair.
   * \tparam    Value  Second type stored in a std::pair.
   * \param[in] n  The node to get the color from.
   * \return    The color of the node \p n.
   */
  static bool GetColor(RBNode<Key, Value> const* n) noexcept { return (n != nullptr) ? n->GetColor() : kBlack; }

  /*!
   * \brief The allocator used to allocate memory for the nodes.
   */
  allocator_type allocator_;
  /*!
   * \brief   The allocator used to allocate memory for RBData pointed to by the nodes.
   * \details The state of this allocator_ is assumed to follow the state of allocator_. No comparisons are performed
   *          on this object.
   */
  data_allocator_type data_allocator_;
  /*!
   * \brief The function to use when sorting the red black tree.
   */
  Compare compare_;
};
}  // namespace internal
}  // namespace container
}  // namespace vac
#endif  // LIB_VAC_INCLUDE_VAC_CONTAINER_RED_BLACK_TREE_H_
