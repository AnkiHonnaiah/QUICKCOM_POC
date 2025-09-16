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
/*!        \file  someip_method_event_props_object.h
 *        \brief  Intermediate parser representation of both SomeipEventProps and SomeipMethodProps
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SOMEIP_METHOD_EVENT_PROPS_OBJECT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SOMEIP_METHOD_EVENT_PROPS_OBJECT_H_

#include <utility>
#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"

#include "someip-protocol/internal/message.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief Intermediate parser representation of both SomeipEventProps and SomeipMethodProps
 * \details The validator then takes this object and validates if mandatory elements are set or not.
 */
class SomeipMethodEventPropsObject final {
 public:
  /*!
   * \brief Destination type to write into.
   */
  using ValueType = ConfigurationTypesAndDefs::SomeipMethodEventProps;

  /*!
   * \brief Method/eventID element of this someip daemon posix json object.
   */
  using IdElement = CfgElement<decltype(std::declval<ValueType>().id)>;

  /*!
   * \brief Timeout element of this someip daemon posix json object.
   */
  using TimeoutElement = CfgElement<decltype(std::declval<ValueType>().timeout)>;

  /*!
   * \brief Trigger element of this someip daemon posix json object.
   */
  using TriggerElement = CfgElement<decltype(std::declval<ValueType>().is_trigger)>;

  /*!
   * \brief Constructor sets the references to the elements of the structure to write to.
   * \param[in] props Reference to the SomeipMethodEventProps structure to write to during parsing.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  explicit SomeipMethodEventPropsObject(ConfigurationTypesAndDefs::SomeipMethodEventProps& props) noexcept
      : id_{props.id}, timeout_{props.timeout}, is_trigger_{props.is_trigger} {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~SomeipMethodEventPropsObject() = default;

  /*!
   * \brief Deleted copy constructor.
   */
  SomeipMethodEventPropsObject(SomeipMethodEventPropsObject const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  SomeipMethodEventPropsObject(SomeipMethodEventPropsObject&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(SomeipMethodEventPropsObject const&) & -> SomeipMethodEventPropsObject& = delete;

  /*!
   * \brief Move assignment is used for resetting.
   * \steady FALSE
   */
  auto operator=(SomeipMethodEventPropsObject&& other) & noexcept -> SomeipMethodEventPropsObject& = default;

  /*!
   * \brief Set the id element.
   * \param[in] value The id
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetId(IdElement::ElementTypeConstRef value) noexcept { id_.SetElement(value); }

  /*!
   * \brief Getter for the id element.
   * \return The id element.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  IdElement const& GetId() const noexcept { return id_; }

  /*!
   * \brief Set the timeout element.
   * \param[in] value The timeout
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetTimeout(TimeoutElement::ElementTypeConstRef value) noexcept { timeout_.SetElement(value); }

  /*!
   * \brief Getter for the timeout element.
   * \return The timeout.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  TimeoutElement const& GetTimeout() const noexcept { return timeout_; }

  /*!
   * \brief Set the trigger.
   * \param[in] value The trigger
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetIsTrigger(TriggerElement::ElementTypeConstRef value) noexcept { is_trigger_.SetElement(value); }

  /*!
   * \brief Getter for the trigger.
   * \return The trigger.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  TriggerElement const& GetIsTrigger() const noexcept { return is_trigger_; }

 private:
  /*!
   * \brief The id_ element.
   */
  IdElement id_;

  /*!
   * \brief The timeout_ element.
   */
  TimeoutElement timeout_;

  /*!
   * \brief The is_trigger_ element.
   */
  TriggerElement is_trigger_;
};

}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SOMEIP_METHOD_EVENT_PROPS_OBJECT_H_
