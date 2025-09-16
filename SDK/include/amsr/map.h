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
/*!      \file  amsr/map.h
 *      \brief  Core type amsr::Map
 *       \unit  VaCommonLib::ContainerLibrary::DynamicContainers::Map
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_AMSR_MAP_H_
#define LIB_VAC_INCLUDE_AMSR_MAP_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include <type_traits>

#include "amsr/container_error_domain.h"
#include "amsr/core/result.h"
#include "ara/core/memory_resource.h"
#include "vac/container/internal/red_black_tree.h"

namespace amsr {

/*!
 * \brief  Forward declare base test suite.
 * \tparam Key    The type of the key.
 * \tparam Value  The type of the mapped value.
 */
template <typename Key, typename Value>
class UT__Map__Amsr__Base;

// VECTOR Next Construct Metric-OO.WMC.One: MD_VAC_Metric-OO.WMC.One
/*!
 * \brief   A sorted container, storing a key and a mapped value.
 * \details Uses a red black tree to store pairs of (key, value).
 * \tparam  Key        The type of the key.
 *                     - Key must fulfill the C++ Named Requirement Erasable (ISO/IEC 14882:2014(E): 23.2.1 General
 *                       container requirements, Table 96 and Subclause 14), i.e. it may be one of the following types:
 *                       - class with accessible destructor
 *                       - scalar type (including pointer types)
 *                     - Allocator adds limitations for Key:
 *                       - Key must be a non-const object type (ISO/IEC 14882:2014(E): 17.6.3.5 Allocator requirements,
 *                         Table 27)
 *                       - to be an object type, Key must not be any of the following types (ISO/IEC 14882:2014(E): 3.9
 *                         Types, Subclause 8):
 *                         - array type
 *                         - function type
 *                         - reference type
 *                         - void
 *                     - many member functions impose stricter requirements on Key.
 * \tparam  T          The type of the mapped value.
 *                       - T must fulfill the C++ Named Requirement Erasable (ISO/IEC 14882:2014(E): 23.2.1 General
 *                         container requirements, Table 96 and Subclause 14), i.e. it may be one of the following
 *                         types:
 *                         - class with accessible destructor
 *                         - scalar type (including pointer types)
 *                     - Allocator adds limitations for T:
 *                         - T must be a non-const object type (ISO/IEC 14882:2014(E): 17.6.3.5 Allocator requirements,
 *                           Table 27)
 *                         - to be an object type, T must not be any of the following types (ISO/IEC 14882:2014(E): 3.9
 *                           Types, Subclause 8):
 *                           - array type
 *                           - function type
 *                           - reference type
 *                           - void
 *                     - many member functions impose stricter requirements on T.
 * \tparam  Compare    Function used to compare Key:s.
 * \tparam  Allocator  Type of allocator. Defaulted to ara::core::PolymorphicAllocator<std::pair<Key const, T>.
 * \trace   CREQ-VaCommonLib-Map
 */
template <typename Key, typename T, typename Compare = std::less<Key>,
          typename Allocator = ara::core::PolymorphicAllocator<std::pair<Key const, T>>>
class Map final {
  /*!
   * \brief declaration for testing purposes
   */
  // VECTOR NC AutosarC++17_10-A11.3.1: MD_VAC_A11.3.1_doNotUseFriend
  template <typename, typename>
  friend class UT__Map__Amsr__Base;

 public:
  /*!
   * \brief Forward declare value compare
   */
  class value_compare;
  /*!
   * \brief Type of the map's Key.
   */
  using key_type = Key;
  /*!
   * \brief Type that key_type maps to.
   */
  using mapped_type = T;
  /*!
   * \brief Pair consisting of key_type and mapped_type.
   */
  using value_type = std::pair<key_type const, mapped_type>;
  /*!
   * \brief Function used to compare key_type:s.
   */
  using key_compare = Compare;
  /*!
   * \brief Allocator type.
   */
  using allocator_type = Allocator;
  /*!
   * \brief Allocator traits type.
   */
  using allocator_traits_type = std::allocator_traits<allocator_type>;
  /*!
   * \brief Allocator type for RBNode.
   */
  using node_allocator_type =
      typename allocator_traits_type::template rebind_alloc<vac::container::internal::RBNode<Key const, T>>;

 private:
  /*!
   * \brief Type type of tree used by amsr::Map.
   */
  using tree_type = vac::container::internal::RBTree<key_type const, mapped_type, value_compare, node_allocator_type>;

 public:
  /*!
   * \brief Unsigned integer type to describe size.
   */
  using size_type = std::size_t;
  /*!
   * \brief Type used to describe difference between iterators.
   */
  using difference_type = std::ptrdiff_t;
  /*!
   * \brief Reference type to value_type.
   */
  using reference = value_type&;
  /*!
   * \brief Const reference type to value_type.
   */
  using const_reference = value_type const&;
  /*!
   * \brief Pointer type.
   */
  using pointer = typename allocator_traits_type::pointer;
  /*!
   * \brief Const pointer type.
   */
  using const_pointer = typename allocator_traits_type::const_pointer;
  /*!
   * \brief Iterator type.
   */
  using iterator = typename tree_type::iterator;
  /*!
   * \brief Iterator type.
   */
  using const_iterator = typename tree_type::const_iterator;
  /*!
   * \brief Iterator type.
   */
  using reverse_iterator = typename tree_type::reverse_iterator;
  /*!
   * \brief Iterator type.
   */
  using const_reverse_iterator = typename tree_type::const_reverse_iterator;

  // Node handle is currently not implemented. Might be added in HUN-4280 if investigation proves that it is necessary.

  /*!
   * \brief Function object that compares objects of type amsr::Map::value_type by comparing the first values of two
   *        pairs.
   */
  class value_compare {
   public:
    /*!
     * \brief     Constructor taking and storing a comparator function object of type Compare.
     * \param[in] c  Comparator function object of type Compare.
     */
    explicit value_compare(key_compare c) : comp_{c} {}

    /*!
     * \brief Default constructor with a default constructed comparator.
     */
    value_compare() : value_compare(key_compare{}) {}

    /*!
     * \brief   Type to determine if a given template parameter is a pair.
     * \details Any type not matching the std::pair overload, the type is false.
     */
    template <typename>
    struct is_pair : std::false_type {};

    /*!
     * \brief   Type to determine if a given template parameter is a pair.
     * \details For any std::pair type, the type is true.
     * \tparam  U  Type of the first element in the pair.
     * \tparam  V  Type of the second element in the pair.
     */
    template <typename U, typename V>
    struct is_pair<std::pair<U, V>> : std::true_type {};

    /*!
     * \brief     Compares lhs.first and the given key by calling the stored comparator.
     * \details   Only participates in resolution overload if U and V is not pair types.
     * \tparam    U  Type of the lhs value.
     * \tparam    V  Type of the rhs value.
     * \param[in] lhs  Left hand side value in the compare operation.
     * \param[in] rhs  Right hand side value in the compare operation.
     * \return    True if the compare operator yields true, false otherwise.
     * \pre       The compare operator must have a functor overload that takes objects of type U and V.
     */
    template <typename U, typename V, std::enable_if_t<(!is_pair<U>::value) && (!is_pair<V>::value), bool> = true>
    auto operator()(U const& lhs, V const& rhs) const noexcept -> bool {
      return comp_(lhs, rhs);
    }

    /*!
     * \brief     Compares lhs.first and the given key by calling the stored comparator.
     * \param[in] lhs  Left hand side pair in the compare operation.
     * \param[in] rhs  Right hand side pair in the compare operation.
     * \return    True if the compare operator yields true, false otherwise.
     * \pre       The compare operator must have a functor overload that takes objects of type key_type.
     */
    auto operator()(value_type const& lhs, value_type const& rhs) const noexcept -> bool {
      return comp_(lhs.first, rhs.first);
    }

    /*!
     * \brief  Get the key comparator used by value_compare.
     * \return A reference to the comparator.
     */
    Compare const& GetKeyComp() const { return comp_; }

   private:
    /*!
     * \brief Comparator used to compare.
     */
    Compare comp_;
  };
  /*!
   * \brief Template alias for void.
   */
  template <typename...>
  using void_t = void;

  /*!
   * \brief  Struct that determines if a type is transparent.
   * \tparam C  Type of the comparator.
   * \tparam K  Type of the key.
   */
  template <typename C, typename K, typename = void_t<>>
  struct IsTransparent {};

  /*!
   * \brief   Struct that determines if a type is transparent.
   * \details Types with is_transparent makes IsTransparent have type as member.
   * \tparam  C  Type of the comparator.
   * \tparam  K  Type of the key.
   */
  template <typename C, typename K>
  struct IsTransparent<C, K, void_t<typename C::is_transparent>> {
    /*!
     * \brief The member that exists if the type is transparent.
     */
    using type = void;
  };

