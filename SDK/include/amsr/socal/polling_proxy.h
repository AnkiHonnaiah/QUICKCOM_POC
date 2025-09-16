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
/**        \file  polling_proxy.h
 *        \brief  Polling Proxy base class of a specific service interface.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_POLLING_PROXY_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_POLLING_PROXY_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/socal/internal/lifecycle.h"
#include "amsr/socal/internal/logging/ara_com_logger.h"
#include "amsr/socal/internal/methods/polling_mode_future_executor.h"
#include "amsr/socal/internal/polling_find_service_handler.h"
#include "amsr/socal/proxy.h"
#include "ara/com/instance_identifier.h"
#include "ara/com/types_common.h"
#include "ara/core/future.h"
#include "ara/core/instance_specifier.h"

namespace amsr {
namespace socal {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOCAL_AutosarC++17_10-M7.3.6_usingDirectiveInHeaderFile
using vac::container::literals::operator""_sv;  // NOLINT(build/namespaces)

/*!
 * \brief   This class represents a specific ServiceInterface Polling Proxy.
 * \details The following APIs are not considered to be threadsafe against each other, or against any other API in this
 *          instance:
 *          - FindService()
 * \tparam ServiceInterface       The corresponding ServiceInterface, should define values for ServiceIdentifier and
 *                                kServiceShortNamePath constants.
 * \tparam ProxyBackendInterface  The proxy backend interface the proxy communicates to.
 * \tparam ProxyHandleType        The corresponding ServiceInterface HandleType.
 *
 * \unit Socal::Proxy::Proxy
 */
template <typename ServiceInterface, typename ProxyBackendInterface, typename ProxyHandleType>
class PollingProxy : public Proxy<ServiceInterface, ProxyBackendInterface> {
 public:
  /*!
   * \brief Type alias for the base Proxy type.
   */
  using BaseType = Proxy<ServiceInterface, ProxyBackendInterface>;

  /*!
   * \brief Type alias for the proxy specific HandleType.
   *
   * \trace SPEC-4980259, SPEC-4980344
   */
  using HandleType = ProxyHandleType;

  /*!
   * \brief Type alias for PollingFindServiceHandler specialization.
   */
  using PollingFindServiceHandlerType = ::amsr::socal::internal::PollingFindServiceHandler<HandleType>;

  /*!
   * \brief Use base class constructors.
   */
  using BaseType::Proxy;

  /*!
   * \brief Type alias for the FutureExecutor interface type.
   */
  using FutureExecutorInterfaceType = ::ara::core::internal::ExecutorInterface;

  /*!
   * \brief Type alias for the PollingModeFutureExecutor type.
   */
  using PollingModeFutureExecutorType = ::amsr::socal::internal::methods::PollingModeFutureExecutor;

  /*!
   * \brief Delete default constructor.
   */
  PollingProxy() noexcept = delete;

  /*!
   * \brief Delete the copy constructor.
   */
  PollingProxy(PollingProxy const&) noexcept = delete;

  /*!
   * \brief Delete the default copy assignment operator.
   */
  PollingProxy& operator=(PollingProxy const&) & noexcept = delete;

  /*!
   * \brief Delete the default move operator.
   */
  PollingProxy(PollingProxy&& other) noexcept = delete;

  /*!
   * \brief Delete the default move assignment operator.
   */
  PollingProxy& operator=(PollingProxy&&) & noexcept = delete;

  /*!
   * \brief   Find service from InstanceIdentifier.
   * \details Opposed to StartFindService(handler, instance) this version is a "one-shot" find request, which is
   *          synchronous, i.e. it returns after the find has been done and a result list of matching service instances
   *          is available. This will return all available service instances, that match the criteria.
   * \param[in] instance  The instance identifier of the required service instance that needs to be searched.
   * \return handles, that match the criteria (instance identifier) and to create a service proxy instance. Note that
   *         those handles shall be released before the Runtime is destroyed; i.e. they cannot be stored in variables
   *         with longer life period than the application's main(). If not followed, it's not guaranteed that the
   *         communication middleware is shut down properly and may lead to segmentation fault.
   * \pre         The given instance identifier must be configured in the ARXML model.
   * \pre         The given instance identifier must belong to the service interface.
   * \pre         Runtime has been initialized.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - If Socal is not initialized.
   *   - Log a fatal message and abort the application.
   * - Invokes the FindService method of PollingFindServiceHandler.
   *
   * Calls "ara::core::Abort()" if:
   * - Socal is not initialized.
   * - The instance identifier is not found in the lookup table (not configured in the ARXML model).
   * - ServiceDiscovery is not registered.
   * \endinternal
   *
   * \trace SPEC-4980367
   */
  static ::ara::com::ServiceHandleContainer<HandleType> FindService(::ara::com::InstanceIdentifier instance) noexcept {
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
    internal::InstanceSpecifierLookupTableEntry const service_instance{
        BaseType::ResolveInstanceSpecifierMapping(instance)};  // VCA_SOCAL_PASSING_FUNCTION_ARG_TO_ANOTHER_FUNCTION

    // VCA_SOCAL_CALLING_STL_APIS
    internal::InstanceSpecifierLookupTableEntryContainer const service_instances{service_instance};

    return PollingFindServiceHandlerType::FindService(service_instances, BaseType::GetServiceDiscoveryInternal());
  }  // VCA_SOCAL_CALLING_STL_APIS

