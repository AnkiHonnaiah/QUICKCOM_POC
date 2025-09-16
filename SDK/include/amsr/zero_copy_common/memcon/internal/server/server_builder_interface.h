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
 *        \brief  Definition of interface ServerBuilderInterface.
 *         \unit  ZeroCopyCommon::MemCon::Server
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_SERVER_BUILDER_INTERFACE_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_SERVER_BUILDER_INTERFACE_H_

#include <memory>

#include "amsr/core/optional.h"
#include "amsr/core/result.h"

#include "amsr/zero_copy_common/logic/internal/server_builder_interface.h"
#include "amsr/zero_copy_common/memcon/internal/server/server_interface.h"
#include "amsr/zero_copy_common/memcon/internal/side_channel/side_channel_interface.h"
#include "amsr/zero_copy_common/memcon/internal/types.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace server {

/*!
 * \brief Interface of a server builder.
 * \vprivate Product Private
 */
class ServerBuilderInterface {
 public:
  /*!
   * \brief Type used for a number of slots.
   */
  using NumSlots = ::amsr::zero_copy_common::memcon::internal::NumSlots;

  /*!
   * \brief Type used for a slot content size.
   */
  using SlotContentSize = ::amsr::zero_copy_common::memcon::internal::SlotContentSize;

  /*!
   * \brief Type used for a slot content alignment.
   */
  using SlotContentAlignment = ::amsr::zero_copy_common::memcon::internal::SlotContentAlignment;

  /*!
   * \brief Type used for a memory technology.
   */
  using MemoryTechnology = ::amsr::zero_copy_common::memcon::internal::MemoryTechnology;

  /*!
   * \brief Type used for a maximum number of receivers.
   */
  using MaxNumberReceivers = ::amsr::zero_copy_common::memcon::internal::MaxNumberReceivers;

  /*!
   * \brief The limit on the maximum number of receivers that can be managed by one server.
   */
  static constexpr MaxNumberReceivers kMaxNumberReceiversLimit{
      ::amsr::zero_copy_common::logic::internal::ServerBuilderInterface::kMaxNumberReceiversLimit};

  /*!
   * \brief Type used for a class handle.
   */
  using ClassHandle = ::amsr::zero_copy_common::memcon::internal::ClassHandle;

  /*!
   * \brief Type used for a receiver class number.
   */
  using ReceiverClassNumber = ::amsr::zero_copy_common::memcon::internal::ReceiverClassNumber;

  /*!
   * \brief Type used for a receiver class limit.
   */
  using ReceiverClassLimit = ::amsr::zero_copy_common::memcon::internal::ReceiverClassLimit;

  /*!
   * \copydoc ::amsr::zero_copy_common::memcon::internal::server::ServerInterface::OnReceiverStateTransitionCallback
   */
  using OnReceiverStateTransitionCallback =
      ::amsr::zero_copy_common::memcon::internal::server::ServerInterface::OnReceiverStateTransitionCallback;

  /*!
   * \brief Destruct the ServerBuilderInterface.
   * \vprivate Product Private
   */
  virtual ~ServerBuilderInterface() noexcept = 0;

  /*!
   * \brief       Set the number of slots for the server to be built.
   * \param       number_slots Number of slots for the server to be built.
   *                           Must be greater than zero.
   * \return      Reference to this builder.
   * \context     ANY
   * \pre         Must not be called after a server was built using this instance.
   * \pre         Must only be called once.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private
   */
  virtual auto WithNumberSlots(NumSlots number_slots) noexcept -> ServerBuilderInterface& = 0;

  /*!
   * \brief       Set the slot content size for the server to be built.
   * \param       slot_content_size Slot content size for the server to be built.
   *                                Must be greater than zero.
   * \return      Reference to this builder.
   * \context     ANY
   * \pre         Must not be called after a server was built using this instance.
   * \pre         Must only be called once.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private
   */
  virtual auto WithSlotContentSize(SlotContentSize slot_content_size) noexcept -> ServerBuilderInterface& = 0;

  /*!
   * \brief       Set the slot content alignment for the server to be built.
   * \param       slot_content_alignment Slot content alignment for the server to be built.
   *                                     Must be greater than zero.
   *                                     Must be a power of two.
   * \return      Reference to this builder.
   * \context     ANY
   * \pre         Must not be called after a server was built using this instance.
   * \pre         Must only be called once.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private
   */
  virtual auto WithSlotContentAlignment(SlotContentAlignment slot_content_alignment) noexcept
      -> ServerBuilderInterface& = 0;

  /*!
   * \brief       Set the memory technology for the server to be built.
   * \param       memory_technology Memory technology for the server to be built.
   * \return      Reference to this builder.
   * \context     ANY
   * \pre         Must not be called after a server was built using this instance.
   * \pre         Must only be called once.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private
   */
  virtual auto WithMemoryTechnology(MemoryTechnology memory_technology) noexcept -> ServerBuilderInterface& = 0;

