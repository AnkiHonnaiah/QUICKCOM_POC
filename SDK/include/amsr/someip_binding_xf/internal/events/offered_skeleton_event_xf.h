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
/**        \file  someip_binding_xf/internal/events/offered_skeleton_event_xf.h
 *        \brief  SOME/IP binding implementation of OfferedSkeletonEventXf.
 *        \unit   SomeIpBinding::SomeIpBindingXf::Communication::Events::SkeletonEventXf
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_OFFERED_SKELETON_EVENT_XF_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_OFFERED_SKELETON_EVENT_XF_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <memory>

#include "amsr/someip_binding/internal/configuration/event_config.h"
#include "amsr/someip_binding_core/internal/local_server_interface.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_core/internal/polling_utilities.h"
#include "amsr/someip_binding_core/internal/safe_ipc_polling_interface.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "amsr/someip_binding_xf/internal/events/allocated_sample.h"
#include "amsr/someip_binding_xf/internal/events/event_serializer_interface.h"
#include "amsr/thread/this_thread.h"
#include "amsr/unique_ptr.h"
#include "ara/com/com_error_domain.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace events {

// VECTOR NC AutosarC++17_10-M7.3.4: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.4_using_directive
// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOMEIPBINDING_AutosarC++17_10-M7.3.6_using_directive
using namespace vac::container::literals;  // NOLINT(build/namespaces)

/*!
 * \brief OfferedSkeletonEventXf holds all objects needed during offered state.
 *
 * \tparam  SampleType Type of the serialized event sample.
 */
template <typename SampleType>
class OfferedSkeletonEventXf final {
 public:
  /*!
   * \brief Type alias for MemoryBufferAllocator
   */
  using MemoryBufferAllocator = amsr::someip_binding_core::internal::MemoryBufferAllocator;

  /*!
   * \brief Type-alias for a MemoryBufferPtr.
   */
  using MemoryBufferPtr = amsr::someip_binding_core::internal::MemoryBufferPtr;

  /*!
   * \brief Type-alias for the SafeIpcPollingInterface.
   */
  using SafeIpcPollingInterface = ::amsr::someip_binding_core::internal::SafeIpcPollingInterface;

  /*!
   * \brief Type alias for the AllocatedSample type.
   */
  using AllocatedSampleType = AllocatedSample<SampleType>;

  /*!
   * \brief Type alias for the AllocatedSample type unique_ptr.
   */
  using AllocatedSampleTypeUniquePtr = amsr::UniquePtr<AllocatedSample<SampleType>>;

  /*!
   * \brief     Constructor of OfferedSkeletonEventXf (optionally with E2E protection).
   *
   * \param[in] server_interface          Reference to the related binding server manager.
   * \param[in] serialization             Type of serialization. Its value is either SomeIp or S2S.
   * \param[in] safe_ipc_polling          Contains poll function, used to trigger the reactor to receive responses.
   * \param[in] tx_buffer_allocator       Memory buffer allocator to be used for sending samples.
   * \param[in] event_serializer          Event serializer
   *
   * \context      Init
   * \threadsafe   FALSE
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady FALSE
   */
  OfferedSkeletonEventXf(
      ::amsr::someip_binding_core::internal::LocalServerInterface& server_interface,
      ::amsr::someip_binding::internal::configuration::EventConfig::Serialization const serialization,
      std::shared_ptr<SafeIpcPollingInterface> safe_ipc_polling,
      std::unique_ptr<MemoryBufferAllocator> tx_buffer_allocator,
      std::unique_ptr<EventSerializerInterface<SampleType>> event_serializer)
      : server_interface_{server_interface},
        serialization_{serialization},
        // VCA_SOMEIPBINDING_PASSING_REFERENCE
        // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
        safe_ipc_polling_{safe_ipc_polling},
        // VCA_SOMEIPBINDING_PASSING_REFERENCE
        tx_buffer_allocator_{std::move(tx_buffer_allocator)},
        // VCA_SOMEIPBINDING_PASSING_REFERENCE
        event_serializer_{std::move(event_serializer)},
        // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
        header_size_{event_serializer_->CalculateHeaderSize()} {
    logger_.LogVerbose(static_cast<char const*>(__func__), __LINE__);
  }

