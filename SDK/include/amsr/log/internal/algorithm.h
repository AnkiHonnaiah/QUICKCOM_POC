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
/**     \file       algorithm.h
 *      \brief
 *      \details
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_ALGORITHM_H_
#define LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_ALGORITHM_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>

#include "amsr/core/vector.h"
#include "amsr/log/internal/ref.h"
#include "amsr/log/internal/utils.h"
#include "ara/core/vector.h"

namespace amsr {
namespace log {
namespace internal {

/*!
 * \brief An entry that's returned from finding
 * \tparam Container The container type that was searched.
 *                   Container::iterator and Container::const_iterator must satisfy the
 *                   requirements of iterator types of the 'Container' named requirement.
 *
 * \unit LogAndTrace::Common::Entry
 */
template <typename Container>
class Entry final {
  /*! \brief A reference to the container that contains the entry. */
  Ref<Container> container_;
  /*! \brief The underlying iterator. */
  typename Container::iterator iterator_;

 public:
  /*!
   * \brief Creates the entry from the container & iterator
   * \param container The container which was searched
   * \param it        The iterator that marks if a value was found
   * \spec
   *   requires true;
   * \endspec
   */
  Entry(Container& container, typename Container::iterator it) noexcept : container_{container}, iterator_{it} {}
  /*!
   * \brief Allow moving.
   * \spec
   *   requires true;
   * \endspec
   */
  Entry(Entry&&) noexcept = default;
  /*!
   * \brief Allow moving.
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator=(Entry&&) & noexcept -> Entry& = default;
  /*!
   * \brief Allow copying.
   * \spec
   *   requires true;
   * \endspec
   */
  Entry(Entry const&) noexcept = default;
  /*!
   * \brief Allow copying.
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator=(Entry const&) & noexcept -> Entry& = default;
  /*!
   * \brief Default destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  ~Entry() noexcept = default;

  /*!
   * \brief Access the value by mimicking an iterator
   * \return The value at the entry
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator*() const& noexcept -> typename Container::reference { return *this->iterator_; }

  /*!
   * \brief Access the value by mimicking an iterator
   * \return The pointer to the entry
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator->() const& noexcept -> typename Container::pointer { return std::addressof(*(this->iterator_)); }

  /*!
   * \brief Get the underlying mutable iterator
   * \return The underlying mutable iterator
   * \spec
   *   requires true;
   * \endspec
   */
  auto IterMut() const noexcept -> typename Container::iterator { return this->iterator_; }

  /*!
   * \brief   Get the index for the entry
   *
   * \tparam  IndexType   The type that should be returned
   *                      Shall be a type convertible from the type returned by std::distance
   *                      for a Container::const_iterator.
   *
   * \return  The index for the entry
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename IndexType = std::ptrdiff_t>
  auto Index() const noexcept -> IndexType {
    using std::begin;
    return static_cast<IndexType>(
        // VCA_LOGAPI_VALID_OBJECT
        std::distance<typename Container::const_iterator>(begin(*this->container_), this->iterator_));
  }

  /*!
   * \brief Returns true if the search was successful
   * \return True if the search was successful
   * \spec
   *   requires true;
   * \endspec
   */
  auto Exists() const noexcept -> bool {
    using std::end;
    // VCA_LOGAPI_VALID_OBJECT
    return this->iterator_ != end(*this->container_);
  }