  /*!
   * \brief  Alias to simplify usage of IsTransparent.
   * \tparam C  Type of the comparator.
   * \tparam K  Type of the key.
   */
  template <typename C, typename K>
  using IsTransparentType = typename IsTransparent<C, K>::type;

  /*!
   * \brief   Constructs an empty map.
   * \pre     -
   * \context ANY
   * \steady  TRUE
   */
  Map() noexcept : Map(key_compare()) {}

  // VECTOR NC AutosarC++17_10-A12.1.5: MD_VAC_A12.1.5_useDelegatingConstructor
  /*!
   * \brief     Constructs a map with \p comp as key comparator and \p alloc as allocator.
   * \param[in] comp   The comparator to use for comparing keys.
   * \param[in] alloc  The allocator to use for allocations.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  explicit Map(key_compare const& comp, allocator_type const& alloc = allocator_type{}) noexcept
      : tree_(alloc, value_compare(comp)) {}

  /*!
   * \brief     Constructs a map with \p alloc as allocator.
   * \param[in] alloc  The allocator to use for allocations.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  explicit Map(allocator_type const& alloc) noexcept : Map(key_compare{}, alloc) {}

  /*!
   * \brief     Constructs a map by a given tree.
   * \param[in] tree  The underlying tree to use in the map.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  explicit Map(tree_type&& tree) noexcept : tree_(std::move(tree)) {}

  // VECTOR NC AutosarC++17_10-A12.8.4: MD_VAC_A12.8.4_moveConstructorShallNotUseCopySemantics
  /*!
   * \brief     Constructs a map moved from \p other.
   * \param[in] other  The map to move from.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  Map(Map&& other) noexcept : tree_(std::move(other.tree_)) {}

  // Deleted move constructor with allocator, replaced with CreateMap(Map&&, allocator_type).
  Map(Map&& other, allocator_type const& alloc) noexcept = delete;

  /*!
   * \brief   Destructor that the destroys the underlying RBTree.
   * \pre     -
   * \context ANY
   * \steady  TRUE
   */
  ~Map() { this->clear(); }

  // Deleted range based constructor. Replaced with CreateMap(InputIt, InputIt, key_compare,
  // allocator_type).
  template <typename InputIt>
  Map(InputIt first, InputIt last, key_compare const& comp = key_compare{},
      allocator_type const& alloc = allocator_type{}) noexcept = delete;

  // Deleted range based constructor. Replaced with CreateMap(InputIt, InputIt, allocator_type).
  template <typename InputIt>
  Map(InputIt first, InputIt last, allocator_type const& alloc) noexcept = delete;

  // Deleted copy constructor. Replaced with CreateMap(Map const&, allocator_type).
  Map(Map const& other, allocator_type const& alloc) = delete;

  // Deleted copy constructor. Replaced with CreateMap(Map const&).
  Map(Map const& other) = delete;

  // Deleted constructor. Replaced with CreateMap(std::initializer_list<value_type>, key_compare, allocator_type).
  explicit Map(std::initializer_list<value_type> init, key_compare const& comp = key_compare{},
               allocator_type const& alloc = allocator_type{}) noexcept = delete;

  // Deleted constructor. Replaced with CreateMap(std::initializer_list<value_type>, allocator_type).
  Map(std::initializer_list<value_type> init, allocator_type const& alloc) noexcept = delete;

  // Deleted copy assignment operator. Replaced with Assign(Map const&).
  Map& operator=(Map const& other) = delete;

  // Deleted move assignment operator. Replaced with Assign(Map&&).
  Map& operator=(Map&& other) = delete;

  // Deleted assignment operator from initializer list. Replaced with Assign(std::initializer_list<value_type>).
  Map& operator=(std::initializer_list<value_type> ilist) = delete;

  /*!
   * \brief     Constructs a map with content copied from \p other using \p alloc as allocator.
   * \details   Types Key and Value must meet C++ Named Requirement: CopyConstructible.
   * \param[in] other  The map to copy from.
   * \param[in] alloc  The allocator to use for allocations.
   * \return    A result with the copied map, or an error.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  static auto CreateMap(Map const& other, allocator_type const& alloc) noexcept -> ara::core::Result<Map> {
    return Map::MapFromTree(std::move(tree_type::CreateTree(other.tree_, alloc)));
  }

  /*!
   * \brief     Constructs a map with content copied from \p other.
   * \details   Types Key and Value must meet C++ Named Requirement: CopyConstructible.
   * \param[in] other  The map to copy from.
   * \return    A result with the copied map, or an error.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  static auto CreateMap(Map const& other) noexcept -> ara::core::Result<Map> {
    return CreateMap(other, allocator_traits_type::select_on_container_copy_construction(other.get_allocator()));
  }

  /*!
   * \brief     Constructs a map with content moved from \p other with \p alloc as allocator.
   * \param[in] other  The map to move from.
   * \param[in] alloc  The allocator to use for allocations.
   * \return    A result with a map, or an error.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  static auto CreateMap(Map&& other, allocator_type const& alloc) noexcept -> ara::core::Result<Map> {
    return MapFromTree(tree_type::CreateTree(std::move(other.tree_), alloc));
  }

  /*!
   * \brief     Constructs a map with content from \p init.
   * \details   Types Key and Value must meet C++ Named Requirement: CopyConstructible.
   * \param[in] init   The initializer list.
   * \param[in] comp   The comparator to use to compare keys.
   * \param[in] alloc  The allocator to use for allocations.
   * \return    A result with the created map, or an error.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  static auto CreateMap(std::initializer_list<value_type> init, key_compare const& comp = key_compare{},
                        allocator_type const& alloc = allocator_type{}) noexcept -> ara::core::Result<Map> {
    return Map::MapFromTree(std::move(tree_type::CreateTree(init, alloc, value_compare(comp))));
  };

  /*!
   * \brief     Constructs a map with content from \p init with \p alloc as allocator.
   * \details   Types Key and Value must meet C++ Named Requirement: CopyConstructible.
   * \param[in] init   The initializer list.
   * \param[in] alloc  The allocator to use for allocations.
   * \return    A result with the created map, or an error.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  static auto CreateMap(std::initializer_list<value_type> init, allocator_type const& alloc) noexcept
      -> ara::core::Result<Map> {
    return CreateMap(init, key_compare{}, alloc);
  }

  /*!
   * \brief     Constructs a map with content from the range [ \p first, \p last ).
   * \details   Types Key and Value must meet C++ Named Requirement: CopyConstructible.
   * \tparam    InputIt  The type of the iterator parameters.
   * \param[in] first  Iterator pointing to the first element in the range.
   * \param[in] last   Iterator pointing to the last element in the range.
   * \param[in] comp   The comparator to compare keys.
   * \param[in] alloc  The allocator to use for allocations.
   * \return    A result with the created map, or an error.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  template <typename InputIt,
            std::enable_if_t<std::is_convertible<typename std::iterator_traits<InputIt>::iterator_category,
                                                 std::input_iterator_tag>::value,
                             bool> = true>
  static auto CreateMap(InputIt first, InputIt last, Compare const& comp = Compare(),
                        allocator_type const& alloc = allocator_type()) noexcept -> ara::core::Result<Map> {
    return Map::MapFromTree(std::move(tree_type::CreateTree(first, last, alloc, value_compare(comp))));
  }

  /*!
   * \brief     Constructs a map with content from the range [ \p first, \p last ).
   * \details   Types Key and Value must meet C++ Named Requirement: CopyConstructible.
   * \tparam    InputIt  The type of the iterator parameters.
   * \param[in] first  Iterator pointing to the first element in the range.
   * \param[in] last   Iterator pointing to the last element in the range.
   * \param[in] alloc  The allocator to use for allocations.
   * \return    A result with the created map, or an error.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  template <typename InputIt,
            std::enable_if_t<std::is_convertible<typename std::iterator_traits<InputIt>::iterator_category,
                                                 std::input_iterator_tag>::value,
                             bool> = true>
  static auto CreateMap(InputIt first, InputIt last, allocator_type const& alloc) noexcept -> ara::core::Result<Map> {
    return CreateMap(first, last, Compare{}, alloc);
  }

  /*!
   * \brief     Assigns the content from \p ilist to this.
   * \details   Types Key and Value must meet C++ Named Requirement: CopyConstructible.
   * \param[in] ilist  The initializer list.
   * \return    An empty result, or an error.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  auto Assign(std::initializer_list<value_type> ilist) noexcept -> ara::core::Result<void> {
    return tree_.Assign(ilist);
  }

  /*!
   * \brief     Copy assigns each element in \p other to this.
   * \details   Types Key and Value must meet C++ Named Requirement: CopyConstructible.
   * \param[in] other  The map to copy from.
   * \return    An empty result, or an error.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  auto Assign(Map const& other) noexcept -> ara::core::Result<void> { return tree_.Assign(other.tree_); }

  /*!
   * \brief     Move assign \p other to this.
   * \details   If the allocator of this is always_equal or propagates_on_move_assignment, this can safely take
   *            ownership of memory allocated by \p other. If not, and the allocators does not compare equal, each
   *            element will be move assigned and additional memory will be allocated if necessary. If each element
   *            must be moved assigned -> inflicts the following requirements on Key and Value: - Must fulfill C++
   *            named requirement CopyConstructible.
   * \param[in] other  The map to move from.
   * \return    An empty result, or an error.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  auto Assign(Map&& other) noexcept -> ara::core::Result<void> { return tree_.Assign(std::move(other.tree_)); }

  /*!
   * \brief   Get a copy of the allocator.
   * \return  A copy of the allocator.
   * \pre     -
   * \context ANY
   * \steady  TRUE
   */
  auto get_allocator() const noexcept -> allocator_type { return tree_.GetAllocator(); }

