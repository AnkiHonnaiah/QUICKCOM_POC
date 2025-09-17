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
/**         \file   generic_memory_cache_interface.h
 *         \brief   Interface to return the generic memory pointer back to its corresponding cache.
 *
 **********************************************************************************************************************/
#ifndef LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_INTERNAL_GENERIC_MEMORY_CACHE_INTERFACE_H_
#define LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_INTERNAL_GENERIC_MEMORY_CACHE_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/generic/activatable_source_code.h"
#include "amsr/socal/internal/generic_memory_wrapper_interface.h"

namespace amsr {
namespace socal {
namespace internal {

/*!
 * \brief Interface to return a memory entry back to its corresponding cache.
 */
class AMSR_LG_BETA GenericMemoryCacheInterface {
 public:
  /*!
   * \brief Type of the entry that shall be returned to this cache.
   */
  using GenericMemoryCacheEntryPtr = std::unique_ptr<::amsr::socal::internal::GenericMemoryWrapperInterface>;

  /*!
   * \brief Creates a generic memory cache.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  GenericMemoryCacheInterface() noexcept = default;

  /*!
   * \brief Destroys the generic memory cache.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  virtual ~GenericMemoryCacheInterface() noexcept = default;

  GenericMemoryCacheInterface(GenericMemoryCacheInterface const&) = delete;
  GenericMemoryCacheInterface& operator=(GenericMemoryCacheInterface const&) = delete;
  GenericMemoryCacheInterface(GenericMemoryCacheInterface&&) = delete;
  GenericMemoryCacheInterface& operator=(GenericMemoryCacheInterface&&) = delete;

  /*!
   * \brief Returns a cache entry back to the cache.
   * \param[in] entry  A preallocated cache entry which is re-added to the cache. This must not be a nullptr.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  virtual void ReturnEntry(GenericMemoryCacheEntryPtr&& entry) noexcept = 0;
};

}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_INTERNAL_GENERIC_MEMORY_CACHE_INTERFACE_H_
