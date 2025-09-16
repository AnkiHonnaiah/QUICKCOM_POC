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
 *      \brief      Contains an interface for general log sink.
 *      \details
 *
 *      \unit       LogAndTrace::LogSinks::ILogSink
 *
 *********************************************************************************************************************/
#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_SINK_I_LOG_SINK_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_SINK_I_LOG_SINK_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/core/string.h"
#include "amsr/log/i_log_sink_base.h"
#include "amsr/log/internal/ext_fwd.h"  // IWYU pragma: keep
#include "amsr/log_fwd.h"

namespace amsr {
namespace log {
namespace internal {
namespace sink {

/*! \brief Length of IDs in DLT protocol */
static constexpr std::size_t kIdLength{4u};

/*!
 * \brief   Interface class of logger sinks.
 * \trace DSGN-LogAndTrace-Sinks
 * \vprivate Vector component internal.
 */
class ILogSink : public ILogSinkBase {
 public:
  /*! \brief Delete copy constructor. */
  ILogSink(ILogSink const&) = delete;
  /*! \brief Delete copy assignment operator.*/
  auto operator=(ILogSink const&) -> ILogSink& = delete;
  /*!
   * \brief Provide a virtual destructor.
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_LOGAPI_MOLE_1298
  ~ILogSink() noexcept override = default;

  /*! \copydoc amsr::log::ILogSinkBase::OnInitializeComponent() */
  auto OnInitializeComponent(core::String const, core::String const) noexcept -> core::Result<void> override;
  /*! \copydoc amsr::log::ILogSinkBase::OnDeinitializeComponent() */
  auto OnDeinitializeComponent() noexcept -> core::Result<void> override;
  /*! \copydoc amsr::log::ILogSinkBase::OnCreateLogger() */
  auto OnCreateLogger(core::String const, core::String const) noexcept -> core::Result<void> override;
  /*! \copydoc amsr::log::ILogSinkBase::OnLog() */
  auto OnLog(internal::MessageMetaData, internal::Message) noexcept -> core::Result<void> override;
  /*! \copydoc amsr::log::ILogSinkBase::Name() */
  auto Name() const noexcept -> core::StringView override { return core::StringView{}; };

  /*!
   * \brief   Member function to log message.
   * \details This function is intended for Vector internal usage in tests only.
   *
   * \return  Positive result in case logging is successful, otherwise an error.
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   *
   * \vprivate
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_LOG_AutosarC++17_10-M9.3.3_stable_api
  virtual auto Log(core::StringView, Logger const&, LogLevel) noexcept -> core::Result<void> {
    return core::Result<void>{};
  };

 protected:
  /*!
   * \brief Default constructor for this interface.
   * \spec
   *   requires true;
   * \endspec
   */
  ILogSink() noexcept = default;
  /*!
   * \brief Default move constructor.
   * \spec
   *   requires true;
   * \endspec
   */
  ILogSink(ILogSink&&) noexcept = default;
  /*!
   * \brief Default move assignment operator.
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator=(ILogSink&&) & noexcept -> ILogSink& = default;
};

}  // namespace sink
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_SINK_I_LOG_SINK_H_
