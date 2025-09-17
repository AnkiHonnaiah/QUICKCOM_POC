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
/*!     \file     provided_service_instance_service_discovery_event_group_object.h
 *      \brief    Configuration object for 'provided_service_instance_service_discovery_event_group'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_PROVIDED_SERVICE_INSTANCE_SERVICE_DISCOVERY_EVENT_GROUP_OBJECT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_PROVIDED_SERVICE_INSTANCE_SERVICE_DISCOVERY_EVENT_GROUP_OBJECT_H_
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
 * \brief This is an intermediate representation of a 'Provided service instance service discovery event group' object
 * which is filled by a
 * parser.
 * \details The validator then takes this object and validates if mandatory elements are set or not.
 */
class ProvidedServiceInstanceServiceDiscoveryEventGroupConfigObject final {
 public:
  /*!
   * \brief Id element.
   */
  using IdElement = CfgElement<someip_protocol::internal::EventgroupId>;

  /*!
   * \brief Event multicast threshold element.
   */
  using EventMulticastThresholdElement = CfgElement<ConfigurationTypesAndDefs::EventMulticastThreshold>;

  /*!
   * \brief Request response delay min ns element.
   */
  using RequestResponseDelayMinNsElement = CfgElement<std::chrono::nanoseconds>;

  /*!
   * \brief Request response delay max ns element.
   */
  using RequestResponseDelayMaxNsElement = CfgElement<std::chrono::nanoseconds>;

  /*!
   * \brief Constructor sets the references to the elements of the structure to write to.
   * \param[in] provided_service_instance_service_discovery_event_group Reference to the 'Provided service instance
   * service discovery event group' structure to write to.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  explicit ProvidedServiceInstanceServiceDiscoveryEventGroupConfigObject(
      ConfigurationTypesAndDefs::ProvidedServiceInstanceSdEventgroup&
          provided_service_instance_service_discovery_event_group) noexcept
      : id_{provided_service_instance_service_discovery_event_group.id_},
        event_multicast_threshold_{provided_service_instance_service_discovery_event_group.event_multicast_threshold_},
        request_response_delay_min_ns_{
            provided_service_instance_service_discovery_event_group.request_response_delay_min_},
        request_response_delay_max_ns_{
            provided_service_instance_service_discovery_event_group.request_response_delay_max_} {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~ProvidedServiceInstanceServiceDiscoveryEventGroupConfigObject() = default;

  /*!
   * \brief Deleted copy constructor.
   */
  ProvidedServiceInstanceServiceDiscoveryEventGroupConfigObject(
      ProvidedServiceInstanceServiceDiscoveryEventGroupConfigObject const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  ProvidedServiceInstanceServiceDiscoveryEventGroupConfigObject(
      ProvidedServiceInstanceServiceDiscoveryEventGroupConfigObject&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(
      ProvidedServiceInstanceServiceDiscoveryEventGroupConfigObject const&) & -> ProvidedServiceInstanceServiceDiscoveryEventGroupConfigObject& =
      delete;

  /*!
   * \brief Move assignment is used for resetting.
   * \steady FALSE
   */
  auto operator=(ProvidedServiceInstanceServiceDiscoveryEventGroupConfigObject&& other) & noexcept
      -> ProvidedServiceInstanceServiceDiscoveryEventGroupConfigObject& = default;

  /*!
   * \brief Set the Id when the parser finds the Id.
   * \param[in] id Id.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetId(someip_protocol::internal::EventgroupId const& id) noexcept { id_.SetElement(id); }

  /*!
   * \brief Getter for the Id element.
   * \return Reference to the Id  of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  IdElement const& GetId() const noexcept { return id_; }

  /*!
   * \brief Set the Event multicast threshold when the parser finds the Event multicast threshold.
   * \param[in] event_multicast_threshold Event multicast threshold.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetEventMulticastThreshold(
      ConfigurationTypesAndDefs::EventMulticastThreshold const& event_multicast_threshold) noexcept {
    event_multicast_threshold_.SetElement(event_multicast_threshold);
  }

  /*!
   * \brief Set the Request response delay min ns when the parser finds the Request response delay min ns.
   * \param[in] request_response_delay_min_ns Request response delay min ns.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \trace SPEC-10144667
   * \steady FALSE
   */
  void SetRequestResponseDelayMinNs(std::chrono::nanoseconds const& request_response_delay_min_ns) noexcept {
    request_response_delay_min_ns_.SetElement(request_response_delay_min_ns);
  }

  /*!
   * \brief Getter for the Request response delay min ns element.
   * \return Reference to the Request response delay min ns of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  RequestResponseDelayMinNsElement const& GetRequestResponseDelayMinNs() const noexcept {
    return request_response_delay_min_ns_;
  }

  /*!
   * \brief Set the Request response delay max ns when the parser finds the Request response delay max ns.
   * \param[in] request_response_delay_max_ns Request response delay max ns.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \trace SPEC-10144667
   * \steady FALSE
   */
  void SetRequestResponseDelayMaxNs(std::chrono::nanoseconds const& request_response_delay_max_ns) noexcept {
    request_response_delay_max_ns_.SetElement(request_response_delay_max_ns);
  }

  /*!
   * \brief Getter for the Request response delay max ns element.
   * \return Reference to the Request response delay max ns of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  RequestResponseDelayMaxNsElement const& GetRequestResponseDelayMaxNs() const noexcept {
    return request_response_delay_max_ns_;
  }

 private:
  /*!
   * \brief The Id element.
   */
  IdElement id_;

  /*!
   * \brief The Event multicast threshold element.
   */
  EventMulticastThresholdElement event_multicast_threshold_;

  /*!
   * \brief The Request response delay min ns element.
   */
  RequestResponseDelayMinNsElement request_response_delay_min_ns_;

  /*!
   * \brief The Request response delay max ns element.
   */
  RequestResponseDelayMaxNsElement request_response_delay_max_ns_;
};

}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_PROVIDED_SERVICE_INSTANCE_SERVICE_DISCOVERY_EVENT_GROUP_OBJECT_H_
