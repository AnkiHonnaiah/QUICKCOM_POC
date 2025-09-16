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
 *         \unit  ZeroCopyCommon::MemCon::Client
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_CLIENT_CLIENT_BUILDER_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_CLIENT_CLIENT_BUILDER_H_

#include <functional>
#include <memory>

#include "amsr/core/optional.h"
#include "amsr/core/result.h"

#include "amsr/zero_copy_common/logic/internal/handle.h"
#include "amsr/zero_copy_common/memcon/internal/client/client_builder_interface.h"
#include "amsr/zero_copy_common/memcon/internal/client/client_interface.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace client {

/*!
 * \brief Implementation of ClientBuilderInterface.
 * \vprivate Product Private
 */
class ClientBuilder final : public ClientBuilderInterface {
 public:
  /*!
   * \brief Base type used for handles.
   */
  using Handle = logic::internal::Handle;

  /*!
   * \brief       Construct a ClientBuilder.
   * \param       group Group of handles for the client to be built.
   * \context     ANY
   * \pre         -
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private
   */
  explicit ClientBuilder(Handle::Group group) noexcept;

  /*!
   * \brief Do not construct a ClientBuilder by copying.
   * \vprivate Product Private
   */
  ClientBuilder(ClientBuilder const&) noexcept = delete;

  /*!
   * \brief Do not construct a ClientBuilder by moving.
   * \vprivate Product Private
   */
  ClientBuilder(ClientBuilder&&) noexcept = delete;

  /*!
   * \brief Do not copy another ClientBuilder into this instance.
   * \vprivate Product Private
   */
  auto operator=(ClientBuilder const&) noexcept -> ClientBuilder& = delete;

  /*!
   * \brief Do not move another ClientBuilder into this instance.
   * \vprivate Product Private
   */
  auto operator=(ClientBuilder&&) noexcept -> ClientBuilder& = delete;

  /*!
   * \brief Destruct the ClientBuilder.
   * \vprivate Product Private
   */
  // VCA_ZEROCOPYCOMMON_SLC_23_DEFAULT_DESTRUCTOR
  ~ClientBuilder() noexcept final = default;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::client::ClientBuilderInterface::WithSlotContentSize
   */
  auto WithSlotContentSize(SlotContentSize slot_content_size) noexcept -> ClientBuilderInterface& final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::client::ClientBuilderInterface::WithSlotContentAlignment
   */
  auto WithSlotContentAlignment(SlotContentAlignment slot_content_alignment) noexcept -> ClientBuilderInterface& final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::client::ClientBuilderInterface::WithMemoryTechnology
   */
  auto WithMemoryTechnology(MemoryTechnology memory_technology) noexcept -> ClientBuilderInterface& final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::client::ClientBuilderInterface::WithSideChannel
   */
  auto WithSideChannel(std::unique_ptr<SideChannelInterface> side_channel_ptr) noexcept
      -> ClientBuilderInterface& final;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::client::ClientBuilderInterface::WithOnStateTransitionCallback
   */
  auto WithOnStateTransitionCallback(OnStateTransitionCallback on_state_transition_callback) noexcept
      -> ClientBuilderInterface& final;
  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::client::ClientBuilderInterface::Build
   */
  auto Build() noexcept -> ::amsr::core::Result<::std::unique_ptr<ClientInterface>> final;

 private:
  /*!
   * \brief       Assert that this instance was not yet used to build a client.
   * \context     ANY
   * \pre         This instance must not have been used to build a client yet.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private
   */
  void AssertBuildNotFinished() const noexcept;

  /*!
   * \brief Internal state indicating whether this instance was already used to build a client.
   */
  bool build_finished_{false};

  /*!
   * \brief Optional containing the configured slot content size.
   */
  ::amsr::core::Optional<SlotContentSize> opt_slot_content_size_{::amsr::core::nullopt};

  /*!
   * \brief Optional containing the configured slot content alignment.
   */
  ::amsr::core::Optional<SlotContentAlignment> opt_slot_content_alignment_{::amsr::core::nullopt};

  /*!
   * \brief Optional containing the configured memory technology.
   */
  ::amsr::core::Optional<MemoryTechnology> opt_memory_technology_{::amsr::core::nullopt};

  /*!
   * \brief Pointer to the configured side channel.
   */
  std::unique_ptr<SideChannelInterface> side_channel_ptr_{nullptr};

  /*!
   * \brief Callback that is called each time when a state transition happened.
   */
  OnStateTransitionCallback on_state_transition_callback_{nullptr};

  /*!
   * \brief Handle group that the client will belong to.
   */
  Handle::Group group_;
};

}  // namespace client
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_CLIENT_CLIENT_BUILDER_H_
