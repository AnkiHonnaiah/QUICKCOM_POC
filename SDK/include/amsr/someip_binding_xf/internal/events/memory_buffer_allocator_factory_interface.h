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
/*!        \file  someip_binding_xf/internal/events/memory_buffer_allocator_factory_interface.h
 *        \brief  Interface for a factory for MemoryBufferAllocators.
 *        \unit   SomeIpBinding::SomeIpBindingXf::Communication::Events::SkeletonEventXf
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_MEMORY_BUFFER_ALLOCATOR_FACTORY_INTERFACE_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_MEMORY_BUFFER_ALLOCATOR_FACTORY_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/someip_binding_core/internal/types.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace events {

/*!
 * \brief Factory interface providing functionality to create several memory buffer allocators.
 */
class MemoryBufferAllocatorFactoryInterface {
 public:
  /*!
   * \brief Type alias for MemoryBufferAllocator
   */
  using MemoryBufferAllocator = amsr::someip_binding_core::internal::MemoryBufferAllocator;

  /*!
   * \brief Constructor.
   * \pre     -
   * \context App
   * \spec
   *   requires true;
   * \endspec
   */
  MemoryBufferAllocatorFactoryInterface() noexcept = default;

  /*!
   * \brief Virtual destructor
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~MemoryBufferAllocatorFactoryInterface() = default;

  MemoryBufferAllocatorFactoryInterface(MemoryBufferAllocatorFactoryInterface const&) = delete;
  MemoryBufferAllocatorFactoryInterface(MemoryBufferAllocatorFactoryInterface&&) = delete;
  MemoryBufferAllocatorFactoryInterface& operator=(MemoryBufferAllocatorFactoryInterface const&) & = delete;
  MemoryBufferAllocatorFactoryInterface& operator=(MemoryBufferAllocatorFactoryInterface&&) & = delete;

  /*!
   * \brief Create an allocator that dynamically allocates memory.
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
  virtual std::unique_ptr<MemoryBufferAllocator> CreateAllocator() const noexcept = 0;

  /*!
   * \brief Create a deterministic allocator.
   *
   * \param[in] elements_count Number of samples.
   * \param[in] element_size   Maximum size of a single sample.
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
  virtual std::unique_ptr<MemoryBufferAllocator> CreateAllocator(std::size_t const elements_count,
                                                                 std::size_t const element_size) const noexcept = 0;
};

}  // namespace events
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_MEMORY_BUFFER_ALLOCATOR_FACTORY_INTERFACE_H_
