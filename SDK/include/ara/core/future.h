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
/**        \file
 *        \brief  Specific implementation of a Future for ara::core.
 *         \unit  VaCommonLib::LanguageSupport::AsynchronousOperations::Future
 *
 *      \details  ara::core::Future is an asynchronous return object similar to std::future but implemented for use with
 *                ara::core::Result.
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_ARA_CORE_FUTURE_H_
#define LIB_VAC_INCLUDE_ARA_CORE_FUTURE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <memory>
#include <utility>
#include "amsr/shared_ptr.h"
#include "amsr/shared_ptr_resource.h"
#include "ara/core/error_code.h"
#include "ara/core/future_error_domain.h"
#include "ara/core/internal/shared_state.h"
#include "ara/core/result.h"
#include "vac/language/compile_exceptions.h"
#include "vac/language/detail/callable_check.h"
#include "vac/language/unique_function.h"

namespace ara {
namespace core {

// Forward declaration.
template <typename T, typename E>
class Future;

// Forward declaration.
template <typename T, typename E>
class Promise;

// Forward declaration.
template <typename E>
class Promise<void, E>;

namespace internal {

// Forward declaration of FutureContinuation to avoid circular dependency.
template <typename T, typename E>
class FutureContinuation;

/*!
 * \brief      Sets a value or error to a Promise p given a Result res.
 * \tparam     T The value type of the Promise and Result.
 * \tparam     E The error type of the Promise and Result.
 * \param[out] p The Promise to set a value or error to.
 * \param[in]  res The Result to get the value or error from.
 * \pre        -
 * \vprivate
 * \spec
 *   requires true;
 * \endspec
 */
template <typename T, typename E>
void SetValueOrError(Promise<T, E>& p, Result<T, E> res) noexcept {
  if (res.HasValue()) {
    p.set_value(res.Value());  // VCA_VAC_VALID_ARGUMENT
  } else {
    p.SetError(res.Error());  // VCA_VAC_VALID_ARGUMENT
  }
}

/*!
 * \brief      Sets a value or error to a Promise p given a Result res when T is void.
 * \tparam     E The error type of the Promise and Result.
 * \param[out] p The Promise to set a value or error to.
 * \param[in]  res The Result to get the value or error from.
 * \pre        -
 * \vprivate
 * \spec
 *   requires true;
 * \endspec
 */
template <typename E>
void SetValueOrError(Promise<void, E>& p, Result<void, E> res) noexcept {
  if (res.HasValue()) {
    p.set_value();
  } else {
    p.SetError(res.Error());
  }
}

/*!
 * \brief  Helper class to get a valid Future based on a given callable.
 * \remark Given the following specializations, T2 should be equal to U if this specialization is used.
 * \tparam T1 The type of the calling Future.
 * \tparam E1 The error type of the calling Future.
 * \tparam U  The callable's return type.
 * \tparam T2 The unwrapped type of U.
 * \tparam E2 The unwrapped error type of U.
 * \vprivate
 */
