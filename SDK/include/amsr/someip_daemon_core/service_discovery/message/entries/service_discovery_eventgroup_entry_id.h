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
/**        \file  service_discovery_eventgroup_entry_id.h
 *        \brief  Data structure to encapsulate ID parameters of Service Discovery event entries
 *
 *      \details  This data structure allows comparison between Service Discovery event entries of different
 *                type (SubscribeEventgroup, SubscribeEventgroupAck, SubscribeEventrgroupNAck, StopSubscribeEventgroup)
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_ENTRIES_SERVICE_DISCOVERY_EVENTGROUP_ENTRY_ID_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_ENTRIES_SERVICE_DISCOVERY_EVENTGROUP_ENTRY_ID_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/vector.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace message {
namespace entries {

/*!
 * \brief Represents a SOME/IP SD Subscribe Eventgroup entry.
 */
struct EventgroupEntryId {
  /*!
   * \brief SOME/IP service identifier
   */
  amsr::someip_protocol::internal::ServiceId service_id;

  /*!
   * \brief SOME/IP instance identifier
   */
  amsr::someip_protocol::internal::InstanceId instance_id;

  /*!
   * \brief Major version of service
   */
  amsr::someip_protocol::internal::MajorVersion major_version;

  /*!
   * \brief Eventgroup identifier of the eventgroup
   */
  amsr::someip_protocol::internal::EventgroupId eventgroup_id;

  /*!
   * \brief Counter
   */
  amsr::someip_protocol::internal::SdEntryCounter counter;

  // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
  /*!
   * \brief Compares SOME/IP SD Eventgroup entry IDs.
   *
   * \param[in] other A SOME/IP SD Eventgroup entry ID.
   * \return true if both entries IDs are equal and false otherwise.
   * \pre -
   * \context App
   * \steady FALSE
   * \reentrant FALSE
   */
  bool operator==(EventgroupEntryId const& other) const {
    return (service_id == other.service_id) && (instance_id == other.instance_id) &&
           (major_version == other.major_version) && (eventgroup_id == other.eventgroup_id) &&
           (counter == other.counter);
  }

  // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
  /*!
   * \brief Smaller than comparison of SOME/IP SD Service entry IDs.
   *
   * \param[in] other A SOME/IP SD Eventgroup entry ID.
   * \return true if the left-hand side entry is smaller than the right-hand side entry
   * \pre -
   * \context Network
   * \steady FALSE
   * \reentrant FALSE
   */
  bool operator<(EventgroupEntryId const& other) const {
    bool result{false};

    if (service_id < other.service_id) {
      result = true;
    } else if ((service_id == other.service_id) && (instance_id < other.instance_id)) {
      result = true;
    } else if ((service_id == other.service_id) && (instance_id == other.instance_id) &&
               (eventgroup_id < other.eventgroup_id)) {
      result = true;
    } else if ((service_id == other.service_id) && (instance_id == other.instance_id) &&
               (eventgroup_id == other.eventgroup_id) && (major_version < other.major_version)) {
      result = true;
    } else {
      //  Service entry IDs not matching. Do nothing.
    }
    return result;
  }
};

}  // namespace entries
}  // namespace message
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_ENTRIES_SERVICE_DISCOVERY_EVENTGROUP_ENTRY_ID_H_
