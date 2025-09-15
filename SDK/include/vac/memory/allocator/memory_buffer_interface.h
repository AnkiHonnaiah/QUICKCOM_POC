/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!
 *        \file  memory_buffer_interface.h
 *        \brief MemoryBuffer interface
 *        \unit  VaCommonLib::MemoryManagement::BufferProvider
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_MEMORY_ALLOCATOR_MEMORY_BUFFER_INTERFACE_H_
#define LIB_VAC_INCLUDE_VAC_MEMORY_ALLOCATOR_MEMORY_BUFFER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "amsr/core/span.h"

namespace vac {
namespace memory {
namespace allocator {

/*!
 * \brief Memory buffer interface.
 * \trace CREQ-VaCommonLib-MemoryBuffer
 */
class MemoryBuffer {
 public:
  /*!
   * \brief Default MemoryBuffer constructor.
   */
  MemoryBuffer() noexcept = default;

  /*!
   * \brief Default MemoryBuffer destructor.
   */
  virtual ~MemoryBuffer() noexcept = default;

  /*!
   * \brief Deleted MemoryBuffer copy constructor.
   */
  MemoryBuffer(MemoryBuffer const &) = delete;

  /*!
   * \brief Deleted MemoryBuffer move constructor.
   */
  MemoryBuffer(MemoryBuffer &&) = delete;

  /*!
   * \brief Deleted MemoryBuffer copy assignment.
   */
  MemoryBuffer &operator=(MemoryBuffer const &) & = delete;

  /*!
   * \brief Deleted MemoryBuffer copy assignment.
   */
  MemoryBuffer &operator=(MemoryBuffer &&) & = delete;

  /*!
   * \brief Alias for the size type used in the buffer.
   */
  using size_type = std::size_t;

  /*!
   * \brief Alias for the value type used in the buffer.
   */
  using value_type = std::uint8_t;

  /*!
   * \brief MemoryBufferView is used as a return type for pre-allocated memory in order to avoid malloc long delays.
   */
  using MemoryBufferView = amsr::core::Span<value_type>;

  /*!
   * \brief   Get view from memory buffer.
   * \param   offset The offset at which to start.
   * \pre     -
   * \return  The desired view of the memory buffer.
   * \spec
   *  requires true;
   * \endspec
   */
  virtual MemoryBufferView GetView(size_type offset) const noexcept = 0;
};

}  // namespace allocator
}  // namespace memory
}  // namespace vac

#endif  // LIB_VAC_INCLUDE_VAC_MEMORY_ALLOCATOR_MEMORY_BUFFER_INTERFACE_H_
