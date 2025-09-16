/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2025 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  WarningDisplayExecutable/src/services/speedlimitservice/speedlimitservice_proxy.cpp
 *        \brief  Proxy for service 'SpeedLimitService'.
 *
 *      \details
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  GENERATOR INFORMATION
 *  -------------------------------------------------------------------------------------------------------------------
 *    Generator Name: amsr_socal
 *         Commit ID: ad694a8ff64fb52ed9a0b9551cf05c10d499927a
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "services/speedlimitservice/speedlimitservice_proxy.h"

/*!
 * \trace SPEC-4980240, SPEC-4980242
 */
namespace services {
namespace speedlimitservice {
namespace proxy {

/*!
 * \trace SPEC-8053550
 */
// ============ Proxy preconstructor (returns a token or an error) ============
SpeedLimitServiceProxy::ConstructionResult SpeedLimitServiceProxy::Preconstruct(
    SpeedLimitServiceProxy::HandleType const& handle) noexcept {
  // VECTOR NL VectorC++-V5.0.1, AutosarC++17_10-A5.0.1: MD_SOCAL_VectorC++-V5.0.1_unsequenced_call_false_positive
  return Base::Preconstruct(handle.GetRequiredInstanceId(), handle.GetProvidedInstanceId());
}

/*!
 * \trace SPEC-8053550
 */
// ====================== Proxy constructor (token based) =====================
// VCA Disable [SLC-10, SLC-22] : VCA_SOCAL_GENERATED_PROXY_GET_BACKEND_REF
SpeedLimitServiceProxy::SpeedLimitServiceProxy(ConstructionToken&& token) noexcept
  : Base{std::move(token)} ,
    // VECTOR NC VectorC++-V5.0.1, AutosarC++17_10-A5.0.1: MD_SOCAL_VectorC++-V5.0.1_unsequenced_call_false_positive
    // VCA_SOCAL_CALLING_STL_APIS
    SpeedOverLimitEvent{&(Base::GetProxyBackend().GetEventBackendSpeedOverLimitEvent()), "SpeedLimitService", "SpeedOverLimitEvent", Base::GetProvidedInstanceId().GetAraComInstanceIdentifier()}    {}
// VCA Enable : VCA_SOCAL_GENERATED_PROXY_GET_BACKEND_REF

// ====================== Proxy constructor ======================
SpeedLimitServiceProxy::SpeedLimitServiceProxy(
  SpeedLimitServiceProxy::HandleType const& handle) noexcept
  : SpeedLimitServiceProxy{Preconstruct(handle).Value()} {}

// ============================= Proxy destructor =============================
SpeedLimitServiceProxy::~SpeedLimitServiceProxy() noexcept {  // VCA_SOCAL_DESTRUCTOR_STATIC_MEMORY
  // Start cleanup of proxy by unsubscribing all event and field notifications
  // Events
  SpeedOverLimitEvent.Unsubscribe();
  SpeedOverLimitEvent.UnsetReceiveHandler();
  SpeedOverLimitEvent.UnsetSubscriptionStateHandler();
  SpeedOverLimitEvent.RemovePendingTasks();

  // Fields
}

}  // namespace proxy
}  // namespace speedlimitservice
}  // namespace services

