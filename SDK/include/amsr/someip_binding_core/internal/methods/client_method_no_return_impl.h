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
/**        \file  amsr/someip_binding_core/internal/methods/client_method_no_return_impl.h
 *        \brief  ClientMethodNoReturnImpl
 *        \unit   SomeIpBinding::SomeIpBindingCore::Client::ClientMethodNoReturn
 *
 *      \details This class shall be used for communication with the binding core.
 *               It will send a non return method request to the server.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_CLIENT_METHOD_NO_RETURN_IMPL_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_CLIENT_METHOD_NO_RETURN_IMPL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <mutex>
#include <utility>

#include "amsr/core/result.h"
#include "amsr/someip_binding_core/internal/client_interface.h"
#include "amsr/someip_binding_core/internal/client_transport_to_daemon_interface.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_core/internal/logging/logger_prefix_generator.h"
#include "amsr/someip_binding_core/internal/methods/method_no_response_interface.h"
#include "amsr/someip_binding_core/internal/session_handler.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/serialization/writer.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {
namespace methods {

/*!
 * \brief Use string literal for creation of StringViews.
 */
// VECTOR NC AutosarC++17_10-M7.3.6, VectorC++-V5.0.1: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.6_using_directive
// VECTOR NC AutosarC++17_10-M7.3.4, VectorC++-V5.0.1: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.4_using_directive
using vac::container::literals::operator""_sv;

/*!
 * \brief   ClientMethodNoReturnImpl class.
 * \details This class shall be used for communication with the binding core.
 *          It will send a non return method request to the server.
 *
 */
class ClientMethodNoReturnImpl final : public MethodNoResponseInterface {
 public:
  /*!
   * \brief Type alias for the method ID.
   */
  using MethodId = ::amsr::someip_protocol::internal::MethodId;

  /*!
   * \brief Type alias for logger.
   */
  using Logger = ::amsr::someip_binding_core::internal::logging::AraComLogger;

  /*!
   * \brief Default destructor.
   *
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~ClientMethodNoReturnImpl() noexcept final = default;

  ClientMethodNoReturnImpl() = delete;
  ClientMethodNoReturnImpl(ClientMethodNoReturnImpl const&) = delete;
  ClientMethodNoReturnImpl(ClientMethodNoReturnImpl&&) = delete;
  ClientMethodNoReturnImpl& operator=(ClientMethodNoReturnImpl const&) & = delete;
  ClientMethodNoReturnImpl& operator=(ClientMethodNoReturnImpl&&) & = delete;

  /*!
   * \brief Constructor.
   *
   * \param[in] proxy_identity              Proxy binding identity, containing service id, major version,
   *                                        instance id and client id.
   * \param[in]  method_id                  SOME/IP method identifier for this method.
   * \param[in] local_client_impl           Local client impl object.
   *
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \internal steady FALSE \endinternal
   */
  ClientMethodNoReturnImpl(ProxyBindingIdentity const& proxy_identity, MethodId const method_id,
                           ClientInterface& local_client_impl) noexcept;

  /*!
   * \copydoc ::amsr::someip_binding_core::internal::methods::MethodNoResponseInterface::SendMethodRequest
   */
  ::amsr::core::Result<void> SendMethodRequest(MemoryBufferPtr packet) noexcept final;

 private:
  /*!
   * \brief Type alias for session handler.
   */
  using Session = ::amsr::someip_binding_core::internal::SessionHandler;

  /*!
   * \brief Type alias for serialization writer.
   */
  using Writer = ::amsr::someip_protocol::internal::serialization::Writer;

  /*!
   * \brief Generates SOME/IP header for the method request.
   * \param[in] payload_size  size of the payload to calculate the length field.
   * \return The generated header. All header parameters for this method would be the same for all requests except for:
   *         Session ID:  increments for each request, and
   *         length field: depends on the given payload.
   *
   * \pre              Payload length must not exceed the maximum allowed length (32 bits field excluding 8 bytes
   *                   for the first part of the SOME/IP Header)
   * \context          App
   * \threadsafe       TRUE
   * \reentrant        TRUE
   * \synchronous      TRUE
   * \exceptionsafety  STRONG No undefined behavior or side effects.
   * \internal steady TRUE \endinternal
   */
  ::amsr::someip_protocol::internal::SomeIpMessageHeader GenerateRequestHeader(std::size_t const payload_size) noexcept;

