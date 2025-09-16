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
/*!        \file  required_service_instance_service_discovery_object.h
 *        \brief  Configuration object for 'RequiredServiceInstanceServiceDiscovery'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_REQUIRED_SERVICE_INSTANCE_SERVICE_DISCOVERY_OBJECT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_REQUIRED_SERVICE_INSTANCE_SERVICE_DISCOVERY_OBJECT_H_
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
 * \brief This is an intermediate representation of a 'required service instance service discovery' object which is
 * filled by a parser.
 * \details The validator then takes this object and validates if mandatory elements are set or not.
 */
class RequiredServiceInstanceServiceDiscoveryObject final {
 public:
  /*!
   * \brief TTL element.
   */
  using TtlElement = CfgElement<someip_protocol::internal::Ttl>;

  /*!
   * \brief Maximum number of repetitions in Repetition Phase element.
   */
  using InitialRepetitionsMaxElement = CfgElement<ConfigurationTypesAndDefs::InitialRepetitionsMax>;

  /*!
   * \brief Minimum delay of initial FindService entry element.
   */
  using InitialDelayMinElement = CfgElement<std::chrono::nanoseconds>;

  /*!
   * \brief Maximum delay of initial FindService entry element.
   */
  using InitialDelayMaxElement = CfgElement<std::chrono::nanoseconds>;

  /*!
   * \brief Base delay of FindService entry in Repetition Phase element.
   */
  using InitialRepetitionsBaseDelayElement = CfgElement<std::chrono::nanoseconds>;

  /*!
   * \brief Event group parameters container of the service discovery element.
   */
  using EventgroupContainerElement =
      CfgElement<ConfigurationTypesAndDefs::RequiredServiceInstanceSdEventgroupContainer>;

  /*!
   * \brief Constructor sets the references to the elements of the structure to write to.
   * \param[in] required_service_instance_service_discovery Reference to the POD structure to write to.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  explicit RequiredServiceInstanceServiceDiscoveryObject(
      ConfigurationTypesAndDefs::RequiredServiceInstanceServiceDiscovery&
          required_service_instance_service_discovery) noexcept
      : ttl_{required_service_instance_service_discovery.ttl_},
        initial_repetitions_max_{required_service_instance_service_discovery.initial_repetitions_max_},
        initial_delay_min_{required_service_instance_service_discovery.initial_delay_min_},
        initial_delay_max_{required_service_instance_service_discovery.initial_delay_max_},
        initial_repetitions_base_delay_{required_service_instance_service_discovery.initial_repetitions_base_delay_},
        event_groups_{required_service_instance_service_discovery.eventgroups_} {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~RequiredServiceInstanceServiceDiscoveryObject() = default;

  /*!
   * \brief Deleted copy constructor.
   */
  RequiredServiceInstanceServiceDiscoveryObject(RequiredServiceInstanceServiceDiscoveryObject const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  RequiredServiceInstanceServiceDiscoveryObject(RequiredServiceInstanceServiceDiscoveryObject&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(
      RequiredServiceInstanceServiceDiscoveryObject const&) & -> RequiredServiceInstanceServiceDiscoveryObject& =
      delete;

  /*!
   * \brief Move assignment is used for resetting.
   * \steady FALSE
   */
  auto operator=(RequiredServiceInstanceServiceDiscoveryObject&& other) & noexcept
      -> RequiredServiceInstanceServiceDiscoveryObject& = default;

  /*!
   * \brief Set the time to live (TTL).
   * \param[in] ttl The TTL value.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetTtl(someip_protocol::internal::Ttl const& ttl) noexcept { ttl_.SetElement(ttl); }

  /*!
   * \brief Getter for the TTL element.
   * \return Reference to the TTL of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  TtlElement const& GetTtl() const noexcept { return ttl_; }

  /*!
   * \brief Set the maximum number of repetitions in Repetition Phase.
   * \param[in] value The maximum number of repetitions in Repetition Phase.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetInitialRepetitionsMax(ConfigurationTypesAndDefs::InitialRepetitionsMax const& value) noexcept {
    initial_repetitions_max_.SetElement(value);
  }

  /*!
   * \brief Getter for the maximum number of repetitions in Repetition Phase.
   * \return Reference to the maximum number of repetitions in Repetition Phase intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  InitialRepetitionsMaxElement const& GetInitialRepetitionsMax() const noexcept { return initial_repetitions_max_; }

  /*!
   * \brief Set the minimum delay of initial FindService entry.
   * \param[in] value The minimum delay of initial FindService entry.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetInitialDelayMin(std::chrono::nanoseconds const& value) noexcept { initial_delay_min_.SetElement(value); }

  /*!
   * \brief Getter for the minimum delay of initial FindService entry.
   * \return Reference to the minimum delay of initial FindService entry intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  InitialDelayMinElement const& GetInitialDelayMin() const noexcept { return initial_delay_min_; }

  /*!
   * \brief Set the maximum delay of initial FindService entry.
   * \param[in] value The maximum delay of initial FindService entry.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetInitialDelayMax(std::chrono::nanoseconds const& value) noexcept { initial_delay_max_.SetElement(value); }

  /*!
   * \brief Getter for the maximum delay of initial FindService entry.
   * \return Reference to the maximum delay of initial FindService entry intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  InitialDelayMaxElement const& GetInitialDelayMax() const noexcept { return initial_delay_max_; }

  /*!
   * \brief Set base delay of FindService entry in Repetition Phase.
   * \param[in] value The base delay of FindService entry in Repetition Phase.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetInitialRepetitionsBaseDelay(std::chrono::nanoseconds const& value) noexcept {
    initial_repetitions_base_delay_.SetElement(value);
  }

  /*!
   * \brief Getter for the base delay of FindService entry in Repetition Phase.
   * \return Reference to the base delay of FindService entry in Repetition Phase intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  InitialRepetitionsBaseDelayElement const& GetInitialRepetitionsBaseDelay() const noexcept {
    return initial_repetitions_base_delay_;
  }

  /*!
   * \brief Set the event group parameter container.
   * \param[in] event_groups Reference to the event group parameter container.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetEventGroups(
      ConfigurationTypesAndDefs::RequiredServiceInstanceSdEventgroupContainer const& event_groups) noexcept {
    event_groups_.SetElement(event_groups);
  }

  /*!
   * \brief Getter for the event group parameter container.
   * \return Reference to the event group parameter container intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  EventgroupContainerElement const& GetEventGroups() const noexcept { return event_groups_; }

 private:
  /*!
   * \brief The TTL element.
   */
  TtlElement ttl_;

  /*!
   * \brief Maximum number of repetitions in Repetition Phase element.
   */
  InitialRepetitionsMaxElement initial_repetitions_max_;

  /*!
   * \brief Minimum delay of initial FindService entry element.
   */
  InitialDelayMinElement initial_delay_min_;

  /*!
   * \brief Maximum delay of initial FindService entry element.
   */
  InitialDelayMaxElement initial_delay_max_;

  /*!
   * \brief Base delay of FindService entry in Repetition Phase element.
   */
  InitialRepetitionsBaseDelayElement initial_repetitions_base_delay_;

  /*!
   * \brief Event group parameters container of the service discovery element.
   */
  EventgroupContainerElement event_groups_;
};

}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_REQUIRED_SERVICE_INSTANCE_SERVICE_DISCOVERY_OBJECT_H_
