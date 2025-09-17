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
/*!        \file  skeleton_field.h
 *        \brief  Generic template class for skeleton fields.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_FIELDS_SKELETON_FIELD_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_FIELDS_SKELETON_FIELD_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <mutex>
#include <string>
#include <type_traits>
#include <utility>
#include "amsr/core/future.h"
#include "amsr/core/promise.h"
#include "amsr/socal/internal/fields/skeleton_field_config.h"
#include "amsr/socal/internal/logging/ara_com_logger.h"
#include "amsr/socal/internal/utility.h"
#include "ara/com/com_error_domain.h"
#include "ara/com/sample_allocatee_ptr.h"
#include "ara/core/memory_resource.h"
#include "ara/core/result.h"

namespace amsr {
namespace socal {
namespace fields {

/*!
 * \brief   Generic template class for skeleton fields, that unifies common behavior.
 * \details The following APIs are not considered to be threadsafe against each other, or against any other API in this
 *          instance:
 *          - RegisterGetHandler(),
 *          - RegisterSetHandler() Further, the APIs of this instance are not threadsafe against.
 *          - Skeleton::OfferService(),
 *          - Skeleton::StopOfferService() of the Skeleton instance that contains this instance as a member.
 * \tparam FieldDataType  Represents the data type of the field.
 * \tparam FieldConfig    Is a model of the configuration from the arxml with info about hasSetter, hasGetter,
 *                        hasNotifier, the corresponding skeleton and the field name.
 *
 * \unit Socal::Skeleton::SkeletonField
 */
template <typename FieldDataType, typename FieldConfig>
// VCA_SOCAL_COMPILER_GENERATED_DESTRUCTOR
class SkeletonField final {
 public:
  /*!
   * \brief Type-alias for the data type of this field.
   * \vpublic
   */
  using FieldType = FieldDataType;

  /*!
   * \brief Type alias for the skeleton of this field.
   */
  using Skeleton = typename FieldConfig::Skeleton;

  /*!
   * \brief Take the interface from the configuration for this field.
   */
  using SkeletonPtr = typename std::add_pointer<Skeleton>::type;

  /*!
   * \brief Type alias for the return type of Allocate()
   */
  using SampleAllocateePtrResultType = typename ::ara::core::Result<::ara::com::SampleAllocateePtr<FieldDataType>>;

  /*!
   * \brief Type alias for the field name (Used for logging).
   */
  static constexpr vac::container::CStringView kFieldName{FieldConfig::Name};

  /*!
   * \brief Creates a SkeletonField.
   * \param[in] skeleton_ptr  A pointer to the skeleton object. The given pointer must not be null and must be valid
   *                          until the current object's lifetime.
   * \pre        -
   * \context    App
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \vprivate
   * \steady FALSE
   * \spec requires skeleton_ptr != nullptr; \endspec
   */
  // VECTOR NC VectorC++-V5.0.1: MD_SOCAL_VectorC++-V5.0.1_unsequenced_call_false_positive
  explicit SkeletonField(SkeletonPtr skeleton_ptr) noexcept
      : logger_{::amsr::socal::internal::logging::kAraComLoggerContextId,
                ::amsr::socal::internal::logging::kAraComLoggerContextDescription, GetLoggerPrefix()},
        skeleton_{::amsr::socal::internal::Utility::GetPointerWithNullCheck(skeleton_ptr)},
        // VCA_SOCAL_CALLING_CONSTRUCTORS_OF_CLASS_MOCKS
        notification_event_{::amsr::socal::internal::Utility::GetPointerWithNullCheck(skeleton_ptr)} {}

  /*!
   * \brief Allocate field notifier data of unique ownership for sending out.
   * \return Result with the requested memory provided by the middleware or an error.
   * \error ::ara::com::ComErrc::kAllocateUnsupportedWithSetup  If multiple provided service instances are registered to
   *                                                            this SkeletonField.
   * \error ::ara::com::ComErrc::kSampleAllocationFailure       If memory allocation failed in the backend.
   * \error ::ara::com::ComErrc::kServiceNotAvailable           If service is not offered.
   * \pre         Service must be offered before calling this API.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   *              Not threadsafe against the following APIs of the associated SkeletonField instance:
   *              - SkeletonField::Send
   *              - SkeletonField::Update
   *              Not threadsafe against following APIs of the associated Skeleton instance:
   *              - Skeleton::OfferService
   *              - Skeleton::StopOfferService
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \internal
   * - Dispatch the Allocate call to the notification event.
   * \endinternal
   */
  template <typename Config = FieldConfig>
  auto Allocate() const noexcept ->
      typename std::enable_if<Config::HasNotification, SampleAllocateePtrResultType>::type {
    return notification_event_.Allocate();
  }

