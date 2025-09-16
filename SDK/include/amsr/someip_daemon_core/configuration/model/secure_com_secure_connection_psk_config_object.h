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
/*!        \file  secure_com_secure_connection_psk_config_object.h
 *        \brief  Elements of a single secure connection psk config object.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SECURE_COM_SECURE_CONNECTION_PSK_CONFIG_OBJECT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SECURE_COM_SECURE_CONNECTION_PSK_CONFIG_OBJECT_H_
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
 * \brief This is a representation of a secure connection psk config object which is filled by a parser.
 * \details The validator then takes this object and validates if mandatory elements are set or not.
 */
// VECTOR NL Metric-OO.WMC.One: MD_SomeIpDaemon_Metric-OO.WMC.One_methods_per_class
class SecureComSecureConnectionPskConfigObject final {
 public:
  /*!
   * \brief Psk identity hint element of this secure connection psk config object. This element is only valid on server
   * side.
   */
  using PskIdentityHintElement = CfgElement<ara::core::Optional<ConfigurationTypesAndDefs::PskIdentityHint>>;

  /*!
   * \brief Psk identity map element of this secure connection object.
   */
  using PskIdentityMapContainerElement = CfgElement<ConfigurationTypesAndDefs::PskIdentityMapElementContainer>;

  /*!
   * \brief Constructor sets the references to the elements of the structure to write to.
   * \param[in] psk_config Reference to the psk_config structure to write to during parsing.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  explicit SecureComSecureConnectionPskConfigObject(ConfigurationTypesAndDefs::PskConfig& psk_config) noexcept
      : psk_identity_hint_{psk_config.psk_identity_hint}, psk_identity_map_container_{psk_config.psk_identity_map} {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~SecureComSecureConnectionPskConfigObject() = default;

  /*!
   * \brief Deleted copy constructor.
   */
  SecureComSecureConnectionPskConfigObject(SecureComSecureConnectionPskConfigObject const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  SecureComSecureConnectionPskConfigObject(SecureComSecureConnectionPskConfigObject&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(SecureComSecureConnectionPskConfigObject const&) -> SecureComSecureConnectionPskConfigObject& = delete;

  /*!
   * \brief Move assignment is used for resetting.
   * \steady FALSE
   */
  auto operator=(SecureComSecureConnectionPskConfigObject&& other) & noexcept
      -> SecureComSecureConnectionPskConfigObject& = default;

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
   * \return The PSK identity hint.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  PskIdentityHintElement const& GetPskIdentityHint() const noexcept { return psk_identity_hint_; }

  /*!
   * \brief Set the PSK identity map.
   * \param[in] psk_identity_map_container The PSK identity map container.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetPskIdentityMap(
      ConfigurationTypesAndDefs::PskIdentityMapElementContainer const& psk_identity_map_container) noexcept {
    psk_identity_map_container_.SetElement(psk_identity_map_container);
  }

  /*!
   * \brief Getter for the PSK identity map element.
   * \return The PSK identity map.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  PskIdentityMapContainerElement const& GetPskIdentityMap() const noexcept { return psk_identity_map_container_; }

 private:
  /*!
   * \brief The PSK identity hint element. It only used for the server side.
   */
  PskIdentityHintElement psk_identity_hint_;

  /*!
   * \brief The PSK identity map element.
   */
  PskIdentityMapContainerElement psk_identity_map_container_;
};

}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SECURE_COM_SECURE_CONNECTION_PSK_CONFIG_OBJECT_H_
