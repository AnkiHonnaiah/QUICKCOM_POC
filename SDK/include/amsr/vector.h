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
/*!      \file  amsr/vector.h
 *      \brief  Core type amsr::Vector
 *       \unit  VaCommonLib::ContainerLibrary::DynamicContainers::Vector
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_AMSR_VECTOR_H_
#define LIB_VAC_INCLUDE_AMSR_VECTOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <memory>
#include <type_traits>
#include <utility>

#include "amsr/container_error_domain.h"
#include "amsr/core/abort.h"
#include "amsr/core/result.h"
#include "ara/core/memory_resource.h"
#include "vac/language/throw_or_terminate.h"
#include "vac/memory/non_zeroing_allocator.h"
#include "vac/testing/test_adapter.h"

// Forward-declaration of ara::core::Vector, so that it can be friended by amsr::Vector. Including
// "ara/core/vector.h" here wouldn't work because it would result in a circular dependency.
namespace ara {
namespace core {

template <typename T, typename Alloc>
class Vector;

}  // namespace core
}  // namespace ara

namespace amsr {

// VECTOR Next Construct Metric-OO.WMC.One: MD_VAC_Metric-OO.WMC.One
/*!
 * \brief   Container which can change in size.
 * \tparam  T      Type of the contained elements.
 *                 - T must fulfill the C++ Named Requirement Erasable (ISO/IEC 14882:2014(E): 23.2.1 General container
 *                   requirements, Table 96 and Subclause 14), i.e. it may be one of the following types:
 *                   - class with accessible destructor
 *                   - scalar type (including pointer types)
 *                 - Alloc adds limitations for T:
 *                   - T must be a non-const object type (ISO/IEC 14882:2014(E): 17.6.3.5 Allocator requirements, Table
 *                     27)
 *                   - to be an object type, T must not be any of the following types (ISO/IEC 14882:2014(E): 3.9
 *                     Types, Subclause 8):
 *                     - array type
 *                     - function type
 *                     - reference type
 *                     - void
 *                 - many member functions impose stricter requirements on T
 * \tparam  Alloc  is an allocator which fulfills C++ Named Requirements: Allocator. The default type of Alloc is
 *                 ara::core::PolymorphicAllocator<T>.
 * \trace   CREQ-VaCommonLib-Vector
 * \vpublic
 */
template <typename T, typename Alloc = ara::core::PolymorphicAllocator<T>>
class Vector final {
  // Declare ara::core::Vector as friend, because it needs access to move constructor and move-assignment which
  // throw instead of abort.
  // VECTOR NL AutosarC++17_10-A11.3.1: MD_VAC_A11.3.1_doNotUseFriend
  friend class ara::core::Vector<T, Alloc>;

  /*!
   * \brief declaration for testing purposes
   */
  // VECTOR NC AutosarC++17_10-A11.3.1: MD_VAC_A11.3.1_doNotUseFriend
  template <typename>
  FRIEND_TEST(UT__VectorSteadyMode__Movable, ThrowingMoveAssignment);

  /*!
   * \brief declaration for testing purposes
   */
  // VECTOR NC AutosarC++17_10-A11.3.1: MD_VAC_A11.3.1_doNotUseFriend
  template <typename>
  FRIEND_TEST(UT__VectorSteadyMode__Movable, ThrowingMoveConstructorWithCustomAllocator);

  /*!
   * \brief declaration for testing purposes
   */
  // VECTOR NC AutosarC++17_10-A11.3.1: MD_VAC_A11.3.1_doNotUseFriend
  template <typename>
  FRIEND_TEST(UT__VectorSteadyMode__Movable, ThrowingMoveConstructorWithCustomEqualAllocators);

  /*!
   * \brief declaration for testing purposes
   */
  // VECTOR NC AutosarC++17_10-A11.3.1: MD_VAC_A11.3.1_doNotUseFriend
  template <typename>
  FRIEND_TEST(UT__VectorSteadyMode__Movable, ThrowingMoveConstructorWithCustomNonEqualAllocators);

  /*!
   * \brief Is std::true_type if the value type is move constructible, otherwise std::false_type.
   */
  using HasMoveCtorT = std::is_move_constructible<T>;

  /*!
   * \brief Is std::true_type if the specified allocator is ara::core::PolymorphicAllocator, otherwise std::false_type.
   */
  using IsPolyAllocT = std::is_same<Alloc, ara::core::PolymorphicAllocator<T>>;

  /*!
   * \brief Is std::true_type if the specified allocator is vac::memory::NonZeroingAllocator, otherwise std::false_type.
   */
  using IsNonZeroingAllocT = std::is_same<Alloc, vac::memory::NonZeroingAllocator<T>>;

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
   * \brief Typedef for the allocator type used after rebinding.
   */
  using actual_allocator_type = typename allocator_traits::template rebind_alloc<T>;

  /*!
   * \brief Type of the size field.
   */
  using size_type = std::size_t;

  /*!
   * \brief Type used for pointer arithmetic.
   */
  using difference_type = std::ptrdiff_t;

  /*!
   * \brief Alias type for a pointer to an element.
   */
  using pointer = typename allocator_traits::pointer;

  /*!
   * \brief Alias type for a const pointer to an element.
   */
  using const_pointer = typename allocator_traits::const_pointer;

  /*!
   * \brief Alias type for a reference to an element.
   */
  using reference = value_type&;

  /*!
   * \brief Alias type for a const reference to an element.
   */
  using const_reference = value_type const&;

  /*!
   * \brief The type of an iterator to elements.
   */
  using iterator = pointer;

  /*!
   * \brief The type of an const_iterator to elements.
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
   * \brief     Constructs an empty vector, using the specified allocator.
   * \details   No allocation.
   * \param[in] allocator  Allocator to use for all memory allocations of this container.
   * \throws    <TYPE> Passes on any exception the allocator might throw on instantiation.
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  explicit Vector(allocator_type const& allocator = allocator_type()) noexcept(noexcept(Alloc()))
      : start_(), finish_(), end_of_storage_(), allocator_(allocator){};  // VCA_VAC_ACCESS_POSSIBLY_OUT_OFF_BOUNDS

  /*!
   * \brief Deleted as errors cannot be reported from constructors. CreateVector(Vector const&) shall be used instead.
   */
  Vector(Vector const&) = delete;

  /*!
   * \brief     Constructs the container with the contents of \p other using move semantics.
   * \details   Allocator is obtained by move-construction from the allocator belonging to \p other. After the move, \p
   *            other is guaranteed to be empty().
   * \param[in] other  Another container to be used as source to initialize the elements of the container with.
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  Vector(Vector&& other) noexcept
      : start_(std::move(other.start_)),                    // VCA_VAC_VALID_CONSTRUCTOR_CALLED
        finish_(std::move(other.finish_)),                  // VCA_VAC_VALID_CONSTRUCTOR_CALLED
        end_of_storage_(std::move(other.end_of_storage_)),  // VCA_VAC_VALID_CONSTRUCTOR_CALLED
        // VECTOR NL AutosarC++17_10-A12.8.4: MD_VAC_A12.8.4_moveConstructorShallNotUseCopySemantics
        allocator_(std::move(other.allocator_)) {  // VCA_VAC_VALID_CONSTRUCTOR_CALLED
    other.ResetStorage();                          // VCA_VAC_VALID_NONSTATIC_METHOD
  }

  /*!
   * \brief     Constructs the container with the contents of \p other using move semantics.
   * \details   Using \p allocator as the allocator for the new container, moving the contents from \p other; if \p
   *            allocator != \p other.get_allocator(), this results in an element-wise move. (In that case, \p other is
   *            not guaranteed to be empty after the move.) Might cause an error which results in termination. Type T
   *            must meet C++ Named Requirements: MoveInsertable.
   * \param[in] other      Another container to be used as source to initialize the elements of the container with.
   * \param[in] allocator  The allocator to use for the new container. Caution: using \p other.get_allocator() here may
   *                       lead to problems depending on the underlying memory resource.
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  Vector(Vector&& other, allocator_type const& allocator)
      : Vector(std::move(other), allocator, std::false_type(), typename allocator_traits::is_always_equal{}) {}

  /*!
   * \brief     Constructs a vector with the copy of the contents of \p other using \p allocator as the allocator.
   * \details   Type T must meet C++ Named Requirements: CopyInsertable. Allocation of size: \p other.cend() - \p
   *            other.cbegin().
   * \param[in] other      Another container to be used as source to initialize the elements of the container with.
   * \param[in] allocator  Allocator to use for all memory allocations of this container.
   * \return    The vector on success, an error code otherwise.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  static auto CreateVector(Vector const& other, allocator_type const& allocator) -> amsr::core::Result<Vector> {
    return CreateVector(other.cbegin(), other.cend(), allocator);
  }

  /*!
   * \brief     Constructs a vector with the copy of the contents of \p other using \p allocator as the allocator.
   * \details   Type T must meet C++ Named Requirements: CopyInsertable. If memory allocation was successful,
   *            allocation of size: \p other.cend() - \p other.cbegin(). If memory allocation was unsuccessful,
   *            amsr::core::Abort() will be called.
   * \param[in] other      Another container to be used as source to initialize the elements of the container with.
   * \param[in] allocator  Allocator to use for all memory allocations of this container.
   * \return    The vector.
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  static auto CreateVectorOrAbort(Vector const& other, allocator_type const& allocator) noexcept -> Vector {
    amsr::core::Result<Vector> result{CreateVector(other, allocator)};

    if (!result.HasValue()) {
      amsr::core::Abort(result.Error().Message().data());
    }
    return std::move(result.Value());
  }

  /*!
   * \brief     Constructs the container with the contents of the initializer list \p i_list.
   * \details   Type T must meet C++ Named Requirements: EmplaceConstructible. Allocation of size: \p i_list.end() - \p
   *            i_list.begin().
   * \param[in] i_list     Initializer list to initialize the elements of the container with.
   * \param[in] allocator  Allocator to use for all memory allocations of this container.
   * \return    The vector on success, an error code otherwise.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \error     ContainerErrorCode::kLengthError if the length of the initializer list is larger than max_size().
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  static auto CreateVector(std::initializer_list<value_type> i_list, allocator_type const& allocator = allocator_type())
      -> amsr::core::Result<Vector> {
    return CreateVector(i_list.begin(), i_list.end(), allocator);
  }

  /*!
   * \brief     Constructs the container with the contents of the initializer list \p i_list.
   * \details   Type T must meet C++ Named Requirements: EmplaceConstructible. If memory allocation was successful,
   *            allocation of size: \p i_list.end() - \p i_list.begin(). If memory allocation was unsuccessful or if
   *            the length of the initializer list is larger than max_size(), amsr::core::Abort() will be called.
   * \param[in] i_list     Initializer list to initialize the elements of the container with.
   * \param[in] allocator  Allocator to use for all memory allocations of this container.
   * \return    The vector.
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  static auto CreateVectorOrAbort(std::initializer_list<value_type> i_list,
                                  allocator_type const& allocator = allocator_type()) noexcept -> Vector {
    amsr::core::Result<Vector> result{CreateVector(i_list, allocator)};

    if (!result.HasValue()) {
      amsr::core::Abort(result.Error().Message().data());
    }
    return std::move(result.Value());
  }

  /*!
   * \brief     Constructs a vector with \p count default-inserted instances of T. No copies are made.
   * \details   Type T must meet C++ Named Requirements: DefaultInsertable. Allocation of size: \p count.
   * \param[in] count      The size of the container.
   * \param[in] allocator  Allocator to use for all memory allocations of this container.
   * \return    The vector on success, an error code otherwise.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \error     ContainerErrorCode::kLengthError if the requested size is larger than max_size().
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  static auto CreateVector(size_type count, allocator_type const& allocator = allocator_type())
      -> amsr::core::Result<Vector> {
    static_assert(std::is_default_constructible<value_type>::value,
                  "The element type must have a default constructor.");

    amsr::core::Result<Vector> result{Vector(allocator)};

    if (count > 0U) {
      Vector& new_vector{result.Value()};
      amsr::core::Result<void> const creation_result{new_vector.CreateEmptyStorage(count)};
      if (creation_result.HasValue()) {
        new_vector.FillWithDefault(new_vector.start_, new_vector.end_of_storage_);
        new_vector.finish_ = new_vector.end_of_storage_;
      } else {
        result.EmplaceError(creation_result.Error());
      }
    }

    return result;
  }

  /*!
   * \brief     Constructs a vector with \p count default-inserted instances of T. No copies are made.
   * \details   Type T must meet C++ Named Requirements: DefaultInsertable. If memory allocation was successful,
   *            allocation of size: \p count. If memory allocation was unsuccessful or if the requested size is larger
   *            than max_size(), amsr::core::Abort() will be called.
   * \param[in] count      The size of the container.
   * \param[in] allocator  Allocator to use for all memory allocations of this container.
   * \return    The vector.
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  static auto CreateVectorOrAbort(size_type count, allocator_type const& allocator = allocator_type()) noexcept
      -> Vector {
    amsr::core::Result<Vector> result{CreateVector(count, allocator)};

    if (!result.HasValue()) {
      amsr::core::Abort(result.Error().Message().data());
    }
    return std::move(result).Value();
  }

  /*!
   * \brief     Constructs a vector with \p count copies of elements with value \p value.
   * \details   Type T must meet C++ Named Requirements: CopyInsertable. Allocation of size: \p count.
   * \param[in] count      The size of the vector.
   * \param[in] value      The value to initialize elements with.
   * \param[in] allocator  Allocator to use for all memory allocations.
   * \return    The vector on success, an error code otherwise.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \error     ContainerErrorCode::kLengthError if the requested size is larger than max_size().
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  static auto CreateVector(size_type count, value_type const& value, allocator_type const& allocator = allocator_type())
      -> amsr::core::Result<Vector> {
    amsr::core::Result<Vector> result{Vector(allocator)};
    Vector& new_vector{result.Value()};

    amsr::core::Result<pointer> const append_result{new_vector.AppendGap(count)};
    if (append_result.HasValue()) {
      new_vector.FillWithCopies(new_vector.start_, new_vector.finish_, value);
    } else {
      result.EmplaceError(append_result.Error());
    }
    return result;
  }

  /*!
   * \brief     Constructs a vector with \p count copies of elements with value \p value.
   * \details   Type T must meet C++ Named Requirements: CopyInsertable. If memory allocation was successful,
   *            allocation of size: \p count. If memory allocation was unsuccessful or if the requested size is larger
   *            than max_size(), amsr::core::Abort() will be called.
   * \param[in] count      The size of the vector.
   * \param[in] value      The value to initialize elements with.
   * \param[in] allocator  Allocator to use for all memory allocations.
   * \return    The vector.
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  static auto CreateVectorOrAbort(size_type count, value_type const& value,
                                  allocator_type const& allocator = allocator_type()) noexcept -> Vector {
    amsr::core::Result<Vector> result{CreateVector(count, value, allocator)};

    if (!result.HasValue()) {
      amsr::core::Abort(result.Error().Message().data());
    }
    return std::move(result).Value();
  }

  /*!
   * \brief     Constructs the container with the contents of the multi-pass range [\p first, \p last).
   * \details   Type T must meet C++ Named Requirements: EmplaceConstructible. Type InputIterator must meet C++ Named
   *            Requirements: LegacyForwardIterator. Allocation of size: std::distance( \p first, \p last ).
   * \tparam    InputIterator  Type of the input iterator.
   * \param[in] first      The beginning of the range to copy the elements from.
   * \param[in] last       The end of the range to copy the elements from. Must be reachable from \p first by
   *                       incrementation.
   * \param[in] allocator  Allocator to use for all memory allocations of this container.
   * \return    The vector on success, an error code otherwise.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \error     ContainerErrorCode::kLengthError if the size derived from \p first and \p last is larger than
   *            max_size().
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  // VECTOR NC AutosarC++17_10-A15.4.4: MD_VAC_A15.4.4_explicitNoexceptSpecificationMissing
  template <typename InputIterator,
            std::enable_if_t<std::is_convertible<typename std::iterator_traits<InputIterator>::iterator_category,
                                                 std::forward_iterator_tag>::value,
                             bool> = true>
  static auto CreateVector(InputIterator first, InputIterator last, allocator_type const& allocator = allocator_type())
      -> amsr::core::Result<Vector> {
    amsr::core::Result<Vector> result{Vector(allocator)};
    Vector& new_vector{result.Value()};

    // VECTOR NL AutosarC++17_10-M5.0.9: MD_VAC_M5.0.9_explicitConversionChangesSignedness
    size_type const count{static_cast<size_type>(std::distance(first, last))};
    amsr::core::Result<pointer> const append_result{new_vector.AppendGap(count)};
    if (append_result.HasValue()) {
      static_cast<void>(FillFromIteratorByCopy(new_vector.start_, first, last));
    } else {
      result.EmplaceError(append_result.Error());
    }

    return result;
  }

  /*!
   * \brief     Constructs the container with the contents of the single-pass range [\p first, \p last).
   * \details   Type T must meet C++ Named Requirements: EmplaceConstructible. Type InputIterator must meet C++ Named
   *            Requirements: InputIterator. The capacity of the resulting Vector might be larger than the number of
   *            elements in the input range.
   * \tparam    InputIterator  Type of the input iterator.
   * \param[in] first      The beginning of the range to copy the elements from.
   * \param[in] last       The end of the range to copy the elements from. Must be reachable from \p first by
   *                       incrementation.
   * \param[in] allocator  Allocator to use for all memory allocations of this container.
   * \return    The vector on success, an error code otherwise.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \error     ContainerErrorCode::kLengthError if the size derived from \p first and \p last is larger than
   *            max_size().
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  // VECTOR NC AutosarC++17_10-A15.4.4: MD_VAC_A15.4.4_explicitNoexceptSpecificationMissing
  template <typename InputIterator,
            std::enable_if_t<std::is_same<typename std::iterator_traits<InputIterator>::iterator_category,
                                          std::input_iterator_tag>::value,
                             bool> = true>
  static auto CreateVector(InputIterator first, InputIterator last, allocator_type const& allocator = allocator_type())
      -> amsr::core::Result<Vector> {
    amsr::core::Result<Vector> result{Vector(allocator)};
    Vector& new_vector{result.Value()};

    while (first != last) {
      amsr::core::Result<void> const push_back_result{new_vector.PushBack(*first)};  // VCA_VAC_VALID_ARGUMENT
      if (!push_back_result.HasValue()) {
        result.EmplaceError(push_back_result.Error());
        break;
      }
      ++first;
    }

    return result;
  }

  /*!
   * \brief     Constructs the container with the contents of the range [\p first, \p last).
   * \details   Type T must meet C++ Named Requirements: EmplaceConstructible. If memory allocation was successful,
   *            allocation of size: std::distance( \p first, \p last ). If memory allocation was unsuccessful or if the
   *            size derived from \p first and \p last is larger than max_size(), amsr::core::Abort() will be called.
   * \tparam    InputIterator  Type of the input iterator.
   * \param[in] first      The beginning of the range to copy the elements from.
   * \param[in] last       The end of the range to copy the elements from. Must be reachable from \p first by
   *                       incrementation.
   * \param[in] allocator  Allocator to use for all memory allocations of this container.
   * \return    The vector.
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  template <class InputIterator,
            std::enable_if_t<std::is_convertible<typename std::iterator_traits<InputIterator>::iterator_category,
                                                 std::input_iterator_tag>::value,
                             bool> = true>
  static auto CreateVectorOrAbort(InputIterator first, InputIterator last,
                                  allocator_type const& allocator = allocator_type()) noexcept -> Vector {
    amsr::core::Result<Vector> result{CreateVector(first, last, allocator)};

    if (!result.HasValue()) {
      amsr::core::Abort(result.Error().Message().data());
    }
    return std::move(result.Value());
  }

  /*!
   * \brief   Destructs the Vector.
   * \details The destructors of the elements are called and the used storage is deallocated. Note that if the elements
   *          are pointers, the pointed-to objects are not destroyed.
   * \pre     -
   * \spec    requires true; \endspec
   * \context ANY
   * \steady  TRUE
   */
  ~Vector() {      // VCA_VAC_VALID_NONSTATIC_METHOD
    clear();       // VCA_VAC_VALID_NONSTATIC_METHOD
    Deallocate();  // VCA_VAC_VALID_NONSTATIC_METHOD
  }                // VCA_VAC_VALID_NONSTATIC_METHOD

