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
/*!        \file  skeleton_method.h
 *        \brief  Provides implementation for SkeletonMethodInterface.
 *
 *      \details  This file contains the implementation for SkeletonMethodInterface.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_SKELETON_METHOD_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_SKELETON_METHOD_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include <memory>
#include <string>
#include <tuple>
#include <utility>

#include "amsr/socal/internal/logging/ara_com_logger.h"
#include "amsr/socal/internal/methods/skeleton_method_interface.h"
#include "amsr/socal/internal/methods/skeleton_method_scheduler_interface.h"
#include "amsr/socal/internal/task.h"
#include "amsr/unique_ptr.h"
#include "ara/com/com_error_domain.h"
#include "ara/core/abort.h"
#include "ara/core/future.h"
#include "ara/core/promise.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace socal {
namespace internal {
namespace methods {

/*!
 * \brief   Skeleton method class for handling requests.
 * \details This class is responsible for receiving method calls for specific method, scheduling the call and return
 *          back the response once is available. The real method implementation is done within the Skeleton.
 * \tparam ResponseDataType  The type of method response data.
 * \tparam ArgsT             The method arguments' types.
 *
 * \unit Socal::Skeleton::SkeletonMethod::SkeletonMethod
 */
template <typename ResponseDataType, typename... ArgsT>
class SkeletonMethod final : public SkeletonMethodInterface<ResponseDataType, ArgsT...> {
  /*!
   * \brief Type alias for RequestorInterface template instantiation.
   */
  using SkeletonMethodRequestInterfaceType = SkeletonMethodRequestInterface<ArgsT...>;

 public:
  /*!
   * \brief Type alias for RequestorInterface required argument.
   */
  using SkeletonMethodRequestInterfaceUniquePtr = ::amsr::UniquePtr<SkeletonMethodRequestInterfaceType>;

  /*!
   * \brief The function type to be called on the skeleton to process the method request.
   */
  using MethodFunction = std::function<::ara::core::Future<ResponseDataType>(ArgsT...)>;

  /*!
   * \brief Type alias for ResponseHandler template instantiation.
   */
  using SkeletonResponseHandlerInterfaceType = SkeletonResponseHandlerInterface<ResponseDataType>;

  /*!
   * \brief Type alias for ResponseHandler required argument.
   */
  using SkeletonResponseHandlerInterfaceUniquePtr = ::amsr::UniquePtr<SkeletonResponseHandlerInterfaceType>;

  /*!
   * \brief Type alias for the Base class.
   */
  using BaseType = SkeletonMethodInterface<ResponseDataType, ArgsT...>;

  /*!
   * \brief Constructs the Skeleton method by initializing the fields by the provided arguments.
   * \param[in] scheduler      The instance responsible to schedule the calls to the skeleton. The given pointer must
   *                           not be nullptr and must be valid until the lifetime of this object.
   * \param[in] func           The method call on the skeleton side (shall not be nullptr). This must not be nullptr and
   *                           must be valid until the lifetime of this object.
   * \param[in] logger_prefix  The prefix to be used for logging.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \spec requires scheduler != nullptr && func != nullptr; \endspec
   * \steady FALSE
   */
  SkeletonMethod(SkeletonMethodSchedulerInterface* scheduler, MethodFunction&& func,
                 vac::container::CStringView const logger_prefix) noexcept
      : BaseType{},
        logger_{::amsr::socal::internal::logging::kAraComLoggerContextId,
                ::amsr::socal::internal::logging::kAraComLoggerContextDescription, logger_prefix.ToString()},
        scheduler_{scheduler},
        func_{std::move(func)} {}

