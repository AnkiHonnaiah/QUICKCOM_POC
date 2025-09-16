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
 *        \brief  An exception-less shared pointer implementation which can be used with
 *                ara::core::PolymorphicAllocator.
 *         \unit  VaCommonLib::MemoryManagement::SharedPtr
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_AMSR_SHARED_PTR_H_
#define LIB_VAC_INCLUDE_AMSR_SHARED_PTR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <memory>
#include <type_traits>
#include "amsr/generic/generic_error_domain.h"
#include "amsr/unique_ptr.h"
#include "ara/core/memory_resource.h"
#include "ara/core/result.h"
#include "vac/language/cpp17_backport.h"

namespace amsr {

// Forward declaration of WeakPtr.
template <typename T>
class WeakPtr;

// Forward declaration of SharedPtr.
template <typename T>
class SharedPtr;

// Forward declaration of EnableSharedFromThis.
template <typename T>
class EnableSharedFromThis;

namespace internal {
/*!
 * \brief           Base class for reference counter
 * \vprivate        Component private
 */
class ReferenceCounter {
 public:
  /*!
   * \brief           Default constructs a new ReferenceCounter
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  ReferenceCounter() noexcept : use_count_{1}, weak_count_{1} {}

  // Reference counter must not be copied.
  ReferenceCounter(ReferenceCounter const&) = delete;

  // Reference counter cannot not be moved as atomic does not support moving.
  ReferenceCounter(ReferenceCounter&&) = delete;

  /*!
   * \brief           Destructs the ReferenceCounter
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  virtual ~ReferenceCounter() noexcept = default;

  // Reference counter must not be copied.
  auto operator=(ReferenceCounter const&) -> ReferenceCounter& = delete;

  // Reference counter cannot not be moved as atomic does not support moving.
  auto operator=(ReferenceCounter&& other) -> ReferenceCounter& = delete;

  /*!
   * \brief           Releases the resources managed by *this when the use count drops to zero
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   */
  virtual void Dispose() noexcept = 0;

  /*!
   * \brief           Destroys this when the weak count drops to zero
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   */
  virtual void Destroy() noexcept = 0;

  /*!
   * \brief           Gets the Deleter
   * \return          The Deleter.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   */
  virtual auto GetDeleter() noexcept -> void* = 0;

  /*!
   * \brief           Adds a reference to the object
   * \details         Increases the use count by one.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  void AddRefCopy() {
    // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    static_cast<void>(std::atomic_fetch_add(&use_count_, 1));  // VCA_VAC_VALID_NONSTATIC_METHOD
  }

  /*!
   * \brief           Releases a reference to the object
   * \details         Use count is decreased by one. If it is zero afterwards, the owned object is destroyed.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  void Release() noexcept {
    // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    if (std::atomic_fetch_add(&use_count_, -1) == 1) {
      Dispose();  // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK

      WeakRelease();  // VCA_VAC_VALID_NONSTATIC_METHOD
    }
  }

  /*!
   * \brief           Adds a weak reference to the object
   * \details         Increases the weak count by one.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  void WeakAddRef() noexcept { static_cast<void>(std::atomic_fetch_add(&weak_count_, 1)); }

  /*!
   * \brief           Releases a weak reference
   * \details         Decreases the weak count by one. If the weak count is zero after decreasing it, the reference
   *                  counter instance is destroyed.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  void WeakRelease() noexcept {
    // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    if (std::atomic_fetch_add(&weak_count_, -1) == 1) {
      Destroy();  // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
    }
  }

  /*!
   * \brief           Adds a reference to the object if the use count is not zero
   * \context         ANY
   * \pre             -
   * \spec            requires true; \endspec
   */
  void AddRefLock() noexcept {
    std::int32_t count{use_count_};
    do {
      if (count == 0) {
        break;
      }
      // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    } while (!std::atomic_compare_exchange_strong_explicit<std::int32_t>(  // COV_LIBVAC_RARE_MULTITHREAD_CASE
        &use_count_, &count, count + 1, std::memory_order::memory_order_acq_rel,
        std::memory_order::memory_order_relaxed));
  }

  /*!
   * \brief           Get the use count, i. e. the number of shared pointers managing the same object
   * \return          The use count.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  auto GetUseCount() const noexcept -> std::int32_t {
    return std::atomic_load_explicit(&use_count_, std::memory_order::memory_order_relaxed);
  }

 private:
  /*!
   * \brief           Use count of the shared pointer
   * \details         It equals the number of shared pointer instances owning the same object.
   */
  std::atomic<std::int32_t> use_count_;

  /*!
   * \brief           The number of weak pointers
   * \details         It equals the number of weak pointer references plus one if at least one shared pointer manages
   *                  the referenced object. If there is no shared pointer left, it equals the number of weak pointers.
   */
  std::atomic<std::int32_t> weak_count_;
};

/*!
 * \brief           Helper struct for empty base optimization (EBO)
 * \vprivate        Component Private
 * \tparam          N
 *                  Number for distinguishing different overloads.
 * \tparam          T
 *                  Type for which EBO shall (not) be used.
 * \tparam          UseEbo
 *                  Whether to use EBO or not.
 */
template <std::int32_t N, typename T, bool UseEbo = (!std::is_final<T>::value) && (std::is_empty<T>::value)>
class SpEboHelper;  // VECTOR SL AutosarC++17_10-A12.4.1: MD_VAC_A12.4.1_compileTimeResolution

/*!
 * \brief           Helper struct specialization using empty base optimization (EBO)
 * \vprivate        Component Private
 * \tparam          N
 *                  Number for distinguishing different overloads.
 * \tparam          T
 *                  Type for which EBO shall be used.
 */
template <std::int32_t N, typename T>
class SpEboHelper<N, T, true> : private T {
 public:
  /*!
   * \brief           Constructs a new SpEboHelper
   * \param[in]       t
   *                  The instance.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  // VECTOR NC AutosarC++17_10-A12.1.6: MD_VAC_A12.1.6_useInheritingConstructor
  // VECTOR NL AutosarC++17_10-A12.1.5: MD_VAC_A12.1.5_useDelegatingConstructor
  explicit SpEboHelper(T const& t) : T(t) {}

  /*!
   * \brief           Constructs a new SpEboHelper by moving \p t
   * \param[in]       t
   *                  The instance to move.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  explicit SpEboHelper(T&& t) : T(std::move(t)) {}

  /*!
   * \brief           Gets the instance of type T from the ebo helper
   * \param[in]       ebo_helper
   *                  The EBO helper instance.
   * \return          The instance of type T.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  static auto Get(SpEboHelper& ebo_helper) noexcept -> T& { return static_cast<T&>(ebo_helper); }
};

/*!
 * \brief           Helper struct specialization not using empty base optimization (EBO)
 * \vprivate        Component Private
 * \tparam          N
 *                  Number for distinguishing different overloads.
 * \tparam          T
 *                  Type for which EBO cannot be used.
 */
template <std::int32_t N, typename T>
class SpEboHelper<N, T, false> {
 public:
  /*!
   * \brief           Constructs a new SpEboHelper
   * \param[in]       t
   *                  The instance.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  // VECTOR NL AutosarC++17_10-A12.1.5: MD_VAC_A12.1.5_useDelegatingConstructor
  explicit SpEboHelper(T const& t) : t_(t) {}

  /*!
   * \brief           Constructs a new SpEboHelper by moving \p t
   * \param[in]       t
   *                  The instance to move.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  explicit SpEboHelper(T&& t) : t_(std::move(t)) {}

  /*!
   * \brief           Gets the instance of type T from the ebo helper
   * \param[in]       ebo_helper
   *                  The EBO helper instance.
   * \return          The instance of type T.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  static auto Get(SpEboHelper& ebo_helper) noexcept -> T& { return ebo_helper.t_; }

 private:
  /*! \brief          The type for which EBO cannot be used */
  T t_;
};

/*!
 * \brief           Reference counter with pointer to the owned object and deleter and allocator
 * \vprivate        Component Private
 * \tparam          Ptr
 *                  Pointer type of the object owned by the shared pointer.
 * \tparam          Deleter
 *                  Type of the deleter.
 * \tparam          Allocator
 *                  Type of the allocator.
 */
// VECTOR NC AutosarC++17_10-M10.2.1: MD_VAC_M10.2.1_uniqueEntityNamesInMultipleInheritance
// VECTOR NC AutosarC++17_10-A10.1.1: MD_VAC_A10.1.1_multipleInheritanceEBO
template <typename Ptr, typename Deleter, typename Alloc = ara::core::PolymorphicAllocator<char>>
class ReferenceCounterPtrDeleter final : public ReferenceCounter,
                                         private SpEboHelper<0, Deleter>,
                                         private SpEboHelper<1, Alloc> {
 public:
  /*! \brief          The allocator type for allocating memory for *this */
  using AllocatorType = typename std::allocator_traits<Alloc>::template rebind_alloc<ReferenceCounterPtrDeleter>;

  /*!
   * \brief           Constructs a new ReferenceCounterPtrDeleter
   * \param[in]       p
   *                  Pointer to the owned object.
   * \param[in]       d
   *                  Deleter.
   * \param[in]       a
   *                  Allocator.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  explicit ReferenceCounterPtrDeleter(Ptr p, Deleter d, Alloc a) noexcept
      : ReferenceCounter{}, SpEboHelper<0, Deleter>(std::move(d)), SpEboHelper<1, Alloc>(a), ptr_(p) {}

  // Reference counter must not be copied.
  ReferenceCounterPtrDeleter(ReferenceCounterPtrDeleter const&) = delete;

  // Reference counter cannot be moved as base class cannot be moved.
  ReferenceCounterPtrDeleter(ReferenceCounterPtrDeleter&&) = delete;

  /*!
   * \brief           Default deleter
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  ~ReferenceCounterPtrDeleter() noexcept final = default;  // VCA_VAC_FREEING_STATIC_THIS_POINTER

  // Reference counter must not be copied.
  auto operator=(ReferenceCounterPtrDeleter const&) -> ReferenceCounterPtrDeleter& = delete;

  // Reference counter cannot be moved as base class cannot be moved.
  auto operator=(ReferenceCounterPtrDeleter&& other) -> ReferenceCounterPtrDeleter& = delete;

  /*!
   * \brief           Disposes the owned object using the deleter
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  void Dispose() noexcept final {
    SpEboHelper<0, Deleter>::Get (*this)(ptr_);  // VCA_VAC_TEMPLATE_REQ_MET
  }

  /*!
   * \brief           Destroys this using the allocator
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  void Destroy() noexcept final {
    AllocatorType a{SpEboHelper<1, Alloc>::Get(*this)};
    std::allocator_traits<AllocatorType>::deallocate(a, this, 1);  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
  }

  /*!
   * \brief           Get the deleter
   * \return          Address of the deleter.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  auto GetDeleter() noexcept -> void* final { return static_cast<void*>(&(SpEboHelper<0, Deleter>::Get(*this))); }

 private:
  /*! \brief          Pointer to the managed object */
  Ptr ptr_;
};

/*!
 * \brief           Reference counter including the owned object and allocator
 * \vprivate        Component Private
 * \tparam          T
 *                  Type of the object owned by the shared pointer.
 * \tparam          Alloc
 *                  Type of the allocator.
 */
// VECTOR NC AutosarC++17_10-A10.1.1: MD_VAC_A10.1.1_multipleInheritanceEBO
template <typename T, typename Alloc>
class ReferenceCounterInplace final : public ReferenceCounter, private SpEboHelper<1, Alloc> {
 public:
  /*! \brief          The allocator type for allocating memory for *this */
  using AllocatorType = typename std::allocator_traits<Alloc>::template rebind_alloc<ReferenceCounterInplace>;

