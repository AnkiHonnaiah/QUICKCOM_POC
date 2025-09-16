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
/**        \file  continuation_functor.h
 *        \brief  Class to store and execute callback from ara::core::Future::then.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_CONTINUATION_FUNCTOR_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_CONTINUATION_FUNCTOR_H_

#include "vac/language/unique_function.h"

namespace amsr {
namespace socal {
namespace internal {
namespace methods {

/*!
 * \brief Functor to execute a callback handed over from ara::core::Future::then.
 *
 * \unit Socal::Proxy::ProxyMethod::FutureExecutor::EventModeFutureExecutor
 */
// VCA_SOCAL_FREEING_STATICALLY_ALLOCATED_MEMORY
class ContinuationFunctor final {
 public:
  /*!
   * \brief Type alias for the work item submitted to this executor.
   */
  using WorkItemType = ::vac::language::UniqueFunction<void()>;

  /*!
   * \brief Construct a new ContinuationFunctor.
   * \param[in] work_item  The callback function which shall be triggered on task execution.
   * \steady TRUE
   */
  explicit ContinuationFunctor(WorkItemType&& work_item) noexcept;

  /*!
   * \brief Delete copy constructor.
   */
  ContinuationFunctor(ContinuationFunctor const&) noexcept = delete;

  /*!
   * \brief Delete copy assignment operator.
   */
  ContinuationFunctor& operator=(ContinuationFunctor const&) noexcept = delete;

  /*!
   * \brief Move constructor.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   */
  ContinuationFunctor(ContinuationFunctor&&) noexcept = default;

  /*!
   * \brief Delete move assignment operator.
   */
  auto operator=(ContinuationFunctor&&) & noexcept -> ContinuationFunctor& = delete;

  /*!
   * \brief Destroys the ContinuationFunctor.
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  ~ContinuationFunctor() noexcept = default;

  /*!
   * \brief   Execute the ara::core::Future::then callback.
   * \details Shall be called in the continuation threadpool context.
   * \context     Callback
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   * \internal
   * - Execute callback.
   * \endinternal
   */
  void operator()() noexcept;

 private:
  /*!
   * \brief The callback function which shall be executed.
   */
  WorkItemType work_item_;
};

}  // namespace methods
}  // namespace internal
}  // namespace socal
}  // namespace amsr
#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_CONTINUATION_FUNCTOR_H_
