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
 *               \file   speedlimit_controller.cpp
 *              \brief    -
 *
 *            \details    -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "speedlimit_controller.h"

#include "vac/container/string_literals.h"
#include "ara/core/instance_specifier.h"

namespace oneapplication {
namespace speedlimitmonitor {

using namespace vac::container::literals;

void SpeedLimitController::DoPreInitialize() {
  ExecutableControllerBase::DoPreInitialize();

  // Logging is allowed from here. Logging is initialized when platform initialization is called in the base class.

  uptr_log_support_->GetInitLogger().LogInfo() << "SpeedLimitController::DoPreInitialize(): Executable SpeedLimit startup is beginning." <<
                                               "Pre initialization (Microsar Adaptive initialization) has successfully been done. Next step is Initialize.";

  // Add other prioritized activities here?

}

void SpeedLimitController::DoInitialize() {
  ExecutableControllerBase::DoInitialize();

  // Create service with corresponding instance specifier
  ara::core::InstanceSpecifier const pport_instance_specifier{
      "SpeedMonitorExecutable/RootSwcPrototype/SpeedLimitMonitor_Provide"_sv};

  uptr_speedlimit_service_provider_ = std::make_unique<PPortSpeedLimitServiceProvider>(pport_instance_specifier);
  uptr_speedlimit_service_provider_->InitService();

  uptr_log_support_->GetInitLogger().LogInfo() << "SpeedLimitController::DoInitialize(): Initialize of executable has been performed.";
}

void SpeedLimitController::DoStart() {
  ExecutableControllerBase::DoStart();

  uptr_speedlimit_service_provider_->StartService();

  uptr_log_support_->GetInitLogger().LogInfo() << "SpeedLimitController::DoStart(): Startup of executable has been performed. "
                                               << "Includes the startup of the service provider and service offering";
}

void SpeedLimitController::DoExecute() {
    ExecutableControllerBase::DoExecute();

    // Add the executable logic here
    // send values for the event
    while (!shutdown_requested_) {

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    uptr_speedlimit_service_provider_->SendLimitNotOver();

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    uptr_speedlimit_service_provider_->SendLimitOverLow();

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    uptr_speedlimit_service_provider_->SendLimitOverHigh();

    }
}

void SpeedLimitController::DoShutdown() {
  uptr_log_support_->GetShutdownLogger().LogInfo() << "SpeedLimitController::DoShutdown(): Shutting down the SpeedLimit executable logic.";

  uptr_speedlimit_service_provider_->StopService();
  // This will destroy the service provider
  uptr_speedlimit_service_provider_.reset(nullptr);

  ExecutableControllerBase::DoShutdown();
}

}  // namespace speedlimitmonitor
}  // namespace oneapplication
