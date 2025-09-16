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
 *         \brief  Zero-Copy Binding implementation of the SkeletonEventManager.
 *         \unit   ZeroCopyBinding::ZeroCopyBindingTransformationLayer::SkeletonEventManager
 *    \complexity  The high complexity of the class SkeletonEventManager from the equally named unit results from:
 *                 - High number of inter-unit and inter-module calls required for correct use of the ZeroCopyCommon
 *                   server.
 *                 - The necessary error handling in addition contributes to the inter-unit and inter-module calls.
 *                 These points cannot be avoided and are necessary for this class to work in a correct and safe
 *                 manner. The error handling can be tested as it is carried out by individual, self-contained
 *                 functions. The functions are an integral part of the functionality of the class and cannot be
 *                 reasonably outsourced as a whole without compromising maintainability.
 *                 During testing, it is possible to reach all states and error cases without major
 *                 effort, as the individual steps can be executed by helper functions performing required standard
 *                 actions.
 *                 In summary, the complexity is increased by many calls between other units and modules that are
 *                 necessary for the correct functionality of this unit and cannot be reasonably refactored. Careful
 *                 design of the class ensures that testing is not impeded and thus the risk is kept as low as possible.
 *
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_SKELETON_EVENT_MANAGER_H_
#define LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_SKELETON_EVENT_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <memory>
#include <type_traits>

#include "amsr/core/array.h"
#include "amsr/core/error_code.h"
#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/ipc/integrity_level.h"
#include "amsr/socal/internal/events/skeleton_event_manager_interface.h"
#include "ara/core/result.h"
#include "vac/testing/test_adapter.h"

#include "amsr/zero_copy_binding/common/internal/config/config_view.h"
#include "amsr/zero_copy_binding/core/internal/life_cycle_manager_core.h"
#include "amsr/zero_copy_binding/core/internal/logging/zero_copy_logger.h"
#include "amsr/zero_copy_binding/core/internal/slot_content.h"
#include "amsr/zero_copy_binding/transformation_layer/internal/skeleton_event_manager_interface.h"

#ifdef AMSR_ZEROCOPYBINDING_ENABLE_TRACE_API
#include "amsr/zero_copy_binding/transformation_layer/internal/trace/local_receiver.h"
#endif  // AMSR_ZEROCOPYBINDING_ENABLE_TRACE_API