template <typename T1, typename E1, typename U, typename T2, typename E2>
class GetFuture {
 public:
  /*!
   * \brief     Set the callable's return value to a Promise and returns a connected Future.
   * \tparam    F  The callable's type.
   * \param[in] callable The callable, taking rdy_future as input parameter.
   * \param[in] rdy_future A ready Future.
   * \pre       -
   * \return    a new Future.
   * \vprivate
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename F>
  auto operator()(F&& callable, Future<T1, E1>&& rdy_future) noexcept -> Future<T2, E2> {
    Promise<T2, E2> promise_new;
    U const func_value{callable(std::move(rdy_future))};  // VCA_VAC_FULFILL_SPEC
    promise_new.set_value(func_value);
    return promise_new.get_future();
  }
};

/*!
 * \brief  Helper class to get a valid Future based on a given callable which returns void.
 * \tparam T1 The type of the calling Future.
 * \tparam E1 The error type of the calling Future.
 * \tparam E2 The unwrapped error type of U.
 * \vprivate
 */
template <typename T1, typename E1, typename E2>
class GetFuture<T1, E1, void, void, E2> {
 public:
  // VECTOR Next Construct AutosarC++17_10-M7.1.2: MD_VAC_M7.1.2_parameterAsReferencePointerToConst
  /*!
   * \brief     Set the callable's return value to a Promise and returns a connected Future.
   * \tparam    F  The callable's type.
   * \param[in] callable The callable, taking rdy_future as input parameter.
   * \param[in] rdy_future A ready Future.
   * \pre       -
   * \return    a new Future.
   * \vprivate
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename F>
  auto operator()(F&& callable, Future<T1, E1>&& rdy_future) noexcept -> Future<void, E2> {
    Promise<void, E2> promise_new;
    callable(std::move(rdy_future));  // VCA_VAC_FULFILL_SPEC
    promise_new.set_value();
    return promise_new.get_future();
  }
};

// VECTOR Next Construct AutosarC++17_10-M7.1.2: MD_VAC_M7.1.2_parameterAsReferencePointerToConst
/*!
 * \brief  Helper class to get a valid Future based on a given callable which returns Result<T2, E2>.
 * \tparam T1 The type of the calling Future.
 * \tparam E1 The error type of the calling Future.
 * \tparam T2 The unwrapped type of U.
 * \tparam E2 The unwrapped error type of U.
 * \vprivate
 */
template <typename T1, typename E1, typename T2, typename E2>
class GetFuture<T1, E1, Result<T2, E2>, T2, E2> {
 public:
  /*!
   * \brief     Unwrapps the callable's returned Result, sets its contents to a Promise, and returns a connected Future.
   * \tparam    F  The callable's type.
   * \param[in] callable The callable, taking rdy_future as input parameter.
   * \param[in] rdy_future A ready Future.
   * \pre       -
   * \return    a new Future.
   * \vprivate
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename F>
  auto operator()(F&& callable, Future<T1, E1>&& rdy_future) noexcept -> Future<T2, E2> {
    Promise<T2, E2> promise_new;
    SetValueOrError(promise_new, callable(std::move(rdy_future)));  // VCA_VAC_VALID_FUNC_CALL_RUN_TIME_CHECK
    return promise_new.get_future();
  }
};

// VECTOR Next Construct AutosarC++17_10-M7.1.2: MD_VAC_M7.1.2_parameterAsReferencePointerToConst
/*!
 * \brief  Helper class to get a valid Future based on a given callable which returns Future<T2, E2>.
 * \tparam T1 The type of the calling Future.
 * \tparam E1 The error type of the calling Future.
 * \tparam T2 The unwrapped type of U.
 * \tparam E2 The unwrapped error type of U.
 * \vprivate
 */
template <typename T1, typename E1, typename T2, typename E2>
class GetFuture<T1, E1, Future<T2, E2>, T2, E2> {
 public:
  /*!
   * \brief     Returns the callable's returned Future.
   * \tparam    F  The callable's type.
   * \param[in] callable The callable, taking rdy_future as input parameter.
   * \param[in] rdy_future A ready Future.
   * \pre       -
   * \return    a new Future.
   * \vprivate
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename F>
  auto operator()(F&& callable, Future<T1, E1>&& rdy_future) noexcept -> Future<T2, E2> {
    return callable(std::move(rdy_future));  // VCA_VAC_FULFILL_SPEC
  }
};

/*!
 * \brief  Helper class to unwrap the return type of the callable sent to Future::then().
 * \tparam U The callable's return type.
 * \tparam E The calling Future's error type.
 * \vprivate
 */
template <typename U, typename E>
class TypeUnwrapping {
 public:
  /*! \brief Type alias. */
  using type = Future<U, E>;

  /*! \brief Type alias. */
  using value_type = U;

  /*! \brief Type alias. */
  using error_type = E;
};

/*!
 * \brief  Helper class to do implicit Result unwrapping.
 * \tparam E The calling Future's error type.
 * \vprivate
 */
template <typename T2, typename E2, typename E>
class TypeUnwrapping<Result<T2, E2>, E> {
 public:
  /*! \brief Type alias. */
  using type = Future<T2, E2>;

  /*! \brief Type alias. */
  using value_type = T2;

  /*! \brief Type alias. */
  using error_type = E2;
};

/*!
 * \brief  Helper class to do implicit Future unwrapping.
 * \tparam E The calling Future's error type.
 * \vprivate
 */
template <typename T2, typename E2, typename E>
class TypeUnwrapping<Future<T2, E2>, E> {
 public:
  /*! \brief Type alias. */
  using type = Future<T2, E2>;

  /*! \brief Type alias. */
  using value_type = T2;

  /*! \brief Type alias. */
  using error_type = E2;
};

/*!
 * \brief Interface class of the Executor to be provided to Future::SetExecutionContext().
 */
class ExecutorInterface {
 public:
  /*! \brief Default constructor. */
  ExecutorInterface() noexcept = default;

  /*! \brief Default copy constructor. */
  ExecutorInterface(ExecutorInterface const&) noexcept = delete;

  /*!
   * \brief Default move constructor.
   * \param[in] other The other instance.
   */
  ExecutorInterface(ExecutorInterface&& other) noexcept = delete;

  /*! \brief Default copy assignment. */
  ExecutorInterface& operator=(ExecutorInterface const&) & noexcept = delete;

  /*!
   * \brief Default move assignment.
   * \param[in] other The other instance.
   * \return Reference to ExecutorInterface object.
   */
  ExecutorInterface& operator=(ExecutorInterface&& other) & noexcept = delete;

  /*! \brief Virtual default destructor. */
  virtual ~ExecutorInterface() noexcept = default;

  /*! \brief Pure virtual function used to submit work to a defined context. */
  virtual void SubmitWork(vac::language::UniqueFunction<void()>&&) noexcept = 0;
};

}  // namespace internal

/*!
 * \brief Operation status for the timed wait functions wait_for() and wait_until().
 * \trace SPEC-7552463
 * \vpublic
 */
enum class future_status : uint8_t {
  /*!
   * \brief The shared state is ready.
   */
  ready = 0,
  /*!
   * \brief The shared state did not become ready before the specified timeout has passed.
   */
  timeout = 1
};

/*!
 * \brief  Provides ara::core specific Future operations to collect the results of an asynchronous call.
 * \details For more information about T and E, please refer to the Result class and its specializations.
 * \tparam T Value type. Can be any type.
 * \tparam E Error type. Defaults to ara::core::ErrorCode, but can be any type.
 * \trace  SPEC-7552464, DSGN-VaCommonLib-AsynchronousOperations
 * \vpublic
 */
template <typename T, typename E = ErrorCode>
class Future final {
  /*!
   * \brief Alias for Result.
   */
  using R = Result<T, E>;

