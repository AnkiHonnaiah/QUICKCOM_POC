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
 *        \brief  Defines an interface for fire and forget proxy method backends.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_BACKEND_INTERFACES_PROXY_FIRE_AND_FORGET_METHOD_BACKEND_INTERFACE_H_
#define LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_BACKEND_INTERFACES_PROXY_FIRE_AND_FORGET_METHOD_BACKEND_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace amsr {
namespace ipc_binding_core {
namespace internal {

/*!
 * \brief Interface for proxy fire and forget method backends.
 *
 * \unit IpcBinding::IpcBindingCore::ConnectionProxy
 */
class ProxyFireAndForgetMethodBackendInterface {
 public:
  /*!
   * \brief Constructor, use default.
   *
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  ProxyFireAndForgetMethodBackendInterface() noexcept = default;

  /*!
   * \brief Destructor, use default.
   *
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  virtual ~ProxyFireAndForgetMethodBackendInterface() noexcept = default;

  ProxyFireAndForgetMethodBackendInterface(ProxyFireAndForgetMethodBackendInterface const&) noexcept = delete;
  ProxyFireAndForgetMethodBackendInterface(ProxyFireAndForgetMethodBackendInterface&&) noexcept = delete;
  auto operator=(ProxyFireAndForgetMethodBackendInterface const&) noexcept
      -> ProxyFireAndForgetMethodBackendInterface& = delete;
  auto operator=(ProxyFireAndForgetMethodBackendInterface&&) noexcept
      -> ProxyFireAndForgetMethodBackendInterface& = delete;

  /*!
   * \brief Set the service instance state.
   *
   * \param[in] service_state  Boolean indicating new service state (ServiceUp or ServiceDown).
   *
   * \pre     -
   * \context Reactor
   * \steady  FALSE
   */
  virtual auto SetServiceState(bool service_state) noexcept -> void = 0;
};

}  // namespace internal
}  // namespace ipc_binding_core
}  // namespace amsr

#endif  // LIB_IPC_BINDING_CORE_INCLUDE_IPC_BINDING_CORE_INTERNAL_BACKEND_INTERFACES_PROXY_FIRE_AND_FORGET_METHOD_BACKEND_INTERFACE_H_
