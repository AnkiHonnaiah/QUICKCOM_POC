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
/*!        \file
 *        \brief  JSON parser of services runtime configurations.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_CONFIG_PARSER_SERVICES_JSON_PARSER_H_
#define LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_CONFIG_PARSER_SERVICES_JSON_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/json/reader/v2/single_array_parser.h"
#include "amsr/json/reader/v2/single_object_parser.h"

#include "ipc_binding/internal/config/parser/json_parser_util.h"
#include "ipc_binding_core/internal/runtime_configuration/event_config.h"
#include "ipc_binding_core/internal/runtime_configuration/field_config.h"
#include "ipc_binding_core/internal/runtime_configuration/method_config.h"
#include "ipc_binding_core/internal/runtime_configuration/runtime_config.h"

namespace amsr {
namespace ipc_binding {
namespace internal {
namespace config {
namespace parser {

/*!
 * \brief Parser for the services element inside the IpcBinding configuration.
 *
 * \steady FALSE
 *
 * \unit IpcBinding::RuntimeConfigJsonParser
 */
class ServicesJsonParser final : public ::amsr::json::v2::SingleArrayParser {
 public:
  /*!
   * \brief Name alias for amsr::ipc_binding_core::internal::runtime_configuration::RuntimeConfig.
   */
  using RuntimeConfig = ::amsr::ipc_binding_core::internal::runtime_configuration::RuntimeConfig;

  /*!
   * \brief Constructor for ServicesJsonParser.
   *
   * \param[in,out] json    The loaded JSON data from the configuration file.
   * \param[in,out] config  The object holding the parsed configuration.
   *
   * \pre     -
   * \context Init
   * \steady  FALSE
   */
  ServicesJsonParser(JsonData& json, RuntimeConfig& config) noexcept;

  /*!
   * \brief Default destructor.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~ServicesJsonParser() noexcept final = default;  // VCA_IPCB_MOLE_1298

  ServicesJsonParser(ServicesJsonParser const&) noexcept = delete;
  auto operator=(ServicesJsonParser const&) noexcept -> ServicesJsonParser& = delete;
  ServicesJsonParser(ServicesJsonParser&&) noexcept = delete;
  auto operator=(ServicesJsonParser&&) noexcept -> ServicesJsonParser& = delete;

 protected:
  /*!
   * \brief Callback on a JSON array element encounter from the parsed JSON object.
   *
   * \return Parser result.
   *
   * \error IpcBindingErrc::json_parsing_failure  If JSON parsing failed.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto OnElement() noexcept -> ParserResult final;

 private:
  /*!
   * \brief Parser for a single service interface configuration.
   *
   * \steady FALSE
   */
  // VCA_IPCB_CLASS_DECLARATION
  class ServiceJsonParser final : public ::amsr::json::v2::SingleObjectParser {
   public:
    /*!  Name alias for amsr::ipc_binding_core::internal::runtime_configuration::ServiceConfig. */
    using ServiceConfig = ::amsr::ipc_binding_core::internal::runtime_configuration::ServiceConfig;
    /*!  Name alias for amsr::ipc_binding_core::internal::runtime_configuration::EventConfig. */
    using EventConfig = ::amsr::ipc_binding_core::internal::runtime_configuration::EventConfig;
    /*!  Name alias for amsr::ipc_binding_core::internal::runtime_configuration::FieldConfig. */
    using FieldConfig = ::amsr::ipc_binding_core::internal::runtime_configuration::FieldConfig;
    /*!  Name alias for amsr::ipc_binding_core::internal::runtime_configuration::MethodConfig. */
    using MethodConfig = ::amsr::ipc_binding_core::internal::runtime_configuration::MethodConfig;
    /*!  Name alias for amsr::ipc_binding_core::internal::runtime_configuration::ProvidedServiceInstanceConfig. */
    using ProvidedServiceInstanceConfig =
        ::amsr::ipc_binding_core::internal::runtime_configuration::ProvidedServiceInstanceConfig;
    /*!  Name alias for amsr::ipc_binding_core::internal::runtime_configuration::RequiredServiceInstanceConfig. */
    using RequiredServiceInstanceConfig =
        ::amsr::ipc_binding_core::internal::runtime_configuration::RequiredServiceInstanceConfig;

