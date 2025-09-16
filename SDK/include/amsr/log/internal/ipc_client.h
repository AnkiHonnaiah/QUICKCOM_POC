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
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file
 *        \brief Contains the declaration for the ipc client to the daemon.
 *      \details
 *
 *         \unit LogAndTrace::Ipc::IpcClient
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_INCLUDE_AMSR_LOG_INTERNAL_IPC_CLIENT_H_
#define LIB_IPC_INCLUDE_AMSR_LOG_INTERNAL_IPC_CLIENT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <cstdint>

#include "amsr/core/array.h"
#include "amsr/core/span.h"
#include "amsr/core/vector.h"
#include "amsr/ipc/connection.h"
#include "amsr/ipc/unicast_address.h"
#include "amsr/log/internal/config/common_compile_time_config.h"
#include "amsr/log/internal/deserialize.h"
#include "amsr/log/internal/ext_fwd.h"
#include "amsr/log/internal/log_error_domain.h"
#include "amsr/log/internal/mpsc/mpsc.h"
#include "amsr/log/internal/reactor.h"
#include "amsr/log/internal/reactor_fwd.h"
#include "amsr/log/internal/sync/strategy.h"
#include "amsr/log/internal/types.h"
#include "amsr/log/internal/utils.h"
#include "amsr/timer/timer.h"
#include "ara/core/vector.h"
#include "vac/language/byte.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace log {
namespace internal {
/*! \brief The send states of the IPC client. */
enum class SendState : std::uint8_t {
  kSync,
  kAsync,
  kIdle,
};

/*!
 * \brief Ipc client.
 *
 * \trace DSGN-LogAndTrace-Ipc
 */
class IpcClient final : amsr::timer::Timer {
 public:
  /*! \brief The data element type. */
  using Data = ara::core::Vector<vac::language::byte>;
  /*! \brief A data queue consumer type. */
  using Consumer = mpsc::Consumer<Data, config::kRemoteSyncStrategy>;

 private:
  /*! \brief The used IPC connection. */
  ipc::Connection connection_;
  /*! \brief The connection to connect to. */
  ipc::UnicastAddress address_;
  /*! \brief Consumers for messages to be sent on IPC connection */
  ara::core::Vector<Consumer> consumers_{};
  /*! \brief Flag to indicate if client try to receive after connection is established. If true use reconnect timer */
  bool const should_receive_;
  /*! \brief Flag to indicate if timer has been started. */
  bool timer_started_{false};
  /*! \brief Callable to execute on reconnects. */
  vac::language::UniqueFunction<void(void) const> on_reconnect_;
  /*! \brief The io vector for the outgoing ipc connection. */
  core::Array<osabstraction::io::ConstIOBuffer, 1> transmission_io_vector_{};
  /*! \brief Data buffer for the current transmitted message. */
  Optional<Data> transmission_data_{};
  /*! \brief Flag that indicates if the client is connected to the server. */
  std::atomic_flag tx_lock_{};
  /*! \brief Flag to indicate if the connection has been reset because of a disconnect. */
  bool reconnect_{false};
  /*! \brief Data buffer used for received messages. */
  Data reception_data_{};
  /*! \brief The io vector for the incoming ipc connection. */
  core::Array<osabstraction::io::MutableIOBuffer, 1> reception_io_vector_{};
  /*! \brief Flag to indicate if sending/receiving messages on connection should be possible. */
  std::atomic_bool is_connected_{false};

 public:
  /*!
   * \brief Constructor for a client.
   *
   * \param[in] reactor         Reactor for asynchronous operations.
   * \param[in] address         Remote address.
   * \param[in] should_receive  Flag to indicate if the client should receive messages.
   * \param[in] on_reconnect    Callable to execute on reconnects.
   * \spec
   *   requires true;
   * \endspec
   */
  IpcClient(
      BorrowedReactor reactor, ipc::UnicastAddress address, bool should_receive,
      // VECTOR NC AutosarC++17_10-M0.1.8: MD_LOG_M0.1.8_EmptyCallback
      vac::language::UniqueFunction<void(void) const>&& on_reconnect = []() {}) noexcept;

  /*! \brief Delete move construction. */
  IpcClient(IpcClient&&) noexcept = delete;
  /*! \brief Delete copy construction. */
  IpcClient(IpcClient const&) noexcept = delete;
  /*! \brief Delete move assignment. */
  auto operator=(IpcClient&&) & noexcept -> IpcClient& = delete;
  /*! \brief Delete copy assignment. */
  auto operator=(IpcClient const&) & noexcept -> IpcClient& = delete;

  /*!
   * \brief Destructor for this client.
   *
   * \pre Cannot be called in context of the owning reactors HandleEvents.
   */
  ~IpcClient() noexcept final;

  /*!
   * \brief Asynchronously connects to the remote server.
   *
   * \return Result of the operation.
   * \spec
   *   requires true;
   * \endspec
   *
   * \vprivate
   */
  void TriggerConnect() noexcept;

  /*!
   * \brief Sends a message to the daemon.
   *
   * \param data The optional data pointer.
   * \return     true if the contained data was adopted.
   * \spec
   *   requires true;
   * \endspec
   *
   * \details Prefers control messages over simple log messages.
   *
   * \vprivate
   */
  auto TriggerSendMessage(Data* data) noexcept -> bool;

  /*!
   * \brief Add a consumer to consumers_.
   *
   * \param consumer The consumer to be added.
   *
   * \spec
   *   requires true;
   * \endspec
   *
   * \vprivate
   */
  void AddConsumer(Consumer&& consumer) noexcept;