  /*! \brief Alias for SharedState. */
  using SharedStateT = internal::SharedState<R, future_errc>;

  /*! \brief Shared pointer type for SharedState. */
  using SharedStatePtr = amsr::SharedPtr<SharedStateT>;

  /*! \brief Alias for FutureContinuation. */
  using FutureContinuationT = ara::core::internal::FutureContinuation<T, E>;
  /*! \brief The future continuation pointer type. */
  using FutureContinuationPtr = amsr::SharedPtr<FutureContinuationT>;

 public:
  /*!
   * \brief Default constructor.
   * \spec
   *   requires true;
   * \endspec
   * \trace SPEC-7552465
   * \vpublic
   */
  Future() noexcept = default;

  /*!
   * \brief Default copy constructor deleted.
   * \trace SPEC-7552466
   * \vpublic
   */
  Future(Future const&) = delete;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief     Move constructor.
   * \param[in] other The other instance.
   * \spec
   *   requires true;
   * \endspec
   * \trace SPEC-7552467
   * \vpublic
   */
  Future(Future&& other) noexcept = default;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief Destructor.
   * \spec
   *   requires true;
   * \endspec
   * \trace SPEC-7552468
   * \vpublic
   */
  ~Future() noexcept {
    // If this Future was created from Promise::get_future(CleanUpCallable&&), it will call the provided callable iff:
    // the Future is valid, the callable is valid (e.g. not nullptr), and the Future is not ready.
    //
    // The clean-up feature is not supported if Future::then() has been called. This is perfect, because a Future in
    // which then() is called, will always be either ready or not valid when it is destroyed. Since it is either ready
    // or not valid, it will never get past this if-statement.
    // A Future returned from a then() call will never have a cleanup callable set, unless the callable given to then()
    // directly returns a Future with a callable set.
    //
    // If a race condition occurs and sets the state to ready after the check, we will not be able to get the value
    // anyway since the Future is in the process of destructing itself. The initial use-case of this functionality is to
    // destruct the Promise if the state is not ready, which will set the state to ready with the error broken_promise,
    // so this "ready after check" situation is accepted.
    if (valid() && static_cast<bool>(cleanup_)) {
      if (!is_ready()) {
        cleanup_();
      }
    }
  }

  /*!
   * \brief  Default copy assignment operator deleted.
   * \return
   * \trace  SPEC-7552469
   * \vpublic
   */
  Future& operator=(Future const&) & = delete;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief     Move assignment operator.
   * \param[in] other The other instance.
   * \return    A reference to "*this".
   * \spec
   *   requires true;
   * \endspec
   * \trace     SPEC-7552470
   * \vpublic
   */
  Future& operator=(Future&& other) & noexcept = default;  // VCA_VAC_VALID_MEMBER_VARIABLE_ACCESS

  /*!
   * \brief  Get the result (does not throw exceptions).
   * \pre    -
   * \return The value stored in the shared state or an error code.
   * \error  future_errc::*, Any error that fulfill conditions in ara::core::future_errc.
   * \error  Any error set in connected ara::core::Promise.
   * \spec
   *   requires true;
   * \endspec
   * \trace  SPEC-7552472
   * \vpublic
   */
  R GetResult() const noexcept {
    R ret_val{R::FromError(future_errc::no_state)};
    if (valid()) {
      ret_val = shared_state_->GetData();  // VCA_VAC_VALID_MEMBER_VARIABLE_ACCESS
    }
    return std::move(ret_val);
  }

  /*!
   * \brief  Get the value.
   * \details This method does participate in overload resolution only if compiling with exceptions enabled. If
   *          exceptions are disabled, GetResult() can be used instead.
   * \pre    -
   * \return The value stored in the shared state.
   * \throws ara::core::FutureException Internal Future/Promise errors see FutureErrc.
   * \throws ara::core::Exception Any error which is set in SetError. The concrete exception type depends on the
   *         Exception type corresponding to the error domain of the ErrorCode.
   * \spec
   *   requires true;
   * \endspec
   * \trace  SPEC-7552471
   * \vpublic
   */
  template <bool enabled = vac::language::internal::kCompileWithExceptions, typename = std::enable_if_t<enabled>>
  T get() const noexcept(false) {  // COV_LIBVAC_COMPILE_WITH_NO_EXCEPTIONS
    static_assert((!enabled) || vac::language::internal::kCompileWithExceptions,
                  "Should not be enabled when exceptions are disabled.");
    R res{GetResult()};
    if (!res.HasValue()) {
      res.Error().ThrowAsException();
    }
    return std::move(res).Value();
  }

  // VECTOR Next Construct AutosarC++17_10-M5.14.1: MD_VAC_M5.14.1_operandHasNoSideEffect
  /*!
   * \brief  Check if the Future has any shared state.
   * \pre    -
   * \return True if *this refers to a shared state, otherwise false.
   * \spec
   *   requires true;
   * \endspec
   * \trace  SPEC-7552473
   * \vpublic
   */
  bool valid() const noexcept {
    return shared_state_ && shared_state_->Valid();  // VCA_VAC_VALID_MEMBER_VARIABLE_ACCESS
  }

