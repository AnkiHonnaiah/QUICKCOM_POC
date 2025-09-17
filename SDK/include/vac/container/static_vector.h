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
/*!        \file
 *        \brief  Static vector manages elements in contiguous memory.
 *         \unit  VaCommonLib::ContainerLibrary::StaticContainers::StaticVector
 *
 *      \details  A contiguous memory is allocated from the allocator StaticVector::reserve(). The memory segment will
 *                be deallocated when the object is destroyed. Besides, the elements in the vector can be removed or
 *                added.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_CONTAINER_STATIC_VECTOR_H_
#define LIB_VAC_INCLUDE_VAC_CONTAINER_STATIC_VECTOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include <memory>
#include <utility>
#include "ara/core/abort.h"
#include "ara/core/span.h"

namespace vac {
namespace container {

// VECTOR Next Construct Metric-OO.WMC.One: MD_VAC_Metric-OO.WMC.One
/*!
 * \brief           Class to (owning) manage a contiguous memory segment. The size of the memory segment can be set
 *                  once. The memory segment is allocated when calling StaticVector::reserve() and is deallocated when
 *                  the object is destroyed. Allocations/Deallocations are performed from the given allocator
 * \tparam          T
 *                  Typedef for the contained element. Internal usage of ara::core::Span<T> adds limitations for T:
 *                  -T is required to be a complete object type that is not an abstract class type. An object type is a
 *                    (possibly cv-qualified) type that is not a function type, not a reference type, and not a void
 *                    type. In current implementation of VaCommonLib, Volatile types are not supported for any container
 *                    types.(CDD ara::core::Span.) Alloc adds limitations for T:
 *                  -T must be cv-unqualified. (ISO/IEC 14882:2014(E): 17.6.3.5 Allocator requirements, Table 27)
 *                    (ISO/IEC 14882:2014(E): 3.9 Types, Subclause 8)
 * \tparam          Alloc
 *                  is an allocator which fulfill C++ Named Requirements: Allocator. The default type of Alloc is
 *                  std::allocator<T>.
 * \trace           CREQ-VaCommonLib-StaticVector
 */
template <typename T, typename Alloc = std::allocator<T>>
class StaticVector final {
 public:
  /*! \brief          Typedef for the ara::core::Span used internally */
  using span_type = ara::core::Span<T>;

  /*! \brief          Typedef for the size value */
  using size_type = typename span_type::size_type;

  /*! \brief          Typedef for the allocator used */
  using allocator_type = Alloc;

  /*! \brief          Type of the allocator traits */
  using allocator_traits = std::allocator_traits<allocator_type>;

  /*! \brief          Typedef for the allocator type used after rebinding */
  using actual_allocator_type = typename allocator_traits::template rebind_alloc<T>;

  /*! \brief          Typedef for the contained element */
  using value_type = typename span_type::value_type;

  /*! \brief          Typedef for a pointer */
  using pointer = typename span_type::pointer;

  /*! \brief          Typedef for a const pointer */
  using const_pointer = T const*;

  /*! \brief          Typedef for a reference */
  using reference = typename span_type::reference;

  /*! \brief          Typedef for a const reference */
  using const_reference = T const&;

  /*!
   * \brief           Typedef for an iterator
   * \trace           CREQ-VaCommonLib-StaticVector
   */
  using iterator = typename span_type::iterator;

  /*! \brief          Typedef for a const iterator */
  using const_iterator = typename span_type::const_iterator;

  /*!
   * \brief           Initialize a StaticVector
   * \param           allocator
   *                  The allocator to use for initialization, default is allocator_type().
   * \pre             -
   * \spec
   * requires true;
   * \endspec
   */
  explicit StaticVector(allocator_type const& allocator = allocator_type()) noexcept(true)
      // VCA_VAC_STD_FUNC_FULFILL_CONTRACT, VCA_VAC_VALID_ASSIGNING_NOT_ACCESSING, VCA_VAC_VALID_CONSTRUCTOR_CALLED
      : delegate_(), allocator_(allocator), max_num_elements_(0) {}

  /*! \brief          Default copy constructor deleted */
  StaticVector(StaticVector const&) = delete;

