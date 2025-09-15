/*********************************************************************************************************************
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
/*!        \file  iam/iam_interface.h
 *        \brief  SOME/IP daemon specific interface for providing identity and access management (IAM).
 *
 *      \details  To achieve a dependency inversion between the SOME/IP daemon and the IAM component to allow optionally
 *                enabling / disabling the component this interface is defined. In order to provide actual IAM services
 *                it has to be implemented by some integration code.
 *
 *********************************************************************************************************************/

#ifndef LIB_EXTENSION_POINTS_INTERFACES_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_IAM_IAM_INTERFACE_H_
#define LIB_EXTENSION_POINTS_INTERFACES_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_IAM_IAM_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include "ara/core/optional.h"
#include "ara/core/string.h"

namespace amsr {
namespace someip_daemon {
namespace extension_points {
namespace iam {

/*!
 * \brief IAM access interface.
 * \vpublic
 */
class IamInterface {
 public:
  /*!
   * \brief Define default constructor.
   * \steady FALSE
   */
  IamInterface() noexcept = default;

  virtual ~IamInterface() noexcept = default;
  IamInterface(IamInterface const &) = delete;
  IamInterface(IamInterface &&) = delete;
  IamInterface &operator=(IamInterface const &) & = delete;
  IamInterface &operator=(IamInterface &&) & = delete;

  /*!
   * \brief Type alias for ApplicationId.
   * \vpublic
   */
  using ApplicationId = std::uint64_t;
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
   * \brief Type alias for IpAddress.
   * \vpublic
   */
  using IpAddress = ara::core::String;
  /*!
   * \brief Type alias for Port.
   * \vpublic
   */
  using Port = std::uint16_t;
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
   * \brief Type alias for MinorVersion.
   * \vpublic
   */
  using MinorVersion = std::uint32_t;

  /*!
   * \brief Network endpoint including IP address and port.
   * \vpublic
   * - IP address
   * - Port
   */
  // VECTOR NL AutosarC++17_10-M8.5.1: MD_SomeIpDaemon_AutosarC++17_10_M8.5.1_false_positive
  struct NetworkEndpoint {
    /*!
     * \brief IP address
     * \vpublic
     */
    IpAddress address;
    /*!
     * \brief Port
     * \vpublic
     */
    Port port;
  };

  // ----- Remote/Ingress checks -----

  /*!
   * \brief Check for access to find service SD operation from remote.
   * \vpublic
   * \param[in] service_id The service interface id of the service in question.
   * \param[in] major_version Major version of the service in question (ANY is allowed (0xFFU)).
   * \param[in] minor_version Minor version of the service in question (ANY is allowed (0xFFFFFFFFU)).
   * \param[in] instance_id The service instance id of the service in question (ALL is allowed (0xFFFFU)).
   * \param[in] sd_sender The address and port of the requesting remote peer.
   * \pre -
   * \context Network
   * \reentrant FALSE
   * \return True if operation allowed, otherwise false.
   * \steady FALSE
   */
  virtual bool CheckFindService(ServiceId const service_id, MajorVersion const major_version,
                                MinorVersion const minor_version, InstanceId const instance_id,
                                NetworkEndpoint const &sd_sender) noexcept = 0;

  /*!
   * \brief Check for access to offer service SD operation from remote.
   * \vpublic
   * \param[in] service_id The service interface id of the service in question.
   * \param[in] major_version Major version of the service in question.
   * \param[in] minor_version Minor version of the service in question.
   * \param[in] instance_id The service instance id of the service in question.
   * \param[in] sd_sender The address and port of the requesting remote peer.
   * \param[in] tcp_endpoint Optional parameter. The address and port of the endpoint remote peer (TCP).
   * \param[in] udp_endpoint Optional parameter. The address and port of the endpoint remote peer (UDP).
   * \pre -
   * \context Network
   * \reentrant FALSE
   * \return True if operation allowed, otherwise false.
   * \steady TRUE
   */
  virtual bool CheckOfferService(ServiceId const service_id, MajorVersion const major_version,
                                 MinorVersion const minor_version, InstanceId const instance_id,
                                 NetworkEndpoint const &sd_sender,
                                 ara::core::Optional<NetworkEndpoint> const &tcp_endpoint,
                                 ara::core::Optional<NetworkEndpoint> const &udp_endpoint) noexcept = 0;

