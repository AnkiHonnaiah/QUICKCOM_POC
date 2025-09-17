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
/**        \file  interface_card_state_observer_interface.h
 *        \brief  A Interface Card State Observer interface
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_NETWORK_INTERFACE_CARD_STATE_OBSERVER_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_NETWORK_INTERFACE_CARD_STATE_OBSERVER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace amsr {
namespace someip_daemon_core {
namespace network {

/*!
 * \brief A Interface Card State Observer interface used to monitor the status of a network interface (up and down).
 */
class InterfaceCardStateObserverInterface {
 public:
  /*!
   * \brief Define default constructor.
   *
   * \steady    FALSE
   */
  InterfaceCardStateObserverInterface() = default;

  /*!
   * \brief Define default destructor.
   *
   * \steady    FALSE
   */
  virtual ~InterfaceCardStateObserverInterface() = default;

  InterfaceCardStateObserverInterface(InterfaceCardStateObserverInterface const&) = delete;
  InterfaceCardStateObserverInterface(InterfaceCardStateObserverInterface&&) = delete;
  InterfaceCardStateObserverInterface& operator=(InterfaceCardStateObserverInterface const&) & = delete;
  InterfaceCardStateObserverInterface& operator=(InterfaceCardStateObserverInterface&&) & = delete;

  /*!
   * \brief Called when network is brought up.
   *
   * \context   Network
   * \reentrant FALSE
   * \steady    FALSE
   * \pre       -
   */
  virtual void OnNetworkUp() noexcept = 0;

  /*!
   * \brief Called when network is brought down.
   *
   * \context Network
   * \steady    FALSE
   * \pre     -
   */
  virtual void OnNetworkDown() noexcept = 0;
};

}  // namespace network
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_NETWORK_INTERFACE_CARD_STATE_OBSERVER_INTERFACE_H_
