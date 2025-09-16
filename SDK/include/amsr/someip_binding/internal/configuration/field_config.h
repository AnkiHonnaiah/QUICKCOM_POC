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
/*!        \file
 *        \brief  Field configuration class.
 *        \unit   SomeIpBinding::SomeIpBinding::Configuration::JsonParser::JsonConfigTypes
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_FIELD_CONFIG_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_FIELD_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/optional.h"
#include "amsr/someip_binding/internal/configuration/config_types.h"
#include "amsr/someip_binding/internal/configuration/notifier_config.h"
#include "amsr/someip_binding/internal/configuration/operation_config.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_binding {
namespace internal {
namespace configuration {

/*!
 * \brief Field configuration.
 * \details Configuration class storing the configuration parameters for a field.
 *          Stored configuration parameters:
 *          - Shortname
 *          - Notifier [optional]
 *          - Setter [optional]
 *          - Getter [optional]
 *
 */
class FieldConfig final {
 public:
  /*!
   * \brief Sets the shortname.
   * \param shortname The shortname.
   *
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  void SetShortname(Shortname const& shortname) noexcept;

  /*!
   * \brief  Gets the shortname.
   * \return The shortname.
   *
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  Shortname const& GetShortname() const noexcept;

  /*!
   * \brief Set the Notifier.
   * \details The notifier defines the Notifier of the field.
   * \param[in] notifier The notifier.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE.
   * \spec
   *   requires true;
   * \endspec
   */
  void SetNotifier(NotifierConfig const notifier) noexcept;

  /*!
   * \brief Get the Notifier.
   * \return The notifier.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Optional<NotifierConfig> const& GetNotifier() const noexcept;

  /*!
   * \brief Set the Setter.
   * \details The setter defines the Setter of the field.
   * \param[in] setter The Setter.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE.
   * \spec
   *   requires true;
   * \endspec
   */
  void SetSetter(OperationConfig const setter) noexcept;

  /*!
   * \brief Get the Setter.
   * \return The setter.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Optional<OperationConfig> const& GetSetter() const noexcept;

  /*!
   * \brief Set the Getter.
   * \details The getter defines the Getter of the field.
   * \param[in] getter The Getter.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE.
   * \spec
   *   requires true;
   * \endspec
   */
  void SetGetter(OperationConfig const getter) noexcept;

  /*!
   * \brief Get the Getter.
   * \return The getter.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Optional<OperationConfig> const& GetGetter() const noexcept;

 private:
  Shortname shortname_{};                           /*!< The Short name  */
  amsr::core::Optional<NotifierConfig> notifier_{}; /*!< The Notifier */
  amsr::core::Optional<OperationConfig> setter_{};  /*!< The Setter */
  amsr::core::Optional<OperationConfig> getter_{};  /*!< The Getter */
};

}  // namespace configuration
}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_FIELD_CONFIG_H_
