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
/**        \file  someipbinding_proxy_event_xf_initializer.h
 *        \brief  SomeIpBinding Proxy Event Xf Initializer
 *        \unit   SomeIpBinding::SomeIpBinding::BindingXfInitializer
 *
 *      \details  Helper class for SomeIpBinding Proxy Event Xf instances initialization.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_SOMEIPBINDING_PROXY_EVENT_XF_INITIALIZER_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_SOMEIPBINDING_PROXY_EVENT_XF_INITIALIZER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <memory>
#include "amsr/core/optional.h"
#include "amsr/e2e/profiles/e2e_profile_configuration.h"
#include "amsr/e2e/profiles/end2end_event_protection_props.h"
#include "amsr/e2e/profiles/profile.h"
#include "amsr/someip_binding/internal/configuration/config_types.h"
#include "amsr/someip_binding/internal/configuration/e2e_props_config.h"
#include "amsr/someip_binding/internal/configuration/event_config.h"
#include "amsr/someip_binding/internal/configuration/notifier_config.h"
#include "amsr/someip_binding/internal/configuration/someipbinding_config.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger_singleton.h"
#include "amsr/someip_binding_xf/internal/e2e/client_side_transformer.h"

namespace amsr {
namespace someip_binding {
namespace internal {

// VECTOR NC AutosarC++17_10-M7.3.6, VectorC++-V5.0.1: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.6_using_directive
// VECTOR NC AutosarC++17_10-M7.3.4, VectorC++-V5.0.1: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.4_using_directive
using namespace ::vac::container::literals;  // NOLINT(build/namespaces)

/*!
 * \brief Static helper class for initialization of SomeIpBinding Proxy Event Xf instances.
 */
class SomeIpBindingProxyEventXfInitializer final {
 public:
  /*!
   * \brief Type alias for E2EProfileMap.
   */
  using E2EProfileMap = ::amsr::someip_binding::internal::configuration::SomeIpBindingConfig::E2EProfileMap;

  /*!
   * \brief Map used to contain events E2E protection Props.
   */
  using E2EPropsMap =
      std::map<::amsr::someip_protocol::internal::EventId,
               std::reference_wrapper<::amsr::someip_binding::internal::configuration::E2EPropsConfig const>>;

