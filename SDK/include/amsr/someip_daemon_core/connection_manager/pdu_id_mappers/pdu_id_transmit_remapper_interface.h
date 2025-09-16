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
 *        \brief  Interface class for remapping PduId in a PduMessageHeader.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_PDU_ID_MAPPERS_PDU_ID_TRANSMIT_REMAPPER_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_PDU_ID_MAPPERS_PDU_ID_TRANSMIT_REMAPPER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someip-protocol/internal/pdu_message.h"

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {
namespace pdu_id_mappers {

/*!
 * \brief           Remap ServiceId/EventId to PduId
 */
class PduIdTransmitRemapperInterface {
 public:
  /*!
   * \brief           Constructor
   * \steady          TRUE
   */
  PduIdTransmitRemapperInterface() noexcept = default;

  /*!
   * \brief           Default destructor
   * \steady          TRUE
   */
  virtual ~PduIdTransmitRemapperInterface() noexcept = default;

  /*!
   * \brief           Delete copy constructor
   * \steady          TRUE
   */
  PduIdTransmitRemapperInterface(PduIdTransmitRemapperInterface const&) = delete;

  /*!
   * \brief           Delete move constructor
   * \steady          TRUE
   */
  PduIdTransmitRemapperInterface(PduIdTransmitRemapperInterface&&) = delete;

  /*!
   * \brief           Delete copy assignment
   * \steady          TRUE
   */
  PduIdTransmitRemapperInterface& operator=(PduIdTransmitRemapperInterface const&) = delete;

  /*!
   * \brief           Delete move assignment
   * \steady          TRUE
   */
  PduIdTransmitRemapperInterface& operator=(PduIdTransmitRemapperInterface&&) = delete;

  /*!
   * \brief           Remap the PduId inside the PduMessage
   * \param           pdu_message
   *                  Message containing PduId to remap.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \steady          TRUE
   */
  virtual void RemapPdu(someip_protocol::internal::PduMessage& pdu_message) noexcept = 0;
};

}  // namespace pdu_id_mappers
}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_PDU_ID_MAPPERS_PDU_ID_TRANSMIT_REMAPPER_INTERFACE_H_
