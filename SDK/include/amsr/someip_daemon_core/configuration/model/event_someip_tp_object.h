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
/*!        \file  event_someip_tp_object.h
 *        \brief  SOME/IP-TP objects for events.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_EVENT_SOMEIP_TP_OBJECT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_EVENT_SOMEIP_TP_OBJECT_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>

#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief Holds the elements of one SOME/IP-TP object of a service event.
 */
class EventSomeIpTpConfigObject final {
 public:
  /*!
   * \brief Type for the is_active element.
   */
  using IsActiveElement = CfgElement<bool>;

  /*!
   * \brief Type for the separation_time element.
   */
  using SeparationTimeElement = CfgElement<std::chrono::nanoseconds>;

  /*!
   * \brief Type for the maximum_segment_length element.
   */
  using MaximumSegmentLengthElement = CfgElement<someip_tp::SomeIpTpSegmentSize>;

  /*!
   * \brief Type for the maximum_rx_message_length element.
   */
  using MaximumRxMessageLengthElement = CfgElement<someip_tp::SomeIpTpMessageSize>;

  /*!
   * \brief Type for the rx_buffer_count element.
   */
  using RxBufferCountElement = CfgElement<someip_tp::SomeIpTpRxBufferCount>;

  /*!
   * \brief Type for the burst size element.
   */
  using BurstSizeElement = CfgElement<someip_tp::SomeIpTpBurstSize>;

  /*!
   * \brief Constructor sets the references to the elements of the structure to write to.
   * \param[in] someip_tp Reference to the structure to write to.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  explicit EventSomeIpTpConfigObject(ConfigurationTypesAndDefs::EventSomeIpTp& someip_tp) noexcept
      : is_active_{someip_tp.is_active_},
        separation_time_{someip_tp.separation_time_},
        maximum_segment_length_{someip_tp.maximum_segment_length_},
        maximum_rx_message_length_{someip_tp.maximum_rx_message_length_},
        rx_buffer_count_{someip_tp.rx_buffer_count_},
        burst_size_{someip_tp.burst_size_} {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~EventSomeIpTpConfigObject() = default;

  /*!
   * \brief Deleted copy constructor.
   */
  EventSomeIpTpConfigObject(EventSomeIpTpConfigObject const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  EventSomeIpTpConfigObject(EventSomeIpTpConfigObject&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(EventSomeIpTpConfigObject const&) & -> EventSomeIpTpConfigObject& = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(EventSomeIpTpConfigObject&&) & -> EventSomeIpTpConfigObject& = delete;

  /*!
   * \brief Set the is_active element.
   * \param[in] is_active Configuration element.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetIsActive(bool const& is_active) noexcept { is_active_.SetElement(is_active); }

  /*!
   * \brief Getter for the is_active element.
   * \return A reference to is_active element.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  IsActiveElement const& GetIsActive() const noexcept { return is_active_; }

  /*!
   * \brief Set the separation_time element.
   * \param[in] separation_time Configuration element.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetSeparationTime(std::chrono::nanoseconds const& separation_time) noexcept {
    separation_time_.SetElement(separation_time);
  }

  /*!
   * \brief Set the maximum_segment_length element.
   * \param[in] maximum_segment_length Configuration element.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   */

  void SetMaximumSegmentLength(someip_tp::SomeIpTpSegmentSize const& maximum_segment_length) noexcept {
    maximum_segment_length_.SetElement(maximum_segment_length);
  }

  /*!
   * \brief Set the maximum_rx_message_length element.
   * \param[in] maximum_rx_message_length Configuration element.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetMaximumRxMessageLength(someip_tp::SomeIpTpSegmentSize const& maximum_rx_message_length) noexcept {
    maximum_rx_message_length_.SetElement(maximum_rx_message_length);
  }

  /*!
   * \brief Set the rx_buffer_count element.
   * \param[in] rx_buffer_count Configuration element.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetRxBufferCount(someip_tp::SomeIpTpRxBufferCount const& rx_buffer_count) noexcept {
    rx_buffer_count_.SetElement(rx_buffer_count);
  }

  /*!
   * \brief Set the burst_size element.
   * \param[in] burst_size Configuration element.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetBurstSize(someip_tp::SomeIpTpBurstSize const& burst_size) noexcept { burst_size_.SetElement(burst_size); }

  /*!
   * \brief Getter for the burst_size element.
   * \return A reference to burst_size element.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  BurstSizeElement const& GetBurstSize() const noexcept { return burst_size_; }

 private:
  /*!
   * \brief is_active configuration element
   */
  IsActiveElement is_active_;

  /*!
   * \brief separation_time configuration element
   */
  SeparationTimeElement separation_time_;

  /*!
   * \brief maximum_segment_length configuration element
   */
  MaximumSegmentLengthElement maximum_segment_length_;

  /*!
   * \brief maximum_rx_message_length configuration element
   */
  MaximumRxMessageLengthElement maximum_rx_message_length_;

  /*!
   * \brief rx_buffer_count_ configuration element
   */
  RxBufferCountElement rx_buffer_count_;

  /*!
   * \brief burst_size configuration element
   */
  BurstSizeElement burst_size_;
};

}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_EVENT_SOMEIP_TP_OBJECT_H_