  // Deleted default constructor
  ReferenceCounterInplace() = delete;

  /*!
   * \brief           Constructs a new ReferenceCounterInplace
   * \tparam          Args
   *                  Arguments needed to construct an object of type T.
   * \param[in]       a
   *                  The allocator.
   * \param[in]       args
   *                  The arguments needed to construct an object of type T.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  template <typename... Args>
  explicit ReferenceCounterInplace(Alloc a, Args&&... args) noexcept
      : ReferenceCounter{}, SpEboHelper<1, Alloc>(a), managed_object_{std::forward<Args>(args)...} {}

  // Reference counter must not be copied.
  ReferenceCounterInplace(ReferenceCounterInplace const&) = delete;

  // Reference counter cannot be moved as base class cannot be moved.
  ReferenceCounterInplace(ReferenceCounterInplace&&) = delete;

  /*!
   * \brief           Default deleter
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  ~ReferenceCounterInplace() noexcept final = default;  // VCA_VAC_FREEING_STATIC_THIS_POINTER

  // Reference counter must not be copied.
  auto operator=(ReferenceCounterInplace const&) -> ReferenceCounterInplace& = delete;

  // Reference counter cannot be moved as base class cannot be moved.
  auto operator=(ReferenceCounterInplace&& other) -> ReferenceCounterInplace& = delete;

  /*!
   * \brief           Destroys the owned object using the allocator
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  void Dispose() noexcept final {
    // VECTOR NC AutosarC++17_10-M0.1.8: MD_VAC_M0.1.8_voidFunctionHasNoExternalSideEffectAllocatorTraits
    // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    std::allocator_traits<Alloc>::destroy(SpEboHelper<1, Alloc>::Get(*this), GetPtr());
  }

  /*!
   * \brief           Destroys this using the allocator
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  void Destroy() noexcept final {
    AllocatorType a{SpEboHelper<1, Alloc>::Get(*this)};
    std::allocator_traits<AllocatorType>::deallocate(a, this, 1);
  }

  /*!
   * \brief           Get the pointer to the managed object
   * \return          Pointer to the managed object.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  auto GetPtr() noexcept -> T* { return &managed_object_; }

 private:
  /*!
   * \brief           Get the deleter
   * \details         No deleter is specified. The allocator is used for deallocation.
   * \return          nullptr.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  auto GetDeleter() noexcept -> void* final { return nullptr; }

  /*! \brief          The managed object */
  T managed_object_;
};

}  // namespace internal

/*!
 * \brief           A weak pointer that holds a non-owning reference to an object managed by a shared pointer
 * \vpublic
 * \tparam          T
 *                  Type of the object.
 * \context         ANY
 * \pre             -
 * \trace           CREQ-VaCommonLib-WeakPtr
 */
template <typename T>
class WeakPtr final {
 public:
  /*! \brief          Type of the object pointed to by the weak pointer */
  using element_type = typename std::remove_extent<T>::type;

  /*!
   * \brief           Checks whether Y* is convertible to T*
   * \tparam          Y
   *                  Object type.
   */
  template <typename Y>
  using Compatible = typename std::enable_if<std::is_convertible<Y*, T*>::value>::type;

  /*!
   * \brief           Constructs an empty WeakPtr
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  // VECTOR NL AutosarC++17_10-A12.1.5: MD_VAC_A12.1.5_useDelegatingConstructor
  constexpr WeakPtr() noexcept : ptr_{nullptr}, reference_counter_{nullptr} {};

  /*!
   * \brief           Constructs a WeakPtr from another WeakPtr
   * \param[in]       other
   *                  The other WeakPtr.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  WeakPtr(WeakPtr const& other) noexcept : ptr_{other.ptr_}, reference_counter_{other.reference_counter_} {
    if (reference_counter_ != nullptr) {
      reference_counter_->WeakAddRef();
    }
  }

  // VECTOR NC VectorC++-V12.1.4: MD_VAC_V12.1.4_AllowedExplicitConstructor
  /*!
   * \brief           Constructs a WeakPtr from another WeakPtr of another type
   * \tparam          Y
   *                  Object type of the other WeakPtr.
   * \param[in]       other
   *                  The other WeakPtr.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  template <typename Y, typename = Compatible<Y>>
  WeakPtr(WeakPtr<Y> const& other) noexcept : ptr_{nullptr}, reference_counter_{other.reference_counter_} {
    ptr_ = other.Lock().Get();
    if (reference_counter_ != nullptr) {
      reference_counter_->WeakAddRef();
    }
  }

  // VECTOR NC VectorC++-V12.1.4: MD_VAC_V12.1.4_AllowedExplicitConstructor
  /*!
   * \brief           Constructs a WeakPtr from SharedPtr
   * \tparam          Y
   *                  Object type of the SharedPtr.
   * \param[in]       other
   *                  The shared pointer.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  template <typename Y, typename = Compatible<Y>>
  WeakPtr(SharedPtr<Y> const& other) noexcept : ptr_{other.Get()}, reference_counter_{other.reference_counter_} {
    if (reference_counter_ != nullptr) {
      reference_counter_->WeakAddRef();
    }
  }

  /*!
   * \brief           Move constructs a WeakPtr
   * \param[in]       other
   *                  The other WeakPtr.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  WeakPtr(WeakPtr&& other) noexcept : ptr_{other.ptr_}, reference_counter_{other.reference_counter_} {
    other.ptr_ = nullptr;
    other.reference_counter_ = nullptr;
  }

  // VECTOR NC VectorC++-V12.1.4: MD_VAC_V12.1.4_AllowedExplicitConstructor
  /*!
   * \brief           Move constructs a Weak Ptr
   * \tparam          Y
   *                  Object type of the other WeakPtr.
   * \param[in]       other
   *                  The other WeakPtr.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  template <typename Y, typename = Compatible<Y>>
  WeakPtr(WeakPtr<Y>&& other) noexcept : ptr_{nullptr}, reference_counter_{other.reference_counter_} {
    ptr_ = other.Lock().Get();
    other.ptr_ = nullptr;
    other.reference_counter_ = nullptr;
  }

  /*!
   * \brief           Destructs the WeakPtr
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  // VCA_VAC_VALID_NONSTATIC_METHOD
  ~WeakPtr() noexcept {
    if (reference_counter_ != nullptr) {
      // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
      reference_counter_->WeakRelease();
    }
  }

  /*!
   * \brief           Copy assigns the WeakPtr from another WeakPtr
   * \param[in]       other
   *                  The other WeakPtr.
   * \return          A reference to *this.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  auto operator=(WeakPtr const& other) & noexcept -> WeakPtr& {
    WeakPtr{other}.Swap(*this);
    return *this;
  }

  /*!
   * \brief           Copy assigns the WeakPtr from another WeakPtr of another type
   * \tparam          Y
   *                  Object type of the other WeakPtr.
   * \param[in]       other
   *                  The other WeakPtr.
   * \return          A reference to *this.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  template <typename Y, typename = Compatible<Y>>
  auto operator=(WeakPtr<Y> const& other) & noexcept -> WeakPtr& {
    WeakPtr{other}.Swap(*this);
    return *this;
  }

  /*!
   * \brief           Copy assigns the WeakPtr from a SharedPtr
   * \tparam          Y
   *                  Object type of the SharedPtr.
   * \param[in]       other
   *                  The shared pointer.
   * \return          A reference to *this.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  template <typename Y, typename = Compatible<Y>>
  auto operator=(SharedPtr<Y> const& other) & noexcept -> WeakPtr& {
    WeakPtr{other}.Swap(*this);
    return *this;
  }

  /*!
   * \brief           Move assigns the WeakPtr from another WeakPtr
   * \param[in]       other
   *                  The other WeakPtr.
   * \return          A reference to *this.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  auto operator=(WeakPtr&& other) & noexcept -> WeakPtr& {
    if (this != &other) {
      WeakPtr{std::move(other)}.Swap(*this);
    }
    return *this;
  }

  /*!
   * \brief           Move assigns the WeakPtr from another WeakPtr
   * \tparam          Y
   *                  Object type of the other WeakPtr.
   * \param[in]       other
   *                  The other WeakPtr.
   * \return          A reference to *this.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  template <typename Y, typename = Compatible<Y>>
  auto operator=(WeakPtr<Y>&& other) & noexcept -> WeakPtr& {
    WeakPtr{std::move(other)}.Swap(*this);
    return *this;
  }

  /*!
   * \brief           Resets the WeakPtr
   * \details         After the reset, *this is empty.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  void Reset() noexcept {
    if (reference_counter_ != nullptr) {
      reference_counter_->WeakRelease();
    }
    ptr_ = nullptr;
    reference_counter_ = nullptr;
  }

  /*!
   * \brief           Swaps the WeakPtr with another WeakPtr
   * \param[in]       other
   *                  The other WeakPtr.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  void Swap(WeakPtr& other) noexcept {
    std::swap(ptr_, other.ptr_);
    internal::ReferenceCounter* const tmp{reference_counter_};
    reference_counter_ = other.reference_counter_;
    other.reference_counter_ = tmp;
  }

  /*!
   * \brief           Returns the number of SharedPtrs referencing this object
   * \return          The number of SharedPtrs referencing this object.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  auto UseCount() const noexcept -> std::int32_t {
    return (reference_counter_ != nullptr) ? reference_counter_->GetUseCount() : 0;
  }

  /*!
   * \brief           Checks whether the referenced object was already deleted
   * \return          True if the referenced object was already deleted, false otherwise.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  auto Expired() const noexcept -> bool {
    return (reference_counter_ == nullptr) || (reference_counter_->GetUseCount() == 0);
  }

  /*!
   * \brief           Creates a SharedPtr that manages the referenced object
   * \details         If the referenced object was already deleted, an empty SharedPtr is returned
   * \return          A SharedPtr that manages the referenced object.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  auto Lock() const noexcept -> SharedPtr<element_type> {
    // VCA_VAC_VALID_ARGUMENT
    return Expired() ? SharedPtr<element_type>{} : SharedPtr<element_type>{*this};
  }

  /*!
   * \brief           Provides owner-based ordering of WeakPtr
   * \tparam          Y
   *                  Object type of the other WeakPtr.
   * \param[in]       other
   *                  The other WeakPtr.
   * \return          True if *this is less than other, false otherwise.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  template <typename Y>
  auto OwnerBefore(WeakPtr<Y> const& other) const noexcept -> bool {
    return std::less<internal::ReferenceCounter*>()(this->reference_counter_, other.reference_counter_);
  }

  /*!
   * \brief           Provides owner-based ordering of WeakPtr
   * \tparam          Y
   *                  Object type of the other SharedPtr.
   * \param[in]       other
   *                  The other SharedPtr.
   * \return          True if *this is less than other, false otherwise.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  template <typename Y>
  auto OwnerBefore(SharedPtr<Y> const& other) const noexcept -> bool {
    return std::less<internal::ReferenceCounter*>()(this->reference_counter_, other.reference_counter_);
  }

 private:
  /*!
   * \brief           Assigns a pointer to the object and a reference counter to the weak pointer
   * \param[in]       ptr
   *                  The pointer to the object.
   * \param[in]       reference_counter
   *                  The reference counter. Must not be nullptr.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   */
  void Assign(T* ptr, internal::ReferenceCounter* reference_counter) noexcept {
    reference_counter->WeakAddRef();
    ptr_ = ptr;
    reference_counter_ = reference_counter;
  }

