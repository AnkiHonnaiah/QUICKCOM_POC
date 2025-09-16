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
/*!        \file  session_handler.h
 *        \brief  Handles a reactor singleton, provides connections and keeps track of loaded providers.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_SESSION_HANDLER_H_
#define LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_SESSION_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <vector>

#include "amsr/generic/singleton_wrapper.h"
#include "amsr/sec/ipc/connector.h"
#include "amsr/sec/ipc/protocol.h"
#include "amsr/thread/thread.h"
#include "crypto/client/provider_manager.h"
#include "crypto/common/util/logger.h"
#include "osabstraction/io/reactor1/reactor1.h"

namespace crypto {
namespace client {

/*!
 * \brief Class handling a reactor singleton, providing connections and keeping track of loaded providers.
 * \vprivate Component Private
 */
class SessionHandler {
 public:
  /*!
   * \brief Creates an instance of the singleton and connects to the daemon.
   * \context During component initialization
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kRuntimeFault When shutdown function could not be registered
   * \error ara::crypto::SecurityErrc::kRuntimeFault When the daemon could not be reached.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto Init() noexcept -> amsr::core::Result<void>;

  /*!
   * \brief Getter for the singleton instance.
   * \details return the singleton if initialized, abort otherwise.
   * \return Access to the singleton instance.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kRuntimeFault Failed to get the instance.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static auto Instance() noexcept -> amsr::generic::SingletonAccess<SessionHandler>;

  /*!
   * \brief Resets the singleton.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static void Reset() noexcept;

  /*!
   * \brief This function initializes the connection provider.
   * \details Initialize IPC relevant members using the given reactor and address
   *          and establish a connection to the given address.
   * \param[in] address The address to set.
   *
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kRuntimeFault if the session handler is already initialized
   * \error ara::crypto::SecurityErrc::kResourceFault if the connection could not be established
   * \reentrant FALSE
   * \synchronous FALSE
   * \vprivate Component Private
   */
  auto Initialize(amsr::ipc::UnicastAddress const address) noexcept -> amsr::core::Result<void>;

  /*!
   * \brief Stops the reactor thread.
   * \context During component deinitialization
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kRuntimeFault Unable to join thread.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void Shutdown() noexcept;

  /*!
   * \brief Provides a connected transporter.
   * \details If there is no connected provider available,
   *          InitializeAndConnect for connection establishment will be called.
   * \return An existing or new transporter.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kRuntimeFault Failed to get transporter.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetTransporter() noexcept -> amsr::core::Result<amsr::sec::ipc::Transporter&>;

  /*!
   * \brief Gets the provider manager.
   * \details -
   * \return The provider manager.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetProviderManager() noexcept -> crypto::client::ProviderManager&;

  /*!
   * \brief Constructor.
   * \details -
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  SessionHandler() noexcept;

  // VECTOR NC AutosarC++17_10-A15.5.1: MD_CRYPTO_AutosarC++17_10-A15.5.1_noexcept
  /*!
   * \brief Destroys the session handler.
   * \vprivate Component Private
   */
  virtual ~SessionHandler() noexcept;

  /*!
   * \brief Deleted copy constructor
   * \vprivate Component Private
   */
  SessionHandler(SessionHandler const& /*other*/) = delete;

  /*!
   * \brief Deleted move constructor
   * \vprivate Component Private
   */
  SessionHandler(SessionHandler&& /*other*/) noexcept = delete;

  /*!
   * \brief Deleted copy assignment operator
   * \return -
   * \vprivate Component Private
   */
  SessionHandler& operator=(SessionHandler const& /*other*/) = delete;

  /*!
   * \brief Deleted move assignment operator
   * \return -
   * \vprivate Component Private
   */
  SessionHandler& operator=(SessionHandler&& /*other*/) noexcept = delete;

  /*!
   * \brief Provides access to the current session's key storage provider.
   * \return The key storage provider.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kRuntimeFault If the Key Storage Provider instance cannot be registered.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetKeyStorageProvider() noexcept -> amsr::core::Result<ara::crypto::keys::KeyStorageProvider::Sptr>;

  /*!
   * \brief Provides access to the current session's X.509 provider.
   * \return The X.509 provider.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Runtime error.
   * \vprivate Component Private
   */
  auto GetX509Provider() noexcept -> amsr::core::Result<ara::crypto::x509::X509Provider::Sptr>;

 private:
  amsr::ipc::UnicastAddress address_; /*!< Address to connect to */
  std::unique_ptr<osabstraction::io::reactor1::Reactor1>
      reactor_ptr_;                                          /*!< Reactor pointer to hold locally owned reactor */
  amsr::thread::Thread reactor_thread_;                      /*!< Thread to handle events on the reactor */
  std::atomic<bool> dispatch_;                               /*!< Whether the reactor thread is active */
  std::unique_ptr<amsr::sec::ipc::Connector> connector_;     /*!< Connector used to establish the connection */
  std::unique_ptr<amsr::sec::ipc::Transporter> transporter_; /*!< Transporter of this SessionHandler */
  std::unique_ptr<crypto::client::ProviderManager> provider_manager_; /*!< Registry for already loaded providers */
  std::shared_ptr<ara::crypto::keys::KeyStorageProvider> ksp_;        /*!< Key storage provider */
  std::shared_ptr<ara::crypto::x509::X509Provider> x509_provider_;    /*!< X.509 provider. */
  crypto::common::util::Logger logger_;                               /*!< Instance of logger */

  static constexpr std::uint16_t kReactorCallbacks{
      3}; /*!< Constant which determines the amount of callbacks the reactor supports */

  /*!
   * \brief Handles incoming events on the reactor.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   */
  void Dispatch() noexcept;

  /*!
   * \brief Tries to connect and assign the transporter
   * \return Successfully initiated Transporter.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kResourceFault If the connection is failed.
   * \reentrant FALSE
   * \synchronous TRUE
   */
  auto Connect() noexcept -> amsr::core::Result<void>;
};

}  // namespace client
}  // namespace crypto

#endif  // LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_SESSION_HANDLER_H_
