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
 *         \brief  Transformation Layer factory at proxy side to create ProxyXf objects on demand.
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_DEFAULT_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_PROXY_XF_FACTORY_H_
#define LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_DEFAULT_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_PROXY_XF_FACTORY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/core/result.h"
#include "amsr/socal/internal/factory/proxy_backend_factory_interface.h"
#include "amsr/socal/internal/internal_instance_identifier.h"
#include "amsr/zero_copy_binding/common/internal/config/config_view.h"
#include "amsr/zero_copy_binding/common/internal/config/types.h"
#include "amsr/zero_copy_binding/common/internal/service_discovery/types.h"
#include "amsr/zero_copy_binding/core/internal/life_cycle_manager_core.h"
#include "amsr/zero_copy_binding/core/internal/logging/zero_copy_logger.h"
#include "amsr/zero_copy_binding/transformation_layer/internal/ara_com_instance_id_xf.h"

namespace amsr {
namespace zero_copy_binding {
namespace transformation_layer {
namespace internal {

/*!
 * \brief   Transformation Layer factory class at proxy side.
 * \tparam  ProxyXf  The proxy backend class of which an object shall be created.
 *
 * \unit  ZeroCopyBinding::ZeroCopyBindingTransformationLayer::RequiredServiceInstance
 */
template <typename ProxyXf>
class ProxyXfFactory final
    : public ::amsr::socal::internal::factory::ProxyBackendFactoryInterface<typename ProxyXf::ProxyBackendInterface> {
  using ProvidedInstanceIdentifier = ::amsr::socal::internal::ProvidedInstanceIdentifier;
  using RequiredInstanceIdentifier = ::amsr::socal::internal::RequiredInstanceIdentifier;
  using RequiredServiceInstanceIdentifier = common::internal::service_discovery::RequiredServiceInstanceIdentifier;
  using ProvidedServiceInstanceIdentifier = common::internal::service_discovery::ProvidedServiceInstanceIdentifier;
  using AraComInstanceIdXf = transformation_layer::internal::AraComInstanceIdXf;

 public:
  using ProxyType = typename ProxyXf::ProxyClass;
  using ProxyBackendInterface = typename ProxyXf::ProxyBackendInterface;
  using ProxyBackendFactoryInterface =
      ::amsr::socal::internal::factory::ProxyBackendFactoryInterface<ProxyBackendInterface>;

  /*!
   * \brief       Constructor of ProxyXfFactory.
   * \param[in]   zero_copy_binding_config_view     A view to the binding's config struct.
   *                                                The LifeCycleManagerXf has to exist for the object's entire
   *                                                lifetime to ensure its validity.
   * \param[in]   life_cycle_manager_core           A pointer to the LifeCycleManagerCore.
   *                                                The LifeCycleManagerCore has to exist for the object's entire
   *                                                lifetime.
   * \context     Init
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product private
   *
   * \internal
   * - Hand over the instance id specific logging prefix to the constructor of the logger.
   * - Store the config view.
   * - Construct an AraComInstanceIdentifier and a service discovery service instance identifier out of the data from
   *   the config struct and store them inside a struct.
   * - Store a weak pointer to the lifecycle manager core.
   * - Get and store the Socal service discovery singleton access.
   * - Abort if the passed lifecycle manager core is invalid.
   * \endinternal
   */
  ProxyXfFactory(common::internal::config::ConfigView const zero_copy_binding_config_view,
                 std::shared_ptr<core::internal::LifeCycleManagerCore> const& life_cycle_manager_core)
      : ProxyBackendFactoryInterface{},
        // VCA_ZEROCOPYBINDING_SLC_20_LOGGER
        logger_{
            "ProxyXfFactory(" +  // VCA_ZEROCOPYBINDING_SPC_15_STRING_OPERATOR_PLUS
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
        life_cycle_manager_core_{life_cycle_manager_core},
        socal_service_discovery_{ProxyType::GetServiceDiscovery()} {
    if (life_cycle_manager_core == nullptr) {
      LOG_FATAL_AND_ABORT(logger_, "The passed LifeCycleManagerCore is invalid, aborting.");
    }
  }

  /*!
   * \brief         Default destructor.
   * \pre           -
   * \context       Shutdown
   * \threadsafe    FALSE for same class instance, TRUE for different instances.
   * \reentrant     FALSE for same class instance, TRUE for different instances.
   * \synchronous   TRUE
   * \vprivate      Product private
   */
  // VCA_ZEROCOPYBINDING_SLC_23_MEMORY_DEALLOCATION_IN_DTOR, VCA_ZEROCOPYBINDING_SPC_15_STD_FUNCTIONS
  ~ProxyXfFactory() noexcept final = default;

  ProxyXfFactory() = delete;
  ProxyXfFactory(ProxyXfFactory const&) = delete;
  ProxyXfFactory(ProxyXfFactory&&) = delete;
  auto operator=(ProxyXfFactory const&) -> ProxyXfFactory& = delete;
  auto operator=(ProxyXfFactory&&) -> ProxyXfFactory& = delete;

  /*!
   * \brief       Create a new proxy backend object and hand over ownership to caller.
   * \details     The function is called from Socal in context of proxy construction.
   * \param[in]   provided_instance_id  A ProvidedInstanceIdentifier of the format ZeroCopy:0..4294967294 (0xFFFFFFFE).
   * \return      A binding specific ProxyXf object via a std::unique_ptr of Socal's interface class.
   * \context     App
   * \pre         The provided instance id must be valid and equal to the required instance id this class represents.
   * \threadsafe  TRUE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product private
   * \internal steady FALSE \endinternal
   *
   * \internal
   * - Check if the ProvidedInstanceIdentifier can be converted to a ServiceInstanceIdentifier and abort otherwise
   *   (this is the case when the format was incorrect).
   * - Check if the required service instance identifier is matching the provided service instance identifier and abort
   *   otherwise.
   * - Construct and return unique pointer to the new instance of type ProxyXf.
   * \endinternal
   */
  auto CreateBackend(ProvidedInstanceIdentifier const& provided_instance_id) noexcept
      -> ::amsr::core::Result<std::unique_ptr<ProxyBackendInterface>> final {
    ::amsr::core::Result<common::internal::service_discovery::ServiceInstanceIdentifier>
        provided_zero_copy_instance_identifier_result{
            // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
            AraComInstanceIdXf::BuildServiceInstanceIdentifier(provided_instance_id.GetAraComInstanceIdentifier())};
    if (!provided_zero_copy_instance_identifier_result.HasValue()) {
      // VCA_ZEROCOPYBINDING_SLC_20_SAFE_LOG_FUNCTION
      LOG_FATAL_AND_ABORT(
          logger_,
          ([&provided_instance_id,
            error_message = provided_zero_copy_instance_identifier_result.Error().UserMessage()](auto& stream) {
            stream << "ProvidedInstanceIdentifier with invalid format or content provided (" << provided_instance_id
                   << "). Further information: '" << error_message << "'. Aborting.";
          }));
    }

    ProvidedServiceInstanceIdentifier const provided_zero_copy_instance_id{
        provided_zero_copy_instance_identifier_result.Value()};
    // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
    if (!required_service_instance_id_.service_discovery_id.IsMatching(provided_zero_copy_instance_id)) {
      // VCA_ZEROCOPYBINDING_SLC_20_SAFE_LOG_FUNCTION
      LOG_FATAL_AND_ABORT(
          logger_, ([&provided_instance_id,
                     &required_instance_id = required_service_instance_id_.service_discovery_id](auto& stream) {
            stream << "Proxy backend is created for a ProvidedInstanceId " << provided_instance_id
                   << " not matching the registered RequiredServiceInstance " << required_instance_id << ", aborting.";
          }));
    }

    // VCA_ZEROCOPYBINDING_SLC_20_SAFE_LOG_FUNCTION
    LOG_DEBUG(logger_, [&provided_instance_id](auto& stream) {
      stream << "Creating Proxy backend for the ProvidedInstanceId" << provided_instance_id << ".";
    });

    std::shared_ptr<core::internal::LifeCycleManagerCore> life_cycle_manager_core_shared_ptr{
        life_cycle_manager_core_.lock()};

    if (life_cycle_manager_core_shared_ptr == nullptr) {
      LOG_FATAL_AND_ABORT(logger_, "The lifetime of LifeCycleManagerCore has expired, aborting.");
    }

    // VECTOR NC AutosarC++17_10-A18.5.8: MD_ZEROCOPYBINDING_A18.5.8_movedObjectDetectedAsLocal
    // VCA_ZEROCOPYBINDING_SPC_15_VALID_PERFECTLY_FORWARDED_CTOR_CALL
    std::unique_ptr<ProxyXf> proxy_xf{std::make_unique<ProxyXf>(
        zero_copy_binding_config_view_, std::move(life_cycle_manager_core_shared_ptr), provided_instance_id)};
    // The proxy backend will remove itself as a requester in its destructor.
    // VECTOR NC VectorC++-V5.2.3, AutosarC++17_10-M5.2.3: MD_ZEROCOPYBINDING_V5.2.3_downcastSocalServiceDiscovery
    // VCA_ZEROCOPYBINDING_LNG_03_VALID_OBJECT_SOCAL_SERVICE_DISCOVERY_PROXY
    static_cast<typename ProxyXf::ProxyServiceDiscovery*>(socal_service_discovery_.operator->())
        //  VCA_ZEROCOPYBINDING_SPC_15_SOCAL_SD_PROXY_ADD_REQUESTER_FUNCTION_CONTRACT_FULFILLED
        ->AddRequester(required_service_instance_id_.socal_id, provided_instance_id, proxy_xf.get());
    return ::amsr::core::Result<std::unique_ptr<ProxyBackendInterface>>{std::move(proxy_xf)};
  }

 private:
  /*!
   * \brief The logger.
   */
  core::internal::logging::ZeroCopyLogger logger_;

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
   * \brief A pointer to the LifeCycleManagerCore. Only a weak pointer, as it should not be owned by this class.
   */
  std::weak_ptr<core::internal::LifeCycleManagerCore> const life_cycle_manager_core_;

  /*!
   * \brief The Socal service discovery singleton access.
   */
  typename ProxyType::ServiceDiscoverySingletonAccess socal_service_discovery_;
};

}  // namespace internal
}  // namespace transformation_layer
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_DEFAULT_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_PROXY_XF_FACTORY_H_