  /*! \brief          Pointer to the object */
  element_type* ptr_;

  /*! \brief          Reference counter for the weak pointer */
  internal::ReferenceCounter* reference_counter_;

  /*!
   * \brief           Friend declaration for WeakPtr
   * \tparam          Y
   *                  Object type of the WeakPtr.
   */
  // VECTOR NL AutosarC++17_10-A11.3.1: MD_VAC_A11.3.1_doNotUseFriendConversion
  template <typename Y>
  friend class WeakPtr;

  /*!
   * \brief           Friend declaration for SharedPtr
   * \tparam          Y
   *                  Object type of the SharedPtr.
   */
  // VECTOR NL AutosarC++17_10-A11.3.1: MD_VAC_A11.3.1_doNotUseFriendConversion
  template <typename Y>
  friend class SharedPtr;

  /*!
   * \brief           Friend declaration for EnableSharedFromThis
   * \tparam          Y
   *                  Object type.
   */
  // VECTOR NL AutosarC++17_10-A11.3.1: MD_VAC_A11.3.1_doNotUseFriendConversion
  template <typename Y>
  friend class EnableSharedFromThis;
};

/*!
 * \brief           Swaps two WeakPtr objects
 * \tparam          T
 *                  Object type.
 * \param[in]       lhs
 *                  The first WeakPtr.
 * \param[in]       rhs
 *                  The second WeakPtr.
 * \context         ANY
 * \pre             -
 * \steady          TRUE
 * \spec            requires true; \endspec
 */
template <typename T>
void swap(WeakPtr<T>& lhs, WeakPtr<T>& rhs) noexcept {
  lhs.Swap(rhs);
}

/*!
 * \brief           A smart pointer that retains shared ownership of an object through a pointer
 * \vpublic
 * \tparam          T
 *                  Type of the object pointed to by the shared pointer. Can only be a single object.
 * \trace           CREQ-VaCommonLib-SharedPtr
 */
template <typename T>
class SharedPtr final {
 public:
  /*!
   * \brief           Type of the object pointed to by the shared pointer
   * \details         May be different from the owned object if the aliasing constructor was used.
   */
  using element_type = typename std::remove_extent<T>::type;

  /*! \brief          Type of the weak pointer */
  using weak_type = WeakPtr<T>;

  /*!
   * \brief           Checks whether T* is constructible from Y*
   * \details         Can be used to enable or disable overloads.
   * \tparam          Y
   *                  Object type.
   */
  template <typename Y>
  using SafeConversion = std::enable_if_t<std::is_constructible<T*, Y*>::value>;

  /*!
   * \brief           Checks whether Y* is convertible to T*
   * \tparam          Y
   *                  Object type.
   */
  template <typename Y>
  using Compatible = std::enable_if_t<std::is_convertible<Y*, T*>::value>;

  /*!
   * \brief           Checks whether Y* is convertible to T* and if the pointer points to a compatible type
   * \tparam          Y
   *                  Object type of the unique pointer.
   * \tparam          Deleter
   *                  Deleter type of the unique pointer.
   * \tparam          Ptr
   *                  Pointer type of the unique pointer.
   */
  template <typename Y, typename Deleter, typename Ptr = typename amsr::UniquePtr<Y, Deleter>::pointer>
  using UniqueCompatible =
      std::enable_if_t<std::is_convertible<Y*, T*>::value && std::is_convertible<Ptr, element_type*>::value>;

