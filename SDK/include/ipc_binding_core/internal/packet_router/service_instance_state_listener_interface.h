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
 *        \brief  Defines types related to provided service instance state.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_PACKET_ROUTER_SERVICE_INSTANCE_STATE_LISTENER_INTERFACE_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_PACKET_ROUTER_SERVICE_INSTANCE_STATE_LISTENER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace amsr {
namespace ipc_binding_core {
namespace internal {

/*!
 * \brief Interface for retrieving ServiceUp/ServiceDown events.
 *
 * \unit IpcBinding::IpcBindingCore::PacketRouterProxy
 */
class ServiceInstanceStateListenerInterface {
 public:
  /*!
   * \brief Constructor, use default.
   *
   * \pre     -
   * \context App
   */
  ServiceInstanceStateListenerInterface() noexcept = default;

  /*!
   * \brief Destructor, use default.
   *
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  virtual ~ServiceInstanceStateListenerInterface() noexcept = default;

  ServiceInstanceStateListenerInterface(ServiceInstanceStateListenerInterface const&) noexcept = delete;
  ServiceInstanceStateListenerInterface(ServiceInstanceStateListenerInterface&&) noexcept = delete;
  auto operator=(ServiceInstanceStateListenerInterface const&) noexcept
      -> ServiceInstanceStateListenerInterface& = delete;
  auto operator=(ServiceInstanceStateListenerInterface&&) noexcept -> ServiceInstanceStateListenerInterface& = delete;

  /*!
   * \brief Called when an offerservice message has been received.
   *
   * \context Reactor
   */
  virtual auto OnServiceInstanceUp() noexcept -> void = 0;

  /*!
   * \brief Called when an stopofferservice message has been received.
   *
   * \context Reactor
   */
  virtual auto OnServiceInstanceDown() noexcept -> void = 0;
};

}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_PACKET_ROUTER_SERVICE_INSTANCE_STATE_LISTENER_INTERFACE_H_
