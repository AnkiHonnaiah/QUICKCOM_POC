/*!********************************************************************************************************************
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
/*!        \file  server_key_exchange_dh_message.h
 *        \brief  Class declaration for ServerKeyExchangeDhMessage.
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_SERVER_KEY_EXCHANGE_DH_MESSAGE_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_SERVER_KEY_EXCHANGE_DH_MESSAGE_H_

#include <cstdint>
#include <string>
#include "ara/core/vector.h"

#include "tls/internal/handshakestatemachine/common_handshake_types.h"
#include "tls/internal/messages/extensions/ec_extension.h"
#include "tls/internal/messages/handshake_message.h"
#include "tls_crypto_adapter/internal/crypto_adapter_types.h"

namespace amsr {
namespace tls {
namespace internal {
namespace messages {

/*!
 * \brief Class declaration for Server Key Exchange Diffie-Hellman message that uses ECDHE key exchange algorithm.
 */
class ServerKeyExchangeDhMessage : public HandshakeMessage {
 public:
  /*!
   * \brief The "TLS HashAlgorithm" registry that is called "Intrinsic" (value 8).
   * \details This mean that hashing is intrinsic to the signature algorithm.
   */
  std::uint8_t const kIntrinsic{8U};

  /*!
   * \brief Type alias for Ed25519 signature, the size of the signature value is 64 bytes.
   */
  using Signature = amsr::tls_crypto_adapter::internal::Signature;

  /*!
   * \brief Type alias for byte string representation of an elliptic curve point, the key has fixed size of 32 bytes.
   */
  using ECPoint = amsr::core::Array<std::uint8_t, 32U>;

  /*!
   * \brief Type alias for the name of the curve.
   */
  using NamedCurve = std::uint16_t;

  /*!
   * \brief Identifies the type of the elliptic curve domain parameters.
   */
  enum class ECCurveType : std::uint8_t { named_curve = 3U };

  /*!
   * \brief Specifies the elliptic curve domain parameters associated with the ECDH public key.
   */
  struct ECParameters {
    /*! \brief Type of the curve. */
    ECCurveType const curve_type{ECCurveType::named_curve};

    /*! \brief Name of the curve. */
    NamedCurve const x25519{29U};
  };

  /*!
   * \brief Specifies the ECDH public key and associated domain parameters.
   */
  // VECTOR NC AutosarC++17_10-M8.5.1: MD_libseccom_M8.5.1_member_not_initialized
  struct ServerECDHParams {
    /*! \brief Curve domain parameters. */
    ECParameters const curve_params;

    /*! \brief Public key. */
    ECPoint public_key;
  };

  /*!
   * \brief Create a new Diffie-Hellman ServerKeyExchangeDhMessage object and initializes the base class.
   * \reentrant FALSE
   */
  ServerKeyExchangeDhMessage() noexcept
      : HandshakeMessage(handshakestatemachine::kMessageSeqServerKeyExchange,
                         handshakestatemachine::HandshakeMessageHandle::kServerKeyExchange) {
    included_in_finish_calc_ = true;
  }

  /*!
   * \brief Destructor.
   */
  ~ServerKeyExchangeDhMessage() noexcept override = default;

  /*!
   * \brief No copy construction.
   */
  ServerKeyExchangeDhMessage(ServerKeyExchangeDhMessage const&) = delete;

  /*!
   * \brief No move construction.
   */
  ServerKeyExchangeDhMessage(ServerKeyExchangeDhMessage&&) = delete;

  /*!
   * \brief Not copy assignable.
   */
  ServerKeyExchangeDhMessage& operator=(ServerKeyExchangeDhMessage const&) = delete;

  /*!
   * \brief Not move assignable.
   */
  ServerKeyExchangeDhMessage& operator=(ServerKeyExchangeDhMessage&&) = delete;

  /*!
   * \copydoc amsr::tls::internal::messages::HandshakeMessage::Serialize
   * \trace SPEC-6562784
   * \vprivate This method is component private.
   */
  ara::core::Vector<std::uint8_t> Serialize(common::TlsCommFlowContext& cxt) const noexcept final;

  /*!
   * \copydoc amsr::tls::internal::messages::HandshakeMessage::Deserialize
   * \trace SPEC-6562784
   * \vprivate This method is component private.
   */
  void Deserialize(common::TlsCommFlowContext& cxt, ara::core::Vector<std::uint8_t> const& buffer) noexcept final;

  /*!
   * \brief Clear the content of this message.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void Cleanup() noexcept;

  /*!
   * \brief Set the signed parameters from the content returned by crypto library.
   * \param[in] digitally_signed_params digitally signed value returned from crypto library.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void SetSignedParams(Signature const digitally_signed_params) noexcept;

  /*!
   * \brief Get the signed parameters for this message.
   * \return A byte string vector containing the signed parameters.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  Signature GetSignedParams() const noexcept;

  /*!
   * \brief Serialize and return params_ field to be used by crypto daemon for digital signature.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   * \return Public key and associated domain parameters.
   */
  ara::core::Vector<std::uint8_t> GetParams() const noexcept;

  /*!
   * \brief Gets the public key.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   * \return Public key.
   */
  ECPoint GetPublicKey() const noexcept;

  /*!
   * \brief Sets the public key.
   * \param[in] public_key ECDH public key.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void SetPublicKey(ECPoint const public_key) noexcept;

  /*!
   * \brief Set the generated ECDH private key.
   * \param[in] private_key The generated ECDH private key.
   */
  void SetPrivateKey(ara::crypto::cryp::PrivateKey::Uptrc private_key) noexcept;
  /*!
   * \brief Gets the ECDH private key.
   * \return unique pointer to the PrivateKey.
   */
  ara::crypto::cryp::PrivateKey::Uptrc GetPrivateKey() noexcept;

  /*! \brief Constant with HandshakeType(12) = server_key_exchange */
  static std::uint8_t const kHandshakeType{
      static_cast<std::uint8_t>(handshakestatemachine::HandshakeMessageHandle::kServerKeyExchange)};

 private:
  /*!
   * \brief Specifies the ECDH public key and associated domain parameters.
   */
  ServerECDHParams params_{};

  /*!
   * \brief A hash of ServerECDHParams params, with the signature appropriate to that hash applied.
   * \details The private key corresponding the the certificate public key in the server's Certificate Message is used
   *          for signing.
   */
  Signature signed_params_{};

  /*!
   * \brief The ECDH private key.
   */
  ara::crypto::cryp::PrivateKey::Uptrc private_key_{};
};

}  // namespace messages
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_SERVER_KEY_EXCHANGE_DH_MESSAGE_H_
