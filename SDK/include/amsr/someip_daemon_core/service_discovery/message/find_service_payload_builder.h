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
/**        \file  find_service_payload_builder.h
 *        \brief  Payload builder for find service message.
 *
 *      \details  Builds a SD message payload for find service message.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_FIND_SERVICE_PAYLOAD_BUILDER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_FIND_SERVICE_PAYLOAD_BUILDER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_find_service_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/service_discovery_message_payload.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace message {

/*!
 * A Service Discovery message payload builder for FindService entry type.
 */
class FindServicePayloadBuilder final {
 public:
  FindServicePayloadBuilder() = delete;
  ~FindServicePayloadBuilder() = delete;
  FindServicePayloadBuilder(FindServicePayloadBuilder const &) = delete;
  FindServicePayloadBuilder(FindServicePayloadBuilder &&) = delete;
  FindServicePayloadBuilder &operator=(FindServicePayloadBuilder const &) & = delete;
  FindServicePayloadBuilder &operator=(FindServicePayloadBuilder &&) & = delete;

  /*!
   * \brief Creates a SD Message Payload containing an entry and its options from a given FindService Entry.
   * \param[in] entry FindService entry.
   * \return A SD Message Payload containing the given entry.
   * \pre -
   * \context Timer
   * \steady FALSE
   * \reentrant FALSE
   * \trace SPEC-10144621
   * \trace SPEC-10144622
   * \trace SPEC-10144623
   * \trace SPEC-10144624
   * \trace SPEC-10144626
   * \trace SPEC-10144628
   * \trace SPEC-10144630
   */
  static ServiceDiscoveryMessagePayload CreatePayload(entries::FindServiceEntry const &entry);
};

}  // namespace message
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_FIND_SERVICE_PAYLOAD_BUILDER_H_
