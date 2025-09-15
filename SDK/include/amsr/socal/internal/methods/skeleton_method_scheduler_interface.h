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
/*!        \file  skeleton_method_scheduler_interface.h
 *        \brief  The interface which any skeleton method scheduler has to implement.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_SKELETON_METHOD_SCHEDULER_INTERFACE_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_SKELETON_METHOD_SCHEDULER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/socal/internal/task.h"
#include "amsr/unique_ptr.h"

namespace amsr {
namespace socal {
namespace internal {
namespace methods {

/*!
 * \brief The interface which any method scheduler has to implement.
 */
class SkeletonMethodSchedulerInterface {
 public:
  /*!
   * \brief Default constructor.
   * \pre     -
   * \context App
   * \steady FALSE
   */
  SkeletonMethodSchedulerInterface() noexcept = default;
  SkeletonMethodSchedulerInterface(SkeletonMethodSchedulerInterface const&) = delete;
  SkeletonMethodSchedulerInterface(SkeletonMethodSchedulerInterface&&) = delete;
  SkeletonMethodSchedulerInterface& operator=(SkeletonMethodSchedulerInterface const&) & = delete;
  SkeletonMethodSchedulerInterface& operator=(SkeletonMethodSchedulerInterface&&) & = delete;

  /*!
   * \brief Handles the given method request asynchronously.
   * \param[in] request  The method request to execute.
   * \return True if the request is accepted to be executed asynchronously, false otherwise.
   * \pre         -
   * \context     Backend
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances in ThreadDriven runtime processing mode,
   *              TRUE for different instances in Polling runtime processing mode.
   * \synchronous FALSE (Task might run to completion and response be sent before this function returns).
   * \steady TRUE
   */
  virtual bool HandleMethodRequest(amsr::UniquePtr<::amsr::socal::internal::Task>&& request) noexcept = 0;

 protected:
  /*!
   * \brief Destructor.
   * \pre     -
   * \context App
   * \steady FALSE
   */
  ~SkeletonMethodSchedulerInterface() noexcept = default;
};

}  // namespace methods
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_SKELETON_METHOD_SCHEDULER_INTERFACE_H_
