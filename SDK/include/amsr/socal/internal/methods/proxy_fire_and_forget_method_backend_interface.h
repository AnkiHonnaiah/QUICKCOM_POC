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
/**        \file  proxy_fire_and_forget_method_backend_interface.h
 *        \brief  The interface which any proxy fire-and-forget method backend has to implement.
 *
 *      \details  This file describes the interface to be implemented by a proxy fire-and-forget method backend.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_PROXY_FIRE_AND_FORGET_METHOD_BACKEND_INTERFACE_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_PROXY_FIRE_AND_FORGET_METHOD_BACKEND_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace amsr {
namespace socal {
namespace internal {
namespace methods {

/*!
 * \brief The interface which proxy fire-and-forget method backend has to implement.
 * \tparam ArgsT  The method arguments' types.
 */
template <typename... ArgsT>
class ProxyFireAndForgetMethodBackendInterface {
 public:
  /*!
   * \brief   Creates a backend for the proxy fire and forget method.
   * \details For each proxy fire and forget method a distinct proxy fire and forget method backend shall be created.
   * \pre     -
   * \context App
   * \steady FALSE
   */
  ProxyFireAndForgetMethodBackendInterface() noexcept = default;

  ProxyFireAndForgetMethodBackendInterface(ProxyFireAndForgetMethodBackendInterface const&) = delete;

  ProxyFireAndForgetMethodBackendInterface(ProxyFireAndForgetMethodBackendInterface&&) = delete;

  ProxyFireAndForgetMethodBackendInterface& operator=(ProxyFireAndForgetMethodBackendInterface const&) & = delete;

  ProxyFireAndForgetMethodBackendInterface& operator=(ProxyFireAndForgetMethodBackendInterface&&) & = delete;

  /*!
   * \brief Carry out the method request.
   * \param[in] args  Method arguments.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \steady TRUE
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_PureVirtualFunctionCanBeConst_FalsePositive
  virtual void HandleRequest(ArgsT const&... args) noexcept = 0;

 protected:
  /*!
   * \brief Destructor.
   * \pre     -
   * \context App
   * \steady FALSE
   */
  ~ProxyFireAndForgetMethodBackendInterface() noexcept = default;
};

}  // namespace methods
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_PROXY_FIRE_AND_FORGET_METHOD_BACKEND_INTERFACE_H_
