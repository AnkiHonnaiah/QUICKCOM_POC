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
/*!        \file  someip_binding_core/internal/methods/pending_request_map.h
 *        \brief  Method request storage
 *        \unit   SomeIpBinding::SomeIpBindingCore::Client::Methods::PendingRequestMap
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_PENDING_REQUEST_MAP_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_PENDING_REQUEST_MAP_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>
#include "amsr/core/future.h"
#include "amsr/core/optional.h"
#include "amsr/someip_binding_core/internal/communication_error_domain.h"
#include "amsr/someip_binding_core/internal/methods/method_cancellation_task.h"
#include "amsr/someip_binding_core/internal/methods/pending_request_map_interface.h"
#include "ara/core/map.h"
#include "ara/core/promise.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {
namespace methods {

/*!
 * \brief Provides functionality to store method requests into a map and restore requests on asynchronous
 * response.
 * \details Protects the map that stores the pending requests from concurrent access, as there could be multiple
 * requests triggered in parallel or a request and response at the same time.
 * \tparam Output Response result value (wrapped in a Promise) to store a positive response
 */
template <typename Output>
// VCA_SPC_15_SOMEIPBINDING_STL_TYPE_DESTRUCTOR
class PendingRequestMap final : public PendingRequestMapInterface<Output>,
                                public std::enable_shared_from_this<PendingRequestMap<Output>> {
 public:
  /*!
   * \brief RequestKey type.
   */
  using RequestKey = ::amsr::someip_protocol::internal::SessionId;
  /*!
   * \brief Value of the map. Objects of this type are stored for each method request in the map.
   */
  using ResponseValuePromise = ::ara::core::Promise<Output>;

  /*!
   * \brief Optional of the response promise type.
   */
  using ResponsePromiseOptional = ::amsr::core::Optional<ResponseValuePromise>;

  /*!
   * \brief Map containing the method request entries.
   */
  using MapType = ara::core::Map<RequestKey, ResponseValuePromise>;

  /*!
   * \brief Future of the response value type.
   */
  using ResponseValueFuture = ::amsr::core::Future<Output>;

  /*!
   * Optional of the future type.
   */
  using FutureOptional = ::amsr::core::Optional<ResponseValueFuture>;

  /*!
   * \brief Store a request for the asynchronous response into the map.
   * \details Possible reasons for a failing emplacement:
   * 1) The request entry is not stored in the map, if there is already a request entry with the identical key existing.
   * 2) Memory allocation fails
   *
   * \param[in] request_key The request will be stored under this key.
   * \return An optional future. (future has value if the request is stored in the map, in case request is not emplaced
   * in map future will not be valid)
   *
   * \pre              -
   * \context          App
   * \threadsafe       TRUE
   * \reentrant        FALSE
   * \synchronous      TRUE
   * \steady           TRUE
   * \exceptionsafety  STRONG No undefined behavior or side effects.
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Create a promise of the templated datatype
   * - Emplace the promise with the given key into the map of pending requests
   * - If the promise was inserted successfully into the map:
   *   - Create a callable to be executed on future destruction.
   *   - Get a future from the promise using the created callable.
   *   - Return the future.
   * \endinternal
   */
  FutureOptional StoreRequestInMap(RequestKey const& request_key) noexcept {
    std::lock_guard<std::mutex> const protect_access{map_access_protection_};

    ResponseValuePromise promise{};
    FutureOptional optional_future{};
    std::pair<typename MapType::iterator, bool> const pending_request_stored{
        // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
        pending_requests_.emplace(request_key, std::move(promise))};

    if (pending_request_stored.second) {
      typename MapType::iterator const iterator{pending_request_stored.first};
      ResponseValuePromise& moved_promise{iterator->second};

      std::weak_ptr<PendingRequestMapInterface<Output>> const pending_request_map_interface{this->shared_from_this()};
      ::vac::language::UniqueFunction<void(), ara::core::PolymorphicAllocator<std::uint8_t>> destruction_callable{
          // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
          [request_key, pending_request_map_interface]() {
            // MethodCancellationTask template specialization
            using MethodCancellationTaskType = MethodCancellationTask<Output>;

            // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
            (MethodCancellationTaskType{request_key, pending_request_map_interface})();
          }};

      // VCA_SOMEIPBINDING_TRIVIAL_VALID_REFERENCE
      optional_future.emplace(moved_promise.get_future(std::move(destruction_callable)));
    }  // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION

    return optional_future;
  }

  /*!
   * \copydoc amsr::someip_binding_core::internal::methods::PendingRequestMapInterface::MoveOutRequest
   * \spec
   *   requires true;
   * \endspec
   * \steady    TRUE
   *
   * \internal
   * - Search for the request with specific key in the pending requests.
   * - If request is found, move out the request from pending requests (also erase it from the map).
   * - Return an optional of moved out request.
   * \endinternal
   */
  ResponsePromiseOptional MoveOutRequest(RequestKey const request_key) noexcept final {
    std::lock_guard<std::mutex> const protect_access{map_access_protection_};

    typename MapType::iterator const request_entry{SearchRequest(request_key)};

    ResponsePromiseOptional optional_promise{};

    // Access the request only, if there is an entry available
    if (request_entry != pending_requests_.end()) {
      // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
      optional_promise = std::move(request_entry->second);

      // Promise moved out of the map before. Value in the map for this key is invalid after moving out and thus the map
      // entry needs to be removed completely.
      // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
      static_cast<void>(pending_requests_.erase(request_entry));
    }

    return optional_promise;
  }

  /*!
   * \brief Reject all pending requests
   *
   * \context          Reactor
   * \threadsafe       TRUE
   * \reentrant        FALSE
   * \synchronous      TRUE
   * \steady           TRUE
   * \exceptionsafety  STRONG No undefined behavior or side effects
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Erase all pending request in the pending requests map
   * \endinternal
   */
  void RejectAll() noexcept {
    std::lock_guard<std::mutex> const protect_access{map_access_protection_};

    typename MapType::iterator request_it{pending_requests_.begin()};

    while (request_it != pending_requests_.end()) {
      // VECTOR NC AutosarC++17_10-A7.1.1: MD_SOMEIPBINDING_AutosarC++17_10-A7.1.1_Immutable_shall_be_qualified_const
      // VECTOR NC AutosarC++17_10-A12.8.3: MD_SOMEIPBINDING_AutosarC++17_10-A12.8.3_ReadAccess_after_move
      // VCA_SOMEIPBINDING_TRIVIAL_VALID_REFERENCE
      ResponseValuePromise promise{std::move(request_it->second)};
      promise.SetError({internal::CommunicationErrc::not_ready, "Service is down."});
      // VCA_SOMEIPBINDING_TRIVIAL_VALID_REFERENCE
      request_it = pending_requests_.erase(request_it);
    }
  }

  /*!
   * \brief Get the current count of pending requests from the request map.
   * \details this is only used for testing
   * \return pending request count.
   * \context     Testing
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \exceptionsafety  STRONG No undefined behavior or side effects.
   * \spec
   *   requires true;
   * \endspec
   */
  std::size_t GetPendingRequestCount() const noexcept { return pending_requests_.size(); }

 private:
  /*!
   * \brief Search for a request entry based on the request key given.
   * \param[in] request_key request key to search for
   * \return An iterator pointing either to a valid request entry (key-value-pair) from the map, if there is a request
   * available or an iterator pointing to the end of the map, to signalize, that there is no request entry found.
   *
   * \pre              A request is already moved to the map
   * \context          App
   * \threadsafe       TRUE
   * \reentrant        FALSE
   * \synchronous      TRUE
   * \steady           TRUE
   * \exceptionsafety  STRONG No undefined behavior or side effects
   */
  typename MapType::iterator SearchRequest(RequestKey const& request_key) noexcept {
    return pending_requests_.find(request_key);
  }

  /*!
   * \brief Holds all the pending requests.
   */
  MapType pending_requests_;

  /*!
   * \brief Used to protect from concurrent access to the map:
   * - if requests and responses are handled at the same time.
   * - if multiple requests are triggered in parallel
   */
  std::mutex map_access_protection_;
};

}  // namespace methods
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_PENDING_REQUEST_MAP_H_

/*!
 * \exclusivearea
 * ::amsr::someip_binding_core::internal::methods::PendingRequestMap::map_access_protection_ Ensures
 * consistency while read and write access to the map of pending requests.
 *
 * \protects ::amsr::someip_binding_core::internal::methods::PendingRequestMap::pending_requests_ is the
 * protected resource of a PendingRequestMap object. The protected resource contains all the pending requests waiting to
 * be resolved as the asynchronous response comes in.
 *
 * \usedin StoreRequestInMap (write), MoveOutRequest (write)
 *
 * \length MEDIUM Limited to a map lookup, an emplacement if a request is added to the map or a removal of a request
 * entry from the map if a response was received. The exclusive area starts and ends always in the smallest possible
 * scope within the protected methods. This exclusive area covers calls to at most one method. The complexity is
 * logarithmic in the size of the container.
 *
 * \endexclusivearea
 */