  /*!
   * \brief   Block until the shared state is ready.
   * \details Returns directly if shared state is invalid.
   * \pre     -
   * \spec
   *   requires true;
   * \endspec
   * \trace   SPEC-7552474
   * \vpublic
   */
  void wait() const noexcept {
    if (valid()) {
      shared_state_->Wait();  // VCA_VAC_VALID_MEMBER_VARIABLE_ACCESS
    }
  }

  // VECTOR Next Construct AutosarC++17_10-M5.14.1: MD_VAC_M5.14.1_operandHasNoSideEffect
  /*!
   * \brief   Wait for a specified relative time.
   * \details Returns future_status::timeout directly if shared state is invalid.
   * \tparam  Rep An arithmetic type representing the number of ticks.
   * \tparam  Period Type representing the tick period.
   * \param   timeout_duration Maximum duration to wait for.
   * \pre     -
   * \return  Status that indicates whether the timeout hit or if a value is available.
   * \spec
   *   requires true;
   * \endspec
   * \trace   SPEC-7552475
   * \vpublic
   */
  template <typename Rep, typename Period>
  future_status wait_for(std::chrono::duration<Rep, Period> const& timeout_duration) const noexcept(false) {
    return (valid() && shared_state_->WaitFor(timeout_duration))  // VCA_VAC_VALID_MEMBER_VARIABLE_ACCESS
               ? future_status::ready
               : future_status::timeout;
  }

  // VECTOR Next Construct AutosarC++17_10-M5.14.1: MD_VAC_M5.14.1_operandHasNoSideEffect
  /*!
   * \brief   Wait until a specified absolute time.
   * \details Returns future_status::timeout directly if shared state is invalid.
   * \tparam  Clock The clock on which this time point is measured.
   * \tparam  Duration A std::chrono::duration type used to measure the time since epoch.
   * \param   abs_time maximum time point to block until.
   * \pre     -
   * \return  Status that indicates whether the time was reached or if a value is available.
   * \spec
   *   requires true;
   * \endspec
   * \trace   SPEC-7552476
   * \vpublic
   */
  template <typename Clock, typename Duration>
  future_status wait_until(std::chrono::time_point<Clock, Duration> const& abs_time) const noexcept(false) {
    return (valid() && shared_state_->WaitUntil(abs_time))  // VCA_VAC_VALID_MEMBER_VARIABLE_ACCESS
               ? future_status::ready
               : future_status::timeout;
  }

  // VECTOR Next Construct AutosarC++17_10-A18.5.8: MD_VAC_A18.5.8_localObjectsShallBeAllocatedOnStack
  /*!
   * \brief     Set a continuation for when the shared state is ready.
   * \details   When callable is called, it is guaranteed that get() and GetResult() will not block.
   *            Returns an invalid Future if shared state is invalid or a nullptr callable is provided.
   *            *this will be invalid after returning from this function. If a nullptr callable is provided,
   *            then *this will remain unchanged.
   * \remark    callable may be called in the context of this call or in the context of Promise::set_value() or
   *            Promise::SetError() or somewhere else.
   *            Warning: This function might use dynamic memory allocation. Use with caution!
   * \tparam    Type of continuation function.
   * \param[in] callable A continuation function to be attached, must have the signature XX callable(Future<T, E>).
   * \pre       -
   * \return    A new Future instance for the result of the continuation.
   * \spec
   *   requires true;
   * \endspec
   * \trace     SPEC-7552477
   * \vpublic
   */
  template <typename F>
  auto then(F&& callable) noexcept -> typename internal::TypeUnwrapping<decltype(callable(std::move(*this))), E>::type {
    /*! \brief Alias for the type of the value contained in the Future returned from this function. */
    using U = decltype(callable(std::move(*this)));

    /*! \brief Alias for the type of the value contained in the Future returned from this function. */
    using T2 = typename internal::TypeUnwrapping<U, E>::value_type;

    /*! \brief Alias for the type of the value contained in the Future returned from this function. */
    using E2 = typename internal::TypeUnwrapping<U, E>::error_type;

    Future<T2, E2> future;

    bool const is_null_ptr{vac::language::detail::IsCallableNullPtr(callable)};
    // Returns an invalid Future if *this is not valid and/or callable is nullptr.
    if ((ValidThen()) && (!is_null_ptr)) {
      // VECTOR NL AutosarC++17_10-A7.1.1: MD_VAC_A7.1.1_immutableVariableShallBeConstOrConstexprQualified
      FutureContinuationPtr fut_cont{future_continuation_};
      // Since the fut_cont pointer can only ever be nullptr if shared_state_ is nullptr,
      // checking shared_state_ in valid() is enough to place the assert here (to please VCA).
      assert(fut_cont.Get() != nullptr);  // COV_MSR_INV_STATE_ASSERT
      std::lock_guard<std::mutex> const lock{fut_cont->GetCallBackMutex()};
      if (is_ready()) {
        // Call the function if Future state is ready.
        // checking shared_state_ in valid() is enough to place the assert here (to please VCA).
        assert(fut_cont.Get() != nullptr);  // COV_MSR_INV_STATE_ASSERT
        if (fut_cont->GetContext()) {
          assert(fut_cont.Get() != nullptr);  // COV_MSR_INV_STATE_ASSERT
          future = fut_cont->SetCallBackHandler(std::forward<F>(callable), std::move(*this), true);
        } else {
          future = internal::GetFuture<T, E, U, T2, E2>()(std::forward<F>(callable), std::move(*this));
        }
      } else {
        // Move the future_continuation pointer to f_continuation to be used after this Future object gets invalidated.
        // VECTOR NL AutosarC++17_10-A7.1.1: MD_VAC_A7.1.1_immutableVariableShallBeConstOrConstexprQualified
        FutureContinuationPtr f_continuation{std::move(future_continuation_)};
        // Since the fut_cont pointer can only ever be nullptr if shared_state_ is nullptr,
        // checking shared_state_ in valid() is enough to place the assert here (to please VCA).
        assert(f_continuation.Get() != nullptr);  // COV_MSR_INV_STATE_ASSERT
        // Save the call back function in continuation to be called later when Promise is set.
        future = f_continuation->SetCallBackHandler(std::forward<F>(callable), std::move(*this));
      }  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    }    // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    return future;
  }

