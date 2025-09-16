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
 *         \brief
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_COMMON_INCLUDE_AMSR_ZERO_COPY_BINDING_COMMON_INTERNAL_CONFIG_ZERO_COPY_BINDING_CONFIG_JSON_PARSER_H_
#define LIB_ZERO_COPY_BINDING_COMMON_INCLUDE_AMSR_ZERO_COPY_BINDING_COMMON_INTERNAL_CONFIG_ZERO_COPY_BINDING_CONFIG_JSON_PARSER_H_

// VECTOR NL AutosarC++17_10-A16.0.1: MD_ZEROCOPYBINDING_A16.0.1_includeBetaDefinition
#ifdef AMSR_LG_BETA_ENABLED

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/abort.h"
#include "amsr/core/error_code.h"
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "amsr/generic/generic_error_domain.h"
#include "amsr/iointegritystream/configuration_stream.h"
#include "amsr/iostream/input_stream.h"
#include "amsr/json/reader.h"
#include "amsr/json/reader/parser_state.h"
// VECTOR NL AutosarC++17_10-A16.0.1: MD_ZEROCOPYBINDING_A16.0.1_conditionalCompilation
#ifdef AMSR_ZEROCOPYBINDING_ENABLE_TRACE_API
#include "amsr/zero_copy_binding/common/internal/config/config_builder.h"
#endif  // AMSR_ZEROCOPYBINDING_ENABLE_TRACE_API
#include "amsr/zero_copy_binding/common/internal/config/zero_copy_binding_config.h"
#include "ara/core/string.h"
#include "vac/language/switch.h"

namespace amsr {
namespace zero_copy_binding {
namespace common {
namespace internal {

namespace config {

auto StringToIntegrityLevel(::amsr::core::StringView level) noexcept
    -> ::amsr::core::Result<::amsr::ipc::IntegrityLevel> {
  using IntegrityLevel = ::amsr::ipc::IntegrityLevel;
  using IntegrityLevelResult = ::amsr::core::Result<IntegrityLevel>;

  return ::vac::language::Switch<IntegrityLevelResult>(level)
      .Case("kQm", []() { return IntegrityLevelResult::FromValue(IntegrityLevel::kQm); })
      .Case("kAsilA", []() { return IntegrityLevelResult::FromValue(IntegrityLevel::kAsilA); })
      .Case("kAsilB", []() { return IntegrityLevelResult::FromValue(IntegrityLevel::kAsilB); })
      .Case("kAsilC", []() { return IntegrityLevelResult::FromValue(IntegrityLevel::kAsilC); })
      .Case("kAsilD", []() { return IntegrityLevelResult::FromValue(IntegrityLevel::kAsilD); })
      .Default(IntegrityLevelResult::FromError(::amsr::generic::GenErrc::kSystemConfigurationInvalid,
                                               "Error converting integrity level string."));
}

auto StringToInitialization(::amsr::core::StringView initialization) noexcept
    -> ::amsr::core::Result<common::internal::config::Initialization::Type> {
  using Initialization = common::internal::config::Initialization;
  using InitializationResult = ::amsr::core::Result<common::internal::config::Initialization::Type>;

  return ::vac::language::Switch<InitializationResult>(initialization)
      .Case("NoInitialization",
            []() { return InitializationResult::FromValue(Initialization::Type::kNoInitialization); })
      .Case("ZeroInitialization",
            []() { return InitializationResult::FromValue(Initialization::Type::kZeroInitialization); })
      .Case("ConstructorInitialization",
            []() { return InitializationResult::FromValue(Initialization::Type::kConstructorInitialization); })
      .Default(InitializationResult::FromError(::amsr::generic::GenErrc::kSystemConfigurationInvalid,
                                               "Error converting initialization string."));
}

auto StringToMemoryTechnology(::amsr::core::StringView memory_technology) noexcept
    -> ::amsr::core::Result<common::internal::config::MemoryTechnology::Type> {
  using MemoryTechnology = common::internal::config::MemoryTechnology;
  using MemoryTechnologyResult = ::amsr::core::Result<common::internal::config::MemoryTechnology::Type>;

  return ::vac::language::Switch<MemoryTechnologyResult>(memory_technology)
      .Case("SharedMemory", []() { return MemoryTechnologyResult::FromValue(MemoryTechnology::Type::kSharedMemory); })
      .Case("CustomMemory", []() { return MemoryTechnologyResult::FromValue(MemoryTechnology::Type::kCustomMemory); })
      .Default(MemoryTechnologyResult::FromError(::amsr::generic::GenErrc::kSystemConfigurationInvalid,
                                                 "Error converting memory technology string."));
}

class ConnectionEndpointParser final : public ::amsr::json::v2::SingleObjectParser {
 public:
  using ParserResult = ::amsr::json::ParserResult;
  using Configuration = common::internal::config::ConnectionEndpoint;

