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
/**        \file  amsr/someip_binding_core/internal/local_server_impl.h
 *        \brief  ara::com SOME/IP Binding Local Server implementation.
 *        \unit   SomeIpBinding::SomeIpBindingCore::Server::LocalServer
 *
 *      \details  Class that represents a service instance.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_LOCAL_SERVER_IMPL_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_LOCAL_SERVER_IMPL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <memory>
#include "amsr/core/result.h"
#include "amsr/someip_binding_core/internal/local_server_interface.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_core/internal/methods/request.h"
#include "amsr/someip_binding_core/internal/methods/request_no_return.h"
#include "amsr/someip_binding_core/internal/methods/skeleton_method_handler_interface.h"
#include "amsr/someip_binding_core/internal/methods/skeleton_method_request_handler_interface.h"
#include "amsr/someip_binding_core/internal/methods/skeleton_method_request_no_return_handler_interface.h"
#include "amsr/someip_binding_core/internal/server_manager_interface.h"
#include "amsr/someip_binding_core/internal/server_transport_to_daemon_interface.h"
#include "amsr/someip_binding_core/internal/types.h"

// VECTOR NC AutosarC++17_10-M7.3.4: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.4_using_directive
// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.6_using_directive
using namespace vac::container::literals;  // NOLINT(build/namespaces)

namespace amsr {
namespace someip_binding_core {
namespace internal {

/*!
 * \brief Class that represents a service instance.
 */
class LocalServerImpl final : public LocalServerInterface, public methods::SkeletonMethodHandlerInterface {
 public:
  /*!
   * \brief Constructor.
   * \param[in]   provided_service_instance_id The provided service instance ID.
   * \param[in]   transport_to_daemon          Reference to the transport-to-daemon object.
   *                                           The object must be valid for Server's entire life time.
   * \param[in]   server_manager               Reference to the server manager.
   *                                           The object must be valid for Server's entire life time.
   * \param[in]   required_handler_set         Set of skeleton method request handlers for which request handlers have
   *                                           to be registered.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  LocalServerImpl(ProvidedServiceInstanceId const& provided_service_instance_id,
                  ServerTransportToDaemonInterface& transport_to_daemon, ServerManagerInterface& server_manager,
                  RequiredSkeletonMethodHandlerSet const& required_handler_set) noexcept;

  LocalServerImpl() = delete;
  /*!
   * \brief Destructor.
   * \steady    FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  ~LocalServerImpl() noexcept final = default;  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298

  LocalServerImpl(LocalServerImpl const&) = delete;
  LocalServerImpl(LocalServerImpl&&) = delete;
  LocalServerImpl& operator=(LocalServerImpl const&) & = delete;
  LocalServerImpl& operator=(LocalServerImpl&&) & = delete;

  /*!
   * \brief Map type for mapping the method id to the correct registered skeleton request handler object.
   */
  using SkeletonRequestHandlerMap =
      std::map<::amsr::someip_protocol::internal::MethodId, std::weak_ptr<SkeletonMethodRequestHandlerInterface>>;

  /*!
   * \brief Map type for mapping the method id to the correct registered skeleton request no return handler object.
   */

  using SkeletonRequestNoReturnHandlerMap = std::map<::amsr::someip_protocol::internal::MethodId,
                                                     std::weak_ptr<SkeletonMethodRequestNoReturnHandlerInterface>>;

  /*!
   * \copydoc amsr::someip_binding_core::internal::LocalServerInterface::RegisterMethodRequestHandler
   */
  void RegisterMethodRequestHandler(::amsr::someip_protocol::internal::MethodId id,
                                    std::weak_ptr<SkeletonMethodRequestHandlerInterface> handler) noexcept final;

  /*!
   * \copydoc amsr::someip_binding_core::internal::LocalServerInterface::UnregisterMethodRequestHandler
   */
  void UnregisterMethodRequestHandler(::amsr::someip_protocol::internal::MethodId id) noexcept final;

  /*!
   * \copydoc amsr::someip_binding_core::internal::LocalServerInterface::RegisterMethodRequestNoReturnHandler
   */
  void RegisterMethodRequestNoReturnHandler(
      ::amsr::someip_protocol::internal::MethodId id,
      std::weak_ptr<SkeletonMethodRequestNoReturnHandlerInterface> handler) noexcept final;

  /*!
   * \copydoc amsr::someip_binding_core::internal::LocalServerInterface::UnregisterMethodRequestNoReturnHandler
   */
  void UnregisterMethodRequestNoReturnHandler(::amsr::someip_protocol::internal::MethodId id) noexcept final;

 private:
  /*!
   * \brief           Offer a service over the SOME/IP binding from the skeleton.
   * \return          amsr::core::Result<void, Error> that holds no value nor error upon success.
   *                  Otherwise, it contains an error.
   * \pre             All offered methods have a registered response handler.
   * \error           ServerError::kNotAvailable If the server is already owned by another provider local application.
   * \error           ServerError::kSystemPrivilegesNoAccess If the application lacking the proper permission
   *                  for offering server.
   * \error           All other possible errors mapped from someip_daemon_client to be reported to that caller, and
   *                  casted from 'ControlMessageReturnCode' type to 'ServerError' type.
   * \context         App
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \exceptionsafety No side effects.
   * \synchronous     TRUE
   * \steady          FALSE
   */
  amsr::core::Result<void> OfferService() noexcept final;