  /*!
   * \brief     Access value mapped to key \p key.
   * \param[in] key  The key to get the mapped value from.
   * \return    A result containing a reference to a value, or an error.
   * \error     ContainerErrorCode::kOutOfRangeError if the key does not exists.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  auto At(Key const& key) const noexcept -> ara::core::Result<T const&> {
    ara::core::Result<T const&> out{amsr::ContainerErrorCode::kOutOfRangeError};
    const_iterator const kIt{tree_.Search(key)};
    if (kIt != tree_.CEnd()) {
      out.EmplaceValue((*kIt).second);
    }
    return out;
  }

  /*!
   * \brief     Access value mapped to key \p key.
   * \param[in] key  The key to get the mapped value from.
   * \return    A result containing a reference to a value, or an error.
   * \error     ContainerErrorCode::kOutOfRangeError if the key does not exists.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  auto At(Key const& key) noexcept -> ara::core::Result<T&> {
    ara::core::Result<T&> out{amsr::ContainerErrorCode::kOutOfRangeError};
    iterator const kIt{tree_.Search(key)};
    if (kIt != tree_.End()) {
      out.EmplaceValue((*kIt.NonConstCast()).second);
    }
    return out;
  }

  // VECTOR Next Construct AutosarC++17_10-A13.5.1: MD_VAC_A13.5.1_noConstVersionOfOperator
  /*!
   * \brief     Access value mapped to key \p key or insert new element with key \p key.
   * \details   Type Value must meet C++ Named Requirement: DefaultConstructible.
   * \param[in] key  The key to search for.
   * \return    A result containing a reference to a value, or an error.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  auto operator[](Key&& key) noexcept -> ara::core::Result<T&> {
    ara::core::Result<T&> out{this->At(std::move(key))};
    if (!out.HasValue()) {
      ara::core::Result<std::pair<iterator, bool>> ins_res{
          this->Emplace(std::piecewise_construct, std::forward_as_tuple(std::move(key)), std::tuple<>())};
      if (ins_res.HasValue()) {
        out.EmplaceValue(GetValue(ins_res.Value().first));
      } else {
        out.EmplaceError(ins_res.Error());
      }
    }
    return out;
  }

  // VECTOR Next Construct AutosarC++17_10-A13.5.1: MD_VAC_A13.5.1_noConstVersionOfOperator
  /*!
   * \brief     Access value mapped to key \p key or insert new element with key \p key.
   * \details   Type Value must meet C++ Named Requirement: DefaultConstructible.
   * \param[in] key  The key to search for.
   * \return    A result containing a reference to a value, or an error.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  auto operator[](Key const& key) noexcept -> ara::core::Result<T&> {
    ara::core::Result<T&> out{this->At(key)};
    if (!out.HasValue()) {
      ara::core::Result<std::pair<iterator, bool>> ins_res{
          this->Emplace(std::piecewise_construct, std::forward_as_tuple(key), std::tuple<>())};
      if (ins_res.HasValue()) {
        out.EmplaceValue(GetValue(ins_res.Value().first));
      } else {
        out.EmplaceError(ins_res.Error());
      }
    }
    return out;
  }

  /*!
   * \brief   Get an iterator to the beginning of the map.
   * \return  An iterator to the first element in the map.
   * \pre     -
   * \context ANY
   * \steady  TRUE
   */
  auto begin() noexcept -> iterator { return tree_.Begin(); }

  /*!
   * \brief   Get an iterator to the beginning of the map.
   * \return  An const iterator to the first element in the map.
   * \pre     -
   * \context ANY
   * \steady  TRUE
   */
  auto begin() const noexcept -> const_iterator { return tree_.Begin(); }

  /*!
   * \brief   Get a const iterator to the beginning of the map.
   * \return  A const iterator to the first element in the map.
   * \pre     -
   * \context ANY
   * \steady  TRUE
   */
  auto cbegin() const noexcept -> const_iterator { return tree_.CBegin(); }

  /*!
   * \brief   Get an iterator to the end of the map.
   * \return  An iterator to the last element in the map.
   * \pre     -
   * \context ANY
   * \steady  TRUE
   */
  auto end() noexcept -> iterator { return tree_.End(); }

  /*!
   * \brief   Get an iterator to the end of the map.
   * \return  A const iterator to the last element in the map.
   * \pre     -
   * \context ANY
   * \steady  TRUE
   */
  auto end() const noexcept -> const_iterator { return tree_.End(); }

  /*!
   * \brief   Get a const iterator to the end of the map.
   * \return  A const iterator to the last element in the map.
   * \pre     -
   * \context ANY
   * \steady  TRUE
   */
  auto cend() const noexcept -> const_iterator { return tree_.CEnd(); }

  /*!
   * \brief   Get an iterator to the reverse beginning of the map.
   * \return  An reverse iterator to the first element in the reversed map.
   * \pre     -
   * \context ANY
   * \steady  TRUE
   */
  auto rbegin() noexcept -> reverse_iterator { return tree_.RBegin(); }

  /*!
   * \brief   Get an iterator to the reverse beginning of the map.
   * \return  A const reverse iterator to the first element in the reversed map.
   * \pre     -
   * \context ANY
   * \steady  TRUE
   */
  auto rbegin() const noexcept -> const_reverse_iterator { return tree_.RBegin(); }

  /*!
   * \brief   Get a const iterator to the reverse beginning of the map.
   * \return  A const reverse iterator to the first element in the reversed map.
   * \pre     -
   * \context ANY
   * \steady  TRUE
   */
  auto crbegin() const noexcept -> const_reverse_iterator { return tree_.CRBegin(); }

  /*!
   * \brief   Get an iterator to the reverse end of the map.
   * \return  A reverse iterator to the last element in the reversed map.
   * \pre     -
   * \context ANY
   * \steady  TRUE
   */
  auto rend() noexcept -> reverse_iterator { return tree_.REnd(); }

  /*!
   * \brief   Get an iterator to the reverse end of the map.
   * \return  A const reverse iterator to the last element in the reversed map.
   * \pre     -
   * \context ANY
   * \steady  TRUE
   */
  auto rend() const noexcept -> const_reverse_iterator { return tree_.REnd(); }

  /*!
   * \brief   Get a const iterator to the reverse end of the map.
   * \return  A const reverse iterator to the last element in the reversed map.
   * \pre     -
   * \context ANY
   * \steady  TRUE
   */
  auto crend() const noexcept -> const_reverse_iterator { return tree_.CREnd(); }

  /*!
   * \brief   Checks if the map has no element.
   * \return  True if the map is empty, i.e begin() == end(), false otherwise.
   * \pre     -
   * \context ANY
   * \steady  TRUE
   */
  auto empty() const noexcept -> bool { return this->cbegin() == this->cend(); }

  /*!
   * \brief   Get the size of the map.
   * \return  The number of elements in the map.
   * \pre     -
   * \context ANY
   * \steady  TRUE
   */
  auto size() const noexcept -> size_type { return tree_.Size(); }

  /*!
   * \brief   Get the theoretical max size of the map.
   * \return  The maximum amount of elements the map can hold.
   * \pre     -
   * \context ANY
   * \steady  TRUE
   */
  auto max_size() const noexcept -> size_type { return tree_.MaxSize(); }

  /*!
   * \brief   Clear the map of all elements.
   * \pre     -
   * \context ANY
   * \steady  TRUE
   */
  void clear() noexcept { tree_.Clear(); }

