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
/*!        \file  intermediate_event_object.h
 *        \brief  Intermediate representation for event object.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_INTERMEDIATE_EVENT_OBJECT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_INTERMEDIATE_EVENT_OBJECT_H_
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
 * \brief This is an intermediate representation of an event object which is filled by a parser.
 * \details The validator then takes this object and validates if mandatory elements are set or not.
 */
class EventConfigObject final {
 public:
  /*!
   * \brief Event ID element of this service.
   */
  using IdElement = CfgElement<someip_protocol::internal::EventId>;

  /*!
   * \brief Marks if this is a field or a regular event.
   */
  using IsFieldElement = CfgElement<bool>;

  /*!
   * \brief Protocol used for this event (UDP/IP or TCP/IP)
   */
  using ProtoElement = CfgElement<ConfigurationTypesAndDefs::Protocol>;

  /*!
   * \brief Marks if this event uses signal based serializer
   */
  using IsSignalBasedElement = CfgElement<bool>;

  /*!
   * \brief SOME/IP-TP element for this event.
   */
  using SomeIpTpElement = CfgElement<ConfigurationTypesAndDefs::EventSomeIpTp>;

  /*!
   * \brief OverloadProtectionFilter for this event.
   */
  using OverloadProtectionFilterElement = CfgElement<ConfigurationTypesAndDefs::OverloadProtectionFilter>;

  /*!
   * \brief TimestampFlag for this event.
   */
  using TimeStampFlagElement = CfgElement<bool>;

  /*!
   * \brief Constructor sets the references to the elements of the structure to write to.
   * \param[in] event Reference to the event structure to write the parsed values to.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  explicit EventConfigObject(ConfigurationTypesAndDefs::Event& event) noexcept
      : id_{event.id_},
        is_field_{event.is_field_},
        proto_{event.proto_},
        is_signal_based_{event.is_signal_based_},
        someip_tp_{event.someip_tp_},
        overload_protection_filter_{event.overload_protection_filter_},
        timestamp_flag_{event.timestamp_flag_} {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~EventConfigObject() = default;

  /*!
   * \brief Deleted copy constructor.
   */
  EventConfigObject(EventConfigObject const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  EventConfigObject(EventConfigObject&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(EventConfigObject const&) & -> EventConfigObject& = delete;

  /*!
   * \brief Move assignment is used for re-using an existing EventConfigObject and resetting.
   * \steady FALSE
   */
  auto operator=(EventConfigObject&& other) & noexcept -> EventConfigObject& = default;

  /*!
   * \brief Set the event ID.
   * \param[in] id SOME/IP Event ID.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetId(someip_protocol::internal::EventId const& id) noexcept { id_.SetElement(id); }

  /*!
   * \brief Getter for the event ID element.
   * \return A reference to Event ID element of this service.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  IdElement const& GetId() const noexcept { return id_; }

  /*!
   * \brief Set the field attribute.
   * \param[in] is_field A bool to determine if this is a field or a regular event.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetIsField(bool const& is_field) noexcept { is_field_.SetElement(is_field); }

  /*!
   * \brief Getter if this is a field notifier.
   * \return A reference to Is_field element of this service.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  IsFieldElement const& GetIsField() const noexcept { return is_field_; }

  /*!
   * \brief Set the protocol for this event.
   * \param[in] proto Protocol to be set.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetProto(ConfigurationTypesAndDefs::Protocol const& proto) noexcept { proto_.SetElement(proto); }

  /*!
   * \brief Getter for the protocol.
   * \return A reference to Protocol element of this service.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ProtoElement const& GetProto() const noexcept { return proto_; }

  /*!
   * \brief Set the signal_based attribute.
   * \param[in] is_signal_based A bool to determine if this is a signal based event.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetIsSignalBased(bool const& is_signal_based) noexcept { is_signal_based_.SetElement(is_signal_based); }

  /*!
   * \brief Set the SOME/IP-TP configuration.
   * \param[in] someip_tp SOME/IP-TP configurations for event.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetSomeIpTp(ConfigurationTypesAndDefs::EventSomeIpTp const& someip_tp) noexcept {
    someip_tp_.SetElement(someip_tp);
  }

  /*!
   * \brief Getter for the SOME/IP-TP configuration.
   * \return A reference to SOME/IP-TP element for this event.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  SomeIpTpElement const& GetSomeIpTp() const noexcept { return someip_tp_; }

  /*!
   * \brief Set the overload_protection_filter for this event.
   * \param[in] filter OverloadProtectionFilter for this event.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetOverloadProtectionFilterElement(ConfigurationTypesAndDefs::OverloadProtectionFilter const& filter) noexcept {
    overload_protection_filter_.SetElement(filter);
  }

  /*!
   * \brief Getter for the overload_protection_filter for this event.
   * \return A reference to OverloadProtectionFilter element for this event.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  OverloadProtectionFilterElement const& GetOverloadProtectionFilterElement() const noexcept {
    return overload_protection_filter_;
  }

  /*!
   * \brief Set the timestamp_flag for this event.
   * \param[in] flag Timestamp_flag for this event.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetTimeStampFlagElement(bool const& flag) noexcept { timestamp_flag_.SetElement(flag); }

  /*!
   * \brief Getter for the timestamp_flag for this event.
   * \return A reference to timestamp_flag element for this event.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  TimeStampFlagElement const& GetTimeStampFlagElement() const noexcept { return timestamp_flag_; }

 private:
  /*!
   * \brief The event ID element.
   */
  IdElement id_;

  /*!
   * \brief Whether this is a field or not.
   */
  IsFieldElement is_field_;

  /*!
   * \brief The event's protocol.
   */
  ProtoElement proto_;

  /*!
   * \brief Whether this event uses signal based serializer.
   */
  IsSignalBasedElement is_signal_based_;

  /*!
   * \brief SOME/IP-TP
   */
  SomeIpTpElement someip_tp_;

  /*!
   * \brief OverloadProtectionFilterElement for this event
   */
  OverloadProtectionFilterElement overload_protection_filter_;

  /*!
   * \brief TimeStampFlagElement for this event
   */
  TimeStampFlagElement timestamp_flag_;
};

}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_INTERMEDIATE_EVENT_OBJECT_H_
