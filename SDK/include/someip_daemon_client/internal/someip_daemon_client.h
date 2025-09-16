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
/*!        \file  someip_daemon_client/internal/someip_daemon_client.h
 *        \brief  Access library to SOME/IP daemon functionality
 *      \details  This file provides an API for communication with the SomeIP daemon.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_SOMEIP_DAEMON_CLIENT_H_
#define LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_SOMEIP_DAEMON_CLIENT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/core/future.h"
#include "osabstraction/io/io_buffer.h"
#include "osabstraction/io/reactor1/reactor1.h"
#include "someip_daemon_client/internal/common_types.h"
#include "someip_daemon_client/internal/config_model.h"
#include "someip_daemon_client/internal/logging/ara_com_logger.h"
#include "vac/memory/allocator/flexible/flexible_unique_memory_buffer_allocator.h"
#include "vac/memory/non_zeroing_allocator.h"

/*!
 * \brief String literals.
 */
// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOMEIPDAEMONCLIENT_AutosarC++17_10-M7.3.6_using-directives_in_header_file
using vac::container::literals::operator""_sv;

namespace amsr {
namespace someip_daemon_client {
namespace internal {

/*!
 * \brief SomeIpDaemonClient API providing access to the SOME/IP daemon functionality.
 * \details An IPC communication channel is used for communication with the SOME/IP daemon.
 * \tparam TemplateConfiguration The configuration of SomeIpDaemonClient.
 *          Contains the following required type definition:
 *         - typename TemplateConfiguration::IpcConnectionWrapperType
 *         - typename TemplateConfiguration::MessageSenderType
 *         - typename TemplateConfiguration::MessageReceiverType
 *         - typename TemplateConfiguration::RoutingControllerType
 *         - typename TemplateConfiguration::ProxySomeIpDaemonClientType
 *         - typename TemplateConfiguration::SkeletonSomeIpDaemonClientType
 */
template <typename TemplateConfiguration>
class SomeIpDaemonClient {
 public:
  /*!
   * \brief Type alias for used IPC connection handler.
   */
  using IpcConnectionWrapper = typename TemplateConfiguration::IpcConnectionWrapperType;

  /*!
   * \brief Type alias used for Message Sender.
   */
  using MessageSender = typename TemplateConfiguration::MessageSenderType;

  /*!
   * \brief Type alias for Message Receiver.
   */
  using MessageReceiver = typename TemplateConfiguration::MessageReceiverType;

  /*!
   * \brief Type alias for Routing Controller.
   */
  using ReceiveRoutingController = typename TemplateConfiguration::RoutingControllerType;

  /*!
   * \brief Type alias for ProxySomeIpDaemonClient.
   */
  using ProxySomeIpDaemonClient = typename TemplateConfiguration::ProxySomeIpDaemonClientType;

  /*!
   * \brief Type alias for SkeletonSomeIpDaemonClient.
   */
  using SkeletonSomeIpDaemonClient = typename TemplateConfiguration::SkeletonSomeIpDaemonClientType;

  SomeIpDaemonClient() = delete;

  SomeIpDaemonClient(SomeIpDaemonClient const&) = delete;

  SomeIpDaemonClient(SomeIpDaemonClient&&) = delete;

  auto operator=(SomeIpDaemonClient const&) & -> SomeIpDaemonClient& = delete;

  auto operator=(SomeIpDaemonClient&&) & -> SomeIpDaemonClient& = delete;

