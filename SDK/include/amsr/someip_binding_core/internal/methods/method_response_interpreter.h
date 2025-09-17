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
/**        \file  amsr/someip_binding_core/internal/methods/method_response_interpreter.h
 *        \brief  Interpreter for method response
 *        \unit   SomeIpBinding::SomeIpBindingCore::Client::MethodResponseInterpreter
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_METHOD_RESPONSE_INTERPRETER_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_METHOD_RESPONSE_INTERPRETER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/core/variant.h"
#include "amsr/someip_binding_core/internal/methods/application_error_response.h"
#include "amsr/someip_binding_core/internal/methods/error_response.h"
#include "amsr/someip_binding_core/internal/methods/positive_response.h"
#include "someipd_app_protocol/internal/someip_message.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {
namespace methods {

/*!
 * \brief Interpreter for method response
 *
 */
class MethodResponseInterpreter final {
 public:
  MethodResponseInterpreter() = delete;
  MethodResponseInterpreter(MethodResponseInterpreter const&) = delete;
  MethodResponseInterpreter(MethodResponseInterpreter&&) = delete;
  MethodResponseInterpreter& operator=(MethodResponseInterpreter const&) & = delete;
  MethodResponseInterpreter& operator=(MethodResponseInterpreter&&) & = delete;
  ~MethodResponseInterpreter() noexcept = delete;

  /*!
   * \brief       Response interpretation.
   *
   * \param[in]   packet   Serialized SOME/IP Method Response [SOME/IP message]
   * \return Result containing the variant of the method response or an error.
   * \error  CommunicationErrc::error_not_ok  If wrong message type or return code range not found
   *
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  static ::amsr::core::Result<::amsr::core::Variant<PositiveResponse, ApplicationErrorResponse, ErrorResponse>>
  InterpretMethodResponse(::amsr::someipd_app_protocol::internal::SomeIpMessage&& packet);
};

}  // namespace methods
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_METHOD_RESPONSE_INTERPRETER_H_
