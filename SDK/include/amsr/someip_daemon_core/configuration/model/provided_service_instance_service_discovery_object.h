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
/*!     \file     provided_service_instance_service_discovery_object.h
 *      \brief    Configuration object for 'provided_service_instance_service_discovery'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_PROVIDED_SERVICE_INSTANCE_SERVICE_DISCOVERY_OBJECT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_PROVIDED_SERVICE_INSTANCE_SERVICE_DISCOVERY_OBJECT_H_
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

// VECTOR NC Metric-OO.WMC.One: MD_SomeIpDaemon_Metric-OO.WMC.One_methods_per_class
/*!
 * \brief This is an intermediate representation of a 'Provided service instance service discovery' object which is
 * filled by a parser.
 * \details The validator then takes this object and validates if mandatory elements are set or not.
 */
class ProvidedServiceInstanceServiceDiscoveryConfigObject final {
 public:
  /*!
   * \brief TTL element.
   */
  using TtlElement = CfgElement<someip_protocol::internal::Ttl>;

  /*!
   * \brief Initial repetitions max element.
   */
  using InitialRepetitionsMaxElement = CfgElement<ConfigurationTypesAndDefs::InitialRepetitionsMax>;

  /*!
   * \brief Initial delay min ns element.
   */
  using InitialDelayMinNsElement = CfgElement<std::chrono::nanoseconds>;

  /*!
   * \brief Initial delay max ns element.
   */
  using InitialDelayMaxNsElement = CfgElement<std::chrono::nanoseconds>;

  /*!
   * \brief Initial repetitions base delay ns element.
   */
  using InitialRepetitionsBaseDelayNsElement = CfgElement<std::chrono::nanoseconds>;

  /*!
   * \brief Cyclic offer delay ns element.
   */
  using CyclicOfferDelayNsElement = CfgElement<std::chrono::nanoseconds>;

  /*!
   * \brief Request response delay min ns element.
   */
  using RequestResponseDelayMinNsElement = CfgElement<std::chrono::nanoseconds>;

  /*!
   * \brief Request response delay max ns element.
   */
  using RequestResponseDelayMaxNsElement = CfgElement<std::chrono::nanoseconds>;

  /*!
   * \brief Eventgroups sub-hierarchy.
   */
  using EventgroupsElement = CfgElement<ConfigurationTypesAndDefs::ProvidedServiceInstanceSdEventgroupContainer>;

  /*!
   * \brief Constructor sets the references to the elements of the structure to write to.
   * \param[in] provided_service_instance_service_discovery Reference to the 'Provided service instance service
   * discovery' structure to write to.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  explicit ProvidedServiceInstanceServiceDiscoveryConfigObject(
      ConfigurationTypesAndDefs::ProvidedServiceInstanceServiceDiscovery&
          provided_service_instance_service_discovery) noexcept
      : ttl_{provided_service_instance_service_discovery.ttl_},
        initial_repetitions_max_{provided_service_instance_service_discovery.initial_repetitions_max_},
        initial_delay_min_ns_{provided_service_instance_service_discovery.initial_delay_min_},
        initial_delay_max_ns_{provided_service_instance_service_discovery.initial_delay_max_},
        initial_repetitions_base_delay_ns_{provided_service_instance_service_discovery.initial_repetitions_base_delay_},
        cyclic_offer_delay_ns_{provided_service_instance_service_discovery.cyclic_offer_delay_},
        request_response_delay_min_ns_{provided_service_instance_service_discovery.request_response_delay_min_},
        request_response_delay_max_ns_{provided_service_instance_service_discovery.request_response_delay_max_},
        eventgroups_{provided_service_instance_service_discovery.eventgroups_} {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~ProvidedServiceInstanceServiceDiscoveryConfigObject() = default;

  /*!
   * \brief Deleted copy constructor.
   */
  ProvidedServiceInstanceServiceDiscoveryConfigObject(ProvidedServiceInstanceServiceDiscoveryConfigObject const&) =
      delete;

