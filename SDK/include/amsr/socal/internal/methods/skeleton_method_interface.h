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
/**        \file  skeleton_method_interface.h
 *        \brief  The interface which any skeleton method has to implement.
 *
+ *      \details  This file describes the interface to be implemented by a skeleton method.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_SKELETON_METHOD_INTERFACE_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_SKELETON_METHOD_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/core/future.h"
#include "amsr/core/result.h"
#include "amsr/socal/internal/methods/skeleton_method_request_interface.h"
#include "amsr/socal/internal/methods/skeleton_response_handler_interface.h"
#include "amsr/unique_ptr.h"

namespace amsr {
namespace socal {
namespace internal {
namespace methods {

/*!
 * \brief The interface which a skeleton method has to implement.
 * \tparam Output  The method output type.
 * \tparam ArgsT   The method arguments' types.
 */
template <typename Output, typename... ArgsT>
class SkeletonMethodInterface {
 public:
  /*!
   * \brief Type alias for RequestorInterface template instantiation.
   */
  using SkeletonMethodRequestInterfaceType = SkeletonMethodRequestInterface<ArgsT...>;

  /*!
   * \brief Type alias for RequestorInterface required argument.
   */
  using SkeletonMethodRequestInterfaceUniquePtr = ::amsr::UniquePtr<SkeletonMethodRequestInterfaceType>;

  /*!
   * \brief Type alias for ResponseHandler template instantiation.
   */
  using SkeletonResponseHandlerInterfaceType = SkeletonResponseHandlerInterface<Output>;

  /*!
   * \brief Type alias for ResponseHandler required argument.
   */
  using SkeletonResponseHandlerInterfaceUniquePtr = ::amsr::UniquePtr<SkeletonResponseHandlerInterfaceType>;

  /*!
   * \brief Default constructor.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  SkeletonMethodInterface() noexcept = default;

  SkeletonMethodInterface(SkeletonMethodInterface const&) = delete;

  SkeletonMethodInterface(SkeletonMethodInterface&&) = delete;

  SkeletonMethodInterface& operator=(SkeletonMethodInterface const&) & = delete;

  SkeletonMethodInterface& operator=(SkeletonMethodInterface&&) & = delete;

  /*!
   * \brief Carry out the method request.
   * \param[in] request           A unique pointer to the request interface to be used to retrieve the arguments.
   * \param[in] response_handler  A unique pointer to the response handler.
   * \pre         Corresponding service must be currently offered.
   * \context     Backend
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous FALSE
   * \steady TRUE
   * \spec requires request != nullptr && response_handler != nullptr; \endspec
   */
  virtual void HandleRequest(SkeletonMethodRequestInterfaceUniquePtr request,
                             SkeletonResponseHandlerInterfaceUniquePtr response_handler) noexcept = 0;

 protected:
  /*!
   * \brief Default destructor.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FASLE
   * \synchronous TRUE
   * \steady FALSE
   */
  ~SkeletonMethodInterface() noexcept = default;
};

}  // namespace methods
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_SKELETON_METHOD_INTERFACE_H_
