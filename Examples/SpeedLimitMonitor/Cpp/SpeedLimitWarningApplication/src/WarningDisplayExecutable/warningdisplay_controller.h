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
 *               \file    warningdisplay_controller.h
 *              \brief    -
 *
 *            \details    -
 *
 *********************************************************************************************************************/
#ifndef SRC_WARNINGDISPLAY_WARNINGDISPLAY_CONTROLLER_H
#define SRC_WARNINGDISPLAY_WARNINGDISPLAY_CONTROLLER_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "executable_controller_base.h"
#include "rport_speedlimit_service_client.h"


namespace oneapplication {
namespace warningdisplay {

class WarningDisplayController final : public common::ExecutableControllerBase{

 public:

  ~WarningDisplayController() override = default;

 protected:
  void DoPreInitialize() override;
  void DoInitialize() override;
  void DoStart() override;
  void DoExecute() override;
  void DoShutdown() override;

 private:
  std::unique_ptr<RPortSpeedLimitServiceClient> uptr_speedlimit_service_client_;
};


}  // namespace warningdisplay
}  // namespace oneapplication

#endif // SRC_WARNINGDISPLAY_WARNINGDISPLAY_CONTROLLER_H
