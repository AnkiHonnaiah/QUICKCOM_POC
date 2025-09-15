// VECTOR SL AutosarC++17_10-A2.9.1: MD_CRYPTO_AutosarC++17_10-A2.9.1
/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  daemon_config.h
 *        \brief  Cryptostack daemon configuration abstraction.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_CONFIG_DAEMON_CONFIG_H_
#define LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_CONFIG_DAEMON_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/result.h"
#include "ara/core/string.h"
#include "crypto/common/data/data_container.h"
#include "crypto/common/data/json_keys.h"
#include "crypto/server/constants.h"

namespace crypto {
namespace server {
namespace config {

/*!
 * \brief Interface for a configuration.
 * \vprivate Component Private
 */
class Config {
 public:
  /*!
   * \brief Constructs the configuration struct.
   * \param[in] name Key under which the configuration is stored.
   * \param[in] parent Parent configuration.
   * \vprivate Component Private
   */
  explicit Config(amsr::core::String name, Config const* parent = nullptr) noexcept;

  /*!
   * \brief Deallocates the configuration class.
   * \vprivate Component Private
   */
  virtual ~Config() noexcept = default;

  /*!
   * \brief Returns the key under which the configuration is stored.
   * \return The key.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  amsr::core::String GetName() const noexcept;

  /*!
   * \brief Returns the fully qualified key under which the configuration is stored.
   * \return The fully qualified key.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  amsr::core::String GetQualifiedName() const noexcept;

  /*!
   * \brief Returns the fully qualified key under which the configuration is stored and appends a given key.
   * \param[in] suffix Key to append.
   * \return The fully qualified key including the appended key.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  amsr::core::String GetQualifiedName(amsr::core::String const& suffix) const noexcept;

  /*!
   * \brief Deserializes the configuration struct.
   * \param[in] serial Serialized configuration.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \error ara::crypto::SecurityErrc::kInvalidArgument If the serialized configuration is invalid.
   * \vprivate Component Private
   */
  virtual amsr::core::Result<void> Deserialize(crypto::common::data::DataContainer const& serial) noexcept = 0;

 protected:
  /*!
   * \brief Copy constructor.
   * \vprivate Component Private
   */
  Config(Config const& /*other*/) noexcept = default;

  /*!
   * \brief Copy assignment operator.
   * \return A reference to the object
   * \vprivate Component Private
   */
  Config& operator=(Config const& /*other*/) & noexcept = default;

  /*!
   * \brief Move constructor.
   * \vprivate Component Private
   */
  Config(Config&& /*other*/) noexcept = default;

  /*!
   * \brief Move assignment.
   * \return A reference to the object
   * \vprivate Component Private
   */
  Config& operator=(Config&& /*other*/) & noexcept = default;

 private:
  amsr::core::String name_; /*!< Key under which the configuration is stored. */
  Config const* parent_;    /*!< Parent configuration. */
};

/*!
 * \brief X.509 storage configuration.
 * \vprivate Component Private
 */
class DaemonX509StorageConfig final : public Config {
 public:
  using Config::Config;

  /*!
   * \brief Constructs the X.509 storage configuration.
   * \vprivate Component Private
   */
  DaemonX509StorageConfig() : Config("", nullptr) {}

  /*!
   * \brief Deallocates the configuration class.
   * \vprivate Component Private
   */
  ~DaemonX509StorageConfig() noexcept final = default;

  /*!
   * \brief Copy constructor.
   * \vprivate Component Private
   */
  DaemonX509StorageConfig(DaemonX509StorageConfig const& /*other*/) noexcept = default;

  /*!
   * \brief Copy assignment operator.
   * \return A reference to the object
   * \vprivate Component Private
   */
  DaemonX509StorageConfig& operator=(DaemonX509StorageConfig const& /*other*/) & noexcept = default;

  /*!
   * \brief Move constructor.
   * \vprivate Component Private
   */
  DaemonX509StorageConfig(DaemonX509StorageConfig&& /*other*/) noexcept = default;

  /*!
   * \brief Move assignment.
   * \return A reference to the object
   * \vprivate Component Private
   */
  DaemonX509StorageConfig& operator=(DaemonX509StorageConfig&& /*other*/) & noexcept = default;

  /*!
   * \copydoc config::Config::Deserialize
   * \vprivate Component Private
   */
  amsr::core::Result<void> Deserialize(crypto::common::data::DataContainer const& serial) noexcept final;

