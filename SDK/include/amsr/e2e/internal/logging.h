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
 *        \brief  Logging class.
 *
 *      \details  Contains the declarations for the Logging class.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBE2E_INCLUDE_AMSR_E2E_INTERNAL_LOGGING_H_
#define LIB_LIBE2E_INCLUDE_AMSR_E2E_INTERNAL_LOGGING_H_

#include "ara/log/common.h"
#include "ara/log/logger.h"
#include "ara/log/logstream.h"

#include "amsr/e2e/profiles/internal/profile_checker_interface.h"
#include "amsr/e2e/state_machine/check_status.h"

namespace amsr {
namespace e2e {
namespace internal {

/*!
 * \brief Helper class for logging.
 * \unit amsr::e2e::internal::Logging
 */
class Logging {
 public:
  /*!
   * \brief Create a logger instance.
   * \return Reference to logger instance.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  static ::ara::log::Logger& CreateLogger() noexcept;

  /*!
   * \brief Determine the log level for the log message of a E2E_PXXCheck() result.
   * \param[in] check_status  The value to be logged.
   * \return The log level to use.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \steady TRUE
   */
  static ::ara::log::LogLevel DetermineLogLevelFromCheckStatus(
      profiles::internal::ProfileCheckerInterface::CheckStatusType check_status) noexcept;

  /*!
   * \brief Appends a CheckStatusType to the log stream.
   * \param[in,out] s             The log stream to append to.
   * \param[in]     check_status  The value to be logged.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  static void LogCheckStatus(::ara::log::LogStream& s,
                             profiles::internal::ProfileCheckerInterface::CheckStatusType check_status) noexcept;

  /*!
   * \brief Appends a state_machine::CheckStatus to the log stream.
   * \param[in,out] s             The log stream to append to.
   * \param[in]     check_status  The value to be logged.
   * \context ANY
   * \pre -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  static void LogCheckStatus(::ara::log::LogStream& s, state_machine::CheckStatus check_status) noexcept;

  /*!
   * \brief Creates a LogStream object with the passed log_level as replacement for ara::log::Logger:WithLevel() -
   *        in case this function is not available.
   * \param[in, out] logger Create the LogStream object from this logger.
   * \param[in] log_level The desired log level for which the log stream shall be created.
   * \return The log stream with the desired level.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  static ::ara::log::LogStream CreateLogStreamWithLevel(::ara::log::Logger& logger,
                                                        ::ara::log::LogLevel log_level) noexcept;
};

}  // namespace internal
}  // namespace e2e
}  // namespace amsr

#endif  // LIB_LIBE2E_INCLUDE_AMSR_E2E_INTERNAL_LOGGING_H_
