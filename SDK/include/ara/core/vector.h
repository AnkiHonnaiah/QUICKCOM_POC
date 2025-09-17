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
/*!        \file  ara/core/vector.h
 *        \brief  SWS core type ara::core::Vector.
 *         \unit  VaCommonLib::ContainerLibrary::DynamicContainers::Vector
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_ARA_CORE_VECTOR_H_
#define LIB_VAC_INCLUDE_ARA_CORE_VECTOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <vector>

#include "amsr/vector.h"
#include "ara/core/memory_resource.h"
#include "vac/language/throw_or_terminate.h"

namespace ara {
namespace core {

// VECTOR Next Construct Metric-OO.WMC.One: MD_VAC_Metric-OO.WMC.One
/*!
 * \brief   Container which can change in size.
 * \tparam  T       Type of the elements in Vector. Must be Erasable, but many methods impose stricter requirements.
 * \tparam  Alloc   Allocator of Vector. Must meet the C++ named requirement "Allocator". The default type is
 *                  ara::core::PolymorphicAllocator<T>.
 * \vpublic
 */
template <typename T, typename Alloc = ara::core::PolymorphicAllocator<T>>
class Vector {
 public:
  /*!
   * \brief Type of the contained element.
   */
  using value_type = T;

  /*!
   * \brief Type of the allocator.
   */
  using allocator_type = Alloc;

  /*!
   * \brief Type of the allocator traits.
   */
  using allocator_traits = std::allocator_traits<allocator_type>;

  /*!
   * \brief Type of the size field.
   */
  using size_type = std::size_t;

  /*!
   * \brief Type used for pointer arithmetic.
   */
  using difference_type = std::ptrdiff_t;

  /*!
   * \brief Alias type for a reference to an element.
   */
  using reference = value_type&;

  /*!
   * \brief Alias type for a const reference to an element.
   */
  using const_reference = value_type const&;

  /*!
   * \brief Alias type for a pointer to an element.
   */
  using pointer = typename allocator_traits::pointer;

  /*!
   * \brief Alias type for a const pointer to an element.
   */
  using const_pointer = typename allocator_traits::const_pointer;

  /*!
   * \brief   The type of an iterator to elements.
   */
  using iterator = pointer;

  /*!
   * \brief   The type of an const_iterator to elements.
   */
  using const_iterator = std::remove_const_t<T> const*;

  /*!
   * \brief Alias type for a reverse_iterator to an element.
   */
  using reverse_iterator = std::reverse_iterator<iterator>;

  /*!
   * \brief Alias type for a const_reverse_iterator to an element.
   */
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  /*!
   * \brief     Constructs an empty vector using the default allocator.
   * \context   ANY
   * \pre       -
   * \spec      requires true; \endspec
   * \vpublic
   */
  // VECTOR NC AutosarC++17_10-A12.1.5: MD_VAC_A12.1.5_useDelegatingConstructor
  Vector() noexcept(noexcept(Alloc())) : delegate_() {}  // VCA_VAC_ACCESS_POSSIBLY_OUT_OFF_BOUNDS

  /*!
   * \brief     Constructs an empty vector using the provided allocator.
   * \param[in] allocator   Allocator to use for all memory allocations of this container.
   * \context   ANY
   * \pre       -
   * \spec      requires true; \endspec
   * \vpublic
   */
  explicit Vector(Alloc const& allocator) : delegate_(allocator) {}

  // VECTOR NC AutosarC++17_10-A12.1.4: MD_VAC_A12.1.4_userDefinedImplicitMoveConversionForContainersAllowed
  // VECTOR NC VectorC++-V12.1.4: MD_VAC_V12.1.4_userDefinedImplicitMoveConversionForContainersAllowed
  /*!
   * \brief     Constructs an ara::core::Vector from an amsr::Vector.
   * \details   \p other is in an unspecified state after the conversion.
   * \param[in] other  The amsr::Vector to move from.
   * \context   ANY
   * \pre       -
   * \spec      requires true; \endspec
   * \vprivate Product private
   */
  Vector(amsr::Vector<T, Alloc>&& other) : delegate_(std::move(other)) {}

  // VECTOR NC AutosarC++17_10-A12.1.4: MD_VAC_A12.1.4_userDefinedImplicitMoveConversionForContainersAllowed
  // VECTOR NC VectorC++-V12.1.4: MD_VAC_V12.1.4_userDefinedImplicitMoveConversionForContainersAllowed
  // Deleted implicit convert constructor with copy
  Vector(amsr::Vector<T, Alloc> const& other) = delete;

  // VECTOR NC AutosarC++17_10-A13.5.2: MD_VAC_A13.5.2_userDefinedImplicitMoveConversionForContainersAllowed
  /*!
   * \brief     Converts an ara::core::Vector to an amsr::Vector.
   * \details   Moves the underlying amsr::Vector object. To use the moved-from object after conversion is
   *            undefined behavior.
   * \return    A amsr::Vector object, moved from this.
   * \context   ANY
   * \pre       -
   * \spec      requires true; \endspec
   * \steady    TRUE
   * \vprivate Product private
   */
  operator amsr::Vector<T, Alloc>() && { return std::move(delegate_); }

  // VECTOR NC AutosarC++17_10-A13.5.2: MD_VAC_A13.5.2_userDefinedImplicitMoveConversionForContainersAllowed
  // Deleted implicit copy conversion function.
  operator amsr::Vector<T, Alloc>() & = delete;

  /*!
   * \brief     Constructor to allow implicit conversion from std::vector.
   * \tparam    A           Allocator type of the std::vector.
   * \param[in] std_vector  The std::vector to convert.
   * \context   ANY
   * \pre       -
   * \vprivate  Product private
   */
  template <typename A>
  // VECTOR NL VectorC++-V12.1.4: MD_VAC_V12-1-4_constructorShallBeDeclaredExplicitTemporary
  [[deprecated("Conversion from std::vector to ara::core::Vector will be removed in HUN-4410.")]] Vector(
      std::vector<value_type, A>& std_vector)
      : Vector(std_vector.begin(), std_vector.end()) {}

