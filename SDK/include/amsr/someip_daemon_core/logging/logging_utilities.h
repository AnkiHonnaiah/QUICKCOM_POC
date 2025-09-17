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
/*!        \file  logging_utilities.h
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_LOGGING_LOGGING_UTILITIES_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_LOGGING_LOGGING_UTILITIES_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace logging {

/*!
 * \brief Logging Utilities.
 * \details Helper class is for logging
 */
class LoggingUtilities final {
 public:
  /*!
   * \brief  Helper function to remove the MSB of the event Id and append it to the log stream.
   *
   * \param[in] stream   The log stream.
   * \param[in] event_id The event id.
   *
   * \return -
   *
   * \context   Any
   * \reentrant FALSE
   * \steady    TRUE
   *
   * \trace SPEC-4981307
   */
  static void LogEventId(ara::log::LogStream& stream, someip_protocol::internal::EventId const event_id) noexcept {
    stream << ara::log::HexFormat(static_cast<someip_protocol::internal::EventId>(event_id & 0x7FFFU));
  }
};
}  // namespace logging
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_LOGGING_LOGGING_UTILITIES_H_
