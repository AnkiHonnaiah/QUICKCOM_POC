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
/*!        \file  ara/com/runtime.h
 *        \brief  contains vpublic APIs for runtime
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_ARA_COM_RUNTIME_H_
#define LIB_SOCAL_INCLUDE_ARA_COM_RUNTIME_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/com/instance_identifier.h"
#include "ara/core/instance_specifier.h"

namespace ara {
namespace com {
namespace runtime {

/*!
 * \brief Trigger handling of pending reactor events (received or transmitted packets, expired timers).
 * \return True if any packet (timer handling or any rx/tx task) has been processed, false otherwise.
 * \pre         Runtime processing mode must be configured to 'kPolling'.
 * \context     App | Init.
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \vpublic
 * \steady TRUE
 * \spec requires true; \endspec
 *
 * \unit Socal::Runtime::Runtime
 */
bool ProcessPolling() noexcept;

/*!
 * \brief Returns all instance identifiers which match the provided instance specifier.
 * \param[in] instance  The provided instance specifier which is used to look up the corresponding service instance
 *                      identifiers.
 * \return A container of matching instance identifiers, which could also be empty.
 * \pre         -
 * \context     App
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \vpublic
 * \steady FALSE
 * \spec requires true; \endspec
 *
 * \unit Socal::Runtime::Runtime
 */
::ara::com::InstanceIdentifierContainer ResolveInstanceIDs(::ara::core::InstanceSpecifier instance) noexcept;

}  // namespace runtime
}  // namespace com
}  // namespace ara

#endif  // LIB_SOCAL_INCLUDE_ARA_COM_RUNTIME_H_
