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
/**        \file  generic_proxy_field_getter.h
 *        \brief  Generic proxy field getter implementation.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_FIELDS_GENERIC_PROXY_FIELD_GETTER_H_
#define LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_FIELDS_GENERIC_PROXY_FIELD_GETTER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/generic/activatable_source_code.h"
#include "amsr/socal/generic_data_ptr.h"
#include "amsr/socal/internal/fields/generic_proxy_field_backend_interface.h"
#include "amsr/socal/internal/logging/ara_com_logger.h"
#include "ara/core/future.h"

namespace amsr {
namespace socal {
namespace fields {

/*!
 * \brief Generic proxy field getter implementation.
 */
class AMSR_LG_BETA GenericProxyFieldGetter final {
  /*!
   * \brief Type alias for generic proxy field backend.
   */
  using GenericProxyFieldBackend = ::amsr::socal::internal::fields::GenericProxyFieldBackendInterface;

  /*!
   * \brief Type alias for future executor class type.
   */
  using ExecutorInterfacePtr = std::shared_ptr<::ara::core::internal::ExecutorInterface>;

 public:
  /*!
   * \brief Constructs a generic proxy field getter.
   * \param[in] field_backend    The corresponding field backend. The given reference must be valid at least until the
   *                             lifetime of this object.
   * \param[in] logger           The logger instance of generic proxy field. The given reference must be valid at least
   *                             until the lifetime of this object.
   * \param[in] future_executor  Executor that triggers the callbacks registered with Future::then.
   *                             The given pointer must not be null.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private.
   * \steady FALSE
   * \spec requires future_executor != nullptr; \endspec
   */
  GenericProxyFieldGetter(GenericProxyFieldBackend& field_backend,
                          ::amsr::socal::internal::logging::AraComLogger const& logger,
                          ExecutorInterfacePtr future_executor) noexcept;

  /*!
   * \brief Destroys the generic proxy field getter.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private.
   * \steady FALSE
   */
  ~GenericProxyFieldGetter() noexcept = default;

  GenericProxyFieldGetter(GenericProxyFieldGetter const&) = delete;
  GenericProxyFieldGetter(GenericProxyFieldGetter&&) = delete;
  GenericProxyFieldGetter& operator=(GenericProxyFieldGetter const&) = delete;
  GenericProxyFieldGetter& operator=(GenericProxyFieldGetter&&) = delete;

  /*!
   * \brief Checks if the getter is configured for this field.
   * \return True if the getter is configured, false otherwise.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   */
  explicit operator bool() const noexcept;

  /*!
   * \brief Get the latest field value.
   * \return A future object that can be used to access the result of the Get call.
   *         The result will be updated asynchronously, either with the latest field value (in serialized format)
   *         retrieved from the remote server, or with an error.
   *         In polling runtime processing mode, it is not allowed to register a Future::then() callback on the returned
   *         future object.
   * \error ComErrc::kServiceNotAvailable    If the remote server is currently not available.
   * \error ComErrc::kNetworkBindingFailure  If the network binding reported a recoverable communication error.
   * \pre         Field getter is configured.
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous FALSE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ::ara::core::Future<::amsr::socal::GenericDataPtr> operator()() noexcept;

 private:
  /*!
   * \brief The field backend.
   */
  GenericProxyFieldBackend& field_backend_;

  /*!
   * \brief An instance of the logger.
   */
  ::amsr::socal::internal::logging::AraComLogger const& logger_;

  /*!
   * \brief The future executor used for callback execution on Future::then() requests.
   */
  ExecutorInterfacePtr future_executor_;
};

}  // namespace fields
}  // namespace socal
}  // namespace amsr
#endif  // LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_FIELDS_GENERIC_PROXY_FIELD_GETTER_H_
