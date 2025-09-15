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
/**        \file  someipbinding_skeleton_event_xf_initializer.h
 *        \brief  SomeIpBinding Skeleton Event Xf Initializer
 *        \unit   SomeIpBinding::SomeIpBinding::BindingXfInitializer
 *
 *      \details  Helper class for SomeIpBinding Skeleton Event Xf instances initialization.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_SOMEIPBINDING_SKELETON_EVENT_XF_INITIALIZER_H_
#define LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_SOMEIPBINDING_SKELETON_EVENT_XF_INITIALIZER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include <map>
#include "amsr/core/optional.h"
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
 * \brief Static helper class for initialization of SomeIpBinding Skeleton Event Xf instances.
 */
class SomeIpBindingSkeletonEventXfInitializer final {
 public:
  /*!
   * \brief Map used to contain events E2E protection Props.
   */
  using E2EPropsMap =
      std::map<::amsr::someip_protocol::internal::EventId,
               std::reference_wrapper<::amsr::someip_binding::internal::configuration::E2EPropsConfig const>>;

  /*!
   * \brief Create SkeletonEventXfE2EParametrizationData from configuration
   * \details To be used while constructing the corresponding event xf object
   *
   * \param[in] event_config           Reference to event config
   * \param[in] e2e_props_map          Reference to e2e props map
   * \param[in] e2e_profile_config_map Reference to e2e profile config map
   *
   * \return ara core optional of SkeletonEventXfE2EParametrizationData.
   *
   * \pre         -
   * \context     App
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   *
   * \internal
   * - Search the e2e props map for an element with the passed event id.
   * - If element was found:
   *    - Search the e2e profile map for an element with the found e2e profile name of the e2e props element.
   *      - If no e2e profile was found: Abort.
   *      - Else: Return Optional with valid value.
   * - Return empty Optional.
   * \endinternal
   */
  // VECTOR NC Metric-HIS.RETURN: MD_SOMEIPBINDING_Metric-HIS.RETURN_returned_optional_contains_consts
  // VECTOR NC VectorC++-V6.6.1: MD_SOMEIPBINDING_VectorC++-V6.6.1_returned_optional_contains_consts
  static ::amsr::core::Optional<::amsr::someip_binding::internal::configuration::SkeletonEventXfE2EParametrizationData>
  GetSkeletonEventXfE2EParametrizationData(
      ::amsr::someip_binding::internal::configuration::EventConfig const& event_config,
      E2EPropsMap const& e2e_props_map,
      ::amsr::someip_binding::internal::configuration::SomeIpBindingConfig::E2EProfileMap const&
          e2e_profile_config_map) noexcept {
    // VCA_SLC_20_SOMEIPBINDING_FUNCTION_IS_DEFINED
    E2EPropsMap::const_iterator const it_props_map{e2e_props_map.find(event_config.GetEventId())};
    if (it_props_map != e2e_props_map.end()) {
      ::amsr::someip_binding::internal::configuration::E2EPropsConfig::E2EProfileShortname const e2e_profile_shortname{
          // VCA_SOMEIPBINDING_E2E_PROPS_CONFIG_LIFECYCLE
          // VCA_SLC_20_SOMEIPBINDING_FUNCTION_IS_DEFINED
          it_props_map->second.get().GetE2EProfileShortname()};
      ::amsr::someip_binding::internal::configuration::SomeIpBindingConfig::E2EProfileMap::const_iterator const
          it_profile_map{e2e_profile_config_map.find(e2e_profile_shortname)};
      if (it_profile_map == e2e_profile_config_map.end()) {
        ::amsr::someip_binding_core::internal::logging::AraComLoggerSingleton::GetInstance()
            .GetAccess()
            ->LogFatalAndAbort(
                ::amsr::core::StringView{"SomeIpBindingSkeletonEventXfInitializer"},
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
      return amsr::core::make_optional<
          ::amsr::someip_binding::internal::configuration::SkeletonEventXfE2EParametrizationData>(
          // VCA_SOMEIPBINDING_E2E_PROPS_CONFIG_LIFECYCLE
          // VCA_SLC_20_SOMEIPBINDING_FUNCTION_IS_DEFINED
          {it_props_map->second.get().GetEnd2EndEventProtectionProps(serialization), e2e_profile});
    }

    // VCA_SOMEIPBINDING_EMPTY_OPTIONAL
    return amsr::core::nullopt;
  }

  /*!
   * \brief Create SkeletonEventXfE2EParametrizationData from configuration
   * \details To be used while constructing the corresponding field notifier xf object
   *
   * \param[in] notifier_config        Reference to event config
   * \param[in] e2e_props_map          Reference to e2e props map
   * \param[in] e2e_profile_config_map Reference to e2e profile config map
   *
   * \return ara core optional of SkeletonEventXfE2EParametrizationData.
   *
   * \pre         -
   * \context     App
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Search the e2e props map for an element with the passed notifier id.
   * - If element was found:
   *    - Search the e2e profile map for an element with the found e2e profile name of the e2e props element.
   *      - If no e2e profile was found: Abort.
   *      - Else: Return Optional with valid value.
   * - Return empty Optional.
   * \endinternal
   */
  // VECTOR NC Metric-HIS.RETURN: MD_SOMEIPBINDING_Metric-HIS.RETURN_returned_optional_contains_consts
  // VECTOR NC VectorC++-V6.6.1: MD_SOMEIPBINDING_VectorC++-V6.6.1_returned_optional_contains_consts
  static ::amsr::core::Optional<::amsr::someip_binding::internal::configuration::SkeletonEventXfE2EParametrizationData>
  GetSkeletonEventXfE2EParametrizationData(
      ::amsr::someip_binding::internal::configuration::NotifierConfig const& notifier_config,
      E2EPropsMap const& e2e_props_map,
      ::amsr::someip_binding::internal::configuration::SomeIpBindingConfig::E2EProfileMap const&
          e2e_profile_config_map) noexcept {
    E2EPropsMap::const_iterator const it_props_map{e2e_props_map.find(notifier_config.GetNotifierId())};
    if (it_props_map != e2e_props_map.end()) {
      ::amsr::someip_binding::internal::configuration::E2EPropsConfig::E2EProfileShortname const e2e_profile_shortname{
          // VCA_SOMEIPBINDING_E2E_PROPS_CONFIG_LIFECYCLE
          // VCA_SLC_20_SOMEIPBINDING_FUNCTION_IS_DEFINED
          it_props_map->second.get().GetE2EProfileShortname()};
      ::amsr::someip_binding::internal::configuration::SomeIpBindingConfig::E2EProfileMap::const_iterator const
          it_profile_map{e2e_profile_config_map.find(e2e_profile_shortname)};
      if (it_profile_map == e2e_profile_config_map.end()) {
        ::amsr::someip_binding_core::internal::logging::AraComLoggerSingleton::GetInstance()
            .GetAccess()
            ->LogFatalAndAbort(
                ::amsr::core::StringView{"SomeIpBindingSkeletonEventXfInitializer"},
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
      return amsr::core::make_optional<
          ::amsr::someip_binding::internal::configuration::SkeletonEventXfE2EParametrizationData>(
          // VCA_SOMEIPBINDING_E2E_PROPS_CONFIG_LIFECYCLE
          // VCA_SLC_20_SOMEIPBINDING_FUNCTION_IS_DEFINED
          {it_props_map->second.get().GetEnd2EndEventProtectionProps(serialization), e2e_profile});
    }

    // VCA_SOMEIPBINDING_EMPTY_OPTIONAL
    return amsr::core::nullopt;
  }
};

}  // namespace internal
}  // namespace someip_binding
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_INCLUDE_AMSR_SOMEIP_BINDING_INTERNAL_SOMEIPBINDING_SKELETON_EVENT_XF_INITIALIZER_H_
