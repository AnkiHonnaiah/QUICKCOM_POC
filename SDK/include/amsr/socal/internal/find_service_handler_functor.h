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
/**        \file  find_service_handler_functor.h
 *        \brief  Specialized Threadpool functor to handle StartFindService requests asynchronously.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_FIND_SERVICE_HANDLER_FUNCTOR_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_FIND_SERVICE_HANDLER_FUNCTOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/generic/singleton_wrapper.h"
#include "amsr/socal/internal/service_handle_explorer.h"
#include "find_service_observers_manager.h"

namespace amsr {
namespace socal {
namespace internal {

/*!
 * \brief Task class meant to invoke the FindServiceHandler callback in the thread pool context.
 *
 * \unit Socal::Proxy::ProxyServiceDiscovery::AsynchronousProxyFindService
 */
// VCA_SOCAL_FREEING_STATICALLY_ALLOCATED_MEMORY
class FindServiceHandlerFunctor final {
 public:
  /*!
   * \brief Type alias for the SingletonAccess of FindServiceObserversManager.
   */
  using ObserverManagerAccess = ::amsr::generic::SingletonAccess<FindServiceObserversManager>;

  /*!
   * \brief Constructs the FindServiceHandlerFunctor.
   * \param[in] observer          Observer that should be triggered in the callback context.
   * \param[in] observer_manager  ObserverManager that needs to be triggered to update listeners.
   * \context App
   * \steady FALSE
   * \spec requires observer != nullptr; \endspec
   */
  explicit FindServiceHandlerFunctor(ServiceHandleExplorer const& observer,
                                     ObserverManagerAccess observer_manager) noexcept;

  /*!
   * \brief Destructor.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ~FindServiceHandlerFunctor() noexcept = default;

  /*!
   * \brief Move constructor.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  FindServiceHandlerFunctor(FindServiceHandlerFunctor&&) noexcept = default;

  /*!
   * \brief Delete copy constructor.
   */
  FindServiceHandlerFunctor(FindServiceHandlerFunctor const&) noexcept = delete;

  /*!
   * \brief Delete copy assignment operator.
   */
  auto operator=(FindServiceHandlerFunctor const&) & noexcept -> FindServiceHandlerFunctor& = delete;

  /*!
   * \brief Delete move assignment operator.
   */
  auto operator=(FindServiceHandlerFunctor&&) & noexcept -> FindServiceHandlerFunctor& = delete;

  /*!
   * \brief   Updates the observers in the observer manager and executes the find service handler.
   * \details Shall be called in the service discovery threadpool context.
   * \context     Callback
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \spec requires true; \endspec
   * \steady FALSE
   */
  void operator()() noexcept;

 private:
  /*!
   * \brief The observer that should be triggered when a service was found.
   */
  ServiceHandleExplorer observer_;

  /*!
   * \brief Access to the find service observers manager.
   */
  ObserverManagerAccess observer_manager_;
};

}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_FIND_SERVICE_HANDLER_FUNCTOR_H_
