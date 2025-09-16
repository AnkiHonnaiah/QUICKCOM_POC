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
/**        \file  ara/core/promise.h
 *        \brief  Specific implementation of a Promise for ara::core.
 *         \unit  VaCommonLib::LanguageSupport::AsynchronousOperations::Promise
 *
 *      \details  ara::core::Promise is the corresponding promise that returns an ara::core::Future.
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_ARA_CORE_PROMISE_H_
#define LIB_VAC_INCLUDE_ARA_CORE_PROMISE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include <memory>
#include <utility>
#include "amsr/shared_ptr.h"
#include "amsr/shared_ptr_resource.h"
#include "ara/core/abort.h"
#include "ara/core/future.h"
#include "ara/core/future_error_domain.h"
#include "ara/core/internal/future_continuation.h"
#include "ara/core/internal/shared_state.h"
#include "ara/core/optional.h"
#include "vac/testing/test_adapter.h"

namespace ara {
namespace core {

/*!
 * \brief  ara::core specific Promise.
 * \details For more information about T and E, please refer to the Result class and its specializations.
 * \tparam T Value type. Can be any type.
 * \tparam E Error type. Defaults to ara::core::ErrorCode, but can be any type.
 * \trace  SPEC-7552479, DSGN-VaCommonLib-AsynchronousOperations
 * \vpublic
 */
