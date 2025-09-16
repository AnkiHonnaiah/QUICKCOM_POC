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
 *         \brief  Transformation Layer service discovery factory at proxy side.
 *
 *       \details  Registers and deregisters required service to the service discovery and Socal.
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_SERVICE_DISCOVERY_PROXY_XF_H_
#define LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_SERVICE_DISCOVERY_PROXY_XF_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <mutex>
#include <string>
#include <type_traits>
#include <utility>

#include "amsr/core/result.h"
#include "amsr/socal/internal/internal_instance_identifier.h"
#include "amsr/zero_copy_binding/common/internal/config/config_view.h"
#include "amsr/zero_copy_binding/common/internal/config/types.h"
#include "amsr/zero_copy_binding/common/internal/config/zero_copy_binding_config.h"
#include "amsr/zero_copy_binding/common/internal/service_discovery/types.h"
#include "amsr/zero_copy_binding/common/internal/service_discovery_proxy_xf_interface.h"
#include "amsr/zero_copy_binding/core/internal/initialize_state_machine.h"
#include "amsr/zero_copy_binding/core/internal/life_cycle_manager_core.h"
#include "amsr/zero_copy_binding/core/internal/logging/zero_copy_logger.h"
#include "amsr/zero_copy_binding/transformation_layer/internal/ara_com_instance_id_xf.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace zero_copy_binding {
namespace transformation_layer {
namespace internal {

/*!
 * \brief   Transformation Layer service discovery factory at proxy side that handles the service discovery.
 * \tparam  ProxyXf                The Proxy backend class of which the ServiceDiscoveryProxyXf creates objects.
 *
 * \unit  ZeroCopyBinding::ZeroCopyBindingTransformationLayer::RequiredServiceInstance
 */
template <typename ProxyXf>
class ServiceDiscoveryProxyXf final : public common::internal::ServiceDiscoveryProxyXfInterface,
                                      public core::internal::InitializeStateMachine {
 public:
  using ProvidedInstanceIdentifier = ::amsr::socal::internal::ProvidedInstanceIdentifier;
  using RequiredInstanceIdentifier = ::amsr::socal::internal::RequiredInstanceIdentifier;
  using RequiredServiceInstanceIdentifier = common::internal::service_discovery::RequiredServiceInstanceIdentifier;
  using AraComInstanceIdXf = transformation_layer::internal::AraComInstanceIdXf;
  using ProxyType = typename ProxyXf::ProxyClass;

  /*!
   * \brief       Constructor of ServiceDiscoveryProxyXf.
   * \param[in]   zero_copy_binding_config_view     A view to the binding's config struct.
   *                                                The LifeCycleManagerXf has to exist for the object's entire
   *                                                lifetime to ensure its validity.
   *              life_cycle_manager_core           A pointer to the LifeCycleManagerCore.
   *                                                The LifeCycleManagerCore has to exist for the object's entire
   *                                                lifetime.
   * \context     Init
   * \pre         The Socal service discovery obtained from the singleton must be valid over the object's entire
   *              lifetime.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   *
   * \internal
   * - Hand over the instance id specific logging prefix to the constructor of InitializeStateMachine.
   * - Store the config view.
   * - Construct an AraComInstanceIdentifier and a service discovery service instance identifier out of the data from
   *   the config struct and store them inside a struct.
   * - Store a weak pointer to the lifecycle manager core.
   * - Obtain and store a shared pointer to the service discovery.
   * - Get and store the Socal service discovery singleton access.
   * - Set the logger reference to InitializeStateMachine's logger.
   * \endinternal
   */
  ServiceDiscoveryProxyXf(common::internal::config::ConfigView const zero_copy_binding_config_view,
                          std::shared_ptr<core::internal::LifeCycleManagerCore> const& life_cycle_manager_core)
      : common::internal::ServiceDiscoveryProxyXfInterface{},
        core::internal::  // VCA_ZEROCOPYBINDING_SLC_20_BASE_CLASS_METHOD
        InitializeStateMachine{
            "ServiceDiscoveryProxyXf(" +  // VCA_ZEROCOPYBINDING_SPC_15_STRING_OPERATOR_PLUS
            std::to_string(
                // VCA_ZEROCOPYBINDING_LNG_03_VALID_CONFIG_VIEW, VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
                zero_copy_binding_config_view.AsRequiredConfig().instance_id.value) +
            ")"},
        zero_copy_binding_config_view_{zero_copy_binding_config_view},
        required_service_instance_id_{
            // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
            RequiredInstanceIdentifier{
                // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
                AraComInstanceIdXf::BuildAraComInstanceIdentifier(RequiredServiceInstanceIdentifier{
                    // VCA_ZEROCOPYBINDING_LNG_03_VALID_CONFIG_VIEW, VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
                    zero_copy_binding_config_view.AsServiceConfig().service_id.value,
                    // VCA_ZEROCOPYBINDING_LNG_03_VALID_CONFIG_VIEW, VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
                    zero_copy_binding_config_view.AsRequiredConfig().instance_id.value,
                    // VCA_ZEROCOPYBINDING_LNG_03_VALID_CONFIG_VIEW, VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
                    zero_copy_binding_config_view.AsServiceConfig().major_version.value,
                    // VCA_ZEROCOPYBINDING_LNG_03_VALID_CONFIG_VIEW, VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
                    zero_copy_binding_config_view.AsServiceConfig().minor_version.value})},
            // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
            RequiredServiceInstanceIdentifier{
                // VCA_ZEROCOPYBINDING_LNG_03_VALID_CONFIG_VIEW, VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
                zero_copy_binding_config_view.AsServiceConfig().service_id.value,
                // VCA_ZEROCOPYBINDING_LNG_03_VALID_CONFIG_VIEW, VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
                zero_copy_binding_config_view.AsRequiredConfig().instance_id.value,
                // VCA_ZEROCOPYBINDING_LNG_03_VALID_CONFIG_VIEW, VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
                zero_copy_binding_config_view.AsServiceConfig().major_version.value,
                // VCA_ZEROCOPYBINDING_LNG_03_VALID_CONFIG_VIEW, VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
                zero_copy_binding_config_view.AsServiceConfig().minor_version.value}},
        // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
        service_discovery_{life_cycle_manager_core->GetServiceDiscovery()},
        socal_service_discovery_{ProxyType::GetServiceDiscovery()},
        // VCA_ZEROCOPYBINDING_SLC_20_GET_LOGGER
        logger_{GetLogger()} {}

  /*!
   * \brief Default destructor.
   */
  // VCA_ZEROCOPYBINDING_SLC_23_MEMORY_DEALLOCATION_IN_DTOR, VCA_ZEROCOPYBINDING_SPC_15_STD_FUNCTIONS, VCA_ZEROCOPYBINDING_SLC_20_BASE_CLASS_METHOD
  ~ServiceDiscoveryProxyXf() noexcept final = default;

  ServiceDiscoveryProxyXf() = delete;
  ServiceDiscoveryProxyXf(ServiceDiscoveryProxyXf const&) = delete;
  ServiceDiscoveryProxyXf(ServiceDiscoveryProxyXf&&) = delete;
  auto operator=(ServiceDiscoveryProxyXf const&) -> ServiceDiscoveryProxyXf& = delete;
  auto operator=(ServiceDiscoveryProxyXf&&) -> ServiceDiscoveryProxyXf& = delete;

 private:
  /*!
   * \brief       Initialize the proxy backend class.
   * \details     Start listening to service discovery.
   * \context     Init
   * \pre         The ServiceDiscoveryProxyXf object is not already initialized.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   *
   * \internal
   * - Start listening to service discovery. Abort if this fails.
   * \endinternal
   */
  void OnInitialize() noexcept final {
    LOG_DEBUG(logger_, "ServiceDiscoveryProxyXf::OnInitialize()");

    // Start listening to service.
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_ZEROCOPYBINDING_M0.3.1_validServiceDiscovery
    service_discovery_
        // VCA_ZEROCOPYBINDING_FORWARDING_FUNCTION_ON_VALID_OBJECT
        ->ListenService(required_service_instance_id_.service_discovery_id, this->GetServiceDiscoveryUpdateHandler())
        .ConsumeError([&logger = logger_, &required_instance_id = required_service_instance_id_.socal_id](auto const&) {
          // VCA_ZEROCOPYBINDING_SLC_20_SAFE_LOG_FUNCTION
          LOG_FATAL_AND_ABORT(logger, [&required_instance_id](auto& stream) {
            stream << "Listening to service discovery failed (required instance id: " << required_instance_id
                   << "), aborting.";
          });
        });
  }

  /*!
   * \brief       Deinitialize the proxy backend class.
   * \details     Stop listening to service discovery.
   * \context     Shutdown
   * \pre         The ServiceDiscoveryProxyXf object is initialized.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   *
   * \internal
   * - Stop listening to service discovery. Abort if this fails.
   * \endinternal
   */
  void OnDeinitialize() noexcept final {
    LOG_DEBUG(logger_, "ServiceDiscoveryProxyXf::OnDeinitialize()");

    // Stop listening to service.
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_ZEROCOPYBINDING_M0.3.1_validServiceDiscovery
    // VCA_ZEROCOPYBINDING_FORWARDING_FUNCTION_ON_VALID_OBJECT
    service_discovery_->StopListenService(required_service_instance_id_.service_discovery_id)
        .ConsumeError([&logger = logger_, &required_instance_id = required_service_instance_id_.socal_id](auto const&) {
          // VCA_ZEROCOPYBINDING_SLC_20_SAFE_LOG_FUNCTION
          LOG_FATAL_AND_ABORT(logger, [&required_instance_id](auto& stream) {
            stream << "Stop listening to service discovery failed (required instance id: " << required_instance_id
                   << "), aborting.";
          });
        });
  }

  /*!
   * \brief       Callback to be called by the ServiceDiscovery on status updates.
   * \return      The update handler.
   * \details     The update handler calls the methods OfferService() or StopOfferService() of the Socal service
   *              discovery depending on the service state reported by the service discovery. Note that the handler
   *              captures a this pointer. It is ensured that the handler does not outlive the ServiceDiscoveryProxyXf
   *              instance, since we stop listening in the Deinitialize() method.
   * \context     Init (Reactor for the returned handler)
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   *
   * \internal
   * - Return a lambda (packed into an object of type ListenServiceCallback) that handles an incoming state change of a
   *   provided service instance via the service discovery.
   * \endinternal
   */
  auto GetServiceDiscoveryUpdateHandler() noexcept -> common::internal::service_discovery::ListenServiceCallback {
    using ServiceInstanceState = common::internal::service_discovery::ProvidedState;
    using ServiceInstanceEndpoint = common::internal::service_discovery::ServiceInstanceEndpoint;
    /*!
     * \brief       Callback to be called when a service instance appeared or disappeared.
     * \param[in]   provided_service_instance_identifier  The provided service instance identifier.
     *                                                    Must coincide with the ServiceDiscoveryProxyXf's required
     *                                                    service instance identifier.
     * \param[in]   service_instance_endpoint             The service instance endpoint that contains amongst others the
     *                                                    provided service instance identifier of the offered/stopped
     *                                                    service.
     * \param[in]   service_instance_state                The new state of the provided service.
     * \details     SOCAL service discovery is informed about the service offer or stop offer.
     * \context     Reactor
     * \pre         -
     * \threadsafe  TRUE
     * \reentrant   TRUE
     * \synchronous TRUE
     * \vprivate    Component private
     *
     * \internal
     * - Check that the required service identifier the lambda is called with coincides with the required service
     *   identifier the ServiceDiscoveryProxyXf represents.
     * - Depending on the service instance state do:
     *   - If the reported service state is Provided, then forward the service offer to Socal.
     *   - If the reported service state is NotProvided, then stop offering the service via Socal.
     *   - Otherwise abort the program.
     * \endinternal
     */
    return [this](RequiredServiceInstanceIdentifier required_service_instance_identifier,
                  ServiceInstanceEndpoint service_instance_endpoint, ServiceInstanceState service_instance_state) {
      // VCA_ZEROCOPYBINDING_LNG_03_CALLING_NONSTATIC_METHOD_ON_INITIALIZED_MEMBER
      if (required_service_instance_id_.service_discovery_id != required_service_instance_identifier) {
        // VCA_ZEROCOPYBINDING_SLC_20_SAFE_LOG_FUNCTION
        LOG_FATAL_AND_ABORT(
            this->logger_, ([this, &required_service_instance_identifier](auto& stream) {
              stream << "The required service instance identifier received by the service discovery update handler ("
                     << required_service_instance_identifier << ") does not match the one of the proxy backend ("
                     << this->required_service_instance_id_.service_discovery_id << "), aborting.";
            }));
      }
      // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
      ProvidedInstanceIdentifier const provided_service_instance_identifier{
          // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
          AraComInstanceIdXf::BuildAraComInstanceIdentifier(
              service_instance_endpoint.provided_service_instance_identifier)};
      switch (service_instance_state) {
        case ServiceInstanceState::kProvided:
          // VCA_ZEROCOPYBINDING_SLC_20_SAFE_LOG_FUNCTION
          LOG_DEBUG(logger_, [&provided_service_instance_identifier](auto& stream) {
            stream << "ServiceDiscoveryUpdateHandler with ServiceInstanceState::kProvided is called for the provided "
                      "instance id "
                   << provided_service_instance_identifier << ".";
          });
          // VCA_ZEROCOPYBINDING_LNG_01_VALID_CONST_REFERENCE_ARGUMENT, VCA_ZEROCOPYBINDING_LNG_03_VALID_OBJECT_SOCAL_SERVICE_DISCOVERY_PROXY, VCA_ZEROCOPYBINDING_SPC_15_SOCAL_SD_PROXY_OFFER_SERVICE_FUNCTION_CONTRACT_FULFILLED
          socal_service_discovery_->OfferService(required_service_instance_id_.socal_id,
                                                 provided_service_instance_identifier);
          break;
        case ServiceInstanceState::kNotProvided:
          // VCA_ZEROCOPYBINDING_SLC_20_SAFE_LOG_FUNCTION
          LOG_DEBUG(logger_, [&provided_service_instance_identifier](auto& stream) {
            stream
                << "ServiceDiscoveryUpdateHandler with ServiceInstanceState::kNotProvided is called for the provided "
                   "instance id "
                << provided_service_instance_identifier << ".";
          });
          // VCA_ZEROCOPYBINDING_LNG_01_VALID_CONST_REFERENCE_ARGUMENT, VCA_ZEROCOPYBINDING_LNG_03_VALID_OBJECT_SOCAL_SERVICE_DISCOVERY_PROXY, VCA_ZEROCOPYBINDING_SPC_15_SOCAL_SD_PROXY_OFFER_SERVICE_FUNCTION_CONTRACT_FULFILLED
          socal_service_discovery_->StopOfferService(required_service_instance_id_.socal_id,
                                                     provided_service_instance_identifier);
          break;
        default:
          LOG_FATAL_AND_ABORT(this->logger_, "Unknown ServiceInstanceState provided, aborting.");
          break;
      }
    };
  }

  /*!
   * \brief       A helper method that will be used while deinitializing the LifeCycleManagerXf.
   * \return      A pointer to the object's InitializeStateMachine part.
   * \context     Shutdown
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   *
   * \internal
   * - Return this pointer casted to InitializeStateMachine pointer.
   * \endinternal
   */
  auto AsInitializeStateMachine() noexcept -> core::internal::InitializeStateMachine* final { return this; }

  /*!
   * \brief A view to the binding's configuration data.
   */
  common::internal::config::ConfigView const zero_copy_binding_config_view_;

  /*!
   * \brief The required service instance identifier this ServiceDiscoveryProxyXf instance is representing in different
   *        data types.
   */
  struct {
    /*!
     * \brief The RequiredInstanceIdentifier type used by Socal.
     */
    RequiredInstanceIdentifier socal_id;

    /*!
     * \brief The RequiredServiceInstanceIdentifier used by the service discovery.
     */
    RequiredServiceInstanceIdentifier service_discovery_id;
  } required_service_instance_id_;

  /*!
   * \brief The pointer to the service discovery.
   */
  std::shared_ptr<common::internal::service_discovery::ServiceDiscoveryInterface> const service_discovery_;

  /*!
   * \brief The Socal service discovery singleton access.
   */
  typename ProxyType::ServiceDiscoverySingletonAccess socal_service_discovery_;

  /*!
   * \brief A reference to the logger that is stored in InitializeStateMachine.
   */
  core::internal::logging::ZeroCopyLogger& logger_;

  /*! Friend test declaration. */
  FRIEND_TEST(UT__RequiredServiceInstance__ServiceDiscoveryProxyXf, CreateBackendSuccessfullyCalled);

  /*! Friend test declaration. */
  FRIEND_TEST(UT__RequiredServiceInstance__ServiceDiscoveryProxyXf, CreateBackendOnInstanceIdAll);
};

}  // namespace internal
}  // namespace transformation_layer
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_SERVICE_DISCOVERY_PROXY_XF_H_
