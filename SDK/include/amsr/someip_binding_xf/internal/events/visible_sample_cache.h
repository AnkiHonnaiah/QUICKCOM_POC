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
/*!       \file   someip_binding_xf/internal/events/visible_sample_cache.h
 *        \brief  Contains an implementation for a stack like cache, with a guarantee of no memory (re-)allocation or
 *                reordering after its construction.
 *        \unit   SomeIpBinding::SomeIpBindingXf::Communication::Events::VisibleSampleCache
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_VISIBLE_SAMPLE_CACHE_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_VISIBLE_SAMPLE_CACHE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <mutex>
#include <utility>

#include "amsr/socal/internal/events/cache_interface.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger_singleton.h"
#include "ara/core/vector.h"
#include "memory_wrapper.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace events {
/*!
 * \brief Use string literal for creation of StringViews.
 */
// VECTOR NC AutosarC++17_10-M7.3.6, VectorC++-V5.0.1: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.6_using_directive
// VECTOR NC AutosarC++17_10-M7.3.4, VectorC++-V5.0.1: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.4_using_directive
using vac::container::literals::operator""_sv;

/*!
 * \brief Generic template class representing a cache for holding preallocated samples with a guarantee of no memory
 *        reallocation after its construction.
 * \tparam SampleType  Type of stored sample
 */
template <typename SampleType>
class VisibleSampleCache final : public ::amsr::socal::internal::events::CacheInterface<SampleType> {
 public:
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
  ~VisibleSampleCache() noexcept final = default;

  VisibleSampleCache() = delete;
  VisibleSampleCache(VisibleSampleCache const&) = delete;
  VisibleSampleCache(VisibleSampleCache&&) = delete;
  VisibleSampleCache& operator=(VisibleSampleCache const&) & = delete;
  VisibleSampleCache& operator=(VisibleSampleCache&&) & = delete;

  /*!
   * \brief Type alias for visible sample cache entry type.
   */
  using CacheEntryType = typename ::amsr::socal::internal::events::CacheInterface<SampleType>::CacheEntryType;

  /*!
   * \brief Type alias for visible sample cache container.
   */
  using CacheContainerType = ara::core::Vector<CacheEntryType>;

  /*!
   * \brief Type alias for CacheInterface.
   */
  using CacheInterface = ::amsr::socal::internal::events::CacheInterface<SampleType>;

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
   *
   * \internal
   * - Reserve the cache capacity to the given size.
   * - Preallocate the memory for the given number of samples, and add them to the cache_.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOMEIPBINDING_AutosarC++17_10-A15.4.2_STL_exceptions_noexcept
  explicit VisibleSampleCache(std::size_t const cache_size) noexcept : CacheInterface{}, cache_capacity_{cache_size} {
    // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
    cache_.reserve(cache_capacity_);
    while (cache_.size() < cache_capacity_) {
      // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
      cache_.emplace_back(std::make_shared<MemoryWrapper<SampleType>>());
    }
  }

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
   *
   * \internal
   * - Guard visible sample cache against parallel access.
   * - If the cache is not empty, return the last element by removing it from the cache.
   * - Otherwise, return a unique_ptr that owns nothing.
   * \endinternal
   * \internal \context GetNextFreeSample \endinternal
   */
  auto GetNextFreeSample() noexcept -> CacheEntryType {
    CacheEntryType sample{nullptr};
    std::lock_guard<std::mutex> const guard{cache_mutex_};
    if (!cache_.empty()) {
      // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
      sample = std::move(cache_.back());
      // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
      cache_.pop_back();
    }
    return sample;
  }

  /*!
   * \brief Returns a CacheEntry to the cache.
   * \param[in] entry A preallocated cache entry which is re-added to the cache. This must not be a nullptr.
   * \pre -
   * \context    ANY
   * \threadsafe TRUE
   * \reentrant  TRUE
   * \steady     TRUE
   * \vprivate   Product Private
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - If cache entry to be returned back into the visible sample cache is a nullptr:
   *   - Log fatal error and abort further process execution.
   * - If no other user is using the cache entry anymore (shared ownership):
   *   - Guard visible sample cache against parallel access.
   *   - Put the cache entry back to the visible sample cache.
   * - Otherwise:
   *   - Release the ownership of the cache entry.
   *     As there are still other users of the cache entry it's not returned back to the visible sample cache.
   * \endinternal
   * \internal context ReturnEntry \endinternal
   */
  void ReturnEntry(CacheEntryType&& entry) noexcept final {
    if (entry == nullptr) {
      ::amsr::someip_binding_core::internal::logging::AraComLoggerSingleton::GetInstance()
          .GetAccess()
          ->LogFatalAndAbort(
              ::amsr::core::StringView{"VisibleSampleCache"},
              [](::amsr::someip_binding_core::internal::logging::StringStream& s) {
                s << "Returned entry is nullptr.";
              },
              static_cast<char const*>(__func__), __LINE__);
    }

    // The assumption that use_count() returns a valid reference count relies on the correct usage described in
    // DSGN-SomeIpBinding-Events-Reception under Pre-allocated visible sample cache.
    if (entry.use_count() == 1) {
      std::lock_guard<std::mutex> const guard{cache_mutex_};
      // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
      cache_.emplace_back(std::move(entry));
    } else {
      // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
      entry.reset();
    }
  }

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
   *
   * \internal
   * - Guard visible sample cache against parallel access.
   * - Return the current cache size.
   * \endinternal
   * \internal context GetFreeSampleCount \endinternal
   */
  auto GetFreeSampleCount() const noexcept -> std::size_t {
    std::lock_guard<std::mutex> const guard{cache_mutex_};
    return cache_.size();
  }

 private:
  /*!
   * \brief Cache to store the preallocated samples.
   * \internal resource Cache \endinternal
   */
  CacheContainerType cache_{};

  /*!
   * \brief Mutex to protect the read/write access of the cache_.
   */
  mutable std::mutex cache_mutex_{};

  /*!
   * \brief Maximum number of samples that can be stored in the cache_.
   */
  std::size_t const cache_capacity_;
};

}  // namespace events
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

/*!
 * \exclusivearea ::amsr::someip_binding_xf::internal::events::VisibleSampleCache::cache_mutex_
 * Used for synchronization by using a lock guard to protect access to the cache.
 *
 * \protects ::amsr::someip_binding_xf::internal::events::VisibleSampleCache::cache_
 * \usedin ::amsr::someip_binding_xf::internal::events::VisibleSampleCache::ReturnSample,
 *         ::amsr::someip_binding_xf::internal::events::VisibleSampleCache::GetNextFreeSample,
 *         ::amsr::someip_binding_xf::internal::events::VisibleSampleCache::GetFreeSampleCount
 * \length SHORT Accessing or modifying the cache.
 * \endexclusivearea
 */

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_VISIBLE_SAMPLE_CACHE_H_