  ConnectionEndpointParser(::amsr::json::JsonData& json, Configuration& config) noexcept
      : ::amsr::json::v2::SingleObjectParser{json}, config_{config} {}

  ConnectionEndpointParser(ConnectionEndpointParser const&) noexcept = delete;
  ConnectionEndpointParser& operator=(ConnectionEndpointParser const&) & = delete;
  ConnectionEndpointParser(ConnectionEndpointParser&&) noexcept = delete;
  ConnectionEndpointParser& operator=(ConnectionEndpointParser&&) & = delete;

  ~ConnectionEndpointParser() noexcept final = default;

  ParserResult OnKey(::amsr::core::StringView json_key) noexcept final {
    return vac::language::Switch<::amsr::json::ParserResult>(json_key)
        .Case("domain", [this]() { return ParseDomain(); })
        .Case("port", [this]() { return ParsePort(); })
        .Default(ParserResult::FromError(::amsr::generic::GenErrc::kSystemConfigurationInvalid,
                                         "Unknown or unexpected JSON key found."));
  }

 private:
  ParserResult ParseDomain() noexcept {
    return Number<common::internal::config::ConnectionEndpoint::Type>(
        [&config = config_](auto json_value) { config.domain = json_value; });
  }

  ParserResult ParsePort() noexcept {
    return Number<common::internal::config::ConnectionEndpoint::Type>(
        [&config = config_](auto json_value) { config.port = json_value; });
  }

  Configuration& config_;
};

class SingleResourceLimitParser final : public ::amsr::json::v2::SingleObjectParser {
 public:
  using ParserResult = ::amsr::json::ParserResult;
  using Configuration = common::internal::config::SingleResourceLimit;

  SingleResourceLimitParser(::amsr::json::JsonData& json, Configuration& config) noexcept
      : ::amsr::json::v2::SingleObjectParser{json}, config_{config} {}

  SingleResourceLimitParser(SingleResourceLimitParser const&) noexcept = delete;
  SingleResourceLimitParser& operator=(SingleResourceLimitParser const&) & = delete;
  SingleResourceLimitParser(SingleResourceLimitParser&&) noexcept = delete;
  SingleResourceLimitParser& operator=(SingleResourceLimitParser&&) & = delete;

  ~SingleResourceLimitParser() noexcept final = default;

  ParserResult OnKey(::amsr::core::StringView json_key) noexcept final {
    return vac::language::Switch<::amsr::json::ParserResult>(json_key)
        .Case("max_slots", [this]() { return ParseMaxSlots(); })
        .Case("max_connections", [this]() { return ParseMaxConnections(); })
        .Default(ParserResult::FromError(::amsr::generic::GenErrc::kSystemConfigurationInvalid,
                                         "Unknown or unexpected JSON key found."));
  }

 private:
  ParserResult ParseMaxSlots() noexcept {
    return Number<common::internal::config::SingleResourceLimit::Type>(
        [&config = config_](auto json_value) { config.max_slots = json_value; });
  }

  ParserResult ParseMaxConnections() noexcept {
    return Number<common::internal::config::SingleResourceLimit::Type>(
        [&config = config_](auto json_value) { config.max_connections = json_value; });
  }

