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
/*!        \file  auditing.h
 *        \brief  Definition of the memory buffer allocator alias.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_AUDITING_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_AUDITING_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/net/internal/types.h"
#include "amsr/net/ip/address.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace auditing {

using InstanceId = someip_protocol::internal::InstanceId;
using MinorVersion = someip_protocol::internal::MinorVersion;
using MajorVersion = someip_protocol::internal::MajorVersion;
using ServiceId = someip_protocol::internal::ServiceId;
using Port = net::ip::Port;
using Address = net::ip::Address;

/*!
 * \brief Possible reasons for auditing event "invalid SomeIp message"
 * \vpublic
 */
enum class InvalidSomeIpMessageReason : std::uint8_t {
  kBadProtocolVersion = 0U,  /*! bad protocol version */
  kBadHeader = 1U,           /*! bad header */
  kBadServiceId = 2U,        /*! bad service ID */
  kBadInterfaceVersion = 3U, /*! bad interface version */
  kBadMethodId = 4U          /*! bad method ID */
};

/*!
 * \brief Payload data type of auditing event "invalid SomeIp message"
 * \vpublic
 */
struct InvalidSomeIpMessage {
  /*!
   * \brief The reason for the invalid message
   */
  InvalidSomeIpMessageReason reason;
  /*!
   * \brief The service ID of the invalid message
   */
  ServiceId service_id;
  /*!
   * \brief The major version of the invalid message
   */
  MajorVersion major_version;
  /*!
   * \brief The minor version of the invalid message
   */
  MinorVersion minor_version;
  /*!
   * \brief The instance ID of the invalid message
   */
  InstanceId instance_id;
};

/*!
 * \brief Possible reasons for auditing event "invalid SomeIpSd message"
 * \vpublic
 */
enum class InvalidSomeIpSdMessageReason : std::uint8_t {
  kBadEntry = 0U,            /*! bad entry */
  kBadMessage = 1U,          /*! bad message */
  kBadOfferServiceEntry = 2U /*! bad OfferService entry */
};

/*!
 * \brief Transport Protocol enum
 * \vpublic
 */
enum class TransportProtocol : std::uint8_t {
  kUdp = 0U, /*! UDP */
  kTcp = 1U, /*! TCP */
};

/*!
 * \brief Payload data type of auditing event "invalid SomeIpSd message"
 * \vpublic
 */
struct InvalidSomeIpSdMessage {
  /*!
   * \brief The reason for the invalid message
   */
  InvalidSomeIpSdMessageReason reason;
  /*!
   * \brief The service ID of the invalid message
   */
  ServiceId service_id;
  /*!
   * \brief The major version of the invalid message
   */
  MajorVersion major_version;
  /*!
   * \brief The minor version of the invalid message
   */
  MinorVersion minor_version;
  /*!
   * \brief The instance ID of the invalid message
   */
  InstanceId instance_id;
  /*!
   * \brief The source endpoint of the invalid message
   */
  struct {
    /*!
     * \brief The transport protocol
     */
    TransportProtocol transport_protocol;
    /*!
     * \brief The IP address
     */
    net::ip::Address address;
    /*!
     * \brief The port
     */
    Port port;
  } source_endpoint;
};

/*!
 * \brief Payload data type of auditing event "event group subscription NACK"
 * \vpublic
 */
struct SubscribeEventgroupNack {
  /*!
   * \brief The service ID related to the subscription NACK
   */
  ServiceId service_id;
  /*!
   * \brief The major version related to the subscription NACK
   */
  MajorVersion major_version;
  /*!
   * \brief The minor version related to the subscription NACK
   */
  MinorVersion minor_version;
  /*!
   * \brief The instance ID related to the subscription NACK
   */
  InstanceId instance_id;
};

}  // namespace auditing
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_AUDITING_H_
