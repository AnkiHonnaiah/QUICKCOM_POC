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
/*!        \file  amsr/log/logging.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_LOGGING_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_LOGGING_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"  // IWYU pragma: keep
#include "amsr/log/i_log_sink_base.h"
#include "amsr/log/logger.h"  // IWYU pragma: export
#include "amsr/log_fwd.h"     // IWYU pragma: export

namespace amsr {
namespace log {

/*!
 * \brief Creates a Logger providing the means to send log messages.
 *
 * \param ctx_id Context ID
 * \param ctx_description Context description
 * \param ctx_def_log_level Default log level
 *
 * \return Logger&        Reference to logger
 *
 * \warning The returned logger will become invalid during calls to core::Deinitialize.
 * \pre Context ID must consist of 4 or less characters
 *
 * \trace DSGN-LogAndTrace-MessageGroups
 *
 * \pre         -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe  TRUE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \context     ANY
 *
 * \vpublic
 */
auto CreateLogger(core::StringView ctx_id, core::StringView ctx_description, LogLevel ctx_def_log_level) noexcept
    -> Logger&;

/*!
 * \brief Creates a Logger providing the means to send log messages.
 *
 * \param ctx_id          Context ID
 * \param ctx_description Context description
 *
 * \return Logger&        Reference to logger
 *
 * \warning The returned logger will become invalid during calls to core::Deinitialize.
 * \pre Context ID must consist of 4 or less characters
 *
 * \trace DSGN-LogAndTrace-MessageGroups
 *
 * \pre         -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe  TRUE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \context     ANY
 *
 * \vpublic
 */
auto CreateLogger(core::StringView ctx_id, core::StringView ctx_description) noexcept -> Logger&;

namespace internal {
/*!
 * \brief Register log sink for logging messages
 * \details Register should be usually used in the single threaded initialization phase.
 *
 * \note If this API is used then UnregisterLogSink must be called prior to the sink's destructor is called.
 *
 * \param log_sink Sink to register
 * \return         The positive result if successful, otherwise an error.
 * \spec
 *   requires true;
 * \endspec
 *
 * \threadsafe FALSE
 *
 *
 * \vprivate Vector component internal.
 */
auto RegisterNewLogSink(ILogSinkBase& log_sink) noexcept -> core::Result<void>;

/*!
 * \brief Unregister log sink for logging messages
 * \details Unregister should be usually used in the single threaded shutdown phase.
 *
 * \note This API must be called prior to the sink's destructor is called to avoid undefined behaviour.
 *
 * \param log_sink Pointer to log sink to unregister
 * \return         The positive result if successful, otherwise an error.
 * \spec
 *   requires true;
 * \endspec
 *
 * \threadsafe FALSE
 *
 *
 * \vprivate Vector component internal.
 */
auto UnregisterLogSink(ILogSinkBase const& log_sink) noexcept -> core::Result<void>;
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_LOGGING_H_
