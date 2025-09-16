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
/*!        \file  skeleton_response_handler_interface.h
 *        \brief  The interface which a skeleton response handler needs to implement.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_SKELETON_RESPONSE_HANDLER_INTERFACE_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_SKELETON_RESPONSE_HANDLER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/error_code.h"
#include "amsr/core/result.h"

namespace amsr {
namespace socal {
namespace internal {
namespace methods {

/*!
 * \brief The interface which a skeleton response handler needs to implement.
 * \tparam Output  The output data type that will be delivered in the response.
 */
template <typename Output>
class SkeletonResponseHandlerInterface {
 public:
  /*!
   * \brief Default constructor.
   * \pre         -
   * \context     Backend
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady TRUE
   */
  SkeletonResponseHandlerInterface() noexcept = default;

  /*!
   * \brief Default destructor.
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady TRUE
   */
  virtual ~SkeletonResponseHandlerInterface() noexcept = default;

  SkeletonResponseHandlerInterface(SkeletonResponseHandlerInterface const&) = delete;
  SkeletonResponseHandlerInterface(SkeletonResponseHandlerInterface&&) = delete;
  SkeletonResponseHandlerInterface& operator=(SkeletonResponseHandlerInterface const&) & = delete;
  SkeletonResponseHandlerInterface& operator=(SkeletonResponseHandlerInterface&&) & = delete;

  /*!
   * \brief Handles the given method response.
   * \param[in] response  The method response to handle the actual response.
   * \pre         -
   * \context     App | Callback.
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \steady TRUE
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_PureVirtualFunctionCanBeConst_FalsePositive
  virtual void HandleMethodResponse(Output const&& response) noexcept = 0;

  /*!
   * \brief Handles the given method ApApplicationError response.
   * \param[in] error_code  The returned ApApplicationError.
   * \pre         -
   * \context     App | Callback.
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \steady TRUE
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_PureVirtualFunctionCanBeConst_FalsePositive
  virtual void HandleApplicationError(::amsr::core::ErrorCode const& error_code) noexcept = 0;

  /*!
   * \brief   Handles the given method error response.
   * \details The error response might be provided back to the response handler in the Reactor context in case the
   *          method can't be scheduled. Otherwise called in App or Callback context only.
   * \param[in] error_code  Error response with ::ara::com::ComErrc::kMethodArgumentsNotRetrieved if the method input
   *                        arguments could not be retrieved from the request OR ::ara::com::ComErrc::kErrorNotOk if the
   *                        request was not processed (i.e. service was stopped).
   * \pre         -
   * \context     Backend | App | Callback.
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \steady TRUE
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_PureVirtualFunctionCanBeConst_FalsePositive
  virtual void HandleErrorResponse(::amsr::core::ErrorCode const& error_code) noexcept = 0;
};

}  // namespace methods
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_METHODS_SKELETON_RESPONSE_HANDLER_INTERFACE_H_