  /*!
   * \brief Getter for the storage root.
   * \return Storage root
   */
  amsr::core::String GetStorageRoot() const noexcept { return root_; }

 private:
  amsr::core::String root_{kDefaultCertStoreRoot}; /*!< Storage root */
};

/*!
 * \brief X.509 access configuration.
 * \vprivate Component Private
 * \trace CREQ-Crypto-X509Provider_X509_Privileged_Applications
 */
class DaemonX509AccessConfig final : public Config {
 public:
  using Config::Config;

  /*!
   * \brief Constructs the X.509 access configuration.
   * \vprivate Component Private
   */
  DaemonX509AccessConfig() : Config("", nullptr) {}

  /*!
   * \brief Deallocates the configuration class.
   * \vprivate Component Private
   */
  ~DaemonX509AccessConfig() noexcept final = default;

  /*!
   * \brief Copy constructor.
   * \vprivate Component Private
   */
  DaemonX509AccessConfig(DaemonX509AccessConfig const& /*other*/) noexcept = default;

  /*!
   * \brief Copy assignment operator.
   * \return A reference to the object
   * \vprivate Component Private
   */
  DaemonX509AccessConfig& operator=(DaemonX509AccessConfig const& /*other*/) & noexcept = default;

  /*!
   * \brief Move constructor.
   * \vprivate Component Private
   */
  DaemonX509AccessConfig(DaemonX509AccessConfig&& /*other*/) noexcept = default;

  /*!
   * \brief Move assignment.
   * \return A reference to the object
   * \vprivate Component Private
   */
  DaemonX509AccessConfig& operator=(DaemonX509AccessConfig&& /*other*/) & noexcept = default;

  /*!
   * \brief Getter for the CA connector id.
   * \return The CA connector id
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::uint64_t GetCaConnectorId() const noexcept { return ca_connector_id_; }

  /*!
   * \brief Getter for the trustmaster id.
   * \return The trustmaster id
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  std::uint64_t GetTrustmasterId() const noexcept { return trustmaster_id_; }

  /*!
   * \copydoc config::Config::Deserialize
   * \vprivate Component Private
   */
  amsr::core::Result<void> Deserialize(crypto::common::data::DataContainer const& serial) noexcept final;

 private:
  std::uint64_t ca_connector_id_{0}; /*!< CA connector ID */
  std::uint64_t trustmaster_id_{0};  /*!< Trustmaster ID */
};

/*!
 * \brief X.509 configuration.
 * \vprivate Component Private
 */
class DaemonX509Config final : public Config {
 public:
  /*!
   * \copydoc config::Config::Config
   * \vprivate Component Private
   */
  explicit DaemonX509Config(amsr::core::String name, Config const* parent = nullptr)
      : Config(std::move(name), parent),
        storage_{crypto::common::data::kDaemonConfigX509Storage, this},
        access_{crypto::common::data::kDaemonConfigX509Access, this} {}

  /*!
   * \brief Constructs the X.509 storage configuration.
   * \vprivate Component Private
   */
  DaemonX509Config() : DaemonX509Config("", nullptr) {}

  /*!
   * \brief Deallocates the configuration class.
   * \vprivate Component Private
   */
  ~DaemonX509Config() noexcept final = default;

  /*!
   * \brief Copy constructor.
   * \vprivate Component Private
   */
  DaemonX509Config(DaemonX509Config const& /*other*/) noexcept = default;

  /*!
   * \brief Copy assignment operator.
   * \return A reference to the object
   * \vprivate Component Private
   */
  DaemonX509Config& operator=(DaemonX509Config const& /*other*/) & noexcept = default;

  /*!
   * \brief Move constructor.
   * \vprivate Component Private
   */
  DaemonX509Config(DaemonX509Config&& /*other*/) noexcept = default;

  /*!
   * \brief Move assignment.
   * \return A reference to the object
   * \vprivate Component Private
   */
  DaemonX509Config& operator=(DaemonX509Config&& /*other*/) & noexcept = default;

  /*!
   * \copydoc config::Config::Deserialize
   * \vprivate Component Private
   */
  amsr::core::Result<void> Deserialize(crypto::common::data::DataContainer const& serial) noexcept final;

  /*!
   * \brief Getter for the storage configuration
   * \return Storage configuration
   */
  DaemonX509StorageConfig const& GetStorageConfig() const noexcept { return storage_; }

  /*!
   * \brief Getter for the access configuration
   * \return Access configuration
   */
  DaemonX509AccessConfig const& GetAccessConfig() const noexcept { return access_; }

