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
/*!        \file  life_cycle_manager_interface.h
 *        \brief  -
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_LIFE_CYCLE_MANAGER_INTERFACE_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_LIFE_CYCLE_MANAGER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/someip_binding/internal/configuration/config_types.h"
#include "amsr/someip_binding/internal/configuration/service_config_wrapper_interface.h"
#include "amsr/someip_binding_core/internal/connection_action.h"
#include "amsr/someip_binding_core/internal/someip_binding_core_interface.h"
#include "ara/core/vector.h"

namespace amsr {
namespace someip_binding {
namespace internal {

/*!
 * \brief Interface class for LifeCycleManager.
 */
class LifeCycleManagerInterface {
 public:
  /*!
   * \brief Type alias for the SOME/IP Binding Core.
   */
  using SomeIpBindingCore = amsr::someip_binding_core::internal::SomeIpBindingCoreInterface;

  /*!
   * \brief Type alias for a container holding SOME/IP Binding Core instances.
   */
  using SomeIpBindingCoreContainer = ::ara::core::Vector<std::shared_ptr<SomeIpBindingCore>>;

  /*!
   * \brief Type alias for the connection actions.
   */
  using ConnectionAction = ::amsr::someip_binding_core::internal::ConnectionAction;

  /*!
   * \brief Type alias for a container holding connection action instances.
   */
  using ConnectionActionContainer = ::ara::core::Vector<std::unique_ptr<ConnectionAction>>;

  /*!
   * \brief Define default constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  LifeCycleManagerInterface() = default;

  /*!
   * \brief Use default destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~LifeCycleManagerInterface() noexcept = default;

  LifeCycleManagerInterface(LifeCycleManagerInterface const&) = delete;
  LifeCycleManagerInterface(LifeCycleManagerInterface&&) = delete;
  LifeCycleManagerInterface& operator=(LifeCycleManagerInterface const&) & = delete;
  LifeCycleManagerInterface& operator=(LifeCycleManagerInterface&&) & = delete;

  /*!
   * \brief Add a SomeIpBindingCore and the corresponding ConnectionAction.
   *
   * \param[in]   someip_binding_core  The SomeIpBindingCore instance.
   * \param[in]   connection_action The connection action for the specific SomeIpBindingCore instance.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  virtual void AddSomeIpBindingCore(std::shared_ptr<SomeIpBindingCore> someip_binding_core,
                                    std::unique_ptr<ConnectionAction>&& connection_action) = 0;

  /*!
   * \brief Stores a service config wrapper object.
   *
   * \param       config_wrapper_uptr Unique pointer to the service config wrapper.
   * \pre -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires config_wrapper_uptr!= nullptr;
   * \endspec
   */
  virtual void SetServiceConfigWrapper(
      std::unique_ptr<configuration::ServiceConfigWrapperInterface>&& config_wrapper_uptr) noexcept = 0;

  /*!
   * \brief Provides access to the service config wrapper object.
   *
   * \return      Service config wrapper object
   * \pre         SetServiceConfigWrapper() was called to set service config wrapper object.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  virtual configuration::ServiceConfigWrapperInterface& GetServiceConfigWrapper() noexcept = 0;

  /*!
   * \brief Gets the ValidatedServiceConfig associated with the given shortname path.
   *
   * \param[in]   service_shortname_path  The shortname path of the service.
   * \return      Reference to the ValidatedServiceConfig object.
   * \pre         SetServiceConfigWrapper() was called to set service config wrapper object.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  virtual configuration::ServiceConfigWrapperInterface::ValidatedServiceConfigRef GetServiceConfig(
      configuration::ShortnamePath const& service_shortname_path) noexcept = 0;

  /*!
   * \brief       Access to the SomeIpBindingCore instances.
   * \return      SomeIpBinding core instances.
   *
   * \pre         Binding is initialized.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  virtual SomeIpBindingCoreContainer const& GetBindingCores() const noexcept = 0;

  /*!
   * \brief Provides access to the connection action objects.
   *
   * \return      Connection action objects.
   * \pre         SetConnectionAction() was called to set connection action objects.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  virtual ConnectionActionContainer const& GetConnectionActions() const noexcept = 0;
};

}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_LIFE_CYCLE_MANAGER_INTERFACE_H_