template <typename T, typename E = ErrorCode>
class Promise final {
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__Future__PromiseMultiValues, GetFutureMultipleTimesWhenFutureIsWaiting);

  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__Promise__MultiValues, GetFutureMultipleTimesWhenFutureIsWaiting);

  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__Future__Valid, WakeAllWaitingThreads);

  /*! \brief The promised type. */
  using ValueType = T;

  /*! \brief Alias for Result. */
  using R = Result<T, E>;

  /*! \brief An alias for FutureContinuation. */
  using FutureContinuationT = ara::core::internal::FutureContinuation<T, E>;
  /*! \brief Shared pointer type for FutureContinuation. */
  using FutureContinuationPtr = amsr::SharedPtr<FutureContinuationT>;

  /*! \brief An alias for SharedState. */
  using SharedStateT = internal::SharedState<R, future_errc>;
  /*! \brief Shared pointer type for SharedState. */
  using SharedStatePtr = amsr::SharedPtr<SharedStateT>;

  /*!
   * \brief Helper function to allocate memory for SharedState & FutureContinuation using provided memory resources.
   * \param[in] shared_state_resource Memory resource to be used to allocate memory for SharedState.
   * \param[in] future_cont_resource Memory resource to be used to allocate memory for FutureContinuation.
   * \vprivate Component Private.
   */
  void AllocateSharedStateAndFutureContinuationUsingMemoryResource(MemoryResource* shared_state_resource,
                                                                   MemoryResource* future_cont_resource) noexcept {
    PolymorphicAllocator<SharedStateT> const shared_state_allocator{shared_state_resource};
    Result<SharedStatePtr> const shared_state_result{amsr::AllocateShared<SharedStateT>(shared_state_allocator)};
    if (shared_state_result.HasValue()) {
      shared_state_ = shared_state_result.Value();
    }
    PolymorphicAllocator<FutureContinuationT> const future_cont_allocator{future_cont_resource};
    Result<FutureContinuationPtr> future_cont_result{amsr::AllocateShared<FutureContinuationT>(future_cont_allocator)};
    if (future_cont_result.HasValue()) {
      future_continuation_ = future_cont_result.Value();
    }
  }

  /*!
   * \brief     Helper function to allocate memory for SharedState & FutureContinuation using provided allocator.
   * \param[in] allocator PolymorphicAllocator to be used to allocate memory  for SharedState and FutureContinuation.
   * \vprivate  Component Private.
   */
  void AllocateSharedStateAndFutureContinuationUsingAllocator(PolymorphicAllocator<T> const& allocator) noexcept {
    Result<SharedStatePtr> const shared_state_result{amsr::AllocateShared<SharedStateT>(allocator)};
    if (shared_state_result.HasValue()) {
      shared_state_ = shared_state_result.Value();
    }
    Result<FutureContinuationPtr> const future_cont_result{amsr::AllocateShared<FutureContinuationT>(allocator)};
    if (future_cont_result.HasValue()) {
      future_continuation_ = future_cont_result.Value();
    }
  }

 public:
  /*!
   * \brief Type used for cleanup callables.
   * \vprivate Product Private
   */
  using CleanUpCallable = typename Future<T, E>::CleanUpCallable;

  /*!
   * \brief Default constructor.
   * \spec
   *   requires true;
   * \endspec
   * \trace SPEC-7552480
   * \vpublic
   */
  // VECTOR NL AutosarC++17_10-A12.1.5: MD_VAC_A12.1.5_useDelegatingConstructor
  Promise() noexcept {
    PolymorphicAllocator<ValueType> const allocator{ara::core::GetDefaultResource()};
    AllocateSharedStateAndFutureContinuationUsingAllocator(allocator);
  }

  /*!
   * \brief     Constructs Promise using ara::core::MemoryResource.
   * \details   Uses given memory resource to pre-allocate memory for SharedState and Future::then()
   *            can't be used when Promise is constructed with pre-allocated SharedState.
   * \param[in] shared_state_resource Memory resource to be used to pre-allocate memory for SharedState.
   * \param[in] future_cont_resource  Memory resource to be used to pre-allocate memory for FutureContinuation.
   * \vprivate  Product private
   * \spec
   *   requires true;
   * \endspec
   */
  explicit Promise(MemoryResource* shared_state_resource,
                   MemoryResource* future_cont_resource = NullMemoryResource()) noexcept {
    AllocateSharedStateAndFutureContinuationUsingMemoryResource(shared_state_resource, future_cont_resource);
  }

  /*!
   * \brief     Constructs Promise using provided ara::core::PolymorphicAllocator.
   * \param[in] allocator PolymorphicAllocator to be used to allocate memory for SharedState & FutureContinuation.
   * \vprivate  Component Private.
   */
  explicit Promise(PolymorphicAllocator<ValueType> const& allocator) noexcept {
    AllocateSharedStateAndFutureContinuationUsingAllocator(allocator);
  }

  /*!
   * \brief Default copy constructor deleted.
   * \trace SPEC-7552481
   * \vpublic
   */
  Promise(Promise const&) = delete;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief Move constructor.
   * \spec
   *   requires true;
   * \endspec
   * \trace SPEC-7552482
   * \vpublic
   */
  Promise(Promise&&) noexcept = default;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief Destructor.
   * \spec
   *   requires true;
   * \endspec
   * \trace SPEC-7552483
   * \vpublic
   */
  ~Promise() noexcept {
    Optional<std::lock_guard<std::mutex>> lock{};
    GetLock(lock, future_continuation_);
    AbandonSharedState();
  }  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT

  /*!
   * \brief  Default copy assignment operator deleted.
   * \return
   * \trace  SPEC-7552484
   * \vpublic
   */
  Promise& operator=(Promise const&) & = delete;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief     Move assignment operator.
   * \param[in] other  The promise to move.
   * \return    A reference to "*this".
   * \spec
   *   requires true;
   * \endspec
   * \trace     SPEC-7552485
   * \vpublic
   */
  auto operator=(Promise&& other) & noexcept -> Promise& {
    // Temporary copy of the future continuation, so that it and with it the mutex in the lock is not destroyed when
    // overwritten with other.future_continuation_.
    // VECTOR NL AutosarC++17_10-A18.5.8: MD_VAC_A18.5.8_localObjectsShallBeAllocatedOnStack
    FutureContinuationPtr const future_continuation_copy{future_continuation_};
    Optional<std::lock_guard<std::mutex>> lock{};
    GetLock(lock, future_continuation_copy);
    AbandonSharedState();
    this->shared_state_ = std::move(other.shared_state_);                // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    this->future_continuation_ = std::move(other.future_continuation_);  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    return *this;
  }

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief          Exchanges the shared states of this and other.
   * \param[in, out] other The other instance.
   * \pre            -
   * \spec
   *   requires true;
   * \endspec
   * \trace          SPEC-7552486
   * \vpublic
   */
  void swap(Promise& other) noexcept {
    using std::swap;
    swap(shared_state_, other.shared_state_);
    swap(future_continuation_, other.future_continuation_);
  }

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConst
  /*!
   * \brief   Return a Future with the same shared state.
   * \details The returned Future is ready as soon as this Promise sets a value or an error.
   *          This method must only be called once as it is not allowed to have multiple Futures per Promise.
   *          Calling this method twice, or more, on the same Promise will set the error future_already_retrieved
   *          to both the existing Future and the newly created Future. If Promise has no shared state,
   *          ara::core::Abort() will be called.
   * \pre     -
   * \spec
   *   requires true;
   * \endspec
   * \return  Future with same shared state.
   * \trace   SPEC-7552487
   * \vpublic
   */
  auto get_future() noexcept -> Future<ValueType, E> {
    // See justification comment in AbandonSharedState().
    if (shared_state_ == nullptr) {
      ara::core::Abort(
          "ara::core::Promise: The Future associated with this Promise cannot be retrieved, since it has no shared "
          "state.");
    }
    shared_state_->MakeValid();
    SharedStatePtr const shared_state{shared_state_};
    FutureContinuationPtr const future_continuation{future_continuation_};
    Future<ValueType, E> returned_future{shared_state, future_continuation};
    return returned_future;
  }

  /*!
   * \brief   Return a Future with the same shared state and a callable to be executed when ~Future() is called.
   * \details The returned Future is ready as soon as this Promise sets a value or an error.
   *          This method must only be called once as it is not allowed to have multiple Futures per Promise. Calling
   *          this method twice, or more, on the same Promise will set the error future_already_retrieved to both the
   *          existing Future and the newly created Future.
   *          If the callable is designed to destroy the Promise, the callable must be thread-safe in respect to the
   *          destruction of the Promise. This means that the owner of the Promise need to have full control over when
   *          the Promise is destroyed to avoid race-conditions. There are no internal logic for thread-safety
   *          related to this callable. If Promise has no shared state, ara::core::Abort() will be called.
   * \param   cleanup A callable to be executed when the connected Future is destroyed.
   *          Must have signature void () noexcept.
   * \pre     -
   * \spec
   *   requires true;
   * \endspec
   * \return  Future with same shared state.
   * \vprivate Product Private
   */
  auto get_future(CleanUpCallable&& cleanup) const noexcept -> Future<ValueType, E> {
    // See justification comment in AbandonSharedState().
    if (shared_state_ == nullptr) {
      ara::core::Abort(
          "ara::core::Promise: The Future associated with this Promise cannot be retrieved, since it has no shared "
          "state.");
    }
    shared_state_->MakeValid();
    SharedStatePtr const shared_state{shared_state_};
    FutureContinuationPtr const future_continuation{future_continuation_};
    Future<ValueType, E> returned_future{shared_state, future_continuation, std::move(cleanup)};
    return returned_future;
  }

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConst
  /*!
   * \brief   Copy result into the Future.
   * \details If Promise has no shared state, ara::core::Abort() will be called.
   * \param   value Value to be set.
   * \pre   -
   * \spec
   *   requires true;
   * \endspec
   * \trace SPEC-7552488
   * \vpublic
   */
  void set_value(ValueType const& value) noexcept {
    if (shared_state_ == nullptr) {
      ara::core::Abort("ara::core::Promise: The value of this Promise cannot be set, since it has no shared state.");
    }
    R const r{R::template FromValue(value)};
    SetValueAndExecuteCallBack(r);
  }

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConst
  /*!
   * \brief     Move the result into the Future.
   * \details   If Promise has no shared state, ara::core::Abort() will be called.
   * \param[in] value Value to be set.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \trace     SPEC-7552489
   * \vpublic
   */
  void set_value(ValueType&& value) noexcept {
    if (shared_state_ == nullptr) {
      ara::core::Abort("ara::core::Promise: The value of this Promise cannot be set, since it has no shared state.");
    }
    R r{R::FromValue(std::move(value))};
    SetValueAndExecuteCallBack(std::move(r));
  }

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConst
  /*!
   * \brief     Save the executable into shared state of the connected Future.
   * \details   If Promise has no shared state, ara::core::Abort() will be called.
   * \tparam    F type of executable.
   * \param[in] exec executable to be executed.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \vprivate
   */
  template <typename F>
  void SetExecutable(F&& exec) noexcept {
    // VECTOR Next Line AutosarC++17_10-A18.5.8: MD_VAC_A18.5.8_localObjectsShallBeAllocatedOnStack
    SharedStatePtr const shared_state_local_copy{shared_state_};
    // See justification comment in AbandonSharedState().
    assert(shared_state_local_copy.Get() != nullptr);  // COV_MSR_INV_STATE_ASSERT
    shared_state_local_copy->SetData(std::forward<F>(exec));
  }

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConst
  /*!
   * \brief     Move an error into the Future.
   * \details   Calling this method if SharedState is nullptr triggers ara::core::Abort().
   * \param[in] err The error to store.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \trace     SPEC-7552490
   * \vpublic
   */
  void SetError(E&& err) noexcept {
    if (shared_state_ == nullptr) {
      ara::core::Abort("ara::core::Promise: The error of this Promise cannot be set, since it has no shared state.");
    }
    R r{R::FromError(std::move(err))};
    SetValueAndExecuteCallBack(std::move(r));
  }

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConst
  /*!
   * \brief   Copy an error into the Future.
   * \details If Promise has no shared state, ara::core::Abort() will be called.
   * \param err The error to store.
   * \pre   -
   * \spec
   *   requires true;
   * \endspec
   * \trace SPEC-7552491
   * \vpublic
   */
  void SetError(E const& err) noexcept {
    if (shared_state_ == nullptr) {
      ara::core::Abort("ara::core::Promise: The error of this Promise cannot be set, since it has no shared state.");
    }
    R const r{R::template FromError(err)};
    SetValueAndExecuteCallBack(r);
  }

 private:
  /*! \brief Future continuation object to manage the call back function. */
  FutureContinuationPtr future_continuation_{nullptr};

  /*! \brief The shared state between *this and the connected Future. */
  SharedStatePtr shared_state_{nullptr};

  /*!
   * \brief Abandons the shared state.
   * \details Abandoning the shared state means that it stores the broken_premise error in the shared state, makes it
   * ready and then releases it.
   * \pre  -
   * \spec
   *   requires true;
   * \endspec
   */
  void AbandonSharedState() noexcept {
    if (shared_state_) {
      // The shared state pointer is created at the creation of the object and will only be
      // destroyed when the object is destructed. Therefore, all accesses to this pointer is
      // always valid during the lifetime of the object.
      assert(shared_state_.Get() != nullptr);  // COV_MSR_INV_STATE_ASSERT
      shared_state_->BreakPromise();
      shared_state_.Reset();  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
      // Call back has been set, but not executed.
      // If this occurs, it means that the Promise is destroyed before a value has been set.
      // BreakPromise() has now set the value to future_errc::broken_promise.
      if (future_continuation_.Get() != nullptr) {
        future_continuation_->ExecuteCallBack();
      }
    }
  }

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConst
  /*!
   * \brief Set value and execute call back if one exist.
   * \param r The Result to set.
   * \pre   -
   * \spec
   *   requires true;
   * \endspec
   */
  void SetValueAndExecuteCallBack(R const& r) noexcept {
    // A local copy of future_continuation and shared_state to support the following code:
    //
    // Thread A:               Thread B:
    // {                       {
    //   Promise p
    //   Future f
    //
    //   f.GetResult()           p.set_value()
    // }                       }
    //
    // The local copy prevents premature destruction of future_continuation and/or shared_state when Thread A goes out
    // of scope before thread B has completed p.set_value().
    // VECTOR Next Line AutosarC++17_10-A18.5.8: MD_VAC_A18.5.8_localObjectsShallBeAllocatedOnStack
    FutureContinuationPtr const fut_cont{future_continuation_};

    Optional<std::lock_guard<std::mutex>> lock{};
    GetLock(lock, fut_cont);
    // VECTOR Next Line AutosarC++17_10-A18.5.8: MD_VAC_A18.5.8_localObjectsShallBeAllocatedOnStack
    SharedStatePtr const shared_state_local_copy{shared_state_};
    // See justification comment in AbandonSharedState().
    assert(shared_state_local_copy.Get() != nullptr);  // COV_MSR_INV_STATE_ASSERT
    shared_state_local_copy->SetData(r);
    if (fut_cont != nullptr) {
      // VCA_VAC_VALID_NONSTATIC_METHOD
      fut_cont->ExecuteCallBack();
    }
  }

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConst
  /*!
   * \brief     Set value and execute call back if one exist.
   * \param[in] r The Result to set.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   */
  void SetValueAndExecuteCallBack(R&& r) noexcept {
    // A copy here to prevent the shared state to destruct prematurely.
    // See SetValueAndExecuteCallBack(R const&) for details.
    // VECTOR Next Line AutosarC++17_10-A18.5.8: MD_VAC_A18.5.8_localObjectsShallBeAllocatedOnStack
    FutureContinuationPtr const fut_cont{future_continuation_};
    Optional<std::lock_guard<std::mutex>> lock{};
    GetLock(lock, fut_cont);
    // VECTOR Next Line AutosarC++17_10-A18.5.8: MD_VAC_A18.5.8_localObjectsShallBeAllocatedOnStack
    SharedStatePtr const shared_state_local_copy{shared_state_};
    // See justification comment in AbandonSharedState().
    assert(shared_state_local_copy.Get() != nullptr);  // COV_MSR_INV_STATE_ASSERT
    shared_state_local_copy->SetData(std::move(r));
    if (fut_cont != nullptr) {
      // VCA_VAC_VALID_NONSTATIC_METHOD
      fut_cont->ExecuteCallBack();
    }
  }

  /*! \brief Locks a mutex if future_continuation_ is not nullptr. */
  static void GetLock(Optional<std::lock_guard<std::mutex>>& lock,
                      FutureContinuationPtr const& future_continuation) noexcept {
    if (future_continuation != nullptr) {
      // VCA_VAC_VALID_ARGUMENT
      lock.emplace(future_continuation->GetCallBackMutex());
    }
  }

  // VECTOR Next Construct AutosarC++17_10-A11.3.1: MD_VAC_A11.3.1_doNotUseFriend
  /*!
   * \brief  Friend to only provide CallBackHolder with access to the shared state.
   */
  template <typename, typename, typename, typename, typename>
  friend class internal::CallBackHolder;
};

