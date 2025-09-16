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
/**     \file       lifecycle.h
 *      \brief
 *      \details
 *
 *********************************************************************************************************************/
#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_LIFECYCLE_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_LIFECYCLE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/core/string.h"
#include "amsr/log/internal/borrowed_reactor.h"
#include "amsr/log/internal/config/compile_time_config.h"
#include "amsr/log/internal/types/types_fwd.h"  // IWYU pragma: keep
#include "vac/container/c_string_view.h"

namespace amsr {
namespace log {
namespace internal {
/*!
 * \brief Initializes the logging framework for application usage.
 *
 * \param path The path to the configuration file.
 * \returns    Nothing on success
 * \error{LogErrorDomain, LogErrc::kInitializationFailed, Could not parse logging config.}
 *
 * \note Calling any ara::log / amsr::log functions before calling this function may have unforseen consequences.
 * \warning InitializeComponent will spawn a thread.
 *
 * \code
 * int main(int argc, char* argv[])
 * {
 *   PerformSignalHandling();
 *   amsr::core::Result<void> init_result{amsr::log::internal::InitializeComponent()};
 * }
 * \endcode
 *
 * \trace DSGN-LogAndTrace-LogInitialization
 *
 * \vprivate Product private.
 */
auto InitializeComponent(core::StringView path = config::kLoggingConfigFile) noexcept -> core::Result<void>;

/*!
 * \brief Initializes the logging framework for application usage.
 *
 * \param config The configuration to be used for initialization.
 * \returns      Nothing on success
 * \error{LogErrorDomain, LogErrc::kInitializationFailed, Could not parse logging config.}
 *
 * \note Calling any ara::log / amsr::log functions before calling this function may have unforseen consequences.
 * \warning InitializeComponent will spawn a thread.
 *
 * \trace DSGN-LogAndTrace-LogInitialization
 *
 * \vprivate Component private.
 */
auto InitializeComponent(config::LoggingConfig&& config) noexcept -> core::Result<void>;

/*!
 * \brief Initializes the logging framework for application usage.
 *
 * \param reactor The reactor to use to initialize the framework
 * \param path    The path to the configuration file
 * \returns       Nothing on success
 * \error{LogErrorDomain, LogErrc::kInitializationFailed, Could not parse logging config.}
 *
 * \note Calling any ara::log / amsr::log functions before calling this function may have unforseen consequences.
 *
 * \trace DSGN-LogAndTrace-LogInitialization
 *
 * \vprivate Product private.
 */
auto InitializeComponent(internal::BorrowedReactor reactor, core::StringView path = config::kLoggingConfigFile) noexcept
    -> core::Result<void>;

/*!
 * \brief Initializes the logging framework for application usage.
 *
 * \param reactor The reactor to use to initialize the framework
 * \param config  The configuration to use.
 * \returns       Nothing on success
 * \error{LogErrorDomain, LogErrc::kInitializationFailed, Could not parse logging config.}
 *
 * \note Calling any ara::log / amsr::log functions before calling this function may have unforseen consequences.
 *
 * \trace DSGN-LogAndTrace-LogInitialization
 *
 * \vprivate Component private.
 */
auto InitializeComponent(internal::BorrowedReactor reactor, config::LoggingConfig&& config) noexcept
    -> core::Result<void>;

/*!
 * \brief Deinitializes the logging framework
 *
 * \return Nothing on success
 *
 * \pre If the component has been initialized using an own reactor, it must be stopped or unblocked before calling
 * this function or a deadlock may occur.
 * \spec
 *   requires true;
 * \endspec
 *
 * \note Calling any ara::log / amsr::log functions after calling this function may have unforeseen consequences.
 * \warning In case a custom reactor has is used to drive logging, the reactor MUST not run during this call.
 *
 * \vprivate Product private.
 */
auto DeinitializeComponent() noexcept -> core::Result<void>;

/*!
 * \brief Gets the initialization state of logging.
 *
 * \return True in case the component is initialized and ready for use.
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate Product private.
 */
auto IsComponentInitialized() noexcept -> bool;
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_LIFECYCLE_H_
