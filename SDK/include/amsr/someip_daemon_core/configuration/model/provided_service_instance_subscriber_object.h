
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
/*!        \file  provided_service_instance_subscriber_object.h
 *        \brief  Config object for "provided_service_instance_subscriber_object"
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_PROVIDED_SERVICE_INSTANCE_SUBSCRIBER_OBJECT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_PROVIDED_SERVICE_INSTANCE_SUBSCRIBER_OBJECT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief This is an intermediate representation of a 'provided service instance subscriber' object which is filled by
 * a parser.
 */
class ProvidedServiceInstanceSubscriberConfigObject final {
 public:
  /*!
   * \brief Address element.
   */
  using AddressElement = CfgElement<someip_daemon_core::IpAddress>;

  /*!
   * \brief Udp port element.
   */
  using UdpPortElement = CfgElement<someip_protocol::internal::Port>;

  /*!
   * \brief Eventgroups element.
   */
  using EventgroupsElement = CfgElement<ConfigurationTypesAndDefs::EventgroupIdContainer>;

  /*!
   * \brief Ctor sets the references to the elements of the structure to write to.
   * \param[in] service_subscriber Reference to the 'Static sd provided service instances
   * subscriber' structure to write to.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  explicit ProvidedServiceInstanceSubscriberConfigObject(
      ConfigurationTypesAndDefs::ServiceSubscriberConfiguration& service_subscriber) noexcept
      : address_{service_subscriber.address_},
        udp_port_{service_subscriber.udp_port_},
        eventgroups_{service_subscriber.eventgroup_} {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~ProvidedServiceInstanceSubscriberConfigObject() = default;

  /*!
   * \brief Deleted copy constructor.
   */
  ProvidedServiceInstanceSubscriberConfigObject(ProvidedServiceInstanceSubscriberConfigObject const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  ProvidedServiceInstanceSubscriberConfigObject(ProvidedServiceInstanceSubscriberConfigObject&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(ProvidedServiceInstanceSubscriberConfigObject const&)
      -> ProvidedServiceInstanceSubscriberConfigObject& = delete;

  /*!
   * \brief Move assignment is used for resetting.
   * \steady FALSE
   */
  auto operator=(ProvidedServiceInstanceSubscriberConfigObject&& other) & noexcept
      -> ProvidedServiceInstanceSubscriberConfigObject& = default;

  /*!
   * \brief Set the Address when the parser finds the Address.
   * \param[in] address Address.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetAddress(someip_daemon_core::IpAddress const& address) noexcept { address_.SetElement(address); }

  /*!
   * \brief Getter for the Address element.
   * \return Reference to the Address  of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  AddressElement const& GetAddress() const noexcept { return address_; }

  /*!
   * \brief Set the Udp port when the parser finds the Udp port.
   * \param[in] udp_port Udp port.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetUdpPort(someip_protocol::internal::Port const& udp_port) noexcept { udp_port_.SetElement(udp_port); }

  /*!
   * \brief Getter for the Udp port element.
   * \return Reference to the Udp port  of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  UdpPortElement const& GetUdpPort() const noexcept { return udp_port_; }

  /*!
   * \brief Set the Eventgroups when the parser finds the Eventgroups.
   * \param[in] eventgroups Eventgroups.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetEventgroups(ConfigurationTypesAndDefs::EventgroupIdContainer const& eventgroups) noexcept {
    eventgroups_.SetElement(eventgroups);
  }

  /*!
   * \brief Getter for the Eventgroups element.
   * \return Reference to the Eventgroups  of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  EventgroupsElement const& GetEventgroups() const noexcept { return eventgroups_; }

 private:
  /*!
   * \brief The Address element.
   */
  AddressElement address_;

  /*!
   * \brief The Udp port element.
   */
  UdpPortElement udp_port_;

  /*!
   * \brief The Eventgroups element.
   */
  EventgroupsElement eventgroups_;
};

}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_PROVIDED_SERVICE_INSTANCE_SUBSCRIBER_OBJECT_H_