  Configuration& config_;
};

class ResourceLimitParser final : public ::amsr::json::v2::SingleObjectParser {
 public:
  using ParserResult = ::amsr::json::ParserResult;
  using Configuration = common::internal::config::ResourceLimits;

  ResourceLimitParser(::amsr::json::JsonData& json, Configuration& config) noexcept
      : ::amsr::json::v2::SingleObjectParser{json}, config_{config} {}

  ResourceLimitParser(ResourceLimitParser const&) noexcept = delete;
  ResourceLimitParser& operator=(ResourceLimitParser const&) & = delete;
  ResourceLimitParser(ResourceLimitParser&&) noexcept = delete;
  ResourceLimitParser& operator=(ResourceLimitParser&&) & = delete;

  ~ResourceLimitParser() noexcept final = default;

  ParserResult OnKey(::amsr::core::StringView json_key) noexcept final {
    return vac::language::Switch<::amsr::json::ParserResult>(json_key)
        .Case("qm", [this]() { return ParseResourceLimitQm(); })
        .Case("asil_a", [this]() { return ParseResourceLimitAsilA(); })
        .Case("asil_b", [this]() { return ParseResourceLimitAsilB(); })
        .Case("asil_c", [this]() { return ParseResourceLimitAsilC(); })
        .Case("asil_d", [this]() { return ParseResourceLimitAsilD(); })
        .Default(ParserResult::FromError(::amsr::generic::GenErrc::kSystemConfigurationInvalid,
                                         "Unknown or unexpected JSON key found."));
  }

 private:
  ParserResult ParseResourceLimitQm() noexcept {
    SingleResourceLimitParser parser{this->GetJsonDocument(), this->config_.qm};
    return parser.SubParse();
  }

  ParserResult ParseResourceLimitAsilA() noexcept {
    SingleResourceLimitParser parser{this->GetJsonDocument(), this->config_.asil_a};
    return parser.SubParse();
  }

  ParserResult ParseResourceLimitAsilB() noexcept {
    SingleResourceLimitParser parser{this->GetJsonDocument(), this->config_.asil_b};
    return parser.SubParse();
  }

  ParserResult ParseResourceLimitAsilC() noexcept {
    SingleResourceLimitParser parser{this->GetJsonDocument(), this->config_.asil_c};
    return parser.SubParse();
  }

  ParserResult ParseResourceLimitAsilD() noexcept {
    SingleResourceLimitParser parser{this->GetJsonDocument(), this->config_.asil_d};
    return parser.SubParse();
  }

  Configuration& config_;
};

class PvInstanceSpecificEventConfigParser final : public ::amsr::json::v2::SingleObjectParser {
 public:
  using ParserResult = ::amsr::json::ParserResult;
  using Configuration = common::internal::config::ProvidedInstanceSpecificEventConfig;

  PvInstanceSpecificEventConfigParser(::amsr::json::JsonData& json, Configuration& config) noexcept
      : ::amsr::json::v2::SingleObjectParser{json}, config_{config} {}

  PvInstanceSpecificEventConfigParser(PvInstanceSpecificEventConfigParser const&) noexcept = delete;
  PvInstanceSpecificEventConfigParser& operator=(PvInstanceSpecificEventConfigParser const&) & = delete;
  PvInstanceSpecificEventConfigParser(PvInstanceSpecificEventConfigParser&&) noexcept = delete;
  PvInstanceSpecificEventConfigParser& operator=(PvInstanceSpecificEventConfigParser&&) & = delete;

  ~PvInstanceSpecificEventConfigParser() noexcept final = default;