/*!
 * \brief Promise specialization for void.
 * \tparam E Error type.
 * \trace DSGN-VaCommonLib-AsynchronousOperations
 * \vpublic
 */
template <typename E>
class Promise<void, E> final {
  /*! Friend declaration for testing purposes */
  FRIEND_TEST(UT__Future__Void__Valid, WakeAllWaitingThreads);

  /*! \brief The promised type. */
  using ValueType = void;

  /*! \brief Alias for Result. */
  using R = Result<ValueType, E>;

  /*! \brief An alias for FutureContinuation. */
  using FutureContinuationT = ara::core::internal::FutureContinuation<ValueType, E>;
  /*! \brief Shared pointer type for FutureContinuation. */
  using FutureContinuationPtr = amsr::SharedPtr<FutureContinuationT>;

  /*! \brief An alias for SharedState. */
  using SharedStateT = internal::SharedState<R, future_errc>;
  /*! \brief Shared pointer type for SharedState. */
  using SharedStatePtr = amsr::SharedPtr<SharedStateT>;

  /*!
   * \brief Helper function to allocate memory for SharedState and FutureContinuation using provided memory resources.
   * \param[in] shared_state_resource Memory resource to be used to allocate memory for SharedState.
   * \param[in] future_cont_resource Memory resource to be used to allocate memory for FutureContinuation.
   * \vprivate Component Private.
   */
  void AllocateSharedStateAndFutureContinuationUsingMemoryResource(MemoryResource* shared_state_resource,
                                                                   MemoryResource* future_cont_resource) noexcept {
    PolymorphicAllocator<SharedStateT> const shared_state_allocator{shared_state_resource};
    Result<SharedStatePtr> const shared_state_result{amsr::AllocateShared<SharedStateT>(shared_state_allocator)};
    if (shared_state_result.HasValue()) {
      shared_state_ = shared_state_result.Value();
    }
    PolymorphicAllocator<FutureContinuationT> const future_cont_allocator{future_cont_resource};
    Result<FutureContinuationPtr> future_cont_result{amsr::AllocateShared<FutureContinuationT>(future_cont_allocator)};
    if (future_cont_result.HasValue()) {
      future_continuation_ = future_cont_result.Value();
    }
  }