  /*!
   * \brief           Default constructor
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  SharedPtr() noexcept : SharedPtr(nullptr){};

  // VECTOR NC VectorC++-V12.1.4: MD_VAC_V12.1.4_AllowedExplicitConstructor
  /*!
   * \brief           Constructs an empty SharedPtr
   * \details         No control block is needed as no object is owned.
   * \param[in]       p
   *                  A nullptr.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  // VECTOR NL AutosarC++17_10-A12.1.5: MD_VAC_A12.1.5_useDelegatingConstructor
  constexpr SharedPtr(std::nullptr_t p) noexcept : ptr_{p}, reference_counter_{nullptr} {}

  /*!
   * \brief           Create a SharedPtr from the given pointer to an object
   * \tparam          Y
   *                  Type of the object.
   * \param[in]       p
   *                  Pointer to the object.
   * \return          A result containing either the newly created SharedPtr or an error code.
   * \error           amsr::generic::GenErrc::kRuntimeResourceOutOf
   *                  if allocation for the control block is not successful.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  template <typename Y, typename = SafeConversion<Y>>
  static auto CreateSharedPtr(Y* p) noexcept -> ara::core::Result<SharedPtr> {
    return CreateSharedPtr(p, DefaultDeleter{});
  }

  /*!
   * \brief           Create a SharedPtr from the given pointer to an object and the deleter
   * \tparam          Y
   *                  Type of the object.
   * \tparam          Deleter
   *                  Type of the deleter.
   * \param[in]       p
   *                  Pointer to the object.
   * \param[in]       d
   *                  The deleter.
   * \return          A result containing either the newly created SharedPtr or an error code.
   * \error           amsr::generic::GenErrc::kRuntimeResourceOutOf
   *                  if allocation for the control block is not successful.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  template <typename Y, typename Deleter, typename = SafeConversion<Y>>
  static auto CreateSharedPtr(Y* p, Deleter d) noexcept -> ara::core::Result<SharedPtr> {
    return CreateSharedPtr(p, d, DefaultAllocator{});
  }

  /*!
   * \brief           Create a SharedPtr from a nullptr and the deleter
   * \tparam          Deleter
   *                  Type of the deleter.
   * \param[in]       p
   *                  A nullptr.
   * \param[in]       d
   *                  The deleter.
   * \return          A result containing either the newly created SharedPtr or an error code.
   * \error           amsr::generic::GenErrc::kRuntimeResourceOutOf
   *                  if allocation for the control block is not successful.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  template <typename Deleter>
  static auto CreateSharedPtr(std::nullptr_t p, Deleter d) noexcept -> ara::core::Result<SharedPtr> {
    return CreateSharedPtr(p, d, DefaultAllocator{});
  }

  /*!
   * \brief           Create a SharedPtr from the given pointer to an object, the deleter and the allocator
   * \details         If allocation fails, the object pointed to by \p p is destroyed.
   * \tparam          Y
   *                  Type of the object.
   * \tparam          Deleter
   *                  Type of the deleter.
   * \param[in]       p
   *                  Pointer to the object.
   * \param[in]       d
   *                  The deleter.
   * \param[in]       a
   *                  The allocator.
   * \return          A result containing either the newly created SharedPtr or an error code.
   * \error           amsr::generic::GenErrc::kRuntimeResourceOutOf
   *                  if allocation for the control block is not successful.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  template <typename Y, typename Deleter, typename Alloc, typename = SafeConversion<Y>>
  static auto CreateSharedPtr(Y* p, Deleter d, Alloc a) noexcept -> ara::core::Result<SharedPtr<T>> {
    ara::core::Result<SharedPtr<T>> result{amsr::generic::GenErrc::kRuntimeResourceOutOf};
    SharedPtr<T> sp{p, d, std::move(a)};  // VCA_VAC_VALID_ARGUMENT
    if (sp.UseCount() != 0) {
      result.EmplaceValue(std::move(sp));
    }
    return result;
  }

  /*!
   * \brief           Create a SharedPtr from a nullptr, the deleter and the allocator
   * \tparam          Deleter
   *                  Type of the deleter.
   * \param[in]       p
   *                  A nullptr.
   * \param[in]       d
   *                  The deleter.
   * \param[in]       a
   *                  The allocator.
   * \return          A result containing either the newly created SharedPtr or an error code.
   * \error           amsr::generic::GenErrc::kRuntimeResourceOutOf
   *                  if allocation for the control block is not successful.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  template <typename Deleter, typename Alloc>
  static auto CreateSharedPtr(std::nullptr_t p, Deleter d, Alloc a) noexcept -> ara::core::Result<SharedPtr<T>> {
    ara::core::Result<SharedPtr<T>> result{amsr::generic::GenErrc::kRuntimeResourceOutOf};
    SharedPtr<T> sp{p, d, std::move(a)};  // VCA_VAC_VALID_ARGUMENT
    if (sp.UseCount() != 0) {
      result.EmplaceValue(std::move(sp));
    }
    return result;
  }

  /*!
   * \brief           Aliasing constructor
   * \details         Constructs a new SharedPtr that shares ownership information with \p sp and holds an unrelated and
   *                  unmanaged pointer \p ptr. It is the responsibility of the user to make sure that this pointer
   *                  remains valid as long as this SharedPtr exists, such as in the typical use cases where \p ptr is a
   *                  member of the object managed by \p sp.
   * \tparam          Y
   *                  Object type of the input SharedPtr.
   * \param[in]       sp
   *                  The SharedPtr from which a new SharedPtr shall be constructed.
   * \param[in]       ptr
   *                  Pointer to the object, the SharedPtr shall point to. The pointed to object is not owned by this.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   */
  template <typename Y>
  SharedPtr(SharedPtr<Y> const& sp, element_type* ptr) noexcept : ptr_{ptr}, reference_counter_{sp.reference_counter_} {
    if (reference_counter_ != nullptr) {
      reference_counter_->AddRefCopy();
    }
  }

  /*!
   * \brief           Copy constructs a SharedPtr
   * \param[in]       other
   *                  The shared pointer to copy.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  SharedPtr(SharedPtr const& other) noexcept : ptr_{other.ptr_}, reference_counter_{other.reference_counter_} {
    if (reference_counter_ != nullptr) {
      // VCA_VAC_VALID_NONSTATIC_METHOD
      reference_counter_->AddRefCopy();
    }
  }

  // VECTOR NC VectorC++-V12.1.4: MD_VAC_V12.1.4_AllowedExplicitConstructor
  /*!
   * \brief           Constructs a SharedPtr from another SharedPtr
   * \tparam          Y
   *                  Object type of the input SharedPtr.
   * \param[in]       other
   *                  The SharedPtr from which a new SharedPtr shall be constructed.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  template <typename Y, typename = Compatible<Y>>
  SharedPtr(SharedPtr<Y> const& other) noexcept : ptr_{other.ptr_}, reference_counter_{other.reference_counter_} {
    if (reference_counter_ != nullptr) {
      reference_counter_->AddRefCopy();
    }
  }

  /*!
   * \brief           Move constructs a SharedPtr
   * \param[in]       other
   *                  The shared pointer to move.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  SharedPtr(SharedPtr&& other) noexcept : ptr_{std::move(other.ptr_)}, reference_counter_{nullptr} {
    internal::ReferenceCounter* const tmp{reference_counter_};
    reference_counter_ = other.reference_counter_;
    other.reference_counter_ = tmp;
    other.ptr_ = nullptr;
  }

  // VECTOR NC VectorC++-V12.1.4: MD_VAC_V12.1.4_AllowedExplicitConstructor
  /*!
   * \brief           Move constructs a SharedPtr
   * \tparam          Y
   *                  Object type of other. Needs to be implicitly convertible to T.
   * \param[in]       other
   *                  The shared pointer to move.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  template <typename Y, typename = Compatible<Y>>
  SharedPtr(SharedPtr<Y>&& other) noexcept : ptr_{std::move(other.ptr_)}, reference_counter_{} {
    internal::ReferenceCounter* const tmp{reference_counter_};
    reference_counter_ = other.reference_counter_;
    other.reference_counter_ = tmp;
    other.ptr_ = nullptr;
  }

  /*!
   * \brief           Creates a SharedPtr from a WeakPtr
   * \tparam          Y
   *                  Object type of the input WeakPtr.
   * \param[in]       wp
   *                  The weak pointer.
   * \return          The shared pointer on success, an error code otherwise.
   * \error           amsr::generic::GenErrc::kRuntimeResourceInvalidHandle
   *                  if the object pointed to by the weak pointer is invalid.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  template <typename Y, typename = Compatible<Y>>
  static auto CreateSharedPtr(WeakPtr<Y> const& wp) noexcept -> ara::core::Result<SharedPtr<T>> {
    ara::core::Result<SharedPtr<T>> result{amsr::generic::GenErrc::kRuntimeResourceInvalidHandle};
    SharedPtr<T> sp{wp};

    if (sp.UseCount() != 0) {
      result.EmplaceValue(std::move(sp));
    }
    return result;
  }

  /*!
   * \brief           Creates a SharedPtr from a UniquePtr
   * \details         If allocation fails, the UniquePtr is unchanged.
   * \tparam          Y
   *                  Object type of the input UniquePtr.
   * \tparam          Deleter
   *                  The deleter.
   * \param[in]       up
   *                  The unique pointer.
   * \return          A result containing either the newly created SharedPtr or an error code.
   * \error           amsr::generic::GenErrc::kRuntimeResourceOutOf
   *                  if allocation for the control block is not successful.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  template <typename Y, typename Deleter, typename = UniqueCompatible<Y, Deleter>>
  static auto CreateSharedPtr(amsr::UniquePtr<Y, Deleter>&& up) noexcept -> ara::core::Result<SharedPtr<T>> {
    ara::core::Result<SharedPtr<T>> result{amsr::generic::GenErrc::kRuntimeResourceOutOf};
    if (up.get() == nullptr) {
      SharedPtr<T> sp{};
      result.EmplaceValue(std::move(sp));
    } else {
      SharedPtr<T> sp{std::move(up)};
      if (sp.UseCount() != 0) {
        result.EmplaceValue(std::move(sp));
      }
    }
    return result;
  }

  /*!
   * \brief           Destructs the SharedPtr
   * \details         If this is the last SharedPtr instance managing this object, the managed object is destroyed. This
   *                  might not be threadsafe when another shared pointer is copy constructed from this.
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE, if this is the last SharedPtr instance managing this object, TRUE otherwise.
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  ~SharedPtr() noexcept {
    internal::ReferenceCounter* const tmp{reference_counter_};
    reference_counter_ = nullptr;
    if (tmp != nullptr) {
      tmp->Release();  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
    }
  }

  /*!
   * \brief           Copy assignment
   * \param[in]       other
   *                  The shared pointer to copy.
   * \return          A reference to *this.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  auto operator=(SharedPtr const& other) & noexcept -> SharedPtr& {
    if (this != &other) {
      SharedPtr{other}.Swap(*this);
    }
    return *this;
  }

  /*!
   * \brief           Copy assignment
   * \tparam          Y
   *                  Object type of the other shared pointer. Must be implicitly convertible to T.
   * \tparam          Allocator
   *                  Allocator type of the other shared pointer.
   * \param[in]       other
   *                  The shared pointer to copy.
   * \return          Reference to *this.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  template <typename Y, typename = SafeConversion<Y>>
  auto operator=(SharedPtr<Y> const& other) & noexcept -> SharedPtr& {
    SharedPtr{other}.Swap(*this);
    return *this;
  }

  /*!
   * \brief           Move assignment
   * \param[in]       other
   *                  The shared pointer to move.
   * \return          Reference to *this.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  auto operator=(SharedPtr&& other) & noexcept -> SharedPtr& {
    if (this != &other) {
      // VECTOR NL AutosarC++17_10-A12.8.3: MD_VAC_A12.8.3_dontReadAccessAMovedFromObject
      SharedPtr{std::move(other)}.Swap(*this);
    }
    // VECTOR NL AutosarC++17_10-A12.8.3: MD_VAC_A12.8.3_dontReadAccessAMovedFromObject
    return *this;
  }

  /*!
   * \brief           Move assignment
   * \tparam          Y
   *                  Object type of the other shared pointer. Must be implicitly convertible to T.
   * \param[in]       other
   *                  The shared pointer to move.
   * \return          Reference to *this.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  template <typename Y, typename = SafeConversion<Y>>
  auto operator=(SharedPtr<Y>&& other) & noexcept -> SharedPtr& {
    SharedPtr{std::move(other)}.Swap(*this);
    return *this;
  }

  /*!
   * \brief           Assigns a unique pointer to this
   * \tparam          Y
   *                  Object type of the other unique pointer. Must be implicitly convertible to T.
   * \tparam          Deleter
   *                  Type of the deleter.
   * \param[in]       other
   *                  The unique pointer to assign.
   * \return          Nothing on success, an error code otherwise.
   * \error           amsr::generic::GenErrc::kRuntimeResourceOutOf
   *                  if the unique pointer is empty
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  template <typename Y, typename Deleter, typename = UniqueCompatible<Y, Deleter>>
  auto Assign(amsr::UniquePtr<Y, Deleter>&& other) noexcept -> ara::core::Result<void> {
    ara::core::Result<void> result{};
    SharedPtr sp{std::move(other)};
    if (sp.UseCount() != 0) {
      sp.Swap(*this);
    } else {
      result.EmplaceError(amsr::generic::GenErrc::kRuntimeResourceOutOf);
    }
    return result;
  }

  // Deleted unique pointer assignment. Assign(amsr::UniquePtr<T, Deleter>&& other) shall be used instead.
  template <typename Y, typename Deleter>
  auto operator=(amsr::UniquePtr<Y, Deleter>&& other) -> SharedPtr& = delete;

  /*!
   * \brief           Resets the shared pointer
   * \details         It points to nullptr and has no reference counter afterwards.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  void Reset() noexcept {
    if (reference_counter_ != nullptr) {
      reference_counter_->Release();
    }
    ptr_ = nullptr;
    reference_counter_ = nullptr;
  }

  /*!
   * \brief           Replaces the managed object with the object pointed to by \p ptr
   * \tparam          Y
   *                  Object type.
   * \param[in]       ptr
   *                  Pointer to the object. Must be different from the current pointer or nullptr.
   * \return          An error code if allocation for the new reference counter fails; nothing otherwise.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  template <typename Y>
  auto Reset(Y* ptr) noexcept -> ara::core::Result<void> {
    return Reset(ptr, DefaultDeleter{}, DefaultAllocator{});
  }

  /*!
   * \brief           Replaces the managed object with the object pointed to by \p ptr and the deleter with \p d
   * \tparam          Y
   *                  Object type.
   * \tparam          Deleter
   *                  Deleter type.
   * \param[in]       ptr
   *                  Pointer to the object.
   * \param[in]       d
   *                  The new deleter.
   * \return          An error code if allocation for the new reference counter fails; nothing otherwise.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  template <typename Y, typename Deleter>
  auto Reset(Y* ptr, Deleter d) noexcept -> ara::core::Result<void> {
    return Reset(ptr, d, DefaultAllocator{});
  }

  /*!
   * \brief           Replaces the managed object with the object pointed to by \p ptr and the deleter with \p d
   * \tparam          Y
   *                  Object type.
   * \tparam          Deleter
   *                  Deleter type.
   * \tparam          Allocator
   *                  Allocator type.
   * \param[in]       ptr
   *                  Pointer to the object.
   * \param[in]       d
   *                  The new deleter.
   * \param[in]       a
   *                  The new allocator.
   * \return          An error code if allocation for the new reference counter fails; nothing otherwise.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  template <typename Y, typename Deleter, typename Allocator>
  auto Reset(Y* ptr, Deleter d, Allocator a) noexcept -> ara::core::Result<void> {
    if ((ptr != nullptr) && (ptr == ptr_)) {
      amsr::core::Abort("Resetting a shared pointer to its current pointer is not allowed.");
    }

    ara::core::Result<void> result{};
    SharedPtr(ptr, std::move(d), std::move(a)).Swap(*this);  // VCA_VAC_VALID_ARGUMENT
    if (UseCount() < 1) {
      result.EmplaceError(amsr::generic::GenErrc::kRuntimeResourceOutOf);
    }
    return result;
  }

  /*!
   * \brief           Swaps this with \p other
   * \param[in]       other
   *                  The other shared pointer.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  void Swap(SharedPtr& other) noexcept {
    std::swap(ptr_, other.ptr_);
    internal::ReferenceCounter* const tmp{reference_counter_};
    reference_counter_ = other.reference_counter_;
    other.reference_counter_ = tmp;
  }

  /*!
   * \brief           Gets the stored pointer
   * \return          The stored pointer.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  auto Get() const noexcept -> element_type* {
    // VECTOR NL AutosarC++17_10-M9.3.1: MD_VAC_M9.3.1_ConstMemberFunctionsShallNotReturnNonConstPointers
    return ptr_;
  }

  /*!
   * \brief           Dereference operator
   * \return          Reference to the object pointed to by the stored pointer.
   * \context         ANY
   * \pre             The stored pointer is not nullptr.
   * \steady          TRUE
   */
  auto operator*() const noexcept -> element_type& {
    if (ptr_ == nullptr) {
      amsr::core::Abort("Dereferencing a nullptr is not allowed.");
    }
    // VECTOR NL AutosarC++17_10-M9.3.1: MD_VAC_M9.3.1_ConstMemberFunctionsShallNotReturnNonConstPointers
    return *ptr_;
  }

