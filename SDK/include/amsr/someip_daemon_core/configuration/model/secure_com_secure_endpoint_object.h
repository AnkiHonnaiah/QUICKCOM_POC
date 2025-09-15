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
/*!        \file  secure_com_secure_endpoint_object.h
 *        \brief  Elements of a single secure endpoint object.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SECURE_COM_SECURE_ENDPOINT_OBJECT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SECURE_COM_SECURE_ENDPOINT_OBJECT_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief This is a representation of a secure endpoint object which is filled by a parser.
 * \details The validator then takes this object and validates if mandatory elements are set or not.
 */
// VECTOR NL Metric-OO.WMC.One: MD_SomeIpDaemon_Metric-OO.WMC.One_methods_per_class
class SecureComSecureEndpointObject final {
 public:
  /*!
   * \brief Address element of this secure endpoint object.
   */
  using AddressElement = CfgElement<someip_daemon_core::IpAddress>;

  /*!
   * \brief Port element of this secure endpoint object.
   */
  using PortElement = CfgElement<someip_protocol::internal::Port>;

  /*!
   * \brief Proto element of this secure endpoint object.
   */
  using ProtoElement = CfgElement<ConfigurationTypesAndDefs::Protocol>;

  /*!
   * \brief IsServer element of this secure endpoint object.
   */
  using IsServerElement = CfgElement<bool>;

  /*!
   * \brief Cipher suite id container element of this secure endpoint object.
   */
  using CipherSuiteIdContainerElement = CfgElement<ConfigurationTypesAndDefs::CipherSuiteIdContainer>;

  /*!
   * \brief PSK UUID element of this secure endpoint object.
   */
  using PskUuidElement = CfgElement<ConfigurationTypesAndDefs::PskUuid>;

  /*!
   * \brief PSK identity element of this secure endpoint object.
   */
  using PskIdentityElement = CfgElement<ConfigurationTypesAndDefs::PskIdentity>;

  /*!
   * \brief PSK identity hint element of this secure endpoint object.
   */
  using PskIdentityHintElement = CfgElement<ConfigurationTypesAndDefs::PskIdentityHint>;

  /*!
   * \brief Dtls cookie verification enabled element of this secure endpoint object.
   * \details This flag is only valid for udp endpoints. Hence it's optional.
   */
  using DtlsCookieVerificationEnabledElement = CfgElement<ara::core::Optional<bool>>;

