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
/*!        \file  someip_binding/internal/runtime_interface.h
 *        \brief  Runtime interface definition.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_RUNTIME_INTERFACE_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_RUNTIME_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/instance_specifier.h"
#include "amsr/core/string_view.h"
#include "amsr/socal/internal/configuration/configuration.h"
#include "amsr/socal/internal/types.h"
#include "amsr/someip_binding/internal/configuration/someipbinding_config.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "ara/com/instance_identifier.h"
#include "osabstraction/io/reactor1/reactor1.h"
#include "vac/container/c_string_view.h"

namespace amsr {
namespace someip_binding {
namespace internal {

/*!
 * \brief   Interface to access the runtime.
 * \details The Interface is used to abstract the access to the runtime object provided by Socal in the productive
 *          environment.
 */
class RuntimeInterface {
 public:
  /*!
   * \brief Use default constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  RuntimeInterface() noexcept = default;

  /*!
   * \brief Use default destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~RuntimeInterface() noexcept = default;

  RuntimeInterface(RuntimeInterface const&) = delete;
  RuntimeInterface(RuntimeInterface&&) = delete;
  RuntimeInterface& operator=(RuntimeInterface const&) & = delete;
  RuntimeInterface& operator=(RuntimeInterface&&) & = delete;

  /*!
   * \brief       Get the used Reactor.
   * \return      Reactor.
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
  virtual osabstraction::io::reactor1::Reactor1& GetReactor() noexcept = 0;

  /*!
   * \brief Returns reference to the steady TimerManager object.
   * \return reference to the timer manager
   *
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ::amsr::steady_timer::TimerManagerInterface& GetSteadyTimerManager() noexcept = 0;

  /*!
   * \brief       Get the function that triggers the polling API in Socal.
   * \return      A function that triggers Socal's polling API.
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
  virtual vac::language::UniqueFunction<void(), ara::core::PolymorphicAllocator<std::uint8_t>>
  GetProcessPollingFunction() noexcept = 0;

  /*!
   * \brief Add a new mapping entry between a ProvidedInstanceSpecifier and a InstanceIdentifier.
   * \details This API must be called multiple times to register all known mappings.
   *
   * \param[in] instance_specifier The instance specifier which will be the key for the new entry.
   * \param[in] instance_identifier The instance identifier which will be the value for the new entry.
   *                                Expected format:"<binding_name>:<binding specific instance ID>".
   * \pre The same InstanceIdentifier and InstanceSpecifier combination must have not been mapped earlier.
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Vector component internal API
   * \synchronous TRUE
   */
  virtual void MapProvidedInstanceSpecifierToInstanceId(
      amsr::core::InstanceSpecifier const& instance_specifier,
      ara::com::InstanceIdentifier const& instance_identifier) noexcept = 0;

  /*!
   * \brief Remove mapping entry between a ProvidedInstanceSpecifier and a InstanceIdentifier.
   * \details This API must be called multiple times to remove all known mappings.
   *
   * \param[in] instance_specifier The instance specifier which will be the key for the new entry.
   * \param[in] instance_identifier The instance identifier which will be the value for the new entry. Expected format:
   * "<Binding type/prefix>:<binding specific instance ID>"
   * \pre The InstanceIdentifier and InstanceSpecifier combination must have been mapped earlier.
   * \context Shutdown
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Vector component internal API
   * \synchronous TRUE
   */
  virtual void RemoveProvidedInstanceSpecifierEntry(
      amsr::core::InstanceSpecifier const& instance_specifier,
      ara::com::InstanceIdentifier const& instance_identifier) noexcept = 0;

  /*!
   * \brief Add a new mapping entry between a RequiredInstanceSpecifier and a InstanceIdentifier.
   * \details This API must be called multiple times to register all known mappings.
   *
   * \param[in] instance_specifier The instance specifier which will be the key for the new entry.
   * \param[in] instance_identifier The instance identifier which will be the value for the new entry.
   *                                Expected format: "<binding_name>:<binding specific instance ID>".
   * \pre The same InstanceIdentifier and InstanceSpecifier combination must have not been mapped earlier.
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Vector component internal API
   * \synchronous TRUE
   */
  virtual void MapRequiredInstanceSpecifierToInstanceId(
      amsr::core::InstanceSpecifier const& instance_specifier,
      ara::com::InstanceIdentifier const& instance_identifier) noexcept = 0;

  /*!
   * \brief Remove mapping entry between a RequiredInstanceSpecifier and a InstanceIdentifier.
   * \details This API must be called multiple times to remove all known mappings.
   *
   * \param[in] instance_specifier The instance specifier which will be the key for the new entry.
   * \param[in] instance_identifier The instance identifier which will be the value for the new entry. Expected format:
   * "<Binding type/prefix>:<binding specific instance ID>"
   * \pre The InstanceIdentifier and InstanceSpecifier combination must have been mapped earlier.
   * \context Shutdown
   * \threadsafe FALSE
   * \reentrant FALSE
   * \vprivate Vector component internal API
   * \synchronous TRUE
   */
  virtual void RemoveRequiredInstanceSpecifierEntry(
      amsr::core::InstanceSpecifier const& instance_specifier,
      ara::com::InstanceIdentifier const& instance_identifier) noexcept = 0;
};

}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_RUNTIME_INTERFACE_H_