  /*!
   * \brief Deleted as possible errors cannot be reported. Assign(Vector const& other) shall be used instead.
   */
  auto operator=(Vector const&) -> Vector& = delete;

  /*!
   * \brief Deleted as possible errors cannot be reported. Assign(std::initializer_list<value_type> i_list) shall be
   *        used instead.
   */
  auto operator=(std::initializer_list<value_type>) -> Vector& = delete;

  /*!
   * \brief     Replaces the contents with those of \p other using move semantics.
   * \details   Might cause an error which results in termination, if the allocator is not propagated and does not
   *            always compare equal and if the allocate() or deallocate() methods of the allocator are not noexcept.
   *            If the allocator is propagated or compares equal to the allocator of \p other, then \p other after the
   *            move is guaranteed to be equal to this vector before the move. Otherwise \p other is cleared with
   *            clear(). Type T must meet C++ Named Requirements: MoveInsertable, MoveAssignable.
   * \param[in] other  Another container to use as data source.
   * \return    A reference to this vector after assigning.
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  auto operator=(Vector&& other) & noexcept(
      allocator_traits::propagate_on_container_move_assignment::value || allocator_traits::is_always_equal::value ||
      (noexcept(std::declval<Alloc>().allocate(1)) && noexcept(std::declval<Alloc>().deallocate(nullptr, 1))))
      -> Vector& {
    constexpr bool const kMoveStorage{allocator_traits::propagate_on_container_move_assignment::value ||
                                      allocator_traits::is_always_equal::value};
    // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
    MoveAssign(std::move(other), false, std::integral_constant<bool, kMoveStorage>());
    // While this is not a loop, as the suppression suggests, it most likely comes from the same bug.
    // VECTOR NL AutosarC++17_10-A12.8.3: MD_VAC_A12.8.3_movedFromObjectReadAccessedInLoop
    return *this;  // VCA_VAC_VALID_RETURN_VALUE
  }

  /*!
   * \brief     Replaces the contents with a copy of the contents of \p other.
   * \details   Type T must meet C++ Named Requirements: CopyInsertable.
   * \param[in] other  Another container to use as data source.
   * \return    An empty result on success, an error code otherwise.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  auto Assign(Vector const& other) -> amsr::core::Result<void> {
    amsr::core::Result<void> result{};

    CopyAllocatorIfNeeded(other.get_allocator(), typename allocator_traits::propagate_on_container_copy_assignment());

    if (std::addressof(other) != this) {
      result = Assign(other.cbegin(), other.cend());
    }
    return result;
  }

  /*!
   * \brief     Replaces the contents with a copy of the contents of \p other.
   * \details   Type T must meet C++ Named Requirements: CopyInsertable. If memory allocation was unsuccessful,
   *            amsr::core::Abort() will be called.
   * \param[in] other  Another container to use as data source.
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  void AssignOrAbort(Vector const& other) noexcept {
    amsr::core::Result<void> const result{Assign(other)};

    if (!result.HasValue()) {
      amsr::core::Abort(result.Error().Message().data());
    }
  }

  // VECTOR Next Construct AutosarC++17_10-A15.4.2: MD_VAC_A15.4.2_exceptionViolatesFunctionsNoexeceptSpec
  // VECTOR Next Construct AutosarC++17_10-A15.5.3: MD_VAC_A15.5.3_exceptionViolatesFunctionsNoexeceptSpec
  /*!
   * \brief     Replaces the contents with copies of those in the multi-pass range [\p first, \p last).
   * \details   The behavior is undefined if either argument is an iterator into *this. Type T must meet C++ Named
   *            Requirements: EmplaceConstructible and assignable from \p *first. Type InputIterator must meet C++
   *            Named Requirements: LegacyForwardIterator.
   * \tparam    InputIterator  Type of the input iterator.
   * \param[in] first  Beginning of the range to copy the elements from.
   * \param[in] last   End of the range to copy the elements from. Must be reachable from \p first by incrementation.
   * \return    An empty result on success, an error code otherwise.
   * \error     ContainerErrorCode::kAllocationError if memory could not be allocated.
   * \error     ContainerErrorCode::kLengthError if the size derived from \p first and \p last is larger than
   *            max_size().
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  // VECTOR NC AutosarC++17_10-A15.4.4: MD_VAC_A15.4.4_explicitNoexceptSpecificationMissing
  template <typename InputIterator,
            std::enable_if_t<std::is_convertible<typename std::iterator_traits<InputIterator>::iterator_category,
                                                 std::forward_iterator_tag>::value,
                             bool> = true>
  auto Assign(InputIterator first, InputIterator last) -> amsr::core::Result<void> {
    static_assert(std::is_copy_assignable<value_type>::value, "");

    amsr::core::Result<void> result{};
    size_type const input_size{static_cast<size_type>(std::distance(first, last))};
    if (input_size > capacity()) {
      if (input_size <= max_size()) {
        pointer const new_start{allocator_.allocate(input_size)};
        if (new_start != nullptr) {
          pointer const new_finish{FillFromIteratorByCopy(new_start, first, last)};
          clear();
          Deallocate();
          start_ = new_start;
          finish_ = new_finish;
          // VECTOR NC AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_pointerArithmetic
          // VECTOR NL AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmetic
          end_of_storage_ = new_start + input_size;
        } else {
          result.EmplaceError(amsr::ContainerErrorCode::kAllocationError);
        }
      } else {
        result.EmplaceError(amsr::ContainerErrorCode::kLengthError);
      }
    } else if (input_size <= size()) {
      pointer const new_finish{std::copy(first, last, start_)};  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
      EraseToEnd(new_finish);
    } else {  // size() < input_size <= capacity()
      InputIterator const current{
          std::next(first, static_cast<typename std::iterator_traits<InputIterator>::difference_type>(size()))};
      static_cast<void>(std::copy(first, current, start_));  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
      finish_ = FillFromIteratorByCopy(finish_, current, last);
    }
    return result;
  }

  // VECTOR Next Construct AutosarC++17_10-A15.4.2: MD_VAC_A15.4.2_exceptionViolatesFunctionsNoexeceptSpec
  // VECTOR Next Construct AutosarC++17_10-A15.5.3: MD_VAC_A15.5.3_exceptionViolatesFunctionsNoexeceptSpec
  /*!
   * \brief     Replaces the contents with copies of those in the single-pass range [\p first, \p last).
   * \details   The behavior is undefined if either argument is an iterator into *this. Type T must meet C++ Named
   *            Requirements: EmplaceConstructible and assignable from \p *first. Type InputIterator must meet C++
   *            Named Requirements: InputIterator.
   * \tparam    InputIterator  Type of the input iterator.
   * \param[in] first  Beginning of the range to copy the elements from.
   * \param[in] last   End of the range to copy the elements from. Must be reachable from \p first by incrementation.
   * \return    An empty result on success, an error code otherwise.
   * \error     ContainerErrorCode::kAllocationError if memory could not be allocated.
   * \error     ContainerErrorCode::kLengthError if the size derived from \p first and \p last is larger than
   *            max_size().
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  // VECTOR NC AutosarC++17_10-A15.4.4: MD_VAC_A15.4.4_explicitNoexceptSpecificationMissing
  template <typename InputIterator,
            std::enable_if_t<std::is_same<typename std::iterator_traits<InputIterator>::iterator_category,
                                          std::input_iterator_tag>::value,
                             bool> = true>
  auto Assign(InputIterator first, InputIterator last) -> amsr::core::Result<void> {
    static_assert(std::is_copy_assignable<value_type>::value, "");

    amsr::core::Result<void> result{};

    pointer current{start_};

    // As long as the input elements fit into the range [start_, finish_), copy them over.
    while ((first != last) && (current != finish_)) {
      *current = *first;  // VCA_VAC_VALID_NONSTATIC_METHOD
      ++first;
      ++current;
    }

    if (first == last) {
      // The input iterator is exhausted -> delete remaining content.
      EraseToEnd(current);
    } else {
      // The input iterator contains more elements than size() -> construct copies of them inside the range [finish_,
      // end_of_storage_).
      while ((first != last) && (current != end_of_storage_)) {
        ConstructElementFromArgs(current, *first);  // VCA_VAC_VALID_ARGUMENT
        ++first;
        ++current;
      }
      finish_ = current;

      // The input iterator contains even more elements than capacity() -> append them one by one to this vector (and
      // increase capacity, when necessary).
      while (first != last) {
        amsr::core::Result<void> const push_back_result{PushBack(*first)};  // VCA_VAC_VALID_ARGUMENT
        if (!push_back_result.HasValue()) {
          result.EmplaceError(push_back_result.Error());
          break;
        }
        ++first;
      }
    }

    return result;
  }

  /*!
   * \brief     Replaces the contents with copies of those in the range [\p first, \p last).
   * \details   The behavior is undefined if either argument is an iterator into *this. Type T must meet C++ Named
   *            Requirements: EmplaceConstructible and assignable from \p *first. If memory allocation was unsuccessful
   *            or if the size derived from \p first and \p last is larger than max_size(), amsr::core::Abort() will be
   *            called.
   * \tparam    InputIterator  Type of the input iterator.
   * \param[in] first  Beginning of the range to copy the elements from.
   * \param[in] last   End of the range to copy the elements from. Must be reachable from \p first by incrementation.
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  template <class InputIterator,
            std::enable_if_t<std::is_convertible<typename std::iterator_traits<InputIterator>::iterator_category,
                                                 std::input_iterator_tag>::value,
                             bool> = true>
  void AssignOrAbort(InputIterator first, InputIterator last) noexcept {
    amsr::core::Result<void> const result{Assign(first, last)};

    if (!result.HasValue()) {
      amsr::core::Abort(result.Error().Message().data());
    }
  }

  /*!
   * \brief     Replaces the contents with the elements in the initializer list \p i_list.
   * \details   Type T must meet C++ Named Requirements: EmplaceConstructible.
   * \param[in] i_list  Initializer list to copy the values from.
   * \return    An empty result on success, an error code otherwise.
   * \error     ContainerErrorCode::kAllocationError if memory could not be allocated.
   * \error     ContainerErrorCode::kLengthError if the requested size is larger than max_size().
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  auto Assign(std::initializer_list<value_type> i_list) -> amsr::core::Result<void> {
    return Assign(i_list.begin(), i_list.end());
  }

  /*!
   * \brief     Replaces the contents with the elements in the initializer list \p i_list.
   * \details   Type T must meet C++ Named Requirements: EmplaceConstructible. If memory allocation was unsuccessful or
   *            if the requested size is larger than max_size(), amsr::core::Abort() will be called.
   * \param[in] i_list  Initializer list to copy the values from.
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  void AssignOrAbort(std::initializer_list<value_type> i_list) noexcept {
    amsr::core::Result<void> const result{Assign(i_list)};

    if (!result.HasValue()) {
      amsr::core::Abort(result.Error().Message().data());
    }
  }

  // VECTOR Next Construct AutosarC++17_10-A15.4.2: MD_VAC_A15.4.2_exceptionViolatesFunctionsNoexeceptSpec
  // VECTOR Next Construct AutosarC++17_10-A15.5.3: MD_VAC_A15.5.3_exceptionViolatesFunctionsNoexeceptSpec
  /*!
   * \brief     Replaces the contents with \p count copies of value \p value.
   * \details   Type T must meet C++ Named Requirements: CopyInsertable, CopyAssignable.
   * \param[in] count  The new size of the container.
   * \param[in] value  The value to initialize elements of the container with.
   * \return    An empty result on success, an error code otherwise.
   * \error     ContainerErrorCode::kAllocationError if memory could not be allocated.
   * \error     ContainerErrorCode::kLengthError if the requested size is larger than max_size().
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  auto Assign(size_type count, value_type const& value) -> amsr::core::Result<void> {
    static_assert(std::is_copy_assignable<value_type>::value, "");
    static_assert(std::is_copy_constructible<value_type>::value, "");

    amsr::core::Result<void> result{};
    size_type const current_size{size()};
    if (count > capacity()) {
      amsr::core::Result<Vector> tmp{CreateVector(count, value, get_allocator())};
      if (tmp.HasValue()) {
        tmp.Value().swap(*this);
      } else {
        result.EmplaceError(tmp.Error());
      }
    } else if (count > current_size) {
      std::fill(begin(), end(), value);  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
      // VECTOR NL AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmetic
      pointer const new_finish{start_ + count};
      FillWithCopies(finish_, new_finish, value);
      finish_ = new_finish;
    } else {
      EraseToEnd(std::fill_n(start_, count, value));  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    }
    return result;
  }

  /*!
   * \brief     Replaces the contents with \p count copies of value \p value
   * \details   Type T must meet C++ Named Requirements: CopyInsertable, CopyAssignable. If memory allocation was
   *            unsuccessful or if the requested size is larger than max_size(), amsr::core::Abort() will be called.
   * \param[in] count  The new size of the container.
   * \param[in] value  The value to initialize elements of the container with.
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  void AssignOrAbort(size_type count, value_type const& value) noexcept {
    amsr::core::Result<void> const result{Assign(count, value)};

    if (!result.HasValue()) {
      amsr::core::Abort(result.Error().Message().data());
    }
  }

  /*!
   * \brief   Gets the allocator.
   * \return  The allocator.
   * \pre     -
   * \spec    requires true; \endspec
   * \context ANY
   * \steady  TRUE
   */
  auto get_allocator() const noexcept -> allocator_type {
    return allocator_;  // VCA_VAC_VALID_ARGUMENT
  }

