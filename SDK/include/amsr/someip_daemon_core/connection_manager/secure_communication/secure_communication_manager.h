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
 *        \brief  The file represents SecureCommunicationManager class
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_SECURE_COMMUNICATION_SECURE_COMMUNICATION_MANAGER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_SECURE_COMMUNICATION_SECURE_COMMUNICATION_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>
#include "amsr/someip_daemon/extension_points/tls/tls_extensions_handler_interface.h"
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/connection_manager/secure_communication/secure_connection_diagnostics.h"
#include "amsr/steady_timer/timer_manager_interface.h"
#include "ara/core/optional.h"
#include "ara/core/string.h"
#include "ara/core/string_view.h"
#include "ara/core/vector.h"
#include "ara/crypto/common/guid.h"
#include "tls/public/config/psk_config.h"
#include "tls/public/tls_comm_callback_interfaces.h"
#include "tls/public/tls_comm_interfaces.h"

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {
namespace secure_communication {

/*!
 * \brief           Buffer size for read I/O calls
 */
static constexpr std::size_t kReadBufferSize{4096U};

/*!
 * \brief           TLS communication provider pointer type definition
 */
using TlsCommunicationProviderPtr = tls::TlsCommProviderUniquePtr;

/*!
 * \brief           The pre-shared key configuration for the SOME/IP daemon
 */
class PskConfigSomeipd final : public tls::config::PskConfig {
 public:
  PskConfigSomeipd(PskConfigSomeipd&&) = delete;
  PskConfigSomeipd operator=(PskConfigSomeipd&&) = delete;
  PskConfigSomeipd operator=(PskConfigSomeipd const&) = delete;

  /*!
   * \brief           Default copy constructor
   * \steady          FALSE
   */
  PskConfigSomeipd(PskConfigSomeipd const&) = default;

  /*!
   * \brief           Default constructor
   * \steady          FALSE
   */
  PskConfigSomeipd() noexcept = default;

  /*!
   * \brief           Default destructor
   * \steady          FALSE
   */
  ~PskConfigSomeipd() noexcept final = default;

  /*!
   * \brief           Add an entry to the map from ID to UUID and an entry in the container of PSK hints, IDs, and UUIDs
   * \param[in]       hint
   *                  A basic string with the pre-shared key server hint.
   * \param[in]       id
   *                  A basic string with the pre-shared key ID.
   * \param[in]       uuid
   *                  An object with the pre-shared key UUID.
   * \context         Init
   * \pre             -
   * \threadsafe      False
   * \reentrant       False
   * \synchronous     True
   * \steady          FALSE
   */
  void AssociateHintIdUuid(PskHint hint, PskId id, ara::crypto::Uuid uuid) noexcept;

  /*!
   * \brief           Get the PSK UUID corresponding to the given PSK identity and hint
   * \param[in]       id
   *                  The PSK identity the UUID shall be fetched for.
   * \param[in]       hint
   *                  The PSK identity hint the UUID shall be fetched for.
   * \param[in]       is_server
   *                  True if this communication provider is a server.
   * \return          The PSK UUID corresponding to the PSK identity and hint. The zero UUID if no UUID can be found.
   * \context         Any
   * \pre             The PSK configuration should be initialized.
   * \threadsafe      True
   * \reentrant       False
   * \synchronous     True
   * \steady          FALSE
   */
  ara::crypto::Uuid GetPskUuid(ara::core::StringView id, ara::core::StringView hint,
                               bool is_server) const noexcept final;

 private:
  /*!
   * \brief           Structure containing one PSK server hint, one PSK ID, and one PSK UUID
   */
  struct PskHintIdUuid {
    PskHint hint{};
    PskId id{};
    ara::crypto::Uuid uuid{};
  };
  /*!
   * \brief           A container of the pre-shared key hints, IDs, and UUIDs, one entry for each secure endpoint
   */
  ara::core::Vector<PskHintIdUuid> vec_hint_id_uuid_{};
};

/*!
 * \brief           Secure communication management component
 */
class SecureCommunicationManager final {
 public:
  /*!
   * \brief           Constructs an instance of SecureCommunicationManager
   * \param[in]       config
   *                  A configuration.
   * \param[in]       timer_manager
   *                  A timer manager.
   * \param[in]       tls_extensions_handlers
   *                  A vector of pointers to the TLS extensions handler.
   * \context         Init
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          FALSE
   */
  SecureCommunicationManager(
      configuration::Configuration const& config, amsr::steady_timer::TimerManagerInterface* timer_manager,
      ara::core::Vector<SecureConnectionDiagnostics::TlsExtensionsHandlerPtr> const& tls_extensions_handlers) noexcept;

  /*!
   * \brief           Destructs an instance of SecureCommunicationManager
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          FALSE
   */
  ~SecureCommunicationManager() noexcept;

  /*!
   * \brief           Deleted copy constructor
   * \steady          FALSE
   */
  SecureCommunicationManager(SecureCommunicationManager const&) = delete;

  /*!
   * \brief           Deleted move constructor
   * \steady          FALSE
   */
  SecureCommunicationManager(SecureCommunicationManager&&) = delete;

  /*!
   * \brief           Deleted copy assignment operator
   * \steady          FALSE
   */
  SecureCommunicationManager& operator=(SecureCommunicationManager const&) = delete;

  /*!
   * \brief           Deleted move assignment operator
   * \steady          FALSE
   */
  SecureCommunicationManager& operator=(SecureCommunicationManager&&) = delete;

  /*!
   * \brief           Creates a new secure communication provider
   *
   * \param[in]       local_address
   *                  The address of the local network endpoint for which the provider shall be created.
   * \param[in]       local_port
   *                  The port of the local network endpoint for which the provider shall be created.
   * \param[in]       remote_address
   *                  The address of the remote connection used by TlsDiagnostic API.
   * \param[in]       remote_port
   *                  The port of the remote connection used by TlsDiagnostic API.
   * \param[in]       callback_handler
   *                  The reference to a TlsCommCallbackInterface instance for the provider.
   * \param[in]       sec_com_config
   *                  The SecCom configuration.
   * \param[in]       is_server
   *                  Wether the provider is server (true) or client (false).
   * \param[in]       is_tcp
   *                  Wether the provider is TLS (true) or DTLS (false).
   * \return          A pointer to the newly created TLS communication provider or nullptr.
   * \context         ANY
   * \pre             SecComConfig shall be correctly initialized.
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          FALSE
   */
  TlsCommunicationProviderPtr CreateCommunicationProvider(
      someip_protocol::internal::IpAddress const& local_address, someip_protocol::internal::Port local_port,
      someip_protocol::internal::IpAddress const& remote_address, someip_protocol::internal::Port remote_port,
      tls::TlsCommCallbackInterface& callback_handler,
      configuration::ConfigurationTypesAndDefs::SecComConfig const* sec_com_config, bool const is_server,
      bool const is_tcp) const noexcept;

 private:
  /*!
   * \brief           Creates a a PSK configuration and a list of cipher suites
   *
   * \param[in]       local_address
   *                  The address of the local network endpoint for which the provider shall be created.
   *                  Address needs to be in standard dot-decimal or hexadecimal notation.
   * \param[in]       local_port
   *                  The port of the local network endpoint for which the provider shall be created.
   * \param[in]       sec_com_config
   *                  The SecCom configuration.
   * \param[in]       is_server
   *                  Wether the provider is server (true) or client (false).
   * \param[in]       is_tcp
   *                  Wether the provider is TLS (true) or DTLS (false).
   * \return          A pair of a unique pointer to the newly created PSK configuration and a vector of cipher suites.
   * \context         ANY
   * \pre             SecComConfig shall be correctly initialized.
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          FALSE
   */
  auto LoadPskConfigAndCipherSuites(someip_protocol::internal::IpAddress const& local_address,
                                    someip_protocol::internal::Port local_port,
                                    configuration::ConfigurationTypesAndDefs::SecComConfig const* sec_com_config,
                                    bool const is_server, bool const is_tcp) const noexcept
      -> std::pair<std::unique_ptr<PskConfigSomeipd>, tls::ListOfCipherSuites>;

  /*!
   * \brief           Internal setup method
   * \steady          FALSE
   */
  void Setup() noexcept;

  /*!
   * \brief           Our logger
   */
  someip_daemon_core::logging::AraComLogger logger_;

  /*!
   * \brief           Secure communication configuration
   */
  configuration::Configuration const& config_;

  /*!
   * \brief           The TLS configuration common for every provider
   */
  tls::TlsCommConfigurationInformation tls_common_config_{};

  /*!
   * \brief           A timer manager;
   */
  amsr::steady_timer::TimerManagerInterface* timer_manager_;

  /*!
   * \brief           TLS extensions handlers provided by the user application
   * \details         The handlers will be used by TLS library to provide information on the used TLS extensions.
   */
  ara::core::Vector<SecureConnectionDiagnostics::TlsExtensionsHandlerPtr> const& tls_extensions_handlers_;

  /*!
   * Friend test for accessing LoadPskConfigAndCipherSuites.
   */
  FRIEND_TEST(Cdd__SomeIpd__ConnectionManagerSecureCom, LoadPskConfigAndCipherSuites);
};

}  // namespace secure_communication
}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_SECURE_COMMUNICATION_SECURE_COMMUNICATION_MANAGER_H_
