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
 *        \brief  Interface for the required service instance listener.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_SERVICE_DISCOVERY_REQUIRED_SERVICE_INSTANCE_LISTENER_INTERFACE_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_SERVICE_DISCOVERY_REQUIRED_SERVICE_INSTANCE_LISTENER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/shared_ptr.h"

#include "ipc_binding_core/internal/provided_service_instance_id.h"
#include "ipc_binding_core/internal/required_service_instance_id.h"
#include "ipc_binding_core/internal/service_discovery/service_instance_listener_interface.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace service_discovery {
/*!
 * \brief Interface for the RequiredServiceInstanceListener.
 *
 * \unit IpcBinding::IpcBinding::RequiredServiceInstanceListener
 */
class RequiredServiceInstanceListenerInterface {
 public:
  /*!
   * \brief Type alias for RequiredServiceInstanceId.
   */
  using RequiredServiceInstanceId = ::amsr::ipc_binding_core::internal::RequiredServiceInstanceId;

  /*!
   * \brief Type alias for ProvidedServiceInstanceId.
   */
  using ProvidedServiceInstanceId = ::amsr::ipc_binding_core::internal::ProvidedServiceInstanceId;

  /*!
   * \brief Type alias for ProvidedServiceInstanceListener.
   */
  using ProvidedServiceInstanceListener =
      ::amsr::ipc_binding_core::internal::service_discovery::ServiceInstanceListenerInterface;

  /*!
   * \brief Default constructor.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  RequiredServiceInstanceListenerInterface() = default;

  /*!
   * \brief Default destructor.
   *
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual ~RequiredServiceInstanceListenerInterface() = default;

  RequiredServiceInstanceListenerInterface(RequiredServiceInstanceListenerInterface const&) noexcept = delete;
  auto operator=(RequiredServiceInstanceListenerInterface const&) noexcept
      -> RequiredServiceInstanceListenerInterface& = delete;
  RequiredServiceInstanceListenerInterface(RequiredServiceInstanceListenerInterface&&) noexcept = delete;
  auto operator=(RequiredServiceInstanceListenerInterface&&) noexcept
      -> RequiredServiceInstanceListenerInterface& = delete;

  /*!
   * \brief Register a service instance listener to receive updates about a specific provided service instance.
   *
   * \param[in]  provided_service_instance_id        The provided service instance id to listen for.
   * \param[out] provided_service_instance_listener  The service instance listener.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual void ListenProvidedServiceInstance(
      ProvidedServiceInstanceId const& provided_service_instance_id,
      amsr::WeakPtr<ProvidedServiceInstanceListener> provided_service_instance_listener) = 0;

  /*!
   * \brief Deregister a service instance listener for a provided service instance.
   *
   * \param[in] provided_service_instance_id  The provided service instance id.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual void StopListenProvidedServiceInstance(ProvidedServiceInstanceId const& provided_service_instance_id) = 0;
};
}  // namespace service_discovery
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_SERVICE_DISCOVERY_REQUIRED_SERVICE_INSTANCE_LISTENER_INTERFACE_H_
