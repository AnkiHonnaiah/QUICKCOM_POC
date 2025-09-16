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
/**        \file  someip_binding_xf/internal/events/memory_wrapper.h
 *        \brief  A wrapper object containing memory for a deserialized sample.
 *        \unit   SomeIpBinding::SomeIpBindingXf::Communication::Events::MemoryWrapper
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_MEMORY_WRAPPER_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_MEMORY_WRAPPER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/socal/internal/events/memory_wrapper_interface.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace events {

/*!
 * \brief  Wrapper object holding memory for a deserialized sample.
 * \tparam SampleType Type of a single event sample.
 */
template <typename SampleType>
class MemoryWrapper final : public ::amsr::socal::internal::events::MemoryWrapperInterface<SampleType> {
 public:
  /*!
   * \brief Base sample type.
   */
  using SampleDataType = typename ::amsr::socal::internal::events::MemoryWrapperInterface<SampleType>::SampleDataType;

  /*!
   * \brief       Default constructor.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for different Event class instances.
   * \reentrant   TRUE for different Event class instances.
   * \synchronous TRUE
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  MemoryWrapper() noexcept = default;

  /*!
   * \brief       Default destructor.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for different Event class instances.
   * \reentrant   TRUE for different Event class instances.
   * \synchronous TRUE
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  ~MemoryWrapper() noexcept final = default;  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298

  MemoryWrapper(MemoryWrapper const&) = delete;
  MemoryWrapper& operator=(MemoryWrapper const&) = delete;
  MemoryWrapper(MemoryWrapper&&) = delete;
  MemoryWrapper& operator=(MemoryWrapper&&) = delete;

  /*!
   * \brief       Member access operator -> for accessing the values inside the sample.
   * \return      Pointer to the underlying sample.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for different Event class instances.
   * \reentrant   TRUE for different Event class instances.
   * \synchronous TRUE
   * \steady      TRUE
   * \vprivate    Product Private
   * \spec
   *   requires true;
   * \endspec
   */
  SampleDataType* operator->() noexcept final { return &sample_; }

  /*!
   * \brief       Member indirection operator * for accessing the underlying sample.
   * \return      Reference to the underlying sample.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for different Event class instances.
   * \reentrant   TRUE for different Event class instances.
   * \synchronous TRUE
   * \steady      TRUE
   * \vprivate    Product Private
   * \spec
   *   requires true;
   * \endspec
   */
  SampleDataType& operator*() noexcept final { return sample_; }

 private:
  /*!
   * \brief Default-initialized memory for SampleType.
   */
  SampleDataType sample_{};
};

}  // namespace events
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_EVENTS_MEMORY_WRAPPER_H_
