/*!********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file
 *        \brief  Definition of class ServerBuilder.
 *         \unit  ZeroCopyCommon::MemCon::Server
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_SERVER_BUILDER_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_SERVER_BUILDER_H_

#include <memory>

#include "amsr/zero_copy_common/external/logic/server_builder.h"
#include "amsr/zero_copy_common/logic/internal/handle.h"
#include "amsr/zero_copy_common/logic/internal/server_interface.h"

#include "amsr/zero_copy_common/memcon/internal/memory_manager/server_memory_manager.h"
#include "amsr/zero_copy_common/memcon/internal/server/server_builder_interface.h"
#include "amsr/zero_copy_common/memcon/internal/types.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace server {

/*!
 * \brief Implementation of ServerBuilderInterface.
 * \vprivate Product Private
 */
class ServerBuilder final : public ServerBuilderInterface {
 public:
  /*!
   * \brief Base type used for handles.
   */
  using Handle = logic::internal::Handle;

  /*!
   * \brief       Construct a ServerBuilder.
   * \param       group Group of handles for the server to be built.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private
   */
  explicit ServerBuilder(Handle::Group const& group) noexcept;

  /*!
   * \brief Do not construct a ServerBuilder by copying.
   * \vprivate Product Private
   */
  ServerBuilder(ServerBuilder const&) noexcept = delete;

  /*!
   * \brief Do not construct a ServerBuilder by moving.
   * \vprivate Product Private
   */
  ServerBuilder(ServerBuilder&&) noexcept = delete;

  /*!
   * \brief Do not copy another ServerBuilder into this instance.
   * \vprivate Product Private
   */
  auto operator=(ServerBuilder const&) & noexcept -> ServerBuilder& = delete;

  /*!
   * \brief Do not move another ServerBuilder into this instance.
   * \vprivate Product Private
   */
  auto operator=(ServerBuilder&&) & noexcept -> ServerBuilder& = delete;

  /*!
   * \brief Destruct the ServerBuilder.
   * \vprivate Product Private
   */
  // VCA_ZEROCOPYCOMMON_SLC_23_DEFAULT_DESTRUCTOR
  ~ServerBuilder() noexcept final = default;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::server::ServerBuilderInterface::WithNumberSlots
   */
  auto WithNumberSlots(NumSlots number_slots) noexcept -> ServerBuilderInterface& final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::server::ServerBuilderInterface::WithSlotContentSize
   */
  auto WithSlotContentSize(SlotContentSize slot_content_size) noexcept -> ServerBuilderInterface& final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::server::ServerBuilderInterface::WithSlotContentAlignment
   */
  auto WithSlotContentAlignment(SlotContentAlignment slot_content_alignment) noexcept -> ServerBuilderInterface& final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::server::ServerBuilderInterface::WithMemoryTechnology
   */
  auto WithMemoryTechnology(MemoryTechnology memory_technology) noexcept -> ServerBuilderInterface& final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::server::ServerBuilderInterface::WithMaxNumberReceivers
   */
  auto WithMaxNumberReceivers(MaxNumberReceivers max_number_receivers) noexcept -> ServerBuilderInterface& final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::server::ServerBuilderInterface::WithNumberClasses
   */
  auto WithNumberClasses(ReceiverClassNumber number_receiver_classes) noexcept -> ServerBuilderInterface& final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::server::ServerBuilderInterface::WithClass
   */
  auto WithClass(ReceiverClassLimit receiver_class_limit,
                 ::amsr::core::Optional<ClassHandle>& opt_class_handle) noexcept -> ServerBuilderInterface& final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::server::ServerBuilderInterface::WithOnReceiverStateTransitionCallback
   */
  auto WithOnReceiverStateTransitionCallback(
      OnReceiverStateTransitionCallback on_receiver_state_transition_cbk) noexcept -> ServerBuilderInterface& final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::server::ServerBuilderInterface::Build
   */
  auto Build() noexcept -> ::amsr::core::Result<std::unique_ptr<ServerInterface>> final;

 private:
  /*!
   * \brief Type used for a logic server builder.
   */
  using LogicServerBuilder = ::amsr::zero_copy_common::external::logic::ServerBuilder;

  /*!
   * \brief Type used for a writable slot descriptor.
   */
  using WritableSlotDescriptorPtr =
      ::amsr::zero_copy_common::memcon::internal::memory_manager::ServerMemoryManager::WritableSlotDescriptorPtr;

  /*!
   * \brief       Assert that this instance was not yet used to build a server.
   * \context     ANY
   * \pre         This instance must not have been used to build a Server yet.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  void AssertBuildNotFinished() const noexcept;

  /*!
   * \brief Assert that this instance contains all information required to build a server.
   * \context ANY
   * \pre A number of slots must be available.
   * \pre A slot content size must be available.
   * \pre A slot content alignment must be available.
   * \pre A memory technology must be available.
   * \pre A maximum number of receivers must be available.
   * \pre A number of receiver classes must be available.
   * \pre The configured number of receiver classes must be added.
   * \pre A receiver on state transition callback must be available.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void AssertBuildPossible() const noexcept;

  /*!
   * \brief Internal state indicating whether this instance was already used to build a server.
   */
  bool build_finished_{false};

  /*!
   * \brief Number of slots the Server will provide.
   */
  ::amsr::core::Optional<NumSlots> opt_slot_number_{::amsr::core::nullopt};

  /*!
   * \brief Slot content size the Server will provide.
   */
  ::amsr::core::Optional<SlotContentSize> opt_slot_content_size_{::amsr::core::nullopt};

  /*!
   * \brief Slot content alignment the Server will provide.
   */
  ::amsr::core::Optional<SlotContentAlignment> opt_slot_content_alignment_{::amsr::core::nullopt};

  /*!
   * \brief MemoryTechnology the Server will use.
   */
  ::amsr::core::Optional<MemoryTechnology> opt_memory_technology_{::amsr::core::nullopt};

  /*!
   * \brief Maximum number of receivers the Server will use.
   */
  ::amsr::core::Optional<MaxNumberReceivers> opt_max_receivers_number_{::amsr::core::nullopt};

  /*!
   * \brief Number of receiver classes the Server will use.
   */
  ::amsr::core::Optional<ReceiverClassNumber> opt_receiver_class_number_{::amsr::core::nullopt};

  /*!
   * \brief Current number of receiver classes which were already added to the ServerBuilder.
   */
  ReceiverClassNumber::Type add_receiver_class_counter_{0U};

  /*!
   * \brief Remote receiver state change callback the Server will use.
   */
  ::amsr::core::Optional<OnReceiverStateTransitionCallback> opt_on_receiver_state_transition_cbk_{
      ::amsr::core::nullopt};

  /*!
   * \brief Handle group that the Server will belong to.
   */
  Handle::Group group_;

  /*!
   * \brief Zero-Copy Logic server builder.
   */
  LogicServerBuilder logic_server_builder_;
};

}  // namespace server
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_SERVER_BUILDER_H_