  /*!
   * \brief Helper function to allocate memory for SharedState and FutureContinuation.
   * \param[in] allocator Allocator to be used to allocate memory for SharedState & FutureContinuation.
   * \vprivate Component Private.
   */
  void AllocateSharedStateAndFutureContinuationUsingAllocator(
      PolymorphicAllocator<ValueType> const& allocator) noexcept {
    Result<SharedStatePtr> const shared_state_result{amsr::AllocateShared<SharedStateT>(allocator)};
    if (shared_state_result.HasValue()) {
      shared_state_ = shared_state_result.Value();
    }
    Result<FutureContinuationPtr> const future_cont_result{amsr::AllocateShared<FutureContinuationT>(allocator)};
    if (future_cont_result.HasValue()) {
      future_continuation_ = future_cont_result.Value();
    }
  }

 public:
  /*!
   * \brief Type used for cleanup callables.
   * \vprivate Product Private
   */
  using CleanUpCallable = typename Future<void, E>::CleanUpCallable;

  /*!
   * \brief Default constructor.
   * \spec
   *   requires true;
   * \endspec
   * \trace SPEC-7552480
   * \vpublic
   */
  // VECTOR NL AutosarC++17_10-A12.1.5: MD_VAC_A12.1.5_useDelegatingConstructor
  Promise() noexcept {
    PolymorphicAllocator<ValueType> const allocator{ara::core::GetDefaultResource()};
    AllocateSharedStateAndFutureContinuationUsingAllocator(allocator);
  }

