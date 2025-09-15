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
/**        \file  generic_proxy_method.h
 *        \brief  Generic proxy method.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_METHODS_GENERIC_PROXY_METHOD_H_
#define LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_METHODS_GENERIC_PROXY_METHOD_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <string>
#include <utility>

#include "amsr/generic/activatable_source_code.h"
#include "amsr/socal/generic_data_ptr.h"
#include "amsr/socal/generic_data_type.h"
#include "amsr/socal/internal/logging/ara_com_logger.h"
#include "amsr/socal/internal/methods/generic_proxy_method_backend_interface.h"
#include "ara/com/instance_identifier.h"
#include "ara/core/future.h"

namespace amsr {
namespace socal {
namespace methods {

/*!
 * \brief Generic proxy method.
 */
class AMSR_LG_BETA GenericProxyMethod final {
  /*!
   * \brief Type alias for generic proxy method backend interface
   */
  using GenericProxyMethodBackend = ::amsr::socal::internal::methods::GenericProxyMethodBackendInterface;

  /*!
   * \brief Type alias for future executor class type.
   */
  using ExecutorInterfacePtr = std::shared_ptr<::ara::core::internal::ExecutorInterface>;

 public:
  /*!
   * \brief Creates a generic request-response method.
   * \param[in] generic_proxy_method_backend  The reference to generic proxy method backend interface. The given
   *                                          reference must be valid at least until the lifetime of this object.
   * \param[in] future_executor               Executor that triggers the callbacks registered with Future::then.
   *                                          The given pointer must not be null.
   * \param[in] service_name                  The name of the service.
   * \param[in] method_name                   The name of the generic method.
   * \param[in] instance_id                   Required instance ID.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private.
   * \steady FALSE
   * \spec requires true; \endspec
   */
  GenericProxyMethod(GenericProxyMethodBackend& generic_proxy_method_backend,
                     ExecutorInterfacePtr const& future_executor, std::string const& service_name,
                     std::string const& method_name, ::ara::com::InstanceIdentifier const& instance_id) noexcept;

  /*!
   * \brief Destroys GenericProxyMethodInstance.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private.
   * \spec requires true; \endspec
   * \steady FALSE
   */
  ~GenericProxyMethod() noexcept = default;

  GenericProxyMethod(GenericProxyMethod const&) = delete;
  GenericProxyMethod(GenericProxyMethod&&) = delete;
  GenericProxyMethod& operator=(GenericProxyMethod const&) = delete;
  GenericProxyMethod& operator=(GenericProxyMethod&&) = delete;

  /*!
   * \brief Carry out the method request.
   * \param[in] args  Serialized method arguments, if any.
   * \return A future object that can be used to access the result of the method call. In polling runtime processing
   *         mode, it is not allowed to register a Future::then() callback on the returned future object.
   * \error ComErrc::kServiceNotAvailable    If the remote server is currently not available.
   * \error ComErrc::kNetworkBindingFailure  If the network binding reported a recoverable communication error.
   *                                         e.g. deserialization error or other Binding internal errors.
   * \error <ApplicationError>               The configured application error, if any.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous FALSE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  ::ara::core::Future<::amsr::socal::GenericDataPtr> operator()(
      ::amsr::socal::GenericDataType const& args = {}) noexcept;

 private:
  /*!
   * \brief Backend interface for generic proxy method.
   */
  GenericProxyMethodBackend& generic_proxy_method_backend_;

  /*!
   * \brief The future executor used for callback execution on Future::then() requests.
   */
  ExecutorInterfacePtr const future_executor_;

  /*!
   * \brief An instance of a logger.
   */
  ::amsr::socal::internal::logging::AraComLogger const logger_;
};

}  // namespace methods
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_METHODS_GENERIC_PROXY_METHOD_H_
