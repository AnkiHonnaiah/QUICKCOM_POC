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
/**        \file  socal/r20_11/events/proxy_event.h
 *        \brief  Proxy Event
 *
 *      \details  ProxyEvent templated class represents an event of a proxy. For proxy instance, ProxyEvent shall be
 *                instantiated for all events defined in this Proxy. Each Proxy/Event type combination represents a
 *                different ProxyEvent class.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_R20_11_EVENTS_PROXY_EVENT_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_R20_11_EVENTS_PROXY_EVENT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/socal/internal/configuration/configuration.h"
#include "amsr/socal/internal/events/timestamp_config.h"

namespace amsr {
namespace socal {
namespace r20_11 {
namespace events {

/*!
 * \brief Generic template class for event handling (Subscribe, Unsubscribe, Update, ...) by a proxy.
 * \tparam EventSampleType         Type of the sample (e.g. uint32, ara::core::VectorVector<T>, struct X).
 * \tparam TimestampConfiguration  The timestamp configuration. Defaults to TimeStampDisabled.
 * \tparam Mode                    The runtime processing mode. Defaults to kThreadDriven.
 *
 * \unit Socal::Proxy::ProxyEvent::ProxyEvent__r20_11_Async
 */
template <typename EventSampleType,
          typename TimestampConfiguration = ::amsr::socal::internal::events::TimeStampDisabled,
          ::amsr::socal::internal::configuration::RuntimeProcessingMode Mode =
              ::amsr::socal::internal::configuration::RuntimeProcessingMode::kThreadDriven>
class ProxyEvent;  // VCA_SOCAL_FREEING_STATICALLY_ALLOCATED_MEMORY

/*!
 * \brief Specialized template class for event handling in kPollingMode mode.
 * \tparam EventSampleType         Type of the sample (e.g. uint32, ara::core::Vector<T>, struct X).
 * \tparam TimestampConfiguration  The timestamp configuration. Defaults to TimeStampDisabled.
 *
 * \unit Socal::Proxy::ProxyEvent::ProxyEvent__r20_11_Sync
 */
template <typename EventSampleType, typename TimestampConfiguration>
class ProxyEvent<EventSampleType, TimestampConfiguration,
                 ::amsr::socal::internal::configuration::RuntimeProcessingMode::kPolling>;

/*!
 * \brief Specialized template class for event handling in kThreadDriven mode.
 * \tparam EventSampleType         Type of the sample (e.g. uint32, ara::core::Vector<T>, struct X).
 * \tparam TimestampConfiguration  The timestamp configuration. Defaults to TimeStampDisabled.
 *
 * \unit Socal::Proxy::ProxyEvent::ProxyEvent__r20_11_Async
 */
template <typename EventSampleType, typename TimestampConfiguration>
class ProxyEvent<EventSampleType, TimestampConfiguration,
                 ::amsr::socal::internal::configuration::RuntimeProcessingMode::kThreadDriven>;

}  // namespace events
}  // namespace r20_11
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_R20_11_EVENTS_PROXY_EVENT_H_