  /*!
   * \brief     Constructs Promise using ara::core::MemoryResource.
   * \details   Uses given memory resource to pre-allocate memory for SharedState and Future::then()
   *            can't be used when Promise is constructed with pre-allocated SharedState.
   * \param[in] shared_state_resource Memory resource to be used to pre-allocate memory for SharedState.
   * \param[in] future_cont_resource  Memory resource to be used to pre-allocate memory for FutureContinuation.
   * \vprivate  Product private
   * \spec
   *   requires true;
   * \endspec
   */
  explicit Promise(MemoryResource* shared_state_resource,
                   MemoryResource* future_cont_resource = NullMemoryResource()) noexcept {
    AllocateSharedStateAndFutureContinuationUsingMemoryResource(shared_state_resource, future_cont_resource);
  }

  /*!
   * \brief     Constructs Promise using provided ara::core::PolymorphicAllocator.
   * \param[in] allocator PolymorphicAllocator to be used to allocate memory for
   *            SharedState and FutureContinuation.
   * \vprivate  Component Private.
   */
  explicit Promise(PolymorphicAllocator<ValueType> const& allocator) noexcept {
    AllocateSharedStateAndFutureContinuationUsingAllocator(allocator);
  }

  /*!
   * \brief Default copy constructor deleted.
   * \trace SPEC-7552481
   * \vpublic
   */
  Promise(Promise const&) = delete;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief Move constructor.
   * \spec
   *   requires true;
   * \endspec
   * \trace SPEC-7552482
   * \vpublic
   */
  Promise(Promise&&) noexcept = default;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief Destructor.
   * \spec
   *   requires true;
   * \endspec
   * \trace SPEC-7552483
   * \vpublic
   */
  ~Promise() noexcept {
    Optional<std::lock_guard<std::mutex>> lock{};
    GetLock(lock, future_continuation_);
    AbandonSharedState();
  }  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT

  /*!
   * \brief  Default copy assignment operator deleted.
   * \return
   * \trace  SPEC-7552484
   * \vpublic
   */
  Promise& operator=(Promise const&) & = delete;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief     Move assignment operator.
   * \param[in] other  The promise to move.
   * \return    A reference to "*this".
   * \spec
   *   requires true;
   * \endspec
   * \trace     SPEC-7552485
   * \vpublic
   */
  Promise& operator=(Promise&& other) & noexcept {
    // Temporary copy of the future continuation, so that it and with it the mutex in the lock is not destroyed when
    // overwritten with other.future_continuation_.
    // VECTOR NL AutosarC++17_10-A18.5.8: MD_VAC_A18.5.8_localObjectsShallBeAllocatedOnStack
    FutureContinuationPtr const future_continuation_copy{future_continuation_};
    Optional<std::lock_guard<std::mutex>> lock{};
    GetLock(lock, future_continuation_copy);
    AbandonSharedState();
    this->shared_state_ = std::move(other.shared_state_);                // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    this->future_continuation_ = std::move(other.future_continuation_);  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    return *this;
  }
  // VCA_VAC_VALID_MEMBER_VARIABLE_ACCESS

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief          Exchanges the shared states of this and other.
   * \param[in, out] other The other instance.
   * \pre            -
   * \spec
   *   requires true;
   * \endspec
   * \trace          SPEC-7552486
   * \vpublic
   */
  void swap(Promise& other) noexcept {
    using std::swap;
    swap(shared_state_, other.shared_state_);
    swap(future_continuation_, other.future_continuation_);
  }

  /*!
   * \brief   Return a Future with the same shared state.
   * \details The returned Future is ready as soon as this Promise sets a value or an error.
   *          This method must only be called once as it is not allowed to have multiple Futures per Promise. Calling
   *          this method twice, or more, on the same Promise will set the error future_already_retrieved to both the
   *          existing Future and the newly created Future. If Promise has no shared state, ara::core::Abort() will be
   *          called.
   * \pre
   * - \spec requires true;
   * \endspec
   * \return  Future with same shared state.
   * \trace   SPEC-7552487 \vpublic
   */
  auto get_future() const noexcept -> Future<ValueType, E> {
    // See justification comment in AbandonSharedState().
    if (shared_state_ == nullptr) {
      ara::core::Abort(
          "ara::core::Promise: The Future associated with this Promise cannot be retrieved, since it has no shared "
          "state.");
    }
    shared_state_->MakeValid();
    SharedStatePtr const shared_state{shared_state_};
    FutureContinuationPtr const future_continuation{future_continuation_};
    Future<ValueType, E> returned_future{shared_state, future_continuation};
    return returned_future;
  }