  /*!
   * \brief   Set the context in which the callable give to Future::then() will execute.
   * \details If this function will be called, it must be called before calling Future::then(). If called after, then:
   *            1. the given context will not propagate to the Futures returned from then(),
   *            2. and it is not guaranteed that the callable will be executed in the given context.
   *          Will only set the context if:
   *            1. the Future is valid,
   *            2. a context has not previously been set,
   *            3. and the shared_ptr points to an object, i.e. "ec.get() != nullptr".
   * \tparam  ExecT The context type. Must inherit from internal::ExecutorInterface.
   * \param   ec A shared_ptr<ExecT> pointing to the object that will decide the context.
   * \return  True if a context was correctly set, false otherwise.
   * \threadsafe FALSE
   * \vprivate Product Private
   */
  template <typename ExecT,
            std::enable_if_t<std::is_base_of<internal::ExecutorInterface, ExecT>::value, std::int8_t> = 0>
  bool SetExecutionContext(std::shared_ptr<ExecT> ec) noexcept {
    // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
    return ValidThen() ? future_continuation_->SetContext(ec) : false;
  }

  /*!
   * \brief  Return true only when the shared state is ready. This method will return immediately and shall not do a
   *         blocking wait.
   * \pre    -
   * \return True if the future contains a value (or exception), false if not.
   * \spec
   *   requires true;
   * \endspec
   * \trace  SPEC-7552478
   * \vpublic
   */
  bool is_ready() const noexcept {
    // VECTOR NL AutosarC++17_10-M5.14.1: MD_VAC_M5.14.1_operandHasNoSideEffect
    return shared_state_ && shared_state_->Ready();  // VCA_VAC_VALID_MEMBER_VARIABLE_ACCESS
  }

 private:
  /*! \brief Checks if Future is valid and future_continuation_ is not a nullptr. */
  bool ValidThen() const noexcept { return valid() && future_continuation_; }

  /*! \brief Type used for cleanup callables. */
  using CleanUpCallable = typename vac::language::UniqueFunction<void()>;

  /*!
   * \brief     Parameterized constructor.
   * \param[in] shared_state object pointer to connect with Promise.
   * \param[in] future_continuation Future continuation pointer to manage the call back function.
   * \param[in] cleanup A callable to be called in ~Future(), iff state is valid and not ready.
   * \vprivate
   * \spec
   *   requires true;
   * \endspec
   */
  Future(SharedStatePtr const& shared_state, FutureContinuationPtr const& future_continuation,
         CleanUpCallable&& cleanup = CleanUpCallable{nullptr}) noexcept
      : shared_state_(shared_state), future_continuation_(future_continuation), cleanup_(std::move(cleanup)) {}

  /*! \brief The shared state between *this and the connected Promise. */
  SharedStatePtr shared_state_{nullptr};

  /*! \brief Future continuation pointer to manage the call back function. */
  FutureContinuationPtr future_continuation_{nullptr};

  /*! \brief UniqueFunction to hold an executable. */
  CleanUpCallable cleanup_{nullptr};

  /*!
   * \brief  Friend to only provide the parameterized ctor for use in Promise::get_future().
   * \tparam T The value type of the Promise and Result.
   * \tparam E The error type of the Promise and Result.
   */
  friend class Promise<T, E>;  // VECTOR SL AutosarC++17_10-A11.3.1: MD_VAC_A11.3.1_doNotUseFriend
};

/*!
 * \brief  void specialized Future
 * \tparam E Error type
 * \trace  DSGN-VaCommonLib-AsynchronousOperations
 * \vpublic
 */
template <typename E>
class Future<void, E> final {
  /*!
   * \brief Alias for result
   */
  using R = Result<void, E>;

  /*! \brief Alias for SharedState. */
  using SharedStateT = internal::SharedState<R, future_errc>;

  /*! \brief Shared pointer type for SharedState. */
  using SharedStatePtr = amsr::SharedPtr<SharedStateT>;