  ParserResult OnKey(::amsr::core::StringView json_key) noexcept final {
    return vac::language::Switch<::amsr::json::ParserResult>(json_key)
        .Case("event_id", [this]() { return ParseEventId(); })
        .Case("num_connections", [this]() { return ParseNumConnections(); })
        .Case("initialization", [this]() { return ParseInitialization(); })
        .Case("allocation_memory_technology", [this]() { return ParseAllocationMemoryTechnology(); })
        .Case("resource_limits", [this]() { return ParseResourceLimits(); })
        .Default(ParserResult::FromError(::amsr::generic::GenErrc::kSystemConfigurationInvalid,
                                         "Unknown or unexpected JSON key found."));
  }

 private:
  ParserResult ParseEventId() noexcept {
    return Number<common::internal::config::EventId::Type>(
        [&config = config_](auto json_value) { config.event_id.value = json_value; });
  }

  ParserResult ParseNumConnections() noexcept {
    return Number<common::internal::config::NumConnections::Type>(
        [&config = config_](auto json_value) { config.num_connections.value = json_value; });
  }

  ParserResult ParseInitialization() noexcept {
    return String([&config = config_](::amsr::core::StringView json_value) {
      auto initialization_result(StringToInitialization(json_value));
      if (!initialization_result.HasValue()) {
        ::amsr::core::Abort("Error when converting string to initialization.");
      } else {
        config.initialization.value = initialization_result.Value();
      }
    });
  }

  ParserResult ParseAllocationMemoryTechnology() noexcept {
    return String([&config = config_](::amsr::core::StringView json_value) {
      auto memory_technology_result(StringToMemoryTechnology(json_value));
      if (!memory_technology_result.HasValue()) {
        ::amsr::core::Abort("Error when converting string to memory technology.");
      } else {
        config.allocation_memory_technology.value = memory_technology_result.Value();
      }
    });
  }

  ParserResult ParseResourceLimits() noexcept {
    ResourceLimitParser parser{this->GetJsonDocument(), this->config_.resource_limits};
    return parser.SubParse();
  }

  Configuration& config_;
};

class ProvidedServiceInstanceParser final : public ::amsr::json::v2::SingleObjectParser {
 public:
  using ParserResult = ::amsr::json::ParserResult;
  using Configuration = common::internal::config::ProvidedServiceInstance;

  ProvidedServiceInstanceParser(::amsr::json::JsonData& json, Configuration& config) noexcept
      : ::amsr::json::v2::SingleObjectParser{json}, config_{config} {}

  ProvidedServiceInstanceParser(ProvidedServiceInstanceParser const&) noexcept = delete;
  ProvidedServiceInstanceParser& operator=(ProvidedServiceInstanceParser const&) & = delete;
  ProvidedServiceInstanceParser(ProvidedServiceInstanceParser&&) noexcept = delete;
  ProvidedServiceInstanceParser& operator=(ProvidedServiceInstanceParser&&) & = delete;

  ~ProvidedServiceInstanceParser() noexcept final = default;

  ParserResult OnKey(::amsr::core::StringView json_key) noexcept final {
    return vac::language::Switch<::amsr::json::ParserResult>(json_key)
        .Case("instance_id", [this]() { return ParseInstanceId(); })
        .Case("instance_specifier", [this]() { return ParseInstanceSpecifier(); })
        .Case("connection_endpoint", [this]() { return ParseConnectionEndpoint(); })
        .Case("pv_instance_specific_event_config", [this]() { return ParsePvInstanceSpecificEventConfig(); })
        .Default(ParserResult::FromError(::amsr::generic::GenErrc::kSystemConfigurationInvalid,
                                         "Unknown or unexpected JSON key found."));
  }

 private:
  ParserResult ParseInstanceId() noexcept {
    return Number<common::internal::config::InstanceId::Type>(
        [&config = config_](auto json_value) { config.instance_id.value = json_value; });
  }

  ParserResult ParseInstanceSpecifier() noexcept {
    return String([&config = config_](::amsr::core::StringView json_value) {
      if (json_value.empty()) {
        ::amsr::core::Abort("Invalid string while parsing instance specifier.");
      } else {
        config.instance_specifier.value = json_value.ToString();
      }
    });
  }

