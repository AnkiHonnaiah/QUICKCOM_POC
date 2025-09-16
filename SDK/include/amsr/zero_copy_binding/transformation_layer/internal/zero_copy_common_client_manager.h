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
 *         \brief  A helper class that manages ZeroCopyCommon clients together with their connections.
 *
 *       \details  This class is needed for the error handling of the proxy event backend.
 *                 When the connection to a peer ZeroCopyCommon client is terminated a new client will be created, but
 *                 the old one has to live until all of its slots have been returned.
 *
 *       \unit     ZeroCopyBinding::ZeroCopyBindingTransformationLayer::ZeroCopyCommonClientManager
 *
 * \complexity The high numer of inter-module calls is caused by the usage of ReactorSoftwareEvents for clean up the
 *             list of clients. This is a design decision and it is considered safe.

 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_ZERO_COPY_COMMON_CLIENT_MANAGER_H_
#define LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_ZERO_COPY_COMMON_CLIENT_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <list>
#include <memory>
#include <mutex>
#include <utility>

#include "amsr/core/optional.h"
#include "amsr/ipc_service_discovery/common/internal/reactor_software_event/reactor_software_event.h"
#include "amsr/zero_copy_binding/core/internal/connection_wrapper_interface.h"
#include "amsr/zero_copy_binding/core/internal/logging/zero_copy_logger.h"
#include "amsr/zero_copy_common/internal/zero_copy_common_client.h"
#include "ara/core/vector.h"
#include "osabstraction/io/reactor1/reactor1.h"

namespace amsr {
namespace zero_copy_binding {
namespace transformation_layer {
namespace internal {

/*!
 * \brief   A helper class managing the various ZeroCopyCommon clients of a single event backend that can exist due to
            error handling.
 * \details Correct use of the class requires that the following methods are always called in the specified order:
 *          AddConnectionWrapper() -> AddZeroCopyCommonClient() -> RetireActiveZeroCopyCommonClient()
 *          or
 *          AddConnectionWrapper() -> RetireActiveZeroCopyCommonClient().
 */
class ZeroCopyCommonClientManager final {
  using ClientInterface = ::amsr::zero_copy_common::internal::ClientInterface;
  using ConnectionWrapperInterface = core::internal::ConnectionWrapperInterface;
  using ReactorSoftwareEvent = ::amsr::ipc_service_discovery::common::internal::ReactorSoftwareEvent;

 public:
  /*!
   * \brief The default constructor.
   */
  ZeroCopyCommonClientManager() noexcept = default;

  /*!
   * \brief       The destructor.
   * \context     ANY
   * \pre         The object has to be empty, i.e., no ZeroCopyCommon client is used anymore.
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ~ZeroCopyCommonClientManager() noexcept;

  ZeroCopyCommonClientManager(ZeroCopyCommonClientManager const&) = delete;
  ZeroCopyCommonClientManager(ZeroCopyCommonClientManager&&) = delete;
  auto operator=(ZeroCopyCommonClientManager const&) -> ZeroCopyCommonClientManager& = delete;
  auto operator=(ZeroCopyCommonClientManager&&) -> ZeroCopyCommonClientManager& = delete;

  /*!
   * \brief       Add a new connection wrapper.
   * \param[in]   connection_wrapper  A unique pointer to the connection wrapper to be added.
   *                                  Must not be a nullptr.
   * \return      A reference to the connection wrapper just added.
   * \context     ANY
   * \pre         There is no active ZeroCopyCommon client managed by this class.
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto AddConnectionWrapper(std::unique_ptr<ConnectionWrapperInterface> connection_wrapper) noexcept
      -> ConnectionWrapperInterface&;

  /*!
   * \brief       Add a new ZeroCopyCommon client.
   * \param[in]   zero_copy_common_client_ptr  A unique pointer to the ZeroCopyCommon client to be added.
   *                                           Must not be a nullptr.
   * \return      A shared pointer to the ZeroCopyCommon client just added.
   * \context     ANY
   * \pre         There is no active ZeroCopyCommon client managed by this class.
   * \pre         A connection has been added previously by AddConnectionWrapper().
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto AddZeroCopyCommonClient(std::unique_ptr<ClientInterface> zero_copy_common_client_ptr) noexcept
      -> std::shared_ptr<ClientInterface>;

  /*!
   * \brief       Retire the active ZeroCopyCommon client along with its connection.
   * \context     ANY
   * \pre         The manager currently stores a connection and optionally a ZeroCopyCommon client.
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   *
   * \spec
   *   requires true;
   * \endspec
   */
  void RetireActiveZeroCopyCommonClient() noexcept;