  /*! \brief          Default copy assignment operator deleted */
  StaticVector& operator=(StaticVector const&) = delete;

  // VECTOR Disable AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  // VECTOR Disable AutosarC++17_10-A12.8.4: MD_VAC_A12.8.4_moveConstructorShallNotUseCopySemantics
  /*!
   * \brief           Move constructor
   * \param[in]       other
   *                  The vector from which to construct the new vector.
   * \pre             -
   * \spec
   * requires true;
   * \endspec
   */
  StaticVector(StaticVector&& other) noexcept : StaticVector() { this->swap(other); }
  // VECTOR Enable AutosarC++17_10-A12.8.4

  /*!
   * \brief           Move Assignment
   * \param[in]       other
   *                  The vector from which to construct the new vector.
   * \return          A reference to the assigned-to object.
   * \pre             -
   * \spec
   * requires true;
   * \endspec
   */
  StaticVector& operator=(StaticVector&& other) & noexcept {
    this->swap(other);  // VCA_VAC_VALID_NONSTATIC_METHOD, VCA_VAC_VALID_ARGUMENT
    return *this;       // VCA_VAC_VALID_DEREFERENCED_POINTER_ACCESS
  }

  /*!
   * \brief           Destructor
   * \details         Terminates if the allocator throws an exception when deallocating the elements.
   * \pre             -
   */
  ~StaticVector() noexcept {  // VCA_VAC_VALID_NONSTATIC_METHOD
    shorten(0);               // VCA_VAC_VALID_NONSTATIC_METHOD
    if (delegate_.data() != nullptr) {
      allocator_.deallocate(delegate_.data(), max_num_elements_);  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    }
  }  // VCA_VAC_VALID_NONSTATIC_METHOD, VCA_VAC_STD_FUNC_FULFILL_CONTRACT
  // VECTOR Enable AutosarC++17_10-A15.5.1

  /*!
   * \brief           Allocate a memory segment from the allocator
   * \details         Terminates if the allocator throws an exception when allocating new elements.
   * \param           num_elements
   *                  The number of elements of T to allocate.
   * \pre             reserve() may only be called once. A second call triggers ara::core::Abort.
   * \trace           CREQ-VaCommonLib-StaticVector
   */
  void reserve(size_type num_elements) noexcept {
    if (delegate_.data() != nullptr) {
      ara::core::Abort("vac::container::StaticVector::reserve(size_type): The memory has already been reserved!");
    }
    max_num_elements_ = num_elements;
    pointer const p{allocator_.allocate(num_elements)};
    if (p == nullptr) {
      ara::core::Abort("vac::container::StaticVector::reserve: Allocation failed!");
    }
    delegate_ = span_type(p, static_cast<std::size_t>(0));
  }

  /*!
   * \brief           Set the vector to be of a certain size
   * \param           num_elements
   *                  The number of elements of T to allocate. Type T must meet C++ Named Requirements:
   *                  DefaultConstructible. Note: currently, resize() implicitly calls reserve() if it increases the
   *                  size.
   * \pre             num_elements <= maximum value for std::ptrdiff_t, otherwise ara::core::Abort will be triggered.
   */
  void resize(size_type num_elements) noexcept {
    if (num_elements > size_type{std::numeric_limits<std::ptrdiff_t>::max()}) {
      ara::core::Abort("vac::container::StaticVector::resize(size_type): Size exceeds allowed limit!");
    }
    if (num_elements > max_num_elements_) {
      reserve(num_elements);
    }

    // Resize the delegate if resizing is needed.
    if (num_elements > delegate_.size()) {
      // Record previous size of the container.
      size_type const old_num_elements{delegate_.size()};

      // Extend the delegate.
      delegate_ = span_type(delegate_.data(), num_elements);

      // Cache the end iterator. We do not modify the size of the vector here
      // and during profiling, the end() operation turned out to be fairly
      // costly for long vectors.
      iterator const end_it{end()};

      // Initialize new elements.
      // VECTOR Next Construct AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_lastElementCompare
      // VECTOR Next Construct AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_castMayTruncateValue
      for (iterator it{std::next(begin(), static_cast<std::ptrdiff_t>(old_num_elements))}; it != end_it; ++it) {
        allocator_.construct(it);  // VCA_VAC_VALID_NONSTATIC_METHOD
      }
    } else {
      // Destroy excess elements.
      shorten(num_elements);
    }
  }

