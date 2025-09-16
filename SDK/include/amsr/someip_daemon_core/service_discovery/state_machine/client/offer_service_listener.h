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
/**        \file  offer_service_listener.h
 *        \brief  An interface to be implemented by classes to notify of service offers.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_OFFER_SERVICE_LISTENER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_OFFER_SERVICE_LISTENER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/types/service_address.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace state_machine {
namespace client {

/*!
 * \brief An observer-pattern interface for the notifications of eventgroup subscription state updates.
 */
class OfferServiceListener {
 public:
  /*!
   * \brief Define default constructor.
   * \context Init
   * \steady FALSE
   */
  OfferServiceListener() noexcept = default;
  /*!
   * \brief Define default destructor.
   * \context Shutdown
   * \steady FALSE
   */
  virtual ~OfferServiceListener() noexcept = default;

  OfferServiceListener(OfferServiceListener const &) = delete;
  OfferServiceListener(OfferServiceListener &&) = delete;
  OfferServiceListener &operator=(OfferServiceListener const &) & = delete;
  OfferServiceListener &operator=(OfferServiceListener &&) & = delete;

  /*!
   * \brief     Called when a service has been offered
   *
   * \param[in] instance_id The instance identifier of the service.
   * \param[in] remote_server_address  The IP address/port of the remote server.
   *
   * \pre       -
   * \context   Network
   * \steady    TRUE
   */
  virtual void OnOfferService(
      amsr::someip_protocol::internal::InstanceId const instance_id,
      amsr::someip_daemon_core::configuration::types::ServiceAddress const &remote_server_address) noexcept = 0;

  /*!
   * \brief     Called when a service is no longer offered offered
   *
   * \param[in] instance_id The instance identifier of the service.
   *
   * \pre       -
   * \context   Network
   * \steady    FALSE
   */
  virtual void OnStopOfferService(amsr::someip_protocol::internal::InstanceId const instance_id) noexcept = 0;
};

}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_OFFER_SERVICE_LISTENER_H_