namespace amsr {
namespace zero_copy_binding {
namespace transformation_layer {
namespace internal {

/*!
 * \brief       A manager for a single event of a skeleton.
 * \tparam      SampleType Type of a single event sample.
 */
template <typename SampleType>
class SkeletonEventManager final : public ::amsr::socal::internal::events::SkeletonEventManagerInterface<SampleType>,
                                   public SkeletonEventManagerInterface {
  /*!
   * \brief Check that the SampleType is not a reference or has cv qualifiers.
   */
  static_assert((!std::is_reference<SampleType>::value) && (!std::is_const<SampleType>::value) &&
                    (!std::is_volatile<SampleType>::value),
                "SampleType must not be a reference or have cv qualifiers.");

  /*!
   *\brief The error domain of ZeroCopyCommon.
   */
  using ZeroCopyCommonErrc = ::amsr::zero_copy_common::common::ZeroCopyCommonErrc;

  /*!
   * \brief Initialization type for allocated slots.
   */
  using InitializationType = common::internal::config::Initialization::Type;

  /*!
   * \brief Type for the ServerBuilderInterface of ZeroCopyCommon.
   */
  using ServerBuilderInterface = ::amsr::zero_copy_common::internal::ServerBuilderInterface;

  /*!
   * \brief Type of the ReceiverState of ZeroCopyCommon.
   */
  using ReceiverState = ServerInterface::ReceiverState;

  /*!
   * \brief MemoryTechnology type for the ServerBuilder.
   */
  using MemoryTechnology = ServerBuilderInterface::MemoryTechnology;

  /*!
   * \brief SlotContent type of the sample slot.
   */
  using SlotContent = core::internal::SlotContent<SampleType>;

  /*!
   * \brief MetaData type of the sample slot.
   */
  using MetaData = core::internal::MetaData;

  /*!
   * \brief The number of receiver classes.
   */
  static constexpr std::size_t kNumReceiverClasses{6U};

  /*!
   *\brief The index of the additional trace receiver class.
   */
  static constexpr std::size_t kTraceClassIndex{kNumReceiverClasses - 1U};

 public:
  /*!
   * \brief Type alias to the AllocatedSampleUniquePtr.
   */
  using AllocatedSampleUniquePtr =
      typename ::amsr::socal::internal::events::SkeletonEventManagerInterface<SampleType>::AllocatedSampleUniquePtr;

  // VECTOR NL AutosarC++17_10-A16.0.1: MD_ZEROCOPYBINDING_A16.0.1_conditionalCompilation
#ifdef AMSR_ZEROCOPYBINDING_ENABLE_TRACE_API
  static constexpr std::uint32_t kTraceMaxSlots{10U};
#else
  static constexpr std::uint32_t kTraceMaxSlots{0U};
  // VECTOR NL AutosarC++17_10-A16.0.1: MD_ZEROCOPYBINDING_A16.0.1_conditionalCompilation
#endif  // AMSR_ZEROCOPYBINDING_ENABLE_TRACE_API

  /*!
   * \brief The maximum number of allowed trace connections.
   */
  static constexpr std::uint32_t kTraceConnectionLimit{2U};

  /*!
   * \brief       Constructor of SkeletonEventManager.
   * \param[in]   zero_copy_binding_config_view A view to the binding's config struct.
   *                                            It must remain valid for the object's entire lifetime.
   * \param[in]   life_cycle_manager_core       A pointer to the LifeCycleManagerCore.
   *                                            It must remain valid for the object's entire lifetime.
   * \param[in]   on_connection_cleanup_cbk     A callback that is called upon a state transition of a receiver that
   *                                            requires a cleanup of the connection.
   * \context     App
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component private
   */
  explicit SkeletonEventManager(common::internal::config::ConfigView zero_copy_binding_config_view,
                                std::shared_ptr<core::internal::LifeCycleManagerCore> life_cycle_manager_core,
                                OnConnectionCleanupCallbackRef on_connection_cleanup_cbk) noexcept;

  /*!
   * \copydoc ::amsr::zero_copy_binding::transformation_layer::internal::SkeletonEventManagerInterface::Initialize
   */
  void Initialize() noexcept final;

  /*!
   * \copydoc ::amsr::zero_copy_binding::transformation_layer::internal::SkeletonEventManagerInterface::Deinitialize
   */
  void Deinitialize() noexcept final;

  /*!
   * \brief       Default destructor of the SkeletonEventManager.
   * \context     ANY
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component private
   */
  // VCA_ZEROCOPYBINDING_SPC_15_STDLIB_FULFILLS_FUNCTION_CONTRACT, VCA_ZEROCOPYBINDING_SPC_15_STATIC_VECTOR_DTOR_FULFILLS_FUNCTION_CONTRACT, VCA_ZEROCOPYBINDING_SLC_23_MEMORY_DEALLOCATION_IN_DTOR
  ~SkeletonEventManager() noexcept final = default;

  SkeletonEventManager() = delete;
  SkeletonEventManager(SkeletonEventManager const&) = delete;
  SkeletonEventManager(SkeletonEventManager&&) = delete;
  auto operator=(SkeletonEventManager const&) -> SkeletonEventManager& = delete;
  auto operator=(SkeletonEventManager&&) -> SkeletonEventManager& = delete;

  /*!
   * \brief       Send an event sample.
   * \param[in]   data Sample value to be transmitted. Unused.
   * \return      This method does not return as it aborts.
   * \details     This method is not supported by Zero-Copy and will lead to an abort.
   * \context     App
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component private
   */
  auto Send(SampleType const& data) noexcept -> ::ara::core::Result<void> final;

  /*!
   * \brief       Send an event sample using a preallocated pointer.
   * \return      A void result on success.
   * \param[in]   sample_ptr The sample pointer of the slot to be sent. The sample pointer must not be nullptr.
   * \details     This method sends the allocated sample to all connected clients of this event.
   *              Since the connections are handled by the underlying ZeroCopyCommonServer to which all clients are
   *              registered to, sending reduces to simply calling send on the ZeroCopyCommonServer.
   * \context     App
   * \pre         Only pointers that were returned by a previous call to Allocate() of this instance must be
   *              used.
   * \pre         Must only be called when SkeletonEventManager is initialized, i.e., between a call to Initialize() and
   *              Deinitialize().
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component private
   */
  auto Send(AllocatedSampleUniquePtr sample_ptr) noexcept -> ::ara::core::Result<void> final;

  /*!
   * \brief       Allocate an event data sample to send.
   * \return      An AllocatedSampleUniquePtr pointing to the memory to write and send.
   * \details     Since the underlying memory for allocation is created and owned by the underlying
   *              ZeroCopyCommonServer, allocation reduces to a call to GetSlot() on the ZeroCopyCommonServer.
   *              Errors, like no slot available, need to be handled by the caller.
   * \error       ComErrc::kSampleAllocationFailure if the allocation of an event data sample failed.
   * \context     App
   * \pre         Must only be called when SkeletonEventManager is initialized, i.e., between a call to Initialize() and
   *              Deinitialize().
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component private
   */
  auto Allocate() noexcept -> ::ara::core::Result<AllocatedSampleUniquePtr> final;

  /*!
   * \brief       Pre-Allocate count memory slots to be used when samples are transmitted with Send().
   * \param[in]   count Number of slots to be pre-allocated. Unused.
   * \details     This method does nothing for Zero-Copy and immediately returns.
   * \context     ANY
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component private
   */
  void PreAllocate(std::size_t count) noexcept final;

  /*!
   * \copydoc ::amsr::zero_copy_binding::transformation_layer::internal::SkeletonEventManagerInterface::AddReceiver
   */
  auto AddReceiver(ConnectionHandle connection_handle,
                   core::internal::ConnectionWrapperInterface const& connection_wrapper,
                   std::unique_ptr<::amsr::zero_copy_common::internal::SideChannelInterface> side_channel,
                   bool is_trace_receiver) noexcept -> ::amsr::core::Result<ReceiverId> final;

  /*!
   * \copydoc ::amsr::zero_copy_binding::transformation_layer::internal::SkeletonEventManagerInterface::ConnectReceiver
   */
  void ConnectReceiver(ReceiverId receiver_id) noexcept final;

  /*!
   * \copydoc ::amsr::zero_copy_binding::transformation_layer::internal::SkeletonEventManagerInterface::RemoveReceiver
   */
  void RemoveReceiver(ReceiverId receiver_id, ::amsr::ipc::IntegrityLevel integrity_level,
                      bool is_trace_receiver) noexcept final;

 private:
  /*!
   * \brief The result of the CanAddReceiver member function.
   */
  struct CanAddReceiverResult;

  void GetOwnIntegrityLevel(core::internal::ConnectionWrapperInterface const& connection_wrapper) noexcept;

  auto GetPeerIdentity(core::internal::ConnectionWrapperInterface const& connection_wrapper) noexcept
      -> ipc::Credentials;

  auto CanAddReceiver(::amsr::ipc::IntegrityLevel integrity_level, bool is_trace_receiver) noexcept
      -> ::amsr::core::Result<CanAddReceiverResult>;

  /*!
   * \brief       Convert an integrity level value as defined by an enum into an unsigned integer value.
   * \param[in]   integrity_level The integrity level value to be converted.
   * \return      A number indicating the integrity level. 0 equals QM, 4 equals ASIL-D.
   * \context     ANY
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  static auto ConvertIntegrityLevelToUnsignedInt(::amsr::ipc::IntegrityLevel integrity_level) noexcept -> std::size_t;

  /*!
   * \brief       Get a single resource limit for an integrity class.
   * \param[in]   provided_instance_specific_event_config The configuration of a provided instance.
   * \param[in]   integrity_level The integrity level for which the resource limit shall be collected.
   * \return      A single resource limit struct.
   * \context     ANY
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  static auto GetSingleResourceLimit(
      common::internal::config::ProvidedInstanceSpecificEventConfig const& provided_instance_specific_event_config,
      ::amsr::ipc::IntegrityLevel integrity_level) noexcept -> common::internal::config::SingleResourceLimit;

  /*!
   * \brief       Callback that is called upon the state transition of a receiver.
   * \details     AddReceiver() must have been called at least once since the integrity level of the server must be set
   *              for handling of the state transition. This should always be the case, since a state transition can
   *              only occur for added receivers.
   * \param[in]   receiver_id The id of the receiver.
   * \param[in]   receiver_state The new state of the receiver.
   * \param[in]   opt_error_code An optional error code detailing the occurred error. Must contain an error code when
   *              receiver_state is kCorrupted.
   * \context     ANY
   * \pre         Must only be called from an exclusive area protecting the receiver_information_ map.
   * \pre         Must only be called when SkeletonEventManager is initialized, i.e., between a call to Initialize() and
   *              Deinitialize().
   * \pre         Must only be called when the OwnIntegrityLevel optional is set, which requires a one-time call to
   *              AddReceiver() for a non-trace receiver.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   */
  void OnReceiverStateTransition(ReceiverId receiver_id, ReceiverState receiver_state,
                                 ::amsr::core::Optional<::amsr::core::ErrorCode> opt_error_code) noexcept;

  /*!
   * \brief       Error handling for corrupted receivers.
   * \details     When a corrupted receiver is detected, this function iterates across all receivers and checks their
   *              current state. For each receiver with kCorrupted or kDisconnected state, the state transition
   *              callback is executed.
   * \context     ANY
   * \pre         Must only be called when SkeletonEventManager is initialized, i.e., between a call to Initialize() and
   *              Deinitialize().
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component private
   */
  void HandleCorruptedReceiver() noexcept;

  /*!
   * \brief The integrity level of this SkeletonEventManager instance.
   */
  ::amsr::core::Optional<::amsr::ipc::IntegrityLevel> own_integrity_level_{};

  /*!
   * \brief The callback that is called on cleanup of a receiver.
   */
  OnConnectionCleanupCallbackRef on_connection_cleanup_cbk_;

  /*!
   * \brief The logger.
   */
  core::internal::logging::ZeroCopyLogger logger_;

  /*!
   * \brief A view to the binding's configuration data.
   */
  common::internal::config::ConfigView const zero_copy_binding_config_view_;

  /*!
   * \brief The ZeroCopyCommon server for the communication of this event instance.
   */
  std::unique_ptr<ServerInterface> zero_copy_common_server_{};

  /*!
   * \brief A pointer to the LifeCycleManagerCore.
   */
  std::shared_ptr<::amsr::zero_copy_binding::core::internal::LifeCycleManagerCore> const life_cycle_manager_core_;

  /*!
   * \brief Class handles for different integrity levels.
   */
  ::vac::container::StaticVector<::amsr::core::Optional<ServerInterface::ClassHandle>> class_handles_{};

  /*!
   * \brief Array to keep track of the number of connections currently established for each integrity level.
   */
  ::amsr::core::Array<::std::atomic<common::internal::config::SingleResourceLimit::Type>, kNumReceiverClasses>
      connection_count_{};

  /*!
   * \brief List of receiver classes for which messages were dropped.
   */
  ServerInterface::DroppedInformation receiver_classes_with_message_drops_{};

  /*!
   * \brief Map that stores receiver information for the removal and cleanup of receivers.
   */
  using ReceiverInformationMap = std::unordered_map<ReceiverId::IdType, ReceiverInformation>;
  ReceiverInformationMap receiver_information_{};

  /*!
   * \brief Initialization status.
   */
  std::atomic<bool> initialized_{false};

  /*!
   * \brief Mutex protecting the receiver information map.
   */
  std::mutex receiver_information_mutex_{};

  /*!
   * \brief The time period that is waited between each check of IsInUse() upon cleanup.
   */
  static std::chrono::milliseconds constexpr kCleanupDelay{10};

  /*! Friend test declaration */
  FRIEND_TEST(ConvertIntegrityLevelToUnsignedInt, IntegrityLevel);

  /*! Friend test declaration */
  FRIEND_TEST(GetSingleResourceLimit, IntegrityLevel);

#ifdef AMSR_ZEROCOPYBINDING_ENABLE_TRACE_API
  /*!
   * \brief Trace client resources.
   */
  std::unique_ptr<trace::LocalReceiver<SampleType>> trace_client_{nullptr};
#endif  // AMSR_ZEROCOPYBINDING_ENABLE_TRACE_API
};

}  // namespace internal
}  // namespace transformation_layer
}  // namespace zero_copy_binding
}  // namespace amsr