  ParserResult ParseConnectionEndpoint() noexcept {
    ConnectionEndpointParser parser{this->GetJsonDocument(), this->config_.connection_endpoint};
    return parser.SubParse();
  }

  ParserResult ParsePvInstanceSpecificEventConfig() noexcept {
    return Array([this](std::size_t) -> ::amsr::core::Result<void> {
      this->config_.pv_instance_specific_event_config.emplace_back(
          common::internal::config::ProvidedInstanceSpecificEventConfig{});

      PvInstanceSpecificEventConfigParser parser{this->GetJsonDocument(),
                                                 config_.pv_instance_specific_event_config.back()};
      return parser.Parse();
    });
  }

  Configuration& config_;
};

class EventParser final : public ::amsr::json::v2::SingleObjectParser {
 public:
  using ParserResult = ::amsr::json::ParserResult;
  using Configuration = common::internal::config::Event;

  EventParser(::amsr::json::JsonData& json, Configuration& config) noexcept
      : ::amsr::json::v2::SingleObjectParser{json}, config_{config} {}

  EventParser(EventParser const&) noexcept = delete;
  EventParser& operator=(EventParser const&) & = delete;
  EventParser(EventParser&&) noexcept = delete;
  EventParser& operator=(EventParser&&) & = delete;

  ~EventParser() noexcept final = default;

  ParserResult OnKey(::amsr::core::StringView json_key) noexcept final {
    return vac::language::Switch<::amsr::json::ParserResult>(json_key)
        .Case("event_id", [this]() { return ParseEventId(); })
        .Case("short_name", [this]() { return ParseShortName(); })
        .Case("trace_read_sample_enabled", [this]() { return ParseReadSampleEnabled(); })
        .Case("trace_send_allocate_enabled", [this]() { return ParseSendAllocateEnabled(); })
        .Default(ParserResult::FromError(::amsr::generic::GenErrc::kSystemConfigurationInvalid,
                                         "Unknown or unexpected JSON key found."));
  }

 private:
  ParserResult ParseEventId() noexcept {
    return Number<common::internal::config::EventId::Type>(
        [&config = config_](auto json_value) { config.event_id.value = json_value; });
  }

  ParserResult ParseShortName() noexcept {
    return String([&config = config_](::amsr::core::StringView json_value) {
      if (json_value.empty()) {
        ::amsr::core::Abort("Invalid string while parsing short name.");
      } else {
        config.short_name.value = json_value.ToString();
      }
    });
  }

  ParserResult ParseReadSampleEnabled() noexcept {
    return Bool([&config = config_](bool json_value) { config.trace_read_sample_enabled = json_value; });
  }

  ParserResult ParseSendAllocateEnabled() noexcept {
    return Bool([&config = config_](bool json_value) { config.trace_send_allocate_enabled = json_value; });
  }

  Configuration& config_;
};

class RqInstanceSpecificEventConfigParser final : public ::amsr::json::v2::SingleObjectParser {
 public:
  using ParserResult = ::amsr::json::ParserResult;
  using Configuration = common::internal::config::RequiredInstanceSpecificEventConfig;

  RqInstanceSpecificEventConfigParser(::amsr::json::JsonData& json, Configuration& config) noexcept
      : ::amsr::json::v2::SingleObjectParser{json}, config_{config} {}

  RqInstanceSpecificEventConfigParser(RqInstanceSpecificEventConfigParser const&) noexcept = delete;
  RqInstanceSpecificEventConfigParser& operator=(RqInstanceSpecificEventConfigParser const&) & = delete;
  RqInstanceSpecificEventConfigParser(RqInstanceSpecificEventConfigParser&&) noexcept = delete;
  RqInstanceSpecificEventConfigParser& operator=(RqInstanceSpecificEventConfigParser&&) & = delete;

  ~RqInstanceSpecificEventConfigParser() noexcept final = default;

