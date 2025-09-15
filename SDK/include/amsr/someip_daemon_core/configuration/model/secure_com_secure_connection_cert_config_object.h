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
/*!        \file  secure_com_secure_connection_cert_config_object.h
 *        \brief  Elements of a single secure connection certificate configuration object.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SECURE_COM_SECURE_CONNECTION_CERT_CONFIG_OBJECT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SECURE_COM_SECURE_CONNECTION_CERT_CONFIG_OBJECT_H_
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
 * \brief This is a representation of a secure connection certificate configuration object which is filled by a parser.
 * \details The validator then takes this object and validates if mandatory elements are set or not.
 */
class SecureComSecureConnectionCertConfigObject final {
 public:
  /*!
   * \brief Private key UUID element of this secure connection certification configuration object.
   */
  using PrivateKeyUuidElement = CfgElement<ConfigurationTypesAndDefs::PrivateKeyUuid>;

  /*!
   * \brief Certificate chain label list element of this secure connection certification configuration object.
   */
  using CertChainLabelListElement = CfgElement<ara::core::Vector<ara::core::String>>;

  /*!
   * \brief Constructor sets the references to the elements of the structure to write to.
   * \param[in] cert_config Reference to the cert_config structure to write to during parsing.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  explicit SecureComSecureConnectionCertConfigObject(ConfigurationTypesAndDefs::CertConfig& cert_config) noexcept
      : private_key_uuid_{cert_config.private_key_uuid},
        cert_chain_label_list_{cert_config.certificate_chain_label_list} {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~SecureComSecureConnectionCertConfigObject() = default;

  /*!
   * \brief Deleted copy constructor.
   */
  SecureComSecureConnectionCertConfigObject(SecureComSecureConnectionCertConfigObject const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  SecureComSecureConnectionCertConfigObject(SecureComSecureConnectionCertConfigObject&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(SecureComSecureConnectionCertConfigObject const&)
      -> SecureComSecureConnectionCertConfigObject& = delete;

  /*!
   * \brief Move assignment is used for resetting.
   * \steady FALSE
   */
  auto operator=(SecureComSecureConnectionCertConfigObject&& other) & noexcept
      -> SecureComSecureConnectionCertConfigObject& = default;

  /*!
   * \brief Set the private key UUID element.
   * \param[in] private_key_uuid The private key UUID.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetPrivateKeyUuid(ConfigurationTypesAndDefs::PrivateKeyUuid const& private_key_uuid) noexcept {
    private_key_uuid_.SetElement(private_key_uuid);
  }

  /*!
   * \brief Getter for the private key UUID element.
   * \return The private key UUID element.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  PrivateKeyUuidElement const& GetPrivateKeyUuid() const noexcept { return private_key_uuid_; }

  /*!
   * \brief Set the certificate chain label list element.
   * \param[in] cert_chain_label_list The certificate chain label list.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetCertificateChainLabelList(ara::core::Vector<ara::core::String> const& cert_chain_label_list) noexcept {
    cert_chain_label_list_.SetElement(cert_chain_label_list);
  }

  /*!
   * \brief Getter for the certificate chain label list element.
   * \return The certificate chain label list element.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  CertChainLabelListElement const& GetCertificateChainLabelList() const noexcept { return cert_chain_label_list_; }

 private:
  /*!
   * \brief The private key UUID element.
   */
  PrivateKeyUuidElement private_key_uuid_;

  /*!
   * \brief The certificate chain label list element.
   */
  CertChainLabelListElement cert_chain_label_list_;
};
}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SECURE_COM_SECURE_CONNECTION_CERT_CONFIG_OBJECT_H_
