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
/**        \file  someip_binding_xf/internal/service_discovery/service_discovery_proxy_xf.h
 *        \brief  Transformation layer at proxy side to handle Service discovery.
 *        \unit   SomeIpBinding::SomeIpBindingXf::ServiceDiscovery::ServiceDiscoveryProxyXf
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_SERVICE_DISCOVERY_SERVICE_DISCOVERY_PROXY_XF_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_SERVICE_DISCOVERY_SERVICE_DISCOVERY_PROXY_XF_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <memory>
#include <utility>

#include "amsr/core/result.h"
#include "amsr/core/string.h"
#include "amsr/e2e/profiles/end2end_event_protection_props.h"
#include "amsr/generic/singleton_wrapper.h"
#include "amsr/iostream/stateful_output_stream.h"
#include "amsr/iostream/stringstream/output_string_stream.h"
#include "amsr/socal/internal/factory/proxy_backend_factory_interface.h"
#include "amsr/socal/internal/internal_instance_identifier.h"
#include "amsr/socal/internal/service_discovery/proxy_service_discovery.h"
#include "amsr/someip_binding/internal/configuration/config_types.h"
#include "amsr/someip_binding/internal/configuration/config_wrapper_interface.h"
#include "amsr/someip_binding/internal/configuration/someipbinding_config.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_core/internal/logging/logger_prefix_generator.h"
#include "amsr/someip_binding_core/internal/required_service_instance.h"
#include "amsr/someip_binding_core/internal/service_discovery/service_listener_interface.h"
#include "amsr/someip_binding_core/internal/someip_binding_client_error.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "amsr/someip_binding_xf/internal/ara_com_instance_id_xf.h"
#include "amsr/someip_binding_xf/internal/service_discovery/service_discovery_listener.h"
#include "ara/com/com_error_domain.h"
#include "someip-protocol/internal/message.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace service_discovery {

/*!
 * \brief   Transformation layer at proxy side to handle Service discovery.
 * \details This class:
 *           - Represents one SomeIpRequiredServiceInstance.
 *           - It finds the matching services in SOMEe/IP.
 *           - and creates the ProxyXf (acts as skeleton) and offers it to Socal.
 *
 * \tparam  ProxyXf                    Communication transformation instance between the SOME/IP client and the
 *                                     ara::com proxy.
 * \tparam  ProxyBackendInterface      (Binding independent) Interface of ProxyXf that is used by Socal.
 * \tparam  ServiceInterfaceProxyType  The type of the ara::com proxy (only modified for testing).
 */
