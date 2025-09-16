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
/*!        \file  psk_key_exchange_algorithm.h
 *        \brief  Class describing PSK key exchange algorithm.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_KEY_EXCHANGE_ALGORITHMS_PSK_KEY_EXCHANGE_ALGORITHM_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_KEY_EXCHANGE_ALGORITHMS_PSK_KEY_EXCHANGE_ALGORITHM_H_

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include "ara/core/vector.h"

#include "key_exchange_algorithm_interface.h"
#include "tls/internal/handshakestatemachine/common_handshake_types.h"
#include "tls/internal/messages/handshake_message.h"
#include "tls/public/config/psk_config.h"
#include "tls/public/tls_comm_error_definitions.h"
#include "tls_crypto_adapter/internal/crypto_adapter_interface.h"

namespace amsr {
namespace tls {
namespace internal {
namespace messages {
namespace key_exchange_algorithms {

/*! \brief Realization of the PSK key exchange algorithm. */
class PskKeyExchangeAlgorithm final : public KeyExchangeAlgorithmInterface {
 public:
  /*!
   * \brief Creates an object of PskKeyExchangeAlgorithm.
   * \return A concrete instance of the class.
   * \param[in] psk_config The pre-shared key configuration.
   * \context ANY
   * \pre -
   * \error TlsCommErrorCode::kTlsErrInvalidArgument if PSK hint is empty.
   * \vprivate Component Private
   */
  static std::unique_ptr<KeyExchangeAlgorithmInterface> Create(config::PskConfig const& psk_config) noexcept;

  /*! \brief Destructor. */
  ~PskKeyExchangeAlgorithm() noexcept final = default;

  /*!
   * \brief Explicit constructor.
   * \param[in] psk_config The pre-shared key configuration.
   */
  explicit PskKeyExchangeAlgorithm(config::PskConfig const& psk_config) noexcept
      : KeyExchangeAlgorithmInterface(), psk_config_{psk_config} {}

  /*! \brief No copy construction. */
  PskKeyExchangeAlgorithm(PskKeyExchangeAlgorithm const&) = delete;

  /*! \brief No move construction. */
  PskKeyExchangeAlgorithm(PskKeyExchangeAlgorithm&&) = delete;

  /*! \brief Not copy assignable. */
  PskKeyExchangeAlgorithm& operator=(PskKeyExchangeAlgorithm const&) = delete;

  /*! \brief Not move assignable. */
  PskKeyExchangeAlgorithm& operator=(PskKeyExchangeAlgorithm&&) = delete;

  /*! \copydoc messages::key_exchange_algorithms::KeyExchangeAlgorithmInterface::PrepareServerKeyExchangeMessage */
  amsr::core::Result<messages::HandshakeMessage&> PrepareServerKeyExchangeMessage(
      messages::MessageManager& msg_mgr) const noexcept final;

  /*! \copydoc messages::key_exchange_algorithms::KeyExchangeAlgorithmInterface::PrepareClientKeyExchangeMessage */
  amsr::core::Result<messages::HandshakeMessage&> PrepareClientKeyExchangeMessage(
      messages::MessageManager& msg_mgr) const noexcept final;

  /*! \copydoc messages::key_exchange_algorithms::KeyExchangeAlgorithmInterface::OnClientKeyExchangeMessageReceived */
  amsr::core::Result<void> OnClientKeyExchangeMessageReceived(ara::core::Vector<std::uint8_t> const& raw_message,
                                                              messages::MessageManager& msg_mgr) const noexcept final;

  /*! \copydoc messages::key_exchange_algorithms::KeyExchangeAlgorithmInterface::OnServerKeyExchangeMessageReceived */
  amsr::core::Result<void> OnServerKeyExchangeMessageReceived(ara::core::Vector<std::uint8_t> const& raw_message,
                                                              messages::MessageManager& msg_mgr) noexcept final;

 private:
  /*!
   * \brief The pre-share key configuration.
   */
  config::PskConfig const& psk_config_;
};

}  // namespace key_exchange_algorithms
}  // namespace messages
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_KEY_EXCHANGE_ALGORITHMS_PSK_KEY_EXCHANGE_ALGORITHM_H_
