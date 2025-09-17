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
/*!        \file  method_someip_tp_object.h
 *        \brief  Internal representation of a SOME/IP-TP configuration object of service methods.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_METHOD_SOMEIP_TP_OBJECT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_METHOD_SOMEIP_TP_OBJECT_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>

#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "someip_tp/someip_tp_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief SOME/IP-TP configuration object that has the same structure as in JSON.
 */
class MethodSomeIpTpConfigObject final {
 public:
  /*!
   * \brief Type for the is_active element.
   */
  using IsActiveElement = CfgElement<bool>;

  /*!
   * \brief Type for the CfgElement containing the separation time of a SOME/IP-TP request.
   */
  using SeparationTimeRequestElement = CfgElement<std::chrono::nanoseconds>;

  /*!
   * \brief Type for the CfgElement holding the separation time of a SOME/IP-TP response.
   */
  using SeparationTimeResponseElement = CfgElement<std::chrono::nanoseconds>;

  /*!
   * \brief Type for the CfgElement holding the maximum segment length of a SOME/IP-TP request.
   */
  using MaximumSegmentLengthRequest = CfgElement<someip_tp::SomeIpTpSegmentSize>;

  /*!
   * \brief Type for the CfgElement holding the maximum segment length of a SOME/IP-TP response.
   */
  using MaximumSegmentLengthResponse = CfgElement<someip_tp::SomeIpTpSegmentSize>;

  /*!
   * \brief Type for the CfgElement that holds the maximum length in bytes of a re-assembled
   * SOME/IP-TP request or response.
   */
  using MaximumRxMessageLength = CfgElement<someip_tp::SomeIpTpMessageSize>;

  /*!
   * \brief Type for the CfgElement that holds the number of rx buffers for re-assembly
   */
  using RxBufferCountElement = CfgElement<someip_tp::SomeIpTpRxBufferCount>;

  /*!
   * \brief Type for the CfgElement containing the burst size of a SOME/IP-TP request.
   */
  using BurstSizeRequestElement = CfgElement<someip_tp::SomeIpTpBurstSize>;

  /*!
   * \brief Type for the CfgElement holding the burst size of a SOME/IP-TP response.
   */
  using BurstSizeResponseElement = CfgElement<someip_tp::SomeIpTpBurstSize>;