    /*!
     * \brief Constructor for ServiceJsonParser.
     *
     * \param[in,out] json    The loaded JSON data from the configuration file.
     * \param[in,out] config  Reference to the service interface config.
     *
     * \pre     -
     * \context Init
     * \steady  FALSE
     */
    ServiceJsonParser(JsonData& json, ServiceConfig& config) noexcept;

   protected:
    /*!
     * \brief Callback on a new key parsed from the JSON object.
     *
     * \param[in] json_key  Parsed key string.
     *
     * \return Parser result.
     *
     * \error IpcBindingErrc::json_parsing_failure  If JSON parsing failed.
     *
     * \pre         -
     * \context     Init
     * \threadsafe  FALSE
     * \reentrant   FALSE
     * \synchronous TRUE
     * \steady      FALSE
     */
    auto OnKey(StringView json_key) noexcept -> ParserResult final;

    /*!
     * \brief Callback to validate that all configuration elements have been accepted.
     *
     * \return Either nothing or the error.
     *
     * \error IpcBindingErrc::json_parsing_failure  If validation fails.
     *
     * \pre         -
     * \context     Init
     * \threadsafe  FALSE
     * \reentrant   FALSE
     * \synchronous TRUE
     * \steady      FALSE
     */
    auto Finalize() noexcept -> ara::core::Result<void> final;

   private:
    /*! JSON key for service identifier. */
    static constexpr StringView kServiceIdentifierKey{"service_id"_sv};
    /*! JSON key for shortname path. */
    static constexpr StringView kShortnamePathKey{"shortname_path"_sv};
    /*! JSON key for major version. */
    static constexpr StringView kMajorVersionKey{"major_version"_sv};
    /*! JSON key for minor version. */
    static constexpr StringView kMinorVersionKey{"minor_version"_sv};
    /*! JSON key for is generic. */
    static constexpr StringView kIsGenericKey{"is_generic"_sv};
    /*! JSON key for event deployments. */
    static constexpr StringView kEventsKey{"events"_sv};
    /*! JSON key for method deployments. */
    static constexpr StringView kMethodsKey{"methods"_sv};
    /*! JSON key for field deployments. */
    static constexpr StringView kFieldsKey{"fields"_sv};
    /*! JSON key for required service instance deployments. */
    static constexpr StringView kRequiredServiceInstancesKey{"required_service_instances"_sv};
    /*! JSON key for provided service instance deployments. */
    static constexpr StringView kProvidedServiceInstancesKey{"provided_service_instances"_sv};

    /*!
     * \brief Callback to parse the value for 'service_id' key.
     *
     * \return A result to indicate whether parsing should continue.
     *
     * \error IpcBindingErrc::json_parsing_failure  If parsing the value fails.
     *
     * \pre         -
     * \context     Init
     * \threadsafe  FALSE
     * \reentrant   FALSE
     * \synchronous TRUE
     * \steady      FALSE
     */
    auto OnServiceIdentifier() noexcept -> ParserResult;

    /*!
     * \brief Callback to parse the value for 'shortname_path' key.
     *
     * \return A result to indicate whether parsing should continue.
     *
     * \error IpcBindingErrc::json_parsing_failure  If parsing the value fails.
     *
     * \pre         -
     * \context     Init
     * \threadsafe  FALSE
     * \reentrant   FALSE
     * \synchronous TRUE
     * \steady      FALSE
     */
    auto OnShortnamePath() noexcept -> ParserResult;

