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
/*!        \file  invisible_sample_cache.h
 *        \brief  Event cache for SomeIp events
 *        \unit   SomeIpBinding::SomeIpBindingCore::Client::Events::InvisibleSampleCache
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_EVENTS_INVISIBLE_SAMPLE_CACHE_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_EVENTS_INVISIBLE_SAMPLE_CACHE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <cstddef>
#include <memory>
#include <mutex>

#include "amsr/core/optional.h"
#include "amsr/someip_binding_core/internal/events/someip_sample_cache_entry.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "amsr/unique_ptr.h"
#include "someip-protocol/internal/someip_posix_types.h"
#include "vac/container/static_list.h"

#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {
namespace events {

/*!
 * \brief SOME/IP binding specific sample cache
 */
class InvisibleSampleCache final {
 public:
  /*!
   * \brief Type alias for time stamp type.
   */
  using TimeStamp = ::amsr::someip_protocol::internal::TimeStamp;

  /*!
   * \brief Type alias for the container which is used by a consumer of InvisibleSampleCache
   */
  using SampleCacheContainer = ::vac::container::StaticList<amsr::UniquePtr<SomeIpSampleCacheEntry>>;

  /*!
   * \brief       Constructor for InvisibleSampleCache
   * \param[in]   capacity The amount of elements that can be cached
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \vprivate
   * \spec
   *   requires true;
   * \endspec
   */
  explicit InvisibleSampleCache(std::size_t const capacity);

  /*!
   * \brief       Destructor for InvisibleSampleCache
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \vprivate
   * \spec
   *   requires true;
   * \endspec
   */
  ~InvisibleSampleCache() noexcept = default;

  InvisibleSampleCache(InvisibleSampleCache const&) = delete;
  InvisibleSampleCache(InvisibleSampleCache const&&) = delete;
  InvisibleSampleCache& operator=(InvisibleSampleCache const&) = delete;
  InvisibleSampleCache& operator=(InvisibleSampleCache const&&) = delete;

  /*!
   * \brief Creates an entry in the invisible sample cache holding the buffer of the event.
   * \details When the buffer is full the oldest entry is removed before the new entry is added.
   * \param[in] packet buffer which contains the event data.
   * \param[in] time_stamp optional timestamp of the event.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \vprivate
   *
   * \return true if the oldest enqueued sample was dropped when enqueuing the new sample, false otherwise
   * \internal context Enqueue \endinternal
   */
  bool Enqueue(::amsr::someip_binding_core::internal::DataBufferSharedPtr packet,
               ::amsr::core::Optional<TimeStamp> const& time_stamp);

  /*!
   * \brief This functions is used for testing purposes only, it returns the number of
   *        enqueued samples in the cache.
   * \return the number of enqueued samples in the cache
   *
   * \pre         -
   * \context     Testing
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \vprivate
   * \spec
   *   requires true;
   * \endspec
   */
  std::size_t GetEnqueuedSampleCount() const noexcept;

  /*!
   * \brief Returns the capacity of the invisible sample cache
   * \return capacity of the invisible sample cache
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \vprivate
   * \spec
   *   requires true;
   * \endspec
   */
  std::size_t Capacity() const noexcept;

  /*!
   * \brief Moves sample pointers from reactor cache to application cache and provides access to the cache.
   *
   * \details The returned SampleCacheContainer reference shall be used to retrieve and remove processed samples from
   *          the cache.
   *          The user of GetSamples are not allowed to call GetSamples while modifying the returned reference.
   *
   *          The sample cache container can have less, equal or more elements after this call.
   *          Less:  There were not as many elements available as requested when calling GetSamples
   *          Equal: There were at least as many elements available as requested
   *          More:  Not all samples were processed from the previous GetSamples call and the current call requests
   *                 less samples than the not processed samples from the previous call.
   *
   * \param[in] requested_sample_count The number of elements are requested to be available.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \vprivate
   * \spec
   *   requires true;
   * \endspec
   *
   * \return Reference to the sample cache container
   * \internal context GetSamples \endinternal
   */
  SampleCacheContainer& GetSamples(std::size_t const requested_sample_count);

 private:
  /*!
   * \brief Maximum number of stored events in the invisible cache.
   */
  std::size_t const capacity_{0U};

  /*!
   * \brief Event storage for events which are intended to be processed by the application.
   * \internal resource AppCache \endinternal
   */
  ::vac::container::StaticList<amsr::UniquePtr<SomeIpSampleCacheEntry>> app_cache_{};

  /*!
   * \brief Event storage for new events coming from the reactor.
   * \internal resource ReactorCache \endinternal
   */
  ::vac::container::StaticList<amsr::UniquePtr<SomeIpSampleCacheEntry>> reactor_cache_{};

  /*!
   * \brief Mutex to protect invisible sample cache
   */
  mutable std::mutex cache_mutex_{};

  /*!
   * \brief Logger for tracing and debugging
   */
  ::amsr::someip_binding_core::internal::logging::AraComLogger logger_;
};

}  // namespace events
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_EVENTS_INVISIBLE_SAMPLE_CACHE_H_

/*!
 * \exclusivearea ::amsr::someip_binding_core::internal::events::InvisibleSampleCache::cache_mutex_,
 *                Used for protecting parallel access to the invisible sample cache by App (read) and Reactor (write).
 *
 * \protects ::amsr::someip_binding_core::internal::events::InvisibleSampleCache::cache_,
 *           ::amsr::someip_binding_core::internal::events::InvisibleSampleCache::capacity_,
 *           are the protected resources of an InvisibleSampleCache object.
 *
 * \usedin ::amsr::someip_binding_core::internal::events::InvisibleSampleCache::Enqueue
 * \usedin ::amsr::someip_binding_core::internal::events::InvisibleSampleCache::GetEnqueuedSampleCount
 * \usedin ::amsr::someip_binding_core::internal::events::InvisibleSampleCache::GetSamples
 * \exclude User threads/Reactor thread from accessing a variable that is already in access.
 * \length Small samples are only moved from one list to another.
 * \endexclusivearea
 */