  /*!
   * \brief           Returns the stored pointer
   * \return          The stored pointer.
   * \context         ANY
   * \pre             The stored pointer is not nullptr.
   * \steady          TRUE
   */
  auto operator->() const noexcept -> element_type* {
    if (ptr_ == nullptr) {
      amsr::core::Abort("Dereferencing a nullptr is not allowed.");
    }
    // VECTOR NL AutosarC++17_10-M9.3.1: MD_VAC_M9.3.1_ConstMemberFunctionsShallNotReturnNonConstPointers
    return ptr_;
  }

  /*!
   * \brief           Return the use count of the shared pointer
   * \details         The use count is the number of SharedPtr instances owning the same object.
   * \return          The use count.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  auto UseCount() const noexcept -> std::int32_t {
    return (reference_counter_ != nullptr) ? reference_counter_->GetUseCount() : 0;
  }

  /*!
   * \brief           Checks whether this is the only SharedPtr instance managing the current object
   * \return          true if use count equals one, false otherwise.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  auto Unique() const noexcept -> bool { return UseCount() == 1; }

  /*!
   * \brief           Bool operator
   * \return          true, if the stored pointer is not nullptr, false otherwise.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  explicit operator bool() const noexcept { return ptr_ != nullptr; }

  /*!
   * \brief           Checks whether this shared pointer precedes \p other
   * \tparam          Y
   *                  Object type of the other shared pointer.
   * \param[in]       other
   *                  The other shared pointer.
   * \return          true if *this precedes other, false otherwise.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  template <typename Y>
  auto OwnerBefore(SharedPtr<Y> const& other) const noexcept -> bool {
    return std::less<internal::ReferenceCounter*>()(this->reference_counter_, other.reference_counter_);
  }

 private:
  /*! \brief          Default deleter */
  using DefaultDeleter = std::default_delete<T>;

  /*! \brief          Default allocator */
  using DefaultAllocator = ara::core::PolymorphicAllocator<char>;

