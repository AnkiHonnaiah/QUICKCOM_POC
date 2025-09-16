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
 *         \brief  Zero-Copy service instance base class.
 *
 *         \details  -
 *
 *         \unit ZeroCopyBinding::ZeroCopyBindingTransformationLayer::SkeletonXf
 *
 * \complexity  The complexity is increased by inter module calls to the IpcServiceDiscovery (especially the
 *              ReactorSoftwareEvent). Those are necessary for a correct connection cleanup in the reactor context.
 *              These inter-dependencies are by design and thoroughly tested.
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_SKELETON_XF_BASE_H_
#define LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_SKELETON_XF_BASE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <cstddef>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <unordered_set>

#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/ipc/unicast_address.h"
#include "amsr/ipc_service_discovery/common/internal/reactor_software_event/reactor_software_event.h"
#include "amsr/zero_copy_binding/common/internal/config/config_view.h"
#include "amsr/zero_copy_binding/common/internal/config/zero_copy_binding_config.h"
#include "amsr/zero_copy_binding/common/internal/factory_interface.h"
#include "amsr/zero_copy_binding/common/internal/service_discovery/types.h"
#include "amsr/zero_copy_binding/core/internal/acceptor_wrapper_interface.h"
#include "amsr/zero_copy_binding/core/internal/connection_wrapper_interface.h"
#include "amsr/zero_copy_binding/core/internal/logging/zero_copy_logger.h"
#include "amsr/zero_copy_binding/core/internal/unique_number_provider.h"
#include "amsr/zero_copy_binding/transformation_layer/internal/skeleton_event_manager.h"
#include "amsr/zero_copy_binding/transformation_layer/internal/skeleton_event_manager_interface.h"
#include "amsr/zero_copy_common/internal/zero_copy_common_side_channel.h"
#include "ara/core/vector.h"
#include "osabstraction/io/reactor1/reactor1.h"
#include "vac/memory/allocator/flexible/flexible_unique_memory_buffer_allocator.h"
#include "vac/testing/test_adapter.h"

namespace amsr {
namespace zero_copy_binding {
namespace transformation_layer {
namespace internal {

/*!
 * \brief Base class with common functionality for all generated SkeletonXf classes derived from it.
 */
class SkeletonXfBase {
 protected:
  /*!
   * \brief       Type alias for the configuration of the SkeletonXfBase.
   */
  using ConfigView = common::internal::config::ConfigView;

 private:
  /*!
   * \brief Type alias for the ProvidedServiceInstanceIdentifier.
   */
  using ProvidedServiceInstanceIdentifier = common::internal::service_discovery::ProvidedServiceInstanceIdentifier;

  /*!
   * \brief Type for the connection cleanup callback.
   */
  using OnConnectionCleanupCallback = SkeletonEventManagerInterface::OnConnectionCleanupCallback;

  /*!
   * \brief Type for the EventId.
   */
  using EventIdType = SkeletonEventManagerInterface::EventIdType;

  /*!
   * \brief Type for the ReceiverId.
   */
  using ReceiverId = SkeletonEventManagerInterface::ReceiverId;

  /*!
   * \brief Type for the ReceiverInformation.
   */
  using ReceiverInformation = SkeletonEventManagerInterface::ReceiverInformation;

  /*!
   * \brief Type for the ConnectionHandle.
   */
  using ConnectionHandle = core::internal::UniqueNumberProvider::NumberType;

  /*!
   * \brief Struct containing information related to connection cleanup.
   */
  struct ConnectionCleanupInformation {
    /*!
     * \brief Connection handle to identify the connection to be cleaned up.
     */
    ConnectionHandle connection_handle{};

    /*!
     * \brief Identify if the connection belongs to a receiver registered in a SkeletonEventManager.
     */
    bool is_registered_receiver{};

    /*!
     * \brief Event id of the receiver to be cleaned up.
     */
    ::amsr::core::Optional<EventIdType> event_id;

