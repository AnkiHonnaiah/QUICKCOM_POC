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
/*!        \file  someip_binding_xf/internal/skeleton_xf_factory.h
 *        \brief  Transformation layer factory at skeleton side to handle SkeletonXf creation.
 *         \unit  SomeIpBinding::SomeIpBindingXf::SkeletonXfFactory
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_SKELETON_XF_FACTORY_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_SKELETON_XF_FACTORY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "amsr/e2e/profiles/end2end_event_protection_props.h"
#include "amsr/socal/internal/factory/skeleton_backend_factory_interface.h"
#include "amsr/someip_binding/internal/configuration/config_wrapper_interface.h"
#include "amsr/someip_binding_core/internal/local_server_manager.h"
#include "amsr/someip_binding_core/internal/logging/logger_prefix_generator.h"
#include "amsr/someip_binding_core/internal/safe_ipc_polling_interface.h"
#include "amsr/someip_binding_core/internal/server_transport_to_daemon_interface.h"
#include "amsr/someip_binding_core/internal/someip_binding_core_interface.h"
#include "ara/com/com_error_domain.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {

/*!
 * \brief   Factory for creation of transformation layer at skeleton side.
 *
 * \tparam SkeletonXf                       class for Communication transformation between the SOME/IP server and the
 *                                          ara::com skeleton.
 */
