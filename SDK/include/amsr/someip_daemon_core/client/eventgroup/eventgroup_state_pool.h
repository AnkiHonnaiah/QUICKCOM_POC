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
/**        \file  eventgroup_state_pool.h
 *        \brief  Client eventgroup state pool
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENTGROUP_STATE_POOL_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENTGROUP_STATE_POOL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/client/eventgroup/eventgroup_state.h"
#include "amsr/someip_daemon_core/client/eventgroup/eventgroup_state_not_subscribed.h"
#include "amsr/someip_daemon_core/client/eventgroup/eventgroup_state_subscribed.h"
#include "amsr/someip_daemon_core/client/eventgroup/eventgroup_state_subscription_pending.h"
#include "amsr/someip_daemon_core/client/eventgroup/eventgroup_state_subscription_renewal.h"
#include "vac/statemachine/state_pool.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {
namespace eventgroup {

/*!
 * \brief A Service Discovery Client eventgroup state pool.
 */
class EventgroupStatePool final : public vac::statemachine::StatePool<EventgroupState> {
 public:
  /*!
   * \brief Constructor of EventgroupStatePool
   * \param[in] eventgroup_id EventgroupID
   * \context Init
   * \pre     -
   * \steady  FALSE
   */
  explicit EventgroupStatePool(someip_protocol::internal::EventgroupId const eventgroup_id) noexcept;

  /*!
   * \brief   Default destructor of EventgroupStatePool
   * \steady  FALSE
   */
  // VECTOR NL AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  ~EventgroupStatePool() noexcept override = default;

  EventgroupStatePool() = delete;
  EventgroupStatePool(EventgroupStatePool const&) = delete;
  EventgroupStatePool(EventgroupStatePool&&) = delete;
  EventgroupStatePool& operator=(EventgroupStatePool const&) & = delete;
  EventgroupStatePool& operator=(EventgroupStatePool&&) & = delete;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Returns the instance of a state identified by the given state handle.
   *
   * \param[in] state_handle A state handle.
   * \return An instance of a state.
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  EventgroupState* GetState(EventgroupStateHandle const state_handle) override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Indicates whether there is a state identified by the given handle.
   *
   * \param[in] state_handle A state handle.
   * \return true if the state is valid and false otherwise.
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  bool IsValid(EventgroupStateHandle const state_handle) const override;

 private:
  /*!
   * \brief An instance of state 'NOT_SUBSCRIBED'.
   */
  EventgroupStateNotSubscribed state_not_subscribed_;
  /*!
   * \brief An instance of state 'SUBSCRIPTION_PENDING'.
   */
  EventgroupStateSubscriptionPending state_subscription_pending_;
  /*!
   * \brief An instance of state 'SUBSCRIBED'.
   */
  EventgroupStateSubscribed state_subscribed_;
  /*!
   * \brief An instance of state 'SUBSCRIPTION_RENEWAL'.
   */
  EventgroupStateSubscriptionRenewal state_subscription_renewal_;
};

}  // namespace eventgroup
}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENTGROUP_STATE_POOL_H_
