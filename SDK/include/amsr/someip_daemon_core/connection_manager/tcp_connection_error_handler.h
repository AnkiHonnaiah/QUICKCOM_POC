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
 *        \brief  TCP connection error handler class
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_TCP_CONNECTION_ERROR_HANDLER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_TCP_CONNECTION_ERROR_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {

/*!
 * \brief           Declaration TCP connection error handler class
 */
class TcpConnectionErrorHandler final {
 public:
  /*!
   * \brief           Constructor of TcpConnectionErrorHandler
   * \param[in]       no_of_invalid_tcp_messages_before_disconnection
   *                  Threshold of number of invalid messages.
   * \param[in]       no_of_consecutive_valid_tcp_messages_before_error_reset
   *                  Threshold of number of valid messages.
   * \context         Init
   * \pre             -
   * \reentrant       FALSE
   * \steady          FALSE
   */
  explicit TcpConnectionErrorHandler(
      std::uint16_t no_of_invalid_tcp_messages_before_disconnection = 0u,
      std::uint16_t no_of_consecutive_valid_tcp_messages_before_error_reset = 0u) noexcept;

  TcpConnectionErrorHandler(TcpConnectionErrorHandler const&) = delete;
  TcpConnectionErrorHandler& operator=(TcpConnectionErrorHandler const&) = delete;
  TcpConnectionErrorHandler(TcpConnectionErrorHandler&&) = delete;
  TcpConnectionErrorHandler& operator=(TcpConnectionErrorHandler&&) = delete;

  /*!
   * \brief           Destroys the instance of TcpConnectionErrorHandler
   * \context         ANY
   * \pre             -
   * \threadsafe      FALSE
   * \reentrant       FALSE
   * \steady          FALSE
   */
  ~TcpConnectionErrorHandler() noexcept = default;

  /*!
   * \brief           Increases the invalid message counter and resets the valid message counter
   *
   * \return          true if the invalid message counter reaches the threshold limit, otherwise false.
   * \context         ANY
   * \pre             -
   * \steady          FALSE
   */
  bool IncreaseInvalidMessageCounterAndTriggerDisconnection() noexcept;

  /*!
   * \brief           Increases the valid message counter and resets the invalid message counter on reaching the
   *                  threshold
   *
   * \context         ANY
   * \pre             -
   * \steady          FALSE
   */
  void IncreaseValidMessageCounter() noexcept;

 private:
  /*!
   * \brief           Threshold of number of invalid messages
   */
  std::uint16_t const no_of_invalid_tcp_messages_before_disconnection_;

  /*!
   * \brief           Threshold of number of consecutive valid messages
   */
  std::uint16_t const no_of_consecutive_valid_tcp_messages_before_error_reset_;

  /*!
   * \brief           Invalid message counter
   */
  std::uint16_t no_of_invalid_messages_{0u};

  /*!
   * \brief           Valid message counter
   */
  std::uint16_t no_of_consecutive_valid_messages_{0u};

  /*!
   * \brief           Logging instance
   */
  someip_daemon_core::logging::AraComLogger logger_{
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextId,
      someip_daemon_core::logging::kServiceDiscoveryLoggerContextDescription,
      ara::core::StringView{"TcpConnectionErrorHandler"}};

  /*!
   * \brief           Friend test, to test TcpConnectionErrorHandler()
   */
  FRIEND_TEST(UT__TcpConnectionErrorHandler, CheckThresholdCounter_Validate);

  /*!
   * \brief           Friend test, to test IncreaseInvalidMessageCounter()
   */
  FRIEND_TEST(UT__TcpConnectionErrorHandler, CheckIncreaseInvalidMessageCounter_Success);

  /*!
   * \brief           Friend test, to test IncreaseInvalidMessageCounter()
   */
  FRIEND_TEST(UT__TcpConnectionErrorHandler, CheckIncreaseInvalidMessageCounter_Failed);

  /*!
   * \brief           Friend test, to test IncreaseInvalidMessageCounter()
   */
  FRIEND_TEST(UT__TcpConnectionErrorHandler, CheckIncreaseInvalidMessageCounter_ZeroThreshold);

  /*!
   * \brief           Friend test, to test IncreaseValidMessageCounter()
   */
  FRIEND_TEST(UT__TcpConnectionErrorHandler, CheckIncreaseValidMessageCounter_Success);

  /*!
   * \brief           Friend test, to test IncreaseValidMessageCounter()
   */
  FRIEND_TEST(UT__TcpConnectionErrorHandler, CheckIncreaseValidMessageCounter_Failed);

  /*!
   * \brief           Friend test, to test IncreaseValidMessageCounter()
   */
  FRIEND_TEST(UT__TcpConnectionErrorHandler, CheckIncreaseValidMessageCounter_ZeroThreshold);

  /*!
   * \brief           Friend test, to test TcpConnectionErrorHandler member variable
   */
  FRIEND_TEST(UT__SomeIpd__ConnectionManager__TcpConnection, TcpConnection_ErrorHandler);
};

}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_TCP_CONNECTION_ERROR_HANDLER_H_