  /*!
   * \brief       Return a reference to the connection wrapper around the currently active connection.
   * \return      A reference to the stored connection wrapper.
   * \context     ANY
   * \pre         A connection wrapper has been added previously by AddConnectionWrapper().
   * \pre         The returned reference is only allowed to be used by the ZeroCopyCommon client after a call to
   *              RetireActiveZeroCopyCommonClient().
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetActiveConnectionWrapper() const noexcept -> ConnectionWrapperInterface&;

  /*!
   * \brief       Return a shared pointer to the ZeroCopyCommon client that is currently active.
   * \return      A shared pointer to the stored ZeroCopyCommon client.
   * \context     ANY
   * \pre         A ZeroCopyCommon client has been added previously by AddZeroCopyCommonClient().
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetActiveZeroCopyCommonClient() const noexcept -> std::shared_ptr<ClientInterface> const;

  /*!
   * \brief       Determine whether the ZeroCopyCommonClientManager contains an active connection wrapper.
   * \return      A boolean indicating whether there is an active connection wrapper.
   * \context     ANY
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto ContainsActiveConnectionWrapper() const noexcept -> bool;

  /*!
   * \brief       Determine whether the ZeroCopyCommonClientManager contains an active ZeroCopyCommon client.
   * \return      A boolean indicating whether there is an active ZeroCopyCommon client.
   * \context     ANY
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto ContainsActiveZeroCopyCommonClient() const noexcept -> bool;

  /*!
   * \brief       Determine whether the ZeroCopyCommonClientManager contains any connection wrappers and ZeroCopyCommon
                  clients.
   * \return      A boolean indicating whether there are managed connection wrappers and ZeroCopyCommon clients.
   * \context     ANY
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto Empty() const noexcept -> bool;

  /*!
   * \brief       Remove all unused ZeroCopyCommon clients along with their connection wrappers.
   * \details     The method will take care of closing the connection. However, the connection might already be closed
   *              by the time the method is called.
   * \details     It should be avoided to use the overload without the reactor argument, as this one does wait on the
   *              connection wrapper as well as the ZeroCopyCommon client. The other member functions are blocked during
   *              a call to GarbageCollect().
   * \param[in]   reactor Reactor to be used for waiting until the connection and ZeroCopyCommon clients can be
   *                      destroyed. If not set, the waiting will be done in the current context.
   * \return      The amount of retired ZeroCopyCommon clients that are still in use.
   * \context     ANY (In callbacks of the connection or ZeroCopyCommon client, the reactor has to be passed.)
   * \pre         The ZeroCopyCommon client manager will call Disconnect() on the ZeroCopyCommon client here.
   *              This means that the latter must not have been called from outside by the user before.
   *              As all ZeroCopyCommon clients eventually get destructed by this method, only the ZeroCopyCommon client
   *              manager is allowed to call Disconnect() on the ZeroCopyCommon client.
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  auto GarbageCollect(::osabstraction::io::reactor1::Reactor1Interface* reactor = nullptr) noexcept -> std::size_t;

  /*!
   * \brief       Return the number of the current generation.
   * \details     The generations start with 0.
   *              The counter will be increased upon every call to RetireActiveZeroCopyCommonClient().
   * \return      The current generation value.
   * \context     ANY
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetCurrentGeneration() const noexcept -> std::size_t;

  /*!
   * \brief       Wait until a cleanup callback signals its completion.
   * \param[in]   timeout The maximum amount of time to wait for in milliseconds.
   * \return      A status variable indicating whether a timeout happened.
   * \context     ANY
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto WaitOnCleanupFor(std::chrono::milliseconds timeout) noexcept -> std::cv_status;

 private:
  /*!
   * \brief       Determine whether the ZeroCopyCommonClientManager contains any connection wrappers and ZeroCopyCommon
                  clients.
   * \details     Method is not protected by a mutex to avoid deadlocks. The user has to call Empty() instead.
   * \return      A boolean indicating whether there are managed connection wrappers and ZeroCopyCommon clients.
   * \context     ANY
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  auto EmptyInternal() const noexcept -> bool;

  /*!
   * \brief An enumeration for tracking the current state of the ZeroCopyCommonClientManager.
   */
  enum class ZeroCopyCommonState : std::uint8_t {
    kInactive,
    kConnected,
    kActive
  } zero_copy_common_state_{ZeroCopyCommonState::kInactive};

