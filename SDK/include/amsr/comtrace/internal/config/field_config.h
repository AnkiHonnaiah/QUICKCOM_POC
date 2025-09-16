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
 *      \details
 *********************************************************************************************************************/

#ifndef LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_FIELD_CONFIG_H_
#define LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_FIELD_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/optional.h"

#include "amsr/comtrace/internal/config/config_types.h"
#include "amsr/comtrace/internal/config/event_config.h"
#include "amsr/comtrace/internal/config/event_config_interface.h"
#include "amsr/comtrace/internal/config/field_config_interface.h"
#include "amsr/comtrace/internal/config/method_config.h"
#include "amsr/comtrace/internal/config/method_config_interface.h"

namespace amsr {
namespace comtrace {
namespace internal {
namespace config {

/*!
 * \brief   Field configuration.
 * \details Configuration class storing the configuration parameters for a field.
 *
 * \unit ComTrace::TraceConfig
 */
class FieldConfig final : public FieldConfigInterface {
 public:
  /*!
   * \brief Construct an empty field configuration.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  FieldConfig() noexcept = default;

  /*!
   * \brief Copy-Construct a field configuration.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  FieldConfig(FieldConfig const&) noexcept = default;

  auto operator=(FieldConfig const&) noexcept -> FieldConfig& = delete;
  FieldConfig(FieldConfig&&) noexcept = delete;
  auto operator=(FieldConfig&&) noexcept -> FieldConfig& = delete;

  /*!
   * \brief Destroy field configuration.
   *
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~FieldConfig() noexcept final = default;  // VCA_COMTRACE_COMPILER_GENERATED_FUNCTIONS

  /*!
   * \copydoc amsr::comtrace::internal::config::FieldConfigInterface::GetShortname()
   *
   * \spec requires true; \endspec
   */
  auto GetShortname() const noexcept -> Shortname final;

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
  auto SetShortname(Shortname const& shortname) noexcept -> void;

  /*!
   * \copydoc amsr::comtrace::internal::config::FieldConfigInterface::GetShortname()
   *
   * \spec requires true; \endspec
   */
  auto GetGetter() const noexcept -> ::ara::core::Optional<MethodConfigInterfaceRef const> final;

  /*!
   * \brief Set the configuration for field operation type "Getter".
   *
   * \param[in] config  The getter configuration.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE.
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto SetGetter(MethodConfig const& config) noexcept -> void;

  /*!
   * \copydoc amsr::comtrace::internal::config::FieldConfigInterface::GetShortname()
   *
   * \spec requires true; \endspec
   */
  auto GetSetter() const noexcept -> ::ara::core::Optional<MethodConfigInterfaceRef const> final;

  /*!
   * \brief Set the configuration for field operation type "Setter".
   *
   * \param[in] config  The setter configuration.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE.
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto SetSetter(MethodConfig const& config) noexcept -> void;

  /*!
   * \copydoc amsr::comtrace::internal::config::FieldConfigInterface::GetShortname()
   *
   * \spec requires true; \endspec
   */
  auto GetNotifier() const noexcept -> ::ara::core::Optional<EventConfigInterfaceRef const> final;

  /*!
   * \brief Set the configuration for field operation type "Notifier".
   *
   * \param[in] config  The notifier configuration.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE.
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto SetNotifier(EventConfig const& config) noexcept -> void;

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

}  // namespace config
}  // namespace internal
}  // namespace comtrace
}  // namespace amsr

#endif  // LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_FIELD_CONFIG_H_
