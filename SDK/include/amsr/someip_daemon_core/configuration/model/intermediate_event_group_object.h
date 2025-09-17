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
/*!        \file  intermediate_event_group_object.h
 *        \brief  Internal representation of the service event groups.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_INTERMEDIATE_EVENT_GROUP_OBJECT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_INTERMEDIATE_EVENT_GROUP_OBJECT_H_
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
 * \brief This is an intermediate representation of an event group object which is filled by a parser.
 * \details The validator then takes this object and validates if mandatory elements are set or not.
 */
class EventGroupConfigObject final {
 public:
  /*!
   * \brief Event group ID element of this service.
   */
  using IdElement = CfgElement<someip_protocol::internal::EventgroupId>;

  /*!
   * \brief Event ID container element of this service.
   */
  using EventIdContainerElement = CfgElement<ConfigurationTypesAndDefs::EventIdContainer>;

  /*!
   * \brief Constructor sets the references to the elements of the structure to write to.
   * \param[in] event_group Reference to the event group structure to write to during parsing.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  explicit EventGroupConfigObject(ConfigurationTypesAndDefs::Eventgroup& event_group) noexcept
      : id_{event_group.id_}, events_container_{event_group.events_} {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~EventGroupConfigObject() = default;

  /*!
   * \brief Deleted copy constructor.
   */
  EventGroupConfigObject(EventGroupConfigObject const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  EventGroupConfigObject(EventGroupConfigObject&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(EventGroupConfigObject const&) & -> EventGroupConfigObject& = delete;

  /*!
   * \brief Move assignment is used for resetting.
   * \steady FALSE
   */
  auto operator=(EventGroupConfigObject&& other) & noexcept -> EventGroupConfigObject& = default;

  /*!
   * \brief Set the event group ID .
   * \param[in] id SOME/IP event group ID.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetId(someip_protocol::internal::EventgroupId const& id) noexcept { id_.SetElement(id); }

  /*!
   * \brief Getter for the event group ID element.
   * \return A reference to event group ID element of this service.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  IdElement const& GetId() const noexcept { return id_; }

  /*!
   * \brief Set the container of event identifier after parsing.
   * \param[in] events event identifier container.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetEventIdContainer(ConfigurationTypesAndDefs::EventIdContainer const& events) noexcept {
    events_container_.SetElement(events);
  }

 private:
  /*!
   * \brief The event group ID element.
   */
  IdElement id_;

  /*!
   * \brief Events contained in this event group.
   */
  EventIdContainerElement events_container_;
};

}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_INTERMEDIATE_EVENT_GROUP_OBJECT_H_
