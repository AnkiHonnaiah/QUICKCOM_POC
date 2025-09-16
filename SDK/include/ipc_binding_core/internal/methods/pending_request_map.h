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
/**        \file
 *        \brief  Stores method requests that wait for their corresponding response.
 *
 *      \details -
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_METHODS_PENDING_REQUEST_MAP_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_METHODS_PENDING_REQUEST_MAP_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <mutex>
#include <utility>

#include "amsr/core/future.h"
#include "amsr/core/optional.h"
#include "amsr/core/promise.h"
#include "ara/core/map.h"

#include "ipc_binding_core/internal/memory.h"
#include "ipc_binding_core/internal/methods/destruction_action.h"
#include "ipc_binding_core/internal/methods/pending_request_map_interface.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace methods {

/*!
 * \brief   Provides functionality to store method requests into a map and restore requests on asynchronous response.
 * \details The map that stores the pending requests is protected from concurrent access, so that multiple requests can
 *          be triggered in parallel or a request and response at the same time.
 *
 * \tparam Output  Type of the response value that will be wrapped in a promise.
 *
 * \unit IpcBinding::IpcBindingCore::ProxyMethod
 */
template <typename Output>
class PendingRequestMap final : public PendingRequestMapInterface<Output>,
                                public amsr::EnableSharedFromThis<PendingRequestMap<Output>> {
 public:
  /*!
   * \brief Alias for SessionId.
   */
  using SessionId = typename PendingRequestMapInterface<Output>::SessionId;

  /*!
   * \brief Alias for Promise.
   */
  using Promise = typename PendingRequestMapInterface<Output>::Promise;

  /*!
   * \brief Optional for map entry.
   */
  using OptionalMapEntry = typename PendingRequestMapInterface<Output>::OptionalMapEntry;

  /*!
   * \brief Construct a pending request map.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  PendingRequestMap() noexcept = default;

  PendingRequestMap(PendingRequestMap&&) noexcept = delete;
  auto operator=(PendingRequestMap&&) noexcept -> PendingRequestMap& = delete;
  PendingRequestMap(PendingRequestMap const&) noexcept = delete;
  auto operator=(PendingRequestMap const&) noexcept -> PendingRequestMap& = delete;

  /*!
   * \brief Destroy the pending request map.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~PendingRequestMap() noexcept final = default;  // VCA_IPCB_COMPILER_GENERATED_DESTRUCTOR

  /*!
   * \copydoc PendingRequestMapInterface::StoreRequest()
   *
   * \internal
   * - Prevent the request map from concurrent access.
   *   - Try to emplace a new promise object into the pending requests map.
   *   - If promise is successfully emplaced in pending request map:
   *     - Construct a new future associated with the emplaced promise including a DestructionAction used.
   * \endinternal
   */
  auto StoreRequest(SessionId const session_id) noexcept -> ::amsr::core::Optional<::amsr::core::Future<Output>> final {
    ::amsr::core::Optional<::amsr::core::Future<Output>> result{};

    std::lock_guard<std::mutex> const pending_requests_guard{pending_requests_lock_};

    std::pair<typename MapType::iterator, bool> const pending_request_stored{
        // VCA_IPCB_STD_FUNCTION_CALLED
        pending_requests_.emplace(session_id.value, ::amsr::core::Promise<Output>{})};

    if (pending_request_stored.second) {
      typename MapType::iterator const iterator{pending_request_stored.first};

      // VECTOR NL AutosarC++17_10-A18.5.8: MD_IPCBINDING_AutosarC++17_10_A18.5.8_false_positive
      amsr::UniquePtr<DestructionAction<Output>> destruction_action{
          amsr::tmp::MakeUnique<DestructionAction<Output>>(this->SharedFromThis().Value(), session_id).Value()};

      // VCA_IPCB_PROXY_REQUEST_HANDLER_OPTIONAL_SET
      result.emplace(iterator->second.get_future(
          // VECTOR NC AutosarC++17_10-A5.1.5: MD_IPCBINDING_AutosarC++17_10-A5.1.5_false_positive
          // VCA_IPCB_PROXY_REQUEST_HANDLER_CALL_OF_DESTRUCTION_ACTION_FUNCTOR
          [destruction_action = std::move(destruction_action)]() { (*destruction_action)(); }));
    }

    return result;
  }

  /*!
   * \copydoc PendingRequestMapInterface::MoveOutRequest()
   *
   * \internal
   * - Prevent the request map from concurrent access.
   * - Search for the request with specific key in the pending requests.
   * - Check if a request was found.
   *   - Store the promise of the found request.
   *   - Delete the entry of the request in the pending request map.
   * - Return a pair of promise and a bool indicating the success/failure.
   * \endinternal
   */
  auto MoveOutRequest(SessionId const session_id) noexcept -> ::amsr::core::Optional<Promise> final {
    std::lock_guard<std::mutex> const pending_requests_guard{pending_requests_lock_};

    typename MapType::iterator const request_entry{pending_requests_.find(session_id.value)};

    ::amsr::core::Optional<Promise> optional_promise{};

    // Access the request only, if there is an entry available
    if (request_entry != pending_requests_.end()) {
      // VCA_IPCB_PROMISE_SAFE_FUNCTION, VCA_IPCB_VALID_PROMISE_ARGUMENT
      optional_promise.emplace(std::move(request_entry->second));

      // Promise moved out of the map before. Value in the map for this key is invalid after moving out and thus the map
      // entry needs to be removed completely.
      // VCA_IPCB_STD_FUNCTION_CALLED
      static_cast<void>(pending_requests_.erase(request_entry));
    }

    return optional_promise;
  }

  /*!
   * \copydoc PendingRequestMapInterface::MoveOutNextRequest()
   *
   * \internal
   * - Prevent the request map from concurrent access.
   * - Search for the next request in the pending requests.
   * - Check if a request was found.
   *   - Store the key and promise of the found request.
   *   - Delete the entry of the request in the pending request map.
   * \endinternal
   */
  auto MoveOutNextRequest() noexcept -> OptionalMapEntry final {
    std::lock_guard<std::mutex> const pending_requests_guard{pending_requests_lock_};

    OptionalMapEntry next_entry{};
    typename MapType::iterator const request_entry{pending_requests_.begin()};
    if (request_entry != pending_requests_.end()) {
      // VCA_IPCB_STD_FUNCTION_CALLED,  VCA_IPCB_VALID_PROMISE_ARGUMENT
      next_entry.emplace(request_entry->first, std::move(request_entry->second));

      // Promise moved out of the map before. Value in the map for this key is invalid after moving out and thus the map
      // entry needs to be removed completely.
      // VCA_IPCB_STD_FUNCTION_CALLED
      static_cast<void>(pending_requests_.erase(request_entry));
    }
    return next_entry;
  }

 private:
  /*!
   * \brief Type of the underlying map for storage of the pending requests.
   */
  using MapType = ::ara::core::Map<typename SessionId::type, Promise>;

  /*!
   * \brief Holds all the pending requests.
   */
  MapType pending_requests_{};

  /*!
   * \brief Used to protect from concurrent access to the pending requests map:
   *        - If requests and responses are handled at the same time.
   *        - If multiple requests are triggered in parallel.
   */
  std::mutex pending_requests_lock_{};
};

}  // namespace methods
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_METHODS_PENDING_REQUEST_MAP_H_

// clang-format off
/*!
 * \exclusivearea amsr::ipc_binding_core::internal:::methods::PendingRequestMap::pending_requests_lock_
 *                Ensures consistency while read and write access to the map of pending requests.
 *
 * \protects amsr::ipc_binding_core::internal:::methods::PendingRequestMap::pending_requests_
 *
 * \usedin amsr::ipc_binding_core::internal:::methods::PendingRequestMap::StoreRequest
 * \usedin amsr::ipc_binding_core::internal:::methods::PendingRequestMap::MoveOutRequest
 * \usedin amsr::ipc_binding_core::internal:::methods::PendingRequestMap::MoveOutNextRequest
 *
 * \length MEDIUM   Limited to a map lookup, an emplacement if a request is added to the map or a removal of a request
 *                  entry from the map if a response was received. The exclusive area starts and ends always in the
 *                  smallest possible scope within the protected methods. This exclusive area covers calls to at most
 *                  one method. The complexity is logarithmic in the size of the container.
 * \endexclusivearea
 */
// clang-format on