  ParserResult OnKey(::amsr::core::StringView json_key) noexcept final {
    return vac::language::Switch<::amsr::json::ParserResult>(json_key)
        .Case("event_id", [this]() { return ParseEventId(); })
        .Case("allocation_memory_technology", [this]() { return ParseAllocationMemoryTechnology(); })
        .Default(ParserResult::FromError(::amsr::generic::GenErrc::kSystemConfigurationInvalid,
                                         "Unknown or unexpected JSON key found."));
  }

 private:
  ParserResult ParseEventId() noexcept {
    return Number<common::internal::config::EventId::Type>(
        [&config = config_](auto json_value) { config.event_id.value = json_value; });
  }

  ParserResult ParseAllocationMemoryTechnology() noexcept {
    return String([&config = config_](::amsr::core::StringView json_value) {
      auto memory_technology_result(StringToMemoryTechnology(json_value));
      if (!memory_technology_result.HasValue()) {
        ::amsr::core::Abort("Error when converting string to memory technology.");
      } else {
        config.allocation_memory_technology.value = memory_technology_result.Value();
      }
    });
  }

  Configuration& config_;
};

class RequiredServiceInstanceParser final : public ::amsr::json::v2::SingleObjectParser {
 public:
  using ParserResult = ::amsr::json::ParserResult;
  using Configuration = common::internal::config::RequiredServiceInstance;

  RequiredServiceInstanceParser(::amsr::json::JsonData& json, Configuration& config) noexcept
      : ::amsr::json::v2::SingleObjectParser{json}, config_{config} {}

  RequiredServiceInstanceParser(RequiredServiceInstanceParser const&) noexcept = delete;
  RequiredServiceInstanceParser& operator=(RequiredServiceInstanceParser const&) & = delete;
  RequiredServiceInstanceParser(RequiredServiceInstanceParser&&) noexcept = delete;
  RequiredServiceInstanceParser& operator=(RequiredServiceInstanceParser&&) & = delete;

  ~RequiredServiceInstanceParser() noexcept final = default;

  ParserResult OnKey(::amsr::core::StringView json_key) noexcept final {
    return vac::language::Switch<::amsr::json::ParserResult>(json_key)
        .Case("instance_id", [this]() { return ParseInstanceId(); })
        .Case("instance_specifier", [this]() { return ParseInstanceSpecifier(); })
        .Case("expected_minimum_server_integrity_level", [this]() { return ParseExpectedMinimumIntegrityLevel(); })
        .Case("rq_instance_specific_event_config", [this]() { return ParseRqInstanceSpecificEventConfig(); })
        .Default(ParserResult::FromError(::amsr::generic::GenErrc::kSystemConfigurationInvalid,
                                         "Unknown or unexpected JSON key found."));
  }

 private:
  ParserResult ParseInstanceId() noexcept {
    return Number<common::internal::config::InstanceId::Type>(
        [&config = config_](auto json_value) { config.instance_id.value = json_value; });
  }

  ParserResult ParseInstanceSpecifier() noexcept {
    return String([&config = config_](::amsr::core::StringView json_value) {
      if (json_value.empty()) {
        ::amsr::core::Abort("Invalid string while parsing instance specifier.");
      } else {
        config.instance_specifier.value = json_value.ToString();
      }
    });
  }

  ParserResult ParseExpectedMinimumIntegrityLevel() noexcept {
    return String([&config = config_](::amsr::core::StringView json_value) {
      config.expected_minimum_server_integrity_level.value = StringToIntegrityLevel(json_value).Value();
    });
  }

  ParserResult ParseRqInstanceSpecificEventConfig() noexcept {
    return Array([this](std::size_t) -> ::amsr::core::Result<void> {
      this->config_.rq_instance_specific_event_config.emplace_back(
          common::internal::config::RequiredInstanceSpecificEventConfig{});

      RqInstanceSpecificEventConfigParser parser{this->GetJsonDocument(),
                                                 config_.rq_instance_specific_event_config.back()};
      return parser.Parse();
    });
  }

  Configuration& config_;
};

class ServiceParser final : public ::amsr::json::v2::SingleObjectParser {
 public:
  using ParserResult = ::amsr::json::ParserResult;
  using Configuration = common::internal::config::Service;