  /*!
   * \brief     Returns a reference to the element at specified location \p pos, with bounds checking.
   * \param[in] pos  Position of the element to return.
   * \return    Result containing either a reference to the requested element or an error code.
   * \error     ContainerErrorCode::kOutOfRangeError, if \p pos is not within the range of the vector.
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  auto At(size_type pos) noexcept -> amsr::core::Result<reference> {
    amsr::core::Result<reference> result{amsr::ContainerErrorCode::kOutOfRangeError};
    if (pos < size()) {
      // VECTOR NL AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_arrayIndexing
      result.EmplaceValue(start_[pos]);  // VCA_VAC_VALID_ARGUMENT
    }

    return result;
  }

  /*!
   * \brief     Returns a reference to the element at specified location \p pos, with bounds checking.
   * \details   If \p pos is not within the range of the vector, amsr::core::Abort() will be called.
   * \param[in] pos  Position of the element to return.
   * \return    A reference to the requested element.
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  auto AtOrAbort(size_type pos) noexcept -> reference {
    amsr::core::Result<reference> result{At(pos)};
    if (!result.HasValue()) {
      amsr::core::Abort(result.Error().Message().data());
    }
    return result.Value();  // VCA_VAC_VALID_RETURN_VALUE
  }

  /*!
   * \brief     Returns a const reference to the element at specified location \p pos, with bounds checking.
   * \param[in] pos  Position of the element to return.
   * \return    Result containing either a const reference to the requested element or an error code.
   * \error     ContainerErrorCode::kOutOfRangeError, if \p pos is not within the range of the vector.
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  auto At(size_type pos) const noexcept -> amsr::core::Result<const_reference> {
    amsr::core::Result<const_reference> result{amsr::ContainerErrorCode::kOutOfRangeError};
    if (pos < size()) {
      // VECTOR NL AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_arrayIndexing
      result.EmplaceValue(start_[pos]);  // VCA_VAC_VALID_ARGUMENT
    }

    return result;
  }

  /*!
   * \brief     Returns a const reference to the element at specified location \p pos, with bounds checking.
   * \details   If \p pos is not within the range of the vector, amsr::core::Abort() will be called.
   * \param[in] pos  Position of the element to return.
   * \return    A const reference to the requested element.
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  auto AtOrAbort(size_type pos) const noexcept -> const_reference {
    amsr::core::Result<const_reference> result{At(pos)};
    if (!result.HasValue()) {
      amsr::core::Abort(result.Error().Message().data());
    }
    return result.Value();  // VCA_VAC_VALID_RETURN_VALUE
  }

  /*!
   * \brief     Returns a reference to the element at specified location \p pos, without bounds checking.
   * \details   Accessing a nonexistent element is undefined behavior.
   * \param[in] pos  Position of the element to return. Must be in the range [0, size()).
   * \return    A reference to the requested element.
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  auto operator[](size_type const pos) noexcept -> reference {
    // VECTOR NL AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_arrayIndexing
    return start_[pos];
  }

  /*!
   * \brief     Returns a const_reference to the element at specified location \p pos, without bounds checking.
   * \details   Accessing a nonexistent element is undefined behavior.
   * \param[in] pos  Position of the element to return. Must be in the range [0, size()).
   * \return    A const_reference to the requested element.
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  auto operator[](size_type const pos) const noexcept -> const_reference {
    // VECTOR NL AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_arrayIndexing
    return start_[pos];
  }

  /*!
   * \brief   Gets a reference to the first element.
   * \return  A reference to the first element.
   * \pre     This vector must not be empty.
   * \context ANY
   * \steady  TRUE
   */
  auto front() noexcept -> reference { return *start_; }

  /*!
   * \brief   Gets a const_reference to the first element.
   * \return  A const_reference to the first element.
   * \pre     This vector must not be empty.
   * \context ANY
   * \steady  TRUE
   */
  auto front() const noexcept -> const_reference { return *start_; }

  /*!
   * \brief   Gets a reference to the last element.
   * \return  A reference to the last element.
   * \pre     This vector must not be empty.
   * \context ANY
   * \steady  TRUE
   */
  auto back() noexcept -> reference {
    // VECTOR NL AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmetic
    return *(finish_ - 1);
  }

  /*!
   * \brief   Gets a const_reference to the last element.
   * \return  A const_reference to the last element.
   * \pre     This vector must not be empty.
   * \context ANY
   * \steady  TRUE
   */
  auto back() const noexcept -> const_reference {
    // VECTOR NL AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmetic
    return *(finish_ - 1);
  }

  /*!
   * \brief   Returns a pointer to the underlying array serving as element storage.
   * \return  A pointer to the underlying element storage. For empty vectors, it may or may not return a null pointer.
   *          Otherwise returns the pointer to the first element.
   * \pre     -
   * \spec    requires true; \endspec
   * \context ANY
   * \steady  TRUE
   */
  auto data() noexcept -> value_type* { return start_; }

  /*!
   * \brief   Returns a const pointer to the underlying array serving as element storage.
   * \return  A const pointer to the underlying element storage. For empty vectors, it may or may not return a null
   *          pointer. Otherwise returns the pointer to the first element.
   * \pre     -
   * \spec    requires true; \endspec
   * \context ANY
   * \steady  TRUE
   */
  auto data() const noexcept -> value_type const* { return static_cast<value_type const*>(start_); }

  /*!
   * \brief   Gets an iterator to the first element. If the vector is empty, the returned iterator is equal to the one
   *          returned by end().
   * \return  The iterator to the first element.
   * \pre     -
   * \spec    requires true; \endspec
   * \context ANY
   * \steady  TRUE
   */
  auto begin() noexcept -> iterator { return start_; }

  /*!
   * \brief   Gets a const_iterator to the first element. If the vector is empty, the returned iterator is equal to the
   *          one returned by end().
   * \return  The const_iterator to the first element.
   * \pre     -
   * \spec    requires true; \endspec
   * \context ANY
   * \steady  TRUE
   */
  auto begin() const noexcept -> const_iterator { return const_iterator(start_); }

  /*!
   * \brief   Gets an iterator to the element after the last element in the vector.
   * \details Trying to dereference the returned iterator results in undefined behavior.
   * \return  The iterator to the element after the last element in the vector.
   * \pre     -
   * \spec    requires true; \endspec
   * \context ANY
   * \steady  TRUE
   */
  auto end() noexcept -> iterator { return finish_; }

  /*!
   * \brief   Gets a const_iterator to the element after the last element in the vector.
   * \details Trying to dereference the returned iterator results in undefined behavior.
   * \return  The const_iterator to the element after the last element in the vector.
   * \pre     -
   * \spec    requires true; \endspec
   * \context ANY
   * \steady  TRUE
   */
  auto end() const noexcept -> const_iterator { return const_iterator(finish_); }

  /*!
   * \brief   Gets a const_iterator to the first element. If the vector is empty, the returned iterator is equal to the
   *          one returned by cend().
   * \return  The const_iterator to the first element.
   * \pre     -
   * \spec    requires true; \endspec
   * \context ANY
   * \steady  TRUE
   */
  auto cbegin() const noexcept -> const_iterator { return const_iterator(start_); }

  /*!
   * \brief   Gets a const_iterator to the element after the last element in the vector.
   * \details Trying to dereference the returned iterator results in undefined behavior.
   * \return  The const_iterator to the element after the last element in the vector.
   * \pre     -
   * \spec    requires true; \endspec
   * \context ANY
   * \steady  TRUE
   */
  auto cend() const noexcept -> const_iterator { return const_iterator(finish_); }

  /*!
   * \brief   Gets a reverse_iterator to the last element. If the vector is empty, the returned iterator is equal to
   *          the one returned by rend().
   * \return  The reverse_iterator to the last element.
   * \pre     -
   * \spec    requires true; \endspec
   * \context ANY
   * \steady  TRUE
   */
  auto rbegin() noexcept -> reverse_iterator { return reverse_iterator(finish_); }

  /*!
   * \brief   Gets a const_reverse_iterator to the last element. If the vector is empty, the returned iterator is equal
   *          to the one returned by rend().
   * \return  The const_reverse_iterator to the last element.
   * \pre     -
   * \spec    requires true; \endspec
   * \context ANY
   * \steady  TRUE
   */
  auto rbegin() const noexcept -> const_reverse_iterator { return const_reverse_iterator(finish_); }

  /*!
   * \brief   Gets a reverse_iterator to the element before the first element in the vector.
   * \details Trying to dereference the returned iterator results in undefined behavior.
   * \return  The reverse_iterator to the element before the first element in the vector.
   * \pre     -
   * \spec    requires true; \endspec
   * \context ANY
   * \steady  TRUE
   */
  auto rend() noexcept -> reverse_iterator { return reverse_iterator(start_); }

  /*!
   * \brief   Gets a const_reverse_iterator to the element before the first element in the vector.
   * \details Trying to dereference the returned iterator results in undefined behavior.
   * \return  The const_reverse_iterator to the element before the first element in the vector.
   * \pre     -
   * \spec    requires true; \endspec
   * \context ANY
   * \steady  TRUE
   */
  auto rend() const noexcept -> const_reverse_iterator { return const_reverse_iterator(start_); }

  /*!
   * \brief   Gets a const_reverse_iterator to the last element. If the vector is empty, the returned iterator is equal
   *          to the one returned by crend().
   * \return  The const_reverse_iterator to the last element.
   * \pre     -
   * \spec    requires true; \endspec
   * \context ANY
   * \steady  TRUE
   */
  auto crbegin() const noexcept -> const_reverse_iterator { return const_reverse_iterator(finish_); }

  /*!
   * \brief   Gets a const_reverse_iterator to the element before the first element in the vector.
   * \details Trying to dereference the returned iterator results in undefined behavior.
   * \return  The const_reverse_iterator to the element before the first element in the vector.
   * \pre     -
   * \spec    requires true; \endspec
   * \context ANY
   * \steady  TRUE
   */
  auto crend() const noexcept -> const_reverse_iterator { return const_reverse_iterator(start_); }

  /*!
   * \brief   Returns the number of elements in the container.
   * \return  The number of elements in the container.
   * \pre     -
   * \spec    requires true; \endspec
   * \context ANY
   * \steady  TRUE
   */
  auto size() const noexcept -> size_type {
    // VECTOR NL AutosarC++17_10-M5.0.9: MD_VAC_M5.0.9_explicitConversionChangesSignedness
    return static_cast<size_type>(finish_ - start_);
  }

  /*!
   * \brief   Checks if the vector has no elements.
   * \return  True if the vector contains no elements, false otherwise.
   * \pre     -
   * \spec    requires true; \endspec
   * \context ANY
   * \steady  TRUE
   */
  auto empty() const noexcept -> bool { return start_ == finish_; }

  /*!
   * \brief   Gets the maximum size for allocation.
   * \return  The maximum size for allocation.
   * \pre     -
   * \spec    requires true; \endspec
   * \context ANY
   * \steady  TRUE
   */
  auto max_size() const noexcept -> size_type {
    size_t const diffmax{static_cast<size_t>(std::numeric_limits<ptrdiff_t>::max()) / sizeof(value_type)};
    size_t const allocmax{allocator_traits::max_size(allocator_)};
    return std::min(diffmax, allocmax);
  }

  /*!
   * \brief   Returns the number of elements that the container has currently allocated space for.
   * \return  Capacity of the currently allocated storage.
   * \pre     -
   * \spec    requires true; \endspec
   * \context ANY
   * \steady  TRUE
   */
  auto capacity() const noexcept -> size_type {
    // VECTOR NL AutosarC++17_10-M5.0.9: MD_VAC_M5.0.9_explicitConversionChangesSignedness
    return static_cast<size_type>(end_of_storage_ - start_);
  }

  /*!
   * \brief Increase the capacity of the vector (the total number of elements that the vector can hold without requiring
   * reallocation) to a value that's greater or equal to \p new_capacity.
   *
   * \details If \p new_capacity is greater than the current capacity(), new storage is allocated, otherwise the
   * function does nothing.
   *
   * Reserve() does not change the size of the vector.
   *
   * If \p new_capacity is greater than capacity(), all iterators, including the end() iterator, and all references to
   * the elements are invalidated. Otherwise, no iterators or references are invalidated.
   *
   * Type T must meet C++ Named Requirements: MoveInsertable.
   *
   * If \p new_capacity <= capacity().
   * No allocation.
   *
   * If \p new_capacity > capacity() && empty() && \p new_capacity <= max_size().
   * Allocation of size: \p new_capacity.
   *
   * If \p new_capacity > max_size().
   * No allocation.
   *
   * Else
   * Allocation of not specified size
   *
   * \param[in] new_capacity New capacity of the vector, in number of elements.
   * \return An empty result on success, an error code otherwise.
   * \error ContainerErrorCode::kLengthError if \p new_capacity > max_size().
   * \error ContainerErrorCode::kAllocationError if memory for the new location cannot be allocated.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \steady TRUE
   */
  auto Reserve(size_type new_capacity) -> amsr::core::Result<void> {
    amsr::core::Result<void> result{};

    if (new_capacity > capacity()) {
      result = Relocate(new_capacity - capacity());
    }

    return result;
  }