  /*!
   * \brief     Constructor to allow implicit conversion from const std::vector.
   * \tparam    A           Allocator type of the std::vector.
   * \param[in] std_vector  The std::vector to convert.
   * \context   ANY
   * \pre       -
   * \vprivate  Product private
   */
  template <typename A>
  // VECTOR NL VectorC++-V12.1.4: MD_VAC_V12-1-4_constructorShallBeDeclaredExplicitTemporary
  [[deprecated("Conversion from std::vector to ara::core::Vector will be removed in HUN-4410.")]] Vector(
      std::vector<value_type, A> const& std_vector)
      : Vector(std_vector.begin(), std_vector.end()) {}

  /*!
   * \brief     Implicit conversion from std::vector.
   * \tparam    A           Allocator type of the std::vector.
   * \param[in] std_vector  The std::vector to convert.
   * \context   ANY
   * \pre       -
   * \vprivate  Product private
   */
  template <typename A>
  // VECTOR NL VectorC++-V12.1.4: MD_VAC_V12-1-4_constructorShallBeDeclaredExplicitTemporary
  [[deprecated("conversion from std::vector to ara::core::Vector will be removed in HUN-4410.")]] Vector(
      std::vector<value_type, A>&& std_vector)
      : Vector(std_vector.begin(), std_vector.end()) {}

  /*!
   * \brief     Constructs a vector with \p count copies of elements with value \p value.
   * \details   Type T must meet C++ Named Requirements: CopyInsertable.
   * \param[in] count       The size of the vector.
   * \param[in] value       The value to initialize elements with.
   * \param[in] allocator   Allocator to use for all memory allocations.
   * \throws    std::bad_alloc If allocation of memory was not successful.
   * \throws    std::length_error If the requested size is larger than max_size().
   * \throws    <TYPE> Passes on any exception the allocator might throw.
   * \context   ANY
   * \pre       -
   * \spec      requires true; \endspec
   * \vpublic
   */
  Vector(size_type count, value_type const& value, allocator_type const& allocator = allocator_type())
      : delegate_(ThrowOnErrorOrReturn(DelegateType::CreateVector(count, value, allocator))) {}

  /*!
   * \brief     Constructs a vector with \p count default-inserted instances of T. No copies are made.
   * \details   Type T must meet C++ Named Requirements: DefaultInsertable.
   * \param[in] count       The size of the container.
   * \param[in] allocator   Allocator to use for all memory allocations of this container.
   * \throws    std::bad_alloc If allocation of memory was not successful.
   * \throws    std::length_error If the requested size is larger than max_size().
   * \throws    <TYPE> Passes on any exception the allocator might throw.
   * \context   ANY
   * \pre       -
   * \spec      requires true; \endspec
   * \vpublic
   */
  explicit Vector(size_type count, allocator_type const& allocator = allocator_type())
      : delegate_(ThrowOnErrorOrReturn(DelegateType::CreateVector(count, allocator))) {}

  /*!
   * \brief     Constructs a vector with the contents from the range [\p first, \p last).
   * \details   Type T must meet C++ Named Requirements: EmplaceConstructible.
   * \tparam    InputIterator   Type of the input iterator.
   * \param[in] first       The beginning of the range to copy the elements from.
   * \param[in] last        The end of the range to copy the elements from. Must be reachable from \p first by
   *                        incrementation.
   * \param[in] allocator   Allocator to use for all memory allocations of this container.
   * \throws    std::bad_alloc If allocation of memory was not successful.
   * \throws    std::length_error If the size derived from \p first and \p last is larger than max_size().
   * \throws    <TYPE> Passes on any exception the allocator might throw.
   * \context   ANY
   * \pre       -
   * \spec      requires true; \endspec
   * \vpublic
   */
  // VECTOR NC AutosarC++17_10-A15.4.4: MD_VAC_A15.4.4_noexceptMightActuallyThrow
  template <typename InputIterator,
            std::enable_if_t<std::is_convertible<typename std::iterator_traits<InputIterator>::iterator_category,
                                                 std::input_iterator_tag>::value,
                             bool> = true>
  Vector(InputIterator first, InputIterator last, allocator_type const& allocator = allocator_type())
      : delegate_(ThrowOnErrorOrReturn(DelegateType::CreateVector(first, last, allocator))) {}

  /*!
   * \brief     Constructs the vector with the contents of the initializer list \p i_list.
   * \details   Type T must meet C++ Named Requirements: EmplaceConstructible.
   * \param[in] i_list      Initializer list to initialize the elements of the container with.
   * \param[in] allocator   Allocator to use for all memory allocations of this container.
   * \throws    std::bad_alloc If allocation of memory was not successful.
   * \throws    std::length_error If the length of the initializer list is larger than max_size().
   * \throws    <TYPE> Passes on any exception the allocator might throw.
   * \context   ANY
   * \pre       -
   * \spec      requires true; \endspec
   * \vpublic
   */
  // VECTOR NC VectorC++-V8.5.4: MD_VAC_V8-5-4_ctorShallNotBeDefinedToAcceptStdInitializerList
  // VECTOR NC AutosarC++17_10-A8.5.4: MD_VAC_A8.5.4_initializerlistShallBeDefinedInClasses
  // VECTOR NL VectorC++-V12.1.4: MD_VAC_V12-1-4_constructorShallBeDeclaredExplicit
  Vector(std::initializer_list<T> i_list, allocator_type const& allocator = allocator_type())
      : delegate_(ThrowOnErrorOrReturn(DelegateType::CreateVector(i_list, allocator))) {}

  /*!
   * \brief     Copy constructor.
   * \details   Type T must meet C++ Named Requirements: CopyInsertable.
   * \param[in] other       Another container to be used as source to initialize the elements of the container with.
   * \throws    std::bad_alloc If allocation of memory was not successful.
   * \throws    <TYPE> Passes on any exception the allocator might throw.
   * \context   ANY
   * \pre       -
   * \spec      requires true; \endspec
   * \vpublic
   */
  Vector(Vector const& other)
      // VCA_VAC_VALID_CONSTRUCTOR_CALLED
      : delegate_(ThrowOnErrorOrReturn(DelegateType::CreateVector(other.delegate_.begin(), other.delegate_.end()))) {}

