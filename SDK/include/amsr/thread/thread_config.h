/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file
 *        \brief  Getter for parsed thread configuration structure.
 *        \unit   amsr::thread::ConfigurableThreads::ThreadConfigurationAPI
 *
 *********************************************************************************************************************/
#ifndef LIB_VATHREAD_INCLUDE_AMSR_THREAD_THREAD_CONFIG_H_
#define LIB_VATHREAD_INCLUDE_AMSR_THREAD_THREAD_CONFIG_H_

#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "amsr/thread/thread_config_types.h"

namespace amsr {
namespace thread {

/*!
 * \brief             Get the parsed thread configuration for the associated thread identifier.
 * \param[in]         identifier The identifier of thread configuration.
 * \return            Returns parsed ThreadConfig if AMSR_THREAD_CONFIGURABLE_SETTINGS environment
 *                    variable was set to ON before InitializeComponent() has been called
 *                    and the passed identifier was found in the json file,
 *                    otherwise a default constructed ThreadConfig is returned.
 * \error             -
 * \context           ANY
 * \pre               The Thread component must be initialized. In general, this means that ara::core::Initialize()
 *                    has to be called before using this API.
 * \threadsafe        TRUE
 * \reentrant         FALSE
 * \synchronous       TRUE
 * \vpublic
 */
amsr::thread::ThreadConfig GetThreadConfiguration(ara::core::StringView identifier) noexcept;

}  // namespace thread
}  // namespace amsr

#endif  // LIB_VATHREAD_INCLUDE_AMSR_THREAD_THREAD_CONFIG_H_