  /*!
   * \brief Increase the capacity of the vector (the total number of elements that the vector can hold without requiring
   * reallocation) to a value that's greater or equal to \p new_capacity.
   *
   * \details If \p new_capacity is greater than the current capacity(), new storage is allocated, otherwise the
   * function does nothing.
   *
   * Reserve() does not change the size of the vector.
   *
   * If \p new_capacity is greater than capacity(), all iterators, including the end() iterator, and all references to
   * the elements are invalidated. Otherwise, no iterators or references are invalidated.
   *
   * After a call to Reserve(), insertions will not trigger reallocation unless the insertion would make the size of the
   * vector greater than the value of capacity().
   *
   * Type T must meet C++ Named Requirements: MoveInsertable.
   *
   * If \p new_capacity <= capacity().
   * No allocation.
   *
   * If \p new_capacity > capacity() && empty() && \p new_capacity <= max_size().
   * Allocation of size: \p new_capacity.
   *
   * If \p new_capacity > max_size() or if memory allocation for the new location was unsuccessful, amsr::core::Abort()
   * will be called and no allocation.
   *
   * Else
   * Allocation of not specified size
   *
   *
   * \param[in] new_capacity New capacity of the vector, in number of elements.
   * \context ANY
   * \pre -
   * \spec requires true; \endspec
   * \steady TRUE
   */
  void ReserveOrAbort(size_type new_capacity) noexcept {
    amsr::core::Result<void> const result{Reserve(new_capacity)};

    if (!result.HasValue()) {
      amsr::core::Abort(result.Error().Message().data());
    }
  }

  /*!
   * \brief   Reduces the capacity to the current size().
   * \details If the capacity is already equal to the current size, nothing happens. Type T must meet C++ Named
   *          Requirements: MoveInsertable.
   * \return  An empty result on success, an error code otherwise.
   * \error   ContainerErrorCode::kAllocationError if memory for the new location cannot be allocated.
   * \pre     -
   * \spec    requires true; \endspec
   * \context ANY
   * \steady  TRUE
   */
  auto ShrinkToFit() -> amsr::core::Result<void> {
    static_assert(std::is_destructible<value_type>::value, "");

    amsr::core::Result<void> result{};

    size_type const new_capacity{size()};

    if (capacity() > new_capacity) {
      if (new_capacity > 0U) {
        pointer const new_start{allocator_.allocate(new_capacity)};
        if (new_start != nullptr) {
          // VECTOR NC AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_pointerArithmetic
          // VECTOR NL AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmetic
          pointer const new_finish{new_start + new_capacity};
          pointer new_current{new_start};
          pointer old_current{start_};
          while (new_current < new_finish) {
            ConstructElement(new_current, old_current, HasMoveCtorT{});
            DestroyElement(old_current, IsPolyAllocT{});
            ++old_current;  // VECTOR SL AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_arrayAccessOutOfBounds
            ++new_current;  // VECTOR SL AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_pointerArithmetic
          }

          Deallocate();

          start_ = new_start;
          finish_ = new_finish;
          end_of_storage_ = new_finish;
        } else {
          result.EmplaceError(amsr::ContainerErrorCode::kAllocationError);
        }
      } else {
        Deallocate();
        ResetStorage();
      }
    }

    return result;
  }

  /*!
   * \brief   Reduces the capacity to the current size().
   * \details If the capacity is already equal to the current size, nothing happens. Type T must meet C++ Named
   *          Requirements: MoveInsertable. If memory allocation for the new location was unsuccessful,
   *          amsr::core::Abort() will be called.
   * \pre     -
   * \spec    requires true; \endspec
   * \context ANY
   * \steady  TRUE
   */
  void ShrinkToFitOrAbort() noexcept {
    amsr::core::Result<void> const result{ShrinkToFit()};

    if (!result.HasValue()) {
      amsr::core::Abort(result.Error().Message().data());
    }
  }

  /*!
   * \brief     Resizes the vector to contain \p new_size elements.
   * \details   If \p new_size is smaller than the current size(), elements are removed from the back. If \p new_size
   *            is larger, elements are appended by default-insertion. Type T must meet C++ Named Requirements:
   *            MoveInsertable, DefaultInsertable.
   * \param[in] new_size  The target size.
   * \return    An empty result on success, an error code otherwise.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \error     ContainerErrorCode::kLengthError if the requested size is larger than max_size().
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  auto Resize(size_type new_size) -> amsr::core::Result<void> {
    static_assert(std::is_default_constructible<value_type>::value,
                  "The element type must have a default constructor.");

    amsr::core::Result<void> result{};

    if (new_size > size()) {
      amsr::core::Result<pointer> const start_of_gap{AppendGap(new_size - size())};
      if (start_of_gap.HasValue()) {
        FillWithDefault(start_of_gap.Value(), finish_);
      } else {
        result.EmplaceError(start_of_gap.Error());
      }
    } else if (!PointersAreNull()) {
      // VECTOR NL AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmetic
      EraseToEnd(start_ + new_size);
    } else {
      // This path can only be reached if the start_ is nullptr and new_size is 0 -> do nothing.
    }

    return result;
  }

  /*!
   * \brief     Resizes the vector to contain \p new_size elements.
   * \details   If \p new_size is smaller than the current size(), elements are removed from the back. If \p new_size
   *            is larger, elements are appended by default-insertion. Type T must meet C++ Named Requirements:
   *            MoveInsertable, DefaultInsertable. If memory allocation was unsuccessful or the requested size is
   *            larger than max_size(), amsr::core::Abort() will be called.
   * \param[in] new_size  The target size.
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  void ResizeOrAbort(size_type new_size) noexcept {
    amsr::core::Result<void> const result{Resize(new_size)};

    if (!result.HasValue()) {
      amsr::core::Abort(result.Error().Message().data());
    }
  }

  /*!
   * \brief     Resizes the vector to contain \p new_size elements.
   * \details   If \p new_size is smaller than the current size(), elements are removed from the back. If \p new_size
   *            is larger, copies of \p value are appended. Type T must meet C++ Named Requirements: CopyInsertable.
   * \param[in] new_size  The target size.
   * \param[in] value     The value to append if the size is increased.
   * \return    An empty result on success, an error code otherwise.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \error     ContainerErrorCode::kLengthError if the requested size is larger than max_size().
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  auto Resize(size_type new_size, value_type const& value) -> amsr::core::Result<void> {
    amsr::core::Result<void> result{};

    if (new_size > size()) {
      amsr::core::Result<pointer> const start_of_gap{AppendGap(new_size - size())};
      if (start_of_gap.HasValue()) {
        FillWithCopies(start_of_gap.Value(), finish_, value);
      } else {
        result.EmplaceError(start_of_gap.Error());
      }
    } else if (!PointersAreNull()) {
      // VECTOR NL AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmetic
      EraseToEnd(start_ + new_size);
    } else {
      // This path can only be reached if the start_ is nullptr and new_size is 0 -> do nothing.
    }

    return result;
  }

  /*!
   * \brief     Resizes the vector to contain \p new_size elements.
   * \details   If \p new_size is smaller than the current size(), elements are removed from the back. If \p new_size
   *            is larger, copies of \p value are appended. Type T must meet C++ Named Requirements: CopyInsertable. If
   *            memory allocation was unsuccessful or the requested size is larger than max_size(), amsr::core::Abort()
   *            will be called.
   * \param[in] new_size  The target size.
   * \param[in] value     The value to append if the size is increased.
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  void ResizeOrAbort(size_type new_size, value_type const& value) noexcept {
    amsr::core::Result<void> const result{Resize(new_size, value)};

    if (!result.HasValue()) {
      amsr::core::Abort(result.Error().Message().data());
    }
  }

  /*!
   * \brief     Inserts an element at the specified location in the vector.
   * \details   Type T must meet C++ Named Requirements: CopyInsertable, CopyAssignable.
   * \param[in] pos    Iterator before which the element will be inserted. Must be in the range [begin(), end()].
   * \param[in] value  Element value to insert.
   * \return    On success an iterator pointing to the inserted value, an error code otherwise.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \error     ContainerErrorCode::kLengthError if the requested final size is larger than max_size().
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  auto Insert(const_iterator pos, value_type const& value) -> amsr::core::Result<iterator> {
    return Emplace(pos, value);
  }

  /*!
   * \brief     Inserts an element at the specified location in the vector.
   * \details   Type T must meet C++ Named Requirements: CopyInsertable, CopyAssignable. If memory allocation was
   *            unsuccessful or the requested final size is larger than max_size(), amsr::core::Abort() will be called.
   * \param[in] pos    Iterator before which the element will be inserted. Must be in the range [begin(), end()].
   * \param[in] value  Element value to insert.
   * \return    An iterator pointing to the inserted value.
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  auto InsertOrAbort(const_iterator pos, value_type const& value) noexcept -> iterator {
    iterator const iter{EmplaceOrAbort(pos, value)};
    return iter;
  }

  /*!
   * \brief     Move inserts an element at the specified location in the vector.
   * \details   Type T must meet C++ Named Requirements: MoveInsertable, MoveAssignable.
   * \param[in] pos    Iterator before which the element will be inserted. Must be in the range [begin(), end()].
   * \param[in] value  Element value to insert.
   * \return    On success an iterator pointing to the inserted value, an error code otherwise.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \error     ContainerErrorCode::kLengthError if the requested final size is larger than max_size().
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  auto Insert(const_iterator pos, value_type&& value) -> amsr::core::Result<iterator> {
    return Emplace(pos, std::move(value));
  }

  /*!
   * \brief     Move inserts an element at the specified location in the vector.
   * \details   Type T must meet C++ Named Requirements: MoveInsertable, MoveAssignable. If memory allocation was
   *            unsuccessful or the requested final size is larger than max_size(), amsr::core::Abort() will be called.
   * \param[in] pos    Iterator before which the element will be inserted. Must be in the range [begin(), end()].
   * \param[in] value  Element value to insert.
   * \return    An iterator pointing to the inserted value.
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  auto InsertOrAbort(const_iterator pos, value_type&& value) noexcept -> iterator {
    iterator const iter{EmplaceOrAbort(pos, std::move(value))};
    return iter;
  }

  /*!
   * \brief     Inserts \p count copies of \p value at the specified location in the vector.
   * \details   Type T must meet C++ Named Requirements: CopyInsertable, CopyAssignable.
   * \param[in] pos    Iterator before which the elements will be inserted. Must be in the range [begin(), end()].
   * \param[in] count  Number of elements to insert.
   * \param[in] value  Element value to insert.
   * \return    On success an iterator pointing to the first element inserted or \p pos if \p count == 0, an error code
   *            otherwise.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \error     ContainerErrorCode::kLengthError if the requested final size is larger than max_size().
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  auto Insert(const_iterator pos, size_type count, value_type const& value) -> amsr::core::Result<iterator> {
    amsr::core::Result<iterator> result{iterator{}};

    amsr::core::Result<pointer> start_of_gap{InsertGap(pos, count)};
    // At this point, pos may be invalid, because a relocation may have happened.
    if (!start_of_gap.HasValue()) {
      result.EmplaceError(start_of_gap.Error());
    } else {
      pointer const gap_position{start_of_gap.Value()};
      result.EmplaceValue(iterator(gap_position));
      // VECTOR NC AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmetic
      // VECTOR NL AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_pointerArithmetic
      FillWithCopies(gap_position, gap_position + count, value);
    }
    return result;
  }

  /*!
   * \brief     Inserts \p count copies of \p value at the specified location in the vector.
   * \details   Type T must meet C++ Named Requirements: CopyInsertable, CopyAssignable. If memory allocation was
   *            unsuccessful or the requested final size is larger than max_size(), amsr::core::Abort() will be called.
   * \param[in] pos    Iterator before which the elements will be inserted. Must be in the range [begin(), end()].
   * \param[in] count  Number of elements to insert.
   * \param[in] value  Element value to insert.
   * \return    An iterator pointing to the first element inserted or \p pos if \p count == 0.
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  auto InsertOrAbort(const_iterator pos, size_type count, value_type const& value) noexcept -> iterator {
    amsr::core::Result<iterator> result{Insert(pos, count, value)};
    if (!result.HasValue()) {
      amsr::core::Abort(result.Error().Message().data());
    }
    return result.Value();
  }

  /*!
   * \brief     Inserts copies of the elements from the multi-pass range [\p first, \p last) at the specified location
   *            in the vector.
   * \details   Type T must meet C++ Named Requirements: EmplaceConstructible. Type InputIterator must meet C++ Named
   *            Requirements: LegacyForwardIterator.
   * \tparam    InputIterator  Type of the input iterator.
   * \param[in] pos    Iterator before which the elements will be inserted. Must be in the range [begin(), end()].
   * \param[in] first  Beginning of the range of elements to insert. Must not be iterator into *this.
   * \param[in] last   Iterator after the last element to insert. Must not be iterator into *this. Must be reachable
   *                   from \p first by incrementation.
   * \return    On success an iterator pointing to the first element inserted or \p pos if \p first == \p last, an
   *            error code otherwise.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \error     ContainerErrorCode::kLengthError if the requested final size is larger than max_size().
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  // VECTOR NC AutosarC++17_10-A15.4.4: MD_VAC_A15.4.4_explicitNoexceptSpecificationMissing
  template <typename InputIterator,
            std::enable_if_t<std::is_convertible<typename std::iterator_traits<InputIterator>::iterator_category,
                                                 std::forward_iterator_tag>::value,
                             bool> = true>
  auto Insert(const_iterator pos, InputIterator first, InputIterator last) -> amsr::core::Result<iterator> {
    static_assert(std::is_copy_constructible<value_type>::value, "");

    amsr::core::Result<iterator> result{iterator{}};

    // VECTOR NL AutosarC++17_10-M5.0.9: MD_VAC_M5.0.9_explicitConversionChangesSignedness
    size_type const length{static_cast<size_type>(std::distance(first, last))};
    amsr::core::Result<pointer> start_of_gap{InsertGap(pos, length)};
    // At this point, pos may be invalid, because a relocation may have happened.
    if (!start_of_gap.HasValue()) {
      result.EmplaceError(start_of_gap.Error());
    } else {
      pointer const gap_position{start_of_gap.Value()};
      result.EmplaceValue(iterator(gap_position));
      static_cast<void>(FillFromIteratorByCopy(gap_position, first, last));
    }
    return result;
  }

  /*!
   * \brief     Inserts copies of the elements from the single-pass range [\p first, \p last) at the specified location
   *            in the vector.
   * \details   Type T must meet C++ Named Requirements: EmplaceConstructible. Type InputIterator must meet C++ Named
   *            Requirements: InputIterator.
   * \tparam    InputIterator  Type of the input iterator.
   * \param[in] pos    Iterator before which the elements will be inserted. Must be in the range [begin(), end()].
   * \param[in] first  Beginning of the range of elements to insert. Must not be iterator into *this.
   * \param[in] last   Iterator after the last element to insert. Must not be iterator into *this. Must be reachable
   *                   from \p first by incrementation.
   * \return    On success an iterator pointing to the first element inserted or \p pos if \p first == \p last, an
   *            error code otherwise.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \error     ContainerErrorCode::kLengthError if the requested final size is larger than max_size().
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  // VECTOR NC AutosarC++17_10-A15.4.4: MD_VAC_A15.4.4_explicitNoexceptSpecificationMissing
  template <typename InputIterator,
            std::enable_if_t<std::is_same<typename std::iterator_traits<InputIterator>::iterator_category,
                                          std::input_iterator_tag>::value,
                             bool> = true>
  auto Insert(const_iterator pos, InputIterator first, InputIterator last) -> amsr::core::Result<iterator> {
    static_assert(std::is_copy_constructible<value_type>::value, "");

    // Convert 'pos' from const_iterator to iterator.
    // VECTOR NL AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmeticAllocatorIterator
    amsr::core::Result<iterator> result{(pos == nullptr) ? nullptr : ((pos - cbegin()) + begin())};

    if (first != last) {
      if (pos == finish_) {
        difference_type const offset{pos - start_};
        while (first != last) {
          amsr::core::Result<void> const push_back_result{PushBack(*first)};  // VCA_VAC_VALID_ARGUMENT
          if (!push_back_result.HasValue()) {
            result.EmplaceError(push_back_result.Error());
            break;
          }
          ++first;
        }

        if (result.HasValue()) {
          // If this vector has been relocated while inserting elements, 'pos' is not valid anymore -> calculate the
          // value of 'pos' again for the result.
          // VECTOR NL AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmeticAllocatorIterator
          result.EmplaceValue(start_ + offset);
        }
      } else {
        amsr::core::Result<Vector> tmp{CreateVector(first, last)};
        if (tmp.HasValue()) {
          result = Insert(pos, tmp.Value().cbegin(), tmp.Value().cend());
        } else {
          result.EmplaceError(tmp.Error());
        }
      }
    }

    return result;
  }

  /*!
   * \brief     Inserts copies of the elements from range [\p first, \p last) at the specified location in the vector.
   * \details   Type T must meet C++ Named Requirements: EmplaceConstructible. If memory allocation was unsuccessful or
   *            the requested final size is larger than max_size(), amsr::core::Abort() will be called.
   * \tparam    InputIterator  Type of the input iterator.
   * \param[in] pos    Iterator before which the elements will be inserted. Must be in the range [begin(), end()].
   * \param[in] first  Beginning of the range of elements to insert. Must not be iterator into *this.
   * \param[in] last   Iterator after the last element to insert. Must not be iterator into *this. Must be reachable
   *                   from \p first by incrementation.
   * \return    An iterator pointing to the first element inserted or \p pos if \p first == \p last.
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  template <class InputIterator,
            std::enable_if_t<std::is_convertible<typename std::iterator_traits<InputIterator>::iterator_category,
                                                 std::input_iterator_tag>::value,
                             bool> = true>
  auto InsertOrAbort(const_iterator pos, InputIterator first, InputIterator last) noexcept -> iterator {
    amsr::core::Result<iterator> result{Insert(pos, first, last)};

    if (!result.HasValue()) {
      amsr::core::Abort(result.Error().Message().data());
    }

    return result.Value();
  }

  /*!
   * \brief     Inserts elements from initializer list at the specified location in the vector.
   * \details   Type T must meet C++ Named Requirements: EmplaceConstructible.
   * \param[in] pos     Iterator before which the elements will be inserted. Must be in the range [begin(), end()].
   * \param[in] i_list  Initializer list to insert the values from.
   * \return    On success an iterator pointing to the first element inserted or \p pos if \p i_list is empty, an error
   *            code otherwise.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \error     ContainerErrorCode::kLengthError if the requested final size is larger than max_size().
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  auto Insert(const_iterator pos, std::initializer_list<value_type> i_list) -> amsr::core::Result<iterator> {
    return Insert(pos, i_list.begin(), i_list.end());
  }

  /*!
   * \brief     Inserts elements from initializer list at the specified location in the vector.
   * \details   Type T must meet C++ Named Requirements: EmplaceConstructible. If memory allocation was unsuccessful or
   *            the requested final size is larger than max_size(), amsr::core::Abort() will be called.
   * \param[in] pos     Iterator before which the elements will be inserted. Must be in the range [begin(), end()].
   * \param[in] i_list  Initializer list to insert the values from.
   * \return    An iterator pointing to the first element inserted or \p pos if \p i_list is empty.
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  auto InsertOrAbort(const_iterator pos, std::initializer_list<value_type> i_list) noexcept -> iterator {
    amsr::core::Result<iterator> result{InsertOrAbort(pos, i_list.begin(), i_list.end())};
    return result.Value();
  }

  /*!
   * \brief     Inserts a new element before the specified location in the vector.
   * \details   Type T must meet C++ Named Requirements: EmplaceConstructible, MoveInsertable, MoveAssignable.
   * \tparam    Args  Types of the arguments.
   * \param[in] pos   Position before which the element will be inserted. Must be in the range [begin(), end()].
   * \param[in] args  Arguments to forward to the constructor of the element.
   * \return    An iterator to the inserted element on success, an error code otherwise.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \error     ContainerErrorCode::kLengthError if the requested final size is larger than max_size().
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  // VECTOR NC AutosarC++17_10-A15.4.4: MD_VAC_A15.4.4_explicitNoexceptSpecificationMissing
  template <typename... Args>
  auto Emplace(const_iterator pos, Args&&... args) -> amsr::core::Result<iterator> {
    static_assert(std::is_move_constructible<value_type>::value, "");

    amsr::core::Result<iterator> result{iterator{}};

    amsr::core::Result<pointer> const start_of_gap{InsertGap(pos, 1)};
    // At this point, pos may be invalid, because a relocation may have happened.
    if (start_of_gap.HasValue()) {
      result.EmplaceValue(iterator(start_of_gap.Value()));
      ConstructElementFromArgs(start_of_gap.Value(), std::forward<Args>(args)...);
    } else {
      result.EmplaceError(start_of_gap.Error());
    }
    return result;
  }

  /*!
   * \brief     Inserts a new element before the specified location in the vector.
   * \details   Type T must meet C++ Named Requirements: EmplaceConstructible, MoveInsertable, MoveAssignable. If
   *            memory allocation was unsuccessful or the requested final size is larger than max_size(),
   *            amsr::core::Abort() will be called.
   * \tparam    Args  Types of the arguments.
   * \param[in] pos   Position before which the element will be inserted. Must be in the range [begin(), end()].
   * \param[in] args  Arguments to forward to the constructor of the element.
   * \return    An iterator to the inserted element.
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  template <class... Args>
  auto EmplaceOrAbort(const_iterator pos, Args&&... args) noexcept -> iterator {
    amsr::core::Result<iterator> result{Emplace(pos, std::forward<Args>(args)...)};

    if (!result.HasValue()) {
      amsr::core::Abort(result.Error().Message().data());
    }

    return result.Value();
  }

  /*!
   * \brief     Appends a copy of the given element \p value to the end of the vector.
   * \details   Type T must meet C++ Named Requirements: CopyInsertable.
   * \param[in] value  The value of the element to append.
   * \return    An empty result on success, an error code otherwise.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \error     ContainerErrorCode::kLengthError if the requested final size is larger than max_size().
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  auto PushBack(value_type const& value) -> amsr::core::Result<void> {
    static_assert(std::is_copy_constructible<value_type>::value, "");
    return AppendElement(value);  // VCA_VAC_VALID_ARGUMENT
  }

  /*!
   * \brief     Appends a copy of the given element \p value to the end of the vector.
   * \details   Type T must meet C++ Named Requirements: CopyInsertable. If memory allocation was unsuccessful or the
   *            requested final size is larger than max_size(), amsr::core::Abort() will be called.
   * \param[in] value  The value of the element to append.
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  void PushBackOrAbort(value_type const& value) noexcept {
    static_assert(std::is_copy_constructible<value_type>::value, "");
    AppendElementOrAbort(value);
  }

  /*!
   * \brief     Appends the given element \p value to the end of the vector by moving it into the new element.
   * \details   Type T must meet C++ Named Requirements: MoveInsertable.
   * \param[in] value  The value of the element to append.
   * \return    An empty result on success, an error code otherwise.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \error     ContainerErrorCode::kLengthError if the requested final size is larger than max_size().
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  auto PushBack(value_type&& value) -> amsr::core::Result<void> {
    return EmplaceBack(std::move(value));  // VCA_VAC_VALID_NONSTATIC_METHOD
  }

  /*!
   * \brief     Appends the given element \p value to the end of the vector by moving it into the new element.
   * \details   Type T must meet C++ Named Requirements: MoveInsertable. If memory allocation was unsuccessful or the
   *            requested final size is larger than max_size(), amsr::core::Abort() will be called.
   * \param[in] value  The value of the element to append.
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  void PushBackOrAbort(value_type&& value) noexcept { EmplaceBackOrAbort(std::move(value)); }

  /*!
   * \brief     Appends a new element to the end of the container.
   * \details   The element is constructed through std::allocator_traits::construct. Type T must meet C++ Named
   *            Requirements: EmplaceConstructible, MoveInsertable.
   * \tparam    Args  Types of the arguments.
   * \param[in] args  Arguments to forward to the constructor of the element.
   * \return    An empty result on success, an error code otherwise.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \error     ContainerErrorCode::kLengthError if the requested final size is larger than max_size().
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  // VECTOR NC AutosarC++17_10-A15.4.4: MD_VAC_A15.4.4_explicitNoexceptSpecificationMissing
  template <typename... Args>
  auto EmplaceBack(Args&&... args) -> amsr::core::Result<void> {
    static_assert(std::is_move_constructible<value_type>::value, "");
    return AppendElement(std::forward<Args>(args)...);  // VCA_VAC_VALID_NONSTATIC_METHOD
  }

  /*!
   * \brief     Appends a new element to the end of the container.
   * \details   The element is constructed through std::allocator_traits::construct. Type T must meet C++ Named
   *            Requirements: EmplaceConstructible, MoveInsertable. If memory allocation was unsuccessful or the
   *            requested final size is larger than max_size(), amsr::core::Abort() will be called.
   * \tparam    Args  Types of the arguments.
   * \param[in] args  Arguments to forward to the constructor of the element.
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  template <class... Args>
  void EmplaceBackOrAbort(Args&&... args) noexcept {
    static_assert(std::is_move_constructible<value_type>::value, "");
    AppendElementOrAbort(std::forward<Args>(args)...);
  }

  /*!
   * \brief   Removes the last element from the vector.
   * \details Calling this method on an empty vector results in undefined behavior.
   * \pre     Vector is not empty().
   * \context ANY
   * \steady  TRUE
   */
  void pop_back() noexcept {
    static_assert(std::is_destructible<value_type>::value, "");

    --finish_;
    DestroyElement(finish_, IsPolyAllocT{});
  }

