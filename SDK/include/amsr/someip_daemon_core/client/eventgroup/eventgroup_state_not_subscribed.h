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
/**        \file  eventgroup_state_not_subscribed.h
 *        \brief  A client eventgroup state 'Not Subscribed'
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENTGROUP_STATE_NOT_SUBSCRIBED_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENTGROUP_STATE_NOT_SUBSCRIBED_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/client/eventgroup/eventgroup_state.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {
namespace eventgroup {

/*!
 * \brief Service Discovery Client eventgroup state 'NOT_SUBSCRIBED'.
 */
class EventgroupStateNotSubscribed final : public EventgroupState {
 public:
  /*!
   * \brief   Constructor of EventgroupStateNotSubscribed.
   * \steady  FALSE
   */
  explicit EventgroupStateNotSubscribed(someip_protocol::internal::EventgroupId const eventgroup_id) noexcept;

  /*!
   * \brief   Default destructor.
   * \steady  FALSE
   */
  // VECTOR NC AutosarC++17_10-A10.3.1: MD_SomeIpDaemon_AutosarC++17_10_A10.3.1_final_keyword_needed_false_positive
  // VECTOR NC AutosarC++17_10-A10.3.2: MD_SomeIpDaemon_AutosarC++17_10_A10.3.2_final_keyword_needed_false_positive
  // VECTOR NL AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  ~EventgroupStateNotSubscribed() noexcept = default;

  EventgroupStateNotSubscribed() = delete;
  EventgroupStateNotSubscribed(EventgroupStateNotSubscribed const&) = delete;
  EventgroupStateNotSubscribed(EventgroupStateNotSubscribed&&) = delete;
  EventgroupStateNotSubscribed& operator=(EventgroupStateNotSubscribed const&) & = delete;
  EventgroupStateNotSubscribed& operator=(EventgroupStateNotSubscribed&&) & = delete;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Indicates whether a state change is valid or not.
   *
   * \param[in] handle A state handle.
   * \return true if the state change is valid and false otherwise.
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  bool IsValidChange(EventgroupStateHandle const handle) const override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when the state is entered.
   *
   * \param[in,out] context A state context.
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  void OnEnter(EventgroupStateContext& context) override;

  // VECTOR NC AutosarC++17_10-M7.1.2: MD_SomeIpDaemon_AutosarC++17_10-M7.1.2_Parameter_can_be_declared_as_const
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when the state is left.
   *
   * \param[in,out] context A state context.
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  void OnLeave(EventgroupStateContext& context) override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when the SOME/IP eventgroup has been requested.
   *
   * \param[in,out] context A state context.
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  void OnRequested(EventgroupStateContext& context) override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when the SOME/IP eventgroup has been released.
   *
   * \param[in,out] context A state context.
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  void OnReleased(EventgroupStateContext& context) override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when a matching OfferService entry has been received.
   *
   * \param[in,out] context A state context.
   * \param[in] is_multicast Indicates whether the received 'OfferService' message has been sent via multicast.
   * \pre     -
   * \context App
   * \steady  FALSE
   * \trace SPEC-5951396
   */
  void OnOfferService(EventgroupStateContext& context, bool is_multicast) override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when a matching StopOfferService entry has been received.
   *
   * \param[in,out] context A state context.
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  void OnStopOfferService(EventgroupStateContext& context) override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when a matching SubscribeEventgroupAck entry has been received.
   *
   * \param[in,out] context A state context.
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  void OnSubscribeEventgroupAck(EventgroupStateContext& context) override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when a matching SubscribeEventgroupNack entry has been received.
   *
   * \param[in,out] context A state context.
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  void OnSubscribeEventgroupNack(EventgroupStateContext& context) override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when a connection to the server has been established.
   *
   * \param[in,out] context A state context.
   * \pre     -
   * \context Network
   * \steady  FALSE
   */
  void OnConnectionEstablished(EventgroupStateContext& context) override;

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief Called when a connection to the server has been closed.
   *
   * \param[in] context A state context.
   * \pre     -
   * \context Network
   * \steady  FALSE
   */
  void OnConnectionClosed(EventgroupStateContext& context) const noexcept override;

 private:
  /*!
   * \brief A SOME/IP eventgroup identifier.
   */
  someip_protocol::internal::EventgroupId const eventgroup_id_;

  /*!
   * \brief A logger instance.
   */
  someip_daemon_core::logging::AraComLogger logger_{
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
      ara::core::StringView{"EventgroupStateNotSubscribed"}};
};

}  // namespace eventgroup
}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_EVENTGROUP_EVENTGROUP_STATE_NOT_SUBSCRIBED_H_
