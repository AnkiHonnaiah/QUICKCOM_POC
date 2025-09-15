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
/*!        \file  tls_extensions.h
 *        \brief  This file contains the definition for TlsExtensionType and a helper function for fast look-up's.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_EXTENSIONS_TLS_EXTENSIONS_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_EXTENSIONS_TLS_EXTENSIONS_H_

#include <cstdint>
#include "amsr/core/map.h"

namespace amsr {
namespace tls {
namespace internal {
namespace messages {
namespace extensions {

/*!
 * \brief This enum class represents the TLS Extension types values define by IANA
 * \details https://www.iana.org/assignments/tls-extensiontype-values/tls-extensiontype-values.xhtml
 */
enum class TlsExtensionType : std::uint16_t {
  server_name = 0,                              // [RFC6066]
  max_fragment_length = 1,                      // [RFC6066], [RFC8449]
  client_certificate_url = 2,                   // [RFC6066]
  trusted_ca_keys = 3,                          // [RFC6066]
  truncated_hmac = 4,                           // [RFC6066], [IESG Action 2018-08-16]
  status_request = 5,                           // [RFC6066]
  user_mapping = 6,                             // [RFC4681]
  client_authz = 7,                             // [RFC5878]
  server_authz = 8,                             // [RFC5878]
  cert_type = 9,                                // [RFC6091]
  supported_groups = 10,                        // [RFC8422], [RFC7919]
  ec_point_formats = 11,                        // [RFC8422]
  srp = 12,                                     // [RFC5054]
  signature_algorithms = 13,                    // [RFC8446]
  use_srtp = 14,                                // [RFC5764]
  heartbeat = 15,                               // [RFC6520]
  application_layer_protocol_negotiation = 16,  // [RFC7301]
  status_request_v2 = 17,                       // [RFC6961]
  signed_certificate_timestamp = 18,            // [RFC6962]
  client_certificate_type = 19,                 // [RFC7250]
  server_certificate_type = 20,                 // [RFC7250]
  padding = 21,                                 // [RFC7685]
  encrypt_then_mac = 22,                        // [RFC7366]
  extended_master_secret = 23,                  // [RFC7627]
  token_binding = 24,                           // [RFC8472]
  cached_info = 25,                             // [RFC7924]
  tls_lts = 26,                                 // [draft-gutmann-tls-lts]
  compress_certificate = 27,                    // [RFC8879]
  record_size_limit = 28,                       // [RFC8449]
  pwd_protect = 29,                             // [RFC8492]
  pwd_clear = 30,                               // [RFC8492]
  password_salt = 31,                           // [RFC8492]
  ticket_pinning = 32,                          // [RFC8672]
  tls_cert_with_extern_psk = 33,                // [RFC8773]
  delegated_credentials = 34,                   // [draft-ietf-tls-subcerts]
  session_ticket = 35,                          // [RFC5077], [RFC8447]
  TLMSP = 36,                                   // [ETSI TS 103 523-2]
  TLMSP_proxying = 37,                          // [ETSI TS 103 523-2]
  TLMSP_delegate = 38,                          // [ETSI TS 103 523-2]
  supported_ekt_ciphers = 39,                   // [RFC8870]
  pre_shared_key = 41,                          // [RFC8446]
  early_data = 42,                              // [RFC8446]
  supported_versions = 43,                      // [RFC8446]
  cookie = 44,                                  // [RFC8446]
  psk_key_exchange_modes = 45,                  // [RFC8446]
  certificate_authorities = 47,                 // [RFC8446]
  oid_filters = 48,                             // [RFC8446]
  post_handshake_auth = 49,                     // [RFC8446]
  signature_algorithms_cert = 50,               // [RFC8446]
  key_share = 51,                               // [RFC8446]
  transparency_info = 52,                       // [RFC9162]
  connection_id_deprecated = 53,                // [RFC9146]
  connection_id = 54,                           // [RFC9146]
  external_id_hash = 55,                        // [RFC8844]
  external_session_id = 56,                     // [RFC8844]
  quic_transport_parameters = 57,               // [RFC9001]
  ticket_request = 58,                          // [RFC9149]
  dnssec_chain = 59,                            // [RFC9102], [RFC Errata 6860]
  reserved_or_unassigned = 65535                // Default for all reserved and unassigned values.
};