  /*!
   * \brief       Unoffer a specific service instance from the skeleton.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  void StopOfferService() noexcept final;

  /*!
   * \brief       An instantiated skeleton implementation will send an event notification to the client.
   *              If the service is not yet offered, the event will be stored as initial value if it belongs
   *              to field notification.
   * \param[in]   packet Contains the complete SomeIpPacket of header and payload.
   * \pre         Connection to the SOME/IP Daemon is established
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  void SendEventNotification(MemoryBufferPtr packet) noexcept final;

  /*!
   * \brief An instantiated skeleton implementation will send a signal based event notification to the client.
   * \param[in] packet Contains the complete SomeIpPacket of header and payload.
   * \pre Connection to the SOME/IP Daemon is established.
   * \pre Service is offered
   * \context App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  void SendPduEventNotification(MemoryBufferPtr packet) noexcept final;

  /*!
   * \brief       An instantiated skeleton implementation will send a method response to the client.
   * \param[in]   packet      Contains the complete SomeIpPacket of header and payload.
   * \param[in]   requester_address Contains the IP address and port of the sender of the method request.
   * \pre         Connection to the SOME/IP Daemon is established
   * \pre         Service is offered
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  void SendMethodResponse(
      MemoryBufferPtr packet,
      ::amsr::someip_binding_core::internal::methods::RequesterAddress const& requester_address) noexcept final;

  /*!
   * \brief       Send a SOME/IP error response packet.
   * \details     Helper function for SendErrorResponse that does the actual work without
   *              checking whether the service is on offer.
   * \param[in]   return_code    The return code to write into the SOME/IP header.
   * \param[in]   request_header The unmodified SOME/IP header.
   *                             This SOME/IP header will be used for assembling the error
   *                             response and is partially being reused.
   * \param[in]   requester_address Contains the IP address and port of the sender of the method request.
   * \pre         Connection to the SOME/IP Daemon is established
   * \pre         Service is offered
   * \context     App|Reactor
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  void DoSendErrorResponse(
      someip_protocol::internal::ReturnCode return_code,
      someip_protocol::internal::SomeIpMessageHeader const& request_header,
      ::amsr::someip_binding_core::internal::methods::RequesterAddress const& requester_address) noexcept;

  /*!
   * \brief       Send a SOME/IP error response packet.
   * \details     Checks if service is on offer and delegates to DoSendErrorResponse. Used in case of:
   *              - Infrastructural checks failed (protocol version, return code, message type)
   *              - Deserialization failed
   *              - Serialization of method responses failed
   *              - Service / Method is not implemented.
   * \param[in]   return_code    The return code to write into the SOME/IP header.
   * \param[in]   request_header The unmodified SOME/IP header.
   *                             This SOME/IP header will be used for assembling the error
   *                             response and is partially being reused.
   * \param[in]   requester_address Contains the IP address and port of the sender of the method request.
   * \pre         Connection to the SOME/IP Daemon is established
   * \pre         Service is offered
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  void SendErrorResponse(
      someip_protocol::internal::ReturnCode return_code,
      someip_protocol::internal::SomeIpMessageHeader const& request_header,
      ::amsr::someip_binding_core::internal::methods::RequesterAddress const& requester_address) noexcept final;

  /*!
   * \copydoc methods::SkeletonMethodRequestHandlerInterface::HandleMethodRequest
   */
  ::amsr::core::Result<void> HandleMethodRequest(
      ::amsr::someip_binding_core::internal::methods::Request&& request) noexcept final;

  /*!
   * \copydoc methods::SkeletonMethodRequestNoReturnHandlerInterface::HandleMethodRequestNoReturn
   */
  ::amsr::core::Result<void> HandleMethodRequestNoReturn(
      ::amsr::someip_binding_core::internal::methods::RequestNoReturn&& request) noexcept final;

  /*!
   * \brief The provided service instance ID.
   */
  ProvidedServiceInstanceId const provided_service_instance_id_;

  /*!
   * \brief The transport-to-daemon object.
   */
  ServerTransportToDaemonInterface& transport_to_daemon_;

  /*!
   * \brief The server manager.
   */
  ServerManagerInterface& server_manager_;

  /*!
   * \brief The method request handlers.
   * \details For each deployed method ID, one entry shall exist in the map. The required handler set that is passed in
   * the constructor is initialized to expired weak pointers. The actual handlers are registered during construction of
   * a Skeleton MethodXf object in generated code and removed from the map during destruction of the Skeleton MethodXf
   * object. During HandleMethodRequest, the map is used to find out whether a corresponding handler for the received
   * method ID exists which will then take care of further processing.
   */
  SkeletonRequestHandlerMap request_handlers_;

  /*!
   * \brief The method request no return handlers.
   * \details For each deployed method ID, one entry shall exist in the map. The required handler set that is passed in
   * the constructor is initialized to expired weak pointers. The actual handlers are registered during construction of
   * a Skeleton FireAndForgetMethodXf object in generated code and removed from the map during destruction of the
   * Skeleton FireAndForgetMethodXf object. During HandleMethodRequest, the map is used to find out whether a
   * corresponding handler for the received method ID exists which will then take care of further processing.
   */
  SkeletonRequestNoReturnHandlerMap request_no_return_handlers_;

  /*!
   * \brief Error message allocator.
   */
  FlexibleUniqueMemoryBufferAllocator error_message_memory_buffer_allocator_;

  /*!
   * \brief Flag used to indicate whether the service is currently on offer.
   */
  bool is_offered_{false};

  /*!
   * \brief Our logger
   */
  ::amsr::someip_binding_core::internal::logging::AraComLogger logger_;
};

}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_LOCAL_SERVER_IMPL_H_
