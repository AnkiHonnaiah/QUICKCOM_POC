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
/**        \file  find_service_observers_manager.h
 *        \brief  FindServiceObserversManager has the purpose to manage asynchronous FindService jobs of one
 *                ServiceProxy.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_FIND_SERVICE_OBSERVERS_MANAGER_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_FIND_SERVICE_OBSERVERS_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <list>
#include <mutex>
#include "amsr/generic/singleton_wrapper.h"
#include "amsr/socal/internal/instance_specifier_lookup_table.h"
#include "amsr/socal/internal/service_discovery/proxy_service_discovery_internal_interface.h"
#include "amsr/socal/internal/service_handle_explorer.h"
#include "amsr/socal/internal/types_service_discovery.h"

namespace amsr {
namespace socal {
namespace internal {

/*!
 * \brief FindServiceObserversManager has the purpose to manage asynchronous FindService jobs of one ServiceProxy.
 *
 * \unit Socal::Proxy::ProxyServiceDiscovery::AsynchronousProxyFindService
 */
class FindServiceObserversManager final {  // VCA_SOCAL_COMPILER_GENERATED_DESTRUCTOR
  /*!
   * \brief A list of service handle explorer listeners/observers associated with the proxy find service handles.
   */
  using FindServiceUpdateJobs = std::list<ServiceHandleExplorer>;

  /*!
   * \brief Type alias for ProxyServiceDiscovery.
   */
  using ServiceDiscovery = ::amsr::socal::internal::service_discovery::ProxyServiceDiscoveryInternalInterface;

 public:
  /*!
   * \brief Add an observer to the set of pending observers.
   * \param[in] service_instances                   The service instances which shall be observed (represented as lookup
   *                                                table entry).
   * \param[in] find_service_callback               Callback called upon detecting a matching service.
   * \param[in] service_discovery_singleton_access  SingletonAccess object for the service discovery singleton instance.
   * \param[in] service_instance_update_callback    Callback to be invoked upon service instance updates. The given
   *                                                callback must be valid at least until the lifetime of *this object.
   * \return A FindServiceHandle managed by this object, to identify and stop the correct FindService update job later
   *         on.
   * \pre         -
   * \context     App
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  ServiceHandleExplorer AddObserver(
      InstanceSpecifierLookupTableEntryContainer const& service_instances,
      StartFindServiceCallback const& find_service_callback,
      ::amsr::generic::SingletonAccess<ServiceDiscovery>& service_discovery_singleton_access,
      ServiceHandleExplorer::ServiceInstanceUpdateCallback const& service_instance_update_callback) noexcept;

  /*!
   * \brief Perform observers housekeeping.
   * \pre         -
   * \context     Callback
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  void UpdateObservers() noexcept;

  /*!
   * \brief Remove all observers marked for deletion from the set of active observers.
   * \pre         -
   * \context     Callback
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  void RemoveInactiveObservers() noexcept;

 private:
  /*!
   * \brief The set of active observers.
   */
  FindServiceUpdateJobs active_observers_;

  /*!
   * \brief The set of pending observers.
   */
  FindServiceUpdateJobs pending_observers_;

  /*!
   * \brief The lock to protect the set of pending observers.
   */
  std::mutex pending_observers_lock_;
};

}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_FIND_SERVICE_OBSERVERS_MANAGER_H_

// clang-format off
/*!
 * \exclusivearea ::amsr::socal::internal::FindServiceObserversManager::pending_observers_lock_
 *                Used for protecting the modification of the set of pending observers.
 *
 * \protects ::amsr::socal::internal::FindServiceObserversManager::pending_observers_ is the protected resource of a
 *           FindServiceObserversManager object.
 * \usedin ::amsr::socal::internal::FindServiceObserversManager::AddObserver
 * \usedin ::amsr::socal::internal::FindServiceObserversManager::UpdateObservers
 * \exclude All other methods of a FindServiceObserversManager object.
 * \length SHORT Looping over and updating a list of a pending observers.
 * \endexclusivearea
 */
// clang-format on
