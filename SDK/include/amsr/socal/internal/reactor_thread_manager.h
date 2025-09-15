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
/*!        \file  reactor_thread_manager.h
 *        \brief  This module is responsible for initialization and de-initialization of the reactor thread.
 *      \details
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_REACTOR_THREAD_MANAGER_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_REACTOR_THREAD_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>

#include "amsr/generic/singleton_wrapper.h"
#include "amsr/socal/external/thread/thread.h"
#include "amsr/socal/internal/logging/ara_com_logger.h"
#include "amsr/socal/internal/runtime_interface.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace socal {
namespace internal {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOCAL_AutosarC++17_10-M7.3.6_usingDirectiveInHeaderFile
using vac::container::literals::operator""_sv;  // NOLINT(build/namespaces)

/*!
 * \brief The reactor thread manager class.
 *
 * \complexity Multiple inter module calls towards thread and libosabstraction are required for the initialization of
 *             the reactor thread. No risk was identified.
 * \unit       Socal::Runtime::ReactorThreadManager
 */
class ReactorThreadManager final {
  /*!
   * \brief Used Thread type.
   */
  using ThreadType = ::amsr::socal::external::thread::Thread;
  /*!
   * \brief Type alias for the singleton access for RuntimeInterface.
   */
  using RuntimeSingletonAccess = ::amsr::generic::SingletonAccess<::amsr::socal::internal::RuntimeInterface>;

 public:
  /*!
   * \brief Returns a singleton instance of ReactorThreadManager class.
   * \return Singleton wrapped ReactorThreadManager instance.
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   */
  static ::amsr::generic::Singleton<ReactorThreadManager>& getSingletonInstance() noexcept;

  /*!
   * \brief Initializes the reactor thread.
   * \note  The constructor cannot be made private because it is used by LibVac (Singleton wrapper class) to create the
   *        instance of ReactorThreadManager.
   * \param[in] runtime  Singleton access to the Runtime.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  explicit ReactorThreadManager(RuntimeSingletonAccess const& runtime) noexcept;

  /*!
   * \brief Deinitializes the reactor thread.
   * \note  The destructor cannot be made private because it is used by LibVac (Singleton wrapper class) to destroy the
   *        instance of ReactorThreadManager.
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ~ReactorThreadManager() noexcept;

  /*!
   * \brief Delete the default copy constructor.
   */
  ReactorThreadManager(ReactorThreadManager const&) = delete;
  /*!
   * \brief Delete the default move constructor.
   */
  ReactorThreadManager(ReactorThreadManager&&) = delete;
  /*!
   * \brief Delete the default copy assignment operator.
   */
  ReactorThreadManager& operator=(ReactorThreadManager const&) & = delete;
  /*!
   * \brief Delete the default move assignment operator.
   */
  ReactorThreadManager& operator=(ReactorThreadManager&&) & = delete;

 private:
  /*!
   * \brief Initializes the reactor thread.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  void InitializeReactorThread() noexcept;

  /*!
   * \brief De-initializes the reactor thread.
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  void DeinitializeReactorThread() noexcept;

  /*!
   * \brief An instance of a logger.
   */
  ::amsr::socal::internal::logging::AraComLogger logger_{
      ::amsr::socal::internal::logging::kAraComLoggerContextId,
      ::amsr::socal::internal::logging::kAraComLoggerContextDescription, "ReactorThreadManager"_sv};

  /*!
   * \brief Flag to indicate if reactor thread can be still active.
   */
  std::atomic<bool> keep_running_{false};

  /*!
   * \brief Thread that drives the reactor.
   */
  // VCA_SOCAL_EXTERNAL_FUNCTION_CONTRACT
  ThreadType reactor_thread_{};

  /*!
   * \brief   Singleton access to the Runtime.
   * \details Destruction of Runtime singleton object is not possible unless this access object is destroyed.
   */
  RuntimeSingletonAccess runtime_;
};

}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_REACTOR_THREAD_MANAGER_H_