  /*!
   * \brief     Insert an element in the map.
   * \details   If the key of \p value compares equal to an already existing element, nothing happens. Types Key and
   *            Value must meet C++ Named Requirements: CopyConstructible, CopyInsertable.
   * \param[in] value  The value to insert.
   * \return    A result containing a pair with an iterator to the inserted element (or the element that prevented the
   *            insertion) and a boolean that states if the element was inserted or not. If an error occurred, the
   *            result contains the error.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  auto Insert(value_type const& value) noexcept -> ara::core::Result<std::pair<iterator, bool>> {
    return GenericEmplace<false>(value, cend());
  }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Insert an element in the map.
   * \details   If the key of \p value compares equal to an already existing element, nothing happens. Types Key and
   *            Value must meet C++ Named Requirements: CopyConstructible, CopyInsertable.
   * \tparam    P  Type of the element to insert. value_type must be constructible from P.
   * \param[in] value  The value to insert.
   * \return    A result containing a pair with an iterator to the inserted element (or the element that prevented the
   *            insertion) and a boolean that states if the element was inserted or not. If an error occurred, the
   *            result contains the error.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  template <typename P, std::enable_if_t<std::is_constructible<value_type, P&&>::value, bool> = true>
  auto Insert(P&& value) noexcept -> ara::core::Result<std::pair<iterator, bool>> {
    return Emplace(std::forward<P>(value));
  }

  /*!
   * \brief     Insert an element in the map.
   * \details   If the key of \p value compares equal to an already existing element, nothing happens. Types Key and
   *            Value must meet C++ Named Requirements: CopyConstructible, CopyInsertable.
   * \param[in] value  The value to insert.
   * \return    A result containing a pair with an iterator to the inserted element (or the element that prevented the
   *            insertion) and a boolean that states if the element was inserted or not. If an error occurred, the
   *            result contains the error.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  auto Insert(value_type&& value) noexcept -> ara::core::Result<std::pair<iterator, bool>> {
    return GenericEmplace<false>(std::move(value), cend());
  }

  /*!
   * \brief     Insert an element in the map as close as possible to the position just before \p hint.
   * \details   If the key of \p value compares equal to an already existing element, nothing happens. Types Key and
   *            Value must meet C++ Named Requirements: CopyConstructible, CopyInsertable.
   * \param[in] hint   An iterator to the position before which the new element will be inserted.
   * \param[in] value  The value to insert.
   * \return    A result containing an iterator to the inserted element (or the element that prevented the insertion),
   *            or an error.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  auto Insert(iterator hint, value_type const& value) noexcept -> ara::core::Result<iterator> {
    return PairToIterator(GenericEmplace<true>(value, const_iterator{hint}));
  }

  /*!
   * \brief     Insert an element in the map as close as possible to the position just before \p hint.
   * \details   If the key of \p value compares equal to an already existing element, nothing happens. Types Key and
   *            Value must meet C++ Named Requirements: CopyConstructible, CopyInsertable.
   * \param[in] hint   An const iterator to the position before which the new element will be inserted.
   * \param[in] value  The value to insert.
   * \return    A result containing an iterator to the inserted element (or the element that prevented the insertion),
   *            or an error.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  auto Insert(const_iterator hint, value_type const& value) noexcept -> ara::core::Result<iterator> {
    return PairToIterator(GenericEmplace<true>(value, hint));
  }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Insert an element in the map as close as possible to the position just before \p hint.
   * \details   If the key of \p value compares equal to an already existing element, nothing happens. Types Key and
   *            Value must meet C++ Named Requirements: CopyConstructible, CopyInsertable.
   * \tparam    P  Type of the element to insert. value_type must be constructible from P.
   * \param[in] hint   An const iterator to the position before which the new element will be inserted.
   * \param[in] value  The value to insert.
   * \return    A result containing an iterator to the inserted element (or the element that prevented the insertion),
   *            or an error.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  template <typename P, std::enable_if_t<std::is_constructible<value_type, P&&>::value, bool> = true>
  auto Insert(const_iterator hint, P&& value) noexcept -> ara::core::Result<iterator> {
    return EmplaceHint(hint, std::forward<P>(value));
  }

  /*!
   * \brief     Insert an element in the map as close as possible to the position just before \p hint.
   * \details   If the key of \p value compares equal to an already existing element, nothing happens. Types Key and
   *            Value must meet C++ Named Requirements: CopyConstructible, CopyInsertable.
   * \param[in] hint   An const iterator to the position before which the new element will be inserted.
   * \param[in] value  The value to insert.
   * \return    A result containing an iterator to the inserted element (or the element that prevented the insertion),
   *            or an error.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  auto Insert(const_iterator hint, value_type&& value) noexcept -> ara::core::Result<iterator> {
    return PairToIterator(GenericEmplace<true>(std::move(value), hint));
  }

  /*!
   * \brief     Insert elements from the range [ \p first, \p last ) in the map.
   * \details   If the key of an value in the range compares equal to an existing element, that element is not
   *            inserted. Types Key and Value must meet C++ Named Requirements: CopyConstructible, CopyInsertable.
   * \tparam    InputIt  The type of the iterators. Must fulfill C++ named requirement LegacyIterator
   * \param[in] first  An iterator to the start of the range to insert from.
   * \param[in] last   An iterator to the end of the range to insert from.
   * \return    A result that is empty if the insertion took place, or an error otherwise.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       It shall be possible to construct a value_type from the iterators underlying object.
   * \context   ANY
   * \steady    TRUE
   * \spec      requires true; \endspec
   */
  template <typename InputIt>
  auto Insert(InputIt first, InputIt last) noexcept -> ara::core::Result<void> {
    ara::core::Result<void> out{};
    for (; first != last; first++) {
      // VCA_VAC_VALID_PRECONDITIONED_ITERATOR_DEREFERENCE
      ara::core::Result<std::pair<iterator, bool>> const kRes{Emplace(*first)};
      if (!kRes.HasValue()) {
        out.EmplaceError(kRes.Error());
        break;
      }
    }
    return out;
  }