  /*!
   * \brief     Constructs a vector with the copy of the contents of \p other using \p allocator as the allocator.
   * \details   Type T must meet C++ Named Requirements: CopyInsertable.
   * \param[in] other       Another container to be used as source to initialize the elements of the container with.
   * \param[in] allocator   Allocator to use for all memory allocations of this container.
   * \throws    std::bad_alloc If allocation of memory was not successful.
   * \throws    <TYPE> Passes on any exception the allocator might throw.
   * \context   ANY
   * \pre       -
   * \spec      requires true; \endspec
   * \vpublic
   */
  Vector(Vector const& other, allocator_type const& allocator)
      : delegate_(ThrowOnErrorOrReturn(DelegateType::CreateVector(other.delegate_, allocator))) {}

  /*!
   * \brief     Constructs the container with the contents of \p other using move semantics.
   * \details   Allocator is obtained by move-construction from the allocator belonging to \p other. After the move,
   *            \p other is guaranteed to be empty().
   * \param[in] other   Another container to be used as source to initialize the elements of the container with.
   * \context   ANY
   * \pre       -
   * \spec      requires true; \endspec
   * \vpublic
   */
  // VCA_VAC_VALID_CONSTRUCTOR_CALLED
  Vector(Vector&& other) noexcept : delegate_(std::move(other.delegate_)) {}

  /*!
   * \brief     Constructs the container with the contents of \p other using move semantics.
   * \details   Using \p allocator as the allocator for the new container, moving the contents from \p other; if \p
   *            allocator != \p other.get_allocator(), this results in an element-wise move. (In that case, \p other is
   *            not guaranteed to be empty after the move.) Type T must meet C++ Named Requirements: MoveInsertable.
   * \param[in] other       Another container to be used as source to initialize the elements of the container with.
   * \param[in] allocator   The allocator to use for the new container. Caution: using \p other.get_allocator() here may
   *                        lead to problems depending on the underlying memory resource.
   * \throws    std::bad_alloc If a relocation happens and new memory can not be allocated.
   * \throws    <TYPE> Passes on any exception the allocator might throw.
   * \context   ANY
   * \pre       -
   * \spec      requires true; \endspec
   * \vpublic
   */
  Vector(Vector&& other, allocator_type const& allocator)
      : delegate_(std::move(other.delegate_), allocator, std::true_type()) {}

  /*!
   * \brief     Destructs the Vector.
   * \details   The destructors of the elements are called and the used storage is deallocated. Note that if the
   *            elements are pointers, the pointed-to objects are not destroyed.
   * \context   ANY
   * \pre       -
   * \spec      requires true; \endspec
   * \vpublic
   */
  // VCA_VAC_SAFE_DEALLOCATION_RUN_TIME_CHECK
  virtual ~Vector() = default;

  /*!
   * \brief     Operator for implicit conversion to std::vector.
   * \tparam    A             Allocator type of the std::vector.
   * \returns   A std::vector with the same content as this vector.
   * \context   ANY
   * \pre       -
   * \vprivate  Product private
   */
  [[deprecated("Implicit conversion from ara::core::Vector to std::vector will be removed in HUN-4410.")]]
  // VECTOR NL AutosarC++17_10-A13.5.2: MD_VAC_A13.5.2_userDefinedConversionOperatorTemporary
  operator std::vector<value_type>() const {
    return std::vector<value_type>(begin(), end());  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
  }

  /*!
   * \brief     Allows assignment of a std::vector to this vector.
   * \tparam    A       Allocator type of the std::vector.
   * \param[in] other   The std::vector to copy the content from.
   * \returns   This vector.
   * \context   ANY
   * \pre       -
   * \vprivate  Product private
   */
  template <typename A>
  [[deprecated("Assignment from std::vector to ara::core::Vector will be removed in HUN-4410.")]] auto operator=(
      std::vector<value_type, A> const& other) & -> Vector& {
    assign(other.begin(), other.end());
    return *this;
  }

  /*!
   * \brief     Allows assignment of a std::vector&& to this vector.
   * \tparam    A       Allocator type of the std::vector.
   * \param[in] other   The std::vector to copy the content from.
   * \returns   This vector.
   * \context   ANY
   * \pre       -
   * \vprivate  Product private
   */
  template <typename A>
  [[deprecated("Assignment from std::vector to ara::core::Vector will be removed in HUN-4410.")]] auto operator=(
      std::vector<value_type, A>&& other) & -> Vector& {
    assign(other.begin(), other.end());
    return *this;
  }

  /*!
   * \brief     Replaces the contents with a copy of the contents of \p other.
   * \details   Type T must meet C++ Named Requirements: CopyInsertable.
   * \param[in] other Another container to use as data source.
   * \return    This vector.
   * \throws    std::bad_alloc If allocation of memory was not successful.
   * \throws    <TYPE> Passes on any exception the allocator might throw.
   * \context   ANY
   * \pre       -
   * \spec      requires true; \endspec
   * \vpublic
   */
  // VECTOR NL AutosarC++17_10-A12.8.7: MD_VAC_A12.8.7_assignmentOperatorWithRefqualifierFulfillApi
  auto operator=(Vector const& other) & -> Vector& {
    ThrowOnError(delegate_.Assign(other.delegate_));
    return *this;
  }

  /*!
   * \brief     Replaces the contents with those of \p other using move semantics.
   * \details   Might throw an error if the allocator is not propagated and does not always compare equal and if the
   *            allocate() or deallocate() methods of the allocator are not noexcept. The latter is a divergence from
   *            std::vector, but facilitate replacing std::vector with ara::core::Vector, because in both types, move
   *            assignment is noexcept.
   *            If the allocator is propagated or compares equal to the allocator of \p other, then \p other after the
   *            move is guaranteed to be equal to this vector before the move.
   *            Type T must meet C++ Named Requirements: MoveInsertable, MoveAssignable.
   * \param[in] other Another container to use as data source.
   * \return    A reference to this vector after assigning.
   * \throws    std::bad_alloc If a relocation happens and new memory can not be allocated.
   * \throws    <TYPE> Passes on any exception the allocator might throw.
   * \context   ANY
   * \pre       -
   * \spec      requires true; \endspec
   * \vpublic
   */
  // VECTOR NL AutosarC++17_10-A12.8.7: MD_VAC_A12.8.7_assignmentOperatorWithRefqualifierFulfillApi
  auto operator=(Vector&& other) & noexcept(
      allocator_traits::propagate_on_container_move_assignment::value || allocator_traits::is_always_equal::value ||
      (noexcept(std::declval<Alloc>().allocate(1)) && noexcept(std::declval<Alloc>().deallocate(nullptr, 1))))
      -> Vector& {
    delegate_.ThrowingMoveAssign(std::move(other.delegate_));  // VCA_VAC_VALID_NONSTATIC_METHOD
    // VECTOR NL AutosarC++17_10-A12.8.3: MD_VAC_A12.8.3_dontReadAccessAMovedFromObject
    return *this;  // VCA_VAC_VALID_RETURN_VALUE
  }

