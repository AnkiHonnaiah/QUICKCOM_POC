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
 *        \brief  Runtime interface definition.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_RUNTIME_INTERFACE_H_
#define LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_RUNTIME_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/instance_specifier.h"
#include "amsr/socal/internal/configuration/configuration.h"
#include "amsr/socal/internal/types.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "amsr/timer/threadsafe_timer_manager.h"
#include "ara/com/instance_identifier.h"
#include "osabstraction/io/reactor1/reactor1.h"

namespace amsr {
namespace ipc_binding {
namespace internal {

/*!
 * \brief   Interface to access the runtime.
 * \details The Interface is used to abstract the access to the runtime object provided by Socal in the productive
 *          environment.
 *
 * \unit IpcBinding::Runtime
 */
class RuntimeInterface {
 public:
  /*!
   * \brief Use default constructor.
   *
   * \pre     -
   * \context Init
   * \steady  FALSE
   */
  RuntimeInterface() noexcept = default;

  /*!
   * \brief Use default destructor.
   *
   * \pre     -
   * \context Shutdown
   * \steady  FALSE
   */
  virtual ~RuntimeInterface() noexcept = default;

  RuntimeInterface(RuntimeInterface const&) noexcept = delete;
  RuntimeInterface(RuntimeInterface&&) noexcept = delete;
  auto operator=(RuntimeInterface const&) noexcept -> RuntimeInterface& = delete;
  auto operator=(RuntimeInterface&&) noexcept -> RuntimeInterface& = delete;

  /*!
   * \brief Get the used Steady Timer Manager.
   *
   * \return Timer Manager.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto GetSteadyTimerManager() noexcept -> amsr::steady_timer::TimerManagerInterface& = 0;

  /*!
   * \brief Get the used Reactor.
   *
   * \return Reactor.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto GetReactor() noexcept -> osabstraction::io::reactor1::Reactor1& = 0;

  /*!
   * \brief Map an InstanceIdentifier to the corresponding P-Port InstanceSpecifier.
   *
   * \param[in] instance_specifier   The instance specifier which will be the key for the new entry.
   * \param[in] instance_identifier  The instance identifier which will be the value for the new entry. Expected format:
   *                                 "<binding_name>:<binding specific instance ID>".
   *
   * \pre         The same InstanceIdentifier and InstanceSpecifier combination must have not been mapped earlier.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto MapProvidedInstanceSpecifierToInstanceId(
      amsr::core::InstanceSpecifier const& instance_specifier,
      ara::com::InstanceIdentifier const& instance_identifier) noexcept -> void = 0;

  /*!
   * \brief Map an InstanceIdentifier to the corresponding R-Port InstanceSpecifier.
   *
   * \param[in] instance_specifier   The instance specifier which will be the key for the new entry.
   * \param[in] instance_identifier  The instance identifier which will be the value for the new entry. Expected format:
   *                                 "<binding_name>:<binding specific instance ID>".
   *
   * \pre         The same InstanceIdentifier and InstanceSpecifier combination must have not been mapped earlier.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto MapRequiredInstanceSpecifierToInstanceId(
      amsr::core::InstanceSpecifier const& instance_specifier,
      ara::com::InstanceIdentifier const& instance_identifier) noexcept -> void = 0;

  /*!
   * \brief Removes the given InstanceIdentifier from the provided instance specifier table.
   *
   * \param[in] instance_specifier   The instance specifier which will be the key for the entry to be removed.
   * \param[in] instance_identifier  The instance identifier which will be the value for the entry to be removed.
   *
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto RemoveProvidedInstanceSpecifierEntry(amsr::core::InstanceSpecifier const& instance_specifier,
                                                    ara::com::InstanceIdentifier const& instance_identifier) noexcept
      -> void = 0;

  /*!
   * \brief Removes the given InstanceIdentifier from the required instance specifier table.
   *
   * \param[in] instance_specifier   The instance specifier which will be the key for the entry to be removed.
   * \param[in] instance_identifier  The instance identifier which will be the value for the entry to be removed.
   *
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto RemoveRequiredInstanceSpecifierEntry(amsr::core::InstanceSpecifier const& instance_specifier,
                                                    ara::com::InstanceIdentifier const& instance_identifier) noexcept
      -> void = 0;
};

}  // namespace internal
}  // namespace ipc_binding
}  // namespace amsr

#endif  // LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_RUNTIME_INTERFACE_H_