  /*!
   * \brief Constructor sets the references to the elements of the structure to write to.
   * \param[in] someip_tp Reference to the structure to write to.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  explicit MethodSomeIpTpConfigObject(ConfigurationTypesAndDefs::MethodSomeIpTp& someip_tp) noexcept
      : is_active_{someip_tp.is_active_},
        separation_time_request_{someip_tp.separation_time_request_},
        separation_time_response_{someip_tp.separation_time_response_},
        maximum_segment_length_request_{someip_tp.maximum_segment_length_request_},
        maximum_segment_length_response_{someip_tp.maximum_segment_length_response_},
        maximum_rx_message_length_{someip_tp.maximum_rx_message_length_},
        rx_buffer_count_{someip_tp.rx_buffer_count_},
        burst_size_request_{someip_tp.burst_size_request_},
        burst_size_response_{someip_tp.burst_size_response_} {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~MethodSomeIpTpConfigObject() = default;

  /*!
   * \brief Deleted copy constructor.
   */
  MethodSomeIpTpConfigObject(MethodSomeIpTpConfigObject const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  MethodSomeIpTpConfigObject(MethodSomeIpTpConfigObject&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(MethodSomeIpTpConfigObject const&) & -> MethodSomeIpTpConfigObject& = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(MethodSomeIpTpConfigObject&&) & -> MethodSomeIpTpConfigObject& = delete;

  /*!
   * \brief Set the is_active element.
   * \param[in] is_active Configuration element
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
  auto GetIsActive() const noexcept -> IsActiveElement const& { return is_active_; }

  /*!
   * \brief Set the separation time of individual SOME/IP-TP request message segments.
   * \param[in] separation_time_request Separation time for a SOME/IP-TP request.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetSeparationTimeRequest(std::chrono::nanoseconds const& separation_time_request) noexcept {
    separation_time_request_.SetElement(separation_time_request);
  }

  /*!
   * \brief Get the separation time of individual SOME/IP-TP request message segments.
   * \return A reference to separation time element of a SOME/IP-TP request.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto GetSeparationTimeRequest() const noexcept -> SeparationTimeRequestElement const& {
    return separation_time_request_;
  }

  /*!
   * \brief Set the separation time of individual SOME/IP-TP response message segments.
   * \param[in] separation_time_response Separation time for a SOME/IP-TP response.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetSeparationTimeResponse(std::chrono::nanoseconds const& separation_time_response) noexcept {
    separation_time_response_.SetElement(separation_time_response);
  }

  /*!
   * \brief Getter for the separation time of individual SOME/IP-TP response message segments.
   * \return A reference to separation time element of a SOME/IP-TP response.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto GetSeparationTimeResponse() const noexcept -> SeparationTimeResponseElement const& {
    return separation_time_response_;
  }

  /*!
   * \brief Set the maximum segment length of a request message in bytes.
   * \param[in] maximum_segment_length_request Maximum segment length of a SOME/IP-TP request.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */

  void SetMaximumSegmentLengthRequest(someip_tp::SomeIpTpSegmentSize const& maximum_segment_length_request) noexcept {
    maximum_segment_length_request_.SetElement(maximum_segment_length_request);
  }

  /*!
   * \brief Get the maximum segment length of a request message in bytes.
   * \return A reference to maximum segment length of a SOME/IP-TP request.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto GetMaximumSegmentLengthRequest() const noexcept -> MaximumSegmentLengthRequest const& {
    return maximum_segment_length_request_;
  }

  /*!
   * \brief Set the maximum segment length of a response message in bytes.
   * \param[in] maximum_segment_length_response Maximum segment length of a SOME/IP-TP response.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */

  void SetMaximumSegmentLengthResponse(someip_tp::SomeIpTpSegmentSize const& maximum_segment_length_response) noexcept {
    maximum_segment_length_response_.SetElement(maximum_segment_length_response);
  }

  /*!
   * \brief Get the maximum segment length of a response message in bytes.
   * \return A reference to maximum segment length of a SOME/IP-TP response.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto GetMaximumSegmentLengthResponse() const noexcept -> MaximumSegmentLengthResponse const& {
    return maximum_segment_length_response_;
  }

  /*!
   * \brief Set the maximum RX message length of a re-assembled SOME/IP-TP request or response in bytes.
   * \param[in] maximum_rx_message_length Maximum RX length of a complete re-assembled SOME/IP-TP request or response.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */

  void SetMaximumRxMessageLength(someip_tp::SomeIpTpSegmentSize const& maximum_rx_message_length) noexcept {
    maximum_rx_message_length_.SetElement(maximum_rx_message_length);
  }

  /*!
   * \brief Get the maximum RX message length of a re-assembled SOME/IP-TP request or response in bytes.
   * \return The maximum RX message length in bytes.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto GetMaximumRxMessageLength() const noexcept -> MaximumRxMessageLength const& {
    return maximum_rx_message_length_;
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
   * \brief Getter for the rx_buffer_count element.
   * \return A reference to rx_buffer_count element.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto GetRxBufferCount() const noexcept -> RxBufferCountElement const& { return rx_buffer_count_; }

  /*!
   * \brief Set the burst size of individual SOME/IP-TP request message segments.
   * \param[in] burst_size_request Burst Size for a SOME/IP-TP request.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetBurstSizeRequest(someip_tp::SomeIpTpBurstSize const& burst_size_request) noexcept {
    burst_size_request_.SetElement(burst_size_request);
  }

  /*!
   * \brief Get the burst size of individual SOME/IP-TP request message segments.
   * \return A reference to Burst Size of a SOME/IP-TP request.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto GetBurstSizeRequest() const noexcept -> BurstSizeRequestElement const& { return burst_size_request_; }

  /*!
   * \brief Set the burst size of individual SOME/IP-TP response message segments.
   * \param[in] burst_size_response Burst Size for a SOME/IP-TP response.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetBurstSizeResponse(someip_tp::SomeIpTpBurstSize const& burst_size_response) noexcept {
    burst_size_response_.SetElement(burst_size_response);
  }

  /*!
   * \brief Getter for the burst size of individual SOME/IP-TP response message segments.
   * \return A reference to Burst Size element of a SOME/IP-TP response.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto GetBurstSizeResponse() const noexcept -> BurstSizeResponseElement const& { return burst_size_response_; }

 private:
  /*!
   * \brief is_active element as the complete SOME/IP-TP for methods is fully optional.
   */
  IsActiveElement is_active_;

  /*!
   * \brief Separation time for a SOME/IP-TP request.
   */
  SeparationTimeRequestElement separation_time_request_;

  /*!
   * \brief Separation time for a SOME/IP-TP response.
   */
  SeparationTimeResponseElement separation_time_response_;

  /*!
   * \brief Maximum segment length of a SOME/IP-TP request.
   */
  MaximumSegmentLengthRequest maximum_segment_length_request_;

  /*!
   * \brief Maximum segment length of a SOME/IP-TP response.
   */
  MaximumSegmentLengthResponse maximum_segment_length_response_;

  /*!
   * \brief Maximum RX length of a complete re-assembled SOME/IP-TP request
   * or response.
   */
  MaximumRxMessageLength maximum_rx_message_length_;

  /*!
   * \brief rx_buffer_count configuration element
   */
  RxBufferCountElement rx_buffer_count_;

  /*!
   * \brief Burst size for a SOME/IP-TP request.
   */
  BurstSizeRequestElement burst_size_request_;

  /*!
   * \brief Burst size for a SOME/IP-TP response.
   */
  BurstSizeResponseElement burst_size_response_;
};

}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_METHOD_SOMEIP_TP_OBJECT_H_
