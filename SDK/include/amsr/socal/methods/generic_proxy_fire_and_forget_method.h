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
/**        \file  generic_proxy_fire_and_forget_method.h
 *        \brief  Generic proxy fire-and-forget method implementation.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_METHODS_GENERIC_PROXY_FIRE_AND_FORGET_METHOD_H_
#define LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_METHODS_GENERIC_PROXY_FIRE_AND_FORGET_METHOD_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>
#include "amsr/socal/generic_data_type.h"
#include "amsr/socal/internal/logging/ara_com_logger.h"
#include "amsr/socal/internal/methods/generic_proxy_fire_and_forget_method_backend_interface.h"
#include "ara/com/instance_identifier.h"
#include "ara/core/result.h"

namespace amsr {
namespace socal {
namespace methods {

/*!
 * \brief Generic proxy fire-and-forget method implementation.
 */
class AMSR_LG_BETA GenericProxyFireAndForgetMethod final {
  /*!
   * \brief Type alias for generic proxy fire-and-forget method backend.
   */
  using GenericProxyFireAndForgetMethodBackend =
      ::amsr::socal::internal::methods::GenericProxyFireAndForgetMethodBackendInterface;

 public:
  /*!
   * \brief Constructs a generic proxy fire-and-forget method.
   * \param[in] method_backend  The corresponding fire-and-forget method backend.
   *                            The given reference must be valid at least until the lifetime of this object.
   * \param[in] service_name    The shortname path of the service interface this fire-and-forget method belongs to.
   * \param[in] method_name     The fire-and-forget method name configured in the model.
   * \param[in] instance_id     The instance identifier of the corresponding proxy instance.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private.
   * \steady FALSE
   * \spec requires true; \endspec
   */
  GenericProxyFireAndForgetMethod(GenericProxyFireAndForgetMethodBackend& method_backend,
                                  std::string const& service_name, std::string const& method_name,
                                  ::ara::com::InstanceIdentifier const& instance_id) noexcept;

  /*!
   * \brief Destroys the generic proxy fire-and-forget method.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private.
   * \steady FALSE
   */
  ~GenericProxyFireAndForgetMethod() noexcept = default;

  GenericProxyFireAndForgetMethod(GenericProxyFireAndForgetMethod const&) = delete;
  GenericProxyFireAndForgetMethod(GenericProxyFireAndForgetMethod&&) = delete;
  GenericProxyFireAndForgetMethod& operator=(GenericProxyFireAndForgetMethod const&) = delete;
  GenericProxyFireAndForgetMethod& operator=(GenericProxyFireAndForgetMethod&&) = delete;

  /*!
   * \brief Carry out the fire-and-forget method request.
   * \param[in] args  Serialized method arguments, if any.
   * \return Result with no value if the request was successful or an error.
   * \error ComErrc::kServiceNotAvailable    If the remote server is currently not available.
   * \error ComErrc::kNetworkBindingFailure  If the network binding reported a recoverable communication error.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  ::ara::core::Result<void> operator()(::amsr::socal::GenericDataType const& args = {}) noexcept;

 private:
  /*!
   * \brief The fire-and-forget method backend.
   */
  GenericProxyFireAndForgetMethodBackend& method_backend_;

  /*!
   * \brief An instance of the logger.
   */
  ::amsr::socal::internal::logging::AraComLogger const logger_;
};

}  // namespace methods
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_METHODS_GENERIC_PROXY_FIRE_AND_FORGET_METHOD_H_
