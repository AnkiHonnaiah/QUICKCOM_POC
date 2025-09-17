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
 *        \brief  Provides reusable helper functions used in the connection manager.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_HELPER_FUNCTIONS_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_HELPER_FUNCTIONS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/abort.h"

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {

/*!
 * \brief           Verifies that a pointer isn't null, aborting the process otherwise
 *
 * \details         This function can be useful in constructor member initializer lists to enforce preconditions for
 *                  non-nullable pointers.
 * \tparam          PointerType
 *                  A pointer-like type.
 * \param[in]       ptr
 *                  The pointer to check.
 * \param[in]       error_message
 *                  The abort message used if the pointer is null.
 * \return          The provided pointer.
 *
 * \context         ANY
 * \pre             The provided pointer should not be null.
 * \reentrant       FALSE
 * \steady          TRUE
 */
template <typename PointerType>
auto RequireNotNull(PointerType ptr, char const* error_message) noexcept -> PointerType {
  if (ptr == nullptr) {
    ara::core::Abort((error_message != nullptr) ? error_message : "Unexpected null pointer");
  }

  return ptr;
}

}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_HELPER_FUNCTIONS_H_
