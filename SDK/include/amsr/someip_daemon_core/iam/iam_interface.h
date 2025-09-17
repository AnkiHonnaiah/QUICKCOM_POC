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
/*!        \file  someip_daemon_core/iam/iam_interface.h
 *        \brief  SOME/IP daemon specific interface for providing identity and access management (IAM).
 *
 *      \details  To achieve a dependency inversion between the SOME/IP daemon and the IAM component to allow optionally
 *                enabling / disabling the component this interface is defined. In order to provide actual IAM services
 *                it has to be implemented by some integration code.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_IAM_IAM_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_IAM_IAM_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/ipc/credentials.h"
#include "amsr/someip_daemon/extension_points/iam/iam_interface.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace iam {

/*!
 * \brief IAM access interface.
 */
class IamInterface {
 public:
  /*!
   * \brief Define default constructor.
   *
   * \steady  FALSE
   */
  IamInterface() noexcept = default;

  virtual ~IamInterface() noexcept = default;
  IamInterface(IamInterface const &) = delete;
  IamInterface(IamInterface &&) = delete;
  IamInterface &operator=(IamInterface const &) & = delete;
  IamInterface &operator=(IamInterface &&) & = delete;

  /*!
   * \brief Type alias for ApplicationId.
   */
  using ApplicationId = amsr::ipc::Credentials;
  /*!
   * \brief Type alias for ServiceId.
   */
  using ServiceId = someip_protocol::internal::ServiceId;
  /*!
   * \brief Type alias for MethodId.
   */
  using MethodId = someip_protocol::internal::MethodId;
  /*!
   * \brief Type alias for EventId.
   */
  using EventId = someip_protocol::internal::EventId;
  /*!
   * \brief Type alias for IpAddress.
   */
  using IpAddress = someip_protocol::internal::IpAddress;
  /*!
   * \brief Type alias for Port.
   */
  using Port = someip_protocol::internal::Port;
  /*!
   * \brief Type alias for EventgroupId.
   */
  using EventgroupId = someip_protocol::internal::EventgroupId;
  /*!
   * \brief Type alias for InstanceId.
   */
  using InstanceId = std::uint16_t;
  /*!
   * \brief Type alias for MajorVersion.
   */
  using MajorVersion = std::uint8_t;
  /*!
   * \brief Type alias for MinorVersion.
   */
  using MinorVersion = std::uint32_t;
  /*!
   * \brief Type alias for NetworkEndpoint.
   */
  using NetworkEndpoint = someip_daemon::extension_points::iam::IamInterface::NetworkEndpoint;

  // ----- Remote/Ingress checks -----

  /*!
   * \brief Check for access to find service SD operation from remote.
   * \param[in] service_id The service interface id of the service in question.
   * \param[in] major_version Major version of the service in question (ANY is allowed).
   * \param[in] minor_version Minor version of the service in question (ANY is allowed).
   * \param[in] instance_id The service instance id of the service in question.
   * \param[in] sd_sender The address and port of the requesting remote peer.
   * \pre -
   * \context   Network
   * \reentrant FALSE
   * \steady    FALSE
   * \return True if operation allowed, otherwise false.
   */
  virtual bool CheckFindService(ServiceId const service_id, MajorVersion const major_version,
                                MinorVersion const minor_version, InstanceId const instance_id,
                                NetworkEndpoint const &sd_sender) noexcept = 0;

  /*!
   * \brief Check for access to offer service SD operation from remote.
   * \param[in] service_id The service interface id of the service in question.
   * \param[in] major_version Major version of the service in question (ANY is allowed).
   * \param[in] minor_version Minor version of the service in question (ANY is allowed).
   * \param[in] instance_id The service instance id of the service in question.
   * \param[in] sd_sender The address and port of the requesting remote peer.
   * \param[in] tcp_endpoint Optional parameter. The address and port of the endpoint remote peer (TCP).
   * \param[in] udp_endpoint Optional parameter. The address and port of the endpoint remote peer (UDP).
   * \pre -
   * \context   Network
   * \reentrant FALSE
   * \steady    TRUE
   * \return True if operation allowed, otherwise false.
   */
  virtual bool CheckOfferService(ServiceId const service_id, MajorVersion const major_version,
                                 MinorVersion const minor_version, InstanceId const instance_id,
                                 NetworkEndpoint const &sd_sender,
                                 ara::core::Optional<NetworkEndpoint> const &tcp_endpoint,
                                 ara::core::Optional<NetworkEndpoint> const &udp_endpoint) noexcept = 0;

