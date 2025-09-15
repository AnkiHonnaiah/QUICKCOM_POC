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
/**        \file  generic_proxy_field_backend_interface.h
 *        \brief  Interface definition for the generic proxy field backend.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_INTERNAL_FIELDS_GENERIC_PROXY_FIELD_BACKEND_INTERFACE_H_
#define LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_INTERNAL_FIELDS_GENERIC_PROXY_FIELD_BACKEND_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/future.h"
#include "amsr/socal/generic_data_ptr.h"
#include "amsr/socal/generic_data_type.h"
#include "amsr/socal/internal/events/generic_proxy_event_backend_interface.h"

namespace amsr {
namespace socal {
namespace internal {
namespace fields {

/*!
 * \brief Interface definition for the generic proxy field backend.
 */
class AMSR_LG_BETA GenericProxyFieldBackendInterface
    : public ::amsr::socal::internal::events::GenericProxyEventBackendInterface {
  /*!
   * \brief Type alias to the generic proxy event backend interface.
   */
  using GenericProxyEventBackendInterface = ::amsr::socal::internal::events::GenericProxyEventBackendInterface;

 public:
  /*!
   * \brief Constructs the generic proxy field backend.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  GenericProxyFieldBackendInterface() noexcept = default;

  GenericProxyFieldBackendInterface(GenericProxyFieldBackendInterface const&) = delete;
  GenericProxyFieldBackendInterface(GenericProxyFieldBackendInterface&&) = delete;
  GenericProxyFieldBackendInterface& operator=(GenericProxyFieldBackendInterface const&) & = delete;
  GenericProxyFieldBackendInterface& operator=(GenericProxyFieldBackendInterface&&) & = delete;

  /*!
   * \brief Check whether a notifier is configured.
   * \return True if a notifier is configured, false otherwise.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   */
  virtual bool HasNotifier() const noexcept = 0;

  /*!
   * \brief Check whether a getter is configured.
   * \return True if a getter is configured, false otherwise.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   */
  virtual bool HasGetter() const noexcept = 0;

  /*!
   * \brief Get the latest field value.
   * \return A future object that can be used to access the result of the Get call.
   *         The result will be updated asynchronously, either with the latest field value (in serialized format)
   *         retrieved from the remote server, or with an error.
   * \error ComErrc::kServiceNotAvailable    If the remote server is currently not available.
   * \error ComErrc::kNetworkBindingFailure  If the network binding reported a recoverable communication error.
   *                                         e.g. deserialization error or other Binding internal errors.
   * \pre         Field getter is configured.
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady TRUE
   * \spec requires true; \endspec
   */
  virtual ::amsr::core::Future<::amsr::socal::GenericDataPtr> Get() noexcept = 0;

  /*!
   * \brief Check whether a setter is configured.
   * \return True if a setter is configured, false otherwise.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   * \spec requires true; \endspec
   */
  virtual bool HasSetter() const noexcept = 0;

  /*!
   * \brief Set the field value.
   * \param[in] value  The serialized field value to be set.
   * \return A future object that can be used to access the result of the Set call.
   *         The result will be updated asynchronously, either with the latest field value (in serialized format)
   *         returned by the remote server, or with an error.
   * \error ComErrc::kServiceNotAvailable    If the remote server is currently not available.
   * \error ComErrc::kNetworkBindingFailure  If the network binding reported a recoverable communication error.
   *                                         e.g. deserialization error or other Binding internal errors.
   * \pre         Field setter is configured.
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady TRUE
   * \spec requires true; \endspec
   */
  virtual ::amsr::core::Future<::amsr::socal::GenericDataPtr> Set(
      ::amsr::socal::GenericDataType const& value) noexcept = 0;

  /*!
   * \copydoc GenericProxyEventBackendInterface::Subscribe()
   * \pre Field notifier is configured.
   */
  using GenericProxyEventBackendInterface::Subscribe;

  /*!
   * \copydoc GenericProxyEventBackendInterface::Unsubscribe()
   * \pre Field notifier is configured.
   */
  using GenericProxyEventBackendInterface::Unsubscribe;

  /*!
   * \copydoc GenericProxyEventBackendInterface::GetSubscriptionState()
   * \pre Field notifier is configured.
   */
  using GenericProxyEventBackendInterface::GetSubscriptionState;

  /*!
   * \copydoc GenericProxyEventBackendInterface::GetFreeSampleCount()
   * \pre Field notifier is configured.
   */
  using GenericProxyEventBackendInterface::GetFreeSampleCount;

  /*!
   * \copydoc GenericProxyEventBackendInterface::ReadSamples()
   * \pre Field notifier is configured.
   */
  using GenericProxyEventBackendInterface::ReadSamples;

  /*!
   * \brief Registers a receive handler to get notified on the reception of new field notifications.
   * \param[in] callable  The callback handler that needs to be invoked on the reception of new field notifications.
   *                      The given callable shall not be null and shall be valid at least until it is de-registered.
   *                      The given callable shall not be invoked prior subscription and after unsubscription.
   * \pre         Field notifier is configured.
   * \pre         Runtime Processing Mode is ThreadDriven.
   * \pre         Notifier is not subscribed i.e., Subscribe() has not been called or Unsubscribe() has been called.
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires callable != nullptr; \endspec
   */
  using GenericProxyEventBackendInterface::RegisterReceiveHandler;

  /*!
   * \brief Deregisters the receive handler.
   * \pre         Field notifier is configured.
   * \pre         Runtime Processing Mode is ThreadDriven.
   * \pre         The receive handler is registered before.
   * \pre         Notifier is not subscribed i.e., Subscribe() has not been called or Unsubscribe() has been called.
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  using GenericProxyEventBackendInterface::DeregisterReceiveHandler;

  /*!
   * \brief Registers a subscription state change handler to get notified whenever the subscription state changes.
   * \param[in] callable  The callback handler that needs to be invoked whenever the subscription state changes.
   *                      The given callable shall not be null and shall be valid at least until it is de-registered.
   *                      The given callable shall not be invoked prior subscription and after unsubscription.
   *                      The given callback shall not be invoked with "kNotSubscribed" state.
   * \pre         Field notifier is configured.
   * \pre         Runtime Processing Mode is ThreadDriven.
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires callable != nullptr; \endspec
   */
  using GenericProxyEventBackendInterface::RegisterSubscriptionStateChangeHandler;

  /*!
   * \brief Deregisters the subscription state change handler.
   * \pre         Field notifier is configured.
   * \pre         Runtime Processing Mode is ThreadDriven.
   * \pre         The subscription state change handler is registered before.
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  using GenericProxyEventBackendInterface::DeregisterSubscriptionStateChangeHandler;

 protected:
  /*!
   * \brief Destroys the generic proxy field backend.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  ~GenericProxyFieldBackendInterface() noexcept = default;
};

}  // namespace fields
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_INTERNAL_FIELDS_GENERIC_PROXY_FIELD_BACKEND_INTERFACE_H_
