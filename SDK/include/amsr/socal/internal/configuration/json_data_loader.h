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
/*!        \file  socal/internal/configuration/json_data_loader.h
 *        \brief  Loads JSON data from a JSON configuration file.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_CONFIGURATION_JSON_DATA_LOADER_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_CONFIGURATION_JSON_DATA_LOADER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "amsr/iointegritystream/configuration_stream.h"
#include "amsr/json/reader/json_data.h"
#include "amsr/socal/internal/logging/ara_com_logger.h"

namespace amsr {
namespace socal {
namespace internal {
namespace configuration {

// VECTOR NC AutosarC++17_10-M7.3.4: MD_SOCAL_AutosarC++17_10-M7.3.4_usingDirectivesShallNotBeUsed
// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOCAL_AutosarC++17_10-M7.3.6_usingDirectiveInHeaderFile
using vac::container::literals::operator""_sv;  // NOLINT(build/namespaces)

/*!
 * \brief Loads JSON data from a JSON configuration file.
 *
 * \complexity Multiple inter module calls to iointegritystream and libiostream are required for loading the json file
 *             and performing integrity checks. Metric is slightly over the threshold, no risk identified.
 * \unit       Socal::Configuration::JsonDataLoader
 */
class JsonDataLoader final {
 public:
  /*!
   * \brief Path to the component specific configuration file.
   */
  static constexpr ::amsr::core::StringView kComponentSpecificConfigurationFile{"./etc/socal_config.json"_sv};

  /*!
   * \brief Default constructor.
   * \steady FALSE
   * \spec requires true; \endspec
   */
  JsonDataLoader() noexcept = default;

  /*!
   * \brief Use default destructor.
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ~JsonDataLoader() noexcept = default;  // VCA_SOCAL_COMPILER_GENERATED_DESTRUCTOR

  JsonDataLoader(JsonDataLoader const&) noexcept = delete;
  JsonDataLoader(JsonDataLoader&&) noexcept = delete;
  JsonDataLoader& operator=(JsonDataLoader const&) & noexcept = delete;
  JsonDataLoader& operator=(JsonDataLoader&&) & noexcept = delete;

  /*!
   * \brief Get the configuration for socal.
   * \return JSON data representation of the loaded config.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_Method_can_be_declared_static
  ::amsr::json::JsonData GetJsonData() noexcept;

 private:
  /*!
   * \brief Perform the integrity check and get the JSON document.
   * \param[in] config_json_path  Path to component specific configuration file.
   * \return JSON data representation of the loaded config.
   * \pre     Environment variable "AMSR_PROCESS_SHORT_NAME_PATH" must be set to Process short name path.
   * \context Init
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ::amsr::json::JsonData ValidateAndGetJsonData(::amsr::core::StringView const config_json_path) noexcept;

  /*!
   * \brief logger.
   */
  ::amsr::socal::internal::logging::AraComLogger const logger_{
      ::amsr::socal::internal::logging::kAraComLoggerContextId,
      ::amsr::socal::internal::logging::kAraComLoggerContextDescription, "JsonDataLoader"_sv};

  /*!
   * \brief Result to store Integrity file stream.
   */
  ::amsr::core::Result<::amsr::iointegritystream::ConfigurationStream> integrity_file_stream_{
      ::amsr::json::JsonErrc::kInvalidJson};
};

}  // namespace configuration
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_CONFIGURATION_JSON_DATA_LOADER_H_