  /*!
   * \brief Generic check for method handling operations from remote.
   * \param[in] service_id The service interface id of the service in question.
   * \param[in] major_version Major version of the service in question (ANY is allowed).
   * \param[in] instance_id The service instance id of the service in question.
   * \param[in] method_id The method id corresponding to the operation.
   * \param[in] sender The address and port of the requesting remote peer.
   * \pre -
   * \context   Network
   * \reentrant FALSE
   * \steady    TRUE
   * \return True if operation allowed, otherwise false.
   */
  virtual bool CheckMethod(ServiceId const service_id, MajorVersion const major_version, InstanceId const instance_id,
                           MethodId const method_id, NetworkEndpoint const &sender) noexcept = 0;

  /*!
   * \brief Check if remote peer may subscribe to the event
   * As this is executed per event, it may contain one endpoint (unicast subscription) or no endpoint (multicast
   * subscription).
   * \param[in] service_id    The service interface id of the service in question.
   * \param[in] major_version Major version of the service in question (ANY is allowed).
   * \param[in] instance_id   The service instance id of the service in question.
   * \param[in] eventgroup_id The event group id corresponding to the operation.
   * \param[in] sd_sender     The address and port of the requesting remote peer.
   * \param[in] tcp_endpoint  Optional parameter. The content of the TCP endpoint option referenced by the
   *                          SubscribeEventgroup entry.
   * \param[in] udp_endpoint  Optional parameter. The content of the UDP endpoint option referenced by the
   *                          SubscribeEventgroup entry.
   * \pre -
   * \context   Network
   * \reentrant FALSE
   * \steady    TRUE
   * \return true if operation allowed,
   * otherwise false.
   */
  virtual bool CheckEventgroup(ServiceId const service_id, MajorVersion const major_version,
                               InstanceId const instance_id, EventgroupId eventgroup_id,
                               NetworkEndpoint const &sd_sender,
                               ara::core::Optional<IamInterface::NetworkEndpoint> const &tcp_endpoint,
                               ara::core::Optional<IamInterface::NetworkEndpoint> const &udp_endpoint) noexcept = 0;

  // ----- Local/Egress checks -----

  /*!
   * \brief Check to allow/deny proxy construction.
   * \details For historical reasons, the API is named as "CheckFindService".
   *          If the check fails, no proxy shall be created.
   * \param[in] service_id The service interface id of the service in question.
   * \param[in] major_version Major version of the service in question (ANY is allowed).
   * \param[in] minor_version Minor version of the service in question (ANY is allowed).
   * \param[in] instance_id The service instance id of the service in question.
   * \param[in] application_id Identifier for the requesting local application.
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady FALSE
   * \return True if operation allowed, otherwise false.
   */
  virtual bool CheckFindService(ServiceId const service_id, MajorVersion const major_version,
                                MinorVersion const minor_version, InstanceId const instance_id,
                                ApplicationId const application_id) noexcept = 0;

  /*!
   * \brief Check for access to offer service SD operation from local.
   * \param[in] service_id The service interface id of the service in question.
   * \param[in] major_version Major version of the service in question (ANY is allowed).
   * \param[in] minor_version Minor version of the service in question (ANY is allowed).
   * \param[in] instance_id The service instance id of the service in question.
   * \param[in] application_id Identifier for the requesting local application.
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   * \return True if operation allowed, otherwise false.
   */
  virtual bool CheckOfferService(ServiceId const service_id, MajorVersion const major_version,
                                 MinorVersion const minor_version, InstanceId const instance_id,
                                 ApplicationId const application_id) noexcept = 0;

  /*!
   * \brief Generic check for method handling operations from local.
   * \param[in] service_id The service interface id of the service in question.
   * \param[in] major_version Major version of the service in question (ANY is allowed).
   * \param[in] instance_id The service instance id of the service in question.
   * \param[in] method_id The method id corresponding to the operation.s
   * \param[in] application_id Identifier for the requesting local application.
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    TRUE
   * \return True if operation allowed, otherwise false.
   */
  virtual bool CheckMethod(ServiceId const service_id, MajorVersion const major_version, InstanceId const instance_id,
                           MethodId const method_id, ApplicationId const application_id) noexcept = 0;

  /*!
   * \brief Check if remote peer may subscribe to the event
   * \param[in] service_id The service interface id of the service in question.
   * \param[in] major_version Major version of the service in question (ANY is allowed).
   * \param[in] instance_id The service instance id of the service in question.
   * \param[in] event_id The event id corresponding to the operation.
   * \param[in] application_id Identifier for the requesting local application.
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    TRUE
   * \return True if operation allowed, otherwise false.
   */
  virtual bool CheckEventSubscription(ServiceId const service_id, MajorVersion const major_version,
                                      InstanceId const instance_id, EventId const event_id,
                                      ApplicationId const application_id) noexcept = 0;

  /*!
   * \brief Checks if IAM is enabled or not.
   * \pre -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   * \return    True if IAM is enabled, otherwise false.
   */
  virtual bool IsEnabled() const noexcept = 0;
};

}  // namespace iam
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_IAM_IAM_INTERFACE_H_
