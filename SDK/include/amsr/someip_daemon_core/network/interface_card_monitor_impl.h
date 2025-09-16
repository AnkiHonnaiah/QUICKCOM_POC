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
/**        \file  interface_card_monitor_impl.h
 *        \brief  A Interface Card Monitor implementation to notify observers about network interface state changes.
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_NETWORK_INTERFACE_CARD_MONITOR_IMPL_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_NETWORK_INTERFACE_CARD_MONITOR_IMPL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/network/interface_card_interface.h"
#include "amsr/someip_daemon_core/network/interface_card_state_observer_interface.h"
#include "ara/core/string.h"
#include "ara/core/vector.h"

namespace amsr {
namespace someip_daemon_core {
namespace network {

/*!
 * \brief   A Interface Card Monitor implementation to notify observers about network interface state changes.
 * \details This class is responsible for forwarding status changes to the observers and polling the current status
 *          upon new observer registration to provide the current network state.
 */
class InterfaceCardMonitorImpl final {
 private:
  /*!
   * \brief Container of Interface Card state observers.
   */
  using InterfaceCardStateObserverContainer = ara::core::Vector<InterfaceCardStateObserverInterface*>;

 public:
  /*!
   * \brief Constructor
   *
   * \param[in] interface_card    The Interface card.
   * \param[in] interface_name    The Interface name.
   *
   * \context   Init
   * \pre       -
   * \reentrant FALSE
   * \steady    FALSE
   */
  InterfaceCardMonitorImpl(InterfaceCardInterface& interface_card, ara::core::String const interface_name) noexcept;

  /*!
   * \brief destructor.
   *
   * \context   Shutdown
   * \pre       -
   * \reentrant FALSE
   * \steady    FALSE
   */
  ~InterfaceCardMonitorImpl() noexcept = default;

  InterfaceCardMonitorImpl(InterfaceCardMonitorImpl const&) = delete;
  InterfaceCardMonitorImpl(InterfaceCardMonitorImpl&&) = delete;
  InterfaceCardMonitorImpl& operator=(InterfaceCardMonitorImpl const&) & = delete;
  InterfaceCardMonitorImpl& operator=(InterfaceCardMonitorImpl&&) & = delete;

  /*!
   * \brief Register the interface card state observer and notify them in case the network interface is already up.
   *
   * \param[in] observer The Network state observer.
   * \context   Init, App
   * \reentrant FALSE
   * \steady    FALSE
   */
  void RegisterInterfaceCardStateObserver(InterfaceCardStateObserverInterface* observer) noexcept;

  /*!
   * \brief Called when network is brought up.
   *
   * \context   Network
   * \reentrant FALSE
   * \steady    FALSE
   * \pre       -
   * \trace SPEC-4981664
   */
  void OnNetworkUp() noexcept;

  /*!
   * \brief Called when network is brought down.
   *
   * \context   Network
   * \reentrant FALSE
   * \steady    FALSE
   * \pre     -
   * \trace SPEC-4981664
   */
  void OnNetworkDown() noexcept;

 private:
  /*!
   * \brief A reference to the Interface Card.
   */
  InterfaceCardInterface& interface_card_;
  /*!
   * \brief The container for the Network State Observer.
   */
  InterfaceCardStateObserverContainer observers_{};
  /*!
   * \brief The logger.
   */
  someip_daemon_core::logging::AraComLogger logger_;
};

}  // namespace network
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_NETWORK_INTERFACE_CARD_MONITOR_IMPL_H_
