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
/*!        \file someip_protocol_log_builder.h
 *        \brief Utilities for logging fatal errors.
 *        \unit SomeIpProtocol::Logging
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_PROTOCOL_LOGGING_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_LOGGING_SOMEIP_PROTOCOL_LOG_BUILDER_H_
#define LIB_SOMEIP_PROTOCOL_LOGGING_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_LOGGING_SOMEIP_PROTOCOL_LOG_BUILDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/string_view.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace logging {

/*!
 * \brief Helper class for Ipc-Binding values logging.
 */
class SomeipProtocolLogBuilder {
 public:
  // VECTOR NC VectorC++-V3.9.2: MD_SOMEIPPROTOCOL_VectorC++-V3.9.2_UsingStrongTypes
  /*!
   * \brief Writes a fatal log message and then abort the process
   * \param[in] log_message Log message (const char pointer)
   * \param[in] location The location/origin of the log message.
   * \context Any
   * \pre -
   * \reentrant FALSE
   * \vprivate Vector API Component private
   */
  static void LogFatalAndAbort(ara::core::StringView const log_message, ara::core::StringView const location) noexcept;
};

}  // namespace logging
}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_PROTOCOL_LOGGING_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_LOGGING_SOMEIP_PROTOCOL_LOG_BUILDER_H_