    /*!
     * \brief Receiver id of the receiver to be cleaned up.
     */
    ::amsr::core::Optional<ReceiverId> receiver_id;

    /*!
     * \brief Integrity level of the receiver to be cleaned up.
     */
    ::amsr::core::Optional<::amsr::ipc::IntegrityLevel> integrity_level;

    /*!
     * \brief Identify if the receiver is a trace receiver.
     */
    ::amsr::core::Optional<bool> is_trace_receiver;
  };

 public:
  /*!
   * \brief       Constructor of SkeletonXfBase.
   * \param[in]   zero_copy_binding_config_view A view to the binding's config struct.
   *                                            It must remain valid for the object's entire lifetime.
   * \param[in]   life_cycle_manager_core       A shared pointer to the life cycle manager core.
   *                                            It must remain valid for the object's entire lifetime.
   * \param[in]   event_id_to_skeleton_event_manager_mapping Mapping of event ids to the individual skeleton event
   *                                                         managers.
   *                                            The stored references to event managers must remain valid for the
   *                                            object's entire lifetime.
   * \context     ANY
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component private
   */
  SkeletonXfBase(ConfigView zero_copy_binding_config_view,
                 std::shared_ptr<::amsr::zero_copy_binding::core::internal::LifeCycleManagerCore>
                     life_cycle_manager_core) noexcept;

  SkeletonXfBase() = delete;
  SkeletonXfBase(SkeletonXfBase const&) = delete;
  SkeletonXfBase(SkeletonXfBase&&) = delete;
  auto operator=(SkeletonXfBase const&) -> SkeletonXfBase& = delete;
  auto operator=(SkeletonXfBase&&) -> SkeletonXfBase& = delete;
  virtual ~SkeletonXfBase() noexcept;

 protected:
  /*!
   * \brief       Offer the service represented by this skeleton xf class via the ZeroCopyBinding.
   * \details     The implementation of this function is intentionally left empty for the binding_if_legacy variant.
   * \context     App
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  void OfferServiceBase() noexcept;

  /*!
   * \brief       Stop offering the service represented by this skeleton xf class via the ZeroCopyBinding.
   * \details     The implementation of this function is intentionally left empty for the binding_if_legacy variant.
   * \context     App
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  void StopOfferServiceBase() noexcept;

  /*!
   * \brief       Initialize the SkeletonXfBase.
   * \context     ANY
   * \pre         SkeletonXfBase must be deinitialized.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component private
   */
  void Initialize() noexcept;

  /*!
   * \brief       Deinitialize the SkeletonXfBase.
   * \details     The deinitialization is done prior to the destruction on the Skeleton side in the context of
   *              StopOfferService. This is required in order to stop accepting new connections and to stop receiving
   *              from newly established connections that have not yet been registered towards a SkeletonEventManager.
   *              No mutex is held during the iteration over all pending connections and the wait until they are no
   *              longer in use.
   * \context     ANY
   * \pre         SkeletonXfBase must be initialized.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component private
   */
  void Deinitialize() noexcept;

  /*!
   * \brief       Cleanup the SkeletonXfBase.
   * \details     The shutdown is done prior to the destruction on the Skeleton side in the context of StopOfferService.
   *              This is required in order to stop accepting new connections and to stop receiving from newly
   *              established connections that have not yet been registered towards a SkeletonEventManager.
   * \context     ANY
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component private
   */
  void Cleanup() noexcept;

 private:
  /*!
   * \brief A view to the binding's configuration data.
   */
  ConfigView const zero_copy_binding_config_view_;

  /*!
   * \brief A pointer to the LifeCycleManagerCore.
   */
  std::shared_ptr<::amsr::zero_copy_binding::core::internal::LifeCycleManagerCore> const life_cycle_manager_core_;

  /*!
   * \brief Mapping of event ids to the specific event manager instance.
   */
  using EventManagersMap = std::unordered_map<EventIdType, SkeletonEventManagerInterface&>;
  EventManagersMap event_id_to_skeleton_event_manager_mapping_{};

