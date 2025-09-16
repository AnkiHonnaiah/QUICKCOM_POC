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
/*!        \file  log_sink_register.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_LOG_SINK_REGISTER_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_LOG_SINK_REGISTER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/log/i_log_sink_base.h"
#include "amsr/log/internal/array_vec.h"
#include "amsr/log/internal/config/compile_time_config.h"
#include "amsr/log/internal/reactor_fwd.h"  // IWYU pragma: keep
#include "amsr/log/internal/ref.h"
#include "amsr/log/internal/sink/i_log_sink.h"
#include "amsr/log/internal/utils.h"

namespace amsr {
namespace log {
namespace internal {
namespace config {
/*! \brief Forward declaration of LogSinks. */
class LogSinks;

/*! \brief The collection of all sinks. */
using Sinks = ArrayVec<internal::Ref<ILogSinkBase>, config::kMaxNumberOfLogSinks>;

/*!
 * \brief  Holds the LogSinks singleton.
 * \return The reference to the singleton.
 * \spec
 *   requires true;
 * \endspec
 */
auto GetRawSinks() noexcept -> Optional<LogSinks>&;

/*!
 * \brief         Registers configured log sinks.
 * \param sinks   Reference to the sink storage.
 * \param reactor The borrowed reactor to pass.
 * \return        The empty Result, or the error that occurred.
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate Component private.
 */
auto RegisterLogSinks(Sinks& sinks, BorrowedReactor reactor) noexcept -> core::Result<void>;

/*!
 * \brief Unregisters configured log sinks.
 * \spec
 *   requires true;
 * \endspec
 */
void UnregisterLogSinks() noexcept;
}  // namespace config
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_LOG_SINK_REGISTER_H_
