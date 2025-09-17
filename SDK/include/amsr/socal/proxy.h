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
/**        \file  proxy.h
 *        \brief  Proxy of a specific service interface.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_PROXY_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_PROXY_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <memory>
#include <string>
#include <type_traits>
#include <utility>

#include "amsr/core/abort.h"
#include "amsr/core/error_code.h"
#include "amsr/core/instance_specifier.h"
#include "amsr/core/result.h"
#include "amsr/core/string.h"
#include "amsr/core/string_view.h"
#include "amsr/generic/singleton_wrapper.h"
#include "amsr/socal/internal/factory/backend_factory_registry.h"
#include "amsr/socal/internal/internal_instance_identifier.h"
#include "amsr/socal/internal/lifecycle.h"
#include "amsr/socal/internal/logging/ara_com_logger.h"
#include "amsr/socal/internal/reboot_detection_state_machine.h"
#include "amsr/socal/internal/runtime.h"
#include "amsr/socal/internal/service_discovery/proxy_service_discovery.h"
#include "amsr/socal/internal/types.h"
#include "amsr/socal/service_state.h"
#include "amsr/socal/skeleton_connection_state.h"
#include "ara/com/instance_identifier.h"
#include "ara/com/service_identifier.h"
#include "ara/core/result.h"
#include "ara/core/string_view.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace socal {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOCAL_AutosarC++17_10-M7.3.6_usingDirectiveInHeaderFile
using vac::container::literals::operator""_sv;  // NOLINT(build/namespaces)

/*!
 * \brief This class represents a specific ServiceInterface Proxy.
 * \tparam ServiceInterface       The corresponding ServiceInterface.
 * \tparam ProxyBackendInterface  The proxy backend interface the proxy communicates to.
 *
 * \unit Socal::Proxy::Proxy
 */
template <typename ServiceInterface, typename ProxyBackendInterface>
class Proxy {
 public:
  /*!
   * \brief Type alias for ProxyBackendInterface.
   */
  using ProxyBackendInterfaceType = ProxyBackendInterface;

  /*!
   * \brief Type alias for ServiceInterface.
   */
  using ServiceInterfaceType = ServiceInterface;

  /*!
   * \brief Type alias for Socal runtime.
   */
  using Runtime = ::amsr::socal::internal::Runtime;

  /*!
   * \brief Type alias for ProxyServiceDiscovery.
   */
  using ServiceDiscovery = ::amsr::socal::internal::service_discovery::ProxyServiceDiscovery;

  /*!
   * \brief Type alias for a singleton of ProxyServiceDiscovery.
   */
  using ServiceDiscoverySingleton = ::amsr::generic::Singleton<ServiceDiscovery>;

  /*!
   * \brief Type alias for a singleton access of ProxyServiceDiscoveryInterface.
   */
  using ServiceDiscoverySingletonAccess =
      ::amsr::generic::SingletonAccess<::amsr::socal::internal::service_discovery::ProxyServiceDiscoveryInterface>;

  /*!
   * \brief Service Identifier.
   */
  static constexpr ::ara::com::ServiceIdentifierType kServiceIdentifier{ServiceInterface::ServiceIdentifier};

  /*!
   * \brief Service shortname path.
   */
  static constexpr ::amsr::core::StringView kServiceShortNamePath{ServiceInterface::kServiceShortNamePath};

  /*!
   * \brief Type alias for the singleton access to the selected runtime.
   */
  using RuntimeAccess = ::amsr::generic::SingletonAccess<Runtime>;

  /*!
   * \brief Type alias for required instance identifier.
   */
  using RequiredInstanceIdentifier = ::amsr::socal::internal::RequiredInstanceIdentifier;

  /*!
   * \brief Type alias for provided instance identifier.
   */
  using ProvidedInstanceIdentifier = ::amsr::socal::internal::ProvidedInstanceIdentifier;

  /*!
   * \brief Type alias for template instantiated ProxyBackendFactoryRegistry.
   */
  using ProxyBackendFactoryRegistry =
      ::amsr::socal::internal::factory::ProxyBackendFactoryRegistry<ProxyBackendInterface>;

