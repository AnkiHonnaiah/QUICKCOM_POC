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
/**        \file  generic_proxy_field_notifier.h
 *        \brief  Generic proxy field notifier implementation.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_FIELDS_GENERIC_PROXY_FIELD_NOTIFIER_H_
#define LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_FIELDS_GENERIC_PROXY_FIELD_NOTIFIER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <cstddef>
#include <limits>
#include <string>
#include <utility>

#include "amsr/core/optional.h"
#include "amsr/generic/activatable_source_code.h"
#include "amsr/socal/events/generic_proxy_event.h"
#include "amsr/socal/generic_sample_ptr.h"
#include "amsr/socal/internal/fields/generic_proxy_field_backend_interface.h"
#include "amsr/socal/internal/logging/ara_com_logger.h"
#include "ara/com/types_common.h"
#include "ara/core/result.h"

namespace amsr {
namespace socal {
namespace fields {

/*!
 * \brief Generic proxy field notifier implementation.
 */
class AMSR_LG_BETA GenericProxyFieldNotifier final {
  /*!
   * \brief Type alias for generic proxy field backend.
   */
  using GenericProxyFieldBackend = amsr::socal::internal::fields::GenericProxyFieldBackendInterface;

  /*!
   * \brief Type alias for subscription return type.
   */
  using SubscriptionResult = ::ara::core::Result<void>;

  /*!
   * \brief Type alias for GetNewSamples return type.
   */
  using GetNewSamplesResult = ::ara::core::Result<std::size_t>;

  /*!
   * \brief Type alias for internal LogStream.
   */
  using LogStream = ::amsr::socal::internal::logging::LogStream;

 public:
  /*!
   * \brief Constructs a generic proxy field notifier.
   * \param[in] field_backend  The corresponding field backend. The given reference must be valid at least until the
   *                           lifetime of this object.
   * \param[in] service_name   The shortname path of the service interface this field belongs to.
   * \param[in] field_name     The field name configured in the model.
   * \param[in] instance_id    The instance identifier of the corresponding proxy instance.
   * \param[in] logger         The logger instance of generic proxy field. The given reference must be valid at least
   *                           until the lifetime of this object.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private.
   * \steady FALSE
   * \spec requires true; \endspec
   */
  GenericProxyFieldNotifier(GenericProxyFieldBackend& field_backend, std::string const& service_name,
                            std::string const& field_name, ::ara::com::InstanceIdentifier const& instance_id,
                            ::amsr::socal::internal::logging::AraComLogger const& logger) noexcept;

  /*!
   * \brief Destroys the generic proxy field notifier.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component Private.
   * \steady FALSE
   */
  ~GenericProxyFieldNotifier() noexcept = default;

  GenericProxyFieldNotifier(GenericProxyFieldNotifier const&) = delete;
  GenericProxyFieldNotifier(GenericProxyFieldNotifier&&) = delete;
  GenericProxyFieldNotifier& operator=(GenericProxyFieldNotifier const&) = delete;
  GenericProxyFieldNotifier& operator=(GenericProxyFieldNotifier&&) = delete;

  /*!
   * \brief Subscribes to the skeleton counterpart of this proxy-side field notifier.
   * \param[in] max_sample_count  Size limits of this cache.
   * \return Result with no value or an error.
   * \error ara::com::ComErrc::kMaxSampleCountNotRealizable  If the given max_sample_count value is different to the
   *                                                         value given in previous subscription.
   * \pre         Field notifier is configured, i.e., bool(*this) returns true.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous FALSE Subscription state is reported asynchronously.
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   */
  SubscriptionResult Subscribe(std::size_t const max_sample_count) noexcept;

  /*!
   * \brief Unsubscribes from the skeleton counterpart of this proxy-side field notifier.
   * \pre         Field notifier is configured, i.e., bool(*this) returns true.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous FALSE Subscription state is reported asynchronously.
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   */
  void Unsubscribe() noexcept;

