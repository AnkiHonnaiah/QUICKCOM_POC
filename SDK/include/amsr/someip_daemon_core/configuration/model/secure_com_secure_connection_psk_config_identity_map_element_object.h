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
/*!        \file  secure_com_secure_connection_psk_config_identity_map_element_object.h
 *        \brief  Elements of a single secure connection psk config identity map element object.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SECURE_COM_SECURE_CONNECTION_PSK_CONFIG_IDENTITY_MAP_ELEMENT_OBJECT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SECURE_COM_SECURE_CONNECTION_PSK_CONFIG_IDENTITY_MAP_ELEMENT_OBJECT_H_
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

 * \brief This is a representation of a PSK configuration identity map element of a secure connection which is filled by
 a parser.
 * \details The validator then takes this object and validates if mandatory elements are set or not.
 */
class SecureComSecureConnectionPskConfigIdentityMapElementObject final {
 public:
  /*!
   * \brief PSK identity hint element of this PSK configuration identity map element object.
   */
  using PskIdentityHintElement = CfgElement<ara::core::Optional<ConfigurationTypesAndDefs::PskIdentityHint>>;

  /*!
   * \brief PSK identity element of this PSK configuration identity map element object.
   */
  using PskIdentityElement = CfgElement<ConfigurationTypesAndDefs::PskIdentity>;

  /*!
   * \brief PSK UUID element of this PSK configuration identity map element object.
   */
  using PskUuidElement = CfgElement<ConfigurationTypesAndDefs::PskUuid>;

  /*!
   * \brief Constructor sets the references to the elements of the structure to write to.
   * \param[in] identity_map_element Reference to the identity_map_element structure to write to during
   * parsing.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  explicit SecureComSecureConnectionPskConfigIdentityMapElementObject(
      ConfigurationTypesAndDefs::PskIdentityMapElement& identity_map_element) noexcept
      : psk_identity_hint_{identity_map_element.psk_identity_hint},
        psk_identity_{identity_map_element.psk_identity},
        psk_uuid_{identity_map_element.psk_uuid} {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~SecureComSecureConnectionPskConfigIdentityMapElementObject() = default;

  /*!
   * \brief Deleted copy constructor.
   */
  SecureComSecureConnectionPskConfigIdentityMapElementObject(
      SecureComSecureConnectionPskConfigIdentityMapElementObject const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  SecureComSecureConnectionPskConfigIdentityMapElementObject(
      SecureComSecureConnectionPskConfigIdentityMapElementObject&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(SecureComSecureConnectionPskConfigIdentityMapElementObject const&)
      -> SecureComSecureConnectionPskConfigIdentityMapElementObject& = delete;

  /*!
   * \brief Move assignment is used for resetting.
   * \steady FALSE
   */
  auto operator=(SecureComSecureConnectionPskConfigIdentityMapElementObject&& other) & noexcept
      -> SecureComSecureConnectionPskConfigIdentityMapElementObject& = default;

  /*!
   * \brief Set the psk identity hint element.
   * \param[in] psk_identity_hint The psk identity hint.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetPskIdentityHint(ConfigurationTypesAndDefs::PskIdentityHint const& psk_identity_hint) noexcept {
    psk_identity_hint_.SetElement(psk_identity_hint);
  }

  /*!
   * \brief Set the psk identity element.
   * \param[in] psk_identity The psk identity.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetPskIdentity(ConfigurationTypesAndDefs::PskIdentity const& psk_identity) noexcept {
    psk_identity_.SetElement(psk_identity);
  }

  /*!
   * \brief Getter for the psk identity element.
   * \return The PSK identity element.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  PskIdentityElement const& GetPskIdentity() const noexcept { return psk_identity_; }

  /*!
   * \brief Set the psk UUID element.
   * \param[in] psk_uuid The psk UUID.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetPskUuid(ConfigurationTypesAndDefs::PskUuid const& psk_uuid) noexcept { psk_uuid_.SetElement(psk_uuid); }

  /*!
   * \brief Getter for the psk UUID element.
   * \return The psk UUID element.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  PskUuidElement const& GetPskUuid() const noexcept { return psk_uuid_; }

 private:
  /*!
   * \brief The psk identity hint element.
   */
  PskIdentityHintElement psk_identity_hint_;

  /*!
   * \brief The psk identity element.
   */
  PskIdentityElement psk_identity_;

  /*!
   * \brief The psk UUID element.
   */
  PskUuidElement psk_uuid_;
};
}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SECURE_COM_SECURE_CONNECTION_PSK_CONFIG_IDENTITY_MAP_ELEMENT_OBJECT_H_
