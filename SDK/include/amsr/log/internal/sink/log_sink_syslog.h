/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file
 *        \brief  Provides an implementation for logging via the syslog daemon.
 *
 *      \details
 *
 *         \unit  LogAndTrace::LogSinks::LogSinkSysLog
 *
 *********************************************************************************************************************/

#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_SINK_LOG_SINK_SYSLOG_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_SINK_LOG_SINK_SYSLOG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/log/internal/dlt_id_conversion.h"
#include "amsr/log/internal/sink/i_log_sink.h"
#include "amsr/log/internal/types/types_fwd.h"  // IWYU pragma: keep
#include "amsr/log_fwd.h"                       // IWYU pragma: keep
#include "amsr/syslog/syslog.h"

namespace amsr {
namespace log {
namespace internal {
namespace sink {

// VECTOR NC Compiler-#611: MD_LOG_Compiler-#611_no_total_override_needed_for_impl
/*!
 * \brief SysLog stream sink of logging
 *
 * \details Console sink sends log messages into syslog
 *
 * \trace DSGN-LogAndTrace-Sinks
 *
 * \vprivate Component private.
 */
class LogSinkSysLog final : public ILogSink {
 public:
  /*!
   * \brief Default constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  LogSinkSysLog() noexcept = default;
  /*!
   * \brief Destructor
   * \spec
   *   requires true;
   * \endspec
   */
  ~LogSinkSysLog() noexcept final = default;
  /*! \brief Inherit constructors. */
  using ILogSink::ILogSink;

  // Delete copy/move constructor
  LogSinkSysLog(LogSinkSysLog const&) = delete;
  LogSinkSysLog(LogSinkSysLog&&) = delete;
  auto operator=(LogSinkSysLog&&) -> LogSinkSysLog& = delete;
  auto operator=(LogSinkSysLog const&) -> LogSinkSysLog& = delete;

  /*! \copydoc amsr::log::internal::sink::ILogSink::OnInitializeComponent() */
  auto OnInitializeComponent(core::String const app_id, core::String const app_desc) noexcept
      -> core::Result<void> final;

  /*! \copydoc amsr::log::internal::sink::ILogSink::OnDeinitializeComponent() */
  auto OnDeinitializeComponent() noexcept -> core::Result<void> final;

  /*! \copydoc amsr::log::internal::sink::ILogSink::OnLog() */
  auto OnLog(MessageMetaData meta_data, Message message) noexcept -> core::Result<void> final;

  /*! \copydoc amsr::log::internal::sink::ILogSink::Name() */
  auto Name() const noexcept -> core::StringView final { return kSysLogSink; }

 private:
  /*! \brief The object used to write to syslog. */
  syslog::SystemLogger logger_;
  /*! \brief Flag to indicate if initialization has completed. */
  bool is_initialized_{false};
};
}  // namespace sink
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_SINK_LOG_SINK_SYSLOG_H_
