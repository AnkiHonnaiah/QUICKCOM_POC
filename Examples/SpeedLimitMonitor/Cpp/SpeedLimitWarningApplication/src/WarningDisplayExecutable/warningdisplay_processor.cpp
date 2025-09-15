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
 *               \file    warningdisplay_processor.cpp
 *              \brief    -
 *
 *            \details    -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "warningdisplay_processor.h"

namespace oneapplication {
namespace warningdisplay {

WarningDisplayProcessing& WarningDisplayProcessing::GetInst() {
  static WarningDisplayProcessing singleton_instance{};

  return singleton_instance;
}

ara::log::Logger& WarningDisplayProcessing::GetLogger() {
  static ara::log::Logger& logger{ara::log::CreateLogger(ara::core::StringView("PROC"),
                                                         ara::core::StringView("Context for SpeedLimit service client"))};
  return logger;
}


void WarningDisplayProcessing::ChangeSpeedLimitState(datatypes::OverLimitEnum limit_state) {
  internal_limit_state_= limit_state;
  WarningMessageControl();
}

void WarningDisplayProcessing::WarningMessageControl() noexcept
{
  if (internal_limit_state_ == datatypes::OverLimitEnum::NOT_OVER)
  {
    //no warning displayed
    GetLogger().LogInfo() << "NO WARNING :) ";
  }
  else if (internal_limit_state_ == datatypes::OverLimitEnum::OVER_LOW)
  {
    //display a yellow warning
    GetLogger().LogInfo() << "YELLOW WARNING!";
  }
   else if (internal_limit_state_ == datatypes::OverLimitEnum::OVER_HIGH)
  {
    //display a red warning
    GetLogger().LogInfo() << "RED WARNING!";
  }
}

}  // namespace warningdisplay
}  // namespace oneapplication
