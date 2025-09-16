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
/**        \file  someip_binding_xf/internal/events/skeleton_event_xf.h
 *        \brief  SOME/IP binding implementation of SkeletonEventXf.
 *        \unit   SomeIpBinding::SomeIpBindingXf::Communication::Events::SkeletonEventXf
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_SKELETON_EVENT_XF_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_SKELETON_EVENT_XF_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <map>
#include <memory>
#include <utility>

#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/core/string.h"
#include "amsr/iostream/stateful_output_stream.h"
#include "amsr/iostream/stringstream/output_string_stream.h"
#include "amsr/socal/internal/events/skeleton_event_manager_interface.h"
#include "amsr/someip_binding/internal/configuration/config_types.h"
#include "amsr/someip_binding/internal/configuration/event_config.h"
#include "amsr/someip_binding_core/internal/local_server_interface.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_core/internal/logging/log_builder.h"
#include "amsr/someip_binding_core/internal/logging/logger_prefix_generator.h"
#include "amsr/someip_binding_core/internal/safe_ipc_polling_interface.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "amsr/someip_binding_xf/internal/events/event_serializer_interface.h"
#include "amsr/someip_binding_xf/internal/events/memory_buffer_allocator_factory_interface.h"
#include "amsr/someip_binding_xf/internal/events/offered_skeleton_event_xf.h"
#include "amsr/someip_binding_xf/internal/events/signal_based_event_serializer.h"
#include "amsr/someip_binding_xf/internal/events/someip_event_serializer.h"
#include "amsr/someip_binding_xf/internal/ser_deser/serializer_interface.h"
#include "amsr/unique_ptr.h"
#include "ara/com/com_error_domain.h"
#include "someip-protocol/internal/message.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace events {

// VECTOR NC AutosarC++17_10-M7.3.4: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.4_using_directive
// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.6_using_directive
using namespace vac::container::literals;  // NOLINT(build/namespaces)

/*!
 * \brief   Template for SOME/IP skeleton event manager.
 *
 * \tparam  SampleType Type of the serialized event sample.
 */
template <typename SampleType>
class SkeletonEventXf final : public ::amsr::socal::internal::events::SkeletonEventManagerInterface<SampleType> {
 public:
  /*!
   * \brief Type alias for SerializerUniquePtr.
   */
  using SerializerUniquePtr =
      std::unique_ptr<::amsr::someip_binding_xf::internal::ser_deser::SerializerInterface<SampleType>>;

  /*!
   * \brief Type alias for SkeletonEventManagerInterface.
   */
  using SkeletonEventManagerInterface = ::amsr::socal::internal::events::SkeletonEventManagerInterface<SampleType>;

  /*!
   * \brief Type-alias for the SafeIpcPollingInterface.
   */
  using SafeIpcPollingInterface = ::amsr::someip_binding_core::internal::SafeIpcPollingInterface;

