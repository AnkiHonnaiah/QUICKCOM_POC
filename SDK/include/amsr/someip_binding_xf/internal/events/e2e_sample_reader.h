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
/*!        \file  e2e_sample_reader.h
 *        \brief  Deserializes event samples which are e2e protected
 *         \unit  SomeIpBinding::SomeIpBindingXf::Communication::Events::E2eSampleReader
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_E2E_SAMPLE_READER_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_E2E_SAMPLE_READER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include <memory>
#include "amsr/someip_binding_core/internal/events/invisible_sample_cache.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_core/internal/logging/log_builder.h"
#include "amsr/someip_binding_core/internal/logging/logger_prefix_generator.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "amsr/someip_binding_xf/internal/e2e/client_side_transformer_interface.h"
#include "amsr/someip_binding_xf/internal/events/e2e_sample_interpreter_interface.h"
#include "amsr/someip_binding_xf/internal/events/proxy_event_xf_interface.h"
#include "amsr/someip_binding_xf/internal/events/sample_reader_interface.h"
#include "amsr/someip_binding_xf/internal/events/thread_safe_e2e_result.h"
#include "amsr/someip_binding_xf/internal/events/visible_sample_cache.h"
#include "amsr/someip_binding_xf/internal/ser_deser/event_deserializer_interface.h"
#include "amsr/unique_ptr.h"
#include "someip-protocol/internal/deserialization/reader.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace events {

/*!
 * \brief Template for SOME/IP Proxy sample reader.
 * \details An E2eSampleReader reads serialized event samples out of the InvisibleSampleCache and uses an
 *          EventDeserializer and an E2ETransformer to deserialize those into preallocated Sample slots of the
 *          VisibleSampleCache.
 * \tparam SampleType Type of a single event sample.
 */
template <typename SampleType>
class E2eSampleReader : public SampleReaderInterface<SampleType> {
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
   * \brief Type alias for E2E Transformer
   */
  using E2eTransformer = ::amsr::someip_binding_xf::internal::e2e::ClientSideTransformerInterface;

  /*!
   * \brief Reads out primitive data types from a buffer view.
   */
  using Reader = ::amsr::someip_protocol::internal::deserialization::Reader;

  /*!
   * \brief Type alias for size type.
   */
  using SizeType = ::amsr::someip_protocol::internal::deserialization::SizeType;

  /*!
   * \brief Type alias for SampleReaderInterface.
   */
  using SampleReaderInterface = ::amsr::someip_binding_xf::internal::events::SampleReaderInterface<SampleType>;

  /*!
   * \brief Constructor.
   * \param[in] deserializer           Event sample deserializer
   * \param[in] e2e_transformer        E2E transformer for e2e check
   * \param[in] e2e_result             Reference to a e2e result which is shared between units
   * \param[in] is_e2e_check_disabled  Disables e2e check if true
   * \param[in] someip_event_identity  SOME/IP event identity. Used for logging.
   * \param[in] e2e_sample_interpreter Interpreter for e2e message samples.
   *
   * \steady FALSE
   */
  E2eSampleReader(EventDeserializerInterface& deserializer, E2eTransformer& e2e_transformer,
                  ThreadSafeE2eResult& e2e_result, bool const is_e2e_check_disabled,
                  ::amsr::someip_binding_core::internal::SomeIpEventIdentity const& someip_event_identity,
                  std::unique_ptr<E2eSampleInterpreterInterface> e2e_sample_interpreter)
      : SampleReaderInterface{},
        deserializer_{deserializer},
        e2e_transformer_{e2e_transformer},
        e2e_result_{e2e_result},
        is_e2e_check_disabled_globally_{is_e2e_check_disabled},
        // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
        e2e_sample_interpreter_{std::move(e2e_sample_interpreter)},
        logger_prefix_{someip_binding_core::internal::logging::LoggerPrefixGenerator::GetLoggerPrefix(
            "E2eSampleReader"_sv, someip_event_identity.GetServiceId(), someip_event_identity.GetInstanceId(),
            someip_event_identity.GetMajorVersion(), someip_event_identity.GetEventId())} {}

