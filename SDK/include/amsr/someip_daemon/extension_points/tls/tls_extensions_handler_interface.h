/*********************************************************************************************************************
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
/*!        \file  tls_extensions_handler_interface.h
 *        \brief  An interface of the callback to report received TLS extensions from libseccom to someipdaemon.
 *         \unit  SomeIpDaemon::TLSExtensionsHandler::InterfaceClass
 *
 *********************************************************************************************************************/
#ifndef LIB_EXTENSION_POINTS_INTERFACES_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_TLS_TLS_EXTENSIONS_HANDLER_INTERFACE_H_
#define LIB_EXTENSION_POINTS_INTERFACES_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_TLS_TLS_EXTENSIONS_HANDLER_INTERFACE_H_

#include "ara/core/string.h"
#include "ara/core/vector.h"

namespace amsr {
namespace someip_daemon {
namespace extension_points {
namespace tls {

/*!
 * \brief The TLSExtensionsHandler interface provide a callback to report received TLS extensions.
 * \trace SomeIpDaemon::TLSExtensionsHandler::InterfaceClass
 */
class TlsExtensionsHandlerInterface {
 protected:
  /*! \brief Local type definition of IpAddress. */
  using IpAddress = ara::core::String;
  /*! \brief Local type definition of Port. */
  using Port = std::uint16_t;
  /*! \brief Local type definition of TlsExtensionType. */
  using TlsExtensionType = std::uint16_t;

 public:
  /*!
   * \brief Default constructor.
   * \steady FALSE
   */
  TlsExtensionsHandlerInterface() noexcept = default;

  TlsExtensionsHandlerInterface(TlsExtensionsHandlerInterface const &) = delete;
  TlsExtensionsHandlerInterface(TlsExtensionsHandlerInterface &&) = delete;
  TlsExtensionsHandlerInterface &operator=(TlsExtensionsHandlerInterface const &) & = delete;
  TlsExtensionsHandlerInterface &operator=(TlsExtensionsHandlerInterface &&) & = delete;

  /*!
   * \brief Virtual default destructor.
   * \steady FALSE
   */
  virtual ~TlsExtensionsHandlerInterface() = default;

  /*!
   * \brief Callback to be called to report the received tls extensions by the secure connection.
   * \details The function is pure virtual in order to make the class abstract.
   * \param[in] remote_address The address of the remote peer.
   * \param[in] remote_port The port of the remote peer.
   * \param[out] tls_extensions A vector of the received tls extensions during handshake.
   * \steady FALSE
   */
  virtual void ReportTlsExtensions(IpAddress const remote_address, Port const remote_port,
                                   ara::core::Vector<TlsExtensionType> const &tls_extensions) = 0;
};
}  // namespace tls
}  // namespace extension_points
}  // namespace someip_daemon
}  // namespace amsr
#endif  // LIB_EXTENSION_POINTS_INTERFACES_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_TLS_TLS_EXTENSIONS_HANDLER_INTERFACE_H_
