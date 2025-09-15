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
/*!        \file  sample_reader.h
 *        \brief  Deserializes event samples which are not e2e protected
 *         \unit  SomeIpBinding::SomeIpBindingXf::Communication::Events::SampleReader
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_SAMPLE_READER_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_SAMPLE_READER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_binding_core/internal/events/invisible_sample_cache.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_core/internal/logging/log_builder.h"
#include "amsr/someip_binding_core/internal/logging/logger_prefix_generator.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "amsr/someip_binding_xf/internal/events/proxy_event_xf_interface.h"
#include "amsr/someip_binding_xf/internal/events/sample_interpreter_interface.h"
#include "amsr/someip_binding_xf/internal/events/sample_reader_interface.h"
#include "amsr/someip_binding_xf/internal/events/visible_sample_cache.h"
#include "amsr/someip_binding_xf/internal/ser_deser/event_deserializer_interface.h"
#include "amsr/unique_ptr.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace events {

/*!
 * \brief Template for SOME/IP Proxy sample reader.
 * \details A SampleReader reads serialized event samples out of the InvisibleSampleCache and uses a
 *          Deserializer to deserialize those into preallocated Sample slots of the VisibleSampleCache.
 * \tparam SampleType Type of a single event sample.
 */
template <typename SampleType>
class SampleReader : public SampleReaderInterface<SampleType> {
 public:
  /*!
   * \brief Alias for VisibleSampleCache.
   */
  using VisibleSampleContainer = ::amsr::someip_binding_xf::internal::events::VisibleSampleCache<SampleType>;

  /*!
   * \brief Alias for InvisibleSampleCache.
   */
  using SampleCacheContainer =
      typename ::amsr::someip_binding_core::internal::events::InvisibleSampleCache::SampleCacheContainer;

  /*!
   * \brief Type Alias for proxy event backend interface.
   */
  using ProxyEventBackendInterface = ::amsr::someip_binding_xf::internal::events::ProxyEventXfInterface<SampleType>;

  /*!
   * \brief Type Alias for Sample data containing the memory pointer, e2e check status and time stamp.
   */
  using SampleData = typename ProxyEventBackendInterface::SampleData;

  /*!
   * \brief Callable type to be invoked for every deserialized sample.
   */
  using CallableReadSamplesResult = typename ProxyEventBackendInterface::CallableReadSamplesResult;

  /*!
   * \brief Type alias for time stamp type.
   */
  using TimeStamp = ::amsr::someip_protocol::internal::TimeStamp;

  /*!
   * \brief Type alias for EventDeserializerInterface.
   */
  using EventDeserializerInterface = internal::ser_deser::EventDeserializerInterface<SampleType>;

  /*!
   * \brief Type alias for SampleReaderInterface.
   */
  using SampleReaderInterface = ::amsr::someip_binding_xf::internal::events::SampleReaderInterface<SampleType>;

  /*!
   * \brief Constructor
   * \param[in] deserializer Event sample deserializer
   * \param[in] someip_event_identity SOME/IP event identity. Used for logging.
   * \param[in] sample_interpreter Interpreter for message samples.
   *
   * \steady FALSE
   */
  SampleReader(EventDeserializerInterface& deserializer,
               ::amsr::someip_binding_core::internal::SomeIpEventIdentity const& someip_event_identity,
               std::unique_ptr<SampleInterpreterInterface> sample_interpreter)
      : SampleReaderInterface{},
        deserializer_{deserializer},
        // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
        sample_interpreter_{std::move(sample_interpreter)},
        logger_prefix_{someip_binding_core::internal::logging::LoggerPrefixGenerator::GetLoggerPrefix(
            "SampleReader"_sv, someip_event_identity.GetServiceId(), someip_event_identity.GetInstanceId(),
            someip_event_identity.GetMajorVersion(), someip_event_identity.GetEventId())} {}

  SampleReader(SampleReader const&) = delete;
  SampleReader(SampleReader&&) = delete;
  SampleReader& operator=(SampleReader const&) & = delete;
  SampleReader& operator=(SampleReader&&) & = delete;

