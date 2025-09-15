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
/*!        \file  json_configuration.h
 *        \brief  This file contains definition for JsonConfiguration methods.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_JSON_CONFIGURATION_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_JSON_CONFIGURATION_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>
#include "ara/core/string.h"

#include "amsr/someip_daemon_core/configuration/configuration.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {

/*!
 * \brief Decouples the internal configuration data model from the JSON functionality
 *        by deriving from Configuration base class.
 *
 * \trace SPEC-5951483
 */
class JsonConfiguration final : public Configuration {
 public:
  /*!
   * \brief Constructor reads in the JSON configurations and transfers the information
   *        contained into the internal data model stored in the attributes of the base object
   *        of type Configuration.
   *
   * \param[in] cfg_path A path to the configuration file "someipd-posix.json".
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  explicit JsonConfiguration(ara::core::String cfg_path) noexcept : Configuration(), cfg_path_{std::move(cfg_path)} {
    ReadJsonConfigurations();
  }

  /*!
   * \brief Delete default constructor.
   */
  JsonConfiguration() = delete;

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~JsonConfiguration() final = default;

  /*!
   * \brief Deleted copy constructor.
   */
  JsonConfiguration(JsonConfiguration const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  JsonConfiguration(JsonConfiguration&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(JsonConfiguration const&) -> JsonConfiguration& = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(JsonConfiguration&&) -> JsonConfiguration& = delete;

 private:
  /*!
   * \brief Parses a "someip_config.json" file.
   * \details If none of the IPC channels domain and port parsed from the Json match the domain and port in the
              hand-written Json, the daemon will terminate.
   * \param[in] file_path Path to the "someip_config.json" file to be parsed.
   * \return true if json file parsing is successful, false otherwise.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  auto ParseApplicationGenConfigJson(ara::core::String const& file_path) noexcept -> bool;

  /*!
   * \brief Parses JSON configuration files.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void ReadJsonConfigurations() noexcept;

  /*!
   * \brief A path to the configuration file someipd-posix.json that contains IPC address settings and the paths to the
   *        application configurations.
   */
  ara::core::String cfg_path_;
};

}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_JSON_CONFIGURATION_H_
