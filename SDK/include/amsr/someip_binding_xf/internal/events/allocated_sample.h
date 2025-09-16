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
/*!        \file  someip_binding_xf/internal/events/allocated_sample.h
 *        \brief  Implements AllocatedSampleInterface to handle the memory allocations for skeleton event transmission.
 *        \unit   SomeIpBinding::SomeIpBindingXf::Communication::Events::SkeletonEventXf
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_ALLOCATED_SAMPLE_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_ALLOCATED_SAMPLE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <type_traits>
#include "amsr/socal/internal/events/allocated_sample_interface.h"
#include "amsr/someip_binding_core/internal/types.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace events {

/*!
 * \brief  Implements AllocatedSampleInterface to handle the memory allocations for skeleton event transmission.
 * \tparam SampleType Event sample data type.
 */
template <typename SampleType>
class AllocatedSample final : public ::amsr::socal::internal::events::AllocatedSampleInterface<SampleType> {
 public:
  /*!
   * \brief Type alias for AllocatedSampleInterface
   */
  using AllocatedSampleInterface = ::amsr::socal::internal::events::AllocatedSampleInterface<SampleType>;

  /*!
   * \brief Base sample data type.
   */
  using SampleDataType = std::remove_cv_t<std::remove_reference_t<SampleType>>;

  /*!
   * \brief Default constructor.
   * \pre -
   * \context App
   * \threadsafe TRUE for different Event class instances.
   * \reentrant TRUE for different Event class instances.
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  AllocatedSample() noexcept : AllocatedSample{nullptr} {}

  /*!
   * \brief     Construct an allocated sample by providing a memory buffer pointer.
   *
   * \details   The provided memory buffer will be used to serialize this allocated
   *            sample data upon send.
   *
   * \param[in] memory_buffer_ptr A unique pointer to the preallocated memory buffer.
   *
   * \pre -
   *
   * \context App
   *
   * \threadsafe  TRUE for different Event class instances.
   * \reentrant   TRUE for different Event class instances.
   * \synchronous TRUE
   *
   * \steady      TRUE
   */
  explicit AllocatedSample(amsr::someip_binding_core::internal::MemoryBufferPtr memory_buffer_ptr)
      : AllocatedSampleInterface{},
        // VCA_SOMEIPBINDING_POSSIBLY_INVALID_ARGUMENT
        memory_buffer_ptr_{std::move(memory_buffer_ptr)} {}

  /*!
   * \brief Default destructor.
   * \pre -
   * \context App
   * \threadsafe TRUE for different Event class instances.
   * \reentrant TRUE for different Event class instances.
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ~AllocatedSample() noexcept final = default;  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298

  AllocatedSample(AllocatedSample const&) = delete;
  AllocatedSample& operator=(AllocatedSample const&) & = delete;
  AllocatedSample(AllocatedSample&&) noexcept = delete;
  AllocatedSample& operator=(AllocatedSample&&) & noexcept = delete;

  /*!
   * \brief Indirection operator * for accessing the sample data.
   * \return Reference to the sample data.
   * \pre -
   * \context App
   * \threadsafe TRUE for different Event class instances.
   * \reentrant TRUE for different Event class instances.
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  SampleDataType& operator*() noexcept final { return sample_data_; }

  /*!
   * \brief Member access operator -> for accessing the values inside the sample.
   * \return Pointer to the sample data. The returned pointer must be valid and not null.
   * \pre -
   * \context App
   * \threadsafe TRUE for different Event class instances.
   * \reentrant TRUE for different Event class instances.
   * \synchronous TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  SampleDataType* operator->() noexcept final { return &sample_data_; }

  /*!
   * \brief  Getter for the preallocated serialization buffer stored in this sample.
   *
   * \return Unique pointer containing the buffer if it has been set and still available
   *         or nullptr otherwise.
   *
   * \context App
   *
   * \threadsafe  TRUE for different Event class instances.
   * \reentrant   TRUE for different Event class instances.
   * \synchronous TRUE
   *
   * \steady      TRUE
   */
  ::amsr::someip_binding_core::internal::MemoryBufferPtr ReleaseBuffer() noexcept {
    return std::move(memory_buffer_ptr_);
  }

 private:
  /*!
   * \brief Sample data.
   */
  SampleDataType sample_data_{};

  /*!
   * \brief    Optional memory buffer pointer
   *
   * \details  If set, this buffer will be used to serialize the sample data
   *           for further sending.
   */
  ::amsr::someip_binding_core::internal::MemoryBufferPtr memory_buffer_ptr_;
};

}  // namespace events
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_ALLOCATED_SAMPLE_H_