  /*!
   * \brief       Default destructor.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~SampleReader() noexcept override = default;

  /*!
   * \brief Reads serialized samples from the given sample cache container, deserializes them and calls the provided
   *        callback function.
   *
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   *
   * \pre   -
   * \param[in] visible_sample_cache         shared pointer to the visible sample cache from which free sample slots
   *                                         are retrieved for each read sample
   * \param[in] serialized_samples_container container which has the enqueued event samples cache entries
   * \param[in] max_samples                  maximum number of samples to process
   * \param[in] callable_sample_result       Callable to be invoked on successful deserialization
   *
   * \return The number of event samples which were successfully deserialized and processed
   * \spec
   *   requires visible_sample_cache != nullptr;
   * \endspec
   *
   * \internal
   * - Calculate how many samples shall be processed by using the min of max_samples or the available serialzied
   * samples
   * - Do repeatedly for the serialized_samples in sample_cache_container
   *   - Retrieve one slot from the visible cache
   *   - When a slot is availble
   *     - Deserialize the sample.
   *     - If deserialization is successful
   *       - Increase the number of successfully processed events
   *       - Invoke callable_sample_result with wrapped, deserialized sample, e2e check status
   *         "NotAvailable" and time stamp.
   *     - Otherwise
   *       - Return the visible cache slot.
   *   - Otherwise
   *     - Stop further processing of samples
   * \endinternal
   */
  std::size_t ReadSamples(std::shared_ptr<VisibleSampleContainer> visible_sample_cache,
                          SampleCacheContainer& serialized_samples_container, std::size_t const max_samples,
                          CallableReadSamplesResult const& callable_sample_result) const noexcept override {
    std::size_t nr_valid_events_processed{0};
    std::size_t const samples_to_process{std::min(max_samples, serialized_samples_container.size())};
    // VECTOR NL AutosarC++17_10-A6.5.1: MD_SOMEIPBINDING_AutosarC++17_10-A6.5.1_loop_counter
    for (std::size_t process_index{0U}; process_index < samples_to_process; ++process_index) {
      // Get free slot for deserialization
      // VECTOR NL AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10_A18.5.8_false_positive
      std::shared_ptr<socal::internal::events::MemoryWrapperInterface<SampleType>> visible_cache_slot{
          // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
          visible_sample_cache->GetNextFreeSample()};

      if (visible_cache_slot != nullptr) {
        // Retrieve serialized event
        amsr::UniquePtr<someip_binding_core::internal::events::SomeIpSampleCacheEntry> const& serialized_event{
            serialized_samples_container.front()};

        // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
        ::amsr::core::Optional<TimeStamp> const time_stamp{serialized_event->GetTimeStamp()};

        // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
        std::size_t const buffer_size{serialized_event->GetBufferSize()};
        ::amsr::someip_binding_core::internal::MemoryBuffer::MemoryBufferView const buffer_view{
            // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
            serialized_event->GetBufferView()};
        // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
        bool const deserialized_successfully{DeserializeSample(**visible_cache_slot, buffer_size, buffer_view)};

        serialized_samples_container.pop_front();

        if (deserialized_successfully) {
          ++nr_valid_events_processed;
          // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
          callable_sample_result(SampleData{std::move(visible_cache_slot), visible_sample_cache,
                                            ara::com::E2E_state_machine::E2ECheckStatus::NotAvailable, time_stamp});
        } else {
          // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
          visible_sample_cache->ReturnEntry(std::move(visible_cache_slot));

          logger_.LogError(
              [](::ara::log::LogStream& s) {
                s << "Deserialization error occurred. Please check that the event datatype for proxy and skeleton "
                     "side are compatible.";
              },
              static_cast<char const*>(__func__), __LINE__);
        }
      } else {  // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
        // This is not an error case, we only process until no more free slot is available.
        logger_.LogDebug([](::ara::log::LogStream& s) { s << "No free slot is available anymore."; },
                         static_cast<char const*>(__func__), __LINE__);
        break;
      }
    }  // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT

    return nr_valid_events_processed;
  }

 private:
  /*!
   * \brief       Performs deserialization of event sample payload
   * \param[out]  sample_placeholder Sample placeholder
   * \param[in]   buffer_size Size of the event buffer
   * \param[in]   buffer_view View to the buffer of the received event sample
   * \return      A bool indicating the success of the deserialization
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Gets the payload from the SampleInterpreter that needs to be deserialized.
   * - If the payload is valid
   *    - Calls the deserializer and update the deserialization status
   * - Otherwise
   *    - Just return the default deserialization status (false)
   * \endinternal
   */
  bool DeserializeSample(
      SampleType& sample_placeholder, std::size_t const buffer_size,
      ::amsr::someip_binding_core::internal::MemoryBuffer::MemoryBufferView const& buffer_view) const noexcept {
    ::amsr::someip_protocol::internal::deserialization::BufferView const serialized_packet_buffer_view{
        // VECTOR Next Line AutosarC++17_10-M5.2.8:MD_SOMEIPBINDING_AutosarC++17_10-M5.2.8_conv_from_voidp
        static_cast<std::uint8_t const*>(buffer_view.data()), buffer_size};
    amsr::core::Optional<::amsr::someip_protocol::internal::deserialization::BufferView> const payload_buffer_view{
        // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
        sample_interpreter_->GetPayloadSpan(serialized_packet_buffer_view)};
    bool deserialization_ok{false};
    if (payload_buffer_view.has_value()) {
      // Deserialize Payload
      // VECTOR NL AutosarC++17_10-A18.5.8:MD_SOMEIPBINDING_AutosarC++17_10-A18.5.8_Large_packets_allocated_on_stack
      ::amsr::someip_protocol::internal::deserialization::Reader payload_reader{payload_buffer_view.value()};
      // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
      deserialization_ok = deserializer_.Deserialize(payload_reader, sample_placeholder);
    }
    return deserialization_ok;
  }

  /*!
   * \brief Deserializer used to deserialize incoming samples.
   */
  EventDeserializerInterface& deserializer_;

  /*!
   * \brief Sample interpreter used to interprete the serialized samples.
   */
  std::unique_ptr<SampleInterpreterInterface> sample_interpreter_;

  /*!
   * \brief Logger Prefix.
   */
  ::amsr::core::String const logger_prefix_;

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

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_SAMPLE_READER_H_