  /*!
   * \brief Type alias for a singleton of ProxyBackendFactoryRegistry.
   */
  using ProxyBackendFactoryRegistrySingleton = ::amsr::generic::Singleton<ProxyBackendFactoryRegistry>;

  /*!
   * \brief Type alias for a singleton access of ProxyBackendFactoryRegistry.
   */
  using ProxyBackendFactoryRegistrySingletonAccess = ::amsr::generic::SingletonAccess<
      ::amsr::socal::internal::factory::ProxyBackendFactoryRegistryInterface<ProxyBackendInterface>>;

  // ======================== Construction Token (start) ========================

  /*!
   * \brief   Construction token class for exception-less construction.
   * \details Created via "Preconstruct()" call(s).
   * \vpublic
   *
   * \trace SPEC-8053549, SPEC-8053550
   */
  // VECTOR NC AutosarC++17_10-M3.2.2: MD_Socal_AutosarC++17_10-M3.2.2_ODR_false_positive
  class ConstructionToken final {
   public:
    /*!
     * \brief Construct a construction token for a proxy.
     * \param[in] required_instance_id  The required instance identifier used for the proxy.
     * \param[in] provided_instance_id  The provided instance identifier used for the proxy.
     * \context  App
     * \vprivate To be created by "Preconstruct()" call(s) only.
     * \steady FALSE
     * \spec requires true; \endspec
     */
    explicit ConstructionToken(RequiredInstanceIdentifier const required_instance_id,
                               ProvidedInstanceIdentifier const provided_instance_id) noexcept
        // VCA_SOCAL_PASSING_FUNCTION_ARG_TO_ANOTHER_FUNCTION
        : required_instance_id_{required_instance_id}, provided_instance_id_{provided_instance_id} {};

    ConstructionToken() = delete;
    ConstructionToken(ConstructionToken const&) = delete;
    ConstructionToken& operator=(ConstructionToken const&) & = delete;

    /*!
     * \brief Use default move constructor.
     * \steady FALSE
     * \spec requires true; \endspec
     */
    ConstructionToken(ConstructionToken&&) noexcept = default;

    /*!
     * \brief Use default move assignment operator.
     * \return Reference to moved-to instance
     * \steady FALSE
     * \spec requires true; \endspec
     */
    ConstructionToken& operator=(ConstructionToken&&) & noexcept = default;

    /*!
     * \brief Destructor.
     * \vprivate
     * \steady FALSE
     * \spec requires true; \endspec
     */
    ~ConstructionToken() noexcept = default;

    /*!
     * \brief Get the provided instance identifier used for proxy creation.
     * \return Provided instance identifier.
     * \vprivate To be used by Proxy constructor only.
     * \steady FALSE
     * \spec requires true; \endspec
     */
    ProvidedInstanceIdentifier const& GetProvidedInstanceId() const noexcept { return provided_instance_id_; }

    /*!
     * \brief Get the required instance identifier used for proxy creation.
     * \return Required instance identifier.
     * \vprivate To be used by Proxy constructor only.
     * \steady FALSE
     * \spec requires true; \endspec
     */
    RequiredInstanceIdentifier const& GetRequiredInstanceId() const noexcept { return required_instance_id_; }

   private:
    /*!
     * \brief The required instance identifier.
     */
    RequiredInstanceIdentifier required_instance_id_;

    /*!
     * \brief The provided instance identifier.
     */
    ProvidedInstanceIdentifier provided_instance_id_;
  };

  /*!
   * \brief Type alias for the ConstructionToken result.
   */
  using ConstructionResult = ::ara::core::Result<ConstructionToken>;

  // ========================= Construction Token (end) =========================

  /*!
   * \brief Delete the default copy constructor.
   */
  Proxy(Proxy const&) = delete;

  /*!
   * \brief Delete the default copy assignment operator.
   */
  Proxy& operator=(Proxy const&) & = delete;

  /*!
   * \brief Delete the default move constructor.
   */
  Proxy(Proxy&&) noexcept = delete;

