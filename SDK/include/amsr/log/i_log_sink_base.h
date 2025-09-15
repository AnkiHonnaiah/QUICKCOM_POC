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
 *      \unit       LogAndTrace::LogSinks::ILogSinkBase
 *
 *********************************************************************************************************************/
#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_I_LOG_SINK_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_I_LOG_SINK_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/core/string.h"
#include "amsr/core/string_view.h"
#include "amsr/log/internal/types/message.h"
#include "amsr/log/internal/types/message_meta_data.h"

namespace amsr {
namespace log {

/*!
 * \brief The ILogSinkBase abstract class is used as a base for built-in sinks part of LogAndTrace but also
 *        for custom defined sink variants.
 * \trace DSGN-LogAndTrace-Sinks
 * \vpublic
 */
class ILogSinkBase {
 public:
  /*! \brief Delete copy constructor. */
  ILogSinkBase(ILogSinkBase const&) = delete;
  /*! \brief Delete copy assignment operator.*/
  auto operator=(ILogSinkBase const&) -> ILogSinkBase& = delete;
  /*! \brief Provide a virtual destructor. */
  virtual ~ILogSinkBase() noexcept = default;
  /*! \brief Default constructor for this interface. */
  ILogSinkBase() noexcept = default;

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
  virtual auto OnInitializeComponent(core::String const app_id, core::String const app_desc) noexcept
      -> core::Result<void> = 0;

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
  virtual auto OnDeinitializeComponent() noexcept -> core::Result<void> = 0;

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
  virtual auto OnCreateLogger(core::String const ctx_id, core::String const ctx_desc) noexcept
      -> core::Result<void> = 0;

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
  virtual auto OnLog(internal::MessageMetaData meta_data, internal::Message message) noexcept -> core::Result<void> = 0;

  /*!
   * \brief   Name of the sink.
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
  virtual auto Name() const noexcept -> core::StringView = 0;

 protected:
  /*! \brief Default move constructor. */
  ILogSinkBase(ILogSinkBase&&) noexcept = default;
  /*! \brief Default move assignment operator. */
  auto operator=(ILogSinkBase&&) & noexcept -> ILogSinkBase& = default;
};

}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_I_LOG_SINK_H_