  /*!
   * \brief     Replaces the contents with the elements in the initializer list \p i_list.
   * \details   Type T must meet C++ Named Requirements: EmplaceConstructible.
   * \param[in] i_list  Initializer list to copy the values from.
   * \return    This vector.
   * \throws    std::bad_alloc If allocation of memory was not successful.
   * \throws    std::length_error If the requested size is larger than max_size().
   * \throws    <TYPE> Passes on any exception the allocator might throw.
   * \context   ANY
   * \pre       -
   * \spec      requires true; \endspec
   * \vpublic
   */
  // VECTOR NL AutosarC++17_10-A12.8.7: MD_VAC_A12.8.7_assignmentOperatorWithRefqualifierFulfillApi
  auto operator=(std::initializer_list<T> i_list) -> Vector& {
    ThrowOnError(delegate_.Assign(i_list));
    return *this;
  }

  /*!
   * \brief     Replaces the contents with \p count copies of value \p value.
   * \details   Type T must meet C++ Named Requirements: CopyInsertable, CopyAssignable.
   * \param[in] count   The new size of the container.
   * \param[in] value   The value to initialize elements of the container with.
   * \throws    std::bad_alloc If allocation of memory was not successful.
   * \throws    std::length_error If the requested size is larger than max_size().
   * \throws    <TYPE> Passes on any exception the allocator might throw.
   * \context   ANY
   * \pre       -
   * \spec      requires true; \endspec
   * \vpublic
   */
  void assign(size_type count, value_type const& value) { ThrowOnError(delegate_.Assign(count, value)); }

  /*!
   * \brief Replaces the contents with copies of those in the range [\p first, \p last).
   * \details The behavior is undefined if either argument is an iterator into *this.
   *          Type T must meet C++ Named Requirements: EmplaceConstructible and assignable from \p *first.
   * \tparam InputIterator Type of the input iterator.
   * \param[in] first Beginning of the range to copy the elements from.
   * \param[in] last End of the range to copy the elements from. Must be reachable from \p first by incrementation.
   * \throws    std::bad_alloc If allocation of memory was not successful.
   * \throws    std::length_error If the size derived from \p first and \p last is larger than max_size().
   * \throws    <TYPE> Passes on any exception the allocator might throw.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  // VECTOR NC AutosarC++17_10-A15.4.4: MD_VAC_A15.4.4_noexceptMightActuallyThrow
  template <typename InputIterator,
            std::enable_if_t<std::is_convertible<typename std::iterator_traits<InputIterator>::iterator_category,
                                                 std::input_iterator_tag>::value,
                             bool> = true>
  void assign(InputIterator first, InputIterator last) {
    ThrowOnError(delegate_.Assign(first, last));
  }

  /*!
   * \brief Replaces the contents with the elements in the initializer list \p i_list.
   * \details Type T must meet C++ Named Requirements: EmplaceConstructible.
   * \param[in] i_list Initializer list to copy the values from.
   * \throws    std::bad_alloc If allocation of memory was not successful.
   * \throws    std::length_error If the requested size is larger than max_size().
   * \throws    <TYPE> Passes on any exception the allocator might throw.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  void assign(std::initializer_list<T> i_list) { ThrowOnError(delegate_.Assign(i_list)); }

  /*!
   * \brief Gets the allocator.
   * \return The allocator.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  auto get_allocator() const noexcept -> allocator_type { return delegate_.get_allocator(); }

  /*!
   * \brief     Returns a reference to the element at specified location \p pos, with bounds checking.
   * \param[in] pos Position of the element to return.
   * \return    A reference to the requested element.
   * \throws    std::out_of_range If \p pos is not within the range of the vector.
   * \context   ANY
   * \pre       -
   * \spec      requires true; \endspec
   * \vpublic
   */
  auto at(size_type pos) -> reference {
    amsr::core::Result<reference> result{delegate_.At(pos)};
    ThrowOnError(result);
    return result.Value();  // VCA_VAC_VALID_RETURN_VALUE
  }

  /*!
   * \brief Returns a const reference to the element at specified location \p pos, with bounds checking.
   * \param[in] pos Position of the element to return.
   * \return A const reference to the requested element.
   * \throws  std::out_of_range If \p pos is not within the range of the vector.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  auto at(size_type pos) const -> const_reference {
    amsr::core::Result<const_reference> const result{delegate_.At(pos)};
    ThrowOnError(result);
    return result.Value();  // VCA_VAC_VALID_RETURN_VALUE
  }

  /*!
   * \brief Returns a reference to the element at specified location \p pos, without bounds checking.
   * \details Accessing a nonexistent element is undefined behavior.
   * \param[in] pos Position of the element to return. Must be in the range [0, size()).
   * \return A reference to the requested element.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  auto operator[](size_type pos) noexcept -> reference { return delegate_[pos]; }

  /*!
   * \brief Returns a const_reference to the element at specified location \p pos, without bounds checking.
   * \details Accessing a nonexistent element is undefined behavior.
   * \param[in] pos Position of the element to return. Must be in the range [0, size()).
   * \return A const_reference to the requested element.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  auto operator[](size_type pos) const noexcept -> const_reference { return delegate_[pos]; }

  /*!
   * \brief Gets a reference to the first element.
   * \return A reference to the first element.
   * \context ANY
   * \pre This vector must not be empty.
   * \vpublic
   */
  auto front() -> reference { return delegate_.front(); }

  /*!
   * \brief Gets a const_reference to the first element.
   * \return A const_reference to the first element.
   * \context ANY
   * \pre This vector must not be empty.
   * \vpublic
   */
  auto front() const -> const_reference { return delegate_.front(); }

  /*!
   * \brief Gets a reference to the last element.
   * \return A reference to the last element.
   * \context ANY
   * \pre This vector must not be empty.
   * \vpublic
   */
  auto back() -> reference { return delegate_.back(); }