  /*!
   * \brief Conversion to boolean
   * \return True if the operation was successful
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NC AutosarC++17_10-A13.5.2: MD_LOG_AutosarC++17_10-A13.5.2_implicit_conversion
  explicit operator bool() const noexcept { return this->Exists(); }
};

/*!
 * \brief   Object wrapper for a back inserter
 * \tparam  Container   The container for which to create the inserter.
 *                      Must implement the 'Container' named requirement.
 *
 * \unit LogAndTrace::Common::BackInserter
 */
template <typename Container>
class BackInserter final {
 public:
  /*!
   * \brief The call operator creating the iterator
   * \param container The container to insert
   * \return The inserter iterator
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator()(Container& container) noexcept -> std::back_insert_iterator<Container> {
    return std::back_inserter(container);
  }
};

/*!
 * \brief   Searches for an element that is equal to the passed one.

 * \tparam  Container The container type to search.
 *                    Must meet the requirements of amsr::log::internal::Entry<Container>.
 * \tparam  T         The value type to search for.
 *                    Must meet the requirements of the EqualityComparable named requirement
 *                    with the type that Container holds.
 *
 * \param   container The container to search
 * \param   value     The value to search for
 *
 * \return  An entry for the search result
 *
 * \spec
 *   requires true;
 * \endspec
 */
template <typename Container, typename T>
auto Find(Container& container, T const& value) noexcept -> Entry<Container> {
  using std::begin;
  using std::end;

  typename Container::iterator const b{begin(container)};
  typename Container::iterator const e{end(container)};
  return Entry<Container>{container, std::find(b, e, value)};
}

/*!
 * \brief   Searches for an element for which predicate p returns true
 *
 * \tparam  Container       The container type to search.
 *                          Must meet the requirements of amsr::log::internal::Entry<Container>.
 * \tparam  UnaryPredicate  The function type to search with.
 *                          Must meet the requirements of UnaryPredicate for
 *                          std::find_if<InputIt, UnaryPredicate>(InputIt, InputIt, UnaryPredicate).
 *
 * \param   container       The container to search
 * \param   p               The predicate function to search with
 *
 * \return  An entry to the element that matches
 *
 * \spec
 *   requires true;
 * \endspec
 */
template <typename Container, typename UnaryPredicate>
auto FindIf(Container& container, UnaryPredicate p) noexcept -> Entry<Container> {
  using std::begin;
  using std::end;

  typename Container::iterator const b{begin(container)};
  typename Container::iterator const e{end(container)};
  return Entry<Container>{container, std::find_if(b, e, p)};
}

/*!
 * \brief   Copies a container to an iterator
 *
 * \tparam  Container   The type of container to copy.
 *                      Must be a type such that std::begin<Container> and std::end<Container>
 *                      returns valid iterators to std::copy<InputIt, OutputIt>(InputIt, InputIt, OutputIt).
 * \tparam  OutputIt    The type of iterator to copy to.
 *                      Must meet the requirements of OutputIt for
 *                      std::copy<InputIt, OutputIt>(InputIt, InputIt, OutputIt).
 *
 * \param   container   The container to copy
 * \param   d_first     The iterator to copy to
 *
 * \return  The iterator position after the last inserted value
 *
 * \spec
 *   requires true;
 * \endspec
 */
template <typename Container, typename OutputIt>
auto Copy(Container const& container, OutputIt d_first) noexcept -> OutputIt {
  using std::begin;
  using std::end;
  // VCA_LOGAPI_CHECKED_CONTRACT
  return std::copy(begin(container), end(container), d_first);
}

/*!
 * \brief   Copies a container to another container
 *
 * \tparam  In            The type of container to copy.
 *                        Must meet the requirements of Container for
 *                        amsr::log::internal::Copy<Container, OutputIt>(Container cont&, OutputIt).
 * \tparam  Out           The type of container to copy to.
 *                        Must implement the Container named requirement.
 *
 * \tparam  Inserter      The type of inserter to use to insert elements.
 *                        Must be a callable that takes a Container and returns a valid type
 *                        that meets the requirements of OutputIt for
 *                        amsr::log::internal::Copy<Container, OutputIt>(Container cont&, OutputIt).
 *
 * \param   in_container  The container to copy
 * \param   out_container The container to copy to
 * \param   inserter      The inserter to use to insert elements
 *
 * \spec
 *   requires true;
 * \endspec
 */
template <typename In, typename Out, typename Inserter = BackInserter<Out>>
void CopyContainer(In&& in_container, Out& out_container, Inserter inserter = BackInserter<Out>{}) noexcept {
  Drop(Copy(std::forward<In>(in_container), inserter(out_container)));
}

/*!
 * \brief   Applies a function to a container and stores the result into a range.
 *
 * \tparam  Container       The type of container to transform.
 *                          Must implement the Container named requirement.
 * \tparam  OutputIt        The type of iterator to store the result.
 *                          Must meet the requirements of OutputIt for
 *                          std::transfrom<InputIt,
 *                                         OutputIt,
 *                                         UnaryOperation>(InputIt, InputIt, OutputIt, UnaryOperation).
 * \tparam  UnaryOperation  The function to apply to every element
 *                          Must meet the requirements of UnaryOperation for
 *                          std::transfrom<InputIt,
 *                                         OutputIt,
 *                                         UnaryOperation>(InputIt, InputIt, OutputIt, UnaryOperation).
 *
 * \param   container       The container to transform
 * \param   d_first         The iterator to store the result
 * \param   unary_op        The function to apply
 *
 * \return  The iterator position after the last transformed value
 *
 * \spec
 *   requires true;
 * \endspec
 */
template <typename Container, typename OutputIt, typename UnaryOperation>
// VECTOR NC AutosarC++17_10-A15.5.3: MD_LOG_AutosarC++17_10-A15.5.3
// VECTOR NC AutosarC++17_10-A15.4.2: MD_LOG_AutosarC++17_10-A15.4.2
auto Transform(Container&& container, OutputIt d_first, UnaryOperation unary_op) noexcept -> OutputIt {
  using std::begin;
  using std::end;
  // VCA_LOGAPI_CHECKED_CONTRACT
  return std::transform(begin(container), end(container), d_first, unary_op);
}

/*!
 * \brief   Applies a function to a container and stores the result into another container.
 *
 * \tparam  InContainer     The type of container to transform.
 *                          Must meet the requirements of Container for
 *                          amsr::log::internal::Transform<Container,
 *                                                         OutputIt,
 *                                                         UnaryOperation>(Container&&, OutputIt, UnaryOperation).
 * \tparam  OutContainer    The type of container to store the result.
 *                          Must meet the requirements of Container for std::back_inserter<Container>(Container&).
 * \tparam  UnaryOperation  The function to apply to every element.
 *                          Must meet the requirements of UnaryOperation for
 *                          amsr::log::internal::Transform<Container,
 *                                                         OutputIt,
 *                                                         UnaryOperation>(Container&&, OutputIt, UnaryOperation).
 * \param   in_container    The container to transform
 * \param   out_container   The container to store the result
 * \param   unary_op        The function to apply
 *
 * \spec
 *   requires true;
 * \endspec
 */
template <typename InContainer, typename OutContainer, typename UnaryOperation>
void TransformContainer(InContainer&& in_container, OutContainer& out_container, UnaryOperation unary_op) noexcept {
  Drop(Transform(std::forward<InContainer>(in_container), std::back_inserter(out_container), unary_op));
}

/*!
 * \brief   Applies a function to a container and stores the result into another container.
 *
 * \tparam  InContainer     The type of container to transform.
 *                          A container that holds a type T that meets the requirements of U for ara::core::Vector<U>.
 *                          Must meet the requirements of Container for
 *                          amsr::log::internal::Transform<Container,
 *                                                         OutputIt,
 *                                                         UnaryOperation>(Container&&, OutputIt, UnaryOperation).
 * \tparam  UnaryOperation  The function to apply to every element.
 *                          Must meet the requirements of UnaryOperation for
 *                          amsr::log::internal::Transform<Container,
 *                                                         OutputIt,
 *                                                         UnaryOperation>(Container&&, OutputIt, UnaryOperation).
 *
 * \param   in_container    The container to transform
 * \param   out_container   The container to store the result
 * \param   unary_op        The function to apply
 *
 * \spec
 *   requires true;
 * \endspec
 */
template <typename InContainer, typename UnaryOperation>
// VECTOR NC AutosarC++17_10-A15.5.3: MD_LOG_AutosarC++17_10-A15.5.3
// VECTOR NC AutosarC++17_10-A15.4.2: MD_LOG_AutosarC++17_10-A15.4.2
// VECTOR NC AutosarC++17_10-A13.3.1: MD_LOG_AutosarC++17_10-A13.3.1
auto Transform(InContainer&& in_container, UnaryOperation unary_op) noexcept
    -> ara::core::Vector<std::decay_t<decltype(unary_op(*in_container.begin()))>> {
  ara::core::Vector<std::decay_t<decltype(unary_op(*in_container.begin()))>> ret_val{};
  // VCA_LOGAPI_CHECKED_CONTRACT
  ret_val.reserve(in_container.size());
  Drop(Transform(std::forward<InContainer>(in_container), std::back_inserter(ret_val), unary_op));
  return ret_val;
}

}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_ALGORITHM_H_