  /*!
   * \brief   Return a Future with the same shared state and a callable to be executed when ~Future() is called.
   * \details The returned Future is ready as soon as this Promise sets a value or an error.
   *          This method must only be called once as it is not allowed to have multiple Futures per Promise. Calling
   *          this method twice, or more, on the same Promise will set the error future_already_retrieved to both the
   *          existing Future and the newely created Future.
   *          If the callable is designed to destroy the Promise, the callable must be thread-safe in respect to the
   *          destruction of the Promise. This means that the owner of the Promise need to have full control over when
   *          the Promise is destroyed to avoid race-conditions. There are no internal logic for thread-safety
   *          related to this callable. If Promise has no shared state, ara::core::Abort() will be called.
   * \param   cleanup A callable to be executed when the connected Future is destroyed.
   *          Must have signature void () noexcept.
   * \pre     -
   * \spec
   *   requires true;
   * \endspec
   * \return  Future with same shared state.
   * \vprivate Product Private
   */
  auto get_future(CleanUpCallable&& cleanup) const noexcept -> Future<ValueType, E> {
    // See justification comment in AbandonSharedState().
    if (shared_state_ == nullptr) {
      ara::core::Abort(
          "ara::core::Promise: The Future associated with this Promise cannot be retrieved, "
          "since it has no shared state.");
    }
    shared_state_->MakeValid();
    SharedStatePtr const shared_state{shared_state_};
    FutureContinuationPtr const future_continuation{future_continuation_};
    Future<ValueType, E> returned_future{shared_state, future_continuation, std::move(cleanup)};
    return returned_future;
  }

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConst
  /*!
   * \brief   Set a value with void.
   * \details If Promise has no shared state, ara::core::Abort() will be called.
   * \pre   -
   * \spec
   *   requires true;
   * \endspec
   * \vpublic
   */
  void set_value() noexcept {
    if (shared_state_ == nullptr) {
      ara::core::Abort("ara::core::Promise: The value of this Promise cannot be set, since it has no shared state.");
    }
    R const r{R::FromValue()};
    SetValueAndExecuteCallBack(r);
  }

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConst
  /*!
   * \brief     Save the executable into shared state of the connected Future.
   * \details   The executable should return ara::core::Result<void, E> and takes no arguments.
   * \tparam    F type of executable.
   * \param[in] exec executable to be executed.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \vprivate
   */
  template <typename F>
  void SetExecutable(F&& exec) noexcept {
    // VECTOR Next Line AutosarC++17_10-A18.5.8: MD_VAC_A18.5.8_localObjectsShallBeAllocatedOnStack
    SharedStatePtr const shared_state_local_copy{shared_state_};
    // See justification comment in AbandonSharedState().
    assert(shared_state_local_copy.Get() != nullptr);  // COV_MSR_INV_STATE_ASSERT
    shared_state_local_copy->SetData(std::forward<F>(exec));
  }  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConst
  /*!
   * \brief     Move an error into the Future.
   * \details   If Promise has no shared state, ara::core::Abort() will be called.
   * \param[in] err The error to store.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \trace     SPEC-7552490
   * \vpublic
   */
  void SetError(E&& err) noexcept {
    if (shared_state_ == nullptr) {
      ara::core::Abort("ara::core::Promise: The error of this Promise cannot be set, since it has no shared state.");
    }
    R r{R::FromError(std::move(err))};

    SetValueAndExecuteCallBack(std::move(r));
  }

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConst
  /*!
   * \brief   Copy an error into the Future.
   * \details If Promise has no shared state, ara::core::Abort() will be called.
   * \param   err The error to store.
   * \pre   -
   * \spec
   *   requires true;
   * \endspec
   * \trace SPEC-7552491
   * \vpublic
   */
  void SetError(E const& err) noexcept {
    if (shared_state_ == nullptr) {
      ara::core::Abort("ara::core::Promise: The error of this Promise cannot be set, since it has no shared state.");
    }
    R const r{R::template FromError(err)};

    SetValueAndExecuteCallBack(r);
  }

 private:
  /*! \brief Future continuation object to manage the call back function. */
  FutureContinuationPtr future_continuation_{nullptr};

  /*! \brief The shared state between *this and the connected Future. */
  SharedStatePtr shared_state_{nullptr};