  /*!
   * \brief       Set the maximum number of receivers for the server to be built.
   * \param       max_number_receivers Maximum number of receivers for the server to be built.
   *                                   Must be greater than zero.
   *                                   Must not exceed kMaxNumberReceiversLimit.
   *                                   Must not exceed std::numeric_limits<std::ptrdiff_t>::max().
   * \return      Reference to this builder.
   * \context     ANY
   * \pre         Must not be called after a server was built using this instance.
   * \pre         Must only be called once.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private
   */
  virtual auto WithMaxNumberReceivers(MaxNumberReceivers max_number_receivers) noexcept -> ServerBuilderInterface& = 0;

  /*!
   * \brief       Set the number of classes for the server to be built.
   * \details     The configured number of classes must be added using the WithClass() method.
   * \param       number_classes Number of classes for the server to be built.
   *                             Must be greater than zero.
   * \return      Reference to this builder.
   * \context     ANY
   * \pre         Must not be called after a server was built using this instance.
   * \pre         Must only be called once.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private
   */
  virtual auto WithNumberClasses(ReceiverClassNumber number_receiver_classes) noexcept -> ServerBuilderInterface& = 0;

  /*!
   * \brief       Add a class for the server to be built.
   * \param       receiver_class_limit The maximum number of slots that can be used by receivers of this receiver class
   *                                   at once.
   * \param[out]  opt_class_handle     Optional class handle which is emplaced with the handle for the newly added
   *                                   class.
   * \return      Reference to this builder.
   * \context     ANY
   * \pre         Must not be called after a server was built using this instance.
   * \pre         WithNumberSlots(), WithMaxNumberReceivers() and WithNumberClasses() must be called before adding any
   *              class.
   * \pre         The number of classes which is was set during WithNumberClasses() must not be exceeded.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private
   */
  virtual auto WithClass(ReceiverClassLimit receiver_class_limit,
                         ::amsr::core::Optional<ClassHandle>& opt_class_handle) noexcept -> ServerBuilderInterface& = 0;

  /*!
   * \brief       Set a callback which will be invoked on every asynchronous receiver state change.
   * \param       on_receiver_state_transition_cbk The callback which is invoked on every receiver state transition.
   *                                               Must not be a nullptr.
   *                                               Must be valid for the whole lifetime of this server builder and the
   *                                               constructed server. This cannot be checked.
   * \return      Reference to this builder.
   * \context     ANY
   * \pre         Must not be called after a server was built using this instance.
   * \pre         Must only be called once.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private
   */
  virtual auto WithOnReceiverStateTransitionCallback(
      OnReceiverStateTransitionCallback on_receiver_state_transition_cbk) noexcept -> ServerBuilderInterface& = 0;

  /*!
   * \brief       Build a server using the configured parameters.
   * \details     Only build a server if all parameters are configured and all receiver classes are provided.
   * \return      Server that was built using the provided information. The returned pointer is never a nullptr.
   * \error       ::amsr::zero_copy_common::common::ZeroCopyCommonErrc::kMemoryError If the allocation of either server
   *              queue memory, slot memory or both of them failed.
   * \context     ANY
   * \pre         Must not be called after a server was built using this instance.
   * \pre         All parameters must be configured and the configured number of receiver classes must be added before
   *              calling this method.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private
   */
  virtual auto Build() noexcept -> ::amsr::core::Result<std::unique_ptr<ServerInterface>> = 0;

 protected:
  /*!
   * \brief Construct a ServerBuilderInterface.
   * \vprivate Product Private
   */
  ServerBuilderInterface() noexcept = default;

  /*!
   * \brief Construct a ServerBuilderInterface by copying.
   * \vprivate Product Private
   */
  ServerBuilderInterface(ServerBuilderInterface const&) noexcept = default;

  /*!
   * \brief Construct a ServerBuilderInterface by moving.
   * \vprivate Product Private
   */
  ServerBuilderInterface(ServerBuilderInterface&&) noexcept = default;

  /*!
   * \brief Copy another ServerBuilderInterface into this instance.
   * \vprivate Product Private
   */
  auto operator=(ServerBuilderInterface const&) & noexcept -> ServerBuilderInterface& = default;

  /*!
   * \brief Move another ServerBuilderInterface into this instance.
   * \vprivate Product Private
   */
  auto operator=(ServerBuilderInterface&&) & noexcept -> ServerBuilderInterface& = default;
};

inline ServerBuilderInterface::~ServerBuilderInterface() noexcept = default;

}  // namespace server
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_SERVER_SERVER_BUILDER_INTERFACE_H_
