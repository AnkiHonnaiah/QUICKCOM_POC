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
/**     \file       new_delete_buffer.h
 *      \brief      Provides a heap memory allocated buffer.
 *
 *********************************************************************************************************************/
#ifndef LIB_APPLICATION_BASE_INCLUDE_AMSR_APPLICATION_BASE_INTERNAL_NEW_DELETE_BUFFER_H_
#define LIB_APPLICATION_BASE_INCLUDE_AMSR_APPLICATION_BASE_INTERNAL_NEW_DELETE_BUFFER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <new>
#include "ara/core/abort.h"

namespace amsr {
namespace application_base {
namespace internal {

/*!
 * \brief A buffer that is allocated and deallocated with operator new and operator delete.
 */
class NewDeleteBuffer final {
  /*!
   * \brief Pointer to the buffer.
   */
  void* buffer_;

 public:
  /*! \brief Delete default constructor. */
  NewDeleteBuffer() noexcept = delete;
  /*! \brief Delete copy constructor. */
  NewDeleteBuffer(NewDeleteBuffer const&) noexcept = delete;
  /*! \brief Delete copy assignment. */
  auto operator=(NewDeleteBuffer const&) noexcept -> NewDeleteBuffer& = delete;
  /*! \brief Delete move constructor. */
  NewDeleteBuffer(NewDeleteBuffer&&) noexcept = delete;
  /*! \brief Delete move assignment. */
  auto operator=(NewDeleteBuffer&&) noexcept -> NewDeleteBuffer& = delete;

  /*!
   * \brief Create a buffer with 'operator new'. Aborts if memory can not be allocated.
   *
   * \param size  Number of bytes to allocate.
   */
  explicit NewDeleteBuffer(std::size_t size) noexcept
      // VCA_APPLICATIONBASE_UNDEFINED_FUNCTION_IN_STDLIB
      : buffer_{operator new(size, std::nothrow)} {
    if (buffer_ == nullptr) {
      ara::core::Abort(
          "amsr::application_base::internal::NewDeleteBuffer: Could not allocate memory. "
          "operator new(std::size_t, std::no_throw_t const&) returned nullptr!");
    }
  }

  /*!
   * \brief Destructor. Deallocates the memory.
   */
  ~NewDeleteBuffer() {
    // VECTOR NL AutosarC++17_10-M0.3.1: MD_APP_BASE_M0.3.1_dynamicMemoryPossiblyReleasedEarlier
    operator delete(buffer_);  // VCA_APPLICATIONBASE_POSSIBLY_FREEING_STATIC_MEMORY, VCA_APPLICATIONBASE_VALID_POINTER
  }

  /*!
   * \brief Get a pointer to the buffer.
   */
  auto AsPtr() noexcept -> void* { return buffer_; }
};

}  // namespace internal
}  // namespace application_base
}  // namespace amsr

#endif  // LIB_APPLICATION_BASE_INCLUDE_AMSR_APPLICATION_BASE_INTERNAL_NEW_DELETE_BUFFER_H_