/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*! \file
 *  \brief Utilities for message usage.
 *  \unit IpcServiceDiscovery::Centralized::Message
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_MESSAGE_MESSAGES_H_
#define LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_MESSAGE_MESSAGES_H_

#include <algorithm>
#include "amsr/ipc_service_discovery/centralized/internal/message/init_message.h"
#include "amsr/ipc_service_discovery/centralized/internal/message/listen_service_message.h"
#include "amsr/ipc_service_discovery/centralized/internal/message/offer_service_message.h"
#include "amsr/ipc_service_discovery/centralized/internal/message/stop_listen_service_message.h"
#include "amsr/ipc_service_discovery/centralized/internal/message/stop_offer_service_message.h"

namespace amsr {
namespace ipc_service_discovery {
namespace centralized {
namespace internal {

/*!
 * \brief Provides utilities for usage of messages.
 * \vprivate Component internal.
 */
class Messages {
 public:
  static constexpr size_t kNumberOfMessages{5};

  /*!
   * \brief Get the maximum size of all messages in bytes.
   * \return The maximum message size.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   *
   * \internal
   * - Calculate and return the maximum message size.
   * \endinternal
   */
  static constexpr auto GetMaxSize() noexcept -> size_t {
    return std::max({InitMessage::GetSize(), OfferServiceMessage::GetSize(), StopOfferServiceMessage::GetSize(),
                     ListenServiceMessage::GetSize(), StopListenServiceMessage::GetSize()});
  }
};

}  // namespace internal
}  // namespace centralized
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_MESSAGE_MESSAGES_H_
