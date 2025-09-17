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
 *        \brief  Declaration of initialization routines for osabstraction.
 *        \unit   osabstraction::LifecycleAPI
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_INTERNAL_LIFECYCLE_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_INTERNAL_LIFECYCLE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"

namespace osabstraction {
namespace internal {

/*!
 * \brief           Initialize osabstraction library
 *
 * \error           osabstraction::OsabErrc::*                       IPC system initialization, file system
 *                                                                   initialization or system constant verification
 *                                                                   failed. Resources may have been partially allocated
 *                                                                   but not yet freed. Do NOT retry initialization and
 *                                                                   exit program as soon as possible.
 *
 * \context         ara::core::Initialize
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          FALSE
 *
 * \vprivate        Vector product internal API
 *
 * \trace           DSGN-Osab-InitializeComponent
 */
::amsr::core::Result<void> InitializeComponent() noexcept;

/*!
 * \brief           De-initialize the osabstraction library
 *
 * \error           osabstraction::OsabErrc::*                       IPC system deinitialization failed. Resources may
 *                                                                   not have been freed or only freed partially. Exit
 *                                                                   program as soon as possible.
 *
 * \context         ara::core::Deinitialize
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          FALSE
 *
 * \vprivate        Vector product internal API
 *
 * \trace           DSGN-Osab-InitializeComponent
 */
::amsr::core::Result<void> DeinitializeComponent() noexcept;

/*!
 * \brief           Returns whether initialization of osabstraction library was successful or not.
 *
 * \return          Returns true if osabstraction library has been initialized, false otherwise.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      TRUE
 * \steady          TRUE
 *
 * \vprivate        Vector product internal API
 * \trace           DSGN-Osab-InitializeComponent
 */
bool IsComponentInitialized() noexcept;

}  // namespace internal
}  // namespace osabstraction

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_INTERNAL_LIFECYCLE_H_
