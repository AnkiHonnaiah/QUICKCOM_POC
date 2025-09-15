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
/**        \file  someip_binding_xf/internal/methods/application_error_builder.h
 *        \brief  -
 *        \unit   SomeIpBinding::SomeIpBindingXf::Communication::Methods::ProxyMethodXf
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_APPLICATION_ERROR_BUILDER_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_APPLICATION_ERROR_BUILDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include "amsr/core/error_code.h"
#include "amsr/core/optional.h"
#include "amsr/someip_binding_core/internal/methods/application_error_response.h"
#include "amsr/someip_binding_xf/internal/methods/types.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace methods {

/*!
 * \brief Builder to create an amsr::core::ErrorCode for Application Errors of a specific method.
 */
class ApplicationErrorBuilder final {
 public:
  /*!
   * \brief An alias to the map of errordomain.
   */
  using ApplicationErrorMap = std::map<amsr::core::ErrorDomain::IdType, ApplicationErrorDomain>;

  /*!
   * \brief   Builder to create an amsr::core::ErrorCode from a specific error domain.
   *
   * \param[in] application_errors         Application error domain map.
   * \param[in] application_error_response Application error response
   * \param[in] user_message               The user message.
   * \return The error code.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \trace       SPEC-13650730
   * \spec
   *   requires true;
   * \endspec
   */
  static ::amsr::core::Optional<::amsr::core::ErrorCode> CreateErrorCode(
      ApplicationErrorMap const& application_errors,
      ::amsr::someip_binding_core::internal::methods::ApplicationErrorResponse const& application_error_response,
      ::amsr::core::ErrorDomain::StringType const& user_message) noexcept;
};

}  // namespace methods
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_APPLICATION_ERROR_BUILDER_H_
