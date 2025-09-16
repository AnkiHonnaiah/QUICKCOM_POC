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
/*!        \file   internal/service_discovery_offer_service_observer.h
 *         \brief  Observer (callbacks' receiver) for service discovery related events of a specific
 *                 service instance.
 *         \unit   SomeIpDaemon::CommunicationExtensionPoints::RequiredServiceInstance
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_INTERNAL_SERVICE_DISCOVERY_OFFER_SERVICE_OBSERVER_H_
#define LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_INTERNAL_SERVICE_DISCOVERY_OFFER_SERVICE_OBSERVER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"
#include "amsr/someip_daemon_core/service_discovery/service_discovery_offer_service_observer.h"

namespace amsr {
namespace someip_daemon_lite {
namespace internal {

/*!
 * \brief Observer (callbacks' receiver) for service discovery related events of a specific service instance.
 * \details Receives notifications from service discovery when service instances become available or
 *          unavailable.
 * \vprivate Component Private
 */
class ServiceDiscoveryOfferServiceObserver final
    : public someip_daemon_core::service_discovery::ServiceDiscoveryOfferServiceObserver {
 public:
  /*!
   * \brief Gets called on a service instance offer.
   * \details The implementation is empty because no callback action is needed when a service is offered.
   *
   * \param[in] service_instance_id    The SOME/IP service instance id.
   * \param[in] remote_server_address  The IP address/port of the remote server.
   *
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \threadsafe FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  // VECTOR NC AutosarC++17_10-M0.1.8: MD_SomeIpDaemon_M0.1.8_FunctionHasNoExternalSideEffect
  void OnStartOfferServiceInstance(  // COV_SOMEIPDAEMONLITE_SERVICEDISCOVERYOFFERSERVICEOBSERVER
      amsr::someip_daemon_core::configuration::types::SomeIpServiceInstanceId const &service_instance_id,
      amsr::someip_daemon_core::configuration::types::ServiceAddress const &remote_server_address) noexcept final {
    static_cast<void>(service_instance_id);
    static_cast<void>(remote_server_address);
  }

  /*!
   * \brief Gets called on a stopped offer of a specific service instance.
   * \details The implementation is empty because no callback action is needed when a service is stopped.
   *
   * \param[in] service_instance_id    The SOME/IP service instance id.
   *
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \threadsafe FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  // VECTOR NC AutosarC++17_10-M0.1.8: MD_SomeIpDaemon_M0.1.8_FunctionHasNoExternalSideEffect
  void OnStopOfferServiceInstance(  // COV_SOMEIPDAEMONLITE_SERVICEDISCOVERYOFFERSERVICEOBSERVER
      amsr::someip_daemon_core::configuration::types::SomeIpServiceInstanceId const &service_instance_id) noexcept
      final {
    static_cast<void>(service_instance_id);
  }
};

}  // namespace internal
}  // namespace someip_daemon_lite
}  // namespace amsr

// COV_JUSTIFICATION_BEGIN
// \ID COV_SOMEIPDAEMONLITE_SERVICEDISCOVERYOFFERSERVICEOBSERVER
//   \ACCEPT  XX
//   \REASON  All justified functions have no side effects and hence it is not meaningful to unit-test them.
//            The integration test checks that everything works as expected.
// COV_JUSTIFICATION_END

#endif  // LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_INTERNAL_SERVICE_DISCOVERY_OFFER_SERVICE_OBSERVER_H_