  /*!
   * \brief   Update the current field value managed by ara::com.
   * \details This field value is provided to requestors in case no custom GetHandler is registered (see
   *          RegisterGetHandler()). The effective field value might be changed by remote proxy Set() requests and the
   *          registered SetHandler (see RegisterSetHandler()). In case a get handler is configured for the field,
   *          Update() has to be called at least once to set the initial field value.
   * \param[in] data  Reference to the new field value.
   * \return Result with no value if the request is successful or an error.
   * \error ComErrc::kCommunicationLinkError   If there was a connection failure.
   * \error ComErrc::kCommunicationStackError  If communication stack fails.
   * \error ComErrc::kSampleAllocationFailure  If no sample-pointer allocation could take place.
   * \pre         -
   * \context     App | Callback.
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   *              Not threadsafe against following APIs of the associated Skeleton instance:
   *              - Skeleton::OfferService
   *              - Skeleton::StopOfferService
   * \reentrant   FALSE for same class instance, FALSE for different instances if there is a notifier configured, TRUE
   *              for different instances if there is no notifier configured.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \internal
   * - Protect against parallel calls to this API and modification of field data.
   *   - Invoke NotifyAndUpdate API.
   *
   * Calls "std::terminate()" if:
   * - Mutex locking fails on "field_data_lock_".
   * \endinternal
   *
   * \trace SPEC-4980363, SPEC-8053497, SPEC-4981462, SPEC-4981463
   */
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOCAL_AutosarC++17_10-A15.5.3_STL_exceptions
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOCAL_AutosarC++17_10-A15.4.2_STL_exceptions
  ::ara::core::Result<void> Update(FieldType const& data) noexcept {
    // PTP-B-Socal-SkeletonField_Update
    logger_.LogDebug(static_cast<char const*>(__func__), __LINE__);

    // ESCAN00113865: We must protect both sending out the notifications and updating the internal value.
    std::lock_guard<std::mutex> const guard{field_data_lock_};
    ::ara::core::Result<void> const result{NotifyAndUpdate(data)};

    return result;
    // PTP-E-Socal-SkeletonField_Update
  }

  /*!
   * \brief   Update the current field value managed by ara::com.
   * \details This field value is provided to requestors in case no custom GetHandler is registered (see
   *          RegisterGetHandler()). The effective field value might be changed by remote proxy Set() requests and the
   *          registered SetHandler (see RegisterSetHandler()). In case a get handler is configured for the field,
   *          Update() has to be called at least once to set the initial field value.
   * \param[in] data  A pointer to the new field value.
   * \return Result with no value if the request is successful or an error.
   * \error ComErrc::kServiceNotOffered        If the service is not offered when this API is called.
   * \error ComErrc::kCommunicationLinkError   If there was a connection failure.
   * \error ComErrc::kCommunicationStackError  If communication stack fails.
   * \error ComErrc::kSampleAllocationFailure  If no sample-pointer allocation could take place.
   * \pre         Service must be offered.
   * \context     App | Callback.
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   *              Not threadsafe against following APIs of the associated Skeleton instance:
   *              - Skeleton::OfferService
   *              - Skeleton::StopOfferService
   * \reentrant   FALSE for same class instance, FALSE for different instances if there is a notifier configured, TRUE
   *              for different instances if there is no notifier configured.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires data != nullptr; \endspec
   * \internal
   * - Calls NotifyAndUpdate().
   *
   * Calls "std::terminate()" if:
   * - The pointer argument is null.
   * - Mutex locking fails on "field_data_lock_".
   * \endinternal
   *
   * \trace SPEC-4980363, SPEC-8053497, SPEC-4981462, SPEC-4981463
   */
  template <typename Config = FieldConfig>
  auto Update(::ara::com::SampleAllocateePtr<FieldType> data) noexcept ->
      typename std::enable_if<Config::HasNotification, ::ara::core::Result<void>>::type {
    // ESCAN00113865: We must protect both sending out the notifications and updating the internal value.
    std::lock_guard<std::mutex> const guard{field_data_lock_};
    ::ara::core::Result<void> const result{NotifyAndUpdate(std::move(data))};

    return result;
  }

  /*!
   * \brief Type-alias for the get handler callback as defined in the ara::com SWS.
   * \note  GetHandler is fully optional.
   *
   * \trace SPEC-4980359
   */
  using GetHandlerInterface = std::function<::ara::core::Future<FieldType>()>;

