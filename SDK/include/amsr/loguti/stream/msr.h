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
 *        \brief
 *      \details
 *
 *********************************************************************************************************************/
#ifndef LIB_LOG_UTILITY_INCLUDE_AMSR_LOGUTI_STREAM_MSR_H_
#define LIB_LOG_UTILITY_INCLUDE_AMSR_LOGUTI_STREAM_MSR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>

#include "amsr/core/result.h"
#include "amsr/loguti/stream/ns.h"
#include "ara/log/logstream.h"
#include "vac/container/c_string_view.h"

namespace AMSR_LOGUTI_LOG_NS {
namespace log {
/*!
 * \brief Appends a CStringView to the log stream.
 * \param stream The log stream to append to.
 * \param value The value to be logged.
 * \return The passed log stream.
 *
 * \context ANY
 * \pre -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe FALSE
 * \reentrant FALSE
 *
 * \vpublic
 * \unit Stream
 */
auto operator<<(ara::log::LogStream& stream, vac::container::CStringView const& value) noexcept -> ara::log::LogStream&;
}  // namespace log
}  // namespace AMSR_LOGUTI_LOG_NS
#endif  // LIB_LOG_UTILITY_INCLUDE_AMSR_LOGUTI_STREAM_MSR_H_
