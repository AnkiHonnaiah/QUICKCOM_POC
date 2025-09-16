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
/*!        \file
 *        \brief  Contains an implementation for a stack like cache, with a guarantee of no memory (re-)allocation or
 *                reordering after its construction.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_EVENTS_VISIBLE_SAMPLE_CACHE_H_
#define LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_EVENTS_VISIBLE_SAMPLE_CACHE_H_

#include <memory>
#include <mutex>
#include <utility>

#include "amsr/shared_ptr.h"
#include "amsr/socal/internal/events/cache_interface.h"
#include "ara/core/vector.h"
#include "vac/container/string_literals.h"

#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_transformation_layer/internal/events/memory_wrapper.h"

namespace amsr {
namespace ipc_binding_transformation_layer {
namespace internal {
namespace events {

/*!
 * \brief Use string literal for creation of StringViews.
 */
// VECTOR NC AutosarC++17_10-M7.3.6, VectorC++-V5.0.1: MD_IPCBINDING_AutosarC++17_10-M7.3.6_using_directive
// VECTOR NC AutosarC++17_10-M7.3.4, VectorC++-V5.0.1: MD_IPCBINDING_AutosarC++17_10-M7.3.4_using_directive
using vac::container::literals::operator""_sv;

/*!
 * \brief Generic template class representing a cache for holding preallocated samples with a guarantee of no memory
 *        reallocation after its construction.
 *
 * \tparam SampleType  Type of stored sample.
 *
 * \unit IpcBinding::IpcBindingTransformationLayer::VisibleSampleCache
 */
template <typename SampleType>
class VisibleSampleCache final : public ::amsr::socal::internal::events::CacheInterface<SampleType> {
 public:
  /*!
   * \brief Destroys the object.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~VisibleSampleCache() noexcept final = default;  // VCA_IPCB_COMPILER_GENERATED_DESTRUCTOR

  VisibleSampleCache(VisibleSampleCache const&) noexcept = delete;
  auto operator=(VisibleSampleCache const&) noexcept -> VisibleSampleCache& = delete;
  VisibleSampleCache(VisibleSampleCache&&) noexcept = delete;
  auto operator=(VisibleSampleCache&&) noexcept -> VisibleSampleCache& = delete;

  /*!
   * \brief Type alias for visible sample cache entry type.
   */
  using CacheEntryType = typename ::amsr::socal::internal::events::CacheInterface<SampleType>::CacheEntryType;

  /*!
   * \brief Type alias for visible sample cache container.
   */
  using CacheContainerType = ara::core::Vector<CacheEntryType>;

  /*!
   * \brief   Constructs the visible sample cache with the given size.
   * \details Pre allocates the memory for the given number of samples, which can be later requested, and can be added
   *          back after its usage.
   *
   * \param[in] cache_size  Capacity to reserve.
   *
   * \pre        -
   * \context    ANY
   * \threadsafe TRUE
   * \reentrant  TRUE
   * \steady     FALSE
   *
   * \internal
   * - Reserve the cache capacity to the given size.
   * - Preallocate the memory for the given number of samples, and add them to the cache_.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_IPCBINDING_AutosarC++17_10-A15.4.2_STL_exceptions
  explicit VisibleSampleCache(std::size_t const cache_size) noexcept
      // () is used to initialize the base class to suppress doxygen issue.
      : ::amsr::socal::internal::events::CacheInterface<SampleType>(), cache_capacity_{cache_size} {
    cache_.reserve(cache_capacity_);  // VCA_IPCB_STD_FUNCTION_CALLED
    while (cache_.size() < cache_capacity_) {
      // VCA_IPCB_STD_FUNCTION_CALLED, VCA_IPCB_STD_UNIQUE_POINTER_DESTRUCTOR
      cache_.emplace_back(std::make_shared<MemoryWrapper<SampleType>>());  // IGNORE_BLACKLIST external API
    }
  }

  /*!
   * \brief Gets the next free sample if available.
   *
   * \return The next free sample pointer if available. Otherwise a unique_ptr that owns nothing.
   *
   * \pre        -
   * \context    ANY
   * \threadsafe TRUE
   * \reentrant  TRUE
   * \steady     TRUE
   *
   * \internal
   * - Guard visible sample cache against parallel access.
   * - If the cache is not empty, return the last element by removing it from the cache.
   * - Otherwise, return a unique_ptr that owns nothing.
   * \endinternal
   */
  auto GetNextFreeSample() noexcept -> CacheEntryType {
    CacheEntryType sample{nullptr};
    std::lock_guard<std::mutex> const guard{cache_mutex_};
    if (!cache_.empty()) {
      // VCA_IPCB_STD_FUNCTION_CALLED
      sample = std::move(cache_.back());
      // VCA_IPCB_STD_FUNCTION_CALLED
      cache_.pop_back();
    }
    return sample;
  }

