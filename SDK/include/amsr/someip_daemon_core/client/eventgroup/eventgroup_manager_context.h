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
/**        \file  eventgroup_manager_context.h
 *        \brief  client state machine context interface for eventgroup.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENTGROUP_MANAGER_CONTEXT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENTGROUP_MANAGER_CONTEXT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <cstdint>
#include "amsr/someip_daemon_core/client/eventgroup/eventgroup_state.h"
#include "amsr/someip_daemon_core/connection_manager/connection_state.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {
namespace eventgroup {

/*!
 * \brief An interface for service discovery client eventgroup manager state machine context.
 */
class EventgroupManagerContext {
 public:
  /*!
   * \brief   Default constructor.
   * \steady  FALSE
   */
  EventgroupManagerContext() noexcept = default;

  /*!
   * \brief   Default destructor.
   * \steady  FALSE
   */
  virtual ~EventgroupManagerContext() noexcept = default;

  EventgroupManagerContext(EventgroupManagerContext const &) = delete;
  EventgroupManagerContext(EventgroupManagerContext &&) = delete;
  EventgroupManagerContext &operator=(EventgroupManagerContext const &) & = delete;
  EventgroupManagerContext &operator=(EventgroupManagerContext &&) & = delete;

  /*!
   * \brief Called when a SOME/IP eventgroup has been subscribed.
   *
   * \param[in] eventgroup_id A SOME/IP eventgroup identifier.
   * \context App
   * \pre     -
   * \steady  FALSE
   */
  virtual void OnEventgroupSubscribed(someip_protocol::internal::EventgroupId const eventgroup_id) = 0;

  /*!
   * \brief Get the connection state.
   *
   * \return True if connection is successfully established. False otherwise.
   *
   * \context App, Network
   * \pre -
   * \steady  FALSE
   */
  virtual bool IsConnected() const noexcept = 0;
};

}  // namespace eventgroup
}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENTGROUP_MANAGER_CONTEXT_H_
