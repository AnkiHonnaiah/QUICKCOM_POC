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
 *         \brief  Lifecycle Manager for the generic proxy feature.
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_GENERIC_INCLUDE_AMSR_ZERO_COPY_BINDING_GENERIC_INTERNAL_LIFE_CYCLE_MANAGER_GENERIC_H_
#define LIB_ZERO_COPY_BINDING_GENERIC_INCLUDE_AMSR_ZERO_COPY_BINDING_GENERIC_INTERNAL_LIFE_CYCLE_MANAGER_GENERIC_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/generic/activatable_source_code.h"
#include "amsr/zero_copy_binding/common/internal/config/zero_copy_binding_config.h"
#include "amsr/zero_copy_binding/common/internal/factory_interface.h"
#include "amsr/zero_copy_binding/core/internal/initialize_state_machine.h"
#include "amsr/zero_copy_binding/core/internal/life_cycle_manager_core.h"
#include "amsr/zero_copy_binding/generic/internal/generic_proxy_xf.h"
#include "amsr/zero_copy_binding/transformation_layer/internal/proxy_xf_factory.h"
#include "amsr/zero_copy_binding/transformation_layer/internal/service_discovery_proxy_xf.h"

namespace amsr {
namespace zero_copy_binding {
namespace generic {
namespace internal {

/*!
 * \brief  Lifecycle Management of the ZeroCopyBinding generic proxy feature.
 * \vprivate
 */
class AMSR_LG_BETA LifeCycleManagerGeneric final : public core::internal::InitializeStateMachine {
 public:
  /*!
   * \brief       Constructor of LifeCycleManagerGeneric.
   * \param[in]   zero_copy_binding_config  A reference to the binding's config struct.
   *                                        Has to be valid for the object's entire lifetime.
   * \context     Init
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  explicit LifeCycleManagerGeneric(
      common::internal::config::ZeroCopyBindingConfig const& zero_copy_binding_config) noexcept;

  /*!
   * \brief       Destructor of LifeCycleManagerGeneric.
   * \context     Shutdown
   * \pre         LifeCycleManagerGeneric is deinitialized.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   */
  ~LifeCycleManagerGeneric() noexcept = default;  // VCA_ZEROCOPYBINDING_SLC_20_BASE_CLASS_METHOD

  LifeCycleManagerGeneric(LifeCycleManagerGeneric const&) = delete;
  auto operator=(LifeCycleManagerGeneric const&) -> LifeCycleManagerGeneric& = delete;
  LifeCycleManagerGeneric(LifeCycleManagerGeneric&&) = delete;
  auto operator=(LifeCycleManagerGeneric&&) -> LifeCycleManagerGeneric& = delete;

  /*!
   * \brief       Initialize all configured generic required service interfaces.
   * \param[in]   life_cycle_manager_core  A pointer to the LifeCycleManagerCore.
   *                                       Must be valid until Deinitialize() is called.
   * \context     Init
   * \pre         LifeCycleManagerGeneric is not initialized.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   */
  void Initialize(std::shared_ptr<core::internal::LifeCycleManagerCore> const& life_cycle_manager_core) noexcept;

 private:
  /*!
   * \brief Type of the generic proxy backend factory.
   */
  using GenericProxyXfFactory = transformation_layer::internal::ProxyXfFactory<GenericProxyXf>;

  /*!
   * \brief Type of the generic proxy service discovery backend.
   */
  using GenericServiceDiscoveryProxyXf = transformation_layer::internal::ServiceDiscoveryProxyXf<GenericProxyXf>;

  /*!
   * \brief Hide InitializeStateMachine::Initialize() which we do not want to use, since we provide our own
   *        Initialize() method with different signature.
   */
  void Initialize() noexcept;

  /*!
   * \brief Hide InitializeStateMachine::OnInitialize() which we do not want to use, since we provide our own
   *        Initialize() method with different signature.
   */
  [[noreturn]] void OnInitialize() noexcept final;

  /*!
   * \brief         Deinitialize all configured generic required service interfaces.
   * \context       Shutdown
   * \pre           LifeCycleManagerGeneric is initialized.
   * \threadsafe    FALSE
   * \reentrant     FALSE
   * \synchronous   TRUE
   * \vprivate      Component private
   */
  void OnDeinitialize() noexcept final;

  /*!
   * \brief The Logger.
   */
  core::internal::logging::ZeroCopyLogger& logger_;

  /*!
   * \brief The model configuration of the ZeroCopyBinding.
   */
  common::internal::config::ZeroCopyBindingConfig const& zero_copy_binding_config_;

  /*!
   * \brief The container of state listeners to Socal service discovery.
   */
  common::internal::FactoryInterface::SDProxyXfInterfacesContainerType sd_proxy_xf_ptrs_{};
};

}  // namespace internal
}  // namespace generic
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_GENERIC_INCLUDE_AMSR_ZERO_COPY_BINDING_GENERIC_INTERNAL_LIFE_CYCLE_MANAGER_GENERIC_H_
