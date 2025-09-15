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
/*!        \file  machine_mapping_parser.h
 *        \brief  JSON parser for machine mapping.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_MACHINE_MAPPING_PARSER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_MACHINE_MAPPING_PARSER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <type_traits>
#include <utility>
#include "ara/core/string.h"

#include "amsr/someip_daemon_core/configuration/model/required_service_instance_machine_mapping_validator.h"

#include "amsr/json/reader.h"
#include "amsr/json/util/json_error_domain.h"
#include "amsr/json/util/number.h"
#include "amsr/log/logstream.h"
#include "amsr/net/ip/network.h"
#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/configuration/model/machine_mapping_object.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "amsr/someip_daemon_core/logging/someip_daemon_log_builder.h"
#include "amsr/someip_daemon_core/someip_daemon_error_code.h"
#include "ara/core/optional.h"
#include "ara/log/logstream.h"
#include "provided_service_instance_signal_serialized_pdus_parser.h"
#include "service_instance_provider_parser.h"
#include "service_instance_subscribers_parser.h"
#include "vac/container/c_string_view.h"
#include "vac/container/string_literals.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace parsing {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_SomeIpDaemon_M7.3.6_sv
/*!
 * \brief String literals.
 */
using vac::container::literals::operator""_sv;

// VECTOR Disable AutosarC++17_10-A10.2.1: MD_SomeIpDaemon_AutosarC++17_10_A10.2.1_redefinition_of_non_virtual_function

/*!
 * \brief Parses a single 'machine mapping' element from JSON.
 * \details  MachineMappingParser is used by multiple contexts:
 * - Required service instances
 * - Provided service instances
 * \tparam Validation Validation used by this parser. Different validators required due to the multiple contexts of
 * used MachineMappings.
 */
template <typename Validation>
class MachineMappingParser final : public amsr::json::Parser<MachineMappingParser<Validation>> {
 public:
  /*!
   * \brief Type-alias for parser base class.
   */
  using Parser = amsr::json::Parser<MachineMappingParser<Validation>>;

  /*!
   * \brief Type-alias for validation result.
   */
  using ValidationResult = typename Validation::ValidationResult;

  /*!
   * \brief Type-alias for easy access to CStringView.
   */
  using CStringView = vac::container::CStringView;

  /*!
   * \brief Type-alias for easy access to ParserResult.
   */
  using ParserResult = amsr::json::ParserResult;

  /*!
   * \brief Type-alias for easy access to ParserState.
   */
  using ParserState = amsr::json::ParserState;

  /*!
   * \brief Port mapping name JSON key.
   */
  static constexpr CStringView kNameKey{"name"_sv};

  /*!
   * \brief IP address JSON key.
   */
  static constexpr CStringView kAddressKey{"address"_sv};

  /*!
   * \brief UDP port JSON key.
   */
  static constexpr CStringView kUdpPortKey{"udp_port"_sv};

  /*!
   * \brief TCP port JSON key.
   */
  static constexpr CStringView kTcpPortKey{"tcp_port"_sv};

  /*!
   * \brief Event multicast address JSON key.
   */
  static constexpr CStringView kEventMulticastAddressKey{"event_multicast_address"_sv};

  /*!
   * \brief Event multicast port JSON key.
   */
  static constexpr CStringView kEventMulticastPortKey{"event_multicast_port"_sv};

  /*!
   * \brief Network mask JSON key.
   */
  static constexpr CStringView kNetworkMaskKey{"network_mask"_sv};

  /*!
   * \brief IP address prefix length JSON key.
   */
  static constexpr CStringView kPrefixLengthKey{"prefix_length"_sv};

  /*!
   * \brief Service discovery JSON key.
   */
  static constexpr CStringView kServiceDiscoveryEnabledKey{"service_discovery"_sv};
  /*!
   * \brief Communication JSON key.
   */
  static constexpr CStringView kCommunicationEnabledKey{"communication"_sv};

  /*!
   * \brief Provider JSON key.
   */
  static constexpr CStringView kProviderKey{"provider"_sv};

  /*!
   * \brief Subscribers JSON key.
   */
  static constexpr CStringView kSubscribersKey{"subscribers"_sv};

  /*!
   * \brief Provided signal serialized PDUs.
   */
  static constexpr CStringView kProvidedPdusKey{"provided_signal_serialized_pdus"_sv};

  /*!
   * \brief UDP collection buffer size threshold
   */
  static constexpr CStringView kUdpCollectionBufferSizeThresholdKey{"udp_collection_buffer_size_threshold"_sv};
  /*!
   * \brief Event group UDP endpoint address JSON key.
   */
  static constexpr CStringView kEventGroupUdpEndpointAddressKey{"eventgroup_udp_endpoint_address"_sv};

  /*!
   * \brief Event group TCP endpoint address JSON key.
   */
  static constexpr CStringView kEventGroupTcpEndpointAddressKey{"eventgroup_tcp_endpoint_address"_sv};

  /*!
   * \brief Event group UDP endpoint port JSON key.
   */
  static constexpr CStringView kEventGroupUdpEndpointPortKey{"eventgroup_udp_endpoint_port"_sv};

  /*!
   * \brief Event group TCP endpoint port JSON key.
   */
  static constexpr CStringView kEventGroupTcpEndpointPortKey{"eventgroup_tcp_endpoint_port"_sv};

