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
/*!        \file  skeleton_event_manager_interface.h
 *        \brief  Interface class for the binding-specific skeleton event manager.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_EVENTS_SKELETON_EVENT_MANAGER_INTERFACE_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_EVENTS_SKELETON_EVENT_MANAGER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "allocated_sample_interface.h"
#include "amsr/unique_ptr.h"
#include "ara/core/result.h"

namespace amsr {
namespace socal {
namespace internal {
namespace events {

/*!
 * \brief Interface class for binding-specific skeleton event manager.
 * \tparam SampleType  Type of the sample (e.g. uint32, ara::core::Vector<T>, struct X).
 */
template <typename SampleType>
class SkeletonEventManagerInterface {
 public:
  /*!
   * \brief AllocatedSampleInterface type.
   */
  using AllocatedSampleInterface = ::amsr::socal::internal::events::AllocatedSampleInterface<SampleType>;

  /*!
   * \brief The uniquely owned allocated sample pointer type.
   */
  using AllocatedSampleUniquePtr = amsr::UniquePtr<AllocatedSampleInterface>;

  /*!
   * \brief Creates a Skeleton Event Manager.
   * \steady FALSE
   */
  SkeletonEventManagerInterface() noexcept = default;
  /*!
   * \brief Define destructor.
   * \steady FALSE
   */
  virtual ~SkeletonEventManagerInterface() noexcept = default;
  /*!
   * \brief Delete copy constructor.
   */
  SkeletonEventManagerInterface(SkeletonEventManagerInterface const&) = delete;
  /*!
   * \brief Delete move constructor.
   */
  SkeletonEventManagerInterface(SkeletonEventManagerInterface&&) = delete;
  /*!
   * \brief Delete copy assignment.
   */
  SkeletonEventManagerInterface& operator=(SkeletonEventManagerInterface const&) & = delete;
  /*!
   * \brief Delete move assignment.
   */
  SkeletonEventManagerInterface& operator=(SkeletonEventManagerInterface&&) & = delete;

  /*!
   * \brief Send an event sample.
   * \param[in] data  Sample value to be transmitted.
   * \return Result with no value if the request was successful.
   * \error ComErrc::kSampleAllocationFailure  If necessary memory for event transmission could not be allocated from
   *                                           the pre-allocate memory pool, otherwise no error shall be returned.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   *              FALSE against other APIs for same class instance.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \steady TRUE
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_PureVirtualFunctionCanBeConst_FalsePositive
  virtual ::ara::core::Result<void> Send(SampleType const& data) noexcept = 0;

  /*!
   * \brief Send an event sample using the preallocated pointer.
   * \param[in] sample_ptr  Sample pointer containing the data to be transmitted. The given pointer must not be null.
   * \return Result with no value if the request was successful.
   * \error ara::com::ComErrc::kCommunicationLinkError   If there was a connection failure.
   * \error ara::com::ComErrc::kCommunicationStackError  If communication stack fails.
   * \error ara::com::ComErrc::kSampleAllocationFailure  If no sample-pointer allocation could take place.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   *              FALSE against other APIs for same class instance.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \steady TRUE
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_PureVirtualFunctionCanBeConst_FalsePositive
  virtual ::ara::core::Result<void> Send(AllocatedSampleUniquePtr sample_ptr) noexcept = 0;

  /*!
   * \brief Allocate event data of unique ownership for sending out.
   * \return Result containing a pointer to the allocated memory. The returned pointer must not be null.
   * \error ara::com::ComErrc::kSampleAllocationFailure  If the sample allocation has failed.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   *              FALSE against other APIs for same class instance.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \steady TRUE
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_PureVirtualFunctionCanBeConst_FalsePositive
  virtual ::ara::core::Result<AllocatedSampleUniquePtr> Allocate() noexcept = 0;

  /*!
   * \brief   Pre-Allocate \p count memory slots to be used when samples are transmitted with Send().
   * \details If this API is called, static memory allocation strategy shall be used, otherwise dynamic memory
   *          allocation strategy will be used.
   * \param[in] count  Number of memory slots to be pre-allocated. Each slot can be used to send one sample. The memory
   *                   is freed once the sample is read by all readers. \p count must be greater than zero.
   * \pre         API has not been called yet.
   * \context     App
   * \threadsafe  FALSE for same class instances, TRUE for different instances.
   *              FALSE against other APIs for same class instance.
   * \reentrant   FALSE for same class instances, FALSE for different instances.
   * \synchronous TRUE
   * \steady FALSE
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_PureVirtualFunctionCanBeConst_FalsePositive
  virtual void PreAllocate(std::size_t const count) noexcept = 0;
};

}  // namespace events
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_EVENTS_SKELETON_EVENT_MANAGER_INTERFACE_H_
