/*********************************************************************************************************************
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
/*!        \file  invisible_sample_cache.h
 *        \brief  Contains an implementation for invisible sample cache.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_EVENTS_INVISIBLE_SAMPLE_CACHE_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_EVENTS_INVISIBLE_SAMPLE_CACHE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <algorithm>
#include <memory>
#include <mutex>
#include <utility>

#include "vac/container/static_list.h"

#include "ipc_binding_core/internal/ipc_protocol/message.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace events {

/*!
 * \brief Use string literal for creation of StringViews.
 */
// VECTOR NC AutosarC++17_10-M7.3.6, VectorC++-V5.0.1: MD_IPCBINDING_AutosarC++17_10-M7.3.6_using_directive
// VECTOR NC AutosarC++17_10-M7.3.4, VectorC++-V5.0.1: MD_IPCBINDING_AutosarC++17_10-M7.3.4_using_directive
using vac::container::literals::operator""_sv;

/*!
 * \brief Generic template class representing a container of NotificationMessages.
 *
 * \tparam SampleType  Type of stored sample.
 *
 * \unit IpcBinding::IpcBindingCore::ProxyEvent
 */
class InvisibleSampleCache final {
 public:
  /*!
   * \brief Type alias for NotificationMessage.
   */
  using NotificationMessage = ::amsr::ipc_binding_core::internal::ipc_protocol::NotificationMessage;

  /*!
   * \brief Type alias for the container which is used by a consumer of InvisibleSampleCache.
   */
  using SampleCacheContainer = vac::container::StaticList<NotificationMessage>;

  /*!
   * \brief Constructor for InvisibleSampleCache.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   * \steady      FALSE
   */
  explicit InvisibleSampleCache()
      : logger_{::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextId,
                ::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextDescription,
                ::amsr::core::StringView{"InvisibleSampleCache"}} {}

  /*!
   * \brief Destructor for InvisibleSampleCache.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   * \steady      FALSE
   */
  ~InvisibleSampleCache() noexcept = default;

  InvisibleSampleCache(InvisibleSampleCache const&) noexcept = delete;
  auto operator=(InvisibleSampleCache const&) noexcept -> InvisibleSampleCache& = delete;
  InvisibleSampleCache(InvisibleSampleCache const&&) noexcept = delete;
  auto operator=(InvisibleSampleCache const&&) noexcept -> InvisibleSampleCache& = delete;

  /*!
   * \brief Clear the caches.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   * \steady      TRUE
   *
   * \internal
   * - Create lock for cache_mutex.
   * - Set the capacity to zero.
   * - Clear app and reactor cache.
   * \endinternal
   */
  void Clear() {
    std::lock_guard<std::mutex> const cache_lock{cache_mutex_};

    capacity_ = 0;
    app_cache_.clear();
    reactor_cache_.clear();
  }

  /*!
   * \brief Resize the caches.
   *
   * \param[in] capacity  The amount of elements that can be cached.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   * \steady      TRUE
   *
   * \internal
   * - Create lock for cache_mutex.
   * - Update the capacity.
   * - Resize app and reactor cache.
   * \endinternal
   */
  void Resize(std::size_t capacity) {
    std::lock_guard<std::mutex> const cache_lock{cache_mutex_};

    capacity_ = capacity;
    app_cache_.reserve(capacity);
    reactor_cache_.reserve(capacity);
  }

  /*!
   * \brief   Creates an entry in the invisible sample cache holding the sample of the event.
   * \details When the buffer is full the oldest entry is removed before the new entry is added.
   *
   * \param[in] notification_message  Notification to add to the sample cache.
   *
   * \return True if the event is pushed in the invisible cache, false otherwise.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   * \steady      TRUE
   *
   * \internal
   * - Create lock for cache_mutex.
   * - When the cache has a capacity greater than zero.
   *   - Check if the cache is full.
   *     - Remove the oldest cache entry.
   *   - Create new entry within cache.
   * \endinternal
   */
  auto Enqueue(NotificationMessage&& notification_message) -> bool {
    // VECTOR NC AutosarC++17_10-M0.1.8: MD_IPCBINDING_AutosarC++17_10_M0.1.8_Function_has_side_effect_logging
    logger_.LogDebug([](::ara::log::LogStream const&) {},
                     Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});

    bool cache_updated{false};
    std::lock_guard<std::mutex> const cache_lock{cache_mutex_};

    if (capacity_ != 0U) {
      if (reactor_cache_.size() == capacity_) {
        reactor_cache_.pop_front();
        logger_.LogDebug([](::ara::log::LogStream& s) { s << "Old sample dropped during notification"; },
                         Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
      }

      reactor_cache_.emplace_back(std::move(notification_message));
      cache_updated = true;
    }
    return cache_updated;
  }

  /*!
   * \brief Returns the capacity of the invisible sample cache.
   *
   * \return Capacity of the invisible sample cache.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   * \steady      FALSE
   */
  auto Capacity() const noexcept -> std::size_t { return capacity_; }

