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
 *        \brief  Interface to deinitialize the binding.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_TARGET_DEPENDENT_DEINITIALIZER_INTERFACE_H_
#define LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_TARGET_DEPENDENT_DEINITIALIZER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ipc_binding_core/internal/remote_server_manager_interface.h"
#include "ipc_binding_core/internal/runtime_configuration/runtime_config.h"
#include "ipc_binding_core/internal/service_discovery/service_instance_listener_interface.h"

namespace amsr {
namespace ipc_binding {
namespace internal {
namespace target_dependent {

/*!
 * \brief Interface to deinitialize the binding.
 *
 * \steady FALSE
 *
 * \unit IpcBinding::Deinitializer
 */
class DeinitializerInterface {
 public:
  /*!
   * \brief Type alias for RuntimeConfig.
   */
  using RuntimeConfig = ::amsr::ipc_binding_core::internal::runtime_configuration::RuntimeConfig;

  /*!
   * \brief Type alias for the ServiceListener.
   */
  using ServiceInstanceListenerPtr =
      amsr::UniquePtr<::amsr::ipc_binding_core::internal::service_discovery::ServiceInstanceListenerInterface>;

  /*!
   * \brief Type alias for RemoteServerManagerInterface.
   */
  using RemoteServerManagerInterface = amsr::ipc_binding_core::internal::RemoteServerManagerInterface;

  DeinitializerInterface(DeinitializerInterface&&) noexcept = delete;
  DeinitializerInterface(DeinitializerInterface const&) noexcept = delete;
  auto operator=(DeinitializerInterface const&) & -> DeinitializerInterface& = delete;
  auto operator=(DeinitializerInterface&&) & -> DeinitializerInterface& = delete;

  /*!
   * \brief Destroys the object.
   *
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual ~DeinitializerInterface() noexcept = default;

  /*!
   * \brief Deinitialize the TransformationLayer.
   *
   * \param[in] runtime_config  Used IPC binding runtime configuration.
   *
   * \pre         The related reactor thread must be stopped.
   * \pre         All proxy and skeleton instances using IpcBinding deployment must have been destroyed.
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto Deinitialize(RuntimeConfig const& runtime_config) const noexcept -> void = 0;

 protected:
  /*!
   * \brief Use default constructor.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  DeinitializerInterface() noexcept = default;
};

}  // namespace target_dependent
}  // namespace internal
}  // namespace ipc_binding
}  // namespace amsr

#endif  // LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_TARGET_DEPENDENT_DEINITIALIZER_INTERFACE_H_
