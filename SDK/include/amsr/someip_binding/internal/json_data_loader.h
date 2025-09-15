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
/*!        \file  someip_binding/internal/json_data_loader.h
 *        \brief  Loads JSON data from a JSON configuration file.
 *        \unit   SomeIpBinding::SomeIpBinding::Configuration::JsonDataLoader
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_JSON_DATA_LOADER_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_JSON_DATA_LOADER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "amsr/iointegritystream/configuration_stream.h"
#include "amsr/someip_binding/internal/configuration/parsing/someipbinding_config_json_parser.h"
#include "amsr/someip_binding/internal/configuration/someipbinding_config.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"

namespace amsr {
namespace someip_binding {
namespace internal {

// VECTOR NC AutosarC++17_10-M7.3.4: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.4_using_directive
// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.6_using_directive
using namespace vac::container::literals;  // NOLINT(build/namespaces)

/*!
 * \brief Configuration provider.
 */
class JsonDataLoader final {
 public:
  /*!
   * \brief Use default constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  JsonDataLoader() noexcept;

  /*!
   * \brief Use default destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  ~JsonDataLoader() noexcept = default;

  JsonDataLoader(JsonDataLoader const&) = delete;
  JsonDataLoader(JsonDataLoader&&) = delete;
  JsonDataLoader& operator=(JsonDataLoader const&) & = delete;
  JsonDataLoader& operator=(JsonDataLoader&&) & = delete;

  /*!
   * \brief       Get the configuration for the someipbinding.
   * \param[in]   runtime_config_json_path  Path to runtime configuration JSON file.
   * \return      Result stores the configuration or an error.
   * \error       amsr::someip_binding_core::internal::SomeIpBindingErrc::json_loading_failure
   *                if component specific JSON configuration could not be found or loaded.
   *
   * \pre         Environment variable "AMSR_PROCESS_SHORT_NAME_PATH" must be set to Process short name path.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   *
   * \internal
   * - Open and verify validation file.
   * - If open / verification failed:
   *   - Log error message and exit.
   * - Otherwise:
   *   - Set the loaded config json data as result.
   * - Return the result.
   * \endinternal
   */
  amsr::core::Result<amsr::json::JsonData> GetJsonData(
      ::amsr::core::StringView const runtime_config_json_path) noexcept;

 private:
  /*!
   * \brief Result to store Integrity file stream.
   */
  ::amsr::core::Result<::amsr::iointegritystream::ConfigurationStream> integrity_file_stream_;

  /*!
   * \brief logger.
   */
  ::amsr::someip_binding_core::internal::logging::AraComLogger const logger_;
};

}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_JSON_DATA_LOADER_H_
