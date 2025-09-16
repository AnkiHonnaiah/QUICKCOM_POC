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
/*!        \file  sample_reader_interface.h
 *        \brief  Interface for reading samples within proxy event xf
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_SAMPLE_READER_INTERFACE_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_SAMPLE_READER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <memory>

#include "amsr/someip_binding_core/internal/events/invisible_sample_cache.h"
#include "amsr/someip_binding_xf/internal/events/proxy_event_xf_interface.h"
#include "amsr/someip_binding_xf/internal/events/visible_sample_cache.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace events {

/*!
 * \brief Interface for the deserialization of serialized samples into free slots of the visible cache
 * \tparam SampleType Type of a single event sample.
 */
template <typename SampleType>
class SampleReaderInterface {
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
   * \brief Default constructor
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  SampleReaderInterface() = default;

  /*!
   * \brief Virtual destructor
   * \steady FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~SampleReaderInterface() noexcept = default;

  SampleReaderInterface(SampleReaderInterface const&) = delete;
  SampleReaderInterface(SampleReaderInterface&&) = delete;
  SampleReaderInterface& operator=(SampleReaderInterface const&) & = delete;
  SampleReaderInterface& operator=(SampleReaderInterface&&) & = delete;

  /*!
   * \brief ReadSamples will try to deserialize \p max_samples serialized samples from \p serialized_samples_container
   *        into free sample slots from \p visible_sample_cache. For each successful deserialization it will call
   *        \p callable_sample_result.
   * \details  If \p serialized_samples_container contains less samples than \p max_samples, then
   *           all the samples of \p serialized_samples_container will be read.
   *           If one of the samples cannot be deserialized (e.g. payload corrupted), it will be skipped and
   *           \p callable_sample_result will not be triggered.
   *           The reading would stop once \p visible_sample_cache has no more free sample.
   * \param[in] visible_sample_cache         Visible Sample Cache where free sample slots are being retrieved from.
   * \param[in] serialized_samples_container Container holding the serialized samples.
   * \param[in] max_samples                  Number of samples to be read at most.
   * \param[in] callable_sample_result       Callback which is invoked on each successful deserialization.
   * \return The number of sucessfully deserialized samples.
   * \steady TRUE
   * \spec
   *   requires visible_sample_cache != nullptr;
   * \endspec
   */
  virtual std::size_t ReadSamples(std::shared_ptr<VisibleSampleContainer> visible_sample_cache,
                                  SampleCacheContainer& serialized_samples_container, std::size_t const max_samples,
                                  CallableReadSamplesResult const& callable_sample_result) const noexcept = 0;
};

}  // namespace events
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_SAMPLE_READER_INTERFACE_H_
