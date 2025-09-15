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
/*!        \file  protocol_handler.h
 *        \brief  Protocol handler for wrapping the client side handshake business logic.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROTOCOL_PROTOCOL_HANDLER_H_
#define LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROTOCOL_PROTOCOL_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/sec/ipc/message_builder.h"
#include "amsr/sec/ipc/message_deserializer.h"
#include "amsr/sec/ipc/message_serializer.h"
#include "amsr/sec/ipc/protocol.h"
#include "amsr/sec/ipc/transporter.h"
#include "crypto/common/util/logger.h"

namespace crypto {
namespace client {
namespace protocol {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_CRYPTO_AutosarC++17_10-M7.3.6_sv
/*! Introduce string literal for StringView creation */
using vac::container::literals::operator"" _sv;

/*!
 * \brief Protocol handler for wrapping the client side handshake business logic
 * \vprivate Component Private
 */
class ProtocolHandler {
 public:
  /*!
   * \brief Constructor
   * \param[in] transporter Transporter to be used for communication
   * \vprivate Component Private
   */
  explicit ProtocolHandler(amsr::sec::ipc::Transporter& transporter)
      : transporter_{transporter},
        serializer_{},
        deserializer_{},
        logger_{common::util::kClientLoggingContext, ""_sv} {}

  /*!
   * \brief Destructor
   * \vprivate Component Private
   */
  virtual ~ProtocolHandler() noexcept = default;

  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  ProtocolHandler(ProtocolHandler const& /*other*/) = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  ProtocolHandler(ProtocolHandler&& /*other*/) noexcept = default;

  /*!
   * \brief Deleted copy assignment operator
   * \return -
   * \vprivate Component Private
   */
  ProtocolHandler& operator=(ProtocolHandler const& /*other*/) & = delete;

  /*!
   * \brief Deleted move assignment operator
   * \return -
   * \vprivate Component Private
   */
  ProtocolHandler& operator=(ProtocolHandler&& /*other*/) & noexcept = delete;

  /*!
   * \brief Register new instance for CryptoProvider
   * \param[in] uuid UUID of the CryptoProvider to be registered.
   * \param[in] proxy_id ProxyId to register the skeleton
   * \return true if the registration was successful
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kRuntimeFault Runtime error.
   * \error ara::crypto::SecurityErrc::kUnsupportedFormat Error in SerializeMessage.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  amsr::core::Result<void> RegisterNewCryptoProviderProxy(ara::crypto::Uuid const& uuid,
                                                          amsr::sec::ipc::ProxyId proxy_id) noexcept;

  /*!
   * \brief Register new instance for KeyStorageProvider
   * \param[in] proxy_id ProxyId to register the skeleton
   * \return true if the registration was successful
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kRuntimeFault Runtime error.
   * \error ara::crypto::SecurityErrc::kUnsupportedFormat Error in SerializeMessage.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  amsr::core::Result<void> RegisterNewKeyStorageProviderProxy(amsr::sec::ipc::ProxyId proxy_id) noexcept;

  /*!
   * \brief Register a new instance of the X.509 provider.
   * \param[in] proxy_id ID to assign to the X.509 provider.
   * \return True iff an X.509 provider was registered under the given ID.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kRuntimeFault Runtime error.
   * \error ara::crypto::SecurityErrc::kUnsupportedFormat Error in SerializeMessage.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  amsr::core::Result<void> RegisterNewX509ProviderProxy(amsr::sec::ipc::ProxyId proxy_id) noexcept;

 private:
  amsr::sec::ipc::Transporter& transporter_;         /*!< Transporter. */
  amsr::sec::ipc::MessageSerializer serializer_;     /*!< Message serializer. */
  amsr::sec::ipc::MessageDeserializer deserializer_; /*!< Message deserializer. */
  crypto::common::util::Logger logger_;              /*!< Logging instance. */
};

}  // namespace protocol
}  // namespace client
}  // namespace crypto

#endif  // LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_PROTOCOL_PROTOCOL_HANDLER_H_
