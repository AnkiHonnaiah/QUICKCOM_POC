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
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_RUNTIME_CONFIGURATION_FIELD_CONFIG_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_RUNTIME_CONFIGURATION_FIELD_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>

#include "ara/core/optional.h"

#include "ipc_binding_core/internal/ipc_protocol/ipc_protocol_types.h"
#include "ipc_binding_core/internal/runtime_configuration/config_types.h"
#include "ipc_binding_core/internal/runtime_configuration/event_config.h"
#include "ipc_binding_core/internal/runtime_configuration/method_config.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace runtime_configuration {

/*!
 * \brief   Field configuration.
 * \details Configuration class storing the configuration parameters for a field.
 *
 * \unit IpcBinding::IpcBindingCore::RuntimeConfiguration
 */
class FieldConfig final {
 public:
  /*! Type name alias for mutable reference of event config. */
  using MutableEventConfigRef = std::reference_wrapper<EventConfig>;

  /*! Type name alias for mutable reference of method config. */
  using MutableMethodConfigRef = std::reference_wrapper<MethodConfig>;

  /*!
   * \brief Construct an empty field configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  FieldConfig() noexcept = default;

  /*!
   * \brief Destroy field configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~FieldConfig() noexcept = default;

  /*!
   * \brief Copy-Construct a field configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  FieldConfig(FieldConfig const&) = default;

  /*!
   * \brief Move-construct a field configuration.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  FieldConfig(FieldConfig&&) noexcept = default;

  auto operator=(FieldConfig const&) noexcept -> FieldConfig& = delete;
  auto operator=(FieldConfig&&) noexcept -> FieldConfig& = delete;

  /*!
   * \brief Set the configuration for field getter.
   *
   * \param[in] config  The getter configuration.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  void SetGetter(MethodConfig const& config) noexcept;

  /*!
   * \brief Get the configuration for field getter.
   *
   * \return The getter configuration.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \pre         The field getter must not be empty.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetGetterConfigChecked() const noexcept -> MethodConfig const&;

  /*!
   * \brief Get a reference to the configuration for the field getter if it is present.
   *
   * \return Optional containing the getter configuration.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetGetterConfig() noexcept -> ara::core::Optional<MutableMethodConfigRef>;

  /*!
   * \brief Check if field has getter.
   *
   * \return True if field has getter.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto HasGetter() const noexcept -> bool;

  /*!
   * \brief Set the configuration for the field setter.
   *
   * \param[in] config  The setter configuration.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  void SetSetter(MethodConfig const& config) noexcept;

  /*!
   * \brief Get the configuration for the field setter.
   *
   * \return The setter configuration.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \pre         The field setter must not be empty.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetSetterConfigChecked() const noexcept -> MethodConfig const&;

  /*!
   * \brief Get a reference to the configuration for the field setter if it is present.
   *
   * \return Optional containing the setter configuration.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetSetterConfig() noexcept -> ara::core::Optional<MutableMethodConfigRef>;

  /*!
   * \brief Check if field has setter.
   *
   * \return True if field has setter.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto HasSetter() const noexcept -> bool;

  /*!
   * \brief Set the configuration for field notifier.
   *
   * \param[in] config  The notifier configuration.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  void SetNotifier(EventConfig const& config) noexcept;

  /*!
   * \brief Get the configuration for field notifier.
   *
   * \return The notifier configuration.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \pre         The field notifier must not be empty.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetNotifierConfigChecked() const noexcept -> EventConfig const&;

  /*!
   * \brief Get a reference to the configuration for the field notifier if it is present.
   *
   * \return Optional containing the notifier configuration.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetNotifierConfig() noexcept -> ara::core::Optional<MutableEventConfigRef>;

  /*!
   * \brief Check if field has notifier.
   *
   * \return True if field has notifier.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto HasNotifier() const noexcept -> bool;

  /*!
   * \brief Gets the shortname.
   *
   * \return The shortname.
   *
   * \pre         Config object was set by runtime configuration loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto GetShortname() const noexcept -> Shortname;

  /*!
   * \brief Sets the shortname.
   *
   * \param[in] shortname  The shortname.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  void SetShortname(Shortname const& shortname) noexcept;

 private:
  /*! Short name for the field. */
  Shortname field_shortname_{};
  /*! Getter for the field. */
  ara::core::Optional<MethodConfig> field_getter_{};
  /*! Setter for the field. */
  ara::core::Optional<MethodConfig> field_setter_{};
  /*! Notifier for the field. */
  ara::core::Optional<EventConfig> field_notifier_{};
};

}  // namespace runtime_configuration
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_RUNTIME_CONFIGURATION_FIELD_CONFIG_H_
