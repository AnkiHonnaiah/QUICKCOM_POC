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
/*!        \file
 *        \brief  This unit handles reception of protocol messages received from UDP datagrams.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_UDP_MESSAGE_RECEPTION_HANDLER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_UDP_MESSAGE_RECEPTION_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon/extension_points/message_authentication/mac_interface.h"
#include "amsr/someip_daemon/extension_points/statistics_handler/statistics_handler_interface.h"
#include "amsr/someip_daemon_core/connection_manager/meta_data_manager/meta_data_manager.h"
#include "amsr/someip_daemon_core/connection_manager/pdu_id_mappers/pdu_id_receive_remapper_interface.h"
#include "amsr/someip_daemon_core/connection_manager/udp_message_reception_handler_interface.h"
#include "amsr/someip_daemon_core/message_reader/datagram_message_reader.h"
#include "amsr/someip_daemon_core/packet_router/network_packet_router_interface.h"
#include "someip_tp/someip_tp_reception_controller.h"

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {

/*!
 * \brief           UdpMessageReceptionHandler processes a received protocol message and forwards it to packet router on
 *                  success
 */
class UdpMessageReceptionHandler final : public UdpMessageReceptionHandlerInterface {
 public:
  /*!
   * \brief           Constructor
   * \param[in]       mac_manager
   *                  MacManager used to verify protocol messages.
   * \param[in]       statistics_handler
   *                  StatisticHandler
   * \param[in]       meta_data_manager
   *                  MetaDataManager used to collect meta data.
   * \param[in]       packet_router
   *                  PacketRouterInterface used to forward received messages to.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \steady          TRUE
   */
  UdpMessageReceptionHandler(
      someip_daemon::extension_points::message_authentication::MacInterface* mac_manager,
      someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface& statistics_handler,
      meta_data_manager::MetaDataManager& meta_data_manager,
      std::shared_ptr<packet_router::NetworkPacketRouterInterface> packet_router) noexcept
      : UdpMessageReceptionHandlerInterface(),
        mac_manager_{mac_manager},
        statistics_handler_{statistics_handler},
        meta_data_manager_{meta_data_manager},
        packet_router_{packet_router},
        pdu_receive_remapper_{ara::core::nullopt} {
    assert(packet_router != nullptr);
  }

  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  /*!
   * \brief           Default destructor
   * \steady          TRUE
   */
  ~UdpMessageReceptionHandler() noexcept override = default;

  UdpMessageReceptionHandler() = delete;
  UdpMessageReceptionHandler(UdpMessageReceptionHandler const&) = delete;
  UdpMessageReceptionHandler(UdpMessageReceptionHandler&&) = delete;
  UdpMessageReceptionHandler& operator=(UdpMessageReceptionHandler const&) = delete;
  UdpMessageReceptionHandler& operator=(UdpMessageReceptionHandler&&) = delete;

  /*!
   * \brief           Processes the given protocol message
   * \details         Upon success the PDU or SOME/IP message is forwarded to the packet router.
   * \param[in]       message
   *                  The protocol message to proccess.
   * \return          Result<void> on success.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \steady          TRUE
   */
  ara::core::Result<void> ProcessMessage(message_reader::ProtocolMessage const& message) const noexcept final {
    // TODO(ARCTOR-5477)
    static_cast<void>(message);
    static_cast<void>(mac_manager_);
    static_cast<void>(statistics_handler_);
    static_cast<void>(meta_data_manager_);
    static_cast<void>(packet_router_);
    static_cast<void>(pdu_receive_remapper_);
    return ara::core::Result<void>{};
  }

 private:
  /*!
   * \brief           MacManager used to verify protocol messages
   */
  someip_daemon::extension_points::message_authentication::MacInterface* mac_manager_;
  /*!
   * \brief           StatisticHandler
   */
  someip_daemon::extension_points::statistics_handler::StatisticsHandlerInterface& statistics_handler_;
  /*!
   * \brief           MetaDataManager used to collect meta data
   */
  meta_data_manager::MetaDataManager& meta_data_manager_;
  /*!
   * \brief           NetworkPacketRouterInterface used to forward received messages to
   */
  std::shared_ptr<packet_router::NetworkPacketRouterInterface> packet_router_;
  /*!
   * \brief           PduIdReceiveRemapper used to map received PDU messages to SOME/IP service instances
   */
  ara::core::Optional<pdu_id_mappers::PduIdReceiveRemapper> pdu_receive_remapper_;
};

}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_UDP_MESSAGE_RECEPTION_HANDLER_H_
