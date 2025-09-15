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
/*!        \file  ara/core/span.h
 *        \brief  SWS core type ara::core::Span.
 *         \unit  VaCommonLib::ContainerLibrary::ViewContainers::Span
 *
 *      \details  The container constitutes a view over a contiguous sequence of objects, the storage of which is
 *                owned by another object.
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_ARA_CORE_SPAN_H_
#define LIB_VAC_INCLUDE_ARA_CORE_SPAN_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include "ara/core/abort.h"
#include "ara/core/array.h"

namespace ara {
namespace core {
/*!
 * \brief A constant for creating Spans with dynamic sizes.
 */
constexpr std::size_t dynamic_extent{std::numeric_limits<std::size_t>::max()};

// Forward declaration of Span.
template <typename T, std::size_t Extent = dynamic_extent>
class Span;

namespace internal {
/*!
 * \brief  Expects class to check if the pointer and the number of elements to take from pointer for constructing a Span
 *         are valid.
 * \tparam T Type of the elements in Span.
 * \tparam Extent Extent of Span.
 * \vprivate
 */
template <typename T, std::size_t Extent>
class Expects final {
 public:
  /*!
   * \brief Default constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr Expects() noexcept = default;

  /*!
   * \brief Constructor to check if the pointer and the number of elements to take from pointer for constructing a Span
   *        are valid.
   * \param[in] ptr Pointer to construct the Span.
   * \param[in] count Number of elements to take from ptr.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr Expects(T* ptr, std::size_t count) noexcept {
    if (Extent != dynamic_extent) {
      if (count != Extent) {
        ara::core::Abort("ara::core::Span: Count shall be equal to Extent!");
      }
    }
    if ((ptr == nullptr) && (count != 0)) {
      ara::core::Abort("ara::core::Span: Nullpointer is only allowed for 0 extent!");
    }
  }
};

/*!
 * \brief  False type of is_Span helper struct.
 * \tparam T Type of the container to construct the Span.
 * \vprivate
 */
template <typename>
struct IsSpanOracle final : std::false_type {};

/*!
 * \brief  True type of is_Span helaper struct.
 * \tparam ElementType Type of the elements in Span.
 * \tparam Extent Extent of Span.
 * \vprivate
 */
template <typename ElementType, std::size_t Extent>
struct IsSpanOracle<Span<ElementType, Extent>> final : std::true_type {};

/*!
 * \brief  Helper struct to check if the container is a Span type.
 * \tparam T Type of the container to construct the Span.
 * \vprivate
 */
template <typename T>
using IsSpan = IsSpanOracle<std::remove_cv_t<T>>;

/*!
 * \brief  False type of is_array helaper struct.
 * \tparam T Type of the container to construct the Span.
 * \vprivate
 */
template <typename>
struct IsArrayOracle final : std::false_type {};

/*!
 * \brief  True type of is_array helaper struct.
 * \tparam ElementType Type of the elements in Span.
 * \tparam Extent Extent of Span.
 * \vprivate
 */
template <typename ElementType, std::size_t Extent>
struct IsArrayOracle<ara::core::Array<ElementType, Extent>> final : std::true_type {};

/*!
 * \brief  Helper struct to check if the container is a Array type.
 * \tparam T Type of the container to construct the Span.
 * \vprivate
 */
template <typename T>
using IsArray = IsArrayOracle<std::remove_cv_t<T>>;

/*!
 * \brief  Helper struct to calculate the type for subspan function.
 * \tparam ElementType Type of the elements in Span.
 * \tparam Extent Extent of Span.
 * \tparam Offset Offset of Span.
 * \tparam Count Number of elements to take from a Span.
 * \vprivate
 */
template <typename ElementType, std::size_t Extent, std::size_t Offset, std::size_t Count>
class CalculateSubSpanType {
 public:
  /*!
   * \brief A typedef for subspan parameters type.
   */
  using type = Span<ElementType, Count>;
};

/*!
 *  \brief Specialization for dynamic extent.
 * \vprivate
 */
template <typename ElementType, std::size_t Extent, std::size_t Offset>
class CalculateSubSpanType<ElementType, Extent, Offset, dynamic_extent> {
 public:
  /*!
   * \brief A typedef for subspan parameters type.
   */
  using type = Span<ElementType, (Extent != dynamic_extent) ? (Extent - Offset) : dynamic_extent>;
};

}  // namespace internal

// VECTOR Next Construct AutosarC++17_10-A12.0.1: MD_VAC_A12.0.1_missingMoveConstructorAssignmentOperator
// VECTOR Next Construct Metric-OO.WMC.One: MD_VAC_Metric-OO.WMC.One
// VECTOR Next Construct AutosarC++17_10-A12.4.2: MD_VAC_A12.4.2_classNotFinal
/*!
 * \brief  A view over a contiguous sequence of objects.
 * \tparam T Type of the elements in Span:
 *         -ElementType is required to be a complete object type that is not an abstract class type.
 *          (N4762: 21.7.3.1 Overview Subclause 2, referenced from AUTOSAR AP Release 19-03: 8.14 Span data type)
 *         -An object type is a (possibly cv-qualified) type that is not a function type, not a reference type,
 *          and not a void type.
 *          (ISO/IEC 14882:2014(E): 3.9 Types Subclause 8)
 *         -In current implementation of VaCommonLib, Volatile types are not supported for any container types as
 *          stated by AUTOSAR rule:A2-11-1.
 * \tparam Extent Extent of Span.
 * \trace  CREQ-VaCommonLib-Span
 * \vpublic
 */
template <typename T, std::size_t Extent>
class Span {
 public:
  /*!
   * \brief Alias for the type of elements in this Span.
   */
  using element_type = T;

