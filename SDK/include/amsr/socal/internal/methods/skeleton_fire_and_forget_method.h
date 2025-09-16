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
/**        \file  skeleton_fire_and_forget_method.h
 *        \brief  Provides implementation for SkeletonFireAndForgetMethodInterface.
 *
 *      \details  This file contains the implementation for SkeletonFireAndForgetMethodInterface.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_SKELETON_FIRE_AND_FORGET_METHOD_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_SKELETON_FIRE_AND_FORGET_METHOD_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include <memory>
#include <string>
#include <tuple>
#include <utility>

#include "amsr/socal/internal/logging/ara_com_logger.h"
#include "amsr/socal/internal/methods/skeleton_fire_and_forget_method_interface.h"
#include "amsr/socal/internal/methods/skeleton_method_request_interface.h"
#include "amsr/socal/internal/methods/skeleton_method_scheduler_interface.h"
#include "amsr/unique_ptr.h"
#include "ara/core/abort.h"

namespace amsr {
namespace socal {
namespace internal {
namespace methods {

/*!
 * \brief   Skeleton method class for handling fire and forget requests.
 * \details This class is responsible for receiving fire and forget method calls for specific method, passing the call
 *          task to the skeleton and eventually executing the call. The real method implementation is done within the
 *          Skeleton.
 * \tparam ArgsT  Types of method's arguments.
 *
 * \unit Socal::Skeleton::SkeletonMethod::SkeletonFireAndForgetMethod
 */
template <typename... ArgsT>
class SkeletonFireAndForgetMethod final : public SkeletonFireAndForgetMethodInterface<ArgsT...> {
  /*!
   * \brief Type alias for RequestorInterface template instantiation.
   */
  using SkeletonMethodRequestInterfaceType = SkeletonMethodRequestInterface<ArgsT...>;

 public:
  /*!
   * \brief Type alias for RequestorInterface required argument.
   */
  using SkeletonMethodRequestInterfaceUniquePtr = amsr::UniquePtr<SkeletonMethodRequestInterfaceType>;

  /*!
   * \brief The function type to be called on the skeleton to process the method request.
   */
  using FireAndForgetMethodFunction = std::function<void(ArgsT...)>;

  /*!
   * \brief Type alias for the Base class.
   */
  using BaseType = SkeletonFireAndForgetMethodInterface<ArgsT...>;

  /*!
   * \brief Constructor.
   * \param[in] scheduler      Schedules the asynchronous method request. The given pointer must not be nullptr and must
   *                           be valid until the lifetime of this object.
   * \param[in] logger_prefix  Fire and forget method name.
   * \param[in] func           The method call on the skeleton side. This must not be nullptr and must be valid until
   *                           the lifetime of this object.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \spec requires scheduler != nullptr && func != nullptr; \endspec
   * \steady FALSE
   */
  SkeletonFireAndForgetMethod(SkeletonMethodSchedulerInterface* scheduler,
                              vac::container::CStringView const logger_prefix,
                              FireAndForgetMethodFunction&& func) noexcept
      : BaseType{},
        logger_{::amsr::socal::internal::logging::kAraComLoggerContextId,
                ::amsr::socal::internal::logging::kAraComLoggerContextDescription, logger_prefix.ToString()},

        scheduler_{scheduler},
        func_{std::move(func)} {};

