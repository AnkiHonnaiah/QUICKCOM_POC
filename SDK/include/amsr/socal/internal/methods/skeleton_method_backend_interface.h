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
/**        \file  skeleton_method_backend_interface.h
 *        \brief  The interface which skeleton method transformation layer has to implement.
 *
 *      \details  This file describes the interface to be implemented by skeleton method transformation layer.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_SKELETON_METHOD_BACKEND_INTERFACE_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_SKELETON_METHOD_BACKEND_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "amsr/socal/internal/methods/skeleton_method_interface.h"

namespace amsr {
namespace socal {
namespace internal {
namespace methods {

/*!
 * \brief The interface which skeleton method backend has to implement.
 * \tparam Output  The method output type.
 * \tparam ArgsT   The method arguments' types.
 */
template <typename Output, typename... ArgsT>
class SkeletonMethodBackendInterface {
 public:
  /*!
   * \brief Type alias for templated SkeletonMethodInterface.
   */
  using SkeletonMethodInterfaceType = ::amsr::socal::internal::methods::SkeletonMethodInterface<Output, ArgsT...>;

  /*!
   * \brief Creates a skeleton method backend.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  SkeletonMethodBackendInterface() noexcept = default;

  SkeletonMethodBackendInterface(SkeletonMethodBackendInterface const&) = delete;

  SkeletonMethodBackendInterface(SkeletonMethodBackendInterface&&) = delete;

  SkeletonMethodBackendInterface& operator=(SkeletonMethodBackendInterface const&) & = delete;

  SkeletonMethodBackendInterface& operator=(SkeletonMethodBackendInterface&&) & = delete;

  /*!
   * \brief Registers a SkeletonMethodInterface to its respective backend.
   * \param[in] skeleton_method  A reference to the skeleton method frontend interface.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \steady FALSE
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_PureVirtualFunctionCanBeConst_FalsePositive
  virtual void RegisterSkeletonMethodInterface(SkeletonMethodInterfaceType& skeleton_method) noexcept = 0;

  /*!
   * \brief   De-registers a skeleton method from the method backend.
   * \details After this call, the registered reference to the skeleton method is considered to be invalid.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances Any calls to the registered skeleton
   *              method must be synchronized against this function.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \steady FALSE
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_PureVirtualFunctionCanBeConst_FalsePositive
  virtual void DeregisterSkeletonMethodInterface() noexcept = 0;

 protected:
  /*!
   * \brief Destroys the skeleton method backend.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, FALSE for different instances.
   * \synchronous TRUE
   * \steady FALSE
   */
  ~SkeletonMethodBackendInterface() noexcept = default;
};

}  // namespace methods
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_SKELETON_METHOD_BACKEND_INTERFACE_H_
