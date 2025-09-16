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
 *        \brief  Definition of class ClientBuilder.
 *         \unit  ZeroCopyCommon::ZeroCopyLogic::Client
 *
 *********************************************************************************************************************/
#ifndef LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_CLIENT_BUILDER_H_
#define LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_CLIENT_BUILDER_H_

#include <memory>
#include <utility>

#include "amsr/core/abort.h"
#include "amsr/core/optional.h"

#include "amsr/zero_copy_common/logic/internal/client.h"
#include "amsr/zero_copy_common/logic/internal/client_builder_interface.h"
#include "amsr/zero_copy_common/logic/internal/client_interface.h"
#include "amsr/zero_copy_common/logic/internal/handle.h"
#include "amsr/zero_copy_common/logic/internal/readable_slot_descriptor.h"
#include "amsr/zero_copy_common/logic/internal/reading_queue_end_interface.h"
#include "amsr/zero_copy_common/logic/internal/slot_handle.h"
#include "amsr/zero_copy_common/logic/internal/slot_manager.h"
#include "amsr/zero_copy_common/logic/internal/writing_queue_end_interface.h"

namespace amsr {
namespace zero_copy_common {
namespace logic {
namespace internal {

/*!
 * \brief Implementation of ClientBuilderInterface.
 * \details A client builder offers to build a client by providing the necessary information.
 *          The client builder performs basic sanity checks and only builds a client if the provided information
 *          meets the expectations of the client builder.
 * \vprivate Component Private
 */
class ClientBuilder final : public ClientBuilderInterface {
 public:
  /*!
   * \brief Construct a ClientBuilder.
   * \param group Group of handles for the client to be built.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \spec
   *   requires true;
   * \endspec
   */
  explicit ClientBuilder(Handle::Group const& group) noexcept;

  /*!
   * \brief Do not construct a ClientBuilder by copying.
   * \vprivate Component Private
   */
  ClientBuilder(ClientBuilder const&) noexcept = delete;

  /*!
   * \brief Construct a ClientBuilder by moving.
   * \vprivate Component Private
   */
  ClientBuilder(ClientBuilder&&) noexcept = default;

  /*!
   * \brief Do not copy another ClientBuilder into this instance.
   * \vprivate Component Private
   */
  auto operator=(ClientBuilder const&) noexcept -> ClientBuilder& = delete;

  /*!
   * \brief Move another ClientBuilder into this instance.
   * \vprivate Component Private
   */
  auto operator=(ClientBuilder&&) & noexcept -> ClientBuilder& = default;

  /*!
   * \brief Destruct the ClientBuilder.
   * \vprivate Component Private
   */
  // VCA_ZEROCOPYCOMMON_SLC_23_DEFAULT_DESTRUCTOR
  ~ClientBuilder() noexcept final = default;

  /*!
   * \copydoc amsr::zero_copy_common::logic::internal::ClientBuilderInterface::WithNumberSlots
   */
  void WithNumberSlots(SlotHandle::SlotIndex number_slots) noexcept final;

  /*!
   * \copydoc amsr::zero_copy_common::logic::internal::ClientBuilderInterface::AddSlot
   */
  void AddSlot(std::unique_ptr<ReadableSlotDescriptor> slot_descriptor) noexcept final;

  /*!
   * \copydoc amsr::zero_copy_common::logic::internal::ClientBuilderInterface::WithFreeQueue
   */
  void WithFreeQueue(std::unique_ptr<WritingQueueEndInterface> free_queue_ptr) noexcept final;

  /*!
   * \copydoc amsr::zero_copy_common::logic::internal::ClientBuilderInterface::WithAvailableQueue
   */
  void WithAvailableQueue(std::unique_ptr<ReadingQueueEndInterface> available_queue_ptr) noexcept final;

  /*!
   * \copydoc amsr::zero_copy_common::logic::internal::ClientBuilderInterface::Build
   */
  auto Build() noexcept -> std::unique_ptr<ClientInterface> final;

 private:
  /*!
   * \brief Assert that this instance was not yet used to build a Client.
   * \context ANY
   * \pre This instance must not have been used to build a Client yet.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void AssertBuildNotFinished() const noexcept;

  /*!
   * \brief Handle group that the Client will belong to.
   */
  Handle::Group group_;

  /*!
   * \brief Internal state indicating whether this instance was already used to build a client.
   */
  bool build_finished_{false};

  /*!
   * \brief Optional containing the SlotManager for the Client.
   */
  amsr::core::Optional<SlotManager<ReadableSlotDescriptor>> opt_slot_manager_{};

  /*!
   * \brief Optional containing the free queue for the Client.
   */
  amsr::core::Optional<std::unique_ptr<WritingQueueEndInterface>> opt_free_queue_ptr_{};

  /*!
   * \brief Optional containing the available queue for the Client.
   */
  amsr::core::Optional<std::unique_ptr<ReadingQueueEndInterface>> opt_available_queue_ptr_{};
};
}  // namespace internal
}  // namespace logic
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_CLIENT_BUILDER_H_