  /*!
   * \brief     Constructor of SkeletonEventXf (optionally with E2E protection).
   * \details   The sample buffer allocator is default initialized as a dynamic allocator created by the user provided
   *            abstract factory. It is overwritten with a new instance of an allocator created through the same factory
   *            if PreAllocate is called with a non-zero count.
   *
   * \param[in] someip_event_identity     SOME/IP event identity.
   * \param[in] server_interface          Reference to the related binding server manager.
   * \param[in] serializer                Unique pointer to the the payload serializer. Cannot be nullptr.
   * \param[in] e2e_parametrization_data  Event E2E parametrization data. Nullopt if not E2E protected.
   * \param[in] session_handling_disabled Session handling flag
   * \param[in] serialization             Type of serialization. Its value is either SomeIp or S2S.
   * \param[in] pdu_header_extn           The pdu header extension fields for signal based messages.
   * \param[in] allocator_factory         Factory memory buffer allocator that is used to create the default dynamic
   *                                      allocator and to create a deterministic allocator if PreAllocate is called
   *                                      with a non-zero count argument. Cannot be nullptr.
   * \param[in] safe_ipc_polling          Contains poll function, used to trigger the reactor to receive responses.
   *
   * \context      Init
   * \threadsafe   FALSE
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       FALSE
   * \spec
   *   requires serializer != nullptr;
   *   requires allocator_factory != nullptr;
   * \endspec
   */
  SkeletonEventXf(
      ::amsr::someip_binding_core::internal::SomeIpEventIdentity const& someip_event_identity,
      ::amsr::someip_binding_core::internal::LocalServerInterface& server_interface, SerializerUniquePtr serializer,
      amsr::core::Optional<
          ::amsr::someip_binding::internal::configuration::SkeletonEventXfE2EParametrizationData> const&
          e2e_parametrization_data,
      bool const session_handling_disabled,
      ::amsr::someip_binding::internal::configuration::EventConfig::Serialization const serialization,
      amsr::core::Optional<::amsr::someip_binding::internal::configuration::EventConfig::PduHeaderExtensionTx> const&
          pdu_header_extn,
      std::shared_ptr<MemoryBufferAllocatorFactoryInterface> allocator_factory,
      std::shared_ptr<SafeIpcPollingInterface> safe_ipc_polling) noexcept
      : SkeletonEventManagerInterface{},
        someip_event_identity_{someip_event_identity},
        server_interface_{server_interface},
        // VCA_SOMEIPBINDING_PASSING_REFERENCE
        serializer_{std::move(serializer)},
        e2e_parametrization_data_{e2e_parametrization_data},
        session_handling_disabled_{session_handling_disabled},
        pdu_header_extn_{pdu_header_extn},
        // VCA_SOMEIPBINDING_PASSING_REFERENCE
        allocator_factory_{std::move(allocator_factory)},
        // VCA_SPC_15_SOMEIPBINDING_STL_FUNCTION
        // VCA_SOMEIPBINDING_PASSING_REFERENCE
        safe_ipc_polling_{safe_ipc_polling},
        // VCA_SOMEIPBINDING_PASSING_REFERENCE
        serialization_{serialization} {
    logger_.LogVerbose(static_cast<char const*>(__func__), __LINE__);
  }