  /*!
   * \brief Create ProxyEventXfE2EParametrizationData from configuration
   * \details To be used while constructing the corresponding event Xf object
   *
   * \param[in] event_config           Reference to event config
   * \param[in] e2e_props_map          Reference to e2e props map
   * \param[in] e2e_profile_config_map Reference to e2e profile config map
   *
   * \return ara core optional of ProxyEventXfE2EParametrizationData.
   *
   * \pre         -
   * \context     App
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Create empty result.
   * - Search the e2e props map for an element with the passed event id.
   * - If element was found:
   *    - Search the e2e profile map for an element with the found e2e profile name of the e2e props element.
   *      - If no e2e profile was found: Abort.
   *      - Else: Assign result with valid value.
   * - Return result.
   * \endinternal
   */
  static ::amsr::core::Optional<::amsr::someip_binding::internal::configuration::ProxyEventXfE2EParametrizationData>
  GetProxyEventXfE2EParametrizationData(
      ::amsr::someip_binding::internal::configuration::EventConfig const& event_config,
      E2EPropsMap const& e2e_props_map, E2EProfileMap const& e2e_profile_config_map) {
    // VCA_SLC_20_SOMEIPBINDING_FUNCTION_IS_DEFINED
    E2EPropsMap::const_iterator const e2e_props_it{e2e_props_map.find(event_config.GetEventId())};
    ::amsr::core::Optional<::amsr::someip_binding::internal::configuration::ProxyEventXfE2EParametrizationData>
        result{};  // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
    if (e2e_props_it != e2e_props_map.end()) {
      ::amsr::someip_binding::internal::configuration::E2EPropsConfig::E2EProfileShortname const e2e_profile_shortname{
          // VCA_SOMEIPBINDING_E2E_PROPS_CONFIG_LIFECYCLE
          // VCA_SLC_20_SOMEIPBINDING_FUNCTION_IS_DEFINED
          e2e_props_it->second.get().GetE2EProfileShortname()};

      // VCA_SOMEIPBINDING_E2E_PROPS_CONFIG_LIFECYCLE
      // VCA_SLC_20_SOMEIPBINDING_FUNCTION_IS_DEFINED
      std::size_t const e2e_protected_length{e2e_props_it->second.get().GetDataLength()};

      E2EProfileMap::const_iterator const it_profile_map{e2e_profile_config_map.find(e2e_profile_shortname)};
      if (it_profile_map == e2e_profile_config_map.end()) {
        ::amsr::someip_binding_core::internal::logging::AraComLoggerSingleton::GetInstance()
            .GetAccess()
            ->LogFatalAndAbort(
                ::amsr::core::StringView{"SomeIpBindingProxyEventXfInitializer"},
                [&e2e_profile_shortname](::amsr::someip_binding_core::internal::logging::StringStream& s) {
                  s << "No e2e profile configuration for ";
                  s << e2e_profile_shortname.shortname;
                  s << " could be found.";
                },
                static_cast<char const*>(__func__), __LINE__);
      }
      // VCA_SOMEIPBINDING_E2E_PROFILE_CONFIG_LIFECYCLE
      // VCA_SLC_20_SOMEIPBINDING_FUNCTION_IS_DEFINED
      ::amsr::e2e::profiles::Profile const e2e_profile{it_profile_map->second.GetE2EProfile()};

      ::amsr::someip_binding::internal::configuration::EventConfig::Serialization const serialization{
          // VCA_SLC_20_SOMEIPBINDING_FUNCTION_IS_DEFINED
          event_config.GetSerialization()};
      // VCA_SLC_20_SOMEIPBINDING_FUNCTION_IS_DEFINED
      bool const is_e2e_disabled{event_config.GetDisableE2eCheck().value_or(false)};

      std::size_t e2e_protected_offset{0};
      // VCA_SLC_20_SOMEIPBINDING_FUNCTION_IS_DEFINED
      if (event_config.GetE2EProtectedOffset().has_value()) {
        // VCA_SLC_20_SOMEIPBINDING_FUNCTION_IS_DEFINED
        e2e_protected_offset = event_config.GetE2EProtectedOffset().value();
      }
      // VCA_SLC_20_SOMEIPBINDING_FUNCTION_IS_DEFINED
      amsr::core::Optional<std::size_t> const e2e_update_bit_position{event_config.GetE2EUpdateBitPosition()};

      bool has_signal_based_e2e_range{false};
      // VCA_SLC_20_SOMEIPBINDING_FUNCTION_IS_DEFINED
      if (event_config.GetHasSignalBasedE2eRange().has_value()) {
        // VCA_SLC_20_SOMEIPBINDING_FUNCTION_IS_DEFINED
        has_signal_based_e2e_range = event_config.GetHasSignalBasedE2eRange().value();
      }

      ::amsr::e2e::profiles::End2EndEventProtectionProps protection_properties{
          // VCA_SOMEIPBINDING_E2E_PROPS_CONFIG_LIFECYCLE
          // VCA_SLC_20_SOMEIPBINDING_FUNCTION_IS_DEFINED
          e2e_props_it->second.get().GetEnd2EndEventProtectionProps(serialization)};

      ::amsr::e2e::profiles::E2EProfileConfiguration protection_configuration{
          // VCA_SOMEIPBINDING_E2E_PROFILE_CONFIG_LIFECYCLE
          // VCA_SLC_20_SOMEIPBINDING_FUNCTION_IS_DEFINED
          // VCA_SOMEIPBINDING_MISS_OF_INFORMATION_OF_POINTER
          it_profile_map->second.GetE2EProfileConfiguration()};

      std::unique_ptr<::amsr::someip_binding_xf::internal::e2e::ClientSideTransformerInterface>
          // VCA_SOMEIPBINDING_PASSING_REFERENCE
          client_side_transformer_interface{
              // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
              // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
              // VCA_SOMEIPBINDING_PASSING_REFERENCE
              std::make_unique<::amsr::someip_binding_xf::internal::e2e::ClientSideTransformer>(
                  e2e_profile, std::move(protection_properties), std::move(protection_configuration))};

      // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
      // VCA_SOMEIPBINDING_PASSING_REFERENCE
      result.emplace(::amsr::someip_binding::internal::configuration::ProxyEventXfE2EParametrizationData{
          // VCA_SOMEIPBINDING_PASSING_REFERENCE
          std::move(client_side_transformer_interface), e2e_profile, is_e2e_disabled, e2e_update_bit_position,
          e2e_protected_offset, e2e_protected_length, has_signal_based_e2e_range});
    }  // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
    return result;
  }