  /*!
   * \brief   Erases all elements from the container.
   * \details After this call, size() returns zero. Invalidates any references, pointers, or iterators referring to
   *          contained elements. Any past-the-end iterators are also invalidated. Leaves the capacity() of the vector
   *          unchanged.
   * \pre     -
   * \spec    requires true; \endspec
   * \context ANY
   * \steady  TRUE
   */
  void clear() noexcept { EraseToEnd(start_); }  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD

  /*!
   * \brief     Remove the element at \p pos.
   * \details   Type T must meet C++ Named Requirements: MoveAssignable.
   * \param[in] pos  The element to remove. Must be in the range [begin(), end()).
   * \return    An iterator following the removed element.
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  auto erase(const_iterator pos) noexcept -> iterator {
    // VECTOR NL AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmetic
    return erase(pos, pos + 1);  // VECTOR SL AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_arrayAccessOutOfBounds
  }

  /*!
   * \brief     Remove the elements from \p first to the element before \p last.
   * \details   Type T must meet C++ Named Requirements: MoveAssignable.
   * \param[in] first  Iterator to first element to remove. Range: [cbegin(), cend()).
   * \param[in] last   Iterator to element after the last element to remove. Range: [cbegin(), cend()]. Must be
   *                   reachable from \p first by incrementation.
   * \return    An iterator following the last removed element.
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  auto erase(const_iterator first, const_iterator last) -> iterator {
    static_assert(std::is_move_assignable<value_type>::value, "");

    if (first != last) {
      if (last != cend()) {
        // This is safe to use as long as elements are moved to the left, which is the case here.
        // VECTOR NL AutosarC++17_10-A5.2.3: MD_VAC_A5.2.3_CastRemovesIteratorConstQualification
        static_cast<void>(std::move(pointer(last), finish_, pointer(first)));  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
      }
      // VECTOR NL AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmetic
      EraseToEnd(finish_ - std::distance(first, last));
    }
    // VECTOR NL AutosarC++17_10-A5.2.3: MD_VAC_A5.2.3_CastRemovesIteratorConstQualification
    return iterator(first);
  }

  /*!
   * \brief     Exchanges content and capacity of this vector with those of \p other.
   * \param[in] other  The container to exchange content with.
   * \pre       -
   * \spec      requires true; \endspec
   * \context   ANY
   * \steady    TRUE
   */
  void swap(Vector& other) noexcept {
    using std::swap;
    swap(start_, other.start_);                    // VCA_VAC_VALID_ARGUMENT
    swap(finish_, other.finish_);                  // VCA_VAC_VALID_ARGUMENT
    swap(end_of_storage_, other.end_of_storage_);  // VCA_VAC_VALID_ARGUMENT

    // VCA_VAC_VALID_ARGUMENT
    SwapAllocators(other.allocator_, typename allocator_traits::propagate_on_container_swap());
  }

 private:
  /*!
   * \brief Pointer to start of vector.
   */
  pointer start_;

  /*!
   * \brief Pointer behind the last element in the Vector.
   */
  pointer finish_;

  /*!
   * \brief Pointer behind the end of storage of Vector.
   */
  pointer end_of_storage_;

  /*!
   * \brief Allocator used to obtain memory.
   */
  actual_allocator_type allocator_;

  /*!
   * \brief     Constructs the container with the contents of \p other using move semantics.
   * \details   The additional two parameters are for tag dispatching:
   *            - std::false_type Indicates that this method would abort in case of an error.
   *            - std::true_type Indicates that all instances of allocator_type compare equal, the \p other Vector can
   *              therefore be moved into this.
   * \param[in] other      Another container to be used as source to initialize the elements of the container with.
   * \param[in] allocator  Allocator to use for all memory allocations of this container.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  Vector(Vector&& other, allocator_type const& allocator, std::false_type, std::true_type) noexcept
      : Vector(allocator) {
    start_ = other.start_;
    finish_ = other.finish_;
    end_of_storage_ = other.end_of_storage_;
    other.ResetStorage();
  }

  /*!
   * \brief     Constructs the container with the contents of \p other using move semantics. Aborts in case of an
   *            error.
   * \details   The additional two parameters are for tag dispatching:
   *            - std::false_type Indicates that this method aborts in case of an error.
   *            - std::false_type Indicates that that not all instances of allocator_type compare equal. If the
   *              allocator of this and the \p other vector do not compare equal and allocation of new memory fails,
   *              this process is terminated by calling amsr::core::Abort()
   * \param[in] other      Another container to be used as source to initialize the elements of the container with.
   * \param[in] allocator  Allocator to use for all memory allocations of this container.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  Vector(Vector&& other, allocator_type const& allocator, std::false_type, std::false_type) : Vector(allocator) {
    if (other.get_allocator() == allocator) {
      std::swap(start_, other.start_);
      std::swap(finish_, other.finish_);
      std::swap(end_of_storage_, other.end_of_storage_);
    } else if (other.size() > 0) {
      amsr::core::Result<void> const result{CreateEmptyStorage(other.size())};

      if (result.HasValue()) {
        finish_ = FillFromIteratorByMove(start_, other.start_, other.finish_);
        other.clear();
      } else {
        amsr::core::Abort(result.Error().Message().data());
      }
    } else {
      // do nothing
    }
  }

  /*!
   * \brief     Constructs the container with the contents of \p other using move semantics. Throws in case of an
   *            error.
   * \details   The additional parameter is for tag dispatching:
   *            - std::true_type Indicates that this method throws an exception in case of an error. Using \p allocator
   *              as the allocator for the new container, moving the contents from \p other; if \p allocator != \p
   *              other.get_allocator(), this results in an element-wise move. (In that case, \p other is not
   *              guaranteed to be empty after the move.) Might throw an error. Type T must meet C++ Named
   *              Requirements: MoveInsertable.
   * \param[in] other      Another container to be used as source to initialize the elements of the container with.
   * \param[in] allocator  The allocator to use for the new container. Caution: using \p other.get_allocator() here may
   *                       lead to problems depending on the underlying memory resource.
   * \throws    std::bad_alloc if a relocation happens and new memory can not be allocated.
   * \pre       -
   * \context   ANY
   */
  Vector(Vector&& other, allocator_type const& allocator, std::true_type)
      : Vector(std::move(other), allocator, std::true_type(), typename allocator_traits::is_always_equal{}) {}