  /*!
   * \brief The pointer to the factory.
   */
  std::shared_ptr<common::internal::FactoryInterface const> const factory_;

 protected:
  /*!
   * \brief The connection cleanup callback.
   */
  // VECTOR NC VectorC++-V11.0.2: MD_ZEROCOPYBINDING_V11.0.2_ProtectedAttributes
  OnConnectionCleanupCallback on_connection_cleanup_cbk_;

  /*!
   * \brief       Initialize the event managers map with the given event managers.
   * \param[in]   event_managers A list of event managers to maintain.
   * \context     ANY
   * \pre         Event managers map must not have been initialized before.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component private
   */
  void InitializeEventManagersMap(std::initializer_list<EventManagersMap::value_type> const& event_managers) noexcept;

 private:
  /*!
   * \brief       Accept a new connection via the acceptor wrapper.
   * \details     The connection acceptance is done asynchronously via the acceptor wrapper.
   * \context     Reactor
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component private
   */
  void AcceptNextConnection() noexcept;

  /*!
   * \brief       Callback that is called when a new connection has been accepted by the acceptor wrapper.
   * \param[in]   accept_completion_result A result with the newly created IPC connection.
   * \context     Reactor
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component private
   */
  void OnAcceptCompletion(::amsr::core::Result<::amsr::ipc::Connection> accept_completion_result) noexcept;

  /*!
   * \brief       Callback that is called when a new message is received by the connection wrapper.
   * \param[in]   connection_handle The connection handle of the connection that received a message.
   * \param[in]   message_view A view to the received message.
   * \context     Reactor
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component private
   */
  void OnMessageReceived(ConnectionHandle connection_handle,
                         ::amsr::core::Result<::amsr::core::Span<std::uint8_t const>> message_view) noexcept;

  /*!
   * \brief       Handle a new message that is successfully received by the connection wrapper.
   * \param[in]   connection_handle The connection handle of the connection that received a message.
   * \param[in]   message_view A view to the received message.
   * \context     ANY
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component private
   */
  void HandleReceivedMessage(ConnectionHandle connection_handle,
                             ::amsr::core::Span<std::uint8_t const> message_view) noexcept;

  /*!
   * \brief       Trigger a connection cleanup.
   * \param[in]   connection_handle      The connection handle to do the cleanup for.
   * \param[in]   is_registered_receiver Identify if the connection belongs to a receiver registered to a
   *                                     SkeletonEventManager.
   * \param[in]   event_id               Optional. The event id. Required when the connection belongs to a
   *                                     registered receiver.
   * \param[in]   receiver_information   Optional. The receiver information. Required when the connection belongs to a
   *                                     registered receiver.
   * \context                            ANY
   * \pre                                -
   * \threadsafe                         TRUE
   * \reentrant                          FALSE
   * \synchronous                        TRUE
   * \vprivate Component private
   */
  void TriggerConnectionCleanup(ConnectionHandle connection_handle, bool is_registered_receiver,
                                ::amsr::core::Optional<EventIdType> event_id = {},
                                ::amsr::core::Optional<ReceiverInformation> receiver_information = {}) noexcept;

  /*!
   * \brief       Callback that is triggered upon connection cleanup.
   * \details     The callback must only be called from within a reactor software event.
   * \context     Reactor
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component private
   */
  void OnConnectionCleanup() noexcept;

  /*!
   * \brief A vector with all connections that are queued for cleanup.
   */
  ::ara::core::Vector<ConnectionCleanupInformation> connection_cleanup_info_{};

  /*!
   * \brief Flag indicating the register status of connection_cleanup_software_event_.
   */
  std::atomic<bool> connection_cleanup_software_event_registered_{false};

  /*!
   * \brief A reactor software event that triggers the connection cleanup.
   */
  ::amsr::ipc_service_discovery::common::internal::ReactorSoftwareEvent connection_cleanup_software_event_;

  /*!
   * \brief Mutex preventing concurrent access to SkeletonXf members.
   */
  std::mutex mutex_{};

