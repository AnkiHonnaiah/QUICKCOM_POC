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
/*!        \file  skeleton_method_request_interface.h
 *        \brief  The interface which any skeleton method backend has to implement.
 *
 *      \details  The interface provides access to the arguments of a method.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_SKELETON_METHOD_REQUEST_INTERFACE_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_SKELETON_METHOD_REQUEST_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <tuple>

namespace amsr {
namespace socal {
namespace internal {
namespace methods {

/*!
 * \brief   The interface to obtain the request arguments.
 * \details Method requesters have to provide/create this interface for each request.
 * \tparam ArgsT  The types of method arguments.
 */
template <typename... ArgsT>
class SkeletonMethodRequestInterface {
 public:
  /*!
   * \brief Default constructor.
   * \pre     -
   * \context App
   * \steady TRUE
   */
  SkeletonMethodRequestInterface() noexcept = default;

  /*!
   * \brief Destructor.
   * \pre     -
   * \context App
   * \steady TRUE
   */
  virtual ~SkeletonMethodRequestInterface() noexcept = default;

  SkeletonMethodRequestInterface(SkeletonMethodRequestInterface const&) = delete;

  SkeletonMethodRequestInterface(SkeletonMethodRequestInterface&&) = delete;

  SkeletonMethodRequestInterface& operator=(SkeletonMethodRequestInterface const&) & = delete;

  SkeletonMethodRequestInterface& operator=(SkeletonMethodRequestInterface&&) & = delete;

  /*!
   * \brief   Retrieves request argument(s).
   * \details Arguments should be provided in a form that can be used directly by the method call (e.g. deserialized).
   *          It is intended to be invoked right before the method would be called.
   * \param[out] args  Tuple containing the default-initialized method request arguments.
   * \return True if all the arguments are successfully deserialized, False otherwise.
   * \pre         -
   * \context     Callback | App.
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \steady TRUE
   */
  // VECTOR NL AutosarC++17_10-M7.1.2: MD_SOCAL_AutosarC++17_10-M7.1.2_parameter_as_pointer_or_reference_to_const
  virtual bool GetArgs(std::tuple<ArgsT...>& args) const noexcept = 0;
};

}  // namespace methods
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_SKELETON_METHOD_REQUEST_INTERFACE_H_
