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
/**        \file  thread_driven_find_service_handler.h
 *        \brief  This file contains the definition for ThreadDrivenFindServiceHandler class.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_THREAD_DRIVEN_FIND_SERVICE_HANDLER_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_THREAD_DRIVEN_FIND_SERVICE_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <type_traits>
#include "amsr/generic/singleton_wrapper.h"
#include "amsr/socal/handle_type.h"
#include "amsr/socal/internal/find_service_handler_functor.h"
#include "amsr/socal/internal/find_service_observers_manager.h"
#include "amsr/socal/internal/service_discovery/proxy_service_discovery_internal_interface.h"
#include "amsr/socal/internal/service_handle_explorer.h"
#include "amsr/socal/internal/thread_pool_task.h"
#include "amsr/socal/internal/thread_pools_manager.h"
#include "amsr/socal/internal/types_service_discovery.h"
#include "ara/com/find_service_handle.h"

namespace amsr {
namespace socal {
namespace internal {

/*!
 * \brief Find service handler for 'kThreadDriven' runtime processing mode.
 * \tparam ServiceHandleType  The service handle type.
 *
 * \unit Socal::Proxy::ProxyServiceDiscovery::AsynchronousProxyFindService
 */
template <typename ServiceHandleType>
class ThreadDrivenFindServiceHandler final {
  static_assert(std::is_base_of<::amsr::socal::HandleType, ServiceHandleType>::value,
                "The given ServiceHandleType type is not valid!");
  /*!
   * \brief Type alias for ProxyServiceDiscovery.
   */
  using ServiceDiscovery = ::amsr::socal::internal::service_discovery::ProxyServiceDiscoveryInternalInterface;

 public:
  /*!
   * \brief Start an asynchronous FindService notification about service updates.
   * \param[in] find_service_callback               Gets called upon detection of a matching service.
   * \param[in] service_instances                   A container of resolved instance specifier lookup table entries.
   * \param[in] service_discovery_singleton_access  SingletonAccess object for the proxy service discovery.
   * \return FindServiceHandle which is needed to stop the service availability monitoring and related firing of the
   *         given callback.
   * \pre -
   * \steady FALSE
   * \internal
   * - Add new observer to monitor the service instance.
   * - Schedule an FindServiceHandlerTask for the added observer.
   * - Return the FindServiceHandle that was returned by the call.
   * \endinternal
   */
  static ::ara::com::FindServiceHandle StartFindService(
      StartFindServiceCallback const find_service_callback,
      InstanceSpecifierLookupTableEntryContainer const& service_instances,
      ::amsr::generic::SingletonAccess<ServiceDiscovery> service_discovery_singleton_access) noexcept {
    ::amsr::generic::SingletonAccess<FindServiceObserversManager> observer_manager{
        findservice_observers_manager_.GetAccess()};
    // PTP-B-Socal-Proxy_StartFindService
    ServiceHandleExplorer const observer{
        // VCA_SOCAL_PASSING_FUNCTION_ARG_TO_ANOTHER_FUNCTION
        observer_manager->AddObserver(service_instances, find_service_callback, service_discovery_singleton_access,
                                      ThreadDrivenFindServiceHandler<ServiceHandleType>::OnServiceInstanceUpdate)};

    ScheduleFindServiceHandlerTask(observer);
    // PTP-E-Socal-Proxy_StartFindService

    return observer.GetHandle();
  }

  /*!
   * \brief To stop receiving further notifications the ServiceProxy class shall provide a StopFindService method.
   * \param[in] handle  The FindServiceHandle returned by the FindService method with handler registration has to be
   *                    provided as input parameter.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - Deactivate the handle.
   * \endinternal
   */
  static void StopFindService(::ara::com::FindServiceHandle const& handle) noexcept { handle.Deactivate(); }

