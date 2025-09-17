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
 *        \brief  Read-only interface for field configuration.
 *      \details
 *********************************************************************************************************************/

#ifndef LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_FIELD_CONFIG_INTERFACE_H_
#define LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_FIELD_CONFIG_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>

#include "ara/core/optional.h"

#include "amsr/comtrace/internal/config/config_types.h"
#include "amsr/comtrace/internal/config/event_config_interface.h"
#include "amsr/comtrace/internal/config/method_config_interface.h"

namespace amsr {
namespace comtrace {
namespace internal {
namespace config {

/*!
 * \brief Read-only interface for field configuration.
 *
 * \unit ComTrace::TraceConfig
 */
class FieldConfigInterface {
 public:
  auto operator=(FieldConfigInterface const&) noexcept -> FieldConfigInterface& = delete;
  FieldConfigInterface(FieldConfigInterface&&) noexcept = delete;
  auto operator=(FieldConfigInterface&&) noexcept -> FieldConfigInterface& = delete;

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
  virtual ~FieldConfigInterface() noexcept = default;

  /*!
   * \brief Gets the shortname.
   *
   * \return The shortname.
   *
   * \pre         Config object was filled by config loader.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto GetShortname() const noexcept -> Shortname = 0;

  /*!
   * \brief Get the configuration for field operation type "Getter".
   *
   * \return The getter configuration.
   *
   * \pre         Config object was filled by config loader.
   * \pre         The field getter must not be empty.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto GetGetter() const noexcept -> ::ara::core::Optional<MethodConfigInterfaceRef const> = 0;

  /*!
   * \brief Get the configuration for field operation type "Setter".
   *
   * \return The setter configuration.
   *
   * \pre         Config object was filled by config loader.
   * \pre         The field setter must not be empty.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto GetSetter() const noexcept -> ::ara::core::Optional<MethodConfigInterfaceRef const> = 0;

  /*!
   * \brief Get the configuration for field operation type "Notifier".
   *
   * \return The notifier configuration.
   *
   * \pre         Config object was filled by config loader.
   * \pre         The field notifier must not be empty.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto GetNotifier() const noexcept -> ::ara::core::Optional<EventConfigInterfaceRef const> = 0;

 protected:
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
  FieldConfigInterface() noexcept = default;

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
  FieldConfigInterface(FieldConfigInterface const&) noexcept = default;
};

/*! Type name alias for reference of field config. */
using FieldConfigInterfaceRef = std::reference_wrapper<FieldConfigInterface const>;

}  // namespace config
}  // namespace internal
}  // namespace comtrace
}  // namespace amsr

#endif  // LIB_COM_TRACE_INCLUDE_AMSR_COMTRACE_INTERNAL_CONFIG_FIELD_CONFIG_INTERFACE_H_
