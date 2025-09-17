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
 *               \file   speedlimit_controller.h
 *              \brief    -
 *
 *            \details    -
 *
 *********************************************************************************************************************/
#ifndef SRC_SPEEDLIMIT_SPEEDLIMIT_CONTROLLER_H
#define SRC_SPEEDLIMIT_SPEEDLIMIT_CONTROLLER_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "executable_controller_base.h"
#include "pport_speedlimit_service_provider.h"

namespace oneapplication {
namespace speedlimitmonitor {

class SpeedLimitController final : public common::ExecutableControllerBase{

 public:

  ~SpeedLimitController() override = default;

 protected:
  void DoPreInitialize() override;
  void DoInitialize() override;
  void DoStart() override;
  void DoExecute() override;
  void DoShutdown() override;

 private:
  std::unique_ptr<PPortSpeedLimitServiceProvider> uptr_speedlimit_service_provider_;
};

}  // namespace speedlimitmonitor
}  // namespace oneapplication

#endif // SRC_SPEEDLIMIT_SPEEDLIMIT_CONTROLLER_H
