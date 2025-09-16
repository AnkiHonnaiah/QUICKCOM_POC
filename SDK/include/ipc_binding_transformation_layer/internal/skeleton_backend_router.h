/**********************************************************************************************************************
 * COPYRIGHT
 * -------------------------------------------------------------------------------------------------------------------
 * \verbatim
 * Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *               This software is copyright protected and proprietary to Vector Informatik GmbH.
 *               Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *               All other rights remain with Vector Informatik GmbH.
 * \endverbatim
 * -------------------------------------------------------------------------------------------------------------------
 * FILE DESCRIPTION
 * -----------------------------------------------------------------------------------------------------------------*/
/*!       \file
 *       \brief  The skeleton backend router.
 *
 *     \details
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_SKELETON_BACKEND_ROUTER_H_
#define LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_SKELETON_BACKEND_ROUTER_H_

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include <initializer_list>
#include <unordered_map>

#include "ipc_binding_core/internal/ipc_protocol/ipc_protocol_types.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/logging/ipc_binding_log_builder.h"
#include "ipc_binding_core/internal/packet_router/skeleton_backend_router_interface.h"
#include "ipc_binding_transformation_layer/internal/events/skeleton_event_xf_backend_interface.h"
#include "ipc_binding_transformation_layer/internal/methods/skeleton_fire_and_forget_method_xf_backend_interface.h"
#include "ipc_binding_transformation_layer/internal/methods/skeleton_method_xf_backend_interface.h"
#include "ipc_binding_transformation_layer/internal/offer_state.h"

namespace amsr {
namespace ipc_binding_transformation_layer {
namespace internal {

/*!
 * \brief   The skeleton backend router for the ipc binding services.
 * \details The skeleton backend router is part of a service instance and is responsible for routing to the event and
 *          method backends within that instance. Its responsibilities for:
 *          - Routing of requests (incl. fire & forget).
 *          - Routing of event notifications.
 *          - Routing of error messages.
 *
 * \unit IpcBinding::IpcBindingTransformationLayer::SkeletonInstance
 */
class SkeletonBackendRouter final : public ipc_binding_core::internal::SkeletonBackendRouterInterface {
 public:
  /*! Type-alias for the ConnectionId type definition. */
  using ConnectionId = ::amsr::ipc_binding_core::internal::connection_manager::ConnectionId;

  /*! Type-alias for the ConnectionSkeletonPtr type definition. */
  using ConnectionSkeletonPtr = ::amsr::ipc_binding_core::internal::ConnectionSkeletonPtr;

  /*! Type-alias for the EventId type definition. */
  using EventId = ::amsr::ipc_binding_core::internal::ipc_protocol::EventId;

  /*! Type definition for the EventIdMap. */
  using EventIdMap = std::unordered_map<EventId, SkeletonEventXfBackendInterface&,
                                        ipc_binding_core::internal::runtime_configuration::EventIdHash>;

  /*! Type-alias for the MethodId type definition. */
  using MethodId = amsr::ipc_binding_core::internal::ipc_protocol::MethodId;

  /*! Type-alias for the SkeletonMethodXfBackendInterface type definition. */
  using SkeletonMethodXfBackendInterface = methods::SkeletonMethodXfBackendInterface;

  /*! Type definition for the MethodIdMap. */
  using MethodIdMap = std::unordered_map<MethodId, SkeletonMethodXfBackendInterface&,
                                         ipc_binding_core::internal::runtime_configuration::MethodIdHash>;

  /*! Type-alias for the SkeletonFireAndForgetMethodXfBackendInterface type definition. */
  using SkeletonFireAndForgetMethodXfBackendInterface = methods::SkeletonFireAndForgetMethodXfBackendInterface;

  /*! Type definition for the MethodFireAndForgetIdMap. */
  using MethodFireAndForgetIdMap = std::unordered_map<MethodId, SkeletonFireAndForgetMethodXfBackendInterface&,
                                                      ipc_binding_core::internal::runtime_configuration::MethodIdHash>;