  /*!
   * \brief Default destructor.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  // VCA_SOCAL_FREEING_STATICALLY_ALLOCATED_MEMORY, VCA_SOCAL_CALLING_STL_APIS
  ~SkeletonMethod() noexcept = default;

  SkeletonMethod() = delete;
  SkeletonMethod(SkeletonMethod const&) = delete;
  SkeletonMethod(SkeletonMethod&&) = delete;
  SkeletonMethod& operator=(SkeletonMethod const&) = delete;
  SkeletonMethod& operator=(SkeletonMethod&&) = delete;

 private:
  /*!
   * \brief Carry out the method request by scheduling a task where it shall be executed.
   * \param[in]  request           An unique pointer to the request interface to be used to retrieve the arguments. This
   *                               must not be a nullptr.
   * \param[out] response_handler  A unique pointer to the response handler. The function HandleMethodResponse() of the
   *                               response handler be called in the context of this function call (but can also be
   *                               called later on). This must not be a nullptr.
   * \pre         Corresponding service must be currently offered.
   * \context     Backend
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous FALSE
   * \spec requires request != nullptr && response_handler != nullptr; \endspec
   * \steady TRUE
   * \internal
   * - If the request and response handler are valid
   *   - Schedule a task via the scheduler.
   *
   * Calls "ara::core::Abort()" if:
   * - Either response anr/or request are invalid.
   * \endinternal
   */
  void HandleRequest(SkeletonMethodRequestInterfaceUniquePtr request,
                     SkeletonResponseHandlerInterfaceUniquePtr response_handler) noexcept final {
    if (request && response_handler) {
      // Schedule Task.
      // If HandleMethodRequest() returns false, feedback will be given through the response_handler during the
      // destruction of the object behind task_ptr.
      bool const result{
          // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
          scheduler_->HandleMethodRequest(ReturnMethodTaskPointer(std::move(request), std::move(response_handler)))};
      using vac::container::literals::operator""_sv;

      if (result) {
        logger_.LogVerbose(
            [](::amsr::socal::internal::logging::LogStream& s) { s << "Succeeded to schedule the task"_sv; },
            static_cast<char const*>(__func__), __LINE__);
      } else {
        logger_.LogError([](::amsr::socal::internal::logging::LogStream& s) { s << "Failed to schedule the task"_sv; },
                         static_cast<char const*>(__func__), __LINE__);
      }
    } else {  // VCA_SOCAL_CALLING_DESTRUCTOR_OF_STACK_OBJECTS
      logger_.LogFatalAndAbort(
          [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            // VCA_SOCAL_CALLING_STL_APIS
            s << "Invalid request or response handler provided by the bindings.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief   Creates a MethodTask using the polymorphic allocator.
   * \details Aborts if memory allocation fails.
   * \param[in]  request           A pointer to the method request that shall be accessed in context of the created
   *                               Task. This must not be a nullptr.
   * \param[out] response_handler  A unique pointer to the response handler. This must not be a nullptr.
   * \return UniquePtr to the allocated memory space of a MethodTask
   * \pre         Request and response_handler are not nullptr
   * \context     Backend
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec requires request != nullptr && response_handler != nullptr; \endspec
   * \steady TRUE
   * \internal
   * - Create a UniquePtr to a MethodTask object.
   *
   * Calls "ara::core::Abort()" if:
   * - AllocateUnique()'s result contains an error.
   * \endinternal
   */
  amsr::UniquePtr<::amsr::socal::internal::Task> ReturnMethodTaskPointer(
      SkeletonMethodRequestInterfaceUniquePtr request,
      SkeletonResponseHandlerInterfaceUniquePtr response_handler) noexcept {
    // Create Task.
    ara::core::Result<amsr::UniquePtr<MethodTask>> task_ptr_result{
        // VCA_SOCAL_PASSING_INITIALIZED_MEMBER_AS_REFERENCE,
        // VCA_SOCAL_PASSING_POINTERS_OR_REFERENCES_RECEIVED_BASED_ON_FUNCTION_CONTRACT
        amsr::AllocateUnique<MethodTask>(ara::core::PolymorphicAllocator<MethodTask>{}, *scheduler_, std::move(request),
                                         std::move(response_handler), func_, logger_)};

    if (!task_ptr_result.HasValue()) {
      ::ara::core::String abort_msg{"Failed to create a RequestAndResponseMethodTask: "};
      abort_msg.append(task_ptr_result.Error().Message());
      logger_.LogFatalAndAbort(
          [&abort_msg](::amsr::socal::internal::logging::AraComLogger::StringStream& s) { s << abort_msg; },
          static_cast<char const*>(__func__), __LINE__);
    }

    return std::move(task_ptr_result.Value());
  }

  /*!
   * \brief Alias for the tuple of skeleton method arguments.
   */
  using ArgsTuple = std::tuple<ArgsT...>;

  /*!
   * \brief Threadpool Task that encapsulates the following:
   *        - Call of the real function on the skeleton.
   *        - Waiting the result.
   *        - Set the output into the promise once it is ready.
   *
   * \unit Socal::Skeleton::SkeletonMethod::SkeletonMethod
   */
  class MethodTask final : public ::amsr::socal::internal::Task {
   public:
    /*!
     * \brief Constructor for the method task.
     * \param[in]  scheduler         Schedules the asynchronous method request.
     * \param[in]  request           A pointer to the request interface to retrieve the request arguments. This must not
     *                               be nullptr.
     * \param[out] response_handler  A pointer to the response handler to provide the response of the method execution.
     *                               This must not be nullptr.
     * \param[in]  func              A reference to the skeleton function to call.
     * \param[in]  logger            Logger reference. This must be valid until the lifetime of this object.
     * \pre         -
     * \context     App
     * \threadsafe  TRUE
     * \reentrant   TRUE
     * \synchronous FALSE
     * \spec requires true; \endspec
     * \steady TRUE
     */
    MethodTask(SkeletonMethodSchedulerInterface const& scheduler, SkeletonMethodRequestInterfaceUniquePtr request,
               SkeletonResponseHandlerInterfaceUniquePtr response_handler, MethodFunction& func,
               ::amsr::socal::internal::logging::AraComLogger const& logger) noexcept
        : ::amsr::socal::internal::Task{&scheduler},  // VCA_SOCAL_CALL_EXTERNAL_FUNCTION_CONST_POINTER_ARGUMENT
          logger_{logger},
          func_{func},
          // VCA_SOCAL_PASSING_POINTERS_OR_REFERENCES_RECEIVED_BASED_ON_FUNCTION_CONTRACT
          request_{std::move(request)},
          // VCA_SOCAL_PASSING_POINTERS_OR_REFERENCES_RECEIVED_BASED_ON_FUNCTION_CONTRACT
          response_handler_{std::move(response_handler)} {}

    /*!
     * \brief   Destructor for the method task.
     * \details If the task has not run yet the promise would be set to error before destroying the object.
     * \pre         -
     * \context     Backend | App | Callback.
     * \threadsafe  FALSE for same class instance, TRUE for different instances.
     * \reentrant   FALSE for same class instance, TRUE for different instances.
     * \synchronous FALSE
     * \spec requires true; \endspec
     * \steady TRUE
     * \internal
     * - If the method was not executed, inform response handler about unscheduled method request.
     * \endinternal
     */
    ~MethodTask() noexcept final {  // VCA_SOCAL_FREEING_STATICALLY_ALLOCATED_MEMORY
      if (!response_sent_) {
        ::ara::com::ComErrorDomain::SupportDataType const no_support_data_provided{0};
        ::amsr::core::ErrorCode const error{::ara::com::MakeErrorCode(
            ::ara::com::ComErrc::kErrorNotOk, no_support_data_provided,
            "Requested service will not be executed anymore due to object destruction of the task. "
            "Either the service has been stopped or the queue is full.")};
        // VECTOR NC AutosarC++17_10-M0.3.1: MD_Socal_AutosarC++17_10-M0.3.1_Pointer_is_null_at_dereference
        // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
        response_handler_->HandleErrorResponse(error);
      }
    }  // VCA_SOCAL_CALLING_DESTRUCTOR_OF_CLASS_MEMBERS

    MethodTask() = delete;
    MethodTask(MethodTask const&) = delete;
    MethodTask(MethodTask&&) = delete;
    MethodTask& operator=(MethodTask const&) = delete;
    MethodTask& operator=(MethodTask&&) = delete;

    /*!
     * \brief   Call the real function, wait the result and forward it to the response handler.
     * \details This function waits until the promise is fulfilled by the customers skeleton method implementation.
     * \pre         -
     * \context     App | Callback.
     * \threadsafe  FALSE for same class instance, TRUE for different instances.
     * \reentrant   FALSE for same class instance, TRUE for different instances.
     * \synchronous TRUE
     * \spec requires true; \endspec
     * \steady TRUE
     * \internal
     * - Fetch the arguments of user-defined method.
     * - In case of an error, inform response handler with an error response.
     * - Invoke method with the fetched arguments if the previous step succeeded.
     * - Forward the result (application error or actual result) of the method to the response handler.
     * \endinternal
     */
    void operator()() noexcept final {
      using vac::container::literals::operator""_sv;

      ArgsTuple args{};

      // VECTOR NL AutosarC++17_10-M0.3.1: MD_Socal_AutosarC++17_10-M0.3.1_Pointer_is_null_at_dereference
      if (request_->GetArgs(args)) {  // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
        logger_.LogVerbose(
            [](::amsr::socal::internal::logging::LogStream& s) { s << "Started execution of method."_sv; },
            static_cast<char const*>(__func__), __LINE__);

        ::ara::core::Future<ResponseDataType> const future{ApplyRequestFunctor(
            args, std::make_index_sequence<std::tuple_size<typename std::decay<ArgsTuple>::type>::value>{})};

        using ResultType = ::amsr::core::Result<ResponseDataType, ::amsr::core::ErrorCode>;

        // Blocking wait until future is fulfilled by the application.
        ResultType result{future.GetResult()};

        logger_.LogVerbose(
            [](::amsr::socal::internal::logging::LogStream& s) { s << "Finished execution of method."_sv; },
            static_cast<char const*>(__func__), __LINE__);

        if (result.HasValue()) {
          // VECTOR NC AutosarC++17_10-M0.3.1: MD_Socal_AutosarC++17_10-M0.3.1_Pointer_is_null_at_dereference
          // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
          response_handler_->HandleMethodResponse(std::move(result.Value()));
        } else {
          // VECTOR NC AutosarC++17_10-M0.3.1: MD_Socal_AutosarC++17_10-M0.3.1_Pointer_is_null_at_dereference
          // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
          response_handler_->HandleApplicationError(result.Error());
        }

      } else {
        logger_.LogError(
            [](::amsr::socal::internal::logging::LogStream& s) {
              s << "Invalid method arguments: Request will be dropped."_sv;
            },
            static_cast<char const*>(__func__), __LINE__);
        ::ara::com::ComErrorDomain::SupportDataType const no_support_data_provided{0};
        ::amsr::core::ErrorCode const error{
            ::ara::com::MakeErrorCode(::ara::com::ComErrc::kMethodArgumentsNotRetrieved, no_support_data_provided,
                                      "Retrieving the arguments for the method call failed.")};
        // VECTOR NC AutosarC++17_10-M0.3.1: MD_Socal_AutosarC++17_10-M0.3.1_Pointer_is_null_at_dereference
        // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
        response_handler_->HandleErrorResponse(error);
      }

      response_sent_ = true;
    }  // VCA_SOCAL_CALLING_DESTRUCTOR_OF_STACK_OBJECTS

   private:
    // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOCAL_AutosarC++17_10-A15.4.2_STL_exceptions
    // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOCAL_AutosarC++17_10-A15.5.3_STL_exceptions
    /*!
     * \brief Calls the method functor on the retrieved tuple of arguments. Needed to expand the tuple of argument into
     *        individual parameters.
     * \tparam    I  Parameter pack for the size of the tuple of arguments.
     * \param[in] t  The tuple of arguments needed by the method.
     * \return Future with response.
     * \pre         -
     * \context     App | Callback.
     * \threadsafe  TRUE
     * \reentrant   TRUE
     * \synchronous TRUE
     * \spec requires true; \endspec
     * \steady TRUE
     */
    template <std::size_t... I>
    constexpr auto ApplyRequestFunctor(ArgsTuple const& t, std::index_sequence<I...>) const noexcept
        -> ::ara::core::Future<ResponseDataType> {
      // VECTOR NL AutosarC++17_10-A5.0.1, VectorC++-V5.0.1: MD_SOCAL_VectorC++-V5.0.1_skeleton_method_arguments_unsequenced_call
      return func_(std::get<I>(t)...);  // VCA_SOCAL_CALLING_STL_APIS
    }

    /*!
     * \brief Logger.
     */
    ::amsr::socal::internal::logging::AraComLogger const& logger_;

    /*!
     * \brief The Skeleton method to be called.
     */
    MethodFunction& func_;

    /*!
     * \brief A pointer to the request object to fetch the method arguments.
     */
    SkeletonMethodRequestInterfaceUniquePtr const request_;

    /*!
     * \brief A response handler which will get the result of the method call.
     */
    SkeletonResponseHandlerInterfaceUniquePtr const response_handler_;

    /*!
     * \brief Indicates if response has already been sent. This is used to know if Skeleton is destroyed before a
     *        response has been sent, then an error response could be directly sent.
     */
    bool response_sent_{false};
  };

  /*!
   * \brief Logger.
   */
  ::amsr::socal::internal::logging::AraComLogger const logger_;

  /*!
   * \brief Pointer to the skeleton task scheduler.
   */
  SkeletonMethodSchedulerInterface* const scheduler_;

  /*!
   * \brief The skeleton function that should be invoked.
   */
  MethodFunction func_;
};

}  // namespace methods
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_SKELETON_METHOD_H_
