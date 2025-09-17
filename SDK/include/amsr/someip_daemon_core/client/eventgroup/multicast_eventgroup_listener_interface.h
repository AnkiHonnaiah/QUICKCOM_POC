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
/**        \file  multicast_eventgroup_listener_interface.h
 *        \brief  Interface for multicast eventgroup listening
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_MULTICAST_EVENTGROUP_LISTENER_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_MULTICAST_EVENTGROUP_LISTENER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {
namespace eventgroup {

/*!
 * \brief An interface for service discovery eventgroup multicast listener
 */
class MulticastEventgroupListenerInterface {
 public:
  /*!
   * \brief   Define default constructor.
   * \steady  FALSE
   */
  MulticastEventgroupListenerInterface() noexcept = default;

  /*!
   * \brief   Define default destructor.
   * \steady  FALSE
   */
  virtual ~MulticastEventgroupListenerInterface() noexcept = default;

  MulticastEventgroupListenerInterface(MulticastEventgroupListenerInterface const &) = delete;
  MulticastEventgroupListenerInterface(MulticastEventgroupListenerInterface &&) = delete;
  MulticastEventgroupListenerInterface &operator=(MulticastEventgroupListenerInterface const &) & = delete;
  MulticastEventgroupListenerInterface &operator=(MulticastEventgroupListenerInterface &&) & = delete;

  /*!
   * \brief Starts listening for multicast UDP events of a given eventgroup.
   *
   * \param[in] multicast_address  A multicast IP address to which events of the given eventgroup will be sent.
   * \param[in] multicast_port     A multicast UDP port number.
   * \param[in] sender_address     The unicast IP address of the sender of the event.
   * \param[in] sender_port        The unicast UDP port of the sender of the event.
   * \context Network
   * \steady  FALSE
   *
   * \return true if we started listening to the given multicast address and false otherwise.
   */
  virtual bool StartListenForMulticastEventgroup(someip_daemon_core::IpAddress const &multicast_address,
                                                 someip_protocol::internal::Port const multicast_port,
                                                 someip_daemon_core::IpAddress const &sender_address,
                                                 someip_protocol::internal::Port const sender_port) noexcept = 0;

  /*!
   * \brief Stops listening for multicast UDP events of a given eventgroup.
   * \context Network
   * \steady  FALSE
   */
  virtual void StopListenForMulticastEventgroup() noexcept = 0;
};

}  // namespace eventgroup
}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_MULTICAST_EVENTGROUP_LISTENER_INTERFACE_H_
