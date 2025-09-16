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
/*!        \file  skeleton.h
 *        \brief  Base class of a specific generated skeleton interface
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_SKELETON_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_SKELETON_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <atomic>
#include <cassert>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <utility>

#include "amsr/core/abort.h"
#include "amsr/core/instance_specifier.h"
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "amsr/generic/singleton_wrapper.h"
#include "amsr/socal/internal/configuration/configuration.h"
#include "amsr/socal/internal/event_mode_method_executor.h"
#include "amsr/socal/internal/factory/backend_factory_registry.h"
#include "amsr/socal/internal/internal_instance_identifier.h"
#include "amsr/socal/internal/lifecycle.h"
#include "amsr/socal/internal/logging/ara_com_logger.h"
#include "amsr/socal/internal/methods/skeleton_method_scheduler_interface.h"
#include "amsr/socal/internal/polling_mode_method_executor.h"
#include "amsr/socal/internal/runtime.h"
#include "amsr/socal/internal/task.h"
#include "amsr/socal/internal/thread_pools_manager.h"
#include "amsr/socal/internal/types.h"
#include "amsr/unique_ptr.h"
#include "ara/com/instance_identifier.h"
#include "ara/com/service_identifier.h"
#include "ara/com/types_common.h"
#include "ara/core/future.h"
#include "ara/core/result.h"
#include "ara/core/string_view.h"
#include "ara/core/vector.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace socal {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOCAL_AutosarC++17_10-M7.3.6_usingDirectiveInHeaderFile
using vac::container::literals::operator""_sv;  // NOLINT(build/namespaces)

// VECTOR NC Metric-OO.WMC.One: MD_Socal_Metric-OO.WMC.methods_per_class_is_above_limit
/*!
 * \brief   Skeleton is the base of a specific generated skeleton interface.
 * \details The base class provides config-independent features all service interfaces have in common. This includes
 *          threading and processing modes. The common behavior is shifted to the base class to be able to test the
 *          threading model and processing modes without the need of generated sources in unit-tests.
 *
 *          The following APIs are not considered to be threadsafe against each other, or against any other API in this
 *          instance:
 *          - OfferService(),
 *          - StopOfferService().
 * \tparam ServiceInterface                  The corresponding ServiceInterface.
 * \tparam ConcreteSkeletonBackendInterface  Type of the concrete generated service-specific skeleton interface.
 * \tparam Mode                              The runtime processing mode, can be [kPolling, kThreadDriven].
 *
 * \unit Socal::Skeleton::Skeleton
 */
template <typename ServiceInterface, typename ConcreteSkeletonBackendInterface,
          ::amsr::socal::internal::configuration::RuntimeProcessingMode Mode>
class Skeleton : public ::amsr::socal::internal::methods::SkeletonMethodSchedulerInterface {
  /*!
   * \brief Type alias for Socal runtime.
   */
  using Runtime = ::amsr::socal::internal::Runtime;

 public:
  /*!
   * \brief Delete default constructor.
   */
  Skeleton() = delete;
  /*!
   * \brief Delete copy constructor.
   */
  Skeleton(Skeleton const&) = delete;
  /*!
   * \brief Delete move constructor.
   */
  Skeleton(Skeleton&&) = delete;
  /*!
   * \brief Delete copy assignment.
   */
  Skeleton& operator=(Skeleton const&) & = delete;
  /*!
   * \brief Delete move assignment.
   */
  Skeleton& operator=(Skeleton&&) & = delete;

  /*!
   * \brief Service Identifier.
   */
  static ::ara::com::ServiceIdentifierType constexpr kServiceIdentifier{ServiceInterface::ServiceIdentifier};

  /*!
   * \brief Service shortname path.
   */
  static ::amsr::core::StringView constexpr kServiceShortNamePath{ServiceInterface::kServiceShortNamePath};

  /*!
   * \brief Type alias for the skeleton backend interface pointer.
   */
  using SkeletonBackendInterfaceUniquePtr = std::unique_ptr<ConcreteSkeletonBackendInterface>;

  /*!
   * \brief Type alias for the collection of skeleton backend interface pointers.
   */
  using SkeletonBackendInterfaceUniquePtrCollection = ::ara::core::Vector<SkeletonBackendInterfaceUniquePtr>;

  /*!
   * \brief Type alias for template instantiated SkeletonBackendFactoryRegistry.
   */
  using SkeletonBackendFactoryRegistry =
      ::amsr::socal::internal::factory::SkeletonBackendFactoryRegistry<ConcreteSkeletonBackendInterface>;

  /*!
   * \brief Type alias for a singleton of SkeletonBackendFactoryRegistry.
   */
  using SkeletonBackendFactoryRegistrySingleton = ::amsr::generic::Singleton<SkeletonBackendFactoryRegistry>;

  /*!
   * \brief Type alias for a singleton access of SkeletonBackendFactoryRegistry.
   */
  using SkeletonBackendFactoryRegistrySingletonAccess = ::amsr::generic::SingletonAccess<
      ::amsr::socal::internal::factory::SkeletonBackendFactoryRegistryInterface<ConcreteSkeletonBackendInterface>>;

  /*!
   * \brief Type alias for the PollingModeMethodExecutor.
   */
  using PollingModeMethodExecutor = ::amsr::socal::internal::PollingModeMethodExecutor;

  /*!
   * \brief Type alias for the EventModeMethodExecutor.
   */
  using EventModeMethodExecutor = ::amsr::socal::internal::EventModeMethodExecutor;

  /*!
   * \brief   ConstructionToken class for exception-less Skeleton construction.
   * \details Created via Preconstruct() API.
   * \vpublic
   *
   * \trace SPEC-8053549, SPEC-8053550
   */
  class ConstructionToken final {
   public:
    /*!
     * \brief Construct a ConstructionToken.
     * \param[in] instance_container  The container of instances of a service, each instance element needed to
     *                                distinguish different instances of exactly the same service in the system.
     * \param[in] binding_impls       The container of binding implementation pointers.
     * \param[in] thread_pool_id      A copy of the thread pool id used to obtain a reference to the thread pool.
     * \param[in] mode                Method call processing mode.
     * \param[in] max_requests        The max number of method requests that can be queued.
     * \pre      -
     * \context  App
     * \vprivate To be created by Skeleton::Preconstruct() APIs only.
     * \steady FALSE
     * \spec requires true; \endspec
     */
    explicit ConstructionToken(internal::InstanceSpecifierLookupTableEntryContainer&& instance_container,
                               SkeletonBackendInterfaceUniquePtrCollection&& binding_impls,
                               ::amsr::socal::internal::ThreadPoolAssignmentIdentifier const& thread_pool_id,
                               ::ara::com::MethodCallProcessingMode const mode, std::size_t const max_requests) noexcept
        : instance_container_{std::move(instance_container)},  // VCA_SOCAL_CALLING_STL_APIS
          binding_implementations_{std::move(binding_impls)},  // VCA_SOCAL_CALLING_STL_APIS
          thread_pool_id_{thread_pool_id.ToString()},
          mode_{mode},
          method_queue_size_{max_requests} {}

    ConstructionToken() = delete;
    ConstructionToken(ConstructionToken const&) = delete;
    ConstructionToken& operator=(ConstructionToken const&) & = delete;

    /*!
     * \brief Use default move constructor.
     * \steady FALSE
     * \spec requires true; \endspec
     */
    ConstructionToken(ConstructionToken&&) noexcept = default;

    /*!
     * \brief Use default move assignment operator.
     * \return ConstructionToken& reference to this.
     * \steady FALSE
     * \spec requires true; \endspec
     */
    ConstructionToken& operator=(ConstructionToken&&) & noexcept = default;  // VCA_SOCAL_CALLING_STL_APIS

    /*!
     * \brief Destructor.
     * \pre        -
     * \context    App
     * \threadsafe FALSE
     * \reentrant  FALSE
     * \vprivate
     * \steady FALSE
     * \spec requires true; \endspec
     * \internal
     * - Remove created instances from the static list of already skeleton instances.
     * \endinternal
     */
    ~ConstructionToken() noexcept {
      // If the container was not moved into a skeleton we ensure that the service instance can be made available
      // again. If it was moved this operation does nothing since this container is now empty.
      RemoveCreatedServiceInstances(instance_container_);
    }  // VCA_SOCAL_CALLING_DESTRUCTOR_OF_CLASS_MEMBERS