  /*!
   * \brief Check if remote peer may subscribe to the event
   * \vpublic
   * \details It may contain one endpoint (unicast subscription) or no endpoint (multicast only event communication)
   * \param[in] service_id The service interface id of the service in question.
   * \param[in] major_version Major version of the service in question.
   * \param[in] instance_id The service instance id of the service in question.
   * \param[in] event_id The event id corresponding to the operation.
   * \param[in] sd_sender The address and port of the requesting remote peer.
   * \param[in] endpoint Optional parameter. The address and port of the endpoint remote peer (unicast subscription). If
   *                     no endpoint (empty parameter) it is considered a multicast subscription.
   * \pre -
   * \context Network
   * \reentrant FALSE
   * \return True if operation allowed, otherwise false.
   * \steady TRUE
   */
  virtual bool CheckEventSubscription(ServiceId const service_id, MajorVersion const major_version,
                                      InstanceId const instance_id, EventId event_id, NetworkEndpoint const &sd_sender,
                                      ara::core::Optional<NetworkEndpoint> const &endpoint) noexcept = 0;

  /*!
   * \brief Generic check for method handling operations from remote.
   * \vpublic
   * \param[in] service_id The service interface id of the service in question.
   * \param[in] major_version Major version of the service in question.
   * \param[in] instance_id The service instance id of the service in question.
   * \param[in] method_id The method id corresponding to the operation.
   * \param[in] sender The address and port of the requesting remote peer.
   * \pre -
   * \context Network
   * \reentrant FALSE
   * \return True if operation allowed, otherwise false.
   * \steady TRUE
   */
  virtual bool CheckMethod(ServiceId const service_id, MajorVersion const major_version, InstanceId const instance_id,
                           MethodId const method_id, NetworkEndpoint const &sender) noexcept = 0;

  // ----- Local/Egress checks -----

  /*!
   * \brief Check to allow/deny proxy construction.
   * \vpublic
   * \details For historical reasons, the API is named as "CheckFindService".
   *          If the check fails, no proxy shall be created.
   * \param[in] service_id The service interface id of the service in question.
   * \param[in] major_version Major version of the service in question.
   * \param[in] minor_version Minor version of the service in question.
   * \param[in] instance_id The service instance id of the service in question.
   * \param[in] application_id Identifier for the requesting local application.
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \return True if operation allowed, otherwise false.
   * \steady FALSE
   */
  virtual bool CheckFindService(ServiceId const service_id, MajorVersion const major_version,
                                MinorVersion const minor_version, InstanceId const instance_id,
                                ApplicationId const application_id) noexcept = 0;

  /*!
   * \brief Check for access to offer service SD operation from local.
   * \vpublic
   * \param[in] service_id The service interface id of the service in question.
   * \param[in] major_version Major version of the service in question.
   * \param[in] minor_version Minor version of the service in question.
   * \param[in] instance_id The service instance id of the service in question.
   * \param[in] application_id Identifier for the requesting local application.
   * \pre -
   * \context App
   * \reentrant FALSE
   * \return True if operation allowed, otherwise false.
   * \steady FALSE
   */
  virtual bool CheckOfferService(ServiceId const service_id, MajorVersion const major_version,
                                 MinorVersion const minor_version, InstanceId const instance_id,
                                 ApplicationId const application_id) noexcept = 0;

  /*!
   * \brief Check if remote peer may subscribe to the event
   * \vpublic
   * \param[in] service_id The service interface id of the service in question.
   * \param[in] major_version Major version of the service in question.
   * \param[in] instance_id The service instance id of the service in question.
   * \param[in] event_id The event id corresponding to the operation.
   * \param[in] application_id Identifier for the requesting local application.
   * \pre -
   * \context App
   * \reentrant FALSE
   * \return True if operation allowed, otherwise false.
   * \steady TRUE
   */
  virtual bool CheckEventSubscription(ServiceId const service_id, MajorVersion const major_version,
                                      InstanceId const instance_id, EventId const event_id,
                                      ApplicationId const application_id) noexcept = 0;

  /*!
   * \brief Generic check for method handling operations from local.
   * \vpublic
   * \param[in] service_id The service interface id of the service in question.
   * \param[in] major_version Major version of the service in question.
   * \param[in] instance_id The service instance id of the service in question.
   * \param[in] method_id The method id corresponding to the operation.s
   * \param[in] application_id Identifier for the requesting local application.
   * \pre -
   * \context App
   * \reentrant FALSE
   * \return True if operation allowed, otherwise false.
   * \steady TRUE
   */
  virtual bool CheckMethod(ServiceId const service_id, MajorVersion const major_version, InstanceId const instance_id,
                           MethodId const method_id, ApplicationId const application_id) noexcept = 0;
};

}  // namespace iam
}  // namespace extension_points
}  // namespace someip_daemon
}  // namespace amsr

#endif  // LIB_EXTENSION_POINTS_INTERFACES_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_IAM_IAM_INTERFACE_H_