  /*!
   * \brief Gets a const_reference to the last element.
   * \return A const_reference to the last element.
   * \context ANY
   * \pre This vector must not be empty.
   * \vpublic
   */
  auto back() const -> const_reference { return delegate_.back(); }

  /*!
   * \brief Returns a pointer to the underlying array serving as element storage.
   * \return A pointer to the underlying element storage. For empty vectors, it may or may not return a null pointer.
   *         Otherwise returns the pointer to the first element.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  auto data() noexcept -> T* { return delegate_.data(); }

  /*!
   * \brief Returns a const pointer to the underlying array serving as element storage.
   * \return A const pointer to the underlying element storage. For empty vectors, it may or may not return a null
   *         pointer. Otherwise returns the pointer to the first element.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  auto data() const noexcept -> T const* { return delegate_.data(); }

  /*!
   * \brief Gets an iterator to the first element. If the vector is empty, the returned iterator is equal to the
   *        one returned by end().
   * \return The iterator to the first element.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  auto begin() noexcept -> iterator { return delegate_.begin(); }

  /*!
   * \brief Gets a const_iterator to the first element. If the vector is empty, the returned iterator is equal to
   *        the one returned by end().
   * \return The const_iterator to the first element.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  auto begin() const noexcept -> const_iterator { return delegate_.begin(); }

  /*!
   * \brief Gets an iterator to the element after the last element in the vector.
   * \details Trying to dereference the returned iterator results in undefined behavior.
   * \return The iterator to the element after the last element in the vector.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  auto end() noexcept -> iterator { return delegate_.end(); }

  /*!
   * \brief Gets a const_iterator to the element after the last element in the vector.
   * \details Trying to dereference the returned iterator results in undefined behavior.
   * \return The const_iterator to the element after the last element in the vector.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  auto end() const noexcept -> const_iterator { return delegate_.end(); }

  /*!
   * \brief Gets a const_iterator to the first element. If the vector is empty, the returned iterator is equal to
   *        the one returned by cend().
   * \return The const_iterator to the first element.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  auto cbegin() const noexcept -> const_iterator { return delegate_.cbegin(); }

  /*!
   * \brief Gets a const_iterator to the element after the last element in the vector.
   * \details Trying to dereference the returned iterator results in undefined behavior.
   * \return The const_iterator to the element after the last element in the vector.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  auto cend() const noexcept -> const_iterator { return delegate_.cend(); }

  /*!
   * \brief Gets a reverse_iterator to the last element. If the vector is empty, the returned iterator is equal to
   *        the one returned by rend().
   * \return The reverse_iterator to the last element.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  auto rbegin() noexcept -> reverse_iterator { return delegate_.rbegin(); }

  /*!
   * \brief Gets a const_reverse_iterator to the last element. If the vector is empty, the returned iterator is
   *        equal to the one returned by rend().
   * \return The const_reverse_iterator to the last element.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  auto rbegin() const noexcept -> const_reverse_iterator { return delegate_.rbegin(); }

  /*!
   * \brief Gets a reverse_iterator to the element before the first element in the vector.
   * \details Trying to dereference the returned iterator results in undefined behavior.
   * \return The reverse_iterator to the element before the first element in the vector.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  auto rend() noexcept -> reverse_iterator { return delegate_.rend(); }

  /*!
   * \brief Gets a const_reverse_iterator to the element before the first element in the vector.
   * \details Trying to dereference the returned iterator results in undefined behavior.
   * \return The const_reverse_iterator to the element before the first element in the vector.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  auto rend() const noexcept -> const_reverse_iterator { return delegate_.rend(); }

  /*!
   * \brief Gets a const_reverse_iterator to the last element. If the vector is empty, the returned iterator is
   *        equal to the one returned by crend().
   * \return The const_reverse_iterator to the last element.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  auto crbegin() const noexcept -> const_reverse_iterator { return delegate_.crbegin(); }

  /*!
   * \brief Gets a const_reverse_iterator to the element before the first element in the vector.
   * \details Trying to dereference the returned iterator results in undefined behavior.
   * \return The const_reverse_iterator to the element before the first element in the vector.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  auto crend() const noexcept -> const_reverse_iterator { return delegate_.crend(); }

  /*!
   * \brief Checks if the vector has no elements.
   * \return True if the vector contains no elements, false otherwise.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  auto empty() const noexcept -> bool { return delegate_.empty(); }

  /*!
   * \brief Returns the number of elements in the container.
   * \return The number of elements in the container.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  auto size() const noexcept -> size_type { return delegate_.size(); }

  /*!
   * \brief Gets the maximum size for allocation.
   * \return The maximum size for allocation.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  auto max_size() const noexcept -> size_type { return delegate_.max_size(); }

  /*!
   * \brief Increase the capacity of the vector (the total number of elements that the vector can hold
   * without requiring reallocation) to a value that's greater or equal to \p new_capacity.
   *
   * \details If \p new_capacity is greater than the current capacity(), new storage is allocated, otherwise the
   * function does nothing.
   *
   * reserve() does not change the size of the vector.
   *
   * If \p new_capacity is greater than capacity(), all iterators, including the end() iterator, and all references to
   * the elements are invalidated. Otherwise, no iterators or references are invalidated.
   *
   * Type T must meet C++ Named Requirements: MoveInsertable.
   *
   * \param[in] new_capacity New capacity of the vector, in number of elements.
   * \throws    std::length_error If \p new_capacity > max_size().
   * \throws    std::bad_alloc If allocation of memory was not successful.
   * \throws    <TYPE> Passes on any exception the allocator might throw.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  void reserve(size_type new_capacity) { ThrowOnError(delegate_.Reserve(new_capacity)); }

  /*!
   * \brief Returns the number of elements that the container has currently allocated space for.
   * \return Capacity of the currently allocated storage.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  auto capacity() const noexcept -> size_type { return delegate_.capacity(); }

  /*!
   * \brief Reduces the capacity to the current size().
   * \details If the capacity is already equal to the current size, nothing happens.
   *          Type T must meet C++ Named Requirements: MoveInsertable.
   * \throws    std::bad_alloc If allocation of memory was not successful.
   * \throws    <TYPE> Passes on any exception the allocator might throw.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  void shrink_to_fit() { ThrowOnError(delegate_.ShrinkToFit()); }

  /*!
   * \brief Erases all elements from the container.
   * \details After this call, size() returns zero.
   *          Invalidates any references, pointers, or iterators referring to contained elements.
   *          Any past-the-end iterators are also invalidated. Leaves the capacity() of the vector unchanged.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  void clear() noexcept { delegate_.clear(); }

  /*!
   * \brief Inserts an element at the specified location in the vector.
   * \details Type T must meet C++ Named Requirements: CopyInsertable, CopyAssignable.
   * \param[in] pos Iterator before which the element will be inserted. Must be in the range [begin(), end()].
   * \param[in] value Element value to insert.
   * \return An iterator pointing to the inserted value.
   * \throws    std::bad_alloc If allocation of memory was not successful.
   * \throws    std::length_error If the requested final size is larger than max_size().
   * \throws    <TYPE> Passes on any exception the allocator might throw.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  auto insert(const_iterator pos, value_type const& value) -> iterator {
    return ThrowOnErrorOrReturn(delegate_.Insert(pos, value));
  }

  /*!
   * \brief Move inserts an element at the specified location in the vector.
   * \details Type T must meet C++ Named Requirements: MoveInsertable, MoveAssignable.
   * \param[in] pos Iterator before which the element will be inserted. Must be in the range [begin(), end()].
   * \param[in] value Element value to insert.
   * \return An iterator pointing to the inserted value.
   * \throws    std::bad_alloc If allocation of memory was not successful.
   * \throws    std::length_error If the requested final size is larger than max_size().
   * \throws    <TYPE> Passes on any exception the allocator might throw.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  auto insert(const_iterator pos, value_type&& value) -> iterator {
    return ThrowOnErrorOrReturn(delegate_.Insert(pos, std::move(value)));
  }

  /*!
   * \brief Inserts \p count copies of \p value before the specified location in the vector.
   * \details Type T must meet C++ Named Requirements: CopyInsertable, CopyAssignable.
   * \param[in] pos Iterator before which the elements will be inserted. Must be in the range [begin(), end()].
   * \param[in] count Number of elements to insert.
   * \param[in] value Element value to insert.
   * \return An iterator pointing to the first element inserted or \p pos if \p count == 0.
   * \throws    std::bad_alloc If allocation of memory was not successful.
   * \throws    std::length_error If the requested final size is larger than max_size().
   * \throws    <TYPE> Passes on any exception the allocator might throw.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  auto insert(const_iterator pos, size_type count, value_type const& value) -> iterator {
    return ThrowOnErrorOrReturn(delegate_.Insert(pos, count, value));
  }

  /*!
   * \brief Inserts copies of the elements from range [\p first, \p last) before the specified location in the vector.
   * \details Type T must meet C++ Named Requirements: EmplaceConstructible.
   * \tparam InputIterator Type of the input iterator.
   * \param[in] pos     Iterator before which the elements will be inserted. Must be in the range [begin(), end()].
   * \param[in] first   Beginning of the range of elements to insert. Must not be iterator into *this.
   * \param[in] last    Iterator after the last element to insert. Must not be iterator into *this. Must be reachable
   *                    from \p first by incrementation.
   * \return An iterator pointing to the first element inserted or \p pos if \p first == \p last.
   * \throws    std::bad_alloc If allocation of memory was not successful.
   * \throws    std::length_error If the requested final size is larger than max_size().
   * \throws    <TYPE> Passes on any exception the allocator might throw.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  // VECTOR NC AutosarC++17_10-A15.4.4: MD_VAC_A15.4.4_noexceptMightActuallyThrow
  template <typename InputIterator,
            std::enable_if_t<std::is_convertible<typename std::iterator_traits<InputIterator>::iterator_category,
                                                 std::input_iterator_tag>::value,
                             bool> = true>
  auto insert(const_iterator pos, InputIterator first, InputIterator last) -> iterator {
    return ThrowOnErrorOrReturn(delegate_.Insert(pos, first, last));
  }

  /*!
   * \brief Inserts elements from initializer list before the specified location in the vector.
   * \details Type T must meet C++ Named Requirements: EmplaceConstructible.
   * \param[in] pos Iterator before which the elements will be inserted. Must be in the range [begin(), end()].
   * \param[in] i_list Initializer list to insert the values from.
   * \return An iterator pointing to the first element inserted or \p pos if \p i_list is empty.
   * \throws    std::bad_alloc If allocation of memory was not successful.
   * \throws    std::length_error If the requested final size is larger than max_size().
   * \throws    <TYPE> Passes on any exception the allocator might throw.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  auto insert(const_iterator pos, std::initializer_list<value_type> i_list) -> iterator {
    return ThrowOnErrorOrReturn(delegate_.Insert(pos, i_list));
  }

  /*!
   * \brief Inserts a new element before the specified location in the vector.
   * \details Type T must meet C++ Named Requirements: EmplaceConstructible, MoveInsertable, MoveAssignable.
   * \tparam Args Types of the arguments.
   * \param[in] pos     Position before which the element will be inserted. Must be in the range [begin(), end()].
   * \param[in] args    Arguments to forward to the constructor of the element.
   * \return An iterator to the inserted element.
   * \throws    std::bad_alloc If allocation of memory was not successful.
   * \throws    std::length_error If the requested final size is larger than max_size().
   * \throws    <TYPE> Passes on any exception the allocator might throw.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  // VECTOR NC AutosarC++17_10-A15.4.4: MD_VAC_A15.4.4_noexceptMightActuallyThrow
  template <typename... Args>
  auto emplace(const_iterator pos, Args&&... args) -> iterator {
    return ThrowOnErrorOrReturn(delegate_.Emplace(pos, std::forward<Args>(args)...));
  }

  /*!
   * \brief Remove the element at \p pos.
   * \details Type T must meet C++ Named Requirements: MoveAssignable.
   * \param[in] pos The element to remove. Must be in the range [begin(), end()).
   * \return An iterator following the removed element.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  auto erase(const_iterator pos) -> iterator { return delegate_.erase(pos); }

  /*!
   * \brief Remove the elements from \p first to the element before \p last.
   * \details Type T must meet C++ Named Requirements: MoveAssignable.
   * \param[in] first Iterator to first element to remove. Range: [cbegin(), cend()).
   * \param[in] last Iterator to element after the last element to remove. Range: [cbegin(), cend()]. Must be reachable
   *                 from \p first by incrementation.
   * \return An iterator following the last removed element.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  auto erase(const_iterator first, const_iterator last) -> iterator { return delegate_.erase(first, last); }

  /*!
   * \brief     Appends a copy of the given element \p value to the end of the vector.
   * \details   Type T must meet C++ Named Requirements: CopyInsertable.
   * \param[in] value   The value of the element to append.
   * \throws    std::bad_alloc If allocation of memory was not successful.
   * \throws    std::length_error If the requested final size is larger than max_size().
   * \throws    <TYPE> Passes on any exception the allocator might throw.
   * \context   ANY
   * \pre       -
   * \spec      requires true; \endspec
   * \vpublic
   */
  void push_back(value_type const& value) { ThrowOnError(delegate_.PushBack(value)); }

