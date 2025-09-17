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
 *         \brief   Interface to be extended by ServiceDiscoverySkeletonXf.
 *
 *         \details ServiceDiscoverySkeletonXf is templated and needs an interface to store it in the list
 *                  which is present in LifeCycleManager.
 *         \unit  ZeroCopyBinding::ZeroCopyBindingTransformationLayer::ProvidedServiceInstance
 *
 *   \complexity  The inter-module call count is increased for this unit due to additional calls to mock instances that
 *                are not considered to be part of the calling module and that are only used for unit testing.
 *                Actual inter-module calls to osabstraction, socal and ipcservicediscovery are within the allowed
 *                range.
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_COMMON_INCLUDE_AMSR_ZERO_COPY_BINDING_COMMON_INTERNAL_SERVICE_DISCOVERY_SKELETON_XF_INTERFACE_H_
#define LIB_ZERO_COPY_BINDING_COMMON_INCLUDE_AMSR_ZERO_COPY_BINDING_COMMON_INTERNAL_SERVICE_DISCOVERY_SKELETON_XF_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace amsr {
namespace zero_copy_binding {

namespace core {
namespace internal {

/*!
 * \brief Declaration of InitializeStateMachine.
 */
class InitializeStateMachine;

}  // namespace internal
}  // namespace core

namespace common {
namespace internal {

/*!
 * \brief Service discovery skeleton xf interface.
 */
class ServiceDiscoverySkeletonXfInterface {
 public:
  /*!
   * \brief   Default constructor.
   * \context Init
   */
  ServiceDiscoverySkeletonXfInterface() noexcept = default;

  /*!
   * \brief   Default destructor.
   * \context Shutdown
   */
  // VCA_ZEROCOPYBINDING_SLC_23_MEMORY_DEALLOCATION_IN_DTOR
  virtual ~ServiceDiscoverySkeletonXfInterface() noexcept = 0;

  ServiceDiscoverySkeletonXfInterface(ServiceDiscoverySkeletonXfInterface const&) = delete;
  auto operator=(ServiceDiscoverySkeletonXfInterface const&) -> ServiceDiscoverySkeletonXfInterface& = delete;
  ServiceDiscoverySkeletonXfInterface(ServiceDiscoverySkeletonXfInterface&&) = delete;
  auto operator=(ServiceDiscoverySkeletonXfInterface&&) -> ServiceDiscoverySkeletonXfInterface& = delete;

  /*!
   * \brief       A helper method that will be used while deinitializing the LifeCycleManagerXf.
   * \return      A pointer to the InitializeStateMachine part of an instance of a derived class.
   * \context     Shutdown
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_ZEROCOPYBINDING_M9.3.3_unusedInterfaceMethodCanBeConst
  virtual auto AsInitializeStateMachine() noexcept -> core::internal::InitializeStateMachine* = 0;
};

inline ServiceDiscoverySkeletonXfInterface::~ServiceDiscoverySkeletonXfInterface() noexcept = default;

}  // namespace internal
}  // namespace common
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_COMMON_INCLUDE_AMSR_ZERO_COPY_BINDING_COMMON_INTERNAL_SERVICE_DISCOVERY_SKELETON_XF_INTERFACE_H_
