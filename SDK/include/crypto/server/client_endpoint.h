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
/*!        \file  client_endpoint.h
 *        \brief  Client connection abstraction.
 *
 *      \details  Server-side client connection endpoint.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_CLIENT_ENDPOINT_H_
#define LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_CLIENT_ENDPOINT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <atomic>
#include <memory>

#include "amsr/sec/ipc/message_serializer.h"
#include "amsr/sec/ipc/protocol.h"
#include "amsr/sec/ipc/transporter_event_handler.h"
#include "crypto/server/call_context_registry.h"
#include "crypto/server/client_endpoint_callback.h"
#include "crypto/server/client_endpoint_handshake_processor.h"
#include "crypto/server/client_endpoint_message_processor.h"
#include "crypto/server/config/daemon_config.h"

namespace crypto {
namespace server {

// VECTOR NC Metric-OO.RFC: MD_CRYPTO_Metric-OO.RFC
/*!
 * \brief Client connection abstraction.
 * \vprivate Component Private
 * \trace DSGN-Crypto-ProcessIsolation, DSGN-Crypto-Broker-Pattern
 */
class ClientEndpoint final : public amsr::sec::ipc::TransporterEventHandler {
 public:
  /*!
   * \brief Constructor.
   *
   * \param[in] transporter Socket abstraction used for communication.
   * \param[in] callback Callback to be registered for this client endpoint
   * \param[in] ksp KeyStorageProvider to be used for new registrations
   * \param[in] x509_provider X509Provider to be used for new registrations
   * \param[in] provider_factory Provider factory to be used to instantiate requested crypto providers
   * \param[in] config Daemon configuration
   * \param[in] call_context_registry Registry to store the current call context
   * \pre transporter is not a nullptr
   * \vprivate Component Private
   */
  explicit ClientEndpoint(std::unique_ptr<amsr::sec::ipc::Transporter> transporter,
                          crypto::server::ClientEndpointCallback* callback,
                          std::shared_ptr<ara::crypto::keys::KeyStorageProvider> ksp,
                          std::shared_ptr<ara::crypto::x509::X509Provider> x509_provider,
                          std::shared_ptr<CryptoProviderFactory> provider_factory, config::DaemonConfig const& config,
                          crypto::server::CallContextRegistry& call_context_registry);

  /*!
   * \brief Destructor.
   * \vprivate Component Private
   */
  ~ClientEndpoint() noexcept final;

  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  ClientEndpoint(ClientEndpoint const& /*other*/) = delete;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  ClientEndpoint(ClientEndpoint&& /*other*/) noexcept = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted copy assignment operator
   * \return -
   * \vprivate Component Private
   */
  ClientEndpoint& operator=(ClientEndpoint const& /*other*/) = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted move assignment operator
   * \return -
   * \vprivate Component Private
   */
  ClientEndpoint& operator=(ClientEndpoint&& /*other*/) noexcept = delete;

  /*!
   * \copydoc amsr::sec::ipc::TransporterEventHandler::OnReceived()
   * \vprivate Component Private
   */
  void OnReceived(ara::crypto::ReadOnlyMemRegion data, amsr::sec::ipc::UserId e_uid,
                  osabstraction::process::ProcessId p_uid) noexcept final;

  /*!
   * \copydoc amsr::sec::ipc::TransporterEventHandler::OnSent()
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void OnSent() noexcept final;

  /*!
   * \copydoc amsr::sec::ipc::TransporterEventHandler::OnDisconnect()
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void OnDisconnect() noexcept final;

 private:
  crypto::common::util::Logger logger_;                            /*!< Logging instance */
  std::unique_ptr<amsr::sec::ipc::Transporter> transporter_;       /*!< Transporter for server/client communication */
  amsr::sec::ipc::MessageBuilder message_builder_;                 /*!< Message builder */
  amsr::sec::ipc::MessageSerializer message_serializer_;           /*!< Message serializer */
  crypto::server::ServerMessageDeserializer message_deserializer_; /*!< Message deserializer */
  crypto::server::ObjectRegistration object_registration_;         /*!< Object registration to be used */
  crypto::server::ObjectProvider object_provider_;                 /*!< Object provider to be used */
  crypto::server::BaseSkeletonRegistration skeleton_registration_; /*!< Reference to the skeleton registration */
  std::unique_ptr<crypto::server::skeleton::SkeletonBuilder> skeleton_builder_; /*!< Builder for new skeleton objects */
  std::unique_ptr<crypto::server::ClientEndpointHandshakeProcessor>
      handshake_processor_; /*!< Processor for incoming handshake messages */
  std::unique_ptr<crypto::server::ClientEndpointMessageProcessor>
      message_processor_;                            /*!< Processor for incoming messages */
  crypto::server::ClientEndpointCallback* callback_; /*!< Callee for callbacks */
  crypto::server::CallContextRegistry&
      call_context_registry_; /*!< Registry to store information about current caller */
};

}  // namespace server
}  // namespace crypto

#endif  // LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_CLIENT_ENDPOINT_H_
