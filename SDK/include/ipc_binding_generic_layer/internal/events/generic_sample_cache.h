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
 *        \brief  Contains a class for a generic memory cache.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_GENERIC_LAYER_INCLUDE_IPC_BINDING_GENERIC_LAYER_INTERNAL_EVENTS_GENERIC_SAMPLE_CACHE_H_
#define LIB_IPC_BINDING_GENERIC_LAYER_INCLUDE_IPC_BINDING_GENERIC_LAYER_INTERNAL_EVENTS_GENERIC_SAMPLE_CACHE_H_

#include <memory>
#include <mutex>

#include "amsr/generic/activatable_source_code.h"
#include "amsr/socal/internal/generic_memory_cache_interface.h"
#include "ara/core/vector.h"

#include "ipc_binding_generic_layer/internal/memory_wrapper/generic_memory_wrapper.h"

namespace amsr {
namespace ipc_binding_generic_layer {
namespace internal {
namespace events {

/*!
 * \brief Generic sample cache representing a container of event ipc packet.
 *
 * \unit IpcBinding::IpcBindingGenericLayer::GenericProxyEvent
 */
class AMSR_LG_BETA GenericSampleCache final : public amsr::socal::internal::GenericMemoryCacheInterface {
 public:
  /*!
   * \brief Type of pointer stored in the cache.
   */
  using GenericSamplePtr = std::unique_ptr<GenericMemoryWrapper>;  // IGNORE_BLACKLIST BSOCIAL-8805

  /*!
   * \brief Type alias for IpcPacketShared.
   */
  using IpcPacketShared = ::amsr::ipc_binding_core::internal::ipc_protocol::IpcPacketShared;

  /*!
   * \brief Type alias for generic sample cache container.
   */
  using GenericSampleContainer = ara::core::Vector<GenericSamplePtr>;

  /*!
   * \brief   Constructs the generic sample cache with the given size.
   * \details Pre allocates the memory for the given number of samples, which can be later requested, and can be added
   *          back after its usage.
   *
   * \param[in] cache_size  Capacity to reserve.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_IPCBINDING_AutosarC++17_10-A15.4.2_STL_exceptions
  explicit GenericSampleCache(std::size_t cache_size) noexcept;

  /*!
   * \brief Destroys the object.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  ~GenericSampleCache() noexcept final = default;

  GenericSampleCache(GenericSampleCache const&) noexcept = delete;
  auto operator=(GenericSampleCache const&) noexcept -> GenericSampleCache& = delete;
  GenericSampleCache(GenericSampleCache&&) noexcept = delete;
  auto operator=(GenericSampleCache&&) noexcept -> GenericSampleCache& = delete;

  /*!
   * \brief Gets the next free sample if available.
   *
   * \return The next free sample pointer if available. Otherwise a unique_ptr that owns nothing.
   *
   * \pre        -
   * \context    ANY
   * \threadsafe TRUE
   * \reentrant  TRUE
   */
  auto GetNextFreeSample() noexcept -> GenericSamplePtr;

  /*!
   * \brief Get the number of free samples inside the cache.
   *
   * \return The number of free samples inside the cache.
   *
   * \pre        -
   * \context    ANY
   * \threadsafe TRUE
   * \reentrant  TRUE
   */
  auto GetFreeSampleCount() const noexcept -> std::size_t;

  /*!
   * \brief Returns a CacheEntry to the cache.
   *
   * \param[in] entry  A preallocated cache entry which is re-added to the cache. This must not be a nullptr.
   *
   * \pre        -
   * \context    ANY
   * \threadsafe TRUE
   * \reentrant  TRUE
   */
  auto ReturnEntry(GenericMemoryCacheEntryPtr&& entry) noexcept -> void final;

 private:
  /*!
   * \brief Cache to store the preallocated samples.
   */
  GenericSampleContainer cache_{};

  /*!
   * \brief Mutex to protect the read/write access of the cache_.
   */
  mutable std::mutex cache_mutex_{};

  /*!
   * \brief Maximum number of samples that can be stored in the cache_.
   */
  std::size_t const capacity_{0};
};

}  // namespace events
}  // namespace internal
}  // namespace ipc_binding_generic_layer
}  // namespace amsr

#endif  // LIB_IPC_BINDING_GENERIC_LAYER_INCLUDE_IPC_BINDING_GENERIC_LAYER_INTERNAL_EVENTS_GENERIC_SAMPLE_CACHE_H_
