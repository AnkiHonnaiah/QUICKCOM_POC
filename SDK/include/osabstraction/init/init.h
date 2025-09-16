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
/**        \file  init.h
 *        \brief  OS specific initialization functions
 *        \unit   osabstraction::LifecycleAPI
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_INIT_INIT_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_INIT_INIT_H_

namespace osabstraction {
namespace init {

/*!
 * \brief       Initialize file system on operating systems where it is necessary.
 *
 * \details     This function does nothing and always returns success on operating systems where no initialization steps
 *              are necessary.
 *
 * \return      true if the initialization succeeded, false otherwise.
 *
 * \context     Execution Manager initialization
 *
 * \pre         -
 *
 * \reentrant   FALSE
 * \synchronous TRUE
 * \threadsafe  FALSE
 * \steady      TRUE
 *
 * \trace       DSGN-Osab-InitializeFileSystem
 *
 * \vprivate    Vector product internal API
 * \spec
 *   requires true;
 * \endspec
 *
 * \trace       DSGN-Osab-InitializeFileSystemPikeOS
 */
bool InitializeFileSystem() noexcept;

}  // namespace init
}  // namespace osabstraction

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_INIT_INIT_H_
