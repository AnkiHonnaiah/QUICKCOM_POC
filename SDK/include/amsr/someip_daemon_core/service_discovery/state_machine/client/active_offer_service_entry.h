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
/**        \file  active_offer_service_entry.h
 *        \brief  A service discovery client active offer
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_ACTIVE_OFFER_SERVICE_ENTRY_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_ACTIVE_OFFER_SERVICE_ENTRY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/net/ip/address.h"
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_offer_service_entry.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace state_machine {
namespace client {

/*!
 * \brief Represents a valid Service Discovery offer with all corresponding data describing it.
 */
struct ActiveOfferServiceEntry {
  /*!
   * \brief The IP address of service discovery that sent the offer.
   */
  someip_daemon_core::IpAddress sd_address;
  /*!
   * \brief The UDP port of service discovery that sent the offer.
   */
  amsr::net::ip::Port sd_port{configuration::Configuration::kInvalidPort};
  /*!
   * \brief Currently active service offer.
   */
  message::entries::OfferServiceEntry offer_entry;
  /*!
   * \brief Indication of whether the active offer was received via unicast or multicast.
   */
  bool is_multicast{false};
};

}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_ACTIVE_OFFER_SERVICE_ENTRY_H_
