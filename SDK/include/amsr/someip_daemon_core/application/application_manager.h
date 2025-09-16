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
/**        \file  application_manager.h
 *        \brief  Manages connections to applications.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_APPLICATION_MANAGER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_APPLICATION_MANAGER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <array>
#include <utility>
#include "ara/core/string.h"

#include "amsr/someip_daemon_core/application/application.h"
#include "amsr/someip_daemon_core/application/application_types.h"
#include "amsr/someip_daemon_core/client/required_service_instance_manager.h"
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/iam/iam_interface.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/packet_handler/local_incoming_packet_validator.h"
#include "amsr/someip_daemon_core/server/local_server_manager.h"
#include "amsr/someip_daemon_core/tracing/communication_tracing_interface.h"
#include "amsr/someip_daemon_core/tracing/sd_tracing_interface.h"
#include "amsr/unique_ptr.h"
#include "ara/core/optional.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"
#include "vac/language/location.h"

namespace amsr {
namespace someip_daemon_core {
namespace application {

/*!
 * \brief Accepts new connections from applications.
 *
 * \tparam ApplicationConnectionType Type used to manage connection between SOME/IP Daemon and the application.
 * \tparam ApplicationAcceptorType   Type used to accept connections from different applications.
 * \tparam MaximumApplications       The maximum number of applications which can be connected at an instant.
 *                                   Note: this is needed for testing purposes only. In production, the default
 *                                   value is used.
 */
template <typename ApplicationConnectionType, typename ApplicationAcceptorType, std::size_t MaximumApplications = 256U>
class ApplicationManager {
 private:
  // VECTOR NC AutosarC++17_10-M3.2.4: MD_SomeIpDaemon_M3.2.4_type_without_definition_false_positive
  /*!
   * \brief The state of application manager.
   */
  enum class AppManState : std::uint8_t {
    kStopped,  /*! Not accepting new connections */
    kListening /*! Accepting new connections */
  };

  /*!
   * \brief The maximum number of applications which can be connected at an instant.
   */
  static constexpr std::size_t kMaximumApplications{MaximumApplications};

 public:
  /*!
   * \brief A type alias for Application.
   */
  using ApplicationType = Application<ApplicationConnectionType>;

  /*!
   * \brief A type alias for Application Connection.
   */
  using ApplicationConnection = ApplicationConnectionType;

  /*!
   * \brief Constructor of ApplicationManager.
   * \param[in] reactor                            A reference to reactor for asynchronous event processing.
   * \param[in] packet_validator                   A validator for incoming packets.
   * \param[in] identity_access_manager            A reference to identity access manager.
   * \param[in] config                             A reference to the daemon configuration.
   * \param[in] required_service_instance_manager  A reference to the RequiredServiceInstanceManager.
   * \param[in] local_server_manager               A reference to the LocalServerManager.
   * \param[in] communication_tracing              A reference to the communication tracing implementation.
   * \param[in] sd_tracing                         A reference to the SD tracing implementation.
   *
   * \pre -
   * \context     Init
   * \reentrant   FALSE
   * \steady      FALSE
   * \internal
   * - Register a SW event for cleanup of disconnected applications
   * - In case the SW event registration in the reactor fails
   *   - Log fatal and abort
   */
  ApplicationManager(osabstraction::io::reactor1::Reactor1Interface& reactor,
                     packet_handler::LocalIncomingPacketValidator const& packet_validator,
                     iam::IamInterface& identity_access_manager, configuration::Configuration const& config,
                     client::RequiredServiceInstanceManager& required_service_instance_manager,
                     server::LocalServerManager& local_server_manager,
                     tracing::CommunicationTracingInterface& communication_tracing,
                     tracing::SdTracingInterface& sd_tracing) noexcept
      : reactor_{reactor},
        packet_validator_{packet_validator},
        identity_access_manager_{identity_access_manager},
        config_{config},
        required_service_instance_manager_{required_service_instance_manager},
        local_server_manager_{local_server_manager},
        communication_tracing_{communication_tracing},
        sd_tracing_{sd_tracing} {
    // Register a SW event for connection establishment
    ::ara::core::Result<osabstraction::io::reactor1::CallbackHandle> register_software_event_result{
        reactor_.RegisterSoftwareEvent(
            [this](osabstraction::io::reactor1::CallbackHandle, ::osabstraction::io::reactor1::EventTypes) {
              // Destroy the disconnected applications
              DestroyDisconnectedApplications(application_pool_default_, number_of_active_connections_default_);
            })};
    if (register_software_event_result) {
      sw_event_handle_cleanup_ = register_software_event_result.Value();
    } else {
      logger_.LogFatalAndAbort(
          [&register_software_event_result](ara::core::String& abort_msg) noexcept {
            abort_msg.append("Failed to register the reactor software event. Detailed error: ");
            abort_msg.append(register_software_event_result.Error().Message());
          },
          AMSR_FILE, static_cast<char const*>(__func__), __LINE__);
    }
  }

  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SomeIpDaemon_AutosarC++17_10_A15.5.3_gtest_exception_escapes
  // VECTOR NC AutosarC++17_10-A15.5.2: MD_SomeIpDaemon_AutosarC++17_10_A15.5.2_gtest_exception_escapes
  // VECTOR NC AutosarC++17_10-A15.5.1: MD_SomeIpDaemon_AutosarC++17_10_A15.5.1_function_exits_with_exception
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SomeIpDaemon_AutosarC++17_10_A15.4.2_noexcept_specifier
  /*!
   * \brief Destructor of ApplicationManager.
   * \pre -
   * \context     Shutdown
   * \reentrant   FALSE
   * \steady      FALSE
   *
   * \internal
   * - Accept no more applications.
   * - Destruct all active applications connections.
   * - Unregister the SW event.
   * \endinternal
   */
  virtual ~ApplicationManager() noexcept {
    Close();
    DestroyApplications(application_pool_default_, number_of_active_connections_default_);

    // Unregister the SW event
    ::ara::core::Result<void> const unregister_result{reactor_.UnregisterSoftwareEvent(sw_event_handle_cleanup_)};
    if (!unregister_result) {
      // Log fatal and abort
      logger_.LogFatalAndAbort(
          [&unregister_result](ara::core::String& abort_msg) noexcept {
            abort_msg.append("Failed to unregister the reactor software event. Detailed error: ");
            abort_msg.append(unregister_result.Error().Message());
          },
          AMSR_FILE, static_cast<char const*>(__func__), __LINE__);
    }
  }

