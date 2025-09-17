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
 *  \brief Listen service message to register interest in a service.
 *  \unit IpcServiceDiscovery::Centralized::Message
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_MESSAGE_LISTEN_SERVICE_MESSAGE_H_
#define LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_MESSAGE_LISTEN_SERVICE_MESSAGE_H_

#include "amsr/ipc_service_discovery/centralized/internal/message/base/message.h"
#include "amsr/ipc_service_discovery/centralized/internal/types/types.h"
#include "amsr/ipc_service_discovery/common/internal/service_instance_identifier/required_service_instance_identifier.h"
#include "ara/log/logstream.h"

namespace amsr {
namespace ipc_service_discovery {
namespace centralized {
namespace internal {

/*!
 * \brief Listen service message to register interest in a service.
 * \vprivate Component internal.
 */
class ListenServiceMessage : public Message<ListenServiceMessage> {
 public:
  /*! Unique message id. */
  static constexpr Id kId{Message::Id::kListenService};
  /*! Service instance identifier of requested service. */
  // VECTOR NC VectorC++-V11.0.2: MD_IPCSERVICEDISCOVERY_V11.0.2_inAClassAllMutableDataMembersShallBePrivate
  RequiredServiceInstanceIdentifier service_instance_identifier{0, 0, 0, 0};

  /*!
   * \brief Create with default values.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr ListenServiceMessage() noexcept : ListenServiceMessage{{0, 0, 0, 0}} {}

  /*!
   * \brief Check if ListenServiceMessage is valid.
   * \details Check if MessageId is valid.
   * \return True if ListenServiceMessage is valid, false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr auto IsValid() const noexcept -> bool { return GetId() == kId; }

  /*!
   * \brief Create with explicit values.
   * \param[in] service_instance_identifier_p Service instance identifier of the requested service.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr explicit ListenServiceMessage(RequiredServiceInstanceIdentifier service_instance_identifier_p) noexcept
      : Message{kId}, service_instance_identifier{service_instance_identifier_p} {}

  /*!
   * \brief Compare listen service message against another.
   * \param[in] other Listen service message to compare against.
   * \return True if equal, false if different.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr auto operator==(ListenServiceMessage const& other) const noexcept -> bool {
    return std::tie(service_instance_identifier) == std::tie(other.service_instance_identifier);
  }
};

/*!
 * \brief Log streaming operator for type ListenServiceMessage.
 * \param[in] log_stream A reference to the LogStream.
 * \param[in] listen_service_message A const reference to the ListenServiceMessage.
 * \return The reference to the LogStream.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vprivate Component Private
 */
// clang-format off
inline auto operator<<(::ara::log::LogStream& log_stream,
                       ListenServiceMessage const& listen_service_message) noexcept -> ::ara::log::LogStream& /* COV_IpcSD_utility */ {
  // clang-format on
  log_stream << "ListenServiceMessage: ";
  log_stream << "[";
  log_stream << listen_service_message.service_instance_identifier;
  return log_stream << "]";
}

}  // namespace internal
}  // namespace centralized
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_MESSAGE_LISTEN_SERVICE_MESSAGE_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_IpcSD_utility
//   \ACCEPT  XX
//   \REASON  FunctionCoverage gap is reported for the API.
//            Since the API is a trivial logging function and tested by unit tests
//            no component test function coverage needs to be achieved for this function.
// COV_JUSTIFICATION_END
