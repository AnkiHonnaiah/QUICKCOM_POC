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
/*!        \file  config_wrapper_interface.h
 *        \brief  Interface for validation wrapper of config of services.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_CONFIG_WRAPPER_INTERFACE_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_CONFIG_WRAPPER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "amsr/someip_binding/internal/configuration/event_config.h"
#include "amsr/someip_binding/internal/configuration/method_config.h"
#include "amsr/someip_binding/internal/configuration/notifier_config.h"
#include "amsr/someip_binding/internal/configuration/operation_config.h"

namespace amsr {
namespace someip_binding {
namespace internal {
namespace configuration {

/*!
 * \brief Interface defining save config api for xf objects.
 */
class ConfigWrapperInterface {
 public:
  /*!
   * \brief Use default constructor.
   * \pre -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ConfigWrapperInterface() noexcept = default;

  /*!
   * \brief Destructor.
   * \pre -
   * \context App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~ConfigWrapperInterface() = default;

  ConfigWrapperInterface(ConfigWrapperInterface const&) = delete;
  ConfigWrapperInterface(ConfigWrapperInterface&&) = delete;
  ConfigWrapperInterface& operator=(ConfigWrapperInterface const&) & = delete;
  ConfigWrapperInterface& operator=(ConfigWrapperInterface&&) & = delete;

  /*!
   * \brief Getter for EventConfig.
   * \param[in] shortname The shortname of the associated event of the EventConfig.
   * \return The EventConfig.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual EventConfig const& GetEventConfig(
      ::amsr::someip_binding::internal::configuration::Shortname const& shortname) const noexcept = 0;

  /*!
   * \brief Getter for MethodConfig.
   * \param[in] shortname The shortname of the associated method of the MethodConfig.
   * \return The MethodConfig.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual MethodConfig const& GetMethodConfig(
      ::amsr::someip_binding::internal::configuration::Shortname const& shortname) const noexcept = 0;

  /*!
   * \brief Getter for NotifierConfig.
   * \param[in] shortname The shortname of the associated field of the NotifierConfig.
   * \return The NotifierConfig.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual NotifierConfig const& GetNotifierConfig(
      ::amsr::someip_binding::internal::configuration::Shortname const& shortname) const noexcept = 0;

  /*!
   * \brief Getter for setter OperationConfig.
   * \param[in] shortname The shortname of the associated field of the setter OperationConfig.
   * \return The setter OperationConfig.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual OperationConfig const& GetSetterConfig(
      ::amsr::someip_binding::internal::configuration::Shortname const& shortname) const noexcept = 0;

  /*!
   * \brief Getter for the getter OperationConfig.
   * \param[in] shortname The shortname of the associated field of the getter OperationConfig.
   * \return The getter OperationConfig.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual OperationConfig const& GetGetterConfig(
      ::amsr::someip_binding::internal::configuration::Shortname const& shortname) const noexcept = 0;
};

}  // namespace configuration
}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_CONFIG_WRAPPER_INTERFACE_H_