  ApplicationManager(ApplicationManager const&) = delete;
  ApplicationManager(ApplicationManager const&&) = delete;
  auto operator=(ApplicationManager const&) -> ApplicationManager& = delete;
  auto operator=(ApplicationManager const&&) -> ApplicationManager& = delete;

  /*!
   * \brief Returns the current number of active connections.
   * \details USED ONLY FOR TESTING!
   * \pre -
   * \context ANY
   * \steady  FALSE
   * \return the current number of active connections.
   */
  std::size_t GetNumberOfActiveConnections() const noexcept { return number_of_active_connections_default_; }

  /*!
   * \brief Start listening for incoming IPC connections on the given address.
   * \param[in] address The IPC address of the SOME/IP daemon.
   * \pre Listen has never been called before.
   * \context   Init
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - If the manager is in a stopped state.
   *   - If the emplace_acceptor is set to true.
   *     - Create a new connection acceptor with the given address.
   *   - Start listening to the connection acceptor.
   *   - Change to a listening state.
   * \endinternal
   */
  void Listen(UnicastAddress const& address) noexcept {
    logger_.LogVerbose(
        [&address](ara::log::LogStream& s) noexcept {
          amsr::ipc::Domain const domain{address.GetDomain()};
          amsr::ipc::Port const port{address.GetPort()};
          s << "Address (Domain: 0x" << ara::log::HexFormat(domain) << ", Port: 0x" << ara::log::HexFormat(port) << ")";
        },
        static_cast<char const*>(__func__), __LINE__);

    if (state_ == AppManState::kStopped) {
      connection_acceptor_.emplace(reactor_, address);
      connection_acceptor_->Listen([this](amsr::UniquePtr<ApplicationConnection> application_connection) noexcept {
        OnAccept(std::move(application_connection));
      });

      state_ = AppManState::kListening;

      logger_.LogInfo(
          [&address](ara::log::LogStream& s) noexcept {
            amsr::ipc::Domain const domain{address.GetDomain()};
            amsr::ipc::Port const port{address.GetPort()};
            s << "Start accepting application connections from IPC Address (Domain: 0x" << ara::log::HexFormat(domain)
              << ", Port: 0x" << ara::log::HexFormat(port) << ")";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief Stops listening for incoming IPC connections.
   * \pre -
   * \context   Shutdown
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - If the manager is in a listening state.
   *   - Close the connection.
   *   - Change to a stopped state.
   * \endinternal
   */
  void Close() noexcept {
    logger_.LogVerbose(
        [this](ara::log::LogStream& s) noexcept {
          s << "AppManState " << static_cast<typename std::underlying_type<AppManState>::type>(state_);
        },
        static_cast<char const*>(__func__), __LINE__);
    if (state_ == AppManState::kListening) {
      connection_acceptor_->Close();
      state_ = AppManState::kStopped;
      logger_.LogInfo([](ara::log::LogStream& s) noexcept { s << "Stop accepting application connections"; },
                      static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief Releases the given instance of ApplicationType.
   * \param[in] application The pointer to the application which shall be released.
   * \return Always returns true.
   * \pre -
   * \context   App
   * \steady    FALSE
   *
   * \internal
   * - Trigger the SW event to initiate cleanup in the next reactor call
   * \endinternal
   */
  void ReleaseApplication(ApplicationType* application) noexcept {
    logger_.LogDebug(
        [&application](ara::log::LogStream& s) noexcept {
          // VECTOR NC AutosarC++17_10-A5.2.4, AutosarC++17_10-M5.2.9: MD_SomeIpDaemon_A5.2.4_reinterpret_cast_logging
          s << "Application 0x" << ara::log::HexFormat(reinterpret_cast<std::uint64_t>(application));
        },
        static_cast<char const*>(__func__), __LINE__);
    // Trigger the SW event to do the cleanup because the application cannot be destroyed instantly.
    // Cleanup will be done in the next reactor call.
    ::ara::core::Result<void> const trigger_reactor_result{reactor_.TriggerSoftwareEvent(sw_event_handle_cleanup_)};
    if (!trigger_reactor_result) {
      // Log fatal and abort
      logger_.LogFatalAndAbort(
          [&trigger_reactor_result](ara::core::String& abort_msg) noexcept {
            abort_msg.append("Failed to trigger the reactor software event. Detailed error: ");
            abort_msg.append(trigger_reactor_result.Error().Message());
          },
          AMSR_FILE, static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief Called when a new potential IPC connection has been established to our server.
   * \param[in,out] handle Represents a new incoming IPC connection.
   * \pre -
   * \context   App
   * \steady    FALSE
   *
   * \internal
   * - Create a new instance of "ApplicationType"
   * \endinternal
   */
  void OnAccept(amsr::UniquePtr<ApplicationConnection> handle) noexcept {
    bool const success{CreateApplication(std::move(handle))};

    if (!success) {
      logger_.LogError([](ara::log::LogStream& s) noexcept { s << "New application connection rejected"; },
                       static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief The application pool entry type.
   */
  using ApplicationPoolEntry = ara::core::Optional<ApplicationType>;

  /*!
   * \brief The application pool type.
   */
  using ApplicationPool = std::array<ApplicationPoolEntry, kMaximumApplications>;

  /*!
   * \brief Obtain a reference to the application pool.
   *
   * \pre -
   * \context ANY
   * \steady  FALSE
   * \return The application pool for default communication.
   */
  ApplicationPool& GetApplicationPool() noexcept { return application_pool_default_; }

  /*!
   * \brief Destroys all not yet destroyed applications for given application pool type.
   * \param[in,out] app_pool   The application pool to search and destroy applications.
   * \param[in,out] pool_size  The amount of active applications inside of the given application pool.
   * \pre -
   * \context   Shutdown
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Go through the given application pool.
   *   - Check for a valid application entry.
   *     - Delete application.
   * \endinternal
   */
  void DestroyApplications(ApplicationPool& app_pool, std::size_t& pool_size) const noexcept {
    logger_.LogDebug(
        [pool_size](ara::log::LogStream& s) noexcept {
          s << "Destroying all applications. Number of active connections: ";
          s << pool_size << " (max " << kMaximumApplications << ")";
        },
        static_cast<char const*>(__func__), __LINE__);
    for (ApplicationPoolEntry& application_pool_entry : app_pool) {
      if (application_pool_entry.has_value()) {
        application_pool_entry.reset();
      }
    }
    pool_size = 0;
  }

 protected:
  /*!
   * \brief A type alias for the IPC Acceptor.
   */
  using ConnectionAcceptor = ApplicationAcceptorType;

  // VECTOR Disable VectorC++-V11.0.2: MD_SomeIpDaemon_V11-0-2_protected_attributes
  /*!
   * \brief An IPC connection acceptor.
   * \note  This member variable must be protected due to testing purposes, as it's needed for testing the
   *        Close() API.
   */
  ara::core::Optional<ConnectionAcceptor> connection_acceptor_{};
  // VECTOR Enable VectorC++-V11-0-2

 private:
  /*!
   * \brief Creates a new instance of ApplicationType and passes it the newly accepted IPC connection.
   * \param[in] connection This represents a new incoming IPC connection.
   * \return true if the operation has been successful and false otherwise.
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Get application identifier.
   * - If peer identity is valid or no identity and access management is enabled,
   *   - Find a free application pool entry.
   * or
   * - Print error on invalid peer.
   * \endinternal
   */
  bool CreateApplication(amsr::UniquePtr<ApplicationConnection> connection) noexcept {
    bool result{false};
    amsr::ipc::Credentials application_identifier{};
    bool const peer_identity_valid{connection->GetPeerIdentity().HasValue()};
    if (peer_identity_valid) {
      application_identifier = connection->GetPeerIdentity().Value();
    }

    // VECTOR NC AutosarC++17_10-M5.14.1: MD_SomeIpDaemon_AutosarC++17_10-M5.14.1_op_has_no_side_effect
    if ((peer_identity_valid) || (!identity_access_manager_.IsEnabled())) {
      result = FindAndCreateNewApplication(std::move(connection), application_identifier);
    } else {
      logger_.LogError(
          [&connection](ara::log::LogStream& s) noexcept {
            ara::core::ErrorCode const error_code{connection->GetPeerIdentity().Error()};
            ara::core::StringView const message{error_code.Message()};
            s << "Error in identification of peer identity: " << message << ". Application can't be created.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    return result;
  }

  /*!
   * \brief Destroys all not yet destroyed applications.
   * \param[in,out] app_pool   The application pool to search and destroy already disconnected applications.
   * \param[in,out] pool_size  The amount of active applications inside of the given application pool.
   * \pre -
   * \context   Timer
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Go through given application pool.
   *   - Search for disconnected application.
   *     - On hit, delete application.
   * \endinternal
   */
  void DestroyDisconnectedApplications(ApplicationPool& app_pool, std::size_t& pool_size) const noexcept {
    std::size_t const original_pool_size{pool_size};
    for (ApplicationPoolEntry& application_pool_entry : app_pool) {
      if (application_pool_entry.has_value() &&
          // VECTOR NC AutosarC++17_10-M5.14.1: MD_SomeIpDaemon_AutosarC++17_10-M5.14.1_op_has_no_side_effect
          (application_pool_entry->GetConnectionState() == ConnectionState::kDisconnected)) {
        logger_.LogVerbose(
            [&application_pool_entry](ara::log::LogStream& s) noexcept {
              // VECTOR NC AutosarC++17_10-A5.2.4: MD_SomeIpDaemon_A5.2.4_reinterpret_cast_logging
              // VECTOR NC AutosarC++17_10-M5.2.9: MD_SomeIpDaemon_A5.2.4_reinterpret_cast_logging
              ara::log::LogHex64 const value_hex{
                  ara::log::HexFormat(reinterpret_cast<std::uint64_t>(&application_pool_entry.value()))};

              s << "Disconnecting application 0x" << value_hex;
            },
            static_cast<char const*>(__func__), __LINE__);

        application_pool_entry.reset();
        --pool_size;
      }
    }

    logger_.LogDebug(
        [&pool_size, &original_pool_size](ara::log::LogStream& s) noexcept {
          s << "Destroyed " << original_pool_size
            << " disconnected applications. Number of remaining active connections: ";
          s << pool_size << " (max " << kMaximumApplications << ")";
        },
        static_cast<char const*>(__func__), __LINE__);
  }

  /*!
   * \brief Goes through the given application pool and searches for a free entry to create a new application on it.
   * \param[in]  connection   The connection pointer to use for the new application.
   * \param[in]      app_identifier   The application identifier.
   * \return True on success, False if the maximum of applications inside of the given pool is already reached.
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Go through given application pool.
   *   - On free entry found.
   *     - Construct a new application instance.
   * \endinternal
   */
  bool FindAndCreateNewApplication(amsr::UniquePtr<ApplicationConnection> connection,
                                   amsr::ipc::Credentials const& app_identifier) noexcept {
    bool result{false};

    for (ApplicationPoolEntry& application_pool_entry : application_pool_default_) {
      if (!application_pool_entry.has_value()) {
        application_pool_entry.emplace(packet_validator_, std::move(connection), identity_access_manager_,
                                       app_identifier, config_, required_service_instance_manager_,
                                       local_server_manager_, communication_tracing_, sd_tracing_);
        // VECTOR NC AutosarC++17_10-A5.0.1: MD_SomeIpDaemon_A5.0.1_lambda_parameter_false_positive
        application_pool_entry->StartReceive(
            [this](ApplicationType* application) noexcept { ReleaseApplication(application); });

        logger_.LogInfo(
            [&application_pool_entry, &app_identifier](ara::log::LogStream& s) noexcept {
              // VECTOR NC AutosarC++17_10-A5.2.4: MD_SomeIpDaemon_A5.2.4_reinterpret_cast_logging
              // VECTOR NC AutosarC++17_10-M5.2.9: MD_SomeIpDaemon_A5.2.4_reinterpret_cast_logging
              ara::log::LogHex64 const value_hex{
                  ara::log::HexFormat(reinterpret_cast<std::uint64_t>(&application_pool_entry.value()))};
              ara::log::LogHex64 const identifier_hex{ara::log::HexFormat(app_identifier)};
              s << "Created new application with id 0x" << value_hex << " and user identifier 0x" << identifier_hex;
            },
            static_cast<char const*>(__func__), __LINE__);

        result = true;
        ++number_of_active_connections_default_;

        break;
      }
    }

    logger_.LogDebug(
        [this](ara::log::LogStream& s) noexcept {
          s << "Connections (active: ";
          s << number_of_active_connections_default_;
          s << ", max allowed: ";
          s << kMaximumApplications;
          s << ").";
        },
        static_cast<char const*>(__func__), __LINE__);

    return result;
  }

  /*!
   * \brief A reference to reactor for asynchronous event processing.
   */
  osabstraction::io::reactor1::Reactor1Interface& reactor_;

  /*!
   * \brief A reference to the packet validator to validate SOME/IP messages.
   */
  packet_handler::LocalIncomingPacketValidator const& packet_validator_;

  /*!
   * \brief Our logger.
   */
  someip_daemon_core::logging::AraComLogger logger_{someip_daemon_core::logging::kApplicationLoggerContextId,
                                                    someip_daemon_core::logging::kApplicationLoggerContextDescription,
                                                    ara::core::StringView{"ApplicationManager"}};

  // VECTOR NC AutosarC++17_10-M8.5.2: MD_SomeIpDaemon_AutosarC++17_10-M8.5.2_array_implicit_initialization
  /*!
   * \brief The application pool for default communication.
   */
  ApplicationPool application_pool_default_{};

  /*!
   * \brief The state of application manager.
   */
  AppManState state_{AppManState::kStopped};

  /*!
   * \brief The number of currently active default connections.
   */
  std::size_t number_of_active_connections_default_{0U};

  /*!
   * \brief Identity and access management.
   */
  iam::IamInterface& identity_access_manager_;

  /*!
   * \brief SomeIpDaemon configuration reference.
   */
  configuration::Configuration const& config_;

  /*!
   * \brief Reference to the Required Service Instance Manager that keeps track of remote offers.
   */
  client::RequiredServiceInstanceManager& required_service_instance_manager_;

  /*!
   * \brief Reference to local server management.
   */
  server::LocalServerManager& local_server_manager_;

  /*!
   * \brief Reference to communication tracing implementation
   */
  tracing::CommunicationTracingInterface& communication_tracing_;

  /*!
   * \brief Reference to service discovery tracing implementation
   */
  tracing::SdTracingInterface& sd_tracing_;

  /*!
   * \brief Handle for Reactor software event for disconnected applications cleanup.
   */
  osabstraction::io::reactor1::CallbackHandle sw_event_handle_cleanup_{};
};

/*!
 * \brief Provide definition for static constexpr member.
 * \tparam ApplicationConnectionType Type used to manage connection between SOME/IP daemon and the application.
 * \tparam ApplicationAcceptorType   Type used to accept connections from different applications.
 * \tparam MaximumApplications       The maximum number of applications which can be connected at an instant.
 */
template <typename ApplicationConnectionType, typename ApplicationAcceptorType, std::size_t MaximumApplications>
constexpr std::size_t
    ApplicationManager<ApplicationConnectionType, ApplicationAcceptorType, MaximumApplications>::kMaximumApplications;

}  // namespace application
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_APPLICATION_APPLICATION_MANAGER_H_
