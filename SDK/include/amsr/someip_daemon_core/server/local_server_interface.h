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
/**        \file  local_server_interface.h
 *        \brief  Local Server Interface.
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_LOCAL_SERVER_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_LOCAL_SERVER_INTERFACE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/someip_daemon_core/packet_sink/packet_sink_interface.h"
#include "someip-protocol/internal/someip_message.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace server {

/*!
 * \brief A Local Sever interface.
 */
class LocalServerInterface {
 public:
  /*!
   * \brief Represents a SOME/IP message.
   */
  using SomeIpPacket = std::shared_ptr<someip_protocol::internal::SomeIpMessage>;

  /*!
   * \brief Represents a SOME/IP message.
   */
  using PduPacket = std::shared_ptr<someip_protocol::internal::PduMessage>;

  /*!
   * \brief Type alias for the shared pointer to LocalPacketSink.
   */
  using LocalPacketSinkSharedPtr = std::shared_ptr<packet_sink::LocalPacketSink>;

  /*!
   * \brief Constructor of LocalServerInterface.
   * \steady FALSE
   */
  LocalServerInterface() = default;
  /*!
   * \brief Destructor of LocalServerInterface.
   * \steady FALSE
   */
  // VECTOR NC AutosarC++17_10-A10.3.2: MD_SomeIpDaemon_AutosarC++17_10_A10.3.2_override_keyword_needed_false_positive
  virtual ~LocalServerInterface() noexcept = default;

  LocalServerInterface(LocalServerInterface const&) = delete;
  LocalServerInterface(LocalServerInterface&&) = delete;
  LocalServerInterface& operator=(LocalServerInterface const&) & = delete;
  LocalServerInterface& operator=(LocalServerInterface&&) & = delete;

  /*!
   * \brief Called when the connected application offers the given service instance.
   *
   * \param[in] application The local application that offers the service.
   *
   * \pre -
   * \context App
   * \reentrant FALSE
   * \steady FALSE
   *
   * \trace SPEC-8053469
   * \trace SPEC-4980348
   */
  virtual void OfferService(LocalPacketSinkSharedPtr const& application) noexcept = 0;

  /*!
   * \brief Called when the connected application stops offering the given service instance.
   *
   * \pre -
   * \context App
   * \reentrant FALSE
   * \steady FALSE
   */
  virtual void StopOfferService() noexcept = 0;

  /*!
   * \brief Forwards a local SOME/IP resopnse message.
   *
   * \param[in] packet           A SOME/IP respons message.
   * \param[in] recipient        The response recipient.
   *
   * \pre -
   * \context App
   * \reentrant FALSE
   * \steady TRUE
   */
  virtual void SendMethodResponse(SomeIpPacket const& packet,
                                  packet_router::RemoteSourceIdentifier const& recipient) const noexcept = 0;

  /*!
   * \brief Forwards a local SOME/IP event.
   *
   * \param[in] packet    A SOME/IP request message.
   *
   * \pre -
   * \context App
   * \reentrant FALSE
   * \steady TRUE
   */
  virtual void SendEvent(SomeIpPacket const& packet) noexcept = 0;

  /*!
   * \brief Forwards a local PDU event.
   *
   * \param[in] packet    A PDU request message.
   *
   * \pre -
   * \context App
   * \reentrant FALSE
   * \steady TRUE
   */
  virtual void SendPduEvent(PduPacket const& packet) const noexcept = 0;
};

}  // namespace server
}  // namespace someip_daemon_core
}  // namespace amsr
#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVER_LOCAL_SERVER_INTERFACE_H_