  /*!
   * \brief           Constructs a SharedPtr from the given pointer to an object and the deleter
   * \details         If allocation fails, the object is deleted with the given deleter.
   * \tparam          Y
   *                  Type of the object.
   * \tparam          Deleter
   *                  The deleter that shall be used to delete the object.
   * \param[in]       p
   *                  Pointer to the object.
   * \param[in]       d
   *                  The deleter.
   * \param[in]       a
   *                  The allocator.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  template <typename Y, typename Deleter, typename Alloc, typename = SafeConversion<Y>>
  SharedPtr(Y* p, Deleter d, Alloc a) noexcept : ptr_{p}, reference_counter_{nullptr} {
    using AllocatorType = typename std::allocator_traits<Alloc>::template rebind_alloc<
        internal::ReferenceCounterPtrDeleter<Y*, Deleter, Alloc>>;
    AllocatorType alloc{a};
    internal::ReferenceCounterPtrDeleter<Y*, Deleter, Alloc>* const ref_count{
        std::allocator_traits<AllocatorType>::allocate(alloc, 1)};
    if (ref_count != nullptr) {
      std::allocator_traits<AllocatorType>::construct(alloc, ref_count, p, d, a);
      reference_counter_ = ref_count;
      EnableSharedFromThisWith(ptr_);
    } else {
      d(p);  // VCA_VAC_TEMPLATE_REQ_MET
    }
  }

  /*!
   * \brief           Constructs an empty SharedPtr from nullptr and deleter
   * \tparam          Deleter
   *                  The deleter that shall be used to delete the object.
   * \tparam          Alloc
   *                  Type of the allocator.
   * \param[in]       p
   *                  A nullptr.
   * \param[in]       d
   *                  The deleter.
   * \param[in]       a
   *                  The allocator.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  template <typename Deleter, typename Alloc>
  SharedPtr(std::nullptr_t p, Deleter d, Alloc a) noexcept : ptr_{p}, reference_counter_{nullptr} {
    using AllocatorType = typename std::allocator_traits<Alloc>::template rebind_alloc<
        internal::ReferenceCounterPtrDeleter<std::nullptr_t, Deleter, Alloc>>;
    AllocatorType alloc{a};
    internal::ReferenceCounterPtrDeleter<std::nullptr_t, Deleter, Alloc>* const ref_count{
        std::allocator_traits<AllocatorType>::allocate(alloc, 1)};
    if (ref_count != nullptr) {
      std::allocator_traits<AllocatorType>::construct(alloc, ref_count, p, d, a);
    }
    reference_counter_ = ref_count;
  }

  /*!
   * \brief           Constructs a SharedPtr
   * \details         This constructor is used by AllocateShared.
   * \tparam          Alloc
   *                  Type of the allocator.
   * \tparam          Args
   *                  Argument types.
   * \param[in]       a
   *                  The allocator.
   * \param[in]       args
   *                  Arguments needed to construct an instance of T.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  template <class Alloc, class... Args>
  explicit SharedPtr(Alloc a, Args&&... args) noexcept : ptr_{nullptr}, reference_counter_{nullptr} {
    using AllocatorType = typename internal::ReferenceCounterInplace<T, Alloc>::AllocatorType;
    AllocatorType alloc{a};
    internal::ReferenceCounterInplace<T, Alloc>* const ref_count{
        std::allocator_traits<AllocatorType>::allocate(alloc, 1)};
    if (ref_count != nullptr) {
      std::allocator_traits<Alloc>::construct(a, ref_count, a, std::forward<Args>(args)...);
      ptr_ = ref_count->GetPtr();
      reference_counter_ = ref_count;
      EnableSharedFromThisWith(ptr_);
    }
  }

  /*!
   * \brief           Constructs a SharedPtr from a WeakPtr
   * \tparam          Y
   *                  Object type of the input WeakPtr.
   * \param[in]       wp
   *                  The weak pointer.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  template <typename Y, typename = Compatible<Y>>
  explicit SharedPtr(WeakPtr<Y> const& wp) noexcept
      : ptr_{wp.ptr_}, reference_counter_{wp.reference_counter_} {  // VCA_VAC_VALID_ARGUMENT
    if (reference_counter_ != nullptr) {
      reference_counter_->AddRefLock();  // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
    }
  }

  /*!
   * \brief           Constructs a SharedPtr from the given unique pointer
   * \details         If allocation fails, the unique pointer is unchanged.
   * \tparam          Y
   *                  Type of the object.
   * \tparam          Deleter
   *                  The deleter.
   * \param[in]       up
   *                  The unique pointer.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  // VECTOR NC VectorC++-V12.1.4: MD_VAC_V12.1.4_AllowedExplicitConstructor
  template <typename Y, typename Deleter>
  SharedPtr(amsr::UniquePtr<Y, Deleter>&& up) noexcept : ptr_{up.get()}, reference_counter_{nullptr} {
    using Del2 =
        typename std::conditional<std::is_reference<Deleter>::value,
                                  std::reference_wrapper<typename std::remove_reference<Deleter>::type>, Deleter>::type;
    using AllocatorType = typename internal::ReferenceCounterPtrDeleter<Y*, Del2>::AllocatorType;
    AllocatorType alloc{};
    internal::ReferenceCounterPtrDeleter<Y*, Del2>* const ref_count{
        std::allocator_traits<AllocatorType>::allocate(alloc, 1)};
    if (ref_count != nullptr) {
      std::allocator_traits<AllocatorType>::construct(alloc, ref_count, up.release(),
                                                      std::forward<Deleter>(up.get_deleter()),
                                                      ara::core::PolymorphicAllocator<char>{});
      reference_counter_ = ref_count;
      EnableSharedFromThisWith(ptr_);
    }
  }

  /*!
   * \brief           Get the Deleter
   * \return          Address of the deleter.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  auto GetDeleter() const noexcept -> void* {
    void* result{nullptr};
    if (reference_counter_ != nullptr) {
      result = reference_counter_->GetDeleter();  // VCA_VAC_VALID_FUNC_CALL_RUN_TIME_CHECK
    }
    return result;
  }

  /*!
   * \brief           EnableSharedFromThis Base type of Y if available
   * \tparam          Y
   *                  The object type.
   */
  // VECTOR NC VectorC++-V0.1.5: MD_VAC_V0.1.5_UnusedTypeDeclaration
  template <typename Y>
  using EsftBaseType = decltype(EnableSharedFromThisBase(std::declval<Y*>()));

  /*!
   * \brief           Check whether Y has an EnableSharedFromThis base
   * \tparam          Y
   *                  The object type.
   */
  // VECTOR NC VectorC++-V0.1.5: MD_VAC_V0.1.5_UnusedTypeDeclaration
  template <typename Y, typename = void>
  struct HasEsftBase : std::false_type {};

  /*!
   * \brief           Check whether Y has an EnableSharedFromThis base and is not an array
   * \tparam          Y
   *                  The object type.
   */
  template <typename Y>
  struct HasEsftBase<Y, vac::language::void_t<EsftBaseType<Y>>>
      : std::integral_constant<bool, !std::is_array<T>::value> {};

  /*!
   * \brief           Initializes the EnableSharedFromThis WeakPtr with the given pointer
   * \tparam          Y
   *                  The object type.
   * \param[in]       ptr
   *                  Pointer to the object.
   * \return          Nothing. Return type is void.
   * \context         ANY
   * \pre             -
   * \spec            requires true; \endspec
   */
  template <typename Y, typename Y2 = typename std::remove_cv<Y>::type>
  auto EnableSharedFromThisWith(Y const* ptr) noexcept -> typename std::enable_if<HasEsftBase<Y>::value>::type {
    // VECTOR NL AutosarC++17_10-A7.1.1: MD_VAC_A7.1.1_immutableVariableShallBeConstOrConstexprQualified
    EnableSharedFromThis<Y2> const* base{EnableSharedFromThisBase(ptr)};
    if (base != nullptr) {
      // VECTOR NL AutosarC++17_10-A5.2.3: MD_VAC_A5.2.3_constCastRemovesConstQualificationWeakPtr
      base->WeakAssign(const_cast<Y2*>(ptr), reference_counter_);
    }
  }

  /*!
   * \brief           Does nothing if Y has no EnableSharedFromThis base or is an array
   * \tparam          Y
   *                  The object type.
   * \return          Nothing. Return type is void.
   * \context         ANY
   * \pre             -
   * \spec            requires true; \endspec
   */
  // VECTOR NC AutosarC++17_10-M0.1.8: MD_VAC_M0.1.8_emptyVoidFunctionHasNoExternalSideEffect
  template <typename Y>
  static auto EnableSharedFromThisWith(Y const*) noexcept -> typename std::enable_if<!HasEsftBase<Y>::value>::type {}

  /*!
   * \brief           Pointer to the object
   * \details         Can be different from the owned object, if the aliasing constructor was used.
   */
  element_type* ptr_;

  /*! \brief          Reference counter for the shared pointer */
  internal::ReferenceCounter* reference_counter_;

  /*!
   * \brief           Friend access between different shared pointers
   * \tparam          T1
   *                  Object type.
   */
  // VECTOR NL AutosarC++17_10-A11.3.1: MD_VAC_A11.3.1_doNotUseFriendConversion
  template <typename T1>
  friend class SharedPtr;

  // VECTOR NL AutosarC++17_10-A11.3.1: MD_VAC_A11.3.1_doNotUseFriendConversion
  template <typename T1>
  friend class WeakPtr;

  /*!
   * \brief           Friend access for AllocateShared
   * \tparam          Alloc
   *                  Type of the allocator.
   * \tparam          T1
   *                  Object type.
   * \tparam          Args
   *                  Argument types
   * \param[in]       alloc
   *                  The allocator.
   * \param[in]       args
   *                  Arguments needed to construct an instance of T1.
   * \return          Result containing a shared pointer on success, an error code otherwise.
   */
  // VECTOR NL AutosarC++17_10-A11.3.1: MD_VAC_A11.3.1_doNotUseFriendFreeFunctions
  template <typename T1, typename Alloc, typename... Args>
  friend auto AllocateShared(Alloc const& alloc, Args&&... args) noexcept -> ara::core::Result<SharedPtr<T1>>;

