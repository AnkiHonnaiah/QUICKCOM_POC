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
/*!        \file  client_endpoint_handshake_processor.h
 *        \brief  Processor for client communication that handles the business logic for the handshake.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_CLIENT_ENDPOINT_HANDSHAKE_PROCESSOR_H_
#define LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_CLIENT_ENDPOINT_HANDSHAKE_PROCESSOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/sec/ipc/message_builder.h"
#include "amsr/sec/ipc/message_deserializer.h"
#include "amsr/sec/ipc/message_serializer.h"
#include "ara/crypto/common/mem_region.h"
#include "crypto/common/util/logger.h"
#include "crypto/server/crypto_provider_factory.h"
#include "crypto/server/skeleton/skeleton_builder.h"

namespace crypto {
namespace server {

// VECTOR NC Metric-OO.RFC: MD_CRYPTO_Metric-OO.RFC
/*!
 * \brief Processor for client communication that handles the business logic for the handshake
 * \vprivate Component Private
 * \trace DSGN-Crypto-ProcessIsolation, DSGN-Crypto-Broker-Pattern
 */
class ClientEndpointHandshakeProcessor {
 public:
  /*!
   * \brief Constructor
   * \param[in] object_registration Registration to register created objects to
   * \param[in] skeleton_registration Registration to register the created skeletons
   * \param[in] skeleton_builder Builder for skeleton objects
   * \param[in] ksp_proxy KeyStorageProvider to be used for new registrations
   * \param[in] x509_provider_proxy X509Provider to be used for new registrations
   * \param[in] provider_factory CryptoProviderFactory to be used for new registrations
   * \vprivate Component Private
   */
  ClientEndpointHandshakeProcessor(crypto::server::ObjectRegistration& object_registration,
                                   crypto::server::BaseSkeletonRegistration& skeleton_registration,
                                   crypto::server::skeleton::SkeletonBuilder& skeleton_builder,
                                   std::shared_ptr<ara::crypto::keys::KeyStorageProvider> ksp_proxy,
                                   std::shared_ptr<ara::crypto::x509::X509Provider> x509_provider_proxy,
                                   std::shared_ptr<CryptoProviderFactory> provider_factory)
      : logger_{common::util::kServerLoggingContext, ""_sv},
        message_deserializer_{},
        crypto_provider_factory_{std::move(provider_factory)},
        message_serializer_{},
        object_registration_(object_registration),
        skeleton_registration_(skeleton_registration),
        skeleton_builder_(skeleton_builder),
        key_storage_provider_{std::move(ksp_proxy)},
        x509_provider_{std::move(x509_provider_proxy)} {}

  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  ClientEndpointHandshakeProcessor(ClientEndpointHandshakeProcessor const& /*other*/) = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  ClientEndpointHandshakeProcessor(ClientEndpointHandshakeProcessor&& /*other*/) noexcept = default;

  /*!
   * \brief Deleted copy assignment operator
   * \return -
   * \vprivate Component Private
   */
  ClientEndpointHandshakeProcessor& operator=(ClientEndpointHandshakeProcessor const& /*other*/) & = delete;

  /*!
   * \brief Deleted move assignment operator
   * \return -
   * \vprivate Component Private
   */
  ClientEndpointHandshakeProcessor& operator=(ClientEndpointHandshakeProcessor&& /*other*/) & noexcept = delete;

  /*!
   * \brief Destroys the object.
   * \vprivate Component Private
   */
  virtual ~ClientEndpointHandshakeProcessor() noexcept = default;

  /*!
   * \brief Process the incoming message and write the response to the response_msg field
   * \param[in] input_msg incoming message to be processed
   * \param[out] response_buffer memory to write the response to
   * \return Subspan of the response buffer that contains the response message
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kRuntimeFault Runtime error.
   * \error ara::crypto::SecurityErrc::kRpcUnknownTask Unknown task.
   * \error ara::crypto::SecurityErrc::kUnsupported Trying to register an X.509 provider while X.509 functionalities are
   * not available.
   * \error ara::crypto::SecurityErrc::kUnsupportedFormat Buffer too small to hold serialized message of
   * required type.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  amsr::core::Result<ara::crypto::ReadOnlyMemRegion> ProcessData(
      ara::crypto::ReadOnlyMemRegion input_msg, ara::crypto::WritableMemRegion response_buffer) const noexcept;

 private:
  /*!
   * \brief Registers a CryptoProvider skeleton for given object ID.
   * \param[in] input_msg The input message to process. Message is supposed to contain a UUID of the requested
   *                  CryptoProvider and the ProxyId of the proxy to be registered.
   * \param[out] response_buffer Message containing the response for the incoming request
   * \return subspan of the response buffer that contains the response message
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kRuntimeFault Runtime error.
   * \error ara::crypto::SecurityErrc::kUnsupportedFormat Error in SerializeMessage.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  amsr::core::Result<ara::crypto::ReadOnlyMemRegion> RegisterCryptoProvider(
      ara::crypto::ReadOnlyMemRegion input_msg, ara::crypto::WritableMemRegion response_buffer) const noexcept;

  /*!
   * \brief Registers a KeyStorageProvider skeleton for given object ID.
   * \param[in] input_msg The input message to process. Message is supposed to contain the ProxyId of the proxy to be
   *                  registered.
   * \param[out] response_buffer Message containing the response for the incoming request
   * \return subspan of the response buffer that contains the response message
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kUnsupportedFormat Error in SerializeMessage.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  amsr::core::Result<ara::crypto::ReadOnlyMemRegion> RegisterKeyStorageProvider(
      ara::crypto::ReadOnlyMemRegion input_msg, ara::crypto::WritableMemRegion response_buffer) const noexcept;

  /*!
   * \brief Registers an X509Provider skeleton for the given object ID.
   * \param[in] input_msg The input message to process. Message is supposed to contain the ProxyId of the proxy to be
   *                  registered.
   * \param[out] response_buffer Message containing the response for the incoming request
   * \return subspan of the response buffer that contains the response message
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kUnsupported Trying to register an X.509 provider while X.509 functionalities are
   * not available.
   * \error ara::crypto::SecurityErrc::kUnsupportedFormat Error in SerializeMessage.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  amsr::core::Result<ara::crypto::ReadOnlyMemRegion> RegisterX509Provider(
      ara::crypto::ReadOnlyMemRegion input_msg, ara::crypto::WritableMemRegion response_buffer) const noexcept;

  crypto::common::util::Logger logger_;                             /*!< Logging instance */
  amsr::sec::ipc::MessageDeserializer message_deserializer_;        /*!< Deserializer for message deserialization */
  std::shared_ptr<CryptoProviderFactory> crypto_provider_factory_;  /*!< Factory for instantiation of Providers */
  amsr::sec::ipc::MessageSerializer message_serializer_;            /*!< Message serializer */
  crypto::server::ObjectRegistration& object_registration_;         /*!< Registration for newly created objects */
  crypto::server::BaseSkeletonRegistration& skeleton_registration_; /*!< Registration for newly created skeletons */
  crypto::server::skeleton::SkeletonBuilder& skeleton_builder_;     /*!< skeleton builder for responses */
  std::shared_ptr<ara::crypto::keys::KeyStorageProvider>
      key_storage_provider_; /*!< KeyStorageProvider used for Handshake registration */
  std::shared_ptr<ara::crypto::x509::X509Provider> x509_provider_; /*!< X509Provider used for Handshake registration */
};

}  // namespace server
}  // namespace crypto

#endif  // LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_CLIENT_ENDPOINT_HANDSHAKE_PROCESSOR_H_
