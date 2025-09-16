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
/*!       \file   amsr/someip_binding_core/internal/local_server_manager.h
 *        \brief  ara::com SOME/IP Binding Local Server Manager
 *        \unit   SomeIpBinding::SomeIpBindingCore::Server::LocalServerManager
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_LOCAL_SERVER_MANAGER_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_LOCAL_SERVER_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <memory>

#include "amsr/someip_binding_core/internal/local_server.h"
#include "amsr/someip_binding_core/internal/local_server_manager_interface.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_core/internal/methods/skeleton_method_handler_interface.h"
#include "amsr/someip_binding_core/internal/server_manager_interface.h"
#include "amsr/someip_binding_core/internal/server_transport_to_daemon_interface.h"
#include "amsr/someip_binding_core/internal/types.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {

/*!
 * \brief Interface for a local server manager.
 */
class LocalServerManager final : public LocalServerManagerInterface,
                                 public std::enable_shared_from_this<LocalServerManager> {
 public:
  /*!
   * \brief Alias to set of method IDs for which a request handler has to be registered.
   */
  using RequiredSkeletonMethodHandlerSet = methods::SkeletonMethodHandlerInterface::RequiredSkeletonMethodHandlerSet;

  /*!
   * \brief       Constructor.
   *
   * \param[in]   server_manager               A reference to the server manager that routes the method response
   *                                           messages to the corresponding local server.
   * \param[in]   server_transport_to_daemon   A reference to the server transport to daemon. Used to access
   *                                           SOME/IP Daemon to request a local server.
   * \pre         -
   *
   * \context     Init
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  LocalServerManager(ServerManagerInterface& server_manager,
                     ServerTransportToDaemonInterface& server_transport_to_daemon) noexcept;

  /*!
   * \brief       Destructor.
   *
   * \pre         -
   *
   * \context     Shutdown
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
  ~LocalServerManager() noexcept final = default;

  LocalServerManager() = delete;
  LocalServerManager(LocalServerManager const&) = delete;
  LocalServerManager(LocalServerManager&&) = delete;
  LocalServerManager& operator=(LocalServerManager const&) & = delete;
  LocalServerManager& operator=(LocalServerManager&&) & = delete;

  /*!
   * \brief       Create a new local server stored inside the LocalServerMap.
   *
   * \param[in]   service_instance_id   The service instance id.
   * \param[in]   required_handler_set  Set of skeleton method request handlers for which request handlers have to be
   *                                    registered.
   *
   * \return      True if emplace succeeded, false if emplace failed due to already-existing server
   *              with same service instance id.
   *
   * \pre         -
   *
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  bool EmplaceLocalServer(ProvidedServiceInstanceId const& service_instance_id,
                          RequiredSkeletonMethodHandlerSet const& required_handler_set) noexcept;

  /*!
   * \brief       Create a server instance with the given ID.
   *
   * \param[in]   service_instance_id   The service instance id.
   *
   * \return      The created server instance in case of success or an error code otherwise.
   *
   * \error       ServerError::kRuntimeConnectionTimeout if no response is received from SOME/IP Daemon.
   * \error       ServerError::kSystemConfigurationNotAvailable if this service instance is not configured.
   * \error       ServerError::kSystemPrivilegesNoAccess if access is denied for this service.
   *              configured.
   * \error       ServerError::kRuntimeConnectionTransmissionFailed If SOME/IP Daemon is not connected.
   * \error       ServerError::kRuntimeResourceOutOf If IPC queue to SOME/IP Daemon is limited and full.
   * \error       ServerError::kUnknownError If unknown error returned from someipdaemonclient.
   *
   * \pre         -
   *
   * \context     App (Skeleton ctor)
   * \threadsafe  TRUE (RequestLocalServer can be called from different threads in parallel)
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Check if ID is configured in the binding.
   *    - If not, return an error.
   * - Check if a LocalServerImpl is still available.
   *    - If not, return an error.
   * - Create a LocalServer by moving the corresponding LocalServerImpl out of the map.
   * - Request the local server instance to the SOME/IP Daemon
   *    - In case of failure, release the created local server to the internal map and also return
   *      the specific error received from the daemon.
   * \endinternal
   */
  amsr::core::Result<LocalServer> RequestLocalServer(ProvidedServiceInstanceId const& service_instance_id) noexcept;

  /*!
   * \brief        Check if a provided service instance is configured in this instance.
   * \param[in]    provided_service_instance_id The provided service instance id.
   *
   * \return       true if the provided service instance is configured.
   *
   * \pre          -
   *
   * \context       Init
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto IsProvidedServiceInstanceIdConfigured(ProvidedServiceInstanceId const& provided_service_instance_id) noexcept
      -> bool;

  /*!
   * \brief       Returns ownership of a local server impl to the local server manager.
   *
   * \details     Delegates the release of the internal local server resources map to ReleaseInternal
   *              and then releases the local server also with the SOME/IP Daemon.
   *
   * \param[in]   service_instance_id    The service instance id.
   * \param[in]   local_server_impl      Implementation object to return memory ownership to the manager.
   *
   * \pre         -
   *
   * \context     App   (Skeleton dtor)
   * \threadsafe  TRUE  (Release can be called from different threads in parallel)
   * \reentrant   FALSE
   * \spec
   *   requires local_server_impl != nullptr;
   * \endspec
   */
  void Release(ProvidedServiceInstanceId const& service_instance_id,
               std::unique_ptr<LocalServerInterface> local_server_impl) noexcept final;

 private:
  /*!
   * \brief Alias for the map of local servers implementations, uniquely identified by the provided service instance ID.
   */
  using LocalServersImplMap = std::map<ProvidedServiceInstanceId, std::unique_ptr<LocalServerInterface>>;

  /*!
   * \brief       Returns ownership of a local server impl to the local servers map.
   *
   * \param[in]   service_instance_id    The service instance id.
   * \param[in]   local_server_impl      Implementation object to return memory ownership to the manager.
   *
   * \pre         -
   *
   * \context     App
   * \threadsafe  TRUE  (Release can be called from different threads in parallel)
   * \reentrant   FALSE
   * \spec
   *   requires local_server_impl != nullptr;
   * \endspec
   */
  void ReleaseInternal(ProvidedServiceInstanceId const& service_instance_id,
                       std::unique_ptr<LocalServerInterface> local_server_impl) noexcept;

  /*!
   * \brief A reference to the server manager that routes the method response messages to the corresponding local
   * server.
   */
  ServerManagerInterface& server_manager_;

  /*!
   * \brief A reference to the server transport to daemon. This will be used for validation purposes when
   *        requesting a local server.
   */
  ServerTransportToDaemonInterface& server_transport_to_daemon_;

  /*!
   * \brief The local servers concrete implementations.
   */
  LocalServersImplMap local_servers_{};

  /*!
   * \brief Lock that protects the map of local servers to manage concurrent access, as the request and release
   *        of resources may be done from different threads.
   */
  std::mutex local_servers_map_lock_{};

  /*!
   * \brief Our logger
   */
  ::amsr::someip_binding_core::internal::logging::AraComLogger logger_;
};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_LOCAL_SERVER_MANAGER_H_
