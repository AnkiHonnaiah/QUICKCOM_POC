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
/**        \file  memory_utilities.h
 *        \brief  Memory utility unit to abstract shared pointer creation
 *
 *      \details  -
 *
 **********************************************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_MEMORY_MEMORY_UTILITIES_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_MEMORY_MEMORY_UTILITIES_H_

/***********************************************************************************************************************************************
 *  INCLUDES
 ***********************************************************************************************************************************************/
#include <memory>
#include "amsr/unique_ptr.h"
#include "ara/core/memory_resource.h"
#include "ara/core/result.h"

namespace amsr {
namespace someip_daemon_core {
namespace memory {

/*!
 * \brief Utility wrapper to abstract creation of smart pointers using a Polymorphic Allocator with a defaulted
 *         upstream resource.
 */
class MemoryUtilities final {
 public:
  /*!
   * \brief Creates shared pointer using a PolymorphicAllocator with a defaulted upstream resource
   * \details In case of lack of available memory, the default memory resource will abort.
   *
   * \tparam T  Typename of the object to be managed by the shared pointer.
   * \tparam Ts Parameter list for constructing the value type.
   *
   * \param[in] args Argument list for constructing the value type.
   *
   * \return A shared pointer to a new instance of value type
   */
  template <typename T, typename... Args>
  static auto CreateSharedPtr(Args&&... args) noexcept -> std::shared_ptr<T> {
    return std::allocate_shared<T>(ara::core::PolymorphicAllocator<T>{}, std::forward<Args>(args)...);
  }

  /*!
   * \brief Creates unique pointer using a PolymorphicAllocator with a defaulted upstream resource
   * \details In case of lack of available memory, the default memory resource will abort.
   *
   * \tparam T  Typename of the object to be managed by the unique pointer.
   * \tparam Ts Parameter list for constructing the value type.
   *
   * \param[in] args Argument list for constructing the value type.
   *
   * \return A unique pointer to a new instance of value type
   */
  template <typename T, typename... Args>
  static auto CreateUniquePtr(Args&&... args) noexcept -> amsr::UniquePtr<T> {
    ara::core::Result<amsr::UniquePtr<T>> result{
        amsr::AllocateUnique<T>(ara::core::PolymorphicAllocator<T>{}, std::forward<Args>(args)...)};

    assert(result.HasValue());          // Error handling is done by the default memory resource.
    assert(result.Value() != nullptr);  // Hence, the returned pointer shall always be valid.
    return std::move(result.Value());
  }
};

}  // namespace memory
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_MEMORY_MEMORY_UTILITIES_H_