  /*!
   * \brief Constructor sets the references to the elements of the structure to write to.
   * \param[in] secure_endpoint Reference to the secure_endpoint structure to write to during parsing.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  explicit SecureComSecureEndpointObject(ConfigurationTypesAndDefs::SecureEndpoint& secure_endpoint) noexcept
      : address_{secure_endpoint.address},
        port_{secure_endpoint.port},
        transport_proto_{secure_endpoint.transport_proto},
        is_server_{secure_endpoint.is_server},
        cipher_suite_ids_{secure_endpoint.cipher_suite_ids},
        psk_uuid_{secure_endpoint.psk_uuid},
        psk_identity_{secure_endpoint.psk_identity},
        psk_identity_hint_{secure_endpoint.psk_identity_hint},
        dtls_cookie_verification_enabled_{secure_endpoint.dtls_cookie_verification_enabled} {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~SecureComSecureEndpointObject() = default;

  /*!
   * \brief Deleted copy constructor.
   */
  SecureComSecureEndpointObject(SecureComSecureEndpointObject const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  SecureComSecureEndpointObject(SecureComSecureEndpointObject&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(SecureComSecureEndpointObject const&) -> SecureComSecureEndpointObject& = delete;

  /*!
   * \brief Move assignment is used for resetting.
   * \steady FALSE
   */
  auto operator=(SecureComSecureEndpointObject&& other) & noexcept -> SecureComSecureEndpointObject& = default;

  /*!
   * \brief Set the address element.
   * \param[in] address The address.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetAddress(someip_daemon_core::IpAddress const& address) noexcept { address_.SetElement(address); }

  /*!
   * \brief Getter for the domain element.
   * \return The address.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  AddressElement const& GetAddress() const noexcept { return address_; }

  /*!
   * \brief Set the port element.
   * \param[in] port The port.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetPort(someip_protocol::internal::Port const& port) noexcept { port_.SetElement(port); }

  /*!
   * \brief Getter for the port element.
   * \return The port.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  PortElement const& GetPort() const noexcept { return port_; }

  /*!
   * \brief Set the transport protocol element.
   * \param[in] transport_proto The transport protocol.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetProto(ConfigurationTypesAndDefs::Protocol const& transport_proto) noexcept {
    transport_proto_.SetElement(transport_proto);
  }

  /*!
   * \brief Getter for the transport protocol element.
   * \return The transport protocol.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ProtoElement const& GetProto() const noexcept { return transport_proto_; }

  /*!
   * \brief Set the is server element.
   * \param[in] is_server The is server value.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetIsServer(bool is_server) noexcept { is_server_.SetElement(is_server); }

  /*!
   * \brief Getter for the is server element.
   * \return The is server element.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  IsServerElement const& GetIsServer() const noexcept { return is_server_; }

  /*!
   * \brief Set the cipher suite IDs element.
   * \param[in] cipher_suite_ids The cipher suite ids
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetCipherSuiteIds(ConfigurationTypesAndDefs::CipherSuiteIdContainer const& cipher_suite_ids) noexcept {
    cipher_suite_ids_.SetElement(cipher_suite_ids);
  }

  /*!
   * \brief Getter for the cipher suite IDs element.
   * \return Reference to the cipher suite ID element.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  CipherSuiteIdContainerElement const& GetCipherSuiteIds() const noexcept { return cipher_suite_ids_; }

  /*!
   * \brief Set the PSK UUID element.
   * \param[in] psk_uuid The PSK UUID.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetPskUuid(ConfigurationTypesAndDefs::PskUuid const& psk_uuid) noexcept { psk_uuid_.SetElement(psk_uuid); }

  /*!
   * \brief Getter for the PSK UUID element.
   * \return Reference to the PSK UUID element.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  PskUuidElement const& GetPskUuid() const noexcept { return psk_uuid_; }

  /*!
   * \brief Set the PSK identity element.
   * \param[in] psk_identity The PSK identity.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetPskIdentity(ConfigurationTypesAndDefs::PskIdentity const& psk_identity) noexcept {
    psk_identity_.SetElement(psk_identity);
  }

  /*!
   * \brief Getter for the PSK identity element.
   * \return Reference to the PSK identity element.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  PskIdentityElement const& GetPskIdentity() const noexcept { return psk_identity_; }

  /*!
   * \brief Set the PSK identity hint element.
   * \param[in] psk_identity_hint The PSK identity hint.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetPskIdentityHint(ConfigurationTypesAndDefs::PskIdentityHint const& psk_identity_hint) noexcept {
    psk_identity_hint_.SetElement(psk_identity_hint);
  }

  /*!
   * \brief Getter for the PSK identity hint element.
   * \return Reference to the PSK identity element.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  PskIdentityHintElement const& GetPskIdentityHint() const noexcept { return psk_identity_hint_; }

  /*!
   * \brief Set the dtls cookie verification enabled element.
   * \param[in] dtls_cookie_verification_enabled the dtls cookie verification enabled.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetDtlsCookieVerificationEnabled(ara::core::Optional<bool> dtls_cookie_verification_enabled) noexcept {
    dtls_cookie_verification_enabled_.SetElement(dtls_cookie_verification_enabled);
  }

  /*!
   * \brief Getter for the dtls cookie verification enabled element.
   * \return Reference to the dtls cookie verification enabled element.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  DtlsCookieVerificationEnabledElement const& GetDtlsCookieVerificationEnabled() const noexcept {
    return dtls_cookie_verification_enabled_;
  }

 private:
  /*!
   * \brief The address element.
   */
  AddressElement address_;

  /*!
   * \brief The port element.
   */
  PortElement port_;

  /*!
   * \brief The transport protocol element.
   */
  ProtoElement transport_proto_;

  /*!
   * \brief The server element.
   */
  IsServerElement is_server_;

  /*!
   * \brief The cipher suite id container element.
   */
  CipherSuiteIdContainerElement cipher_suite_ids_;

  /*!
   * \brief The PSK UUID element.
   */
  PskUuidElement psk_uuid_;

  /*!
   * \brief The psk identity element.
   */
  PskIdentityElement psk_identity_;

  /*!
   * \brief The psk identity hint element.
   */
  PskIdentityHintElement psk_identity_hint_;

  /*!
   * \brief The cookie verification enabled elemnt.
   */
  DtlsCookieVerificationEnabledElement dtls_cookie_verification_enabled_;
};

}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SECURE_COM_SECURE_ENDPOINT_OBJECT_H_
