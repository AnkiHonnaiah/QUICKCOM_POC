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
/**        \file
 *        \brief  internal signal handling types
 *        \unit   osabstraction::Signal_Linux
 *        \unit   osabstraction::Signal_PikeOS
 *        \unit   osabstraction::Signal_QNX
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_SIGNAL_INTERNAL_SIGNAL_NOTIFICATION_ENTRY_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_SIGNAL_INTERNAL_SIGNAL_NOTIFICATION_ENTRY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>
// VECTOR Disable AutosarC++17_10-M18.7.1: MD_OSA_M18.7.1_SignalHandling
#include "amsr/signal/types.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace signal {
namespace internal {

/*!
 * \brief   SignalManager signal notification registrations entry type
 * \details Implements an entry of the SignalNotificationRegistrations container.
 */
struct SignalNotificationRegistrationEntry final {
  /*!
   * \brief           Construct a SignalNotificationRegistrationEntry object.
   * \param[in]       sig_callback          User notification callback.
   * \param[in]       sig_notification_type Signal type of notification.
   * \context         InitPhase
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \vprivate        Vector product internal API
   */
  // VECTOR NL VectorC++-V11.0.3: MD_OSA_V11.0.3_SignalNotificationRegistrationEntry
  explicit SignalNotificationRegistrationEntry(SignalCallback&& sig_callback,
                                               SignalNotificationType sig_notification_type)
      : callback{std::move(sig_callback)}, notification_type{sig_notification_type} {};

  /*!
   * \brief           Deleted copy constructor.
   */
  // VECTOR NL VectorC++-V11.0.3: MD_OSA_V11.0.3_SignalNotificationRegistrationEntry
  SignalNotificationRegistrationEntry(SignalNotificationRegistrationEntry const&) = delete;

  /*!
   * \brief           Default move constructor.
   */
  // VECTOR NL VectorC++-V11.0.3: MD_OSA_V11.0.3_SignalNotificationRegistrationEntry
  SignalNotificationRegistrationEntry(SignalNotificationRegistrationEntry&&) noexcept = default;

  /*!
   * \brief           Deleted assignment operator.
   */
  // VECTOR NL VectorC++-V11.0.3: MD_OSA_V11.0.3_SignalNotificationRegistrationEntry
  SignalNotificationRegistrationEntry& operator=(SignalNotificationRegistrationEntry const&) = delete;

  /*!
   * \brief           Default move assignment operator.
   */
  // VECTOR NL VectorC++-V11.0.3: MD_OSA_V11.0.3_SignalNotificationRegistrationEntry
  SignalNotificationRegistrationEntry& operator=(SignalNotificationRegistrationEntry&&) & noexcept = default;

  /*!
   * \brief           Destructor.
   */
  // VECTOR NL VectorC++-V11.0.3: MD_OSA_V11.0.3_SignalNotificationRegistrationEntry
  ~SignalNotificationRegistrationEntry() noexcept = default;  // VCA_OSA_MOLE_1298

  /*!
   * \brief           User callback.
   */
  SignalCallback callback;

  /*!
   * \brief           Notification type.
   */
  SignalNotificationType notification_type;
};
// VECTOR Enable AutosarC++17_10-M18.7.1: MD_OSA_M18.7.1_SignalHandling

}  // namespace internal
}  // namespace signal
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_SIGNAL_INTERNAL_SIGNAL_NOTIFICATION_ENTRY_H_