  /*!
   * \brief     Constructs the container with the contents of \p other using move semantics.
   * \details   The additional two parameters are for tag dispatching:
   *            - std::true_type Indicates that this method would throw an exception in case of an error.
   *            - std::true_type Indicates that all instances of allocator_type compare equal, the \p other Vector can
   *              therefore be moved into this.
   * \param[in] other      Another container to be used as source to initialize the elements of the container with.
   * \param[in] allocator  Allocator to use for all memory allocations of this container.
   * \pre       -
   * \context   ANY
   */
  Vector(Vector&& other, allocator_type const& allocator, std::true_type, std::true_type) : Vector(allocator) {
    start_ = other.start_;
    finish_ = other.finish_;
    end_of_storage_ = other.end_of_storage_;
    other.ResetStorage();
  }

  /*!
   * \brief     Constructs the container with the contents of \p other using move semantics. Throws in case of an
   *            error.
   * \details   The additional two parameters are for tag dispatching:
   *            - std::true_type Indicates that this method throws an exception in case of an error.
   *            - std::false_type Indicates that that not all instances of allocator_type compare equal. If the
   *              allocator of this and the \p other vector do not compare equal and allocation of new memory fails, an
   *              exception is thrown.
   * \param[in] other      Another container to be used as source to initialize the elements of the container with.
   * \param[in] allocator  Allocator to use for all memory allocations of this container.
   * \throws    std::bad_alloc if a relocation happens and new memory can not be allocated.
   * \pre       -
   * \context   ANY
   */
  Vector(Vector&& other, allocator_type const& allocator, std::true_type, std::false_type) : Vector(allocator) {
    if (other.get_allocator() == allocator) {
      std::swap(start_, other.start_);
      std::swap(finish_, other.finish_);
      std::swap(end_of_storage_, other.end_of_storage_);
    } else if (other.size() > 0) {
      amsr::core::Result<void> const result{CreateEmptyStorage(other.size())};

      if (result.HasValue()) {
        finish_ = FillFromIteratorByMove(start_, other.start_, other.finish_);
        other.clear();
      } else {
        vac::language::ThrowOrTerminate<std::bad_alloc>();
      }
    } else {
      // do nothing
    }
  }

  /*!
   * \brief   Set the pointers to the underlying storage to nullptr.
   * \pre     -
   * \context ANY
   * \steady  TRUE
   */
  void ResetStorage() noexcept {
    start_ = pointer();           // VCA_VAC_VALID_MEMBER_VARIABLE_ACCESS
    finish_ = pointer();          // VCA_VAC_VALID_MEMBER_VARIABLE_ACCESS
    end_of_storage_ = pointer();  // VCA_VAC_VALID_MEMBER_VARIABLE_ACCESS
  }

  /*!
   * \brief   Deallocates the memory of this vector.
   * \pre     The elements of this vector must have been destroyed before.
   * \context ANY
   * \steady  TRUE
   */
  void Deallocate() {
    if (start_ != end_of_storage_) {
      // VECTOR NC AutosarC++17_10-M5.0.9: MD_VAC_M5.0.9_explicitConversionChangesSignedness
      // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
      allocator_.deallocate(start_, static_cast<size_type>(end_of_storage_ - start_));
    }
  }

  /*!
   * \brief     Clears the existing storage and copies the \p other allocator to this.
   * \details   The additional parameter is for tag dispatching:
   *            - std::true_type Indicates that the allocator of this Vector does propagate on copy assignment.
   * \param[in] other  The allocator of the container to copy.
   * \steady    TRUE
   */
  void CopyAllocatorIfNeeded(allocator_type const& other, std::true_type) noexcept {
    if ((!allocator_traits::is_always_equal::value) && (allocator_ != other)) {
      // replacement allocator cannot free existing storage
      clear();
      Deallocate();
      ResetStorage();
    }
    allocator_ = other;
  }

  /*!
   * \brief   Dummy function in case of allocator which does not propagate on copy assignment.
   * \details The additional parameter is for tag dispatching:
   *          - std::false_type Indicates that the allocator of this Vector does not propagate on copy assignment.
   *            Needed such that assignment of allocators is not compiled.
   * \steady  TRUE
   */
  // VECTOR NL AutosarC++17_10-M0.1.8: MD_VAC_M0.1.8_emptyVoidFunctionWithoutExternalSideEffect
  static void CopyAllocatorIfNeeded(allocator_type const&, std::false_type) noexcept {}

  /*!
   * \brief     Swaps the allocator of this vector with the \p other allocator.
   * \details   The additional parameter is for tag dispatching:
   *            - std::true_type Indicates that the allocator of this Vector does propagate on swap.
   * \param[in] other  The allocator to swap with.
   * \steady    TRUE
   */
  void SwapAllocators(allocator_type& other, std::true_type) noexcept {
    using std::swap;
    // Call swap() without the namespace, so that the allocator's swap function is used if it exists.
    swap(allocator_, other);
  }

  /*!
   * \brief   Dummy function for the case that allocators cannot be swapped.
   * \details The additional parameter is for tag dispatching:
   *          - std::false_type Indicates that the allocator of this Vector does not propagate on swap.
   * \steady  TRUE
   */
  // VECTOR NL AutosarC++17_10-M0.1.8: MD_VAC_M0.1.8_emptyVoidFunctionWithoutExternalSideEffect
  static void SwapAllocators(allocator_type const&, std::false_type) noexcept {}

  /*!
   * \brief     Appends a new element to the end of the container.
   * \tparam    Args  Types of the arguments.
   * \param[in] args  Arguments to forward to the constructor of the element. Can also be the element itself. In this
   *                  case, the element is constructed using its copy constructor.
   * \return    An empty result on success, an error code otherwise.
   * \error     ContainerErrorCode::kAllocationError if allocation of memory was not successful.
   * \error     ContainerErrorCode::kLengthError if the requested final size is larger than max_size().
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  // VECTOR NC AutosarC++17_10-A15.4.4: MD_VAC_A15.4.4_explicitNoexceptSpecificationMissing
  template <typename... Args>
  auto AppendElement(Args&&... args) -> amsr::core::Result<void> {
    amsr::core::Result<void> result{};

    bool const do_relocation{finish_ == end_of_storage_};
    if (do_relocation) {
      result = Relocate(1U);  // VCA_VAC_VALID_NONSTATIC_METHOD
    }

    // The check for !do_relocation is logically not necessary, but it's a small optimization, because it skips the
    // relatively expensive check for result.HasValue(), which is always true if no relocation happened.
    if ((!do_relocation) || result.HasValue()) {
      ConstructElementFromArgs(finish_, std::forward<Args>(args)...);  // VCA_VAC_VALID_ARGUMENT
      // VCA_VAC_VALID_ASSIGNING_NOT_ACCESSING, VCA_VAC_VALID_ACCESS_RUN_TIME_CHECK
      ++finish_;  // VCA_VAC_VALID_PTR_ASSIGNING_NOT_DEREFERENCING
    }

    return result;
  }

  /*!
   * \brief     Appends a new element to the end of the container.
   * \details   If memory allocation was unsuccessful or the requested final size is larger than max_size(),
   *            amsr::core::Abort() will be called.
   * \tparam    Args  Types of the arguments.
   * \param[in] args  Arguments to forward to the constructor of the element. Can also be the element itself. In this
   *                  case, the element is constructed using its copy constructor.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  template <class... Args>
  void AppendElementOrAbort(Args&&... args) noexcept {
    amsr::core::Result<void> const result{AppendElement(std::forward<Args>(args)...)};

    if (!result.HasValue()) {
      amsr::core::Abort(result.Error().Message().data());
    }
  }

  /*!
   * \brief     Constructs an element in a piece of memory using move construction.
   * \details   The additional parameter is for tag dispatching:
   *            - std::true_type Indicates that value_type is move constructible.
   * \param[in] memory  A pointer to the memory to construct the value in. The memory must be allocated and not
   *                    occupied with a valid object.
   * \param[in] value   A pointer to the value which should be used for construction.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  void ConstructElement(pointer memory, value_type* value, std::true_type) {
    static_assert(std::is_move_constructible<value_type>::value, "");
    ConstructElementFromArgs(memory, std::move(*value));  // VCA_VAC_VALID_ARGUMENT
  }

  /*!
   * \brief     Constructs an element in a piece of memory using copy construction.
   * \details   The additional parameter is for tag dispatching:
   *            - std::false_type Indicates that value_type is not move constructible.
   * \param[in] memory  A pointer to the memory to construct the value in. The memory must be allocated and not
   *                    occupied with a valid object.
   * \param[in] value   A pointer to the value which should be used for construction.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  void ConstructElement(pointer memory, value_type* value, std::false_type) {
    static_assert(std::is_copy_constructible<value_type>::value, "");
    ConstructElementFromArgs(memory, *value);  // VCA_VAC_VALID_ARGUMENT
  }

  /*!
   * \brief     Move assign vector in case allocator is propagated.
   * \details   The additional parameter is for tag dispatching:
   *            - std::true_type Indicates that the allocator of this Vector propagates on move assignment or always
   *              compares equal.
   * \param[in] other  Another container to use as data source.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  void MoveAssign(Vector&& other, bool, std::true_type) noexcept {
    swap(other);  // VCA_VAC_VALID_ARGUMENT
  }

  /*!
   * \brief     Move assign vector in case allocator is not propagated.
   * \details   The additional parameter is for tag dispatching:
   *            - std::false_type Indicates that the allocator of this Vector does not propagate on move assignment and
   *              does not always compare equal.
   * \param[in] other           Another container to use as data source.
   * \param[in] throw_on_error  Whether an exception is thrown if a memory allocation fails. If false or exceptions are
   *                            turned off, the process is terminated in case of an error.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  void MoveAssign(Vector&& other, bool throw_on_error, std::false_type) {
    // VCA_VAC_VALID_NONSTATIC_METHOD
    if (other.get_allocator() == this->get_allocator()) {
      // VECTOR NC AutosarC++17_10-A18.9.2: MD_VAC_A18.9.2_useStdMoveIfTheValueIsARvalueReference
      // VCA_VAC_VALID_NONSTATIC_METHOD, VCA_VAC_VALID_ARGUMENT
      MoveAssign(std::move(other), throw_on_error, std::true_type());
    } else {
      // The rvalue's allocator cannot be moved and is not equal,
      // so we need to individually move each element.
      MoveAssignElements(other.begin(), other.end(), throw_on_error);  // VCA_VAC_VALID_NONSTATIC_METHOD
      other.clear();                                                   // VCA_VAC_VALID_NONSTATIC_METHOD
    }
  }

  /*!
   * \brief     Move assign each element in the range [\p first, \p last) individually to this vector.
   * \details   \p this has the new length ( \p last - \p first ). If memory allocation is necessary and fails, an
   *            exception is thrown or the process is terminated.
   * \param[in] first           Beginning of the range to move.
   * \param[in] last            End of the range to move. Must be reachable from \p first by incrementation.
   * \param[in] throw_on_error  Whether an exception is thrown if a memory allocation fails. If false or exceptions are
   *                            turned off, the process is terminated in case of an error.
   * \throws    std::bad_alloc if \p throw_on_error is true, a relocation happens and new memory can not be allocated.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  void MoveAssignElements(iterator first, iterator last, bool throw_on_error) {
    static_assert(std::is_move_assignable<value_type>::value, "");

    pointer current{start_};
    while ((current != finish_) && (first != last)) {
      *current = std::move(*first);  // VCA_VAC_VALID_ARGUMENT
      // VECTOR NL AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_arrayAccessOutOfBounds
      ++current;
      // VECTOR NL AutosarC++17_10-A12.8.3:MD_VAC_A12.8.3_movedFromObjectReadAccessedInLoop
      ++first;  // VECTOR SL AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_arrayAccessOutOfBounds
    }

    if (first == last) {
      EraseToEnd(current);  // VCA_VAC_VALID_NONSTATIC_METHOD
    } else {
      // VECTOR NL AutosarC++17_10-A12.8.3: MD_VAC_A12.8.3_movedFromObjectReadAccessedInLoop
      while (first != last) {
        // VECTOR NL AutosarC++17_10-A12.8.3: MD_VAC_A12.8.3_movedFromObjectReadAccessedInLoop
        amsr::core::Result<void> const push_back_result{PushBack(std::move(*first))};  // VCA_VAC_VALID_ARGUMENT
        if (!push_back_result.HasValue()) {
          if (throw_on_error) {
            vac::language::ThrowOrTerminate<std::bad_alloc>();
          } else {
            amsr::core::Abort(push_back_result.Error().Message().data());
          }
        }
        // VECTOR NL AutosarC++17_10-A12.8.3:MD_VAC_A12.8.3_movedFromObjectReadAccessedInLoop
        ++first;  // VECTOR SL AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_arrayAccessOutOfBounds
      }
    }
  }

  /*!
   * \brief     Replaces the contents with those of \p other using move semantics. Throws in case of an error.
   * \details   Might throw an error if the allocator is not propagated and does not always compare equal and if the
   *            allocate() or deallocate() methods of the allocator are not noexcept. If the allocator is propagated or
   *            compares equal to the allocator of \p other, then \p other after the move is guaranteed to be equal to
   *            this vector before the move. Type T must meet C++ Named Requirements: MoveInsertable, MoveAssignable.
   * \param[in] other  Another container to use as data source.
   * \return    A reference to this vector after assigning.
   * \throws    std::bad_alloc if a relocation happens and new memory can not be allocated.
   * \pre       -
   * \context   ANY
   */
  void ThrowingMoveAssign(Vector&& other) noexcept(
      allocator_traits::propagate_on_container_move_assignment::value || allocator_traits::is_always_equal::value ||
      (noexcept(std::declval<Alloc>().allocate(1)) && noexcept(std::declval<Alloc>().deallocate(nullptr, 1)))) {
    constexpr bool kMoveStorage{allocator_traits::propagate_on_container_move_assignment::value ||
                                allocator_traits::is_always_equal::value};
    // VCA_VAC_VALID_NONSTATIC_METHOD
    MoveAssign(std::move(other), true, std::integral_constant<bool, kMoveStorage>());
  }

  /*!
   * \brief     Creates storage for a new vector.
   * \details   Afterwards, this vector has the given capacity and a size of 0.
   * \param[in] target_capacity  The target capacity of the new vector. Must not be 0.
   * \return    Nothing on success, an error code otherwise.
   * \error     ContainerErrorCode::kLengthError if the requested capacity is larger than max_size().
   * \error     ContainerErrorCode::kAllocationError if memory for the new location cannot be allocated.
   * \throws    <TYPE> Passes on any exception the allocator might throw.
   * \pre       Size and capacity of this vector must be 0.
   * \context   ANY
   * \steady    TRUE
   */
  auto CreateEmptyStorage(size_type target_capacity) -> amsr::core::Result<void> {
    amsr::core::Result<void> result{};

    if (target_capacity > max_size()) {
      result.EmplaceError(amsr::ContainerErrorCode::kLengthError);
    } else {
      pointer const new_start{allocator_.allocate(target_capacity)};
      if (new_start == nullptr) {
        result.EmplaceError(amsr::ContainerErrorCode::kAllocationError);
      } else {
        start_ = new_start;
        finish_ = start_;
        // VECTOR NL AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmeticAllocatorIterator
        end_of_storage_ = start_ + target_capacity;
      }
    }

    return result;
  }

