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
/*!        \file  required_service_instance_service_discovery_event_group_object.h
 *        \brief  Configuration object for 'RequiredServiceInstanceServiceDiscoveryEventGroup'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_REQUIRED_SERVICE_INSTANCE_SERVICE_DISCOVERY_EVENT_GROUP_OBJECT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_REQUIRED_SERVICE_INSTANCE_SERVICE_DISCOVERY_EVENT_GROUP_OBJECT_H_
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
 * \brief This is an intermediate representation of a 'required service instance service discovery event group' object
 * which is filled by a parser.
 * \details The validator then takes this object and validates if mandatory elements are set or not.
 */
class RequiredServiceInstanceServiceDiscoveryEventGroupObject final {
 public:
  /*!
   * \brief Delay of SubscribeEventgroup retry element.
   */
  using SubscribeEventgroupRetryDelay = someip_protocol::internal::SubscribeEventgroupRetryDelay;

  /*!
   * \brief Maximum retries of SubscribeEventgroup element.
   */
  using SubscribeEventgroupRetryMax = someip_protocol::internal::SubscribeEventgroupRetryMax;

  /*!
   * \brief EventGroup ID element.
   */
  using EventgroupIdElement = CfgElement<someip_protocol::internal::EventgroupId>;

  /*!
   * \brief SubscribeEventgroupRetryDelay element.
   */
  using SubscribeEventgroupRetryDelayElement = CfgElement<ara::core::Optional<SubscribeEventgroupRetryDelay>>;

  /*!
   * \brief SubscribeEventgroupRetryMax element.
   */
  using SubscribeEventgroupRetryMaxElement = CfgElement<ara::core::Optional<SubscribeEventgroupRetryMax>>;

  /*!
   * \brief TTL element.
   */
  using TtlElement = CfgElement<someip_protocol::internal::Ttl>;

  /*!
   * \brief Minimum delay of SubscribeEventgroup entry element.
   */
  using RequestResponseDelayMinElement = CfgElement<std::chrono::nanoseconds>;

  /*!
   * \brief Maximum delay of SubscribeEventgroup entry element.
   */
  using RequestResponseDelayMaxElement = CfgElement<std::chrono::nanoseconds>;

