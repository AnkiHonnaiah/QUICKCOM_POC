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
/**        \file  socal/include/amsr/socal/r18_03/events/proxy_event_polling_mode.h
 *        \brief  This file contains the partial specialization of the ProxyEvent class.
 *
 *      \details  This specialization is used for the RuntimeProcessingMode kPolling.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_R18_03_EVENTS_PROXY_EVENT_POLLING_MODE_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_R18_03_EVENTS_PROXY_EVENT_POLLING_MODE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <cstddef>
#include <iterator>
#include <memory>
#include <mutex>
#include <string>
#include <type_traits>
#include <utility>

#include "amsr/socal/r18_03/events/proxy_event.h"
#include "amsr/socal/r18_03/events/proxy_event_base.h"
#include "ara/com/types_common.h"

namespace amsr {
namespace socal {
namespace r18_03 {
namespace events {

/*!
 * \brief   Specialized template class for event handling in kPollingMode mode.
 * \details The following APIs are not considered to be threadsafe against each other, or against any other API in this
 *          instance:
 *          - Subscribe(),
 *          - Unsubscribe(),
 *          - GetSubscriptionState()
 *          The following APIs shall not be called concurrently in this instance:
 *          - Update(),
 *          - GetCachedSamples(),
 *          - Cleanup()
 * \tparam EventSampleType         Type of the sample (e.g. uint32, ara::core::Vector<T>, struct X).
 * \tparam TimestampConfiguration  The timestamp configuration. Defaults to TimeStampDisabled.
 *
 * \unit Socal::Proxy::ProxyEvent::ProxyEvent__r18_03_Sync
 */
template <typename EventSampleType, typename TimestampConfiguration>
class ProxyEvent<EventSampleType, TimestampConfiguration,
                 ::amsr::socal::internal::configuration::RuntimeProcessingMode::kPolling>
    : public ::amsr::socal::r18_03::events::ProxyEventBase<EventSampleType, TimestampConfiguration> {
 public:
  /*!
   * \brief Type alias for the base class.
   */
  using Base = ::amsr::socal::r18_03::events::ProxyEventBase<EventSampleType, TimestampConfiguration>;

  /*!
   * \brief Type-alias for the data type of the event.
   * \vpublic
   */
  using SampleType = EventSampleType;

  /*!
   * \brief Type alias for proxy event backend.
   */
  using ProxyEventBackend = ::amsr::socal::internal::events::ProxyEventBackendInterface<SampleType>;

  /*!
   * \brief Type alias for proxy event backend pointer.
   */
  using ProxyEventBackendPtr = std::add_pointer_t<ProxyEventBackend>;

  /*!
   * \brief Creates an unsubscribed proxy event.
   * \param[in] event_backend  Pointer to the event backend. The given pointer must not be null and must be valid until
   *                           the current object's lifetime.
   * \param[in] service_name   The service name for this proxy event.
   * \param[in] event_name     The event name of this proxy event.
   * \param[in] instance_id    The instance identifier to be used with this proxy event.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vprivate    Component Private.
   * \steady FALSE
   * \spec requires event_backend != nullptr; \endspec
   */
  explicit ProxyEvent(ProxyEventBackendPtr event_backend, std::string const& service_name,
                      std::string const& event_name, ::ara::com::InstanceIdentifier const& instance_id) noexcept
      : Base{event_backend, service_name, event_name, instance_id} {
    static_assert(std::is_same<TimestampConfiguration, internal::events::TimeStampDisabled>::value,
                  "Time stamp feature can only be enabled when using R20-11");
  }

  /*!
   * \brief   Unsubscribes from the skeleton counterpart of this proxy-side event.
   * \details Calling Unsubscribe() will invalidate any reference to a SampleContainer that has been acquired via
   *          GetCachedSamples() API before.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous FALSE Subscription state is reported asynchronously.
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - Forward the unsubscription to the base class.
   * \endinternal
   *
   * \trace SPEC-4980373
   */
  void Unsubscribe() noexcept { Base::UnsubscribeInternal(); }

 private:
  // VECTOR NC AutosarC++17_10-M0.1.8: MD_SOCAL_M0.1.8_NoSideEffectInPollingMode
  /*!
   * \brief   Handles a received event notification. Nothing to be done in kPolling Mode.
   * \details -
   * \pre         -
   * \context     Backend
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vprivate    Product Private.
   * \steady TRUE
   * \spec requires true; \endspec
   */
  void HandleEventNotification() noexcept override {
    // Nothing to be done.
  }

  /*!
   * \brief   Updates the subscription state according to the provided state.
   * \details Called upon subscription state update of this event.
   * \pre         -
   * \context     Backend | App during Unsubscription.
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vprivate    Product Private.
   * \steady TRUE
   * \spec requires true; \endspec
   * \internal
   * - If the event is subscribed
   *   - Set subscription state to \p state.
   * \endinternal
   *
   * \trace SPEC-4980117, SPEC-4980378
   */
  void HandleEventSubscriptionStateUpdate(::ara::com::SubscriptionState const) noexcept override {
    // This API is deprecated and will be removed.
  }
};

}  // namespace events
}  // namespace r18_03
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_R18_03_EVENTS_PROXY_EVENT_POLLING_MODE_H_
