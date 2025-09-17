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
/**        \file  SpeedMonitorExecutable/include/amsr/socal/internal/lifecycle_manager.h
 *        \brief  Manages the lifecycle of internal objects with static storage duration.
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

#ifndef SPEEDMONITOREXECUTABLE_INCLUDE_AMSR_SOCAL_INTERNAL_LIFECYCLE_MANAGER_H_
#define SPEEDMONITOREXECUTABLE_INCLUDE_AMSR_SOCAL_INTERNAL_LIFECYCLE_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/socal/internal/thread_pools_manager.h"
#include "services/speedlimitservice/speedlimitservice_skeleton.h"

namespace amsr {
namespace socal {
namespace internal {

/*!
 * \brief Triggers the creation and destruction of objects with static storage duration.
 *
 * \unit Socal::Runtime::LifecycleManager
 */
class LifecycleManager final {
 public:
  /*!
   * \brief       Triggers the creation of singletons.
   * \pre         This function must only be called once.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   *
   * \internal
   * - Create singleton instances of all the skeleton classes.
   * - Create singleton instances of all the proxy classes.
   * - Create singleton instance of the thread pool manager.
   * \endinternal
   */
  static void Initialize() noexcept {
    // Consider the dependencies when changing the initialization order below.
    services::speedlimitservice::skeleton::SpeedLimitServiceSkeleton::CreateSingletons();


    ::amsr::socal::internal::ThreadPoolsManager::getSingletonInstance().Create();
  }

  /*!
   * \brief       Triggers the destruction of singletons.
   * \pre         Initialize has been called.
   *              Reactor thread and bindings have been deinitialized.
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   *
   * \internal
   * - Destroy singleton instance of the thread pool manager.
   * - Destroys singleton instances of all the proxy classes.
   * - Destroys singleton instances of all the skeleton classes.
   * \endinternal
   */
  static void Deinitialize() noexcept {
    // Consider the dependencies when changing the deinitialization order below.
    ::amsr::socal::internal::ThreadPoolsManager::getSingletonInstance().Destroy();


    services::speedlimitservice::skeleton::SpeedLimitServiceSkeleton::DestroySingletons();
  }
};

}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // SPEEDMONITOREXECUTABLE_INCLUDE_AMSR_SOCAL_INTERNAL_LIFECYCLE_MANAGER_H_