  /*!
   * \brief       Default destructor.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \vprivate
   * \synchronous TRUE
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~E2eSampleReader() noexcept override = default;

  E2eSampleReader(E2eSampleReader const&) = delete;
  E2eSampleReader(E2eSampleReader&&) = delete;
  E2eSampleReader& operator=(E2eSampleReader const&) & = delete;
  E2eSampleReader& operator=(E2eSampleReader&&) & = delete;

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
   * \return The number of event samples for which the provided callback is called
   * \spec
   *   requires visible_sample_cache != nullptr;
   * \endspec
   *
   * \internal
   * - If serialized_samples_container has at least one samples
   *   - Calculate how many samples shall be processed by using the min of max_samples or the available serialized
   *     samples
   *   - Do repeatedly for the serialized_samples in sample_cache_container
   *     - Retrieve one slot from the visible cache
   *     - When a slot is available
   *       - Perform e2e check on the sample
   *       - If the check is successful
   *         - Deserialize the sample
   *         - If deserialization is successful
   *           - Increase the number of calls to the callback
   *           - Invoke callable_sample_result with wrapped, deserialized sample, the e2e check status and time stamp
   *         - Otherwise but when an e2e error is reported
   *           - Return the visible cache slot
   *           - Increase the number of calls to the callback
   *           - Invoke callable_sample_result with a nullptr, the e2e check status and time stamp
   *       - Otherwise
   *         - Return the visible cache slot
   *     - Otherwise
   *       - Stop further processing of samples
   * - Otherwise
   *   - Reset the e2e result to the value returned by the e2e transformer on an empty buffer
   * \endinternal
   */
  std::size_t ReadSamples(std::shared_ptr<VisibleSampleContainer> visible_sample_cache,
                          SampleCacheContainer& serialized_samples_container, std::size_t const max_samples,
                          CallableReadSamplesResult const& callable_sample_result) const noexcept override {
    std::size_t nr_callback_calls{0};

    if (!serialized_samples_container.empty()) {
      std::size_t const samples_to_process{std::min(max_samples, serialized_samples_container.size())};
      // VECTOR NL AutosarC++17_10-A6.5.1: MD_SOMEIPBINDING_AutosarC++17_10-A6.5.1_loop_counter
      for (std::size_t process_index{0U}; process_index < samples_to_process; ++process_index) {
        // Get free slot for deserialization
        // VECTOR NL AutosarC++17_10-A18.5.8: MD_SOMEIPBINDING_AutosarC++17_10_A18.5.8_false_positive
        std::shared_ptr<socal::internal::events::MemoryWrapperInterface<SampleType>> visible_cache_slot{
            // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
            visible_sample_cache->GetNextFreeSample()};

        if (visible_cache_slot != nullptr) {
          // Retrieve serialized event
          amsr::UniquePtr<someip_binding_core::internal::events::SomeIpSampleCacheEntry> const& serialized_event{
              serialized_samples_container.front()};

          // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
          ::amsr::core::Optional<TimeStamp> const time_stamp{serialized_event->GetTimeStamp()};

          // Do E2E checks & deserialize payload
          ::amsr::someip_binding_core::internal::MemoryBuffer::MemoryBufferView const buffer_view{
              // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
              serialized_event->GetBufferView()};
          ::amsr::e2e::Result e2e_result{ara::com::E2E_state_machine::E2EState::NoData,
                                         ara::com::E2E_state_machine::E2ECheckStatus::NotAvailable};

          bool const deserialized_successfully{
              // VCA_SOMEIPBINDING_ACCESSING_MEMBERS_OF_REFERENCE_CLASS_ATTRIBUTES
              DeserializeE2ESample(**visible_cache_slot, serialized_event->GetBufferSize(), buffer_view, e2e_result)};

          e2e_result_ = e2e_result;

          serialized_samples_container.pop_front();

          if (deserialized_successfully) {
            ++nr_callback_calls;
            // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
            callable_sample_result(typename ProxyEventBackendInterface::SampleData{
                // VCA_SOMEIPBINDING_PASSING_REFERENCE
                std::move(visible_cache_slot), visible_sample_cache, e2e_result.GetCheckStatus(), time_stamp});
          } else if (e2e_result.GetCheckStatus() == ::amsr::e2e::state_machine::CheckStatus::Error) {
            // As deserialization failed, we do not keep the slot in the visible sample cache,
            // Therefore we return the slot in this case, although the callable_sample_result is called.
            // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
            visible_sample_cache->ReturnEntry(std::move(visible_cache_slot));

            ++nr_callback_calls;

            // When an e2e error is detected, which is only possible when is_e2e_protected_ == true,
            // we provide a nullptr sample to the callable_sample_result.
            // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
            callable_sample_result(typename ProxyEventBackendInterface::SampleData{
                // VCA_SOMEIPBINDING_POSSIBLY_INVALID_ARGUMENT
                nullptr, visible_sample_cache, e2e_result.GetCheckStatus(), time_stamp});
          } else {
            // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
            visible_sample_cache->ReturnEntry(std::move(visible_cache_slot));

            logger_.LogError(
                [](::ara::log::LogStream& s) {
                  s << "Deserialization error occurred. Please check that the event datatypes for proxy and skeleton "
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
    } else {
      // No new serialized samples
      // [SWS_CM_90415] E2ECheck shall be invoked, providing to it a null sample.
      // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
      ::amsr::e2e::Result const e2e_result{e2e_transformer_.Check(::amsr::core::Span<std::uint8_t const>{})};

      // [SWS_CM_90417] Update shall store the pair sample and e2eCheckStatus in the application cache
      // and it shall update/overwrite event.e2eState with e2eResult.e2eState.
      e2e_result_ = e2e_result;

      logger_.LogDebug([](::ara::log::LogStream& s) { s << "No new sample was received."; },
                       static_cast<char const*>(__func__), __LINE__);
    }

    return nr_callback_calls;
  }

 private:
  /*!
   * \brief       Performs deserialization of event sample payload with an E2E protected region.
   * \param[out]  sample_placeholder Sample placeholder
   * \param[in]   buffer_size        Size of the event buffer
   * \param[in]   buffer_view        View to the buffer of the received event sample
   * \param[in, out] e2e_result      E2E result to write to.
   * \return      A bool indicating the success of the deserialization
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \trace SPEC-13650588
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Gets the e2e protected span from the SampleInterpreter that needs to be checked.
   * - If the e2e region is valid
   *    - SampleInterpreter checks if the e2e update bit is valid and set
   *    - If the e2e update has a value (valid case)
   *      - Checks if the e2e update bit is set (true) AND checks if the e2e check not is globally disabled AND
   *             Execute e2e check via e2e transformer
   *          - Checks if the e2e status is not an error
   *              Calls DeserializeSample
   *      - Otherwise
   *          Calls DeserializeSample
   *    - Otherwise
   *       Sets deserialized_successfully directly on false
   * - Otherwise
   *    - Notify the e2e transformer that a invalid sample was received
   * returns the deserialization status
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A8.4.4: MD_SOMEIPBINDING_A8.4.4_useReturnValueInsteadOfOutputParameter
  bool DeserializeE2ESample(SampleType& sample_placeholder, std::size_t const buffer_size,
                            ::amsr::someip_binding_core::internal::MemoryBuffer::MemoryBufferView const& buffer_view,
                            ::amsr::e2e::Result& e2e_result) const noexcept {
    bool deserialized_successfully{false};
    // VECTOR Next Line AutosarC++17_10-M5.2.8:MD_SOMEIPBINDING_AutosarC++17_10-M5.2.8_conv_from_voidp
    ::amsr::core::Span<::std::uint8_t> const message{static_cast<::std::uint8_t*>(buffer_view.data()), buffer_size};
    ::amsr::core::Optional<::amsr::core::Span<::std::uint8_t const>> const e2e_region{
        // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
        e2e_sample_interpreter_->GetE2eProtectedSpan(message)};
    if (e2e_region.has_value()) {
      ::amsr::core::Optional<bool> const is_e2e_check_enabled_for_message{
          // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
          e2e_sample_interpreter_->IsE2eCheckEnabled(message)};
      if (is_e2e_check_enabled_for_message.has_value()) {
        if ((is_e2e_check_enabled_for_message.value()) && (!is_e2e_check_disabled_globally_)) {
          e2e_result =
              // VCA_SOMEIPBINDING_POSSIBLY_ACCESSING_NULLPTR_RETRIEVED_FROM_EXTERNAL_FUNCTION
              e2e_transformer_.Check(e2e_region.value());

          // Deserialize if E2E check passes
          if (e2e_result.GetCheckStatus() != ::amsr::e2e::state_machine::CheckStatus::Error) {
            deserialized_successfully = DeserializeSample(sample_placeholder, buffer_size, buffer_view);
          }
        } else {
          // Skipped E2E check (e.g. when updateBit is set to false or E2eCheck is disabled globally)
          deserialized_successfully = DeserializeSample(sample_placeholder, buffer_size, buffer_view);
        }
      } else {
        // Handle as deserialization error
        deserialized_successfully = false;
      }
    } else {
      // Invalid sample received
      // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
      e2e_result = e2e_transformer_.NotifyInvalidSample();

      logger_.LogError([](::ara::log::LogStream& s) { s << "Invalid sample was received."; },
                       static_cast<char const*>(__func__), __LINE__);
    }
    return deserialized_successfully;
  }

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

    // Skip the headers
    ::amsr::core::Optional<::amsr::core::Span<::std::uint8_t const>> const payload_buffer_view{
        // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
        e2e_sample_interpreter_->GetPayloadSpan(serialized_packet_buffer_view)};

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
   * \brief Unique pointer to E2E Transformer.
   */
  E2eTransformer& e2e_transformer_;

  /*!
   * \brief Reference to e2e result
   */
  ThreadSafeE2eResult& e2e_result_;

  /*!
   * \brief E2e check disabled flag.
   *        true if E2E check is disabled, false otherwise.
   *        If true, E2e header shall be skipped and no CRC check shall be done.
   */
  bool const is_e2e_check_disabled_globally_;

  /*!
   * \brief Sample interpreter used to interpret the serialized samples.
   */
  std::unique_ptr<E2eSampleInterpreterInterface> e2e_sample_interpreter_;

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

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_E2E_SAMPLE_READER_H_