    // ---- Getter API ----
    /*!
     * \brief Gets a copy of the offered service instances container and clears the entry container for this token.
     * \return List of service instances to be offered.
     * \pre         -
     * \context     App
     * \threadsafe  FALSE
     * \reentrant   FALSE
     * \synchronous TRUE
     * \vprivate    To be used by Skeleton constructor only.
     * \steady FALSE
     * \spec requires true; \endspec
     */
    internal::InstanceSpecifierLookupTableEntryContainer ConsumeOfferedInstances() noexcept {
      return std::move(instance_container_);
    }

    /*!
     * \brief Gets a copy of the binding implementation pointers and clears the entry from this token.
     * \return List of binding implementation pointers.
     * \pre         -
     * \context     App
     * \threadsafe  FALSE
     * \reentrant   FALSE
     * \synchronous TRUE
     * \vprivate    To be used by Skeleton constructor only.
     * \steady FALSE
     * \spec requires true; \endspec
     */
    SkeletonBackendInterfaceUniquePtrCollection ConsumeBindingImplementations() noexcept {
      return std::move(binding_implementations_);
    }

    /*!
     * \brief Get thread pool identifier.
     * \return The thread pool identifier to be used by the skeleton.
     * \pre         -
     * \context     App
     * \threadsafe  FALSE
     * \reentrant   FALSE
     * \synchronous TRUE
     * \vprivate    To be used by Skeleton constructor only.
     * \steady FALSE
     * \spec requires true; \endspec
     */
    std::string& GetThreadPoolID() noexcept { return thread_pool_id_; }

    /*!
     * \brief Get method call processing mode.
     * \return Method call processing mode to be used by the constructor.
     * \pre         -
     * \context     App
     * \threadsafe  FALSE
     * \reentrant   FALSE
     * \synchronous TRUE
     * \vprivate    To be used by Skeleton constructor only.
     * \steady FALSE
     * \spec requires true; \endspec
     */
    ::ara::com::MethodCallProcessingMode GetProcessingMode() const noexcept { return mode_; }

    /*!
     * \brief Get the method queue size.
     * \return Method queue size.
     * \pre         -
     * \context     App
     * \threadsafe  FALSE
     * \reentrant   FALSE
     * \synchronous TRUE
     * \vprivate    To be used by Skeleton constructor only.
     * \steady FALSE
     * \spec requires true; \endspec
     */
    std::size_t GetMethodQueueSize() const noexcept { return method_queue_size_; }

   private:
    /*!
     * \brief Offered service instances.
     */
    internal::InstanceSpecifierLookupTableEntryContainer instance_container_;

    /*!
     * \brief List of binding-specific implementation of a service instance.
     */
    SkeletonBackendInterfaceUniquePtrCollection binding_implementations_;

    /*!
     * \brief Thread pool ID.
     */
    std::string thread_pool_id_;

    /*!
     * \brief Method call processing mode.
     */
    ::ara::com::MethodCallProcessingMode mode_;

    /*!
     * \brief The max number of method requests that can be queued.
     */
    std::size_t method_queue_size_;
  };

