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
/**        \file  eventgroup_resubscription_listener.h
 *        \brief  A listener to notify when resubscription timer expires.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENTGROUP_RESUBSCRIPTION_LISTENER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENTGROUP_RESUBSCRIPTION_LISTENER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace amsr {
namespace someip_daemon_core {
namespace client {
namespace eventgroup {

/*!
 * \brief A listener to notify when a resubscription timer expires.
 */
class EventgroupResubscriptionListener {
 public:
  /*!
   * \brief Define default constructor.
   * \steady FALSE
   */
  EventgroupResubscriptionListener() noexcept = default;

  /*!
   * \brief Define default destructor.
   * \steady FALSE
   */
  virtual ~EventgroupResubscriptionListener() noexcept = default;

  EventgroupResubscriptionListener(EventgroupResubscriptionListener const &) = delete;
  EventgroupResubscriptionListener(EventgroupResubscriptionListener &&) = delete;
  EventgroupResubscriptionListener &operator=(EventgroupResubscriptionListener const &) & = delete;
  EventgroupResubscriptionListener &operator=(EventgroupResubscriptionListener &&) & = delete;

  /*!
   * \brief   Gets called when eventgroup subscription timer expires. Pure virtual method shall be implemented by
   *          a concrete listener.
   * \context Timer
   * \pre     -
   * \steady  FALSE
   */
  virtual void OnTimerExpired() noexcept = 0;
};

}  // namespace eventgroup
}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENTGROUP_RESUBSCRIPTION_LISTENER_H_