  /*!
   * \brief Deleted move constructor.
   */
  ProvidedServiceInstanceServiceDiscoveryConfigObject(ProvidedServiceInstanceServiceDiscoveryConfigObject&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(
      ProvidedServiceInstanceServiceDiscoveryConfigObject const&) & -> ProvidedServiceInstanceServiceDiscoveryConfigObject& =
      delete;

  /*!
   * \brief Move assignment is used for resetting.
   * \steady FALSE
   */
  auto operator=(ProvidedServiceInstanceServiceDiscoveryConfigObject&& other) & noexcept
      -> ProvidedServiceInstanceServiceDiscoveryConfigObject& = default;

  /*!
   * \brief Set the TTL when the parser finds the TTL.
   * \param[in] ttl TTL.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetTtl(someip_protocol::internal::Ttl const& ttl) noexcept { ttl_.SetElement(ttl); }

  /*!
   * \brief Getter for the TTL element.
   * \return Reference to the TTL  of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  TtlElement const& GetTtl() const noexcept { return ttl_; }

  /*!
   * \brief Set the Initial repetitions max when the parser finds the Initial repetitions max.
   * \param[in] initial_repetitions_max Initial repetitions max.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetInitialRepetitionsMax(
      ConfigurationTypesAndDefs::InitialRepetitionsMax const& initial_repetitions_max) noexcept {
    initial_repetitions_max_.SetElement(initial_repetitions_max);
  }

  /*!
   * \brief Getter for the Initial repetitions max element.
   * \return Reference to the Initial repetitions max  of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  InitialRepetitionsMaxElement const& GetInitialRepetitionsMax() const noexcept { return initial_repetitions_max_; }

  /*!
   * \brief Set the Initial delay min ns when the parser finds the Initial delay min ns.
   * \param[in] initial_delay_min_ns Initial delay min ns.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetInitialDelayMinNs(std::chrono::nanoseconds const& initial_delay_min_ns) noexcept {
    initial_delay_min_ns_.SetElement(initial_delay_min_ns);
  }

  /*!
   * \brief Getter for the Initial delay min ns element.
   * \return Reference to the Initial delay min ns of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  InitialDelayMinNsElement const& GetInitialDelayMinNs() const noexcept { return initial_delay_min_ns_; }

  /*!
   * \brief Set the Initial delay max ns when the parser finds the Initial delay max ns.
   * \param[in] initial_delay_max_ns Initial delay max ns.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetInitialDelayMaxNs(std::chrono::nanoseconds const& initial_delay_max_ns) noexcept {
    initial_delay_max_ns_.SetElement(initial_delay_max_ns);
  }

  /*!
   * \brief Getter for the Initial delay max ns element.
   * \return Reference to the Initial delay max ns of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  InitialDelayMaxNsElement const& GetInitialDelayMaxNs() const noexcept { return initial_delay_max_ns_; }

  /*!
   * \brief Set the Initial repetitions base delay ns when the parser finds the Initial repetitions base delay ns.
   * \param[in] initial_repetitions_base_delay_ns Initial repetitions base delay ns.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetInitialRepetitionsBaseDelayNs(std::chrono::nanoseconds const& initial_repetitions_base_delay_ns) noexcept {
    initial_repetitions_base_delay_ns_.SetElement(initial_repetitions_base_delay_ns);
  }

  /*!
   * \brief Getter for the Initial repetitions base delay ns element.
   * \return Reference to the Initial repetitions base delay ns  of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  InitialRepetitionsBaseDelayNsElement const& GetInitialRepetitionsBaseDelayNs() const noexcept {
    return initial_repetitions_base_delay_ns_;
  }

  /*!
   * \brief Set the Cyclic offer delay ns when the parser finds the Cyclic offer delay ns.
   * \param[in] cyclic_offer_delay_ns Cyclic offer delay ns.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetCyclicOfferDelayNs(std::chrono::nanoseconds const& cyclic_offer_delay_ns) noexcept {
    cyclic_offer_delay_ns_.SetElement(cyclic_offer_delay_ns);
  }

  /*!
   * \brief Getter for the Cyclic offer delay ns element.
   * \return Reference to the Cyclic offer delay ns  of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  CyclicOfferDelayNsElement const& GetCyclicOfferDelayNs() const noexcept { return cyclic_offer_delay_ns_; }

  /*!
   * \brief Set the Request response delay min ns when the parser finds the Request response delay min ns.
   * \param[in] request_response_delay_min_ns Request response delay min ns.
   * \pre -
   * \context ANY
   * \reentrant FALSE
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

  /*!
   * \brief Set the Eventgroups sub-hierarchy Eventgroups.
   * \param[in] eventgroups Eventgroups.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetEventgroups(
      ConfigurationTypesAndDefs::ProvidedServiceInstanceSdEventgroupContainer const& eventgroups) noexcept {
    eventgroups_.SetElement(eventgroups);
  }

  /*!
   * \brief Getter for the Eventgroups sub-hierarchy.
   * \return Reference to the Eventgroups sub-hierarchy of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  EventgroupsElement const& GetEventgroups() const noexcept { return eventgroups_; }

 private:
  /*!
   * \brief The TTL element.
   */
  TtlElement ttl_;

  /*!
   * \brief The Initial repetitions max element.
   */
  InitialRepetitionsMaxElement initial_repetitions_max_;

  /*!
   * \brief The Initial delay min ns element.
   */
  InitialDelayMinNsElement initial_delay_min_ns_;

  /*!
   * \brief The Initial delay max ns element.
   */
  InitialDelayMaxNsElement initial_delay_max_ns_;

  /*!
   * \brief The Initial repetitions base delay ns element.
   */
  InitialRepetitionsBaseDelayNsElement initial_repetitions_base_delay_ns_;

  /*!
   * \brief The Cyclic offer delay ns element.
   */
  CyclicOfferDelayNsElement cyclic_offer_delay_ns_;

  /*!
   * \brief The Request response delay min ns element.
   */
  RequestResponseDelayMinNsElement request_response_delay_min_ns_;

  /*!
   * \brief The Request response delay max ns element.
   */
  RequestResponseDelayMaxNsElement request_response_delay_max_ns_;

  /*!
   * \brief The Eventgroups sub-hierarchy.
   */
  EventgroupsElement eventgroups_;
};

}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_PROVIDED_SERVICE_INSTANCE_SERVICE_DISCOVERY_OBJECT_H_
