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
/*!        \file  ara/core/internal/future_continuation.h
 *        \brief  Continuation class to register, store and execute the callback for future then.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_ARA_CORE_INTERNAL_FUTURE_CONTINUATION_H_
#define LIB_VAC_INCLUDE_ARA_CORE_INTERNAL_FUTURE_CONTINUATION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>
#include "ara/core/future.h"
#include "ara/core/promise.h"
#include "vac/language/unique_function.h"

namespace ara {
namespace core {
namespace internal {

/*!
 * \brief CallBackHolder interface.
 * \vprivate
 */
class CallBackHolderInterface {
 public:
  /*!
   * \brief Default constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  CallBackHolderInterface() noexcept = default;

  /*!
   * \brief Deleted copy constructor.
   */
  CallBackHolderInterface(CallBackHolderInterface const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  CallBackHolderInterface(CallBackHolderInterface&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  CallBackHolderInterface& operator=(CallBackHolderInterface const&) & = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  CallBackHolderInterface& operator=(CallBackHolderInterface&&) & = delete;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief Destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~CallBackHolderInterface() noexcept = default;

  /*!
   * \brief Executes the callback.
   * \spec
   *   requires true;
   * \endspec
   */
  virtual void ExecuteCallBack() noexcept = 0;
};

/*!
 * \brief  Callback holder base class to perform the callback execution.
 * \tparam T is type for the calling Future.
 * \tparam E is the error type for the calling Future.
 * \tparam U is the type returned back from the registered call back function.
 * \tparam T2 The unwrapped value type returned back from the registered call back function.
 * \tparam E2 The unwrapped error type returned back from the registered call back function.
 * \vprivate
 */
template <typename T, typename E, typename U, typename T2, typename E2>
class CallBackHolderBase : public CallBackHolderInterface {  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
 public:
  /*!
   * \brief Callback handler type.
   */
  using CallBackHandler = vac::language::UniqueFunction<U(ara::core::Future<T, E>)>;

  /*!
   * \brief     Constructor.
   * \param[in] new_promise New promise to be set when the callback is called (it needs to be set with the return value
   *            from the callback).
   * \param[in] calling_future The future on which the then function is called (calling future needs to be passed as a
   *            parameter to the call back function and it gets invalidated once then() is called).
   * \param[in] handler The handler to the callback function.
   * \pre   -
   * \spec
   *   requires true;
   * \endspec
   */
  CallBackHolderBase(ara::core::Promise<T2, E2>&& new_promise, ara::core::Future<T, E>&& calling_future,
                     CallBackHandler handler) noexcept
      : CallBackHolderInterface(),
        new_promise_(std::make_unique<ara::core::Promise<T2, E2>>(std::move(new_promise))),
        future_(std::move(calling_future)),
        callback_handler_(std::move(handler)) {}  // VCA_VAC_VALID_FUNC_CALL_RUN_TIME_CHECK

 protected:
  // VECTOR Disable VectorC++-V11.0.2: MD_VAC_V11-0-2_mutableMemberShallBePrivate
  /*!
   * \brief New promise to be set when the callback is called.
   */
  std::unique_ptr<ara::core::Promise<T2, E2>> new_promise_;

  /*!
   * \brief Future on which the function is called.
   */
  ara::core::Future<T, E> future_;

  /*! \brief Callback handler. */
  CallBackHandler callback_handler_;

  // VECTOR Enable VectorC++-V11.0.2
};

/*!
 * \brief  Callback holder class to perform the callback execution.
 * \tparam T The type for the calling Future.
 * \tparam E The error type for the calling Future.
 * \tparam U The type returned back from the registered call back function.
 * \tparam T2 The unwrapped value type returned back from the registered call back function.
 * \tparam E2 The unwrapped error type returned back from the registered call back function.
 * \vprivate
 */
template <typename T, typename E, typename U, typename T2, typename E2>
class CallBackHolder final : public CallBackHolderBase<T, E, U, T2, E2> {  // VCA_VAC_SAFE_DEALLOCATION
 public:
  using CallBackHolderBase<T, E, U, T2, E2>::CallBackHolderBase;

  /*!
   * \brief   Function to execute the registered call back.
   * \details Sets the new promise.
   * \pre     -
   * \spec
   *   requires true;
   * \endspec
   */
  void ExecuteCallBack() noexcept final {
    // VECTOR NL AutosarC++17_10-A12.8.3: MD_VAC_A12.8.3_dontReadAccessAMovedFromObject
    U const ret_val{this->callback_handler_(std::move(this->future_))};
    // VECTOR NL AutosarC++17_10-A12.8.3: MD_VAC_A12.8.3_dontReadAccessAMovedFromObject
    assert(this->new_promise_.get() != nullptr);  // COV_MSR_INV_STATE_ASSERT
    // VECTOR NL AutosarC++17_10-A12.8.3: MD_VAC_A12.8.3_dontReadAccessAMovedFromObject
    this->new_promise_->set_value(ret_val);
  }
};

/*!
 * \brief  Specialization callback holder class to perform the callback execution for when U is void.
 * \tparam T is type for the calling future.
 * \tparam E is the error type for the calling Future.
 * \tparam T2 The unwrapped value type returned back from the registered call back function.
 * \tparam E2 The unwrapped error type returned back from the registered call back function.
 * \vprivate
 */
template <typename T, typename E, typename T2, typename E2>
class CallBackHolder<T, E, void, T2, E2> final : public CallBackHolderBase<T, E, void, T2, E2> {
 public:
  using CallBackHolderBase<T, E, void, T2, E2>::CallBackHolderBase;

  /*!
   * \brief   Function to execute the registered call back.
   * \details Sets the new promise.
   * \pre     -
   * \spec
   *   requires true;
   * \endspec
   */
  void ExecuteCallBack() noexcept final {
    // VECTOR NL AutosarC++17_10-A12.8.3: MD_VAC_A12.8.3_dontReadAccessAMovedFromObject
    this->callback_handler_(std::move(this->future_));
    // VECTOR NL AutosarC++17_10-A12.8.3: MD_VAC_A12.8.3_dontReadAccessAMovedFromObject
    assert(this->new_promise_.get() != nullptr);  // COV_MSR_INV_STATE_ASSERT
    // VECTOR NL AutosarC++17_10-A12.8.3: MD_VAC_A12.8.3_dontReadAccessAMovedFromObject
    this->new_promise_->set_value();
  }
};

/*!
 * \brief  Specialization callback holder class to perform the callback execution for when U is Future<T2, E2>.
 * \tparam T is type for the calling future.
 * \tparam E is the error type for the calling Future.
 * \tparam T2 The unwrapped value type returned back from the registered call back function.
 * \tparam E2 The unwrapped error type returned back from the registered call back function.
 * \vprivate
 */
template <typename T, typename E, typename T2, typename E2>
class CallBackHolder<T, E, Future<T2, E2>, T2, E2> final : public CallBackHolderBase<T, E, Future<T2, E2>, T2, E2> {
 public:
  using CallBackHolderBase<T, E, Future<T2, E2>, T2, E2>::CallBackHolderBase;

  /*!
   * \brief   Function to execute the registered call back.
   * \details Sets the new promise.
   * \pre     -
   * \spec
   *   requires true;
   * \endspec
   */
  void ExecuteCallBack() noexcept final {
    // No need to set context on the Future here. If the calling Future has a custom context, then this overload of
    // ExecuteCallback() is called inside that context, and the Future returned from the callback will implicitly also
    // execute its then() in the same context.
    // This point is moot if the returned Future was already given a different context. This is not allowed, since the
    // users won't have access to SetExecutionContext() (product private).
    // VECTOR NL AutosarC++17_10-A12.8.3: MD_VAC_A12.8.3_dontReadAccessAMovedFromObject
    Future<T2, E2> chain_future{this->callback_handler_(std::move(this->future_))};
    if (chain_future.valid()) {
      if (chain_future.is_ready()) {
        // VECTOR NL AutosarC++17_10-A12.8.3: MD_VAC_A12.8.3_dontReadAccessAMovedFromObject
        assert(this->new_promise_.get() != nullptr);  // COV_MSR_INV_STATE_ASSERT
        // VECTOR NL AutosarC++17_10-A12.8.3: MD_VAC_A12.8.3_dontReadAccessAMovedFromObject
        SetValueOrError(*(this->new_promise_), chain_future.GetResult());
      } else {
        // VECTOR NL VectorC++-V50.1.1: MD_VAC_V50-1-1_futureThenShallNotBeUsed
        static_cast<void>(chain_future.then([this](Future<T2, E2> unwrapped_future) {
          Result<T2, E2> const res{unwrapped_future.GetResult()};  // VCA_VAC_VALID_NONSTATIC_METHOD
          assert(this->new_promise_.get() != nullptr);             // COV_MSR_INV_STATE_ASSERT
          SetValueOrError(*(this->new_promise_), res);
        }));
      }
    } else {
      // VECTOR NL AutosarC++17_10-A12.8.3: MD_VAC_A12.8.3_dontReadAccessAMovedFromObject
      assert(this->new_promise_.get() != nullptr);  // COV_MSR_INV_STATE_ASSERT
      // VECTOR NL AutosarC++17_10-A12.8.3: MD_VAC_A12.8.3_dontReadAccessAMovedFromObject
      SetValueOrError(*(this->new_promise_), Result<T2, E2>::FromError(future_errc::no_state));
    }
  }
};

/*!
 * \brief  Specialization callback holder class to perform the callback execution for when U is Result<T2, E2>.
 * \tparam T is type for the calling future.
 * \tparam E is the error type for the calling Future.
 * \tparam T2 The unwrapped value type returned back from the registered call back function.
 * \tparam E2 The unwrapped error type returned back from the registered call back function.
 * \vprivate
 */
template <typename T, typename E, typename T2, typename E2>
class CallBackHolder<T, E, Result<T2, E2>, T2, E2> final : public CallBackHolderBase<T, E, Result<T2, E2>, T2, E2> {
 public:
  using CallBackHolderBase<T, E, Result<T2, E2>, T2, E2>::CallBackHolderBase;

  /*!
   * \brief   Function to execute the registered call back.
   * \details Sets the new promise.
   * \pre     -
   * \spec
   *   requires true;
   * \endspec
   */
  void ExecuteCallBack() noexcept final {
    // VECTOR NL AutosarC++17_10-A12.8.3: MD_VAC_A12.8.3_dontReadAccessAMovedFromObject
    Result<T2, E2> const ret_val{this->callback_handler_(std::move(this->future_))};
    // VECTOR NL AutosarC++17_10-A12.8.3: MD_VAC_A12.8.3_dontReadAccessAMovedFromObject
    assert(this->new_promise_.get() != nullptr);  // COV_MSR_INV_STATE_ASSERT
    // VECTOR NL AutosarC++17_10-A12.8.3: MD_VAC_A12.8.3_dontReadAccessAMovedFromObject
    SetValueOrError(*(this->new_promise_), ret_val);
  }
};

/*!
 * \brief   Future continuation class accessible to the Future and Promise.
 * \details Promise creates object of FutureContinuation and passes it to the Future to be able to store and access the
 *          callback function.
 * \tparam  T The type for the calling Future.
 * \tparam  E The error type for the calling Future.
 * \vprivate
 */
template <typename T, typename E = ErrorCode>
class FutureContinuation final {
 public:
  // VECTOR Disable AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief Default constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  FutureContinuation() noexcept = default;

  /*!
   * \brief Default move constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  FutureContinuation(FutureContinuation&&) noexcept = default;

  /*!
   * \brief  Default move assignment operator.
   * \return A reference to the assigned-to object.
   * \spec
   *   requires true;
   * \endspec
   */
  FutureContinuation& operator=(FutureContinuation&&) & noexcept = default;

  /*!
   * \brief Default destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  ~FutureContinuation() noexcept = default;  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT

  // VECTOR Enable AutosarC++17_10-A15.5.1
  /*!
   * \brief Deleted copy constructor deleted.
   */
  FutureContinuation(FutureContinuation const&) = delete;

  /*!
   * \brief  Deleted copy assignment operator deleted.
   * \return
   */
  FutureContinuation& operator=(FutureContinuation const&) = delete;

  /*!
   * \brief     Registers a callback handler to be called when the Promise is set and the state is ready.
   * \tparam    Func The type for the callback function.
   * \param[in] handler A callback handler.
   * \param[in] fut The calling Future.
   * \param[in] is_ready A bool to indicate that the calling Future fut is ready and the callback handler can be
                instantly executed.
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \return    new future.
   */
  template <typename Func>
  auto SetCallBackHandler(Func&& handler, ara::core::Future<T, E>&& fut, bool const is_ready = false) noexcept ->
      typename TypeUnwrapping<decltype(handler(std::move(fut))), E>::type {
    /*! \brief Alias for the return type of the callable */
    using U = decltype(handler(std::move(fut)));
    /*! \brief Alias for the unwrapped return value type of the callable */
    using T2 = typename TypeUnwrapping<U, E>::value_type;
    /*! \brief Alias for the unwrapped return error type of the callable */
    using E2 = typename TypeUnwrapping<U, E>::error_type;

    ara::core::Promise<T2, E2> new_promise{};
    ara::core::Future<T2, E2> new_future{new_promise.get_future()};
    static_cast<void>(new_future.SetExecutionContext(ec_));  // Propagate the execution context to the next Future.
    // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    call_back_ = std::make_unique<CallBackHolder<T, E, U, T2, E2>>(std::move(new_promise), std::move(fut),
                                                                   std::forward<Func>(handler));
    if (is_ready) {
      ExecuteCallBack();
    }
    return new_future;
  }

  /*!
   * \brief  Set the execution context to the callable.
   * \tparam ExecT The executor type to determine where to execute the callable. Must inherit from ExecutorInterface.
   *         Must overload the Execute(...) function.
   * \param  ec A shared pointer to the executor.
   * \return False if a context was already set, true if a new and valid context was set.
   */
  template <typename ExecT, std::enable_if_t<std::is_base_of<ExecutorInterface, ExecT>::value, std::int8_t> = 0>
  bool SetContext(std::shared_ptr<ExecT> ec) noexcept {
    bool set_context_result{false};
    if (!ec_) {
      ec_ = ec;  // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
      set_context_result = static_cast<bool>(ec_);
    }
    return set_context_result;
  }

  // VECTOR Next Construct AutosarC++17_10-M9.3.1: MD_VAC_M9.3.1_ConstMemberFunctionsShallNotReturnNonConstPointers
  /*!
   * \brief  Get the context.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return The call back mutex.
   */
  std::shared_ptr<ExecutorInterface> GetContext() const noexcept { return ec_; }

  /*!
   * \brief Executes the callback.
   * \pre   -
   * \spec
   *   requires true;
   * \endspec
   */
  void ExecuteCallBack() noexcept {
    if (IsCallBackSet()) {
      // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
      call_back_ready_ = false;  // VCA_VAC_VALID_NONSTATIC_METHOD
      if (ec_) {
        // VECTOR Next Construct VectorC++-V5.0.1: MD_VAC_V5.0.1_UnsequencedFunctionCalls
        // VECTOR Next Construct AutosarC++17_10-A5.0.1: MD_VAC_A5.0.1_functionOrderIsNotRelevant
        // VECTOR Next Construct AutosarC++17_10-M6.2.1: MD_VAC_M6.2.1_AssignmentOperatorsShallNotBeUsedInSubExpressions
        // VECTOR Next Construct AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConst
        // VCA_VAC_VALID_FUNC_CALL_RUN_TIME_CHECK
        ec_->SubmitWork([call_back = std::move(call_back_)]() mutable noexcept { call_back->ExecuteCallBack(); });
      } else {
        assert(this->call_back_ != nullptr);  // COV_MSR_INV_STATE_ASSERT
        call_back_->ExecuteCallBack();        // VCA_VAC_VALID_FUNC_CALL_RUN_TIME_CHECK
      }
    }
  }

  /*!
   * \brief  Get the call_back_mutex.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return The call back mutex.
   */
  std::mutex& GetCallBackMutex() noexcept { return call_back_mutex_; }

 private:
  /*!
   * \brief  Returns the registered state of the call back.
   * \pre    -
   * \spec
   *   requires true;
   * \endspec
   * \return True if the callback is registered, false otherwise.
   */
  bool IsCallBackSet() const noexcept {  // Comment to force line-break. See TAR-22835.
    return (call_back_ != nullptr) && call_back_ready_;
  }

  /*! \brief A mutex to prevent setting a value while a call back is being set and vice versa. */
  std::mutex call_back_mutex_{};

  /*! \brief Shared pointer to the ExecutorInterface, set by SetContext(). */
  std::shared_ptr<ExecutorInterface> ec_{nullptr};

  /*! \brief Pointer to CallBackHolderInterface. */
  std::unique_ptr<CallBackHolderInterface> call_back_{nullptr};

  /*! \brief Boolean to keep track of if the call back has been executed or not. */
  std::atomic<bool> call_back_ready_{true};
};

}  // namespace internal
}  // namespace core
}  // namespace ara

#endif  // LIB_VAC_INCLUDE_ARA_CORE_INTERNAL_FUTURE_CONTINUATION_H_
