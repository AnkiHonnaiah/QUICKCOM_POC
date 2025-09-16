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
/*!        \file  thread_safe_e2e_result.h
 *        \brief  Holds an e2e result and provides thread safe read-write access to it
 *        \unit   SomeIpBinding::SomeIpBindingXf::Communication::Events::ProxyEventXf
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_THREAD_SAFE_E2E_RESULT_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_THREAD_SAFE_E2E_RESULT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <mutex>
#include "ara/com/e2e_types.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace events {

/*!
 * \brief Container to read and store an E2EResult using locking to provide atomic read/write access to it.
 */
class ThreadSafeE2eResult final {
 public:
  /*!
   * \brief       Constructor.
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
  ThreadSafeE2eResult() noexcept;

  /*!
   * \brief       Default destructor.
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
  ~ThreadSafeE2eResult() noexcept = default;

  /*!
   * \brief       Gets the last stored E2E result.
   * \return      The E2E result
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ::ara::com::e2e::Result operator()() const noexcept;

  /*!
   * \brief Updates the e2e_result using a lock to avoid inconsistent state
   * \param[in] e2e_result the new E2eResult value which is assigned
   * \return      Updated E2E result.
   * \pre         -
   * \context     Reactor
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ThreadSafeE2eResult& operator=(::ara::com::e2e::Result const& e2e_result) & noexcept;

  ThreadSafeE2eResult(ThreadSafeE2eResult const&) = delete;
  ThreadSafeE2eResult(ThreadSafeE2eResult&&) = delete;
  ThreadSafeE2eResult& operator=(ThreadSafeE2eResult const&) = delete;
  ThreadSafeE2eResult& operator=(ThreadSafeE2eResult&&) = delete;

 private:
  /*!
   * \brief Mutex to protect e2e state
   */
  mutable std::mutex e2e_result_lock_{};

  /*!
   * \brief E2E result containing the E2EState and E2ECheckStatus.
   */
  ::ara::com::e2e::Result e2e_result_;
};

}  // namespace events
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_THREAD_SAFE_E2E_RESULT_H_

/*!
 * \exclusivearea ::amsr::someip_binding_xf::internal::events::ThreadSafeE2eResult::e2e_result_lock_
 *                Used for protecting parallel access to the e2e state by App (read) and Reactor (write).
 *
 * \protects ::amsr::someip_binding_xf::internal::events::ThreadSafeE2eResult::e2e_result_
 *
 * \usedin ::amsr::someip_binding_xf::internal::events::ThreadSafeE2eResult::UpdateE2eResult
 * \usedin ::amsr::someip_binding_xf::internal::events::ThreadSafeE2eResult::GetE2EResult
 *
 * \exclude User threads/Reactor thread from accessing a variable that is already in access.
 * \length small only used to access one variable consistently.
 * \endexclusivearea
 */