  /*!
   * \brief Destructor.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  ~SkeletonFireAndForgetMethod() noexcept = default;  // VCA_SOCAL_FREEING_STATICALLY_ALLOCATED_MEMORY

  SkeletonFireAndForgetMethod() = delete;
  SkeletonFireAndForgetMethod(SkeletonFireAndForgetMethod const&) = delete;
  SkeletonFireAndForgetMethod(SkeletonFireAndForgetMethod&&) = delete;
  SkeletonFireAndForgetMethod& operator=(SkeletonFireAndForgetMethod const&) & = delete;
  SkeletonFireAndForgetMethod& operator=(SkeletonFireAndForgetMethod&&) & = delete;

 private:
  /*!
   * \brief Handle incoming method request.
   * \param[in] request  A unique pointer to the request interface to be used to retrieve the arguments. This must not
   *                     be a nullptr.
   * \pre         Corresponding service must be currently offered.
   * \context     Backend
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous FALSE
   * \spec requires true; \endspec
   * \steady TRUE
   * \internal
   * - Create and schedule a task to execute the user-defined fire and forget method.
   *
   * Calls "ara::core::Abort()" if:
   * - The AllocateUnique() return result is an error.
   * \endinternal
   */
  void HandleRequest(SkeletonMethodRequestInterfaceUniquePtr request) noexcept final {
    if (request) {
      // Schedule Task by calling ReturnMethodTaskPointer(), which returns a UniquePtr object to a MethodTask
      // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
      bool const result{scheduler_->HandleMethodRequest(ReturnMethodTaskPointer(std::move(request)))};
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
            s << "Invalid request pointer provided by the bindings.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief   Creates a FireAndForgetMethodTask using the polymorphic allocator.
   * \details Aborts if memory allocation fails.
   * \param[in] request  A pointer to the method request that shall be accessed in context of the created Task. This
   *                     must not be a nullptr.
   * \return UniquePtr to the allocated memory space of a FireAndForgetMethodTask
   * \pre         -
   * \context     Backend
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec requires request != nullptr; \endspec
   * \steady TRUE
   * \internal
   * - Create a UniquePtr to a FireAndForgetMethodTask object.
   *
   * Calls "ara::core::Abort()" if:
   * - AllocateUnique()'s result contains an error.
   * \endinternal
   */
  amsr::UniquePtr<::amsr::socal::internal::Task> ReturnMethodTaskPointer(
      SkeletonMethodRequestInterfaceUniquePtr request) noexcept {
    // Create Task
    ara::core::Result<amsr::UniquePtr<FireAndForgetMethodTask>> task_ptr_result{
        // VCA_SOCAL_PASSING_INITIALIZED_MEMBER_AS_REFERENCE,
        // VCA_SOCAL_PASSING_POINTERS_OR_REFERENCES_RECEIVED_BASED_ON_FUNCTION_CONTRACT
        amsr::AllocateUnique<FireAndForgetMethodTask>(ara::core::PolymorphicAllocator<FireAndForgetMethodTask>{},
                                                      *scheduler_, std::move(request), func_, logger_)};

    if (!task_ptr_result.HasValue()) {
      ::ara::core::String abort_msg{"Failed to create a FireAndForgetMethodTask: "};
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
   * \brief Task that calls the real function on the skeleton.
   */
  // VCA_SOCAL_FREEING_STATICALLY_ALLOCATED_MEMORY
  class FireAndForgetMethodTask final : public ::amsr::socal::internal::Task {
   public:
    /*!
     * \brief Constructor for the method task.
     * \param[in] scheduler  To schedule fire and forget method invocation.
     * \param[in] request    Skeleton method request interface to fetch the arguments of fire and forget method. This
     *                       must not be nullptr.
     * \param[in] func       Reference to the user-defined fire and forget method.
     * \param[in] logger     Logger reference. This must be valid until the lifetime of this object.
     * \pre         -
     * \context     Backend
     * \threadsafe  TRUE
     * \reentrant   TRUE
     * \synchronous TRUE
     * \spec requires scheduler != nullptr; \endspec
     * \steady TRUE
     */
    FireAndForgetMethodTask(SkeletonMethodSchedulerInterface const& scheduler,
                            SkeletonMethodRequestInterfaceUniquePtr request, FireAndForgetMethodFunction& func,
                            ::amsr::socal::internal::logging::AraComLogger const& logger) noexcept
        : ::amsr::socal::internal::Task{&scheduler},  // VCA_SOCAL_CALL_EXTERNAL_FUNCTION_CONST_POINTER_ARGUMENT
          logger_{logger},
          func_{func},
          // VCA_SOCAL_PASSING_POINTERS_OR_REFERENCES_RECEIVED_BASED_ON_FUNCTION_CONTRACT
          request_{std::move(request)} {}

    /*!
     * \brief Executes the user-defined fire and forget method at the customers end.
     * \pre         -
     * \context     App | Callback.
     * \threadsafe  FALSE for same class instance, TRUE for different instances.
     * \reentrant   FALSE for same class instance, TRUE for different instances.
     * \synchronous TRUE
     * \spec requires true; \endspec
     * \steady TRUE
     * \internal
     * - Fetch the arguments of user-defined fire and forget method.
     * - Invoke fire and forget method with the fetched arguments.
     * \endinternal
     */
    void operator()() noexcept final {
      ArgsTuple args{};
      // VECTOR NL AutosarC++17_10-M0.3.1: MD_Socal_AutosarC++17_10-M0.3.1_Pointer_is_null_at_dereference
      if (!request_->GetArgs(args)) {  // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
        logger_.LogError(
            [](::amsr::socal::internal::logging::LogStream& s) {
              s << "Invalid method arguments. Request will be dropped.";
            },
            static_cast<char const*>(__func__), __LINE__);
      } else {
        logger_.LogVerbose(
            [](::amsr::socal::internal::logging::LogStream& s) { s << "Started execution of fire and forget method."; },
            static_cast<char const*>(__func__), __LINE__);

        ApplyRequestFunctor(args, std::index_sequence_for<ArgsT...>{});

        logger_.LogVerbose(
            [](::amsr::socal::internal::logging::LogStream& s) {
              s << "Finished execution of fire and forget method.";
            },
            static_cast<char const*>(__func__), __LINE__);
      }
    }  // VCA_SOCAL_CALLING_DESTRUCTOR_OF_STACK_OBJECTS

   private:
    /*!
     * \brief Calls the method functor on the retrieved tuple of arguments. Needed to expand the tuple of argument
     *        into individual parameters.
     * \tparam    I  Parameter pack for the size of the arguments tuple, allowed unsigned
     *               value range cannot be smaller than: [0 - 65535].
     * \param[in] t  The tuple of arguments needed by the method.
     * \pre         -
     * \context     App | Callback.
     * \threadsafe  FALSE for same class instance, TRUE for different instances.
     * \reentrant   FALSE for same class instance, TRUE for different instances.
     * \synchronous TRUE
     * \spec requires true; \endspec
     * \steady TRUE
     */
    template <std::size_t... I>
    constexpr void ApplyRequestFunctor(ArgsTuple const& t, std::index_sequence<I...>) const noexcept {
      // VECTOR NL AutosarC++17_10-A5.0.1, VectorC++-V5.0.1: MD_SOCAL_VectorC++-V5.0.1_skeleton_method_arguments_unsequenced_call
      func_(std::get<I>(t)...);  // VCA_SOCAL_CALLING_STL_APIS
    }

    /*!
     * \brief Logger.
     */
    ::amsr::socal::internal::logging::AraComLogger const& logger_;

    /*!
     * \brief The Skeleton method to be called.
     */
    FireAndForgetMethodFunction& func_;

    /*!
     * \brief Request interface to fetch the method arguments.
     */
    SkeletonMethodRequestInterfaceUniquePtr const request_;
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
  FireAndForgetMethodFunction func_;
};

}  // namespace methods
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_SKELETON_FIRE_AND_FORGET_METHOD_H_
