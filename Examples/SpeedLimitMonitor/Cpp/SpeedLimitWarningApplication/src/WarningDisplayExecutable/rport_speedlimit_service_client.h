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
 *               \file    rport_speedlimit_service_client.h
 *              \brief    -
 *
 *            \details    -
 *
 *********************************************************************************************************************/
#ifndef SRC_WARNINGDISPLAY_RPORT_SPEEDLIMIT_SERVICE_CLIENT_H
#define SRC_WARNINGDISPLAY_RPORT_SPEEDLIMIT_SERVICE_CLIENT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <thread>

#include "ara/log/logging.h"
#include "ara/core/result.h"

#include "services/speedlimitservice/speedlimitservice_proxy.h"

namespace oneapplication {
namespace warningdisplay {

class RPortSpeedLimitServiceClient final {
 public:

  using TyServiceProxy = services::speedlimitservice::proxy::SpeedLimitServiceProxy;

  RPortSpeedLimitServiceClient() = default;

  ~RPortSpeedLimitServiceClient() = default;

  void InitClient() noexcept;

  void StartClient(ara::core::InstanceSpecifier instance_specifier) noexcept;

  void ShutdownClient() noexcept;

  bool IsClientConnected() noexcept;

 private: // methods
  void ProcessFindServiceNotification(
      ara::com::ServiceHandleContainer<TyServiceProxy::HandleType> found_services) noexcept;

  void EventSubscriptionChgHndl_SpeedOverLimitEvent(ara::com::SubscriptionState state) noexcept;

  void EventReceiveHndl_SpeedOverLimitEvent() noexcept;

 private:
  /*!
  * \brief  Holds the logging context for the service provider
  */
  static ara::log::Logger& GetLogger();

  ara::core::Optional<ara::com::FindServiceHandle> opt_find_service_handle_{};

  std::unique_ptr<TyServiceProxy> uptr_consumer_proxy_{nullptr};

  std::atomic<bool> service_provider_found_{false};

};

}  // namespace warningdisplay
}  // namespace oneapplication

#endif // SRC_WARNINGDISPLAY_RPORT_SPEEDLIMIT_SERVICE_CLIENT_H
