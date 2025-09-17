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
/**        \file  service_handle_explorer.h
 *        \brief  Defining wrapper class for FindServiceHandle.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_SERVICE_HANDLE_EXPLORER_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_SERVICE_HANDLE_EXPLORER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/generic/singleton_wrapper.h"
#include "amsr/socal/internal/logging/ara_com_logger.h"
#include "amsr/socal/internal/service_discovery/proxy_service_discovery_internal_interface.h"
#include "amsr/socal/internal/service_discovery/proxy_service_discovery_listener.h"
#include "amsr/socal/internal/types_service_discovery.h"
#include "ara/com/find_service_handle.h"
#include "vac/language/function_ref.h"

namespace amsr {
namespace socal {
namespace internal {

/*!
 * \brief Wrapper class for FindServiceHandle to be triggered by the Service Discovery.
 *
 * \unit Socal::Proxy::ProxyServiceDiscovery::AsynchronousProxyFindService
 */
// VCA_SOCAL_CALLING_STL_APIS, VCA_SOCAL_PASSING_ARGUMENT_WITH_INVALID_SIZE,
// VCA_SOCAL_FREEING_STATICALLY_ALLOCATED_MEMORY
class ServiceHandleExplorer final : public ::amsr::socal::internal::service_discovery::ProxyServiceDiscoveryListener {
  /*!
   * \brief Alias for service discovery singleton access type.
   */
  using ServiceDiscoverySingletonAccessType = ::amsr::generic::SingletonAccess<
      ::amsr::socal::internal::service_discovery::ProxyServiceDiscoveryInternalInterface>;

 public:
  /*!
   * \brief Type of the callback to be invoked for every service instance update.
   */
  using ServiceInstanceUpdateCallback = ::vac::language::FunctionRef<void(ServiceHandleExplorer const&)>;

  /*!
   * \brief Constructs a ServiceHandleExplorer object.
   * \param[in] service_instances                   The service instances (represented as lookup table entry).
   * \param[in] service_discovery_singleton_access  SingletonAccess for the service discovery singleton instance.
   * \param[in] find_service_callback               Callback called upon detecting a matching service.
   * \param[in] service_instance_update_callback    Callback to be invoked upon service instance updates. The given
   *                                                callback must be valid at least until the lifetime of *this object.
   * \pre -
   * \steady FALSE
   * \spec requires find_service_callback != nullptr; \endspec
   */
  ServiceHandleExplorer(InstanceSpecifierLookupTableEntryContainer const& service_instances,
                        ServiceDiscoverySingletonAccessType const& service_discovery_singleton_access,
                        StartFindServiceCallback const& find_service_callback,
                        ServiceInstanceUpdateCallback const& service_instance_update_callback) noexcept;

  /*!
   * \brief Executes a FindService request and the associated callback (if it is active).
   * \pre        -
   * \context    Callback
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  void ExploreFoundServices() const noexcept;

  /*!
   * \brief Registers this ServiceHandlerExplorer instance as a listener to the ProxyServiceDiscovery.
   * \pre        -
   * \context    Callback
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  void RegisterAsServiceDiscoveryListener() noexcept;

  /*!
   * \brief Deregisters this ServiceHandlerExplorer instance as a listener from the ProxyServiceDiscovery.
   * \pre        -
   * \context    Callback
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  void DeRegisterAsServiceDiscoveryListener() noexcept;

  /*!
   * \brief Obtain a reference of the associated FindServiceHandle.
   * \return The associated FindServiceHandle.
   * \pre     -
   * \context App | Callback.
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ::ara::com::FindServiceHandle const& GetHandle() const noexcept;

  /*!
   * \brief Find out if the handle is active.
   * \return true if active, false otherwise
   * \pre     -
   * \context ANY
   * \steady FALSE
   * \spec requires true; \endspec
   */
  bool IsActive() const noexcept;

 private:
  /*!
   * \brief   Callback from ProxyServiceDiscoveryListener used by ProxyServiceDiscovery when a service is offered.
   * \details This callback is called once a service is offered by the proxy-side service discovery.
   * \pre         -
   * \context     Backend
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  void OnServiceOffered() noexcept final;

  /*!
   * \brief   Callback from ProxyServiceDiscoveryListener used by ProxyServiceDiscovery when a service offer is stopped.
   * \details This callback is called once a service is stopped being offered by the proxy-side service discovery.
   * \pre         -
   * \context     Backend
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  void OnServiceStopped() noexcept final;

  /*!
   * \brief Logger for ServiceHandleExplorer
   */
  ::amsr::socal::internal::logging::AraComLogger logger_{
      ::amsr::socal::internal::logging::kAraComLoggerContextId,
      ::amsr::socal::internal::logging::kAraComLoggerContextDescription, "ServiceHandleExplorer"_sv};

  /*!
   * \brief The activation state of the handle.
   */
  std::shared_ptr<::ara::com::FindServiceHandle::State> const state_{
      // VCA_SOCAL_CALLING_STL_APIS
      std::make_shared<::ara::com::FindServiceHandle::State>()};
  /*!
   * \brief The underlying FindServiceHandle.
   */
  ::ara::com::FindServiceHandle handle_;
  /*!
   * \brief Reference to the service discovery singleton access type.
   */
  ServiceDiscoverySingletonAccessType service_discovery_singleton_access_;
  /*!
   * \brief The callable that will perform the actual FindService call.
   */
  StartFindServiceCallback find_service_callback_;
  /*!
   * \brief The callback that needs to be invoked on every service instance update.
   */
  ServiceInstanceUpdateCallback const service_instance_update_callback_;
};

}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_SERVICE_HANDLE_EXPLORER_H_
