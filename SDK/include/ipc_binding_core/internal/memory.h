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
 *        \brief  MakeUnique implementation.
 *         \unit  IpcBinding::IpcBindingCore::CoreUtility
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_MEMORY_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_MEMORY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/unique_ptr.h"
#include "ara/core/memory_resource.h"
#include "ara/core/result.h"

namespace amsr {
namespace tmp {

/*!
 * \brief   Makes an UniquePtr.
 * \details Forwards to AllocateUnique. Temporary function until libvac provides an implementation.
 *
 * \tparam T     The type.
 * \tparam Args  The argument types.
 *
 * \param[in] args  The arguments.
 *
 * \return Result with an UniquePtr.
 *
 * \pre         -
 * \context     ANY
 * \threadsafe  TRUE
 * \reentrant   TRUE
 * \synchronous TRUE
 *
 * \spec requires true; \endspec
 */
template <typename T, typename... Args>
auto MakeUnique(Args&&... args) noexcept -> ara::core::Result<UniquePtr<T>> {
  using Alloc = typename ara::core::PolymorphicAllocator<T>;
  // VCA_IPCB_VALID_REFERENCE
  return AllocateUnique<T, Alloc>(Alloc{}, std::forward<Args>(args)...);
}  // COV_IpcBinding_FunctionCoverage_HUN_4637

}  // namespace tmp
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_MEMORY_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_IpcBinding_FunctionCoverage_HUN_4637
//   \ACCEPT  XX
//   \REASON  This is a temporary fuction provided which would be removed with HUN-4637.
//            The function is tested implicitly with the calling units.
// COV_JUSTIFICATION_END