  /*!
   * \brief Asynchronously starts receiving messages from the daemon.
   * \spec
   *   requires true;
   * \endspec
   *
   * \vprivate
   */
  void TriggerReceive() noexcept;

  /*!
   * \brief Set the callable triggered on reconnect to a new value.
   *
   * \param callable The callback to be invoked on a reconnect attempt.
   * \spec
   *   requires true;
   * \endspec
   *
   * \vprivate
   */
  void SetOnReconnectCallable(vac::language::UniqueFunction<void(void) const>&& callable) noexcept;

 private:
  /*!
   * \brief Sends a single message to the daemon.
   *
   * \param opt_data The optional message.
   * \return         Either that it's finished sending, it's still sending or sending failed.
   * \spec
   *   requires true;
   * \endspec
   *
   * \details Prefers control messages over simple log messages.
   */
  auto SendMessageInternal(Data** opt_data = nullptr) noexcept -> Result<SendState>;

  /*!
   * \brief React to a finished connection attempt.
   * \spec
   *   requires true;
   * \endspec
   *
   * \param result The result of the connection.
   */
  void OnConnectionComplete(Result<void> result) noexcept;

  /*!
   * \brief React to a finished send attempt.
   * \spec
   *   requires true;
   * \endspec
   *
   * \param result The result of the TX process.
   */
  void OnSendComplete(Result<void> result) noexcept;

  /*!
   * \brief Will handle errors during connecting
   * \spec
   *   requires true;
   * \endspec
   *
   * \param error_code The error that occurred.
   */
  void HandleErrorConnecting(ErrorCode error_code) noexcept;

  /*!
   * \brief Will handle errors during send operations.
   * \spec
   *   requires true;
   * \endspec
   *
   * \param error_code The error that occurred.
   */
  void HandleErrorSending(ErrorCode error_code) noexcept;

  /*!
   * \brief Gets the buffer view to pass to the connection.
   * \spec
   *   requires true;
   * \endspec
   *
   * \return The buffer view.
   */
  auto GetBuffer() noexcept -> core::Span<osabstraction::io::ConstIOBuffer>;

  /*!
   * \brief Gets more data from one of the consumer queues and inserts it into the buffer.
   *
   * \pre The passed opt_data pointer, if not nullptr, must refer to a valid pointer i.e. not nullptr.
   *
   * \param opt_data The optional message.
   * \return         True in case data is available to send.
   */
  auto PrepareData(Data** opt_data) noexcept -> bool;

  /*!
   * \brief React to a message being ready to be received.
   * \spec
   *   requires true;
   * \endspec
   *
   * \param[in] message_length The length of the message.
   * \return                   The available buffer to write the message to.
   */
  auto OnMessageAvailable(std::size_t message_length) noexcept -> amsr::core::Span<osabstraction::io::MutableIOBuffer>;

  /*!
   * \brief React to a message being received.
   * \spec
   *   requires true;
   * \endspec
   *
   * \param[in] received_length Contains the result of the reception & the length.
   */
  void OnMessageReceived(amsr::core::Result<std::size_t> received_length) noexcept;

  /*!
   * \brief Call if a SetLogLevel message is received.
   * \spec
   *   requires true;
   * \endspec
   *
   * \param message_result The result of deserializing the message.
   * \return               Nothing on success, error on failure to set log level.
   *
   * \details The deserialized SetLogLevel message should only update the log level if the application id is empty or if
   *          it matches the id in the config. Empty application id indicates that the message has been sent to all
   *          applications. Is aligned with DLT SetLogLevel command specified in [PRS_Dlt_00194].
   */
  static auto OnSetLogLevel(Deserializer de) noexcept -> Result<void>;

  /*!
   * \brief Handle the timer callback.
   * \spec
   *   requires true;
   * \endspec
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   * \return      True, because the timer has to be scheduled again.
   */
  auto HandleTimer() noexcept -> bool final;

  FRIEND_TEST(UT__IpcClient, OnConnectionCompleteHandlesFailure);     /*!< \brief Friend declaration for testing. */
  FRIEND_TEST(UT__IpcClient__Async, ClientCanHandleFailingSends);     /*!< \brief Friend declaration for testing. */
  FRIEND_TEST(UT__IpcClient, OnMessageReceivedConnectionClosed);      /*!< \brief Friend declaration for testing. */
  FRIEND_TEST(UT__IpcClient, HandleTimerCanEstablishConnection);      /*!< \brief Friend declaration for testing. */
  FRIEND_TEST(UT__IpcClient, TriggerConnectCanBeCalledMultipleTimes); /*!< \brief Friend declaration for testing. */
  FRIEND_TEST(UT__IpcClient,
              TriggerConnectCanBeCalledMultipleTimesNonExistingServer); /*!< \brief Friend declaration for testing. */
  FRIEND_TEST(UT__IpcClient, SendMessageInternalFails);                 /*!< \brief Friend declaration for testing. */
  FRIEND_TEST(UT__IpcClient, ReconnectOnTimer);                         /*!< \brief Friend declaration for testing. */
  FRIEND_TEST(UT__IpcClient, OnReconnectIsCalled);                      /*!< \brief Friend declaration for testing. */
  FRIEND_TEST(UT__IpcClient__Receive, DisconnectMessage);               /*!< \brief Friend declaration for testing. */
};

}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_IPC_INCLUDE_AMSR_LOG_INTERNAL_IPC_CLIENT_H_