  /*!
   * \brief     Relocates the content of the vector to new memory with at least the specified surplus capacity.
   * \details   Overload for the case that the value type is not trivially copyable. In this case, all elements are
   *            individually constructed in the new memory and destroyed in the old memory.
   * \param[in] min_surplus_capacity  The target minimum capacity on top of the current capacity. Must not be 0.
   * \return    An empty result on success, an error code otherwise.
   * \error     ContainerErrorCode::kLengthError if the requested capacity is larger than max_size().
   * \error     ContainerErrorCode::kAllocationError if memory for the new location cannot be allocated.
   * \pre       -
   * \context   ANY
   * \note      The new capacity is at least double the current size() and at least capacity() plus \p
   *            min_surplus_capacity, but capped at max_size().
   * \steady    TRUE
   */
  template <typename U = value_type, std::enable_if_t<!std::is_trivially_copyable<U>::value, bool> = true>
  auto Relocate(size_type min_surplus_capacity) noexcept(false) -> amsr::core::Result<void> {
    static_assert(std::is_destructible<value_type>::value, "");

    amsr::core::Result<void> result{};
    if ((capacity() + min_surplus_capacity) > max_size()) {  // VCA_VAC_VALID_NONSTATIC_METHOD
      result.EmplaceError(amsr::ContainerErrorCode::kLengthError);
    } else {
      size_type new_capacity{std::max(2 * size(), capacity() + min_surplus_capacity)};
      new_capacity = std::min(max_size(), new_capacity);  // VCA_VAC_VALID_NONSTATIC_METHOD
      // VCA_VAC_STD_FUNC_FULFILL_CONTRACT, VCA_VAC_VALID_ARGUMENT
      pointer const new_start{allocator_.allocate(new_capacity)};

      if (new_start == nullptr) {
        result.EmplaceError(amsr::ContainerErrorCode::kAllocationError);
      } else {
        // Move elements to the start of the new location.
        pointer new_current{new_start};
        pointer old_current{start_};
        while (old_current < finish_) {
          // VCA_VAC_VALID_NONSTATIC_METHOD
          ConstructElement(new_current, old_current, HasMoveCtorT{});
          // VCA_VAC_VALID_NONSTATIC_METHOD
          DestroyElement(old_current, IsPolyAllocT{});
          ++old_current;  // VECTOR SL AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_pointerArithmetic
          ++new_current;  // VECTOR SL AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_pointerArithmetic
        }

        Deallocate();  // VCA_VAC_VALID_NONSTATIC_METHOD

        // VCA_VAC_VALID_ASSIGNING_NOT_ACCESSING, VCA_VAC_VALID_ACCESS_RUN_TIME_CHECK
        start_ = new_start;  // VCA_VAC_VALID_PTR_ASSIGNING_NOT_DEREFERENCING
        // VCA_VAC_VALID_ASSIGNING_NOT_ACCESSING, VCA_VAC_VALID_ACCESS_RUN_TIME_CHECK
        finish_ = new_current;  // VCA_VAC_VALID_PTR_ASSIGNING_NOT_DEREFERENCING
        // VECTOR NC AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmetic
        // VCA_VAC_VALID_ASSIGNING_NOT_ACCESSING, VCA_VAC_VALID_ACCESS_RUN_TIME_CHECK
        end_of_storage_ = start_ + new_capacity;  // VCA_VAC_VALID_PTR_ASSIGNING_NOT_DEREFERENCING
      }
    }

    return result;
  }

  /*!
   * \brief     Relocates the content of the vector to new memory with at least the specified surplus capacity.
   * \details   Overload for the case that the value type is trivially copyable. In this case, the elements can be
   *            copied to the new memory using std::memcpy().
   * \param[in] min_surplus_capacity  The target minimum capacity on top of the current capacity. Must not be 0.
   * \return    An empty result on success, an error code otherwise.
   * \error     ContainerErrorCode::kLengthError if the requested capacity is larger than max_size().
   * \error     ContainerErrorCode::kAllocationError if memory for the new location cannot be allocated.
   * \pre       -
   * \context   ANY
   * \note      The new capacity is at least double the current size() and at least capacity() plus \p
   *            min_surplus_capacity, but capped at max_size().
   * \steady    TRUE
   */
  template <typename U = value_type, std::enable_if_t<std::is_trivially_copyable<U>::value, bool> = true>
  auto Relocate(size_type min_surplus_capacity) noexcept(false) -> amsr::core::Result<void> {
    static_assert(std::is_destructible<value_type>::value, "");

    amsr::core::Result<void> result{};
    if ((capacity() + min_surplus_capacity) > max_size()) {  // VCA_VAC_VALID_NONSTATIC_METHOD
      result.EmplaceError(amsr::ContainerErrorCode::kLengthError);
    } else {
      size_type new_capacity{std::max(2 * size(), capacity() + min_surplus_capacity)};
      new_capacity = std::min(max_size(), new_capacity);  // VCA_VAC_VALID_NONSTATIC_METHOD
      // VCA_VAC_STD_FUNC_FULFILL_CONTRACT, VCA_VAC_VALID_ARGUMENT
      pointer const new_start{allocator_.allocate(new_capacity)};

      if (new_start == nullptr) {
        result.EmplaceError(amsr::ContainerErrorCode::kAllocationError);
      } else {
        std::size_t const number_of_elements{size()};

        if (number_of_elements > 0) {
          // Copy elements to the start of the new location.
          // VECTOR NC AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_wrapAroundMaxSize
          // VECTOR NC AutosarC++17_10-A4.7.1: MD_VAC_A4.7.1_wrapAroundMaxSize
          // VCA_VAC_ACCESS_POSSIBLY_OUT_OF_BOUNDS_COMPUTED_AT_RUNTIME
          static_cast<void>(std::memcpy(new_start, start_, number_of_elements * sizeof(value_type)));
          clear();  // VCA_VAC_VALID_NONSTATIC_METHOD
        }

        Deallocate();  // VCA_VAC_VALID_NONSTATIC_METHOD

        // VCA_VAC_VALID_ASSIGNING_NOT_ACCESSING, VCA_VAC_VALID_ACCESS_RUN_TIME_CHECK
        start_ = new_start;  // VCA_VAC_VALID_PTR_ASSIGNING_NOT_DEREFERENCING
        // VECTOR NC AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmetic
        // VCA_VAC_VALID_ASSIGNING_NOT_ACCESSING, VCA_VAC_VALID_ACCESS_RUN_TIME_CHECK
        finish_ = new_start + number_of_elements;  // VCA_VAC_VALID_PTR_ASSIGNING_NOT_DEREFERENCING
        // VECTOR NC AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmetic
        // VCA_VAC_VALID_ASSIGNING_NOT_ACCESSING, VCA_VAC_VALID_ACCESS_RUN_TIME_CHECK
        end_of_storage_ = start_ + new_capacity;  // VCA_VAC_VALID_PTR_ASSIGNING_NOT_DEREFERENCING
      }
    }

    return result;
  }

  /*!
   * \brief     Relocates the content of the vector to new memory so that there is a gap before \p behind_gap in the
   *            new location.
   * \details   Overload for the case that the value type is not trivially copyable. In this case, all elements are
   *            individually constructed in the new memory and destroyed in the old memory.
   * \param[in] behind_gap  The position in the old vector of the elements which should come after the gap in the new
   *                        location.
   * \param[in] gap_size    The gap size. Must not be 0.
   * \return    The start position of the gap on success, an error code otherwise.
   * \error     ContainerErrorCode::kLengthError if the requested capacity is larger than max_size().
   * \error     ContainerErrorCode::kAllocationError if memory for the new location cannot be allocated.
   * \pre       -
   * \context   ANY
   * \note      The new capacity is at least double the current size() and at least size() plus \p gap_size, but capped
   *            at max_size().
   * \steady    TRUE
   */
  template <typename U = value_type, std::enable_if_t<!std::is_trivially_copyable<U>::value, bool> = true>
  auto RelocateWithGap(pointer behind_gap, size_type gap_size) noexcept(false) -> amsr::core::Result<pointer> {
    static_assert(std::is_move_constructible<value_type>::value, "");
    static_assert(std::is_destructible<value_type>::value, "");

    amsr::core::Result<pointer> result{pointer()};

    if ((size() + gap_size) > max_size()) {
      result.EmplaceError(amsr::ContainerErrorCode::kLengthError);
    } else {
      size_type new_capacity{size() + std::max(size(), gap_size)};
      new_capacity = std::min(max_size(), new_capacity);
      pointer const new_start{allocator_.allocate(new_capacity)};

      if (new_start == nullptr) {
        result.EmplaceError(amsr::ContainerErrorCode::kAllocationError);
      } else {
        // Move elements from start_ to behind_gap to the start of the new location.
        pointer new_current{new_start};
        pointer old_current{start_};
        // VECTOR Disable AutosarC++17_10-A12.8.3: MD_VAC_A12.8.3_movedFromObjectReadAccessedInLoop
        while (old_current < behind_gap) {
          // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
          ConstructElementFromArgs(new_current, std::move(*old_current));
          DestroyElement(old_current, IsPolyAllocT{});
          ++old_current;  // VECTOR SL AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_arrayAccessOutOfBounds
          ++new_current;  // VECTOR SL AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_pointerArithmetic
        }

        // new_current now points to the start of the gap.
        result.EmplaceValue(new_current);

        // Leave a gap in the new location.
        // VECTOR NL AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmetic
        new_current += gap_size;  // VECTOR SL AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_pointerArithmetic

        // Move elements from behind_gap to finish_ to the position after the gap in the new location.
        while (old_current < finish_) {
          // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
          ConstructElementFromArgs(new_current, std::move(*old_current));
          DestroyElement(old_current, IsPolyAllocT{});
          ++old_current;  // VECTOR SL AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_arrayAccessOutOfBounds
          ++new_current;  // VECTOR SL AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_pointerArithmetic
        }

        Deallocate();

        start_ = new_start;
        finish_ = new_current;
        // VECTOR NL AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmetic
        end_of_storage_ = start_ + new_capacity;
        // VECTOR Enable AutosarC++17_10-A12.8.3: MD_VAC_A12.8.3_movedFromObjectReadAccessedInLoop
      }
    }

    return result;
  }

  /*!
   * \brief     Relocates the content of the vector to new memory so that there is a gap before \p behind_gap in the
   *            new location.
   * \details   Overload for the case that the value type is trivially copyable. In this case, the elements can be
   *            copied to the new memory using std::memcpy().
   * \param[in] behind_gap  The position in the old vector of the elements which should come after the gap in the new
   *                        location.
   * \param[in] gap_size    The gap size. Must not be 0.
   * \return    The start position of the gap on success, an error code otherwise.
   * \error     ContainerErrorCode::kLengthError if the requested capacity is larger than max_size().
   * \error     ContainerErrorCode::kAllocationError if memory for the new location cannot be allocated.
   * \pre       -
   * \context   ANY
   * \note      The new capacity is at least double the current size() and at least size() plus \p gap_size, but capped
   *            at max_size().
   * \steady    TRUE
   */
  template <typename U = T, std::enable_if_t<std::is_trivially_copyable<U>::value, bool> = true>
  auto RelocateWithGap(pointer behind_gap, size_type gap_size) noexcept(false) -> amsr::core::Result<pointer> {
    static_assert(std::is_move_constructible<value_type>::value, "");
    static_assert(std::is_destructible<value_type>::value, "");

    amsr::core::Result<pointer> result{pointer()};

    if ((size() + gap_size) > max_size()) {
      result.EmplaceError(amsr::ContainerErrorCode::kLengthError);
    } else {
      size_type new_capacity{size() + std::max(size(), gap_size)};
      new_capacity = std::min(max_size(), new_capacity);
      pointer const new_start{allocator_.allocate(new_capacity)};

      if (new_start == nullptr) {
        result.EmplaceError(amsr::ContainerErrorCode::kAllocationError);
      } else if (start_ != finish_) {
        // Copy elements from start_ to behind_gap to the start of the new location.
        std::ptrdiff_t number_of_elements{std::distance(start_, behind_gap)};
        // VECTOR NC AutosarC++17_10-A4.7.1: MD_VAC_A4.7.1_wrapAroundMaxSize
        static_cast<void>(
            // VCA_VAC_ACCESS_POSSIBLY_OUT_OF_BOUNDS_COMPUTED_AT_RUNTIME
            std::memcpy(new_start, start_, static_cast<std::size_t>(number_of_elements) * sizeof(value_type)));
        // VECTOR NL AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmetic
        pointer gap{new_start + number_of_elements};

        // 'gap' now points to the start of the gap.
        result.EmplaceValue(gap);
        // VECTOR NL AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmetic
        gap += gap_size;

        // Copy elements from behind_gap to finish_ to the position after the gap in the new location.
        number_of_elements = std::distance(behind_gap, finish_);
        // VECTOR NC AutosarC++17_10-A4.7.1: MD_VAC_A4.7.1_wrapAroundMaxSize
        static_cast<void>(
            // VCA_VAC_ACCESS_POSSIBLY_OUT_OF_BOUNDS_COMPUTED_AT_RUNTIME
            std::memcpy(gap, behind_gap, static_cast<std::size_t>(number_of_elements) * sizeof(value_type)));

        clear();
        Deallocate();
        start_ = new_start;
        // VECTOR NL AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmetic
        finish_ = gap + number_of_elements;
        // VECTOR NL AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmetic
        end_of_storage_ = start_ + new_capacity;
      } else {
        Deallocate();
        start_ = new_start;
        // VECTOR NL AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmetic
        finish_ = start_ + gap_size;
        // VECTOR NL AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmetic
        end_of_storage_ = start_ + new_capacity;
        result.EmplaceValue(start_);
      }
    }

    return result;
  }

  /*!
   * \brief     Moves the elements from \p pos on further along, so that a gap emerges. Relocates the content, if
   *            necessary. Afterwards, the elements in the gap are not initialized.
   * \param[in] pos       The position of the elements which should be moved further along. Range: [cbegin(), cend()].
   * \param[in] gap_size  The gap size.
   * \return    The start position of the gap on success, an error code otherwise.
   * \error     ContainerErrorCode::kAllocationError if there is no capacity left for the gap and new memory cannot be
   *            allocated.
   * \error     ContainerErrorCode::kLengthError if there is no capacity left for the gap and trying to allocate new
   *            memory would exceed max_size().
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  auto InsertGap(const_iterator pos, size_type gap_size) -> amsr::core::Result<pointer> {
    static_assert(std::is_move_assignable<value_type>::value, "");
    static_assert(std::is_destructible<value_type>::value, "");

    // Convert 'pos' from const_iterator to pointer.
    // VECTOR NL AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmetic
    pointer const behind_gap{(pos == nullptr) ? nullptr : ((pos - cbegin()) + start_)};
    amsr::core::Result<pointer> result{behind_gap};

    if (gap_size > 0U) {
      // VECTOR NL AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmetic
      if ((!PointersAreNull()) && ((finish_ + gap_size) <= end_of_storage_)) {
        if (behind_gap != finish_) {
          // VECTOR NL AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmetic
          pointer const before_finish{finish_ - 1};
          // VECTOR NC AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_pointerArithmetic
          // VECTOR NL AutosarC++17_10-A12.8.3: MD_VAC_A12.8.3_movedFromObjectReadAccessedInLoop
          for (pointer i{before_finish}; i >= behind_gap; --i) {
            // VECTOR NC AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_pointerArithmetic
            // VECTOR NL AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmetic
            pointer const new_place{i + gap_size};
            ConstructElement(new_place, i, HasMoveCtorT{});
            DestroyElement(i, IsPolyAllocT{});
          }
        }
        // VECTOR NL AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmetic
        finish_ += gap_size;
      } else {
        result = RelocateWithGap(behind_gap, gap_size);
      }
    }
    return result;
  }

  /*!
   * \brief     Inserts a gap after the last element. Relocates the content, if necessary. Afterwards, the elements in
   *            the gap are not initialized.
   * \param[in] gap_size  The gap size.
   * \return    The start position of the gap on success, an error code otherwise.
   * \error     ContainerErrorCode::kAllocationError if there is no capacity left for the gap and new memory cannot be
   *            allocated.
   * \error     ContainerErrorCode::kLengthError if there is no capacity left for the gap and trying to allocate new
   *            memory would exceed max_size().
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  auto AppendGap(size_type gap_size) -> amsr::core::Result<pointer> {
    amsr::core::Result<pointer> result{finish_};

    if (gap_size > 0U) {
      // VECTOR NL AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmetic
      if (PointersAreNull() || ((finish_ + gap_size) > end_of_storage_)) {
        amsr::core::Result<void> const relocation_result{Relocate(gap_size)};
        if (!relocation_result.HasValue()) {
          result.EmplaceError(relocation_result.Error());
        }
      }

      if (result.HasValue()) {
        result.EmplaceValue(finish_);
        // VECTOR NL AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmetic
        finish_ += gap_size;
      }
    }

    return result;
  }

  /*!
   * \brief     Fills a range of memory with the default value.
   * \details   Overload for the case that the value type is trivial.
   * \param[in] start  The start of the memory. Range: [start_, end_of_storage_).
   * \param[in] end    The location behind the end of the memory to be filled. Range: [start_, end_of_storage_]. Must
   *                   be larger than \p start.
   * \pre       The memory to fill must be allocated and uninitialized.
   * \context   ANY
   * \steady    TRUE
   */
  template <typename U = value_type, std::enable_if_t<std::is_trivial<U>::value, bool> = true>
  void FillWithDefault(pointer start, pointer end) noexcept(false) {
    static_assert(std::is_default_constructible<value_type>::value,
                  "The element type must have a default constructor.");
    if (IsPolyAllocT::value) {
      // If we know that the used allocator is ara::core::PolymorphicAllocator, we can apply a shortcut instead of
      // calling the allocator to construct the elements. Calling the allocator is relatively slow.

      // If the type is trivial, use placement-new to initialize an array of elements in the allocated memory.
      std::size_t const number_of_elements{static_cast<std::size_t>(std::distance(start, end))};
      // VCA_VAC_ACCESS_POSSIBLY_OUT_OF_BOUNDS_COMPUTED_AT_RUNTIME
      new (static_cast<void*>(start)) value_type[number_of_elements]{};

    } else if (IsNonZeroingAllocT::value) {
      // For trivially constructible types, vac::memory::NonZeroingAllocator doesn't do anything to construct a value,
      // so we don't need to call it in the first place.
    } else {
      // VECTOR NL AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_pointerArithmetic
      for (pointer i{start}; i < end; i++) {
        allocator_traits::construct(allocator_, i);  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
      }
    }
  }