  /*!
   * \brief Alias for the type of values in this Span.
   */
  using value_type = typename std::remove_cv<element_type>::type;

  /*!
   * \brief Alias for the type of parameters that indicate an index into the Span.
   */
  using index_type = std::size_t;

  /*!
   * \brief Alias for the type of parameters that indicate a difference of indexes into the Span.
   */
  using difference_type = std::ptrdiff_t;

  /*!
   * \brief Alias for the type of parameters that indicate a size or a number of values.
   */
  using size_type = index_type;

  /*!
   * \brief Alias type for a pointer to an element.
   */
  using pointer = element_type*;

  /*!
   * \brief Alias type for a reference to an element.
   */
  using reference = element_type&;

  /*!
   * \brief   The type of an iterator to elements.
   * \details This iterator shall implement the concepts RandomAccessIterator, ContiguousIterator, and
   *          ConstexprIterator.
   */
  using iterator = pointer;

  /*!
   * \brief   The type of an const_iterator to elements.
   * \details This iterator shall implement the concepts RandomAccessIterator, ContiguousIterator, and
   *          ConstexprIterator.
   */
  using const_iterator = typename std::remove_const<T>::type const*;

  /*!
   * \brief Alias type for a reverse_iterator to an element.
   */
  using reverse_iterator = std::reverse_iterator<iterator>;

  /*!
   * \brief Alias type for a const_reverse_iterator to an element.
   */
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  /*!
   * \brief A constant reflecting the configured Extent of this Span.
   */
  constexpr static index_type extent{Extent};

  /*!
   * \brief   Default constructor of Span.
   * \details This constructor shall not participate in overload resolution unless
   *          (Extent == 0 || Extent == dynamic_extent) is true.
   * \spec
   *   requires true;
   * \endspec
   * \vpublic
   */
  template <bool Dependent = false,
            typename = std::enable_if_t<Dependent || (Extent == 0) || (Extent == dynamic_extent)>>
  constexpr Span() noexcept : Span(nullptr, static_cast<std::size_t>(0)) {}  // VCA_VAC_VALID_CONSTRUCTOR_CALLED

  /*!
   * \brief     Construct a new Span from the given pointer and size.
   * \param[in] ptr Pointer to construct the Span.
   * \param[in] count Number of elements to take from ptr.
   * \details   Admissible are ptr == (ptr + count) or [ptr, ptr + count) shall be a valid range.
   *            If Extent is not equal to dynamic_extent, then count shall be equal to Extent.
   * \pre       -
   * \vpublic
   */
  // VCA_VAC_VALID_PTR_ASSIGNING_NOT_DEREFERENCING, VCA_VAC_VALID_CONSTRUCTOR_CALLED
  constexpr Span(pointer ptr, index_type count) noexcept : data_{ptr}, size_{(ptr == nullptr) ? 0 : count} {
    static_cast<void>(internal::Expects<T, Extent>{ptr, count});
  }

