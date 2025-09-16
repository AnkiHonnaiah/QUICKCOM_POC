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
 *        \brief  Factory to injected into an endpoint to create a transmit remapper.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_PDU_ID_MAPPERS_PDU_ID_TRANSMIT_REMAPPER_FACTORY_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_PDU_ID_MAPPERS_PDU_ID_TRANSMIT_REMAPPER_FACTORY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/connection_manager/pdu_id_mappers/pdu_id_transmit_remapper.h"
#include "amsr/someip_daemon_core/connection_manager/pdu_id_mappers/pdu_id_transmit_remapper_interface.h"
#include "amsr/someip_daemon_core/memory/memory_utilities.h"
#include "amsr/unique_ptr.h"

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {
namespace pdu_id_mappers {

/*!
 * \brief           Factory which will be injected into endpoints to create transmit remapper
 */
class PduIdTransmitRemapperFactory {
 public:
  /*!
   * \brief           Constructor of the factory
   * \param[in]       config
   *                  The SOME/IP Daemon configuration.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \steady          TRUE
   */
  explicit PduIdTransmitRemapperFactory(configuration::Configuration const* config) noexcept : config_{config} {}

  /*!
   * \brief           Creation function for transmit remapper
   * \steady          TRUE
   */
  amsr::UniquePtr<PduIdTransmitRemapperInterface> Create(someip_protocol::internal::IpAddress const& address,
                                                         someip_protocol::internal::Port port) noexcept {
    amsr::UniquePtr<PduIdTransmitRemapperInterface> remapper{
        memory::MemoryUtilities::CreateUniquePtr<PduIdTransmitRemapper>(config_, address, port)};

    return remapper;
  }

 private:
  /*!
   * \brief           The SOME/IP Daemon configuration
   */
  configuration::Configuration const* config_;
};

}  // namespace pdu_id_mappers
}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_PDU_ID_MAPPERS_PDU_ID_TRANSMIT_REMAPPER_FACTORY_H_
