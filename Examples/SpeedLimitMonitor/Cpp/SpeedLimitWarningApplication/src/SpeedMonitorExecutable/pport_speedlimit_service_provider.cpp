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
 *               \file   pport_speedlimit_service_provider.cpp
 *              \brief    -
 *
 *            \details    -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "pport_speedlimit_service_provider.h"

namespace oneapplication {
namespace speedlimitmonitor {

ara::log::Logger& PPortSpeedLimitServiceProvider::GetLogger() {
  static ara::log::Logger& logger{ara::log::CreateLogger(ara::core::StringView("PSWI"),
                                                         ara::core::StringView("Context for SpeedLimit service provider"))};
  return logger;
}

PPortSpeedLimitServiceProvider::PPortSpeedLimitServiceProvider(ara::core::InstanceSpecifier instance_specifier)
    : services::speedlimitservice::skeleton::SpeedLimitServiceSkeleton(instance_specifier) {
}

PPortSpeedLimitServiceProvider::~PPortSpeedLimitServiceProvider() = default;


void PPortSpeedLimitServiceProvider::InitService() noexcept {

  // There is no set method configured in the model => No need to register a set handler before offering the service.

}

void PPortSpeedLimitServiceProvider::StartService() noexcept {
  OfferService();

  if (IsServiceOffered()) {
    GetLogger().LogInfo() << "PPortSpeedLimitServiceProvider::StartService(): The SpeedLimit service has been succesfully offered";
  } else {
    GetLogger().LogError() << "PPortSpeedLimitServiceProvider::StartService(): The OfferService call did not succeed.";
  }
}

void PPortSpeedLimitServiceProvider::StopService() noexcept {
  StopOfferService();
}

void PPortSpeedLimitServiceProvider::SendLimitNotOver() noexcept {
  SpeedOverLimitEvent.Send(TyEventData_SpeedOverLimitEvent::NOT_OVER);

  GetLogger().LogInfo() << "PPortSpeedLimitServiceProvider::SendLimitNotOver(): Sending an event with new limit state. New value = NOT_OVER";
}

void PPortSpeedLimitServiceProvider::SendLimitOverLow() noexcept {
  SpeedOverLimitEvent.Send(TyEventData_SpeedOverLimitEvent::OVER_LOW);

  GetLogger().LogInfo() << "PPortSpeedLimitServiceProvider::SendLimitOverLow(): Sending an event with new limit state. New value = OVER_LOW";
}

void PPortSpeedLimitServiceProvider::SendLimitOverHigh() noexcept {
  SpeedOverLimitEvent.Send(TyEventData_SpeedOverLimitEvent::OVER_HIGH);

  GetLogger().LogInfo() << "PPortSpeedLimitServiceProvider::SendLimitOverHigh(): Sending an event with new limit state. New value = OVER_HIGH";
}

}  // namespace speedlimitmonitor
}  // namespace oneapplication