  /*!
   * \brief Get the number of free samples inside the cache.
   *
   * \return The number of free samples inside the cache.
   *
   * \pre        -
   * \context    ANY
   * \threadsafe TRUE
   * \reentrant  TRUE
   * \steady     TRUE
   *
   * \internal
   * - Guard visible sample cache against parallel access.
   * - Return the current cache size.
   * \endinternal
   */
  auto GetFreeSampleCount() const noexcept -> std::size_t {
    std::lock_guard<std::mutex> const guard{cache_mutex_};
    return cache_.size();
  }

  /*!
   * \brief Returns a CacheEntry to the cache.
   *
   * \param[in] entry  A preallocated cache entry which is re-added to the cache. This must not be a nullptr.
   *
   * \pre        -
   * \context    ANY
   * \threadsafe TRUE
   * \reentrant  TRUE
   * \steady     TRUE
   *
   * \internal
   * - If cache entry to be returned back into the visible sample cache is a nullptr:
   *   - Log fatal error and abort further process execution.
   * - If no other user is using the cache entry anymore (shared ownership):
   *   - Guard visible sample cache against parallel access.
   *   - Put the cache entry back to the visible sample cache.
   * - Otherwise:
   *   - Release the ownership of the cache entry. As there are still other users of the cache entry it's not returned
   *     back to the visible sample cache.
   * \endinternal
   */
  void ReturnEntry(CacheEntryType&& entry) noexcept final {
    if (entry == nullptr) {
      using Logger = ::amsr::ipc_binding_core::internal::logging::AraComLogger;
      Logger const logger{::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextId,
                          ::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextDescription,
                          "VisibleSampleCache"_sv};

      constexpr char const* error_message{"Returned entry is nullptr."};
      logger.TerminateOnViolation(  // COV_IpcBinding_CodeCoverageComponentOverall_logger_call_in_terminating_branch
          error_message, [](::ara::log::LogStream& s) { s << error_message; },
          Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
    }
    // The assumption that use_count() returns a valid reference count relies on the correct usage described in
    // DSGN-IpcBinding-Events-Reception under Pre-allocated visible sample cache.
    if (entry.use_count() == 1) {
      std::lock_guard<std::mutex> const guard{cache_mutex_};
      // VCA_IPCB_STD_FUNCTION_CALLED
      cache_.emplace_back(std::move(entry));
    } else {
      // VCA_IPCB_STD_FUNCTION_CALLED
      entry.reset();
    }
  }

 private:
  /*!
   * \brief Cache to store the preallocated samples.
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
}  // namespace ipc_binding_transformation_layer
}  // namespace amsr

#endif  // LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_EVENTS_VISIBLE_SAMPLE_CACHE_H_

// clang-format off
/*!
 * \exclusivearea ::amsr::ipc_binding_transformation_layer::internal::events::VisibleSampleCache::cache_mutex_
 * Used for synchronization by using a lock guard to protect access to the cache.
 *
 * \protects ::amsr::ipc_binding_transformation_layer::internal::events::VisibleSampleCache::cache_
 * \usedin ::amsr::ipc_binding_transformation_layer::internal::events::VisibleSampleCache::ReturnEntry,
 *         ::amsr::ipc_binding_transformation_layer::internal::events::VisibleSampleCache::GetNextFreeSample,
 *         ::amsr::ipc_binding_transformation_layer::internal::events::VisibleSampleCache::GetFreeSampleCount
 * \length SHORT Accessing or modifying the cache.
 * \endexclusivearea
 */
// clang-format on

// COV_JUSTIFICATION_BEGIN
// \ID COV_IpcBinding_CodeCoverageComponentOverall_logger_call_in_terminating_branch
//   \ACCEPT  XX
//   \REASON  CallCoverage gap reported as logger is a separate unit.
//            As the error scenario is handled as violation no further unit interactions are possible.
//            Therefore no separate test of CT or MUT level necessary.
// COV_JUSTIFICATION_END