  /*!
   * \brief Serialize the given SOME/IP header into the given view.
   * \param[in] header_view  view where the header shall be serialized.
   * \param[in] header       SOME/IP header to be serialized.
   *
   * \pre              -
   * \context          App
   * \threadsafe       TRUE
   * \reentrant        TRUE
   * \synchronous      TRUE
   * \exceptionsafety  STRONG No undefined behavior or side effects.
   * \internal steady TRUE \endinternal
   */
  static void SerializeHeader(amsr::core::Span<std::uint8_t> header_view,
                              ::amsr::someip_protocol::internal::SomeIpMessageHeader const& header) noexcept;

  /*!
   * \brief Unique identifier for this client.
   */
  ProxyBindingIdentity const proxy_identity_;

  /*!
   * \brief SOME/IP method identifier for this method.
   */
  MethodId const method_id_;

  /*!
   * \brief The binding local client implementation.
   */
  ClientInterface& local_client_impl_;

  /*!
   * \brief payload offset within the method request.
   */
  static std::size_t const kPayloadOffset{::amsr::someip_protocol::internal::kHeaderSize};

  /*!
   * \brief SOME/IP header offset within the method request.
   */
  static std::size_t const kSomeIpHeaderOffset{0};

  /*!
   * \brief maximum allowed payload length (including E2E) of a SOME/IP message.
   */
  static std::size_t const kMaximumSomeIpPayloadLength{
      std::numeric_limits<::amsr::someip_protocol::internal::LengthField>::max() -
      ::amsr::someip_protocol::internal::kMinimumPayloadLength};

  /*!
   * \brief Provides functionality to create unique IDs for each request (session).
   */
  Session session_{};

  /*!
   * \brief Mutex to protect parallel access to the session id generator.
   */
  std::mutex session_id_lock_{};

  /*!
   * \brief Mutex to protect parallel requests to transmit a method request.
   */
  std::mutex transmission_lock_{};

  /*!
   * \brief Logger Prefix.
   */
  ::amsr::core::String const logger_prefix_{
      someip_binding_core::internal::logging::LoggerPrefixGenerator::GetClientMethodLoggerPrefix(
          "ClientMethodNoReturn"_sv, proxy_identity_, method_id_)};

  /*!
   * \brief Method logger for printing debug and error messages.
   * The concrete method will set the prefix for identification.
   */
  Logger logger_{::amsr::someip_binding_core::internal::logging::kSomeIpLoggerContextId,
                 ::amsr::someip_binding_core::internal::logging::kSomeIpLoggerContextDescription, logger_prefix_};
};

}  // namespace methods
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_CLIENT_METHOD_NO_RETURN_IMPL_H_

/*!
 * \exclusivearea ::amsr::someip_binding_xf::internal::methods::ClientMethodNoReturn::session_id_lock_
 *                Used for protecting simultaneous calls to SendMethodRequest
 *
 * \protects ::amsr::someip_binding_xf::internal::methods::ClientMethodNoReturn::session_
 *           is the protected resource of an ProxyMethodXf object.
 *
 * \usedin Generate method request(read, write)
 * \exclude User threads/Reactor thread from accessing a variable that is already in access.
 * \length SHORT counter increment/reset.
 * \endexclusivearea
 */

/*!
 * \exclusivearea ::amsr::someip_binding_xf::internal::methods::ClientMethodNoReturn::transmission_lock_
 *                Used for protecting simultaneous calls to SendMethodRequest
 *
 * \protects ::amsr::someip_binding_xf::internal::methods::ClientMethodNoReturn::client_transport_to_daemon_
 *           is the protected resources of an ClientMethodNoReturn object.
 *
 * \usedin Send method request (read, write)
 * \exclude User threads/Reactor thread from accessing a variable that is already in access.
 * \length LONG IPC communication.
 * \endexclusivearea
 */
