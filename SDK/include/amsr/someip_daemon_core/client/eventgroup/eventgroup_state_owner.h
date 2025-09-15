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
/**        \file  eventgroup_state_owner.h
 *        \brief  A service discovery client eventgroup state owner
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENTGROUP_STATE_OWNER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENTGROUP_STATE_OWNER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/client/eventgroup/eventgroup_state.h"
#include "amsr/someip_daemon_core/client/eventgroup/eventgroup_state_pool.h"
#include "vac/statemachine/state_owner.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {
namespace eventgroup {

/*!
 * \brief A Service Discovery Client eventgroup state owner.
 */
class EventgroupStateOwner final : public vac::statemachine::StateOwner<EventgroupState> {
 public:
  /*!
   * \brief Constructor of EventgroupStateOwner.
   *
   * \param[in,out] state_pool A state pool.
   * \param[in,out] context A state context.
   * \context App
   * \pre     -
   * \steady  FALSE
   */
  EventgroupStateOwner(EventgroupStatePool& state_pool, EventgroupStateContext& context) noexcept
      : vac::statemachine::StateOwner<EventgroupState>::StateOwner{state_pool, EventgroupStateHandle::kNotSubscribed},
        context_{context} {}

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief   Destructor of ServiceDiscoveryEventgroupClientStateOwner.
   * \steady  FALSE
   */
  ~EventgroupStateOwner() noexcept override = default;

  EventgroupStateOwner() = delete;
  EventgroupStateOwner(EventgroupStateOwner const&) = delete;
  EventgroupStateOwner(EventgroupStateOwner&&) = delete;
  EventgroupStateOwner& operator=(EventgroupStateOwner const&) & = delete;
  EventgroupStateOwner& operator=(EventgroupStateOwner&&) & = delete;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief   Returns the context for states.
   *
   * \return  The state context.
   * \context App
   * \steady  FALSE
   * \pre     -
   */
  EventgroupStateContext& GetContext() override { return context_; }

 private:
  /*!
   * \brief A state context.
   */
  EventgroupStateContext& context_;
};

}  // namespace eventgroup
}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENTGROUP_STATE_OWNER_H_