  /*! \brief Alias for FutureContinuation. */
  using FutureContinuationT = ara::core::internal::FutureContinuation<void, E>;
  /*! \brief The future continuation pointer type. */
  using FutureContinuationPtr = amsr::SharedPtr<FutureContinuationT>;

 public:
  /*!
   * \brief Default constructor
   * \spec
   *   requires true;
   * \endspec
   * \trace SPEC-7552465
   * \vpublic
   */
  Future() noexcept = default;

  /*!
   * \brief Default copy constructor deleted
   * \trace SPEC-7552466
   * \vpublic
   */
  Future(Future const&) = delete;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief     Move constructor.
   * \param[in] other The other instance.
   * \spec
   *   requires true;
   * \endspec
   * \trace     SPEC-7552467
   * \vpublic
   */
  Future(Future&& other) noexcept = default;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief Destructor.
   * \spec
   *   requires true;
   * \endspec
   * \trace SPEC-7552468
   * \vpublic
   */
  ~Future() noexcept {
    // If this Future was created from Promise::get_future(CleanUpCallable&&), it will call the provided callable iff:
    // the Future is valid, the callable is valid (e.g. not nullptr), and the Future is not ready.
    //
    // The clean-up feature is not supported if Future::then() has been called. This is perfect, because a Future in
    // which then() is called, will always be either ready or not valid when it is destroyed. Since it is either ready
    // or not valid, it will never get past this if-statement.
    // A Future returned from a then() call will never have a cleanup callable set, unless the callable given to then()
    // directly returns a Future with a callable set.
    //
    // If a race condition occurs and sets the state to ready after the check, we will not be able to get the value
    // anyway since the Future is in the process of destructing itself. The initial use-case of this functionality is to
    // destruct the Promise if the state is not ready, which will set the state to ready with the error broken_promise,
    // so this "ready after check" situation is accepted.
    if (valid() && static_cast<bool>(cleanup_)) {
      if (!is_ready()) {
        cleanup_();
      }
    }
  }

  /*!
   * \brief  Default copy assignment operator deleted.
   * \return
   * \trace  SPEC-7552469
   * \vpublic
   */
  Future& operator=(Future const&) & = delete;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief     Move assignment operator.
   * \param[in] other The other instance.
   * \return    A reference to "*this".
   * \spec
   *   requires true;
   * \endspec
   * \trace  SPEC-7552470
   * \vpublic
   */
  Future& operator=(Future&& other) & noexcept = default;  // VCA_VAC_VALID_MEMBER_VARIABLE_ACCESS

  /*!
   * \brief  Get the result (does not throw exceptions).
   * \pre    -
   * \return The value stored in the shared state or an error code.
   * \error  future_errc::*, Any error that fulfill conditions in ara::core::future_errc.
   * \error  Any error set in connected ara::core::Promise.
   * \spec
   *   requires true;
   * \endspec
   * \trace  SPEC-7552472
   * \vpublic
   */
  R GetResult() const noexcept {
    R ret_val{R::FromError(future_errc::no_state)};
    if (valid()) {
      ret_val = shared_state_->GetData();  // VCA_VAC_VALID_MEMBER_VARIABLE_ACCESS
    }
    return std::move(ret_val);
  }

  /*!
   * \brief  Get the value.
   * \details This method does participate in overload resolution only if compiling with exceptions enabled. If
   *          exceptions are disabled, GetResult() can be used instead.
   * \pre    -
   * \return The value stored in the shared state.
   * \throws ara::core::FutureException Internal Future/Promise errors see FutureErrc.
   * \throws ara::core::Exception Any error which is set in SetError. The concrete exception type depends on the
   *         Exception type corresponding to the error domain of the ErrorCode.
   * \spec
   *   requires true;
   * \endspec
   * \trace  SPEC-7552471
   * \vpublic
   */
  template <bool enabled = vac::language::internal::kCompileWithExceptions, typename = std::enable_if_t<enabled>>
  void get() const noexcept(false) {  // COV_LIBVAC_COMPILE_WITH_NO_EXCEPTIONS
    static_assert((!enabled) || vac::language::internal::kCompileWithExceptions,
                  "Should not be enabled when exceptions are disabled.");
    R res{GetResult()};
    if (!res.HasValue()) {
      res.Error().ThrowAsException();
    }
  }

  // VECTOR Next Construct AutosarC++17_10-M5.14.1: MD_VAC_M5.14.1_operandHasNoSideEffect
  /*!
   * \brief  Check if the Future has any shared state.
   * \pre    -
   * \return True if *this refers to a shared state, otherwise false.
   * \spec
   *   requires true;
   * \endspec
   * \trace  SPEC-7552473
   * \vpublic
   */
  bool valid() const noexcept {
    return shared_state_ && shared_state_->Valid();  // VCA_VAC_VALID_MEMBER_VARIABLE_ACCESS
  }

  /*!
   * \brief   Block until the shared state is ready.
   * \details Returns directly if shared state is invalid.
   * \pre     -
   * \spec
   *   requires true;
   * \endspec
   * \trace   SPEC-7552474
   * \vpublic
   */
  void wait() const noexcept {
    if (valid()) {
      shared_state_->Wait();  // VCA_VAC_VALID_MEMBER_VARIABLE_ACCESS
    }
  }

