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
 *         \unit    LogAndTrace::LogSinks::ICustomLogSink
 *
 *********************************************************************************************************************/
#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_I_CUSTOM_LOG_SINK_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_I_CUSTOM_LOG_SINK_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/core/string.h"
#include "amsr/core/string_view.h"
#include "amsr/log/i_log_sink_base.h"

namespace amsr {
namespace log {

/*!
 * \brief The ICustomLogSink abstract class is used as a intermediate base class for custom sink variants.
 *
 * \details A sink derived from this interface is expected to provide implementations for every interface
 *          function except for Name().
 * \trace DSGN-LogAndTrace-Sinks
 * \vpublic
 */
class ICustomLogSink : public ILogSinkBase {
 public:
  /*! \brief Delete copy constructor. */
  ICustomLogSink(ICustomLogSink const&) = delete;
  /*! \brief Delete copy assignment operator.*/
  auto operator=(ICustomLogSink const&) -> ICustomLogSink& = delete;
  /*! \brief Provide a virtual destructor. */
  ~ICustomLogSink() noexcept override = default;
  /*! \brief Default constructor for this interface. */
  ICustomLogSink() noexcept = default;

  /*!
   * \brief  Function called during initialization of LogAndTrace. Can be used to prepare the sink for further usage.
   *
   * \param  app_id   The application identifier specified from the configuration.
   * \param  app_desc The application description specified from the configuration.
   * \return Positive result in case initialization of the sink succeeded, otherwise an error.
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   *
   * \vpublic
   */
  auto OnInitializeComponent(core::String const app_id, core::String const app_desc) noexcept
      -> core::Result<void> override = 0;

  /*!
   * \brief  Function called during deinitialization of LogAndTrace. Can be used to perform clean up of used resources.
   *
   * \return Positive result in case deinitialization of the sink succeeded, otherwise an error.
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   *
   * \vpublic
   */
  auto OnDeinitializeComponent() noexcept -> core::Result<void> override = 0;

  /*!
   * \brief  Function invoked when new logger instances are created.
   *
   * \param  ctx_id   The identifier for the created logger.
   * \param  ctx_desc The description for the created logger.
   * \return Positive result in case of success, otherwise an error.
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   *
   * \vpublic
   */
  auto OnCreateLogger(core::String const ctx_id, core::String const ctx_desc) noexcept
      -> core::Result<void> override = 0;

  /*!
   * \brief  Function invoked when messages are being logged by the application.
   *
   * \param  meta_data Contains support elements that can be used to construct a log message.
   * \param  message   The actual message containing native data types.
   * \return Positive result in case logging succeeded, otherwise an error.
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   *
   * \vpublic
   */
  auto OnLog(internal::MessageMetaData meta_data, internal::Message message) noexcept
      -> core::Result<void> override = 0;

  /*!
   * \brief   Name of the sink. Always returns the value "custom".
   *
   * \note This function should not be changed. The name "custom" is used to enable the sink variant during
   * initialization of LogAndTrace.
   *
   * \return The name of the sink.
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   *
   * \vpublic
   */
  auto Name() const noexcept -> core::StringView final { return internal::kCustomSink; };

 protected:
  /*! \brief Default move constructor. */
  ICustomLogSink(ICustomLogSink&&) noexcept = default;
  /*! \brief Default move assignment operator. */
  auto operator=(ICustomLogSink&&) & noexcept -> ICustomLogSink& = default;
};

}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_I_CUSTOM_LOG_SINK_H_
