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
/*!        \file  skeleton_event.h
 *        \brief  Skeleton event
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_EVENTS_SKELETON_EVENT_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_EVENTS_SKELETON_EVENT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/socal/internal/events/skeleton_event_manager_interface.h"
#include "amsr/socal/internal/logging/ara_com_logger.h"
#include "amsr/socal/internal/utility.h"
#include "ara/com/com_error_domain.h"
#include "ara/com/sample_allocatee_ptr.h"
#include "ara/core/abort.h"
#include "ara/core/result.h"

namespace amsr {
namespace socal {
namespace events {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOCAL_AutosarC++17_10-M7.3.6_usingDirectiveInHeaderFile
using vac::container::literals::operator""_sv;  // NOLINT(build/namespaces)

/*!
 * \brief Generic SkeletonEvent class which unifies common behavior.
 * \tparam Skeleton                  Type of the service-specific skeleton frontend class.
 * \tparam EventSampleType           Type of the sample (e.g. uint32, ara::core::Vector<T>, struct X).
 * \tparam SkeletonBackendInterface  Type of the service-specific skeleton backend interface.
 * \tparam EventManagerReturnType    Concrete event manager type returned by GetEventManagerMethod.
 * \tparam GetEventManagerMethod     Method to get the event manager object/interface for handling events.
 * \pre The user of the SkeletonEvent class shall ensure that the generated interface GetEventManagerMethod returns a
 *      valid pointer to the event manager object.
 *
 * \unit Socal::Skeleton::SkeletonEvent::SkeletonEvent
 */
template <typename Skeleton, typename EventSampleType, typename SkeletonBackendInterface,
          typename EventManagerReturnType, EventManagerReturnType* (SkeletonBackendInterface::*GetEventManagerMethod)()>
class SkeletonEvent {
  /*!
   * \brief Type alias to the AllocatedSampleUniquePtr.
   */
  using AllocatedSampleUniquePtr = typename ::amsr::socal::internal::events::SkeletonEventManagerInterface<
      EventSampleType>::AllocatedSampleUniquePtr;

  /*!
   * \brief Type alias for the skeleton backend interface pointer.
   */
  using SkeletonBackendInterfaceUniquePtr = typename Skeleton::SkeletonBackendInterfaceUniquePtr;

 public:
  /*!
   * \brief Type-alias for the data type of the event.
   * \vpublic
   */
  using SampleType = EventSampleType;

  /*!
   * \brief Type alias for skeleton pointer.
   */
  using SkeletonPtr = typename std::add_pointer<Skeleton>::type;

  /*!
   * \brief Type alias for the result type of Allocate.
   */
  using AllocateResultType = ::ara::core::Result<::ara::com::SampleAllocateePtr<SampleType>>;

  /*!
   * \brief Delete default constructor.
   * \steady FALSE
   */
  SkeletonEvent() = delete;
  /*!
   * \brief Define destructor.
   * \steady FALSE
   */
  virtual ~SkeletonEvent() noexcept = default;  // VCA_SOCAL_COMPILER_GENERATED_DESTRUCTOR
  /*!
   * \brief Delete copy constructor.
   */
  SkeletonEvent(SkeletonEvent const&) = delete;
  /*!
   * \brief Delete move constructor.
   */
  SkeletonEvent(SkeletonEvent&&) = delete;
  /*!
   * \brief Delete copy assignment.
   */
  SkeletonEvent& operator=(SkeletonEvent const&) & = delete;
  /*!
   * \brief Delete move assignment.
   */
  SkeletonEvent& operator=(SkeletonEvent&&) & = delete;

  /*!
   * \brief Default constructor must set the reference to the skeleton.
   * \param[in] skeleton  A pointer to the skeleton object. The given pointer must not be null and must be valid until
   *                      the current object's lifetime.
   * \pre        -
   * \context    App
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \vprivate
   * \steady FALSE
   */
  explicit SkeletonEvent(SkeletonPtr skeleton) noexcept
      : skeleton_{::amsr::socal::internal::Utility::GetPointerWithNullCheck(skeleton)} {}

