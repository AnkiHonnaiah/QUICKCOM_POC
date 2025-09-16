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
/*!        \file  client_endpoint_message_processor.h
 *        \brief  Processor for incoming messages on server side.
 *
 *      \details  The received data is inspected and perform the correct action depending on the messages task.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_CLIENT_ENDPOINT_MESSAGE_PROCESSOR_H_
#define LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_CLIENT_ENDPOINT_MESSAGE_PROCESSOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>
#include "amsr/sec/ipc/message_deserializer.h"
#include "amsr/sec/ipc/message_serializer.h"
#include "client_endpoint_handshake_processor.h"
#include "crypto/server/base_skeleton_registration.h"

namespace crypto {
namespace server {

/*!
 * \brief Processor for incoming messages on server side
 * \vprivate Component Private
 * \trace DSGN-Crypto-ProcessIsolation, DSGN-Crypto-Broker-Pattern
 */
class ClientEndpointMessageProcessor {
 public:
  /*!
   * \brief Constructor
   * \param[in] skeleton_registration Skeleton registration to be used in the processor
   * \param[in] object_registration Registration for arguments
   * \param[in] handshake_processor Handshake processor to be used for handshake messages
   * \vprivate Component Private
   */
  ClientEndpointMessageProcessor(crypto::server::BaseSkeletonRegistration& skeleton_registration,
                                 crypto::server::ObjectRegistration& object_registration,
                                 crypto::server::ClientEndpointHandshakeProcessor& handshake_processor)
      : message_serializer_{},
        object_provider_{object_registration},
        skeleton_registration_{skeleton_registration},
        message_deserializer_{object_provider_},
        handshake_processor_{handshake_processor},
        logger_{common::util::kServerLoggingContext, ""_sv} {}

  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  ClientEndpointMessageProcessor(ClientEndpointMessageProcessor const& /*other*/) = delete;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  ClientEndpointMessageProcessor(ClientEndpointMessageProcessor&& /*other*/) noexcept = delete;

  /*!
   * \brief Deleted copy assignment operator
   * \return -
   * \vprivate Component Private
   */
  ClientEndpointMessageProcessor& operator=(ClientEndpointMessageProcessor const& /*other*/) = delete;

  /*!
   * \brief Deleted move assignment operator
   * \return -
   * \vprivate Component Private
   */
  ClientEndpointMessageProcessor& operator=(ClientEndpointMessageProcessor&& /*other*/) noexcept = delete;

  /*!
   * \brief Destroys the object.
   * \vprivate Component Private
   */
  virtual ~ClientEndpointMessageProcessor() noexcept = default;

  /*!
   * \brief Dispatching for incoming messages basic task
   * \param[in] data of the incoming message
   * \param[in] out_buffer buffer to write the result message into
   * \param[in] e_uid Effective user id of the process which sent the message.
   * \return Part of thwe buffer with the result message
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kRuntimeFault Runtime error.
   * \error ara::crypto::SecurityErrc::kRpcUnknownTask Unknown task.
   * \error ara::crypto::SecurityErrc::kUnsupportedFormat Buffer too small to hold serialized message of required type.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  amsr::core::Result<ara::crypto::ReadOnlyMemRegion> ProcessData(ara::crypto::ReadOnlyMemRegion data,
                                                                 ara::crypto::WritableMemRegion out_buffer,
                                                                 amsr::sec::ipc::UserId e_uid) noexcept;

 private:
  amsr::sec::ipc::MessageSerializer message_serializer_;            /*!< Serializer for messages */
  crypto::server::ObjectProvider object_provider_;                  /*!< Pool holding object instances */
  crypto::server::BaseSkeletonRegistration& skeleton_registration_; /*!< Reference to the skeleton registration */
  crypto::server::ServerMessageDeserializer message_deserializer_;  /*!< Deserializer for messages */
  crypto::server::ClientEndpointHandshakeProcessor&
      handshake_processor_;             /*!< Processor for handling handshake messages */
  crypto::common::util::Logger logger_; /*!< Logging instance */
};

}  // namespace server
}  // namespace crypto

#endif  // LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_CLIENT_ENDPOINT_MESSAGE_PROCESSOR_H_
