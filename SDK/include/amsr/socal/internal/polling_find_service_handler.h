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
/**        \file  polling_find_service_handler.h
 *        \brief  This file contains the definition for PollingFindServiceHandler class.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_POLLING_FIND_SERVICE_HANDLER_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_POLLING_FIND_SERVICE_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/generic/singleton_wrapper.h"
#include "amsr/socal/internal/instance_specifier_lookup_table.h"
#include "amsr/socal/internal/service_discovery/proxy_service_discovery_internal_interface.h"
#include "ara/com/types_common.h"
#include "ara/core/vector.h"

namespace amsr {
namespace socal {
namespace internal {
/*!
 * \brief Find service handler for polling found services.
 * \tparam ServiceHandleType  The corresponding ServiceInterface HandleType.
 *
 * \unit Socal::Proxy::ProxyServiceDiscovery::SynchronousProxyFindService
 */
template <typename ServiceHandleType>
class PollingFindServiceHandler final {
  /*!
   * \brief Type alias for ProxyServiceDiscovery.
   */
  using ServiceDiscovery = ::amsr::socal::internal::service_discovery::ProxyServiceDiscoveryInternalInterface;

 public:
  // VECTOR NC AutosarC++17_10-A15.4.2:MD_SOCAL_AutosarC++17_10-A15.4.2_STL_exceptions
  // VECTOR NC AutosarC++17_10-A15.5.3:MD_SOCAL_AutosarC++17_10-A15.5.3_STL_exceptions
  /*!
   * \brief Returns the available service handles.
   * \param[in] service_instances                   Container of searched service instances (InstanceSpecifier lookup
   *                                                table entries).
   * \param[in] service_discovery_singleton_access  SingletonAccess object for the proxy service discovery.
   * \return Found service instances.
   * \pre         -
   * \context     App | Callback.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \internal
   * - For each instance id in the given service instance container.
   *   - Create a find service handle if the service is offered.
   *
   * Calls "ara::core::Abort()" if:
   * - ServiceDiscovery is not registered.
   * \endinternal
   */
  static ::ara::com::ServiceHandleContainer<ServiceHandleType> FindService(
      InstanceSpecifierLookupTableEntryContainer const& service_instances,
      ::amsr::generic::SingletonAccess<ServiceDiscovery> service_discovery_singleton_access) noexcept {
    // PTP-B-Socal-PollingFindServiceHandler_FindService
    ::ara::com::ServiceHandleContainer<ServiceHandleType> handles{};

    // VCA_SOCAL_CALLING_STL_APIS
    for (internal::InstanceSpecifierLookupTableEntry const& service_instance : service_instances) {
      // VCA_SOCAL_ACCESSING_TEMPORARY_REFERENCE_OR_ITERATOR, VCA_SOCAL_UNDEFINED_FUNCTION_WITH_IMPLICIT_THIS_PTR
      RequiredInstanceIdentifier const req_instance_id{service_instance};
      ::ara::core::Vector<ProvidedInstanceIdentifier> const offered_instance_ids{
          // VCA_SOCAL_PROXY_SD_SINGLETON_PTR, VCA_SOCAL_EXTERNAL_FUNCTION_CONTRACT
          service_discovery_singleton_access->GetOfferedServiceInstances(req_instance_id)};
      for (ProvidedInstanceIdentifier const& offered_instance_id : offered_instance_ids) {
        // VCA_SOCAL_CALLING_STL_APIS
        handles.emplace_back(req_instance_id, offered_instance_id);
      }
    }  // VCA_SOCAL_CALLING_STL_APIS
    // PTP-E-Socal-PollingFindServiceHandler_FindService

    return handles;
  }
};

}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_POLLING_FIND_SERVICE_HANDLER_H_
