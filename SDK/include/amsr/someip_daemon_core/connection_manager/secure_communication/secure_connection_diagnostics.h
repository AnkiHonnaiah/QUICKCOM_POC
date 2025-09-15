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
 *        \brief  Declaration of SecureConnectionDiagnostics API.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_SECURE_COMMUNICATION_SECURE_CONNECTION_DIAGNOSTICS_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_SECURE_COMMUNICATION_SECURE_CONNECTION_DIAGNOSTICS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/someip_daemon/extension_points/tls/tls_extensions_handler_interface.h"
#include "ara/core/vector.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "tls/public/diagnostics_interface.h"

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {
namespace secure_communication {

/*!
 * \brief           The class is responsible for reporting the used TLS extensions by TLS library to the remote peer
 */
class SecureConnectionDiagnostics final : public tls::DiagnosticsInterface {
 public:
  /*!
   * \brief           Type alias for a pointer of TlsExtensionsHandlerInterface
   */
  using TlsExtensionsHandlerPtr =
      std::unique_ptr<amsr::someip_daemon::extension_points::tls::TlsExtensionsHandlerInterface>;

  SecureConnectionDiagnostics() = delete;
  SecureConnectionDiagnostics(SecureConnectionDiagnostics const&) = delete;
  SecureConnectionDiagnostics(SecureConnectionDiagnostics&&) = delete;
  SecureConnectionDiagnostics& operator=(SecureConnectionDiagnostics const&) = delete;
  SecureConnectionDiagnostics& operator=(SecureConnectionDiagnostics&&) = delete;

  /*!
   * \brief           Constructs an instance of SecureConnectionDiagnostics
   * \param[in]       address
   *                  The address of the remote connection.
   * \param[in]       port
   *                  The port of the remote connection.
   * \param[in]       tls_extensions_handlers
   *                  Reference to the user implemented extension points handlers.
   * \context         Init
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          FALSE
   */
  SecureConnectionDiagnostics(someip_protocol::internal::IpAddress const& address,
                              someip_protocol::internal::Port const port,
                              ara::core::Vector<TlsExtensionsHandlerPtr> const& tls_extensions_handlers);

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief           Default destructor
   * \steady          FALSE
   */
  ~SecureConnectionDiagnostics() noexcept override = default;

  /*!
   * \brief           A callout used by TLS library to report back the used TLS extensions to the interested application
   * \param[in]       extensions
   *                  TLS extensions received from the remote peer during the TLS handshake.
   * \context         Init
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          FALSE
   */
  void ReportTlsExtensions(ara::core::Vector<tls::TlsExtensionData> extensions) noexcept final;

 private:
  /*!
   * \brief           IP address of the remote connection
   */
  someip_protocol::internal::IpAddress const address_;

  /*!
   * \brief           Port number of the remote connection
   */
  someip_protocol::internal::Port const port_;

  /*!
   * \brief           User implemented extension points handlers
   * \details         The handlers will be used by TLS library to provide information on the used TLS extensions
   *                  received from the remote peer during the TLS handshake.
   */
  ara::core::Vector<TlsExtensionsHandlerPtr> const& tls_extensions_handlers_{};
};

}  // namespace secure_communication
}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_SECURE_COMMUNICATION_SECURE_CONNECTION_DIAGNOSTICS_H_
