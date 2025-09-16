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
/**        \file  service_discovery_offer_service_observer.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_SERVICE_DISCOVERY_OFFER_SERVICE_OBSERVER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_SERVICE_DISCOVERY_OFFER_SERVICE_OBSERVER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdlib>
#include "amsr/someip_daemon_core/configuration/types/service_address.h"
#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {

/*!
 * \brief This interface provides a pure virtual method to inform a concrete observer on service offers and
 * stopped offers.
 */
class ServiceDiscoveryOfferServiceObserver {
 public:
  /*!
   * \brief Define default constructor.
   * \context App
   * \steady FALSE
   */
  ServiceDiscoveryOfferServiceObserver() noexcept = default;
  /*!
   * \brief Define default destructor.
   * \context Shutdown, Timer
   * \steady FALSE
   */
  virtual ~ServiceDiscoveryOfferServiceObserver() noexcept = default;

  ServiceDiscoveryOfferServiceObserver(ServiceDiscoveryOfferServiceObserver const &) = delete;
  ServiceDiscoveryOfferServiceObserver(ServiceDiscoveryOfferServiceObserver &&) = delete;
  ServiceDiscoveryOfferServiceObserver &operator=(ServiceDiscoveryOfferServiceObserver const &) & = delete;
  ServiceDiscoveryOfferServiceObserver &operator=(ServiceDiscoveryOfferServiceObserver &&) & = delete;

  /*!
   * \brief Gets called on a service instance offer. Pure virtual method shall be
   * implemented by a concrete observer.
   *
   * \param[in] service_instance_id    The SOME/IP service instance id.
   * \param[in] remote_server_address  The IP address/port of the remote server.
   *
   * \context Network
   * \steady FALSE
   */
  virtual void OnStartOfferServiceInstance(
      configuration::types::SomeIpServiceInstanceId const &service_instance_id,
      amsr::someip_daemon_core::configuration::types::ServiceAddress const &remote_server_address) noexcept = 0;
  /*!
   * \brief Gets called on a stopped offer of a specific service instance. Pure virtual method shall be
   * implemented by a concrete observer.
   *
   * \param[in] service_instance_id  The SOME/IP service instance id.
   *
   * \context Network
   * \steady FALSE
   */
  virtual void OnStopOfferServiceInstance(
      configuration::types::SomeIpServiceInstanceId const &service_instance_id) noexcept = 0;
};

}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_SERVICE_DISCOVERY_OFFER_SERVICE_OBSERVER_H_
