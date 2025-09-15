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
/**        \file  someip_binding_core/internal/methods/method_response.h
 *        \brief Declaration of the method response variants.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_METHOD_RESPONSE_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_METHOD_RESPONSE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/variant.h"
#include "amsr/someip_binding_core/internal/methods/application_error_response.h"
#include "amsr/someip_binding_core/internal/methods/error_response.h"
#include "amsr/someip_binding_core/internal/methods/positive_response.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {
namespace methods {

/*!
 * \brief Type alias for variant of the method response(PositiveResponse, ApplicationErrorResponse).
 */
using MethodResponse = ::amsr::core::Variant<PositiveResponse, ApplicationErrorResponse>;

/*!
 * \brief Type alias for variant of the method response(PositiveResponse, ApplicationErrorResponse, ErrorResponse).
 */
using MethodResponseVariant = ::amsr::core::Variant<PositiveResponse, ApplicationErrorResponse, ErrorResponse>;

}  // namespace methods
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_METHOD_RESPONSE_H_
