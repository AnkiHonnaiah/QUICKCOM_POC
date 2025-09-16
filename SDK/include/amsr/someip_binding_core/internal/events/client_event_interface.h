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
/**        \file  client_event_interface.h
 *        \brief  Client Event Interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_EVENTS_CLIENT_EVENT_INTERFACE_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_EVENTS_CLIENT_EVENT_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/core/result.h"
#include "amsr/someip_binding_core/internal/events/event_notification_interface.h"
#include "amsr/someip_binding_core/internal/events/invisible_sample_cache.h"
#include "amsr/someip_binding_core/internal/events/someip_sample_cache_entry.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {
namespace events {

/*!
 * \brief Interface class for SOME/IP Client Events implementations.
 */
class ClientEventInterface {
 public:
  /*!
   * \brief Define default constructor.
   * \steady      FALSE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ClientEventInterface() = default;

  /*!
   * \brief Define destructor.
   * \steady      FALSE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~ClientEventInterface() noexcept = default;

  ClientEventInterface(ClientEventInterface const&) = delete;
  ClientEventInterface(ClientEventInterface&&) = delete;
  ClientEventInterface& operator=(ClientEventInterface const&) & = delete;
  ClientEventInterface& operator=(ClientEventInterface&&) & = delete;

  /*!
   * \brief       Subscribe to this event by registering a notification handler.
   *
   * \details
   *
   * \param[in]   cache_size  The maximum number of cached events. The binding is responsible for pre-allocating the
   *                          memory for storing at least the cache_size number of deserialized samples.
   * \param[in]   handler     Handler to trigger upon event notification reception.
   *
   * \return      Empty result in case of success or an error code otherwise.
   * \error       ClientError::kRuntimeConnectionTransmissionFailed If SOME/IP Daemon is not connected.
   * \error       ClientError::kRuntimeResourceOutOf If IPC queue from SOME/IP Daemon is limited and full.
   * \error       ClientError::kUnknownError If unknown error returned from someipdaemonclient.
   *
   * \pre         Event was not subscribed.
   *
   * \context     App
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \vprivate
   */
  virtual amsr::core::Result<void> SubscribeEvent(std::size_t const cache_size,
                                                  std::weak_ptr<EventNotificationInterface> handler) noexcept = 0;

  /*!
   * \brief       Unsubscribe from this event to stop receiving notifications.
   *
   * \details
   *
   * \pre         -
   *
   * \context     App
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \vprivate
   */
  virtual void UnsubscribeEvent() noexcept = 0;

  /*!
   * \brief   Moves sample pointers from reactor cache to application cache and provides access to the cache.
   *
   * \details The returned SampleCacheContainer reference shall be used to retrieve and remove processed samples from
   *          the cache.
   *          The user of ReadSamples is not allowed to call ReadSamples while modifying the returned reference.
   *
   *          The sample cache container can have less, equal or more elements after this call.
   *          Less:  There were not as many elements available as requested when calling ReadSamples
   *          Equal: There were at least as many elements available as requested
   *          More:  Not all samples were processed from the previous ReadSamples call and the current call requests
   *                 less samples than the not processed samples from the previous call.
   *
   * \param[in] max_samples The maximum number of elements to be retrieved.
   *
   * \return    Reference to the sample cache container
   *
   * \pre         -
   *
   * \context     App
   *
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   *
   * \spec
   *   requires true;
   * \endspec
   *
   * \vprivate
   */
  virtual ::amsr::someip_binding_core::internal::events::InvisibleSampleCache::SampleCacheContainer& ReadSamples(
      std::size_t const max_samples) noexcept = 0;

  /*!
   * \brief   Getter for the Event Id.
   * \return  Event ID.
   *
   * \context     App
   *
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec
   *   requires true;
   * \endspec
   *
   * \vprivate
   */
  virtual ::amsr::someip_protocol::internal::EventId GetEventId() const noexcept = 0;

  /*!
   * \brief   Getter for the subscription call state.
   * \return  Subscription call state.
   *
   * \context     App
   *
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec
   *   requires true;
   * \endspec
   *
   * \vprivate
   */
  virtual bool GetSubscriptionCallState() const noexcept = 0;
};
}  // namespace events
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_EVENTS_CLIENT_EVENT_INTERFACE_H_