  /*!
   * \brief       Clean up resources.
   *
   * \pre         -
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
   * - In case allocator cannot be destroyed (because not all allocated buffers have been already deallocated)
   *   - wait until memory is deallocated, or
   *   - until timeout expires.
   * - In case of timeout expiry, abort.
   * \endinternal
   */
  ~OfferedSkeletonEventXf() {
    logger_.LogVerbose(static_cast<char const*>(__func__), __LINE__);
    // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_AFTER_NULLPTR_CHECK
    bool can_destroy{tx_buffer_allocator_->CanDestroy()};

    if (!can_destroy) {
      // give info to the user on what is happening
      logger_.LogInfo(
          [](::ara::log::LogStream& s) noexcept {
            s << "Some events are still in the send queue. Blocking until all events are sent out.";
          },
          static_cast<char const*>(__func__), __LINE__);

      can_destroy = ::amsr::someip_binding_core::internal::PollingUtilities::WaitForInPollingMode(
          safe_ipc_polling_,
          [this] {
            // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_AFTER_NULLPTR_CHECK
            return tx_buffer_allocator_->CanDestroy();
          },
          kDefaultWaitTimeOut);

      if (!can_destroy) {
        logger_.LogFatalAndAbort(
            [](::amsr::someip_binding_core::internal::logging::StringStream& s) noexcept {
              s << "Aborting: Timeout expired while waiting to send out events."
                   "Possible reason: IPC channel between application and SOME/IP daemon is blocked. No more data "
                   "could be sent out.";
            },
            static_cast<char const*>(__func__), __LINE__);
      }
    }  // VCA_SPC_15_SOMEIPBINDING_STL_FUNCTION
    // VCA_SPC_15_SOMEIPBINDING_STL_FUNCTION
  }

  OfferedSkeletonEventXf(OfferedSkeletonEventXf const&) = delete;
  OfferedSkeletonEventXf(OfferedSkeletonEventXf&&) noexcept = delete;
  OfferedSkeletonEventXf& operator=(OfferedSkeletonEventXf const&) & = delete;
  OfferedSkeletonEventXf& operator=(OfferedSkeletonEventXf&&) & noexcept = delete;

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
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   * \internal
   * - Try to allocate a buffer of the required size.
   * - If allocation succeeded
   *   - Serialize event and send.
   * - Else return an error code.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_SOMEIPBINDING_AutosarC++17_10-A15.4.2_A15.5.3_Exception_caught
  // VECTOR NC AutosarC++17_10-A15.5.3: MD_SOMEIPBINDING_AutosarC++17_10-A15.4.2_A15.5.3_Exception_caught
  ::amsr::core::Result<void> SendEvent(SampleType const& data) noexcept {
    logger_.LogVerbose(static_cast<char const*>(__func__), __LINE__);

    amsr::core::Result<void> result{::ara::com::ComErrc::kSampleAllocationFailure};
    // Allocate required memory size
    // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
    std::size_t const payload_size{event_serializer_->GetRequiredBufferSize(data)};
    std::size_t const alloc_size{header_size_ + payload_size};

    logger_.LogDebug(
        [&payload_size, &alloc_size](::ara::log::LogStream& s) noexcept {
          s << "Calculated Payload size: ";
          s << payload_size;
          s << " bytes. Allocating (header + payload): ";
          s << alloc_size;
          s << " bytes.";
        },
        static_cast<char const*>(__func__), __LINE__);

    // Allocate memory for the serialization
    // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
    amsr::core::Result<MemoryBufferPtr> packet{tx_buffer_allocator_->Allocate(alloc_size)};

    if (packet.HasValue()) {
      result.EmplaceValue();

      // Serialize and send the event by providing both the serialization memory and payload size.
      SerializeAndSend(data, payload_size, std::move(packet.Value()));
    } else {
      logger_.LogWarn([](::ara::log::LogStream& s) noexcept { s << "Failed to allocate buffer."; },
                      static_cast<char const*>(__func__), __LINE__);
    }
    return result;
  }

