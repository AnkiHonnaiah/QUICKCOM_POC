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
/*!        \file  tracing/tracing_datatypes.h
 *        \brief  SOME/IP daemon specific interface for providing tracing about communication and SD messages.
 *
 *      \details  To achieve a dependency inversion between the SOME/IP daemon and the tracing component to allow
 *                optionally enabling / disabling the component this interface is defined.
 *                In order to provide actual tracing services, it has to be implemented by some integration code.
 *
 *********************************************************************************************************************/

#ifndef LIB_EXTENSION_POINTS_INTERFACES_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_TRACING_TRACING_DATATYPES_H_
#define LIB_EXTENSION_POINTS_INTERFACES_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_TRACING_TRACING_DATATYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <sys/types.h>
#include <cstdint>

namespace amsr {
namespace someip_daemon {
namespace extension_points {
namespace tracing {

/*!
 * \brief Type alias for ProcessId.
 * \vpublic
 */
using ProcessId = pid_t;
/*!
 * \brief Type alias for ServiceId.
 * \vpublic
 */
using ServiceId = std::uint16_t;
/*!
 * \brief Type alias for MethodId.
 * \vpublic
 */
using MethodId = std::uint16_t;
/*!
 * \brief Type alias for EventId.
 * \vpublic
 */
using EventId = std::uint16_t;

/*!
 * \brief Type alias for InstanceId.
 * \vpublic
 */
using InstanceId = std::uint16_t;

/*!
 * \brief Type alias for MajorVersion.
 * \vpublic
 */
using MajorVersion = std::uint8_t;

/*!
 * \brief Type alias for MessageSize.
 * \vpublic
 */
using MessageSize = std::size_t;

/*!
 * \brief Metadata related to the process.
 * \vpublic
 */
struct ProcessMetaData {
  /*!
   * \brief The process id
   */
  ProcessId process_id;
};

/*!
 * \brief Metadata related to the service instance of a SOME/IP message.
 * \vpublic
 */
struct SomeIpInstanceMetaData {
  /*!
   * \brief The service id
   */
  ServiceId service_id;
  /*!
   * \brief The major version
   */
  MajorVersion major_version;
  /*!
   * \brief The instance id
   */
  InstanceId instance_id;
};

/*!
 * \brief Metadata related to the service instance of a PDU message.
 * \vpublic
 */
struct PduInstanceMetaData {
  /*!
   * \brief The service id
   */
  ServiceId service_id;
  /*!
   * \brief The instance id
   */
  InstanceId instance_id;
};

/*!
 * \brief Metadata related to an event.
 * \vpublic
 */
struct EventMetaData {
  /*!
   * \brief The event id
   */
  EventId event_id;

  /*!
   * \brief The message size in bytes
   */
  MessageSize size;
};

/*!
 * \brief Metadata related to the send of a SOME/IP event.
 * \vpublic
 */
struct SomeIpEventSendMetaData {
  /*!
   * \brief The process metadata
   */
  ProcessMetaData process;
  /*!
   * \brief The SOME/IP instance metadata
   */
  SomeIpInstanceMetaData instance;
  /*!
   * \brief The event metadata
   */
  EventMetaData event;
};

/*!
 * \brief Metadata related to the reception of a SOME/IP event.
 * \vpublic
 */
struct SomeIpEventReceiveMetaData {
  /*!
   * \brief The process metadata
   */
  ProcessMetaData process;
  /*!
   * \brief The SOME/IP instance metadata
   */
  SomeIpInstanceMetaData instance;
  /*!
   * \brief The event metadata
   */
  EventMetaData event;
};

/*!
 * \brief Metadata related to the send of a PDU event.
 * \vpublic
 */
struct PduEventSendMetaData {
  /*!
   * \brief The process metadata
   */
  ProcessMetaData process;

  /*!
   * \brief The PDU instance metadata
   */
  PduInstanceMetaData instance;

  /*!
   * \brief The event metadata
   */
  EventMetaData event;
};

/*!
 * \brief Metadata related to the reception of a PDU event.
 * \vpublic
 */
struct PduEventReceiveMetaData {
  /*!
   * \brief The process metadata
   */
  ProcessMetaData process;
  /*!
   * \brief The PDU instance metadata
   */
  PduInstanceMetaData instance;
  /*!
   * \brief The event metadata
   */
  EventMetaData event;
};

/*!
 * \brief Metadata related to a method.
 * \vpublic
 */
struct MethodMetaData {
  /*!
   * \brief The method id
   */
  MethodId method_id;

  /*!
   * \brief The message size in bytes
   */
  MessageSize size;
};

/*!
 * \brief Metadata related to the send of a method request.
 * \vpublic
 */
struct MethodRequestSendMetaData {
  /*!
   * \brief The process metadata
   */
  ProcessMetaData process;
  /*!
   * \brief The SOME/IP instance metadata
   */
  SomeIpInstanceMetaData instance;
  /*!
   * \brief The method id
   */
  MethodMetaData method;
};

/*!
 * \brief Metadata related to the send of a method response.
 * \vpublic
 */
struct MethodResponseSendMetaData {
  /*!
   * \brief The process metadata
   */
  ProcessMetaData process;
  /*!
   * \brief The instance metadata
   */
  SomeIpInstanceMetaData instance;
  /*!
   * \brief The method metadata
   */
  MethodMetaData method;
};

/*!
 * \brief Metadata related to the reception of a method request.
 * \vpublic
 */
struct MethodRequestReceiveMetaData {
  /*!
   * \brief The process metadata
   */
  ProcessMetaData process;
  /*!
   * \brief The SOME/IP instance metadata
   */
  SomeIpInstanceMetaData instance;
  /*!
   * \brief The method metadata
   */
  MethodMetaData method;
};

/*!
 * \brief Metadata related to the reception of a method response.
 * \vpublic
 */
struct MethodResponseReceiveMetaData {
  /*!
   * \brief The process metadata id
   */
  ProcessMetaData process;
  /*!
   * \brief The SOME/IP instance metadata
   */
  SomeIpInstanceMetaData instance;
  /*!
   * \brief The method id
   */
  MethodMetaData method;
};

/*!
 * \brief Metadata related to the reception of a service instance update message.
 * \vpublic
 */
struct ServiceInstanceUpdateMetaData {
  /*!
   * \brief The process metadata
   */
  ProcessMetaData process;
  /*!
   * \brief The SOME/IP instance metadata
   */
  SomeIpInstanceMetaData instance;
};

}  // namespace tracing
}  // namespace extension_points
}  // namespace someip_daemon
}  // namespace amsr

#endif  // LIB_EXTENSION_POINTS_INTERFACES_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_TRACING_TRACING_DATATYPES_H_
