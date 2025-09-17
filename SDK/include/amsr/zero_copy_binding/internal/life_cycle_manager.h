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
 *         \brief  Lifecycle Manager for the binding.
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_INCLUDE_AMSR_ZERO_COPY_BINDING_INTERNAL_LIFE_CYCLE_MANAGER_H_
#define LIB_ZERO_COPY_BINDING_INCLUDE_AMSR_ZERO_COPY_BINDING_INTERNAL_LIFE_CYCLE_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/core/optional.h"
#include "amsr/zero_copy_binding/common/internal/factory_interface.h"
#include "amsr/zero_copy_binding/common/internal/service_discovery/types.h"
#include "amsr/zero_copy_binding/core/internal/access_control/access_control_interface.h"
#include "amsr/zero_copy_binding/core/internal/initialize_state_machine.h"
#include "amsr/zero_copy_binding/core/internal/life_cycle_manager_core.h"
#include "amsr/zero_copy_binding/core/internal/logging/zero_copy_logger.h"
#include "amsr/zero_copy_binding/internal/runtime.h"
#include "amsr/zero_copy_binding/transformation_layer/internal/life_cycle_manager_xf.h"
#include "ara/core/instance_specifier.h"

// VECTOR NL AutosarC++17_10-A16.0.1: MD_ZEROCOPYBINDING_A16.0.1_includeBetaDefinition
#ifdef AMSR_LG_BETA_ENABLED
#include "amsr/zero_copy_binding/generic/internal/life_cycle_manager_generic.h"
#endif  // AMSR_LG_BETA_ENABLED

namespace amsr {
namespace zero_copy_binding {
namespace internal {

/*!
 * \brief   Lifecycle Management of the Zero-Copy Binding.
 * \unit    ZeroCopyBinding::ZeroCopyBinding::LifeCycleManager
 */
class LifeCycleManager final : public core::internal::InitializeStateMachine {
  /*!
   * \brief Type of the provided service instance identifier.
   */
  using ProvidedServiceInstanceIdentifier = common::internal::service_discovery::ProvidedServiceInstanceIdentifier;

  /*!
   * \brief Type of the required service instance identifier.
   */
  using RequiredServiceInstanceIdentifier = common::internal::service_discovery::RequiredServiceInstanceIdentifier;

  /*!
   * \brief Type of the instance specifier.
   */
  using InstanceSpecifier = ::ara::core::InstanceSpecifier;

 public:
  /*!
   * \brief       Construct a LifeCycleManager.
   * \param[in]   runtime Valid runtime to communicate with Socal.
   * \param[in]   factory Factory to retrieve generated configurations and initialization functions.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component private
   */
  LifeCycleManager(std::unique_ptr<internal::Runtime> runtime,
                   std::shared_ptr<common::internal::FactoryInterface>&& factory) noexcept;
  LifeCycleManager(LifeCycleManager const&) = delete;
  auto operator=(LifeCycleManager const&) -> LifeCycleManager& = delete;
  LifeCycleManager(LifeCycleManager&&) = delete;
  auto operator=(LifeCycleManager&&) -> LifeCycleManager& = delete;
  ~LifeCycleManager() noexcept = default;  // VCA_ZEROCOPYBINDING_SLC_20_BASE_CLASS_METHOD

  /*!
   * \brief         Getter method for the LifeCycleManagerCore.
   * \return        A reference to the LifeCycleManagerCore. Valid as long as this instance exists.
   * \context       Any
   * \pre           -
   * \threadsafe    TRUE
   * \reentrant     TRUE
   * \synchronous   TRUE
   * \vprivate      Component private
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetLifeCycleManagerCore() -> core::internal::LifeCycleManagerCore& { return *life_cycle_manager_core_; }

 private:
  /*!
   * \brief       Initializer function to setup the LifeCycleManager.
   *
   * \context     Init
   * \pre         The object must not be initialized already.
   * \pre         Runtime and Factory may not be nullptr.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component private
   */
  void OnInitialize() noexcept final;

  /*!
   * \brief       Deinitializer function of the LifeCycleManager.
   *
   * \context     Shutdown
   * \pre         Initialize() must have been called before.
   * \pre         Runtime and Factory may not be nullptr.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component private
   */
  void OnDeinitialize() noexcept final;

  /*!
   * \brief The Logger.
   */
  core::internal::logging::ZeroCopyLogger& logger_;

  /*!
   * \brief A pointer to the Runtime.
   */
  std::unique_ptr<internal::Runtime> runtime_;

  /*!
   * \brief A pointer to the factory.
   */
  std::shared_ptr<common::internal::FactoryInterface> factory_;

  /*!
   * \brief A pointer to the LifeCycleManager of the Core.
   */
  std::shared_ptr<core::internal::LifeCycleManagerCore> life_cycle_manager_core_{nullptr};

  /*!
   * \brief The LifeCycleManager of the Transformation Layer.
   */
  ::amsr::core::Optional<transformation_layer::internal::LifeCycleManagerXf> life_cycle_manager_xf_{
      ::amsr::core::nullopt};

  /*!
   * \brief The LifeCycleManager of the generic proxy feature.
   */
// VECTOR NL AutosarC++17_10-A16.0.1: MD_ZEROCOPYBINDING_A16.0.1_includeBetaDefinition
#ifdef AMSR_LG_BETA_ENABLED
  ::amsr::core::Optional<generic::internal::LifeCycleManagerGeneric> life_cycle_manager_generic_{::amsr::core::nullopt};
#endif  // AMSR_LG_BETA_ENABLED
};

}  // namespace internal
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_INCLUDE_AMSR_ZERO_COPY_BINDING_INTERNAL_LIFE_CYCLE_MANAGER_H_