  /*!
   * \brief Abandons the shared state.
   * \details Abandoning the shared state means that it stores the broken_premise error in the shared state, makes it
   * ready and then releases it.
   * \pre  -
   * \spec
   *   requires true;
   * \endspec
   */
  void AbandonSharedState() noexcept {
    if (shared_state_) {
      // The shared state pointer is created at the creation of the object and will only be
      // destroyed when the object is destructed. Therefore, all accesses to this pointer is
      // always valid during the lifetime of the object.
      assert(shared_state_.Get() != nullptr);  // COV_MSR_INV_STATE_ASSERT
      shared_state_->BreakPromise();
      shared_state_.Reset();  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
      // Call back has been set, but not executed.
      // If this occurs, it means that the Promise is destroyed before a value has been set.
      // BreakPromise() has now set the value to future_errc::broken_promise.
      if (future_continuation_.Get() != nullptr) {
        future_continuation_->ExecuteCallBack();
      }
    }
  }

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConst
  /*!
   * \brief Set value and execute call back if one exist.
   * \param r The Result to set.
   * \pre   -
   * \spec
   *   requires true;
   * \endspec
   */
  void SetValueAndExecuteCallBack(R const& r) noexcept {
    // A copy here to prevent the shared state to destruct prematurely.
    // See the non void Promise definition of SetValueAndExecuteCallBack(R const&) for details.
    // VECTOR Next Line AutosarC++17_10-A18.5.8: MD_VAC_A18.5.8_localObjectsShallBeAllocatedOnStack
    FutureContinuationPtr const fut_cont{future_continuation_};
    // VECTOR Next Line AutosarC++17_10-A18.5.8: MD_VAC_A18.5.8_localObjectsShallBeAllocatedOnStack
    SharedStatePtr const shared_state_local_copy{shared_state_};
    Optional<std::lock_guard<std::mutex>> lock{};
    GetLock(lock, fut_cont);
    // See justification comment in AbandonSharedState().
    assert(shared_state_local_copy.Get() != nullptr);  // COV_MSR_INV_STATE_ASSERT
    shared_state_local_copy->SetData(r);
    if (fut_cont != nullptr) {
      // VCA_VAC_VALID_NONSTATIC_METHOD
      fut_cont->ExecuteCallBack();
    }
  }

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConst
  /*!
   * \brief     Set value and execute call back if one exist.
   * \param[in] r The Result to set.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   */
  void SetValueAndExecuteCallBack(R&& r) noexcept {
    // A copy here to prevent the shared state to destruct prematurely.
    // See the non void Promise definition of SetValueAndExecuteCallBack(R const&) for details.
    // VECTOR Next Line AutosarC++17_10-A18.5.8: MD_VAC_A18.5.8_localObjectsShallBeAllocatedOnStack
    FutureContinuationPtr const fut_cont{future_continuation_};
    Optional<std::lock_guard<std::mutex>> lock{};
    GetLock(lock, fut_cont);
    // VECTOR Next Line AutosarC++17_10-A18.5.8: MD_VAC_A18.5.8_localObjectsShallBeAllocatedOnStack
    SharedStatePtr const shared_state_local_copy{shared_state_};
    // See justification comment in AbandonSharedState().
    assert(shared_state_local_copy.Get() != nullptr);  // COV_MSR_INV_STATE_ASSERT
    shared_state_local_copy->SetData(std::move(r));
    if (fut_cont != nullptr) {
      // VCA_VAC_VALID_NONSTATIC_METHOD
      fut_cont->ExecuteCallBack();
    }
  }

  /*! \brief Locks a mutex if future_continuation_ is not nullptr. */
  static void GetLock(Optional<std::lock_guard<std::mutex>>& lock,
                      FutureContinuationPtr const& future_continuation) noexcept {
    if (future_continuation != nullptr) {
      // VCA_VAC_VALID_ARGUMENT
      lock.emplace(future_continuation->GetCallBackMutex());
    }
  }

  // VECTOR Next Construct AutosarC++17_10-A11.3.1: MD_VAC_A11.3.1_doNotUseFriend
  /*!
   * \brief  Friend to only provide CallBackHolder with access to the shared state.
   */
  template <typename, typename, typename, typename, typename>
  friend class internal::CallBackHolder;
};

/*!
 * \brief   An alias for amsr::SharedPtrResource.
 * \details This is the recommended resource to be used as upstream memory resource to allocate memory
 *          for SharedState when Promise is constructed using pre-allocated SharedState. If this resource
 *          can not allocate enough memory required for SharedState then ara::core::Abort() is called.
 \tparam    T The type for memory resource and SharedState value.
 \tparam    N The number of shared pointers to allocate memory for.
 \tparam    E The type for SharedState error.
 */
template <typename T, size_t N = 1, typename E = ara::core::ErrorCode>
using SharedStateMemoryResource = amsr::SynchronizedSharedPtrResource<
    ara::core::internal::SharedState<ara::core::Result<T, E>, ara::core::future_errc>, N, false>;

}  // namespace core
}  // namespace ara

/*!
 * \exclusivearea   ara::core::internal::FutureContinuation::call_back_mutex_
 *                  Protect against parallel execution and setting the callback function
 *                  for future continuation
 * \protects        ara::core::Promise::fut_cont
 * \usedin          ara::core::Promise::set_value
 *                  ara::core::Promise::SetError
 * \exclude         All other methods of a Promise object
 * \length          LONG
 *                  This exclusive area may cover calls to several sub-functions
 * \endexclusivearea
 */

#endif  // LIB_VAC_INCLUDE_ARA_CORE_PROMISE_H_
