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
 *        \brief  Interface that intend to be extended by event xf backend.
 *
 *      \details  The event xf backend is templated and needs an interface to store it in the list which is present in
 *the generated SkeletonXf class.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_EVENTS_SKELETON_EVENT_XF_BACKEND_INTERFACE_H_
#define LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_EVENTS_SKELETON_EVENT_XF_BACKEND_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ipc_binding_core/internal/connection_manager/connection_skeleton_interface.h"
#include "ipc_binding_core/internal/ipc_protocol/message.h"

namespace amsr {
namespace ipc_binding_transformation_layer {
namespace internal {

/*!
 * \brief Event xf backend to handle event request on skeleton side.
 *
 * \unit IpcBinding::IpcBindingTransformationLayer::SkeletonEvent
 */
class SkeletonEventXfBackendInterface {
 public:
  /*! Type alias for SubscribeEventMessageHeader. */
  using SubscribeEventMessageHeader = ::amsr::ipc_binding_core::internal::ipc_protocol::SubscribeEventMessageHeader;

  /*! Type alias for ConnectionId. */
  using ConnectionId = ipc_binding_core::internal::connection_manager::ConnectionId;

  /*! Type alias for ConnectionSkeletonPtr. */
  using ConnectionSkeletonPtr = ipc_binding_core::internal::ConnectionSkeletonPtr;

  /*!
   * \brief Default constructor.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  SkeletonEventXfBackendInterface() noexcept = default;

  SkeletonEventXfBackendInterface(SkeletonEventXfBackendInterface const&) noexcept = delete;
  auto operator=(SkeletonEventXfBackendInterface const&) noexcept -> SkeletonEventXfBackendInterface& = delete;
  SkeletonEventXfBackendInterface(SkeletonEventXfBackendInterface&&) noexcept = delete;
  auto operator=(SkeletonEventXfBackendInterface&&) noexcept -> SkeletonEventXfBackendInterface& = delete;

  /*!
   * \brief Default destructor.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual ~SkeletonEventXfBackendInterface() noexcept = default;

  /*!
   * \brief   Handle reception of a received event subscription message.
   * \details Sends subscription Ack / NAck messages and initial field notifications (field notifier only).
   *
   * \param[in]     connection_id   The connection ID of the connection where the subscribe event was received.
   * \param[in,out] connection_ptr  A shared pointer of the connection where the subscribe event was received.
   * \param[in]     header          The header of the received event subscription message.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto HandleEventSubscription(ConnectionId connection_id, ConnectionSkeletonPtr& connection_ptr,
                                       SubscribeEventMessageHeader const& header) noexcept -> void = 0;

  /*!
   * \brief   Called on a reception of a new unsubscription request.
   * \details Remove decrease counter of subscribed connection from subscriber list.
   *
   * \param[in] connection_id  The connection ID of the connection where the unsubscribe event was received.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto HandleEventUnsubscription(ConnectionId connection_id) noexcept -> void = 0;

  /*!
   * \brief   Called when a connection is disconnected.
   * \details Remove whole connection from subscriber list.
   *
   * \param[in] connection_id  The disconnecting connection ID.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto HandleDisconnect(ConnectionId connection_id) noexcept -> void = 0;

  /*!
   * \brief   Called when skeleton stop offering.
   * \details Clear subscriber list.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  virtual auto HandleStopOffer() noexcept -> void = 0;
};

}  // namespace internal
}  // namespace ipc_binding_transformation_layer
}  // namespace amsr

#endif  // LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_EVENTS_SKELETON_EVENT_XF_BACKEND_INTERFACE_H_
