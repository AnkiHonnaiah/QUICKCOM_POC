/*!********************************************************************************************************************
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
 *         \brief  TLS provider.
 *         \unit   SecCom::LibTLS::TlsProvider
 *
 *********************************************************************************************************************/
#ifndef LIB_TLS_PROVIDER_COMMON_INCLUDE_AMSR_TLS_TLS_PROVIDER_H_
#define LIB_TLS_PROVIDER_COMMON_INCLUDE_AMSR_TLS_TLS_PROVIDER_H_
/*!********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/tls/data_types.h"
#include "amsr/tls/provider_interface.h"
#include "ara/core/result.h"
#include "ara/core/string.h"
#include "ara/core/vector.h"

namespace amsr {
namespace tls {

/*!
 * \vprivate Product private.
 * \brief TLS provider.
 */
class TlsProvider final {
 public:
  /*!
   * \brief Create an instance of a TLS client.
   * \param[in] tls_protocol_version TLS protocol version to use.
   * \param[in] cipher_suites The cipher suites to use. It shall not contain duplicate entries.
   * \param[in] certificate_chain_specs Specifications of the chain.
   * \param[in] private_key_specs Specifications of the private key.
   * \error GenErrc::kSystemConfigurationInvalid If any of the passed-in arguments is invalid.
   * \return \p ara::core::Result with a pointer to an instance of a class derived from \p ProviderInterface
   *         or an error.
   * \context App
   * \pre -
   * \reentrant FALSE
   * \threadsafe FALSE
   */
  static ara::core::Result<std::unique_ptr<ProviderInterface>> CreateClient(
      TLSProtocolVersion const tls_protocol_version, ara::core::Vector<CipherSuite> const& cipher_suites,
      ara::core::String const& certificate_chain_specs, ara::core::String const& private_key_specs) noexcept;

  /*!
   * \brief Constructor.
   * \context App
   * \pre -
   * \reentrant FALSE
   * \threadsafe FALSE
   */
  TlsProvider() noexcept = default;

  /*!
   * \brief Destructor.
   * \context App
   * \pre -
   * \reentrant FALSE
   * \threadsafe FALSE
   */
  ~TlsProvider() noexcept = default;

  TlsProvider(TlsProvider&&) noexcept = delete;
  TlsProvider(TlsProvider const&) = delete;
  TlsProvider& operator=(TlsProvider const&) = delete;
  TlsProvider& operator=(TlsProvider&&) = delete;
};

}  // namespace tls
}  // namespace amsr

#endif  // LIB_TLS_PROVIDER_COMMON_INCLUDE_AMSR_TLS_TLS_PROVIDER_H_