  /*!
   * \brief Type alias for the ConstructionToken result.
   */
  using ConstructionResult = ::ara::core::Result<ConstructionToken>;

  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOCAL_AutosarC++17_10-A15.5.3_GTestException
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOCAL_AutosarC++17_10-A15.4.2_GTestException
  /*!
   * \brief Exception-less pre-construction of a service skeleton using an InstanceIdentifier.
   * \param[in] instance_id   The InstanceIdentifier of the provided service instance. The given InstanceIdentifier must
   *                          fulfill the following preconditions:
   *                          - Must be configured in the ARXML model.
   *                          - Must belong to the service interface.
   * \param[in] mode          Processing mode to set for this skeleton object. Preconditions to be fulfilled:
   *                          - If the mode 'kEventSingleThread' is used, a possible user-defined ThreadPool must have
   *                            exactly one worker thread configured.
   * \param[in] max_requests  The max number of method requests that can be queued. Value '0' indicates unlimited queue
   *                          size.
   * \return Result<ConstructionToken> Result containing construction token from which a Skeleton object can be
   *         constructed.
   * \error ComErrc::kGrantEnforcementError   If the request is refused by IAM.
   * \error ComErrc::kExceedQueueSize         If the given max_requests value exceeds the max_number_of_tasks configured
   *                                          for the threadpool.
   * \error ComErrc::kCommunicationLinkError  If there was a connection failure.
   * \pre         No other service skeleton for the same instance must exist concurrently. After destruction of the
   *              already created skeleton object, the instantiation for the same service instance will be possible.
   * \pre         If the given processing mode is set to kEventSingleThread, the assigned threadpool must be configured
   *              with only one thread.
   * \pre         The given instance identifier must be configured in the ARXML model.
   * \pre         The given instance identifier must belong to the service interface.
   * \context     App
   * \threadsafe  FALSE against all overloads of Preconstruct.
   * \reentrant   FALSE against all overloads of Preconstruct.
   * \synchronous TRUE
   * \vprivate
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * Calls Abort() if:
   * - Socal is not initialized.
   * \endinternal
   *
   * \trace SPEC-8053551, SPEC-8053550
   */
  static ConstructionResult Preconstruct(::ara::com::InstanceIdentifier const& instance_id,
                                         ::ara::com::MethodCallProcessingMode const mode,
                                         std::size_t const max_requests) noexcept {
    if (!internal::IsComInitialized()) {
      ::amsr::socal::internal::logging::AraComLogger const logger{
          ::amsr::socal::internal::logging::kAraComLoggerContextId,
          ::amsr::socal::internal::logging::kAraComLoggerContextDescription, "Skeleton"_sv};
      logger.LogFatalAndAbort(
          [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            // VCA_SOCAL_CALLING_STL_APIS
            s << "API called before Socal is initialized.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    ::amsr::generic::SingletonAccess<Runtime> runtime{Runtime::getSingletonInstance().GetAccess()};
    ::amsr::socal::internal::ThreadPoolAssignmentIdentifier const thread_pool_id{
        // VCA_SOCAL_RUNTIME_LIFECYCLE, VCA_SOCAL_EXTERNAL_FUNCTION_CONTRACT
        runtime->GetProvidedInstanceSpecifierLookupTable()
            .ResolveInstanceSpecifier(instance_id, kServiceShortNamePath)
            .ToString()};
    // VCA_SOCAL_RUNTIME_LIFECYCLE, VCA_SOCAL_EXTERNAL_FUNCTION_CONTRACT
    return Preconstruct(runtime->GetProvidedInstanceSpecifierLookupTable().ConstructEntryContainerFromIdentifier(
                            instance_id, kServiceShortNamePath),
                        thread_pool_id, mode, max_requests);
  }  // VCA_SOCAL_CALLING_DESTRUCTOR_OF_STACK_OBJECTS

  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOCAL_AutosarC++17_10-A15.5.3_GTestException
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOCAL_AutosarC++17_10-A15.4.2_GTestException
  /*!
   * \brief Exception-less pre-construction of a service skeleton using an InstanceSpecifier.
   * \param[in] instance      The InstanceSpecifier of the provided service interface. The given InstanceSpecifier must
   *                          fulfill the following preconditions:
   *                          - Must be configured in the ARXML model.
   *                          - Must belong to the service interface.
   * \param[in] mode          The mode of the service implementation for processing service method invocations.
   *                          Preconditions to be fulfilled:
   *                          - If the mode 'kEventSingleThread' is used, a possible user-defined ThreadPool must have
   *                            exactly one worker thread configured.
   * \param[in] max_requests  The max number of method requests that can be queued. Value '0' indicates unlimited queue
   *                          size.
   * \return Result<ConstructionToken> Result containing construction token from which a skeleton object can be
   *         constructed.
   * \error ComErrc::kGrantEnforcementError   If the request is refused by IAM.
   * \error ComErrc::kExceedQueueSize         If the given max_requests value exceeds the max_number_of_tasks configured
   *                                          for the threadpool.
   * \error ComErrc::kCommunicationLinkError  If there was a connection failure.
   * \pre         No other service skeleton for the same instance must exist concurrently. After destruction of the
   *              already created skeleton object, the instantiation for the same service instance will be possible.
   * \pre         If the given processing mode is set to kEventSingleThread, the assigned threadpool must be configured
   *              with only one thread.
   * \pre         The given instance specifier must be configured in the ARXML model.
   * \pre         The given instance specifier must belong to the service interface.
   * \context     App
   * \threadsafe  FALSE against all overloads of Preconstruct.
   * \reentrant   FALSE against all overloads of Preconstruct.
   * \synchronous TRUE
   * \vprivate
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * Calls Abort() if:
   * - Socal is not initialized.
   * \endinternal
   *
   * \trace SPEC-8053553, SPEC-8053550
   */
  static ConstructionResult Preconstruct(::amsr::core::InstanceSpecifier const& instance,
                                         ::ara::com::MethodCallProcessingMode const mode,
                                         std::size_t const max_requests) noexcept {
    if (!internal::IsComInitialized()) {
      ::amsr::socal::internal::logging::AraComLogger const logger{
          ::amsr::socal::internal::logging::kAraComLoggerContextId,
          ::amsr::socal::internal::logging::kAraComLoggerContextDescription, "Skeleton"_sv};
      logger.LogFatalAndAbort(
          [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            // VCA_SOCAL_CALLING_STL_APIS
            s << "API called before Socal is initialized.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    ::amsr::socal::internal::ThreadPoolAssignmentIdentifier const thread_pool_id{instance.ToString()};
    ::amsr::generic::SingletonAccess<Runtime> runtime{Runtime::getSingletonInstance().GetAccess()};
    return Preconstruct(
        // VCA_SOCAL_RUNTIME_LIFECYCLE
        (runtime->GetProvidedInstanceSpecifierLookupTable())
            // VCA_SOCAL_EXTERNAL_FUNCTION_CONTRACT
            .ConstructEntryContainerFromSpecifier(instance, kServiceShortNamePath),
        thread_pool_id, mode, max_requests);
  }

  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOCAL_AutosarC++17_10-A15.5.3_GTestException
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOCAL_AutosarC++17_10-A15.4.2_GTestException
  /*!
   * \brief Exception-less pre-construction of a service skeleton using an InstanceIdentifierContainer.
   * \param[in] instance_identifiers  The container of instance identifiers of the provided instances of a service,
   *                                  each instance element needed to distinguish different instances of exactly the
   *                                  same service in the system.
   *                                  The given InstanceIdentifierContainer must fulfill the following preconditions:
   *                                  - Every InstanceIdentifier of the container must be configured in the ARXML model.
   *                                  - Every InstanceIdentifier of the container must belong to the service interface.
   *                                  - The container must not be empty.
   *                                  - All elements of the container must be unique (no duplicates).
   * \param[in] mode                  The mode of the service implementation for processing service method invocations.
   *                                  Default: Event-driven processing. Preconditions to be fulfilled:
   *                                  - If the mode 'kEventSingleThread' is used, a possible user-defined ThreadPool
   *                                    must have exactly one worker thread configured.
   * \param[in] max_requests          The max number of method requests that can be queued. Value '0' indicates
   *                                  unlimited queue size.
   * \return Result<ConstructionToken> Result containing construction token from which a skeleton object can be
   *         constructed.
   * \error ComErrc::kGrantEnforcementError   If the request is refused by IAM.
   * \error ComErrc::kExceedQueueSize         If the given max_requests value exceeds the max_number_of_tasks configured
   *                                          for the threadpool.
   * \error ComErrc::kCommunicationLinkError  If there was a connection failure.
   * \pre         No other service skeleton for the same instance must exist concurrently. After destruction of the
   *              already created skeleton object, the instantiation for the same service instance will be possible.
   * \pre         If the given processing mode is set to kEventSingleThread, the assigned threadpool must be configured
   *              with only one thread.
   * \pre         The given InstanceIdentifierContainer shall not be empty and shall not contain duplicates.
   * \pre         The given instance identifiers must be configured in the ARXML model.
   * \pre         The given instance identifiers must belong to the service interface.
   * \context     App
   * \threadsafe  FALSE against all overloads of Preconstruct.
   * \reentrant   FALSE against all overloads of Preconstruct.
   * \synchronous TRUE
   * \vprivate
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * Calls Abort() if:
   * - Socal is not initialized.
   * - Provided InstanceIdentifierContainer is empty.
   * \endinternal
   *
   * \trace SPEC-8053555, SPEC-8053550
   */
  static ConstructionResult Preconstruct(::ara::com::InstanceIdentifierContainer const& instance_identifiers,
                                         ::ara::com::MethodCallProcessingMode const mode,
                                         std::size_t const max_requests) noexcept {
    ::amsr::socal::internal::logging::AraComLogger const logger{
        ::amsr::socal::internal::logging::kAraComLoggerContextId,
        ::amsr::socal::internal::logging::kAraComLoggerContextDescription, "Skeleton"_sv};
    if (!internal::IsComInitialized()) {
      logger.LogFatalAndAbort(
          [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            // VCA_SOCAL_CALLING_STL_APIS
            s << "API called before Socal is initialized.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    if (instance_identifiers.empty()) {
      logger.LogFatalAndAbort(
          [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            // VCA_SOCAL_CALLING_STL_APIS
            s << "The user provided instance identifier container must not be empty. [SWS_CM_10451].";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    ::amsr::generic::SingletonAccess<Runtime> runtime{Runtime::getSingletonInstance().GetAccess()};
    ::amsr::socal::internal::ThreadPoolAssignmentIdentifier const thread_pool_id{
        runtime
            ->GetProvidedInstanceSpecifierLookupTable()  // VCA_SOCAL_RUNTIME_LIFECYCLE
            // VCA_SOCAL_EXTERNAL_FUNCTION_CONTRACT
            .ResolveInstanceSpecifier(instance_identifiers[0U], kServiceShortNamePath)
            .ToString()};
    // VCA_SOCAL_CALLING_DESTRUCTOR_OF_STACK_OBJECTS
    return Preconstruct(
        // VCA_SOCAL_RUNTIME_LIFECYCLE, VCA_SOCAL_EXTERNAL_FUNCTION_CONTRACT
        runtime->GetProvidedInstanceSpecifierLookupTable().ConstructEntryContainerFromIdentifierContainer(
            instance_identifiers, kServiceShortNamePath),
        thread_pool_id, mode, max_requests);
  }

  /*!
   * \brief Exception-less constructor of a skeleton using a ConstructionToken.
   * \param[in] token  ConstructionToken created with Preconstruct() API.
   *                   - Must be created with the Preconstruct API.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * Calls Abort() if:
   * - Socal is not initialized.
   * \endinternal
   *
   * \trace SPEC-8053551, SPEC-8053553, SPEC-8053555
   */
  // VECTOR NC AutosarC++17_10-A12.1.5: MD_SOCAL_AutosarC++17_10-A12.1.5_delegating_constructor_for_common_init
  explicit Skeleton(ConstructionToken&& token) noexcept
      : ::amsr::socal::internal::methods::SkeletonMethodSchedulerInterface{},
        offered_instances_{token.ConsumeOfferedInstances()},
        processing_mode_{token.GetProcessingMode()},
        binding_implementations_{token.ConsumeBindingImplementations()},
        thread_pool_id_{std::move(token.GetThreadPoolID())},
        method_executor_{InitializeMethodExecutor(processing_mode_, ::amsr::core::StringView{thread_pool_id_},
                                                  offered_instances_, token.GetMethodQueueSize())} {
    logger_.LogInfo(
        [&token](::amsr::socal::internal::logging::LogStream& s) {
          ::ara::core::StringView const service_id_name{kServiceIdentifier.ToString()};
          std::size_t const executor_queue_size{token.GetMethodQueueSize()};
          // VCA_SOCAL_AMSR_LOGGER_UNDEFINED_OPERATOR_FUNCTION
          s << "Skeleton created for Service '" << service_id_name << "' with method executor queue size '"
            << executor_queue_size << "'.";
        },
        static_cast<char const*>(__func__), __LINE__);
  }

  /*!
   * \brief Offer the service.
   * \pre         The instance identifier must be valid and must not be malformed.
   * \pre         All of the fields of the Skeleton with Setter must have a SetHandler registered.
   * \pre         If SOME/IP is configured as the communication binding the application must be connected to the Daemon.
   * \pre         All fields of the Skeleton must be initialized that have a notifier or a getter with a registered
   *              "GetHandler".
   * \pre         IAM access must be granted.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - If the service is not already offered.
   *   - Do field initialization check.
   *   - Synchronize against HandleMethodRequest and set the accept_method_requests_ flag.
   *   - Update the service offered state to true.
   *   - Synchronize against the parallel IsServiceOfferedSync() calls.
   *   - Forward the offer service message to all the binding implementations.
   * - Otherwise, log an error that the service has been offered already.
   *
   * Calls Abort() if:
   * - No backend was registered by the bindings.
   * \endinternal
   *
   * \trace SPEC-4980347, SPEC-4980348, SPEC-4980364, SPEC-4980365
   */
  void OfferService() noexcept {
    // PTP-B-Socal-Skeleton_OfferService
    if (!service_offered_) {
      DoFieldInitializationChecks();  // VCA_SOCAL_CALLING_MEMBER_FUNCTION_ON_THIS_PTR
      logger_.LogInfo(
          [this](::amsr::socal::internal::logging::LogStream& s) {
            // VCA_SOCAL_AMSR_LOGGER_UNDEFINED_OPERATOR_FUNCTION
            s << "Offering the service with instance ID(s):";
            // VCA_SOCAL_OPERATOR_IN_RANGE_BASED_LOOP
            for (internal::InstanceSpecifierLookupTableEntry const& entry : offered_instances_) {
              // VCA_SOCAL_REFERENCE_VARIABLE_IN_RANGE_BASED_LOOP
              ::ara::core::StringView const instance_id_sv{entry.ToString()};
              s << " " << instance_id_sv;
            }
          },
          static_cast<char const*>(__func__), __LINE__);
      {
        // Set accept_method_requests_ to TRUE as skeleton may receive method requests during the OfferService call to
        // the binding implementations.
        std::lock_guard<std::mutex> const request_guard{method_request_sync_lock_};
        accept_method_requests_ = true;
      }
      // Set service_offered_ to TRUE before calling the OfferService on the binding implementations, as the skeleton
      // may receive the method requests immediately after the call to the bindings, and the application may send
      // events/field notifications from the method callback. See ESCAN00113185.
      service_offered_ = true;

      std::lock_guard<std::mutex> const request_guard{offer_service_sync_lock_};
      // PTP-B-Socal-Skeleton_OfferService_OverTheBindings
      for (SkeletonBackendInterfaceUniquePtr const& impl_ptr : binding_implementations_) {
        // VECTOR NL AutosarC++17_10-M0.3.1: MD_Socal_AutosarC++17_10-M0.3.1_null_at_dereference_falsepositive
        impl_ptr->OfferService();  // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
      }
      // PTP-E-Socal-Skeleton_OfferService_OverTheBindings
    } else {
      logger_.LogError(
          [this](::amsr::socal::internal::logging::LogStream& s) {
            ::ara::core::StringView const service_identifier{kServiceIdentifier.ToString()};
            // VCA_SOCAL_AMSR_LOGGER_UNDEFINED_OPERATOR_FUNCTION
            s << "'" << service_identifier << "' service with instance ID(s) '";
            // VCA_SOCAL_OPERATOR_IN_RANGE_BASED_LOOP
            for (internal::InstanceSpecifierLookupTableEntry const& entry : offered_instances_) {
              // VCA_SOCAL_REFERENCE_VARIABLE_IN_RANGE_BASED_LOOP
              ::ara::core::StringView const instance_id{entry.ToString()};
              s << " " << instance_id;
            }
            s << "' is already offered.";
          },
          static_cast<char const*>(__func__), __LINE__);

      // NOTE: Do not abort the process here.
      // As per AUTOSAR_EXP_ARAComAPI: method is idempotent - could be called repeatedly.
    }
    // PTP-E-Socal-Skeleton_OfferService
  }

  /*!
   * \brief   Stop offering the service.
   * \details If the service is already stopped, the call will directly return. This call will be blocked until all
   *          current method request callbacks are finished/canceled. So it must not be called within the method
   *          implementation or inside field Get/Set handlers.
   * \pre         The instance identifier must be valid and must not be malformed.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - If the service has been offered.
   *   - Synchronize against HandleMethodRequest to reject new incoming method requests.
   *   - Stop all scheduled method tasks related to this skeleton.
   *   - Reset the service offered state to false.
   *   - Forward the stop offer message to all the binding implementations.
   * - Otherwise, silently return (As per AUTOSAR_EXP_ARAComAPI: method is idempotent - could be called repeatedly).
   * \endinternal
   *
   * \trace SPEC-4980350
   */
  void StopOfferService() noexcept {
    if (service_offered_) {
      logger_.LogInfo(
          [this](::amsr::socal::internal::logging::LogStream& s) {
            // VCA_SOCAL_AMSR_LOGGER_UNDEFINED_OPERATOR_FUNCTION
            s << "StopOfferService for instance ID(s):";
            // VCA_SOCAL_OPERATOR_IN_RANGE_BASED_LOOP
            for (internal::InstanceSpecifierLookupTableEntry const& entry : offered_instances_) {
              // VCA_SOCAL_REFERENCE_VARIABLE_IN_RANGE_BASED_LOOP
              ::ara::core::StringView const instance_id_sv{entry.ToString()};
              s << " " << instance_id_sv;
            }
          },
          static_cast<char const*>(__func__), __LINE__);
      // PTP-B-Socal-Skeleton_StopOfferService
      {
        // method_request_sync_lock_ must be scoped to unblock caller of HandleMethodRequest.
        std::lock_guard<std::mutex> const request_guard{method_request_sync_lock_};
        accept_method_requests_ = false;
        // After this no more new method requests shall be enqueued.
      }
      // Clean-up all pending tasks / wait for completion of a task in progress.
      // This must be done prior to resetting the service offered flag as the SkeletonEvent APIs will trigger process
      // abort, if called from the method callback context (see ESCAN00113578 / ESCAN00113580).
      method_executor_->Cleanup();  // VCA_SOCAL_SKELETON_METHOD_EXECUTOR

      service_offered_ = false;

      // PTP-B-Socal-Skeleton_StopOfferService_OverTheBindings
      // Stop all service offers of all binding-specific instances. After this call, no more callbacks might be
      // received. This must be done before any pending tasks are removed to avoid that new tasks are added after
      // stopping the service.
      for (SkeletonBackendInterfaceUniquePtr const& impl_ptr : binding_implementations_) {
        // VECTOR NL AutosarC++17_10-M0.3.1: MD_Socal_AutosarC++17_10-M0.3.1_null_at_dereference_falsepositive
        impl_ptr->StopOfferService();  // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
      }
      // PTP-E-Socal-Skeleton_StopOfferService_OverTheBindings
      // PTP-E-Socal-Skeleton_StopOfferService
    }
  }

  /*!
   * \brief   Fetches the next call from the Communication Management and executes it.
   * \details This method is only available in polling mode and aborts the process when called in event mode.
   *          ProcessNextMethodCall() will block until the returned future from the skeleton method callback is ready
   *          (value or error set to promise). Therefore, if the application is single threaded, the method callback
   *          must ensure to return a future which is ready. A multi- threaded application may also use another thread
   *          to set the future to ready.
   * \return The future contains a boolean value of true, if there is a method request pending in the global request
   *         queue, which is being processed, false if there was no method request pending. The promise's value is set
   *         as soon as the request has been processed.
   * \pre         This function must not be called on a skeleton that uses a processing mode other than "kPoll".
   * \pre         The service is offered.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   * \internal
   * - Defer the call to ExecuteNextMethodCall() of the concrete MethodExecutor and return its result.
   *
   * Calls "std::terminate()" if:
   * - "std::bad_alloc" is thrown from std::make_unique().
   * - Mutex locking in deferred ExecuteNextMethodCall() fails.
   *
   * Calls Abort() if:
   * - The service is currently not offered.
   * - The method call processing mode is other than "kPoll".
   * \endinternal
   *
   * \trace SPEC-4980093, SPEC-4980128, SPEC-4980357
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_Socal_AutosarC++17_10-M9.3.3_method_can_be_declared_const
  auto ProcessNextMethodCall() noexcept -> ::ara::core::Future<bool> {
    if (not IsServiceOffered()) {
      logger_.LogFatalAndAbort(
          [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            // VCA_SOCAL_CALLING_STL_APIS
            s << "API called before offering the service or after stop offering.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    if (not IsMethodCallProcessingInPollingMode(processing_mode_)) {  // COV_SOCAL_CONDITION_SUBJECT_TO_PROCESSING_MODE
      logger_.LogFatalAndAbort(
          [](::amsr::socal::internal::logging::AraComLogger::StringStream&
                 s) {  // COV_SOCAL_UNCHECKED_LAMBDA_IN_POLLING_MODE
            // VCA_SOCAL_CALLING_STL_APIS
            s << "Must only be called if 'MethodCallProcessingMode' is 'kPoll'.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    return method_executor_->ExecuteNextMethodCall();  // VCA_SOCAL_SKELETON_METHOD_EXECUTOR
  }

  /*!
   * \brief Getter for all backend-specific implementations provided after OfferService call.
   * \return The collection of bakend-specific instance implementations.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vprivate
   * \steady TRUE
   * \spec requires true; \endspec
   */
  SkeletonBackendInterfaceUniquePtrCollection const& GetBackendInterfaces() const noexcept {
    return binding_implementations_;
  }

  /*!
   * \brief   Get the current state of service offer.
   * \details API returns only the current state.
   * \return true if service is offered, otherwise false.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   * \steady TRUE
   * \spec requires true; \endspec
   */
  bool IsServiceOffered() const noexcept { return service_offered_; }

  /*!
   * \brief   Get the current state of service offer by synchronizing against OfferService over the bindings.
   * \details This API must only be used to synchronize sending of events against the offer service towards bindings.
   * \return true if service is offered, otherwise false.
   * \pre         -
   * \context     App
   * \synchronous TRUE
   * \vprivate
   * \steady TRUE
   * \spec requires true; \endspec
   */
  bool IsServiceOfferedSync() noexcept {
    std::lock_guard<std::mutex> const request_guard{offer_service_sync_lock_};
    return IsServiceOffered();
  }

  /*!
   * \brief Creates the singleton static instances.
   * \pre         This function must only be called once.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - Create the singleton instance of created_service_instances_container_.
   * - Create the singleton instance of skeleton_backend_factory_registry_.
   * \endinternal
   */
  // VCA_SOCAL_EXTERNAL_FUNCTION_CONTRACT
  static void CreateSingletons() noexcept {
    created_service_instances_container_.Create();
    skeleton_backend_factory_registry_.Create();
  }

  /*!
   * \brief Destroys the singleton static instances.
   * \pre         This function must only be called once.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - Destroy the singleton instance of created_service_instances_container_.
   * - Destroy the singleton instance of skeleton_backend_factory_registry_.
   * \endinternal
   */
  // VCA_SOCAL_EXTERNAL_FUNCTION_CONTRACT
  static void DestroySingletons() noexcept {
    created_service_instances_container_.Destroy();
    skeleton_backend_factory_registry_.Destroy();
  }

  /*!
   * \brief Getter for SkeletonBackendFactoryRegistry singleton member.
   * \return SingletonAccess object from the singleton SkeletonBackendFactoryRegistry member.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * Calls "amsr::core::Abort()" if:
   * - SkeletonBackendFactoryRegistry Singleton was not initialized.
   * \endinternal
   */
  static auto GetSkeletonFactoryRegistry() noexcept -> SkeletonBackendFactoryRegistrySingletonAccess {
    return static_cast<SkeletonBackendFactoryRegistrySingletonAccess>(skeleton_backend_factory_registry_.GetAccess());
  }

 protected:
  /*!
   * \brief Removes the instance id from the created skeletons list.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, FALSE for different instances. FALSE against all overloads of
   *              Preconstruct API for same/different class instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vprivate
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - Remove service instances from static list of created instances.
   * \endinternal
   */
  ~Skeleton() noexcept {
    logger_.LogInfo(
        [](::amsr::socal::internal::logging::LogStream& s) {
          ::ara::core::StringView const service_id_name{kServiceIdentifier.ToString()};
          // VCA_SOCAL_AMSR_LOGGER_UNDEFINED_OPERATOR_FUNCTION
          s << "Destroy Skeleton for Service '" << service_id_name << "'.";
        },
        static_cast<char const*>(__func__), __LINE__);

    // Remove service instances from static list of already created service instances.
    RemoveCreatedServiceInstances(offered_instances_);
  }  // VCA_SOCAL_CALLING_DESTRUCTOR_OF_STACK_OBJECTS

  /*!
   * \brief Checks if all fields are correctly initialized and throws the corresponding exception in case of failure.
   * \pre     All of the fields of the Skeleton with Setter must have a SetHandler registered.
   * \pre     If SOME/IP is configured as the communication binding and the application must be connected to the Daemon.
   * \pre     All fields of the Skeleton must be initialized that have a notifier or a getter with a registered
   *          "GetHandler"
   * \context App
   * \steady FALSE
   * \spec requires true; \endspec
   *
   * \trace SPEC-4980364, SPEC-4980365
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_PureVirtualFunctionCanBeConst_FalsePositive
  virtual void DoFieldInitializationChecks() noexcept = 0;

  /*!
   * \brief Logger.
   */
  // VECTOR NC VectorC++-V11.0.2: MD_SOCAL_V11-0-2_protected_member_used_for_test_code
  ::amsr::socal::internal::logging::AraComLogger logger_{
      ::amsr::socal::internal::logging::kAraComLoggerContextId,
      ::amsr::socal::internal::logging::kAraComLoggerContextDescription, kServiceShortNamePath.ToString()};

  /*!
   * \brief   Instances represented by this skeleton object.
   * \details Depending on how this skeleton object was created, it could contain either.
   *          - A single entry (i.e. skeleton was created with an instance id) or.
   *          - Multiple entries (i.e. skeleton was created with an instance specifier configured for multiple instance
   *            identifiers).
   */
  internal::InstanceSpecifierLookupTableEntryContainer const offered_instances_;

  /*!
   * \brief Determines whether this service instance is offered. Set to true once the service is offered. Set to false
   *        once StopOfferService is called. Initially set to false.
   *
   * \trace SPEC-4980348
   */
  // VECTOR NC VectorC++-V11.0.2: MD_SOCAL_V11-0-2_protected_member_used_for_test_code
  std::atomic<bool> service_offered_{false};

 private:
  /*!
   * \brief Enqueues an incoming method request.
   * \param[in] request  This object contains all the information necessary to:
   *                     - Call the concrete method in the frontend;
   *                     - Get the return value with a given type from the future, because the return type is also known
   *                       to the backend.
   *                     - Call the response path in the binding-related part with the information provided within this
   *                       object.
   * \return true if a request was enqueued, false if not, i.e. scheduling fails or service is not offered.
   * \pre         -
   * \context     Backend
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances in ThreadDriven runtime processing mode,
   *              TRUE for different instances in Polling runtime processing mode.
   * \synchronous FALSE
   * \vprivate
   * \steady TRUE
   * \spec requires true; \endspec
   * \internal
   * - If the method requests can be enqueued.
   *   - Forward the method request to the concrete MethodExecutor and return its result.
   *
   * Calls "std::terminate()" if:
   * - Mutex locking fails.
   * \endinternal
   *
   * \trace SPEC-4980093, SPEC-4980092, SPEC-4980127, SPEC-4980128
   */
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOCAL_AutosarC++17_10-A15.4.2_STL_exceptions
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOCAL_AutosarC++17_10-A15.5.3_STL_exceptions
  auto HandleMethodRequest(amsr::UniquePtr<internal::Task>&& request) noexcept -> bool override {
    assert(request != nullptr);
    bool method_executed{false};
    {
      std::lock_guard<std::mutex> const request_guard{method_request_sync_lock_};
      if (accept_method_requests_) {
        method_executed =
            method_executor_->HandleMethodRequest(std::move(request));  // VCA_SOCAL_SKELETON_METHOD_EXECUTOR
      } else {
        logger_.LogDebug(
            [](::amsr::socal::internal::logging::LogStream& s) {
              s << "Service is not offered, method request will not be scheduled.";
            },
            static_cast<char const*>(__func__), __LINE__);
      }
    }
    return method_executed;
  }

  /*!
   * \brief Compare the runtime processing mode and method processing mode.
   * \param[in] processing_mode  Skeleton method processing mode.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * Calls Abort() if:
   * - Method processing mode is not 'kPoll' in runtime polling mode configuration.
   * \endinternal
   */
  static void CheckMethodProcessingModeIsApplicable(
      ::ara::com::MethodCallProcessingMode const processing_mode) noexcept {
    // VECTOR NC AutosarC++17_10-M0.1.2: MD_SOCAL_AutosarC++17_10-M0.1.2_dead_branch_BasedOnProcessingMode
    if ((Mode ==  // COV_SOCAL_CONDITION_SUBJECT_TO_PROCESSING_MODE
         internal::configuration::RuntimeProcessingMode::kPolling) &&
        (processing_mode !=  // COV_SOCAL_CONDITION_SUBJECT_TO_PROCESSING_MODE
         ::ara::com::MethodCallProcessingMode::kPoll)) {
      ::amsr::socal::internal::logging::AraComLogger const logger{
          ::amsr::socal::internal::logging::kAraComLoggerContextId,
          ::amsr::socal::internal::logging::kAraComLoggerContextDescription, kServiceShortNamePath.ToString()};
      logger.LogFatalAndAbort(
          [](::amsr::socal::internal::logging::AraComLogger::StringStream&
                 s) {  // COV_SOCAL_UNCHECKED_LAMBDA_IN_THREADED_MODE
            // VCA_SOCAL_CALLING_STL_APIS
            s << "Wrong MethodCallProcessingMode configured. In RuntimeProcessingMode 'kPolling' only 'kPoll' is "
                 "supported "
                 "as MethodCallProcessingMode.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief Provides a singleton access to the runtime.
   * \return Singleton access to the runtime.
   * \pre         Runtime must be initialized.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * Calls Abort() if:
   * - Socal is not initialized.
   * \endinternal
   */
  static ::amsr::generic::SingletonAccess<Runtime> GetRuntimeAccess() noexcept {
    if (!internal::IsComInitialized()) {
      ::amsr::socal::internal::logging::AraComLogger const logger{
          ::amsr::socal::internal::logging::kAraComLoggerContextId,
          ::amsr::socal::internal::logging::kAraComLoggerContextDescription, "Skeleton"_sv};
      logger.LogFatalAndAbort(
          [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            // VCA_SOCAL_CALLING_STL_APIS
            s << "API called before Socal is initialized.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    return Runtime::getSingletonInstance().GetAccess();
  }

  /*!
   * \brief Exception-less pre-construction of a service skeleton using an InstanceSpecifierLookupTableEntryContainer.
   * \param[in] offered_instances  The container of service instance to be offered. Each instance element needed to
   *                               distinguish different instances of exactly the same service in the system.
   *                               Preconditions to be fulfilled:
   *                               - All service instances must be configured in the ARXML model.
   *                               - All service instances must belong to the service interface.
   *                               - The container must not be empty.
   *                               - All service instances must be unique (no duplicates).
   * \param[in] thread_pool_id     Identifier of the thread pool assigned to this skeleton.
   * \param[in] mode               Processing mode to set for this skeleton object. Preconditions to be fulfilled:
   *                               - If the mode 'kEventSingleThread' is used, a possible user-defined ThreadPool must
   *                                 have exactly one worker thread configured.
   * \param[in] max_requests       The max number of method requests that can be queued. Value '0' indicates unlimited
   *                               queue size.
   * \return Result<ConstructionToken> Result containing construction token from which a Skeleton object can be
   *         constructed.
   * \error ComErrc::kGrantEnforcementError   If the request is refused by IAM.
   * \error ComErrc::kExceedQueueSize         If the given max_requests value exceeds the max_number_of_tasks configured
   *                                          for the threadpool.
   * \error ComErrc::kCommunicationLinkError  If there was a connection failure.
   * \pre         No other service skeleton for the same instance must exist concurrently. After destruction of an
   *              already created skeleton object, the instantiation for the same service instance will be possible.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - Check if the construction token was already created for any of the provided service instances before.
   * - Set the assignement identifier to P-Port Default Thread Pool, if no User-Defined Thread Pool is configured
   *   for the identifier.
   * - Validate if the skeleton can be constructed with the given method call processing mode.
   * - For event driven method call processing mode return an error code, if the method request queue count exceeds the
   *   maximum no of threadpool task count.
   * - Create skeleton backend for each provided instance identifier
   * - Add all the provided service instances to the static list of already created skeleton instances.
   * - Create a construction token with the skeleton backends and return it to the caller.
   *
   * Calls Abort() if:
   * - The construction token was already created for any of the provided instance identifiers.
   * - If the skeleton cannot be constructed with the given method call processing mode.
   * - The skeleton backend factory is not found for any of the provided service instances.
   * - Creation of skeleton backend fails with an error other than kGrantEnforcementError or kCommunicationLinkError.
   * \endinternal
   *
   * \trace SPEC-8053551, SPEC-8053550
   */
  static ConstructionResult Preconstruct(internal::InstanceSpecifierLookupTableEntryContainer&& offered_instances,
                                         ::amsr::socal::internal::ThreadPoolAssignmentIdentifier const& thread_pool_id,
                                         ::ara::com::MethodCallProcessingMode const mode,
                                         std::size_t const max_requests) noexcept {
    ConstructionResult result{::ara::com::ComErrc::kErrorNotOk};
    SkeletonBackendInterfaceUniquePtrCollection binding_implementations{};
    // Perform consistency checks
    CheckServiceSkeletonInstanceAlreadyCreated(offered_instances);
    CheckMethodProcessingModeIsApplicable(mode);
    ::amsr::socal::internal::ThreadPoolAssignmentIdentifier pool_id{thread_pool_id};

    if (mode != ::ara::com::MethodCallProcessingMode::kPoll) {  // COV_SOCAL_CONDITION_SUBJECT_TO_PROCESSING_MODE

      ::amsr::generic::SingletonAccess<::amsr::socal::internal::ThreadPoolsManagerInterface> thread_pool_manager{
          static_cast<::amsr::generic::SingletonAccess<::amsr::socal::internal::ThreadPoolsManagerInterface>>(
              ::amsr::socal::internal::ThreadPoolsManager::getSingletonInstance().GetAccess())};

      // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
      if (!thread_pool_manager->IsThreadPoolAssigned(pool_id)) {  // COV_SOCAL_CONDITION_SUBJECT_TO_PROCESSING_MODE
        pool_id = internal::kAssignmentNamePPortDefaultThreadPool;
      }

      ::amsr::core::Result<void> const processing_mode_validity_result{
          EventModeMethodExecutor::CheckMethodCallProcessingModeValidity(mode, pool_id, thread_pool_manager,
                                                                         max_requests)};
      if (!processing_mode_validity_result.HasValue()) {  // COV_SOCAL_CONDITION_SUBJECT_TO_PROCESSING_MODE
        result.EmplaceError(::ara::com::ComErrc::kExceedQueueSize);
      }
    }

    if (result != ::ara::com::ComErrc::kExceedQueueSize) {  // COV_SOCAL_CONDITION_SUBJECT_TO_PROCESSING_MODE
      CreateBackendsForOfferedInstances(offered_instances, binding_implementations, result);

      if (!binding_implementations.empty()) {
        // Treat a construction token like a already created skeleton instance and update the static list of service
        // instances already in the Preconstruct() API.
        AddCreatedServiceInstances(offered_instances);

        // VCA_SOCAL_EXTERNAL_FUNCTION_CONTRACT
        result.EmplaceValue(std::move(offered_instances), std::move(binding_implementations), pool_id, mode,
                            max_requests);
      }
    }
    return result;
  }

  /*!
   * \brief Loops through all offered instances, creates backends out of them and adds these to the list of binding
   *        implementations
   * \param[in]     offered_instances        Offered service instances to create backends from
   * \param[in]     binding_implementations  Container of all service instance backends that are created out of
   *                                         offered_instances
   * \param[in,out] result                   Result to tell the calling method if CreateBackendsForOfferedInstances
   *                                         was successful
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - For each provided instance identifier in the given offered_instances container:
   *   - Create skeleton backend from the corresponding skeleton backend factory registry.
   *   - If the creation fails for any of the instance identifier,
   *     - If IAM access is denied or connection error has occurred.
   *       - Destroy the previously created skeleton backends and return the error code.
   * \endinternal
   */
  static void CreateBackendsForOfferedInstances(
      internal::InstanceSpecifierLookupTableEntryContainer const& offered_instances,
      SkeletonBackendInterfaceUniquePtrCollection& binding_implementations, ConstructionResult& result) noexcept {
    // VCA_SOCAL_OPERATOR_IN_RANGE_BASED_LOOP
    for (internal::InstanceSpecifierLookupTableEntry const& instance : offered_instances) {
      // VCA_SOCAL_REFERENCE_VARIABLE_IN_RANGE_BASED_LOOP
      ::amsr::socal::internal::ProvidedInstanceIdentifier const instance_id{instance};
      ::amsr::core::Result<SkeletonBackendInterfaceUniquePtr> impl_result{
          skeleton_backend_factory_registry_.GetAccess()->CreateBackend(instance_id)};
      if (impl_result.HasValue()) {
        assert(impl_result.Value() != nullptr);
        binding_implementations.emplace_back(std::move(impl_result).Value());  // VCA_SOCAL_CALLING_STL_APIS
      } else {
        ::amsr::socal::internal::logging::AraComLogger const logger{
            ::amsr::socal::internal::logging::kAraComLoggerContextId,
            ::amsr::socal::internal::logging::kAraComLoggerContextDescription, kServiceShortNamePath.ToString()};
        result.EmplaceError(impl_result.Error());
        binding_implementations.clear();
        if (impl_result.CheckError(::ara::com::ComErrc::kGrantEnforcementError)) {
          logger.LogError(
              [&instance_id](::amsr::socal::internal::logging::LogStream& s) {
                // VCA_SOCAL_UNDEFINED_FUNCTION_WITH_IMPLICIT_THIS_PTR
                ::amsr::core::StringView const instance_id_sv{instance_id.ToString()};
                s << "Failed to create skeleton backend. IAM access denied for the InstanceIdentifier: "
                  << instance_id_sv;
              },
              static_cast<char const*>(__func__), __LINE__);
          break;
        } else if (impl_result.CheckError(::ara::com::ComErrc::kCommunicationLinkError)) {
          // Report connection failure to the user as it is up to the application to retry and clear all other
          // bindings
          logger.LogError(
              [&instance_id](::amsr::socal::internal::logging::LogStream& s) {
                // VCA_SOCAL_UNDEFINED_FUNCTION_WITH_IMPLICIT_THIS_PTR
                ::amsr::core::StringView const instance_id_sv{instance_id.ToString()};
                s << "Communication Link Error: ";
                s << "Failed to create skeleton backend for the InstanceIdentifier: " << instance_id_sv;
              },
              static_cast<char const*>(__func__), __LINE__);
          break;
        } else {
          ::ara::core::ErrorCode const error_code{impl_result.Error()};
          logger.LogFatalAndAbort(
              [&error_code](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
                ::ara::core::StringView const error_message{error_code.Message()};
                // VCA_SOCAL_CALLING_STL_APIS
                s << "Failed to create skeleton backend. Error code: " << error_message;
              },
              static_cast<char const*>(__func__), __LINE__);
        }
      }
    }
  }

  /*!
   * \brief   Check if the service instance which shall be created was already offered.
   * \details The [Service]Skeleton class keeps track of all created skeleton objects via a static list.
   * \param[in] offered_instances  Offered service instances to be checked.
   * \pre         Instance identifiers in offered instances list haven't been used to create skeleton before.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   *
   * \trace SPEC-4980348
   */
  static void CheckServiceSkeletonInstanceAlreadyCreated(
      internal::InstanceSpecifierLookupTableEntryContainer const& offered_instances) noexcept {
    ::amsr::generic::SingletonAccess<internal::InstanceSpecifierLookupTableEntryContainer> service_instance_container{
        created_service_instances_container_.GetAccess()};
    typename internal::InstanceSpecifierLookupTableEntryContainer::const_iterator const begin{
        service_instance_container->cbegin()};
    typename internal::InstanceSpecifierLookupTableEntryContainer::const_iterator const end{
        service_instance_container->cend()};
    // VCA_SOCAL_OPERATOR_IN_RANGE_BASED_LOOP
    for (internal::InstanceSpecifierLookupTableEntry const& offered_instance : offered_instances) {
      // VCA_SOCAL_VALID_LOOKUP_TABLE_ENTRY_FROM_CONTAINER_REFERENCE_ARGUMENT, VCA_SOCAL_CALLING_STL_APIS
      if (std::find(begin, end, offered_instance) != end) {
        ::amsr::core::Abort(
            "Skeleton::CheckServiceSkeletonInstanceAlreadyCreated: Cannot create more than one skeletons using the "
            "same instance identifier.");
      }
    }
  }

  /*!
   * \brief   Add instances to the static list of already created skeleton instances.
   * \details Used during construction / preconstruction of skeleton instances.
   * \param[in] instances  List of skeleton instances to be added.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   *
   * \trace SPEC-4980348
   */
  static void AddCreatedServiceInstances(
      internal::InstanceSpecifierLookupTableEntryContainer const& instances) noexcept {
    // VCA_SOCAL_CALLING_STL_APIS
    created_service_instances_container_.GetAccess()->insert(instances.cbegin(), instances.cend());
  }

  /*!
   * \brief   Remove instances from the static list of already created skeleton instances.
   * \details Used during destruction of skeleton instances and ConstructionTokens.
   * \param[in] instances  List of skeleton instances to be removed.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   *
   * \trace SPEC-4980348
   */
  static void RemoveCreatedServiceInstances(
      internal::InstanceSpecifierLookupTableEntryContainer const& instances) noexcept {
    // PTP-B-Socal-Skeleton_RemoveCreatedServiceInstances
    // VCA_SOCAL_OPERATOR_IN_RANGE_BASED_LOOP
    for (internal::InstanceSpecifierLookupTableEntry const& instance : instances) {
      ::amsr::generic::SingletonAccess<internal::InstanceSpecifierLookupTableEntryContainer> service_instance_container{
          created_service_instances_container_.GetAccess()};
      internal::InstanceSpecifierLookupTableEntryContainer::const_iterator const it{
          // VCA_SOCAL_VALID_LOOKUP_TABLE_ENTRY_FROM_CONTAINER_REFERENCE_ARGUMENT, VCA_SOCAL_CALLING_STL_APIS
          std::find(service_instance_container->cbegin(), service_instance_container->cend(), instance)};
      // The erase() call returns a new iterator which is not required for this implementation. Thus, it is valid to
      // ignore the return value.
      static_cast<void>(service_instance_container->erase(it));  // VCA_SOCAL_CALLING_STL_APIS
    }
    // PTP-E-Socal-Skeleton_RemoveCreatedServiceInstances
  }

  /*!
   * \brief Instantiate the method executor depending on the method processing mode.
   * \param[in] processing_mode    Method call processing mode used by this skeleton instance.
   * \param[in] thread_pool_id     Identifier of the thread pool assigned to this skeleton.
   * \param[in] offered_instances  The container of service instance to be offered.
   * \param[in] queue_size         The max number of method requests that can be queued.
   * \return Unique pointer to concrete instantiated method executor.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - If method processing mode is kPoll.
   *   - Instantiate a PollingModeExecutor.
   * - Otherwise
   *   - Instantiate an EventModeMethodExecutor.
   * - Return the instantiated method executor.
   * \endinternal
   */
  static auto InitializeMethodExecutor(::ara::com::MethodCallProcessingMode const processing_mode,
                                       ::amsr::core::StringView thread_pool_id,
                                       internal::InstanceSpecifierLookupTableEntryContainer const& offered_instances,
                                       std::size_t const queue_size) noexcept
      -> std::unique_ptr<internal::MethodExecutorInterface> {
    std::unique_ptr<internal::MethodExecutorInterface> method_executor;

    if (IsMethodCallProcessingInPollingMode(processing_mode)) {  // COV_SOCAL_CONDITION_SUBJECT_TO_PROCESSING_MODE
      method_executor = std::make_unique<PollingModeMethodExecutor>(queue_size);
    } else {
      // VCA_SOCAL_EXTERNAL_FUNCTION_CONTRACT
      method_executor = std::make_unique<EventModeMethodExecutor>(
          offered_instances, kServiceIdentifier, thread_pool_id,
          static_cast<::amsr::generic::SingletonAccess<::amsr::socal::internal::ThreadPoolsManagerInterface>>(
              ::amsr::socal::internal::ThreadPoolsManager::getSingletonInstance().GetAccess()),
          queue_size);
    }
    return method_executor;
  }  // VCA_SOCAL_CALLING_DESTRUCTOR_OF_STACK_OBJECTS

  /*!
   * \brief Check if polling mode is used for method processing.
   * \param[in] method_processing_mode  Method call processing mode used by this skeleton instance.
   * \return boolean value true if method_processing_mode is 'kPoll', false otherwise.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  static auto IsMethodCallProcessingInPollingMode(
      ::ara::com::MethodCallProcessingMode const method_processing_mode) noexcept -> bool {
    return method_processing_mode ==  // COV_SOCAL_CONDITION_SUBJECT_TO_PROCESSING_MODE
           ::ara::com::MethodCallProcessingMode::kPoll;
  }

  /*!
   * \brief   Singleton access to the Runtime.
   * \details De-initializing Socal is not possible until this object is destroyed.
   */
  ::amsr::generic::SingletonAccess<Runtime> runtime_{GetRuntimeAccess()};

  /*!
   * \brief   Mutex to synchronize sending of OfferService over the binidng against the IsServiceOfferedSync() calls.
   * \details Synchronizes sending of OfferService over the bindings against sending of events within method callbacks.
   */
  std::mutex offer_service_sync_lock_{};

  /*!
   * \brief Mutex to synchronize HandleMethodRequest and StopOfferService to avoid scheduling of new requests while
   *        service is being stopped.
   */
  std::mutex method_request_sync_lock_{};

  /*!
   * \brief Flag to determine whether the incoming method requests from the bindings can be accepted.
   */
  bool accept_method_requests_{false};

  /*!
   * \brief Method call processing mode used by this skeleton instance.
   */
  ::ara::com::MethodCallProcessingMode const processing_mode_;

  /*!
   * \brief   List of binding-specific implementation of a service instance.
   * \details Example: A skeleton object is created using an InstanceSpecifier mapping to instance ID '3' on the SOME/IP
   *          binding and instance ID '5' on IPC binding. After OfferService() is called, this list will contain one
   *          interface pointer to the SOME/IP binding-specific implementation of instance ID '3' and one pointer for
   *          the appropriate IPC binding-specific implementation of instance ID '5'.
   */
  SkeletonBackendInterfaceUniquePtrCollection const binding_implementations_;

  /*!
   * \brief Singleton instance of SkeletonBackendFactoryRegistry.
   */
  // VECTOR NC AutosarC++17_10-A3.3.2: MD_SOCAL_AutosarC++17_10-A3.3.2_StaticStorageDurationOfNonPODType
  static SkeletonBackendFactoryRegistrySingleton skeleton_backend_factory_registry_;

  /*!
   * \brief   Map of all currently created skeletons.
   * \details Once a skeleton is constructed, its instance identifier related information is added to the container and
   *          removed upon calling its destructor. This list is needed to prohibit the creation of a skeleton with the
   *          same instance id twice.
   *
   * \trace SPEC-4980348
   */
  // VECTOR NC AutosarC++17_10-A3.3.2: MD_SOCAL_AutosarC++17_10-A3.3.2_StaticStorageDurationOfNonPODType
  static ::amsr::generic::Singleton<internal::InstanceSpecifierLookupTableEntryContainer>
      created_service_instances_container_;

  /*!
   * \brief Identifier of the thread pool assigned to this skeleton.
   */
  std::string thread_pool_id_;

  /*!
   * \brief Concrete method executor implementation.
   */
  std::unique_ptr<internal::MethodExecutorInterface> method_executor_;
};

/*!
 * \brief Definition of the static kServiceIdentifier rolled out by the compiler for all template instances used in rest
 *        of the code.
 * \tparam ServiceInterface          The corresponding ServiceInterface.
 * \tparam SkeletonBackendInterface  Generic skeleton base type.
 * \tparam Mode                      The runtime processing mode, can be [kPolling, kThreadDriven].
 */
template <typename ServiceInterface, typename SkeletonBackendInterface,
          ::amsr::socal::internal::configuration::RuntimeProcessingMode Mode>
::ara::com::ServiceIdentifierType constexpr Skeleton<ServiceInterface, SkeletonBackendInterface,
                                                     Mode>::kServiceIdentifier;

/*!
 * \brief Definition of the static kServiceShortNamePath rolled out by the compiler for all template instances used in
 *        rest of the code.
 * \tparam ServiceInterface          The corresponding ServiceInterface.
 * \tparam SkeletonBackendInterface  Generic skeleton base type.
 * \tparam Mode                      The runtime processing mode, can be [kPolling, kThreadDriven].
 */
template <typename ServiceInterface, typename SkeletonBackendInterface,
          ::amsr::socal::internal::configuration::RuntimeProcessingMode Mode>
::ara::core::StringView constexpr Skeleton<ServiceInterface, SkeletonBackendInterface, Mode>::kServiceShortNamePath;

/*!
 * \brief Definition for the static singleton of skeleton backend factory registry.
 * \tparam ServiceInterface          The corresponding ServiceInterface.
 * \tparam SkeletonBackendInterface  Generic skeleton base type.
 * \tparam RuntimeSelected           This parameter is used to decouple from a concrete singleton (Runtime). The default
 *                                   template parameter shall be the amsr::socal::internal::Runtime.
 */
// VECTOR NC AutosarC++17_10-A3.3.2: MD_SOCAL_AutosarC++17_10-A3.3.2_StaticStorageDurationOfNonPODType
template <typename ServiceInterface, typename SkeletonBackendInterface,
          amsr::socal::internal::configuration::RuntimeProcessingMode Mode>
typename Skeleton<ServiceInterface, SkeletonBackendInterface, Mode>::SkeletonBackendFactoryRegistrySingleton
    Skeleton<ServiceInterface, SkeletonBackendInterface, Mode>::skeleton_backend_factory_registry_;

/*!
 * \brief Definition of the static created_service_instances_container_.
 * \tparam ServiceInterface          The corresponding ServiceInterface.
 * \tparam SkeletonBackendInterface  Generic skeleton base type.
 * \tparam Mode                      The runtime processing mode, can be [kPolling, kThreadDriven].
 */
// VECTOR NC AutosarC++17_10-A3.3.2: MD_SOCAL_AutosarC++17_10-A3.3.2_StaticStorageDurationOfNonPODType
template <typename ServiceInterface, typename SkeletonBackendInterface,
          ::amsr::socal::internal::configuration::RuntimeProcessingMode Mode>
::amsr::generic::Singleton<internal::InstanceSpecifierLookupTableEntryContainer>
    Skeleton<ServiceInterface, SkeletonBackendInterface, Mode>::created_service_instances_container_;

}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_SKELETON_H_

// clang-format off
/*!
 * \exclusivearea ::amsr::socal::Skeleton::offer_service_sync_lock_
 * Synchronizes sending of OfferService over the bindings against IsServiceOfferedSync calls.
 * i.e. sending of OfferService over the bindings against sending of events within method callbacks.
 *
 * \protects Parallel calls to OfferService() over the bindings against the calls to IsServiceOfferedSync().
 * \usedin ::amsr::socal::Skeleton::OfferService
 * \usedin ::amsr::socal::events::SkeletonEvent::Send
 * \exclude All other methods of a Skeleton object.
 * \length MEDIUM Forwarding the offer service to all the registered bindings or accessing a boolean flag.
 * \endexclusivearea
 */

/*!
 * \exclusivearea ::amsr::socal::Skeleton::method_request_sync_lock_
 * Used to synchronize StopOfferService / OfferService against HandleMethodRequest.
 *
 * \protects ::amsr::socal::Skeleton::accept_method_requests_
 * \usedin ::amsr::socal::Skeleton::OfferService
 * \usedin ::amsr::socal::Skeleton::StopOfferService
 * \usedin ::amsr::socal::Skeleton::HandleMethodRequest
 *
 * \exclude All other methods of a Skeleton object.
 * \length SHORT Either the value of accept_method_requests_ is modified or method task is added to a queue.
 * \endexclusivearea
 */
// clang-format on
// COV_JUSTIFICATION_BEGIN
//
// \ID COV_SOCAL_CONDITION_SUBJECT_TO_PROCESSING_MODE
//  \ACCEPT XX xx xx
//  \ACCEPT XF xf xx
//  \ACCEPT TX tx tf
//  \ACCEPT TF tx tf
//  \REASON The event driven processing mode checks won't cover in polling mode configuration and the polling mode
//          checks won't cover in event driven mode. There is no risk by not testing this cases explicitly in polling
//          or thread driven mode scenario.
//
// \ID COV_SOCAL_UNCHECKED_LAMBDA_IN_POLLING_MODE
//  \ACCEPT XX
//  \REASON This code part cannot be reached when polling runtime processing mode is chosen.
//          There is no risk by not testing this in polling runtime processing mode.
//
// \ID COV_SOCAL_UNCHECKED_LAMBDA_IN_THREADED_MODE
//  \ACCEPT XX
//  \REASON This code part cannot be reached when event driven runtime processing mode is chosen.
//          There is no risk by not testing in event driven runtime processing mode.
//
// COV_JUSTIFICATION_END