  /*!
   * \brief           Shorten the vector to the given size
   * \details         Terminates if a removed element throws in its destructor.
   * \param           num_elements
   *                  The number elements of T the vector shall be shortened to.
   * \pre             num_elements <= maximum value for std::ptrdiff_t, otherwise ara::core::Abort will be triggered.
   * \note            Does nothing if num_elements is not smaller than size().
   */
  void shorten(size_type num_elements) noexcept {
    if (num_elements > size_type{std::numeric_limits<std::ptrdiff_t>::max()}) {
      ara::core::Abort("vac::container::StaticVector::shorten(size_type): Size exceeds allowed limit!");
    }
    // will this #elements actually shorten?
    if (num_elements < delegate_.size()) {
      // Cache the end iterator. We do not modify the size of the vector here
      // and during profiling, the end() operation turned out to be fairly
      // costly for long vectors.
      iterator const end_it{end()};

      // VECTOR Next Construct AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_lastElementCompare
      // VECTOR Next Construct AutosarC++17_10-M0.3.1: MD_VAC_M0.3.1_castMayTruncateValue
      for (iterator it{std::next(begin(), static_cast<std::ptrdiff_t>(num_elements))}; it != end_it; ++it) {
        allocator_.destroy(it);  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
      }

      delegate_ = span_type(delegate_.data(), num_elements);  // VCA_VAC_VALID_PTR_ASSIGNING_NOT_DEREFERENCING
    }
  }

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief           Swap the contents of two StaticVector objects including their allocator
   * \param[in]       other
   *                  The second vector whose contents are swapped.
   * \pre             -
   * \spec
   * requires true;
   * \endspec
   */
  void swap(StaticVector& other) noexcept {
    // VCA_VAC_STD_FUNC_FULFILL_CONTRACT, VCA_VAC_VALID_MEMBER_VARIABLE_ACCESS, VCA_VAC_VALID_ARGUMENT
    std::swap(delegate_, other.delegate_);
    // VCA_VAC_STD_FUNC_FULFILL_CONTRACT, VCA_VAC_VALID_MEMBER_VARIABLE_ACCESS, VCA_VAC_VALID_ARGUMENT
    std::swap(allocator_, other.allocator_);
    // VCA_VAC_STD_FUNC_FULFILL_CONTRACT, VCA_VAC_VALID_MEMBER_VARIABLE_ACCESS, VCA_VAC_VALID_ARGUMENT
    std::swap(max_num_elements_, other.max_num_elements_);
  }

  /*!
   * \brief           Get the length of the contained array
   * \return          The number of contained elements.
   * \pre             -
   * \spec
   * requires true;
   * \endspec
   */
  size_type size() const noexcept { return delegate_.size(); }

  /*!
   * \brief           Get the maximum number of elements this StaticVector can hold
   * \return          The number of elements this vector can store.
   * \pre             -
   * \spec
   * requires true;
   * \endspec
   */
  size_type capacity() const noexcept { return max_num_elements_; }

  /*!
   * \brief           Check whether the contained array is empty
   * \return          True if the vector is empty.
   * \pre             -
   * \spec
   * requires true;
   * \endspec
   */
  bool empty() const noexcept { return delegate_.empty(); }

  // VECTOR Next Construct AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConst
  /*!
   * \brief           Get a pointer to the contained data
   * \return          A pointer to the data.
   * \pre             -
   * \spec
   * requires true;
   * \endspec
   */
  pointer data() noexcept { return delegate_.data(); }

  /*!
   * \brief           Get a const pointer to the contained data
   * \return          A pointer to the data.
   * \pre             -
   * \spec
   * requires true;
   * \endspec
   */
  const_pointer data() const noexcept { return delegate_.data(); }

