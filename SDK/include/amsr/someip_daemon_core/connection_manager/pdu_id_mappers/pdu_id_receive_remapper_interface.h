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
 *        \brief  Interface class for checking and remapping PduId in a PduMessageHeader.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_PDU_ID_MAPPERS_PDU_ID_RECEIVE_REMAPPER_INTERFACE_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_PDU_ID_MAPPERS_PDU_ID_RECEIVE_REMAPPER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/map.h"
#include "someip-protocol/internal/message.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {
namespace pdu_id_mappers {

/*!
 * \brief           Check and remap PduId to ServiceId/EventId
 */
class PduIdReceiveRemapperInterface {
 public:
  /*!
   * \brief           Container to hold pdu id to event id mappings
   */
  using PduIdToEventIdMap = ara::core::Map<someip_protocol::internal::PduId, someip_protocol::internal::EventId>;
  /*!
   * \brief           Constructor
   * \steady          TRUE
   */
  PduIdReceiveRemapperInterface() noexcept = default;

  /*!
   * \brief           Default destructor
   * \steady          TRUE
   */
  virtual ~PduIdReceiveRemapperInterface() noexcept = default;

  /*!
   * \brief           Delete copy constructor
   * \steady          TRUE
   */
  PduIdReceiveRemapperInterface(PduIdReceiveRemapperInterface const&) = delete;

  /*!
   * \brief           Delete move constructor
   * \steady          TRUE
   */
  PduIdReceiveRemapperInterface(PduIdReceiveRemapperInterface&&) = delete;

  /*!
   * \brief           Delete copy assignment operator
   * \steady          TRUE
   */
  PduIdReceiveRemapperInterface& operator=(PduIdReceiveRemapperInterface const&) = delete;

  /*!
   * \brief           Delete move assignment operator
   * \steady          TRUE
   */
  PduIdReceiveRemapperInterface& operator=(PduIdReceiveRemapperInterface&&) = delete;

  /*!
   * \brief           Read configuration and populate a map with all PduId to ServiceId/EventId mappings
   * \param[in]       service_id
   *                  Service ID of the service whose pdu ids are to be added.
   * \param[in]       pdu_list
   *                  list holds the pdu to event mapping
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \steady          TRUE
   */
  virtual void AddPduMappings(someip_protocol::internal::ServiceId const service_id,
                              PduIdToEventIdMap const& pdu_list) noexcept = 0;

  /*!
   * \brief           Remap the PduId inside the PduMessageHeader
   * \details         It is expected that the PduId is present in the registered signal serialized pdu remappings, run
   *                  run CheckPduId() before to make sure that it is the case.
   * \param[in]       pdu_id
   *                  PduId remap.
   * \return          true if PduId exists in configuration.
   * \context         ANY
   * \pre             The PduId must be present in the registered signal serialized pdu remappings.
   * \threadsafe      TRUE, for different this pointer
   * \steady          TRUE
   */
  virtual void RemapPduId(someip_protocol::internal::PduId& pdu_id) noexcept = 0;

  /*!
   * \brief           Check if PduId is present in the registered signal serialized pdu remappings
   * \param           pdu_id
   *                  PduId to find.
   * \return          true if PduId exists in configuration.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \steady          TRUE
   */
  virtual bool CheckPduId(someip_protocol::internal::PduId pdu_id) noexcept = 0;
};

}  // namespace pdu_id_mappers
}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_PDU_ID_MAPPERS_PDU_ID_RECEIVE_REMAPPER_INTERFACE_H_
