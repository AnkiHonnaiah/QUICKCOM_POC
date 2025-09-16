/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  server_runtime.h
 *        \brief  Runtime handling all client connections.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_SERVER_RUNTIME_H_
#define LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_SERVER_RUNTIME_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <memory>
#include <utility>
#include <vector>

#include "amsr/ipc/acceptor.h"
#include "amsr/ipc/unicast_address.h"
#include "amsr/sec/ipc/transporter_factory.h"
#include "amsr/signal/internal/signal_manager.h"
#include "amsr/thread/thread.h"
#include "crypto/server/call_context_registry.h"
#include "crypto/server/client_endpoint.h"
#include "crypto/server/client_endpoint_callback.h"
#include "crypto/server/config/daemon_config.h"
#include "crypto/server/keys/composite_key_storage_provider.h"
#include "osabstraction/io/reactor1/reactor1.h"

namespace crypto {
namespace server {

// VECTOR NC Metric-OO.RFC: MD_CRYPTO_Metric-OO.RFC
/*!
 * \brief Class for the server side runtime component.
 * \vprivate Product Private
 */
class ServerRuntime : public ClientEndpointCallback {
 public:
  /*!
   * \brief Constructor with injectable key storage provider
   * \param[in] transporter_provider Provider for IPC transporters
   * \param[in] reactor IO reactor for IPC handling
   * \param[in] address IPC address to listen to
   * \param[in] cksp Composite key storage provider instance
   * \param[in] config Daemon configuration
   * \param[in] call_context_registry Registry to store the current call context
   * \vprivate Product Private
   */
  ServerRuntime(amsr::sec::ipc::TransporterFactory& transporter_provider,
                osabstraction::io::reactor1::Reactor1& reactor, amsr::ipc::UnicastAddress address,
                std::shared_ptr<crypto::server::keys::CompositeKeyStorageProvider> cksp,
                config::DaemonConfig const& config, crypto::server::CallContextRegistry& call_context_registry);

  /*!
   * \brief Default copy constructor
   * \vprivate Product Private
   */
  ServerRuntime(ServerRuntime const& /*other*/) = delete;

  /*!
   * \brief Default move constructor
   * \vprivate Product Private
   */
  ServerRuntime(ServerRuntime&& /*other*/) noexcept = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted copy assignment operator
   * \return -
   * \vprivate Product Private
   */
  ServerRuntime& operator=(ServerRuntime const& /*other*/) = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted move assignment operator
   * \return -
   * \vprivate Product Private
   */
  ServerRuntime& operator=(ServerRuntime&& /*other*/) noexcept = delete;

  /*!
   * \brief Destructor.
   * \vprivate Product Private
   */
  ~ServerRuntime() noexcept override;

  /*!
   * \brief Starts to listen for connection requests.
   * \context ANY
   * \pre -
   * \throw crypto::softwareprovider::CryptoRuntimeError Listen failed on the acceptor.
   * \exceptionsafety BASIC The acceptor internals may change.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous FALSE
   * \vprivate Product Private
   */
  void Listen();

  /*!
   * \brief Callback for BasicIPC
   * \param[in] connection_result Communication abstraction.
   * \context ANY
   * \pre -
   * \throw std::bad_alloc Memory allocation failed.
   * \exceptionsafety STRONG
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  void OnAccept(amsr::core::Result<amsr::ipc::Connection>&& connection_result);

  /*!
   * \brief Handle connection closed.
   * \param[in] caller The caller.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  void OnDisconnect(ClientEndpoint* caller) override;

  /*!
   * \brief Creates the X.509 provider (if X.509 functionalities are included in the build).
   * \param[in] config Daemon configuration.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void InitX509Provider(config::DaemonConfig const& config);

  /*!
   * \brief           Waits until the reactor notifies about termination signal reception.
   * \details         This will happen upon SIGTERM, SIGINT signal reception.
   * \pre             -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void WaitForTerminationSignal();

  /*!
   * \brief          Handle signal callback.
   * \param[in]      signal_info Received signal information.
   * \context        ReactorThread
   * \threadsafe     FALSE
   * \reentrant      FALSE
   * \synchronous    TRUE
   * \vprivate Component Private
   */
  void HandleSignalCallback(amsr::signal::SupportedSignalInfo signal_info) noexcept;

  /*!
   * \brief          Setup and start signal handling.
   * \param[in]      reactor Reference to Reactor1.
   * \context        InitPhase
   * \threadsafe     FALSE
   * \reentrant      FALSE
   * \synchronous    TRUE
   * \vprivate Component Private
   */
  void SetupSignalHandling(osabstraction::io::reactor1::Reactor1Interface& reactor) noexcept;

 protected:
  /*!
   * \brief Gets access to the provider factory
   * \return A reference to the provider factory
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  auto GetProviderFactory() const noexcept -> CryptoProviderFactory& { return *crypto_provider_factory_; }

 private:
  /*!
   * \brief Initiates listening to incoming connection requests.
   * \vprivate Component Private
   * \context ANY
   * \error Forwards BasicIPC errors.
   * \threadsafe FALSE
   * \synchronous TRUE
   * \reentrant FALSE
   */
  amsr::core::Result<void> BindAccept() noexcept;

  /*!
   * \brief Handles incoming events on the reactor.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void Dispatch();

  /*!
   * \brief Cleans up obsolete ClientEndpoints.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void Cleanup();

  crypto::common::util::Logger logger_;                                     /*!< Logging instance */
  osabstraction::io::reactor1::Reactor1& reactor_;                          /*!< Reference to reactor */
  std::shared_ptr<crypto::server::keys::CompositeKeyStorageProvider> cksp_; /*!< Composite KeyStorageProvider */
  config::DaemonConfig const& config_;                                      /*!< Daemon config */
  crypto::server::CallContextRegistry&
      call_context_registry_; /*!< Registry storing information about the current caller */
  amsr::sec::ipc::TransporterFactory& transporter_provider_;            /*!< Transporter factory */
  ara::core::Vector<std::unique_ptr<ClientEndpoint>> client_endpoints_; /*!< Reference to all client endpoints */
  amsr::ipc::Acceptor acceptor_;                                        /*!< Acceptor for connection requests */
  std::unique_ptr<amsr::signal::internal::SignalManager>
      signal_manager_ptr_; /*!< Signal manager unique pointer to object responsible for signal handling. */
  std::mutex mutex_cv_;    /*!< Mutex to protect access to condition variable */
  std::condition_variable condition_variable_;   /*!< The condition variable for synchronization with reactor thread */
  amsr::thread::Thread reactor_thread_;          /*!< Thread to handle events on the reactor */
  std::atomic_bool dispatch_;                    /*!< Whether the reactor thread is active */
  std::atomic_bool termination_signal_received_; /*!< Whether SIGTERM or SIGINT has been received */
  // VECTOR NC AutosarC++17_10-A12.1.3: MD_CRYPTO_AutosarC++17_10-A12.1.3
  ara::core::Vector<ClientEndpoint*> to_delete_;                   /*!< List ClientEndpoints to delete */
  std::shared_ptr<ara::crypto::x509::X509Provider> x509_provider_; /*!< X.509 provider */
  std::shared_ptr<CryptoProviderFactory> crypto_provider_factory_; /*!< Provider factory for CryptoProviders */
  std::atomic<bool> listening_; /*!< Indicates if the runtime is currently listening. */
};

}  // namespace server
}  // namespace crypto

#endif  // LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_SERVER_RUNTIME_H_