template <typename ProxyXf, typename ProxyBackendInterface, typename ServiceInterfaceProxyType>
class ServiceDiscoveryProxyXf final
    : public ::amsr::someip_binding_core::internal::service_discovery::ServiceListenerInterface,
      public ::amsr::socal::internal::factory::ProxyBackendFactoryInterface<ProxyBackendInterface> {
 public:
  /*!
   * \brief Alias for service discovery service instance identifier.
   */
  using RequiredServiceInstanceId = ::amsr::someip_binding_core::internal::RequiredServiceInstanceId;

  /*!
   * \brief Map used to contain events E2E protection Props.
   */
  using E2EPropsMap =
      std::map<::amsr::someip_protocol::internal::EventId,
               std::reference_wrapper<::amsr::someip_binding::internal::configuration::E2EPropsConfig const>>;

  /*!
   * \brief Type alias for ProxyBackendFactoryInterface.
   */
  using ProxyBackendFactoryInterface =
      ::amsr::socal::internal::factory::ProxyBackendFactoryInterface<ProxyBackendInterface>;

  /*!
   * \brief Constructor
   * \param[in] service_instance_id       The reference to service instance identifier.
   * \param[in] e2e_props_map             E2E Event properties for all E2E Events (can be empty if no E2E events are
   *                                      configured).
   * \param[in] config_wrapper            Reference to the configuration wrapper
   * \param[in] e2e_profile_config_map    Reference to the E2EProfileConfig map
   * \param[in] rsi                       The required service instance
   *
   * \context       Init
   * \pre           -
   *
   * \internal
   * - Register as a listener to the binding for service updates.
   * \endinternal
   */
  ServiceDiscoveryProxyXf(
      RequiredServiceInstanceId const& service_instance_id, E2EPropsMap const& e2e_props_map,
      ::amsr::someip_binding::internal::configuration::ConfigWrapperInterface const& config_wrapper,
      ::amsr::someip_binding::internal::configuration::SomeIpBindingConfig::E2EProfileMap const& e2e_profile_config_map,
      ::amsr::someip_binding_core::internal::RequiredServiceInstance&& rsi) noexcept
      : ServiceListenerInterface{},
        ProxyBackendFactoryInterface{},
        service_instance_id_{service_instance_id},
        // VCA_SLC_20_SOMEIPBINDING_FUNCTION_IS_DEFINED
        required_instance_id_{::amsr::someip_binding_xf::internal::AraComInstanceIdXf::BuildAraComInstanceIdentifier(
            service_instance_id.GetServiceId(), service_instance_id.GetMajorVersion(),
            service_instance_id.GetInstanceId())},
        // VCA_SOMEIPBINDING_PASSING_REFERENCE
        sd_{ServiceInterfaceProxyType::GetServiceDiscovery()},
        // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
        e2e_props_map_{e2e_props_map},
        config_wrapper_{config_wrapper},
        e2e_profile_config_map_{e2e_profile_config_map},
        rsi_{std::move(rsi)} {
    // VCA_SOMEIPBINDING_SERVICEDISCOVERY_PROXY_XF_OBJECT_LIFECYCLE
    logger_.LogVerbose(static_cast<char const*>(__func__), __LINE__);
    // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    rsi_.RegisterListener(sd_listener_);
  }

  ServiceDiscoveryProxyXf() = delete;
  ServiceDiscoveryProxyXf(ServiceDiscoveryProxyXf const&) = delete;
  ServiceDiscoveryProxyXf(ServiceDiscoveryProxyXf&&) = delete;
  ServiceDiscoveryProxyXf& operator=(ServiceDiscoveryProxyXf const&) = delete;
  ServiceDiscoveryProxyXf& operator=(ServiceDiscoveryProxyXf&&) = delete;

  /*!
   * \brief Destructor.
   * \context  Shutdown
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Deregister from service updates.
   * \endinternal
   */
  // VCA_SOMEIPBINDING_SERVICEDISCOVERY_PROXY_XF_OBJECT_LIFECYCLE
  ~ServiceDiscoveryProxyXf() noexcept final {
    logger_.LogVerbose(static_cast<char const*>(__func__), __LINE__);
    // VCA_SLC_20_SOMEIPBINDING_FUNCTION_OUTSIDE_OF_ANALYZED_SCOPE_POINTER_PARAMETER
    rsi_.UnregisterListener(sd_listener_);
  }

 private:
  /*!
   * \brief Creates the proxy backend.
   *
   * \param[in] provided_instance_id A provided instance identifier which is used for the construction of a
   *                                 SOME/IP ProxyXf. When the identifier is not a valid SOME/IP instance
   *                                 identifier this will result in an abort.
   *
   * \return Result containing a pointer to the created proxy backend or an error.
   *         It may not be used after the deinitialization of the SOME/IP Binding, i.e.
   *         (amsr::someip_binding::internal::DeinitializeComponent()).

   * \error ComErrc::kGrantEnforcementError  If the request is refused by IAM.
   * \error ComErrc::kCommunicationLinkError If the connection to the SomeIpDaemon could not be established.
   * \error ComErrc::kNetworkBindingFailure  Any other error occurred.
   *
   * \pre -
   * \context App
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Translate Socal's ProvidedInstanceIdentifier to SOME/IP InstanceId.
   *   - Abort if this fails.
   * - Create a client for the SomeIpProvidedServiceInstance.
   * - If the client could be created
   *   - Create the ProxyXf from it and return it
   * - else return the corresponding error.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOMEIPBINDING_AutosarC++17_10-A15.4.2_A15.5.3_Exception_caught
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOMEIPBINDING_AutosarC++17_10-A15.4.2_A15.5.3_Exception_caught
  ::amsr::core::Result<std::unique_ptr<ProxyBackendInterface>> CreateBackend(
      amsr::socal::internal::ProvidedInstanceIdentifier const& provided_instance_id) noexcept final {
    ::amsr::core::Result<std::unique_ptr<ProxyBackendInterface>> result{::ara::com::ComErrc::kNetworkBindingFailure};

    ::amsr::core::Result<AraComInstanceIdXf::ParsedInstanceIdentifier> const conversion_result{
        // VCA_SLC_20_SOMEIPBINDING_FUNCTION_IS_DEFINED
        AraComInstanceIdXf::ComToBindingInstanceIdentifier(provided_instance_id.GetAraComInstanceIdentifier())};

    if (conversion_result.HasValue()) {
      AraComInstanceIdXf::ParsedInstanceIdentifier const& converted_identifier_fields{conversion_result.Value()};

      someip_binding_core::internal::ProvidedServiceInstanceId const psid{
          converted_identifier_fields.service_id, converted_identifier_fields.major_version,
          service_instance_id_.GetMinorVersion(), converted_identifier_fields.instance_id};

      // VCA_SOMEIPBINDING_UNDEFINED_FUNCTION
      amsr::core::Result<someip_binding_core::internal::LocalClient> local_client_result{rsi_.CreateClient(psid)};
      if (local_client_result.HasValue()) {
        someip_binding_core::internal::LocalClient& local_client{local_client_result.Value()};
        result.EmplaceValue(
            // VCA_SOMEIPBINDING_REFERENCE_VALID_LIFECYCLE
            std::make_unique<ProxyXf>(service_instance_id_.GetServiceId(), service_instance_id_.GetMajorVersion(),
                                      converted_identifier_fields.instance_id, e2e_props_map_, config_wrapper_,
                                      e2e_profile_config_map_, std::move(local_client)));
      } else if (local_client_result.CheckError(
                     ::amsr::someip_binding_core::internal::ClientError::kSystemPrivilegesNoAccess)) {
        logger_.LogWarn(
            [](ara::log::LogStream& s) {
              s << "Access was denied. Recovery action: Please check that this service has the correct access rights "
                   "configured.";
            },
            static_cast<char const*>(__func__), __LINE__);
        result.EmplaceError(::ara::com::ComErrc::kGrantEnforcementError);
      } else if (local_client_result.CheckError(
                     ::amsr::someip_binding_core::internal::ClientError::kRuntimeConnectionTransmissionFailed) ||
                 local_client_result.CheckError(
                     ::amsr::someip_binding_core::internal::ClientError::kRuntimeConnectionTimeout)) {
        logger_.LogWarn(
            [](ara::log::LogStream& s) {
              s << "Transmission to the SOME/IP Daemon failed. Recovery action: Restart app.";
            },
            static_cast<char const*>(__func__), __LINE__);
        result.EmplaceError(::ara::com::ComErrc::kCommunicationLinkError);
      } else if (local_client_result.CheckError(
                     ::amsr::someip_binding_core::internal::ClientError::kSystemConfigurationNotAvailable)) {
        logger_.LogFatalAndAbort(
            [](::amsr::someip_binding_core::internal::logging::StringStream& s) {
              s << "Please check that configuration (JSON) of SOME/IP Daemon and this process are compatible "
                   "(generated within same generator run).";
            },
            static_cast<char const*>(__func__), __LINE__);
      } else if (local_client_result.CheckError(
                     ::amsr::someip_binding_core::internal::ClientError::kRuntimeResourceOutOf)) {
        logger_.LogWarn(
            [](ara::log::LogStream& s) {
              s << "Transmission to the SOME/IP Daemon failed. Transmission queue size is limited and reached its "
                   "maximum. Recovery action: retry.";
            },
            static_cast<char const*>(__func__), __LINE__);
        result.EmplaceError(::ara::com::ComErrc::kNetworkBindingFailure);
      } else if (local_client_result.CheckError(
                     ::amsr::someip_binding_core::internal::ClientError::kClientIdsOverflow)) {
        logger_.LogWarn(
            [](ara::log::LogStream& s) {
              s << "Maximum number of proxies created. No free client ID available. Recovery action: retry";
            },
            static_cast<char const*>(__func__), __LINE__);
        result.EmplaceError(::ara::com::ComErrc::kNetworkBindingFailure);
      } else {
        logger_.LogFatalAndAbort(
            [](::amsr::someip_binding_core::internal::logging::StringStream& s) {
              s << "Unexpected error, please report it.";
            },
            static_cast<char const*>(__func__), __LINE__);
      }
    } else {
      amsr::core::ErrorCode const& error{conversion_result.Error()};
      // VCA_SLC_20_SOMEIPBINDING_FUNCTION_OUTSIDE_OF_ANALYZED_SCOPE
      amsr::core::StringView const& provided_instance_id_sv{provided_instance_id.ToString()};
      logger_.LogFatalAndAbort(
          [&provided_instance_id_sv, &error](::amsr::someip_binding_core::internal::logging::StringStream& s) {
            s << "ProvidedInstanceIdentifier (";
            // VCA_SOMEIPBINDING_LAMBDA_CAPTURE
            s << provided_instance_id_sv;
            s << ") is malformed. Error message: ";
            // VCA_SOMEIPBINDING_LAMBDA_CAPTURE
            s << error.Message();
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    return result;
  }

  /*!
   * \brief         Callback function to be called from binding whenever one service instance gets offered.
   * \details       SOCAL service discovery is informed about the service offer and a pointer to proxy xf so as to be
   *                later used by service proxy.
   * \param[in]     offered_service The provided service instance ID of the offered service.
   * \pre           -
   *
   * \context       Reactor
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Inform SOCAL service discovery about the service offer.
   * \endinternal
   */
  void OnServiceInstanceUp(
      ::amsr::someip_binding_core::internal::ProvidedServiceInstanceId const& offered_service) noexcept final {
    logger_.LogVerbose(static_cast<char const*>(__func__), __LINE__);

    ::ara::com::InstanceIdentifier const provided_aracom_instance_id{AraComInstanceIdXf::BuildAraComInstanceIdentifier(
        offered_service.GetServiceId(), offered_service.GetMajorVersion(), offered_service.GetInstanceId())};
    // VCA_SOMEIPBINDING_TEMPLATED_INSTANCE_IDENTIFIER_CONSTRUCTOR
    ::amsr::socal::internal::ProvidedInstanceIdentifier const provided_instance_id{provided_aracom_instance_id};
    // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    sd_->OfferService(required_instance_id_, provided_instance_id);
  }

  /*!
   * \brief         Callback function to be called from binding whenever one service instance is stopped.
   * \details       Socal service discovery is informed about the stopped service offer.
   * \param[in]     stopped_service The provided service instance ID of the stopped service.
   * \pre           -
   *
   * \context       Reactor
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Inform SOCAL service discovery about the stopped service offer.
   * \endinternal
   */
  void OnServiceInstanceDown(
      ::amsr::someip_binding_core::internal::ProvidedServiceInstanceId const& stopped_service) noexcept final {
    logger_.LogVerbose(static_cast<char const*>(__func__), __LINE__);

    ::ara::com::InstanceIdentifier const provided_aracom_instance_id{AraComInstanceIdXf::BuildAraComInstanceIdentifier(
        stopped_service.GetServiceId(), stopped_service.GetMajorVersion(), stopped_service.GetInstanceId())};
    // VCA_SOMEIPBINDING_TEMPLATED_INSTANCE_IDENTIFIER_CONSTRUCTOR
    ::amsr::socal::internal::ProvidedInstanceIdentifier const provided_instance_id{provided_aracom_instance_id};
    // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    sd_->StopOfferService(required_instance_id_, provided_instance_id);
  }

  /*!
   * \brief Service discovery service instance identifier.
   */
  RequiredServiceInstanceId const service_instance_id_;

  /*!
   * \brief Required instance identifier which is used to notify the availability of the service instance.
   */
  ::amsr::socal::internal::RequiredInstanceIdentifier const required_instance_id_;

  /*!
   * \brief SingletonAccess for the Socal service discovery.
   */
  typename ServiceInterfaceProxyType::ServiceDiscoverySingletonAccess sd_;

  /*!
   * \brief Logger Prefix.
   */
  amsr::core::String const logger_prefix_{
      someip_binding_core::internal::logging::LoggerPrefixGenerator::GetLoggerPrefix(
          "SomeIpSdProxyXf"_sv, service_instance_id_.GetServiceId(), service_instance_id_.GetInstanceId(),
          service_instance_id_.GetMajorVersion(), service_instance_id_.GetMinorVersion())};

  /*!
   * \brief E2E Event properties for all E2E Events (can be empty if no E2E events are configured).
   */
  E2EPropsMap const e2e_props_map_;

  /*!
   * \brief Reference to the configuration wrapper.
   */
  amsr::someip_binding::internal::configuration::ConfigWrapperInterface const& config_wrapper_;

  /*!
   * \brief Reference to the E2EProfileConfig map.
   */
  ::amsr::someip_binding::internal::configuration::SomeIpBindingConfig::E2EProfileMap const& e2e_profile_config_map_;

  /*!
   * \brief The required service instance.
   */
  someip_binding_core::internal::RequiredServiceInstance rsi_;

  /*!
   * \brief Service discovery listener to be registered into the RequiredServiceInstance to listen for service status
   *        updates. The RequiredServiceInstance will hold a shared pointer to this listener in order to forward the
   *        calls. This object will be registered upon construction of this class and later deregistered during
   *    destruction.
   */
  // VCA_SPC_15_SOMEIPBINDING_STL_FUNCTION
  std::shared_ptr<ServiceDiscoveryListener> sd_listener_{std::make_shared<ServiceDiscoveryListener>(*this)};

  /*!
   * \brief Logger to print out debug & error messages.
   */
  ::amsr::someip_binding_core::internal::logging::AraComLogger logger_{
      ::amsr::someip_binding_core::internal::logging::kSomeIpLoggerContextId,
      ::amsr::someip_binding_core::internal::logging::kSomeIpLoggerContextDescription, logger_prefix_};
};

}  // namespace service_discovery
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr
#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_SERVICE_DISCOVERY_SERVICE_DISCOVERY_PROXY_XF_H_
