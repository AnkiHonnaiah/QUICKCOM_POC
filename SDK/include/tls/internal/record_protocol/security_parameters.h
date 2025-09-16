/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2019 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!
 *               \file     security_parameters.h
 *              \brief     This file contains SecurityParameters class and some relevant enumerations.
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_SECURITY_PARAMETERS_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_SECURITY_PARAMETERS_H_

#include <cstdint>
#include "ara/core/vector.h"

#include "tls/internal/common/common_general_types.h"
#include "tls/internal/record_protocol/record_protocol_common_types.h"
#include "tls/public/supported_cipher_suites.h"
#include "tls_crypto_adapter/internal/crypto_adapter_types.h"

namespace amsr {
namespace tls {
namespace internal {
namespace record_protocol {

/*!
 * \brief Enum class for connection end.
 * \vprivate This enum class is component private.
 */
enum class ConnectionEnd : uint8_t { kServer = 0U, kClient };
/*!
 * \brief Enum class for compression method.
 * \vprivate This enum class is component private.
 */
enum class CompressionMethod : uint8_t { kCNull = 0U };

/*!
 * \brief Contains parameters needed by record protocol to start encrypting/decrypting data.
 * \vprivate This struct is component private.
 */
struct SecurityParameters {
  /*! \brief Are we client or server */
  ConnectionEnd entity_{ConnectionEnd::kClient};

  /*! \brief Always null. We don't support compression. */
  CompressionMethod compression_method_{CompressionMethod::kCNull};

  /*!\brief The encryption key used by the client to encrypt data */
  ara::core::Vector<uint8_t> client_write_key_{};

  /*! \brief The encryption key used by the server to encrypt data */
  ara::core::Vector<uint8_t> server_write_key_{};

  /*! \brief Length of the Cipher keys. */
  uint8_t cipher_key_length_{0U};

  /*! \brief Message Authentication Code(MAC) algorithm. Only "Sha256" or "none" supported. */
  tls_crypto_adapter::internal::EnMacAlgorithm mac_algorithm_{tls_crypto_adapter::internal::EnMacAlgorithm::kNone};

  /*! \brief The length of the MAC */
  uint8_t mac_length_{0U};

  /*! \brief The MAC key used by the client when writing data. */
  ara::core::Vector<uint8_t> client_write_mac_key_{};

  /*! \brief The MAC key used by the server when writing data. */
  ara::core::Vector<uint8_t> server_write_mac_key_{};

  /*! \brief Length of the MAC keys. */
  uint8_t mac_key_length_{0U};

  /*! \brief Length of the nonce_explicit. See RFC5288 page 3 */
  uint8_t record_iv_length_{0U};

  /*! \brief Length of the implicit part of the nonce, called salt. See RFC5288 page 3 */
  uint8_t fixed_iv_length_{0U};

  /*! \brief The initialization vector used by the client when writing. */
  ara::core::Vector<uint8_t> client_write_iv_{};

  /*! \brief The initialization vector used by the server when writing. */
  ara::core::Vector<uint8_t> server_write_iv_{};

  /*! \brief The cipher algorithm used. */
  tls_crypto_adapter::internal::EnCipherAlgorithm cipher_algorithm_{
      tls_crypto_adapter::internal::EnCipherAlgorithm::kNone};

  /*! \brief Record size limit extension, max allowed plain text length. */
  std::uint16_t record_size_limit_{common::kMaxFragmentSize};

  /*! \brief The combination of cipher and MAC algorithms used. */
  SupportedCipherSuites cipher_suite_{SupportedCipherSuites::TLS_NULL_WITH_NULL_NULL};

  /*! \brief Encrypt-then-MAC flag. */
  bool encrypt_then_mac_{false};
};

/*!
 * \brief Create an instance of SecurityParameters and set the ConnectionEnd (Client or Server)
 * \param[in] connectionEnd The connection end associated with the created security parameters
 * \return An instance of SecurityParameters
 * \vprivate This method is component private.
 * \reentrant TRUE
 */
inline SecurityParameters CreateSecurityParams(ConnectionEnd const connectionEnd) noexcept {
  SecurityParameters sp{};
  sp.entity_ = connectionEnd;
  return sp;
}

/*!
 * \brief Reset the passed SecurityParameter fields and set the new ConnectionEnd
 * \param[in, out] sp Reference to the SecurityParameter to reset
 * \param[in] connectionEnd The new ConnectionEnd for the reset SecurityParameter
 * \vprivate This method is component private.
 * \reentrant FALSE
 */
inline void ResetSecurityParametersTo(SecurityParameters &sp, ConnectionEnd const connectionEnd) noexcept {
  sp.entity_ = connectionEnd;
  sp.compression_method_ = CompressionMethod::kCNull;
  sp.client_write_key_.clear();
  sp.server_write_key_.clear();
  sp.mac_algorithm_ = tls_crypto_adapter::internal::EnMacAlgorithm::kNone;
  sp.mac_length_ = 0U;
  sp.client_write_mac_key_.clear();
  sp.server_write_mac_key_.clear();
  sp.mac_key_length_ = 0U;
  sp.record_iv_length_ = 0U;
  sp.fixed_iv_length_ = 0U;
  sp.client_write_iv_.clear();
  sp.server_write_iv_.clear();
  sp.cipher_algorithm_ = tls_crypto_adapter::internal::EnCipherAlgorithm::kNone;
  sp.cipher_key_length_ = 0U;
  sp.record_size_limit_ = common::kMaxFragmentSize;
  sp.encrypt_then_mac_ = false;
}

}  // namespace record_protocol
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_SECURITY_PARAMETERS_H_
