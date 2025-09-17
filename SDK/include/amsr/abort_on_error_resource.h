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
/*!        \file  amsr/abort_on_error_resource.h
 *        \brief  A resource that will abort if its upstream resource returns a nullptr.
 *         \unit  VaCommonLib::MemoryManagement::MemoryResource
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_AMSR_ABORT_ON_ERROR_RESOURCE_H_
#define LIB_VAC_INCLUDE_AMSR_ABORT_ON_ERROR_RESOURCE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cassert>
#include <cstdint>

#include "ara/core/memory_resource.h"

namespace amsr {

/*!
 * \brief    A memory resource that aborts if its upstream resource fails to allocate.
 * \trace    CREQ-VaCommonLib-AbortOnErrorResource
 * \vprivate Product Private
 */
class AbortOnErrorResource final : public ara::core::MemoryResource {
 public:
  /*!
   * \brief Constructs a new instance of the AbortOnErrorResource class.
   * \details The upstream resource is set to ara::core::GetDefaultResource().
   * \context ANY
   * \pre -
   * \steady TRUE
   */
  AbortOnErrorResource() noexcept;

  /*!
   * \brief Constructs a new instance of the AbortOnErrorResource class.
   * \param[in] upstream_resource The upstream resource.
   * \context ANY
   * \pre -
   * \steady TRUE
   * \spec requires true; \endspec
   */
  explicit AbortOnErrorResource(ara::core::MemoryResource* upstream_resource) noexcept;

  /*!
   * \brief  Copy constructor.
   * \pre    -
   * \steady TRUE
   * \spec requires true; \endspec
   */
  AbortOnErrorResource(AbortOnErrorResource const&) noexcept = default;

  /*!
   * \brief     Move constructor.
   * \details   The upstream resource of \p other will be set to amsr::NullMemoryResource after the move.
   * \param[in] other The value to be moved.
   * \pre       -
   * \steady TRUE
   * \spec requires true; \endspec
   */
  AbortOnErrorResource(AbortOnErrorResource&& other) noexcept;

  /*!
   * \brief   Destructs the instance of AbortOnErrorResource.
   * \context ANY
   * \pre -
   * \steady TRUE
   * \spec requires true; \endspec
   */
  ~AbortOnErrorResource() noexcept final = default;  // VCA_VAC_FREEING_STATIC_THIS_POINTER

  /*!
   * \brief  Copy-assignment operator.
   * \return *this, containing the contents of other.
   * \pre    -
   * \steady TRUE
   * \spec requires true; \endspec
   */
  auto operator=(AbortOnErrorResource const&) & noexcept -> AbortOnErrorResource& = default;

  /*!
   * \brief     Move-assignment operator.
   * \details   The upstream resource of \p other will be set to amsr::NullMemoryResource after the move.
   * \param[in] other The value to be move assigned.
   * \return    *this, containing the contents of other.
   * \pre       -
   * \steady TRUE
   * \spec requires true; \endspec
   */
  auto operator=(AbortOnErrorResource&& other) & noexcept -> AbortOnErrorResource&;

  /*!
   * \brief  Returns a pointer to the upstream memory resource.
   * \return Pointer to the upstream resource.
   * \context ANY
   * \pre -
   * \steady TRUE
   * \spec requires true; \endspec
   */
  auto GetUpstreamResource() const noexcept -> MemoryResource*;

 protected:
  /*!
   * \brief Allocates storage with a size of at least \p bytes bytes.
   * \details Forwards the call to the upstream resource and if the upstream resource cannot allocate it aborts.
   * \param[in] bytes The number of bytes to allocate.
   * \param[in] alignment The alignment of the allocated storage. Must be a power of two.
   * \return Pointer to the allocated storage if memory from the pool was available.
   * \steady TRUE
   */
  auto DoAllocate(std::size_t bytes, std::size_t alignment) noexcept -> void* final;

  /*!
   * \brief Deallocates storage.
   * \details Forwards the call to the upstream resource.
   * \param[in] p Pointer to the storage to deallocate.
   * \param[in] bytes Size, in bytes, of the storage to deallocate.
   * \param[in] alignment The alignment of the storage to deallocate.
   * \steady TRUE
   */
  void DoDeallocate(void* p, std::size_t bytes, std::size_t alignment) noexcept final;

  /*!
   * \brief Checks whether \p other equals \p this.
   * \param[in] other The memory resource to compare.
   * \return true, if the addresses of the memory resources are equal; false, otherwise.
   * \steady TRUE
   */
  auto DoIsEqual(MemoryResource const& other) const noexcept -> bool final;

 private:
  /*!
   * \brief The upstream resource to use for allocation/deallocation.
   */
  ara::core::MemoryResource* upstream_resource_;
};

}  // namespace amsr

#endif  // LIB_VAC_INCLUDE_AMSR_ABORT_ON_ERROR_RESOURCE_H_