  // VECTOR Next Construct VectorC++-V3.9.2: MD_VAC_V3-9-2_parametersBasedOnBuiltInTypes
  /*!
   * \brief     Construct a new Span from the open range between [firstElem, lastElem).
   * \param[in] firstElem Pointer to pass the first element.
   * \param[in] lastElem Pointer to pass the last element (excluded).
   * \details   Admissible are firstElem == lastElem or [firstElem, lastElem) shall be a valid range.
   *            If extent is not equal to dynamic_extent, then (lastElem - firstElem) shall be equal to Extent.
   * \pre       [firstElem, lastElem) is a valid interval.
   * \vpublic
   */
  constexpr Span(pointer firstElem, pointer lastElem) noexcept
      : Span(firstElem, static_cast<std::size_t>(std::distance(firstElem, lastElem))) {}

  // VECTOR Next Construct AutosarC++17_10-A18.1.1: MD_VAC_A18.1.1_cStyleArraysShouldNotBeUsed
  // VECTOR Next Construct VectorC++-V3.0.1: MD_VAC_V3-0-1_cStyleArraysShouldNotBeUsed
  // VECTOR Next Construct VectorC++-V12.1.4: MD_VAC_V12-1-4_constructorShallBeDeclaredExplicit
  /*!
   * \brief     Construct a new Span from the given raw array.
   * \tparam    N The size of the raw array.
   * \param[in] arr The raw array.
   * \details   This constructor shall not participate in overload resolution unless: Extent == dynamic_extent || N ==
   *            Extent is true, and std::remove_pointer<decltype(ara::core::data(arr))>::type(*)[] is convertible to
   *            T(*)[].
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \vpublic
   */
  template <std::size_t N, typename = std::enable_if_t<((Extent == dynamic_extent) || (N == Extent))>>
  constexpr Span(element_type (&arr)[N]) noexcept : Span(std::addressof(arr[0]), N) {}

  // VECTOR Next Construct VectorC++-V12.1.4: MD_VAC_V12-1-4_constructorShallBeDeclaredExplicit
  /*!
   * \brief     Construct a new Span from the given Array.
   * \tparam    N The size of the Array.
   * \param[in] arr The Array.
   * \details   This constructor shall not participate in overload resolution unless: Extent == dynamic_extent || N ==
   *            Extent is true, and std::remove_pointer<decltype(ara::core::data(arr))>::type(*)[] is convertible to
   *            T(*)[].
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \vpublic
   */
  template <std::size_t N, typename = std::enable_if_t<((Extent == dynamic_extent) || (N == Extent))>>
  constexpr Span(ara::core::Array<value_type, N>& arr) noexcept : Span(arr.data(), arr.size()) {}

  // VECTOR Next Construct VectorC++-V12.1.4: MD_VAC_V12-1-4_constructorShallBeDeclaredExplicit
  /*!
   * \brief   Construct a new Span from the given const Array.
   * \tparam  N The size of the Array.
   * \param   arr The Array.
   * \details This constructor shall not participate in overload resolution unless:
   *          Extent == dynamic_extent || N == Extent is true,
   *          and std::remove_pointer<decltype(ara::core::data(arr))>::type(*)[] is convertible to T(*)[].
   * \pre     -
   * \spec
   *   requires true;
   * \endspec
   * \vpublic
   */
  template <std::size_t N, typename = std::enable_if_t<((Extent == dynamic_extent) || (N == Extent))>>
  constexpr Span(ara::core::Array<value_type, N> const& arr) noexcept : Span(arr.data(), arr.size()) {}