  /*!
   * \brief     Insert elements from an initializer list in the map.
   * \details   If the key of an value in the list compares equal to an existing element, that element is not inserted.
   *            Types Key and Value must meet C++ Named Requirements: CopyConstructible, CopyInsertable.
   * \param[in] ilist  An initializer list that contains the elements to insert.
   * \return    A result that is empty if the insertion took place, or an error otherwise.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  auto Insert(std::initializer_list<value_type> ilist) noexcept -> ara::core::Result<void> {
    return Insert(ilist.begin(), ilist.end());
  }

  /*!
   * \brief     Insert an element constructed in-place with the arguments \p args.
   * \details   If the element would have the same key as an already existing element, the insertion is prevented. Key
   *            and Value must meet C++ Named Requirement: EmplaceConstructible.
   * \tparam    Args  The types of the arguments to forward to the constructor of the element.
   * \param[in] args  The arguments to forward to the constructor of the element.
   * \return    A result containing a pair with an iterator to the inserted element (or the element that prevented the
   *            insertion) and a boolean that states if the element was inserted or not. If an error occurred, the
   *            result contains the error.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  template <typename... Args>
  auto Emplace(Args&&... args) noexcept -> ara::core::Result<std::pair<iterator, bool>> {
    ara::core::Result<std::pair<iterator, bool>> out{amsr::ContainerErrorCode::kAllocationError};
    iterator const found_it{find(ExtractKey(args...))};  // VCA_VAC_VALID_ARGUMENT
    if (found_it == end()) {
      out = IteratorToPair(
          tree_.EmplaceHint(const_iterator{found_it}, std::forward<Args>(args)...));  // VCA_VAC_VALID_ARGUMENT

    } else {
      out.EmplaceValue(found_it, false);
    }
    return out;
  }

  /*!
   * \brief     Insert an element constructed in-place with the arguments \p args as close to the element just before
   *            \p hint as possible.
   * \details   If the element would have the same key as an already existing element, the insertion is prevented.
   *            Types Key and Value must meet C++ Named Requirement: EmplaceConstructible.
   * \tparam    Args  The types of the arguments to forward to the constructor of the element.
   * \param[in] hint  An const iterator to the position before which the new element will be inserted.
   * \param[in] args  The arguments to forward to the constructor of the element.
   * \return    A result containing an iterator to the inserted element (or the element that prevented the insertion),
   *            or an error.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  template <typename... Args>
  auto EmplaceHint(const_iterator hint, Args&&... args) noexcept -> ara::core::Result<iterator> {
    ara::core::Result<iterator> out{amsr::ContainerErrorCode::kAllocationError};
    iterator found_it{find(ExtractKey(args...))};
    if (found_it == end()) {
      out = tree_.EmplaceHint(hint, std::forward<Args>(args)...);
    } else {
      out.EmplaceValue(found_it);
    }
    return out;
  }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Insert an element piecewise constructed in-place with key \p key and value constructed from \p args.
   * \details   If \p key compares equal to an already existing key, the insertion is prevented. Type Key must meet C++
   *            Named Requirements: MoveConstructible, MoveInsertable. Type Value must meet C++ Named Requirement:
   *            EmplaceConstructible.
   * \tparam    Args  The types of the arguments to forward to the constructor of the value of the new element.
   * \param[in] key   The key of the new element.
   * \param[in] args  The arguments to forward to the constructor of the value of the new element.
   * \return    A result containing a pair with an iterator to the inserted element (or the element that prevented the
   *            insertion) and a boolean that states if the element was inserted or not. If an error occurred, the
   *            result contains the error.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  template <typename... Args>
  auto TryEmplace(key_type const& key, Args&&... args) noexcept -> ara::core::Result<std::pair<iterator, bool>> {
    return Emplace(std::piecewise_construct, std::forward_as_tuple(key),
                   std::forward_as_tuple(std::forward<Args>(args)...));
  }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Insert an element piecewise constructed in-place with key \p key and value constructed from \p args.
   * \details   If \p key compares equal to an already existing key, the insertion is prevented. Type Key must meet C++
   *            Named Requirements: MoveConstructible, MoveInsertable. Type Value must meet C++ Named Requirement:
   *            EmplaceConstructible.
   * \tparam    Args  The types of the arguments to forward to the constructor of the value of the new element.
   * \param[in] key   The key of the new element.
   * \param[in] args  The arguments to forward to the constructor of the value of the new element.
   * \return    A result containing a pair with an iterator to the inserted element (or the element that prevented the
   *            insertion) and a boolean that states if the element was inserted or not. If an error occurred, the
   *            result contains the error.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  template <typename... Args>
  auto TryEmplace(key_type&& key, Args&&... args) noexcept -> ara::core::Result<std::pair<iterator, bool>> {
    return Emplace(std::piecewise_construct, std::forward_as_tuple(std::move(key)),
                   std::forward_as_tuple(std::forward<Args>(args)...));
  }

  /*!
   * \brief     Insert an element piecewise constructed in-place with key \p key and value constructed from \p args as
   *            close as possible to the position just before \p hint.
   * \details   If \p key compares equal to an already existing key, the insertion is prevented. Type Key must meet C++
   *            Named Requirements: MoveConstructible, MoveInsertable. Type Value must meet C++ Named Requirement:
   *            EmplaceConstructible.
   * \tparam    Args  The types of the arguments to forward to the constructor of the value of the new element.
   * \param[in] hint  A const iterator to the position before which the new element will be inserted.
   * \param[in] key   The key of the new element.
   * \param[in] args  The arguments to forward to the constructor of the value of the new element.
   * \return    A result containing an iterator to the inserted element (or the element that prevented the insertion),
   *            or an error.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  template <typename... Args>
  auto TryEmplace(const_iterator hint, key_type const& key, Args&&... args) noexcept -> ara::core::Result<iterator> {
    return EmplaceHint(hint, std::piecewise_construct, std::forward_as_tuple(key),
                       std::forward_as_tuple(std::forward<Args>(args)...));
  }

  /*!
   * \brief     Insert an element piecewise constructed in-place with key \p key and value constructed from \p args as
   *            close as possible to the position just before \p hint.
   * \details   If \p key compares equal to an already existing key, the insertion is prevented. Type Key must meet C++
   *            Named Requirements: MoveConstructible, MoveInsertable. Type Value must meet C++ Named Requirement:
   *            EmplaceConstructible.
   * \tparam    Args  The types of the arguments to forward to the constructor of the value of the new element.
   * \param[in] hint  A const iterator to the position before which the new element will be inserted.
   * \param[in] key   The key of the new element.
   * \param[in] args  The arguments to forward to the constructor of the value of the new element.
   * \return    A result containing an iterator to the inserted element (or the element that prevented the insertion),
   *            or an error.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  template <typename... Args>
  auto TryEmplace(const_iterator hint, key_type&& key, Args&&... args) noexcept -> ara::core::Result<iterator> {
    return EmplaceHint(hint, std::piecewise_construct, std::forward_as_tuple(std::move(key)),
                       std::forward_as_tuple(std::forward<Args>(args)...));
  }

  //
  // ========================== INSERT_OR_ASSIGN ====================================
  //

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Insert an element to the map if the key \p k does not exists, otherwise assigns the value of the
   *            existing element to \p obj.
   * \details   Types Key and Value must meet C++ Named Requirements: CopyConstructible, CopyInsertable.
   * \tparam    M  The type of the value. Must be nothrow copy assignable.
   * \param[in] k    The key used both to look up and to insert if not found.
   * \param[in] obj  The value to insert or assign.
   * \return    A result containing a pair with an iterator to the inserted element (or the element that was updated)
   *            and a boolean that is true if inserting took place, and false if assignment took place.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  template <typename M>
  auto InsertOrAssign(key_type const& k, M&& obj) noexcept -> ara::core::Result<std::pair<iterator, bool>> {
    return GenericInsertOrAssign<false>(cend(), k, std::forward<M>(obj));
  }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Insert an element to the map if the key \p k does not exists, otherwise assigns the value of the
   *            existing element to \p obj.
   * \details   Types Key and Value must meet C++ Named Requirements: CopyConstructible, CopyInsertable.
   * \tparam    M  The type of the value. Must be nothrow copy assignable.
   * \param[in] k    The key used both to look up and to insert if not found.
   * \param[in] obj  The value to insert or assign.
   * \return    A result containing a pair with an iterator to the inserted element (or the element that was updated)
   *            and a boolean that is true if inserting took place, and false if assignment took place.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  template <typename M>
  auto InsertOrAssign(key_type&& k, M&& obj) noexcept -> ara::core::Result<std::pair<iterator, bool>> {
    return GenericInsertOrAssign<false>(cend(), std::move(k), std::forward<M>(obj));
  }

  /*!
   * \brief     Insert an element to the map as close as possible to the position just before \p hint if the key \p k
   *            does not exists, otherwise assigns the value of the existing element to \p obj.
   * \details   Types Key and Value must meet C++ Named Requirements: CopyConstructible, CopyInsertable.
   * \tparam    M  The type of the value. Must be nothrow copy assignable.
   * \param[in] hint  A const iterator to the position before which the new element will be inserted.
   * \param[in] k     The key used both to look up and to insert if not found.
   * \param[in] obj   The value to insert or assign.
   * \return    A result containing an iterator to the inserted element (or the updated element), or an error.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  template <typename M>
  auto InsertOrAssign(const_iterator hint, key_type const& k, M&& obj) noexcept -> ara::core::Result<iterator> {
    return PairToIterator(GenericInsertOrAssign<true>(hint, k, std::forward<M>(obj)));
  }

  /*!
   * \brief     Insert an element to the map as close as possible to the position just before \p hint if the key \p k
   *            does not exists, otherwise assigns the value of the existing element to \p obj.
   * \details   Types Key and Value must meet C++ Named Requirements: CopyConstructible, CopyInsertable.
   * \tparam    M  The type of the value. Must be nothrow copy assignable.
   * \param[in] hint  A const iterator to the position before which the new element will be inserted.
   * \param[in] k     The key used both to look up and to insert if not found.
   * \param[in] obj   The value to insert or assign.
   * \return    A result containing an iterator to the inserted element (or the updated element), or an error.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  template <typename M>
  auto InsertOrAssign(const_iterator hint, key_type&& k, M&& obj) noexcept -> ara::core::Result<iterator> {
    return PairToIterator(GenericInsertOrAssign<true>(hint, std::move(k), std::forward<M>(obj)));
  }

  /*!
   * \brief     Removes the element at position \p pos.
   * \param[in] pos  An iterator to the element to remove.
   * \return    An iterator to the element just after the removed element.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  auto erase(iterator pos) noexcept -> iterator { return tree_.Delete(pos); }

  /*!
   * \brief     Removes the element at position \p pos.
   * \param[in] pos  An const iterator to the element to remove.
   * \return    An iterator to the element just after the removed element.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  auto erase(const_iterator pos) noexcept -> iterator { return tree_.Delete(pos); }

  /*!
   * \brief     Removes the elements in the range [ \p first, \p last).
   * \param[in] first  An iterator to the beginning of the range of elements to remove.
   * \param[in] last   An iterator to the end of the range of elements to remove.
   * \return    An iterator to the element just after the last removed element.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  auto erase(iterator first, iterator last) noexcept -> iterator {
    iterator out{last};
    if ((first == begin()) && (last == end())) {
      clear();
    } else {
      while (first != last) {
        out = erase(first++);
      }
    }
    return out;
  }

  /*!
   * \brief     Removes the elements in the range [ \p first, \p last).
   * \param[in] first  A const iterator to the beginning of the range of elements to remove.
   * \param[in] last   A const iterator to the end of the range of elements to remove.
   * \return    An iterator to the element just after the last removed element.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  auto erase(const_iterator first, const_iterator last) noexcept -> iterator {
    return erase(first.NonConstCast(), last.NonConstCast());
  }

  /*!
   * \brief     Removes the element with key \p key if it exists.
   * \param[in] key  The key to match.
   * \return    The amount of removed elements (0 or 1).
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  auto erase(key_type const& key) noexcept -> size_type {
    size_type out{0};
    iterator const kIt{tree_.Search(key)};
    if (kIt != end()) {
      static_cast<void>(tree_.Delete(kIt));
      out = 1;
    }
    return out;
  }

  /*!
   * \brief         Swap the content with another map.
   * \param[in,out] other  The map to swap content with.
   * \pre           -
   * \context       ANY
   * \steady        TRUE
   */
  void swap(Map& other) noexcept { tree_.Swap(other.tree_); }