    /*!
     * \brief Callback to parse the value for 'major_version' key.
     *
     * \return A result to indicate whether parsing should continue.
     *
     * \error IpcBindingErrc::json_parsing_failure  If parsing the value fails.
     *
     * \pre         -
     * \context     Init
     * \threadsafe  FALSE
     * \reentrant   FALSE
     * \synchronous TRUE
     * \steady      FALSE
     */
    auto OnMajorVersion() noexcept -> ParserResult;

    /*!
     * \brief Callback to parse the value for 'minor_version' key.
     *
     * \return A result to indicate whether parsing should continue.
     *
     * \error IpcBindingErrc::json_parsing_failure  If parsing the value fails.
     *
     * \pre         -
     * \context     Init
     * \threadsafe  FALSE
     * \reentrant   FALSE
     * \synchronous TRUE
     * \steady      FALSE
     */
    auto OnMinorVersion() noexcept -> ParserResult;

    /*!
     * \brief Callback to parse the value for 'is_generic' key.
     *
     * \return A result to indicate whether parsing should continue.
     *
     * \error IpcBindingErrc::json_parsing_failure  If parsing the value fails.
     *
     * \pre         -
     * \context     Init
     * \threadsafe  FALSE
     * \reentrant   FALSE
     * \synchronous TRUE
     */
    auto OnIsGeneric() noexcept -> ParserResult;

    /*!
     * \brief Callback to parse the array of 'events' key.
     *
     * \return A result to indicate whether parsing should continue.
     *
     * \error IpcBindingErrc::json_parsing_failure  If parsing the value fails.
     *
     * \pre         -
     * \context     Init
     * \threadsafe  FALSE
     * \reentrant   FALSE
     * \synchronous TRUE
     * \steady      FALSE
     */
    auto ParseEvents() noexcept -> ParserResult;

    /*!
     * \brief Callback to parse the value for one 'event' object.
     *
     * \param[out] event_config  Reference to the event object.
     *
     * \return A result to indicate whether parsing should continue.
     *
     * \error IpcBindingErrc::json_parsing_failure  If parsing the value fails.
     *
     * \pre         -
     * \context     Init
     * \threadsafe  FALSE
     * \reentrant   FALSE
     * \synchronous TRUE
     * \steady      FALSE
     */
    auto ParseEventObject(EventConfig& event_config) noexcept -> ara::core::Result<void>;

    /*!
     * \brief Callback to parse the array of 'methods' key.
     *
     * \return A result to indicate whether parsing should continue.
     *
     * \error IpcBindingErrc::json_parsing_failure  If parsing the value fails.
     *
     * \pre         -
     * \context     Init
     * \threadsafe  FALSE
     * \reentrant   FALSE
     * \synchronous TRUE
     * \steady      FALSE
     */
    auto ParseMethods() noexcept -> ParserResult;

    /*!
     * \brief Callback to parse the value for one 'method' object.
     *
     * \param[out] method_config  Reference to the method object.
     *
     * \return A result to indicate whether parsing should continue.
     *
     * \error IpcBindingErrc::json_parsing_failure  If parsing the value fails.
     *
     * \pre         -
     * \context     Init
     * \threadsafe  FALSE
     * \reentrant   FALSE
     * \synchronous TRUE
     * \steady      FALSE
     */
    auto ParseMethodObject(MethodConfig& method_config) noexcept -> ara::core::Result<void>;

    /*!
     * \brief Callback to parse the array of 'fields' key.
     *
     * \return A result to indicate whether parsing should continue.
     *
     * \error IpcBindingErrc::json_parsing_failure  If parsing the value fails.
     *
     * \pre         -
     * \context     Init
     * \threadsafe  FALSE
     * \reentrant   FALSE
     * \synchronous TRUE
     * \steady      FALSE
     */
    auto ParseFields() noexcept -> ParserResult;

