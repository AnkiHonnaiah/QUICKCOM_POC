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
/**        \file  remote_client_interface.h
 *        \brief  Remote Client(s) interface.
 *
 *      \details  Interface for sending SOMEIP and PDU events.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_REMOTE_CLIENT_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_REMOTE_CLIENT_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/net/ip/address.h"
#include "amsr/someip_daemon_core/packet_sink/packet_sink_interface.h"
#include "amsr/someip_daemon_core/server/server_field_cache_interface.h"
#include "ara/core/result.h"
#include "someip-protocol/internal/pdu_message.h"
#include "someip-protocol/internal/someip_message.h"

namespace amsr {
namespace someip_daemon_core {
namespace server {

/*!
 * \brief RemoteClientInterface.
 */

class RemoteClientInterface {
 public:
  /*!
   * \brief Represents a SOME/IP message.
   */
  using SomeIpPacketSharedPtr = std::shared_ptr<someip_protocol::internal::SomeIpMessage>;

  /*!
   * \brief Represents a S2S PDU message.
   */
  using PduPacketSharedPtr = std::shared_ptr<someip_protocol::internal::PduMessage>;

  /*!
   * \brief default constructor.
   * \steady FALSE
   */
  RemoteClientInterface() = default;

  /*!
   * \brief Define default constructor.
   * \steady FALSE
   */
  virtual ~RemoteClientInterface() noexcept = default;

  RemoteClientInterface(RemoteClientInterface const&) = delete;
  RemoteClientInterface(RemoteClientInterface&&) = delete;
  RemoteClientInterface& operator=(RemoteClientInterface const&) & = delete;
  RemoteClientInterface& operator=(RemoteClientInterface&&) & = delete;

  /*!
   * \brief Offer a provided service instance.
   *
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   */
  virtual void OfferService() noexcept = 0;

  /*!
   * \brief Stop offering a provided service instance.
   *
   * \context App
   * \reentrant FALSE
   * \steady FALSE
   */
  virtual void StopOfferService() noexcept = 0;

  /*!
   * \brief Send a SOME/IP event message to a remote peer.
   *
   * \param[in] packet A SOME/IP message.
   *
   * \return A result with the value void if successful, with an error otherwise.
   * \error amsr::generic::GenErrc::kSystemConfiguration If the event is not found
   *        in any provided eventgroup.
   * \error amsr::generic::GenErrc::kRuntimeConnectionTransmissionFailed if the event couldn't be
   *        forwarded.
   *
   * \context App
   * \reentrant FALSE
   * \steady TRUE
   */

  virtual ara::core::Result<void> SendSomeIpEvent(SomeIpPacketSharedPtr packet) noexcept = 0;

  /*!
   * \brief Send a PDU event message to a remote peer.
   *
   * \param[in] packet A PDU message.
   *
   * \return A result with the value void if successful, with an error otherwise.
   * \error amsr::generic::GenErrc::kSystemConfiguration If the event is not found
   *        in any provided eventgroup.
   * \error amsr::generic::GenErrc::kRuntimeConnectionTransmissionFailed if the event couldn't be
   *        forwarded.
   *
   * \context Network
   * \reentrant FALSE
   * \steady TRUE
   */
  virtual ara::core::Result<void> SendPduEvent(PduPacketSharedPtr packet) noexcept = 0;

  /*!
   * \brief Register the field cache handler.
   *
   * \param[in] server_field_cache    The Server field cache.
   *
   * \pre -
   * \context App
   * \reentrant FALSE
   * \steady FALSE
   */
  virtual void RegisterFieldCacheHandler(ServerCacheInterface* server_field_cache) noexcept = 0;
};

}  // namespace server
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_REMOTE_CLIENT_INTERFACE_H_