  /*!
   * \brief Create ProxyEventXfE2EParametrizationData from configuration
   * \details To be used while constructing the corresponding field notifier Xf object
   *
   * \param[in] notifier_config        Reference to field notifier config
   * \param[in] e2e_props_map          Reference to e2e props map
   * \param[in] e2e_profile_config_map Reference to e2e profile config map
   *
   * \return ara core optional of ProxyEventXfE2EParametrizationData.
   *
   * \pre         -
   * \context     App
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Create empty result.
   * - Search the e2e props map for an element with the passed field notifier id.
   * - If element was found:
   *    - Search the e2e profile map for an element with the found e2e profile name of the e2e props element.
   *      - If no e2e profile was found: Abort.
   *      - Else: Assign result with valid value.
   * - Return result.
   * \endinternal
   */
  static ::amsr::core::Optional<::amsr::someip_binding::internal::configuration::ProxyEventXfE2EParametrizationData>
  GetProxyEventXfE2EParametrizationData(
      ::amsr::someip_binding::internal::configuration::NotifierConfig const& notifier_config,
      E2EPropsMap const& e2e_props_map, E2EProfileMap const& e2e_profile_config_map) {
    E2EPropsMap::const_iterator const e2e_props_it{e2e_props_map.find(notifier_config.GetNotifierId())};
    ::amsr::core::Optional<::amsr::someip_binding::internal::configuration::ProxyEventXfE2EParametrizationData>
        result{};  // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
    if (e2e_props_it != e2e_props_map.end()) {
      ::amsr::someip_binding::internal::configuration::E2EPropsConfig::E2EProfileShortname const e2e_profile_shortname{
          // VCA_SOMEIPBINDING_E2E_PROPS_CONFIG_LIFECYCLE
          // VCA_SLC_20_SOMEIPBINDING_FUNCTION_IS_DEFINED
          e2e_props_it->second.get().GetE2EProfileShortname()};

      // e2e_protected_length is not used for fields set by default to 0
      std::size_t const e2e_protected_length{0};

      E2EProfileMap::const_iterator const it_profile_map{e2e_profile_config_map.find(e2e_profile_shortname)};
      if (it_profile_map == e2e_profile_config_map.end()) {
        ::amsr::someip_binding_core::internal::logging::AraComLoggerSingleton::GetInstance()
            .GetAccess()
            ->LogFatalAndAbort(
                ::amsr::core::StringView{"SomeIpBindingProxyEventXfInitializer"},
                [&e2e_profile_shortname](::amsr::someip_binding_core::internal::logging::StringStream& s) {
                  s << "No e2e profile configuration for ";
                  s << e2e_profile_shortname.shortname;
                  s << " could be found.";
                },
                static_cast<char const*>(__func__), __LINE__);
      }
      // VCA_SOMEIPBINDING_E2E_PROFILE_CONFIG_LIFECYCLE
      // VCA_SLC_20_SOMEIPBINDING_FUNCTION_IS_DEFINED
      ::amsr::e2e::profiles::Profile const e2e_profile{it_profile_map->second.GetE2EProfile()};
      ::amsr::someip_binding::internal::configuration::EventConfig::Serialization const serialization{
          ::amsr::someip_binding::internal::configuration::EventConfig::Serialization::someip};
      bool const is_e2e_disabled{notifier_config.GetDisableE2eCheck().value_or(false)};

      // e2e_update_bit_position, e2e_protected_offset and has_signal_based_e2e_range are not used for fields set by
      amsr::core::Optional<std::size_t> const e2e_update_bit_position{amsr::core::nullopt};
      std::size_t const e2e_protected_offset{0};
      bool const has_signal_based_e2e_range{false};

      ::amsr::e2e::profiles::End2EndEventProtectionProps protection_properties{
          // VCA_SOMEIPBINDING_E2E_PROPS_CONFIG_LIFECYCLE
          // VCA_SLC_20_SOMEIPBINDING_FUNCTION_IS_DEFINED
          e2e_props_it->second.get().GetEnd2EndEventProtectionProps(serialization)};

      ::amsr::e2e::profiles::E2EProfileConfiguration protection_configuration{
          // VCA_SOMEIPBINDING_E2E_PROFILE_CONFIG_LIFECYCLE
          // VCA_SLC_20_SOMEIPBINDING_FUNCTION_IS_DEFINED
          // VCA_SOMEIPBINDING_MISS_OF_INFORMATION_OF_POINTER
          it_profile_map->second.GetE2EProfileConfiguration()};

      std::unique_ptr<::amsr::someip_binding_xf::internal::e2e::ClientSideTransformerInterface>
          // VCA_SOMEIPBINDING_PASSING_REFERENCE
          client_side_transformer_interface{
              // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
              // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
              // VCA_SOMEIPBINDING_PASSING_REFERENCE
              std::make_unique<::amsr::someip_binding_xf::internal::e2e::ClientSideTransformer>(
                  e2e_profile, std::move(protection_properties), std::move(protection_configuration))};

      // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
      // VCA_SOMEIPBINDING_PASSING_REFERENCE
      result.emplace(::amsr::someip_binding::internal::configuration::ProxyEventXfE2EParametrizationData{
          // VCA_SOMEIPBINDING_PASSING_REFERENCE
          std::move(client_side_transformer_interface), e2e_profile, is_e2e_disabled, e2e_update_bit_position,
          e2e_protected_offset, e2e_protected_length, has_signal_based_e2e_range});
    }  // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
    return result;
  }
};

}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_SOMEIPBINDING_PROXY_EVENT_XF_INITIALIZER_H_
