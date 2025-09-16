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
/**        \file  required_service_instance_interface.h
 *        \brief  Required Service Instance Interface.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REQUIRED_SERVICE_INSTANCE_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REQUIRED_SERVICE_INSTANCE_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/client/local_client_interface.h"
#include "amsr/someip_daemon_core/configuration/types/service_address.h"
#include "amsr/someip_daemon_core/service_discovery/service_discovery_offer_service_observer.h"
#include "amsr/someip_daemon_core/service_discovery/state_machine/client/offer_service_listener.h"
#include "amsr/unique_ptr.h"
#include "ara/core/map.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {
/*!
 * \brief Interface for Required Service Instances
 */
class RequiredServiceInstanceInterface : public service_discovery::state_machine::client::OfferServiceListener {
 public:
  /*!
   * \brief Alias for "ServiceDiscoveryOfferServiceObserver" raw pointer.
   */
  using ServiceObserverPtr = std::add_pointer<service_discovery::ServiceDiscoveryOfferServiceObserver>::type;

  /*!
   * \brief A SOME/IP service instance/address map.
   */
  using ServiceInstanceAddressMap = ara::core::Map<someip_protocol::internal::InstanceId,
                                                   amsr::someip_daemon_core::configuration::types::ServiceAddress>;

  /*!
   * \brief Default constructor.
   * \context Init
   * \steady FALSE
   */
  RequiredServiceInstanceInterface() noexcept = default;

  /*!
   * \brief Default destructor
   * \context Shutdown
   * \steady FALSE
   */
  ~RequiredServiceInstanceInterface() noexcept override = default;

  RequiredServiceInstanceInterface(RequiredServiceInstanceInterface const &) = delete;
  RequiredServiceInstanceInterface(RequiredServiceInstanceInterface &&) = delete;
  RequiredServiceInstanceInterface &operator=(RequiredServiceInstanceInterface const &) & = delete;
  RequiredServiceInstanceInterface &operator=(RequiredServiceInstanceInterface &&) & = delete;

  /*!
   * \brief    Get a container(map) with the offered services.
   * \context  App
   * \pre      -
   * \steady   FALSE
   * \return   A container with the offered services
   */
  virtual ServiceInstanceAddressMap const &GetOfferedServices() const noexcept = 0;

  /*!
   * \brief Request the service
   *
   * \param[in] requestor The service observer
   *
   * \context   App
   * \pre       -
   * \steady    FALSE
   */
  virtual void StartServiceDiscovery(ServiceObserverPtr const requestor) noexcept = 0;

  /*!
   * \brief Release the service
   *
   * \param[in] requestor The service observer
   * \context   App
   * \pre       The service must have been requested at least once
   *
   * \steady    FALSE
   */
  virtual void StopServiceDiscovery(ServiceObserverPtr const requestor) noexcept = 0;

  /*!
   * \brief Request the service.
   * \details Request the service from the remote server
   *
   * \param[in] requested_instance_id  The specific instance id requested by the application.
   *                                   It must not be kInstanceIdAll
   *
   * \return The newly allocated SOME/IP client identifier if the operation is successful and error otherwise.
   * \error SomeIpDaemonErrc::error_not_ok If no remote server is found.
   * \context   App
   * \steady    FALSE
   */
  virtual ara::core::Result<amsr::UniquePtr<client::LocalClientInterface>> RequestService(
      someip_protocol::internal::InstanceId const requested_instance_id) noexcept = 0;
};

}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_REQUIRED_SERVICE_INSTANCE_INTERFACE_H_