  /*!
   * \brief   Moves sample pointers from reactor cache to application cache and provides access to the cache.
   * \details The returned SampleCacheContainer reference shall be used to retrieve and remove processed samples from
   *          the cache. The user of GetSamples are not allowed to call GetSamples while modifying the returned
   *          reference.
   *
   *          The sample cache container can have less, equal or more elements after this call.
   *          - Less:  There were not as many elements available as requested when calling GetSamples.
   *          - Equal: There were at least as many elements available as requested.
   *          - More:  Not all samples were processed from the previous GetSamples call and the current call requests
   *            less samples than the not processed samples from the previous call.
   *
   * \param[in] requested_sample_count  The number of elements are requested to be available.
   *
   * \return Reference to the sample cache container.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   * \steady      TRUE
   *
   * \internal
   * - Create lock for cache_mutex.
   * - Calculate the number of stored samples within app cache and reactor cache.
   * - If this is greater than the capacity.
   *   - Drop excess from app cache.
   * - If the requested sample count is greater than the stored samples within app cache.
   *   - Calculate how many samples are needed from reactor cache.
   *   - Move those samples to the app cache.
   * \endinternal
   */
  auto GetSamples(std::size_t const requested_sample_count) -> SampleCacheContainer& {
    // VECTOR NC AutosarC++17_10-M0.1.8: MD_IPCBINDING_AutosarC++17_10_M0.1.8_Function_has_side_effect_logging
    logger_.LogDebug([](::ara::log::LogStream const&) {},
                     Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
    std::lock_guard<std::mutex> const cache_lock{cache_mutex_};

    // Drop samples which are pushed out of the buffer

    std::size_t const total_cache_size{reactor_cache_.size() + app_cache_.size()};
    if (total_cache_size > capacity_) {
      std::size_t const samples_to_drop{total_cache_size - capacity_};
      // VECTOR NL AutosarC++17_10-A6.5.1: MD_IPCBINDING_AutosarC++17_10-A6.5.1_loop_counter
      for (std::size_t drop_index{0}; drop_index < samples_to_drop; ++drop_index) {
        // invariant is that app cache has always at least the excessive amount of samples
        app_cache_.pop_front();
        logger_.LogDebug(  // COV_IpcBinding_CoverageComponentOverall_trivial_utility
            [](::ara::log::LogStream& s) { s << "Old sample dropped during notification"; },
            Logger::LogLocation{static_cast<char const*>(__func__), __LINE__});
      }
    }

    // move samples to application cache if needed
    std::size_t const cleaned_app_cache_size{app_cache_.size()};
    if (cleaned_app_cache_size < requested_sample_count) {
      std::size_t const missing_samples{requested_sample_count - cleaned_app_cache_size};

      std::size_t const samples_to_move{std::min(missing_samples, reactor_cache_.size())};
      // VECTOR NL AutosarC++17_10-A6.5.1: MD_IPCBINDING_AutosarC++17_10-A6.5.1_loop_counter
      for (std::size_t move_index{0}; move_index < samples_to_move; ++move_index) {
        // VCA_IPCB_CALLED_FUNCTION_OUTSIDE_COMPONENT
        app_cache_.push_back(std::move(reactor_cache_.front()));

        reactor_cache_.pop_front();
      }
    }

    return app_cache_;
  }

 private:
  /*!
   * \brief Maximum number of stored events in the invisible cache.
   */
  std::size_t capacity_{0U};

  /*!
   * \brief Event storage for events which are intended to be processed by the application.
   */
  vac::container::StaticList<NotificationMessage> app_cache_{};

  /*!
   * \brief Event storage for new events coming from the reactor.
   */
  vac::container::StaticList<NotificationMessage> reactor_cache_{};

  /*!
   * \brief Mutex to protect invisible sample cache.
   */
  mutable std::mutex cache_mutex_{};

  /*!
   * \brief Type alias for the user logger.
   */
  using Logger = ::amsr::ipc_binding_core::internal::logging::AraComLogger;

  /*!
   * \brief Logger for tracing and debugging.
   */
  Logger logger_;
};

}  // namespace events
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_EVENTS_INVISIBLE_SAMPLE_CACHE_H_

// clang-format off
/*!
 * \exclusivearea ::amsr::ipc_binding_core::internal::events::InvisibleSampleCache::cache_mutex_,
 *                Used for protecting parallel access to the invisible sample cache by App (read) and Reactor (write).
 *
 * \protects ::amsr::ipc_binding_core::internal::events::InvisibleSampleCache::app_cache_,
 *           ::amsr::ipc_binding_core::internal::events::InvisibleSampleCache::reactor_cache_,
 *           ::amsr::ipc_binding_core::internal::events::InvisibleSampleCache::capacity_,
 *           are the protected resources of an InvisibleSampleCache object.
 *
 * \usedin ::amsr::ipc_binding_core::internal::events::InvisibleSampleCache::Enqueue
 * \usedin ::amsr::ipc_binding_core::internal::events::InvisibleSampleCache::GetSamples
 * \usedin ::amsr::ipc_binding_core::internal::events::InvisibleSampleCache::Clear
 * \usedin ::amsr::ipc_binding_core::internal::events::InvisibleSampleCache::Resize
 * \exclude User threads/Reactor thread from accessing a variable that is already in access.
 * \length Small smaples are only moved from one list to another.
 * \endexclusivearea
 */
// clang-format on

// COV_JUSTIFICATION_BEGIN
// \ID COV_IpcBinding_CoverageComponentOverall_trivial_utility
//   \ACCEPT  XX
//   \REASON  FunctionCoverage gap is reported for the API.
//            Since the API is a trivial helper function and tested by unit tests
//            no component test function coverage needs to be achieved for this function.
// COV_JUSTIFICATION_END
