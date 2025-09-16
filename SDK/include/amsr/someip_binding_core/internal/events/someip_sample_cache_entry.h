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
/*!        \file  someip_binding_core/internal/events/someip_sample_cache_entry.h
 *        \brief  SOME/IP binding specific event sample cache entry.
 *        \unit   SomeIpBinding::SomeIpBindingCore::Client::Events::InvisibleSampleCache
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_EVENTS_SOMEIP_SAMPLE_CACHE_ENTRY_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_EVENTS_SOMEIP_SAMPLE_CACHE_ENTRY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>
#include "amsr/core/optional.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {
namespace events {

/*!
 * \brief SOME/IP binding specific sample cache entry
 */
class SomeIpSampleCacheEntry final {
 public:
  /*!
   * \brief Type alias for time stamp type.
   */
  using TimeStamp = ::amsr::someip_protocol::internal::TimeStamp;

  /*!
   * \brief       Constructor for SomeIpSampleCacheEntry
   * \param[in]   packet Serialized event packet
   * \param[in]   time_stamp Optional timestamp of the event packet
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \vprivate
   * \spec
   *   requires packet != nullptr;
   * \endspec
   */
  SomeIpSampleCacheEntry(::amsr::someip_binding_core::internal::DataBufferSharedPtr packet,
                         amsr::core::Optional<TimeStamp> const& time_stamp)
      // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
      : serialized_sample_{packet}, buffer_view_{packet->GetView(0U)}, time_stamp_{time_stamp} {
    assert(serialized_sample_ != nullptr);  // COV_MSR_INV_STATE_ASSERT
  }

  /*!
   * \brief       Destructor for SomeIpSampleCacheEntry
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \vprivate
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
  ~SomeIpSampleCacheEntry() noexcept = default;

  SomeIpSampleCacheEntry(SomeIpSampleCacheEntry const&) = delete;
  SomeIpSampleCacheEntry(SomeIpSampleCacheEntry const&&) = delete;
  SomeIpSampleCacheEntry& operator=(SomeIpSampleCacheEntry const&) = delete;
  SomeIpSampleCacheEntry& operator=(SomeIpSampleCacheEntry const&&) = delete;

  /*!
   * \brief return the timestamp of this sample.
   * \return Optional timestamp.
   *
   * \pre -
   * \context App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  amsr::core::Optional<TimeStamp> GetTimeStamp() const { return time_stamp_; }

  /*!
   * \brief Get a view to the buffer of the serialized sample.
   * \return The MemoryBufferView
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  MemoryBuffer::MemoryBufferView GetBufferView() const { return buffer_view_; };

  /*!
   * \brief Get the size of the buffer which holds the serialized sample.
   * \return The buffer size
   * \steady TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  std::size_t GetBufferSize() const {
    // VECTOR NC AutosarC++17_10-M0.3.1: MD_SOMEIPBINDING_AutosarC++17_10-M0.3.1_ptr_null_at_dereference
    // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
    return serialized_sample_->GetView(0).size();
  }

 private:
  /*!
   * \brief Shared pointer to serialized (raw) event sample
   */
  DataBufferSharedPtr serialized_sample_;

  /*!
   * \brief Buffer view of the serialized sample
   */
  MemoryBuffer::MemoryBufferView const buffer_view_;

  /*!
   * \brief Time stamp when receiving a message
   */
  amsr::core::Optional<TimeStamp> const time_stamp_;
};

}  // namespace events
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_EVENTS_SOMEIP_SAMPLE_CACHE_ENTRY_H_
