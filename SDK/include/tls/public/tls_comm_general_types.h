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
 *         \brief This file contains general type declarations for types used in the external API for the TLS
 *                Library.
 *         \details Includes several central type declarations related to the external API of the TLS library.
 *                  The following areas are covered:
 *                  - General enumerations.
 *                  - Configuration information.
 *                  - Flow context.
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_PUBLIC_TLS_COMM_GENERAL_TYPES_H_
#define LIB_LIBTLS_INCLUDE_TLS_PUBLIC_TLS_COMM_GENERAL_TYPES_H_

#include <cstddef>
#include <cstdint>
#include "ara/core/vector.h"

#include "amsr/core/span.h"
#include "amsr/core/string.h"
#include "ara/crypto/common/guid.h"

namespace amsr {
namespace tls {

/*!
 * \brief Cipher suite ID.
 * \details
 * Allowed values in the list:
 * 0x0000 = TLS_NULL_WITH_NULL_NULL
 * 0x00b0 = TLS_PSK_WITH_NULL_SHA256
 * 0x00a8 = TLS_PSK_WITH_AES_128_GCM_SHA256
 * \trace SPEC-6181147
 */
using CipherSuiteId = std::uint16_t;

/*!
 * \brief List of cipher suites.
 * \details
 * Allowed values in the list:
 * 0x0000 = TLS_NULL_WITH_NULL_NULL
 * 0x00b0 = TLS_PSK_WITH_NULL_SHA256
 * 0x00a8 = TLS_PSK_WITH_AES_128_GCM_SHA256
 * \trace SPEC-6181147
 */
using ListOfCipherSuites = ara::core::Vector<CipherSuiteId>;

/*! \brief Raw data buffer type that allows dynamic allocation / deallocation. */
using DynamicMemoryBuffer = ara::core::Vector<std::uint8_t>;

/*!\brief Writeable data buffer view type.*/
using WriteableBufferView = amsr::core::Span<std::uint8_t>;

/*!\brief Read-Only data buffer view type.*/
using ReadOnlyBufferView = amsr::core::Span<std::uint8_t const>;

/*! \brief Type alias for Tls extension value */
using TlsExtensionData = std::uint16_t;

/*!
 * \brief Enumeration defining the different roles that one communication provider can take.
 * \details The role of the communication provider is set upon creation.
 * kCommRoleNotDefined: Indicates that the role is currently not defined. Shall not be used together with the API.
 * kCommRoleTlsClient: The protocol TLS and acting as client.
 * kCommRoleTlsServer: The protocol TLS and acting as server.
 * kCommRoleDtlsClient: The protocol DTLS and acting as client.
 * kCommRoleDtlsServer: The protocol DTLS and acting as server.
 */
enum class EnTlsCommRole : std::uint8_t {
  kCommRoleNotDefined,
  kCommRoleTlsClient,
  kCommRoleTlsServer,
  kCommRoleDtlsClient,
  kCommRoleDtlsServer,
};

/*!
 * \brief The connection state that a communication provider can take.
 * \details The connection state of the provider can be read using TlsCommBase::GetConnectionState().
 * The following values are supported:
 * kRoleStateNotDefined: Initialization value. Used internally in the library. Shall not be used externally.
 * kRoleStateNotConnected: The communication provider is not connected.
 * kRoleStatePerfomingHandshake: The communication provider is performing handshake.
 * kRoleStateConnected: The communication provider is connected.
 * kRoleStateConnectedAndParallelHandshake: The provider is connected and performing a parallel handshake.
 * \trace DSGN-SecCom-ConnectionStates
 */
enum class EnTlsCommRoleState : std::uint8_t {
  kRoleStateNotDefined,
  kRoleStateNotConnected,
  kRoleStatePerformingHandshake,
  kRoleStateConnected,
  kRoleStateConnectedAndParallelHandshake
};

/*! \brief Default value for timeout duration in ms used during the Handshake procedure */
constexpr std::uint32_t const kDefaultTimeoutMs{5'000U};

/*! \brief Default value for timeout duration in ms required by the final state of the Handshake procedure */
constexpr std::uint32_t const kDefaultTimeoutConnectedStateMs{240'000U};

/*! \brief Default value for the number of repeated timeouts */
constexpr std::uint32_t const kDefaultNrOfTimeouts{5U};

/*! \brief Default value for the upper limit of the maximum transmission unit */
constexpr std::size_t const kDefaultMTU{16384U};

/*! \brief Default value for psk hint string */
constexpr char const* kDefaultPskHint{"tls_default_psk_hint"};

/*!
 * \brief Structure containing the configuration information for the TLS library.
 * \details This struct shall be used to configure (D)TLS providers.
 */
struct TlsCommConfigurationInformation final {
  /*!
   * \brief Whether the provider is TCP (that is, TLS) or not (that is, DTLS).
   */
  bool is_tcp{true};

  /*!
   * \brief Timeout in millisecond for (D)TLS handshake messages, 5000 ms by default.
   */
  std::uint32_t handshake_message_timeout_in_ms{kDefaultTimeoutMs};

  /*!
   * \brief Duration in millisecond for DTLS connected state to stay active, 240'000 ms by default.
   *        At least twice the default maximum segment lifetime defined for TCP.
   */
  std::uint32_t dtls_connected_state_resend_timeout_in_ms{kDefaultTimeoutConnectedStateMs};

  /*!
   * \brief Number of times DTLS handshake messages will be retransmitted before an error is generated;
   *        5 times by default.
   */
  std::uint32_t dtls_handshake_message_no_of_repeat{kDefaultNrOfTimeouts};

  /*!
   * \brief The maximum transmission unit in bytes, 16'384 bytes by default.
   *        For DTLS, configuring this value can prevent IP fragmentation and optimize the protocol.
   */
  std::size_t max_transmission_unit{kDefaultMTU};  // \trace SPEC-6562013 broken trace due to tooling issue

  /*!
   * \brief Flag controlling if cookie verification shall be included during DTLS handshakes. Disabled by default.
   */
  bool cookie_verification_is_on{false};

  /*!
   * \brief Flag used to enable the remote only crypto mode. By default the performance crypto mode is used.
   * \details The remote crypto mode executes all cryptographic functions on the remote crypto daemon application.
   * The performance mode executes some cryptographic functions (e.g. RNG) within the runtime of the application which
   * uses the TLS library.
   */
  bool enable_crypto_mode_remote_{false};

  /*!
   * \brief UUID of the trustzone crypto provider.
   * \details If the crypto adapter is "performance", this is the UUID of the trustzone crypto provider.
   * If the crypto adapter is "remote", this is the UUID of the trustzone and the remote crypto provider.
   * Default value is the Nil UUID.
   */
  ara::crypto::Uuid trustzone_uuid{/* Nil UUID */};

  /*!
   * \brief The list of cipher suites this provider shall use.
   * A list of supported cipher suites in priority order with the preferred cipher suite having highest priority.
   * Any one of the supported suites can be selected during negotiation and will be supported until the next handshake.
   */
  ListOfCipherSuites cipher_suites{};

  /*!
   * \brief Structure with the configuration for certificate-based TLS communication.
   * \details If one of the provided cipher suites is certificate-based, THEN cert_chain_labels OR root_cert_labels
   *          OR both SHALL NOT to be empty.
   */
  struct CertConf final {
    /*!
     * \brief Type alias for a function pointer to a time getter callback (used for the certificate).
     */
    using GetTimeFptr = time_t (*)();

    /*!
     * \brief Certificate chain label for identifying the local provider.
     * \details The local certificate chain is sent during the handshake (see Sec. 7.4.2 in RFC5246).
     *          The first label is the provider certificate and the last is the root certificate.
     *          The root certificate is not mandatory.
     */
    ara::core::Vector<amsr::core::String> cert_chain_labels{/* empty */};

    /*!
     * \brief UUID of the local private key.
     * \details This field is meaningful IF AND ONLY IF the cert_chain_labels is not empty.
     */
    ara::crypto::Uuid private_key{/* Nil UUID */};

    /*!
     * \brief List of root certificate labels for the remote providers,
     * \details Only remote providers whose root certificate is present in this list will be authenticated.
     */
    ara::core::Vector<amsr::core::String> root_cert_labels{/* empty */};

    /*!
     * \brief Function providing the time reference for certificate validation.
     * \details If it is not set (e.g. nullptr), no time reference is passed to the crypto daemon.
     *          Most users can leave this field unset.
     */
    GetTimeFptr get_cert_time_fptr{nullptr};
  };

  /*!
   * \brief The certificate configuration.
   */
  CertConf cert_conf{};
};

}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_PUBLIC_TLS_COMM_GENERAL_TYPES_H_
