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
/*!       \file
 *        \brief  Contains an implementation for a stack like cache, with a guarantee of no memory (re-)allocation or
 *                reordering after its construction.
 *        \unit   SomeIpBinding::SomeIpBindingGenericLayer::GenericProxyVisibleSampleCache
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_GENERIC_PROXY_VISIBLE_SAMPLE_CACHE_H_
#define LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_GENERIC_PROXY_VISIBLE_SAMPLE_CACHE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <mutex>
#include <utility>

#include "amsr/socal/internal/generic_memory_cache_interface.h"
#include "amsr/someip_binding_generic_layer/internal/events/generic_memory_wrapper.h"

namespace amsr {
namespace someip_binding_generic_layer {
namespace internal {

/*!
 * \brief Generic template class representing a cache for holding preallocated samples with a guarantee of no memory
 *        reallocation after its construction.
 */
class GenericProxyVisibleSampleCache final : public ::amsr::socal::internal::GenericMemoryCacheInterface {
 public:
  /*!
   * \brief Type alias for visible sample cache entry type.
   */
  using CacheEntryType = std::unique_ptr<events::GenericMemoryWrapper>;

  /*!
   * \brief Type alias for visible sample cache container.
   */
  using CacheContainerType = ara::core::Vector<CacheEntryType>;

  /*!
   * \brief       Destroys the object
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~GenericProxyVisibleSampleCache() noexcept final = default;

  GenericProxyVisibleSampleCache() = delete;
  GenericProxyVisibleSampleCache(GenericProxyVisibleSampleCache const&) = delete;
  GenericProxyVisibleSampleCache(GenericProxyVisibleSampleCache&&) = delete;
  GenericProxyVisibleSampleCache& operator=(GenericProxyVisibleSampleCache const&) & = delete;
  GenericProxyVisibleSampleCache& operator=(GenericProxyVisibleSampleCache&&) & = delete;

  /*!
   * \brief Returns a cache entry back to the cache.
   * \param[in] entry  A preallocated cache entry which is re-added to the cache. This must not be a nullptr.
   * \pre         The returned entry must originate from exactly this cache instance.
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  void ReturnEntry(GenericMemoryCacheEntryPtr&& entry) noexcept final;

  /*!
   * \brief Constructs the visible sample cache with the given size.
   * \details Pre allocates the memory for the given number of samples, which can be later
   *          requested, and can be added back after its usage.
   * \param[in] cache_size Capacity to reserve.
   * \pre -
   * \context    Init
   * \threadsafe TRUE
   * \reentrant  TRUE
   * \steady     FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOMEIPBINDING_AutosarC++17_10-A15.4.2_STL_exceptions_noexcept
  explicit GenericProxyVisibleSampleCache(std::size_t const cache_size) noexcept;

  /*!
   * \brief Gets the next free sample if available.
   * \return The next free sample pointer if available. Otherwise a unique_ptr that owns nothing.
   * \pre -
   * \context    ANY
   * \threadsafe TRUE
   * \reentrant  TRUE
   * \steady     TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetNextFreeSample() noexcept -> CacheEntryType;

  /*!
   * \brief Gets the number of free samples inside the cache.
   * \return The number of free samples inside the cache.
   * \pre -
   * \context    ANY
   * \threadsafe TRUE
   * \reentrant  TRUE
   * \steady     TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetFreeSampleCount() const noexcept -> std::size_t;

 private:
  /*!
   * \brief Cache to store the preallocated samples.
   */
  CacheContainerType cache_{};

  /*!
   * \brief Mutex to protect the read/write access of the cache_.
   */
  std::mutex cache_mutex_{};

  /*!
   * \brief Maximum number of samples that can be stored in the cache_.
   */
  std::size_t const cache_capacity_;
};

}  // namespace internal
}  // namespace someip_binding_generic_layer
}  // namespace amsr

/*!
 * \exclusivearea ::amsr::someip_binding_generic_layer::internal::GenericProxyVisibleSampleCache::cache_mutex_
 * Used for synchronization by using a lock guard to protect access to the cache.
 *
 * \protects ::amsr::someip_binding_generic_layer::internal::GenericProxyVisibleSampleCache::cache_
 * \usedin ::amsr::someip_binding_generic_layer::internal::GenericProxyVisibleSampleCache::ReturnSample,
 *         ::amsr::someip_binding_generic_layer::internal::GenericProxyVisibleSampleCache::GetNextFreeSample,
 *         ::amsr::someip_binding_generic_layer::internal::GenericProxyVisibleSampleCache::GetFreeSampleCount
 * \length SHORT Accessing or modifying the cache.
 * \endexclusivearea
 */

#endif  // LIB_SOMEIP_BINDING_GENERIC_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_GENERIC_LAYER_INTERNAL_GENERIC_PROXY_VISIBLE_SAMPLE_CACHE_H_
