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
/**        \file
 *        \brief  Interface that intend to be extended by method xf backend.
 *
 *      \details  Method xf backend is templated and needs an interface to store it in the list which is present in the
 *                generated SkeletonXf class.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_METHODS_SKELETON_METHOD_XF_BACKEND_INTERFACE_H_
#define LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_METHODS_SKELETON_METHOD_XF_BACKEND_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ipc_binding_core/internal/ipc_protocol/message.h"
namespace amsr {
namespace ipc_binding_transformation_layer {
namespace internal {
namespace methods {

/*!
 * \brief Method xf backend to handle method request on skeleton side.
 *
 * \unit IpcBinding::IpcBindingTransformationLayer::SkeletonMethod
 */
class SkeletonMethodXfBackendInterface {
 public:
  /*!
   * \brief Default destructor.
   *
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  virtual ~SkeletonMethodXfBackendInterface() noexcept = default;

  /*!
   * \brief Forward method request to request handler.
   *
   * \param[in,out] request  Incoming method request to be deserialized and processed.
   *
   * \pre     -
   * \context Reactor
   * \steady  TRUE
   */
  virtual auto HandleMethodRequest(
      ::amsr::ipc_binding_core::internal::ipc_protocol::RemoteRequestMessage&& request) noexcept -> void = 0;

  SkeletonMethodXfBackendInterface(SkeletonMethodXfBackendInterface const&) noexcept = delete;
  auto operator=(SkeletonMethodXfBackendInterface const&) noexcept -> SkeletonMethodXfBackendInterface& = delete;
  SkeletonMethodXfBackendInterface(SkeletonMethodXfBackendInterface&&) noexcept = delete;
  auto operator=(SkeletonMethodXfBackendInterface&&) noexcept -> SkeletonMethodXfBackendInterface& = delete;

 protected:
  /*!
   * \brief Default constructor.
   *
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  SkeletonMethodXfBackendInterface() noexcept = default;
};

}  // namespace methods
}  // namespace internal
}  // namespace ipc_binding_transformation_layer
}  // namespace amsr
#endif  // LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_METHODS_SKELETON_METHOD_XF_BACKEND_INTERFACE_H_
