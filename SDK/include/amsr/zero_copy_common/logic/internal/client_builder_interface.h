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
 *        \brief  Declaration of interface ClientBuilderInterface.
 *         \unit  ZeroCopyCommon::ZeroCopyLogic::Client
 *
 *********************************************************************************************************************/
#ifndef LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_CLIENT_BUILDER_INTERFACE_H_
#define LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_CLIENT_BUILDER_INTERFACE_H_

#include <memory>

#include "amsr/zero_copy_common/logic/internal/client_interface.h"
#include "amsr/zero_copy_common/logic/internal/readable_slot_descriptor.h"
#include "amsr/zero_copy_common/logic/internal/reading_queue_end_interface.h"
#include "amsr/zero_copy_common/logic/internal/slot_handle.h"
#include "amsr/zero_copy_common/logic/internal/writing_queue_end_interface.h"

namespace amsr {
namespace zero_copy_common {
namespace logic {
namespace internal {

/*!
 * \brief Interface of a client builder.
 * \details A client builder offers to build a client by providing the necessary information.
 *          The client builder performs basic sanity checks and only builds a client if the provided information
 *          meets the expectations of the client builder.
 * \vprivate Component Private
 */
class ClientBuilderInterface {
 public:
  /*!
   * \brief Destruct the ClientBuilderInterface.
   * \vprivate Component Private
   */
  virtual ~ClientBuilderInterface() noexcept = 0;

  /*!
   * \brief Set the number of slots for the client to be built.
   * \details The configured number of slots must be added using the AddSlot() method.
   * \param number_slots Number of slots for the client to be built.
   * \context ANY
   * \pre Must not be called after a client was built using this instance.
   * \pre Must only be called once.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual void WithNumberSlots(SlotHandle::SlotIndex number_slots) noexcept = 0;

  /*!
   * \brief Add a slot for the client to be built.
   * \details This will transfer ownership of the given \p slot_descriptor to the client builder.
   *          Once the client is successfully built, ownership will be passed to the client.
   *          The passed \p slot_descriptor is alive as long as the client builder or client instance exists.
   *          The user of the client builder or client must ensure that the memory referenced by the \p slot_descriptor
   *          outlives this client builder and the constructed client.
   * \param slot_descriptor Slot descriptor for the client to be built.
   *                        The referenced memory must be valid for the whole lifetime of this client builder and the
   *                        constructed client.
   *                        Must not be a nullptr.
   * \context ANY
   * \pre Must not be called after a client was built using this instance.
   * \pre WithNumberSlots() must be called before adding any slot descriptor. The set number of slots must not be
   *      exceeded.
   * \pre The queue implementations and the slot implementations must properly synchronize with each other.
   *      This cannot be checked. See the CAD for more details.
   * \pre The slot descriptors (referring to the same memory) must be added in the same order for the server and all
   *      clients. This cannot be checked. See the CAD for more details.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual void AddSlot(std::unique_ptr<ReadableSlotDescriptor> slot_descriptor) noexcept = 0;

  /*!
   * \brief Set the writable free queue for the client to be built.
   * \param free_queue_ptr Free queue for the client to be built.
   *                       Must not contain nullptr.
   *                       Must point to valid objects. This cannot be checked.
   *                       Must provide valid implementations of the respective base class. This cannot be checked.
   * \context ANY
   * \pre Must not be called after a client was built using this instance.
   * \pre Must only be called once.
   * \pre The queue implementations and the slot implementations must properly synchronize with each other.
   *      This cannot be checked. See the CAD for more details.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual void WithFreeQueue(std::unique_ptr<WritingQueueEndInterface> free_queue_ptr) noexcept = 0;

  /*!
   * \brief Set the readable available queue for the client to be built.
   * \param available_queue_ptr Available queue for the client to be built.
   *                            Must not contain nullptr.
   *                            Must point to valid objects. This cannot be checked.
   *                            Must provide valid implementations of the respective base class. This cannot be checked.
   * \context ANY
   * \pre Must not be called after a client was built using this instance.
   * \pre Must only be called once.
   * \pre The queue implementations and the slot implementations must properly synchronize with each other.
   *      This cannot be checked. See the CAD for more details.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual void WithAvailableQueue(std::unique_ptr<ReadingQueueEndInterface> available_queue_ptr) noexcept = 0;

  /*!
   * \brief Build a client using the provided queues and slots.
   * \details Only build a client if all queues and slots are provided.
   * \return Client that was built using the provided information.
   * \context ANY
   * \pre Must not be called after a client was built using this instance.
   * \pre The number of slots must be set and the same number of slots must be added before calling this method.
   * \pre The free queue and the available queue must be provided before calling this method.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual auto Build() noexcept -> std::unique_ptr<ClientInterface> = 0;

 protected:
  /*!
   * \brief Construct a ClientBuilderInterface.
   * \vprivate Component Private
   */
  ClientBuilderInterface() noexcept = default;

  /*!
   * \brief Construct a ClientBuilderInterface by copying.
   * \vprivate Component Private
   */
  ClientBuilderInterface(ClientBuilderInterface const&) noexcept = default;

  /*!
   * \brief Construct a ClientBuilderInterface by moving.
   * \vprivate Component Private
   */
  ClientBuilderInterface(ClientBuilderInterface&&) noexcept = default;

  /*!
   * \brief Copy another ClientBuilderInterface into this instance.
   * \vprivate Component Private
   */
  auto operator=(ClientBuilderInterface const&) & noexcept -> ClientBuilderInterface& = default;

  /*!
   * \brief Move another ClientBuilderInterface into this instance.
   * \vprivate Component Private
   */
  auto operator=(ClientBuilderInterface&&) & noexcept -> ClientBuilderInterface& = default;
};

inline ClientBuilderInterface::~ClientBuilderInterface() noexcept = default;

}  // namespace internal
}  // namespace logic
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_CLIENT_BUILDER_INTERFACE_H_
