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
 *        \brief  Interface that intend to be extended by fire and forget method xf backend.
 *
 *      \details  The fire and forget method xf backend is templated and needs an interface to store it in the list
 *which is present in the generated SkeletonXf class.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_METHODS_SKELETON_FIRE_AND_FORGET_METHOD_XF_BACKEND_INTERFACE_H_
#define LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_METHODS_SKELETON_FIRE_AND_FORGET_METHOD_XF_BACKEND_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ipc_binding_core/internal/ipc_protocol/message.h"
namespace amsr {
namespace ipc_binding_transformation_layer {
namespace internal {
namespace methods {

/*!
 * \brief Method xf backend to handle fire and forget method request on skeleton side.
 *
 * \unit IpcBinding::IpcBindingTransformationLayer::SkeletonMethod
 */
class SkeletonFireAndForgetMethodXfBackendInterface {
 public:
  SkeletonFireAndForgetMethodXfBackendInterface(SkeletonFireAndForgetMethodXfBackendInterface const&) noexcept = delete;
  auto operator=(SkeletonFireAndForgetMethodXfBackendInterface const&) noexcept
      -> SkeletonFireAndForgetMethodXfBackendInterface& = delete;
  SkeletonFireAndForgetMethodXfBackendInterface(SkeletonFireAndForgetMethodXfBackendInterface&&) noexcept = delete;
  auto operator=(SkeletonFireAndForgetMethodXfBackendInterface&&) noexcept
      -> SkeletonFireAndForgetMethodXfBackendInterface& = delete;

  /*!
   * \brief Default destructor.
   *
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  virtual ~SkeletonFireAndForgetMethodXfBackendInterface() noexcept = default;

  /*!
   * \brief Forwards the fire and forget method request to the request handler.
   *
   * \param[in,out] request  Incoming method request to be deserialized and processed.
   *
   * \pre     -
   * \context Reactor
   * \steady  TRUE
   */
  virtual auto HandleMethodRequest(
      ::amsr::ipc_binding_core::internal::ipc_protocol::RequestNoReturnMessage&& request) noexcept -> void = 0;

 protected:
  /*!
   * \brief Default constructor.
   *
   * \pre     -
   * \context App
   * \steady  FALSE
   */
  SkeletonFireAndForgetMethodXfBackendInterface() noexcept = default;
};

}  // namespace methods
}  // namespace internal
}  // namespace ipc_binding_transformation_layer
}  // namespace amsr

#endif  // LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_METHODS_SKELETON_FIRE_AND_FORGET_METHOD_XF_BACKEND_INTERFACE_H_