  // VECTOR Next Construct VectorC++-V12.1.4: MD_VAC_V12-1-4_constructorShallBeDeclaredExplicit
  /*!
   * \brief     Construct a new Span from the given container.
   * \tparam    Container The type of the container.
   * \param[in] cont The container.
   * \details   Admissible are ara::core::data(cont) == (ara::core::data(cont) + ara::core::size(cont))
   *            or [ara::core::data(cont), ara::core::data(cont) + ara::core::size(cont)) shall be a valid range.
   *            If Extent is not equal to dynamic_extent, then ara::core::size(cont) shall be equal to Extent.
   *            These constructors shall not participate in overload resolution unless: Container is not a
   *            specialization of Span, Container is not a specialization of Array, std::is_array<Container>::value is
   *            false, ara::core::data(cont) and ara::core::size(cont) are both well-formed,
   *            and std::remove_pointer<decltype(ara::core::data(cont))>::type(*)[] is convertible to T(*)[].
   * \pre       -
   * \vpublic
   */
  template <
      typename Container,
      typename = std::enable_if_t<
          ((!internal::IsSpan<Container>::value) && (!internal::IsArray<Container>::value)) &&
          (std::is_convertible<std::remove_pointer_t<decltype(std::declval<Container>().data())>*, pointer>::value)>>
  constexpr Span(Container& cont) noexcept : Span(cont.data(), cont.size()) {}

  // VECTOR Next Construct VectorC++-V12.1.4: MD_VAC_V12-1-4_constructorShallBeDeclaredExplicit
  /*!
   * \brief   Construct a new Span from the given const container.
   * \tparam  Container The type of the container.
   * \param   cont The container.
   * \details Admissible are ara::core::data(cont) == (ara::core::data(cont) + ara::core::size(cont))
   *          or [ara::core::data(cont), ara::core::data(cont) + ara::core::size(cont)) shall be a valid range.
   *          If Extent is not equal to dynamic_extent, then ara::core::size(cont) shall be equal to Extent.
   *          These constructors shall not participate in overload resolution unless: Container is not a
   *          specialization of Span, Container is not a specialization of Array, std::is_array<Container>::value is
   *          false,ara::core::data(cont) and ara::core::size(cont) are both well-formed,
   *          and std::remove_pointer<decltype(ara::core::data(cont))>::type(*)[] is convertible to T(*)[].
   * \pre     -
   * \vpublic
   */
  template <
      typename Container,
      typename = std::enable_if_t<
          ((!internal::IsSpan<Container>::value) && (!internal::IsArray<Container>::value)) &&
          (std::is_convertible<std::remove_pointer_t<decltype(std::declval<Container>().data())>*, pointer>::value)>>
  constexpr Span(Container const& cont) noexcept : Span(cont.data(), cont.size()) {}

  // VECTOR Next Construct AutosarC++17_10-A12.8.6:MD_VAC_A12.8.6_copyMoveConstructorAssignmentShallBeProtectedOrDeleted
  /*!
   * \brief Copy constructor.
   * \param other The other instance.
   * \spec
   *   requires true;
   * \endspec
   * \vpublic
   */
  constexpr Span(Span const& other) noexcept = default;

  // clang-format off
  // VECTOR Next Construct AutosarC++17_10-A18.1.1: MD_VAC_A18.1.1_cStyleArraysShouldNotBeUsed
  // VECTOR Next Construct VectorC++-V3.0.1: MD_VAC_V3-0-1_cStyleArraysShouldNotBeUsed
  // VECTOR Next Construct VectorC++-V12.1.4: MD_VAC_V12-1-4_constructorShallBeDeclaredExplicit
  /*!
   * \brief   Converting constructor.
   * \tparam  U The type of elements within the other Span.
   * \tparam  N The Extent of the other Span.
   * \param   s The other Span instance.
   * \details This constructor allows construction of a Span with const-qualified element type from a Span with
   *          nonconst-qualified element type, and also of a dynamic_extent-Span<> from a static extent-one.
   * \pre     -
   * \spec
   *   requires true;
   * \endspec
   * \vpublic
   */
  template <typename U, std::size_t N,
            typename = std::enable_if_t<((Extent == dynamic_extent) || (Extent == N)) &&
                                        (std::is_convertible<U(*)[], element_type(*)[]>::value)>>
  constexpr Span(Span<U, N> const& s) noexcept : Span(s.data(), s.size()) {}
  // clang-format on

