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
/*!        \file  io_buffer.h
 *        \brief  Mutable and immutable buffers for IO operations.
 *        \unit   osabstraction::IOBuffer
 *
 *      \details  Theses buffers are compatible with an operating system's scatter/getter buffer implementation.
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_IO_BUFFER_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_IO_BUFFER_H_

#include <cstddef>
#include <cstdint>

#include "amsr/core/span.h"

namespace osabstraction {
namespace io {

/*!
 * \brief Mutable IO buffer
 *
 * \details
 * IO operations such as read or receive may write data to this buffer.
 *
 * The memory layout has to comply with the operating system expectations for IO scatter / gather buffers (e.g. iovec).
 *
 * \trace DSGN-Osab-MutableIOBuffer
 *
 * \vprivate Vector product internal API
 */
struct MutableIOBuffer {
  /*!
   * \brief Base pointer to a mutable buffer.
   */
  void* base_pointer;

  /*!
   * \brief Buffer's size.
   */
  std::size_t size;
};

static_assert(sizeof(MutableIOBuffer) == (sizeof(MutableIOBuffer::base_pointer) + sizeof(MutableIOBuffer::size)),
              "MutableIOBuffer struct has padding bytes and cannot be used as IO scatter / gather buffer");

/*!
 * \brief Constant IO buffer
 *
 * \details
 * It is not possible to write to this buffer. IO operation such as write or send may read from the buffer.
 *
 * The memory layout has to comply with the operating system expectations for IO scatter / gather buffers (e.g. iovec).
 *
 * \trace DSGN-Osab-ConstantIOBuffer
 *
 * \vprivate Vector product internal API
 */
struct ConstIOBuffer {
  /*!
   * \brief Base pointer to a constant buffer.
   */
  void const* base_pointer;

  /*!
   * \brief Buffer's size
   */
  std::size_t size;
};

static_assert(sizeof(ConstIOBuffer) == (sizeof(ConstIOBuffer::base_pointer) + sizeof(ConstIOBuffer::size)),
              "ConstIOBuffer struct has padding bytes and cannot be used as IO scatter / gather buffer");

/*!
 * \brief Converts the content of a mutable span of a standard layout type.
 *
 * \param[in] span           A span to a mutable array.
 *
 * \tparam    Type  The type of objects the input span points to. Restricted to standard layout, not const types.
 *
 * \return Mutable IOBuffer that points to the same memory area as the passed span.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          TRUE
 *
 * \trace           DSGN-Osab-ConceptIoBuffers
 *
 * \vprivate        Vector product internal API
 */
template <class Type,
          typename = std::enable_if_t<std::is_standard_layout<Type>::value && (!std::is_const<Type>::value)>>
auto ConvertSpanToMutableIOBuffer(::amsr::core::Span<Type> span) noexcept -> MutableIOBuffer {
  MutableIOBuffer mutable_buffer{};
  mutable_buffer.base_pointer = span.data();
  mutable_buffer.size = span.size() * sizeof(Type);
  return mutable_buffer;
}

/*!
 * \brief Converts the content of a constant span of a standard layout type.
 *
 * \param[in] span           A span to a constant array.
 *
 * \tparam    Type  The type of objects the input span points to. Restricted to standard layout types.
 *
 * \return Constant IOBuffer that points to the same memory area as the passed span.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          TRUE
 *
 * \trace           DSGN-Osab-ConceptIoBuffers
 *
 * \vprivate        Vector product internal API
 */
template <class Type, typename = std::enable_if_t<std::is_standard_layout<Type>::value>>
auto ConvertSpanToConstIOBuffer(::amsr::core::Span<Type> span) noexcept -> ConstIOBuffer {
  ConstIOBuffer const_buffer{};
  const_buffer.base_pointer = span.data();
  const_buffer.size = span.size() * sizeof(Type);
  return const_buffer;
}

/*!
 * \brief Converts a constant I/O buffer to a Span of constant bytes.
 *
 * \param[in] io_buf           I/O buffer that points to a constant memory.
 *
 * \return Span of constant bytes that point to the same memory as passed I/O buffer
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          TRUE
 *
 * \trace           DSGN-Osab-ConceptIoBuffers
 *
 * \vprivate        Vector product internal API
 */
inline auto ConvertIOBufferToSpan(ConstIOBuffer io_buf) noexcept -> ::amsr::core::Span<std::uint8_t const> {
  // VECTOR Next Construct AutosarC++17_10-M5.2.8: MD_OSA_M5.2.8_StaticCastVoidPointerToBytePointer
  return ::amsr::core::Span<std::uint8_t const>{static_cast<std::uint8_t const*>(io_buf.base_pointer), io_buf.size};
}

/*!
 * \brief Converts a mutable I/O buffer to Span of bytes.
 *
 * \param[in] io_buf           I/O buffer that points to a mutable memory segment.
 *
 * \return Span of bytes that point to the same memory as passed I/O buffer
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          TRUE
 *
 * \trace           DSGN-Osab-ConceptIoBuffers
 *
 * \vprivate        Vector product internal API
 */
inline auto ConvertIOBufferToSpan(MutableIOBuffer io_buf) noexcept -> ::amsr::core::Span<std::uint8_t> {
  // VECTOR Next Construct AutosarC++17_10-M5.2.8: MD_OSA_M5.2.8_StaticCastVoidPointerToBytePointer
  return ::amsr::core::Span<std::uint8_t>{static_cast<std::uint8_t*>(io_buf.base_pointer), io_buf.size};
}

}  // namespace io
}  // namespace osabstraction

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_IO_BUFFER_H_
