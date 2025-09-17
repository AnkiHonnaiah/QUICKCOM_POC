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
/*!        \file  notifier_config.h
 *        \brief  Notifier configuration class.
 *        \unit   SomeIpBinding::SomeIpBinding::Configuration::JsonParser::JsonConfigTypes
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_NOTIFIER_CONFIG_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_NOTIFIER_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/optional.h"
#include "amsr/someip_binding/internal/configuration/config_types.h"
#include "amsr/someip_binding/internal/configuration/event_config.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_binding {
namespace internal {
namespace configuration {

/*!
 * \brief Notifier deployment configuration.
 * \details Configuration class storing the configuration parameters for a notifier deployment.
 *          Stored configuration parameters:
 *          - NotifierId
 *          - Serialization [optional]
 *          - DisableE2eCheck [optional]
 *          - DisableSessionHandling [optional]
 *
 */
class NotifierConfig final {
 public:
  /*!
   * \brief Name alias for EventConfig::Serialization
   */
  using Serialization = EventConfig::Serialization;

  /*! Name alias for event identifier type. */
  using NotifierId = ::amsr::someip_protocol::internal::EventId;

  /*! Type-alias for the disable E2E check type. */
  using DisableE2eCheck = bool;

  /*! Type-alias for the disable session handling type. */
  using DisableSessionHandling = bool;

  /*!
   * \brief Set the id of the notifier.
   * \details The id defines the id of the notifier.
   * \param[in] id The notifier id.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE.
   * \spec
   *   requires true;
   * \endspec
   */
  void SetNotifierId(NotifierId const id) noexcept;

  /*!
   * \brief Get the id of the notifier.
   * \return The id of the notifier.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  NotifierId GetNotifierId() const noexcept;

  /*!
   * \brief Set the disable E2E check value.
   * \param[in] disable_e2e_check The disable E2E check value.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE.
   * \spec
   *   requires true;
   * \endspec
   */
  void SetDisableE2eCheck(DisableE2eCheck const disable_e2e_check) noexcept;

  /*!
   * \brief Get the disable E2E check value.
   * \return The disable E2E check value.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Optional<DisableE2eCheck> const& GetDisableE2eCheck() const noexcept;

  /*!
   * \brief Set the disable session handling value.
   * \param[in] disable_session_handling The disable session handling value.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE.
   * \spec
   *   requires true;
   * \endspec
   */
  void SetDisableSessionHandling(DisableSessionHandling const disable_session_handling) noexcept;

  /*!
   * \brief Get the disable session handling value.
   * \return The disable session handling value.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Optional<DisableSessionHandling> const& GetDisableSessionHandling() const noexcept;

 private:
  NotifierId notifier_id_{};                                                /*!< The notifier identifier. */
  amsr::core::Optional<DisableE2eCheck> disable_e2e_check_{};               /*!< The disable E2E check value */
  amsr::core::Optional<DisableSessionHandling> disable_session_handling_{}; /*!< The disable session handling value */
};

}  // namespace configuration
}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_CONFIGURATION_NOTIFIER_CONFIG_H_
