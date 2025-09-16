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
/**        \file
 *        \brief  Factory class to be registered to socal factory registry to create proxy backend.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_PROXY_BACKEND_FACTORY_H_
#define LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_PROXY_BACKEND_FACTORY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>

#include "amsr/ipc/integrity_level.h"
#include "amsr/socal/internal/factory/proxy_backend_factory_interface.h"

#include "ipc_binding_core/internal/aracom_ipc_binding_interface.h"
#include "ipc_binding_core/internal/instance_id_util.h"
#include "ipc_binding_core/internal/ipc_protocol/ipc_protocol_types.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/logging/ipc_binding_log_builder.h"
#include "ipc_binding_core/internal/memory.h"
#include "ipc_binding_core/internal/packet_router/proxy_router_factory_interface.h"
#include "ipc_binding_core/internal/remote_server_manager_interface.h"
#include "ipc_binding_core/internal/required_service_instance_id.h"
#include "ipc_binding_core/internal/runtime_configuration/service_config.h"
#include "ipc_binding_xf_common/internal/trace/trace_sink_factory_interface.h"

namespace amsr {
namespace ipc_binding {
namespace internal {

/*!
 * \brief Proxy backend factory implementation.
 *
 * \tparam ProxyBackend  Representation of a required service instance.
 *
 * \unit IpcBinding::IpcBinding::RequiredServiceInstance
 */