  /*!
   * \brief Constructor of SomeIpDaemonClient.
   * \details In this constructor all events triggered by the reactor thread must be activated / registered.
   * \param[in]      config  A configuration.
   * \param[in, out] reactor A reactor used for asynchronous event notification.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \vprivate  Product Private
   * \steady FALSE
   *
   * \internal
   * - Construct an IpcConnectionWrapper.
   * - Construct IpcConnection providing the IpcConnectionWrapper to it.
   * - Construct a MessageSender.
   * - Construct a MessageReceiver.
   * - Register the BasicIpc callback functions in the IpcConnectionWrapper.
   * \endinternal
   */
  SomeIpDaemonClient(SomeIpDaemonClientConfigModel const& config, osabstraction::io::reactor1::Reactor1& reactor)
      // VCA_SDCL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
      : logger_{logging::kSomeIpLoggerContextId, logging::kSomeIpLoggerContextDescription,
                // VCA_SDCL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
                amsr::core::StringView{"SomeIpDaemonClient"}},
        rx_buffer_allocator_{},
        config_{config},
        receive_routing_controller_{},  // VCA_SDCL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED
                                        // VCA_SDCL_REFERENCE_PARAMETERS_ALLOCATED_IN_OTHER_COMPONENT
        ipc_connection_wrapper_{reactor},
        message_sender_{config_, ipc_connection_wrapper_, &is_running_},        // VCA_SDCL_THIS_POINTER
        message_receiver_{rx_buffer_allocator_, receive_routing_controller_} {  // VCA_SDCL_THIS_POINTER
    // clang-format off
    // VCA_SDCL_UNIQUE_FUNCTION
    vac::language::UniqueFunction<void()> process_received_message{ [this]() { message_receiver_.ProcessReceivedMessage(); }};  // COV_SOMEIPDAEMON_CLIENT_LAMBDA  // NOLINT(whitespace/line_length)
    // clang-format on
    // VCA_SDCL_UNIQUE_FUNCTION
    ipc_connection_wrapper_.RegisterOnReceiveCompletionCallback(std::move(process_received_message));

    // clang-format off
    // VCA_SDCL_UNIQUE_FUNCTION
    vac::language::UniqueFunction<void()> complete_message_send{ [this]() { message_sender_.CompleteMessageSend(); }};  // COV_SOMEIPDAEMON_CLIENT_LAMBDA  // NOLINT(whitespace/line_length)
    // clang-format on
    // VCA_SDCL_UNIQUE_FUNCTION
    ipc_connection_wrapper_.RegisterOnSendCompletionCallback(std::move(complete_message_send));

    // clang-format off
    // VCA_SDCL_UNIQUE_FUNCTION
    vac::language::UniqueFunction<MutableIOBufferContainerView(std::size_t const&)> prepare_receive_header_memory_buffer{ [this](std::size_t const& message_length) { return message_receiver_.PrepareReceiveHeaderMemoryBuffer(message_length); }};  // COV_SOMEIPDAEMON_CLIENT_LAMBDA  // NOLINT(whitespace/line_length)
    // clang-format on
    // VCA_SDCL_UNIQUE_FUNCTION
    ipc_connection_wrapper_.RegisterOnMessageAvailableCallback(std::move(prepare_receive_header_memory_buffer));

    // clang-format off
    // VCA_SDCL_UNIQUE_FUNCTION
    vac::language::UniqueFunction<MutableIOBufferContainerView(std::size_t const&)> process_and_prepare_receive_memory_buffer{ [this](std::size_t const& remaining_message_length) { return message_receiver_.ProcessChunkAndPrepareReceiveMemoryBuffer(remaining_message_length); }};  // COV_SOMEIPDAEMON_CLIENT_LAMBDA  // NOLINT(whitespace/line_length)
    // clang-format on
    // VCA_SDCL_UNIQUE_FUNCTION
    ipc_connection_wrapper_.RegisterOnChunkAvailableCallback(std::move(process_and_prepare_receive_memory_buffer));
  }  // VCA_SDCL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED

  /*!
   * \brief Destructor.
   *
   * \pre       The related reactor thread must be stopped.
   * \context   Shutdown
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate  Product Private
   * \steady FALSE
   *
   * \internal
   * - Disconnect from the SOME/IP daemon.
   * \endinternal
   */
  virtual ~SomeIpDaemonClient() noexcept { Disconnect(); }  // VCA_SDCL_COMPILER_GENERATED_FUNCTION_IMPLICITLY_CALLED

