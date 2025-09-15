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
/**     \file
 *      \brief      Template implementation class of ICustomLogSink interface for custom sink extension point.
 *      \details
 *
 *      \unit       LogAndTrace::LogSinks::LogSinkCustom
 *
 *********************************************************************************************************************/
#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_LOG_SINK_CUSTOM_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_LOG_SINK_CUSTOM_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/core/string.h"
#include "amsr/core/string_view.h"
#include "amsr/log/i_custom_log_sink.h"

namespace amsr {
namespace log {

/*!
 * \brief Template implementation for the custom log sink extension point. This class shall be used to define custom
 * sink implementation.
 *
 * \details This class serves as a base class to derive from when integrating custom sinks into LogAndTrace.
 *
 * \trace DSGN-LogAndTrace-Sinks
 * \vpublic
 */
class LogSinkCustom : public ICustomLogSink {
 public:
  /*! \brief Delete copy constructor. */
  LogSinkCustom(LogSinkCustom const&) = delete;
  /*! \brief Delete copy assignment operator.*/
  auto operator=(LogSinkCustom const&) -> LogSinkCustom& = delete;
  LogSinkCustom() noexcept = default;
  // VCA_LOGAPI_MOLE_1298
  ~LogSinkCustom() noexcept override = default;

  /*! \copydoc amsr::log::ICustomLogSink::OnInitializeComponent() */
  auto OnInitializeComponent(core::String const app_id, core::String const app_desc) noexcept
      -> core::Result<void> override;
  /*! \copydoc amsr::log::ICustomLogSink::OnDeinitializeComponent() */
  auto OnDeinitializeComponent() noexcept -> core::Result<void> override;
  /*! \copydoc amsr::log::ICustomLogSink::OnCreateLogger() */
  auto OnCreateLogger(core::String const ctx_id, core::String const ctx_desc) noexcept -> core::Result<void> override;
  /*! \copydoc amsr::log::ICustomLogSink::OnLog() */
  auto OnLog(internal::MessageMetaData meta_data, internal::Message message) noexcept -> core::Result<void> override;

 protected:
  /*! \brief Default move constructor. */
  LogSinkCustom(LogSinkCustom&&) noexcept = default;
  /*! \brief Default move assignment operator. */
  auto operator=(LogSinkCustom&&) & noexcept -> LogSinkCustom& = default;
};

}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_LOG_SINK_CUSTOM_H_