/*!
 * \brief Map with static storage used for fast lookup of value to TlsExtensionType's.
 */
// VECTOR NL AutosarC++17_10-A3.3.2: MD_libseccom_A3.3.2_DoNotUseNonPODTypeObjectsWithStaticStorageDuration
static amsr::core::Map<std::uint16_t const, TlsExtensionType const> const kValueToTlsExtensionsMapping{
    {0, TlsExtensionType::server_name},
    {1, TlsExtensionType::max_fragment_length},
    {2, TlsExtensionType::client_certificate_url},
    {3, TlsExtensionType::trusted_ca_keys},
    {4, TlsExtensionType::truncated_hmac},
    {5, TlsExtensionType::status_request},
    {6, TlsExtensionType::user_mapping},
    {7, TlsExtensionType::client_authz},
    {8, TlsExtensionType::server_authz},
    {9, TlsExtensionType::cert_type},
    {10, TlsExtensionType::supported_groups},
    {11, TlsExtensionType::ec_point_formats},
    {12, TlsExtensionType::srp},
    {13, TlsExtensionType::signature_algorithms},
    {14, TlsExtensionType::use_srtp},
    {15, TlsExtensionType::heartbeat},
    {16, TlsExtensionType::application_layer_protocol_negotiation},
    {17, TlsExtensionType::status_request_v2},
    {18, TlsExtensionType::signed_certificate_timestamp},
    {19, TlsExtensionType::client_certificate_type},
    {20, TlsExtensionType::server_certificate_type},
    {21, TlsExtensionType::padding},
    {22, TlsExtensionType::encrypt_then_mac},
    {23, TlsExtensionType::extended_master_secret},
    {24, TlsExtensionType::token_binding},
    {25, TlsExtensionType::cached_info},
    {26, TlsExtensionType::tls_lts},
    {27, TlsExtensionType::compress_certificate},
    {28, TlsExtensionType::record_size_limit},
    {29, TlsExtensionType::pwd_protect},
    {30, TlsExtensionType::pwd_clear},
    {31, TlsExtensionType::password_salt},
    {32, TlsExtensionType::ticket_pinning},
    {33, TlsExtensionType::tls_cert_with_extern_psk},
    {34, TlsExtensionType::delegated_credentials},
    {35, TlsExtensionType::session_ticket},
    {36, TlsExtensionType::TLMSP},
    {37, TlsExtensionType::TLMSP_proxying},
    {38, TlsExtensionType::TLMSP_delegate},
    {39, TlsExtensionType::supported_ekt_ciphers},
    {41, TlsExtensionType::pre_shared_key},
    {42, TlsExtensionType::early_data},
    {43, TlsExtensionType::supported_versions},
    {44, TlsExtensionType::cookie},
    {45, TlsExtensionType::psk_key_exchange_modes},
    {47, TlsExtensionType::certificate_authorities},
    {48, TlsExtensionType::oid_filters},
    {49, TlsExtensionType::post_handshake_auth},
    {50, TlsExtensionType::signature_algorithms_cert},
    {51, TlsExtensionType::key_share},
    {52, TlsExtensionType::transparency_info},
    {53, TlsExtensionType::connection_id_deprecated},
    {54, TlsExtensionType::connection_id},
    {55, TlsExtensionType::external_id_hash},
    {56, TlsExtensionType::external_session_id},
    {57, TlsExtensionType::quic_transport_parameters},
    {58, TlsExtensionType::ticket_request},
    {59, TlsExtensionType::dnssec_chain}};

/*!
 * \brief Returns the TlsExtensionType for the given value or "reserved_or_unassigned" if the value cannot be found.
 * \param[in] value A value representing a TLS extension type.
 * \return "reserved_or_unassigned" or the TlsExtensionType corresponding to the given value.
 */
static inline TlsExtensionType ValueToTlsExtensionType(std::uint16_t const value) noexcept {
  TlsExtensionType extension_type{TlsExtensionType::reserved_or_unassigned};
  if (kValueToTlsExtensionsMapping.count(value) > 0U) {
    extension_type = kValueToTlsExtensionsMapping.at(value);
  }
  return extension_type;
}

}  // namespace extensions
}  // namespace messages
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_EXTENSIONS_TLS_EXTENSIONS_H_
