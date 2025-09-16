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
/*!        \file  timestamp_config.h
 *        \brief  Timestamnp configuration for ProxyEvent / SamplePtr.
 *
 *        \details Configuration required to template ProxyEvent class.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_EVENTS_TIMESTAMP_CONFIG_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_EVENTS_TIMESTAMP_CONFIG_H_

#include <chrono>

namespace amsr {
namespace socal {
namespace internal {
namespace events {

/*!
 * \brief Parametrize TimeStamp option of an Event.
 * \tparam Enabled  Set this template-parameter to true, and use it as template parameter when creating a ProxyEvent, if
 *                  TimeStamp is enabled for the event. Set this template-parameter to false, and use it as template
 *                  parameter when creating a ProxyEvent, if TimeStamp is disabled for the Event.
 */
template <typename Enabled>
struct TimeStampEnabling {
  enum { IsEnabled = Enabled::active };
};

/*!
 * \brief Parameter-set for one event.
 * \tparam Active  The configured value of Timestamp Enabling (contains info if TimeStampEnabled is set to true /
 *                 false).
 */
template <bool Active>
struct TimeStampActive {
  enum { active = Active };
};

/*!
 * \brief Type alias for enabling timestamps.
 */
using TimeStampEnabled = TimeStampEnabling<TimeStampActive<true>>;
/*!
 * \brief Type alias for disabling timestamps.
 */
using TimeStampDisabled = TimeStampEnabling<TimeStampActive<false>>;

/*!
 * \brief Type alias for time stamp type.
 */
using TimeStamp = std::chrono::system_clock::time_point;

}  // namespace events
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_EVENTS_TIMESTAMP_CONFIG_H_
