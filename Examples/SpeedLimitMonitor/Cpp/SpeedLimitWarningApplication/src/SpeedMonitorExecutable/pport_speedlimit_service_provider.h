/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *
 *                Copyright (c) 2022 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *
 *               \file   pport_speedlimit_service_provider.h
 *              \brief    -
 *
 *            \details    -
 *
 *********************************************************************************************************************/
#ifndef SRC_SPEEDLIMIT_PPORT_SPEEDLIMIT_SERVICE_PROVIDER_H
#define SRC_SPEEDLIMIT_PPORT_SPEEDLIMIT_SERVICE_PROVIDER_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <ara/log/logging.h>

#include "services/speedlimitservice/speedlimitservice_skeleton.h"

namespace oneapplication {
namespace speedlimitmonitor {

class PPortSpeedLimitServiceProvider : public services::speedlimitservice::skeleton::SpeedLimitServiceSkeleton {
 public:

  using TyEventData_SpeedOverLimitEvent = datatypes::OverLimitEnum;

  /*!
    * \brief  Constructor of the class speedlimit that provides the speedlimitService
    * \param  speedlimit_server_port identifies PortPrototype
    */
  explicit PPortSpeedLimitServiceProvider(ara::core::InstanceSpecifier instance_specifier);
  ~PPortSpeedLimitServiceProvider() override;


  void SendLimitNotOver() noexcept;
  void SendLimitOverLow() noexcept;
  void SendLimitOverHigh() noexcept;

  void InitService() noexcept;

  void StartService() noexcept;

  void StopService() noexcept;

 private:
  /*!
    * \brief  Holds the logging context for the service provider
    */
  static ara::log::Logger& GetLogger();

};

}  // namespace speedlimitmonitor
}  // namespace oneapplication

#endif // SRC_SPEEDLIMIT_PPORT_SPEEDLIMIT_SERVICE_PROVIDER_H