  /*!
   * \brief   The connection wrapper that is currently in use.
   * \details Stored separately to exclude it from garbage collection.
   */
  std::unique_ptr<ConnectionWrapperInterface> ipc_connection_wrapper_{nullptr};

  /*!
   * \brief   The ZeroCopyCommon client that is currently in use.
   * \details Stored separately to exclude it from garbage collection.
   */
  std::shared_ptr<ClientInterface> zero_copy_common_client_{nullptr};

  /*!
   * \brief   A vector containing retired ZeroCopyCommon clients, bundled together with their corresponding connection.
   */
  ::ara::core::Vector<std::pair<std::shared_ptr<ClientInterface>, std::unique_ptr<ConnectionWrapperInterface>>>
      retired_zero_copy_common_clients_{};

  /*!
   * \brief The generation counter.
   */
  std::size_t generation_counter_{0U};

  /*!
   * \brief The currently ongoing cleanup events.
   */
  std::list<::amsr::core::Optional<ReactorSoftwareEvent>> cleanup_events_{};

  /*!
   * \brief A mutex to protect the ZeroCopyCommonClientManager against parallel access.
   */
  std::mutex mutable zero_copy_common_client_manager_lock_{};

  /*!
   * \brief A conditional variable to signal when a cleanup callback has finished.
   */
  std::condition_variable cleanup_cv_{};  // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT

  /*!
   * \brief The time period that is waited between each check of IsInUse() upon cleanup.
   */
  static std::chrono::milliseconds constexpr kCleanupDelay{10};

  /*!
   * \brief The Logger.
   */
  // VCA_ZEROCOPYBINDING_SLC_20_LOGGER
  core::internal::logging::ZeroCopyLogger logger_{"ZeroCopyCommonClientManager"};
};

// clang-format off
/*!
 * \exclusivearea ::amsr::zero_copy_binding::transformation_layer::internal::ZeroCopyCommonClientManager::zero_copy_common_client_manager_lock_
 *                Serializes all ZeroCopyCommonClientManager API calls.
 *
 * \protects All members of an ZeroCopyCommonClientManager object.
 * \usedin   ::amsr::zero_copy_binding::transformation_layer::internal::ZeroCopyCommonClientManager::~ZeroCopyCommonClientManager
 *           ::amsr::zero_copy_binding::transformation_layer::internal::ZeroCopyCommonClientManager::AddConnectionWrapper
 *           ::amsr::zero_copy_binding::transformation_layer::internal::ZeroCopyCommonClientManager::AddZeroCopyCommonClient
 *           ::amsr::zero_copy_binding::transformation_layer::internal::ZeroCopyCommonClientManager::RetireActiveZeroCopyCommonClient
 *           ::amsr::zero_copy_binding::transformation_layer::internal::ZeroCopyCommonClientManager::GetActiveConnectionWrapper
 *           ::amsr::zero_copy_binding::transformation_layer::internal::ZeroCopyCommonClientManager::GetActiveZeroCopyCommonClient
 *           ::amsr::zero_copy_binding::transformation_layer::internal::ZeroCopyCommonClientManager::ContainsActiveConnectionWrapper
 *           ::amsr::zero_copy_binding::transformation_layer::internal::ZeroCopyCommonClientManager::ContainsActiveZeroCopyCommonClient
 *           ::amsr::zero_copy_binding::transformation_layer::internal::ZeroCopyCommonClientManager::Empty
 *           ::amsr::zero_copy_binding::transformation_layer::internal::ZeroCopyCommonClientManager::GarbageCollect
 *           ::amsr::zero_copy_binding::transformation_layer::internal::ZeroCopyCommonClientManager::GetCurrentGeneration
 * \exclude  -
 * \length   MEDIUM The lock is held during adding and removing the connection wrapper and the ZeroCopyCommon client and
 *           checking the ZeroCopyCommonClientManager's state. The lock is held during the whole function call, however
 *           the most methods have a comparably short execution time. The only exception is the overload of
 *           GarbageCollect() not taking arguments, which does wait.
 * \endexclusivearea
 */
// clang-format on

}  // namespace internal
}  // namespace transformation_layer
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_ZERO_COPY_COMMON_CLIENT_MANAGER_H_