  /*!
   * \brief Query the number of available sample slots.
   * \return The number of free sample slots available.
   * \pre         Field notifier is configured, i.e., bool(*this) returns true.
   * \pre         Subscribe() has been called and Unsubscribe() has not been called.
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  std::size_t GetFreeSampleCount() const noexcept;

  /*!
   * \brief Reads the samples from underlying receive buffers.
   * \tparam    F            User provided callable function with the signature void(GenericSamplePtr).
   * \param[in] f            Callable to be invoked on every newly available sample.
   * \param[in] max_samples  Maximum number of samples that can be processed within this call.
   * \return Result containing the number of successfully processed events within this call or an error.
   * \error ara::com::ComErrc::kMaxSamplesReached  If application holds all the pre-allocated samples.
   * \pre         Field notifier is configured, i.e., bool(*this) returns true.
   * \pre         Subscribe() has been called and Unsubscribe() has not been called.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   * \internal
   * - Abort the application if the field notifier is not configured.
   * - Otherwise, invoke GetNewSamples on the underlying event and return the result.
   * \endinternal
   */
  template <typename F>
  auto GetNewSamples(F&& f, std::size_t const max_samples = std::numeric_limits<std::size_t>::max()) noexcept
      -> GetNewSamplesResult {
    if (!event_) {
      logger_.LogFatalAndAbort(
          [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            // VCA_SOCAL_CALLING_STL_APIS
            s << "Field notifier is not configured.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    return event_->GetNewSamples(std::forward<F>(f), max_samples);
  }

  /*!
   * \brief Query current subscription state.
   * \return current state of the subscription.
   * \pre         Field notifier is configured, i.e., bool(*this) returns true.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  ::ara::com::SubscriptionState GetSubscriptionState() const noexcept;

  /*!
   * \brief Registers a receive handler to get notified on reception of new field notifications.
   * \param[in] handler  The receive handler that needs to be invoked on the reception of new field notifications.
   *                     The given callable shall not be null and shall be valid at least until it is de-registered.
   *                     Any exception thrown by the callback will lead to a termination through "std::terminate()".
   * \return Result with no value or an error.
   * \error ComErrc::kSetHandlerFailure  Failed to register the handler.
   * \pre         Field notifier is configured, i.e., bool(*this) returns true.
   * \pre         Subscribe() has not been called or Unsubscribe() has been called.
   * \pre         The 'araComRuntimeProcessingMode' is configured as 'ThreadDriven'.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires handler != nullptr; \endspec
   */
  ::ara::core::Result<void> SetReceiveHandler(::ara::com::EventReceiveHandler handler) noexcept;

  /*!
   * \brief Deregisters the receive handler.
   * \return Result with no value or an error.
   * \error ComErrc::kUnsetHandlerFailure  Failed to unregister the handler.
   * \pre         Field notifier is configured, i.e., bool(*this) returns true.
   * \pre         Subscribe() has not been called or Unsubscribe() has been called.
   * \pre         The 'araComRuntimeProcessingMode' is configured as 'ThreadDriven'.
   * \pre         The receive handler is registered before.
   * \pre         This API should not be called if any user-defined mutex is locked in EventReceiveHandler and in the
   *              thread from where this API is called.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ::ara::core::Result<void> UnsetReceiveHandler() noexcept;

  /*!
   * \brief   Registers a subscription state change handler to get notified on subscription state changes.
   * \details Communication Management implementation will serialize calls to the registered handler. If multiple
   *          changes of the subscription state take place during the runtime of a previous call to a handler, the
   *          Communication Management aggregates all changes to one call with the last/effective state.
   * \param[in] handler  The callback handler that needs to be invoked whenever the subscription state changes.
   *                     The given callable shall not be null and shall be valid at least until it is de-registered.
   *                     Any exception thrown by the callback will lead to a termination through "std::terminate()".
   * \return Result with no value or an error.
   * \error ComErrc::kSetHandlerFailure  Failed to register the handler.
   * \pre         Field notifier is configured, i.e., bool(*this) returns true.
   * \pre         The 'araComRuntimeProcessingMode' is configured as 'ThreadDriven'.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires handler != nullptr; \endspec
   */
  ::ara::core::Result<void> SetSubscriptionStateHandler(::ara::com::SubscriptionStateChangeHandler handler) noexcept;

  /*!
   * \brief Deregisters the subscription state change handler.
   * \return Result with no value or an error.
   * \error ComErrc::kUnsetHandlerFailure  Failed to unregister the handler.
   * \pre         Field notifier is configured, i.e., bool(*this) returns true.
   * \pre         The 'araComRuntimeProcessingMode' is configured as 'ThreadDriven'.
   * \pre         The subscription state change handler is registered before.
   * \pre         This API should not be called if any user-defined mutex is locked in the
   *              SubscriptionStateChangeHandler and in the thread from where this API is called.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ::ara::core::Result<void> UnsetSubscriptionStateHandler() noexcept;

  /*!
   * \brief Check whether a notifier is configured for this field.
   * \return True if a notifier is configured, otherwise false.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  explicit operator bool() const noexcept;

 private:
  /*!
   * \brief The optional field notifier.
   */
  ::amsr::core::Optional<amsr::socal::events::GenericProxyEvent> event_;

  /*!
   * \brief An instance of the logger.
   */
  ::amsr::socal::internal::logging::AraComLogger const& logger_;
};

}  // namespace fields
}  // namespace socal
}  // namespace amsr
#endif  // LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_FIELDS_GENERIC_PROXY_FIELD_NOTIFIER_H_