  /*!
   * \brief       Send an event sample using the preallocated pointer.
   *
   * \details     This call shall never fail, as the only way of triggering it right now is only by using dynamic
   *              memory allocation.
   *
   * \param[in]   data  A reference to the allocated sample.
   *
   * \return      An empty result if memory allocation is successful, otherwise an error.
   * \error       ComErrc::kSampleAllocationFailure if memory allocation fails.
   *              This call shall never fail, as the only way of triggering it right now is only by using dynamic
   *              memory allocation.
   *
   * \context     App
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   *
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   *  - Try to acquire a wrapped memory buffer inside the allocated sample.
   *  - If available
   *    - Retrieve the reserved memory buffer and use it to serialize and send the event.
   *  - If no buffer has been preallocated
   *    - Forward the call to the overloaded API that will handle of the memory buffer allocation.
   * \endinternal
   */
  ::amsr::core::Result<void> SendPreallocatedEvent(AllocatedSampleType& data) noexcept {
    logger_.LogVerbose(static_cast<char const*>(__func__), __LINE__);

    amsr::core::Result<void> result{::ara::com::ComErrc::kSampleAllocationFailure};

    // Try to retrieve a memory buffer from the already allocated sample.
    // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10-A18.5.8_Local_object_in_heap
    // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
    MemoryBufferPtr packet{data.ReleaseBuffer()};

    // If a memory buffer is available.
    if (packet != nullptr) {
      result.EmplaceValue();
      // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
      std::size_t const payload_size{event_serializer_->GetRequiredBufferSize(*data)};
      // The memory buffer should be resized based on the data size. This will be handled once full steady
      // mode is implemented and this path gets activated.
      SerializeAndSend(*data, payload_size, std::move(packet));
    } else {
      // Call the overload Send() that will handle the memory buffer allocation.
      amsr::core::Result<void> const send_event_result{SendEvent(*data)};

      if (send_event_result.HasValue()) {
        result.EmplaceValue();
      } else {
        result.EmplaceError(send_event_result.Error());
      }
    }
    return result;
  }

  /*!
   * \brief       Allocate event data of unique ownership for sending out.
   *
   * \details     If specifically requested, a memory buffer of the maximum serialization size for this event
   *              will be preallocated and wrapped into the allocated sample pointer that will then be handed
   *              over to the user. This serialization slot will be used later, when sending the event.
   *
   * \param[in]   reserve_serialization_slot Flag to signal whether a serialization slot shall be reserved.
   *
   * \return      Result containing a pointer to the allocated memory. The returned pointer must not be null.
   * \error       ComErrc::kSampleAllocationFailure if memory allocation fails.
   *
   * \context     App
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   *
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - If reserving a serialization slot is requested
   *    - Verify if the event has a maximum size
   *      - If not, return an error indicating that the allocation failed.
   *    - Else, allocate a memory buffer of the maximum possible size and
   *      emplace it into the allocated sample that will be returned to the caller.
   * - Else
   *    - return an allocated sample unique pointer.
   * \endinternal
   */
  ::amsr::core::Result<AllocatedSampleTypeUniquePtr> Allocate(bool reserve_serialization_slot = false) noexcept {
    amsr::core::Result<AllocatedSampleTypeUniquePtr> result{::ara::com::ComErrc::kSampleAllocationFailure};

    if (reserve_serialization_slot) {
      // At this point we don't know the size of the sample that will be serialized into this slot, therefore we
      // allocate the maximum buffer size
      // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
      amsr::core::Result<std::size_t> const alloc_size{event_serializer_->GetMaximumBufferSize()};
      if (alloc_size.HasValue()) {
        // Allocate memory for the serialization
        amsr::core::Result<MemoryBufferPtr> memory_buffer_ptr_alloc_result{
            // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
            tx_buffer_allocator_->Allocate(alloc_size.Value())};

        if (memory_buffer_ptr_alloc_result.HasValue()) {
          // VECTOR NC AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10-A18.5.8_Local_object_in_heap
          MemoryBufferPtr allocated_packet{std::move(memory_buffer_ptr_alloc_result.Value())};

          result.EmplaceValue(amsr::AllocateUnique<AllocatedSampleType>(
                                  ara::core::PolymorphicAllocator<AllocatedSampleType>{}, std::move(allocated_packet))
                                  .Value());
        }
      } else {
        logger_.LogError(
            [](::ara::log::LogStream& s) noexcept {
              s << "Failed to preallocate serialization memory. The modelled data type is of dynamic size and has no "
                   "maximum predefined value.";
            },
            static_cast<char const*>(__func__), __LINE__);
      }
    } else {
      result.EmplaceValue(
          amsr::AllocateUnique<AllocatedSampleType>(ara::core::PolymorphicAllocator<AllocatedSampleType>{}).Value());
    }

    return result;
  }

