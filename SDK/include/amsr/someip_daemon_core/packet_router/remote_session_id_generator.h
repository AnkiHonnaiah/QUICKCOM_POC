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
/**        \file  remote_session_id_generator.h
 *        \brief  Remote session id generator
 *
 *      \details  The remote session id generator is responsible for assigning unique session ids to remote requests
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_ROUTER_REMOTE_SESSION_ID_GENERATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_ROUTER_REMOTE_SESSION_ID_GENERATOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/types/someip_service_instance_id.h"
#include "ara/core/map.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace packet_router {

/*!
 * \brief Represents the key for the assignment of a remote session id value.
 */
class RemoteSessionIdKey final {
 public:
  /*!
   * \brief     Constructor of RemoteSessionIdKey.
   * \param[in] service_instance_id   SOME/IP service instance id.
   * \param[in] method_id_val         SOME/IP method id.
   * \pre       -
   * \context   Network
   * \reentrant FALSE
   * \steady    TRUE
   */
  RemoteSessionIdKey(configuration::types::SomeIpServiceInstanceIdCommunication const service_instance_id,
                     someip_protocol::internal::MethodId const method_id)
      : service_instance_id_{service_instance_id}, method_id_{method_id} {}
  /*!
   * \brief       Defines a strict weak ordering relationship for remote session id keys.
   * \param       other A key to compare against.
   * \return      True if this key is less than the other one, false otherwise.
   * \pre         -
   * \context     Network
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  bool operator<(RemoteSessionIdKey const& other) const {
    return std::make_tuple(this->service_instance_id_, this->method_id_) <
           std::make_tuple(other.service_instance_id_, other.method_id_);
  }

 private:
  /*!
   * \brief SOME/IP service instance identifier containing Service id, Instance Id, Major Version.
   */
  configuration::types::SomeIpServiceInstanceIdCommunication service_instance_id_;

  /*!
   * \brief SOME/IP method identifier.
   */
  someip_protocol::internal::MethodId method_id_;
};

/*!
 * \brief Class responsible for assigning unique session ids to remote requests.
 * \trace SPEC-10144394
 */
class RemoteSessionIdGenerator final {
 public:
  /*!
   * \brief     Default constructor.
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \steady    FALSE
   */
  RemoteSessionIdGenerator() = default;
  /*!
   * \brief     Default destructor.
   * \pre       -
   * \context   Shutdown
   * \reentrant FALSE
   * \steady    FALSE
   */
  ~RemoteSessionIdGenerator() = default;

  RemoteSessionIdGenerator(RemoteSessionIdGenerator const&) = delete;
  RemoteSessionIdGenerator(RemoteSessionIdGenerator&&) = delete;
  RemoteSessionIdGenerator& operator=(RemoteSessionIdGenerator const&) & = delete;
  RemoteSessionIdGenerator& operator=(RemoteSessionIdGenerator&&) & = delete;

  /*!
   * \brief       Generates a new and unique remote session id for the given key.
   * \param[in]   service_instance_id SOME/IP service instance id.
   * \param[in]   method_id           SOME/IP method id.
   * \return      The generated SessionId.
   * \pre         -
   * \context     Network
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \trace SPEC-10144522
   * \trace SPEC-10144526
   * \trace SPEC-10144521
   */
  someip_protocol::internal::SessionId GenerateRemoteSessionId(
      configuration::types::SomeIpServiceInstanceIdCommunication const& service_instance_id,
      someip_protocol::internal::MethodId const method_id);

 private:
  /*!
   * \brief Alias for the datatype of the remote session id map.
   */
  using RemoteSessionIdMap = ara::core::Map<RemoteSessionIdKey, someip_protocol::internal::SessionId>;

  /*!
   * \brief   Table of remote session ids.
   * \details Maps a RemoteSessionIdKey with the last used remote session id by a method requests with its parameters
   *          matching the key.
   */
  RemoteSessionIdMap remote_session_id_map_{};
};

}  // namespace packet_router
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_PACKET_ROUTER_REMOTE_SESSION_ID_GENERATOR_H_
