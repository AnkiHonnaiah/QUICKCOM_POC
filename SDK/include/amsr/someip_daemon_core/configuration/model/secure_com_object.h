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
/*!     \file     secure_com_object.h
 *      \brief    Configuration object for 'sec_com'.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SECURE_COM_OBJECT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SECURE_COM_OBJECT_H_
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
 * \brief This is an intermediate representation of a 'Sec com' object which is filled by a parser.
 * \details The validator then takes this object and validates if mandatory elements are set or not.
 */
class SecureComConfigObject final {
 public:
  /*!
   * \brief PSK identity element of this secure endpoint object.
   */
  using PskIdentityElement = CfgElement<ConfigurationTypesAndDefs::PskIdentity>;

  /*!
   * \brief TLS Crypto mode remote flag element wrapper type.
   */
  using TlsCryptoModeRemoteElement = CfgElement<bool>;

  /*!
   * \brief Secure endpoint sub-hierarchy.
   */
  using SecureEndpointsElement = CfgElement<configuration::ConfigurationTypesAndDefs::SecureEndpointContainer>;

  /*!
   * \brief Secure connections sub-hierarchy.
   */
  using SecureConnectionsElement =
      CfgElement<ara::core::Optional<ConfigurationTypesAndDefs::SecureConnectionContainer>>;

  /*!
   * \brief Constructor sets the references to the elements of the structure to write to.
   * \param[in] sec_com Reference to the 'Sec com' structure to write to.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  explicit SecureComConfigObject(configuration::ConfigurationTypesAndDefs::SecureCom& sec_com) noexcept
      : psk_identity_hint_{sec_com.psk_identity_hint},
        secure_endpoints_{sec_com.secure_endpoints},
        secure_connections_{sec_com.secure_connections} {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~SecureComConfigObject() = default;

  /*!
   * \brief Deleted copy constructor.
   */
  SecureComConfigObject(SecureComConfigObject const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  SecureComConfigObject(SecureComConfigObject&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(SecureComConfigObject const&) -> SecureComConfigObject& = delete;

  /*!
   * \brief Move assignment is used for resetting.
   * \steady FALSE
   */
  auto operator=(SecureComConfigObject&& other) & noexcept -> SecureComConfigObject& = default;

  /*!
   * \brief Set the machines PSK identity hint.
   * \param[in] psk_identity_hint The machines PSK identity hint.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetPskIdentityHint(configuration::ConfigurationTypesAndDefs::PskIdentityHint const& psk_identity_hint) noexcept {
    psk_identity_hint_.SetElement(psk_identity_hint);
  }

  /*!
   * \brief Getter for the machines PSK identity hint.
   * \return Reference to the PSK identity hint element of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  PskIdentityElement const& GetPskIdentityHint() const noexcept { return psk_identity_hint_; }

  /*!
   * \brief Set the Secure channels sub-hierarchy Secure endpoints.
   * \param[in] secure_endpoints Secure secure endpoints.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetSecureEndpoints(
      configuration::ConfigurationTypesAndDefs::SecureEndpointContainer const& secure_endpoints) noexcept {
    secure_endpoints_.SetElement(secure_endpoints);
  }

  /*!
   * \brief Getter for the Secure endpoints sub-hierarchy.
   * \return Reference to the Secure endpoints sub-hierarchy of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  SecureEndpointsElement const& GetSecureEndpoints() const noexcept { return secure_endpoints_; }

  /*!
   * \brief Set the Secure connections sub-hierarchy.
   * \param[in] secure_connections secure connections.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetSecureConnections(
      configuration::ConfigurationTypesAndDefs::SecureConnectionContainer const& secure_connection) noexcept {
    secure_connections_.SetElement(secure_connection);
  }

  /*!
   * \brief Getter for the Secure connections sub-hierarchy.
   * \return Reference to the Secure connections sub-hierarchy of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  SecureConnectionsElement const& GetSecureConnections() const noexcept { return secure_connections_; }

 private:
  /*!
   * \brief The machines own PSK identity hint.
   */
  PskIdentityElement psk_identity_hint_;

  /*!
   * \brief The Secure endpoints sub-hierarchy.
   */
  SecureEndpointsElement secure_endpoints_;

  /*!
   * \brief The Secure connections sub-hierarchy.
   */
  SecureConnectionsElement secure_connections_;
};

}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SECURE_COM_OBJECT_H_
