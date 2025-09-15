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
 *        \brief  Class that overwrites a ServiceId/EventId pair in a PduMessageHeader with a PduId if existent in the
 *                supplied configuration.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_PDU_ID_MAPPERS_PDU_ID_TRANSMIT_REMAPPER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_PDU_ID_MAPPERS_PDU_ID_TRANSMIT_REMAPPER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/net/ip/udp.h"
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/connection_manager/pdu_id_mappers/pdu_id_transmit_remapper_interface.h"
#include "ara/core/map.h"

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {
namespace pdu_id_mappers {

/*!
 * \brief           Remap ServiceId/EventId to PduId
 */
class PduIdTransmitRemapper : public PduIdTransmitRemapperInterface {
 public:
  PduIdTransmitRemapper() = delete;
  PduIdTransmitRemapper(PduIdTransmitRemapper const&) = delete;
  PduIdTransmitRemapper(PduIdTransmitRemapper&&) = delete;
  PduIdTransmitRemapper& operator=(PduIdTransmitRemapper const&) = delete;
  PduIdTransmitRemapper& operator=(PduIdTransmitRemapper&&) = delete;

  /*!
   * \brief           Read configuration and populate a map with all ServiceId/EventId to PduId mappings
   * \param[in]       config
   *                  Global configuration for a SOME/IP daemon.
   * \param[in]       address
   *                  Local address.
   * \param[in]       port
   *                  Local port.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \synchronous     TRUE
   * \steady          TRUE
   */
  explicit PduIdTransmitRemapper(configuration::Configuration const* config,
                                 someip_protocol::internal::IpAddress const& address,
                                 someip_protocol::internal::Port port) noexcept;

  /*!
   * \brief           Default destructor
   * \steady          TRUE
   */
  ~PduIdTransmitRemapper() noexcept override = default;

  /*!
   * \copydoc         connection_manager::pdu_id_mappers::PduIdTransmitRemapperInterface::RemapPdu
   * \steady          TRUE
   */
  void RemapPdu(someip_protocol::internal::PduMessage& pdu_message) noexcept override;

 private:
  /*!
   * \brief           A provided signal pdu remapping container
   */
  using ProvidedSignalPduRemappingContainer =
      ara::core::Map<someip_protocol::internal::PduId, someip_protocol::internal::PduId>;

  /*!
   * \brief           A container of registered provided signal serialized pdu remappings
   */
  ProvidedSignalPduRemappingContainer provided_signal_pdu_remappings_;

  /*!
   * \brief           Friend test to access the private member required_signal_pdu_remappings_
   */
  FRIEND_TEST(UT__SomeIpd__ConnectionManager__PduIdTransmitRemapper, PduIdTransmitRemapper);
};

}  // namespace pdu_id_mappers
}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_PDU_ID_MAPPERS_PDU_ID_TRANSMIT_REMAPPER_H_