  /*!
   * \brief Constructor sets the references to the elements of the structure to write to.
   * \param[in] required_service_instance_sd_eventgroup Reference to the POD structure to write to.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  explicit RequiredServiceInstanceServiceDiscoveryEventGroupObject(
      ConfigurationTypesAndDefs::RequiredServiceInstanceSdEventgroup& required_service_instance_sd_eventgroup) noexcept
      : id_{required_service_instance_sd_eventgroup.id_},
        subscribe_eventgroup_retry_delay_ms_{
            required_service_instance_sd_eventgroup.subscribe_eventgroup_retry_delay_ms_},
        subscribe_eventgroup_retry_max_{required_service_instance_sd_eventgroup.subscribe_eventgroup_retry_max_},
        ttl_{required_service_instance_sd_eventgroup.ttl_},
        request_response_delay_min_{required_service_instance_sd_eventgroup.request_response_delay_min_},
        request_response_delay_max_{required_service_instance_sd_eventgroup.request_response_delay_max_} {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~RequiredServiceInstanceServiceDiscoveryEventGroupObject() = default;

  /*!
   * \brief Deleted copy constructor.
   */
  RequiredServiceInstanceServiceDiscoveryEventGroupObject(
      RequiredServiceInstanceServiceDiscoveryEventGroupObject const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  RequiredServiceInstanceServiceDiscoveryEventGroupObject(RequiredServiceInstanceServiceDiscoveryEventGroupObject&&) =
      delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(
      RequiredServiceInstanceServiceDiscoveryEventGroupObject const&) & -> RequiredServiceInstanceServiceDiscoveryEventGroupObject& =
      delete;

  /*!
   * \brief Move assignment is used for resetting.
   * \steady FALSE
   */
  auto operator=(RequiredServiceInstanceServiceDiscoveryEventGroupObject&& other) & noexcept
      -> RequiredServiceInstanceServiceDiscoveryEventGroupObject& = default;

  /*!
   * \brief Set the EventGroup ID.
   * \param[in] id EventGroup ID.
   * \steady FALSE
   */
  void SetEventGroupId(someip_protocol::internal::EventgroupId const& id) noexcept { id_.SetElement(id); }

  /*!
   * \brief Getter for the EventGroup ID element.
   * \return Reference to the EventGroup ID of this intermediate object.
   * \steady FALSE
   */
  EventgroupIdElement const& GetEventGroupId() const noexcept { return id_; }

  /*!
   * \brief Set the SubscribeEventgroupRetryDelay.
   * \param[in] subscribe_eventgroup_retry_delay_ms The SubscribeEventgroupRetryDelay in milliseconds.
   * \steady FALSE
   */
  void SetSubscribeEventgroupRetryDelay(
      SubscribeEventgroupRetryDelay const& subscribe_eventgroup_retry_delay_ms) noexcept {
    subscribe_eventgroup_retry_delay_ms_.SetElement(subscribe_eventgroup_retry_delay_ms);
  }

  /*!
   * \brief Getter for the SubscribeEventgroupRetryDelay element.
   * \return Reference to the SubscribeEventgroupRetryDelay of this intermediate object.
   * \steady FALSE
   */
  SubscribeEventgroupRetryDelayElement const& GetSubscribeEventgroupRetryDelay() const noexcept {
    return subscribe_eventgroup_retry_delay_ms_;
  }

  /*!
   * \brief Set the SubscribeEventgroupRetryMax.
   * \param[in] subscribe_eventgroup_retry_max The SubscribeEventgroupRetryMax value.
   * \steady FALSE
   */
  void SetSubscribeEventgroupRetryMax(SubscribeEventgroupRetryMax const& subscribe_eventgroup_retry_max) noexcept {
    subscribe_eventgroup_retry_max_.SetElement(subscribe_eventgroup_retry_max);
  }

  /*!
   * \brief Getter for the SubscribeEventgroupRetryMax element.
   * \return Reference to the SubscribeEventgroupRetryMax of this intermediate object.
   * \steady FALSE
   */
  SubscribeEventgroupRetryMaxElement const& GetSubscribeEventgroupRetryMax() const noexcept {
    return subscribe_eventgroup_retry_max_;
  }

  /*!
   * \brief Set the time to live (TTL).
   * \param[in] ttl The TTL value.
   * \steady FALSE
   */
  void SetTtl(someip_protocol::internal::Ttl const& ttl) noexcept { ttl_.SetElement(ttl); }

  /*!
   * \brief Getter for the TTL element.
   * \return Reference to the TTL of this intermediate object.
   * \steady FALSE
   */
  TtlElement const& GetTtl() const noexcept { return ttl_; }

  /*!
   * \brief Set the minimum delay of SubscribeEventgroup entry.
   * \param[in] value The minimum delay.
   * \trace SPEC-10144667
   * \steady FALSE
   */
  void SetRequestResponseDelayMin(std::chrono::nanoseconds const& value) noexcept {
    request_response_delay_min_.SetElement(value);
  }

  /*!
   * \brief Getter for the minimum delay of SubscribeEventgroup entry.
   * \return Reference to the minimum delay of SubscribeEventgroup entry intermediate object.
   * \steady FALSE
   */
  RequestResponseDelayMinElement const& GetRequestResponseDelayMin() const noexcept {
    return request_response_delay_min_;
  }

  /*!
   * \brief Set the maximum delay of SubscribeEventgroup entry.
   * \param[in] value The maximum delay.
   * \trace SPEC-10144667
   * \steady FALSE
   */
  void SetRequestResponseDelayMax(std::chrono::nanoseconds const& value) noexcept {
    request_response_delay_max_.SetElement(value);
  }

  /*!
   * \brief Getter for the maximum delay of SubscribeEventgroup entry.
   * \return Reference to the maximum delay of SubscribeEventgroup entry intermediate object.
   * \steady FALSE
   */
  RequestResponseDelayMaxElement const& GetRequestResponseDelayMax() const noexcept {
    return request_response_delay_max_;
  }

 private:
  /*!
   * \brief The event ID element.
   */
  EventgroupIdElement id_;

  /*!
   * \brief The SubscribeEventgroupRetryDelay element.
   */
  SubscribeEventgroupRetryDelayElement subscribe_eventgroup_retry_delay_ms_;

  /*!
   * \brief The SubscribeEventgroupRetryMax element.
   */
  SubscribeEventgroupRetryMaxElement subscribe_eventgroup_retry_max_;

  /*!
   * \brief The TTL element.
   */
  TtlElement ttl_;

  /*!
   * \brief The minimum delay of SubscribeEventgroup element.
   */
  RequestResponseDelayMinElement request_response_delay_min_;

  /*!
   * \brief The maximum delay of SubscribeEventgroup element.
   */
  RequestResponseDelayMaxElement request_response_delay_max_;
};

}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_REQUIRED_SERVICE_INSTANCE_SERVICE_DISCOVERY_EVENT_GROUP_OBJECT_H_
