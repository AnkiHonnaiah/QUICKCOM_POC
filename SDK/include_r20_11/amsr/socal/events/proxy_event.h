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
/**        \file  socal/include_r20_11/amsr/socal/events/proxy_event.h
 *        \brief  Proxy Event header file.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_R20_11_AMSR_SOCAL_EVENTS_PROXY_EVENT_H_
#define LIB_SOCAL_INCLUDE_R20_11_AMSR_SOCAL_EVENTS_PROXY_EVENT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/socal/internal/configuration/configuration.h"
#include "amsr/socal/internal/events/timestamp_config.h"
#include "amsr/socal/r20_11/events/proxy_event_polling_mode.h"
#include "amsr/socal/r20_11/events/proxy_event_thread_driven.h"

namespace amsr {
namespace socal {
namespace events {

/*!
 * \brief Type alias for enabling timestamps.
 */
using TimeStampEnabled = ::amsr::socal::internal::events::TimeStampEnabled;

/*!
 * \brief Type alias for disabling timestamps.
 */
using TimeStampDisabled = ::amsr::socal::internal::events::TimeStampDisabled;

/*!
 * \brief Type alias for the R20-11 ProxyEvent implementation.
 * \tparam SampleType              Type of the sample (e.g. uint32, ara::core::Vector<T>, struct X).
 * \tparam Mode                    Runtime processing mode value, can be [kPolling, kThreadDriven].
 * \tparam TimestampConfiguration  Type of the time stamp configuration (e.g. TimeStampDisabled, TimeStampEnabled).
 * \vpublic
 */
template <typename EventSampleType, ::amsr::socal::internal::configuration::RuntimeProcessingMode Mode,
          typename TimestampConfiguration = ::amsr::socal::internal::events::TimeStampDisabled>
using ProxyEvent = ::amsr::socal::r20_11::events::ProxyEvent<EventSampleType, TimestampConfiguration, Mode>;

}  // namespace events
}  // namespace socal
}  // namespace amsr
#endif  // LIB_SOCAL_INCLUDE_R20_11_AMSR_SOCAL_EVENTS_PROXY_EVENT_H_