  // VECTOR Next Construct AutosarC++17_10-M5.14.1: MD_VAC_M5.14.1_operandHasNoSideEffect
  /*!
   * \brief   Wait for a specified relative time.
   * \details Returns future_status::timeout directly if shared state is invalid.
   * \tparam  Rep An arithmetic type representing the number of ticks.
   * \tparam  Period Type representing the tick period.
   * \param   timeout_duration Maximum duration to wait for.
   * \pre     -
   * \spec
   *   requires true;
   * \endspec
   * \return  Status that indicates whether the timeout hit or if a value is available.
   * \trace   SPEC-7552475
   * \vpublic
   */
  template <typename Rep, typename Period>
  future_status wait_for(std::chrono::duration<Rep, Period> const& timeout_duration) const noexcept(false) {
    return (valid() && shared_state_->WaitFor(timeout_duration))  // VCA_VAC_VALID_MEMBER_VARIABLE_ACCESS
               ? future_status::ready
               : future_status::timeout;
  }

  // VECTOR Next Construct AutosarC++17_10-M5.14.1: MD_VAC_M5.14.1_operandHasNoSideEffect
  /*!
   * \brief   Wait until a specified absolute time.
   * \details Returns future_status::timeout directly if shared state is invalid.
   * \tparam  Clock The clock on which this time point is measured.
   * \tparam  Duration A std::chrono::duration type used to measure the time since epoch.
   * \param   abs_time maximum time point to block until.
   * \pre     -
   * \spec
   *   requires true;
   * \endspec
   * \return  Status that indicates whether the time was reached or if a value is available.
   * \trace   SPEC-7552476
   * \vpublic
   */
  template <typename Clock, typename Duration>
  future_status wait_until(std::chrono::time_point<Clock, Duration> const& abs_time) const noexcept(false) {
    return (valid() && shared_state_->WaitUntil(abs_time))  // VCA_VAC_VALID_MEMBER_VARIABLE_ACCESS
               ? future_status::ready
               : future_status::timeout;
  }

