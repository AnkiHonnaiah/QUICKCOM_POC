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
/**        \file  generic_proxy_method_backend_interface.h
 *        \brief  Interface definition for the generic proxy method backend.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_GENERIC_PROXY_METHOD_BACKEND_INTERFACE_H_
#define LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_GENERIC_PROXY_METHOD_BACKEND_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "amsr/core/future.h"
#include "amsr/generic/activatable_source_code.h"
#include "amsr/socal/generic_data_ptr.h"
#include "amsr/socal/generic_data_type.h"

namespace amsr {
namespace socal {
namespace internal {
namespace methods {

/*!
 * \brief Interface definition for the generic proxy method backend.
 */
class AMSR_LG_BETA GenericProxyMethodBackendInterface {
 public:
  /*!
   * \brief   Creates a backend for the generic proxy method.
   * \details For each method a distinct method backend shall be created.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  GenericProxyMethodBackendInterface() noexcept = default;

  GenericProxyMethodBackendInterface(GenericProxyMethodBackendInterface const&) = delete;
  GenericProxyMethodBackendInterface(GenericProxyMethodBackendInterface&&) = delete;
  GenericProxyMethodBackendInterface& operator=(GenericProxyMethodBackendInterface const&) = delete;
  GenericProxyMethodBackendInterface& operator=(GenericProxyMethodBackendInterface&&) = delete;

  /*!
   * \brief Carry out the method request.
   * \param[in] args  Serialized method arguments (buffer can be empty in case of no input arguments).
   * \return A future object that can be used to access the result of the method call.
   * \error ComErrc::kServiceNotAvailable    If the remote server is currently not available.
   * \error ComErrc::kNetworkBindingFailure  If the network binding reported a recoverable communication error.
   *                                         e.g. deserialization error or other Binding internal errors.
   * \error <ApplicationError>               The configured application error, if any.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady TRUE
   * \spec requires true; \endspec
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_PureVirtualFunctionCanBeConst_FalsePositive
  virtual ::amsr::core::Future<::amsr::socal::GenericDataPtr> HandleRequest(
      ::amsr::socal::GenericDataType const& args) noexcept = 0;

 protected:
  /*!
   * \brief Destroys the generic proxy method backend.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  ~GenericProxyMethodBackendInterface() noexcept = default;
};

}  // namespace methods
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_GENERIC_PROXY_METHOD_BACKEND_INTERFACE_H_
