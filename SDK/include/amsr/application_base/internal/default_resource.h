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
/**     \file       default_resource.h
 *      \brief      The default memory resource.
 *
 *********************************************************************************************************************/
#ifndef LIB_APPLICATION_BASE_INCLUDE_AMSR_APPLICATION_BASE_INTERNAL_DEFAULT_RESOURCE_H_
#define LIB_APPLICATION_BASE_INCLUDE_AMSR_APPLICATION_BASE_INTERNAL_DEFAULT_RESOURCE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/abort_on_error_resource.h"
#include "amsr/application_base/internal/new_delete_buffer.h"
#include "ara/core/memory_resource.h"
#include "ara/core/synchronized_pool_resource.h"
#include "vac/testing/test_adapter.h"

namespace amsr {
namespace application_base {
namespace internal {

/*!
 * \brief   Class that represents the DefaultResource.
 *
 * \details The default memory resource is a SynchronizedPoolResource with a MonotonicBufferResource as uptream
 *          resource, which in turn has a AbortOnErrorResource as upstream resource. The final upstream resource,
 *          used by the AbortOnErrorResource, is either a NewDeleteResource or NullMemoryResource
 *          depending on configuration.
 */
// VCA_APPLICATIONBASE_UNDEFINED_FUNCTION_NO_POINTER_PARAMETER_FALSE_POSITIVE
class DefaultResource final {
  /*!
   * \brief The pre-allocated buffer.
   */
  NewDeleteBuffer buffer_;

  /*!
   * \brief The aborting upstream resource.
   */
  AbortOnErrorResource aborting_resource_;

  /*!
   * \brief The monotonic upstream resource.
   */
  ara::core::MonotonicBufferResource monotonic_resource_;

  /*!
   * \brief The default memory resource for amsr containers.
   */
  ara::core::SynchronizedPoolResource default_resource_;

 public:
  /*!
   * \brief   Constructor.
   *
   * \details Provides a pre-allocated buffer of the given size.
   *          If expanding is 'false' the amsr::internal::NullMemoryResource is used,
   *          otherwise amsr::internal::NewDeleteResource is used as the final upstream resource.
   *
   * \param   size      The size of pre-allocated memory.
   * \param   expanding Flag if the memory resource is allowed to grow.
   *
   */
  explicit DefaultResource(std::size_t size, bool expanding) noexcept
      : buffer_(size),
        aborting_resource_{expanding ? ara::core::NewDeleteResource() : ara::core::NullMemoryResource()},
        // VCA_APPLICATIONBASE_UNDEFINED_FUNCTION_POINTER_ARGUMENT_VALID
        monotonic_resource_{buffer_.AsPtr(), size, &aborting_resource_},
        // VCA_APPLICATIONBASE_UNDEFINED_FUNCTION_POINTER_ARGUMENT_VALID
        default_resource_{&monotonic_resource_} {}

  /*!
   * \brief Get the memory resource to be used by a Polymorphic Allocator.
   */
  auto AsMemoryResource() noexcept -> ara::core::SynchronizedPoolResource* { return &default_resource_; }

  /*!
   * \brief Returns the amount of used memory by the MonotonicBufferResource.
   */
  auto Size() const noexcept -> std::size_t {
    // VCA_APPLICATIONBASE_UNDEFINED_FUNCTION_NO_POINTER_PARAMETER_FALSE_POSITIVE
    return monotonic_resource_.Size();
  }

 private:
  /*! \brief Friend test declaration. */
  FRIEND_TEST(UT__DefaultResource, Constructor);
  /*! \brief Friend test declaration. */
  FRIEND_TEST(UT__DefaultResource, AsMemoryResource);
  /*! \brief Friend test declaration. */
  FRIEND_TEST(UT__DefaultResource, Size);
  /*! \brief Friend test declaration. */
  FRIEND_TEST(UT__DefaultResource, CorrectUpstream);
};

}  // namespace internal
}  // namespace application_base
}  // namespace amsr

#endif  // LIB_APPLICATION_BASE_INCLUDE_AMSR_APPLICATION_BASE_INTERNAL_DEFAULT_RESOURCE_H_