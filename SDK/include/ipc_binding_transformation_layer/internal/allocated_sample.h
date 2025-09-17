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
/**       \file
 *        \brief  Implements AllocatedSampleInterface to handle the memory allocations for skeleton event transmission.
 *
 *********************************************************************************************************************/

#ifndef LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_ALLOCATED_SAMPLE_H_
#define LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_ALLOCATED_SAMPLE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <type_traits>

#include "amsr/socal/internal/events/allocated_sample_interface.h"

namespace amsr {
namespace ipc_binding_transformation_layer {
namespace internal {

/*!
 * \brief Implements AllocatedSampleInterface to handle the memory allocations for skeleton event transmission.
 *
 * \tparam SampleType  Event sample data type.
 *
 * \unit IpcBinding::IpcBindingTransformationLayer::SkeletonEvent
 */
template <typename SampleType>
class AllocatedSample final : public ::amsr::socal::internal::events::AllocatedSampleInterface<SampleType> {
 public:
  /*!
   * \brief Base sample data type.
   */
  using SampleDataType = std::remove_cv_t<std::remove_reference_t<SampleType>>;

  /*!
   * \brief Default constructor.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for different Event class instances.
   * \reentrant   TRUE for different Event class instances.
   * \synchronous TRUE
   * \steady      FALSE
   */
  AllocatedSample() noexcept = default;

  /*!
   * \brief Default destructor.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for different Event class instances.
   * \reentrant   TRUE for different Event class instances.
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  ~AllocatedSample() noexcept final = default;  // VCA_IPCB_COMPILER_GENERATED_DESTRUCTOR

  AllocatedSample(AllocatedSample const&) noexcept = delete;
  auto operator=(AllocatedSample const&) noexcept -> AllocatedSample& = delete;
  AllocatedSample(AllocatedSample&&) noexcept = delete;
  auto operator=(AllocatedSample&&) noexcept -> AllocatedSample& = delete;

  /*!
   * \brief Indirection operator * for accessing the sample data.
   *
   * \return Reference to the sample data.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for different Event class instances.
   * \reentrant   TRUE for different Event class instances.
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto operator*() noexcept -> SampleDataType& final { return sample_data_; }

  /*!
   * \brief Member access operator -> for accessing the values inside the sample.
   *
   * \return Pointer to the sample data. The returned pointer must be valid and not null.
   *
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for different Event class instances.
   * \reentrant   TRUE for different Event class instances.
   * \synchronous TRUE
   * \steady      FALSE
   *
   * \spec requires true; \endspec
   */
  auto operator->() noexcept -> SampleDataType* final { return &sample_data_; }

 private:
  /*!
   * \brief Sample data.
   */
  SampleDataType sample_data_{};
};

}  // namespace internal
}  // namespace ipc_binding_transformation_layer
}  // namespace amsr

#endif  // LIB_IPC_BINDING_TRANSFORMATION_LAYER_INCLUDE_IPC_BINDING_TRANSFORMATION_LAYER_INTERNAL_ALLOCATED_SAMPLE_H_