  /*!
   * \brief Sending event data over the backend registered.
   * \param[in] data  A reference to event data allocated by the service application developer.
   * \return Result with no value if the request is successful or an error.
   * \error ComErrc::kServiceNotOffered        If the service is not offered.
   * \error ComErrc::kCommunicationLinkError   If there was a connection failure.
   * \error ComErrc::kCommunicationStackError  If communication stack fails.
   * \error ComErrc::kSampleAllocationFailure  If no sample-pointer allocation could take place.
   * \pre         Service must be offered before calling this API.
   * \pre         PreAllocate has not been called.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   *              Not threadsafe against following APIs of this instance:
   *              - Allocate
   *              - TrySend Not threadsafe against following APIs of the associated Skeleton instance:
   *              - Skeleton::OfferService
   *              - Skeleton::StopOfferService
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \internal
   * - Synchronously obtain the offer service state (if OfferService is not yet finished, this call is blocked until
   *   the offer is forwarded to all the backends).
   * - If the service is currently not offered, return kServiceNotOffered error.
   * - Otherwise, call the internal implementation for sending the event data and return its result.
   *
   * Calls Abort() if:
   * - PreAllocate is called.
   * \endinternal
   *
   * \trace SPEC-4980353
   */
  ::ara::core::Result<void> Send(SampleType const& data) noexcept {
    ::ara::core::Result<void> result{::ara::com::ComErrc::kServiceNotOffered};
    // PTP-B-Socal-SkeletonEvent_Send_ToAllBindings
    if (!skeleton_->IsServiceOfferedSync()) {  // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
      logger_.LogError(
          [](::amsr::socal::internal::logging::LogStream& s) {
            s << "API called before offering the service or after stop offering.";
          },
          static_cast<char const*>(__func__), __LINE__);
    } else if (preallocate_called_) {
      logger_.LogFatalAndAbort(
          [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            // VCA_SOCAL_CALLING_STL_APIS
            s << "This API must not be called in combination with PreAllocate().";
          },
          static_cast<char const*>(__func__), __LINE__);
    } else {
      result = SendInternal(data);

      if (!result.HasValue()) {
        logger_.LogError(
            [&result](::amsr::socal::internal::logging::LogStream& s) {
              ::amsr::core::ErrorCode const err_code{result.Error()};
              s << "Send()-request failed with following error code: " << err_code;
            },
            static_cast<char const*>(__func__), __LINE__);
      }
    }
    // PTP-E-Socal-SkeletonEvent_Send_ToAllBindings
    return result;
  }

  /*!
   * \brief   Second variant of the Send method, which requires the call of method Allocate before.
   * \details If the memory to the sample to be sent is not valid, the transmission is dropped, a fatal message is
   *          logged and the process is aborted.
   * \param[in] data  A pointer of unique ownership to the data provided by the concrete backend implementation. Should
   *                  not be nullptr.
   * \return Result with no value if the request is successful or an error.
   * \error ComErrc::kServiceNotOffered        If the service is not offered.
   * \error ComErrc::kCommunicationLinkError   If there was a connection failure.
   * \error ComErrc::kCommunicationStackError  If communication stack fails.
   * \error ComErrc::kSampleAllocationFailure  If no sample-pointer allocation could take place.
   * \pre         Service must be offered before calling this API.
   * \pre         PreAllocate has not been called.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   *              Not threadsafe against following APIs of this instance:
   *              - Allocate
   *              - TrySend Not threadsafe against following APIs of the associated Skeleton instance:
   *              - Skeleton::OfferService
   *              - Skeleton::StopOfferService
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires data != nullptr; \endspec
   * \internal
   * - If the service is currently not offered, return kServiceNotOffered error.
   * - For each registered backend interface (one if any is registered)
   *   - Call Send with provided data on backend and return its result.
   *
   * Calls Abort() if:
   * - \p data is nullptr.
   * - \p PreAllocate is called.
   * - \p Multiple provided service instances are registered to this SkeletonEvent.
   * \endinternal
   *
   * \trace SPEC-5951491
   */
  ::ara::core::Result<void> Send(::ara::com::SampleAllocateePtr<SampleType> data) const noexcept {
    ::ara::core::Result<void> result{::ara::com::ComErrc::kServiceNotOffered};
    if (!skeleton_->IsServiceOffered()) {
      logger_.LogError(
          [](::amsr::socal::internal::logging::LogStream& s) {
            s << "API called before offering the service or after stop offering.";
          },
          static_cast<char const*>(__func__), __LINE__);
    } else if (!data) {
      logger_.LogFatalAndAbort(
          [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            // VCA_SOCAL_CALLING_STL_APIS
            s << "Could not send provided event sample as the given pointer to data is not valid.";
          },
          static_cast<char const*>(__func__), __LINE__);
    } else if (preallocate_called_) {
      logger_.LogFatalAndAbort(
          [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            // VCA_SOCAL_CALLING_STL_APIS
            s << "This API must not be called in combination with PreAllocate().";
          },
          static_cast<char const*>(__func__), __LINE__);
    } else {
      typename Skeleton::SkeletonBackendInterfaceUniquePtrCollection const& backend_interfaces{
          skeleton_->GetBackendInterfaces()};

      if (backend_interfaces.size() > 1) {
        logger_.LogFatalAndAbort(
            [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
              // VCA_SOCAL_CALLING_STL_APIS
              s << "This API cannot be used when multiple provided service instances are present. Memory allocation "
                   "must be handled by Application instead.";
            },
            static_cast<char const*>(__func__), __LINE__);
      }
      // Make sure that Send is only called on a backend if there is one registered.
      // VCA_SOCAL_VALID_SKELETON_BACKEND_INTERFACE_COLLECTION
      for (SkeletonBackendInterfaceUniquePtr const& interface : backend_interfaces) {
        // VCA_SOCAL_SEND_TO_BACKEND_GENERATED_FUNCTION, VCA_SOCAL_CALLING_METHODS_ON_OBJECTS_PASSED_TO_FUNCTION
        result = (*interface.*GetEventManagerMethod)()->Send(data.GetAllocatedSamplePtr());
        if (!result.HasValue()) {
          // VCA_SOCAL_CALLING_STL_APIS
          ::amsr::core::String const error_message_string{"Internal error occurred while sending event: " +
                                                          result.Error().Message().ToString()};
          logger_.LogError([&error_message_string](::ara::log::LogStream& s) { s << error_message_string; },
                           static_cast<char const*>(__func__), __LINE__);
        }
      }
    }
    return result;
  }