  // VECTOR Next Construct AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConst
  /*!
   * \brief           Iterator to the start of the array
   * \return          An iterator to the start of the vector.
   * \pre             -
   * \spec
   * requires true;
   * \endspec
   */
  iterator begin() noexcept { return delegate_.begin(); }
  // VECTOR Next Construct AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConst
  /*!
   * \brief           Past-The-End iterator of the array
   * \return          An iterator to the sentinel value at the end.
   * \pre             -
   * \spec
   * requires true;
   * \endspec
   */
  iterator end() noexcept { return delegate_.end(); }

  /*!
   * \brief           Const iterator to the start of the array
   * \return          An const iterator to the start of the vector.
   * \pre             -
   * \spec
   * requires true;
   * \endspec
   */
  const_iterator begin() const noexcept { return delegate_.begin(); }
  /*!
   * \brief           Const Past-The-End iterator of the array
   * \return          A const Past-The-End iterator of the array.
   * \pre             -
   * \spec
   * requires true;
   * \endspec
   */
  const_iterator end() const noexcept { return delegate_.end(); }

  /*!
   * \brief           Const iterator to the start of the array
   * \return          A const iterator to the start of the array.
   * \pre             -
   * \spec
   * requires true;
   * \endspec
   */
  const_iterator cbegin() const noexcept { return delegate_.cbegin(); }
  /*!
   * \brief           Const past-The-End iterator of the array
   * \return          A const iterator to the sentinel value at the end.
   * \pre             -
   * \spec
   * requires true;
   * \endspec
   */
  const_iterator cend() const noexcept { return delegate_.cend(); }

  /*!
   * \brief           Get the element at the specified position
   * \details         Abortion will be triggered if pos >= size().
   * \param           pos
   *                  Index of the element to get.
   * \return          A reference to the resulting element.
   * \pre             -
   */
  reference at(size_type pos) noexcept { return delegate_[pos]; }

  /*!
   * \brief           Get the element at the specified position (const version)
   * \details         Abortion will be triggered if pos >= size().
   * \param           pos
   *                  Index of the element to get.
   * \return          A reference to the resulting element.
   * \pre             -
   */
  const_reference at(size_type pos) const noexcept { return delegate_[pos]; }

  /*!
   * \brief           Unchecked array access. The behavior is unspecified if the container is empty or if pos >= size()
   * \param           pos
   *                  Index of the element to get.
   * \return          A reference to the resulting element.
   * \pre             -
   */
  reference operator[](size_type pos) noexcept { return delegate_[pos]; }

  /*!
   * \brief           Unchecked array access. The behavior is unspecified if the container is empty or if pos >= size()
   * \param           pos
   *                  Index of the element to get.
   * \return          A reference to the resulting element.
   * \pre             -
   */
  const_reference operator[](size_type pos) const noexcept { return delegate_[pos]; }

  /*!
   * \brief           Unchecked access to first element. The behavior is unspecified if the container is empty
   * \return          A reference to the first element.
   * \pre             -
   */
  reference front() noexcept { return delegate_[0]; }

  /*!
   * \brief           Unchecked access to first element. The behavior is unspecified if the container is empty
   * \return          A reference to the first element.
   * \pre             -
   */
  const_reference front() const noexcept { return delegate_[0]; }

  /*!
   * \brief           Unchecked access to last element. The behavior is unspecified if the container is empty
   * \return          A reference to the last element.
   * \pre             -
   */
  reference back() noexcept { return delegate_[this->size() - 1]; }

  /*!
   * \brief           Unchecked access to last element. The behavior is unspecified if the container is empty
   * \return          A reference to the last element.
   * \pre             -
   */
  const_reference back() const noexcept { return delegate_[this->size() - 1]; }  // VCA_VAC_VALID_RETURN_VALUE

  // VECTOR Next Construct AutosarC++17_10-A18.1.5: MD_VAC_A18.1.5_constStdUniquePtrRefAsParam
  /*!
   * \brief           Add an element to the back of the StaticVector
   * \details         Terminates if the added elment throws in its constructor.
   * \param           elem
   *                  The element to add. Type T must meet C++ Named Requirements: CopyConstructible.
   * \pre             The StaticVector must not be filled, otherwise ara::core::Abort will be triggered.
   */
  void push_back(T const& elem) noexcept {
    size_type const delegate_size{delegate_.size()};
    if (delegate_size >= max_num_elements_) {
      ara::core::Abort(
          "va::container::StaticVector::push_back(const T&): The number of elements cannot exceed the maximum!");
    } else {
      delegate_ = span_type(delegate_.data(), delegate_size + 1);
      allocator_.construct(&delegate_[this->size() - 1], elem);
    }
  }

