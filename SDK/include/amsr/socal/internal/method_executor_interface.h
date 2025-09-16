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
/**        \file  socal/internal/method_executor_interface.h
 *        \brief  Interface defining the method execution APIs for the Skeleton side.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHOD_EXECUTOR_INTERFACE_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHOD_EXECUTOR_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/core/future.h"
#include "amsr/socal/internal/task.h"
#include "amsr/unique_ptr.h"

namespace amsr {
namespace socal {
namespace internal {

/*!
 * \brief Class interface defining the APIs for method execution on the skeleton side.
 */
class MethodExecutorInterface {
 public:
  /*!
   * \brief Default constructor.
   * \steady FALSE
   */
  MethodExecutorInterface() noexcept = default;

  MethodExecutorInterface(MethodExecutorInterface const&) = delete;
  MethodExecutorInterface(MethodExecutorInterface&) = delete;
  MethodExecutorInterface(MethodExecutorInterface&&) noexcept = delete;
  MethodExecutorInterface& operator=(MethodExecutorInterface const&) & = delete;
  MethodExecutorInterface& operator=(MethodExecutorInterface&&) & noexcept = delete;

  /*!
   * \brief Virtual default destructor.
   * \pre Cleanup must be called prior to the destruction.
   * \steady FALSE
   */
  virtual ~MethodExecutorInterface() noexcept = default;

  /*!
   * \brief   Enqueues an incoming method request with its parameters.
   * \details A binding-specific implementation shall not know anything about the processing-mode chosen in the
   *          frontend. The frontend decides how to proceed based on the processing-mode given in the constructor of
   *          this object.
   * \param[in] request  This object contains all the information necessary to:
   *                     1. Call the concrete method in the frontend; the binding is aware of the method to call and
   *                        stores this information in the functor.
   *                     2. Get the return value with a given type from the future, because the return type is also
   *                        known to the backend.
   *                     3. Call the response path in the binding-related part with the information provided within this
   *                        object.
   * \return true if a request was enqueued, false if not.
   * \pre         -
   * \context     Backend
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   *              TRUE against ExecuteNextMethodCall API for same/different class instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances in ThreadDriven runtime processing mode,
   *              TRUE for different instances in Polling runtime processing mode.
   * \synchronous FALSE
   * \steady TRUE
   * \spec requires request != nullptr; \endspec
   */
  virtual auto HandleMethodRequest(amsr::UniquePtr<Task>&& request) noexcept -> bool = 0;

  /*!
   * \brief Fetches the next method call from the method queue and executes it.
   * \return The future contains a boolean value of true, if there is a method request pending in the global request
   *         queue, which is being processed, false if there was no method request pending. The promise's value is set
   *         as soon as the request has been processed.
   * \pre         -
   * \context     App | Callback
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   *              TRUE against HandleMethodRequest API for same/different class instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \steady TRUE
   */
  virtual auto ExecuteNextMethodCall() noexcept -> ::amsr::core::Future<bool> = 0;

  /*!
   * \brief Execute cleanup of thread-pool tasks which are created by the method executor.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances in ThreadDriven runtime processing mode,
   *              TRUE for different instances in Polling runtime processing mode.
   * \synchronous TRUE
   * \spec requires true; \endspec
   */
  virtual void Cleanup() noexcept = 0;
};

}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHOD_EXECUTOR_INTERFACE_H_
