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
 *        \brief  This file contains a collection of utility functions used by handshake state machine
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_HANDSHAKE_STATEMACHINE_UTILITY_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_HANDSHAKE_STATEMACHINE_UTILITY_H_

#include <cstdint>
#include "ara/core/vector.h"

#include "tls/internal/common/timer_extension.h"
#include "tls/internal/handshakestatemachine/common_handshake_types.h"
#include "tls/internal/handshakestatemachine/handshake_message_aggregator.h"
#include "tls/internal/handshakestatemachine/handshake_state_pool_interface.h"
#include "tls/internal/messages/alert_message.h"
#include "tls/internal/messages/message_manager.h"
#include "tls/internal/record_protocol/record_protocol.h"
#include "tls/public/tls_comm_general_types.h"

namespace amsr {
namespace tls {
namespace internal {
namespace handshakestatemachine {

/*! \brief Forward declaration. */
class HandshakeStateMachine;

namespace utility {

/*!
 * \brief Serializes a handshake message at the tail of the destination buffer if it should be included.
 * \param[in,out] dest The destination buffer to append to at the tail.
 * \param[in] message A handshake message.
 * \param[in,out] cxt A flow context to trace errors which may occur during serialization.
 */
void SerializeIfIncluded(ara::core::Vector<std::uint8_t>& dest, messages::HandshakeMessage const& message,
                         common::TlsCommFlowContext& cxt) noexcept;

/*!
 * \brief         Serializes all handshake messages from the specified perspective (in order), and concatenates them.
 *
 * \param[in,out] cxt             The flow context used for error reporting.
 * \param[in]     message_manager The associated message manager.
 * \param[in]     cipher_suite    The cipher suite that is used.
 * \param[in]     connection_end  The connection end used to determine which messages to include.
 * \return        A serialized blob of handshake messages.
 *
 * \trace         SPEC-6561970, SPEC-6561972, SPEC-6561973, SPEC-6562064
 */
auto SerializeHandshakeMessages(common::TlsCommFlowContext& cxt, messages::MessageManager& message_manager,
                                SupportedCipherSuites cipher_suite,
                                record_protocol::ConnectionEnd connection_end) noexcept
    -> ara::core::Vector<std::uint8_t>;

/*!
 * \brief         Serializes all handshake messages before the CertificateVerify message, and concatenates them.
 *
 * \param[in,out] cxt             The flow context used for error reporting.
 * \param[in]     message_manager The associated message manager.
 * \param[in]     cipher_suite    The cipher suite that is used.
 * \return        A serialized blob of handshake messages.
 */
auto SerializeHandshakeMessagesBeforeCertificateVerifyMessage(common::TlsCommFlowContext& cxt,
                                                              messages::MessageManager& message_manager,
                                                              SupportedCipherSuites cipher_suite) noexcept
    -> ara::core::Vector<std::uint8_t>;

/*!
 * \brief Is this HandshakeStateMachine in DTLS mode.
 * \param[in] role This is the role to be used by this instance of the provider.
 * \return true If it is DTLS. False if it is TLS.
 * \context ANY
 * \pre -
 * \reentrant TRUE
 * \vprivate This method is component private.
 */
bool IsDtls(EnTlsCommRole role) noexcept;

/*!
 * \brief Return true if this HandshakeStateMachine is in Client mode.
 * \param[in] role This is the role to be used by this instance of the provider.
 * \return true If it is a client. False if it is a server.
 * \context ANY
 * \pre -
 * \reentrant TRUE
 * \vprivate This method is component private.
 */
bool IsClient(EnTlsCommRole role) noexcept;

/*!
 * \brief Set the state secret to be used in the hash cookie calculation
 * \details Cookie verification secret will only be set if the role is Dtls server
 * \param[in] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
 * \param[in] hsm Reference to HandshakeStateMachine
 * \context ANY
 * \pre- cxt has no previous recorded error
 * \error TlsCommErrorCode::kTlsErrInternal is reported if cookie Verification secret set for other than
 * DTLS server
 * \reentrant FALSE
 * \vprivate This method is component private.
 */
void SetCookieVerificationSecret(common::TlsCommFlowContext& cxt, HandshakeStateMachine& hsm) noexcept;

}  // namespace utility
}  // namespace handshakestatemachine
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_HANDSHAKE_STATEMACHINE_UTILITY_H_