  /*!
   * \brief     Appends the given element \p value to the end of the vector by moving it into the new element.
   * \details   Type T must meet C++ Named Requirements: MoveInsertable.
   * \param[in] value   The value of the element to append.
   * \throws    std::bad_alloc If allocation of memory was not successful.
   * \throws    std::length_error If the requested final size is larger than max_size().
   * \throws    <TYPE> Passes on any exception the allocator might throw.
   * \context   ANY
   * \pre       -
   * \spec      requires true; \endspec
   * \vpublic
   */
  void push_back(value_type&& value) { ThrowOnError(delegate_.PushBack(std::move(value))); }

  /*!
   * \brief Appends a new element to the end of the container.
   * \details The element is constructed through std::allocator_traits::construct.
   *          Type T must meet C++ Named Requirements: EmplaceConstructible, MoveInsertable.
   * \tparam Args Types of the arguments.
   * \param[in] args Arguments to forward to the constructor of the element.
   * \throws    std::bad_alloc If allocation of memory was not successful.
   * \throws    std::length_error If the requested final size is larger than max_size().
   * \throws    <TYPE> Passes on any exception the allocator might throw.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  // VECTOR NC AutosarC++17_10-A15.4.4: MD_VAC_A15.4.4_noexceptMightActuallyThrow
  template <typename... Args>
  void emplace_back(Args&&... args) {
    ThrowOnError(delegate_.EmplaceBack(std::forward<Args>(args)...));
  }

  /*!
   * \brief Removes the last element from the vector.
   * \details Calling this method on an empty vector results in undefined behavior.
   * \context ANY
   * \pre Vector is not empty().
   * \vpublic
   */
  void pop_back() { delegate_.pop_back(); }

