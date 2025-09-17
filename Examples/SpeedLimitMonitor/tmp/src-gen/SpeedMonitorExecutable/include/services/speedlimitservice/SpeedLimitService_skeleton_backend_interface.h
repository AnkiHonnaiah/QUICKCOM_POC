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
/**        \file  SpeedMonitorExecutable/include/services/speedlimitservice/SpeedLimitService_skeleton_backend_interface.h
 *        \brief  Skeleton backend interface of service 'SpeedLimitService'.
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

#ifndef SPEEDMONITOREXECUTABLE_INCLUDE_SERVICES_SPEEDLIMITSERVICE_SPEEDLIMITSERVICE_SKELETON_BACKEND_INTERFACE_H_
#define SPEEDMONITOREXECUTABLE_INCLUDE_SERVICES_SPEEDLIMITSERVICE_SPEEDLIMITSERVICE_SKELETON_BACKEND_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/socal/internal/events/skeleton_event_manager_interface.h"
#include "amsr/socal/internal/methods/skeleton_fire_and_forget_method_backend_interface.h"
#include "amsr/socal/internal/methods/skeleton_method_backend_interface.h"
#include "services/speedlimitservice/SpeedLimitService_datatypes.h"
#include "services/speedlimitservice/speedlimitservice_common.h"

namespace services {
namespace speedlimitservice {
namespace internal {

/*!
 * \brief Skeleton backend interface of service 'SpeedLimitService'
 */
// VECTOR NL Metric-OO.WMC.One: MD_SOCAL_Metric.OO.WMC.One_GeneratedClass
class SpeedLimitServiceSkeletonBackendInterface {
 public:
  /*!
   * \brief Creates a skeleton backend instance
   * \pre     -
   * \context App
   * \steady FALSE
   */
  SpeedLimitServiceSkeletonBackendInterface() noexcept = default;

  /*!
   * \brief Use default destructor
   * \steady FALSE
   */
  virtual ~SpeedLimitServiceSkeletonBackendInterface() noexcept = default;

 protected:
  /*!
   * \brief Use default move constructor
   * \pre     -
   * \context App
   * \steady FALSE
   */
  SpeedLimitServiceSkeletonBackendInterface(SpeedLimitServiceSkeletonBackendInterface&&) noexcept = default;

  /*!
   * \brief Use default move assignment
   * \pre     -
   * \context App
   * \steady FALSE
   */
  SpeedLimitServiceSkeletonBackendInterface& operator=(SpeedLimitServiceSkeletonBackendInterface&&) & noexcept = default;

  SpeedLimitServiceSkeletonBackendInterface(SpeedLimitServiceSkeletonBackendInterface const&) = delete;

  SpeedLimitServiceSkeletonBackendInterface& operator=(SpeedLimitServiceSkeletonBackendInterface const&) & = delete;

 public:
  // ---- Events ---------------------------------------------------------------------------------------------------

  /*!
   * \brief   Get the event manager object for the service event 'SpeedOverLimitEvent'.
   * \details Event sample type: ::datatypes::OverLimitEnum.
   *          For each SkeletonEvent a distinct Skeleton Event Manager instance shall be created to ensure thread
   *          safety.
   * \return A binding-specific event management object/interface supporting event transmission (the returned pointer
   *         should not be a null pointer).
   * \pre        -
   * \context    App
   * \threadsafe TRUE for different impl instances, FALSE otherwise.
   * \reentrant  TRUE for different impl instances, FALSE otherwise.
   * \steady TRUE
   */
  virtual ::amsr::socal::internal::events::SkeletonEventManagerInterface<::datatypes::OverLimitEnum>*
  GetEventManagerSpeedOverLimitEvent() noexcept = 0;

  // ---- Fields ---------------------------------------------------------------------------------------------------

  /*!
   * \brief Offer this service.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_PureVirtualFunctionCanBeConst_FalsePositive
  virtual void OfferService() noexcept = 0;

  /*!
   * \brief Stop offering this service.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_PureVirtualFunctionCanBeConst_FalsePositive
  virtual void StopOfferService() noexcept = 0;
};

}  // namespace internal
}  // namespace speedlimitservice
}  // namespace services

#endif  // SPEEDMONITOREXECUTABLE_INCLUDE_SERVICES_SPEEDLIMITSERVICE_SPEEDLIMITSERVICE_SKELETON_BACKEND_INTERFACE_H_

