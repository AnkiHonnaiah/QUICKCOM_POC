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
/**        \file
 *        \brief  Constructs a binding core.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_ARACOM_IPC_BINDING_FACTORY_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_ARACOM_IPC_BINDING_FACTORY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ipc_binding_core/internal/aracom_ipc_binding_factory_interface.h"
#include "ipc_binding_core/internal/memory.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {

/*!
 * \brief Constructs an ipc binding core.
 *
 * \unit IpcBinding::IpcBindingCore::LifeCycleManagerCore
 */
class AraComIpcBindingFactory final : public AraComIpcBindingFactoryInterface {
 public:
  /*!
   * \brief Construct the factory.
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
  AraComIpcBindingFactory() noexcept = default;

  /*!
   * \brief Destroy the factory.
   *
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  ~AraComIpcBindingFactory() noexcept final = default;  // VCA_IPCB_MOLE_1298

  AraComIpcBindingFactory(AraComIpcBindingFactory const&) noexcept = delete;
  auto operator=(AraComIpcBindingFactory const&) noexcept -> AraComIpcBindingFactory& = delete;
  AraComIpcBindingFactory(AraComIpcBindingFactory&&) noexcept = delete;
  auto operator=(AraComIpcBindingFactory&&) noexcept -> AraComIpcBindingFactory& = delete;

  /*!
   * \copydoc ::amsr::ipc_binding_core::internal::AraComIpcBindingFactoryInterface::Create
   */
  auto Create(osabstraction::io::reactor1::Reactor1Interface& reactor,
              amsr::UniquePtr<connection_manager::ConnectionManagerProxyFactoryInterface> cm_proxy_factory,
              amsr::UniquePtr<connection_manager::ConnectionManagerSkeletonFactoryInterface> cm_skeleton_factory,
              amsr::UniquePtr<ServiceDiscoveryInterface> service_discovery,
              access_control::AccessControlInterface& access_control,
              ipc_binding_core::internal::trace::TraceMonitor const& trace_monitor) noexcept
      -> amsr::UniquePtr<AraComIpcBindingInterface> final;
};

}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_ARACOM_IPC_BINDING_FACTORY_H_