  /*!
   * \brief Resizes the vector to contain \p new_size elements.
   * \details If \p new_size is smaller than the current size(), elements are removed from the back. If \p new_size is
   *          larger, elements are appended by default-insertion.
   *          Type T must meet C++ Named Requirements: MoveInsertable, DefaultInsertable.
   * \param[in] new_size The target size.
   * \throws    std::bad_alloc If allocation of memory was not successful.
   * \throws    std::length_error If the requested size is larger than max_size().
   * \throws    <TYPE> Passes on any exception the allocator might throw.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  void resize(size_type new_size) { ThrowOnError(delegate_.Resize(new_size)); }

  /*!
   * \brief Resizes the vector to contain \p new_size elements.
   * \details If \p new_size is smaller than the current size(), elements are removed from the back. If \p new_size is
   *          larger, copies of \p value are appended.
   *          Type T must meet C++ Named Requirements: CopyInsertable.
   * \param[in] new_size    The target size.
   * \param[in] value       The value to append if the size is increased.
   * \throws    std::bad_alloc If allocation of memory was not successful.
   * \throws    std::length_error If the requested size is larger than max_size().
   * \throws    <TYPE> Passes on any exception the allocator might throw.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  void resize(size_type new_size, value_type const& value) { ThrowOnError(delegate_.Resize(new_size, value)); }

  /*!
   * \brief Exchanges content and capacity of this vector with those of \p other.
   * \param[in] other   The container to exchange content with.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \vpublic
   */
  void swap(Vector& other) noexcept { delegate_.swap(other.delegate_); }

 private:
  /*!
   * \brief The type of the wrapped vector.
   */
  using DelegateType = amsr::Vector<T, Alloc>;

  /*!
   * \brief The wrapped vector to which all method calls are delegated.
   */
  DelegateType delegate_;

  /*!
   * \brief     If the given result is an error, throws it as exception, does nothing otherwise.
   * \details   If exceptions are switched off, the process is terminated instead of throwing an exception.
   * \tparam    V       The result type.
   * \param[in] result  The result.
   * \throws    std::out_of_range If \p result is kOutOfRangeError.
   * \throws    std::length_error If \p result is kLengthError.
   * \throws    std::bad_alloc If \p result is kAllocationError.
   * \context   ANY
   * \pre       -
   */
  // VECTOR NC AutosarC++17_10-A15.4.4: MD_VAC_A15.4.4_noexceptMightActuallyThrow
  template <typename V>
  static void ThrowOnError(amsr::core::Result<V> const& result) {
    if (!result.HasValue()) {
      // VECTOR NL AutosarC++17_10-A7.2.1: MD_VAC_A7.2.1_valueIsNotWithinEnumeratorRange
      switch (static_cast<amsr::ContainerErrorCode>(result.Error().Value())) {
        // VECTOR NC AutosarC++17_10-M6.4.3: MD_VAC_M6.4.3_switchNotTerminatedWithBreak
        // VECTOR NL AutosarC++17_10-M6.4.5: MD_VAC_M6.4.5_switchNotTerminatedWithBreak
        case amsr::ContainerErrorCode::kOutOfRangeError: {
          vac::language::ThrowOrTerminate<std::out_of_range>(result.Error().Message().ToString());
        }
        // VECTOR NC AutosarC++17_10-M6.4.3: MD_VAC_M6.4.3_switchNotTerminatedWithBreak
        // VECTOR NL AutosarC++17_10-M6.4.5: MD_VAC_M6.4.5_switchNotTerminatedWithBreak
        case amsr::ContainerErrorCode::kLengthError: {
          vac::language::ThrowOrTerminate<std::length_error>(result.Error().Message().ToString());
        }
        // VECTOR NC AutosarC++17_10-M6.4.3: MD_VAC_M6.4.3_switchNotTerminatedWithBreak
        // VECTOR NL AutosarC++17_10-M6.4.5: MD_VAC_M6.4.5_switchNotTerminatedWithBreak
        case amsr::ContainerErrorCode::kAllocationError: {
          vac::language::ThrowOrTerminate<std::bad_alloc>();
        }
      }
    }
  }