  /*!
   * \brief     Get the number of elements with key \p key.
   * \details   For map any key can only appear once, so this function either returns 1 if an element with key \p key
   *            exists, or 0 otherwise.
   * \param[in] key  The key to match.
   * \return    The amount of elements with a key that compares equal to \p key.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  auto count(key_type const& key) const noexcept -> size_type { return tree_.Count(key); }

  // VECTOR Next Construct AutosarC++17_10-A13.3.1: MD_VAC_A13.3.1_forwardingFunctionsShallNotBeOverloaded
  /*!
   * \brief     Get the number of elements with key \p key.
   * \details   For map any key can only appear once, so this function either returns 1 if an element with key \p key
   *            exists, or 0 otherwise.
   * \tparam    K    The type of the key.
   * \tparam    Req  Must be true for this function to participate in resolution overload.
   * \param[in] x  The key to match.
   * \return    The amount of elements with a key that compares equal to \p key.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  template <typename K, typename Req = IsTransparentType<Compare, K>>
  auto count(K&& x) const noexcept -> size_type {
    return tree_.Count(x);
  }

  /*!
   * \brief     Find an element with key \p key.
   * \param[in] key  The key to match.
   * \return    An iterator to the element if found, otherwise end().
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  auto find(key_type const& key) noexcept -> iterator { return tree_.Search(key); }  // VCA_VAC_VALID_ARGUMENT

  /*!
   * \brief     Find an element with key \p key.
   * \param[in] key  The key to match.
   * \return    A const iterator to the element if found, otherwise end().
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  auto find(key_type const& key) const noexcept -> const_iterator { return tree_.Search(key); }

  /*!
   * \brief     Find an element with key \p key.
   * \tparam    K    The type of the key.
   * \tparam    Req  Must be true for this function to participate in resolution overload.
   * \param[in] x  The key to match.
   * \return    An iterator to the element if found, otherwise end().
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  template <typename K, typename Req = IsTransparentType<Compare, K>>
  auto find(K const& x) noexcept -> iterator {
    return tree_.Search(x);
  }

  /*!
   * \brief     Find an element with key \p key.
   * \tparam    K    The type of the key.
   * \tparam    Req  Must be true for this function to participate in resolution overload.
   * \param[in] x  The key to match.
   * \return    A const iterator to the element if found, otherwise end().
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  template <typename K, typename Req = IsTransparentType<Compare, K>>
  auto find(K const& x) const noexcept -> const_iterator {
    return tree_.Search(x);
  }

  /*!
   * \brief     Get a range of all elements with a key that compares equal to \p key.
   * \param[in] key  The key to match.
   * \return    A pair with an iterator to the beginning of the range with the first element not less than \p key, and
   *            an iterator to the end of the range with the first element that is greater than \p key. If the key is
   *            not found, the two iterators will point to the same object.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  auto equal_range(key_type const& key) noexcept -> std::pair<iterator, iterator> { return tree_.EqualRange(key); }

  /*!
   * \brief     Get a range of all elements with a key that compares equal to \p key.
   * \param[in] key  The key to match.
   * \return    A pair with a const iterator to the beginning of the range with the first element not less than \p key,
   *            and a const iterator to the end of the range with the first element that is greater than \p key.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  auto equal_range(key_type const& key) const noexcept -> std::pair<const_iterator, const_iterator> {
    return tree_.EqualRange(key);
  }

  /*!
   * \brief     Get a range of all elements with a key that compares equal to \p key.
   * \tparam    K    The type of the key.
   * \tparam    Req  Must be true for this function to participate in resolution overload.
   * \param[in] x  The key to match.
   * \return    A pair with an iterator to the beginning of the range with the first element not less than \p key, and
   *            an iterator to the end of the range with the first element that is greater than \p key.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  template <typename K, typename Req = IsTransparentType<Compare, K>>
  auto equal_range(K const& x) noexcept -> std::pair<iterator, iterator> {
    return tree_.EqualRange(x);
  }

  /*!
   * \brief     Get a range of all elements with a key that compares equal to \p key.
   * \tparam    K    The type of the key.
   * \tparam    Req  Must be true for this function to participate in resolution overload.
   * \param[in] x  The key to match.
   * \return    A pair with a const iterator to the beginning of the range with the first element not less than \p key,
   *            and a const iterator to the end of the range with the first element that is greater than \p key.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  template <typename K, typename Req = IsTransparentType<Compare, K>>
  auto equal_range(K const& x) const noexcept -> std::pair<const_iterator, const_iterator> {
    return tree_.EqualRange(x);
  }

  /*!
   * \brief     Get the first element not less than \p key.
   * \param[in] key  The key to match.
   * \return    An iterator to the first element not less than \p key.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  auto lower_bound(key_type const& key) noexcept -> iterator { return tree_.LowerBound(key); }

  /*!
   * \brief     Get the first element not less than \p key.
   * \param[in] key  The key to match.
   * \return    A const iterator to the first element not less than \p key.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  auto lower_bound(key_type const& key) const noexcept -> const_iterator { return tree_.LowerBound(key); }

  /*!
   * \brief     Get the first element not less than \p key.
   * \tparam    K    The type of the key.
   * \tparam    Req  Must be true for this function to participate in resolution overload.
   * \param[in] x  The key to match.
   * \return    An iterator to the first element not less than \p key.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  template <typename K, typename Req = IsTransparentType<Compare, K>>
  auto lower_bound(K const& x) noexcept -> iterator {
    return tree_.LowerBound(x);
  }

  /*!
   * \brief     Get the first element not less than \p key.
   * \tparam    K    The type of the key.
   * \tparam    Req  Must be true for this function to participate in resolution overload.
   * \param[in] x  The key to match.
   * \return    A const iterator to the first element not less than \p key.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  template <typename K, typename Req = IsTransparentType<Compare, K>>
  auto lower_bound(K const& x) const noexcept -> const_iterator {
    return tree_.LowerBound(x);
  }

  /*!
   * \brief     Get the first element greater than \p key.
   * \param[in] key  The key to match.
   * \return    An iterator to the first element not less than \p key.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  auto upper_bound(key_type const& key) noexcept -> iterator { return tree_.UpperBound(key); }

  /*!
   * \brief     Get the first element greater than \p key.
   * \param[in] key  The key to match.
   * \return    A const iterator to the first element not less than \p key.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  auto upper_bound(key_type const& key) const noexcept -> const_iterator { return tree_.UpperBound(key); }

  /*!
   * \brief     Get the first element greater than \p key.
   * \tparam    K    The type of the key.
   * \tparam    Req  Must be true for this function to participate in resolution overload.
   * \param[in] x  The key to match.
   * \return    An iterator to the first element not less than \p key.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  template <typename K, typename Req = IsTransparentType<Compare, K>>
  auto upper_bound(K const& x) noexcept -> iterator {
    return tree_.UpperBound(x);
  }

  /*!
   * \brief     Get the first element greater than \p key.
   * \tparam    K    The type of the key.
   * \tparam    Req  Must be true for this function to participate in resolution overload.
   * \param[in] x  The key to match.
   * \return    A const iterator to the first element not less than \p key.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  template <typename K, typename Req = IsTransparentType<Compare, K>>
  auto upper_bound(K const& x) const noexcept -> const_iterator {
    return tree_.UpperBound(x);
  }

  /*!
   * \brief     Check if the map compares equal to \p other.
   * \param[in] other  The map to compare equality to.
   * \return    True if the maps compare equal, false otherwise.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  auto CompareEqual(Map const& other) const noexcept -> bool { return this->tree_.CompareTreeEqual(other.tree_); }

  /*!
   * \brief     Check if the map compares lexicographically to \p other.
   * \param[in] other  The map to compare with.
   * \return    True if the lexicographical comparison is true, false otherwise.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  auto CompareTreeLexicographically(Map const& other) const noexcept -> bool {
    return this->tree_.CompareTreeLexicographically(other.tree_);
  }

  /*!
   * \brief   Get the function that compares keys.
   * \return  The key comparison function object.
   * \pre     -
   * \context ANY
   * \steady  TRUE
   */
  auto key_comp() const noexcept -> Compare { return tree_.GetCompare().GetKeyComp(); }

  /*!
   * \brief   Get the function that compares object of type value_type.
   * \return  The value_type comparison function object.
   * \pre     -
   * \context ANY
   * \steady  TRUE
   */
  auto value_comp() const noexcept -> value_compare { return tree_.GetCompare(); }

 private:
  /*!
   * \brief The underlying RBTree.
   */
  tree_type tree_;