  /*!
   * \brief       Destructor
   *
   * \details     Cleanup the resources by explicitly stop offering this event.
   *
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SOMEIPBINDING_SKELETON_EVENT_XF_OBJECT_LIFECYCLE
  ~SkeletonEventXf() noexcept final { StopOfferService(); }

  SkeletonEventXf(SkeletonEventXf const&) = delete;
  SkeletonEventXf(SkeletonEventXf&&) noexcept = delete;
  SkeletonEventXf& operator=(SkeletonEventXf const&) & = delete;
  SkeletonEventXf& operator=(SkeletonEventXf&&) & noexcept = delete;

  /*!
   * \brief       Initializes all the necessary elements when the service is being offered
   *              and sends the initial value to the Daemon for fields with notifications.
   *
   * \pre         Service is not being offered.
   *
   * \context     App
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \internal
   *  - If the event has already been offered.
   *    - Log violation and abort.
   *  - Else
   *    - Instantiate the event serializer based on the serialization type (SOME/IP or S2S).
   *    - Instantiate the tx buffer allocator with the stored value for the memory slot count,
   *      being deterministic if zero or dynamic otherwise.
   *    - If the tx buffer allocator could not be created.
   *      - Log fatal and abort.
   *    - Else
   *        - Create an instance of an OfferedSkeletonEventXf by moving both the serializer and
   *          tx buffer allocator.
   *        - If the initial field value has ever been set
   *            - Send an event to the Daemon to fill their cache.
   * \endinternal
   */
  void OfferService() noexcept {
    logger_.LogVerbose(static_cast<char const*>(__func__), __LINE__);

    if (!offered_skeleton_event_xf_.has_value()) {
      // VECTOR NL AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10_A18.5.8_false_positive
      std::unique_ptr<EventSerializerInterface<SampleType>> event_serializer{nullptr};

      // If this is an E2E protected event, the E2E transformer state machine will be created as part
      // of the event serializer.
      if (serialization_ == ::amsr::someip_binding::internal::configuration::EventConfig::Serialization::someip) {
        // VCA_SOMEIPBINDING_POSSIBLY_INVALID_ARGUMENT
        event_serializer = std::make_unique<SomeIpEventSerializer<SampleType>>(
            someip_event_identity_, *serializer_, e2e_parametrization_data_, session_handling_disabled_);
      } else {
        // VCA_SOMEIPBINDING_POSSIBLY_INVALID_ARGUMENT
        event_serializer = std::make_unique<SignalBasedEventSerializer<SampleType>>(
            someip_event_identity_, *serializer_, e2e_parametrization_data_, pdu_header_extn_);
      }

      // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
      std::size_t const header_size{event_serializer->CalculateHeaderSize()};

      std::unique_ptr<MemoryBufferAllocator> tx_buffer_allocator{nullptr};
      // Create the tx buffer allocator and move it into the offered skeleton event xf.
      if (memory_slots_count_ > 0) {
        // Deterministic Memory
        // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
        amsr::core::Result<std::size_t> const max_sample_size{serializer_->GetMaximumBufferSize()};
        if (max_sample_size.HasValue()) {
          // Allocate header size + payload size
          std::size_t const alloc_size{header_size + max_sample_size.Value()};

          logger_.LogDebug(
              [this, &max_sample_size, &alloc_size](::ara::log::LogStream& s) noexcept {
                s << "PreAllocate: ";
                s << memory_slots_count_;
                s << " samples. Each of maximum payload size: ";
                s << max_sample_size.Value();
                s << " bytes. Allocating (header + payload): ";
                s << alloc_size;
                s << " bytes per sample.";
              },
              static_cast<char const*>(__func__), __LINE__);

          // VECTOR NC AutosarC++17_10-M0.3.1: MD_SOMEIPBINDING_AutosarC++17_10-M0.3.1_ptr_null_at_dereference
          // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
          // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
          tx_buffer_allocator = allocator_factory_->CreateAllocator(memory_slots_count_, alloc_size);
        } else {
          logger_.LogFatalAndAbort(
              [](::amsr::someip_binding_core::internal::logging::StringStream& s) {
                s << "Event notification buffer pre-allocation is not possible for datatypes where the max. necessary "
                     "buffer size cannot be determined.";
              },
              static_cast<char const*>(__func__), __LINE__);
        }
      } else {
        // Dynamic Memory
        // VECTOR NC AutosarC++17_10-M0.3.1: MD_SOMEIPBINDING_AutosarC++17_10-M0.3.1_ptr_null_at_dereference
        // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
        // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
        tx_buffer_allocator = allocator_factory_->CreateAllocator();
      }

      // Assert that the factory created a valid allocator
      if (tx_buffer_allocator != nullptr) {
        // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
        offered_skeleton_event_xf_.emplace(server_interface_, serialization_, safe_ipc_polling_,
                                           std::move(tx_buffer_allocator), std::move(event_serializer));

        // If this represents a field, we must send the latest cached value to the daemon in order to set their cache
        // field as well. By doing this, any client that subscribes immediately after our service is offered will
        // have access to a valid initial value.
        if (IsField()) {
          amsr::core::Result<void> const res{
              offered_skeleton_event_xf_.value().SendEvent(initial_field_value_.value())};

          if (!res.HasValue()) {
            logger_.LogFatalAndAbort(
                [](::amsr::someip_binding_core::internal::logging::StringStream& s) {
                  s << "Unable to send initial field value.";
                },
                static_cast<char const*>(__func__), __LINE__);
          }
        }
      } else {
        logger_.LogFatalAndAbort(
            [](::amsr::someip_binding_core::internal::logging::StringStream& s) {
              s << "Violation: memory allocator is invalid.";
            },
            static_cast<char const*>(__func__), __LINE__);
      }
    } else {
      logger_.LogFatalAndAbort(
          [](::amsr::someip_binding_core::internal::logging::StringStream& s) {
            s << "Violation: Skeleton event already offered.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief       Cleanup the resources used while service is being offered.
   *
   * \details     This will delete the event serializer which holds the E2E state machine, so
   *              that if the service is offered once again, all the initial states will be
   *              at their correct reset value.
   *
   * \context     App
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   *  - If the optional objet holding all the needed elements while being on offered has a value
   *      - Reset it.
   * \endinternal
   */
  void StopOfferService() noexcept {
    logger_.LogVerbose(static_cast<char const*>(__func__), __LINE__);
    if (offered_skeleton_event_xf_.has_value()) {
      offered_skeleton_event_xf_.reset();
    }
  }

 private:
  /*!
   * \brief Type alias to the AllocatedSampleInterfaceUniquePtr.
   */
  using AllocatedSampleInterfaceUniquePtr =
      typename ::amsr::socal::internal::events::SkeletonEventManagerInterface<SampleType>::AllocatedSampleUniquePtr;

  /*!
   * \brief Type alias for the AllocatedSample type unique_ptr.
   */
  using AllocatedSampleTypeUniquePtr = amsr::UniquePtr<AllocatedSample<SampleType>>;

  /*!
   * \brief Type alias for MemoryBufferAllocator
   */
  using MemoryBufferAllocator = amsr::someip_binding_core::internal::MemoryBufferAllocator;

  /*!
   * \brief       Send an event sample.
   *
   * \param[in]   data  Event sample value to be transmitted.
   * \return      an empty result if memory allocation is successful, otherwise an error.
   * \error       ComErrc::kSampleAllocationFailure if memory allocation fails.
   *              This could happen if preallocated memory is used,
   *              and at the moment Send() is called, there was no empty slots.
   *              Possible error handling: Retry to send after some time.
   *
   * \pre \p count > 0 is allowed for events of types with deterministic maximum size, i.e. AUTOSAR Primitive Data Types
   * or Vector types. Otherwise the API call will abort. The API must be called once after construction.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \vprivate Product Private
   * \trace SPEC-4980043
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   *
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOMEIPBINDING_AutosarC++17_10-A15.4.2_A15.5.3_Exception_caught
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOMEIPBINDING_AutosarC++17_10-A15.4.2_A15.5.3_Exception_caught
  ::amsr::core::Result<void> Send(SampleType const& data) noexcept final {
    logger_.LogDebug(
        [this](ara::log::LogStream& s) {
          // VCA_SOMEIPBINDING_PASSING_REFERENCE
          amsr::someip_binding_core::internal::logging::LogBuilder::LogEventId(s, someip_event_identity_);
        },
        static_cast<char const*>(__func__), __LINE__);

    amsr::core::Result<void> result{::ara::com::ComErrc::kSampleAllocationFailure};

    // Only send the event to the Daemon if the event has been offered. By avoiding the serialization, we want to
    // prevent changes to the e2e states from the e2e state machine.
    if (offered_skeleton_event_xf_.has_value()) {
      OfferedSkeletonEventXf<SampleType>& offered_skeleton_event_xf{offered_skeleton_event_xf_.value()};

      amsr::core::Result<void> const res{offered_skeleton_event_xf.SendEvent(data)};

      // Update the initial value if this represents a field. This will allow us to always have a valid state among
      // multiple cycles of offering and stopping the service for initial fields notifications.
      if (IsField()) {
        initial_field_value_.emplace(data);
      }

      if (res.HasValue()) {
        result.EmplaceValue();
      }
    } else {
      // If the service has not been offered and we receive a send, it must be for setting the initial field value.
      // Update the initial value. This will allow us to always have a valid state among
      // multiple cycles of offering and stopping the service for initial fields notifications.
      initial_field_value_.emplace(data);
      // Clear the error, as we are only caching the initial value.
      result.EmplaceValue();
    }

    return result;
  }

  /*!
   * \brief       Send an event sample using the preallocated pointer.
   *
   * \param[in]   sample_ptr Sample pointer containing the data to be transmitted. The given pointer must not be null.
   * \return      A result signalling successful event sample sending.
   *
   * \pre         The skeleton needs to be in an offered state.
   * \pre         The usage of Allocate() API paired together with PreAllocate() API is not supported.
   * \pre         The provided sample pointer must have been requested to this same instance by the corresponding call
   *              to the Allocate() API.
   *
   * \context     App
   *
   * \threadsafe  TRUE for different Event class instances.
   * \reentrant   TRUE for different Event class instances.
   * \synchronous TRUE
   *
   * \steady      TRUE
   *
   * \vprivate    Product Private
   *
   * \spec
   *   requires sample_ptr != nullptr;
   * \endspec
   *
   * \internal
   *  - If the event is not in an offered state OR preallocate has been called.
   *    - Abort, as this is considered a violation.
   *  - Else
   *    - Downcast the provided allocated sample to the binding specific type. The binding specific type can hold a
   *      memory buffer that can be used during steady mode to serialize and send the sample. This buffer must have
   *      been previously requested during the call to Allocate(). As for now, only dynamic memory allocation will be
   *      allowed for this path, hence the buffer will not be available yet. Once full steady mode is implemented, this
   *      feature can then be enabled so that, upon a a call to Allocate(), both serialization and deserialization
   *      memories are allocated from the already configured memory resources, guaranteeing that a later call to Send()
   *      will not fail due to memory allocation failure.
   *    - Forward the call to the OffererdSkeletonEventXf, who will handle serialization and send of this event.
   * \endinternal
   */
  ::ara::core::Result<void> Send(AllocatedSampleInterfaceUniquePtr sample_ptr) noexcept final {
    if (!offered_skeleton_event_xf_.has_value()) {
      logger_.LogFatalAndAbort(
          [](::amsr::someip_binding_core::internal::logging::StringStream& s) {
            s << "Precondition Violation: Allocate cannot be called if the skeleton event is not offered.";
          },
          static_cast<char const*>(__func__), __LINE__);
    } else {
      // Check if PreAllocation was called before
      if (memory_slots_count_ == 0) {
        OfferedSkeletonEventXf<SampleType>& offered_skeleton_event_xf{offered_skeleton_event_xf_.value()};

        AllocatedSample<SampleType>& allocated_someip_binding_sample{
            // clang-format off
            // VECTOR NC AutosarC++17_10-M5.2.3: MD_SOMEIPBINDING_AutosarC++17_10-M5.2.3_cast_from_polymorphic_base_class_to_derived_class
            // VECTOR NC VectorC++-V5.2.3: MD_SOMEIPBINDING_VectorC++-V5.2.3_cast_from_polymorphic_base_class_to_derived_class
            // clang-format on
            static_cast<AllocatedSample<SampleType>&>(*(sample_ptr.get()))};

        amsr::core::Result<void> const res{
            // VCA_SOMEIPBINDING_POSSIBLY_INVALID_ARGUMENT
            offered_skeleton_event_xf.SendPreallocatedEvent(allocated_someip_binding_sample)};

      } else {
        logger_.LogFatalAndAbort(
            [](::amsr::someip_binding_core::internal::logging::StringStream& s) {
              s << "Precondition Violation: The usage of Allocate/Send API pair together with PreAllocate API is not "
                   "supported.";
            },
            static_cast<char const*>(__func__), __LINE__);
      }
    }

    return ::ara::core::Result<void>{};
  }

  /*!
   * \brief   Allocate event data of unique ownership for sending out.
   *
   * \return  Result containing a pointer to the allocated memory. The returned pointer must not be null.
   *
   * \pre     The skeleton needs to be in an offered state.
   * \pre     The usage of Allocate() API paired together with PreAllocate() API is not supported.
   *
   * \context App
   *
   * \threadsafe  TRUE for different Event class instances.
   * \reentrant   TRUE for different Event class instances.
   * \synchronous TRUE
   *
   * \steady      TRUE
   *
   * \vprivate Product Private
   *
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   *  - If the event has not been offered yet OR if preallocate has been called
   *    - Log fatal and abort, as this is a precondition violation.
   *  - Else
   *    - Request the allocated sample pointer to the OfferedSkeletonEventXf. Once full steady mode is implemented, this
   *      call can be updated to indicate that a serialization slot must be reserved for further usage upon call to
   *      Send(). The serialization slot will be eventually be allocated by the OfferedSkeletonEventXf using its own
   *      memory buffer allocator. For now, we rely on the default behavior, that is, no memory buffer will be reserver
   *      so that when Send() is called, dynamic memory allocation will still happen for the serialization memory.
   *    - Return the allocated sample pointer to the caller or an error if the operation failed.
   * \endinternal
   */
  ::amsr::core::Result<AllocatedSampleInterfaceUniquePtr> Allocate() noexcept final {
    logger_.LogVerbose(static_cast<char const*>(__func__), __LINE__);

    ::amsr::core::Result<AllocatedSampleInterfaceUniquePtr> allocated_sample_interface_ptr_result{
        ::ara::com::ComErrc::kSampleAllocationFailure};

    if (!offered_skeleton_event_xf_.has_value()) {
      logger_.LogFatalAndAbort(
          [](::amsr::someip_binding_core::internal::logging::StringStream& s) {
            s << "Precondition Violation: Allocate cannot be called if the skeleton event is not offered.";
          },
          static_cast<char const*>(__func__), __LINE__);
    } else {
      // Check if PreAllocation was called before
      if (memory_slots_count_ == 0) {
        ::amsr::core::Result<AllocatedSampleTypeUniquePtr> allocated_sample_ptr_result{
            ::ara::com::ComErrc::kSampleAllocationFailure};

        // VCA_SOMEIPBINDING_UNKNOWN_CONSTRUCTOR_TEMPLATE_INSTANCE
        allocated_sample_ptr_result = offered_skeleton_event_xf_.value().Allocate();

        // Until the new path gets integrated, the call to Allocate() shall not fail under any circumstances, as only
        // dynamic memory allocation is allowed in the existing implementation.
        assert(allocated_sample_ptr_result.HasValue() == true);  // COV_MSR_INV_STATE_ASSERT

        allocated_sample_interface_ptr_result.EmplaceValue(std::move(allocated_sample_ptr_result.Value()));
      } else {
        logger_.LogFatalAndAbort(
            [](::amsr::someip_binding_core::internal::logging::StringStream& s) {
              s << "Precondition Violation: The usage of Allocate/Send API pair together with PreAllocate API is not "
                   "supported.";
            },
            static_cast<char const*>(__func__), __LINE__);
      }
    }

    return allocated_sample_interface_ptr_result;
  }

  /*!
   * \brief           Pre-Allocate \p count memory slots to be used when samples are transmitted with Send().
   *
   * \details         This will not create any allocator, but instead store the memory slots count to be used
   *                  later during the call to OfferService, where the actual tx buffer allocator will be
   *                  instantiated. Calling this functions multiple times will only override the previous value.
   *
   * \param[in] count Number of memory slots to be pre-allocated. Each slot can be used to send one sample.
   *                  The memory is freed once the sample is read by all readers.
   *                  Value 0 indicates that dynamic memory allocation strategy shall be use in context of Send().
   *                  A default flexible dynamic allocator is used if PreAllocate is never called.
   * \pre             The event must not be in an offered state.
   * \pre             No events have been sent so it's safe to overwrite the allocator without deleting
   *                  pending event data.
   * \context     App
   * \threadsafe  FALSE for same class instances, TRUE for different instances.
   * \reentrant   FALSE for same class instances, FALSE for different instances.
   * \synchronous TRUE
   * \steady      FALSE
   * \vprivate Product Private
   */
  // VECTOR NC AutosarC++17_10-M0.1.8: MD_SOMEIPBINDING_AutosarC++17_10-M0.1.8_Void_function_no_side_effect
  void PreAllocate(std::size_t const count) noexcept final {
    logger_.LogVerbose(static_cast<char const*>(__func__), __LINE__);

    if (offered_skeleton_event_xf_.has_value()) {
      logger_.LogFatalAndAbort(
          [](::amsr::someip_binding_core::internal::logging::StringStream& s) {
            s << "Precondition Violation: PreAllocate cannot be called if the skeleton event is already offered.";
          },
          static_cast<char const*>(__func__), __LINE__);
    } else {
      memory_slots_count_ = count;
    }
  }

  /*!
   * \brief           Checks whether the current SkeletonEventXf represents a field.
   *
   * \details         Distinguishing an event from a field is done using the optional initial field value.
   *                  This value is only set for fields by sending a sample while the service is not yet
   *                  offered.
   *
   * \return          True if this is a field, false otherwise.
   *
   * \pre             -
   *
   * \context     App
   *
   * \threadsafe  FALSE for same class instances, TRUE for different instances.
   * \reentrant   FALSE for same class instances, FALSE for different instances.
   * \synchronous TRUE
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  bool IsField() const noexcept { return initial_field_value_.has_value(); }

  /*!
   * \brief SOME/IP ID of this event.
   */
  someip_binding_core::internal::SomeIpEventIdentity const someip_event_identity_;

  /*!
   * \brief Related SOME/IP server interface.
   */
  ::amsr::someip_binding_core::internal::LocalServerInterface& server_interface_;

  /*!
   * \brief Payload serializer.
   */
  SerializerUniquePtr serializer_;

  /*!
   * \brief Event E2E parametrization data.
   */
  amsr::core::Optional<::amsr::someip_binding::internal::configuration::SkeletonEventXfE2EParametrizationData> const
      e2e_parametrization_data_;

  /*!
   * \brief Session handling flag.
   */
  bool const session_handling_disabled_;

  /*!
   * \brief The offset of the pdu payload.
   */
  amsr::core::Optional<::amsr::someip_binding::internal::configuration::EventConfig::PduHeaderExtensionTx> const
      pdu_header_extn_;

  /*!
   * \brief Memory allocator factory.
   */
  std::shared_ptr<MemoryBufferAllocatorFactoryInterface> allocator_factory_;

  /*!
   * \brief Interface for polling wrappers which contains reactor polling function, it is called while waiting for
   * a command response.
   */
  std::shared_ptr<SafeIpcPollingInterface> safe_ipc_polling_;

  /*!
   * \brief Type of serialization. Its value is either SomeIp or S2S.
   */
  ::amsr::someip_binding::internal::configuration::EventConfig::Serialization const serialization_;

  /*!
   * \brief   Variable to store the initial field value for this event. This value will be sent to the daemon just
   *          before offering the service, ensuring that the cache has a valid value when the service is offered.
   *          The value will be updated with each call to Send() to maintain validity, even during multiple cycles
   *          of offering and stopping the service.
   */
  amsr::core::Optional<SampleType> initial_field_value_{};

  /*!
   * \brief OfferedSkeletonEventXf holds all objects needed during offered state. During offer, this optional will
   *        be set. It has no value when this SkeletonEventXf is not offered.
   */
  amsr::core::Optional<OfferedSkeletonEventXf<SampleType>> offered_skeleton_event_xf_{};

  /*!
   * \brief Number of memory slots to be pre-allocated. If zero, then it means that we will create a dynamic buffer
   *        allocator for initializing the optional OfferedSkeletonEventXf.
   */
  std::size_t memory_slots_count_{0};

  /*!
   * \brief Logger Prefix.
   */
  amsr::core::String const logger_prefix_{
      someip_binding_core::internal::logging::LoggerPrefixGenerator::GetLoggerPrefix(
          "SkeletonEventXf"_sv, someip_event_identity_.GetServiceId(), someip_event_identity_.GetInstanceId(),
          someip_event_identity_.GetMajorVersion(), someip_event_identity_.GetEventId())};

  /*!
   * \brief Logger for tracing and debugging
   */
  ::amsr::someip_binding_core::internal::logging::AraComLogger logger_{
      ::amsr::someip_binding_core::internal::logging::kSomeIpLoggerContextId,
      ::amsr::someip_binding_core::internal::logging::kSomeIpLoggerContextDescription, logger_prefix_};
};

}  // namespace events
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_SKELETON_EVENT_XF_H_
