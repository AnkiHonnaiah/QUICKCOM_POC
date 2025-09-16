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
/*!        \file  someip_binding/internal/life_cycle_manager.h
 *        \brief  Holds the global instantiation of SOME/IP binding.
 *        \unit   SomeIpBinding::SomeIpBinding::LifeCycleManager
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_LIFE_CYCLE_MANAGER_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_LIFE_CYCLE_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/core/optional.h"
#include "amsr/generic/singleton_wrapper.h"
#include "amsr/someip_binding/internal/configuration/service_config_wrapper_interface.h"
#include "amsr/someip_binding/internal/life_cycle_manager_interface.h"
#include "amsr/someip_binding/internal/runtime_interface.h"

namespace amsr {
namespace someip_binding {
namespace internal {

/*!
 * \brief Class that holds the global instantiation of SOME/IP binding.
 */
class LifeCycleManager final : public LifeCycleManagerInterface {
 public:
  /*!
   * \brief Type alias for the SOME/IP Binding Core instances.
   */
  using SomeIpBindingCoreContainer = LifeCycleManagerInterface::SomeIpBindingCoreContainer;

  /*!
   * \brief Type alias for connection action to SomeIpDaemon.
   */
  using ConnectionActionContainer = LifeCycleManagerInterface::ConnectionActionContainer;

  /*!
   * \brief Type alias for SomeIpBindingConfig.
   */
  using SomeIpBindingConfig = ::amsr::someip_binding::internal::configuration::SomeIpBindingConfig;

  /*!
   * \brief Constructor.
   *
   * \param[in]   someip_binding_config The runtime configuration of the binding.
   * \param[in]   runtime               The runtime instance.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  LifeCycleManager(std::unique_ptr<SomeIpBindingConfig>&& someip_binding_config,
                   std::unique_ptr<RuntimeInterface>&& runtime);

  /*!
   * \brief Destructor.
   *
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ~LifeCycleManager() noexcept final;

  /*!
   * \brief Use default destructor.
   */
  LifeCycleManager() = delete;
  LifeCycleManager(LifeCycleManager const&) noexcept = delete;
  LifeCycleManager& operator=(LifeCycleManager const&) & = delete;
  LifeCycleManager(LifeCycleManager&&) noexcept = delete;
  LifeCycleManager& operator=(LifeCycleManager&&) & = delete;

  /*!
   * \brief Get the singleton instance.
   * \return Reference to the single instance of the class.
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static amsr::generic::Singleton<LifeCycleManager>& GetInstance() noexcept;

  /*!
   * \copydoc LifeCycleManagerInterface::AddSomeIpBindingCore
   */
  void AddSomeIpBindingCore(std::shared_ptr<SomeIpBindingCore> someip_binding_core,
                            std::unique_ptr<ConnectionAction>&& connection_action) noexcept final;

  /*!
   * \brief       Get the Runtime object.
   * \return      Reference to the Runtime object.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  std::unique_ptr<RuntimeInterface>& GetRuntime() noexcept;

  /*!
   * \brief       Get the SomeIpBindingConfig object.
   * \return      Reference to the SomeIpBindingConfig object.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetSomeIpBindingConfig() noexcept -> SomeIpBindingConfig&;

  /*!
   * \brief Stores the service config wrapper object.
   *
   * \param[in]       config_wrapper_uptr Unique pointer to the service config wrapper.
   * \pre -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires config_wrapper_uptr!= nullptr;
   * \endspec
   */
  void SetServiceConfigWrapper(
      std::unique_ptr<configuration::ServiceConfigWrapperInterface>&& config_wrapper_uptr) noexcept final;

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
  configuration::ServiceConfigWrapperInterface& GetServiceConfigWrapper() noexcept final;

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
  configuration::ServiceConfigWrapperInterface::ValidatedServiceConfigRef GetServiceConfig(
      configuration::ShortnamePath const& service_shortname_path) noexcept final;

  /*!
   * \copydoc LifeCycleManagerInterface::GetBindingCores
   */
  SomeIpBindingCoreContainer const& GetBindingCores() const noexcept final;

  /*!
   * \copydoc LifeCycleManagerInterface::GetConnectionActions
   */
  ConnectionActionContainer const& GetConnectionActions() const noexcept final;

 private:
  /*!
   * \brief Unique pointer to SomeIp binding config.
   */
  std::unique_ptr<SomeIpBindingConfig> someip_binding_config_;

  /*!
   * \brief Runtime instance.
   */
  std::unique_ptr<RuntimeInterface> runtime_;

  /*!
   * \brief Global SomeIpBindingCore instances.
   */
  SomeIpBindingCoreContainer someip_binding_cores_;

  /*!
   * \brief Connection actions for connecting the SomeIpBindingCore to the SomeIpDaemon.
   */
  ConnectionActionContainer connection_actions_;

  /*!
   * \brief Unique pointer to Service config wrapper object holding all validated service configurations.
   */
  std::unique_ptr<configuration::ServiceConfigWrapperInterface> service_config_wrapper_;
};

}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_LIFE_CYCLE_MANAGER_H_
