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
/**        \file  WarningDisplayExecutable/include/services/speedlimitservice/SpeedLimitService_proxy_backend_interface.h
 *        \brief  Proxy backend interface of service 'SpeedLimitService'.
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

#ifndef WARNINGDISPLAYEXECUTABLE_INCLUDE_SERVICES_SPEEDLIMITSERVICE_SPEEDLIMITSERVICE_PROXY_BACKEND_INTERFACE_H_
#define WARNINGDISPLAYEXECUTABLE_INCLUDE_SERVICES_SPEEDLIMITSERVICE_SPEEDLIMITSERVICE_PROXY_BACKEND_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/socal/internal/events/proxy_event_backend_interface.h"
#include "amsr/socal/internal/types.h"
#include "amsr/socal/skeleton_connection_state.h"
#include "ara/core/future.h"
#include "services/speedlimitservice/SpeedLimitService_types.h"

namespace services {
namespace speedlimitservice {
namespace internal {

/*!
 * \brief  Proxy backend interface for the Service 'SpeedLimitService'
 * \remark generated
 */
// VECTOR NC Metric-OO.WMC.One: MD_SOCAL_Metric.OO.WMC.One_GeneratedClass
class SpeedLimitServiceProxyBackendInterface {
 public:
  /*!
   * \brief Define default constructor.
   * \pre         -
   * \context     App
   * \synchronous TRUE
   * \steady FALSE
   */
  SpeedLimitServiceProxyBackendInterface() noexcept = default;

  /*!
   * \brief Define default destructor.
   * \pre         -
   * \context     App
   * \synchronous TRUE
   * \steady FALSE
   */
  virtual ~SpeedLimitServiceProxyBackendInterface() noexcept = default;

  SpeedLimitServiceProxyBackendInterface(SpeedLimitServiceProxyBackendInterface const&) = delete;

  SpeedLimitServiceProxyBackendInterface(SpeedLimitServiceProxyBackendInterface&&) = delete;

  SpeedLimitServiceProxyBackendInterface& operator=(SpeedLimitServiceProxyBackendInterface const&) & = delete;

  SpeedLimitServiceProxyBackendInterface& operator=(SpeedLimitServiceProxyBackendInterface&&) & = delete;

  // ---- Methods --------------------------------------------------------------------------------------------------

  // ---- Events ---------------------------------------------------------------------------------------------------
  /*!
   * \brief   Get the event manager object for the service event 'SpeedOverLimitEvent'
   * \details To ensure thread-safety, for each event instance a distinct proxy event backend instance shall be
   *          returned.
   * \return A proxy event object supporting event sample and subscription.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for different backend instances, FALSE otherwise.
   * \reentrant   TRUE for different backend instances, FALSE otherwise.
   * \synchronous TRUE
   * \steady FALSE
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_PureVirtualFunctionCanBeConst_FalsePositive
  virtual ::amsr::socal::internal::events::ProxyEventBackendInterface<::datatypes::OverLimitEnum>& GetEventBackendSpeedOverLimitEvent() noexcept = 0;

  // ---- Fields ---------------------------------------------------------------------------------------------------

  /*!
   * \brief Get the connection state between proxy and skeleton.
   * \return amsr::socal::SkeletonConnectionState.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for different backend instances, FALSE otherwise.
   * \reentrant   TRUE for different backend instances, FALSE otherwise.
   * \synchronous TRUE
   * \steady TRUE
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_PureVirtualFunctionCanBeConst_FalsePositive
  virtual ::amsr::socal::SkeletonConnectionState ReadSkeletonConnectionState() noexcept = 0;
};

}  // namespace internal
}  // namespace speedlimitservice
}  // namespace services

#endif  // WARNINGDISPLAYEXECUTABLE_INCLUDE_SERVICES_SPEEDLIMITSERVICE_SPEEDLIMITSERVICE_PROXY_BACKEND_INTERFACE_H_

