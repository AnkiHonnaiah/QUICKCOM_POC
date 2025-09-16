/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!       \file   dh_key_exchange_algorithm.h
 *        \brief  Class describing Diffie-Hellman Key Exchange Algorithm.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_KEY_EXCHANGE_ALGORITHMS_DH_KEY_EXCHANGE_ALGORITHM_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_KEY_EXCHANGE_ALGORITHMS_DH_KEY_EXCHANGE_ALGORITHM_H_

#include <cstdint>
#include <string>
#include "ara/core/vector.h"
#include "memory"

#include "key_exchange_algorithm_interface.h"
#include "tls/internal/handshakestatemachine/common_handshake_types.h"
#include "tls/internal/messages/handshake_message.h"
#include "tls_crypto_adapter/internal/crypto_adapter_interface.h"
#include "tls_crypto_adapter/internal/signature_generator_impl.h"

namespace amsr {
namespace tls {
namespace internal {
namespace messages {
namespace key_exchange_algorithms {

/*! \brief Concretion for Diffie-Hellman key exchange algorithm. */
class DhKeyExchangeAlgorithm final : public KeyExchangeAlgorithmInterface {
  /*!
   * \brief CryptoAdapterInterface type.
   */
  using CryptoAdapterInterface = tls_crypto_adapter::internal::CryptoAdapterInterface;

 public:
  /*!
   * \brief Creates an object of DhKeyExchangeAlgorithm.
   * \return A concrete instance of the class.
   * \param[in] uuid UUID corresponding to the key included in the server certificate message.
   * \param[in] crypto_adapter Reference to the crypto adapter.
   * \context ANY
   * \pre -
   * \vprivate Component Private
   */
  static std::unique_ptr<KeyExchangeAlgorithmInterface> Create(
      amsr::core::StringView const uuid,
      tls_crypto_adapter::internal::CryptoAdapterInterface const& crypto_adapter) noexcept;

  /*!
   * \brief Constructor.
   * \param[in] uuid UUID corresponding to the key included in the server certificate message.
   * \param[in] crypto_adapter Reference to the crypto adapter.
   */
  DhKeyExchangeAlgorithm(amsr::core::StringView const& uuid,
                         tls_crypto_adapter::internal::CryptoAdapterInterface const& crypto_adapter) noexcept
      : KeyExchangeAlgorithmInterface(), ed25519_uuid_{uuid}, crypto_adapter_{crypto_adapter} {}

  /*! \brief Destructor. */
  ~DhKeyExchangeAlgorithm() noexcept final = default;

  /*! \brief No copy construction. */
  DhKeyExchangeAlgorithm(DhKeyExchangeAlgorithm const&) = delete;

  /*! \brief No move construction. */
  DhKeyExchangeAlgorithm(DhKeyExchangeAlgorithm&&) = delete;

  /*! \brief Not copy assignable. */
  DhKeyExchangeAlgorithm& operator=(DhKeyExchangeAlgorithm const&) = delete;

  /*! \brief Not move assignable. */
  DhKeyExchangeAlgorithm& operator=(DhKeyExchangeAlgorithm&&) = delete;

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
   * \brief UUID corresponding to the key included in the server certificate message.
   */
  amsr::core::String const ed25519_uuid_{};

  /*!
   * \brief Reference to Crypto Adapter.
   */
  CryptoAdapterInterface const& crypto_adapter_;
  /*!
   * \brief Prepares the authentication data that shall be digitally signed.
   * \param msg_mgr Reference to the message manager.
   * \context ANY
   * \pre -
   * \vprivate Component Private
   * \return Data shall be signed ClientHello.random + ServerHello.random + ServerKeyExchangeDhMessage.params.
   */
  static ara::core::Vector<std::uint8_t> PrepareAuthenticationDataForDigitalSignature(MessageManager& msg_mgr) noexcept;
};

}  // namespace key_exchange_algorithms
}  // namespace messages
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_KEY_EXCHANGE_ALGORITHMS_DH_KEY_EXCHANGE_ALGORITHM_H_
