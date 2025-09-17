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
 *        \brief Declaration of interface ServerBuilderInterface.
 *         \unit ZeroCopyCommon::ZeroCopyLogic::Server
 *
 *********************************************************************************************************************/
#ifndef LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_SERVER_BUILDER_INTERFACE_H_
#define LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_SERVER_BUILDER_INTERFACE_H_

#include <memory>

#include "amsr/zero_copy_common/logic/internal/borrowed_manager.h"
#include "amsr/zero_copy_common/logic/internal/class_handle.h"
#include "amsr/zero_copy_common/logic/internal/receiver_class.h"
#include "amsr/zero_copy_common/logic/internal/receiver_handle.h"
#include "amsr/zero_copy_common/logic/internal/server_interface.h"
#include "amsr/zero_copy_common/logic/internal/slot_handle.h"
#include "amsr/zero_copy_common/logic/internal/writable_slot_descriptor.h"

namespace amsr {
namespace zero_copy_common {
namespace logic {
namespace internal {

/*!
 * \brief Interface of a server builder.
 * \details A server builder offers to build a server by providing the necessary information.
 *          The server builder performs basic sanity checks and only builds a server if the provided information
 *          meets the expectations of the server builder.
 * \vprivate Component Private
 */
class ServerBuilderInterface {
 public:
  /*!
   * \brief The limit on the maximum number of receivers that can be managed by one server.
   */
  static constexpr ReceiverHandle::ReceiverIndex kMaxNumberReceiversLimit{BorrowedManager::kReceiverLimit};

  /*!
   * \brief Destruct the ServerBuilderInterface.
   * \vprivate Component Private
   */
  virtual ~ServerBuilderInterface() noexcept = 0;

  /*!
   * \brief Set the number of slots for the server to be built.
   * \details The configured number of slots must be added using the AddSlot() method.
   * \param number_slots Number of slots for the server to be built.
   * \context ANY
   * \pre Must not be called after a server was built using this instance.
   * \pre Must only be called once.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual void WithNumberSlots(SlotHandle::SlotIndex number_slots) noexcept = 0;

  /*!
   * \brief Set the maximum number of receivers for the server to be built.
   * \param max_number_receivers Maximum number of receivers for the server to be built.
   *                             Must not exceed kMaxNumberReceiversLimit.
   * \context ANY
   * \pre Must not be called after a server was built using this instance.
   * \pre Must only be called once.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual void WithMaxNumberReceivers(ReceiverHandle::ReceiverIndex max_number_receivers) noexcept = 0;

  /*!
   * \brief Set the number of classes for the server to be built.
   * \details The configured number of classes must be added using the AddClass() method.
   * \param number_classes Number of classes for the server to be built.
   * \context ANY
   * \pre Must not be called after a server was built using this instance.
   * \pre Must only be called once.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual void WithNumberClasses(ClassHandle::ReceiverClassIndex number_classes) noexcept = 0;

  /*!
   * \brief Add a slot for the server to be built.
   * \details This will transfer ownership of the given \p slot_descriptor to the server builder.
   *          Once the server is successfully built, ownership will be passed to the server.
   *          The passed \p slot_descriptor is alive as long as the server builder or server instance exists.
   *          The user of the server builder or server must ensure that the memory referenced by the \p slot_descriptor
   *          outlives this server builder and the constructed server.
   * \param slot_descriptor Slot descriptor for the server to be built.
   *                        The referenced memory must be valid for the whole lifetime of this server builder and the
   *                        constructed server.
   *                        Must not be a nullptr.
   * \context ANY
   * \pre Must not be called after a server was built using this instance.
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
  virtual void AddSlot(std::unique_ptr<WritableSlotDescriptor> slot_descriptor) noexcept = 0;

  /*!
   * \brief Add a class for the server to be built.
   * \param receiver_class_limit The maximum number of slots that can be used by receivers of this receiver class at
   *                             once.
   * \return Handle representing the newly added receiver class.
   * \context ANY
   * \pre Must not be called after a server was built using this instance.
   * \pre WithNumberSlots(), WithMaxNumberReceivers() and WithNumberClasses() must be called before adding any class.
   *      The set number of classes must not be exceeded.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual auto AddClass(ReceiverClass::ClassLimit receiver_class_limit) noexcept -> ClassHandle = 0;

  /*!
   * \brief Build a server using the provided slots, receiver classes and the maximum number of receivers.
   * \details Only build a server if all slots and receiver classes are provided.
   *          Calls ResetSlotMemory() for all slot descriptors before building a server.
   * \return Server that was built using the provided information.
   * \context ANY
   * \pre Must not be called after a server was built using this instance.
   * \pre The number of slots must be set and the same number of slots must be added before calling this method.
   * \pre The number of receiver classes must be set and the same number of receiver classes must be added before
   *      calling this method.
   * \pre The maximum number of receivers must be set before calling this method.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  virtual auto Build() noexcept -> std::unique_ptr<ServerInterface> = 0;

 protected:
  /*!
   * \brief Construct a ServerBuilderInterface.
   * \vprivate Component Private
   */
  ServerBuilderInterface() noexcept = default;

  /*!
   * \brief Construct a ServerBuilderInterface by copying.
   * \vprivate Component Private
   */
  ServerBuilderInterface(ServerBuilderInterface const&) noexcept = default;

  /*!
   * \brief Construct a ServerBuilderInterface by moving.
   * \vprivate Component Private
   */
  ServerBuilderInterface(ServerBuilderInterface&&) noexcept = default;

  /*!
   * \brief Copy another ServerBuilderInterface into this instance.
   * \vprivate Component Private
   */
  auto operator=(ServerBuilderInterface const&) & noexcept -> ServerBuilderInterface& = default;

  /*!
   * \brief Move another ServerBuilderInterface into this instance.
   * \vprivate Component Private
   */
  auto operator=(ServerBuilderInterface&&) & noexcept -> ServerBuilderInterface& = default;
};

inline ServerBuilderInterface::~ServerBuilderInterface() noexcept = default;

}  // namespace internal
}  // namespace logic
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_SERVER_BUILDER_INTERFACE_H_
