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
/**        \file  skeleton_fire_and_forget_method_interface.h
 *        \brief  The interface which any skeleton fire-and-forget method has to implement.
 *
 *      \details  This file describes the interface to be implemented by a skeleton fire-and-forget method.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_SKELETON_FIRE_AND_FORGET_METHOD_INTERFACE_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_SKELETON_FIRE_AND_FORGET_METHOD_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/socal/internal/methods/skeleton_method_request_interface.h"
#include "amsr/unique_ptr.h"

namespace amsr {
namespace socal {
namespace internal {
namespace methods {

/*!
 * \brief The interface which Skleton fire-and-forget method has to implement.
 * \tparam ArgsT  The method arguments' types.
 */
template <typename... ArgsT>
class SkeletonFireAndForgetMethodInterface {
 public:
  /*!
   * \brief Type alias for RequestorInterface template instantiation.
   */
  using SkeletonMethodRequestInterfaceType = SkeletonMethodRequestInterface<ArgsT...>;

  /*!
   * \brief Type alias for RequestorInterface required argument.
   */
  using SkeletonMethodRequestInterfaceUniquePtr = amsr::UniquePtr<SkeletonMethodRequestInterfaceType>;

  /*!
   * \brief Default constructor.
   * \pre     -
   * \context App
   * \steady FALSE
   */
  SkeletonFireAndForgetMethodInterface() noexcept = default;

  SkeletonFireAndForgetMethodInterface(SkeletonFireAndForgetMethodInterface const&) = delete;

  SkeletonFireAndForgetMethodInterface(SkeletonFireAndForgetMethodInterface&&) = delete;

  SkeletonFireAndForgetMethodInterface& operator=(SkeletonFireAndForgetMethodInterface const&) & = delete;

  SkeletonFireAndForgetMethodInterface& operator=(SkeletonFireAndForgetMethodInterface&&) & = delete;

  /*!
   * \brief   Carry out the method request.
   * \details Method-specific errors will be returned in the result Future.
   * \param[in] request  A unique pointer to the request interface to be used to retrieve the arguments.
   * \pre         Corresponding service must be currently offered.
   * \context     Backend
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous FALSE
   * \steady TRUE
   * \spec requires request != nullptr; \endspec
   */
  virtual void HandleRequest(SkeletonMethodRequestInterfaceUniquePtr request) noexcept = 0;

 protected:
  /*!
   * \brief Destructor.
   * \pre     -
   * \context App
   * \steady FALSE
   */
  ~SkeletonFireAndForgetMethodInterface() noexcept = default;
};

}  // namespace methods
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_SKELETON_FIRE_AND_FORGET_METHOD_INTERFACE_H_
