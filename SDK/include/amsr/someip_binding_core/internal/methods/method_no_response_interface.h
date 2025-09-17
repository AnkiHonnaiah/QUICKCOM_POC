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
/**        \file  someip_binding_core/internal/methods/method_no_response_interface.h
 *        \brief Interface class for defining methods API for the Proxy side.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_METHOD_NO_RESPONSE_INTERFACE_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_METHOD_NO_RESPONSE_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/core/result.h"
#include "amsr/someip_binding_core/internal/types.h"

namespace amsr {
namespace someip_binding_core {
namespace internal {
namespace methods {

/*!
 * \brief Interface class for defining methods API for the Proxy side.
 */
class MethodNoResponseInterface {
 public:
  /*!
   * \brief Default constructor.
   * \pre -
   * \context App
   * \spec
   *   requires true;
   * \endspec
   * \steady    FALSE
   */
  MethodNoResponseInterface() noexcept = default;

  /*!
   * \brief Destructor.
   * \pre -
   * \context App
   * \spec
   *   requires true;
   * \endspec
   * \steady    FALSE
   */
  virtual ~MethodNoResponseInterface() noexcept = default;

  MethodNoResponseInterface(MethodNoResponseInterface const&) = delete;
  MethodNoResponseInterface(MethodNoResponseInterface&&) = delete;
  MethodNoResponseInterface& operator=(MethodNoResponseInterface const&) & = delete;
  MethodNoResponseInterface& operator=(MethodNoResponseInterface&&) & = delete;

  /*!
   * \brief Generates SOME/IP header for the method request and sends the complete SOME/IP message to the server.
   *
   * \param[in] packet The packet to be sent (containing the memory for the serialized header + the serialized
   *                    payload).
   * \return      void if the message was sent successfully, an error otherwise.
   *
   * \pre         Packet must contains sufficient size(atleast header size).
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual amsr::core::Result<void> SendMethodRequest(MemoryBufferPtr packet) noexcept = 0;
};

}  // namespace methods
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_METHODS_METHOD_NO_RESPONSE_INTERFACE_H_
