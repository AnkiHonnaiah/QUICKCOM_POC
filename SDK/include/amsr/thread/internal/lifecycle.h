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
 *        \brief  Initiliazation routines for Thread library.
 *        \unit   amsr::thread::ConfigurableThreads::ComponentLifecycleAPI
 *
 *********************************************************************************************************************/
#ifndef LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_LIFECYCLE_H_
#define LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_LIFECYCLE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"

namespace amsr {
namespace thread {
namespace internal {

/*!
 * \brief           Initialize Thread library
 *
 * \details         This function reads AMSR_THREAD_CONFIGURABLE_SETTINGS environment variable whose value
 *                  is restricted to [ON, OFF, not defined] and parse and validate etc/thread_config.json
 *                  ,which is expected in the current working directory, if the environment variable was set to ON.
 *
 * \return          Empty result if initialization is successful, error otherwise.
 *
 * \error           ThreadErrc::kThreadConfigJsonValidationFailed if integrity check is enabled
 *                  and validation of thread_config.json failed.
 * \error           ThreadErrc::kThreadConfigJsonFileOpenFailed if openeing
 *                  thread_config.json failed.
 * \error           ThreadErrc::kThreadConfigJsonFileParsingFailed if parsing
 *                  thread_config.json failed.
 * \error           ThreadErrc::kInsufficientResources if configured instances inside
 *                  thread_config.json are more than kMaxThreadConfigs.
 *
 * \context         ara::core::Initialize
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \vprivate        Vector product internal API
 *
 * \trace           DSGN-VectorThreadLibrary-ComponentInitialization
 */
amsr::core::Result<void> InitializeComponent() noexcept;

/*!
 * \brief           De-initialize the Thread library
 *
 * \error           -
 *
 * \context         ara::core::Deinitialize
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 *
 * \vprivate        Vector product internal API
 *
 * \trace           DSGN-VectorThreadLibrary-ComponentInitialization
 */
amsr::core::Result<void> DeinitializeComponent() noexcept;

/*!
 * \brief           Returns whether initialization of Thread library was successful or not.
 *
 * \return          Returns true if Thread library has been initialized, false otherwise.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       TRUE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 *
 * \vprivate        Vector product internal API
 *
 * \trace           DSGN-VectorThreadLibrary-ComponentInitialization
 */
bool IsComponentInitialized() noexcept;

}  // namespace internal
}  // namespace thread
}  // namespace amsr

#endif  // LIB_VATHREAD_INCLUDE_AMSR_THREAD_INTERNAL_LIFECYCLE_H_
