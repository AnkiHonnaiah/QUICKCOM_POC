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
/**     \file       to_string.h
 *      \brief
 *      \details
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_TO_STRING_H_
#define LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_TO_STRING_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/string_view.h"

#include "amsr/log/log_level.h"

namespace amsr {
namespace log {
namespace internal {
/*!
 * \brief Convert the given severity to a string representation.
 *
 * \param log_level The log level to convert to string
 * \return          Log level as string
 * \spec
 *   requires true;
 * \endspec
 * \vprivate Component private.
 */
auto GetSeverityString(LogLevel log_level) noexcept -> core::StringView;
}  // namespace internal
}  // namespace log
}  // namespace amsr
#endif  // LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_TO_STRING_H_
