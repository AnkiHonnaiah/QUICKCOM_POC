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
 *  \brief Init message to register client by server.
 *  \unit IpcServiceDiscovery::Centralized::Message
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_MESSAGE_INIT_MESSAGE_H_
#define LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_MESSAGE_INIT_MESSAGE_H_

#include "amsr/ipc_service_discovery/centralized/internal/message/base/message.h"
#include "amsr/ipc_service_discovery/centralized/internal/types/types.h"
#include "amsr/ipc_service_discovery/common/internal/types/binding_type.h"
#include "amsr/ipc_service_discovery/common/internal/version/version.h"
#include "ara/log/logstream.h"

namespace amsr {
namespace ipc_service_discovery {
namespace centralized {
namespace internal {

/*!
 * \brief Init message to register client by server.
 * \vprivate Component internal.
 */
class InitMessage : public Message<InitMessage> {
 public:
  /*! Unique message id. */
  static constexpr Id kId{Message::Id::kInit};
  /*! Binding type. */
  // VECTOR NC VectorC++-V11.0.2: MD_IPCSERVICEDISCOVERY_V11.0.2_inAClassAllMutableDataMembersShallBePrivate
  BindingType binding_type{};
  /*! Version with major and minor. */
  // VECTOR NC VectorC++-V11.0.2: MD_IPCSERVICEDISCOVERY_V11.0.2_inAClassAllMutableDataMembersShallBePrivate
  Version version{};

  /*!
   * \brief Create with default values.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr InitMessage() noexcept : InitMessage{BindingType{}, Version{}} {}

  /*!
   * \brief Create with explicit values.
   * \param[in] binding_type Binding type of the client.
   * \param[in] version_p Version of the client.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr InitMessage(BindingType binding_type_p, Version version_p) noexcept
      : Message{kId}, binding_type{binding_type_p}, version{version_p} {}

  /*!
   * \brief Check if InitMessage is valid.
   * \details Check if every member is valid.
   * \return True if InitMessage is valid, false otherwise.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  // VECTOR NC AutosarC++17_10-M0.1.2: MD_IPCSERVICEDISCOVERY_M0.1.2_infeasiblePathsFalsePositive
  constexpr auto IsValid() const noexcept -> bool { return (GetId() == kId) && binding_type.IsValid(); }

  /*!
   * \brief Compare init message against another.
   * \param[in] other Init message to compare against.
   * \return True if equal, false if different.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr auto operator==(InitMessage const& other) const noexcept -> bool {
    return (std::tie(binding_type, version) == std::tie(other.binding_type, other.version));
  }
};

/*!
 * \brief Log streaming operator for type InitMessage.
 * \param[in] log_stream A reference to the LogStream.
 * \param[in] init_message A const reference to the InitMessage.
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
                       InitMessage const& init_message) noexcept -> ::ara::log::LogStream& /* COV_IpcSD_utility */ {
  // clang-format on
  log_stream << "InitMessage: ";
  log_stream << "[";
  log_stream << init_message.binding_type << ", ";
  log_stream << init_message.version;
  return log_stream << "]";
}

}  // namespace internal
}  // namespace centralized
}  // namespace ipc_service_discovery
}  // namespace amsr

#endif  // LIB_IPC_SERVICE_DISCOVERY_CENTRALIZED_INCLUDE_AMSR_IPC_SERVICE_DISCOVERY_CENTRALIZED_INTERNAL_MESSAGE_INIT_MESSAGE_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_IpcSD_utility
//   \ACCEPT  XX
//   \REASON  FunctionCoverage gap is reported for the API.
//            Since the API is a trivial logging function and tested by unit tests
//            no component test function coverage needs to be achieved for this function.
// COV_JUSTIFICATION_END