 private:
  DaemonX509StorageConfig storage_; /*!< Storage configuration */
  DaemonX509AccessConfig access_;   /*!< Access configuration */
};

/*!
 * \brief Struct for the server config.
 * \vprivate Component Private
 */
class DaemonServerConfig final : public Config {
 public:
  using Config::Config;

  /*!
   * \brief Constructs the Daemon configuration struct.
   * \vprivate Component Private
   */
  DaemonServerConfig() : Config("", nullptr) {}

  /*!
   * \brief Deallocates the configuration class.
   * \vprivate Component Private
   */
  ~DaemonServerConfig() noexcept final = default;

  /*!
   * \brief Copy constructor.
   * \vprivate Component Private
   */
  DaemonServerConfig(DaemonServerConfig const& /*other*/) noexcept = default;

  /*!
   * \brief Copy assignment operator.
   * \return A reference to the object
   * \vprivate Component Private
   */
  DaemonServerConfig& operator=(DaemonServerConfig const& /*other*/) & noexcept = default;

  /*!
   * \brief Move constructor.
   * \vprivate Component Private
   */
  DaemonServerConfig(DaemonServerConfig&& /*other*/) noexcept = default;

  /*!
   * \brief Move assignment.
   * \return A reference to the object
   * \vprivate Component Private
   */
  DaemonServerConfig& operator=(DaemonServerConfig&& /*other*/) & noexcept = default;

  /*!
   * \brief Getter for the maximum number of client connections
   * \return maximum number of client connections
   */
  std::uint16_t GetMaxConnectionNum() const noexcept { return max_connection_num_; }

  /*! \brief Whether access control on keyslots is enforced */
  bool IsAccessControlEnabled() const noexcept { return key_access_control_; }

  /*! \brief Whether security events shall be reported to IDSM */
  bool IsIdsmReportingEnabled() const noexcept { return idsm_reporting_; }

  /*!
   * \copydoc config::Config::Deserialize
   * \vprivate Component Private
   */
  amsr::core::Result<void> Deserialize(crypto::common::data::DataContainer const& serial) noexcept final;

 private:
  std::uint8_t max_connection_num_{
      10};                         /*!< Maximum number of client connections which should be supported by the server */
  bool key_access_control_{false}; /*!< Whether access control on keyslots is enforced */
  bool idsm_reporting_{false};     /*!< Whether security events shall be reported to IDSM */
};

/*!
 * \brief Daemon configuration.
 * \vprivate Component Private
 */
class DaemonConfig final : public Config {
 public:
  /*!
   * \brief Constructs the daemon configuration.
   * \vprivate Component Private
   */
  DaemonConfig()
      : Config("", nullptr),
        x509_{crypto::common::data::kDaemonConfigX509, this},
        server_{crypto::common::data::kDaemonConfigServer, this} {}

  /*!
   * \brief Deallocates the configuration class.
   * \vprivate Component Private
   */
  ~DaemonConfig() noexcept final = default;

  /*!
   * \brief Copy constructor.
   * \vprivate Component Private
   */
  DaemonConfig(DaemonConfig const& /*other*/) noexcept = default;

  /*!
   * \brief Copy assignment operator.
   * \return A reference to the object
   * \vprivate Component Private
   */
  DaemonConfig& operator=(DaemonConfig const& /*other*/) & noexcept = default;

  /*!
   * \brief Move constructor.
   * \vprivate Component Private
   */
  DaemonConfig(DaemonConfig&& /*other*/) noexcept = default;

  /*!
   * \brief Move assignment.
   * \return A reference to the object
   * \vprivate Component Private
   */
  DaemonConfig& operator=(DaemonConfig&& /*other*/) & noexcept = default;

  /*!
   * \brief Getter for the x509 config
   * \return X.509 configuration
   */
  DaemonX509Config const& GetX509Config() const noexcept { return x509_; }

  /*!
   * \brief Getter for the server config
   * \return General server configuration
   */
  DaemonServerConfig const& GetServerConfig() const noexcept { return server_; }

  /*!
   * \copydoc config::Config::Deserialize
   * \vprivate Component Private
   */
  amsr::core::Result<void> Deserialize(crypto::common::data::DataContainer const& serial) noexcept final;

 private:
  DaemonX509Config x509_;     /*!< X.509 configuration */
  DaemonServerConfig server_; /*!< General server configuration */
};

}  // namespace config
}  // namespace server
}  // namespace crypto

#endif  // LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_CONFIG_DAEMON_CONFIG_H_
