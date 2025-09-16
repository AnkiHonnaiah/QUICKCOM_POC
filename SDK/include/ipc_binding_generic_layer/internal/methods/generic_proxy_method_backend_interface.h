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
 *        \brief  Generic proxy method backend interface.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_GENERIC_LAYER_INCLUDE_IPC_BINDING_GENERIC_LAYER_INTERNAL_METHODS_GENERIC_PROXY_METHOD_BACKEND_INTERFACE_H_
#define LIB_IPC_BINDING_GENERIC_LAYER_INCLUDE_IPC_BINDING_GENERIC_LAYER_INTERNAL_METHODS_GENERIC_PROXY_METHOD_BACKEND_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/generic/activatable_source_code.h"
#include "amsr/socal/internal/methods/generic_proxy_method_backend_interface.h"

#include "ipc_binding_core/internal/backend_interfaces/proxy_method_backend_interface.h"

namespace amsr {
namespace ipc_binding_generic_layer {
namespace internal {
namespace methods {

/*!
 * \brief Generic Proxy Method backend interface.
 *
 * \unit IpcBinding::IpcBindingGenericLayer::GenericProxyMethod
 */
class AMSR_LG_BETA GenericProxyMethodBackendInterface
    : public amsr::socal::internal::methods::GenericProxyMethodBackendInterface,
      public ipc_binding_core::internal::ProxyMethodBackendInterface {
 public:
  /*!
   * \brief Type alias for method id.
   */
  using MethodId = ipc_binding_core::internal::ipc_protocol::MethodId;

  /*!
   * \brief Default constructor.
   *
   * \pre        -
   * \context    App
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady     FALSE
   */
  GenericProxyMethodBackendInterface() noexcept = default;

  /*!
   * \brief Default destructor.
   *
   * \pre        -
   * \context    App
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady     FALSE
   */
  ~GenericProxyMethodBackendInterface() noexcept override = default;

  GenericProxyMethodBackendInterface(GenericProxyMethodBackendInterface const&) = delete;
  GenericProxyMethodBackendInterface(GenericProxyMethodBackendInterface&&) = delete;
  auto operator=(GenericProxyMethodBackendInterface const&) -> GenericProxyMethodBackendInterface& = delete;
  auto operator=(GenericProxyMethodBackendInterface&&) -> GenericProxyMethodBackendInterface& = delete;

  /*!
   * \brief Return the currently used method id for this backend.
   *
   * \return The method id.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   */
  virtual auto GetMethodId() const noexcept -> MethodId = 0;
};

}  // namespace methods
}  // namespace internal
}  // namespace ipc_binding_generic_layer
}  // namespace amsr

#endif  // LIB_IPC_BINDING_GENERIC_LAYER_INCLUDE_IPC_BINDING_GENERIC_LAYER_INTERNAL_METHODS_GENERIC_PROXY_METHOD_BACKEND_INTERFACE_H_