  // VECTOR Next Construct AutosarC++17_10-A12.4.1: MD_VAC_A12.4.1_noVirtualDtor
  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_deletedFunction
  /*!
   * \brief Destructor.
   * \spec
   *   requires true;
   * \endspec
   * \vpublic
   */
  ~Span() noexcept = default;

  // VECTOR Next Construct AutosarC++17_10-A12.8.6:MD_VAC_A12.8.6_copyMoveConstructorAssignmentShallBeProtectedOrDeleted
  /*!
   * \brief  Copy assignment operator.
   * \param  other The other instance.
   * \return A reference to the assigned to Span.
   * \spec
   *   requires true;
   * \endspec
   * \vpublic
   */
  Span& operator=(Span const& other) & noexcept = default;

  /*!
   * \brief  Return a subspan containing only the first elements of this Span.
   * \tparam Count The number of elements to take over.
   * \pre    Count must be less than or equal to the size of this Span.
   * \return A subspan containing only the first elements of this Span.
   * \vpublic
   */
  template <std::size_t Count>
  constexpr auto first() const noexcept -> Span<element_type, Count> {
    bool const count_valid{Count <= this->size()};
    if (!count_valid) {
      ara::core::Abort("ara::core::Span::first<std::size_t>(): Count is invalid!");
    }
    return {this->data(), Count};
  }

  /*!
   * \brief     Return a subspan containing only the first elements of this Span.
   * \param[in] count The number of elements to take over.
   * \pre       count must be less than or equal to the size of this Span.
   * \return    A subspan containing only the first elements of this Span.
   * \vpublic
   */
  constexpr Span<element_type, dynamic_extent> first(index_type count) const noexcept {
    bool const count_valid{count <= this->size()};
    if (!count_valid) {
      ara::core::Abort("ara::core::Span::first(index_type): Count is invalid!");
    }
    return {this->data(), count};
  }

  /*!
   * \brief  Return a subspan containing only the last elements of this Span.
   * \tparam Count The number of elements to take over.
   * \pre    Count must be less than or equal to the size of this Span.
   * \return A subspan containing only the last elements of this Span.
   * \vpublic
   */
  template <std::size_t Count>
  constexpr auto last() const noexcept -> Span<element_type, Count> {
    bool const count_valid{Count <= this->size()};
    if (!count_valid) {
      ara::core::Abort("ara::core::Span::last<std::size_t>(): Count is invalid!");
    }
    // VECTOR Next Construct AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_pointerArithmetic
    // VECTOR Next Line AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmetic
    return Span<element_type, Count>(&this->data()[this->size() - Count], Count);
  }

  /*!
   * \brief     Return a subspan containing only the last elements of this Span.
   * \param[in] count The number of elements to take over.
   * \pre       count must be less than or equal to the size of this Span.
   * \return    A subspan containing only the last elements of this Span.
   * \vpublic
   */
  constexpr Span<element_type, dynamic_extent> last(index_type count) const noexcept {
    bool const count_valid{count <= this->size()};
    if (!count_valid) {
      ara::core::Abort("ara::core::Span::last(index_type): Count is invalid!");
    }
    return subspan(this->size() - count, count);
  }

  // VECTOR Next Construct AutosarC++17_10-M5.19.1: MD_VAC_M5.19.1_overflowDuringEvaluationOfConstantExpression
  /*!
   * \brief   Return a subspan of this Span.
   * \details The second template argument of the returned Span type is:
   *          Count != dynamic_extent ? Count : (Extent != dynamic_extent ? Extent - Offset : dynamic_extent).
   * \tparam  Offset Offset into this Span from which to start.
   * \tparam  Count The number of elements to take over.
   * \pre     Offset + Count must be less than or equal to size of this Span.
   * \return  A subspan of this Span.
   * \vpublic
   */
  template <std::size_t Offset, std::size_t Count>
  constexpr auto subspan() const noexcept ->
      typename internal::CalculateSubSpanType<element_type, Extent, Offset, Count>::type {
    bool const offset_count_valid{
        ((Offset == 0) || (this->size() >= Offset)) &&
        ((Count == dynamic_extent) || (((Offset + Count) == 0) || ((Offset + Count) <= this->size())))};
    if (!offset_count_valid) {
      ara::core::Abort("ara::core::Span::subspan<std::size_t, std::size_t>(): Offset and Count are invalid!");
    }
    // VECTOR Next Construct AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_pointerArithmetic
    // VECTOR Next Line AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmetic
    return {&this->data()[Offset], (Count == dynamic_extent) ? (this->size() - Offset) : Count};
  }

