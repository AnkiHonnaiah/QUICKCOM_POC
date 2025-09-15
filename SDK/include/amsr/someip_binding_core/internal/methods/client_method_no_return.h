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
/**        \file  amsr/someip_binding_core/internal/methods/client_method_no_return.h
 *        \brief  ClientMethodNoReturn
 *        \unit   SomeIpBinding::SomeIpBindingCore::Client::ClientMethodNoReturn
 *
 *      \details This class shall be used for fire and forget method communication with the binding core.
 *               The life cycle is controller by its user.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_CLIENT_METHOD_NO_RETURN_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_CLIENT_METHOD_NO_RETURN_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/core/result.h"
#include "amsr/someip_binding_core/internal/methods/method_no_response_interface.h"
#include "amsr/someip_binding_core/internal/types.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {
namespace methods {

/*!
 * \brief   ClientMethodNoReturn class.
 * \details This class shall be used for fire and forget method communication with the binding core.
 *
 */
class ClientMethodNoReturn final {
 public:
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
  ~ClientMethodNoReturn() noexcept = default;

  /*!
   * \brief Default move constructor.
   *
   * \context     ANY
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ClientMethodNoReturn(ClientMethodNoReturn&&) noexcept = default;

  ClientMethodNoReturn() = delete;
  ClientMethodNoReturn(ClientMethodNoReturn const&) = delete;
  ClientMethodNoReturn& operator=(ClientMethodNoReturn const&) & = delete;
  ClientMethodNoReturn& operator=(ClientMethodNoReturn&&) & = delete;

  /*!
   * \brief Explicit constructor.
   *
   * \param[in] implementation  A shared pointer to the binding client method no return implementation.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \internal steady FALSE \endinternal
   */
  explicit ClientMethodNoReturn(std::shared_ptr<MethodNoResponseInterface> implementation) noexcept;

  /*!
   * \copydoc ::amsr::someip_binding_core::internal::methods::MethodNoResponseInterface::SendMethodRequest
   */
  ::amsr::core::Result<void> SendMethodRequest(MemoryBufferPtr packet) noexcept;

 private:
  /*!
   * \brief A shared pointer to the binding client method no return implementation.
   */
  std::shared_ptr<MethodNoResponseInterface> implementation_;
};

}  // namespace methods
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_CLIENT_METHOD_NO_RETURN_H_
