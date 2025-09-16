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
/*!        \file  someip_binding_xf/internal/events/memory_buffer_allocator_factory.h
 *        \brief  Factory for MemoryBufferAllocators.
 *        \unit   SomeIpBinding::SomeIpBindingXf::Communication::Memory::MemoryBufferAllocatorFactory
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_MEMORY_BUFFER_ALLOCATOR_FACTORY_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_MEMORY_BUFFER_ALLOCATOR_FACTORY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/someip_binding_core/internal/types.h"
#include "amsr/someip_binding_xf/internal/events/memory_buffer_allocator_factory_interface.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace events {

/*!
 * \brief Factory to create several memory buffer allocators.
 */
// VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
class MemoryBufferAllocatorFactory final : public MemoryBufferAllocatorFactoryInterface {
 public:
  /*!
   * \brief Typedef for the size type used in this implementation.
   */
  using size_type = std::size_t;
  /*!
   * \brief Type alias for MemoryBufferAllocator
   */
  using DeterministicMemoryBufferAllocator = amsr::someip_binding_core::internal::DeterministicMemoryBufferAllocator;
  /*!
   * \brief Dynamic memory allocator type.
   */
  using DynamicMemoryAllocator = amsr::someip_binding_core::internal::FlexibleUniqueMemoryBufferAllocator;

  /*!
   * \brief Create a dynamic allocator.
   *
   * \return Pointer to the created memory buffer allocator.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  std::unique_ptr<MemoryBufferAllocator> CreateAllocator() const noexcept final {
    return std::make_unique<DynamicMemoryAllocator>();
  }

  /*!
   * \brief Create a determinstic allocator.
   *
   * \param[in] elements_count Number of samples. Must be greater than zero.
   * \spec
   *    requires elements_count > 0;
   * \endspec
   * \param[in] element_size   Maximum size of a single sample. Must be greater than zero.
   * \spec
   *    requires element_size > 0;
   * \endspec
   *
   * \return Pointer to the created memory buffer allocator.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  std::unique_ptr<MemoryBufferAllocator> CreateAllocator(size_type const elements_count,
                                                         size_type const element_size) const noexcept final {
    std::unique_ptr<MemoryBufferAllocator> allocator{nullptr};
    if ((elements_count > 0) && (element_size > 0)) {
      // VCA_SPC_15_SOMEIPBINDING_STL_FUNCTION
      allocator = std::make_unique<DeterministicMemoryBufferAllocator>(elements_count, element_size);
    } else {
      amsr::core::Abort("MemoryBufferAllocatorFactory::CreateAllocator - Invalid parameter passed");
    }
    return allocator;
  }
};

}  // namespace events
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_MEMORY_BUFFER_ALLOCATOR_FACTORY_H_
