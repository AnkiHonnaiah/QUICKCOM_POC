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
/**        \file  amsr/someip_binding_core/internal/methods/client_method.h
 *        \brief  ClientMethod
 *        \unit   SomeIpBinding::SomeIpBindingCore::Client::ClientMethod
 *
 *      \details This class shall be used for method communication with the binding core.
 *               The life cycle is controller by its user.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_CLIENT_METHOD_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_CLIENT_METHOD_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/core/future.h"
#include "amsr/someip_binding_core/internal/methods/method_interface.h"
#include "amsr/someip_binding_core/internal/methods/method_response.h"
#include "amsr/someip_binding_core/internal/types.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {
namespace methods {

/*!
 * \brief   ClientMethod class.
 * \details This class shall be used for method communication with the binding core.
 *
 */
class ClientMethod final {
 public:
  /*!
   * \brief Type alias for variant of the method response(PositiveResponse, ApplicationErrorResponse).
   */
  using MethodResponseType = ::amsr::someip_binding_core::internal::methods::MethodResponse;

  /*!
   * \brief Future type for the response value.
   */
  using ResponseValueFuture = ::amsr::core::Future<MethodResponseType>;

  /*!
   * \brief Default destructor.
   *
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~ClientMethod() noexcept = default;

  /*!
   * \brief Default move constructor.
   *
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ClientMethod(ClientMethod&&) noexcept = default;

  ClientMethod() = delete;
  ClientMethod(ClientMethod const&) = delete;
  ClientMethod& operator=(ClientMethod const&) & = delete;
  ClientMethod& operator=(ClientMethod&&) & = delete;

  /*!
   * \brief Explicit constructor.
   *
   * \param[in] implementation  A shared pointer to the binding client method implementation.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  explicit ClientMethod(std::shared_ptr<MethodInterface> implementation) noexcept;

  /*!
   * \copydoc ::amsr::someip_binding_core::internal::methods::MethodInterface::SendMethodRequest
   */
  ResponseValueFuture SendMethodRequest(MemoryBufferPtr packet) noexcept;

 private:
  /*!
   * \brief A shared pointer to the binding client method implementation.
   */
  std::shared_ptr<MethodInterface> implementation_;
};

}  // namespace methods
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_CLIENT_METHOD_H_
