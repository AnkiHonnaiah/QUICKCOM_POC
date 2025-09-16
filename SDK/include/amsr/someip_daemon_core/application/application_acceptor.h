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
/**        \file  application_acceptor.h
 *        \brief  Wrapper for IPC acceptor class.
 *
 *      \details  This wrapper is encapsulating all IPC acceptor functionalities. It is useful for localizing the
 *                dependency on different component and can be replaced with mocks for testing of local functionality.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_APPLICATION_ACCEPTOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_APPLICATION_ACCEPTOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/ipc/acceptor.h"
#include "amsr/ipc/connection.h"
#include "amsr/someip_daemon_core/application/application_connection.h"
#include "amsr/someip_daemon_core/application/application_types.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/memory_buffer_allocator.h"
#include "amsr/unique_ptr.h"
#include "ara/core/result.h"
#include "osabstraction/io/io_buffer.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace someip_daemon_core {
namespace application {

/*!
 * \brief Helper class that encapsulates a connection acceptor.
 */
class ApplicationAcceptor final {
 public:
  /*!
   * \brief Type alias for the IPC connection acceptor.
   */
  using ConnectionAcceptor = amsr::ipc::Acceptor;

  /*!
   * \brief Type alias for the IPC connection.
   */
  using Connection = amsr::ipc::Connection;

  /*!
   * \brief Type alias for the acceptance function.
   */
  using AcceptanceFunction = vac::language::UniqueFunction<void(amsr::UniquePtr<ApplicationConnection>)>;

  /*!
   * \brief Constructor.
   *
   * \details Creates and initializes IPC connection acceptor.
   *          In case of error creating the acceptor, abort occurs.
   *
   * \param[in] reactor                    A reactor.
   * \param[in] address                    The IPC address to accept a connection.
   *
   * \pre -
   * \context Init
   * \steady  FALSE
   */
  ApplicationAcceptor(osabstraction::io::reactor1::Reactor1Interface& reactor, UnicastAddress address) noexcept;

  /*!
   * \brief  Default destructor.
   * \steady FALSE
   */
  ~ApplicationAcceptor() = default;

  ApplicationAcceptor(ApplicationAcceptor const&) = delete;
  ApplicationAcceptor(ApplicationAcceptor&&) = delete;
  auto operator=(ApplicationAcceptor const&) -> ApplicationAcceptor& = delete;
  auto operator=(ApplicationAcceptor&&) -> ApplicationAcceptor& = delete;

  /*!
   * \brief Accept connection and start listening.
   *
   * \param[in] acceptance_function  The callback function that will be called when a new connection is accepted.
   *
   * \pre -
   * \reentrant FALSE
   * \steady    FALSE
   * \context   Init
   */
  void Listen(AcceptanceFunction&& acceptance_function) noexcept;

  /*!
   * \brief Acceptor stops accepting new connections.
   *
   * \pre       -
   * \reentrant FALSE
   * \context   Shutdown
   * \steady    FALSE
   * \return    -
   */
  void Close() noexcept;

 private:
  /*!
   * \brief Called when a new potential IPC connection has been established to our server.
   *
   * \param[in] connection Represents a new incoming IPC connection.
   *
   * \pre -
   * \reentrant FALSE
   * \context   App
   * \steady    FALSE
   */
  void OnAccept(Connection&& connection) noexcept;

  /*!
   * \brief Start listening to accept new connections. In case of failure, abort will occur.
   *
   * \pre -
   * \reentrant FALSE
   * \context   Init, App
   * \steady    FALSE
   */
  void ListenAsync() noexcept;

  /*!
   * \brief A connection acceptor.
   */
  ConnectionAcceptor acceptor_;

  /*!
   * \brief An acceptance function.
   */
  AcceptanceFunction acceptance_function_{};

  /*!
   * \brief The logger.
   */
  someip_daemon_core::logging::AraComLogger logger_{someip_daemon_core::logging::kApplicationLoggerContextId,
                                                    someip_daemon_core::logging::kApplicationLoggerContextDescription,
                                                    ara::core::StringView{"ApplicationAcceptor"}};

  /*!
   * \brief Defines if an existing IPC socket shall be overwritten (ownership taken) if it exists or abort.
   *        true: Existing IPC socket shall be overwritten.
   *        false: Existing IPC socket shall not be overwritten and abort instead.
   */
  static bool constexpr kOverwriteIpcSocketOwnership{true};
};
}  // namespace application
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_APPLICATION_ACCEPTOR_H_