template <typename SkeletonXf>
class SkeletonXfFactory final : public ::amsr::socal::internal::factory::SkeletonBackendFactoryInterface<
                                    typename SkeletonXf::SkeletonBackendInterface> {
 public:
  /*!
   * \brief Map used to contain events E2E protection Props.
   */
  using E2EPropsMap =
      std::map<::amsr::someip_protocol::internal::EventId,
               std::reference_wrapper<::amsr::someip_binding::internal::configuration::E2EPropsConfig const>>;

  /*!
   * \brief Type alias for the skeleton backend interface pointer.
   */
  using SkeletonBackendInterfaceUniquePtrType = std::unique_ptr<typename SkeletonXf::SkeletonBackendInterface>;

  /*!
   * \brief Type alias for SkeletonBackendFactoryInterface.
   */
  using SkeletonBackendFactoryInterface =
      ::amsr::socal::internal::factory::SkeletonBackendFactoryInterface<typename SkeletonXf::SkeletonBackendInterface>;

  /*!
   * \brief Type-alias for the SafeIpcPollingInterface.
   */
  using SafeIpcPollingInterface = ::amsr::someip_binding_core::internal::SafeIpcPollingInterface;

  /*!
   * \brief         Constructor
   *
   * \param[in] someip_binding_core     The reference to SOME/IP Binding core.
   *                                    The object must be valid for SkeletonXf's entire life time.
   * \param[in] service_instance_id     The reference to service instance identifier.
   *                                    The object must be valid for SkeletonXf's entire life time.
   * \param[in] e2e_props_map           E2E Event properties for all E2E Events (can be empty if no E2E events are
   *                                    configured).
   * \param[in] config_wrapper          Reference to the configuration wrapper
   *                                    The object must be valid for SkeletonXf's entire life time.
   * \param[in] e2e_profile_config_map  Reference to the E2EProfileConfig map
   *                                    The object must be valid for SkeletonXf's entire life time.
   * \param[in] safe_ipc_polling        Contains poll function, used to trigger the reactor to receive responses
   *
   * \context       Init
   *
   * \pre           -
   */
  SkeletonXfFactory(
      ::amsr::someip_binding_core::internal::SomeIpBindingCoreInterface& someip_binding_core,
      ::amsr::someip_binding_core::internal::ProvidedServiceInstanceId const& service_instance_id,
      std::map<::amsr::someip_protocol::internal::EventId,
               std::reference_wrapper<::amsr::someip_binding::internal::configuration::E2EPropsConfig const>> const&
          e2e_props_map,
      ::amsr::someip_binding::internal::configuration::ConfigWrapperInterface const& config_wrapper,
      ::amsr::someip_binding::internal::configuration::SomeIpBindingConfig::E2EProfileMap const& e2e_profile_config_map,
      std::shared_ptr<SafeIpcPollingInterface> safe_ipc_polling) noexcept
      : SkeletonBackendFactoryInterface{},
        someip_binding_core_{someip_binding_core},
        service_instance_id_{service_instance_id},
        // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
        e2e_props_map_{e2e_props_map},
        config_wrapper_{config_wrapper},
        e2e_profile_config_map_{e2e_profile_config_map},
        // VCA_SOMEIPBINDING_PASSING_REFERENCE
        safe_ipc_polling_{safe_ipc_polling} {
    // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
    logger_.LogVerbose(static_cast<char const*>(__func__), __LINE__);
  }

  SkeletonXfFactory() = delete;

  SkeletonXfFactory(SkeletonXfFactory const&) = delete;
  SkeletonXfFactory& operator=(SkeletonXfFactory const&) = delete;
  SkeletonXfFactory(SkeletonXfFactory&&) noexcept = delete;
  SkeletonXfFactory& operator=(SkeletonXfFactory&&) noexcept = delete;

  /*!
   * \brief       Destructor.
   *
   * \context     Shutdown
   *
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
  ~SkeletonXfFactory() noexcept final = default;

  /*!
   * \brief       Create SkeletonXf (transformation class for skeleton side communication).
   *
   * \return      Result containing the SkeletonXf unique pointer.
   *
   * \error       ComErrc::kGrantEnforcementError If the request is refused by IAM.
   * \error       ComErrc::kCommunicationLinkError Daemon connection did not succeed.
   *
   * \pre         Cannot be called after the captured SomeIpBindingCore was deinitialized, otherwise undefined behavior.
   *
   * \context     App
   *
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vprivate Product Private
   */
  ::amsr::core::Result<SkeletonBackendInterfaceUniquePtrType> CreateBackend() noexcept final {
    logger_.LogVerbose(static_cast<char const*>(__func__), __LINE__);

    amsr::core::Result<SkeletonBackendInterfaceUniquePtrType> result{::ara::com::ComErrc::kErrorNotOk};

    // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
    if (someip_binding_core_.SomeIpDaemonConnectionSucceeded()) {
      // NOTE: do not call RequestLocalServer if there is no connection as this will lead to an abort
      amsr::core::Result<::amsr::someip_binding_core::internal::LocalServer> request_result{
          // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
          someip_binding_core_.RequestLocalServer(service_instance_id_)};

      if (request_result.HasValue()) {
        // VECTOR NL AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10_A18.5.8_false_positive
        SkeletonBackendInterfaceUniquePtrType skeleton_xf_ptr{
            // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
            std::make_unique<SkeletonXf>(service_instance_id_, std::move(request_result.Value()), e2e_props_map_,
                                         config_wrapper_, e2e_profile_config_map_, safe_ipc_polling_)};

        result.EmplaceValue(std::move(skeleton_xf_ptr));
      } else if (request_result.Error() ==
                 ::amsr::someip_binding_core::internal::ServerError::kSystemPrivilegesNoAccess) {
        result.EmplaceError(::ara::com::ComErrc::kGrantEnforcementError);
      } else {
        logger_.LogFatalAndAbort(
            [](::amsr::someip_binding_core::internal::logging::StringStream& s) {
              s << "Unknown error while requesting local server during SkeletonXf construction.";
            },
            static_cast<char const*>(__func__), __LINE__);
      }
    } else {
      // Pass connection error to the upper layer, so the user can write application logic to retry
      result.EmplaceError(::ara::com::ComErrc::kCommunicationLinkError);
    }

    return result;
  }

 private:
  /*!
   *  \brief Reference to the SOME/IP Binding Core.
   */
  ::amsr::someip_binding_core::internal::SomeIpBindingCoreInterface& someip_binding_core_;

  /*!
   * \brief   Service discovery service instance identifier.
   * \details This ID is extracted from the model (id of the SomeIpProvidedServiceInstance).
   */
  ::amsr::someip_binding_core::internal::ProvidedServiceInstanceId const service_instance_id_;

  /*!
   * \brief E2E Event properties for all E2E Events (can be empty if no E2E events are configured).
   */
  std::map<::amsr::someip_protocol::internal::EventId,
           std::reference_wrapper<::amsr::someip_binding::internal::configuration::E2EPropsConfig const>> const
      e2e_props_map_;

  /*!
   * \brief Reference to the configuration wrapper.
   */
  amsr::someip_binding::internal::configuration::ConfigWrapperInterface const& config_wrapper_;

  /*!
   * \brief Reference to the E2EProfileConfig map.
   */
  ::amsr::someip_binding::internal::configuration::SomeIpBindingConfig::E2EProfileMap const& e2e_profile_config_map_;

  /*!
   * \brief Interface for polling wrappers which contains reactor polling function, it is called while waiting for
   * a command response.
   */
  std::shared_ptr<SafeIpcPollingInterface> safe_ipc_polling_;

  /*!
   * \brief Logger Prefix.
   */
  amsr::core::String const logger_prefix_{
      someip_binding_core::internal::logging::LoggerPrefixGenerator::GetLoggerPrefix(
          "SkeletonXfFactory"_sv, service_instance_id_.GetServiceId(), service_instance_id_.GetInstanceId(),
          service_instance_id_.GetMajorVersion(), service_instance_id_.GetMinorVersion())};

  /*!
   * \brief The logger.
   */
  someip_binding_core::internal::logging::AraComLogger const logger_{
      someip_binding_core::internal::logging::kSomeIpLoggerContextId,
      someip_binding_core::internal::logging::kSomeIpLoggerContextDescription, logger_prefix_};
};

}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_SKELETON_XF_FACTORY_H_
