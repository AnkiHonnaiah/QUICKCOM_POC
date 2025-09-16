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
/**
 * \file  diagnostics_interface.h
 *
 * \brief Includes the TLS Diagnostics callback interface.
 *
 * \details This API is used to get the TlsExtension information.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_PUBLIC_DIAGNOSTICS_INTERFACE_H_
#define LIB_LIBTLS_INCLUDE_TLS_PUBLIC_DIAGNOSTICS_INTERFACE_H_

#include "amsr/core/string_view.h"
#include "ara/core/vector.h"
#include "tls/public/tls_comm_error_definitions.h"
#include "tls/public/tls_comm_general_types.h"

namespace amsr {
namespace tls {

/*!
 * \brief The callback interface for the TLS Dignostics.
 * \details This pure virtual class is required to be implemented by the user. (D)TLS service providers
 * use this interface to send information to the user application. This includes the Tls Extension informtion which is
 * received during the handshake procedure.
 */
class DiagnosticsInterface {
 public:
  /*! \brief Default constructor.*/
  DiagnosticsInterface() noexcept = default;

  /*! \brief Not copy constructible. */
  DiagnosticsInterface(DiagnosticsInterface const&) = delete;

  /*! \brief Not move constructible. */
  DiagnosticsInterface(DiagnosticsInterface&&) = delete;

  /*! \brief Not copy assginable. */
  DiagnosticsInterface& operator=(DiagnosticsInterface const&) = delete;

  /*! \brief Not move assignable. */
  DiagnosticsInterface& operator=(DiagnosticsInterface&&) = delete;

  /*! \brief Virtual destructor */
  virtual ~DiagnosticsInterface() noexcept = default;

  /*!
   * \brief Called by the (D)TLS provider during handshake.
   * \param[in] extensions Specifies the Tls extension type value.
   * \context (D)TLS Provider
   * \pre -
   * \reentrant FALSE
   *
   * \trace DSGN-SecCom-OnConnected
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_libseccom_M9.3.3_pure_virtual_method_can_be_declared_const
  virtual void ReportTlsExtensions(ara::core::Vector<TlsExtensionData> extensions) noexcept = 0;
};

}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_PUBLIC_DIAGNOSTICS_INTERFACE_H_
