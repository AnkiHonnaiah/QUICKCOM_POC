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
 *               \file    warningdisplay_controller.cpp
 *              \brief    -
 *
 *            \details    -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "warningdisplay_controller.h"

#include "vac/container/string_literals.h"
#include "ara/core/instance_specifier.h"

namespace oneapplication {
namespace warningdisplay {

using namespace vac::container::literals;

void WarningDisplayController::DoPreInitialize() {
  ExecutableControllerBase::DoPreInitialize();

  // Logging is allowed from here. Logging is initialized when platform initialization is called in the base class.

  uptr_log_support_->GetInitLogger().LogInfo() << "WarningDisplayController::DoPreInitialize(): Executable WarningDisplay startup is beginning." <<
                                               "Pre initialization (Microsar Adaptive initialization) has successfully been done. Next step is Initialize.";

  // Add other prioritized activities here?

}

void WarningDisplayController::DoInitialize() {
  ExecutableControllerBase::DoInitialize();

  uptr_speedlimit_service_client_ = std::make_unique<RPortSpeedLimitServiceClient>();
  uptr_speedlimit_service_client_->InitClient();

  uptr_log_support_->GetInitLogger().LogInfo() << "WarningDisplayController::DoInitialize(): Initialize of executable has been performed. "
                                               << "Initialization include the creation of the service clients";
}

void WarningDisplayController::DoStart() {
  ExecutableControllerBase::DoStart();

  ara::core::InstanceSpecifier const speedlimit_instance_spec{
      "WarningDisplayExecutable/RootSwcPrototype/SpeedLimitMonitor_Require"_sv};

  uptr_speedlimit_service_client_->StartClient(speedlimit_instance_spec);

  uptr_log_support_->GetInitLogger().LogInfo() << "WarningDisplayController::DoStart(): Startup of executable has been performed. "
                                               << "Includes the startup of the service clients and activating the FindService functionality.";
}

void WarningDisplayController::DoExecute() {
  ExecutableControllerBase::DoExecute();

  //Add some execution logic here

  uptr_log_support_->GetControllerLogger().LogInfo() << "WarningDisplayController::DoExecute: Execution logic is finished. Looping and waiting for shutdown via signals.";

  WaitForShutdown();
}

void WarningDisplayController::DoShutdown() {
  uptr_log_support_->GetShutdownLogger().LogInfo() << "WarningDisplayController::DoShutdown(): Shutting down the WarningDisplay executable logic.";

  uptr_speedlimit_service_client_->ShutdownClient();

  // This will destroy the service provider
  uptr_speedlimit_service_client_.reset(nullptr);

  ExecutableControllerBase::DoShutdown();
}

}  // namespace warningdisplay
}  // namespace oneapplication
