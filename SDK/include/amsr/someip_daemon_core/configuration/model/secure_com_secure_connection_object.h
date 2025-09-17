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
/*!        \file  secure_com_secure_connection_object.h
 *        \brief  Elements of a single secure connection object.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SECURE_COM_SECURE_CONNECTION_OBJECT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SECURE_COM_SECURE_CONNECTION_OBJECT_H_
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
 * \brief This is a representation of a secure connection object which is filled by a parser.
 * \details The validator then takes this object and validates if mandatory elements are set or not.
 */
class SecureComSecureConnectionObject final {
 public:
  /*!
   * \brief Secure ID element of this secure connection object.
   */
  using SecureIdElement = CfgElement<ConfigurationTypesAndDefs::SecureId>;

  /*!
   * \brief TLS version element of this secure connection object.
   */
  using TlsVersionElement = CfgElement<ConfigurationTypesAndDefs::TlsVersion>;

  /*!
   * \brief Cipher suite ID element of this secure connection object.
   */
  using CipherSuiteIdElement = CfgElement<ConfigurationTypesAndDefs::CipherSuiteId>;

  /*!
   * \brief Priority element of this secure connection object.
   */
  using PriorityElement = CfgElement<ConfigurationTypesAndDefs::SecureConnectionPriority>;

  /*!
   * \brief PSK configuration element of this secure connection object.
   */
  using PskConfigElement = CfgElement<ara::core::Optional<ConfigurationTypesAndDefs::PskConfig>>;

  /*!
   * \brief Certification configuration element of this secure connection object.
   */
  using CertConfigElement = CfgElement<ara::core::Optional<ConfigurationTypesAndDefs::CertConfig>>;

  /*!
   * \brief Dtls cookie verification enabled element of this secure endpoint object.
   * \details This flag is only valid for udp endpoints. Hence it's optional.
   */
  using DtlsCookieVerificationEnabledElement = CfgElement<ara::core::Optional<bool>>;

  /*!
   * \brief Constructor sets the references to the elements of the structure to write to.
   * \param[in] secure_connection Reference to the secure_connection structure to write to during parsing.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  explicit SecureComSecureConnectionObject(ConfigurationTypesAndDefs::SecureConnection& secure_connection) noexcept
      : secure_id_{secure_connection.secure_id},
        tls_version_{secure_connection.tls_version},
        cipher_suite_id_{secure_connection.cipher_suite_id},
        priority_{secure_connection.priority},
        psk_config_{secure_connection.psk_config},
        cert_config_{secure_connection.cert_config},
        dtls_cookie_verification_enabled_{secure_connection.dtls_cookie_verification_enabled} {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~SecureComSecureConnectionObject() = default;

  /*!
   * \brief Deleted copy constructor.
   */
  SecureComSecureConnectionObject(SecureComSecureConnectionObject const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  SecureComSecureConnectionObject(SecureComSecureConnectionObject&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(SecureComSecureConnectionObject const&) -> SecureComSecureConnectionObject& = delete;

  /*!
   * \brief Move assignment is used for resetting.
   * \steady FALSE
   */
  auto operator=(SecureComSecureConnectionObject&& other) & noexcept -> SecureComSecureConnectionObject& = default;

  /*!
   * \brief Set the secure ID element.
   * \param[in] secure_id The secure ID.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetSecureId(ConfigurationTypesAndDefs::SecureId const& secure_id) noexcept { secure_id_.SetElement(secure_id); }

  /*!
   * \brief Getter for the secure ID element.
   * \return The secure ID.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  SecureIdElement const& GetSecureId() const noexcept { return secure_id_; }

  /*!
   * \brief Set the TLS version element.
   * \param[in] tls_version The TLS version.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetTlsVersion(ConfigurationTypesAndDefs::TlsVersion const& tls_version) noexcept {
    tls_version_.SetElement(tls_version);
  }

  /*!
   * \brief Getter for the TLS version element.
   * \return The TLS version.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  TlsVersionElement const& GetTlsVersion() const noexcept { return tls_version_; }

  /*!
   * \brief Set the cipher suite ID element.
   * \param[in] cipher_suite_id The cipher suite ID.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetCipherSuiteId(ConfigurationTypesAndDefs::CipherSuiteId const& cipher_suite_id) noexcept {
    cipher_suite_id_.SetElement(cipher_suite_id);
  }

  /*!
   * \brief Getter for the cipher suite ID element.
   * \return The cipher suite ID.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  CipherSuiteIdElement const& GetCipherSuiteId() const noexcept { return cipher_suite_id_; }

  /*!
   * \brief Set the priority element.
   * \param[in] priority The priority.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetPriority(ConfigurationTypesAndDefs::SecureConnectionPriority const& priority) noexcept {
    priority_.SetElement(priority);
  }

  /*!
   * \brief Getter for the priority element.
   * \return The priority.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  PriorityElement const& GetPriority() const noexcept { return priority_; }

  /*!
   * \brief Set the PSK configuration element.
   * \param[in] psk_config The PSK configuration.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetPskConfig(ConfigurationTypesAndDefs::PskConfig const& psk_config) noexcept {
    psk_config_.SetElement(psk_config);
  }

  /*!
   * \brief Getter for the PSK configuration element.
   * \return The PSK configuration.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  PskConfigElement const& GetPskConfig() const noexcept { return psk_config_; }

  /*!
   * \brief Set the certification configuration element.
   * \param[in] cert_config The certification configuration.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetCertConfig(ConfigurationTypesAndDefs::CertConfig const& cert_config) noexcept {
    cert_config_.SetElement(cert_config);
  }

  /*!
   * \brief Getter for the certification configuration element.
   * \return The certification configuration.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  CertConfigElement const& GetCertConfig() const noexcept { return cert_config_; }

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

 private:
  /*!
   * \brief The secure ID element.
   */
  SecureIdElement secure_id_;

  /*!
   * \brief The TLS version element.
   */
  TlsVersionElement tls_version_;

  /*!
   * \brief The cipher suite ID element.
   */
  CipherSuiteIdElement cipher_suite_id_;

  /*!
   * \brief The priority element.
   */
  PriorityElement priority_;

  /*!
   * \brief The PSK configuration element.
   */
  PskConfigElement psk_config_;

  /*!
   * \brief The certification configuration element.
   */
  CertConfigElement cert_config_;

  /*!
   * \brief The cookie verification enabled element.
   */
  DtlsCookieVerificationEnabledElement dtls_cookie_verification_enabled_;
};
}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SECURE_COM_SECURE_CONNECTION_OBJECT_H_