  /*!
   * \brief     If the given result is an error, throws it as exception, returns the value otherwise.
   * \details   If exceptions are switched off, the process is terminated instead of throwing an exception.
   * \tparam    V       The result type.
   * \param[in] result  The result.
   * \return    The value of the result.
   * \throws    std::out_of_range If \p result is kOutOfRangeError.
   * \throws    std::length_error If \p result is kLengthError.
   * \throws    std::bad_alloc If \p result is kAllocationError.
   * \context   ANY
   * \pre       -
   */
  // VECTOR NC AutosarC++17_10-A15.4.4: MD_VAC_A15.4.4_noexceptMightActuallyThrow
  template <typename V>
  static auto ThrowOnErrorOrReturn(amsr::core::Result<V>&& result) -> V&& {
    ThrowOnError(result);
    return std::move(result.Value());
  }
};

/*!
 * \brief Checks if two vectors are equal.
 * \details Two vectors are equal if they have the same size() and their elements compare equal.
 * \param[in] lhs   The first vector.
 * \param[in] rhs   The second vector.
 * \return True if sizes and elements are equal, false otherwise.
 * \context ANY
 * \pre -
 * \spec requires true; \endspec
 * \vpublic
 */
// VECTOR NC AutosarC++17_10-A15.4.4: MD_VAC_A15.4.4_noexceptMightActuallyThrow
template <typename T, typename Alloc>
auto operator==(Vector<T, Alloc> const& lhs, Vector<T, Alloc> const& rhs) -> bool {
  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
  return (lhs.size() == rhs.size()) && std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin());
}

/*!
 * \brief Checks if two vectors are not equal.
 * \details Two vectors are equal if they have the same size() and their elements compare equal.
 * \param[in] lhs   The first vector.
 * \param[in] rhs   The second vector.
 * \return False if sizes and elements are equal, true otherwise.
 * \context ANY
 * \pre -
 * \spec requires true; \endspec
 * \vpublic
 */
// VECTOR NC AutosarC++17_10-A15.4.4: MD_VAC_A15.4.4_noexceptMightActuallyThrow
template <typename T, typename Alloc>
auto operator!=(Vector<T, Alloc> const& lhs, Vector<T, Alloc> const& rhs) -> bool {
  return !(lhs == rhs);
}

/*!
 * \brief Checks if the first vector is lexicographically lesser than the second vector.
 * \details Vector a is lexicographically lesser than vector b, if one of the following applies:
 *           - a is empty and b is nonempty
 *           - a is a prefix of b
 *           - for the smallest index i where a[i] != b[i], a[i] < b[i] applies.
 * \param[in] lhs   The first vector.
 * \param[in] rhs   The second vector.
 * \return True if \p lhs is lexicographically lesser than \p rhs, false otherwise.
 * \context ANY
 * \pre -
 * \spec requires true; \endspec
 * \vpublic
 */
// VECTOR NC AutosarC++17_10-A15.4.4: MD_VAC_A15.4.4_noexceptMightActuallyThrow
template <typename T, typename Alloc>
auto operator<(Vector<T, Alloc> const& lhs, Vector<T, Alloc> const& rhs) -> bool {
  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
  return std::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}

/*!
 * \brief Checks if the first vector is lexicographically greater than the second vector.
 * \details Vector a is lexicographically greater than vector b, iff b is lexicographically lesser than a.
 * \param[in] lhs   The first vector.
 * \param[in] rhs   The second vector.
 * \return True if \p lhs is lexicographically greater than \p rhs, false otherwise.
 * \context ANY
 * \pre -
 * \spec requires true; \endspec
 * \vpublic
 */
// VECTOR NC AutosarC++17_10-A15.4.4: MD_VAC_A15.4.4_noexceptMightActuallyThrow
template <typename T, typename Alloc>
auto operator>(Vector<T, Alloc> const& lhs, Vector<T, Alloc> const& rhs) -> bool {
  return rhs < lhs;
}

/*!
 * \brief Checks if the first vector is lexicographically lesser or equal than the second vector.
 * \details Vector a is lexicographically lesser or equal than vector b, iff a is not lexicographically greater than b.
 * \param[in] lhs   The first vector.
 * \param[in] rhs   The second vector.
 * \return True if \p lhs is lexicographically lesser or equal than \p rhs, false otherwise.
 * \context ANY
 * \pre -
 * \spec requires true; \endspec
 * \vpublic
 */
// VECTOR NC AutosarC++17_10-A15.4.4: MD_VAC_A15.4.4_noexceptMightActuallyThrow
template <typename T, typename Alloc>
auto operator<=(Vector<T, Alloc> const& lhs, Vector<T, Alloc> const& rhs) -> bool {
  return !(lhs > rhs);
}

/*!
 * \brief Checks if the first vector is lexicographically greater or equal than the second vector.
 * \details Vector a is lexicographically greater or equal than vector b, iff a is not lexicographically lesser than b.
 * \param[in] lhs   The first vector.
 * \param[in] rhs   The second vector.
 * \return True if \p lhs is lexicographically greater or equal than \p rhs, false otherwise.
 * \context ANY
 * \pre -
 * \spec requires true; \endspec
 * \vpublic
 */
// VECTOR NC AutosarC++17_10-A15.4.4: MD_VAC_A15.4.4_noexceptMightActuallyThrow
template <typename T, typename Alloc>
auto operator>=(Vector<T, Alloc> const& lhs, Vector<T, Alloc> const& rhs) -> bool {
  return !(lhs < rhs);
}

/*!
 * \brief Swaps the content of the two vectors.
 * \param[in] lhs   The first vector.
 * \param[in] rhs   The second vector.
 * \context ANY
 * \pre -
 * \spec requires true; \endspec
 * \vpublic
 */
// VECTOR NC AutosarC++17_10-A15.4.4: MD_VAC_A15.4.4_noexceptMightActuallyThrow
template <typename T, typename Alloc>
void swap(Vector<T, Alloc>& lhs, Vector<T, Alloc>& rhs) noexcept {
  lhs.swap(rhs);
}

}  // namespace core
}  // namespace ara

#endif  // LIB_VAC_INCLUDE_ARA_CORE_VECTOR_H_
