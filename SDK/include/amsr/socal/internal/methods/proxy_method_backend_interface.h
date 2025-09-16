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
/**        \file  proxy_method_backend_interface.h
 *        \brief  The interface which any proxy method backend has to implement.
 *
 *      \details  This file describes the interface to be implemented by a method backend.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_PROXY_METHOD_BACKEND_INTERFACE_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_PROXY_METHOD_BACKEND_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "amsr/core/future.h"
namespace amsr {
namespace socal {
namespace internal {
namespace methods {

/*!
 * \brief The interface which proxy method backend has to implement.
 * \tparam Output  The method output type.
 * \tparam ArgsT   The method arguments' types.
 */

template <typename Output, typename... ArgsT>
class ProxyMethodBackendInterface {
 public:
  /*!
   * \brief   Creates a backend for the proxy method.
   * \details For each proxy method a distinct proxy method backend shall be created.
   * \pre     -
   * \context App
   * \steady FALSE
   */
  ProxyMethodBackendInterface() noexcept = default;

  ProxyMethodBackendInterface(ProxyMethodBackendInterface const&) = delete;

  ProxyMethodBackendInterface(ProxyMethodBackendInterface&&) = delete;

  ProxyMethodBackendInterface& operator=(ProxyMethodBackendInterface const&) & = delete;

  ProxyMethodBackendInterface& operator=(ProxyMethodBackendInterface&&) & = delete;

  /*!
   * \brief Carry out the method request.
   * \param[in] args  Method arguments.
   * \return A future object to hold the result of the method call.
   * \error Method-specific  Errors will be returned in the result Future.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous FALSE
   * \steady TRUE
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_PureVirtualFunctionCanBeConst_FalsePositive
  virtual ::amsr::core::Future<Output> HandleRequest(ArgsT const&... args) noexcept = 0;

 protected:
  /*!
   * \brief Destructor.
   * \pre     -
   * \context App
   * \steady FALSE
   */
  ~ProxyMethodBackendInterface() noexcept = default;
};
}  // namespace methods
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_PROXY_METHOD_BACKEND_INTERFACE_H_