  /*!
   * \brief Start all dynamic actions of the SOME/IP daemon client.
   * \details In this API context, asynchronous messages reception is started.
   * \pre Connect() API of SomeIpDaemonClient is called.
   * \pre Start() API of SomeIpDaemonClient is not called.
   * \context Init
   * \threadsafe FALSE
   * \vprivate  Product Private
   * \steady FALSE
   */
  void Start() noexcept {
    if (is_running_) {
      logger_.LogFatalAndAbort(
          [](logging::AraComLogger::StringStream& s) {
            s << "SomeIpDaemonClient already started.";  // VCA_SDCL_STRING_STREAM_USAGE
                                                         // VCA_SDCL_STRING_STREAM_USAGE
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    is_running_ = true;
    // Start receive path of IPC connection
    ipc_connection_wrapper_.StartReceiving();  // VCA_SDCL_NO_PARAMETERS_TO_CHECK
  }

  // ---- Connection handling API ----------------------------------------------------------------------------------

  /*!
   * \brief Connect to the SOME/IP daemon listening on the configured address.
   * \details This method initiates a new connection to SOME/IP daemon.
   * \return  Future of state showing if the connection has been established and false otherwise.
   * \error   amsr::ipc::Connection::ConnectAsync() error codes if the connect failed synchronously.
   * \error   SomeIpDaemonClientErrc::kAlreadyConnected The error is returned if connection is already established.
   * \pre Start() API of SomeIpDaemonClient is not called.
   * \context Init
   * \threadsafe TRUE
   * \reentrant FALSE
   * \vprivate  Product Private
   * \synchronous FALSE
   * \steady FALSE
   *
   * \internal
   * - Establish connection to the SOME/IP daemon (asynchronous)
   * \endinternal
   */
  auto Connect() noexcept -> amsr::core::Future<void> {
    if (is_running_) {
      logger_.LogFatalAndAbort(
          [](logging::AraComLogger::StringStream& s) {
            s << "Reconnect after SomeIpDaemonClient started is not supported.";  // VCA_SDCL_STRING_STREAM_USAGE
            // VCA_SDCL_STRING_STREAM_USAGE
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    logger_.LogDebug(
        [this](ara::log::LogStream& s) {
          s << "(0x" << ara::log::HexFormat(config_.someipd_ipc_domain) << ", 0x"
            << ara::log::HexFormat(config_.someipd_ipc_port) << ")";
        },
        {__func__}, {__LINE__});
    // VCA_SDCL_PARAMETERS_ON_STACK
    amsr::core::Future<void> future{ipc_connection_wrapper_.ConnectAsync(
        amsr::ipc::UnicastAddress{config_.someipd_ipc_domain, config_.someipd_ipc_port})};
    return future;
  }

  /*!
   * \brief Get a reference for the skeleton of SOME/IP daemon client.
   * \return Reference for the skeleton of SOME/IP daemon client.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetSkeleton() noexcept -> SkeletonSomeIpDaemonClient& { return skeleton_; }
  /*!
   * \brief Get a reference for the proxy of SOME/IP daemon client.
   * \return Reference for the proxy of SOME/IP daemon client.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetProxy() noexcept -> ProxySomeIpDaemonClient& { return proxy_; }

 protected:
  /*!
   * \brief Gets a reference of the config.
   * \details This function is used for testing.
   * \return A reference of the config.
   * \pre -
   * \context Init
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetConfig() const noexcept -> SomeIpDaemonClientConfigModel const& { return config_; }

  /*!
   * \brief Get a reference for the wrapper handler for the IPC connection.
   * \details This function is used for testing.
   * \return Reference for the IPC connection wrapper handler.
   * \pre -
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetIpcConnectionWrapper() noexcept -> IpcConnectionWrapper& { return ipc_connection_wrapper_; }

 private:
  /*!
   * \copydoc amsr::someip_daemon_client::internal::IpcConnectionWrapper::Disconnect
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   * \internal
   * - Disconnect from the SOME/IP daemon.
   * \endinternal
   */
  void Disconnect() noexcept { ipc_connection_wrapper_.Disconnect(); }  // VCA_SDCL_NO_PARAMETERS_TO_CHECK

  /*!
   * \brief Logger.
   */
  logging::AraComLogger logger_;

  /*!
   * brief Memory buffer allocator used for received routing messages.
   */
  vac::memory::allocator::flexible::FlexibleUniqueMemoryBufferAllocator<vac::memory::NonZeroingAllocator<std::uint8_t>>
      rx_buffer_allocator_{};

  /*!
   * \brief SomeIpDaemonClient configuration model.
   */
  SomeIpDaemonClientConfigModel const config_;

  /*!
   * \brief ReceiveRoutingController handling routing messages.
   */
  ReceiveRoutingController receive_routing_controller_;

  /*!
   * \brief Wrapper for IPC communication with SOME/IP daemon.
   */
  IpcConnectionWrapper ipc_connection_wrapper_;

  /*!
   * \brief Flag indicating if the SomeIpDaemonClient is started.
   */
  bool is_running_{false};

  /*!
   * \brief A class responsible for handling the sending of IPC messages.
   */
  MessageSender message_sender_;

  /*!
   * \brief A class responsible for handling the receiving of IPC messages.
   */
  MessageReceiver message_receiver_;

  /*!
   * \brief ProxySomeIpDaemonClient handling the proxy part of SOME/IP daemon client.
   */
  ProxySomeIpDaemonClient proxy_{ipc_connection_wrapper_, message_receiver_, message_sender_,
                                 &receive_routing_controller_};

  /*!
   * \brief SkeletonSomeIpDaemonClient handling the skeleton part of SOME/IP daemon client.
   */
  SkeletonSomeIpDaemonClient skeleton_{ipc_connection_wrapper_, message_receiver_, message_sender_,
                                       &receive_routing_controller_};
};

}  // namespace internal
}  // namespace someip_daemon_client
}  // namespace amsr

// COV_JUSTIFICATION_BEGIN
//
// \ID COV_SOMEIPDAEMON_CLIENT_LAMBDA
//   \ACCEPT XX
//   \REASON The callback functions are called and covered using SomeipdaemonclientMock class hence real object for
//           Someipdaemonclient not created.
//
// COV_JUSTIFICATION_END

#endif  // LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_SOMEIP_DAEMON_CLIENT_H_