  /*!
   * \brief           Friend access for get_deleter
   * \tparam          Deleter
   *                  Type of the deleter.
   * \tparam          T1
   *                  Object type.
   */
  // VECTOR NL AutosarC++17_10-A11.3.1: MD_VAC_A11.3.1_doNotUseFriendFreeFunctions
  template <typename Deleter, typename T1>
  friend auto GetDeleter(SharedPtr<T1> const&) noexcept -> Deleter*;
};

/*!
 * \brief           Static casts a shared pointer to another type
 * \tparam          T
 *                  Object type of the resulting shared pointer.
 * \tparam          Y
 *                  Object type of the source shared pointer.
 * \param[in]       sp
 *                  The shared pointer.
 * \return          A shared pointer of type T.
 * \context         ANY
 * \pre             -
 * \steady          TRUE
 * \spec            requires true; \endspec
 */
template <typename T, typename Y>
inline auto StaticPointerCast(SharedPtr<Y> const& sp) noexcept -> SharedPtr<T> {
  using ElementType = typename SharedPtr<T>::element_type;
  return {sp, static_cast<ElementType*>(sp.Get())};
}

/*!
 * \brief           Const casts a shared pointer to another type
 * \tparam          T
 *                  Object type of the resulting shared pointer.
 * \tparam          Y
 *                  Object type of the source shared pointer.
 * \param[in]       sp
 *                  The shared pointer.
 * \return          A shared pointer of type T.
 * \context         ANY
 * \pre             -
 * \steady          TRUE
 * \spec            requires true; \endspec
 */
template <typename T, typename Y>
inline auto ConstPointerCast(SharedPtr<Y> const& sp) noexcept -> SharedPtr<T> {
  using ElementType = typename SharedPtr<T>::element_type;
  return {sp, const_cast<ElementType*>(sp.Get())};
}

/*!
 * \brief           Checks if two shared pointers are equal
 * \tparam          T1
 *                  Object type of the first shared pointer.
 * \tparam          T2
 *                  Object type of the second shared pointer.
 * \param[in]       a
 *                  First shared pointer.
 * \param[in]       b
 *                  Second shared pointer.
 * \return          true, if \p a and \p b point to the same object, false otherwise.
 * \context         ANY
 * \pre             -
 * \steady          TRUE
 * \spec            requires true; \endspec
 */
template <typename T1, typename T2>
inline auto operator==(SharedPtr<T1> const& a, SharedPtr<T2> const& b) noexcept -> bool {
  return a.Get() == b.Get();
}

/*!
 * \brief           Compares a shared pointer with a nullptr for equality
 * \tparam          T
 *                  Object type of the shared pointer.
 * \param[in]       a
 *                  The shared pointer.
 * \return          true, if the shared pointer points to nullptr, false otherwise.
 * \context         ANY
 * \pre             -
 * \steady          TRUE
 * \spec            requires true; \endspec
 */
template <typename T>
inline auto operator==(SharedPtr<T> const& a, std::nullptr_t) noexcept -> bool {
  return !a;
}

/*!
 * \brief           Compares a nullptr with a shared pointer for equality
 * \tparam          T
 *                  Object type of the shared pointer.
 * \param[in]       a
 *                  The shared pointer.
 * \return          true, if the shared pointer points to nullptr, false otherwise.
 * \context         ANY
 * \pre             -
 * \steady          TRUE
 * \spec            requires true; \endspec
 */
template <typename T>
inline auto operator==(std::nullptr_t, SharedPtr<T> const& a) noexcept -> bool {
  return !a;
}

/*!
 * \brief           Checks if two shared pointers are not equal
 * \tparam          T1
 *                  Object type of the first shared pointer.
 * \tparam          T2
 *                  Object type of the second shared pointer.
 * \param[in]       a
 *                  First shared pointer.
 * \param[in]       b
 *                  Second shared pointer.
 * \return          true, if \p a and \p b point to different objects, false otherwise.
 * \context         ANY
 * \pre             -
 * \steady          TRUE
 * \spec            requires true; \endspec
 */
template <typename T1, typename T2>
inline auto operator!=(SharedPtr<T1> const& a, SharedPtr<T2> const& b) noexcept -> bool {
  return !(a == b);
}

/*!
 * \brief           Compares a shared pointer with a nullptr for inequality
 * \tparam          T
 *                  Object type of the shared pointer.
 * \param[in]       a
 *                  The shared pointer.
 * \return          true, if the shared pointer does not point to nullptr, false otherwise.
 * \context         ANY
 * \pre             -
 * \steady          TRUE
 * \spec            requires true; \endspec
 */
template <typename T>
inline auto operator!=(SharedPtr<T> const& a, std::nullptr_t) noexcept -> bool {
  return static_cast<bool>(a);
}

/*!
 * \brief           Compares a nullptr with a shared pointer for inequality
 * \tparam          T
 *                  Object type of the shared pointer.
 * \param[in]       a
 *                  The shared pointer.
 * \return          true, if the shared pointer does not point to nullptr, false otherwise.
 * \context         ANY
 * \pre             -
 * \steady          TRUE
 * \spec            requires true; \endspec
 */
template <typename T>
inline auto operator!=(std::nullptr_t, SharedPtr<T> const& a) noexcept -> bool {
  return static_cast<bool>(a);
}

/*!
 * \brief           Checks if \p a is smaller than \p b
 * \tparam          T1
 *                  Object type of the first shared pointer.
 * \tparam          T2
 *                  Object type of the second shared pointer.
 * \param[in]       a
 *                  First shared pointer.
 * \param[in]       b
 *                  Second shared pointer.
 * \return          std::less<CommonType>()(a.Get(), b.Get()) where CommonType is the type that both T1 and T2 can
 *                  implicitly converted to.
 * \context         ANY
 * \pre             -
 * \steady          TRUE
 * \spec            requires true; \endspec
 */
template <typename T1, typename T2>
inline auto operator<(SharedPtr<T1> const& a, SharedPtr<T2> const& b) noexcept -> bool {
  using ElementType1 = typename SharedPtr<T1>::element_type;
  using ElementType2 = typename SharedPtr<T2>::element_type;
  using CommonType = std::common_type_t<ElementType1*, ElementType2*>;
  return std::less<CommonType>()(a.Get(), b.Get());
}

/*!
 * \brief           Checks whether a shared pointer is smaller than a nullptr
 * \tparam          T
 *                  Object type of the shared pointer.
 * \param[in]       a
 *                  The shared pointer.
 * \return          std::less<ElementType*>()(a.Get(), nullptr).
 * \context         ANY
 * \pre             -
 * \steady          TRUE
 * \spec            requires true; \endspec
 */
template <typename T>
inline auto operator<(SharedPtr<T> const& a, std::nullptr_t) noexcept -> bool {
  using ElementType = typename SharedPtr<T>::element_type;
  return std::less<ElementType*>()(a.Get(), nullptr);  // COV_LIBVAC_SMALLER_NULLPTR
}

/*!
 * \brief           Checks whether a nullptr is smaller than a shared pointer
 * \tparam          T
 *                  Object type of the shared pointer.
 * \param[in]       a
 *                  The shared pointer.
 * \return          std::less<ElementType*>()(nullptr, a.Get()).
 * \context         ANY
 * \pre             -
 * \steady          TRUE
 * \spec            requires true; \endspec
 */
template <typename T>
inline auto operator<(std::nullptr_t, SharedPtr<T> const& a) noexcept -> bool {
  using ElementType = typename SharedPtr<T>::element_type;
  return std::less<ElementType*>()(nullptr, a.Get());
}

/*!
 * \brief           Checks if \p a is greater than \p b
 * \tparam          T1
 *                  Object type of the first shared pointer.
 * \tparam          T2
 *                  Object type of the second shared pointer.
 * \param[in]       a
 *                  First shared pointer.
 * \param[in]       b
 *                  Second shared pointer.
 * \return          true, if \p b < \p a , false otherwise.
 * \context         ANY
 * \pre             -
 * \steady          TRUE
 * \spec            requires true; \endspec
 */
template <typename T1, typename T2>
inline auto operator>(SharedPtr<T1> const& a, SharedPtr<T2> const& b) noexcept -> bool {
  return b < a;
}

/*!
 * \brief           Checks whether a shared pointer is greater than a nullptr
 * \tparam          T
 *                  Object type of the shared pointer.
 * \param[in]       a
 *                  The shared pointer.
 * \return          true, if nullptr < \p a , false otherwise.
 * \context         ANY
 * \pre             -
 * \steady          TRUE
 * \spec            requires true; \endspec
 */
template <typename T>
inline auto operator>(SharedPtr<T> const& a, std::nullptr_t) noexcept -> bool {
  return nullptr < a;
}

/*!
 * \brief           Checks whether a nullptr is greater than a shared pointer
 * \tparam          T
 *                  Object type of the shared pointer.
 * \param[in]       a
 *                  The shared pointer.
 * \return          true, if \p a < nullptr, false otherwise.
 * \context         ANY
 * \pre             -
 * \steady          TRUE
 * \spec            requires true; \endspec
 */
template <typename T>
inline auto operator>(std::nullptr_t, SharedPtr<T> const& a) noexcept -> bool {
  return a < nullptr;  // COV_LIBVAC_SMALLER_NULLPTR
}

/*!
 * \brief           Checks if \p a is smaller than or equal \p b
 * \tparam          T1
 *                  Object type of the first shared pointer.
 * \tparam          T2
 *                  Object type of the second shared pointer.
 * \param[in]       a
 *                  First shared pointer.
 * \param[in]       b
 *                  Second shared pointer.
 * \return          true, if !( \p b < \p a ), false otherwise.
 * \context         ANY
 * \pre             -
 * \steady          TRUE
 * \spec            requires true; \endspec
 */
template <typename T1, typename T2>
inline auto operator<=(SharedPtr<T1> const& a, SharedPtr<T2> const& b) noexcept -> bool {
  return !(b < a);
}

/*!
 * \brief           Checks whether a shared pointer is smaller than or equal a nullptr
 * \tparam          T
 *                  Object type of the shared pointer.
 * \param[in]       a
 *                  The shared pointer.
 * \return          true, if !( nullptr < \p a ), false otherwise.
 * \context         ANY
 * \pre             -
 * \steady          TRUE
 * \spec            requires true; \endspec
 */
template <typename T>
inline auto operator<=(SharedPtr<T> const& a, std::nullptr_t) noexcept -> bool {
  return !(nullptr < a);
}

/*!
 * \brief           Checks whether a nullptr is smaller than or equal a shared pointer
 * \tparam          T
 *                  Object type of the shared pointer.
 * \param[in]       a
 *                  The shared pointer.
 * \return          true, if !( \p a < nullptr ), false otherwise.
 * \context         ANY
 * \pre             -
 * \steady          TRUE
 * \spec            requires true; \endspec
 */
template <typename T>
inline auto operator<=(std::nullptr_t, SharedPtr<T> const& a) noexcept -> bool {
  return !(a < nullptr);  // COV_LIBVAC_LARGER_EQUAL_NULLPTR
}

/*!
 * \brief           Checks if \p a is greater than or equal \p b
 * \tparam          T1
 *                  Object type of the first shared pointer.
 * \tparam          T2
 *                  Object type of the second shared pointer.
 * \param[in]       a
 *                  First shared pointer.
 * \param[in]       b
 *                  Second shared pointer.
 * \return          true, if !( \p a < \p b ) , false otherwise.
 * \context         ANY
 * \pre             -
 * \steady          TRUE
 * \spec            requires true; \endspec
 */
template <typename T1, typename T2>
inline auto operator>=(SharedPtr<T1> const& a, SharedPtr<T2> const& b) noexcept -> bool {
  return !(a < b);
}

/*!
 * \brief           Checks whether a shared pointer is greater than or equal a nullptr
 * \tparam          T
 *                  Object type of the shared pointer.
 * \param[in]       a
 *                  The shared pointer.
 * \return          true, if !( \p a < nullptr ) , false otherwise.
 * \context         ANY
 * \pre             -
 * \steady          TRUE
 * \spec            requires true; \endspec
 */
template <typename T>
inline auto operator>=(SharedPtr<T> const& a, std::nullptr_t) noexcept -> bool {
  return !(a < nullptr);  // COV_LIBVAC_LARGER_EQUAL_NULLPTR
}

/*!
 * \brief           Checks whether a nullptr is greater than or equal a shared pointer
 * \tparam          T
 *                  Object type of the shared pointer.
 * \param[in]       a
 *                  The shared pointer.
 * \return          true, if !( nullptr < \p a ), false otherwise.
 * \context         ANY
 * \pre             -
 * \steady          TRUE
 * \spec            requires true; \endspec
 */
template <typename T>
inline auto operator>=(std::nullptr_t, SharedPtr<T> const& a) noexcept -> bool {
  return !(nullptr < a);
}

/*!
 * \brief           Swaps two shared pointers
 * \tparam          T
 *                  Object type of the shared pointers.
 * \param[in]       a
 *                  The first shared pointer.
 * \param[in]       b
 *                  The second shared pointer.
 * \context         ANY
 * \pre             -
 * \steady          TRUE
 * \spec            requires true; \endspec
 */
template <typename T>
void swap(SharedPtr<T>& a, SharedPtr<T>& b) noexcept {
  a.Swap(b);
}

/*!
 * \brief           Creates a SharedPtr including the managed object
 * \tparam          T
 *                  Object type.
 * \tparam          Alloc
 *                  The allocator type.
 * \tparam          Args
 *                  Argument types.
 * \param[in]       alloc
 *                  The allocator.
 * \param[in]       args
 *                  Arguments needed to construct an instance of T.
 * \return          A SharedPtr instance on success, an error code otherwise.
 * \error           amsr::generic::GenErrc::kRuntimeResourceOutOf
 *                  if allocation for the control block including the managed object fails.
 * \context         ANY
 * \pre             -
 * \steady          TRUE
 * \spec            requires true; \endspec
 */
template <typename T, typename Alloc, typename... Args>
auto AllocateShared(Alloc const& alloc, Args&&... args) noexcept -> ara::core::Result<SharedPtr<T>> {
  ara::core::Result<SharedPtr<T>> result{amsr::generic::GenErrc::kRuntimeResourceOutOf};
  SharedPtr<T> sp{alloc, std::forward<Args>(args)...};
  if (sp.UseCount() != 0) {
    result.EmplaceValue(std::move(sp));
  }
  return result;
}

/*!
 * \brief           Creates a SharedPtr including the managed object using a default constructed
 *                  ara::core::PolymorphicAllocator
 * \tparam          T
 *                  Object type.
 * \tparam          Args
 *                  Argument types.
 * \param[in]       args
 *                  Arguments needed to construct an instance of T.
 * \return          A SharedPtr instance on success, an error code otherwise.
 * \error           amsr::generic::GenErrc::kRuntimeResourceOutOf
 *                  if allocation for the control block including the managed object fails.
 * \context         ANY
 * \pre             -
 * \steady          TRUE
 * \spec            requires true; \endspec
 */
template <typename T, typename... Args>
auto MakeShared(Args&&... args) noexcept -> ara::core::Result<SharedPtr<T>> {
  return AllocateShared<T>(ara::core::PolymorphicAllocator<char>{}, std::forward<Args>(args)...);
}

/*!
 * \brief           Get the deleter of the shared pointer
 * \tparam          Deleter
 *                  Type of the deleter.
 * \tparam          T
 *                  Object type of the shared pointer.
 * \param[in]       sp
 *                  The shared pointer.
 * \return          Pointer to the deleter.
 * \context         ANY
 * \pre             -
 * \steady          TRUE
 * \spec            requires true; \endspec
 */
template <typename Deleter, typename T>
auto GetDeleter(SharedPtr<T> const& sp) noexcept -> Deleter* {
  // VECTOR NL AutosarC++17_10-M5.2.8: MD_VAC_M5.2.8_voidPtrToIntPtr
  return static_cast<Deleter*>(sp.GetDeleter());
}

/*!
 * \brief           This class allows an object of type T that is currently managed by an amsr::SharedPtr to safely
 *                  generate additional amsr::SharedPtr instances that all share ownership
 * \vpublic
 * \tparam          T
 *                  Object type.
 * \trace           CREQ-VaCommonLib-EnableSharedFromThis
 */
template <typename T>
class EnableSharedFromThis {
 protected:
  /*!
   * \brief           Constructs an instance of EnableSharedFromThis
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  EnableSharedFromThis() noexcept {}

  /*!
   * \brief           Copy constructs an instance of EnableSharedFromThis
   * \details         Nothing is copied from the argument. weak_this_ is default constructed.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  EnableSharedFromThis(EnableSharedFromThis const&) noexcept {}

  /*!
   * \brief           Move constructs an instance of EnableSharedFromThis
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  EnableSharedFromThis(EnableSharedFromThis&&) noexcept = default;

  /*!
   * \brief           Copy assignment operator does nothing but return *this
   * \return          *this.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  auto operator=(EnableSharedFromThis const&) & noexcept -> EnableSharedFromThis& { return *this; }

  /*!
   * \brief           Move assignment operator
   * \return          *this.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  auto operator=(EnableSharedFromThis&&) & noexcept -> EnableSharedFromThis& = default;

  /*!
   * \brief           Destructor
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  // VCA_VAC_VALID_NONSTATIC_METHOD
  ~EnableSharedFromThis() noexcept {}

 public:
  /*!
   * \brief           Returns a shared pointer to the object referenced by the weak pointer
   * \return          A shared pointer to the object on success, an error code otherwise.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  auto SharedFromThis() noexcept -> ara::core::Result<SharedPtr<T>> {
    return SharedPtr<T>::CreateSharedPtr(this->weak_this_);
  }

  /*!
   * \brief           Returns a shared pointer with type const T to the object referenced by the weak pointer
   * \return          A shared pointer with type const T to the object on success, an error code otherwise.
   * \context         ANY
   * \pre             -
   * \steady          TRUE
   * \spec            requires true; \endspec
   */
  auto SharedFromThis() const noexcept -> ara::core::Result<SharedPtr<T const>> {
    return SharedPtr<T const>::CreateSharedPtr(this->weak_this_);
  }