  /*!
   * \brief Third variant of the Send, which can report any error occurred while sending event samples to the caller.
   * \param[in] data  A reference to event data allocated by the service application developer.
   * \return Result with no value if the request was successful or an error.
   * \error ara::com::ComErrc::kServiceNotAvailable      If service is not offered.
   * \error ara::com::ComErrc::kSampleAllocationFailure  If free memory is not available.
   * \error ara::com::ComErrc::kCommunicationLinkError   If communication link is broken.
   * \error ara::com::ComErrc::kCommunicationStackError  If communication stack failure.
   * \pre         Service must be offered before calling this API.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   *              Not threadsafe against following APIs of this instance:
   *              - Send
   *              - Allocate Not threadsafe against following APIs of the associated Skeleton instance:
   *              - Skeleton::OfferService
   *              - Skeleton::StopOfferService
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \internal
   * - Synchronously obtain the offer service state (if OfferService is not yet finished, this call is blocked until
   *   the offer is forwarded to all the backends).
   * - If service is not offered, log an error message.
   * - Else, forward the send request to the skeleton event manager of all registered backends.
   * - If the request was not successful, provide the error code to the caller.
   * \endinternal
   */
  [[deprecated("TrySend() is deprecated and will be removed in the future; AMSR-31145")]] ::ara::core::Result<void>
  TrySend(SampleType const& data) noexcept {
    // PTP-B-Socal-SkeletonEvent_TrySend_ToAllBindings
    ::ara::core::Result<void> result{::ara::com::ComErrc::kServiceNotAvailable};

    if (!skeleton_->IsServiceOfferedSync()) {  // VCA_SOCAL_FUNCTION_CALL_ON_VALID_OBJECTS_ADHERING_TO_FUNCTION_CONTRACT
      logger_.LogError(
          [](::amsr::socal::internal::logging::LogStream& s) {
            s << "API called before offering the service or after stop offering.";
          },
          static_cast<char const*>(__func__), __LINE__);

    } else {
      // Call SendInternal which will return kSampleAllocationFailure on memory allocation failure.
      result = SendInternal(data);
      if (!result.HasValue()) {
        logger_.LogError(
            [&result](::amsr::socal::internal::logging::LogStream& s) {
              ::amsr::core::ErrorCode const err_code{result.Error()};
              s << "Send()-request failed with following error code: " << err_code;
            },
            static_cast<char const*>(__func__), __LINE__);
      }
    }
    // PTP-E-Socal-SkeletonEvent_TrySend_ToAllBindings
    return result;
  }

