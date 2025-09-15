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
/*!        \file  allocated_sample_interface.h
 *        \brief  Interface class for accessing the underlying event sample allocated by the bindings on skeleton side.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_EVENTS_ALLOCATED_SAMPLE_INTERFACE_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_EVENTS_ALLOCATED_SAMPLE_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <type_traits>

namespace amsr {
namespace socal {
namespace internal {
namespace events {

/*!
 * \brief Interface class for accessing the underlying event sample allocated by the bindings on skeleton side.
 * \tparam SampleType  Type of the sample (e.g. uint32, ara::core::Vector<T>, struct X).
 */
template <typename SampleType>
class AllocatedSampleInterface {
 public:
  /*!
   * \brief Base sample data type.
   */
  using SampleDataType = std::remove_cv_t<std::remove_reference_t<SampleType>>;

  /*!
   * \brief Default constructor.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for different allocated sample instances.
   * \reentrant   TRUE for different allocated sample instances.
   * \synchronous TRUE
   * \steady TRUE
   */
  AllocatedSampleInterface() noexcept = default;

  /*!
   * \brief Default destructor.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady TRUE
   */
  virtual ~AllocatedSampleInterface() noexcept = default;

  AllocatedSampleInterface(AllocatedSampleInterface const&) = delete;
  AllocatedSampleInterface& operator=(AllocatedSampleInterface const&) = delete;
  AllocatedSampleInterface(AllocatedSampleInterface&&) = delete;
  AllocatedSampleInterface& operator=(AllocatedSampleInterface&&) = delete;

  /*!
   * \brief Indirection operator * for accessing the underlying sample.
   * \return Reference to the underlying sample.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for different allocated sample instances, FALSE otherwise.
   * \reentrant   TRUE for different allocated sample instances, FALSE otherwise.
   * \synchronous TRUE
   * \steady TRUE
   */
  virtual SampleDataType& operator*() noexcept = 0;

  /*!
   * \brief Member access operator -> for accessing the values inside the sample.
   * \return Pointer to the underlying sample. The returned pointer must be valid and not null.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for different allocated sample instances, FALSE otherwise.
   * \reentrant   TRUE for different allocated sample instances, FALSE otherwise.
   * \synchronous TRUE
   * \steady TRUE
   */
  virtual SampleDataType* operator->() noexcept = 0;
};

}  // namespace events
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_EVENTS_ALLOCATED_SAMPLE_INTERFACE_H_