  /*!
   * \brief     Fills a range of memory with the default value.
   * \details   Overload for the case that the value type is not trivial.
   * \param[in] start  The start of the memory. Range: [start_, end_of_storage_).
   * \param[in] end    The location behind the end of the memory to be filled. Range: [start_, end_of_storage_]. Must
   *                   be larger or equal \p start.
   * \pre       The memory to fill must be allocated and uninitialized.
   * \context   ANY
   * \steady    TRUE
   */
  template <typename U = value_type, std::enable_if_t<!std::is_trivial<U>::value, bool> = true>
  void FillWithDefault(pointer start, pointer end) noexcept(false) {
    static_assert(std::is_default_constructible<value_type>::value,
                  "The element type must have a default constructor.");
    if (IsPolyAllocT::value) {
      // If we know that the used allocator is ara::core::PolymorphicAllocator, we can apply some shortcuts instead of
      // calling the allocator to construct the elements. Calling the allocator is relatively slow.

      // If the type is not trivial, placement-new with an array may not work, because the type may have an explicit
      // default constructor. Instead, do what the allocator would do in its construct() method. This is still
      // substantially faster than calling the allocator.
      for (pointer i{start}; i < end; i++) {
        // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK, VCA_VAC_VALID_CONSTRUCTOR_CALLED
        new (static_cast<void*>(i)) value_type();
      }

    } else {
      // VECTOR NL AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_pointerArithmetic
      for (pointer i{start}; i < end; i++) {
        allocator_traits::construct(allocator_, i);  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
      }
    }
  }

  /*!
   * \brief     Fills a range of memory with copies of a value.
   * \param[in] start  The start of the memory. Range: [start_, end_of_storage_).
   * \param[in] end    The location behind the end of the memory to be filled. Range: [start_, end_of_storage_]. Must
   *                   be larger or equal \p start.
   * \param[in] value  The value.
   * \pre       The memory to fill must be allocated and uninitialized.
   * \context   ANY
   * \steady    TRUE
   */
  void FillWithCopies(pointer start, pointer end, value_type const& value) {
    static_assert(std::is_copy_constructible<value_type>::value, "");
    // VECTOR NL AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_pointerArithmetic
    for (pointer i{start}; i < end; i++) {
      ConstructElementFromArgs(i, value);
    }
  }

  /*!
   * \brief     Fills a range of memory with values from another range. The values are copied.
   * \tparam    InputIterator  Type of the input iterator.
   * \param[in] start  The start of the memory. Range: [start_, end_of_storage_).
   * \param[in] first  The start of the range to take the values from.
   * \param[in] last   The end of the range to take the values from. Must be reachable from \p first by incrementation.
   * \return    A pointer to the location behind the last constructed value.
   * \pre       The memory to fill must be allocated and uninitialized.
   * \context   ANY
   * \steady    TRUE
   */
  // VECTOR NC AutosarC++17_10-A15.4.4: MD_VAC_A15.4.4_explicitNoexceptSpecificationMissing
  template <typename InputIterator,
            std::enable_if_t<std::is_convertible<typename std::iterator_traits<InputIterator>::iterator_category,
                                                 std::input_iterator_tag>::value,
                             bool> = true>
  static auto FillFromIteratorByCopy(pointer start, InputIterator first, InputIterator last) -> pointer {
    static_assert(std::is_copy_constructible<value_type>::value, "");
    return std::uninitialized_copy(first, last, start);  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
  }

  /*!
   * \brief     Fills a range of memory with values from another range. The values are moved.
   * \tparam    InputIterator  Type of the input iterator.
   * \param[in] start  The start of the memory. Range: [start_, end_of_storage_).
   * \param[in] first  The start of the range to take the values from.
   * \param[in] last   The end of the range to take the values from. Must be reachable from \p first by incrementation.
   * \return    A pointer to the location behind the last constructed value.
   * \pre       The memory to fill must be allocated and uninitialized.
   * \context   ANY
   * \steady    TRUE
   */
  // VECTOR NC AutosarC++17_10-A15.4.4: MD_VAC_A15.4.4_explicitNoexceptSpecificationMissing
  template <typename InputIterator,
            std::enable_if_t<std::is_convertible<typename std::iterator_traits<InputIterator>::iterator_category,
                                                 std::input_iterator_tag>::value,
                             bool> = true>
  auto FillFromIteratorByMove(pointer start, InputIterator first, InputIterator last) -> pointer {
    static_assert(std::is_move_constructible<value_type>::value, "");
    // VECTOR Disable AutosarC++17_10-A12.8.3: MD_VAC_A12.8.3_movedFromObjectReadAccessedInLoop
    while (first != last) {
      ConstructElementFromArgs(start, std::move(*first));  // VCA_VAC_VALID_ARGUMENT
      ++first;  // VECTOR SL AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_arrayAccessOutOfBounds
      ++start;  // VECTOR SL AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_arrayAccessOutOfBounds
    }
    return start;
    // VECTOR Enable AutosarC++17_10-A12.8.3: MD_VAC_A12.8.3_movedFromObjectReadAccessedInLoop
  }

  /*!
   * \brief     Removes all elements from \p pos to the end.
   * \param[in] pos  The beginning of the range to remove. Range: [start_, finish_].
   * \pre       -
   * \context   ANY
   * \note      The capacity is not changed.
   * \steady    TRUE
   */
  void EraseToEnd(pointer pos) {
    static_assert(std::is_destructible<value_type>::value, "");

    if (IsPolyAllocT::value) {
      // If we know that the used allocator is ara::core::PolymorphicAllocator, we can apply some shortcuts instead of
      // calling the allocator to destruct the elements. Calling the allocator is relatively slow.
      if (std::is_trivially_destructible<value_type>::value) {
        // Do nothing, because if the element type has a trivial destructor, it is not necessary to call it at all.
      } else {
        // Do what the allocator would do in its destroy() method. This is substantially faster than calling the
        // allocator.
        pointer current{pos};
        while (current != finish_) {
          current->~value_type();  // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK, VCA_VAC_VALID_NONSTATIC_METHOD
          ++current;               // VECTOR SL AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_pointerArithmetic
        }
      }
    } else {
      pointer current{pos};
      while (current != finish_) {
        allocator_traits::destroy(allocator_, current);  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
        ++current;  // VECTOR SL AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_pointerArithmetic
      }
    }

    finish_ = pos;  // VCA_VAC_VALID_ASSIGNING_NOT_ACCESSING
  }

  /*!
   * \brief     Destroy an element with an ara::core::PolymorphicAllocator<value_type>.
   * \details   This function is called if the allocator is ara::core::PolymorphicAllocator, i.e. if kIsPolyAlloc is
   *            true.
   * \param[in] pos  A pointer to the element to destroy. Must point to a valid object in the range [start_, finish_).
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConstLogicalNonConst
  // VECTOR NL AutosarC++17_10-M0.1.8: MD_VAC_M0.1.8_functionHasNoExternalSideEffect
  void DestroyElement(pointer pos, std::true_type) {
    // If we know that the used allocator is ara::core::PolymorphicAllocator, we can call the allocator directly
    // instead of going through allocator_traits. The latter is relatively slow.
    allocator_.destroy(pos);
  }

  /*!
   * \brief     Destroy an element with any allocator.
   * \details   This function is called if the allocator is not ara::core::PolymorphicAllocator, i.e. if kIsPolyAlloc is
   *            false.
   * \param[in] pos  A pointer to the element to destroy. Must point to a valid object in the range [start_, finish_).
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  void DestroyElement(pointer pos, std::false_type) {
    allocator_traits::destroy(allocator_, pos);  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
  }

  /*!
   * \brief     Construct an element from its constructor arguments.
   * \tparam    Args  Types of the constructor arguments.
   * \param[in] pos   Memory to construct the element at. Must point to allocated, uninitialized memory in the range
   *                  [start_, finish_).
   * \param[in] args  Arguments to forward to the constructor of the element. Can also be the element itself. In this
   *                  case, the element is constructed using its copy constructor.
   * \pre       -
   * \context   ANY
   * \steady    TRUE
   */
  // VECTOR NC AutosarC++17_10-A15.4.4: MD_VAC_A15.4.4_explicitNoexceptSpecificationMissing
  template <typename... Args>
  void ConstructElementFromArgs(pointer pos, Args&&... args) {
    // If we know that the used allocator is ara::core::PolymorphicAllocator, we can copy the behavior of the allocator
    // instead of calling it through allocator_traits. The latter is relatively slow.
    if (IsPolyAllocT::value) {
      // VCA_VAC_VALID_ASSIGNING_NOT_ACCESSING, VCA_VAC_VALID_CONSTRUCTOR_CALLED,
      // VCA_VAC_VALID_FUNC_CALL_RUN_TIME_CHECK, VCA_VAC_VALID_ARGUMENT
      new (static_cast<void*>(pos)) value_type(std::forward<Args>(args)...);
    } else {
      // VCA_VAC_VALID_ARGUMENT, VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
      allocator_traits::construct(allocator_, pos, std::forward<Args>(args)...);
    }
  }

  /*!
   * \brief   Checks if the pointers to the underlying memory are nullptr.
   * \return  True if the pointers to the underlying memory are nullptr, False otherwise.
   * \pre     -
   * \context ANY
   * \note    To check for nullptr is in some cases necessary, because some kinds of pointer arithmetic are not allowed
   *          when dealing with nullptrs.
   * \steady  TRUE
   */
  auto PointersAreNull() const noexcept -> bool {
    // It is sufficient to check start_, because there is no way that finish_ or end_of_storage_ are nullptr, but start_
    // isn't.
    return start_ == nullptr;
  }
};

/*!
 * \brief     Checks if two vectors are equal.
 * \details   Two vectors are equal if they have the same size() and their elements compare equal.
 * \param[in] lhs  The first vector.
 * \param[in] rhs  The second vector.
 * \return    True if sizes and elements are equal, false otherwise.
 * \pre       -
 * \context   ANY
 * \vpublic
 * \steady    TRUE
 */
template <typename T, typename Alloc>
auto operator==(Vector<T, Alloc> const& lhs, Vector<T, Alloc> const& rhs) noexcept -> bool {
  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
  return (lhs.size() == rhs.size()) && std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin());
}

/*!
 * \brief     Checks if two vectors are not equal.
 * \details   Two vectors are equal if they have the same size() and their elements compare equal.
 * \param[in] lhs  The first vector.
 * \param[in] rhs  The second vector.
 * \return    False if sizes and elements are equal, true otherwise.
 * \pre       -
 * \context   ANY
 * \vpublic
 * \steady    TRUE
 */
template <typename T, typename Alloc>
auto operator!=(Vector<T, Alloc> const& lhs, Vector<T, Alloc> const& rhs) noexcept -> bool {
  return !(lhs == rhs);
}

/*!
 * \brief     Checks if the first vector is lexicographically lesser than the second vector.
 * \details   Vector a is lexicographically lesser than vector b, if one of the following applies:
 *            - a is empty and b is nonempty
 *            - a is a prefix of b
 *            - for the smallest index i where a[i] != b[i], a[i] < b[i] applies.
 * \param[in] lhs  The first vector.
 * \param[in] rhs  The second vector.
 * \return    True if \p lhs is lexicographically lesser than \p rhs, false otherwise.
 * \pre       -
 * \context   ANY
 * \vpublic
 * \steady    TRUE
 */
template <typename T, typename Alloc>
auto operator<(Vector<T, Alloc> const& lhs, Vector<T, Alloc> const& rhs) noexcept -> bool {
  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
  return std::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}

/*!
 * \brief     Checks if the first vector is lexicographically greater than the second vector.
 * \details   Vector a is lexicographically greater than vector b, iff b is lexicographically lesser than a.
 * \param[in] lhs  The first vector.
 * \param[in] rhs  The second vector.
 * \return    True if \p lhs is lexicographically greater than \p rhs, false otherwise.
 * \pre       -
 * \context   ANY
 * \vpublic
 * \steady    TRUE
 */
template <typename T, typename Alloc>
auto operator>(Vector<T, Alloc> const& lhs, Vector<T, Alloc> const& rhs) noexcept -> bool {
  return rhs < lhs;
}

/*!
 * \brief     Checks if the first vector is lexicographically lesser or equal than the second vector.
 * \details   Vector a is lexicographically lesser or equal than vector b, iff a is not lexicographically greater than
 *            b.
 * \param[in] lhs  The first vector.
 * \param[in] rhs  The second vector.
 * \return    True if \p lhs is lexicographically lesser or equal than \p rhs, false otherwise.
 * \pre       -
 * \context   ANY
 * \vpublic
 * \steady    TRUE
 */
template <typename T, typename Alloc>
auto operator<=(Vector<T, Alloc> const& lhs, Vector<T, Alloc> const& rhs) noexcept -> bool {
  return !(lhs > rhs);
}

/*!
 * \brief     Checks if the first vector is lexicographically greater or equal than the second vector.
 * \details   Vector a is lexicographically greater or equal than vector b, iff a is not lexicographically lesser than
 *            b.
 * \param[in] lhs  The first vector.
 * \param[in] rhs  The second vector.
 * \return    True if \p lhs is lexicographically greater or equal than \p rhs, false otherwise.
 * \pre       -
 * \context   ANY
 * \vpublic
 * \steady    TRUE
 */
template <typename T, typename Alloc>
auto operator>=(Vector<T, Alloc> const& lhs, Vector<T, Alloc> const& rhs) noexcept -> bool {
  return !(lhs < rhs);
}

/*!
 * \brief     Swaps the content of the two vectors.
 * \param[in] lhs  The first vector.
 * \param[in] rhs  The second vector.
 * \pre       -
 * \context   ANY
 * \vpublic
 * \steady    TRUE
 */
template <typename T, typename Alloc>
void swap(Vector<T, Alloc>& lhs, Vector<T, Alloc>& rhs) noexcept {
  lhs.swap(rhs);
}

}  // namespace amsr

#endif  // LIB_VAC_INCLUDE_AMSR_VECTOR_H_