  /*!
   * \brief     Get the value from an iterator.
   * \param[in] it  The iterator to get the value from.
   * \return    The mapped value in the object pointer to by \p it.
   * \vprivate  Component Private
   */
  static auto GetValue(iterator& it) noexcept -> mapped_type& { return (*it).second; }

  /*!
   * \brief     Emplace an element from a given value_type. If the key of \p val already exists in map, do nothing.
   * \details   Types Key and Value must meet C++ Named Requirement: CopyConstructible.
   * \tparam    EmplaceAtPos  Boolean to determine if the given position or the found position will be used as initial
   *                          hint to the emplace function.
   * \param[in] val  The value_type to insert in the map.
   * \param[in] pos  A const iterator to the position before which the new element will be inserted.
   * \return    A result containing a pair with an iterator to the inserted element (or the element that prevented the
   *            insertion) and a boolean that states if the element was inserted or not. If an error occurred, the
   *            result contains the error.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       -
   * \context   ANY
   * \vprivate  Component Private
   * \steady    TRUE
   */
  template <bool EmplaceAtPos>
  auto GenericEmplace(value_type const& val, const_iterator pos) noexcept
      -> ara::core::Result<std::pair<iterator, bool>> {
    ara::core::Result<std::pair<iterator, bool>> out{amsr::ContainerErrorCode::kAllocationError};
    const_iterator const found_it{static_cast<Map const*>(this)->find(val.first)};
    if (found_it == cend()) {
      const_iterator const kEpos{EmplaceAtPos ? pos : found_it};
      out = IteratorToPair(tree_.EmplaceHint(kEpos, val));

    } else {
      out.EmplaceValue(found_it.NonConstCast(), false);
    }
    return out;
  }

  /*!
   * \brief     Emplace an element from a given value_type. If the key of \p val already exists in map, do nothing.
   * \details   Types Key and Value must meet C++ Named Requirement: CopyConstructible.
   * \tparam    EmplaceAtPos  Boolean to determine if the given position or the found position will be used as initial
   *                          hint to the emplace function.
   * \param[in] val  The value_type to insert in the map.
   * \param[in] pos  A const iterator to the position before which the new element will be inserted.
   * \return    A result containing a pair with an iterator to the inserted element (or the element that prevented the
   *            insertion) and a boolean that states if the element was inserted or not. If an error occurred, the
   *            result contains the error.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       -
   * \context   ANY
   * \vprivate  Component Private
   * \steady    TRUE
   */
  template <bool EmplaceAtPos>
  auto GenericEmplace(value_type&& val, const_iterator pos) noexcept -> ara::core::Result<std::pair<iterator, bool>> {
    ara::core::Result<std::pair<iterator, bool>> out{amsr::ContainerErrorCode::kAllocationError};
    const_iterator const found_it{static_cast<Map const*>(this)->find(val.first)};
    if (found_it == cend()) {
      const_iterator const kEpos{EmplaceAtPos ? pos : found_it};
      out = IteratorToPair(tree_.EmplaceHint(kEpos, std::move(val)));

    } else {
      out.EmplaceValue(found_it.NonConstCast(), false);
    }
    return out;
  }

  /*!
   * \brief     Insert an element to the map if the key \p k does not exists, otherwise assigns the value of the
   *            existing element to \p obj. If EmplaceAtPos is true, the element will be inserted as close as possible
   *            to the position just before \p pos.
   * \details   Types Key and Value must meet C++ Named Requirement: CopyConstructible.
   * \tparam    EmplaceAtPos  Boolean to determine if the given position or the found position will be used as initial
   *                          hint to the emplace function.
   * \param[in] pos  A const iterator to the position before which the new element will be inserted.
   * \param[in] key  The key used both to look up and to insert if not found.
   * \param[in] obj  The value to insert or assign.
   * \return    A result containing a pair with an iterator to the inserted element (or the element that was updated)
   *            and a boolean that is true if inserting took place, and false if assignment took place.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       -
   * \context   ANY
   * \vprivate  Component Private
   * \steady    TRUE
   */
  template <bool EmplaceAtPos>
  auto GenericInsertOrAssign(const_iterator pos, key_type&& key, mapped_type&& obj) noexcept
      -> ara::core::Result<std::pair<iterator, bool>> {
    ara::core::Result<std::pair<iterator, bool>> out{amsr::ContainerErrorCode::kAllocationError};
    iterator found_it{find(key)};
    if (found_it == cend()) {
      const_iterator const kEpos{EmplaceAtPos ? pos : const_iterator{found_it}};
      out = IteratorToPair(tree_.EmplaceHint(kEpos, std::move(key), std::move(obj)));
    } else {
      static_assert(std::is_nothrow_copy_assignable<mapped_type>::value, "mapped_type must be nothrow copy assignable");
      (*found_it).second = obj;
      out.EmplaceValue(found_it, false);
    }
    return out;
  }

  /*!
   * \brief     Insert an element to the map if the key \p k does not exists, otherwise assigns the value of the
   *            existing element to \p obj. If EmplaceAtPos is true, the element will be inserted as close as possible
   *            to the position just before \p pos.
   * \details   Types Key and Value must meet C++ Named Requirement: CopyConstructible.
   * \tparam    EmplaceAtPos  Boolean to determine if the given position or the found position will be used as initial
   *                          hint to the emplace function.
   * \param[in] pos  A const iterator to the position before which the new element will be inserted.
   * \param[in] key  The key used both to look up and to insert if not found.
   * \param[in] obj  The value to insert or assign.
   * \return    A result containing a pair with an iterator to the inserted element (or the element that was updated)
   *            and a boolean that is true if inserting took place, and false if assignment took place.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       -
   * \context   ANY
   * \vprivate  Component Private
   * \steady    TRUE
   */
  template <bool EmplaceAtPos>
  auto GenericInsertOrAssign(const_iterator pos, key_type const& key, mapped_type&& obj) noexcept
      -> ara::core::Result<std::pair<iterator, bool>> {
    ara::core::Result<std::pair<iterator, bool>> out{amsr::ContainerErrorCode::kAllocationError};
    iterator found_it{find(key)};
    if (found_it == cend()) {
      const_iterator const kEpos{EmplaceAtPos ? pos : const_iterator{found_it}};
      out = IteratorToPair(tree_.EmplaceHint(kEpos, key, std::move(obj)));
    } else {
      (*found_it).second = obj;
      out.EmplaceValue(found_it, false);
    }
    return out;
  }

  /*!
   * \brief     Help function to create a map object from a given result, containing a RBTree.
   * \details   Used by the CreateMap functions where a tree shall be created via RBTree::CreateTree and the return
   *            type is a result with a RBTree as value.
   * \param[in] r  The result with a RBTree as value.
   * \return    A result containing a new map initialized with the RBTree from \p r.
   * \error     Any error from \p r propagates to the return type.
   * \pre       -
   * \context   ANY
   * \vprivate  Component Private
   * \steady    TRUE
   */
  static auto MapFromTree(ara::core::Result<tree_type>&& r) noexcept -> ara::core::Result<Map> {
    ara::core::Result<Map> out{amsr::ContainerErrorCode::kAllocationError};
    r.HasValue() ? out.EmplaceValue(std::move(r).Value()) : out.EmplaceError(r.Error());
    return out;
  }

  /*!
   * \brief     Help function to convert an result with value type std::pair<iterator,bool> to an result type with
   *            value type iterator.
   * \details   The Insert/Emplace functions with a hint shall return a Result with an iterator while the functions
   *            without a hint shall return a std::pair with an iterator and a bool. To avoid unnecessary code these
   *            will use the same underlying function, and this function to convert the pair type to iterator type.
   * \param[in] pair  A result with a std::pair as value.
   * \return    A result containing the first element in the pair as value type.
   * \error     Any error from \p r propagates to the return type.
   * \pre       -
   * \context   ANY
   * \vprivate  Component Private
   * \steady    TRUE
   */
  static auto PairToIterator(ara::core::Result<std::pair<iterator, bool>>&& pair) noexcept
      -> ara::core::Result<iterator> {
    ara::core::Result<iterator> out{amsr::ContainerErrorCode::kAllocationError};
    pair.HasValue() ? out.EmplaceValue(pair.Value().first) : out.EmplaceError(pair.Error());
    return out;
  }

  /*!
   * \brief     Help function to convert a result with an iterator to a pair with an iterator and boolean.
   * \details   The boolean should indicate if the iterator was inserted or not, and if it is not, \p res should
   *            contain an error. To be removed after HUN-4253.
   * \param[in] res  The result with an iterator to convert to a pair.
   * \return    A result containing a pair with an iterator and a boolean, where to boolean always is true. Or a result
   *            with an error if \p res contains an error.
   * \pre       \p res should contain an iterator pointing to the element that was inserted.
   * \context   ANY
   * \vprivate  Component Private
   * \steady    TRUE
   */
  static auto IteratorToPair(ara::core::Result<iterator> const& res) -> ara::core::Result<std::pair<iterator, bool>> {
    ara::core::Result<std::pair<iterator, bool>> out{amsr::ContainerErrorCode::kAllocationError};
    if (res.HasValue()) {
      out.EmplaceValue(res.Value(), true);
    } else {
      out.EmplaceError(res.Error());
    }
    return out;
  }