  /*!
   * \brief Delete the default move assignment operator.
   */
  Proxy& operator=(Proxy&&) & noexcept = delete;

  /*!
   * \brief Creates the singleton static instances.
   * \pre         This function must only be called once.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private.
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - Create the singleton instance of backend factory registry.
   * - Create the singleton instance of service discovery.
   * \endinternal
   */
  static void CreateSingletons() noexcept {
    backend_factory_registry_.Create();
    service_discovery_.Create(ServiceInterface::kServiceShortNamePath);
  }

  /*!
   * \brief Destroys the singleton static instances.
   * \pre         This function must only be called once.
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private.
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - Destroy the singleton instance of service discovery.
   * - Destroy the singleton instance of backend factory registry.
   * \endinternal
   */
  static void DestroySingletons() noexcept {
    service_discovery_.Destroy();
    backend_factory_registry_.Destroy();
  }

  /*!
   * \brief Getter for ProxyBackendFactoryRegistry singleton member.
   * \return SingletonAccess to the ProxyBackendFactoryRegistryInterface.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private.
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * Calls "amsr::core::Abort()" if:
   * - ProxyBackendFactoryRegistry Singleton was not initialized.
   * \endinternal
   */
  static auto GetFactoryRegistry() noexcept -> ProxyBackendFactoryRegistrySingletonAccess {
    return static_cast<ProxyBackendFactoryRegistrySingletonAccess>(backend_factory_registry_.GetAccess());
  }

  /*!
   * \brief Returns the service discovery singleton access to the ProxyServiceDiscoveryInterface.
   * \return SingletonAccess to the ProxyServiceDiscoveryInterface.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private.
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * Calls "amsr::core::Abort()" if:
   * - ProxyServiceDiscovery Singleton was not initialized.
   * \endinternal
   */
  static auto GetServiceDiscovery() noexcept -> ServiceDiscoverySingletonAccess {
    return static_cast<ServiceDiscoverySingletonAccess>(service_discovery_.GetAccess());
  }

  /*!
   * \brief Poll the current state of the service backend.
   * \return A ServiceState indicating if the service backend is up, down or restarted.
   * \pre         -
   * \context     App | Callback.
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   * \internal
   * - Call PollState() on the state machine and return its state.
   * \endinternal
   */
  ::amsr::socal::ServiceState ReadServiceState() noexcept {
    // VCA_SOCAL_UNDEFINED_FUNCTION_WITH_IMPLICIT_THIS_PTR
    return reboot_detection_.PollState();
  }

