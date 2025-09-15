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
/*!        \file  amsr/someip_binding_core/internal/connection_action.h
 *        \brief  Action to attempt a connection to the SomeIpDaemon.
 *        \unit   SomeIpBinding::SomeIpBindingCore::ConnectionAction
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_CONNECTION_ACTION_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_CONNECTION_ACTION_H_

#include <atomic>
#include <chrono>
#include <functional>
#include <memory>

#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "amsr/someip_binding_core/internal/someip_binding_core_interface.h"
#include "amsr/steady_timer/timer_interface.h"
#include "amsr/steady_timer/timer_manager_interface.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {

/*!
 * \brief Connection action that attempts the connection to the daemon and starts the service discovery logic on
 * success.
 */
class ConnectionAction final {
 public:
  /*!
   * \brief Constructor of ConnectionAction.
   * \param[in] someip_binding_core         Weak pointer to the SomeIpBindingCore used to connect to the SomeIpDaemon.
   * \param[in] timer_manager               Reference to timer manager to schedule the connection retrial task in.
   * \param[in] connection_retrial_enabled  Flag to enable connection retrial after a failed connection attempt.
   * \param[in] connection_retrial_ms       Connection retrial interval in milliseconds.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   */
  explicit ConnectionAction(std::weak_ptr<SomeIpBindingCoreInterface> someip_binding_core,
                            ::amsr::steady_timer::TimerManagerInterface* timer_manager, bool connection_retrial_enabled,
                            std::chrono::milliseconds connection_retrial_ms) noexcept;

  /*!
   * \brief Default destructor.
   */
  // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
  ~ConnectionAction() noexcept = default;

  ConnectionAction() noexcept = delete;
  ConnectionAction(ConnectionAction const& other) noexcept = delete;
  ConnectionAction& operator=(ConnectionAction const& other) & noexcept = delete;

  /*!
   * \brief Default move constructor.
   * \param other Instance to be moved.
   */
  ConnectionAction(ConnectionAction&& other) noexcept = default;

  /*!
   * \brief Default move assignment.
   * \param other Instance to be moved.
   * \return Reference to the move assigned instance.
   */
  ConnectionAction& operator=(ConnectionAction&& other) & noexcept = default;

  /*!
   * \brief       Try to connect to the SomeIpDaemon.
   * \pre         SomeIpBindingCore and TimerManager have not been released.
   * \context     Init, Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \error kRuntimeConnectionTimeout The connection timed out.
   * \error any_error_code The connection failed.
   * \internal steady FALSE \endinternal
   * \return Connection result without any value.
   */
  ::amsr::core::Result<void> TryConnect() noexcept;

 private:
  /*!
   * \brief SomeIpDaemon connection retrial feature enable flag.
   */
  bool connection_retrial_enabled_;

  /*!
   * \brief SomeIpDaemon connection retrial interval in milliseconds.
   */
  std::chrono::milliseconds connection_retrial_ms_;

  /*!
   * \brief Weak pointer to the SomeIpBindingCore. The reason for the weak_ptr is that there
   * might be pending connection actions are called from the reactor thread and need to check
   * if the core is still valid.
   */
  std::weak_ptr<SomeIpBindingCoreInterface> someip_binding_core_;

  /*!
   * \brief Flag indicating whether a retrial timer is currently pending.
   */
  std::atomic_bool retrial_pending_{false};

  /*!
   * \brief Logger.
   */
  ::amsr::someip_binding_core::internal::logging::AraComLogger const logger_;

  /*!
   * \brief Unique pointer to steady timer interface.
   */
  std::unique_ptr<amsr::steady_timer::TimerInterface> timer_;

  /*!
   * \brief SomeIpDaemon connection succeeded flag.
   */
  bool connection_succeeded_{false};
};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_CONNECTION_ACTION_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_SOMEIPBINDING_CONNECTIONACTION
//   \ACCEPT  XX
//   \REASON  To be able to test the TimerManager callback in SomeIpBinding would be a big effort. The Callback is
//   already tested in libosabstraction.
// COV_JUSTIFICATION_END