  /*!
   * \brief   Adds an FindServiceHandlerTask to the default thread pool.
   * \details This function is called as soon as the remote server comes up or goes down.
   * \param[in] observer  The observer that should be triggered.
   * \pre         -
   * \context     Backend
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady FALSE
   * \spec requires observer != nullptr; \endspec
   * \internal
   * - Add a FindServiceHandlerTask for the given observer.
   * \endinternal
   */
  static void OnServiceInstanceUpdate(ServiceHandleExplorer const& observer) noexcept {
    ScheduleFindServiceHandlerTask(observer);
  }

  /*!
   * \brief Creates the singleton static instances.
   * \pre         This function must only be called once.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   * \internal
   * - Create the singleton instance of findservice_observers_manager_.
   * \endinternal
   */
  static void CreateSingletons() noexcept { findservice_observers_manager_.Create(); }

  /*!
   * \brief Destroys the singleton static instances.
   * \pre         This function must only be called once.
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   * \internal
   * - Destroy the singleton instance of findservice_observers_manager_.
   * \endinternal
   */
  static void DestroySingletons() noexcept { findservice_observers_manager_.Destroy(); }

 private:
  /*!
   * \brief   Adds an FindServiceHandlerTask to the default thread pool to trigger the FindService handler in the
   *          callback context.
   * \details This function is called upon registration of find service handler for retrieving the initial snapshot and
   *          whenever the service discovery reports if an instance has been offered or stopped.
   * \param[in] observer  - The observer that should be triggered.
   * \pre         -
   * \context     App | Backend.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \spec requires true; \endspec
   * \steady FALSE
   * \internal
   * - Add FindServiceHandlerFunctor to the R-Port Default Thread Pool.
   * \endinternal
   */
  static void ScheduleFindServiceHandlerTask(ServiceHandleExplorer const& observer) noexcept {
    ::amsr::generic::SingletonAccess<::amsr::socal::internal::ThreadPoolsManager> thread_pools_manager_instance{
        ::amsr::socal::internal::ThreadPoolsManager::getSingletonInstance().GetAccess()};

    ::amsr::generic::SingletonAccess<FindServiceObserversManager> observer_manager{
        findservice_observers_manager_.GetAccess()};

    // The return value of EnqueueTask() is discarded by design. There is no retry handling or notification to the
    // application when the task queue is full. See also TechnicalReference_AraCom, section "Thread Pools".
    // VCA_SOCAL_DEREFERENCING_SINGLETON_ACCESS
    static_cast<void>(thread_pools_manager_instance->EnqueueTask(
        internal::kAssignmentNameRPortDefaultThreadPool,
        // Key must not be nullptr, so we pass the pointer to the observer manager's singleton object as the dummy key.
        // For FindServiceHandlerFunctor, we never call RemoveTasks, so we expect no side effects here.
        // VCA_SOCAL_UNDEFINED_FUNCTION_WITH_IMPLICIT_THIS_PTR
        ThreadPoolTask{observer_manager.operator->(),
                       // VCA_SOCAL_UNDEFINED_FUNCTION_WITH_REFERENCE_ARGUMENTS
                       ThreadPoolTask::Callable{FindServiceHandlerFunctor{observer, observer_manager}}}));
  }

  /*!
   * \brief   Manages the incoming service updates (offers and stopped offers) after StartFindService.
   * \details This is a static object, because a StartFindService call is not bound to a concrete ServiceProxy object
   *          but static.
   */
  // VECTOR NL AutosarC++17_10-A3.3.2: MD_SOCAL_AutosarC++17_10-A3.3.2_StaticStorageDurationOfNonPODType
  static ::amsr::generic::Singleton<FindServiceObserversManager> findservice_observers_manager_;
};

/*!
 * \brief StartFindService observer list for a certain service interface.
 * \tparam ServiceHandleType  The service handle type.
 */
// VECTOR NC AutosarC++17_10-A3.3.2: MD_SOCAL_AutosarC++17_10-A3.3.2_StaticStorageDurationOfNonPODType
template <typename ServiceHandleType>
::amsr::generic::Singleton<FindServiceObserversManager>
    ThreadDrivenFindServiceHandler<ServiceHandleType>::findservice_observers_manager_;

}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_THREAD_DRIVEN_FIND_SERVICE_HANDLER_H_
