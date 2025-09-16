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
 *        \brief  -
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_METHODS_APPLICATION_ERROR_BUILDER_H_
#define LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_METHODS_APPLICATION_ERROR_BUILDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/core/error_code.h"
#include "ara/core/map.h"
#include "someip-protocol/internal/ap_application_error.h"

#include "ipc_binding_core/internal/logging/ara_com_logger.h"
#include "ipc_binding_transformation_layer/internal/methods/types.h"

namespace amsr {
namespace ipc_binding_transformation_layer {
namespace internal {
namespace methods {

/*!
 * \brief Builder to create an ara::core::ErrorCode for Application Errors of a specific method.
 *
 * \steady FALSE
 *
 * \unit IpcBinding::IpcBindingTransformationLayer::ProxyMethod
 */
class ApplicationErrorBuilder final {
 public:
  /*!
   * \brief An alias to the map of errordomain.
   */
  using ApplicationErrorMap = ara::core::Map<::ara::core::ErrorDomain::IdType, ApplicationErrorDomain>;

  /*!
   * \brief Constructor.
   *
   * \param[in] error_codes  The error domain map.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  explicit ApplicationErrorBuilder(ApplicationErrorMap const& error_codes);

  ApplicationErrorBuilder(ApplicationErrorBuilder&&) noexcept = delete;
  auto operator=(ApplicationErrorBuilder&&) noexcept -> ApplicationErrorBuilder& = delete;
  ApplicationErrorBuilder(ApplicationErrorBuilder const&) noexcept = delete;
  auto operator=(ApplicationErrorBuilder const&) noexcept -> ApplicationErrorBuilder& = delete;

  /*!
   * \brief Destroy the builder.
   *
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~ApplicationErrorBuilder() noexcept = default;

  /*!
   * \brief Builder to create an ara::core::ErrorCode from a specific error domain.
   *
   * \param[in] error_struct  The ApApplicationError structure.
   *
   * \return The error code.
   *
   * \pre         -
   * \context     Callback
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto CreateErrorCode(::amsr::someip_protocol::internal::serialization::ApApplicationErrorStruct& error_struct) const
      -> ::ara::core::ErrorCode;

 private:
  /*!
   * \brief Logger for printing debug and error messages.
   */
  ::amsr::ipc_binding_core::internal::logging::AraComLogger logger_;

  /*!
   * \brief Application error domain map.
   */
  ApplicationErrorMap const& application_errors_;
};

}  // namespace methods
}  // namespace internal
}  // namespace ipc_binding_transformation_layer
}  // namespace amsr

#endif  // LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_METHODS_APPLICATION_ERROR_BUILDER_H_