  /*!
   * \brief     Overloaded help function to extract the key from different parameter setups.
   * \details   Used to extract the key to pass to find() in order to determine if the key already exists in the map
   *            before emplacing. When optimizing map (removing the extra search) this function shall be removed.
   * \tparam    K  Type of the key.
   * \tparam    V  Type of the value.
   * \param[in] p  The value type object to extract the key from.
   * \return    The key.
   * \pre       -
   * \context   ANY
   * \vprivate  Component Private
   * \steady    TRUE
   */
  template <typename K, typename V>
  static auto ExtractKey(std::pair<K, V> const& p) noexcept -> K const& {
    return p.first;
  }

  /*!
   * \brief     Overloaded help function to extract the key from different parameter setups.
   * \details   Used to extract the key to pass to find() in order to determine if the key already exists in the map
   *            before emplacing. When optimizing map (removing the extra search) this function shall be removed.
   * \param[in] k  The key to return.
   * \return    The key.
   * \pre       -
   * \context   ANY
   * \vprivate  Component Private
   * \steady    TRUE
   */
  static auto ExtractKey(key_type const& k, mapped_type const&) noexcept -> key_type const& {
    return std::forward<key_type const&>(k);
  }

  /*!
   * \brief     Overloaded help function to extract the key from different parameter setups.
   * \details   Used to extract the key to pass to find() in order to determine if the key already exists in the map
   *            before emplacing. When optimizing map (removing the extra search) this function shall be removed.
   * \tparam    U     The type of the key.
   * \tparam    Args  The types of the arguments
   * \param[in] key  A tuple containing the key to return.
   * \return    The key.
   * \pre       -
   * \context   ANY
   * \vprivate  Component Private
   * \steady    TRUE
   */
  template <typename U, typename... Args>
  static auto ExtractKey(std::piecewise_construct_t, std::tuple<U> const& key, std::tuple<Args...> const&) noexcept
      -> key_type const& {
    return std::get<0>(key);
  }
};

/*!
 * \brief     Equality operator for two maps.
 * \details   Equal if each element (traversed from begin() to end()) is equal.
 * \tparam    Key      Type of the key.
 * \tparam    T        Type of the value.
 * \tparam    Compare  Type of the compare function.
 * \tparam    Alloc    Type of the allocator.
 * \param[in] lhs  The lhs map in the equality comparison.
 * \param[in] rhs  The rhs map in the equality comparison.
 * \return    True if the maps compare equal, false otherwise.
 * \pre       -
 * \context   ANY
 * \vprivate  Component Private
 * \steady    TRUE
 */
template <typename Key, class T, class Compare, class Alloc>
bool operator==(amsr::Map<Key, T, Compare, Alloc> const& lhs, amsr::Map<Key, T, Compare, Alloc> const& rhs) noexcept {
  return lhs.CompareEqual(rhs);
}

/*!
 * \brief     Not equal operator for two maps.
 * \details   Not equal if the equality operator returns false.
 * \tparam    Key      Type of the key.
 * \tparam    T        Type of the value.
 * \tparam    Compare  Type of the compare function.
 * \tparam    Alloc    Type of the allocator.
 * \param[in] lhs  The lhs map in the equality comparison.
 * \param[in] rhs  The rhs map in the equality comparison.
 * \return    True if the maps compare not equal, false otherwise.
 * \pre       -
 * \context   ANY
 * \vprivate  Component Private
 * \steady    TRUE
 */
template <typename Key, class T, class Compare, class Alloc>
bool operator!=(amsr::Map<Key, T, Compare, Alloc> const& lhs, amsr::Map<Key, T, Compare, Alloc> const& rhs) noexcept {
  return !(lhs == rhs);
}

/*!
 * \brief     Less than operator for two maps.
 * \tparam    Key      Type of the key.
 * \tparam    T        Type of the value.
 * \tparam    Compare  Type of the compare function.
 * \tparam    Alloc    Type of the allocator.
 * \param[in] lhs  The lhs map in the lexicographical comparison.
 * \param[in] rhs  The rhs map in the lexicographical comparison.
 * \return    True if lhs is lexicographically less than rhs , false otherwise.
 * \pre       -
 * \context   ANY
 * \vprivate  Component Private
 * \steady    TRUE
 */
template <typename Key, class T, class Compare, class Alloc>
bool operator<(amsr::Map<Key, T, Compare, Alloc> const& lhs, amsr::Map<Key, T, Compare, Alloc> const& rhs) noexcept {
  return lhs.CompareTreeLexicographically(rhs);
}

/*!
 * \brief     Less than, or equal to operator for two maps.
 * \tparam    Key      Type of the key.
 * \tparam    T        Type of the value.
 * \tparam    Compare  Type of the compare function.
 * \tparam    Alloc    Type of the allocator.
 * \param[in] lhs  The lhs map in the lexicographical comparison.
 * \param[in] rhs  The rhs map in the lexicographical comparison.
 * \return    True if lhs is lexicographically less than or equal to rhs, false otherwise.
 * \pre       -
 * \context   ANY
 * \vprivate  Component Private
 * \steady    TRUE
 */
template <typename Key, class T, class Compare, class Alloc>
bool operator<=(amsr::Map<Key, T, Compare, Alloc> const& lhs, amsr::Map<Key, T, Compare, Alloc> const& rhs) noexcept {
  return lhs.CompareTreeLexicographically(rhs) || lhs.CompareEqual(rhs);
}

/*!
 * \brief     Greater than operator for two maps.
 * \tparam    Key      Type of the key.
 * \tparam    T        Type of the value.
 * \tparam    Compare  Type of the compare function.
 * \tparam    Alloc    Type of the allocator.
 * \param[in] lhs  The lhs map in the lexicographical comparison.
 * \param[in] rhs  The rhs map in the lexicographical comparison.
 * \return    True if lhs is lexicographically greater than rhs , false otherwise.
 * \pre       -
 * \context   ANY
 * \vprivate  Component Private
 * \steady    TRUE
 */
template <typename Key, class T, class Compare, class Alloc>
bool operator>(amsr::Map<Key, T, Compare, Alloc> const& lhs, amsr::Map<Key, T, Compare, Alloc> const& rhs) noexcept {
  return rhs.CompareTreeLexicographically(lhs);
}

/*!
 * \brief     Greater than or equal to operator for two maps.
 * \tparam    Key      Type of the key.
 * \tparam    T        Type of the value.
 * \tparam    Compare  Type of the compare function.
 * \tparam    Alloc    Type of the allocator.
 * \param[in] lhs  The lhs map in the lexicographical comparison.
 * \param[in] rhs  The rhs map in the lexicographical comparison.
 * \return    True if lhs is lexicographically greater than or equal to rhs , false otherwise.
 * \pre       -
 * \context   ANY
 * \vprivate  Component Private
 * \steady    TRUE
 */
template <typename Key, class T, class Compare, class Alloc>
bool operator>=(amsr::Map<Key, T, Compare, Alloc> const& lhs, amsr::Map<Key, T, Compare, Alloc> const& rhs) noexcept {
  return rhs.CompareTreeLexicographically(lhs) || rhs.CompareEqual(lhs);
}

// VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
// VECTOR Next Construct AutosarC++17_10-M17.0.3: MD_VAC_M17.0.3_standardLibraryFunctionNameOverride
// VECTOR Next Construct AutosarC++17_10-A2.11.4: MD_VAC_A2.11.4_nameOfObjectOrFunctionReused
/*!
 * \brief     An overloaded std::swap function for amsr::Map data type.
 * \tparam    K      key for (key-value) pair for amsr::Map data type.
 * \tparam    V      Value for (key-value) pair for amsr::Map data type.
 * \tparam    C      Comparer for K (std::less<K>) for (key-value) pair for amsr::Map data type.
 * \tparam    Alloc  ara::core::PolymorphicAllocator for amsr::Map data type.
 * \param[in] lhs  Left hand side map to swap.
 * \param[in] rhs  Right hand side map to swap.
 * \pre       -
 * \trace     SPEC-7552503
 * \vpublic
 * \spec      requires true; \endspec
 */
template <typename K, typename V, typename C = std::less<K>,
          typename Alloc = ara::core::PolymorphicAllocator<std::pair<K const, V>>>
void swap(Map<K, V, C, Alloc>& lhs, Map<K, V, C, Alloc>& rhs) noexcept {
  lhs.swap(rhs);  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
}

}  // namespace amsr

#endif
