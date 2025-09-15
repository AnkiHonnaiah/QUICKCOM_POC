/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2020 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!       \file   psk_callback_interface.h
 *        \brief  This file contains the declaration of PskCallbackInterface class
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_PUBLIC_PSK_CALLBACK_INTERFACE_H_
#define LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_PUBLIC_PSK_CALLBACK_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/string_view.h"
#include "ara/crypto/common/guid.h"

namespace amsr {
namespace tls_crypto_adapter {
/*!
 * \brief PskCallbackInterface class
 * \details Callback to get the PSK (Pre-shared Key) Id from the PSK Id Hint and to get the PSK UUID for a key from the
 * PSK ID. This should be implemented by the user of the Crypto Adapter API.
 * \vprivate Vector component internal API
 */
class PskCallbackInterface {
 public:
  /*! \brief Default constructor */
  PskCallbackInterface() = default;
  /*! \brief Destructs the object of PskCallbackInterface type */
  virtual ~PskCallbackInterface() = default;
  PskCallbackInterface(PskCallbackInterface const &) = delete;
  PskCallbackInterface &operator=(PskCallbackInterface const &) = delete;
  PskCallbackInterface(PskCallbackInterface &&) = delete;
  PskCallbackInterface &operator=(PskCallbackInterface &&) = delete;

  /*!
   * \brief Get the PSK UUID corresponding to the given PSK identity and hint.
   * \param[in] psk_identity The PSK identity the UUID shall be fetched for.
   * \param[in] psk_identity_hint The PSK identity hint the UUID shall be fetched for.
   * \param[in] is_server True if this communication provider is a server.
   * \return The PSK UUID corresponding to the PSK identity and hint. The zero UUID if no UUID can be found.
   * \context Any
   * \pre -
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  virtual ara::crypto::Uuid GetPskUuid(amsr::core::StringView psk_identity, amsr::core::StringView psk_identity_hint,
                                       bool is_server) noexcept = 0;

  /*!
   * \brief Get the PSK identity corresponding to the given PSK identity hint.
   * \param[in] psk_identity_hint The PSK identity hint the PSK identity shall be fetched for.
   * \return The PSK identity corresponding to the given PSK identity hint.
   * \context Any
   * \pre -
   * \threadsafe False
   * \reentrant False
   * \synchronous True
   */
  virtual amsr::core::StringView GetPskIdentity(amsr::core::StringView psk_identity_hint) noexcept = 0;
};

}  // namespace tls_crypto_adapter
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_CRYPTO_ADAPTER_PUBLIC_PSK_CALLBACK_INTERFACE_H_
