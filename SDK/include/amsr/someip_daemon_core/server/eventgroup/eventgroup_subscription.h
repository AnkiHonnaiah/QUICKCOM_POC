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
/**        \file  eventgroup_subscription.h
 *        \brief  Represents an eventgroup subscription and handles TTL monitoring
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_EVENTGROUP_EVENTGROUP_SUBSCRIPTION_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_EVENTGROUP_EVENTGROUP_SUBSCRIPTION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/net/ip/address.h"
#include "amsr/someip_daemon_core/service_discovery/message/options/ip_endpoint_option.h"
#include "amsr/steady_timer/timer_interface.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "ara/core/optional.h"
#include "ara/core/vector.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace server {
namespace eventgroup {

// Forward declaration
class EventgroupSubscription;

/*!
 * \brief Interface for handling the expiry of eventgroup subscription
 */
class EventgroupSubscriptionExpiryHandler {
 public:
  /*!
   * \brief Define default constructor.
   *
   * \pre          -
   * \context      Init
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  EventgroupSubscriptionExpiryHandler() = default;
  /*!
   * \brief Define default destructor.
   *
   * \pre          -
   * \context      Shutdown
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady FALSE
   */
  virtual ~EventgroupSubscriptionExpiryHandler() = default;

  EventgroupSubscriptionExpiryHandler(EventgroupSubscriptionExpiryHandler const&) = delete;
  EventgroupSubscriptionExpiryHandler(EventgroupSubscriptionExpiryHandler&&) = delete;
  EventgroupSubscriptionExpiryHandler& operator=(EventgroupSubscriptionExpiryHandler const&) & = delete;
  EventgroupSubscriptionExpiryHandler& operator=(EventgroupSubscriptionExpiryHandler&&) & = delete;

  /*!
   * \brief Called on eventgroup subscription expiration.
   *
   * \param[in] egs An eventgroup subscription which expired.
   *
   * \pre          -
   * \context      Timer
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   * \trace SPEC-10144845
   */
  virtual void OnSubscribeEventgroupExpired(EventgroupSubscription* egs) = 0;
};

/*!
 * \brief Represents an eventgroup subscription.
 */
class EventgroupSubscription final {
 public:
  /*!
   * \brief Constructor of EventgroupSubscription.
   *
   * \param[in] timer_manager  The time manager instance.
   * \param[in] sd_address     The IP address of the service discovery endpoint that sent the eventgroup
   *                           subscription request.
   * \param[in] sd_port        The UDP port of the service discovery endpoint that sent the eventgroup
   *                           subscription request.
   * \param[in] udp_endpoint   A subscriber's UDP endpoint.
   * \param[in] tcp_endpoint   A subscriber's TCP endpoint.
   * \param[in] eventgroup_id  A SOME/IP eventgroup identifier.
   * \param[in] handler        Handler to be called on ttl timeout.
   *
   * \pre          -
   * \context      Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  EventgroupSubscription(amsr::steady_timer::TimerManagerInterface* timer_manager,
                         someip_daemon_core::IpAddress const& sd_address, amsr::net::ip::Port const sd_port,
                         ara::core::Optional<service_discovery::message::options::IpEndpointOption> const& udp_endpoint,
                         ara::core::Optional<service_discovery::message::options::IpEndpointOption> const& tcp_endpoint,
                         someip_protocol::internal::EventgroupId const eventgroup_id,
                         EventgroupSubscriptionExpiryHandler* handler) noexcept
      : sd_endpoint_{sd_address, sd_port},
        udp_endpoint_{udp_endpoint},
        tcp_endpoint_{tcp_endpoint},
        eventgroup_id_{eventgroup_id},
        ttl_timeout_timer_{timer_manager->CreateTimer(
            [this, handler]() {
              // Upon timer expiration, notify the handler about the subscription expiration
              handler->OnSubscribeEventgroupExpired(this);
            },
            amsr::steady_timer::MissedTimeoutBehavior::kDiscardMissedTimeouts)} {
    assert(ttl_timeout_timer_ != nullptr);
  }

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~EventgroupSubscription() noexcept = default;

  EventgroupSubscription(EventgroupSubscription const&) = delete;
  EventgroupSubscription& operator=(EventgroupSubscription const&) = delete;
  EventgroupSubscription(EventgroupSubscription&&) = delete;
  EventgroupSubscription& operator=(EventgroupSubscription&&) & = delete;

  /*!
   * \brief Starts TTL timeout timer using the given duration.
   * \details If the timer is already running, it will be re-started.
   *
   * \param[in] ttl_duration The duration after which the TTL timeout timer will trigger.
   *
   * \pre          -
   * \context      Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  void StartTtlTimeoutTimer(amsr::steady_timer::Duration const ttl_duration) noexcept {
    ttl_timeout_timer_->Start(ara::core::Optional<amsr::steady_timer::Duration>(ttl_duration), {});
  }

  /*!
   * \brief Stops current TTL timer.
   *
   * \pre          -
   * \context      Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   */
  void StopTtlTimeoutTimer() noexcept { ttl_timeout_timer_->Stop(); }

  /*!
   * \brief  Getter for the subscribed Eventgroup ID
   * \return Eventgroup ID
   *
   * \pre          -
   * \context      Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       TRUE
   */
  someip_protocol::internal::EventgroupId GetEventgroupId() const noexcept { return eventgroup_id_; }

  /*!
   * \brief  Getter for the endpoint of the service discovery that sent the eventgroup subscription request
   * \return SD endpoint
   *
   * \pre          -
   * \context      Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       TRUE
   */
  service_discovery::message::options::IpEndpointOption const& GetSdEndpoint() const noexcept { return sd_endpoint_; }

  /*!
   * \brief  Getter for the TCP endpoint of the subscriber
   * \return Optional containing the UDP endpoint of the subscriber if exists
   *
   * \pre          -
   * \context      Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       TRUE
   */
  ara::core::Optional<service_discovery::message::options::IpEndpointOption> GetTcpEndpoint() const noexcept {
    return tcp_endpoint_;
  }

  /*!
   * \brief  Getter for the UDP endpoint of the subscriber
   * \return Optional containing the UDP endpoint of the subscriber if exists
   *
   * \pre          -
   * \context      Network
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       TRUE
   */
  ara::core::Optional<service_discovery::message::options::IpEndpointOption> GetUdpEndpoint() const noexcept {
    return udp_endpoint_;
  }

 private:
  /*!
   * \brief The endpoint of the service discovery that sent the eventgroup subscription request
   */
  service_discovery::message::options::IpEndpointOption const sd_endpoint_;

  /*!
   * \brief Subscriber's udp endpoint
   */
  ara::core::Optional<service_discovery::message::options::IpEndpointOption> const udp_endpoint_;

  /*!
   * \brief Subscriber's tcp endpoint
   */
  ara::core::Optional<service_discovery::message::options::IpEndpointOption> const tcp_endpoint_;

  /*!
   * \brief SOME/IP event group identifier
   */
  someip_protocol::internal::EventgroupId const eventgroup_id_;

  /*!
   * \brief TTL timer for the subscription timeout
   */
  std::unique_ptr<amsr::steady_timer::TimerInterface> ttl_timeout_timer_;
};

}  // namespace eventgroup
}  // namespace server
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_EVENTGROUP_EVENTGROUP_SUBSCRIPTION_H_
