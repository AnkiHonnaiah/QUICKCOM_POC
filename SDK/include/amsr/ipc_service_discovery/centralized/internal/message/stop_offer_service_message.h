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
 *  \brief Stop offer service message to stop offer of a service instance.
 *  \unit IpcServiceDiscovery::Centralized::Message
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_MESSAGE_STOP_OFFER_SERVICE_MESSAGE_H_
#define LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_MESSAGE_STOP_OFFER_SERVICE_MESSAGE_H_

#include "amsr/ipc/unicast_address.h"
#include "amsr/ipc_service_discovery/centralized/internal/message/base/message.h"
#include "amsr/ipc_service_discovery/centralized/internal/types/types.h"
#include "amsr/ipc_service_discovery/common/internal/service_instance_identifier/provided_service_instance_identifier.h"
#include "ara/log/logstream.h"

namespace amsr {
namespace ipc_service_discovery {
namespace centralized {
namespace internal {

/*!
 * \brief Stop offer service message to stop offer of a service instance.
 * \vprivate Component internal.
 */
class StopOfferServiceMessage : public Message<StopOfferServiceMessage> {
 public:
  /*! Unique message id. */
  static constexpr Id kId{Message::Id::kStopOfferService};
  /*! Service instance identifier of service offer to stop. */
  // VECTOR NC VectorC++-V11.0.2: MD_IPCSERVICEDISCOVERY_V11.0.2_inAClassAllMutableDataMembersShallBePrivate
  ProvidedServiceInstanceIdentifier service_instance_identifier{0, 0, 0, 0};
  /*! Unicast address of service offer to stop. */
  // VECTOR NC VectorC++-V11.0.2: MD_IPCSERVICEDISCOVERY_V11.0.2_inAClassAllMutableDataMembersShallBePrivate
  UnicastAddress unicast_address{0, 0};

  /*!
   * \brief Create with default values.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr StopOfferServiceMessage() noexcept : StopOfferServiceMessage{{0, 0, 0, 0}, {0, 0}} {}

  /*!
   * \brief Create with explicit values.
   * \param[in] service_instance_identifier_p Service instance identifier of service offer to stop.
   * \param[in] unicast_address Unicast address of service offer to stop.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr explicit StopOfferServiceMessage(ProvidedServiceInstanceIdentifier service_instance_identifier_p,
                                             UnicastAddress unicast_address_p) noexcept
      : Message{kId}, service_instance_identifier{service_instance_identifier_p}, unicast_address{unicast_address_p} {}

  /*!
   * \brief Check if StopOfferServiceMessage is valid.
   * \details Check if MessageId and ProvidedServiceInstanceIdentifier is valid.
   * \return True if StopOfferServiceMessage is valid, false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr auto IsValid() const noexcept -> bool { return (GetId() == kId) && service_instance_identifier.IsValid(); }

  /*!
   * \brief Compare stop offer service message against another.
   * \param[in] other Stop offer service message to compare against.
   * \return True if equal, false if different.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr auto operator==(StopOfferServiceMessage const& other) const noexcept -> bool {
    return std::tie(service_instance_identifier, unicast_address) ==
           std::tie(other.service_instance_identifier, other.unicast_address);
  }
};

/*!
 * \brief Log streaming operator for type StopOfferServiceMessage.
 * \param[in] log_stream A reference to the LogStream.
 * \param[in] stop_offer_service_message A const reference to the StopOfferServiceMessage.
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
                       StopOfferServiceMessage const& stop_offer_service_message) noexcept -> ::ara::log::LogStream& /* COV_IpcSD_utility */ {
  // clang-format on
  log_stream << "StopOfferServiceMessage: ";
  log_stream << "[";
  log_stream << stop_offer_service_message.service_instance_identifier << ", ";
  log_stream << stop_offer_service_message.unicast_address;
  return log_stream << "]";
}

}  // namespace internal
}  // namespace centralized
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_MESSAGE_STOP_OFFER_SERVICE_MESSAGE_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_IpcSD_utility
//   \ACCEPT  XX
//   \REASON  FunctionCoverage gap is reported for the API.
//            Since the API is a trivial logging function and tested by unit tests
//            no component test function coverage needs to be achieved for this function.
// COV_JUSTIFICATION_END
