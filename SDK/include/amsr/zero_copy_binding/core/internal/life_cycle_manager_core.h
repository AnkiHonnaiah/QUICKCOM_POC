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
 *         \brief  Lifecycle Manager for the binding core.
 *
 *       \details  Manages the lifecycle of the service discovery and ZeroCopyCommon.
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_LIFE_CYCLE_MANAGER_CORE_H_
#define LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_LIFE_CYCLE_MANAGER_CORE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include <memory>

#include "amsr/core/optional.h"
#include "amsr/zero_copy_binding/common/internal/factory_interface.h"
#include "amsr/zero_copy_binding/common/internal/service_discovery/types.h"
#include "amsr/zero_copy_binding/core/internal/access_control/access_control_default.h"
#include "amsr/zero_copy_binding/core/internal/initialize_state_machine.h"
#include "amsr/zero_copy_binding/core/internal/logging/zero_copy_logger.h"
#include "amsr/zero_copy_common/internal/zero_copy_common_client.h"
#include "amsr/zero_copy_common/internal/zero_copy_common_server.h"
#include "osabstraction/io/reactor1/reactor1.h"

#ifdef AMSR_ZEROCOPYBINDING_ENABLE_TRACE_API
#include "amsr/zero_copy_binding/core/internal/trace/life_cycle_manager_trace.h"
#endif  // AMSR_ZEROCOPYBINDING_ENABLE_TRACE_API

namespace amsr {
namespace zero_copy_binding {
namespace core {
namespace internal {

/*!
 * \brief  Lifecycle Management of the zero copy binding core.
 * \unit   ZeroCopyBinding::ZeroCopyBindingCore::ZeroCopyCoreLifecycleManagement
 * \vprivate Component private
 */
class LifeCycleManagerCore final : public InitializeStateMachine {
 public:
  /*!
   * \brief         Constructor of LifeCycleManagerCore.
   * \param[in]     factory  Factory object to obtain the service discovery from. Must not be nullptr.
   * \pre           -
   * \context       Init
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component private
   */
  explicit LifeCycleManagerCore(std::shared_ptr<common::internal::FactoryInterface const> factory) noexcept;

  /*!
   * \brief         Default destructor of LifeCycleManagerCore.
   * \context       Shutdown
   * \pre           The object must be in a deinitialized state.
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component private
   */
  ~LifeCycleManagerCore() noexcept = default;  // VCA_ZEROCOPYBINDING_SLC_20_BASE_CLASS_METHOD

  LifeCycleManagerCore(LifeCycleManagerCore const&) = delete;
  auto operator=(LifeCycleManagerCore const&) -> LifeCycleManagerCore& = delete;
  LifeCycleManagerCore(LifeCycleManagerCore&&) = delete;
  auto operator=(LifeCycleManagerCore&&) -> LifeCycleManagerCore& = delete;

  /*!
   * \brief         Getter method for the service discovery.
   * \return        A shared pointer to the service discovery.
   * \context       Any
   * \pre           -
   * \threadsafe    TRUE
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \vprivate      Component private
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetServiceDiscovery() noexcept
      -> std::shared_ptr<common::internal::service_discovery::ServiceDiscoveryInterface>;

  /*!
   * \brief         Getter method for the reactor.
   * \return        A reference to the reactor.
   * \context       Any
   * \pre           -
   * \threadsafe    TRUE
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \vprivate      Component private
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetReactor() noexcept -> ::osabstraction::io::reactor1::Reactor1Interface&;

  /*!
   * \brief         Getter method for the factory.
   * \return        A shared pointer to the factory.
   * \context       Any
   * \pre           -
   * \threadsafe    TRUE
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \vprivate      Component private
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetFactory() const noexcept -> std::shared_ptr<common::internal::FactoryInterface const> const;

  /*!
   * \brief         Getter method for the access control.
   * \return        A reference to the access control. Valid as long as this instance exists.
   * \context       Any
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component private
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetAccessControl() const noexcept -> core::internal::access_control::AccessControlInterface const&;

  /*!
   * \brief         Set the access control implementation.
   * \param[in]     access_control Valid pointer to the access control implementation.
   * \pre           -
   * \context       App
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component private
   *
   * \spec
   *   requires true;
   * \endspec
   */
  void SetAccessControl(
      std::unique_ptr<core::internal::access_control::AccessControlInterface> access_control) noexcept;

  /*!
   * \brief Reset the user-defined access control implementation to the default implementation.
   * \context       Any
   * \pre           -
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component private
   *
   * \spec
   *   requires true;
   * \endspec
   */
  void ResetAccessControl() noexcept;

// VECTOR NL AutosarC++17_10-A16.0.1: MD_ZEROCOPYBINDING_A16.0.1_conditionalCompilation
#ifdef AMSR_ZEROCOPYBINDING_ENABLE_TRACE_API
  /*!
   * \brief         Getter method for the trace lifecycle manager.
   * \return        A reference to the trace lifecycle manager.
   * \context       Any
   * \pre           -
   * \threadsafe    TRUE
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \vprivate      Component private
   */
  auto GetLifeCycleManagerTrace() noexcept -> core::internal::trace::LifeCycleManagerTrace&;
#endif  // AMSR_ZEROCOPYBINDING_ENABLE_TRACE_API

 private:
  /*!
   * \brief         Initializer function setting up the service discovery.
   * \context       Init
   * \pre           The object is not already initialized.
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component private
   */
  void OnInitialize() noexcept final;

  /*!
   * \brief         Deinitializer function. Disconnects from the service discovery.
   * \context       Shutdown
   * \pre           The object is initialized.
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component private
   */
  void OnDeinitialize() noexcept final;

  /*!
   * \brief The Logger.
   */
  core::internal::logging::ZeroCopyLogger& logger_;

  /*!
   * \brief The factory object to use for generated code and external dependencies.
   */
  std::shared_ptr<common::internal::FactoryInterface const> const factory_;

  /*!
   * \brief A reference to the reactor.
   */
  ::amsr::core::Optional<std::reference_wrapper<::osabstraction::io::reactor1::Reactor1Interface>> reactor_;

  /*!
   * \brief The AccessControl instance.
   */
  std::unique_ptr<access_control::AccessControlInterface> access_control_{
      std::make_unique<access_control::AccessControlDefault>()};

  /*!
   * \brief The service discovery.
   */
  std::shared_ptr<common::internal::service_discovery::ServiceDiscoveryInterface> service_discovery_;

// VECTOR NL AutosarC++17_10-A16.0.1: MD_ZEROCOPYBINDING_A16.0.1_conditionalCompilation
#ifdef AMSR_ZEROCOPYBINDING_ENABLE_TRACE_API
  /*!
   * \brief The tracing lifecycle manager.
   */
  core::internal::trace::LifeCycleManagerTrace life_cycle_manager_trace_;
#endif  // AMSR_ZEROCOPYBINDING_ENABLE_TRACE_API
};

}  // namespace internal
}  // namespace core
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_LIFE_CYCLE_MANAGER_CORE_H_
