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
 *         \unit  ZeroCopyCommon::ZeroCopyLogic::Server
 *
 *********************************************************************************************************************/
#ifndef LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_SERVER_BUILDER_H_
#define LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_SERVER_BUILDER_H_

#include <memory>
#include <utility>

#include "amsr/core/optional.h"

#include "amsr/zero_copy_common/logic/internal/borrowed_manager.h"
#include "amsr/zero_copy_common/logic/internal/handle.h"
#include "amsr/zero_copy_common/logic/internal/receiver_manager.h"
#include "amsr/zero_copy_common/logic/internal/server.h"
#include "amsr/zero_copy_common/logic/internal/server_builder_interface.h"
#include "amsr/zero_copy_common/logic/internal/server_interface.h"
#include "amsr/zero_copy_common/logic/internal/slot_manager.h"
#include "amsr/zero_copy_common/logic/internal/writable_slot_descriptor.h"

namespace amsr {
namespace zero_copy_common {
namespace logic {
namespace internal {

/*!
 * \brief Implementation of ServerBuilderInterface.
 * \details A server builder offers to build a server by providing the necessary information.
 *          The server builder performs basic sanity checks and only builds a server if the provided information
 *          meets the expectations of the server builder.
 * \vprivate Component Private
 */
class ServerBuilder final : public ServerBuilderInterface {
 public:
  /*!
   * \brief Construct a ServerBuilder.
   * \param group Group of handles for the server to be built.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  explicit ServerBuilder(Handle::Group group) noexcept;

  /*!
   * \brief Do not construct a ServerBuilder by copying.
   * \vprivate Component Private
   */
  ServerBuilder(ServerBuilder const&) noexcept = delete;

  /*!
   * \brief Construct a ServerBuilder by moving.
   * \vprivate Component Private
   */
  ServerBuilder(ServerBuilder&&) noexcept = default;

  /*!
   * \brief Do not copy another ServerBuilder into this instance.
   * \vprivate Component Private
   */
  auto operator=(ServerBuilder const&) noexcept -> ServerBuilder& = delete;

  /*!
   * \brief Move another ServerBuilder into this instance.
   * \vprivate Component Private
   */
  auto operator=(ServerBuilder&&) & noexcept -> ServerBuilder& = default;

  /*!
   * \brief Destruct the ServerBuilder.
   * \vprivate Component Private
   */
  // VCA_ZEROCOPYCOMMON_SLC_23_DEFAULT_DESTRUCTOR
  ~ServerBuilder() noexcept final = default;

  /*!
   * \copydoc ::amsr::zero_copy_common::logic::internal::ServerBuilderInterface::WithNumberSlots
   */
  void WithNumberSlots(SlotHandle::SlotIndex number_slots) noexcept final;

  /*!
   * \copydoc ::amsr::zero_copy_common::logic::internal::ServerBuilderInterface::WithMaxNumberReceivers
   */
  void WithMaxNumberReceivers(ReceiverHandle::ReceiverIndex max_number_receivers) noexcept final;

  /*!
   * \copydoc ::amsr::zero_copy_common::logic::internal::ServerBuilderInterface::WithNumberClasses
   */
  void WithNumberClasses(ClassHandle::ReceiverClassIndex number_classes) noexcept final;

  /*!
   * \copydoc ::amsr::zero_copy_common::logic::internal::ServerBuilderInterface::AddSlot
   */
  void AddSlot(std::unique_ptr<WritableSlotDescriptor> slot_descriptor) noexcept final;

  /*!
   * \copydoc ::amsr::zero_copy_common::logic::internal::ServerBuilderInterface::AddClass
   */
  auto AddClass(ReceiverClass::ClassLimit receiver_class_limit) noexcept -> ClassHandle final;

  /*!
   * \copydoc ::amsr::zero_copy_common::logic::internal::ServerBuilderInterface::Build
   */
  auto Build() noexcept -> std::unique_ptr<ServerInterface> final;

 private:
  /*!
   * \brief Assert that this instance was not yet used to build a server.
   * \context ANY
   * \pre This instance must not have been used to build a Server yet.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void AssertBuildNotFinished() const noexcept;

  /*!
   * \brief Check whether all parameters required to create a BorrowedManager are configured and create a
   *        BorrowedManager if that is the case.
   * \details Required parameters to create a BorrowedManager are the number of slots, the maximum number of receivers
   *          and the number of receiver classes.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void CreateBorrowedManagerIfPossible() noexcept;

  /*!
   * \brief Assert that this instance contains all information required to build a server.
   * \context ANY
   * \pre A SlotManager containing the configured number of slots must be available.
   * \pre A ReceiverManager must be available.
   * \pre A BorrowedManager containing the configured number of receiver classes must be available.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void AssertBuildPossible() const noexcept;

  /*!
   * \brief Handle group that the server will belong to.
   */
  Handle::Group group_;

  /*!
   * \brief Internal state indicating whether this instance was already used to build a server.
   */
  bool build_finished_{false};

  /*!
   * \brief Optional containing the configured number of slots.
   */
  amsr::core::Optional<SlotHandle::SlotIndex> opt_number_slots_{};

  /*!
   * \brief Optional containing the SlotManager for the server.
   */
  amsr::core::Optional<SlotManager<WritableSlotDescriptor>> opt_slot_manager_{};

  /*!
   * \brief Optional containing the configured maximum number of receivers.
   */
  amsr::core::Optional<ReceiverHandle::ReceiverIndex> opt_max_number_receivers_{};

  /*!
   * \brief Optional containing the ReceiverManager for the server.
   */
  amsr::core::Optional<ReceiverManager> opt_receiver_manager_{};

  /*!
   * \brief Optional containing the configured number of receiver classes.
   */
  amsr::core::Optional<ClassHandle::ReceiverClassIndex> opt_number_classes_{};

  /*!
   * \brief Optional containing the BorrowedManager for the server.
   */
  amsr::core::Optional<BorrowedManager> opt_borrowed_manager_{};
};

}  // namespace internal
}  // namespace logic
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_SERVER_BUILDER_H_
