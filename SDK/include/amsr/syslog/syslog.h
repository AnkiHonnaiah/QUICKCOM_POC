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
/*!       \file  syslog.h
 *        \brief  System logger.
 *        \unit   osabstraction::syslog
 *
 *********************************************************************************************************************/

#ifndef LIB_SYSLOG_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_SYSLOG_SYSLOG_H_
#define LIB_SYSLOG_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_SYSLOG_SYSLOG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/core/string.h"
#include "amsr/syslog/types.h"
#include "vac/container/c_string_view.h"

namespace amsr {
namespace syslog {

/*!
 * \brief   System logger abstraction
 * \details Implements functionality to log messages to system log.
 */
class SystemLogger final {
 public:
  /*!
   * \brief           Construct a SystemLogger object.
   * \context         SystemLogger constructor
   * \pre             -
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   * \vprivate        Vector product internal API
   */
  SystemLogger() noexcept = default;

  /*!
   * \brief           Opens a connection to the system logger.
   *
   * \param[in]       settings        Settings to open system logger.
   *
   * \return          Result if connection to system logger was successful.
   *
   * \error           osabstraction::OsabErrc::kSystemEnvironmentError       System logging is not available on the
   *                  target environment.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-Syslog
   *
   * \vprivate        Product private
   */
  ::amsr::core::Result<void> Open(SyslogSettings const& settings) noexcept;

  /*!
   * \brief           Writes a log message to the system logger.
   *
   * \details         Messages are logged according to settings of Open().
   *                  The usage of Open() is optional however recommended. Write() will automatically establish a
   *                  connection with the system logger if necessary with following default settings:
   *                  - identifier: Behavior not specified, may be the name of the executable
   *                  - options: Opening of the connection to the system logger is delayed until Write() is called
   *                  - facility: kUser
   *
   * \param[in]       log_level        Log level.
   * \param[in]       message          Formatted message to log.
   *
   *
   * \context         ANY
   *
   * \pre             Target platform supports system logging.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \trace           DSGN-Osab-Syslog
   *
   * \vprivate        Product private
   */
  static void Write(LogPriority log_level, vac::container::CStringView message) noexcept;

  /*!
   * \brief           Destructor
   * \details         Closes connection to the system logger.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \trace           DSGN-Osab-Syslog
   *
   * \vprivate        Product private
   */
  ~SystemLogger() noexcept;

  /*!
   * \brief           Copy-construction not supported as copying a system logger is not meaningful.
   */
  SystemLogger(SystemLogger const&) noexcept = delete;

  /*!
   * \brief           Copy-assignment not supported as copying a system logger is not meaningful.
   */
  SystemLogger& operator=(SystemLogger const&) & noexcept = delete;

  /*!
   * \brief           Move-construction not supported.
   */
  SystemLogger(SystemLogger&&) noexcept = delete;

  /*!
   * \brief           Move-assignment not supported.
   */
  SystemLogger& operator=(SystemLogger&&) & noexcept = delete;

 private:
  /*!
   * \brief           If set, Identifier which is prepended on system log write operation.
   */
  ::amsr::core::String identifier_;
};

}  // namespace syslog
}  // namespace amsr

#endif  // LIB_SYSLOG_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_SYSLOG_SYSLOG_H_