  /*!
   * \brief           Add an element to the back of the StaticVector
   * \details         Terminates if the added elment throws in its constructor.
   * \tparam          Args
   *                  Types of arguments for the T constructor.
   * \param[in]       args
   *                  Constructor arguments for T.
   * \pre             The StaticVector should not be fulled, otherwise ara::core::Abort will be triggered.
   * \trace           CREQ-VaCommonLib-StaticVector
   */
  template <typename... Args>
  void emplace_back(Args&&... args) noexcept {
    size_type const delegate_size{delegate_.size()};
    if (delegate_size >= max_num_elements_) {
      ara::core::Abort(
          "vac::container::StaticVector::emplace_back<>(Args&&): The number of elements cannot exceed the maximum!");
    } else {
      delegate_ = span_type(delegate_.data(), delegate_size + 1);
      allocator_.construct(&delegate_[this->size() - 1], std::forward<Args>(args)...);
    }
  }

  /*!
   * \brief           Remove an element to the back of the StaticVector
   * \details         Terminates if the removed elment throws in its destructor.
   * \pre             The StaticVector should not be empty, otherwise ara::core::Abort will be triggered.
   */
  void pop_back() noexcept {
    size_type const delegate_size{delegate_.size()};
    if (delegate_size <= 0) {
      ara::core::Abort("vac::container::StaticVector::pop_back(): Vector cannot be empty!");
    } else {
      reference elem{delegate_[this->size() - 1]};
      allocator_.destroy(&elem);  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
      delegate_ = span_type(delegate_.data(), delegate_size - 1);
    }
  }

  /*!
   * \brief           Remove an element anywhere in the static vector
   * \details         Terminates if the removed elment throws in its destructor. Removing elements from a nullptr will
   *                  have no effect, and the function will return a nullptr.
   * \param           elem
   *                  Iterator to the element of type T to be removed. Type T must meet C++ Named Requirements:
   *                  MoveAssignable
   * \return          An iterator to the element following the removed element.
   * \pre             -
   * \trace           CREQ-VaCommonLib-StaticVector
   */
  iterator erase(iterator elem) noexcept {
    if ((elem != end()) && (elem != nullptr)) {
      iterator current{elem};
      iterator next{std::next(elem, 1)};

      while (next != end()) {
        *current = std::move(*next);  // VCA_VAC_CHECKED_ACCESS_TO_VALID_POINTER
        ++current;                    // VECTOR SL AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_arrayAccessOutOfBounds
        // VECTOR NC AutosarC++17_10-M5.0.16: MD_VAC_M5.0.16_pointerArithmetic
        // VECTOR Next Line AutosarC++17_10-A12.8.3: MD_VAC_A12.8.3_movedFromObjectReadAccessedInLoop
        ++next;
      }

      pop_back();
    }

    return elem;
  }

  /*!
   * \brief           Remove all elements from the vector
   * \details         Terminates if any removed elment throws in its destructor.
   * \pre             -
   */
  void clear() noexcept { shorten(0); }

 private:
  /*!
   * \brief           A Span to implement access to the data. The Span is also used to hold the pointer to the owned
   *                  data. Ownership semantics are implemented by the StaticVector itself. The size of the delegate_
   *                  represents the current length of the vector, not the amount of memory allocated
   */
  span_type delegate_;

  /*! \brief          Allocator used to obtain memory */
  actual_allocator_type allocator_;

  /*!
   * \brief           The maximum number of elements (i.e., amount of memory allocated) that this StaticVector can hold
   */
  size_type max_num_elements_;
};

}  // namespace container
}  // namespace vac

#endif  // LIB_VAC_INCLUDE_VAC_CONTAINER_STATIC_VECTOR_H_