  /*!
   * \brief   Register an optional handler called if any remote proxy requests the current field value.
   * \details The registered handler must provide the current field value using asynchronous future/promise approach. In
   *          case no custom handler is registered, ara::com will internally handle the request and provide the last
   *          field value set by last Update() API call. This implicitly requires at least one call to Update() before a
   *          service instance gets offered.
   * \tparam    Config       Configuration to find if a getter is configured for the current field.
   * \param[in] get_handler  The handler function which is called on a get from the client. Any exception thrown by the
   *                         callback will lead to a termination through "std::terminate()". The given handler must not
   *                         be nullptr and must be valid as long as the service is offered.
   * \return function returns void.
   * \pre         The service is not currently being offered.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires get_handler != nullptr; \endspec
   * \internal
   * - Store the given get handler.
   * - Reset field_data_ as get handler is registered.
   *
   * Calls abort if:
   * - The get handler is invalid.
   * - The service is currently offered.
   * \endinternal
   *
   * \trace SPEC-4980359, SPEC-4980360
   */
  template <typename Config = FieldConfig>
  auto RegisterGetHandler(GetHandlerInterface get_handler) noexcept ->
      typename std::enable_if<Config::HasGetter, void>::type {
    logger_.LogInfo(static_cast<char const*>(__func__), __LINE__);

    // Allow modification of get handler only if service is currently not offered.
    if (!skeleton_->IsServiceOffered()) {
      if (get_handler != nullptr) {
        // VCA_SOCAL_CALLING_STL_APIS, VCA_SOCAL_PASSING_ARGUMENT_AFTER_CHECK
        get_handler_ = get_handler;
        field_data_.reset();
      } else {
        // Do not allow setting the handler to nullptr, otherwise we will have problems if we get a Set request.
        ::ara::core::Abort(
            "SkeletonField::RegisterGetHandler: Registration of the invalid GetHandler 'nullptr' not possible.");
      }
    } else {
      logger_.LogFatalAndAbort(
          [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            // VCA_SOCAL_CALLING_STL_APIS
            s << "Field GetHandler modified while service is offered. The handler can "
                 "only be modified if the service is currently not offered.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief Get the value of the field either via the registered GetHandler or returns the stored value in case no
   *        GetHandler is registered.
   * \tparam Config  Configuration to find if a getter is configured for the current field.
   * \return A future object that contains the field value.
   * \pre         Service is currently offered.
   * \context     App | Callback.
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous FALSE
   * \vprivate
   * \steady TRUE
   * \internal
   * - If the get handler is valid.
   *   - Call get handler.
   *   - Abort if the get handler returns an error.
   * - Otherwise, get the previously updated field value.
   * - Return the obtained result.
   * \endinternal
   *
   * \trace SPEC-8053495
   */
  template <typename Config = FieldConfig>
  auto Get() const noexcept -> typename std::enable_if<Config::HasGetter, ::amsr::core::Future<FieldType>>::type {
    ::amsr::core::Future<FieldType> result{};
    // PTP-B-Socal-SkeletonField_Get
    if (get_handler_ != nullptr) {
      // VCA_SOCAL_CALLING_STL_APIS
      ::amsr::core::Future<FieldType> const get_handler_future{get_handler_()};
      ::amsr::core::Result<FieldType> get_handler_result{get_handler_future.GetResult()};
      if (!get_handler_result.HasValue()) {
        logger_.LogFatalAndAbort(
            [&get_handler_result](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
              // VCA_SOCAL_VALID_REFERENCE_CAPTURED_IN_LAMBDA_EXECUTED_SYNCHRONOUSLY
              ::amsr::core::StringView const error_message{get_handler_result.Error().Message()};
              // VCA_SOCAL_CALLING_STL_APIS
              s << "Unexpected error while getting value of future: " << error_message;
            },
            static_cast<char const*>(__func__), __LINE__);
      }
      ::amsr::core::Promise<FieldType> promise{::ara::core::GetDefaultResource()};
      promise.set_value(std::move(get_handler_result).Value());
      result = promise.get_future();
    } else {
      result = GetValue();
    }
    // PTP-E-Socal-SkeletonField_Get
    return result;
  }

  /*!
   * \brief   Set the value of the field via set_handler_ interface.
   * \details This internal API must only be called by the concrete binding to set field value. The user application
   *          must not modify the current field value with this API.
   * \tparam    Config  Configuration to find if a setter is configured for the current field.
   * \param[in] data    The value to set for this field.
   * \return A future object containing the mirrored value.
   * \pre         Service is currently offered.
   * \context     App | Callback.
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous FALSE
   * \vprivate
   * \steady TRUE
   * \internal
   * - Call the registered SetHandler.
   * - Retrieve the effective field value (this call is blocked until the user sets the value for the returned future).
   * - Return a promise that is initialized with the effective field value retrieved from the SetHandler.
   *
   * Calls abort if:
   * - Getting the value from the future object obtained from the set handler has failed.
   * \endinternal
   *
   * \trace SPEC-8053496
   */
  template <typename Config = FieldConfig>
  auto Set(FieldType const& data) noexcept ->
      typename std::enable_if<Config::HasSetter, ::amsr::core::Future<FieldType>>::type {
    // PTP-B-Socal-SkeletonField_Set
    ::amsr::core::Promise<FieldType> promise{::ara::core::GetDefaultResource()};
    logger_.LogDebug(static_cast<char const*>(__func__), __LINE__);

    // The registered handler can only be modified if service is not offered.
    // Therefore, no multi-threading protection required here.
    // VCA_SOCAL_CALLING_STL_APIS
    ::amsr::core::Future<FieldType> const set_handler_future{set_handler_(data)};
    // Synchronous wait for set handler to provide the effective field value.
    ::amsr::core::Result<FieldType> set_handler_get_result{set_handler_future.GetResult()};
    if (set_handler_get_result.HasValue()) {
      FieldType const& effective_field_value{set_handler_get_result.Value()};
      // VECTOR NL AutosarC++17_10-A1.1.1, Compiler-#1444: MD_Socal_AutosarC++17_10-A1.1.1_AcknowledgedDeprecationWarning
      static_cast<void>(Update(effective_field_value));
      promise.set_value(std::move(set_handler_get_result).Value());
    } else {
      ::amsr::core::Abort("SkeletonField::Set: Unexpected error while getting value of future");
    }
    // PTP-E-Socal-SkeletonField_Set
    return promise.get_future();
  }

  /*!
   * \brief Type-alias for the set handler callback.
   *
   * \trace SPEC-4980361
   */
  using SetHandlerInterface = std::function<::ara::core::Future<FieldType>(FieldType const& data)>;

  /*!
   * \brief   Register a handler called if any remote proxy tries to set the current field value.
   * \details The registered handler must provide the effective field value using asynchronous future/promise approach.
   *          The SetHandler is mandatory for fields with HAS-SETTER=true configuration.
   * \tparam    Config       Configuration to find if a setter is configured for the current field.
   * \param[in] set_handler  The handler which is called on a set call from the client. Any exception thrown by the
   *                         callback will lead to a termination through "std::terminate()". The given handler must not
   *                         be nullptr and must be valid as long as the service is offered.
   * \return function returns void.
   * \pre         The service is not currently being offered.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires set_handler != nullptr; \endspec
   * \internal
   * - If no service has been offered and the passed SetHandler is valid, register the SetHandler.
   *
   * Calls abort if:
   * - The set handler is invalid.
   * - The service is currently offered.
   * \endinternal
   *
   * \trace SPEC-4980362, SPEC-4980361
   */
  template <typename Config = FieldConfig>
  auto RegisterSetHandler(SetHandlerInterface set_handler) noexcept ->
      typename std::enable_if<Config::HasSetter, void>::type {
    logger_.LogInfo(static_cast<char const*>(__func__), __LINE__);

    // Allow modification of set handler only if service is currently not offered.
    if (!skeleton_->IsServiceOffered()) {
      if (set_handler != nullptr) {
        // VCA_SOCAL_EXTERNAL_FUNCTION_CONTRACT, VCA_SOCAL_STD_FUNCTION_VALIDITY_CHECK_BEFORE_ASSIGNMENT
        set_handler_ = set_handler;
      } else {
        // Do not allow setting the handler to nullptr, otherwise we will have problems if we get a Set request.
        ::ara::core::Abort(
            "SkeletonField::RegisterSetHandler: Registration of the invalid SetHandler 'nullptr' not possible.");
      }
    } else {
      logger_.LogFatalAndAbort(
          [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            // VCA_SOCAL_CALLING_STL_APIS
            s << "Field SetHandler modified while service is offered. The handler can "
                 "only be modified if the service is currently not offered.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief Checks if Update() has ever been called on this field.
   * \return true if Update() is called at least once for this field, false if it has never been called.
   * \pre       -
   * \context   App
   * \reentrant FALSE
   * \vprivate
   * \steady FALSE
   */
  bool IsUpdated() const noexcept { return field_update_once_.load(); }

  /*!
   * \brief   Sends the current field value (managed by ara::com) to field subscribers, Same as Update().
   * \details This field value is provided to requestors in case no custom GetHandler is registered (see
   *          RegisterGetHandler()). The effective field value might be changed by remote proxy Set() requests and the
   *          registered SetHandler (see RegisterSetHandler()). In case a get handler is configured for the field,
   *          Update() has to be called at least once to set the initial field value.
   * \param[in] data  Reference to the new field value.
   * \return Result with no value if the request is successful or an error.
   * \error ComErrc::kCommunicationLinkError   If there was a connection failure.
   * \error ComErrc::kCommunicationStackError  If communication stack fails.
   * \error ComErrc::kSampleAllocationFailure  If no sample-pointer allocation could take place.
   * \pre         -
   * \context     App | Callback.
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   *              Not threadsafe against following APIs of the associated Skeleton instance:
   *              - Skeleton::OfferService
   *              - Skeleton::StopOfferService
   * \reentrant   FALSE for same class instance, FALSE for different instances if there is a notifier configured, TRUE
   *              for different instances if there is no notifier configured.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \internal
   * - Protect against parallel calls to this API and modification of field data.
   *   - Invoke NotifyAndUpdate API.
   *
   * Calls "std::terminate()" if:
   * - Mutex locking fails on "field_data_lock_".
   * \endinternal
   *
   * \trace SPEC-4980363, SPEC-8053497, SPEC-4981462, SPEC-4981463
   */
  ara::core::Result<void> Send(FieldDataType const& data) noexcept { return Update(data); }

  /*!
   * \brief   Sends the current field value (managed by ara::com) to field subscribers, Same as Update().
   * \details This field value is provided to requestors in case no custom GetHandler is registered (see
   *          RegisterGetHandler()). The effective field value might be changed by remote proxy Set() requests and the
   *          registered SetHandler (see RegisterSetHandler()). In case a get handler is configured for the field,
   *          Update() has to be called at least once to set the initial field value.
   * \param[in] data  A pointer to the new field value.
   * \return Result with no value if the request is successful or an error.
   * \error ComErrc::kServiceNotOffered        If the service is not offered when this API is called.
   * \error ComErrc::kCommunicationLinkError   If there was a connection failure.
   * \error ComErrc::kCommunicationStackError  If communication stack fails.
   * \error ComErrc::kSampleAllocationFailure  If no sample-pointer allocation could take place.
   * \pre         Service must be offered.
   * \context     App | Callback.
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   *              Not threadsafe against following APIs of the associated Skeleton instance:
   *              - Skeleton::OfferService
   *              - Skeleton::StopOfferService
   * \reentrant   FALSE for same class instance, FALSE for different instances if there is a notifier configured, TRUE
   *              for different instances if there is no notifier configured.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires data != nullptr; \endspec
   * \internal
   * - Calls NotifyAndUpdate().
   *
   * Calls "std::terminate()" if:
   * - The pointer argument is null.
   * - Mutex locking fails on "field_data_lock_".
   * \endinternal
   *
   * \trace SPEC-4980363, SPEC-8053497, SPEC-4981462, SPEC-4981463
   */
  template <typename Config = FieldConfig>
  auto Send(::ara::com::SampleAllocateePtr<FieldType> data) noexcept ->
      typename std::enable_if<Config::HasNotification, ::ara::core::Result<void>>::type {
    // VCA_SOCAL_VALID_MOVE_CONSTRUCTION
    return Update(std::move(data));
  }

  /*!
   * \brief Check if field was initialized properly.
   * \return true if field was initialized properly, false otherwise.
   * \pre      -
   * \context  App
   * \vprivate
   * \steady FALSE
   * \internal
   * - Check if all fields are initialized correctly.
   *   - if a field has Setter, SetHandler must be registered.
   *   - if a field has Notification, Update must have been called.
   *   - if a field has Getter and no GetHandler is registered, Update() must have been called.
   * \endinternal
   */
  bool IsFieldValid() const noexcept {
    std::uint8_t result{1U};

    // To avoid coverage problems we had to use bitwise and-operator instead of logical and-operator.
    result = result & CheckSetter();
    result = result & CheckNotifier();
    result = result & CheckGetter();

    return (result != 0U);
  }

 private:
  /*!
   * \brief Obtain the actual value as stored in the field by bypassing the get handler.
   * \tparam Config  Configuration to find if a getter is configured for the current field.
   * \return A future object that contains the field value.
   * \throws std::runtime_error  If no GetHandler is registered and the field value has never been updated.
   * \pre     -
   * \context Callback
   * \steady TRUE
   * \internal
   * Calls "std::terminate()" if:
   * - Mutex locking fails on "field_data_lock_"
   * \endinternal
   */
  template <typename Config = FieldConfig>
  auto GetValue() const noexcept -> typename std::enable_if<Config::HasGetter, ::amsr::core::Future<FieldType>>::type {
    ::amsr::core::Promise<FieldType> promise{::ara::core::GetDefaultResource()};
    // Protect concurrent read/write to field_data_ by Get() and Update() API.
    std::lock_guard<std::mutex> const guard{field_data_lock_};
    promise.set_value(field_data_.value());

    return promise.get_future();
  }

  /*!
   * \brief Store the field value internally if getter is configured for the current field and get handler is not
   *        registered.
   * \tparam    Config  Configuration to find if a getter is configured for the current field.
   * \param[in] data    Reference to the new field value.
   * \return function returns void.
   * \pre     -
   * \context App | Callback
   * \steady TRUE
   */
  template <typename Config = FieldConfig>
  auto StoreFieldValue(FieldType const& data) noexcept -> typename std::enable_if<Config::HasGetter, void>::type {
    if (get_handler_ == nullptr) {
      field_data_.emplace(data);  // VCA_SOCAL_EXTERNAL_FUNCTION_CONTRACT
    }
  }

  /*!
   * \brief DO NOT store the field value internally if getter is not configured for the current field.
   * \tparam Config  Configuration to find if a getter is configured for the current field.
   * \return function returns void.
   * \pre         -
   * \context     App | Callback.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  // VECTOR NC AutosarC++17_10-M0.1.8: MD_SOCAL_M0.1.8_VoidFunctionHasNoExternalSideEffect
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_MethodCanBeDeclaredStatic
  template <typename Config = FieldConfig>
  auto StoreFieldValue(FieldType const&) noexcept -> typename std::enable_if<Config::HasGetter == false, void>::type {
    // function body left empty on purpose (see description).
  }

  /*!
   * \brief   Send a notification to this field's subscribers and update the new field data.
   * \details This implementation is active in case the field allows subscription and notification.
   * \tparam    Config  Configuration to find if a notifier and getter is configured for the current field.
   * \param[in] data    The new field data to be notified and updated.
   * \return Result with no value if the request is successful or an error.
   * \error ComErrc::kCommunicationLinkError   If there was a connection failure.
   * \error ComErrc::kCommunicationStackError  If communication stack fails.
   * \error ComErrc::kSampleAllocationFailure  If no sample-pointer allocation could take place.
   * \pre     -
   * \context App | Callback
   * \steady TRUE
   * \internal
   * - Synchronize against OfferService.
   * - Send field notifications to all the registered event managers.
   * - If the send request is successful
   *   - Set the field data with the given value.
   *   - Update the flag indicating field value is set at least once.
   * \endinternal
   */
  template <typename Config = FieldConfig>
  auto NotifyAndUpdate(FieldType const& data) noexcept ->
      typename std::enable_if<Config::HasNotification, ara::core::Result<void>>::type {
    // Block if OfferService is ongoing to avoid that initial field values are overwritten by new value in multi-binding
    // usecase.
    // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
    static_cast<void>(skeleton_->IsServiceOfferedSync());
    ::ara::core::Result<void> const result{notification_event_.SendInternal(data)};
    if (!result.HasValue()) {
      logger_.LogError(
          [&result](::amsr::socal::internal::logging::LogStream& s) {
            ::amsr::core::ErrorCode const err_code{result.Error()};
            s << "Updating field notifier failed with following error code: " << err_code;
          },
          static_cast<char const*>(__func__), __LINE__);
    } else {
      StoreFieldValue(data);
      field_update_once_.store(true);
    }
    return result;
  }

  /*!
   * \brief   Send a notification to this field's subscribers and update the new field data.
   * \details This implementation is active in case field notifier is configured but no field getter.
   * \tparam    Config  Configuration containing the info if a notifier is configured for the current field.
   * \param[in] data    The new field data to update to and use for notification.
   * \return Result with no value if the request is successful or an error.
   * \error ComErrc::kCommunicationLinkError   If there was a connection failure.
   * \error ComErrc::kCommunicationStackError  If communication stack fails.
   * \error ComErrc::kSampleAllocationFailure  If no sample-pointer allocation could take place.
   * \pre     -
   * \context App | Callback
   * \steady TRUE
   * \internal
   * - Send field notifications to all the registered event managers and return result.
   * \endinternal
   */
  template <typename Config = FieldConfig>
  auto NotifyAndUpdate(::ara::com::SampleAllocateePtr<FieldType> data) noexcept ->
      typename std::enable_if<static_cast<bool>(Config::HasNotification) && !static_cast<bool>(Config::HasGetter),
                              ara::core::Result<void>>::type {
    return NotifyFieldSubscribers(std::move(data));
  }

  /*!
   * \brief   Send a notification to this field's subscribers and update the new field data.
   * \details This implementation is active in case the field allows subscription and notification and a getter can be
   *          configured.
   * \tparam    Config  Configuration to find if a notifier and getter is configured for the current field.
   * \param[in] data    The new field data to update to and use for notification.
   * \return Result with no value if the request is successful or an error.
   * \error ComErrc::kCommunicationLinkError   If there was a connection failure.
   * \error ComErrc::kCommunicationStackError  If communication stack fails.
   * \error ComErrc::kSampleAllocationFailure  If no sample-pointer allocation could take place.
   * \pre     -
   * \context App | Callback
   * \steady TRUE
   * \internal
   * - If get handler is not set
   *   - Create a copy of the input data.
   *   - Send field notifications to all field subscribers and store result.
   *   - If the send request is successful
   *     - Set the field data with the copy of the given value.
   * - Otherwise, only send field notifications to all field subscribers and store result.
   * - Return sending result.
   * \endinternal
   */
  template <typename Config = FieldConfig>
  auto NotifyAndUpdate(::ara::com::SampleAllocateePtr<FieldType> data) noexcept ->
      typename std::enable_if<static_cast<bool>(Config::HasNotification) && static_cast<bool>(Config::HasGetter),
                              ara::core::Result<void>>::type {
    ::ara::core::Result<void> result{
        ::ara::core::Result<void>::FromError(::ara::com::ComErrc::kCommunicationStackError)};
    if (get_handler_ == nullptr) {
      // Send() will pass the pointer to the bindings, which in turn free it. So we need to keep a copy that we can
      // later
      // pass to StoreFieldValue.
      // VECTOR NL AutosarC++17_10-A7.1.1: MD_Socal_AutosarC++17_10-A7.1.1_Immutable_shall_be_qualified_const_false_positive
      FieldType data_copy{*data};
      result = NotifyFieldSubscribers(std::move(data));
      if (result.HasValue()) {
        StoreFieldValue(data_copy);
      }
    } else {
      // If a get_handler_ is registered, we don't need to cache the value for a get request.
      result = NotifyFieldSubscribers(std::move(data));
    }
    return result;
  }

  /*!
   * \brief   Send a notification to this field's subscribers and update the new field data.
   * \details This implementation is active in case the field allows subscription and notification.
   * \tparam    Config  Configuration to find if a notifier is configured for the current field.
   * \param[in] data    The new field data to update to and use for notification.
   * \return Result with no value if the request is successful or an error.
   * \error ComErrc::kCommunicationLinkError   If there was a connection failure.
   * \error ComErrc::kCommunicationStackError  If communication stack fails.
   * \error ComErrc::kSampleAllocationFailure  If no sample-pointer allocation could take place.
   * \pre     -
   * \context App | Callback
   * \steady TRUE
   * \internal
   * - Synchronize against OfferService.
   * - Send field notifications to all the registered event managers.
   * \endinternal
   */
  template <typename Config = FieldConfig>
  auto NotifyFieldSubscribers(::ara::com::SampleAllocateePtr<FieldType> data) noexcept ->
      typename std::enable_if<Config::HasNotification, ara::core::Result<void>>::type {
    // Block if OfferService is ongoing to avoid that initial field values are overwritten by new value in multi-binding
    // usecase.
    // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
    static_cast<void>(skeleton_->IsServiceOfferedSync());
    ::ara::core::Result<void> const result{notification_event_.Send(std::move(data))};
    if (!result.HasValue()) {
      logger_.LogError(
          [&result](::amsr::socal::internal::logging::LogStream& s) {
            ::amsr::core::ErrorCode const err_code{result.Error()};
            s << "Updating field notifier failed with following error code: " << err_code;
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    return result;
  }

  /*!
   * \brief   Update the new field data.
   * \details This implementation is active in case the field does not allow subscription and notification.
   * \tparam    Config  Configuration to find if a notifier is configured for the current field.
   * \param[in] data    The new field data to be updated.
   * \return Result with no value.
   * \pre     -
   * \context App | Callback
   * \steady TRUE
   * \internal
   * - Set the field data with the given value.
   * - Update the flag indicating field value is set at least once.
   * \endinternal
   */
  template <typename Config = FieldConfig>
  auto NotifyAndUpdate(FieldType const& data) noexcept ->
      typename std::enable_if<Config::HasNotification == false, ara::core::Result<void>>::type {
    StoreFieldValue(data);
    field_update_once_.store(true);
    return ara::core::Result<void>::FromValue();
  }

  /*!
   * \brief Generates a unique logger prefix.
   * \return a string representing the logger prefix.
   * \pre     -
   * \context App
   * \steady FALSE
   */
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOCAL_AutosarC++17_10-A15.4.2_STL_exceptions
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOCAL_AutosarC++17_10-A15.5.3_STL_exceptions
  static std::string GetLoggerPrefix() noexcept {
    // VCA_SOCAL_EXTERNAL_FUNCTION_CONTRACT
    std::string const service_identifier_as_string{Skeleton::kServiceIdentifier.ToString().data()};
    std::size_t const service_identifier_size{service_identifier_as_string.size()};
    // Add 17 for the static part "SkeletonField(" and 1 for null termination
    std::size_t const descriptor_size{18};
    std::size_t const field_name_size{kFieldName.size()};
    std::string constructed_prefix{};
    // VCA_SOCAL_UNDEFINED_EXTERNAL_LIBCPP_FUNCTION
    constructed_prefix.reserve(descriptor_size + service_identifier_size + field_name_size);
    // VCA_SOCAL_CALLING_STL_APIS
    constructed_prefix += "SkeletonField(" + service_identifier_as_string + ", " + kFieldName.ToString() + ")";
    return constructed_prefix;
  }

  /*!
   * \brief Checks if field with setter is initialized properly.
   * \return 1 if field was initialized properly, 0 otherwise.
   * \pre     -
   * \context App
   * \steady FALSE
   */
  template <typename Config = FieldConfig>
  auto CheckSetter() const noexcept -> std::enable_if_t<Config::HasSetter, std::uint8_t> {
    std::uint8_t result{1U};
    if (set_handler_ == nullptr) {
      logger_.LogError(
          [](::amsr::socal::internal::logging::LogStream& s) { s << "No SetHandler has been registered for field."; },
          static_cast<char const*>(__func__), __LINE__);
      result = 0U;
    }
    return result;
  }

  /*!
   * \brief Checks if field without setter is initialized properly.
   * \return 1
   * \pre     -
   * \context App
   * \steady FALSE
   * \internal
   * A specialization for the case where there is no setter.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_Method_can_be_declared_static
  template <typename Config = FieldConfig>
  auto CheckSetter() const noexcept -> std::enable_if_t<Config::HasSetter == false, std::uint8_t> {
    return 1U;
  }

  /*!
   * \brief Checks if field with notifier is initialized properly.
   * \return 1 if the field was initialized properly, 0 otherwise.
   * \pre     -
   * \context App
   * \steady FALSE
   */
  template <typename Config = FieldConfig>
  auto CheckNotifier() const noexcept -> std::enable_if_t<Config::HasNotification, std::uint8_t> {
    std::uint8_t result{1U};
    if (!IsUpdated()) {
      logger_.LogError(
          [](::amsr::socal::internal::logging::LogStream& s) {
            s << "No initial value has been set for field. Update(...) has never been called.";
          },
          static_cast<char const*>(__func__), __LINE__);
      result = 0U;
    }
    return result;
  }

  /*!
   * \brief Checks if field without notifier is initialized properly.
   * \return 1
   * \pre     -
   * \context App
   * \steady FALSE
   * \internal
   * A specialization for the case where there is no notifier.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_Method_can_be_declared_static
  template <typename Config = FieldConfig>
  auto CheckNotifier() const noexcept -> std::enable_if_t<Config::HasNotification == false, std::uint8_t> {
    return 1U;
  }

  /*!
   * \brief Checks if field with getter is initialized properly.
   * \return 1 if the field was initialized properly, 0 otherwise.
   * \pre     -
   * \context App
   * \steady FALSE
   */
  template <typename Config = FieldConfig>
  auto CheckGetter() const noexcept -> std::enable_if_t<Config::HasGetter, std::uint8_t> {
    std::uint8_t result{1U};

    if ((!IsUpdated()) && (get_handler_ == nullptr)) {
      logger_.LogError(
          [](::amsr::socal::internal::logging::LogStream& s) {
            s << "No initial value has been set for field (Update(...) has never been called) and no GetHandler "
                 "has been registered.";
          },
          static_cast<char const*>(__func__), __LINE__);
      result = 0U;
    }
    return result;
  }

  /*!
   * \brief Checks if field without getter is initialized properly.
   * \return 1
   * \pre     -
   * \context App
   * \steady FALSE
   * \internal
   * A specialization for the case where there is no getter.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_Method_can_be_declared_static
  template <typename Config = FieldConfig>
  auto CheckGetter() const noexcept -> std::enable_if_t<Config::HasGetter == false, std::uint8_t> {
    return 1U;
  }

  /*!
   * \brief An instance of a logger.
   */
  ::amsr::socal::internal::logging::AraComLogger logger_;

  /*!
   * \brief   Pointer to the concrete skeleton object.
   * \details The skeleton is used to fetch all binding interfaces over which an event is transmitted.
   */
  SkeletonPtr const skeleton_;

  /*!
   * \brief Callback for a getter call.
   */
  GetHandlerInterface get_handler_{nullptr};

  /*!
   * \brief Callback for a setter call.
   */
  SetHandlerInterface set_handler_{nullptr};

  /*!
   * \brief This flag is set to true, when the user called Update once.
   */
  std::atomic_bool field_update_once_{false};

  /*!
   * \brief The field on skeleton-side shall always have access to the latest value, which has been set via Update. This
   *        is necessary, in case no GetHandler is registered.
   */
  ::amsr::core::Optional<FieldType> field_data_{};

  /*!
   * \brief Mutex to protect concurrent access of field_data_ and concurrent invocations of Update API.
   */
  mutable std::mutex field_data_lock_{};

  /*!
   * Event used for notifying subscribers.
   */
  typename FieldConfig::NotificationType notification_event_;
};

/*!
 * \brief Definition of field name.
 * \tparam FieldDataType  Represents the data type of the field.
 * \tparam FieldConfig    Is a model of the configuration from the arxml with info about.
 */
template <typename FieldDataType, typename FieldConfig>
constexpr vac::container::CStringView SkeletonField<FieldDataType, FieldConfig>::kFieldName;

}  // namespace fields
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_FIELDS_SKELETON_FIELD_H_

// clang-format off
/*!
 * \exclusivearea ::amsr::socal::fields::SkeletonField::field_data_lock_
 * Used for protecting the concurrent access of the field data and concurrent invocations of Update and between GetValue APIs.
 *
 * \protects ::amsr::socal::fields::SkeletonField::field_data_ is the protected resource of a SkeletonField object.
 * \usedin ::amsr::socal::fields::SkeletonField::GetValue
 * \usedin ::amsr::socal::fields::SkeletonField::Update
 * \exclude All other methods of a SkeletonField object.
 * \length MEDIUM assigning/reading the value of a variable, and/or
 *                sending the field notifications over all the registered event managers.
 * \endexclusivearea
 */
// clang-format on