  // VECTOR Next Construct AutosarC++17_10-A18.5.8: MD_VAC_A18.5.8_localObjectsShallBeAllocatedOnStack
  /*!
   * \brief     Set a continuation for when the shared state is ready.
   * \details   When callable is called, it is guaranteed that get() and GetResult() will not block.
   *            Returns an invalid Future if shared state is invalid or a nullptr callable is provided.
   *            *this will be invalid after returning from this function. If a nullptr callable is provided,
   *            then *this will remain unchanged.
   * \remark    callable may be called in the context of this call or in the context of Promise::set_value() or
   *            Promise::SetError() or somewhere else.
   *            Warning: This function might use dynamic memory allocation. Use with caution!
   * \tparam    Type of continuation function.
   * \param[in] callable A continuation function to be attached, must have the signature XX callable(Future<void, E>).
   * \pre       -
   * \spec
   *   requires true;
   * \endspec
   * \return    A new Future instance for the result of the continuation.
   * \trace     SPEC-7552477
   * \vpublic
   */
  template <typename F>
  auto then(F&& callable) noexcept -> typename internal::TypeUnwrapping<decltype(callable(std::move(*this))), E>::type {
    /*! \brief Alias for the type of the value contained in the Future returned from this function. */
    using U = decltype(callable(std::move(*this)));

    /*! \brief Alias for the type of the value contained in the Future returned from this function. */
    using T2 = typename internal::TypeUnwrapping<U, E>::value_type;

    /*! \brief Alias for the type of the value contained in the Future returned from this function. */
    using E2 = typename internal::TypeUnwrapping<U, E>::error_type;

    Future<T2, E2> future;

    bool const is_null_ptr{vac::language::detail::IsCallableNullPtr(callable)};
    // Returns an invalid Future if *this is not valid and/or callable is nullptr.
    if ((ValidThen()) && (!is_null_ptr)) {
      // VECTOR NL AutosarC++17_10-A7.1.1: MD_VAC_A7.1.1_immutableVariableShallBeConstOrConstexprQualified
      FutureContinuationPtr fut_cont{future_continuation_};
      // Since the fut_cont pointer can only ever be nullptr if shared_state_ is nullptr,
      // checking shared_state_ in valid() is enough to place the assert here (to please VCA).
      assert(fut_cont.Get() != nullptr);  // COV_MSR_INV_STATE_ASSERT
      std::lock_guard<std::mutex> const lock{fut_cont->GetCallBackMutex()};
      if (is_ready()) {
        // Call the function if Future state is ready.
        // checking shared_state_ in valid() is enough to place the assert here (to please VCA).
        assert(fut_cont.Get() != nullptr);  // COV_MSR_INV_STATE_ASSERT
        if (fut_cont->GetContext()) {
          assert(fut_cont.Get() != nullptr);  // COV_MSR_INV_STATE_ASSERT
          future = fut_cont->SetCallBackHandler(std::forward<F>(callable), std::move(*this), true);
        } else {
          future = internal::GetFuture<void, E, U, T2, E2>()(std::forward<F>(callable), std::move(*this));
        }
      } else {
        // Move the future_continuation pointer to f_continuation to be used after this Future object gets invalidated.
        // VECTOR NL AutosarC++17_10-A7.1.1: MD_VAC_A7.1.1_immutableVariableShallBeConstOrConstexprQualified
        FutureContinuationPtr f_continuation{std::move(future_continuation_)};
        // Since the fut_cont pointer can only ever be nullptr if shared_state_ is nullptr,
        // checking shared_state_ in valid() is enough to place the assert here (to please VCA).
        assert(f_continuation.Get() != nullptr);  // COV_MSR_INV_STATE_ASSERT
        // Save the call back function in continuation to be called later when Promise is set.
        future = f_continuation->SetCallBackHandler(std::forward<F>(callable), std::move(*this));
      }  // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    }    // VCA_VAC_STD_FUNC_FULFILL_CONTRACT
    return future;
  }

  /*!
   * \brief   Set the context in which the callable give to Future::then() will execute.
   * \details If this function will be called, it must be called before calling Future::then(). If called after, then:
   *            1. the given context will not propagate to the Futures returned from then(),
   *            2. and it is not guaranteed that the callable will be executed in the given context.
   *          Will only set the context if:
   *            1. the Future is valid,
   *            2. a context has not previously been set,
   *            3. and the shared_ptr points to an object, i.e. "ec.get() != nullptr".
   * \tparam  ExecT The context type. Must inherit from internal::ExecutorInterface.
   * \param   ec A shared_ptr<ExecT> pointing to the object that will decide the context.
   * \return  True if a context was correctly set, false otherwise.
   * \threadsafe FALSE
   * \vprivate Product Private
   */
  template <typename ExecT,
            std::enable_if_t<std::is_base_of<internal::ExecutorInterface, ExecT>::value, std::int8_t> = 0>
  bool SetExecutionContext(std::shared_ptr<ExecT> ec) noexcept {
    // VCA_VAC_VALID_PRECONDITIONED_NONSTATIC_METHOD
    return ValidThen() ? future_continuation_->SetContext(ec) : false;
  }

  // VECTOR Next Construct AutosarC++17_10-M5.14.1: MD_VAC_M5.14.1_operandHasNoSideEffect
  /*!
   * \brief  Return true only when the shared state is ready. This method will return immediately and shall not do a
   *         blocking wait.
   * \pre    -
   * \return True if the future contains a value (or exception), false if not.
   * \spec
   *   requires true;
   * \endspec
   * \trace  SPEC-7552478
   * \vpublic
   */
  bool is_ready() const noexcept {
    return shared_state_ && shared_state_->Ready();  // VCA_VAC_VALID_MEMBER_VARIABLE_ACCESS
  }

 private:
  /*! \brief Checks if Future is valid and future_continuation_ is not a nullptr. */
  bool ValidThen() const noexcept { return valid() && future_continuation_; }

  /*! \brief Type used for cleanup callables. */
  using CleanUpCallable = vac::language::UniqueFunction<void()>;

  /*!
   * \brief     Parameterized constructor.
   * \param[in] shared_state object pointer to connect with Promise.
   * \param[in] future_continuation Future continuation pointer to manage the call back function.
   * \param[in] cleanup A callable to be called in ~Future(), iff state is valid and not ready.
   * \vprivate
   * \spec
   *   requires true;
   * \endspec
   */
  Future(SharedStatePtr const& shared_state, FutureContinuationPtr const& future_continuation,
         CleanUpCallable&& cleanup = CleanUpCallable{nullptr}) noexcept
      : shared_state_(shared_state), future_continuation_(future_continuation), cleanup_(std::move(cleanup)) {}

  /*! \brief The shared state between *this and the connected Promise. */
  SharedStatePtr shared_state_{nullptr};

  /*! \brief Future continuation pointer to manage the call back function. */
  FutureContinuationPtr future_continuation_{nullptr};

  /*! \brief UniqueFunction to hold an executable. */
  CleanUpCallable cleanup_{nullptr};

  /*!
   * \brief  Friend to only provide the parameterized ctor for use in Promise::get_future().
   * \tparam E The error type of the Promise and Result.
   */
  friend class Promise<void, E>;  // VECTOR SL AutosarC++17_10-A11.3.1: MD_VAC_A11.3.1_doNotUseFriend
};

}  // namespace core
}  // namespace ara

/*!
 * \exclusivearea   ara::core::internal::FutureContinuation::call_back_mutex_
 *                  Protect against parallel execution and setting the callback function
 *                  for future continuation
 * \protects        ara::core::Future::f_continuation
 * \usedin          ara::core::Future::then
 * \exclude         All other methods of a Future object
 * \length          LONG
 *                  This exclusive area may cover calls to several sub-functions
 * \endexclusivearea
 */

#endif  // LIB_VAC_INCLUDE_ARA_CORE_FUTURE_H_

//  COV_JUSTIFICATION_BEGIN
//    \ID COV_LIBVAC_COMPILE_WITH_NO_EXCEPTIONS
//      \ACCEPT XX
//      \REASON The function is designed to throw an exeception and should not be used when compiling without exception
//              support. Therefore, a flag is provided to enable/disable compiling with exceptions. However, function
//              cannot be covered because a compile-time flag prevents it to be called.
//  COV_JUSTIFICATION_END