template <typename ProxyBackend>
class ProxyBackendFactory final : public ::amsr::socal::internal::factory::ProxyBackendFactoryInterface<
                                      typename ProxyBackend::ProxyBackendInterface> {
 public:
  /*!
   * \brief Type alias for ProxyBackend's ProxyBackendInterface.
   */
  using ProxyBackendInterface = typename ProxyBackend::ProxyBackendInterface;

  /*!
   * \brief Type alias for AraComIpcBindingInterface.
   */
  using AraComIpcBindingInterface = ::amsr::ipc_binding_core::internal::AraComIpcBindingInterface;

  /*!
   * \brief Type alias for RequiredServiceInstanceId.
   */
  using RequiredServiceInstanceId = ::amsr::ipc_binding_core::internal::RequiredServiceInstanceId;

  /*!
   * \brief Type alias for ProvidedServiceInstanceId.
   */
  using ProvidedServiceInstanceId = ::amsr::ipc_binding_core::internal::ProvidedServiceInstanceId;

  /*!
   * \brief Type alias for IntegrityLevel.
   */
  using IntegrityLevel = ::amsr::ipc::IntegrityLevel;

  /*!
   * \brief Type alias for RuntimeProcessingMode.
   */
  using RuntimeProcessingMode = ::amsr::ipc_binding_core::internal::runtime_configuration::RuntimeProcessingMode;

  /*!
   * \brief Runtime service configuration type.
   */
  using ServiceConfig = ::amsr::ipc_binding_core::internal::runtime_configuration::ServiceConfig;

  /*!
   * \brief Type alias for trace sink factory interface.
   */
  using TraceSinkFactoryInterface = ::amsr::ipc_binding_xf_common::internal::trace::TraceSinkFactoryInterface;

  /*!
   * \brief Type alias for Remote Server Manager interface.
   */
  using RemoteServerManagerInterface = ::amsr::ipc_binding_core::internal::RemoteServerManagerInterface;

  /*!
   * \brief Constructor
   *
   * \param[in,out] ipc_binding                   A reference to IpcBinding. The object must be valid for
   *                                              ProxyBackendFactory's entire life time.
   * \param[in,out] remote_server_manager         RemoteServerManagerInterface reference. The object must be valid for
   *                                              ProxyBackendFactory's entire life time.
   * \param[in]     required_service_instance_id  The RequiredServiceInstanceId this ProxyBackend shall represent.
   * \param[in]     integrity_level               The integrity level for the required service instance.
   * \param[in]     service_config                The runtime service config for this Service Discovery for a specific
   *                                              RequiredServiceInstance.
   * \param[in]     trace_sink_factory            Reference to the trace sink factory.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  ProxyBackendFactory(AraComIpcBindingInterface& ipc_binding, RemoteServerManagerInterface& remote_server_manager,
                      RequiredServiceInstanceId const required_service_instance_id,
                      IntegrityLevel const integrity_level, ServiceConfig const& service_config,
                      TraceSinkFactoryInterface const& trace_sink_factory) noexcept
      :  // () is used to initialize the base class to suppress doxygen issue.
        ::amsr::socal::internal::factory::ProxyBackendFactoryInterface<ProxyBackendInterface>(),
        binding_{ipc_binding},
        remote_server_manager_{remote_server_manager},
        required_service_instance_id_{required_service_instance_id},
        integrity_level_{integrity_level},
        service_config_{service_config},
        trace_sink_factory_{trace_sink_factory},
        logger_{::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextId,
                ::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextDescription,
                ::amsr::core::StringView{"ProxyBackendFactory"}} {}

  ProxyBackendFactory(ProxyBackendFactory const&) noexcept = delete;
  ProxyBackendFactory(ProxyBackendFactory&&) noexcept = delete;
  auto operator=(ProxyBackendFactory const&) noexcept -> ProxyBackendFactory& = delete;
  auto operator=(ProxyBackendFactory&&) noexcept -> ProxyBackendFactory& = delete;

  /*!
   * \brief Destructor.
   *
   * \pre     -
   * \context Shutdown
   * \steady  FALSE
   *
   * \spec requires true; \endspec
   */
  // VCA_IPCB_COMPILER_GENERATED_DESTRUCTOR
  ~ProxyBackendFactory() noexcept final = default;

  /*!
   * \brief Creates the translation class for proxy side communication.
   *
   * \param[in] provided_instance_id  Valid IpcBinding ProvidedInstanceIdentifier Required format: "Ipc:<id>" with
   *                                  id=0..0xFFFFFFFE.
   *
   * \return Result containing a pointer to the created proxy backend or an error.
   *
   * \pre         The provided_instance_id must be valid.
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   *
   * \internal
   * - Parse the IpcBinding instance ID from the ProvidedInstanceIdentifier string.
   * - If the ProvidedInstanceIdentifier parsing failed:
   *   - Log a fatal error and abort further process execution.
   * - Check if the provided instance ID to be constructed matches to the RequiredServiceInstance.
   *   - Log a fatal error and abort further process execution if a mismatch was detected.
   * - Create the translation class for proxy side communication.
   * - Return the created ProxyBackend pointer.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_IPCBINDING_AutosarC++17_10-A15.4.2_STL_exceptions
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_IPCBINDING_AutosarC++17_10-A15.5.3_STL_exceptions
  auto CreateBackend(::amsr::socal::internal::ProvidedInstanceIdentifier const& provided_instance_id) noexcept
      -> ::amsr::core::Result<std::unique_ptr<ProxyBackendInterface>> final {  // IGNORE_BLACKLIST external API
    ::amsr::core::Result<ProvidedServiceInstanceId> ipc_provided_instance_id{
        InstanceIdUtil::BuildProvidedInstanceIdentifier(provided_instance_id)};

    if (!ipc_provided_instance_id.HasValue()) {
      logger_.TerminateOnViolation(
          "ProvidedInstanceId of the requested backend has an invalid format.",
          [&provided_instance_id](::ara::log::LogStream& s) {
            ::amsr::core::StringView const provided_instance_id_sv{provided_instance_id.ToString()};
            s << " Invalid identifier: '" << provided_instance_id_sv
              << "'. Expected format: \"IpcBinding:<ServiceId>:<InstanceId>:<MajorVersion>:<MinorVersion>\" with "
                 "id=0..0xFFFFFFFE";
          },
          Logger::LogLocation{__func__, __LINE__});
    }

    bool const is_required_instance_id{required_service_instance_id_.Matches(ipc_provided_instance_id.Value())};

    // RequiredInstanceId=<concrete> -> ProvidedInstanceId must match with RequiredInstanceId
    if (!is_required_instance_id) {
      logger_.TerminateOnViolation(
          "Proxy backend is created for a ProvidedInstanceId not matching the registered RequiredServiceInstance.",
          [this, &ipc_provided_instance_id](::ara::log::LogStream& s) {
            s << " ProvidedServiceInstance: (";
            // VCA_IPCB_LOG_LAMBDA_PASS_CLASS_MEMBER_BY_REFERENCE
            LogBuilder::LogServiceInstanceId(
                s,
                ipc_provided_instance_id.Value());  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
            s << "), RequiredServiceInstance: (";
            // VCA_IPCB_LOG_LAMBDA_PASS_CLASS_MEMBER_BY_REFERENCE
            LogBuilder::LogServiceInstanceId(
                s, required_service_instance_id_);  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
            s << ").";
          },
          Logger::LogLocation{__func__, __LINE__});
    }

    // VCA_IPCB_VALID_MEMBER_REFERENCE
    ipc_binding_core::internal::ProxyRouterFactoryInterface& proxy_router_factory{binding_.GetProxyRouterFactory()};
    ClientId client_id{GetNextClientId()};

    return ::amsr::core::Result<std::unique_ptr<ProxyBackendInterface>>  // IGNORE_BLACKLIST external API
        // VCA_IPCB_STD_MAKE_UNIQUE_SAFE_FUNCTION
        {std::make_unique<ProxyBackend>(remote_server_manager_,  // IGNORE_BLACKLIST external API
                                        ipc_provided_instance_id.Value(), required_service_instance_id_, client_id,
                                        integrity_level_, service_config_, trace_sink_factory_, proxy_router_factory)};
  }

 private:
  /*!
   * \brief Type alias for the instance id utility.
   */
  using InstanceIdUtil = ::amsr::ipc_binding_core::internal::InstanceIdUtil;

  /*!
   * \brief Type alias for the user logger.
   */
  using Logger = ::amsr::ipc_binding_core::internal::logging::AraComLogger;

  /*!
   * \brief Type alias for the used LogBuilder.
   */
  using LogBuilder = ::amsr::ipc_binding_core::internal::logging::IpcBindingLogBuilder;

  /*!
   * \brief Type alias for Client ID.
   */
  using ClientId = ::amsr::ipc_binding_core::internal::ipc_protocol::ClientId;

  /*!
   * \brief Reference to the IpcBinding.
   */
  AraComIpcBindingInterface& binding_;

  /*!
   * \brief RemoteServerManager reference.
   */
  RemoteServerManagerInterface& remote_server_manager_;

  /*!
   * \brief The RequiredServiceInstance ID the proxy backend is representing (can be InstanceId=ALL).
   */
  RequiredServiceInstanceId const required_service_instance_id_;

  /*!
   * \brief IntegrityLevel expected by this Required Service Instance.
   */
  IntegrityLevel const integrity_level_;

  /*!
   * \brief The runtime service config for the related service interface.
   */
  ServiceConfig const& service_config_;

  /*!
   * \brief Reference to the trace sink factory.
   */
  TraceSinkFactoryInterface const& trace_sink_factory_;

  /*!
   * \brief Logger to print out debug & error messages.
   */
  Logger logger_;

  /*!
   * \brief Unique client ID counter.
   */
  std::atomic<ClientId::type> next_client_id_{1};

  /*!
   * \brief Get the next unique proxy instance ID.
   *
   * \return Unique ClientId.
   *
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \internal
   * - Atomically return the current value of the ID counter and increment it.
   * \endinternal
   */
  auto GetNextClientId() noexcept -> ClientId { return ClientId{next_client_id_++}; }
};

}  // namespace internal
}  // namespace ipc_binding
}  // namespace amsr

#endif  // LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_PROXY_BACKEND_FACTORY_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_IpcBinding_CoverageComponentOverall_trivial_utility
//   \ACCEPT  XX
//   \REASON  FunctionCoverage gap is reported for the API.
//            Since the API is a trivial helper function and tested by unit tests
//            no component test function coverage needs to be achieved for this function.
// COV_JUSTIFICATION_END
