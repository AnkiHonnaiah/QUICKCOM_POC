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
 *        \brief  Handles all subscribers for a skeleton event.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_SKELETON_EVENT_SUBSCRIBER_HANDLER_H_
#define LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_SKELETON_EVENT_SUBSCRIBER_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <memory>

#include "amsr/shared_ptr.h"
#include "ara/core/map.h"

#include "ipc_binding_core/internal/connection_manager/connection_skeleton_interface.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"

namespace amsr {
namespace ipc_binding_transformation_layer {
namespace internal {

/*!
 * \brief   Skeleton event subscriber handler.
 * \details Stores all subscribed connection to an event.
 *
 * \unit IpcBinding::IpcBindingTransformationLayer::SkeletonEvent
 */
class SkeletonEventSubscriberHandler final {
 public:
  /*! Alias for ConnectionId. */
  using ConnectionId = ipc_binding_core::internal::connection_manager::ConnectionId;

  /*! Alias for ConnectionPtr. */
  using ConnectionPtr = ipc_binding_core::internal::ConnectionSkeletonPtr;

  /*! Alias for ConnectionWeakPtr. */
  using ConnectionWeakPtr = amsr::WeakPtr<ConnectionPtr::element_type>;

  /*!
   * \brief Construct an event subscriber handler.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  SkeletonEventSubscriberHandler() noexcept = default;

  /*!
   * \brief Default destructor.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  ~SkeletonEventSubscriberHandler() noexcept = default;

  SkeletonEventSubscriberHandler(SkeletonEventSubscriberHandler const&) noexcept = delete;
  auto operator=(SkeletonEventSubscriberHandler const&) noexcept -> SkeletonEventSubscriberHandler& = delete;
  SkeletonEventSubscriberHandler(SkeletonEventSubscriberHandler&&) noexcept = delete;
  auto operator=(SkeletonEventSubscriberHandler&&) noexcept -> SkeletonEventSubscriberHandler& = delete;

  /*!
   * \brief Add a subscriber.
   *
   * \param[in] connection_id   The connection id.
   * \param[in] connection_ptr  A shared pointer to the connection.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto AddSubscriber(ConnectionId connection_id, ConnectionPtr const& connection_ptr) noexcept -> void;

  /*!
   * \brief Remove a subscriber.
   *
   * \param[in] connection_id  The connection id.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto RemoveSubscriber(ConnectionId connection_id) noexcept -> void;

  /*!
   * \brief Remove all subscriber for that connection.
   *
   * \param[in] connection_id  The connection id.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto RemoveConnection(ConnectionId connection_id) noexcept -> void;

  /*!
   * \brief Remove all subscriber from all connections.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto Clear() noexcept -> void;

  // private block, because iterator need knowledge of SubscriberMap
 private:
  /*! Subscriber entry * \steady FALSE
   */
  // VCA_IPCB_STD_WEAK_POINTER_DESTRUCTOR
  struct Subscriber {
    /*! Alias for type for counter. */
    using CounterType = std::uint32_t;

    /*! Weak pointer to the connection. */
    ConnectionWeakPtr connection{};

    /*! The counter of subscribed events over this connection. */
    CounterType counter{};
  };

  /*! Alias for subscriber map. */
  using SubscriberMap = ara::core::Map<ConnectionId, Subscriber>;

 public:
  /*!
   * \brief Custom iterator to begin iterating over ConnectionWeakPtrs.
   *
   * \unit IpcBinding::IpcBindingTransformationLayer::SkeletonEvent
   */
  class iterator final {
   public:
    /*!
     * \brief Construct an iterator.
     *
     * \param[in,out] it  An iterator to the underlying map.
     *
     * \pre         -
     * \context     App
     * \threadsafe  FALSE
     * \reentrant   FALSE
     * \synchronous TRUE
     * \steady      FALSE
     */
    explicit iterator(SubscriberMap::iterator it) noexcept : it_{it} {}

    iterator(iterator const&) = delete;

    /*!
     * \brief Move construct an iterator.
     *
     * \pre         -
     * \context     App
     * \threadsafe  FALSE
     * \reentrant   FALSE
     * \synchronous TRUE
     * \steady      FALSE
     */
    iterator(iterator&&) noexcept = default;

    auto operator=(iterator const&) -> iterator& = delete;
    auto operator=(iterator&&) noexcept -> iterator& = delete;

    /*!
     * \brief Destruct an iterator.
     *
     * \pre         -
     * \context     App
     * \threadsafe  FALSE
     * \reentrant   FALSE
     * \synchronous TRUE
     * \steady      FALSE
     *
     * \spec requires true; \endspec
     */
    ~iterator() noexcept = default;

    /*!
     * \brief Dereference operator points to the weak_ptr in the underlying map.
     *
     * \return Reference to the weak_ptr.
     *
     * \pre         -
     * \context     App
     * \threadsafe  FALSE
     * \reentrant   FALSE
     * \synchronous TRUE
     * \steady      FALSE
     *
     * \spec requires true; \endspec
     */
    auto operator*() const -> ConnectionWeakPtr& { return it_->second.connection; }

    /*!
     * \brief Not equal to operator.
     *
     * \param[in] other  The other interator.
     *
     * \return True if the values are not equal, and false otherwise.
     *
     * \pre         -
     * \context     App
     * \threadsafe  FALSE
     * \reentrant   FALSE
     * \synchronous TRUE
     * \steady      FALSE
     *
     * \spec requires true; \endspec
     */
    auto operator!=(iterator const& other) const -> bool { return it_ != other.it_; }

    /*!
     * \brief Increment operator.
     *
     * \return The incremented iterator.
     *
     * \pre         -
     * \context     App
     * \threadsafe  FALSE
     * \reentrant   FALSE
     * \synchronous TRUE
     * \steady      FALSE
     */
    auto operator++() -> iterator& {
      ++it_;  // VCA_IPCB_VALID_ITERATOR_PARAMETER
      return *this;
    }

   private:
    /*! The iterator to the underlying map. */
    SubscriberMap::iterator it_;
  };

  /*!
   * \brief The begin.
   *
   * \return The iterator to the beginning.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto begin() -> iterator;

  /*!
   * \brief The end.
   *
   * \return The iterator to the end.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto end() -> iterator;

 private:
  /*! Type-alias for the logger. */
  using Logger = ipc_binding_core::internal::logging::AraComLogger;

  /*! The Logger. */
  Logger logger_{::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextId,
                 ::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextDescription,
                 ::amsr::core::StringView{"SkeletonEventSubscriberHandler"}};

  /*! The subscriber map. */
  SubscriberMap subscriber_map_{};
};

}  // namespace internal
}  // namespace ipc_binding_transformation_layer
}  // namespace amsr

#endif  // LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_SKELETON_EVENT_SUBSCRIBER_HANDLER_H_