  /*!
   * \brief Allocate event data of unique ownership for sending out.
   * \return Result with the requested memory provided by the middleware or an error.
   * \error ::ara::com::ComErrc::kAllocateUnsupportedWithSetup  If multiple provided service instances are registered to
   *                                                            this SkeletonEvent.
   * \error ::ara::com::ComErrc::kSampleAllocationFailure       If memory allocation failed in the backend.
   * \error ::ara::com::ComErrc::kServiceNotAvailable           If service is not offered.
   * \error ::ara::com::ComErrc::kInvalidAPICall                If this API is called in combination with PreAllocate().
   * \pre         Service must be offered before calling this API.
   * \pre         PreAllocate has not been called.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   *              Not threadsafe against following APIs of this instance:
   *              - Send
   *              - TrySend Not threadsafe against following APIs of the associated Skeleton instance:
   *              - Skeleton::OfferService
   *              - Skeleton::StopOfferService
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \internal
   * - If service is not offered, return kServiceNotAvailable,
   * - Else if PreAllocate is already called, return kInvalidAPICall,
   * - Else if more than one backend instance is registered, return kAllocateUnsupportedWithSetup.
   * - Otherwise
   *   - Call Allocate on registered backend for this event.
   *   - If the backend return a valid sample slot.
   *     - Create SampleAllocateePtr from it and emplace it in result.
   *   - Otherwise
   *     - Emplace kSampleAllocationFailure and log error message.
   * - Return result.
   * \endinternal
   *
   * \trace SPEC-5951110
   */
  auto Allocate() const noexcept -> AllocateResultType {
    AllocateResultType result{AllocateResultType::FromError(::ara::com::ComErrc::kSampleAllocationFailure)};
    if (!skeleton_->IsServiceOffered()) {
      result.EmplaceError(::ara::com::ComErrc::kServiceNotAvailable);
      logger_.LogError(
          [](::amsr::socal::internal::logging::LogStream& s) {
            s << "API called before offering the service or after stop offering.";
          },
          static_cast<char const*>(__func__), __LINE__);
    } else if (preallocate_called_) {
      result.EmplaceError(::ara::com::ComErrc::kInvalidAPICall);
      logger_.LogError(
          [](::amsr::socal::internal::logging::LogStream& s) {
            s << "This API must not be called in combination with PreAllocate().";
          },
          static_cast<char const*>(__func__), __LINE__);
    } else {
      typename Skeleton::SkeletonBackendInterfaceUniquePtrCollection const& backend_interfaces{
          skeleton_->GetBackendInterfaces()};
      if (backend_interfaces.size() > 1) {
        result.EmplaceError(::ara::com::ComErrc::kAllocateUnsupportedWithSetup);
        logger_.LogError(
            [](::amsr::socal::internal::logging::LogStream& s) {
              s << "This API cannot be used when multiple provided service instances are present. Memory allocation "
                   "must be handled by Application instead.";
            },
            static_cast<char const*>(__func__), __LINE__);
      } else {
        // Make sure that Allocate is only called on a backend if there is one registered.
        // VCA_SOCAL_VALID_SKELETON_BACKEND_INTERFACE_COLLECTION
        for (SkeletonBackendInterfaceUniquePtr const& interface : backend_interfaces) {
          ::ara::core::Result<AllocatedSampleUniquePtr> backend_result{
              // VCA_SOCAL_SEND_TO_BACKEND_GENERATED_FUNCTION
              (*interface.*GetEventManagerMethod)()->Allocate()};
          if (backend_result.HasValue()) {
            // VCA_SOCAL_CALLING_STL_APIS
            result.EmplaceValue(::ara::com::SampleAllocateePtr<SampleType>{std::move(backend_result.Value())});
          } else {
            logger_.LogError(
                [](::amsr::socal::internal::logging::LogStream& s) {
                  s << "Registered binding failed to allocate memory for the requested event.";
                },
                static_cast<char const*>(__func__), __LINE__);
          }
        }
      }
    }

    return result;
  }

