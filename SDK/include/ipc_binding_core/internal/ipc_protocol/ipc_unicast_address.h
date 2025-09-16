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
 *        \brief  IPC unicast address.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_IPC_PROTOCOL_IPC_UNICAST_ADDRESS_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_IPC_PROTOCOL_IPC_UNICAST_ADDRESS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/ipc/unicast_address.h"

namespace amsr {
namespace ipc_binding_core {
namespace internal {
namespace ipc_protocol {

/*!
 * \brief Type for BasicIpc port.
 */
using BasicIpcDomain = amsr::ipc::Domain;

/*!
 * \brief Type for BasicIpc port.
 */
using BasicIpcPort = amsr::ipc::Port;

/*!
 * \brief   Basic IPC unicast address.
 * \details Contains the BasicIPC domain and port on which the provided service instance shall be reachable.
 *
 * \unit IpcBinding::IpcBindingCore::IpcProtocol
 */
class IpcUnicastAddress final {
 public:
  /*!
   * \brief Construct an IPC unicast address.
   *
   * \param[in] domain  Address domain number.
   * \param[in] port    Address port number.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  IpcUnicastAddress(BasicIpcDomain const domain, BasicIpcPort const port) noexcept : domain_{domain}, port_{port} {};

  /*!
   * \brief Copy-Construct an IPC unicast address.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  IpcUnicastAddress(IpcUnicastAddress const&) noexcept = default;

  /*!
   * \brief Copy-Assign an IPC unicast address.
   *
   * \return Reference to the copy-assigned unicast address.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto operator=(IpcUnicastAddress const&) & noexcept -> IpcUnicastAddress& = default;

  /*!
   * \brief Move-Construct an IPC unicast address.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  IpcUnicastAddress(IpcUnicastAddress&&) noexcept = default;

  /*!
   * \brief Move-Assign an IPC unicast address.
   *
   * \return Reference to the move-assigned unicast address.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto operator=(IpcUnicastAddress&&) & noexcept -> IpcUnicastAddress& = default;

  /*!
   * \brief Destroy the unicast address.
   *
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~IpcUnicastAddress() noexcept = default;

  /*!
   * \brief Compares this IPC unicast address to the given one for equality.
   *
   * \param[in] other  An IPC unicast address to compare to.
   *
   * \return True if both the IPC unicast addresses are equal, false otherwise.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  auto operator==(IpcUnicastAddress const& other) const noexcept -> bool;

  /*!
   * \brief Compares this IPC unicast address to the given one.
   *
   * \param[in] other  An IPC unicast address to compare to.
   *
   * \return True if the current address is less than the other.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  auto operator<(IpcUnicastAddress const& other) const noexcept -> bool;

  /*!
   * \brief Get the domain.
   *
   * \return The domain.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  auto Domain() const noexcept -> BasicIpcDomain;

  /*!
   * \brief Get the port.
   *
   * \return The port.
   *
   * \pre       -
   * \context   ANY
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \spec requires true; \endspec
   */
  auto Port() const noexcept -> BasicIpcPort;

 private:
  /*!
   * \brief BasicIPC domain.
   */
  BasicIpcDomain domain_;

  /*!
   * \brief BasicIPC port.
   */
  BasicIpcPort port_;
};

}  // namespace ipc_protocol
}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_IPC_PROTOCOL_IPC_UNICAST_ADDRESS_H_