  /*!
   * \brief     Return a subspan of this Span.
   * \param[in] offset Offset into this Span from which to start.
   * \param[in] count The number of elements to take over.
   * \pre       offset + count must be less than or equal to size of this Span.
   * \return    A subspan of this Span.
   * \vpublic
   */
  constexpr Span<element_type, dynamic_extent> subspan(index_type offset,
                                                       index_type count = dynamic_extent) const noexcept {
    bool const offset_count_valid{(this->size() >= offset) &&
                                  ((count == dynamic_extent) || (count <= (this->size() - offset)))};
    if (!offset_count_valid) {
      ara::core::Abort("ara::core::Span::subspan(index_type, index_type): Offset and Count are invalid!");
    }
    // VECTOR NC AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmetic
    // VECTOR NL AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_pointerArithmetic
    return {&this->data()[offset], (count == dynamic_extent) ? (this->size() - offset) : count};
  }

  /*!
   * \brief  Return the size of this Span.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return The size of this Span.
   * \vpublic
   */
  constexpr index_type size() const noexcept { return size_; }

  /*!
   * \brief  Return the size of this Span in bytes.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return The size of this Span in bytes.
   * \vpublic
   */
  constexpr index_type size_bytes() const noexcept { return size() * sizeof(T); }

  /*!
   * \brief  Return whether this Span is empty.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return Whether this Span is empty.
   * \vpublic
   */
  constexpr bool empty() const noexcept { return size_ == 0; }

  /*!
   * \brief     Return a reference to the n-th element of this Span.
   * \param[in] idx The index into this Span.
   * \pre       idx must be valid, i.e. 0 <= idx < size().
   * \return    A reference to the n-th element of this Span.
   * \vpublic
   */
  reference operator[](index_type idx) const noexcept {
    if (idx >= size_) {
      ara::core::Abort("ara::core::Span::operator[]: Out of range access!");
    }
    // VECTOR Disable AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_arrayAccessOutOfBounds
    // VECTOR NL AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmetic
    return data_[idx];  // VCA_VAC_VALID_RETURN_VALUE_RUN_TIME_CHECK
    // VECTOR Enable AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_arrayAccessOutOfBounds
  }

  /*!
   * \brief  Return a pointer to the start of the memory block covered by this Span.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return A pointer to the start of the memory block covered by this Span.
   * \vpublic
   */
  constexpr pointer data() const noexcept { return data_; }

  /*!
   * \brief  Return an iterator to the first element of this Span.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return An iterator to the first element of this Span.
   * \vpublic
   */
  constexpr iterator begin() const noexcept { return data(); }

  /*!
   * \brief  Return an iterator to the last element of this Span.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return An iterator to the last element of this Span.
   * \vpublic
   */
  constexpr iterator end() const noexcept {
    iterator ret_value{nullptr};
    if (data_ == nullptr) {
      ret_value = nullptr;
    } else {
      // VECTOR Next Line AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmetic
      ret_value = &data_[size_];
    }
    return ret_value;
  }

  /*!
   * \brief  Return a const_iterator to the first element of this Span.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return A const_iterator to the first element of this Span.
   * \vpublic
   */
  constexpr const_iterator cbegin() const noexcept { return data_; }

  /*!
   * \brief  Return a const_iterator to the last element of this Span.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return A const_iterator to the last element of this Span.
   * \vpublic
   */
  constexpr const_iterator cend() const noexcept {
    // VECTOR Next Line AutosarC++17_10-M5.0.15: MD_VAC_M5.0.15_pointerArithmetic
    return (this->data_ == nullptr) ? nullptr : &this->data_[this->size_];
  }

  /*!
   * \brief  Return a reverse_iterator to the last element of this Span.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return A reverse_iterator to the last element of this Span.
   * \vpublic
   */
  constexpr reverse_iterator rbegin() const noexcept { return reverse_iterator{end()}; }