  /*!
   * \brief Mutex protecting the connection cleanup information.
   */
  std::mutex mutex_connection_cleanup_info_{};

  /*!
   * \brief The Logger.
   */
  core::internal::logging::ZeroCopyLogger logger_;

  /*!
   * \brief The unicast address at which the skeleton can be reached.
   */
  ::amsr::ipc::UnicastAddress unicast_address_;

  /*!
   * \brief Provider for unique numbers for connection enumeration.
   */
  core::internal::UniqueNumberProvider connection_handle_provider_{};

  /*!
   * \brief The pointer to the acceptor wrapper of the Skeleton.
   */
  std::unique_ptr<core::internal::AcceptorWrapperInterface> acceptor_wrapper_;

  /*!
   * \brief Mapping of connection handles to the connection wrappers.
   */
  using ConnectionWrappersMap =
      std::unordered_map<ConnectionHandle, std::unique_ptr<core::internal::ConnectionWrapperInterface>>;
  ConnectionWrappersMap connection_wrappers_{};

  /*!
   * \brief A vector with all connections that are queued for cleanup.
   */
  std::unordered_set<ConnectionHandle> pending_connection_wrapper_{};

  /*!
   * \brief The provided service instance identifier used by the service discovery.
   */
  ProvidedServiceInstanceIdentifier provided_service_instance_identifier_;

  /*!
   * \brief The pointer to the service discovery.
   */
  std::shared_ptr<common::internal::service_discovery::ServiceDiscoveryInterface> const service_discovery_;

  /*!
   * \brief Allocator for receive message buffers.
   */
  ::vac::memory::allocator::flexible::FlexibleUniqueMemoryBufferAllocator<> receive_message_buffer_allocator_{};

  /*!
   * \brief The time period that is waited between each check of IsInUse() upon cleanup.
   */
  static std::chrono::milliseconds constexpr kCleanupDelay{10};

  /*! Friend test declaration. */
  FRIEND_TEST(UT__SkeletonXf__DeathTest, InitializeEventManagersMap);
  FRIEND_TEST(UT__SkeletonXf__SkeletonXfGenerated, ConstructionSuccessful);
  FRIEND_TEST(UT__SkeletonXf__SkeletonXfGenerated, GetEventManagers);
  FRIEND_TEST(UT__SkeletonXf__SkeletonXfGenerated, OfferStopOfferSuccessful);
};

/*!
 * \exclusivearea   ::amsr::zero_copy_binding::transformation_layer::SkeletonXfBase::mutex_
 *                  Ensures consistency while reading/modifying the SkeletonXf members.
 * \protects        ::amsr::zero_copy_binding::transformation_layer::SkeletonXfBase::connection_wrappers_
 *                  ::amsr::zero_copy_binding::transformation_layer::SkeletonXfBase::pending_connection_wrapper_
 *                  ::amsr::zero_copy_binding::transformation_layer::SkeletonXfBase::acceptor_wrapper_
 * \usedin          ::amsr::zero_copy_binding::transformation_layer::SkeletonXfBase::Cleanup
 *                  ::amsr::zero_copy_binding::transformation_layer::SkeletonXfBase::Deinitialize
 * \length          LONG
 *                  Receive messages and calling callbacks when mutex locked.
 *                  Depth call-tree > 1.
 * \endexclusivearea
 */

/*!
 * \exclusivearea   ::amsr::zero_copy_binding::transformation_layer::SkeletonXfBase::mutex_connection_cleanup_info_
 *                  Ensures consistency while reading/modifying the cleanup connection information.
 * \protects        ::amsr::zero_copy_binding::transformation_layer::SkeletonXfBase::connection_cleanup_info_
 * \usedin          ::amsr::zero_copy_binding::transformation_layer::SkeletonXfBase::Deinitialize
 * \length          SHORT
 *                  Only used to unregister/trigger the cleanup software event and modify connection_cleanup_info_
 * \endexclusivearea
 */

}  // namespace internal
}  // namespace transformation_layer
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_SKELETON_XF_BASE_H_
