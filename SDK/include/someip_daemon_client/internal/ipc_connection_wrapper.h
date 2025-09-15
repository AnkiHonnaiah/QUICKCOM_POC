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
/*!        \file  someip_daemon_client/internal/ipc_connection_wrapper.h
 *        \brief  Wrapper for the BasicIPC connection operations
 *        \details  This class holds the IPC connection object and keeps track of the connection state.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_IPC_CONNECTION_WRAPPER_H_
#define LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_IPC_CONNECTION_WRAPPER_H_

#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <type_traits>
#include <utility>

#include "amsr/core/future.h"
#include "amsr/core/promise.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/core/string_view.h"
#include "amsr/ipc/ipc_types.h"
#include "amsr/ipc/unicast_address.h"
#include "ara/core/memory_resource.h"
#include "ara/log/logger.h"
#include "ara/log/logstream.h"
#include "osabstraction/io/io_buffer.h"
#include "osabstraction/io/reactor1/reactor1.h"
#include "someip_daemon_client/internal/common_types.h"
#include "someip_daemon_client/internal/logging/ara_com_logger.h"
#include "someip_daemon_client/internal/someip_daemon_client_error_code.h"
#include "someipd_app_protocol/internal/message.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace someip_daemon_client {
namespace internal {

/*!
 * \brief The maximum wait time to receive a response from daemon for a control command.
 */
using ResponseTimeout = std::chrono::steady_clock::duration;

/*!
 * \brief Holds the IPC connection object and keeps track of the connection state.
 * \tparam TemplateConfiguration The configuration of the Ipc Connection Wrapper.
 *          Contains the following required type definition:
 *         - typename TemplateConfiguration::ConnectionConfigType
 */
template <typename TemplateConfiguration>
class IpcConnectionWrapper {
 public:
  /*!
   * \brief A type alias for the IPC connection.
   */
  using ConnectionType = typename TemplateConfiguration::ConnectionConfigType::IpcConnectionType;

  /*!
   * \brief Type-alias for the const I/O memory buffer.
   */
  using ConstIOBuffer = osabstraction::io::ConstIOBuffer;

  /*!
   * \brief Type-alias for the const I/O memory buffer view.
   */
  using ConstIOBufferContainerView = amsr::core::Span<ConstIOBuffer>;

  /*!
   * \brief     Initialize all the resources of the IPC Connection.
   * \param[in] reactor                      Reactor for asynchronous event processing.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  explicit IpcConnectionWrapper(osabstraction::io::reactor1::Reactor1& reactor)
      : lock_{},
        reactor_{reactor},
        logger_{logging::kSomeIpLoggerContextId, logging::kSomeIpLoggerContextDescription,
                amsr::core::StringView{"IpcConnectionWrapper"}},
        // VCA_SDCL_PARAMETERS_ON_STACK, VCA_SDCL_PARAMETER_PASSED_BY_VALUE
        connection_{reactor_},
        connection_state_{ConnectionState::kDisconnected},
        send_completion_callback_function_{},
        message_available_callback_function_{},
        disconnect_request_service_callback_function_{},
        disconnect_request_local_server_callback_function_{},
        receive_chunk_callback_function_{},
        receive_completion_callback_function_{} {}

  /*!
   * \brief Delete default constructor.
   */
  IpcConnectionWrapper() = delete;

  /*!
   * \brief Delete copy constructor.
   */
  IpcConnectionWrapper(IpcConnectionWrapper const&) = delete;

  /*!
   * \brief Delete move constructor.
   */
  IpcConnectionWrapper(IpcConnectionWrapper&&) = delete;

  /*!
   * \brief Delete copy assignment.
   */
  auto operator=(IpcConnectionWrapper const&) & -> IpcConnectionWrapper& = delete;

  /*!
   * \brief Delete move assignment.
   */
  auto operator=(IpcConnectionWrapper&&) & -> IpcConnectionWrapper& = delete;