  /*!
   * \brief  Return a reverse_iterator to the first element of this Span.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return A reverse_iterator to the first element of this Span.
   * \vpublic
   */
  constexpr reverse_iterator rend() const noexcept { return reverse_iterator{begin()}; }

  /*!
   * \brief  Return a const_reverse_iterator to the last element of this Span.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return A const_reverse_iterator to the last element of this Span.
   * \vpublic
   */
  constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator{cend()}; }

  /*!
   * \brief  Return a const_reverse_iterator to the first element of this Span.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return A const_reverse_iterator to the first element of this Span.
   * \vpublic
   */
  constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator{cbegin()}; }

 protected:
  // VECTOR Next Construct VectorC++-V11.0.2: MD_VAC_V11-0-2_mutableMemberShallBePrivate
  /*!
   * \brief Pointer to the contained data.
   */
  pointer data_;

  // VECTOR Next Construct VectorC++-V11.0.2: MD_VAC_V11-0-2_mutableMemberShallBePrivate
  /*!
   * \brief Size of the contained data.
   */
  size_type size_;
};

/*!
 * \brief     Create a new Span from the given pointer and size.
 * \tparam    T The type of elements.
 * \param[in] ptr The pointer.
 * \param[in] count The number of elements to take from ptr.
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \return    A new Span from the given pointer and size.
 * \vpublic
 */
template <typename T>
constexpr auto MakeSpan(T* ptr, typename Span<T>::index_type count) noexcept -> Span<T> {
  return Span<T>(ptr, count);
}

// VECTOR Next Construct VectorC++-V3.9.2: MD_VAC_V3-9-2_parametersBasedOnBuiltInTypes
/*!
 * \brief     Create a new Span from the open range between [firstElem, lastElem).
 * \tparam    T The type of elements.
 * \param[in] firstElem Pointer to pass the first element.
 * \param[in] lastElem Pointer to pass the last element (excluded).
 * \pre       [firstElem, lastElem) is a valid interval.
 * \spec
 *   requires true;
 * \endspec
 * \return    A new Span from the open range between [firstElem, lastElem).
 * \vpublic
 */
template <typename T>
constexpr auto MakeSpan(T* firstElem, T* lastElem) noexcept -> Span<T> {
  return Span<T>(firstElem, lastElem);
}

// VECTOR Next Construct VectorC++-V3.9.2: MD_VAC_V3-9-2_parametersBasedOnBuiltInTypes
// VECTOR Next Construct AutosarC++17_10-A18.1.1: MD_VAC_A18.1.1_cStyleArraysShouldNotBeUsed
// VECTOR Next Construct VectorC++-V3.0.1: MD_VAC_V3-0-1_cStyleArraysShouldNotBeUsed
/*!
 * \brief     Create a new Span from the given raw array.
 * \tparam    T The type of elements.
 * \tparam    N The size of the raw array.
 * \param[in] arr The raw array.
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \return    A new Span from the given raw array.
 * \vpublic
 */
template <typename T, std::size_t N>
constexpr auto MakeSpan(T (&arr)[N]) noexcept -> Span<T, N> {
  return Span<T, N>(arr);
}

/*!
 * \brief     Create a new Span from the given container.
 * \tparam    Container The type of the container.
 * \param[in] cont The container.
 * \pre       -
 * \spec
 *   requires true;
 * \endspec
 * \return    A new Span from the given container.
 * \vpublic
 */
template <typename Container>
constexpr auto MakeSpan(Container& cont) noexcept -> Span<typename Container::value_type> {
  return Span<typename Container::value_type>(cont);
}

/*!
 * \brief  Create a new Span from the given const container.
 * \tparam Container The type of the container.
 * \param  cont The container.
 * \pre    -
 * \spec
 *   requires true;
 * \endspec
 * \return A new Span from the given const container.
 * \vpublic
 */
template <typename Container>
constexpr auto MakeSpan(Container const& cont) noexcept -> Span<typename Container::value_type const> {
  return Span<typename Container::value_type const>(cont);
}

}  // namespace core
}  // namespace ara

#endif  // LIB_VAC_INCLUDE_ARA_CORE_SPAN_H_