  /*!
   * \brief Construct the skeleton backend router with empty routing tables.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  SkeletonBackendRouter() noexcept = default;

  /*!
   * \brief Destruct the skeleton backend router.
   *
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  ~SkeletonBackendRouter() final = default;  // VCA_IPCB_STD_UNORDERED_MAP_DESTRUCTOR

  SkeletonBackendRouter(SkeletonBackendRouter const&) noexcept = delete;
  auto operator=(SkeletonBackendRouter const&) noexcept -> SkeletonBackendRouter& = delete;
  SkeletonBackendRouter(SkeletonBackendRouter&&) noexcept = delete;
  auto operator=(SkeletonBackendRouter&&) noexcept -> SkeletonBackendRouter& = delete;

  /*!
   * \brief Register a list of methods.
   *
   * \param[in] methods  The list of <method id, method interface> pairs.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  void RegisterMethods(std::initializer_list<MethodIdMap::value_type> methods) noexcept;

  /*!
   * \brief Register a list of fire and forget methods.
   *
   * \param[in] fire_and_forget_methods  The list of <method id, fire and forget method interface> pairs.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  void RegisterFireAndForgetMethods(
      std::initializer_list<MethodFireAndForgetIdMap::value_type> fire_and_forget_methods) noexcept;

  /*!
   * \brief Register a list of events.
   *
   * \param[in] events  The list of <event id, event interface> pairs.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  void RegisterEvents(std::initializer_list<EventIdMap::value_type> events) noexcept;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::SkeletonBackendRouterInterface::OnRequestReceived
   */
  void OnRequestReceived(
      ::amsr::ipc_binding_core::internal::ipc_protocol::RemoteRequestMessage&& request) noexcept final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::SkeletonBackendRouterInterface::OnRequestNoReturnReceived
   */
  void OnRequestNoReturnReceived(
      ::amsr::ipc_binding_core::internal::ipc_protocol::RequestNoReturnMessage&& request) noexcept final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::SkeletonBackendRouterInterface::OnSubscribeEventReceived
   */
  auto OnSubscribeEventReceived(
      ::amsr::ipc_binding_core::internal::ipc_protocol::SubscribeEventMessageHeader const& header,
      ConnectionId connection_id, ConnectionSkeletonPtr& connection_ptr) noexcept -> void final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::SkeletonBackendRouterInterface::OnUnsubscribeEventReceived
   */
  void OnUnsubscribeEventReceived(
      ::amsr::ipc_binding_core::internal::ipc_protocol::UnsubscribeEventMessageHeader const& header,
      ConnectionId connection_id) noexcept final;

  /*!
   * \brief Gets a reference to the offer state.
   *
   * \return Reference to offer state.
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
  auto GetOfferState() const noexcept -> OfferState const&;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::SkeletonBackendRouterInterface::StartOffering
   */
  auto StartOffering() noexcept -> void final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::SkeletonBackendRouterInterface::StopOffering
   */
  auto StopOffering() noexcept -> void final;

  /*!
   * \copydoc amsr::ipc_binding_core::internal::SkeletonBackendRouterInterface::RemoveConnection
   */
  auto RemoveConnection(ConnectionId connection_id) noexcept -> void final;

 private:
  /*!
   * \brief   Determine if the service is currently offered.
   * \details The service session ID increments with each start and stop. When the ID is even, the service is not
   *          offered, whereas when the ID is odd, it signifies that the service is offered.
   *
   * \return True, when offered, else false.
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
  auto IsOffered() const noexcept -> bool;

  /*! Type alias for the logger. */
  using Logger = ::amsr::ipc_binding_core::internal::logging::AraComLogger;

  /*! Type alias for the log builder. */
  using LogBuilder = ::amsr::ipc_binding_core::internal::logging::IpcBindingLogBuilder;

  /*! Map of method ids to method xf backend objects. */
  MethodIdMap method_id_map_{};

  /*! Map of fire and forget method ids to fire and forget method xf backend objects. */
  MethodFireAndForgetIdMap method_fire_and_forget_id_map_{};

  /*! Map of event ids to event xf backend objects. */
  EventIdMap event_id_map_{};

  /*! OfferState for the current service is offered (true) or is stoped (false) state. */
  OfferState offer_state_{};

  /*! Logger to print out debug & error messages. */
  Logger logger_{::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextId,
                 ::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextDescription,
                 ::amsr::core::StringView{"SkeletonBackendRouter"}};
};

}  // namespace internal
}  // namespace ipc_binding_transformation_layer
}  // namespace amsr

#endif  // LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_SKELETON_BACKEND_ROUTER_H_
