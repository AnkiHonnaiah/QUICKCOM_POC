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
/*!     \file     network_endpoint_port_keep_alive_object.h
 *      \brief    Configuration object for 'network_endpoint_port_keep_alive'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_NETWORK_ENDPOINT_PORT_KEEP_ALIVE_OBJECT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_NETWORK_ENDPOINT_PORT_KEEP_ALIVE_OBJECT_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>

#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "ara/core/optional.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief This is an intermediate representation of a 'Network endpoint port keep alive' object which is filled by a
 * parser.
 * \details The validator then takes this object and validates if mandatory elements are set or not.
 */
class NetworkEndpointPortKeepAliveConfigObject final {
 public:
  /*!
   * \brief Idle time s element.
   */
  using IdleTimeSElement = CfgElement<ara::core::Optional<std::chrono::seconds>>;

  /*!
   * \brief Alive interval s element.
   */
  using AliveIntervalSElement = CfgElement<ara::core::Optional<std::chrono::seconds>>;

  /*!
   * \brief Retry count element.
   */
  using RetryCountElement = CfgElement<ara::core::Optional<someip_protocol::internal::KeepAliveRetryCount>>;

  /*!
   * \brief Constructor sets the references to the elements of the structure to write to.
   * \param[in] network_endpoint_port_keep_alive Reference to the 'Network endpoint port keep alive' structure to write
   * to.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  explicit NetworkEndpointPortKeepAliveConfigObject(
      someip_protocol::internal::KeepAliveParameters& network_endpoint_port_keep_alive) noexcept
      : idle_time_s_{network_endpoint_port_keep_alive.keep_alive_time_},
        alive_interval_s_{network_endpoint_port_keep_alive.keep_alive_interval_},
        retry_count_{network_endpoint_port_keep_alive.keep_alive_retry_count_} {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~NetworkEndpointPortKeepAliveConfigObject() = default;

  /*!
   * \brief Deleted copy constructor.
   */
  NetworkEndpointPortKeepAliveConfigObject(NetworkEndpointPortKeepAliveConfigObject const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  NetworkEndpointPortKeepAliveConfigObject(NetworkEndpointPortKeepAliveConfigObject&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(NetworkEndpointPortKeepAliveConfigObject const&) & -> NetworkEndpointPortKeepAliveConfigObject& =
      delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(NetworkEndpointPortKeepAliveConfigObject&&) & -> NetworkEndpointPortKeepAliveConfigObject& = delete;

  /*!
   * \brief Set the Idle time s when the parser finds the Idle time s.
   * \param[in] idle_time_s Idle time s.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetIdleTimeS(std::chrono::seconds const& idle_time_s) noexcept { idle_time_s_.SetElement(idle_time_s); }

  /*!
   * \brief Getter for the Idle time s element.
   * \return Reference to the Idle time s  of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  IdleTimeSElement const& GetIdleTimeS() const noexcept { return idle_time_s_; }

  /*!
   * \brief Set the Alive interval s when the parser finds the Alive interval s.
   * \param[in] alive_interval_s Alive interval s.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetAliveIntervalS(std::chrono::seconds const& alive_interval_s) noexcept {
    alive_interval_s_.SetElement(alive_interval_s);
  }

  /*!
   * \brief Getter for the Alive interval s element.
   * \return Reference to the Alive interval s  of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  AliveIntervalSElement const& GetAliveIntervalS() const noexcept { return alive_interval_s_; }

  /*!
   * \brief Set the Retry count when the parser finds the Retry count.
   * \param[in] retry_count Retry count.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetRetryCount(someip_protocol::internal::KeepAliveRetryCount const& retry_count) noexcept {
    retry_count_.SetElement(retry_count);
  }

  /*!
   * \brief Getter for the Retry count element.
   * \return Reference to the Retry count  of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  RetryCountElement const& GetRetryCount() const noexcept { return retry_count_; }

 private:
  /*!
   * \brief The Idle time s element.
   */
  IdleTimeSElement idle_time_s_;

  /*!
   * \brief The Alive interval s element.
   */
  AliveIntervalSElement alive_interval_s_;

  /*!
   * \brief The Retry count element.
   */
  RetryCountElement retry_count_;
};

}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_NETWORK_ENDPOINT_PORT_KEEP_ALIVE_OBJECT_H_