  /*!
   * \brief Polls the current connection state to the skeleton.
   * \return SkeletonConnectionState enum value representing the connection state to the skeleton.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  auto ReadSkeletonConnectionState() noexcept -> ::amsr::socal::SkeletonConnectionState {
    // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
    return GetProxyBackend().ReadSkeletonConnectionState();
  }

 protected:
  // ========================== Preconstruction (start) =========================
  // VECTOR NC AutosarC++17_10-A15.4.2:MD_SOCAL_AutosarC++17_10-A15.4.2_STL_exceptions
  // VECTOR NC AutosarC++17_10-A15.5.3:MD_SOCAL_AutosarC++17_10-A15.5.3_STL_exceptions
  /*!
   * \brief Exception-less pre-construction of a service proxy.
   * \param[in] required_instance_id  The required instance identifier used for the proxy.
   * \param[in] provided_instance_id  The provided instance identifier used for the proxy.
   * \return Result<ConstructionToken> Result containing construction token from which a proxy object can be
   *         constructed.
   * \pre         The instance identifier must be known.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - If Socal is not initialized.
   *   - Log a fatal message and abort the application.
   * - Check if instance_id is mapped to any R-Port instance specifier.
   *
   * Calls "amsr::core::Abort()" if:
   * - Socal is not initialized.
   * - Instance identifier of the handle is unknown.
   * \endinternal
   *
   * \trace SPEC-8053550
   */
  static ConstructionResult Preconstruct(RequiredInstanceIdentifier required_instance_id,
                                         ProvidedInstanceIdentifier provided_instance_id) noexcept {
    ::amsr::socal::internal::logging::AraComLogger const logger{
        ::amsr::socal::internal::logging::kAraComLoggerContextId,
        ::amsr::socal::internal::logging::kAraComLoggerContextDescription, "Proxy"_sv};
    if (!internal::IsComInitialized()) {
      logger.LogFatalAndAbort(
          [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            // VCA_SOCAL_CALLING_STL_APIS
            s << "API called before Socal is initialized.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    // VCA_SOCAL_UNDEFINED_FUNCTION_WITH_IMPLICIT_THIS_PTR
    std::string required_instance_id_str{required_instance_id.ToString().ToString()};
    // VCA_SOCAL_UNDEFINED_FUNCTION_WITH_IMPLICIT_THIS_PTR
    std::string provided_instance_id_str{provided_instance_id.ToString().ToString()};

    logger.LogInfo(
        [&required_instance_id_str, &provided_instance_id_str](::amsr::socal::internal::logging::LogStream& s) {
          // VCA_SOCAL_AMSR_LOGGER_UNDEFINED_OPERATOR_FUNCTION
          s << "Preconstructing Proxy with required instance identifier "
            // VECTOR NL VectorC++-V5.0.1, AutosarC++17_10-A5.0.1: MD_SOCAL_VectorC++-V5.0.1_unsequenced_call_false_positive, MD_SOCAL_AutosarC++17_10_A5.0.1_falsepositive
            << required_instance_id_str  // VCA_SOCAL_AMSR_LOGGER_UNDEFINED_OPERATOR_FUNCTION
            // VECTOR NL VectorC++-V5.0.1, AutosarC++17_10-A5.0.1: MD_SOCAL_VectorC++-V5.0.1_unsequenced_call_false_positive, MD_SOCAL_AutosarC++17_10_A5.0.1_falsepositive
            << " and provided instance ID "  // VCA_SOCAL_AMSR_LOGGER_UNDEFINED_OPERATOR_FUNCTION
            // VECTOR NL VectorC++-V5.0.1, AutosarC++17_10-A5.0.1: MD_SOCAL_VectorC++-V5.0.1_unsequenced_call_false_positive, MD_SOCAL_AutosarC++17_10_A5.0.1_falsepositive
            << provided_instance_id_str;  // VCA_SOCAL_AMSR_LOGGER_UNDEFINED_OPERATOR_FUNCTION
        },
        static_cast<char const*>(__func__), __LINE__);

    // Resolve the InstanceIdentifier of the provided handle
    RuntimeAccess runtime{Runtime::getSingletonInstance().GetAccess()};
    // VCA_SOCAL_UNDEFINED_FUNCTION_WITH_IMPLICIT_THIS_PTR
    ::ara::com::InstanceIdentifier const& instance_identifier{required_instance_id.GetAraComInstanceIdentifier()};
    ::amsr::core::Result<::amsr::socal::internal::InstanceSpecifierLookupTableEntry> const resolve_result{
        // VCA_SOCAL_UNDEFINED_FUNCTION_WITH_IMPLICIT_THIS_PTR, VCA_SOCAL_UNDEFINED_FUNCTION_WITH_REFERENCE_ARGUMENTS
        runtime->GetRequiredInstanceSpecifierLookupTable().Resolve(instance_identifier, kServiceShortNamePath)};

    if (!resolve_result.HasValue()) {
      logger.LogFatalAndAbort(
          [&required_instance_id,
           &provided_instance_id](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            s << "The required instance identifier ";      // VCA_SOCAL_CALLING_STL_APIS
            s << required_instance_id.ToString();          // VCA_SOCAL_UNDEFINED_FUNCTION_WITH_IMPLICIT_THIS_PTR
            s << " related to the provided instance ID ";  // VCA_SOCAL_CALLING_STL_APIS
            s << provided_instance_id.ToString();          // VCA_SOCAL_UNDEFINED_FUNCTION_WITH_IMPLICIT_THIS_PTR
            s << "' is not known from the model.";         // VCA_SOCAL_CALLING_STL_APIS
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    return ConstructionResult::FromValue(required_instance_id, provided_instance_id);
  }  // VCA_SOCAL_CALLING_DESTRUCTOR_OF_STACK_OBJECTS

  // =========================== Preconstruction (end) ==========================

  // ==================== Constructors / Destructors (start) ====================

  /*!
   * \brief Exception-less constructor using a "ConstructionToken".
   * \param[in] token  "ConstructionToken" created with "Preconstruct()" call. Must be created with the "Preconstruct()"
   *                   call.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - Request the creation of a backend from the ProxyServiceDiscovery instance.
   * - Register reboot detection state machine to ProxyServiceDiscovery instance.
   * - If creation fails:
   *   - Abort with log message.
   *
   * Calls "amsr::core::Abort()" if:
   * - Socal is not initialized.
   * - Service discovery is not initialized.
   * - Backend creation has failed due to a grant enforcement error from IAM.
   * - Backend creation has failed due to a connection establishment failure.
   * - Backend creation has failed due to a network binding failure.
   * \endinternal
   *
   * \trace SPEC-8053550
   */
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOCAL_AutosarC++17_10-A15.4.2_STL_exceptions
  explicit Proxy(ConstructionToken const&& token) noexcept
      : required_instance_id_{token.GetRequiredInstanceId()}, provided_instance_id_{token.GetProvidedInstanceId()} {
    ::amsr::core::Result<typename ProxyBackendFactoryRegistry::BackendInterfacePtr> create_backend_result{
        backend_factory_registry_.GetAccess()->CreateBackend(required_instance_id_, provided_instance_id_)};

    if (!create_backend_result) {
      ::amsr::core::ErrorCode const error_code{create_backend_result.Error()};
      ::amsr::core::StringView message{error_code.Message()};

      // VCA_SOCAL_UNDEFINED_FUNCTION_WITH_IMPLICIT_THIS_PTR
      ::amsr::core::StringView const instance_identifier_view{provided_instance_id_.ToString()};
      logger_.LogFatalAndAbort(
          [&instance_identifier_view, &message](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            s << "Could not create proxy with provided instance identifier '";  // VCA_SOCAL_CALLING_STL_APIS
            s << instance_identifier_view << "': " << message;                  // VCA_SOCAL_CALLING_STL_APIS
          },
          static_cast<char const*>(__func__), __LINE__);

    } else {
      assert(create_backend_result.Value() != nullptr);
      proxy_backend_ptr_.swap(create_backend_result.Value());
      // VCA_SOCAL_DEREFERENCING_SINGLETON_ACCESS
      GetServiceDiscoveryInternal()->AddRequester(required_instance_id_, provided_instance_id_, &reboot_detection_);
    }
  }  // VCA_SOCAL_CALLING_DESTRUCTOR_OF_STACK_OBJECTS

  /*!
   * \brief Destructor.
   * \pre        -
   * \context    App
   * \threadsafe FALSE for same class instance, TRUE for different instances.
   * \reentrant  FALSE for same class instance, TRUE for different instances.
   * \vprivate
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - Log destruction.
   * - Unregister reboot detection state machine from service discovery instance.
   * \endinternal
   */
  ~Proxy() noexcept {  // VCA_SOCAL_DESTRUCTOR_STATIC_MEMORY
    logger_.LogInfo(
        [this](::amsr::socal::internal::logging::LogStream& s) {
          // VCA_SOCAL_VALID_REFERENCE_CAPTURED_IN_LAMBDA_EXECUTED_SYNCHRONOUSLY
          // VCA_SOCAL_UNDEFINED_FUNCTION_WITH_IMPLICIT_THIS_PTR
          ::amsr::core::StringView const required_id_sv{required_instance_id_.ToString()};
          // VCA_SOCAL_VALID_REFERENCE_CAPTURED_IN_LAMBDA_EXECUTED_SYNCHRONOUSLY
          // VCA_SOCAL_UNDEFINED_FUNCTION_WITH_IMPLICIT_THIS_PTR
          ::amsr::core::StringView const provided_id_sv{provided_instance_id_.ToString()};
          ::ara::core::StringView const service_id_name{kServiceIdentifier.ToString()};
          // VCA_SOCAL_AMSR_LOGGER_UNDEFINED_OPERATOR_FUNCTION
          s << "Destroy Proxy for Service '" << service_id_name << "' with required InstanceID " << required_id_sv
            << " / provided InstanceId " << provided_id_sv << "'.";
        },
        static_cast<char const*>(__func__), __LINE__);
    // VCA_SOCAL_DEREFERENCING_SINGLETON_ACCESS
    GetServiceDiscoveryInternal()->RemoveRequester(provided_instance_id_, &reboot_detection_);
  }  // VCA_SOCAL_CALLING_DESTRUCTOR_OF_STACK_OBJECTS

  // ===================== Constructors / Destructors (end) =====================

  /*!
   * \brief Returns a reference to an provided instance identifier.
   * \return Reference to the provided instance identifier
   * \pre     -
   * \context App
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ProvidedInstanceIdentifier const& GetProvidedInstanceId() const noexcept { return provided_instance_id_; }

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_Method_can_be_declared_const_falsenegative
  /*!
   * \brief Returns a reference to the proxy backend retrieved from the service discovery.
   * \return Reference to the proxy backend.
   * \pre     -
   * \context App
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ProxyBackendInterface& GetProxyBackend() noexcept { return *proxy_backend_ptr_; }

  /*!
   * \brief Resolve an InstanceIdentifier via InstanceSpecifier lookup table.
   * \param[in] instance  The InstanceIdentifier to be resolved.
   * \return The resolved service instance.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - Get the R-Port InstanceSpecifierLookupTable from the runtime instance.
   * - Resolve \p instance to InstanceSpecifierLookupTableEntry from the table.
   * - If the result from the resolve has no value.
   *   - Log error and abort.
   * - Otherwise return the InstanceSpecifierLookupTableEntry.
   *
   * Calls "amsr::core::Abort()" if:
   * - The instance identifier is not found in the lookup table (not configured in the ARXML model).
   * \endinternal
   */
  static internal::InstanceSpecifierLookupTableEntry ResolveInstanceSpecifierMapping(
      ::ara::com::InstanceIdentifier const& instance) noexcept {
    // Resolve the InstanceIdentifier
    RuntimeAccess runtime{Runtime::getSingletonInstance().GetAccess()};
    internal::InstanceSpecifierLookupTable const& instance_specifier_table{
        // VCA_SOCAL_UNDEFINED_FUNCTION_WITH_IMPLICIT_THIS_PTR
        runtime->GetRequiredInstanceSpecifierLookupTable()};

    ::amsr::core::Result<internal::InstanceSpecifierLookupTableEntry> const service_instance{
        // VCA_SOCAL_UNDEFINED_FUNCTION_WITH_REFERENCE_ARGUMENTS
        instance_specifier_table.Resolve(instance, kServiceShortNamePath)};

    if (!service_instance.HasValue()) {
      ::amsr::socal::internal::logging::AraComLogger const logger{
          ::amsr::socal::internal::logging::kAraComLoggerContextId,
          ::amsr::socal::internal::logging::kAraComLoggerContextDescription, "Proxy"_sv};

      ::ara::core::StringView const service_identifier{kServiceIdentifier.ToString()};

      logger.LogFatalAndAbort(
          [&instance, &service_identifier](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            ::amsr::core::StringView const service_identifier_view{service_identifier};
            ::amsr::core::StringView const instance_view{instance.ToString()};
            s << "No service instances of service " << service_identifier_view;  // VCA_SOCAL_CALLING_STL_APIS
            s << " for InstanceIdentifier '" << instance_view << "' found.";     // VCA_SOCAL_CALLING_STL_APIS
          },
          static_cast<char const*>(__func__), __LINE__);
    }

    return service_instance.Value();
  }

  /*!
   * \brief Type alias for a singleton access of ProxyServiceDiscoveryInternalInterface.
   */
  using InternalServiceDiscoverySingletonAccess = ::amsr::generic::SingletonAccess<
      ::amsr::socal::internal::service_discovery::ProxyServiceDiscoveryInternalInterface>;

  /*!
   * \brief Returns the service discovery singleton access to the ProxyServiceDiscoveryInternalInterface.
   * \return SingletonAccess to the ProxyServiceDiscoveryInternalInterface.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private.
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * Calls "amsr::core::Abort()" if:
   * - ProxyServiceDiscovery Singleton was not initialized.
   * \endinternal
   */
  static auto GetServiceDiscoveryInternal() noexcept -> InternalServiceDiscoverySingletonAccess {
    return static_cast<InternalServiceDiscoverySingletonAccess>(service_discovery_.GetAccess());
  }

  /*!
   * \brief Resolve an InstanceSpecifier via InstanceSpecifier lookup table.
   * \param[in] instance  The InstanceSpecifier to be resolved.
   * \return A container of the resolved service instances. The list can be empty in case no matching entry exists.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - Get the R-Port InstanceSpecifierLookupTable from the runtime instance.
   * - Get all InstanceIdentifiers which match the provided instance specifier into
   *   InstanceSpecifierLookupTableEntryContainer.
   * - If the container is empty.
   *   - Abort.
   * - Return InstanceSpecifierLookupTableEntryContainer.
   *
   * Calls "amsr::core::Abort()" if:
   * - The instance specifier ( \p instance ) is not configured in the ARXML model.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOCAL_AutosarC++17_10-A15.5.3_GTestException
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOCAL_AutosarC++17_10-A15.4.2_GTestException
  static internal::InstanceSpecifierLookupTableEntryContainer ResolveInstanceSpecifierMapping(
      ::amsr::core::InstanceSpecifier const& instance) noexcept {
    // Resolve the InstanceSpecifier to the mapped InstanceIdentifiers

    RuntimeAccess runtime{Runtime::getSingletonInstance().GetAccess()};
    internal::InstanceSpecifierLookupTable const& instance_specifier_lookup_table{
        // VCA_SOCAL_UNDEFINED_FUNCTION_WITH_IMPLICIT_THIS_PTR
        runtime->GetRequiredInstanceSpecifierLookupTable()};

    // VCA_SOCAL_CALLING_STL_APIS
    internal::InstanceSpecifierLookupTableEntryContainer const resolved_lookup_table_entries{
        instance_specifier_lookup_table.Resolve(instance, kServiceShortNamePath)
            .ValueOr(internal::InstanceSpecifierLookupTableEntryContainer{})};

    if (resolved_lookup_table_entries.empty()) {
      // VCA_SOCAL_CALLING_STL_APIS
      std::string log_message{
          "Proxy::ResolveInstanceSpecifierMapping: Unknown instance specifier "
          "(stringified instance: "};
      log_message.append(instance.ToString().ToString());  // VCA_SOCAL_CALLING_STL_APIS
      log_message.append(").");                            // VCA_SOCAL_CALLING_STL_APIS

      ::amsr::core::Abort(log_message.c_str());
    }

    return resolved_lookup_table_entries;
  }

 private:
  /*!
   * \brief State machine for reboot detection.
   */
  internal::RebootDetectionStateMachine reboot_detection_{};

  /*!
   * \brief   Singleton access to the Runtime.
   * \details De-initializing Socal is not possible until this object is destroyed. So this member is added here to make
   *          sure no Proxy object exists during the deinitialization phase.
   */
  RuntimeAccess runtime_{GetRuntimeAccess()};

  /*!
   * \brief Required instance identifier for this proxy.
   */
  ::amsr::socal::internal::RequiredInstanceIdentifier const required_instance_id_;

  /*!
   * \brief Provided instance identifier for this proxy.
   */
  ::amsr::socal::internal::ProvidedInstanceIdentifier const provided_instance_id_;

  /*!
   * \brief Proxy backend (shared pointer).
   */
  typename ProxyBackendFactoryRegistry::BackendInterfacePtr proxy_backend_ptr_{nullptr};

  /*!
   * \brief Returns a singleton access to the runtime.
   * \return Singleton access to the runtime.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - If Socal is not initialized.
   *   - Log a fatal message and abort the application.
   * - Return the SingletonAccess to the runtime.
   *
   * Calls "amsr::core::Abort()" if:
   * - Socal is not initialized.
   * \endinternal
   */
  static RuntimeAccess GetRuntimeAccess() noexcept {
    if (!internal::IsComInitialized()) {
      ::amsr::socal::internal::logging::AraComLogger const logger{
          ::amsr::socal::internal::logging::kAraComLoggerContextId,
          ::amsr::socal::internal::logging::kAraComLoggerContextDescription, "Proxy"_sv};
      logger.LogFatalAndAbort(
          [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            // VCA_SOCAL_CALLING_STL_APIS
            s << "API called before Socal is initialized.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    return Runtime::getSingletonInstance().GetAccess();
  }

  /*!
   * \brief Singleton instance of ProxyBackendFactoryRegistry.
   */
  // VECTOR NL AutosarC++17_10-A3.3.2: MD_SOCAL_AutosarC++17_10-A3.3.2_StaticStorageDurationOfNonPODType
  static ProxyBackendFactoryRegistrySingleton backend_factory_registry_;

  /*!
   * \brief The service discovery instance.
   */
  // VECTOR NL AutosarC++17_10-A3.3.2: MD_SOCAL_AutosarC++17_10-A3.3.2_StaticStorageDurationOfNonPODType
  static ServiceDiscoverySingleton service_discovery_;

  /*!
   * \brief An instance of the logger.
   */
  ::amsr::socal::internal::logging::AraComLogger logger_{
      ::amsr::socal::internal::logging::kAraComLoggerContextId,
      ::amsr::socal::internal::logging::kAraComLoggerContextDescription, "Proxy"_sv};
};
/*!
 * \brief Definition for the static singleton of proxy backend factory registry.
 * \tparam ServiceInterface       The corresponding ServiceInterface.
 * \tparam ProxyBackendInterface  The proxy backend interface the proxy communicates to.
 */
// VECTOR NC AutosarC++17_10-A3.3.2: MD_SOCAL_AutosarC++17_10-A3.3.2_StaticStorageDurationOfNonPODType
template <typename ServiceInterface, typename ProxyBackendInterface>
typename Proxy<ServiceInterface, ProxyBackendInterface>::ProxyBackendFactoryRegistrySingleton
    Proxy<ServiceInterface, ProxyBackendInterface>::backend_factory_registry_;

/*!
 * \brief Static instance of service discovery.
 * \tparam ServiceInterface       The corresponding ServiceInterface.
 * \tparam ProxyBackendInterface  The proxy backend interface the proxy communicates to.
 */
// VECTOR NC AutosarC++17_10-A3.3.2: MD_SOCAL_AutosarC++17_10-A3.3.2_StaticStorageDurationOfNonPODType
template <typename ServiceInterface, typename ProxyBackendInterface>
typename Proxy<ServiceInterface, ProxyBackendInterface>::ServiceDiscoverySingleton
    Proxy<ServiceInterface, ProxyBackendInterface>::service_discovery_;

/*!
 * \brief Service Identifier.
 * \tparam ServiceInterface       The corresponding ServiceInterface.
 * \tparam ProxyBackendInterface  The proxy backend interface the proxy communicates to.
 */
template <typename ServiceInterface, typename ProxyBackendInterface>
constexpr ::ara::com::ServiceIdentifierType Proxy<ServiceInterface, ProxyBackendInterface>::kServiceIdentifier;

/*!
 * \brief Service Shortname path Identifier.
 * \tparam ServiceInterface       The corresponding ServiceInterface.
 * \tparam ProxyBackendInterface  The proxy backend interface the proxy communicates to.
 */
template <typename ServiceInterface, typename ProxyBackendInterface>
constexpr ::ara::core::StringView Proxy<ServiceInterface, ProxyBackendInterface>::kServiceShortNamePath;

}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_PROXY_H_