  /*!
   * \brief Secure connection IDs JSON key.
   */
  static constexpr CStringView kSecureConnectionIds{"secure_ids"_sv};

  /*!
   * \brief Input for this parser is the container of 'machine mapping' from the config to fill.
   * \param[in] file JSON document to parse.
   * \param[out] machine_mapping A reference to the element which is written by the parser.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  MachineMappingParser(amsr::json::JsonData& file, ConfigurationTypesAndDefs::MachineMapping& machine_mapping) noexcept
      : Parser{file},
        logger_(someip_daemon_core::logging::kConfigurationLoggerContextId,
                someip_daemon_core::logging::kConfigurationLoggerContextDescription,
                ara::core::StringView{"MachineMappingParser"}),
        is_inside_sub_array_{false},
        machine_mapping_element_wrapper_{model::MakeMachineMappingConfigObject(machine_mapping)},
        service_provider_{},
        service_subscribers_{} {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~MachineMappingParser() final = default;

  /*!
   * \brief Deleted copy constructor.
   */
  MachineMappingParser(MachineMappingParser const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  MachineMappingParser(MachineMappingParser&&) = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(MachineMappingParser const&) -> MachineMappingParser& = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(MachineMappingParser&&) -> MachineMappingParser& = delete;

  /*!
   * \brief Callback on a new key parsed from JSON object. Used to detect unknown JSON keys.
   * \param[in] key Parsed key string.
   * \return ParserResult with ParserState::kRunning or error code if any unknown JSON key was detected.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnKey(CStringView key) noexcept {
    ParserResult result{amsr::json::ParserState::kRunning};
    // VECTOR NC AutosarC++17_10-M5.14.1: MD_SomeIpDaemon_AutosarC++17_10-M5.14.1_op_has_no_side_effect
    if (!((key == kNameKey) || (key == kAddressKey) || (key == kUdpPortKey) || (key == kTcpPortKey) ||
          (key == kEventMulticastAddressKey) || (key == kEventMulticastPortKey) || (key == kNetworkMaskKey) ||
          (key == kPrefixLengthKey) || (key == kCommunicationEnabledKey) || (key == kServiceDiscoveryEnabledKey) ||
          (key == kProviderKey) || (key == kSubscribersKey) || (key == kProvidedPdusKey) ||
          (key == kUdpCollectionBufferSizeThresholdKey) || (key == kEventGroupUdpEndpointAddressKey) ||
          (key == kEventGroupTcpEndpointAddressKey) || (key == kEventGroupUdpEndpointPortKey) ||
          (key == kEventGroupTcpEndpointPortKey) || (key == kSecureConnectionIds))) {
      result = ParserResult{SomeIpDaemonErrc::json_parsing_failed};
      logger_.LogError(
          [&key](ara::log::LogStream& s) {
            char const* const key_cstr{key.c_str()};
            s << "Unknown JSON key found: '" << key_cstr << "'.";
          },
          static_cast<char const*>(__func__), __LINE__);
    } else if (key == kProviderKey) {
      // Create a sub-parser for the port mapping parameters
      ServiceInstanceProviderParser provider_parser{this->GetJsonDocument(), this->service_provider_};

      ara::core::Result<void> const subparser_result{provider_parser.Parse()};
      // Abort further parsing if sub-parser failed
      if (subparser_result) {
        model::SetProviderElementOnMachineMappingConfigObject(machine_mapping_element_wrapper_,
                                                              std::move(this->service_provider_));
      } else {
        result = ParserResult{SomeIpDaemonErrc::json_parsing_failed};
      }
    } else if (key == kSubscribersKey) {
      ServiceInstanceSubscribersParser subscribers_parser{this->GetJsonDocument(), this->service_subscribers_};
      ara::core::Result<void> const subparser_result{subscribers_parser.Parse()};
      if (subparser_result) {
        model::SetSubscribersElementOnMachineMappingConfigObject(machine_mapping_element_wrapper_,
                                                                 std::move(this->service_subscribers_));
      } else {
        result = ParserResult{SomeIpDaemonErrc::json_parsing_failed};
      }
    } else if (key == kProvidedPdusKey) {
      ProvidedServiceInstanceSignalSerializedPduParser provided_pdus_parser{this->GetJsonDocument(),
                                                                            this->provided_pdus_};
      ara::core::Result<void> const subparser_result{provided_pdus_parser.Parse()};
      if (subparser_result) {
        model::SetProvidedPdusElementOnMachineMappingConfigObject(machine_mapping_element_wrapper_,
                                                                  std::move(this->provided_pdus_));
      } else {
        result = ParserResult{SomeIpDaemonErrc::json_parsing_failed};
      }
    } else {
      // Valid key value will be parsed in Number() or String() callback
    }

    return result;
  }
  /*!
   * \brief A type alias for the UniqueFunction
   */
  using ActionFuncType = vac::language::UniqueFunction<void()>;

  /*!
   * \brief Invoke a callback function if the passed element status is not set.
   * \param[in] status Status of the passed element.
   * \param[in] action Callback invoked if the status is not set.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  static void InvokeIfStatusNotSet(ElementStatus status, ActionFuncType&& action) noexcept {
    if (status == ElementStatus::kNotSet) {
      action();
    }
  }

  /*!
   * \brief Set default value for the machine mapping's elements if they are not set.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetDefaultValues() noexcept {
    InvokeIfStatusNotSet(machine_mapping_element_wrapper_.event_multicast_address.GetStatus(), [this]() noexcept {
      model::SetEventMulticastAddressOnMachineMappingConfigObject(machine_mapping_element_wrapper_,
                                                                  ara::core::Optional<someip_daemon_core::IpAddress>{});
    });

    InvokeIfStatusNotSet(machine_mapping_element_wrapper_.event_multicast_port.GetStatus(), [this]() noexcept {
      model::SetEventMulticastPortOnMachineMappingConfigObject(machine_mapping_element_wrapper_,
                                                               ConfigurationTypesAndDefs::kInvalidPort);
    });

    InvokeIfStatusNotSet(machine_mapping_element_wrapper_.network_mask.GetStatus(), [this]() noexcept {
      model::SetNetworkMaskOnMachineMappingConfigObject(
          machine_mapping_element_wrapper_,
          someip_daemon_core::IpAddress::MakeAddress(ConfigurationTypesAndDefs::kDefaultNetworkMask).Value());
    });

    InvokeIfStatusNotSet(machine_mapping_element_wrapper_.prefix_length.GetStatus(), [this]() noexcept {
      model::SetPrefixLengthOnMachineMappingConfigObject(machine_mapping_element_wrapper_,
                                                         ConfigurationTypesAndDefs::kDefaultPrefixLength);
    });
  }

  /*!
   * \brief Check the consistency of the Service Discovery.
   * \param[out] result value of ParserResult which will be updated in case of failure.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void CheckServiceDiscoveryConsistency(ParserResult& result) const noexcept {
    if ((machine_mapping_element_wrapper_.service_discovery_enabled.GetElement() == false) &&
        (machine_mapping_element_wrapper_.communication_enabled.GetElement() == true)) {
      if ((machine_mapping_element_wrapper_.provider.GetStatus() == ElementStatus::kNotSet) &&
          (machine_mapping_element_wrapper_.subscribers.GetStatus() == ElementStatus::kNotSet)) {
        logger_.LogError(
            [](ara::log::LogStream& s) {
              s << "Expected service provider or service subscriber data is not provided in the model.";
            },
            static_cast<char const*>(__func__), __LINE__);
        result = ParserResult{SomeIpDaemonErrc::json_parsing_failed};
      }
    }

    if ((machine_mapping_element_wrapper_.service_discovery_enabled.GetElement() == false) &&
        (machine_mapping_element_wrapper_.communication_enabled.GetElement() == false)) {
      logger_.LogError(
          [](ara::log::LogStream& s) {
            s << "Neither 'communication' nor 'service discovery' are configured to be used.";
          },
          static_cast<char const*>(__func__), __LINE__);
      result = ParserResult{SomeIpDaemonErrc::json_parsing_failed};
    }

    if ((machine_mapping_element_wrapper_.service_discovery_enabled.GetElement() == true) &&
        ((machine_mapping_element_wrapper_.provider.GetStatus() == ElementStatus::kSet) ||
         (machine_mapping_element_wrapper_.subscribers.GetStatus() == ElementStatus::kSet))) {
      logger_.LogError([](ara::log::LogStream& s) { s << "Static Service Discovery is not configured to be used."; },
                       static_cast<char const*>(__func__), __LINE__);
      result = ParserResult{SomeIpDaemonErrc::json_parsing_failed};
    }
  }

  /*!
   * \brief Check that the mapping is set either a serivce provider or a service consumer.
   * \param[out] result value of ParserResult which will be updated in case of failure.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void CheckMachineMappingRole(ParserResult& result) const noexcept {
    if ((machine_mapping_element_wrapper_.provider.GetStatus() == ElementStatus::kSet) &&
        (machine_mapping_element_wrapper_.subscribers.GetStatus() == ElementStatus::kSet)) {
      logger_.LogError(
          [](ara::log::LogStream& s) {
            s << "Each mapping shall only be either a service provider or a service consumer.";
          },
          static_cast<char const*>(__func__), __LINE__);
      result = ParserResult{SomeIpDaemonErrc::json_parsing_failed};
    }
  }

  /*!
   * \brief Validate the TCP or UDP configuration when either of them is set to be used.
   * \param[out] result value of ParserResult which will be updated in case of failure.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void CheckInvalidTcpOrUdp(ParserResult& result) const noexcept {
    if ((machine_mapping_element_wrapper_.tcp_port.GetElement().has_value()) &&
        (!machine_mapping_element_wrapper_.udp_port.GetElement().has_value())) {
      if ((machine_mapping_element_wrapper_.provider.GetStatus() == ElementStatus::kSet) &&
          (!machine_mapping_element_wrapper_.provider.GetElement()->tcp_port_.has_value())) {
        logger_.LogError(
            [](ara::log::LogStream& s) {
              s << "TCP port shall be configured for this service instance provider since there is ONLY"
                   "TCP port mapping for this Service Instance.";
            },
            static_cast<char const*>(__func__), __LINE__);
        result = ParserResult{SomeIpDaemonErrc::json_parsing_failed};
      }
    }

    if ((machine_mapping_element_wrapper_.udp_port.GetElement().has_value()) &&
        (!machine_mapping_element_wrapper_.tcp_port.GetElement().has_value())) {
      if ((machine_mapping_element_wrapper_.provider.GetStatus() == ElementStatus::kSet) &&
          (!machine_mapping_element_wrapper_.provider.GetElement()->udp_port_.has_value())) {
        logger_.LogError(
            [](ara::log::LogStream& s) {
              s << "UDP port shall be configured for this service instance provider since there is ONLY"
                   "UDP port mapping for this Service Instance.";
            },
            static_cast<char const*>(__func__), __LINE__);
        result = ParserResult{SomeIpDaemonErrc::json_parsing_failed};
      }
    }
  }

  /*!
   * \brief Obtain the network object from the machine mapping.
   * \param[out] result value of ParserResult which will be updated in case of failure
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void CreateNetworkObject(ParserResult& result) noexcept {
    assert(machine_mapping_element_wrapper_.address.GetElement().has_value());
    ara::core::Result<void> ret{model::SetNetworkOnMachineMappingConfigObject(machine_mapping_element_wrapper_)};

    if (!ret.HasValue()) {
      result = ParserResult{SomeIpDaemonErrc::json_parsing_failed};

      logger_.LogError(
          [this, &ret](ara::log::LogStream& s) {
            ara::core::ErrorCode const error{ret.Error()};

            ara::core::String const error_msg_str{error.Message().ToString()};
            ara::core::String const error_usr_msg_str{error.UserMessage().ToString()};
            ara::core::String const error_support_data_str{std::to_string(error.SupportData())};

            ara::core::String const address_str{
                machine_mapping_element_wrapper_.address.GetElement().value().ToString()};
            ara::core::String const network_mask_str{
                machine_mapping_element_wrapper_.network_mask.GetElement().ToString()};
            ara::core::String const prefix_length_str{
                std::to_string(machine_mapping_element_wrapper_.prefix_length.GetElement())};

            s << "Cannot create network object (address: ";
            s << address_str;
            s << ", network mask: ";
            s << network_mask_str;
            s << ", prefix length: ";
            s << prefix_length_str;
            s << ", user message: ";
            s << error_usr_msg_str;
            s << ", support data: ";
            s << error_support_data_str;
            s << ").";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief Callback on end of JSON object.
   * \return ParserResult with ParserState::kFinished if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnEndObject(std::size_t) noexcept {
    ParserResult result{amsr::json::ParserState::kFinished};

    // Validate the parsed object
    ValidationResult const validation_result{Validation::Check(machine_mapping_element_wrapper_)};

    if (validation_result == ValidationResult::kOk) {
      SetDefaultValues();
      CheckServiceDiscoveryConsistency(result);
      CheckMachineMappingRole(result);
      CheckInvalidTcpOrUdp(result);

      if (machine_mapping_element_wrapper_.provided_pdus.GetStatus() == ElementStatus::kSet) {
        logger_.LogDebug([](ara::log::LogStream& s) { s << "Provided signal serialized PDUs are supplied."; },
                         static_cast<char const*>(__func__), __LINE__);
      }
      if (machine_mapping_element_wrapper_.address.GetStatus() != ElementStatus::kSet) {
        logger_.LogDebug([](ara::log::LogStream& s) { s << "IP address is not configured."; },
                         static_cast<char const*>(__func__), __LINE__);
      } else {
        CreateNetworkObject(result);
      }

    } else {
      result = ParserResult{SomeIpDaemonErrc::json_parsing_failed};

      logger_.LogError(
          [&validation_result](ara::log::LogStream& s) {
            s << "Parsed object is invalid. Validation result: ";
            s << Validation::ValidationResultToString(validation_result);
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    return result;
  }

  /*!
   * \brief Start Array
   * \return ParserResult with ParserState::kRunning.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnStartArray() noexcept {
    CStringView const key{this->GetCurrentKey()};

    if (key == kSecureConnectionIds) {
      // Set indication flag for expecting the next bracket to be a part of a key array.
      is_inside_sub_array_ = true;
    }

    return ParserResult{amsr::json::ParserState::kRunning};
  }

  /*!
   * \brief End Array
   * \return ParserResult with ParserState::kRunning or if unexpected end of array with json_parsing_failed.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnEndArray(std::size_t) noexcept {
    ParserResult result{SomeIpDaemonErrc::json_parsing_failed};

    if (is_inside_sub_array_) {
      result = ParserResult{amsr::json::ParserState::kRunning};
      is_inside_sub_array_ = false;
      SetSecureConnectionIds(machine_mapping_element_wrapper_, secure_connection_ids_);
    } else {
      logger_.LogError([](ara::log::LogStream& s) { s << "Unexpected end of array."; },
                       static_cast<char const*>(__func__), __LINE__);
    }

    return result;
  };

  /*!
   * \brief Callback on number found in JSON
   * \param[in] num value returned by the parser.
   * \return ParserResult with ParserState::kFinished if parsing was successful. Otherwise an error code is returned.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnNumber(amsr::json::JsonNumber num) noexcept {
    ParserResult result{amsr::json::ParserState::kRunning};

    // Get current key
    CStringView key{this->GetCurrentKey()};

    char const* function_name{static_cast<char const*>(__func__)};

    auto log_error = [&result, &logger = logger_, &key, &function_name] {
      result = ParserResult{SomeIpDaemonErrc::json_parsing_failed};
      logger.LogError(
          [&key](ara::log::LogStream& s) {
            char const* const key_cstr{key.c_str()};
            s << "Invalid format of key '" << key_cstr << "'.";
          },
          function_name, __LINE__);
    };

    // Create map to store actions for each different kind of key
    ara::core::Map<CStringView, vac::language::UniqueFunction<void()>> on_number_map{};
    // Add kUdpPortKey action to the map
    static_cast<void>(on_number_map.emplace(kUdpPortKey, [this, &num, &function_name, &log_error] {
      ara::core::Optional<someip_protocol::internal::Port> udp_port{num.template As<someip_protocol::internal::Port>()};
      if (udp_port.has_value()) {
        model::SetUdpPortOnMachineMappingConfigObject(machine_mapping_element_wrapper_, udp_port);
        logger_.LogVerbose(
            [&udp_port](ara::log::LogStream& s) {
              someip_protocol::internal::Port const& udp_port_ref{*udp_port};
              s << "UDP port: " << udp_port_ref;
            },
            function_name, __LINE__);
      } else {
        log_error();
      }
    }));
    // Add kTcpPortKey action to the map
    static_cast<void>(on_number_map.emplace(kTcpPortKey, [this, &num, &function_name, &log_error] {
      ara::core::Optional<someip_protocol::internal::Port> tcp_port{num.template As<someip_protocol::internal::Port>()};
      if (tcp_port.has_value()) {
        model::SetTcpPortOnMachineMappingConfigObject(machine_mapping_element_wrapper_, tcp_port);
        logger_.LogVerbose(
            [&tcp_port](ara::log::LogStream& s) {
              someip_protocol::internal::Port const& tcp_port_ref{*tcp_port};
              s << "TCP port: " << tcp_port_ref;
            },
            function_name, __LINE__);
      } else {
        log_error();
      }
    }));
    // Add kEventGroupUdpEndpointPortKey action to the map
    static_cast<void>(on_number_map.emplace(kEventGroupUdpEndpointPortKey, [this, &num, &function_name, &log_error] {
      ara::core::Optional<someip_protocol::internal::Port> udp_port{num.template As<someip_protocol::internal::Port>()};
      if (udp_port.has_value()) {
        model::SetEventGroupUdpPortOnMachineMappingConfigObject(machine_mapping_element_wrapper_, *udp_port);
        logger_.LogVerbose(
            [&udp_port](ara::log::LogStream& s) {
              someip_protocol::internal::Port const& udp_port_ref{*udp_port};
              s << "Event group UDP port: " << udp_port_ref;
            },
            function_name, __LINE__);
      } else {
        log_error();
      }
    }));
    // Add kEventGroupTcpEndpointPortKey action to the map
    static_cast<void>(on_number_map.emplace(kEventGroupTcpEndpointPortKey, [this, &num, &function_name, &log_error] {
      ara::core::Optional<someip_protocol::internal::Port> tcp_port{num.template As<someip_protocol::internal::Port>()};
      if (tcp_port.has_value()) {
        model::SetEventGroupTcpPortOnMachineMappingConfigObject(machine_mapping_element_wrapper_, *tcp_port);
        logger_.LogVerbose(
            [&tcp_port](ara::log::LogStream& s) {
              someip_protocol::internal::Port const& udp_port_ref{*tcp_port};
              s << "Event group TCP port: " << udp_port_ref;
            },
            function_name, __LINE__);
      } else {
        log_error();
      }
    }));
    // Add kEventMulticastPortKey action to the map
    static_cast<void>(on_number_map.emplace(kEventMulticastPortKey, [this, &num, &function_name, &log_error] {
      ara::core::Optional<someip_protocol::internal::Port> event_multicast_port{
          num.template As<someip_protocol::internal::Port>()};
      if (event_multicast_port.has_value()) {
        model::SetEventMulticastPortOnMachineMappingConfigObject(machine_mapping_element_wrapper_,
                                                                 *event_multicast_port);
        logger_.LogVerbose(
            [&event_multicast_port](ara::log::LogStream& s) {
              someip_protocol::internal::Port const& event_multicast_port_ref{*event_multicast_port};
              s << "Event multicast port: " << event_multicast_port_ref;
            },
            function_name, __LINE__);
      } else {
        log_error();
      }
    }));
    // Add kPrefixLengthKey action to the map
    static_cast<void>(on_number_map.emplace(kPrefixLengthKey, [this, &num, &function_name, &log_error] {
      ara::core::Optional<someip_protocol::internal::IpAddressPrefixLength> prefix_length{
          num.template As<someip_protocol::internal::IpAddressPrefixLength>()};
      if (prefix_length.has_value()) {
        model::SetPrefixLengthOnMachineMappingConfigObject(machine_mapping_element_wrapper_, *prefix_length);
        logger_.LogVerbose(
            [&prefix_length](ara::log::LogStream& s) {
              someip_protocol::internal::IpAddressPrefixLength const& prefix_length_ref{*prefix_length};
              s << "Prefix length: " << prefix_length_ref;
            },
            function_name, __LINE__);
      } else {
        log_error();
      }
    }));
    // Add kUdpCollectionBufferSizeThresholdKey action to the map
    static_cast<void>(
        on_number_map.emplace(kUdpCollectionBufferSizeThresholdKey, [this, &num, &function_name, &log_error] {
          ara::core::Optional<ConfigurationTypesAndDefs::UdpCollectionBufferSizeThreshold> value{
              num.template As<ConfigurationTypesAndDefs::UdpCollectionBufferSizeThreshold>()};
          if (value.has_value()) {
            model::SetUdpCollectionBufferSizeThresholdOnMachineMappingConfigObject(machine_mapping_element_wrapper_,
                                                                                   *value);
            logger_.LogVerbose(
                [&value](ara::log::LogStream& s) {
                  ConfigurationTypesAndDefs::UdpCollectionBufferSizeThreshold const& ref{*value};
                  s << "UdpCollectionBufferSizeThreshold: " << ref;
                },
                function_name, __LINE__);
          } else {
            log_error();
          }
        }));
    // Add kSecureConnectionIds action to the map
    static_cast<void>(on_number_map.emplace(kSecureConnectionIds, [this, &num, &function_name, &log_error] {
      ara::core::Optional<ConfigurationTypesAndDefs::SecureId> secure_id{
          num.template As<ConfigurationTypesAndDefs::SecureId>()};
      if (secure_id.has_value()) {
        this->secure_connection_ids_.push_back(secure_id.value());
        logger_.LogVerbose(
            [&secure_id](ara::log::LogStream& s) {
              ConfigurationTypesAndDefs::SecureId const& ref{*secure_id};
              s << "SecureId: " << ref;
            },
            function_name, __LINE__);
      } else {
        log_error();
      }
    }));

    auto const iter = on_number_map.find(key);

    if (iter != on_number_map.end()) {
      iter->second();
    } else {
      result = ParserResult{SomeIpDaemonErrc::json_parsing_failed};
      logger_.LogError(
          [&key](ara::log::LogStream& s) {
            char const* const key_cstr{key.c_str()};
            s << "Unexpected numeric key found: " << key_cstr << ".";
          },
          function_name, __LINE__);
    }

    return result;
  }

  // VECTOR NC Metric-HIS.VG: MD_SomeIpDaemon_Metric-HIS.VG_cyclomatic_complexity
  /*!
   * \brief Callback on string found in JSON
   * \param[in] val Parsed string value
   * \return ParserResult Continue parsing.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ParserResult OnString(CStringView val) noexcept {
    CStringView key{this->GetCurrentKey()};
    ParserResult result{amsr::json::ParserState::kRunning};

    if (key == kAddressKey) {
      someip_protocol::internal::IpAddress ip_address{val.ToString()};
      ara::core::Result<someip_daemon_core::IpAddress> ip_address_result{
          someip_daemon_core::IpAddress::MakeAddress(ip_address)};
      if (ip_address_result.HasValue()) {
        model::SetIpAddressOnMachineMappingConfigObject(machine_mapping_element_wrapper_, ip_address_result.Value());
        logger_.LogVerbose(
            [&ip_address](ara::log::LogStream& s) {
              s << "IP address: ";
              s << ip_address;
            },
            static_cast<char const*>(__func__), __LINE__);
      } else {
        logger_.LogFatalAndAbort(
            [&ip_address](ara::core::String& abort_msg) noexcept {
              abort_msg.append("Invalid format for ip address '");
              abort_msg.append(ip_address);
              abort_msg.append("'");
            },
            AMSR_FILE, static_cast<char const*>(__func__), __LINE__);
      }
    } else if (key == kEventGroupUdpEndpointAddressKey) {
      ara::core::String const ip_address{val.ToString()};
      ara::core::Result<someip_daemon_core::IpAddress> ip_address_result{
          someip_daemon_core::IpAddress::MakeAddress(ip_address)};
      if (ip_address_result.HasValue()) {
        model::SetEventGroupUdpIpAddressOnMachineMappingConfigObject(machine_mapping_element_wrapper_,
                                                                     ip_address_result.Value());
        logger_.LogVerbose(
            [&ip_address](ara::log::LogStream& s) {
              s << "Event group UDP IP address: ";
              s << ip_address;
            },
            static_cast<char const*>(__func__), __LINE__);
      } else {
        logger_.LogFatalAndAbort(
            [&ip_address](ara::core::String& abort_msg) noexcept {
              abort_msg.append("Invalid format for udp endpoint ip address '");
              abort_msg.append(ip_address);
              abort_msg.append("'");
            },
            AMSR_FILE, static_cast<char const*>(__func__), __LINE__);
      }
    } else if (key == kEventGroupTcpEndpointAddressKey) {
      ara::core::String const ip_address{val.ToString()};
      ara::core::Result<someip_daemon_core::IpAddress> ip_address_result{
          someip_daemon_core::IpAddress::MakeAddress(ip_address)};
      if (ip_address_result.HasValue()) {
        model::SetEventGroupTcpIpAddressOnMachineMappingConfigObject(machine_mapping_element_wrapper_,
                                                                     ip_address_result.Value());
        logger_.LogVerbose(
            [&ip_address](ara::log::LogStream& s) {
              s << "Event group TCP IP address: ";
              s << ip_address;
            },
            static_cast<char const*>(__func__), __LINE__);
      } else {
        logger_.LogFatalAndAbort(
            [&ip_address](ara::core::String& abort_msg) noexcept {
              abort_msg.append("Invalid format for tcp endpoint ip address '");
              abort_msg.append(ip_address);
              abort_msg.append("'");
            },
            AMSR_FILE, static_cast<char const*>(__func__), __LINE__);
      }
    } else if (key == kEventMulticastAddressKey) {
      ara::core::String const event_multicast_address{val.ToString()};
      ara::core::Result<someip_daemon_core::IpAddress> multicast_address_result{
          someip_daemon_core::IpAddress::MakeAddress(event_multicast_address)};
      if (multicast_address_result.HasValue()) {
        model::SetEventMulticastAddressOnMachineMappingConfigObject(machine_mapping_element_wrapper_,
                                                                    multicast_address_result.Value());
        logger_.LogVerbose(
            [&event_multicast_address](ara::log::LogStream& s) {
              s << "Event multicast address: ";
              s << event_multicast_address;
            },
            static_cast<char const*>(__func__), __LINE__);
      } else {
        logger_.LogFatalAndAbort(
            [&event_multicast_address](ara::core::String& abort_msg) noexcept {
              abort_msg.append("Invalid format for event multicast ip address '");
              abort_msg.append(event_multicast_address);
              abort_msg.append("'");
            },
            AMSR_FILE, static_cast<char const*>(__func__), __LINE__);
      }
    } else if (key == kNetworkMaskKey) {
      ara::core::String const network_mask{val.ToString()};
      ara::core::Result<someip_daemon_core::IpAddress> network_mask_result{
          someip_daemon_core::IpAddress::MakeAddress(network_mask)};
      if (network_mask_result.HasValue()) {
        model::SetNetworkMaskOnMachineMappingConfigObject(machine_mapping_element_wrapper_,
                                                          network_mask_result.Value());
        logger_.LogVerbose(
            [&network_mask](ara::log::LogStream& s) {
              s << "Network mask: ";
              s << network_mask;
            },
            static_cast<char const*>(__func__), __LINE__);
      } else {
        logger_.LogFatalAndAbort(
            [&network_mask](ara::core::String& abort_msg) noexcept {
              abort_msg.append("Invalid format for network mask '");
              abort_msg.append(network_mask);
              abort_msg.append("'");
            },
            AMSR_FILE, static_cast<char const*>(__func__), __LINE__);
      }
    } else if (key == kNameKey) {
      // Ignore port mapping name info. Name is only used as comment to simplify readability of JSON file
      logger_.LogVerbose(
          [&val](ara::log::LogStream& s) {
            ara::core::String const val_string{val.ToString()};
            s << "Port mapping name: ";
            s << val_string;
          },
          static_cast<char const*>(__func__), __LINE__);
    } else {
      result = ParserResult{SomeIpDaemonErrc::json_parsing_failed};
      logger_.LogError(
          [&key](ara::log::LogStream& s) {
            char const* const key_cstr{key.c_str()};
            s << "Unexpected string key found: " << key_cstr << ".";
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    return result;
  }

  /*!
   * \brief Callback on bool found in JSON
   * \param[in] val Parsed string value
   * \return ParserResult Continue parsing.
   * \error amsr::json::JsonErrc::kUserValidationFailed
   * \pre -
   * \context ANY
   * \reentrant FALSE
   *
   * \internal
   * - Set the ParserResult to "Running".
   * - Get the current key.
   *   - Check if the key is "kCommunicationEnabledKey".
   *     - Update the communication element with the provided value.
   *   - Otherwise, check if the key is "kServiceDiscoveryEnabledKey".
   *     - Update the service discovery element with the provided value.
   *   - Otherwise, log an error that unexpected boolean key fod.
   * - Return the ParserResult.
   * \endinternal
   * \steady FALSE
   */
  ParserResult OnBool(bool val) {
    CStringView key{this->GetCurrentKey()};
    ParserResult result{amsr::json::ParserState::kRunning};
    if (key == kCommunicationEnabledKey) {
      model::SetCommunicationOnMachineMappingConfigObject(machine_mapping_element_wrapper_, val);
    } else if (key == kServiceDiscoveryEnabledKey) {
      model::SetServiceDiscoveryOnMachineMappingConfigObject(machine_mapping_element_wrapper_, val);
    } else {
      result = ParserResult{SomeIpDaemonErrc::json_parsing_failed};
      logger_.LogError(
          [&key, &val](ara::log::LogStream& s) {
            char const* const key_cstr{key.c_str()};
            s << "Unexpected boolean key found: '" << key_cstr << "': " << val;
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    if (machine_mapping_element_wrapper_.service_discovery_enabled.GetElement() &&
        machine_mapping_element_wrapper_.communication_enabled.GetElement()) {
      model::SetCommunicationTypeOnMachineMappingConfigObject(
          machine_mapping_element_wrapper_,
          ConfigurationTypesAndDefs::ServiceInstanceCommunicationType::kSdAndCommunication);
    } else if (machine_mapping_element_wrapper_.communication_enabled.GetElement()) {
      model::SetCommunicationTypeOnMachineMappingConfigObject(
          machine_mapping_element_wrapper_,
          ConfigurationTypesAndDefs::ServiceInstanceCommunicationType::kCommunicationOnly);
    } else {
      model::SetCommunicationTypeOnMachineMappingConfigObject(
          machine_mapping_element_wrapper_, ConfigurationTypesAndDefs::ServiceInstanceCommunicationType::kSdOnly);
    }
    return result;
  }

  /*!
   * \brief Default callback if a type is encountered for which no callback exists.
   * \return ParserResult with ParserState::kRunning to continue parsing.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  static ParserResult OnUnexpectedEvent() noexcept { return amsr::json::ParserState::kRunning; }

 private:
  /*!
   * \brief Logger.
   */
  someip_daemon_core::logging::AraComLogger logger_;

  /*!
   * \brief Flag to distinguish if parser is inside a sub-value array.
   */
  bool is_inside_sub_array_{false};

  /*!
   * \brief Intermediate 'machine mapping' object to fill with parsed information.
   */
  model::MachineMappingConfigObject machine_mapping_element_wrapper_;

  /*!
   * \brief This POD structure will be added to the port mapping element if the parser succeeded.
   */
  ConfigurationTypesAndDefs::ServiceProviderConfiguration service_provider_;

  /*!
   * \brief The subscribers container.
   */
  ConfigurationTypesAndDefs::ServiceSubscriberConfigurationContainer service_subscribers_;

  /*!
   * \brief The provided signal serialized PDUs.
   */
  ConfigurationTypesAndDefs::SignalSerializedPduContainer provided_pdus_{};

  /*!
   * \brief The secure connection IDs container for clients.
   */
  ConfigurationTypesAndDefs::SecureIdContainer secure_connection_ids_{};
};
// VECTOR Enable AutosarC++17_10-A10.2.1

/*!
 * \brief Definition of port mapping name key.
 */
template <typename Validation>
constexpr vac::container::CStringView MachineMappingParser<Validation>::kNameKey;

/*!
 * \brief Definition of IP address key.
 */
template <typename Validation>
constexpr vac::container::CStringView MachineMappingParser<Validation>::kAddressKey;

/*!
 * \brief Definition of UDP port key.
 */
template <typename Validation>
constexpr vac::container::CStringView MachineMappingParser<Validation>::kUdpPortKey;

/*!
 * \brief Definition of TCP port key.
 */
template <typename Validation>
constexpr vac::container::CStringView MachineMappingParser<Validation>::kTcpPortKey;

/*!
 * \brief Definition of event multicast address key.
 */
template <typename Validation>
constexpr vac::container::CStringView MachineMappingParser<Validation>::kEventMulticastAddressKey;

/*!
 * \brief Definition of event multicast port key.
 */
template <typename Validation>
constexpr vac::container::CStringView MachineMappingParser<Validation>::kEventMulticastPortKey;

/*!
 * \brief Definition of network mask key.
 */
template <typename Validation>
constexpr vac::container::CStringView MachineMappingParser<Validation>::kNetworkMaskKey;

/*!
 * \brief Definition of prefix length key.
 */
template <typename Validation>
constexpr vac::container::CStringView MachineMappingParser<Validation>::kPrefixLengthKey;

/*!
 * \brief Definition of service discovery key.
 */
template <typename Validation>
constexpr vac::container::CStringView MachineMappingParser<Validation>::kServiceDiscoveryEnabledKey;

/*!
 * \brief Definition of communication key.
 */
template <typename Validation>
constexpr vac::container::CStringView MachineMappingParser<Validation>::kCommunicationEnabledKey;

/*!
 * \brief Definition of provider key.
 */
template <typename Validation>
constexpr vac::container::CStringView MachineMappingParser<Validation>::kProviderKey;

/*!
 * \brief Definition of subscribers key.
 */
template <typename Validation>
constexpr vac::container::CStringView MachineMappingParser<Validation>::kSubscribersKey;

/*!
 * \brief Definition of provided signal serialized PDUs key.
 */
template <typename Validation>
constexpr vac::container::CStringView MachineMappingParser<Validation>::kProvidedPdusKey;

/*!
 * \brief Definition of kUdpCollectionBufferSizeThresholdKey.
 */
template <typename Validation>
constexpr vac::container::CStringView MachineMappingParser<Validation>::kUdpCollectionBufferSizeThresholdKey;

/*!
 * \brief Definition of event group UDP endpoint address key.
 */
template <typename Validation>
constexpr vac::container::CStringView MachineMappingParser<Validation>::kEventGroupUdpEndpointAddressKey;

/*!
 * \brief Definition of event group TCP endpoint address key.
 */
template <typename Validation>
constexpr vac::container::CStringView MachineMappingParser<Validation>::kEventGroupTcpEndpointAddressKey;

/*!
 * \brief Definition of event group UDP endpoint port key.
 */
template <typename Validation>
constexpr vac::container::CStringView MachineMappingParser<Validation>::kEventGroupUdpEndpointPortKey;

/*!
 * \brief Definition of event group TCP endpoint port key.
 */
template <typename Validation>
constexpr vac::container::CStringView MachineMappingParser<Validation>::kEventGroupTcpEndpointPortKey;

/*!
 * \brief Definition of secure connection IDs key.
 */
template <typename Validation>
constexpr vac::container::CStringView MachineMappingParser<Validation>::kSecureConnectionIds;

}  // namespace parsing
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_PARSING_MACHINE_MAPPING_PARSER_H_
