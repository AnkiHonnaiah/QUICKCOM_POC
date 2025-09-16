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
 *        \brief  Generic proxy fire and forget method backend interface.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_GENERIC_LAYER_INCLUDE_IPC_BINDING_GENERIC_LAYER_INTERNAL_METHODS_GENERIC_PROXY_FIRE_AND_FORGET_METHOD_BACKEND_INTERFACE_H_
#define LIB_IPC_BINDING_GENERIC_LAYER_INCLUDE_IPC_BINDING_GENERIC_LAYER_INTERNAL_METHODS_GENERIC_PROXY_FIRE_AND_FORGET_METHOD_BACKEND_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/generic/activatable_source_code.h"
#include "amsr/socal/internal/methods/generic_proxy_fire_and_forget_method_backend_interface.h"

#include "ipc_binding_core/internal/backend_interfaces/proxy_fire_and_forget_method_backend_interface.h"

namespace amsr {
namespace ipc_binding_generic_layer {
namespace internal {
namespace methods {

/*!
 * \brief Generic Proxy Fire and Forget Method backend interface.
 *
 * \unit IpcBinding::IpcBindingGenericLayer::GenericProxyMethod
 */
class AMSR_LG_BETA GenericProxyFireAndForgetMethodBackendInterface
    : public amsr::socal::internal::methods::GenericProxyFireAndForgetMethodBackendInterface,
      public ipc_binding_core::internal::ProxyFireAndForgetMethodBackendInterface {
 public:
  GenericProxyFireAndForgetMethodBackendInterface() noexcept = default;
  ~GenericProxyFireAndForgetMethodBackendInterface() noexcept override = default;

  GenericProxyFireAndForgetMethodBackendInterface(GenericProxyFireAndForgetMethodBackendInterface const&) = delete;
  GenericProxyFireAndForgetMethodBackendInterface(GenericProxyFireAndForgetMethodBackendInterface&&) = delete;
  auto operator=(GenericProxyFireAndForgetMethodBackendInterface const&)
      -> GenericProxyFireAndForgetMethodBackendInterface& = delete;
  auto operator=(GenericProxyFireAndForgetMethodBackendInterface&&)
      -> GenericProxyFireAndForgetMethodBackendInterface& = delete;
};

}  // namespace methods
}  // namespace internal
}  // namespace ipc_binding_generic_layer
}  // namespace amsr

#endif  // LIB_IPC_BINDING_GENERIC_LAYER_INCLUDE_IPC_BINDING_GENERIC_LAYER_INTERNAL_METHODS_GENERIC_PROXY_FIRE_AND_FORGET_METHOD_BACKEND_INTERFACE_H_