  /*!
   * \brief   Pre-allocates the requested amount of memory slots, which can be used to serialize the event samples while
   *          event transmission.
   * \details If the memory has been pre-allocated, the user must use the TrySend() API to transmit the event sample,
   *          since the memory slots might get exhausted. Other combinations with Send() or Allocate() are not
   *          permitted.
   * \param[in] count  Number of memory slots to be pre-allocated. Each slot can be used to send one sample. The memory
   *                   is freed once the sample is read by all readers.
   * \pre         Service must not be offered before calling this API.
   * \pre         The datatype used has a deterministic maximum size, i.e. AUTOSAR Primitive Data Types or Vector types.
   * \pre         PreAllocate has not been called.
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   *              Not threadsafe against following APIs of the associated Skeleton instance:
   *              - Skeleton::OfferService
   *              - Skeleton::StopOfferService
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \internal
   * If the service is not already offered and PreAllocate is never called before.
   * - If data count is greater than zero,
   *   - The sample data count is passed to the registered backend for this event.
   * - Else silently ignore the call and log an error message.
   *
   * Calls "ara::core::Abort()" if:
   * - The service is already offered.
   * - \p PreAllocate is already called.
   * \endinternal
   */
  [[deprecated("PreAllocate() is deprecated and will be removed in the future; AMSR-31145")]] void PreAllocate(
      std::size_t const count) noexcept {
    if (skeleton_->IsServiceOffered()) {
      logger_.LogFatalAndAbort(
          [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            // VCA_SOCAL_CALLING_STL_APIS
            s << "The service is already offered.";
          },
          static_cast<char const*>(__func__), __LINE__);
    } else if (preallocate_called_) {
      logger_.LogFatalAndAbort(
          [](::amsr::socal::internal::logging::AraComLogger::StringStream& s) {
            // VCA_SOCAL_CALLING_STL_APIS
            s << "The API is already called.";
          },
          static_cast<char const*>(__func__), __LINE__);
    } else if (count == 0U) {
      logger_.LogError(
          [](::amsr::socal::internal::logging::LogStream& s) {
            s << "PreAllocate called with count 0. Pre-allocation of samples will be ignored.";
          },
          static_cast<char const*>(__func__), __LINE__);
    } else {
      preallocate_called_ = true;

      typename Skeleton::SkeletonBackendInterfaceUniquePtrCollection const& backend_interfaces{
          skeleton_->GetBackendInterfaces()};

      // VCA_SOCAL_VALID_SKELETON_BACKEND_INTERFACE_COLLECTION
      for (SkeletonBackendInterfaceUniquePtr const& concrete_backend_interface : backend_interfaces) {
        // VCA_SOCAL_SEND_TO_BACKEND_GENERATED_FUNCTION
        (*concrete_backend_interface.*GetEventManagerMethod)()->PreAllocate(count);
      }
    }
  }

  /*!
   * \brief Send event data over the backend registered.
   * \param[in] data  A reference to event data allocated by the service application developer.
   * \return Result with no value or ComErrc::kSampleAllocationFailure if memory allocation failed in backends.
   * \error ComErrc::kSampleAllocationFailure  In case of no free memory is available.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate
   * \steady TRUE
   * \internal
   * - Get the backend interfaces.
   * - For each backend.
   *   - Get the event manager.
   *   - Send the data via the event manager.
   *   - Break on first failure.
   * \endinternal
   */
  ::ara::core::Result<void> SendInternal(SampleType const& data) const noexcept {
    ::ara::core::Result<void> result{::ara::com::ComErrc::kServiceNotAvailable};

    // Get the backend implementations that are retrieved on an OfferService of the skeleton object.
    typename Skeleton::SkeletonBackendInterfaceUniquePtrCollection const& backend_interfaces{
        skeleton_->GetBackendInterfaces()};

    // Send the event sample to all the backend implementations via the backend-specific event manager.
    // VCA_SOCAL_VALID_SKELETON_BACKEND_INTERFACE_COLLECTION
    for (SkeletonBackendInterfaceUniquePtr const& interface : backend_interfaces) {
      // VCA_SOCAL_SEND_TO_BACKEND_GENERATED_FUNCTION, VCA_SOCAL_CALLING_METHODS_ON_OBJECTS_PASSED_TO_FUNCTION
      result = (*interface.*GetEventManagerMethod)()->Send(data);

      if (!result.HasValue()) {
        break;
      }
    }
    return result;
  }

 private:
  /*!
   * \brief A reference to the skeleton instance, which will be used to fetch all the registered backend interfaces at a
   *        later point, e.g. during the event transmission.
   */
  SkeletonPtr const skeleton_;

  /*!
   * \brief An instance of the logger.
   */
  ::amsr::socal::internal::logging::AraComLogger const logger_{
      ::amsr::socal::internal::logging::kAraComLoggerContextId,
      ::amsr::socal::internal::logging::kAraComLoggerContextDescription, "SkeletonEvent"_sv};

  /*!
   * \brief Flag to indicate whether PreAllocate API is called.
   */
  bool preallocate_called_{false};
};

}  // namespace events
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_EVENTS_SKELETON_EVENT_H_
