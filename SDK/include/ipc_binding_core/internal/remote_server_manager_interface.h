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
 *        \brief  Interface to request / release RemoteServer instances.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_REMOTE_SERVER_MANAGER_INTERFACE_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_REMOTE_SERVER_MANAGER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/ipc/integrity_level.h"
#include "amsr/shared_ptr.h"

#include "ipc_binding_core/internal/remote_server_interface.h"
#include "ipc_binding_core/internal/required_service_instance_id.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
/*!
 * \brief RemoteServerManager interface.
 */
class RemoteServerManagerInterface {
 public:
  /*!
   * \brief Type alias for remote server pointer.
   */
  using RemoteServerSharedPtr = amsr::SharedPtr<RemoteServerInterface>;

  /*!
   * \brief Type alias for a ProvidedServiceInstanceId.
   */
  using ProvidedServiceInstanceId = ::amsr::ipc_binding_core::internal::ProvidedServiceInstanceId;

  /*!
   * \brief Type alias for a RequiredServiceInstanceId.
   */
  using RequiredServiceInstanceId = ::amsr::ipc_binding_core::internal::RequiredServiceInstanceId;

  /*!
   * \brief Type alias for the integrity level.
   */
  using IntegrityLevel = ::amsr::ipc::IntegrityLevel;

  RemoteServerManagerInterface(RemoteServerManagerInterface const&) noexcept = delete;
  RemoteServerManagerInterface(RemoteServerManagerInterface&&) noexcept = delete;
  auto operator=(RemoteServerManagerInterface const&) noexcept -> RemoteServerManagerInterface& = delete;
  auto operator=(RemoteServerManagerInterface&&) noexcept -> RemoteServerManagerInterface& = delete;

  /*!
   * \brief Destructor, use default.
   *
   * \pre     -
   * \context Shutdown
   * \steady  FALSE
   */
  virtual ~RemoteServerManagerInterface() noexcept = default;

  /*!
   * \brief Request a RemoteServer.
   *
   * \param[in] provided_service_instance  The provided service instance.
   * \param[in] required_service_instance  The required service instance.
   * \param[in] integrity_level            The expected integrity level of the IPC connection.
   *
   * \return RemoteServer
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto RequestRemoteServer(ProvidedServiceInstanceId const& provided_service_instance,
                                   RequiredServiceInstanceId const& required_service_instance,
                                   IntegrityLevel integrity_level) noexcept -> RemoteServerSharedPtr = 0;

  /*!
   * \brief Release a RemoteServer.
   *
   * \param[in] provided_service_instance  The provided service instance.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  virtual auto ReleaseRemoteServer(ProvidedServiceInstanceId const& provided_service_instance) noexcept -> void = 0;

 protected:
  /*!
   * \brief Default constructor.
   *
   * \pre     -
   * \context ANY
   * \steady  FALSE
   */
  RemoteServerManagerInterface() noexcept = default;
};

}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_REMOTE_SERVER_MANAGER_INTERFACE_H_
