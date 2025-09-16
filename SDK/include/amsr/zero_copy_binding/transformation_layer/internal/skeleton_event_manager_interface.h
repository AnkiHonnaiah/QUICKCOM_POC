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
 *         \brief  SkeletonEventManager interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_SKELETON_EVENT_MANAGER_INTERFACE_H_
#define LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_SKELETON_EVENT_MANAGER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include <memory>

#include "amsr/ipc/integrity_level.h"
#include "amsr/zero_copy_binding/common/internal/config/types.h"
#include "amsr/zero_copy_binding/core/internal/connection_wrapper_interface.h"
#include "amsr/zero_copy_binding/core/internal/unique_number_provider.h"
#include "amsr/zero_copy_common/internal/zero_copy_common_server.h"
#include "amsr/zero_copy_common/internal/zero_copy_common_side_channel.h"
#include "vac/language/function_ref.h"
#include "vac/language/unique_function.h"

namespace amsr {
namespace zero_copy_binding {
namespace transformation_layer {
namespace internal {

/*!
 * \brief     The interface of the skeleton event manager.
 *
 * \unit ZeroCopyBinding::ZeroCopyBindingTransformationLayer::SkeletonEventManager
 * \vprivate  Component private
 */
class SkeletonEventManagerInterface {
 public:
  /*!
   * \brief Type for the ServerInterface of ZeroCopyCommon.
   */
  using ServerInterface = ::amsr::zero_copy_common::internal::ServerInterface;

  /*!
   * \brief Type for the ReceiverId.
   */
  using ReceiverId = ServerInterface::ReceiverId;

  /*!
   * \brief Type for the EventId.
   */
  using EventIdType = common::internal::config::EventId::Type;

  /*!
   * \brief Type for the ConnectionHandle identifying connections towards SkeletonXfBase.
   */
  using ConnectionHandle = core::internal::UniqueNumberProvider::NumberType;

  /*!
   * \brief Struct containing information about the receivers of a SkeletonEventManager.
   */
  struct ReceiverInformation {
    /*!
     * \brief The receiver id of a receiver.
     */
    ReceiverId receiver_id;

    /*!
     * \brief Connection handle identifying the connection in SkeletonXfBase that belongs to a receiver.
     */
    ConnectionHandle connection_handle;

    /*!
     *\brief Specify if the receiver is a trace receiver.
     */
    bool is_trace_receiver;

    /*!
     * \brief Specify the integrity level of a receiver.
     */
    ::amsr::ipc::IntegrityLevel integrity_level;
  };

  /*!
   * \brief Type used for the callback that is called when a state transition happened for any receiver.
   */
  using OnConnectionCleanupCallback =
      ::vac::language::UniqueFunction<void(ConnectionHandle const&, EventIdType const&, ReceiverInformation const&)>;

  /*!
   * \brief Reference type used for the callback that is called when a state transition happened for any receiver.
   */
  using OnConnectionCleanupCallbackRef =
      ::vac::language::FunctionRef<void(ConnectionHandle const&, EventIdType const&, ReceiverInformation const&)>;

  /*!
   * \brief       Default constructor.
   */
  SkeletonEventManagerInterface() noexcept = default;

  /*!
   * \brief       Destructor.
   */
  virtual ~SkeletonEventManagerInterface() noexcept = default;

  SkeletonEventManagerInterface(SkeletonEventManagerInterface const&) = delete;
  auto operator=(SkeletonEventManagerInterface const&) -> SkeletonEventManagerInterface& = delete;
  SkeletonEventManagerInterface(SkeletonEventManagerInterface&&) = delete;
  auto operator=(SkeletonEventManagerInterface&&) -> SkeletonEventManagerInterface& = delete;

  /*!
   * \brief       Initialize the event manager.
   * \context     App
   * \pre         Must only be called when the SkeletonEventManager is deinitialized.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component private
   */
  virtual auto Initialize() noexcept -> void = 0;

  /*!
   * \brief       Deinitialize the event manager.
   * \context     App
   * \pre         Must only be called when the SkeletonEventManager is initialized.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component private
   */
  virtual auto Deinitialize() noexcept -> void = 0;

  /*!
   * \brief       Add a receiver to the event manager.
   * \param[in]   connection_handle     The handle that is assigned to the connection used for the receiver.
   * \param[in]   connection_wrapper    The connection wrapper of the receiver.
   * \param[in]   side_channel          The side channel to be used for the connection of the receiver.
   *                                    Must not be a nullptr.
   * \param[in]   is_trace_receiver     Identify if the receiver is a trace receiver.
   * \return      A result with the receiver id as assigned by the underlying communication logic.
   * \error       GenErrc::kRuntimeResourceOutOf in case the configured resource limits for this integrity
   *              class are already reached.
   * \error       GenErrc::kRuntime in case the integrity level of the remote cannot be determined.
   * \context     Reactor
   * \pre         Must only be called when SkeletonEventManager is initialized, i.e., between a call to Initialize() and
   *              Deinitialize().
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component private
   */
  virtual auto AddReceiver(ConnectionHandle connection_handle,
                           core::internal::ConnectionWrapperInterface const& connection_wrapper,
                           std::unique_ptr<::amsr::zero_copy_common::internal::SideChannelInterface> side_channel,
                           bool is_trace_receiver) noexcept -> ::amsr::core::Result<ReceiverId> = 0;

  /*!
   * \brief       Connect a receiver.
   * \param[in]   receiver_id           The id of the receiver as returned by AddReceiver().
   * \context     Reactor
   * \pre         Must only be called when SkeletonEventManager is initialized, i.e., between a call to Initialize() and
   *              Deinitialize().
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component private
   */
  virtual auto ConnectReceiver(ReceiverId receiver_id) noexcept -> void = 0;
  /*!
   * \brief       Remove a receiver from the event manager.
   * \param[in]   receiver_id The receiver id of the receiver to be removed.
   * \param[in]   integrity_level The integrity of the receiver to be removed.
   * \param[in]   is_trace_receiver Identify whether the receiver to be removed is a trace receiver.
   * \details     This function must only be called from a ReactorSoftwareEvent that is triggered in the context of the
   *              receiver state transition callback to ensure that no callback is executing for the receiver to be
   *              removed. Otherwise, this function will abort.
   * \context     Reactor
   * \pre         Receiver to be removed must no longer be in use.
   * \pre         Must only be called when SkeletonEventManager is initialized, i.e., between a call to Initialize() and
   *              Deinitialize().
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component private
   */
  virtual auto RemoveReceiver(ReceiverId receiver_id, ::amsr::ipc::IntegrityLevel integrity_level,
                              bool is_trace_receiver) noexcept -> void = 0;
};

}  // namespace internal
}  // namespace transformation_layer
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_SKELETON_EVENT_MANAGER_INTERFACE_H_
