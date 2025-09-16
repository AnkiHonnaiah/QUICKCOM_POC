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
/*!        \file  someip_binding_core/internal/methods/method_cancellation_task.h
 *        \brief  Task to perform cleanup of a pending method request promise
 *        \unit   SomeIpBinding::SomeIpBindingCore::Client::Methods::MethodCancellationTask
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_METHOD_CANCELLATION_TASK_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_METHOD_CANCELLATION_TASK_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/someip_binding_core/internal/methods/pending_request_map_interface.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {
namespace methods {

/*!
 * \brief A task to perform cleaning-up of cancelled method requests.
 * \tparam Output return type of the cancelled method.
 */
template <typename Output>
class MethodCancellationTask final {
 public:
  /*!
   * \brief Alias for someip_protocol::internal::SessionId for easier access.
   */
  using SessionId = someip_protocol::internal::SessionId;

  /*!
   * \brief Template instantiation for PendingRequestMapInterface.
   */
  using PendingRequestMapInterfaceType = PendingRequestMapInterface<Output>;

  /*!
   * \brief Alias for shared pointer of type: PendingRequestMapInterfaceType.
   */
  using PendingRequestMapInterfaceSharedPtr = std::shared_ptr<PendingRequestMapInterfaceType>;

  /*!
   * \brief Alias for weak pointer of type: PendingRequestMapInterfaceType.
   */
  using PendingRequestMapInterfaceWeakPtr = std::weak_ptr<PendingRequestMapInterfaceType>;

  /*!
   * \brief Create MethodCancellationTask object.
   * \param[in] key session ID for the method request.
   * \param[in] pending_request_map_weak_ptr weak pointer to the pending request map object.
   * \spec
   *   requires pending_request_map_weak_ptr != nullptr;
   * \endspec
   * \steady    TRUE
   */
  MethodCancellationTask(SessionId const key,
                         PendingRequestMapInterfaceWeakPtr const pending_request_map_weak_ptr) noexcept
      : key_{key}, promise_map_{pending_request_map_weak_ptr} {}  // VCA_SOMEIPBINDING_POSSIBLY_INVALID_ARGUMENT

  MethodCancellationTask(MethodCancellationTask const&) = delete;
  MethodCancellationTask& operator=(MethodCancellationTask const&) & = delete;
  MethodCancellationTask& operator=(MethodCancellationTask&&) & = delete;
  MethodCancellationTask(MethodCancellationTask&&) = delete;

  /*!
   * \brief Default destructor.
   * \spec
   *   requires true;
   * \endspec
   * \steady    TRUE
   */
  // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_DESTRUCTOR
  ~MethodCancellationTask() noexcept = default;

  /*!
   * \brief Destroy the promise.
   * \pre               -
   * \context           App
   * \threadsafe        FALSE
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \steady            TRUE
   * \exceptionsafety   STRONG No undefined behavior or side effects
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Acquire temporary ownership of the request map.
   * - If the shared request map object still exists, call MoveOutRequest
   * \endinternal
   */
  void operator()() const noexcept {
    // VECTOR NL AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10-A18.5.8_Local_object_in_heap
    PendingRequestMapInterfaceSharedPtr const pending_request_map_shared_ptr{promise_map_.lock()};
    if (pending_request_map_shared_ptr) {
      // VECTOR NC AutosarC++17_10-M0.3.1: MD_SOMEIPBINDING_AutosarC++17_10-M0.3.1_ptr_null_at_dereference
      // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_AFTER_NULLPTR_CHECK
      static_cast<void>(pending_request_map_shared_ptr->MoveOutRequest(key_));
    }
  };

 private:
  /*!
   * \brief Session ID used as a key for the cancelled request.
   */
  SessionId key_;

  /*!
   * \brief Weak pointer to the map of pending method requests.
   */
  PendingRequestMapInterfaceWeakPtr promise_map_;
};

}  // namespace methods
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_METHOD_CANCELLATION_TASK_H_
