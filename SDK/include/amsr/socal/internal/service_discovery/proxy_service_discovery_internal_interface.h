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
/**        \file  proxy_service_discovery_internal_interface.h
 *        \brief  Interface for interacting with Proxy service discovery within Socal.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_SERVICE_DISCOVERY_PROXY_SERVICE_DISCOVERY_INTERNAL_INTERFACE_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_SERVICE_DISCOVERY_PROXY_SERVICE_DISCOVERY_INTERNAL_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/socal/internal/internal_instance_identifier.h"
#include "amsr/socal/internal/service_discovery/proxy_service_discovery_listener.h"
#include "ara/core/vector.h"

namespace amsr {
namespace socal {
namespace internal {
namespace service_discovery {

/*!
 * \brief Interface for interacting with Proxy service discovery within Socal.
 *
 * \unit Socal::ServiceDiscovery::ProxyServiceDiscovery
 */
class ProxyServiceDiscoveryInternalInterface {
 public:
  /*!
   * \brief Type alias for Service discovery listener.
   */
  using SdListener = ProxyServiceDiscoveryListener;

  /*!
   * \brief Type alias for the container that stores the list of offered services.
   */
  using OfferedServiceInstanceContainer = ::ara::core::Vector<ProvidedInstanceIdentifier>;

  /*!
   * \brief Constructs the ProxyServiceDiscoveryInternalInterface.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  ProxyServiceDiscoveryInternalInterface() noexcept = default;

  ProxyServiceDiscoveryInternalInterface(ProxyServiceDiscoveryInternalInterface const&) = delete;
  ProxyServiceDiscoveryInternalInterface(ProxyServiceDiscoveryInternalInterface&&) = delete;
  ProxyServiceDiscoveryInternalInterface& operator=(ProxyServiceDiscoveryInternalInterface const&) = delete;
  ProxyServiceDiscoveryInternalInterface& operator=(ProxyServiceDiscoveryInternalInterface&&) = delete;

  /*!
   * \brief   Registers a requester for a specific service.
   * \details Once a requester is registered, it will receive all service updates related to the given provided
   *          instance id, including an initial snapshot of already offered services.
   * \param[in] req_id    The identifier of the required service instance.
   * \param[in] prov_id   The identifier of the offered service instance to be monitored.
   * \param[in] listener  Pointer of the callback handler. The pointer must not be nullptr and must be valid until
   *                      RemoveRequester is called.
   * \pre         Same listener was not already registered for the provided instance id.
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  virtual void AddRequester(RequiredInstanceIdentifier const& req_id, ProvidedInstanceIdentifier const& prov_id,
                            SdListener* listener) noexcept = 0;

  /*!
   * \brief De-register a requester for a specific service.
   * \param[in] prov_id   Provided Instance Identifier of the monitored service.
   * \param[in] listener  Pointer of the callback handler.
   * \pre         AddRequester() has been called for id and listener.
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \spec requires listener != nullptr; \endspec
   * \steady FALSE
   */
  virtual void RemoveRequester(ProvidedInstanceIdentifier const& prov_id, SdListener* listener) noexcept = 0;

  /*!
   * \brief   Register a listener for a specific service.
   * \details Once a listener is registered, it will receive all service updates related to the given instance id.
   * \param[in] req_instance_id  Required Instance Identifier of the service to be monitored.
   * \param[in] listener         Pointer of the callback handler. Must be valid until StopListen is being called. The
   *                             pointer must not be nullptr. Currently the only listener is the ServiceHandleExplorer.
   * \pre         Same listener was not already registered for the given instance id.
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  virtual void Listen(RequiredInstanceIdentifier const& req_instance_id, SdListener* listener) noexcept = 0;

  /*!
   * \brief De-register a listener for a specific service.
   * \param[in] req_id    Required Instance Identifier of the monitored service.
   * \param[in] listener  Pointer of the callback handler.
   * \pre         Listen() has been called for id and listener.
   * \context     Callback
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \spec requires listener != nullptr; \endspec
   * \steady FALSE
   */
  virtual void StopListen(RequiredInstanceIdentifier const& req_id, SdListener* listener) noexcept = 0;

  /*!
   * \brief Get the Provided Instance ID of the OfferedServiceInstances.
   * \param[in] required_instance_id  Required Instance Identifier of the service.
   * \return Container of ProvidedInstanceID's.
   * \context     App | Callback.
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  virtual OfferedServiceInstanceContainer GetOfferedServiceInstances(
      RequiredInstanceIdentifier const& required_instance_id) const noexcept = 0;

 protected:
  /*!
   * \brief Destroys the ProxyServiceDiscoveryInternalInterface.
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  ~ProxyServiceDiscoveryInternalInterface() noexcept = default;
};

}  // namespace service_discovery
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_SERVICE_DISCOVERY_PROXY_SERVICE_DISCOVERY_INTERNAL_INTERFACE_H_
