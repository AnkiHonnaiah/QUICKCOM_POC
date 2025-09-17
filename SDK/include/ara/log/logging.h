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
/**     \file       ara/log/logging.h
 *      \brief
 *      \details
 *
 *********************************************************************************************************************/
#ifndef LIB_ARA_LOGGING_INCLUDE_ARA_LOG_LOGGING_H_
#define LIB_ARA_LOGGING_INCLUDE_ARA_LOG_LOGGING_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>
#include "amsr/core/string_view.h"  // IWYU pragma: keep
#include "amsr/log/logging.h"       // IWYU pragma: export
#include "ara/log/common.h"
#include "ara/log/logger.h"  // IWYU pragma: export

namespace ara {
namespace log {
/*!
 * \brief Creates a Logger providing the means to send log messages.
 *
 * \param ctx_id Context ID
 * \param ctx_description Context description
 * \param ctx_def_log_level Default log level
 *
 * \return Logger& reference to logger
 * \pre Context ID must consist of 4 or less characters
 *
 * \details As defined in SWS_LOG_00021, SWS_LOG_00005, SWS_LOG_00006
 * \trace DSGN-LogAndTrace-MessageGroups
 *
 * \spec
 *   requires true;
 * \endspec
 *
 * \vpublic
 */
auto CreateLogger(core::StringView ctx_id, core::StringView ctx_description, LogLevel ctx_def_log_level) noexcept
    -> Logger&;

/*!
 * \brief Creates a Logger providing the means to send log messages.
 *
 * \param ctx_id Context ID
 * \param ctx_description Context description
 *
 * \return Logger& reference to logger
 * \pre Context ID must consist of 4 or less characters
 *
 * \details As defined in SWS_LOG_00021, SWS_LOG_00005
 * \trace DSGN-LogAndTrace-MessageGroups
 *
 * \spec
 *   requires true;
 * \endspec
 *
 * \vpublic
 */
auto CreateLogger(core::StringView ctx_id, core::StringView ctx_description) noexcept -> Logger&;
}  // namespace log
}  // namespace ara

#endif  // LIB_ARA_LOGGING_INCLUDE_ARA_LOG_LOGGING_H_
