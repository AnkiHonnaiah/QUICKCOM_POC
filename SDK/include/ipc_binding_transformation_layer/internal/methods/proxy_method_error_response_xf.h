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
 *        \brief  Method error response transformation layer class.
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_METHODS_PROXY_METHOD_ERROR_RESPONSE_XF_H_
#define LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_METHODS_PROXY_METHOD_ERROR_RESPONSE_XF_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/core/result.h"
#include "ara/com/com_error_domain.h"
#include "someip-protocol/internal/deserialization/deser_ap_application_error.h"
#include "someip-protocol/internal/deserialization/reader.h"

#include "ipc_binding_core/internal/ipc_protocol/ipc_protocol_types.h"
#include "ipc_binding_core/internal/ipc_protocol/message.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_core/internal/logging/ipc_binding_log_builder.h"
#include "ipc_binding_transformation_layer/internal/methods/application_error_builder.h"

namespace amsr {
namespace ipc_binding_transformation_layer {
namespace internal {
namespace methods {

/*!
 * \brief   Method error response transformation layer class.
 * \details This class stores the error response in serialized byte stream and also provides the deserialization logic
 *          for that response. Deserialization is triggered by Application thread.
 *
 * \tparam Output  The Output type for method response.
 *
 * \unit IpcBinding::IpcBindingTransformationLayer::ProxyMethod
 */
template <typename Output>
class ProxyMethodErrorResponseXf final {
 public:
  /*!
   * \brief Constructor.
   *
   * \param[in] response  Error response message.
   *
   * \pre     -
   * \context Reactor
   * \steady  FALSE
   */
  explicit ProxyMethodErrorResponseXf(::amsr::ipc_binding_core::internal::ipc_protocol::ErrorResponseMessage&& response)
      : logger_{::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextId,
                ::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextDescription,
                ::amsr::core::StringView{"ProxyMethodErrorResponseXf"}},
        response_{std::move(response)} {};

  auto operator=(ProxyMethodErrorResponseXf const&) noexcept -> ProxyMethodErrorResponseXf& = delete;
  ProxyMethodErrorResponseXf(ProxyMethodErrorResponseXf const&) noexcept = delete;
  auto operator=(ProxyMethodErrorResponseXf&&) noexcept -> ProxyMethodErrorResponseXf& = delete;

  /*!
   * \brief Default move constructor.
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ProxyMethodErrorResponseXf(ProxyMethodErrorResponseXf&&) noexcept = default;

  /*!
   * \brief Destroys the object.
   *
   * \pre         -
   * \context     APP
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~ProxyMethodErrorResponseXf() noexcept = default;

  /*!
   * \brief Handles error response message.
   *
   * \return Result containing an error.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous FALSE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   *
   * \internal
   * - Return error response message.
   * \endinternal
   */
  auto operator()() noexcept -> ::amsr::core::Result<Output, ::amsr::core::ErrorCode> { return GetOutput(); }

 private:
  /*!
   * \brief Handles error response message.
   *
   * \return Result contains an error.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous FALSE
   * \steady      FALSE
   *
   * \internal
   * - Set an ErrorCode in result which indicates general network binding failure.
   * - Get the return code from message's header.
   * - If return code is kServiceNotAvailable.
   *   - Set result as error which indicates service is not available.
   * - Return the result.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_IPCBINDING_AutosarC++17_10-M9.3.3_const_method_getter
  auto GetOutput() noexcept -> ::amsr::core::Result<Output, ::amsr::core::ErrorCode> {
    ::amsr::core::Result<Output, ::amsr::core::ErrorCode> result{
        ::amsr::core::ErrorCode{::ara::com::ComErrc::kNetworkBindingFailure}};
    ::amsr::ipc_binding_core::internal::ipc_protocol::ErrorResponseMessageHeader const& header{
        response_.GetMessageHeader()};
    ::amsr::ipc_binding_core::internal::ipc_protocol::ReturnCode const error_code{header.return_code_};

    if (error_code == ::amsr::ipc_binding_core::internal::ipc_protocol::ReturnCode::kServiceNotAvailable) {
      result.EmplaceError(::amsr::core::ErrorCode{::ara::com::ComErrc::kServiceNotAvailable});
    }

    return result;
  }

  /*!
   * \brief Logger for printing debug and error messages.
   */
  ::amsr::ipc_binding_core::internal::logging::AraComLogger logger_;

  /*!
   * \brief Stores the response.
   */
  ::amsr::ipc_binding_core::internal::ipc_protocol::ErrorResponseMessage response_;
};

}  // namespace methods
}  // namespace internal
}  // namespace ipc_binding_transformation_layer
}  // namespace amsr

#endif  // LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_METHODS_PROXY_METHOD_ERROR_RESPONSE_XF_H_
