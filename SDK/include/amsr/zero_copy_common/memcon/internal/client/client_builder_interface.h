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
 *        \brief  Definition of interface ClientBuilderInterface.
 *         \unit  ZeroCopyCommon::MemCon::Client
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_CLIENT_CLIENT_BUILDER_INTERFACE_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_CLIENT_CLIENT_BUILDER_INTERFACE_H_

#include <memory>

#include "amsr/core/result.h"

#include "amsr/zero_copy_common/memcon/internal/client/client_interface.h"
#include "amsr/zero_copy_common/memcon/internal/side_channel/side_channel_interface.h"
#include "amsr/zero_copy_common/memcon/internal/types.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace client {

/*!
 * \brief    Interface of a client builder.
 * \vprivate Product Private
 */
class ClientBuilderInterface {
 public:
  /*!
   * \brief Type used for the size of the content of a slot.
   */
  using SlotContentSize = ::amsr::zero_copy_common::memcon::internal::SlotContentSize;

  /*!
   * \brief Type used for the alignment of the content of a slot.
   */
  using SlotContentAlignment = ::amsr::zero_copy_common::memcon::internal::SlotContentAlignment;

  /*!
   * \brief Type used for the memory technology.
   */
  using MemoryTechnology = ::amsr::zero_copy_common::memcon::internal::MemoryTechnology;

  /*!
   * \brief Type used for the side channel interface.
   */
  using SideChannelInterface = ::amsr::zero_copy_common::memcon::internal::side_channel::SideChannelInterface;

  /*!
   * \brief Type used for the callback that is called when a state transition happened in the client.
   * \details The callback is called each time a state transition happened asynchronously in the client.
   *          I.e., the state transition is triggered by the peer, not by the user of the client.
   *          Therefore, this callback is only called in the reactor context.
   * \param client_state The new client state.
   * \param opt_error_code Optional containing an error code that identifies the cause of the error.
   *                       Only contains a value when client_state is set to ClientState::kCorrupted.
   *                       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerDisconnectedError If the server
   *                       closed the connection without adhering to the protocol.
   *                       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kPeerCrashedError If the server
   *                       crashed.
   *                       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kProtocolError If the communication
   *                       with the server is erroneous for any other reason.
   * \context REACTOR
   * \pre -
   *
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  using OnStateTransitionCallback =
      ::amsr::zero_copy_common::memcon::internal::client::ClientInterface::OnStateTransitionCallback;

  /*!
   * \brief    Destruct the ClientBuilderInterface.
   * \vprivate Product Private
   */
  virtual ~ClientBuilderInterface() noexcept = 0;

  /*!
   * \brief       Set the slot content size for the client to be built.
   * \details     This parameter is optional and will be used for sanity checks, if provided.
   * \param       slot_content_size Slot content size for the client to be built.
   *                                Must be greater than zero.
   * \return      Reference to this builder.
   * \context     ANY
   * \pre         Must not be called after a client was built using this instance.
   * \pre         Must only be called once.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private
   */
  virtual auto WithSlotContentSize(SlotContentSize slot_content_size) noexcept -> ClientBuilderInterface& = 0;

  /*!
   * \brief       Set the slot content alignment for the client to be built.
   * \details     This parameter is optional and will be used for sanity checks, if provided.
   * \param       slot_content_alignment Slot content alignment for the client to be built.
   *                                     Must be greater than zero.
   *                                     Must be a power of two.
   * \return      Reference to this builder.
   * \context     ANY
   * \pre         Must not be called after a client was built using this instance.
   * \pre         Must only be called once.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private
   */
  virtual auto WithSlotContentAlignment(SlotContentAlignment slot_content_alignment) noexcept
      -> ClientBuilderInterface& = 0;

  /*!
   * \brief       Set the memory technology for the client to be built.
   * \param       memory_technology Memory technology for the client to be built.
   * \return      Reference to this builder.
   * \context     ANY
   * \pre         Must not be called after a client was built using this instance.
   * \pre         Must only be called once.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private
   */
  virtual auto WithMemoryTechnology(MemoryTechnology memory_technology) noexcept -> ClientBuilderInterface& = 0;

  /*!
   * \brief       Set the side channel for the client to be built.
   * \param       side_channel_ptr Side channel to communicate with the server for the client to be built.
   *                               Must not be a nullptr.
   * \return      Reference to this builder.
   * \context     ANY
   * \pre         Must not be called after a client was built using this instance.
   * \pre         Must only be called once.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private
   */
  virtual auto WithSideChannel(std::unique_ptr<SideChannelInterface> side_channel_ptr) noexcept
      -> ClientBuilderInterface& = 0;

  /*!
   * \brief       Set the callback that is called each time a state transition happened for the client to be built.
   * \param       on_state_transition_callback Callback that is called each time a state transition happened.
   *                                           Must not contain a nullptr.
   *                                           Must be valid for the whole lifetime of this client builder and the
   *                                           constructed client. This cannot be checked.
   * \return      Reference to this builder.
   * \context     ANY
   * \pre         Must not be called after a client was built using this instance.
   * \pre         Must only be called once.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private
   */
  virtual auto WithOnStateTransitionCallback(OnStateTransitionCallback on_state_transition_callback) noexcept
      -> ClientBuilderInterface& = 0;

  /*!
   * \brief       Build a client using the configured parameters.
   * \return      Client that was built using the provided information.
   * \context     ANY
   * \pre         Must not be called after a client was built using this instance.
   * \pre         All parameters must be configured before calling this method.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private
   */
  virtual auto Build() noexcept -> ::amsr::core::Result<::std::unique_ptr<ClientInterface>> = 0;

 protected:
  /*!
   * \brief    Construct a ClientBuilderInterface.
   * \vprivate Product Private
   */
  ClientBuilderInterface() noexcept = default;

  /*!
   * \brief    Construct a ClientBuilderInterface by copying.
   * \vprivate Product Private
   */
  ClientBuilderInterface(ClientBuilderInterface const&) noexcept = default;

  /*!
   * \brief    Construct a ClientBuilderInterface by moving.
   * \vprivate Product Private
   */
  ClientBuilderInterface(ClientBuilderInterface&&) noexcept = default;

  /*!
   * \brief    Copy another ClientBuilderInterface into this instance.
   * \vprivate Product Private
   */
  auto operator=(ClientBuilderInterface const&) & noexcept -> ClientBuilderInterface& = default;

  /*!
   * \brief    Move another ClientBuilderInterface into this instance.
   * \vprivate Product Private
   */
  auto operator=(ClientBuilderInterface&&) & noexcept -> ClientBuilderInterface& = default;
};

inline ClientBuilderInterface::~ClientBuilderInterface() noexcept = default;

}  // namespace client
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_CLIENT_CLIENT_BUILDER_INTERFACE_H_
