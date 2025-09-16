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
/**        \file  someip_binding_xf/internal/methods/types.h
 *        \brief  SOME/IP binding specific types.
 *        \unit   SomeIpBinding::SomeIpBindingXf::Communication::Methods::ProxyMethodXf
 *
 *      \details  -
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_TYPES_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <set>
#include "amsr/core/error_domain.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace methods {

/*!
 * \brief ApplicationErrorDomain specific struct for wrapping the Error Domain and Error Codes.
 */
// VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
struct ApplicationErrorDomain {
  /*!
   * \brief Error Domain.
   */
  amsr::core::ErrorDomain const& error_domain;

  /*!
   * \brief Error Codes.
   */
  std::set<amsr::core::ErrorDomain::CodeType> error_codes;
};

}  // namespace methods
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_TYPES_H_
