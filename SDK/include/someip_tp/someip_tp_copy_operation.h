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
/*!        \file  someip_tp_copy_operation.h
 *        \brief Abstracted copy operation.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_COPY_OPERATION_H_
#define LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_COPY_OPERATION_H_

#include <cassert>
#include "ara/core/span.h"
#include "vac/memory/memory_buffer.h"

namespace amsr {
namespace someip_tp {

/*!
 * \brief A copy operation abstracting the source.
 *
 * The intent is to wrap copies from non-contiguous buffers. Such contiguous
 * buffer might provide a copy operation that does the copy of each segment with
 * "N" memcpy (equivalent) calls, "N" being the number of segments.
 *
 * Such buffer abstractions might also provide an iterator, but using an
 * "std::copy" over such iterator might lead to segmented buffer bounds checking
 * on every byte, that's the reason for this class to exist.
 *
 * Every specialization of "CopyOperation" has to provide member functions,
 * "Size" and "operator()". See CopyOperation<::vac::memory::MemoryBuffer<U>>
 * for more details on its signatures.
 *
 * \tparam T underlying type of the copy operation.
 */
template <class T>
class CopyOperation;

/*!
 * \brief A copy operation on a ::vac::memory::MemoryBuffer
 *
 * \tparam BufferType the type of the ::vac::memory::MemoryBuffer
 */
template <class IovecType>
class CopyOperation<::vac::memory::MemoryBuffer<IovecType>> final {
 public:
  /*!
   * \brief     Construct a new Copy Operation object
   * \param[in] src The source of the data.
   * \param     offset The offset at which the data will start to be copied.
   * \param     size The number of elements to copy.
   * \steady    TRUE
   */
  CopyOperation(::vac::memory::MemoryBuffer<IovecType> const& src, std::size_t const offset,
                std::size_t const size) noexcept
      : src_{&src}, offset_{offset}, size_{size} {};

  /*!
   * \brief       Executes the copy operation
   * \param[out]  dst The destination memory. dst has to contain at least "Size" bytes.
   * steady       TRUE
   */
  void operator()(ara::core::Span<std::uint8_t> const dst) const noexcept {
    assert(dst.size() >= size_);
    static_cast<void>(src_->CopyOut(offset_, size_, dst.data()));
  }

  /*!
   * \brief   Gets the size of the operation
   * \return  std::size_t Number of bytes that calling operator() will copy.
   * steady   TRUE
   */
  std::size_t Size() const noexcept { return size_; }

  /*!
   * \brief   Delete default constructor
   * \steady  FALSE
   */
  CopyOperation() = delete;

  /*!
   * \brief   Default copy constructor
   * \steady  FALSE
   */
  CopyOperation(CopyOperation const&) noexcept = default;

  /*!
   * \brief   Default move constructor
   * \steady  FALSE
   */
  CopyOperation(CopyOperation&& other) noexcept = default;

  /*!
   * \brief   Default copy assignment
   * \steady  FALSE
   */
  CopyOperation& operator=(CopyOperation const&) & noexcept = default;

  /*!
   * \brief   Default move assignment
   * \steady  TRUE
   */
  CopyOperation& operator=(CopyOperation&&) & noexcept = default;

  /*!
   * \brief   Default destructor
   * \steady  FALSE
   */
  ~CopyOperation() = default;

 private:
  ::vac::memory::MemoryBuffer<IovecType> const* src_;
  std::size_t offset_;
  std::size_t size_;
  /*!
   * \endcond
   */
};

/*!
 * \brief     Helper to create a CopyOperation, as C++14 doesn't have User-defined deduction guides.
 * \tparam    IovecType U the type of the ::vac::memory::MemoryBuffer
 * \param[in] src     The source of the data.
 * \param     offset  The offset at which the data will start to be copied.
 * \param     size    The number of elements to copy.
 * \return    CopyOperation<::vac::memory::MemoryBuffer<U>>
 * \steady    TRUE
 */
template <class IovecType>
inline auto MakeCopyOperation(::vac::memory::MemoryBuffer<IovecType> const& src, std::size_t const offset,
                              std::size_t const size) noexcept
    -> CopyOperation<::vac::memory::MemoryBuffer<IovecType>> {
  return {src, offset, size};
}

}  // namespace someip_tp
}  // namespace amsr

#endif  // LIB_SOMEIP_TP_INCLUDE_SOMEIP_TP_SOMEIP_TP_COPY_OPERATION_H_
