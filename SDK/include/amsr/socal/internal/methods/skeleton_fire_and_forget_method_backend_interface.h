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
/**        \file  skeleton_fire_and_forget_method_backend_interface.h
 *        \brief  The interface which skeleton fire-and-forget method transformation layer has to implement.
 *
 *      \details  This file describes the interface to be implemented by a skeleton fire-and-forget method backend.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_SKELETON_FIRE_AND_FORGET_METHOD_BACKEND_INTERFACE_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_SKELETON_FIRE_AND_FORGET_METHOD_BACKEND_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/socal/internal/methods/skeleton_fire_and_forget_method_interface.h"

namespace amsr {
namespace socal {
namespace internal {
namespace methods {

/*!
 * \brief The interface which skeleton fire-and-forget method backend has to implement.
 * \tparam ArgsT  The method arguments' types.
 */
template <typename... ArgsT>
class SkeletonFireAndForgetMethodBackendInterface {
 public:
  /*!
   * \brief Type alias for templated SkeletonFireAndForgetMethodInterface.
   */
  using SkeletonFireAndForgetMethodInterfaceType =
      ::amsr::socal::internal::methods::SkeletonFireAndForgetMethodInterface<ArgsT...>;

  /*!
   * \brief Creates a skeleton fire-and-forget method backend.
   * \pre         -
   * \context     App
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  SkeletonFireAndForgetMethodBackendInterface() noexcept = default;

  SkeletonFireAndForgetMethodBackendInterface(SkeletonFireAndForgetMethodBackendInterface const&) = delete;
  SkeletonFireAndForgetMethodBackendInterface(SkeletonFireAndForgetMethodBackendInterface&&) = delete;
  SkeletonFireAndForgetMethodBackendInterface& operator=(SkeletonFireAndForgetMethodBackendInterface const&) & = delete;
  SkeletonFireAndForgetMethodBackendInterface& operator=(SkeletonFireAndForgetMethodBackendInterface&&) & = delete;

  /*!
   * \brief Registers a skeleton fire-and-forget method to its respective backend.
   * \param[in] skeleton_fnf_method  The skeleton fire-and-forget method frontend.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \steady FALSE
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_PureVirtualFunctionCanBeConst_FalsePositive
  virtual void RegisterSkeletonFireAndForgetMethodInterface(
      SkeletonFireAndForgetMethodInterfaceType& skeleton_fnf_method) noexcept = 0;

  /*!
   * \brief   De-registers a skeleton fire-and-forget method from the method backend.
   * \details After this call, the registered skeleton fire-and-forget method is considered to be invalid.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances Any calls to the registered skeleton f&f
   *              method must be synchronized against this function.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \steady FALSE
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_PureVirtualFunctionCanBeConst_FalsePositive
  virtual void DeregisterSkeletonFireAndForgetMethodInterface() noexcept = 0;

 protected:
  /*!
   * \brief Destroys the skeleton fire-and-forget method backend.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \steady FALSE
   */
  ~SkeletonFireAndForgetMethodBackendInterface() noexcept = default;
};

}  // namespace methods
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_SKELETON_FIRE_AND_FORGET_METHOD_BACKEND_INTERFACE_H_