  /*!
   * \brief       Destructs IpcConnectionWrapper.
   *
   * \pre         The related reactor thread must be stopped.
   *              This is necessary to avoid race conditions with potentially running BasicIpc callback functions
   *              and the destruction of the BasicIpc connection.
   * \context     Shutdown
   * \reentrant   FALSE
   * \steady FALSE
   *
   * \internal
   * - Check precondition that no BasicIpc callback is executed before destruction.
   * \endinternal
   */
  // VCA_SDCL_SLC23_NO_STATIC_MEMORY_FREED
  virtual ~IpcConnectionWrapper() noexcept {
    // The BasicIpc connection can be destructed now without waiting for potentially running callback functions.
    // No synchronization / wait necessary due to precondition that the reactor thread must be already stopped/joined.

    // Perform a final precondition check that no BasicIpc callback is executed.
    // VCA_SDCL_FUNCTION_NO_PRECONDITION
    if (connection_.IsInUse()) {
      logger_.LogFatalAndAbort(
          [](logging::AraComLogger::StringStream& s) {
            s << "The BasicIpc connection is still in use during destruction. This is a violation of the "  // VCA_SDCL_STRING_STREAM_USAGE
                 "implementation "  // VCA_SDCL_STRING_STREAM_USAGE
                 "precondition that the reactor thread must be stopped before destruction.";  // VCA_SDCL_STRING_STREAM_USAGE
            // VCA_SDCL_STRING_STREAM_USAGE
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }  // VCA_SDCL_DESTRUCTOR_NO_PRECONDITION

  /*!
   * \brief     Returns the current connection state.
   * \return    The current connection state.
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetConnectionState() const noexcept -> ConnectionState { return connection_state_; }

  /*!
   * \brief     Connects asynchronously to the SOME/IP daemon listening on the given address.
   * \details   This method initiates a new connection to SOME/IP daemon asynchronously.
   * \param[in] address The IPC address of a SOME/IP daemon.
   * \return    Future containing the state showing if the connection has been established or not.
   * \error     amsr::ipc::Connection::ConnectAsync() error codes if the asynchronous connection establishment has not
   * been started successfully.
   * \error     SomeIpDaemonClientErrc::kAlreadyConnected The error is returned if connection is already established.
   * \pre -
   * \context   Init
   * \reentrant FALSE
   * \synchronous FALSE
   * \threadsafe TRUE
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Set the result (return value of the function) to false.
   * - Enter the critical section.
   * - Check if the connection has not been established yet.
   *   - Establish the connection asynchronously.
   *   - Check if the asynchronous connection request was successful.
   *     - Log the debug message.
   *   - Otherwise log an error and reset own connection state to 'disconnected'.
   * - Leave the critical section.
   * \endinternal
   */
  auto ConnectAsync(amsr::ipc::UnicastAddress const& address) noexcept -> amsr::core::Future<void> {
    amsr::core::Future<void> connection_result{};
    logger_.LogDebug(
        [this, &address](ara::log::LogStream& s) {
          ara::log::LogHex32 const address_domain_hex{ara::log::HexFormat(address.GetDomain())};
          ara::log::LogHex32 const address_port_hex{ara::log::HexFormat(address.GetPort())};
          ConnectionState const connection_state{connection_state_};  // VCA_SDCL_CONNECTIION_STATE_IS_VALID
          ConnectionStateUnderlyingType const connection_state_select{
              static_cast<ConnectionStateUnderlyingType>(connection_state)};
          s << "Connecting to the SOME/IP daemon (BasicIPC domain: 0x" << address_domain_hex << ", port: 0x"
            << address_port_hex << ", connection state: " << connection_state_select << ")";
        },
        {__func__}, {__LINE__});

    std::unique_lock<std::mutex> unique_lock{lock_, std::defer_lock};
    unique_lock.lock();

    if (connection_state_ == ConnectionState::kDisconnected) {
      connection_result = connection_promise_.get_future();
      is_connection_promise_in_use_ = true;
      // Start asynchronous connection establishment
      amsr::core::Result<void> const connect_result{
          // VCA_SDCL_PARAMETERS_ON_STACK, VCA_SDCL_PARAMETER_PASSED_BY_VALUE
          connection_.ConnectAsync(address, [this](amsr::core::Result<void>&& connect_complete_result) {
            OnConnectCompletion(std::move(connect_complete_result));  // VCA_SDCL_LNG03_POINTER_LAMBDA_CAPTURE
          })};

      // Check if the asynchronous connection establishment has been started successfully.
      if (connect_result.HasValue()) {
        logger_.LogDebug(
            [&address](ara::log::LogStream& s) {
              ara::log::LogHex32 const address_domain_hex{ara::log::HexFormat(address.GetDomain())};
              ara::log::LogHex32 const address_port_hex{ara::log::HexFormat(address.GetPort())};
              s << "Waiting for connection establishment to the SOME/IP daemon (BasicIPC domain: 0x"
                << address_domain_hex << ", port: 0x" << address_port_hex;
            },
            {__func__}, {__LINE__});
      } else {  // ConnectAsync failed.
        connection_state_ = ConnectionState::kDisconnected;
        connection_promise_.SetError(connect_result.Error());
        ResetConnectionPromise();

        logger_.LogError(
            [&address, &connect_result](::ara::log::LogStream& s) {
              ara::log::LogHex32 const address_domain_hex{ara::log::HexFormat(address.GetDomain())};
              ara::log::LogHex32 const address_port_hex{ara::log::HexFormat(address.GetPort())};
              // VCA_SDCL_METHOD_ERROR_CODE_MESSAGE_IS_VALID
              ::amsr::core::StringView const err_str{connect_result.Error().Message()};
              s << "Error occurred while establishing connection to the SOME/IP daemon. BasicIPC domain: 0x"
                << address_domain_hex << ", port: 0x" << address_port_hex << ", ConnectAsync error: " << err_str;
            },
            {__func__}, {__LINE__});
      }
    } else {
      amsr::core::Promise<void> connection_promise{ara::core::GetDefaultResource()};
      connection_result = connection_promise.get_future();
      connection_promise.SetError(SomeIpDaemonClientErrc::kAlreadyConnected);
      logger_.LogError(
          [](ara::log::LogStream& s) {
            s << "Connection to SOME/IP daemon is already established or is currently being established.";
          },
          {__func__}, {__LINE__});
    }
    return connection_result;
  }

  /*!
   * \brief     Disconnects from the connected SOME/IP daemon.
   * \pre       -
   * \context   Reactor | Shutdown
   * \reentrant FALSE
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Initialize the result (return value of the function) to false.
   * - Enter the critical section.
   * - Check if the connection is established.
   *   - Close the BasicIpc connection
   * - Check if the connection promise has value
   *   - Reset the connection promise.
   * - Reset the connection state to 'disconnected'
   * - Notify the potentially blocked control commands.
   * - Leave the critical section.
   * - Notify about connection state change.
   * \endinternal
   */
  void Disconnect() noexcept {
    logger_.LogDebug(
        [this](ara::log::LogStream& s) {
          ConnectionState const connection_state{connection_state_};  // VCA_SDCL_CONNECTIION_STATE_IS_VALID
          ConnectionStateUnderlyingType const connection_state_select{
              static_cast<ConnectionStateUnderlyingType>(connection_state)};
          s << "Connection state " << connection_state_select;
        },
        {__func__}, {__LINE__});

    std::unique_lock<std::mutex> unique_lock{lock_, std::defer_lock};
    unique_lock.lock();

    // Close the BasicIpc connection if connection was already established.
    if (connection_state_ == ConnectionState::kConnected) {
      static_cast<void>(connection_.Close());  // VCA_SDCL_FUNCTION_NO_PRECONDITION
    }
    if (is_connection_promise_in_use_) {
      connection_promise_.SetError(SomeIpDaemonClientErrc::kDisconnected);
      ResetConnectionPromise();
    }
    connection_state_ = ConnectionState::kDisconnected;
    // Notify blocked control command.
    disconnect_request_service_callback_function_();
    disconnect_request_local_server_callback_function_();
    // Release lock and notify about connection state change
    unique_lock.unlock();
    logger_.LogDebug(
        [this](ara::log::LogStream& s) {
          // VCA_SDCL_CALLING_GET_CONNECTIION_STATE_IS_SAFE
          ConnectionState const connection_state{this->GetConnectionState()};
          s << "Connection state: "
            << kConnectionStateEnumStr[static_cast<std::underlying_type_t<ConnectionState>>(connection_state)];
        },
        {__func__}, {__LINE__});
  }

  /*!
   * \brief     Trigger an asynchronous reception.
   * \details   If message reception is not successfully completed, the error will be handled in the OnReceiveCompletion
   *            callback.
   *            If message reception was successful the next asynchronous reception will be started immediately.
   * \pre       -
   * \context   Reactor
   * \reentrant FALSE
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Start the asynchronous reception of the next packet
   * - If the asynchronous reception could not be started, log an error and disconnect from the SOME/IP daemon.
   * \endinternal
   */
  void StartReceiving() noexcept {
    amsr::ipc::MessageAvailableCallback available_callback{
        [this](std::size_t const& message_size) -> MutableIOBufferContainerView {
          return OnMessageAvailable(message_size);  // VCA_SDCL_LNG03_POINTER_LAMBDA_CAPTURE
        }};
    amsr::ipc::MessageChunkReceivedCallback chunk_received_callback{
        [this](amsr::core::Result<amsr::ipc::ReceiveChunkResult>&& result) -> MutableIOBufferContainerView {
          return OnMessageChunkReceived(std::move(result));  // VCA_SDCL_LNG03_POINTER_LAMBDA_CAPTURE
        }};

    // Start asynchronous reception via BasicIPC connection
    amsr::core::Result<void> const receive_async_result{
        // VCA_SDCL_PARAMETERS_ON_STACK, VCA_SDCL_PARAMETERS_ARE_VALID
        connection_.ReceiveAsync(std::move(available_callback), std::move(chunk_received_callback))};

    if (!receive_async_result.HasValue()) {
      logger_.LogError(
          [&receive_async_result](::ara::log::LogStream& s) {
            // VCA_SDCL_METHOD_ERROR_CODE_MESSAGE_IS_VALID
            ::amsr::core::StringView const err_str{receive_async_result.Error().Message()};
            s << "An error occurred while starting an asynchronous message reception. Error message: " << err_str;
          },
          {__func__}, {__LINE__});

      Disconnect();
    }
  }

  /*!
   * \brief     Register a callback function to be called when the send operation is completed successfully.
   * \param     callback_function The callback function to be called when the send is completed.
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Set the variable that holds a reference to the callback function.
   * \endinternal
   */
  void RegisterOnSendCompletionCallback(vac::language::UniqueFunction<void()> callback_function) noexcept {
    // VCA_SDCL_LNG01_POSSIBLY_INVALID_ARGUMENT
    send_completion_callback_function_ = std::move(callback_function);
  }

  /*!
   * \brief     Register a callback function to be called when there is a message available to be received.
   * \param     callback_function The callback function to be called when there is a message available.
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Set the variable that holds a reference to the callback function.
   * \endinternal
   */
  void RegisterOnMessageAvailableCallback(
      vac::language::UniqueFunction<MutableIOBufferContainerView(std::size_t const&)> callback_function) noexcept {
    // VCA_SDCL_LNG01_POSSIBLY_INVALID_ARGUMENT
    message_available_callback_function_ = std::move(callback_function);
  }

  /*!
   * \brief     Register a callback function to be called when there is a message chunk available to be received.
   * \param     callback_function The callback function to be called when there is a message chunk available.
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Set the variable that holds a reference to the callback function.
   * \endinternal
   */
  void RegisterOnChunkAvailableCallback(
      vac::language::UniqueFunction<MutableIOBufferContainerView(std::size_t const&)> callback_function) noexcept {
    // VCA_SDCL_LNG01_POSSIBLY_INVALID_ARGUMENT
    receive_chunk_callback_function_ = std::move(callback_function);
  }

  /*!
   * \brief     Register a callback function to be called when the receive operation is completed successfully.
   * \param     callback_function The callback function to be called when the receive is completed.
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Set the variable that holds a reference to the callback function.
   * \endinternal
   */
  void RegisterOnReceiveCompletionCallback(vac::language::UniqueFunction<void()> callback_function) noexcept {
    // VCA_SDCL_LNG01_POSSIBLY_INVALID_ARGUMENT
    receive_completion_callback_function_ = std::move(callback_function);
  }

  /*!
   * \brief     Register a callback function for request service to be called when the connection state changes to
   *            Disconnected.
   * \param     callback_function The callback function to be called when the state changes to
   *            Disconnected.
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Set the variable that holds a reference to the callback function.
   * \endinternal
   */
  void RegisterRequestServiceOnDisconnectCallback(vac::language::UniqueFunction<void()> callback_function) noexcept {
    // VCA_SDCL_LNG01_POSSIBLY_INVALID_ARGUMENT
    disconnect_request_service_callback_function_ = std::move(callback_function);
  }

  /*!
   * \brief     Register a callback function for request local server to be called when the connection state changes to
   *            Disconnected.
   * \param     callback_function The callback function to be called when the state changes to
   *            Disconnected.
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \steady FALSE
   *
   * \internal
   * - Set the variable that holds a reference to the callback function.
   * \endinternal
   */
  void RegisterRequestLocalServerOnDisconnectCallback(
      vac::language::UniqueFunction<void()> callback_function) noexcept {
    // VCA_SDCL_LNG01_POSSIBLY_INVALID_ARGUMENT
    disconnect_request_local_server_callback_function_ = std::move(callback_function);
  }

  /*!
   * \brief     Triggers a transmission via BasicIpc connection
   * \param     view Buffer to send
   * \return    Result of the BasicIpc Send() operation.
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Call the BasicIpc Send operation. Call OnSendCompletion() callback when the send is completed.
   * \endinternal
   */
  auto Send(ConstIOBufferContainerView const view) noexcept -> amsr::core::Result<amsr::ipc::SendResult> {
    // VCA_SDCL_PARAMETERS_ON_STACK, VCA_SDCL_PARAMETER_PASSED_BY_VALUE
    return connection_.Send(view, [this](amsr::core::Result<void>&& send_completion_result) {
      OnSendCompletion(std::move(send_completion_result));  // VCA_SDCL_LNG03_POINTER_LAMBDA_CAPTURE
    });
  }

 protected:
  /*!
   * \brief     Get the created IPC connection.
   * \return    Reference to the used IPC connection.
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetConnection() noexcept -> ConnectionType& { return connection_; }

 private:
  /*!
   * \brief The header length is the sum of the generic header length and the specific header length.
   */
  static std::size_t constexpr kHeaderLength{someipd_app_protocol::internal::kGenericMessageHeaderLength +
                                             someipd_app_protocol::internal::kSpecificMessageHeaderLength};

  /*!
   * \brief Underlying type of ConnectionState.
   */
  using ConnectionStateUnderlyingType = std::underlying_type_t<ConnectionState>;

  // ---- BasicIpc asynchronous callback implementations ------------------------------------------

  /*!
   * \brief Implementation of BasicIpc callback notifying about completion or failure of the asynchronous connection
   *        establishment.
   *
   * \param[in] connect_complete_result Result of the asynchronous connection
   * establishment.
   *
   * \pre         ConnectAsync()
   * \context     Reactor
   * \reentrant   FALSE
   * \steady FALSE
   *
   * \internal
   * - If the result from BasicIpc connection establishment call has a value.
   *   - Set own connection state to 'connected'
   *   - Set the connection promise value to true.
   *   - Log about connection state change
   * - Otherwise log an error and reset connection state.
   * \endinternal
   */
  void OnConnectCompletion(amsr::core::Result<void> connect_complete_result) noexcept {
    if (connect_complete_result.HasValue()) {
      {
        std::lock_guard<std::mutex> const lock_guard{lock_};
        if (is_connection_promise_in_use_) {
          connection_state_ = ConnectionState::kConnected;
          logger_.LogInfo([](ara::log::LogStream& s) { s << "Connection to SOME/IP daemon established."; }, {__func__},
                          {__LINE__});
          connection_promise_.set_value();
        }
      }
      // Log about connection state change
      logger_.LogDebug(
          [this](ara::log::LogStream& s) {
            // VCA_SDCL_CALLING_GET_CONNECTIION_STATE_IS_SAFE
            ConnectionState const connection_state{this->GetConnectionState()};
            s << "Connection state: "
              << kConnectionStateEnumStr[static_cast<std::underlying_type_t<ConnectionState>>(connection_state)];
          },
          {__func__}, {__LINE__});
    } else {
      logger_.LogError(
          [&connect_complete_result](::ara::log::LogStream& s) {
            // VCA_SDCL_METHOD_ERROR_CODE_MESSAGE_IS_VALID
            ::amsr::core::StringView const err_str{connect_complete_result.Error().Message()};
            s << "Error occurred while establishing connection to the SOME/IP daemon. Error: " << err_str;
          },
          {__func__}, {__LINE__});
      {
        std::lock_guard<std::mutex> const lock_guard{lock_};
        if (is_connection_promise_in_use_) {
          connection_promise_.SetError(connect_complete_result.Error());
          ResetConnectionPromise();
        }
      }
      Disconnect();
    }
  }

  /*!
   * \brief Implementation of BasicIpc callback notifying about availability of a new message.
   *
   * \param[in] message_length The length of the available message in bytes.
   * \return A view to the mutable I/O buffer to be used by BasicIpc to store the available message.
   *
   * \pre         -
   * \context     Reactor
   * \reentrant   FALSE
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Set available message length.
   * - Check if the available message length has at least the size of the generic + specific protocol header
   *   - Perform receive buffer allocation (delegated).
   * - Otherwise log an error and return an empty buffer view.
   * \endinternal
   */
  auto OnMessageAvailable(std::size_t const& message_length) noexcept -> MutableIOBufferContainerView {
    logger_.LogVerbose(
        [&message_length](ara::log::LogStream& s) {
          s << "Length of available message: " << message_length << " bytes.";
        },
        {__func__}, {__LINE__});
    MutableIOBufferContainerView view{};

    if (message_length >= kHeaderLength) {
      available_message_length_ = message_length;
      view = message_available_callback_function_(message_length);
    } else {
      logger_.LogError(
          [&message_length](ara::log::LogStream& s) {
            s << "Received message is too small " << message_length
              << ", expected at least generic header + specific header of size "
              << static_cast<std::uint64_t>(kHeaderLength);
          },
          {__func__}, {__LINE__});
    }

    return view;
  }

  /*!
   * \brief Implementation of BasicIpc callback notifying about completion or failure of an asynchronous message chunk
   *        reception.
   *
   * \param[in] result   Result of the chunk reception, contain remaining chunk size or an error.
   * \return A view to the mutable I/O buffer to be used by BasicIpc to store the next available message chunk into.
   * \pre -
   * \context     Reactor
   * \reentrant   FALSE
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Check if BasicIpc reported a successful message chunk reception.
   *   - Check if remaining chunk size is zero.
   *     - Report message received completion with complete message size.
   *   - Else,
   *     - Log error and report message received completion with an error.
   * - Else, log error and report message received error using the received error.
   * - Reset available message length.
   * \endinternal
   */
  auto OnMessageChunkReceived(amsr::core::Result<amsr::ipc::ReceiveChunkResult> const&& result) noexcept
      -> MutableIOBufferContainerView {
    MutableIOBufferContainerView view{};

    if (result.HasValue()) {
      std::size_t const remaining_message_length{result.Value().remaining_message_size};
      logger_.LogVerbose(
          [&remaining_message_length](ara::log::LogStream& s) {
            s << "Chunk received. Remaining message size: " << remaining_message_length << " bytes.";
          },
          {__func__}, {__LINE__});
      // Chunk received.
      view = receive_chunk_callback_function_(remaining_message_length);

      if (remaining_message_length == 0U) {
        logger_.LogVerbose([](ara::log::LogStream& s) { s << "All chunks received. Receive completed."; }, {__func__},
                           {__LINE__});
        // Everything received.
        OnReceiveCompletion(available_message_length_);
      }
    } else {
      OnReceiveCompletion(amsr::core::Result<std::size_t>::FromError(result.Error()));
    }

    return view;
  }

  /*!
   * \brief Implementation of helper notifying about completion or failure of an asynchronous message reception.
   *
   * \param[in] receive_complete_result   Result of reception completion.
   * \pre -
   * \context     Reactor
   * \reentrant   FALSE
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Check if successful message reception.
   *   - Check if the received message length has at least the size of the generic + specific protocol header.
   *     - Process the received message contents (delegated)
   *     - Start the next asynchronous message reception (chaining of receive requests / exhaustive mode)
   *   - Otherwise, log an error
   * - Otherwise, log an error and disconnect from the SOME/IP daemon.
   * \endinternal
   */
  void OnReceiveCompletion(amsr::core::Result<std::size_t> receive_complete_result) noexcept {
    available_message_length_ = 0U;
    if (receive_complete_result.HasValue()) {
      std::size_t const received_length{receive_complete_result.Value()};
      logger_.LogVerbose([&received_length](ara::log::LogStream& s) { s << "Received length " << received_length; },
                         {__func__}, {__LINE__});

      // Verify that we received at least generic header and specific header
      if (received_length >= kHeaderLength) {
        receive_completion_callback_function_();

        // Trigger an asynchronous reception again.
        StartReceiving();
      } else {
        logger_.LogError(
            [&received_length](ara::log::LogStream& s) {
              s << "Received message is too small " << received_length
                << ", expected at least generic header + specific header of size "
                << static_cast<std::uint64_t>(kHeaderLength);
            },
            {__func__}, {__LINE__});
      }
    } else {
      logger_.LogError(
          [&receive_complete_result](::ara::log::LogStream& s) {
            // VCA_SDCL_METHOD_ERROR_CODE_MESSAGE_IS_VALID
            ::amsr::core::StringView const err_str{receive_complete_result.Error().Message()};
            s << "Error occurred while reception with error message: " << err_str;
          },
          {__func__}, {__LINE__});

      Disconnect();
    }
  }

  /*!
   * \brief Implementation of BasicIpc callback notifying about completion or failure of asynchronous message
   *        transmission.
   *
   * \param[in] send_completion_result    Result of send / transmission completion.
   *
   * \pre -
   * \context     Reactor
   * \reentrant   FALSE
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Check if BasicIpc reported a successful message transmission.
   *   - Enter the critical section.
   *   - Finalize the message transmission (delegated)
   *   - Leave the critical section.
   * - Otherwise, log an error and disconnect from the SOME/IP daemon.
   * \endinternal
   */
  void OnSendCompletion(amsr::core::Result<void> send_completion_result) noexcept {
    if (send_completion_result.HasValue()) {
      // CompleteMessageSend is used from multiple context: Reactor and application thread(s).
      //   case application thread(s): internal data structure are protected by transmit context
      //                               (SendMessage())
      //   case reactor:               internal data structures must be protected.
      //
      send_completion_callback_function_();
    } else {
      logger_.LogError(
          [&send_completion_result](::ara::log::LogStream& s) {
            // VCA_SDCL_METHOD_ERROR_CODE_MESSAGE_IS_VALID
            ::amsr::core::StringView const err_str{send_completion_result.Error().Message()};
            s << "Error occurred during message transmission. Error message: " << err_str;
          },
          {__func__}, {__LINE__});

      Disconnect();
    }
  }

  /*!
   * \brief  Reset the value of the promise and reset the is_promise_in_use_ flag.
   */
  void ResetConnectionPromise() noexcept {
    amsr::core::Promise<void> new_connection_promise{ara::core::GetDefaultResource()};
    connection_promise_.swap(new_connection_promise);
    is_connection_promise_in_use_ = false;
  }

  // ---- Other utilities -------------------------------------------------------------------------
  /*!
   * \brief Mutex used for protecting the IPC connection.
   */
  std::mutex lock_;

  /*!
   * \brief A reference to reactor for asynchronous event processing.
   */
  osabstraction::io::reactor1::Reactor1& reactor_;

  /*!
   * \brief Our logger.
   */
  logging::AraComLogger logger_;

  /*!
   * \brief An IPC connection.
   */
  ConnectionType connection_;

  /*!
   * \brief The connection state.
   */
  std::atomic<ConnectionState> connection_state_;

  /*!
   * \brief Callback function that will be called when the send operation is completed successfully.
   */
  vac::language::UniqueFunction<void()> send_completion_callback_function_;

  /*!
   * \brief Callback function that will be called when there is a message available to be received.
   */
  vac::language::UniqueFunction<MutableIOBufferContainerView(std::size_t const&)> message_available_callback_function_;

  /*!
   * \brief Callback function that will be called when the connection state changes to Disconnected for Request Service.
   */
  vac::language::UniqueFunction<void()> disconnect_request_service_callback_function_;

  /*!
   * \brief Callback function that will be called when the connection state changes to Disconnected for Request Local
   *        Server.
   */
  vac::language::UniqueFunction<void()> disconnect_request_local_server_callback_function_;

  /*!
   * \brief Callback function that will be called when the chunkwise receive operation completed successfully.
   */
  vac::language::UniqueFunction<MutableIOBufferContainerView(std::size_t const&)> receive_chunk_callback_function_;

  /*!
   * \brief Callback function that will be called when the receive operation is completed successfully.
   */
  vac::language::UniqueFunction<void()> receive_completion_callback_function_;

  /*!
   * \brief The promise of connection result.
   */
  amsr::core::Promise<void> connection_promise_{ara::core::GetDefaultResource()};

  /*!
   * \brief  A flag indicating whether the connection promise is in use.
   */
  bool is_connection_promise_in_use_{false};

  /*!
   * \brief Length of message available to be received.
   */
  std::size_t available_message_length_{0U};
};

}  // namespace internal
}  // namespace someip_daemon_client
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CLIENT_INCLUDE_SOMEIP_DAEMON_CLIENT_INTERNAL_IPC_CONNECTION_WRAPPER_H_

/*!
 * \exclusivearea amsr::someip_daemon_client::internal::IpcConnectionWrapper::lock_
 *                Used for protecting the IPC connection.
 *
 * \protects amsr::someip_daemon_client::internal::IpcConnectionWrapper::connection_state_
 *
 *
 * \usedin  amsr::someip_daemon_client::internal::IpcConnectionWrapper::ConnectAsync
 *          amsr::someip_daemon_client::internal::IpcConnectionWrapper::Disconnect
 *          amsr::someip_daemon_client::internal::IpcConnectionWrapper::OnConnectCompletion
 * \exclude User threads from accessing a variable that is already in access.
 * \length LONG IPC communication.
 * \endexclusivearea
 */
