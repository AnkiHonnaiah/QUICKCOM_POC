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
 *        \brief  Class that overwrites a PduId in a PduMessageHeader with a ServiceId/EventId pair if existent in the
 *                supplied configuration.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_PDU_ID_MAPPERS_PDU_ID_RECEIVE_REMAPPER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_PDU_ID_MAPPERS_PDU_ID_RECEIVE_REMAPPER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/connection_manager/pdu_id_mappers/pdu_id_receive_remapper_interface.h"
#include "ara/core/map.h"  // map
#include "someip-protocol/internal/deserialization/reader.h"
#include "someip-protocol/internal/marshalling.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "vac/container/static_vector.h"

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {
namespace pdu_id_mappers {

/*!
 * \brief           Check and remap PduId to ServiceId/EventId
 */
class PduIdReceiveRemapper final : public PduIdReceiveRemapperInterface {
 public:
  /*!
   * \brief           Define default constructor
   * \steady          TRUE
   */
  PduIdReceiveRemapper() = default;

  /*!
   * \copydoc         connection_manager::pdu_id_mappers::PduIdReceiveRemapperInterface::AddPduMappings
   * \steady          TRUE
   */
  void AddPduMappings(someip_protocol::internal::ServiceId const service_id,
                      PduIdToEventIdMap const& pdu_list) noexcept final;

  /*!
   * \copydoc         connection_manager::pdu_id_mappers::PduIdReceiveRemapperInterface::RemapPduId
   * \steady          TRUE
   */
  void RemapPduId(someip_protocol::internal::PduId& pdu_id) noexcept final;

  /*!
   * \copydoc         connection_manager::pdu_id_mappers::PduIdReceiveRemapperInterface::CheckPduId
   * \steady          TRUE
   */
  bool CheckPduId(someip_protocol::internal::PduId pdu_id) noexcept final;

 private:
  /*!
   * \brief           A required signal pdu remapping container
   */
  using RequiredSignalPduRemappingContainer =
      ara::core::Map<someip_protocol::internal::PduId, someip_protocol::internal::PduId>;

  /*!
   * \brief           A container of registered required signal serialized pdu remappings
   */
  RequiredSignalPduRemappingContainer required_signal_pdu_remappings_;

  /*!
   * \brief           Friend test to access the private member required_signal_pdu_remappings_
   */
  FRIEND_TEST(UT__SomeIpd__ConnectionManager__PduIdReceiveRemapper, PduIdReceiveRemapper_ValidateMappings);
};

}  // namespace pdu_id_mappers
}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_PDU_ID_MAPPERS_PDU_ID_RECEIVE_REMAPPER_H_
