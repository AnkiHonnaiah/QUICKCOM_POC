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
 *         \brief   Interface to be extended by ServiceDiscoveryProxyXf.
 *
 *         \details ServiceDiscoveryProxyXf is templated and needs an interface to store it in the list
 *                  which is present in LifeCycleManager.
 *
 *         \unit    ZeroCopyBinding::ZeroCopyBindingTransformationLayer::RequiredServiceInstance
 *
 * \complexity The high number of inter-module calls is caused by the concrete service discovery implementations. The
 *             task of this unit is to (de-)register required services to the Socal service discovery, and (stop)
 *             listening to and forward calls from the Ipc service discovery. Hence, these calls are part of the design
 *             and cannot be avoided.
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_COMMON_INCLUDE_AMSR_ZERO_COPY_BINDING_COMMON_INTERNAL_SERVICE_DISCOVERY_PROXY_XF_INTERFACE_H_
#define LIB_ZERO_COPY_BINDING_COMMON_INCLUDE_AMSR_ZERO_COPY_BINDING_COMMON_INTERNAL_SERVICE_DISCOVERY_PROXY_XF_INTERFACE_H_

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
 * \brief Service discovery proxy xf interface.
 */
class ServiceDiscoveryProxyXfInterface {
 public:
  /*!
   * \brief   Default constructor.
   * \context Init
   */
  ServiceDiscoveryProxyXfInterface() = default;

  /*!
   * \brief   Default destructor.
   * \context Shutdown
   */
  virtual ~ServiceDiscoveryProxyXfInterface() noexcept = 0;

  ServiceDiscoveryProxyXfInterface(ServiceDiscoveryProxyXfInterface const&) = delete;
  ServiceDiscoveryProxyXfInterface(ServiceDiscoveryProxyXfInterface&&) = delete;
  auto operator=(ServiceDiscoveryProxyXfInterface const&) -> ServiceDiscoveryProxyXfInterface& = delete;
  auto operator=(ServiceDiscoveryProxyXfInterface&&) -> ServiceDiscoveryProxyXfInterface& = delete;

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
  virtual auto AsInitializeStateMachine() noexcept -> core::internal::InitializeStateMachine* = 0;
};

inline ServiceDiscoveryProxyXfInterface::~ServiceDiscoveryProxyXfInterface() noexcept = default;

}  // namespace internal
}  // namespace common
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_COMMON_INCLUDE_AMSR_ZERO_COPY_BINDING_COMMON_INTERNAL_SERVICE_DISCOVERY_PROXY_XF_INTERFACE_H_
