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
 *         \brief  Lifecycle Manager for the Transformation Layer
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_LIFE_CYCLE_MANAGER_XF_H_
#define LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_LIFE_CYCLE_MANAGER_XF_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/zero_copy_binding/common/internal/config/config_view.h"
#include "amsr/zero_copy_binding/common/internal/factory_interface.h"
#include "amsr/zero_copy_binding/common/internal/service_discovery_proxy_xf_interface.h"
#include "amsr/zero_copy_binding/common/internal/service_discovery_skeleton_xf_interface.h"
#include "amsr/zero_copy_binding/core/internal/initialize_state_machine.h"
#include "amsr/zero_copy_binding/core/internal/life_cycle_manager_core.h"
#include "amsr/zero_copy_binding/core/internal/logging/zero_copy_logger.h"
#include "vac/testing/test_adapter.h"

namespace amsr {
namespace zero_copy_binding {
namespace transformation_layer {
namespace internal {

/*!
 * \brief  Lifecycle Management of the ZeroCopyBinding Transformation Layer.
 * \unit   ZeroCopyBinding::ZeroCopyBindingTransformationLayer::ZeroCopyXfLifecycleManagement
 * \vprivate
 */
class LifeCycleManagerXf final : public core::internal::InitializeStateMachine {
 public:
  /*!
   * \brief       Constructor of LifeCycleManagerXf.
   * \param[in]   factory Factory object to obtain generated code and external dependencies from. Must not be nullptr.
   * \context     Init
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   */
  explicit LifeCycleManagerXf(
      std::shared_ptr<::amsr::zero_copy_binding::common::internal::FactoryInterface const> factory);

  /*!
   * \brief       Initializer function setting up provided and required service instances.
   * \context     Init
   * \pre         LifeCycleManagerXf is not already initialized.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   */
  void Initialize(std::shared_ptr<core::internal::LifeCycleManagerCore> const&) noexcept;

  /*!
   * \brief       Destructor of LifeCycleManagerXf.
   * \context     Shutdown
   * \pre         LifeCycleManagerXf is deinitialized.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   */
  ~LifeCycleManagerXf() noexcept = default;  // VCA_ZEROCOPYBINDING_SLC_20_BASE_CLASS_METHOD

  LifeCycleManagerXf(LifeCycleManagerXf const&) = delete;
  auto operator=(LifeCycleManagerXf const&) -> LifeCycleManagerXf& = delete;
  LifeCycleManagerXf(LifeCycleManagerXf&&) = delete;
  auto operator=(LifeCycleManagerXf&&) -> LifeCycleManagerXf& = delete;

 private:
  /*!
   * \brief Hide InitializeStateMachine::Initialize() which we do not want to use, since we provide our own
   *        Initialize() method with different signature.
   */
  void Initialize() noexcept;

  /*!
   * \brief Hide InitializeStateMachine::OnInitialize() which we do not want to use, since we provide our own
   *        Initialize() method with different signature.
   */
  void OnInitialize() noexcept final;

  /*!
   * \brief       Deinitializer function for all required and provided service instances.
   * \context     Shutdown
   * \pre         LifeCycleManagerXf is initialized.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  void OnDeinitialize() noexcept final;

  /*!
   * \brief Logger for debug output.
   */
  ::amsr::zero_copy_binding::core::internal::logging::ZeroCopyLogger& logger_;

  /*!
   * \brief The factory object to use for generated code and external dependencies.
   */
  std::shared_ptr<::amsr::zero_copy_binding::common::internal::FactoryInterface const> factory_;

  /*!
   * \brief The model configuration of the ZeroCopyBinding.
   */
  ::amsr::zero_copy_binding::common::internal::config::ZeroCopyBindingConfig zero_copy_binding_config_{};

  /*!
   * \brief Type alias for the instance state listener interface.
   */
  using InstanceStateListenerPtr =
      std::unique_ptr<::amsr::zero_copy_binding::common::internal::ServiceDiscoveryProxyXfInterface>;

  /*!
   * \brief The container of state listeners to Socal service discovery.
   */
  std::vector<InstanceStateListenerPtr> sd_proxy_xfs_{};

  /*!
   * \brief Type alias for the ServiceDiscoverySkeletonXf interface.
   */
  using SDSkeletonXfPtr =
      std::unique_ptr<::amsr::zero_copy_binding::common::internal::ServiceDiscoverySkeletonXfInterface>;

  /*!
   * \brief The container of ServiceDiscoverySkeletonXf instances handled by this binding.
   */
  std::vector<SDSkeletonXfPtr> sd_skeleton_xfs_{};

  /*! Friend test declaration. */
  FRIEND_TEST(UT__ZeroCopyXfLifecycleManagement__LifeCycleManagerXf__PredefinedDeInitializers,
              ConfigProvidedToInitializers);
};

}  // namespace internal
}  // namespace transformation_layer
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_LIFE_CYCLE_MANAGER_XF_H_
