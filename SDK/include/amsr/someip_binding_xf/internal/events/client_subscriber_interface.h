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
/*!        \file  someip_binding_xf/internal/events/client_subscriber_interface.h
 *        \brief  Client subscriber interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_CLIENT_SUBSCRIBER_INTERFACE_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_CLIENT_SUBSCRIBER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/someip_binding_xf/internal/events/proxy_event_xf_interface.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace events {

/*!
 * \brief Interface class for handling SOME/IP client subscription to events.
 * \tparam SampleType  Type of a single event sample.
 */
template <typename SampleType>
class ClientSubscriberInterface {
 public:
  /*!
   * \brief Type Alias for proxy event backend interface.
   */
  using ProxyEventBackendInterface = ProxyEventXfInterface<SampleType>;

  /*!
   * \brief Type alias for shortening.
   */
  using EventSubscriberInterface = typename ProxyEventBackendInterface::EventSubscriberInterface;

  /*!
   * \brief Default Constructor.
   *
   * \pre     -
   * \context App
   * \spec
   *   requires true;
   * \endspec
   */
  ClientSubscriberInterface() noexcept = default;

  /*!
   *\brief Virtual destructor
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~ClientSubscriberInterface() noexcept = default;

  ClientSubscriberInterface(ClientSubscriberInterface const&) = delete;
  ClientSubscriberInterface(ClientSubscriberInterface&&) = delete;
  ClientSubscriberInterface& operator=(ClientSubscriberInterface const&) & = delete;
  ClientSubscriberInterface& operator=(ClientSubscriberInterface&&) & = delete;

  /*!
   * \brief Sends SubscribeEvent to the ClientManager if it is the first subscriber.
   *
   * \param[in] event A pointer to the corresponding proxy event. Used for notification handling.
   *                    The ownership of the object is not transferred. The caller of the API must ensure
   *                    passing a valid pointer. The passed pointer must be valid until Unsubscribe() is called.
   *
   * \return     An empty result if Subscription is successful, otherwise an error.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      FALSE
   * \vprivate
   */
  virtual ::amsr::core::Result<void> Subscribe(EventSubscriberInterface* event) noexcept = 0;

  /*!
   * \brief Unsubscribes from event.
   *
   * \param[in] event A pointer to the corresponding proxy event. Used for notification handling.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      FALSE
   * \vprivate
   * \spec
   *   requires event != nullptr;
   * \endspec
   */
  virtual void Unsubscribe(EventSubscriberInterface* event) noexcept = 0;
};
}  // namespace events
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_CLIENT_SUBSCRIBER_INTERFACE_H_
