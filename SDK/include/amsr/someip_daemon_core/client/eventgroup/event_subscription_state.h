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
/**        \file  event_subscription_state.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENT_SUBSCRIPTION_STATE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENT_SUBSCRIPTION_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include "ara/core/string.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {
namespace eventgroup {

/*!
 * \brief Event subscription states.
 */
enum class EventSubscriptionState : std::uint8_t {
  kSubscribed = 0,          /*! 'SUBSCRIBED' state */
  kNotSubscribed = 1,       /*! 'NOT_SUBSCRIBED' state */
  kSubscriptionPending = 2, /*! 'SUBSCRIPTION_PENDING' state */
};

/*!
 * \brief Stops offering a SOME/IP service instance.
 *
 * \param[in] state Event subscription state
 *
 * \return the string representation of the state
 *
 * \pre         -
 * \context     App, Network
 * \reentrant   FALSE
 * \synchronous TRUE
 * \steady      FALSE
 */
ara::core::String EventSubscriptionStateToString(EventSubscriptionState const& state);

}  // namespace eventgroup
}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENT_SUBSCRIPTION_STATE_H_
