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
/**        \file  service_discovery_entry.h
 *        \brief  SD entry in wire format
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_ENTRIES_SERVICE_DISCOVERY_ENTRY_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_ENTRIES_SERVICE_DISCOVERY_ENTRY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include "ara/core/vector.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace message {
namespace entries {

/*!
 * \brief Represents a SOME/IP SD entry.
 */
struct ServiceDiscoveryEntry {
  /*!
   * \brief SOME/IP SD entry type
   */
  amsr::someip_protocol::internal::SdEntryType type_;

  /*!
   * \brief SOME/IP service identifier
   */
  amsr::someip_protocol::internal::ServiceId service_id_;

  /*!
   * \brief SOME/IP instance identifier
   */
  amsr::someip_protocol::internal::InstanceId instance_id_;

  /*!
   * \brief Major version of service
   */
  amsr::someip_protocol::internal::MajorVersion major_version_;

  /*!
   * \brief Minor version of service (not used in SubscribeEventgroup/StopSubscribeEventgroup and
   * SubscribeEventgroupAck/SubscribeEventgroupNack entries)
   */
  amsr::someip_protocol::internal::MinorVersion minor_version_;

  /*!
   * \brief Index of the first options
   */
  amsr::someip_protocol::internal::SdEntryOptionIndex index_1st_opts_;

  /*!
   * \brief Index of the second options
   */
  amsr::someip_protocol::internal::SdEntryOptionIndex index_2nd_opts_;

  /*!
   * \brief Number of first options
   */
  amsr::someip_protocol::internal::SdEntryOptionCount number_1st_opts_;

  /*!
   * \brief Number of second options
   */
  amsr::someip_protocol::internal::SdEntryOptionCount number_2nd_opts_;

  /*!
   * \brief Time to live
   */
  amsr::someip_protocol::internal::Ttl ttl_;

  /*!
   * \brief Counter (valid only for SubscribeEventgroup/StopSubscribeEventgroup entries).
   * \details It contains always 0x00 and is the combination of initial_data_requested + reserved2 + counter within the
   * SD Eventgroup entry. It goes against PRS_SOMEIPSD_00703.
   * \trace SPEC-4981690
   */
  amsr::someip_protocol::internal::SdEntryCounter counter_;

  /*!
   * \brief Event group identifier (valid only for SubscribeEventgroup/StopSubscribeEventgroup entries)
   */
  amsr::someip_protocol::internal::EventgroupId eventgroup_id_;

  // VECTOR NC VectorC++-V11.0.3: MD_SomeIpDaemon_V11.0.3_public_data_members
  /*!
   * \brief Compares SOME/IP SD entries.
   *
   * \param[in] other A SOME/IP SD entry to compare to.
   * \return true if both entries are equal and false otherwise.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   */
  bool operator==(ServiceDiscoveryEntry const& other) const {
    return (type_ == other.type_) && (service_id_ == other.service_id_) && (instance_id_ == other.instance_id_) &&
           (major_version_ == other.major_version_) && (minor_version_ == other.minor_version_) &&
           (index_1st_opts_ == other.index_1st_opts_) && (index_2nd_opts_ == other.index_2nd_opts_) &&
           (number_1st_opts_ == other.number_1st_opts_) && (number_2nd_opts_ == other.number_2nd_opts_) &&
           (ttl_ == other.ttl_) && (counter_ == other.counter_) && (eventgroup_id_ == other.eventgroup_id_);
  }
};

/*!
 * \brief A container for SD entries.
 */
using ServiceDiscoveryEntryContainer = ara::core::Vector<ServiceDiscoveryEntry>;

}  // namespace entries
}  // namespace message
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_ENTRIES_SERVICE_DISCOVERY_ENTRY_H_
