/*********************************************************************************************************************
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
 *        \brief  provide the functionality to deserialize the stored byte stream
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_METHODS_SKELETON_METHOD_REQUEST_XF_H_
#define LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_METHODS_SKELETON_METHOD_REQUEST_XF_H_

#include <memory>
#include <tuple>
#include <utility>

#include "amsr/socal/internal/methods/skeleton_method_request_interface.h"
#include "someip-protocol/internal/deserialization/reader.h"

#include "ipc_binding_core/internal/error_domain.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/logging/ipc_binding_log_builder.h"

namespace amsr {
namespace ipc_binding_transformation_layer {
namespace internal {
namespace methods {

/*!
 * \brief   Method request at skeleton side.
 * \details This class stores the request in serialized byte stream and also provides the deserialization logic for that
 *          method request. Deserialization is triggered by the application thread.
 *
 * \tparam RequestDeserializer  The deserializer type for the input arguments. Either void (if the method has no input
 *                              parameters), or a generated deserializer. Also used to inject a mocked deserializer for
 *                              testing.
 * \tparam InputArgs            The variadic method arguments.
 *
 * \unit IpcBinding::IpcBindingTransformationLayer::SkeletonMethod
 */
template <typename RequestDeserializer, typename... InputArgs>
class SkeletonMethodRequestXf final
    : public ::amsr::socal::internal::methods::SkeletonMethodRequestInterface<InputArgs...> {
 public:
  /*!
   * \brief Constructor of SkeletonMethodRequestXf.
   *
   * \param[in,out] request  An incoming method request to be deserialized and processed.
   *
   * \pre         -
   * \context     Reactor
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  explicit SkeletonMethodRequestXf(::amsr::ipc_binding_core::internal::ipc_protocol::IpcPacketShared&& request) noexcept
      // () is used to initialize the base class to suppress doxygen issue.
      : ::amsr::socal::internal::methods::SkeletonMethodRequestInterface<InputArgs...>(),
        request_{std::move(request)} {}

  SkeletonMethodRequestXf(SkeletonMethodRequestXf&&) noexcept = delete;
  auto operator=(SkeletonMethodRequestXf&&) noexcept -> SkeletonMethodRequestXf& = delete;
  SkeletonMethodRequestXf(SkeletonMethodRequestXf const&) noexcept = delete;
  auto operator=(SkeletonMethodRequestXf const&) noexcept -> SkeletonMethodRequestXf& = delete;

  /*!
   * \brief Destructor of SkeletonMethodRequestXf.
   *
   * \pre         -
   * \context     App
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  ~SkeletonMethodRequestXf() noexcept final = default;  // VCA_IPCB_STD_UNIQUE_POINTER_DESTRUCTOR, VCA_IPCB_MOLE_1298

  // VECTOR NC AutosarC++17_10-M7.1.2: MD_IPCBINDING_AutosarC++17_10-M7.1.2_config_variant
  /*!
   * \brief Retrieves request argument(s).
   *
   * \param[out] args  Tuple containing the default-initialized method request arguments.
   *
   * \return True if all the arguments are successfully deserialized, False otherwise.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   *
   * \internal
   * - Process the method request and request argument(s).
   * \endinternal
   */
  auto GetArgs(std::tuple<InputArgs...>& args) const noexcept -> bool final { return HandleMethodRequest(args); }

 private:
  /*!
   * \brief Deserialize service method.
   *
   * \tparam T  Is not void (method has request deserializer) this function will be available; otherwise it is removed.
   *
   * \param[out] args  Tuple containing the default-initialized method request arguments.
   *
   * \return Serialization result.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   *
   * \internal
   * - Deserialize the received message.
   * - Return deserialization result.
   * \endinternal
   */
  template <typename T = RequestDeserializer>
  auto HandleMethodRequest(std::tuple<InputArgs...>& args) const noexcept ->
      typename std::enable_if_t<!(std::is_void<T>::value), bool> {
    // Shortening
    namespace deserialization = ::amsr::someip_protocol::internal::deserialization;

    // PTP-B-IpcBinding-Xf-Skeleton-Deserialize-MethodRequest

    // Setup reader for payload deserialization
    deserialization::Reader::BufferView const message{request_->data(), request_->size()};
    deserialization::Reader reader{message.subspan(kHeaderLengthRequest)};

    bool const deserialization_ok{RequestDeserializer::Deserialize(reader, args)};

    // PTP-E-IpcBinding-Xf-Skeleton-Deserialize-MethodRequest
    return deserialization_ok;
  }

  /*!
   * \brief Dummy function when no deserializer is configured.
   *
   * \tparam T  Is void (method has no request deserializer) this function will be available; otherwise it is removed.
   *
   * \return True.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_IPCBINDING_AutosarC++17_10-M9.3.3_static_method
  template <typename T = RequestDeserializer>
  auto HandleMethodRequest(std::tuple<InputArgs...> const&) const noexcept ->
      typename std::enable_if_t<std::is_void<T>::value, bool> {
    return true;
  }

  /*!
   * \brief IPC header length for the RequestMessage messages including ProtocolMessage header length.
   */
  static ::amsr::ipc_binding_core::internal::ipc_protocol::MessageLength constexpr kHeaderLengthRequest{
      ::amsr::ipc_binding_core::internal::ipc_protocol::kProtocolMessageHeaderLength +
      ::amsr::ipc_binding_core::internal::ipc_protocol::kRequestMessageHeaderLength};

  /*!
   * \brief Incoming method request to be deserialized and processed.
   */
  ::amsr::ipc_binding_core::internal::ipc_protocol::IpcPacketShared request_;
};

}  // namespace methods
}  // namespace internal
}  // namespace ipc_binding_transformation_layer
}  // namespace amsr

#endif  // LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_METHODS_SKELETON_METHOD_REQUEST_XF_H_