  ServiceParser(::amsr::json::JsonData& json, Configuration& config) noexcept
      : ::amsr::json::v2::SingleObjectParser{json}, config_{config} {}

  ServiceParser(ServiceParser const&) noexcept = delete;
  ServiceParser& operator=(ServiceParser const&) & = delete;
  ServiceParser(ServiceParser&&) noexcept = delete;
  ServiceParser& operator=(ServiceParser&&) & = delete;

  ~ServiceParser() noexcept final = default;

  ParserResult OnKey(::amsr::core::StringView json_key) noexcept final {
    return vac::language::Switch<::amsr::json::ParserResult>(json_key)
        .Case("service_id", [this]() { return ParseServiceId(); })
        .Case("shortname_path", [this]() { return ParseShortNamePath(); })
        .Case("major_version", [this]() { return ParseMajorVersion(); })
        .Case("minor_version", [this]() { return ParseMinorVersion(); })
        .Case("is_generic", [this]() { return ParseIsGeneric(); })
        .Case("provided_service_instances", [this]() { return ParseProvidedServiceInstances(); })
        .Case("required_service_instances", [this]() { return ParseRequiredServiceInstances(); })
        .Case("events", [this]() { return ParseEvents(); })
        .Default(ParserResult::FromError(::amsr::generic::GenErrc::kSystemConfigurationInvalid,
                                         "Unknown or unexpected JSON key found."));
  }

 private:
  ParserResult ParseServiceId() noexcept {
    return Number<common::internal::config::ServiceId::Type>(
        [&config = config_](auto json_value) { config.service_id.value = json_value; });
  }

  ParserResult ParseShortNamePath() noexcept {
    return String([&config = config_](::amsr::core::StringView json_value) {
      if (json_value.empty()) {
        ::amsr::core::Abort("Invalid string while parsing short name path.");
      } else {
        config.shortname_path.value = json_value.ToString();
      }
    });
  }

  ParserResult ParseMajorVersion() noexcept {
    return Number<common::internal::config::MajorVersion::Type>(
        [&config = config_](auto json_value) { config.major_version.value = json_value; });
  }

  ParserResult ParseMinorVersion() noexcept {
    return Number<common::internal::config::MinorVersion::Type>(
        [&config = config_](auto json_value) { config.minor_version.value = json_value; });
  }

  ParserResult ParseIsGeneric() noexcept {
    return Bool([&config = config_](bool json_value) { config.is_generic = json_value; });
  }

  ParserResult ParseProvidedServiceInstances() noexcept {
    return Array([this](std::size_t) -> ::amsr::core::Result<void> {
      this->config_.provided_service_instances.emplace_back(common::internal::config::ProvidedServiceInstance{});

      ProvidedServiceInstanceParser parser{this->GetJsonDocument(), config_.provided_service_instances.back()};
      return parser.Parse();
    });
  }

  ParserResult ParseRequiredServiceInstances() noexcept {
    return Array([this](std::size_t) -> ::amsr::core::Result<void> {
      this->config_.required_service_instances.emplace_back(common::internal::config::RequiredServiceInstance{});

      RequiredServiceInstanceParser parser{this->GetJsonDocument(), config_.required_service_instances.back()};
      return parser.Parse();
    });
  }

  ParserResult ParseEvents() noexcept {
    return Array([this](std::size_t) -> ::amsr::core::Result<void> {
      this->config_.events.emplace_back(common::internal::config::Event{});

      EventParser parser{this->GetJsonDocument(), config_.events.back()};
      return parser.Parse();
    });
  }

  Configuration& config_;
};

class ZeroCopyBindingConfigJsonParser final : public ::amsr::json::v2::SingleObjectParser {
 public:
  using ParserResult = ::amsr::json::ParserResult;
  using Configuration = common::internal::config::ZeroCopyBindingConfig;

  ZeroCopyBindingConfigJsonParser(::amsr::json::JsonData& json, Configuration& config) noexcept
      : ::amsr::json::v2::SingleObjectParser{json}, config_{config} {}

