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
 *         \brief  Transformation Layer factory at skeleton side to create SkeletonXf objects on demand.
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_DEFAULT_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_SKELETON_XF_FACTORY_H_
#define LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_DEFAULT_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_SKELETON_XF_FACTORY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/core/result.h"
#include "amsr/socal/internal/factory/skeleton_backend_factory_interface.h"
#include "amsr/zero_copy_binding/common/internal/config/config_view.h"
#include "amsr/zero_copy_binding/core/internal/life_cycle_manager_core.h"
#include "amsr/zero_copy_binding/core/internal/logging/zero_copy_logger.h"

namespace amsr {
namespace zero_copy_binding {
namespace transformation_layer {
namespace internal {

/*!
 * \brief   Transformation Layer factory class at skeleton side.
 * \tparam  SkeletonXf  The skeleton backend class of which an object shall be created.
 * \tparam  Skeleton    The Socal skeleton class the user called OfferService() on.
 *
 * \unit  ZeroCopyBinding::ZeroCopyBindingTransformationLayer::ProvidedServiceInstance
 */
template <typename SkeletonXf, typename Skeleton>
class SkeletonXfFactory final : public ::amsr::socal::internal::factory::SkeletonBackendFactoryInterface<
                                    typename SkeletonXf::SkeletonBackendInterface> {
 public:
  using SkeletonBackendFactoryInterface =
      ::amsr::socal::internal::factory::SkeletonBackendFactoryInterface<typename SkeletonXf::SkeletonBackendInterface>;
  using SkeletonType = Skeleton;

  /*!
   * \brief       Constructor of SkeletonXfFactory.
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
   * \vprivate    Component private
   *
   * \internal
   * - Hand over the instance id specific logging prefix to the constructor of the logger.
   * - Store the config view.
   * - Store a weak pointer to the lifecycle manager core.
   * - Abort if the passed lifecycle manager core is invalid.
   * \endinternal
   */
  SkeletonXfFactory(common::internal::config::ConfigView const zero_copy_binding_config_view,
                    std::shared_ptr<core::internal::LifeCycleManagerCore> const& life_cycle_manager_core)
      : SkeletonBackendFactoryInterface{},
        // VCA_ZEROCOPYBINDING_SLC_20_LOGGER
        logger_{
            "SkeletonXfFactory(" +  // VCA_ZEROCOPYBINDING_SPC_15_STRING_OPERATOR_PLUS
            std::to_string(
                // VCA_ZEROCOPYBINDING_LNG_03_VALID_CONFIG_VIEW, VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
                zero_copy_binding_config_view.AsProvidedConfig().instance_id.value) +
            ")"},
        zero_copy_binding_config_view_{zero_copy_binding_config_view},
        life_cycle_manager_core_{life_cycle_manager_core} {
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
   * \vprivate      Component private
   */
  // VCA_ZEROCOPYBINDING_SLC_23_MEMORY_DEALLOCATION_IN_DTOR, VCA_ZEROCOPYBINDING_SPC_15_STD_FUNCTIONS
  ~SkeletonXfFactory() noexcept final = default;

  SkeletonXfFactory() = delete;
  SkeletonXfFactory(SkeletonXfFactory const&) = delete;
  SkeletonXfFactory(SkeletonXfFactory&&) = delete;
  auto operator=(SkeletonXfFactory const&) -> SkeletonXfFactory& = delete;
  auto operator=(SkeletonXfFactory&&) -> SkeletonXfFactory& = delete;

  /*!
   * \brief         Create a new SkeletonXf object and hand over ownership to caller.
   * \details       The function is called from Socal in context of skeleton construction.
   * \return        A binding specific SkeletonXf object via a std::unique_ptr of Socal's interface class.
   * \context       App
   * \pre           -
   * \threadsafe    FALSE for same class instance, TRUE for different instances.
   * \reentrant     FALSE for same class instance, TRUE for different instances.
   * \synchronous   TRUE
   * \vprivate      Component private
   *
   * \internal
   * - Obtain shared pointer to LifeCycleManagerCore.
   * - Create new unique pointer to SkeletonXf object and return it.
   * \endinternal
   */
  auto CreateBackend() noexcept
      -> ::amsr::core::Result<typename SkeletonBackendFactoryInterface::BackendInterfacePtr> final {
    LOG_DEBUG(logger_, "SkeletonXfFactory::CreateBackend() called.");

    std::shared_ptr<core::internal::LifeCycleManagerCore> life_cycle_manager_core_shared_ptr{
        life_cycle_manager_core_.lock()};
    if (life_cycle_manager_core_shared_ptr == nullptr) {
      LOG_FATAL_AND_ABORT(logger_, "The lifetime of LifeCycleManagerCore has expired, aborting.");
    }

    return ::amsr::core::Result<typename SkeletonBackendFactoryInterface::BackendInterfacePtr>{
        // VCA_ZEROCOPYBINDING_SPC_15_VALID_PERFECTLY_FORWARDED_CTOR_CALL
        std::make_unique<SkeletonXf>(zero_copy_binding_config_view_, std::move(life_cycle_manager_core_shared_ptr))};
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
   * \brief A pointer to the LifeCycleManagerCore. Only a weak pointer, as it should not be owned by this class.
   */
  std::weak_ptr<core::internal::LifeCycleManagerCore> const life_cycle_manager_core_;
};

}  // namespace internal
}  // namespace transformation_layer
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_DEFAULT_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_SKELETON_XF_FACTORY_H_
