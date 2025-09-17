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
/*!       \file   someip_binding_core/internal/someip_binding_core_interface.h
 *        \brief  Interface for "SomeIpBindingCore" class.
 *        \unit   SomeIpBinding::SomeIpBindingCore::SomeIpBindingCore
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SOMEIP_BINDING_CORE_INTERFACE_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SOMEIP_BINDING_CORE_INTERFACE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>

#include "amsr/core/result.h"
#include "amsr/someip_binding_core/internal/local_server.h"
#include "amsr/someip_binding_core/internal/local_server_manager.h"
#include "amsr/someip_binding_core/internal/local_server_manager_interface.h"
#include "amsr/someip_binding_core/internal/methods/skeleton_method_handler_interface.h"
#include "amsr/someip_binding_core/internal/required_service_instance.h"
#include "amsr/someip_binding_core/internal/types.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {

/*!
 * \brief Interface class for SOME/IP binding core.
 */
class SomeIpBindingCoreInterface {
 public:
  /*!
   * \brief Set of method IDs for which a request handler has to be registered.
   */
  using RequiredSkeletonMethodHandlerSet = methods::SkeletonMethodHandlerInterface::RequiredSkeletonMethodHandlerSet;

  /*!
   * \brief Constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  SomeIpBindingCoreInterface() noexcept = default;

  SomeIpBindingCoreInterface(SomeIpBindingCoreInterface const&) = delete;
  SomeIpBindingCoreInterface(SomeIpBindingCoreInterface&&) = delete;
  SomeIpBindingCoreInterface& operator=(SomeIpBindingCoreInterface const&) = delete;
  SomeIpBindingCoreInterface& operator=(SomeIpBindingCoreInterface&&) = delete;

  /*!
   * \brief Destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~SomeIpBindingCoreInterface() noexcept = default;

  /*!
   * \brief       Start all dynamic actions of the SOME/IP binding.
   * \param[in]   connection_retrial_enabled  Flag to enable connection retrial after a failed connection attempt.
   * \pre         Initialize() has been called.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \error kRuntimeConnectionTimeout The connection timed out.
   * \error any_error_code The connection failed.
   * \return Connection result without any value.
   */
  virtual ::amsr::core::Result<void> Start(bool connection_retrial_enabled) noexcept = 0;

  /*!
   * \brief       Emplace a local server implementation with the given ID
   *              into the LocalServerManager.
   *
   * \param[in]   service_instance_id   The service instance id.
   * \param[in]   required_handler_set  Set of skeleton method request handlers for which request handlers have to be
   *                                    registered.
   *
   * \pre         -
   *
   * \context     Init
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual void EmplaceLocalServer(ProvidedServiceInstanceId const& service_instance_id,
                                  RequiredSkeletonMethodHandlerSet const& required_handler_set) noexcept = 0;

  /*!
   * \brief       Create a server instance with the given ID.
   *
   * \param[in]   service_instance_id   The service instance id.
   *
   * \return      The created server instance.
   *
   * \error       ServerError::kRuntimeConnectionTimeout if no response is received from SOME/IP Daemon.
   * \error       ServerError::kSystemConfigurationNotAvailable if this service instance is not configured.
   * \error       ServerError::kSystemPrivilegesNoAccess if access is denied for this service.
   *              configured.
   * \error       ServerError::kRuntimeConnectionTransmissionFailed If SOME/IP Daemon is not connected.
   * \error       ServerError::kRuntimeResourceOutOf If IPC queue to SOME/IP Daemon is limited and full.
   * \error       ServerError::kUnknownError If unknown error returned from someipdaemonclient.
   *
   * \pre         -
   *
   * \context     App (Skeleton ctor)
   * \threadsafe  TRUE (RequestLocalServer can be called from different threads in parallel)
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual amsr::core::Result<LocalServer> RequestLocalServer(
      ProvidedServiceInstanceId const& service_instance_id) noexcept = 0;

  /*!
   * \brief        Creates a required service instance for a specific service id
   * \param[in]    service_instance_id The required service instance id
   *
   * \return       amsr::core::Result with the created required service instance or an error
   *
   * \error       ClientError::kSystemConfigurationNotAvailable If the given instance id is not configured.
   * \error       SomeIpDaemonClientErrc::kNotConnected If the Communication with SOME/IP Daemon failed.
   *
   * \pre          -
   *
   * \context       Init
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual amsr::core::Result<RequiredServiceInstance> RequestRequiredServiceInstance(
      RequiredServiceInstanceId const& service_instance_id) noexcept = 0;

  /*!
   * \brief       Emplace a required service instance implementation with the given ID
   *              into the RequiredServiceInstanceManager.
   *
   * \param[in]   service_instance_id   The service instance id.
   * \param[in]   service_information   Service interface information used in the service instance.
   *
   * \pre         The given required service instance ID must not have been previously registered.
   *
   * \context     Init
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  virtual void EmplaceRequiredServiceInstance(RequiredServiceInstanceId const& service_instance_id,
                                              ServiceInformation&& service_information) noexcept = 0;

  /*!
   * \brief        Check if a required service instance is configured in this instance.
   * \param[in]    required_service_instance_id The required service instance id.
   *
   * \return       true if the required service instance is configured.
   *
   * \pre          -
   *
   * \context       Init
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   */
  virtual auto IsRequiredServiceInstanceIdConfigured(
      RequiredServiceInstanceId const& required_service_instance_id) noexcept -> bool = 0;

  /*!
   * \brief        Check if a provided service instance is configured in this instance.
   * \param[in]    provided_service_instance_id The provided service instance id.
   *
   * \return       true if the provided service instance is configured.
   *
   * \pre          -
   *
   * \context       Init
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   */
  virtual auto IsProvidedServiceInstanceIdConfigured(
      ProvidedServiceInstanceId const& provided_service_instance_id) noexcept -> bool = 0;

  /*!
   * \brief       Start Service Discovery for all registered required service instances.
   *
   * \pre         SomeIpDaemon must be connected to the application.
   *
   * \context     App (If RestartServiceDiscovery is used)
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  virtual void StartServiceDiscovery() noexcept = 0;

  /*!
   * \brief       Stop Service Discovery for all registered required service instances.
   *
   * \pre         SomeIpDaemon must be connected to the application.
   *
   * \context     Shutdown, App (If RestartServiceDiscovery is used)
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  virtual void StopServiceDiscovery() const noexcept = 0;

  /*!
   * \brief       Get whether the Daemon connection has succeeded. Is not reset if the connection is lost.
   *
   * \return      True if the Daemon connection has succeeded once.
   *
   * \context     Init
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual std::atomic_bool const& SomeIpDaemonConnectionSucceeded() const noexcept = 0;
};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_SOMEIP_BINDING_CORE_INTERFACE_H_