 private:
  /*!
   * \brief           Assigns a pointer to the object and a reference counter to the weak pointer
   * \tparam          Y
   *                  Object type.
   * \param[in]       ptr
   *                  The pointer to the object.
   * \param[in]       reference_counter
   *                  The reference counter.
   * \context         ANY
   * \pre             -
   * \spec            requires true; \endspec
   */
  template <typename Y>
  void WeakAssign(Y* ptr, internal::ReferenceCounter* reference_counter) const noexcept {
    weak_this_.Assign(ptr, reference_counter);
  }

  /*!
   * \brief           Returns \p ptr
   * \param[in]       ptr
   *                  Pointer to an object which inherits from EnableSharedFromThis.
   * \return          \p ptr.
   * \context         ANY
   * \pre             -
   * \spec            requires true; \endspec
   */
  friend auto EnableSharedFromThisBase(EnableSharedFromThis const* ptr) noexcept -> EnableSharedFromThis const* {
    return ptr;
  }

  /*!
   * \brief           Friend declaration for SharedPtr
   * \tparam          Y
   *                  Object type of the shared pointer.
   */
  // VECTOR NL AutosarC++17_10-A11.3.1: MD_VAC_A11.3.1_doNotUseFriendConversion
  template <typename Y>
  friend class SharedPtr;

  /*! \brief          Weak pointer to object */
  mutable WeakPtr<T> weak_this_{};
};

}  // namespace amsr

// COV_JUSTIFICATION_BEGIN
//
// \ID COV_LIBVAC_LARGER_EQUAL_NULLPTR
//   \ACCEPT TX
//   \REASON Any pointer >= nullptr is always true.
//
// \ID COV_LIBVAC_RARE_MULTITHREAD_CASE
//   \ACCEPT XF xf
//   \REASON The true branch can only be reached if a shared pointer is created from a weak pointer while the last
//           remaining shared pointer managing the object is being destroyed. The use count must be decreased to 0
//           between reading it the first time and trying to change it. This situation is impossible to trigger in a
//           unit test but could sporadically occur.
//
// \ID COV_LIBVAC_SMALLER_NULLPTR
//   \ACCEPT XF
//   \REASON Any pointer < nullptr is always false.
//
// COV_JUSTIFICATION_END

#endif  // LIB_VAC_INCLUDE_AMSR_SHARED_PTR_H_