  /*!
   * \brief   Find service from InstanceSpecifier.
   * \details Opposed to StartFindService(handler, instance) this version is a "one-shot" find request, which is
   *          synchronous, i.e. it returns after the find has been done and a result list of matching service instances
   *          is available. This will return all available service instances, that match the criteria.
   * \param[in] instance  The instance specifier of the required service interface, for which a matching service
   *                      instance should be searched.
   * \return handles, that match the criteria (instance identifier) and to create a service proxy instance. Note that
   *         those handles shall be released before the Runtime is destroyed. I.e. they cannot be stored in variables
   *         with longer life period than the application's main(). If not followed, it's not guaranteed that the
   *         communication middleware is shut down properly and may lead to segmentation fault.
   * \pre         The given instance specifier must be configured in the ARXML model.
   * \pre         The given instance specifier must belong to the service interface.
   * \pre         Runtime has been initialized.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   * \internal
   * - If Socal is not initialized.
   *   - Log a fatal message and abort the application.
   * - Invokes the FindService method of PollingFindServiceHandler.
   *
   * Calls "ara::core::Abort()" if:
   * - Socal is not initialized.
   * - The instance identifier is not found in the lookup table (not configured in the ARXML model).
   * - ServiceDiscovery is not registered.
   * \endinternal
   *
   * \trace SPEC-4980367
   */
  static ::ara::com::ServiceHandleContainer<HandleType> FindService(
      ::ara::core::InstanceSpecifier const& instance) noexcept {
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
    internal::InstanceSpecifierLookupTableEntryContainer const service_instances{
        BaseType::ResolveInstanceSpecifierMapping(instance)};

    return PollingFindServiceHandlerType::FindService(service_instances, BaseType::GetServiceDiscoveryInternal());
  }  // VCA_SOCAL_CALLING_STL_APIS

 protected:
  /*!
   * \brief Use default destructor.
   * \pre        -
   * \context    App
   * \threadsafe FALSE for same class instance, TRUE for different instances.
   * \reentrant  FALSE for same class instance, TRUE for different instances.
   * \vprivate
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ~PollingProxy() noexcept = default;  // VCA_SOCAL_DESTRUCTOR_STATIC_MEMORY

  /*!
   * \brief   Get the future executor instance.
   * \details The returned future executor is responsible for executing the callbacks registered in context of
   *          ara::core::Future::then() on a proxy method or field get/set requests, in polling mode.
   * \return The future executor instance.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   * \steady TRUE
   * \spec requires true; \endspec
   */
  virtual std::shared_ptr<FutureExecutorInterfaceType> GetFutureExecutorInstance() const noexcept
  /* COV_SOCAL_POLLING_MODE_FUNCTION */ {
    // VECTOR NC AutosarC++17_10-M9.3.1: MD_SOCAL_AutosarC++17_10-M9.3.1_nonConstPointerOrReference
    return polling_mode_future_executor_;
  }

 private:
  /*!
   * \brief Executor for callbacks registered in context of ara::core::Future::then() on a proxy method or field get/set
   *        requests, in polling mode.
   */
  std::shared_ptr<FutureExecutorInterfaceType> polling_mode_future_executor_{
      // VCA_SOCAL_EXTERNAL_FUNCTION_CONTRACT
      std::make_shared<PollingModeFutureExecutorType>()};
};
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_POLLING_PROXY_H_

// COV_JUSTIFICATION_BEGIN
//
// \ID COV_SOCAL_POLLING_MODE_FUNCTION
//  \ACCEPT XX
//  \REASON This function is executed only if the runtime processing mode configured is polling. For event-driven mode,
//   this function will not be executed. There is no risk by not testing in event driven mode.
//
// COV_JUSTIFICATION_END
