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
/*!        \file  someipd_posix_json_object.h
 *        \brief  Elements of a someip daemon posix json object.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SOMEIPD_POSIX_JSON_OBJECT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SOMEIPD_POSIX_JSON_OBJECT_H_
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
 * \brief This is an intermediate representation of a someip daemon posix json object which is filled by a parser.
 * \details The validator then takes this object and validates if mandatory elements are set or not.
 */
class SomeIpdPosixJsonObject final {
 public:
  /*!
   * \brief Type for the application elements.
   */
  using ApplicationContainerElement = CfgElement<ConfigurationTypesAndDefs::ApplicationContainer>;

  /*!
   * \brief Type for the crypto provider uuid element.
   */
  using CryptoProviderUuidElement = CfgElement<ConfigurationTypesAndDefs::CryptoProviderUuid>;

  /*!
   * \brief TLS Crypto mode remote flag element wrapper type.
   */
  using TlsEnableRemoteCryptoModeElement = CfgElement<bool>;

  /*!
   * \brief Merge RSI with different minor version flag element wrapper type.
   */
  using MergeRSIWithDifferentMinorVersionElement = CfgElement<bool>;

  /*!
   * \brief Constructor sets the references to the elements of the structure to write to.
   * \param[in] someipd_posix_json Reference to the someipd_posix_json structure to write to during parsing.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  explicit SomeIpdPosixJsonObject(ConfigurationTypesAndDefs::SomeIpdPosixJson& someipd_posix_json) noexcept
      : applications_{someipd_posix_json.applications_},
        trustzone_crypto_provider_uuid_{someipd_posix_json.trustzone_crypto_provider_uuid},
        tls_crypto_mode_remote_{someipd_posix_json.tls_crypto_mode_remote_},
        merge_rsi_with_different_minor_versions_{someipd_posix_json.merge_rsi_with_different_minor_versions} {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~SomeIpdPosixJsonObject() = default;

  /*!
   * \brief Deleted copy constructor.
   */
  SomeIpdPosixJsonObject(SomeIpdPosixJsonObject const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  SomeIpdPosixJsonObject(SomeIpdPosixJsonObject&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(SomeIpdPosixJsonObject const&) & -> SomeIpdPosixJsonObject& = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(SomeIpdPosixJsonObject&&) & -> SomeIpdPosixJsonObject& = delete;

  /*!
   * \brief Set the container of applications after parsing.
   * \param[in] applications The application container
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetApplicationContainer(ConfigurationTypesAndDefs::ApplicationContainer const& applications) noexcept {
    applications_.SetElement(applications);
  }

  /*!
   * \brief Getter for the application container element.
   * \return The application container.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ApplicationContainerElement const& GetApplicationContainer() const noexcept { return applications_; }

  /*!
   * \brief Set the trustzone crypto provider UUID element.
   * \param[in] crypto_provider_uuid The trustzone crypto provider UUID.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetTrustzoneCryptoProviderUuid(
      ConfigurationTypesAndDefs::CryptoProviderUuid const& crypto_provider_uuid) noexcept {
    trustzone_crypto_provider_uuid_.SetElement(crypto_provider_uuid);
  }

  /*!
   * \brief Getter for the trustzone crypto provider UUID.
   * \return The trustzone crypto provider UUID element.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  CryptoProviderUuidElement const& GetTrustzoneCryptoProviderUuid() const noexcept {
    return trustzone_crypto_provider_uuid_;
  }

  /*!
   * \brief Set the passed value on the TLS crypto mode remote flag.
   * \param[in] value "true" to enabled the flag and "false" to disable it.
   * \steady FALSE
   */
  void SetTlsRemoteCryptoMode(bool value) noexcept { tls_crypto_mode_remote_.SetElement(value); }

  /*!
   * \brief Get the TLS crypto mode remote flag wrapper.
   * \return Reference to the element wrapper for the flag.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  TlsEnableRemoteCryptoModeElement const& GetTlsRemoteCryptoMode() const noexcept { return tls_crypto_mode_remote_; }

  /*!
   * \brief Set the passed value on the merge RSI with different minor version flag.
   * \param[in]value "true" to enabled the flag and "false" to disable it.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetMergeRSIWithDifferentMinorVersion(bool value) noexcept {
    merge_rsi_with_different_minor_versions_.SetElement(value);
  }

  /*!
   * \brief Get the merge RSI with different minor version flag wrapper.
   * \return Reference to the element wrapper for the flag.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto GetMergeRSIWithDifferentMinorVersion() const noexcept -> MergeRSIWithDifferentMinorVersionElement const& {
    return merge_rsi_with_different_minor_versions_;
  }

 private:
  /*!
   * \brief Applications contained in this someipd_posix_json struct.
   */
  ApplicationContainerElement applications_;

  /*!
   * \brief Trustzone crypto provider UUID.
   */
  CryptoProviderUuidElement trustzone_crypto_provider_uuid_;

  /*!
   * \brief TLS remote crypto mode element.
   */
  TlsEnableRemoteCryptoModeElement tls_crypto_mode_remote_;

  /*!
   * \brief A flag to indicate merge different required service instances that differ in minor version together or not.
   */
  MergeRSIWithDifferentMinorVersionElement merge_rsi_with_different_minor_versions_;
};

}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_SOMEIPD_POSIX_JSON_OBJECT_H_