  ZeroCopyBindingConfigJsonParser(ZeroCopyBindingConfigJsonParser const&) noexcept = delete;
  ZeroCopyBindingConfigJsonParser& operator=(ZeroCopyBindingConfigJsonParser const&) & = delete;
  ZeroCopyBindingConfigJsonParser(ZeroCopyBindingConfigJsonParser&&) noexcept = delete;
  ZeroCopyBindingConfigJsonParser& operator=(ZeroCopyBindingConfigJsonParser&&) & = delete;

  ~ZeroCopyBindingConfigJsonParser() noexcept final = default;

  ParserResult OnKey(::amsr::core::StringView json_key) noexcept final {
    return vac::language::Switch<::amsr::json::ParserResult>(json_key)
        .Case("services", [this]() { return ParseServices(); })
        .Case("trace_enabled", [this]() { return ParseTraceEnabled(); })
        .Default(ParserResult::FromError(::amsr::generic::GenErrc::kSystemConfigurationInvalid,
                                         "Unknown or unexpected JSON key found."));
  }

 private:
  ParserResult ParseServices() noexcept {
    return Array([this](std::size_t) {
      this->config_.services.emplace_back(common::internal::config::Service{});

      ServiceParser parser{this->GetJsonDocument(), config_.services.back()};
      return parser.Parse();
    });
  }

  ParserResult ParseTraceEnabled() noexcept {
    return Bool([&config = config_](bool json_value) { config.trace_enabled = json_value; });
  }

  Configuration& config_;
};

ZeroCopyBindingConfig ParseZeroCopyBindingConfigurationFile() noexcept {
  ZeroCopyBindingConfig zero_copy_binding_config{};

  ::amsr::core::Result<::amsr::iointegritystream::ConfigurationStream> configuration_stream{
      ::amsr::iointegritystream::ConfigurationStream::Create("./etc/zerocopybinding_config.json.validation")};

  if (configuration_stream.HasValue()) {
    configuration_stream.Value()
        .Open("./etc/zerocopybinding_config.json")
        .AndThen([&zero_copy_binding_config](::amsr::stream::InputStream& input_stream) {
          ::amsr::json::JsonData json_document{input_stream};
          ZeroCopyBindingConfigJsonParser json_parser{json_document, zero_copy_binding_config};
          return json_parser.Parse();
        })
        .ConsumeError([](auto const&) {
          ::amsr::core::Abort(
              "Parsing the ZeroCopyBinding configuration file failed. File might not exist or might have invalid "
              "structure or integrity.");
        });
  } else {
    ::ara::core::String abort_msg{"Parsing the ZeroCopyBinding validation file failed. Message: "};
    abort_msg.append(configuration_stream.Error().UserMessage());
    ::amsr::core::Abort(abort_msg.c_str());
  }

  // This config_builder is added also in the factory_impl.cpp. When this line needs to be changed, it must be adapted
  // in both locations.
  // VECTOR NL AutosarC++17_10-A16.0.1: MD_ZEROCOPYBINDING_A16.0.1_conditionalCompilation
#ifdef AMSR_ZEROCOPYBINDING_ENABLE_TRACE_API
  ConfigBuilder config_builder{zero_copy_binding_config};
  config_builder.WithTraceEnabled(ConfigBuilder::EnvironmentVariable<bool>{"AMSR_ZEROCOPYBINDING_ENABLE_TRACE_API"});
  config_builder.WithTraceConfig(ConfigBuilder::ComTraceConfig{});
  return config_builder.Build();
#else
  return zero_copy_binding_config;
#endif
}

}  // namespace config

}  // namespace internal
}  // namespace common
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // AMSR_LG_BETA_ENABLED

#endif  // LIB_ZERO_COPY_BINDING_COMMON_INCLUDE_AMSR_ZERO_COPY_BINDING_COMMON_INTERNAL_CONFIG_ZERO_COPY_BINDING_CONFIG_JSON_PARSER_H_