/*!
 * \exclusivearea   ::amsr::zero_copy_binding::transformation_layer::SkeletonEventManager::receiver_information_mutex_
 *                  Ensures consistency while reading/modifying the receiver information map.
 * \protects        ::amsr::zero_copy_binding::transformation_layer::SkeletonEventManager::receiver_information_
 * \usedin          ::amsr::zero_copy_binding::transformation_layer::SkeletonEventManager::Deinitialize
 *                  ::amsr::zero_copy_binding::transformation_layer::SkeletonEventManager::AddReceiver
 *                  ::amsr::zero_copy_binding::transformation_layer::SkeletonEventManager::OnReceiverStateTransition
 *                  ::amsr::zero_copy_binding::transformation_layer::SkeletonEventManager::HandleCorruptedReceiver
 * \length          SHORT
                    In rare circumstances the mutex can block for a longer time in Deinitialize() when it is waited
                    until the zero_copy_common_server_ finishes its asynchronous callbacks. This is considered
                    uncritical as this only happens during the cleanup phase.
 * \endexclusivearea
 */

// VECTOR NL AutosarC++17_10-M16.0.1: MD_ZEROCOPYBINDING_M16.0.1_temporaryPostponedIncludeQuickFix
#include "amsr/zero_copy_binding/transformation_layer/internal/skeleton_event_manager_impl.h"

#endif  // LIB_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_ZERO_COPY_BINDING_TRANSFORMATION_LAYER_INTERNAL_SKELETON_EVENT_MANAGER_H_
