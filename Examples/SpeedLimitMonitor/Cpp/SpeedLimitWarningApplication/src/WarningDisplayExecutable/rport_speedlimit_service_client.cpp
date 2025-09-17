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
 *               \file    rport_speedlimit_service_client.cpp
 *              \brief    -
 *
 *            \details    -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "rport_speedlimit_service_client.h"

#include "warningdisplay_processor.h"

namespace oneapplication {
namespace warningdisplay {

using namespace vac::container::literals;

ara::log::Logger& RPortSpeedLimitServiceClient::GetLogger() {
  static ara::log::Logger& logger{ara::log::CreateLogger(ara::core::StringView("RSWI"),
                                                         ara::core::StringView("Context for warningdisplay service client"))};
  return logger;
}

void RPortSpeedLimitServiceClient::InitClient() noexcept {

  // Currently, nothing to do here

}

void RPortSpeedLimitServiceClient::StartClient(ara::core::InstanceSpecifier speedlimit_instance_spec) noexcept {

  opt_find_service_handle_ = TyServiceProxy::StartFindService(
      [this](ara::com::ServiceHandleContainer<TyServiceProxy::HandleType> service_handle_container) {
        ProcessFindServiceNotification(std::move(service_handle_container));
      }, speedlimit_instance_spec);
}

void RPortSpeedLimitServiceClient::ShutdownClient() noexcept {
  if (IsClientConnected()) {
    uptr_consumer_proxy_->SpeedOverLimitEvent.Unsubscribe();
    uptr_consumer_proxy_->SpeedOverLimitEvent.UnsetReceiveHandler();
    uptr_consumer_proxy_->SpeedOverLimitEvent.UnsetSubscriptionStateHandler();
    uptr_consumer_proxy_->StopFindService(opt_find_service_handle_.value());
  }
  if (opt_find_service_handle_.has_value()) {
    opt_find_service_handle_.value().Deactivate();
  }
  service_provider_found_ = false;
  uptr_consumer_proxy_.reset();
}

bool RPortSpeedLimitServiceClient::IsClientConnected() noexcept {
  return service_provider_found_;
}

void RPortSpeedLimitServiceClient::ProcessFindServiceNotification (
    ara::com::ServiceHandleContainer<TyServiceProxy ::HandleType> found_services) noexcept
{
  if (found_services.empty())
  {
    GetLogger().LogInfo() << "RPortSpeedLimitServiceClient::ProcessFindServiceNotification: No SpeedLimit Service instance found when the notification callback was called. "
                          << "Either the service is not offered yet or the service was closed down?";
    return;
  }
  else if (found_services.size() == 1)
  {
    uptr_consumer_proxy_ = std::make_unique<TyServiceProxy>(found_services[0]);
    service_provider_found_ = true;
    GetLogger().LogInfo() << "RPortSpeedLimitServiceClient::ProcessFindServiceNotification: Found SpeedLimit service instance. Client proxy is now available for access.";

    uptr_consumer_proxy_->SpeedOverLimitEvent.SetReceiveHandler([this]() { EventReceiveHndl_SpeedOverLimitEvent();});
    uptr_consumer_proxy_->SpeedOverLimitEvent.SetSubscriptionStateHandler(
        [this](ara::com::SubscriptionState state) { EventSubscriptionChgHndl_SpeedOverLimitEvent(state);});

    uptr_consumer_proxy_->SpeedOverLimitEvent.Subscribe(10);
    GetLogger().LogInfo() << "RPortSpeedLimitServiceClient::ProcessFindServiceNotification: Subscription has been requested and related callbacks registered.";
  }
  else
  {
    // The example doesn't handle this case
    GetLogger().LogError() << "RPortSpeedLimitServiceClient::ProcessFindServiceNotification: "
                           << "Found multiple service instances. This case is not handled by this example";
  }
}

void RPortSpeedLimitServiceClient::EventSubscriptionChgHndl_SpeedOverLimitEvent(ara::com::SubscriptionState state) noexcept {

  GetLogger().LogInfo() << "RPortSpeedLimitServiceClient::EventSubscriptionChgHndl_SpeedOverLimitEvent: Subscription state changed: "
                        << static_cast<std::uint8_t>(state);
}

void RPortSpeedLimitServiceClient::EventReceiveHndl_SpeedOverLimitEvent() noexcept {

  datatypes::OverLimitEnum new_state = datatypes::OverLimitEnum::NOT_OVER;

  // In case there are more than one sample in the event queue, the latest will decide the state used.
  // The new state is stored in the local variable new_state which is used after the samples has been
  // handled.
  uptr_consumer_proxy_->SpeedOverLimitEvent.GetNewSamples(
      [this, &new_state] (ara::com::SamplePtr<const services::speedlimitservice::proxy::events::SpeedOverLimitEvent::SampleType> ptr_sample) {

        ara::com::SamplePtr<const services::speedlimitservice::proxy::events::SpeedOverLimitEvent::SampleType> ptr_local_sample {std::move(ptr_sample)};

        new_state = *ptr_local_sample;
     });

  GetLogger().LogInfo() << "RPortSpeedLimitServiceClient::EventReceiveHndl_SpeedOverLimitEvent: SpeedLimit state changed to: " << static_cast<std::uint8_t>(new_state);
  WarningDisplayProcessing::GetInst().ChangeSpeedLimitState(new_state);
}

}  // namespace warningdisplay
}  // namespace oneapplication