    /*!
     * \brief Callback to parse the value for one 'field' object.
     *
     * \param[out] field_config  Reference to the field object.
     *
     * \return A result to indicate whether parsing should continue.
     *
     * \error IpcBindingErrc::json_parsing_failure  If parsing the value fails.
     *
     * \pre         -
     * \context     Init
     * \threadsafe  FALSE
     * \reentrant   FALSE
     * \synchronous TRUE
     * \steady      FALSE
     */
    auto ParseFieldObject(FieldConfig& field_config) noexcept -> ara::core::Result<void>;

    /*!
     * \brief Callback to parse the array of 'required_service_instances' key.
     *
     * \return A result to indicate whether parsing should continue.
     *
     * \error IpcBindingErrc::json_parsing_failure  If parsing the value fails.
     *
     * \pre         -
     * \context     Init
     * \threadsafe  FALSE
     * \reentrant   FALSE
     * \synchronous TRUE
     * \steady      FALSE
     */
    auto ParseRequiredServiceInstances() noexcept -> ParserResult;

    /*!
     * \brief Callback to parse the value for one 'required_service' object.
     *
     * \param[out] required_service_config  Reference to the required service object.
     *
     * \return A result to indicate whether parsing should continue.
     *
     * \error IpcBindingErrc::json_parsing_failure  If parsing the value fails.
     *
     * \pre         -
     * \context     Init
     * \threadsafe  FALSE
     * \reentrant   FALSE
     * \synchronous TRUE
     * \steady      FALSE
     */
    auto ParseRequiredServiceInstanceObject(RequiredServiceInstanceConfig& required_service_config) noexcept
        -> ara::core::Result<void>;

    /*!
     * \brief Callback to parse the array of 'provided_service_instances' key.
     *
     * \return A result to indicate whether parsing should continue.
     *
     * \error IpcBindingErrc::json_parsing_failure  If parsing the value fails.
     *
     * \pre         -
     * \context     Init
     * \threadsafe  FALSE
     * \reentrant   FALSE
     * \synchronous TRUE
     * \steady      FALSE
     */
    auto ParseProvidedServiceInstances() noexcept -> ParserResult;

    /*!
     * \brief Callback to parse the value for one 'provided_service' object.
     *
     * \param[out] provided_service_config  Reference to the provided service object.
     *
     * \return A result to indicate whether parsing should continue.
     *
     * \error IpcBindingErrc::json_parsing_failure  If parsing the value fails.
     *
     * \pre         -
     * \context     Init
     * \threadsafe  FALSE
     * \reentrant   FALSE
     * \synchronous TRUE
     * \steady      FALSE
     */
    auto ParseProvidedServiceInstanceObject(ProvidedServiceInstanceConfig& provided_service_config) noexcept
        -> ara::core::Result<void>;

    std::size_t number_of_service_ids_found_{0};      /*!< Number of key 'service_id' found.*/
    std::size_t number_of_shortname_path_found_{0};   /*!< Number of key 'shortname_path' found.*/
    std::size_t number_of_minor_version_found_{0};    /*!< Number of key 'minor_version' found.*/
    std::size_t number_of_is_generic_found_{0};       /*!< Number of key 'is_generic' found.*/
    std::size_t number_of_major_version_found_{0};    /*!< Number of key 'major_version' found.*/
    std::size_t number_of_events_found_{0};           /*!< Number of key 'events' found.*/
    std::size_t number_of_methods_found_{0};          /*!< Number of key 'methods' found.*/
    std::size_t number_of_fields_found_{0};           /*!< Number of key 'fields' found.*/
    std::size_t number_of_required_service_found_{0}; /*!< Number of key 'required_service_instances' found.*/
    std::size_t number_of_provided_service_found_{0}; /*!< Number of key 'provided_service_instances' found.*/

    /*! Reference to the service interface config. */
    ServiceConfig& config_;
  };

  /*! Reference for the RuntimeConfig configuration object. */
  RuntimeConfig& runtime_config_;
};

}  // namespace parser
}  // namespace config
}  // namespace internal
}  // namespace ipc_binding
}  // namespace amsr

#endif  // LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_CONFIG_PARSER_SERVICES_JSON_PARSER_H_