 private:
  /*!
   * \brief       Serializes the event into the provided buffer and sends it.
   *
   * \param[in]   data          Event sample value to be transmitted.
   * \param[in]   payload_size  The payload size.
   * \param[in]   buffer        Memory for serializing the event.
   *
   * \context     App
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   *
   * \steady      TRUE
   *
   * \internal
   *  - If a SOME/IP event is to be sent, serialize it accordingly and send it.
   *  - Else serialize it as a PDU event and send it.
   * \endinternal
   */
  void SerializeAndSend(SampleType const& data, std::size_t const payload_size, MemoryBufferPtr buffer) noexcept {
    logger_.LogVerbose(static_cast<char const*>(__func__), __LINE__);

    // VECTOR NC AutosarC++17_10-M0.3.1: MD_SOMEIPBINDING_AutosarC++17_10-M0.3.1_ptr_null_at_dereference_fp
    // VCA_SOMEIPBINDING_PACKET_BUILDER_PACKET_ALLOCATION
    amsr::core::Span<std::uint8_t> body_view{buffer->GetView(0)};

    // Serialize the headers + payload
    // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
    event_serializer_->SerializeEvent(body_view, payload_size, data);

    // Finally transmit the serialized packet via ServerManager
    if (serialization_ == ::amsr::someip_binding::internal::configuration::EventConfig::Serialization::someip) {
      // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
      server_interface_.SendEventNotification(std::move(buffer));
    } else {
      // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
      server_interface_.SendPduEventNotification(std::move(buffer));
    }
  }

  /*!
   * \brief Related SOME/IP server interface.
   */
  ::amsr::someip_binding_core::internal::LocalServerInterface& server_interface_;

  /*!
   * \brief Type of serialization. Its value is either SomeIp or S2S.
   */
  ::amsr::someip_binding::internal::configuration::EventConfig::Serialization const serialization_;

  /*!
   * \brief Interface for polling wrappers.
   */
  std::shared_ptr<SafeIpcPollingInterface> safe_ipc_polling_;

  /*!
   * \brief Memory allocator for the serialized packet.
   */
  std::unique_ptr<MemoryBufferAllocator> tx_buffer_allocator_;

  /*!
   * \brief Event serializer
   */
  std::unique_ptr<EventSerializerInterface<SampleType>> event_serializer_;

  /*!
   * \brief The event header size.
   */
  std::size_t header_size_;

  /*!
   * \brief The default maximum time in milliseconds to wait until all memory is deallocated before aborting during
   * destruction.
   */
  std::chrono::steady_clock::duration const kDefaultWaitTimeOut{std::chrono::milliseconds{10000U}};

  /*!
   * \brief The default time period in nanoseconds to wait between different polls until all memory is deallocated.
   */
  std::chrono::steady_clock::duration const kDefaultWaitTriggerPeriod{std::chrono::nanoseconds{100U}};

  /*!
   * \brief Logger for tracing and debugging
   */
  ::amsr::someip_binding_core::internal::logging::AraComLogger logger_{
      ::amsr::someip_binding_core::internal::logging::kSomeIpLoggerContextId,
      ::amsr::someip_binding_core::internal::logging::kSomeIpLoggerContextDescription, "OfferedSkeletonEventXf"_sv};
};

}  // namespace events
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_OFFERED_SKELETON_EVENT_XF_H_
