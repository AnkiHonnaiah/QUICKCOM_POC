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
/**        \file  skeleton_backend_factory_interface.h
 *        \brief  Factory interface to create the skeleton backends from the Bindings.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_FACTORY_SKELETON_BACKEND_FACTORY_INTERFACE_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_FACTORY_SKELETON_BACKEND_FACTORY_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/core/result.h"

namespace amsr {
namespace socal {
namespace internal {
namespace factory {

/*!
 * \brief Factory interface to create the skeleton backends from the Bindings.
 * \tparam SkeletonBackendInterfaceType  The skeleton backend interface to be created.
 */
template <typename SkeletonBackendInterfaceType>
class SkeletonBackendFactoryInterface {
 public:
  /*!
   * \brief Type alias for the skeleton backend interface pointer.
   */
  using BackendInterfacePtr = std::unique_ptr<SkeletonBackendInterfaceType>;

  /*!
   * \brief Constructs the skeleton backend factory.
   * \pre         -
   * \context     BindingInit
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private.
   * \steady FALSE
   */
  SkeletonBackendFactoryInterface() noexcept = default;

  SkeletonBackendFactoryInterface(SkeletonBackendFactoryInterface const&) = delete;
  SkeletonBackendFactoryInterface(SkeletonBackendFactoryInterface&&) = delete;
  SkeletonBackendFactoryInterface& operator=(SkeletonBackendFactoryInterface const&) & = delete;
  SkeletonBackendFactoryInterface& operator=(SkeletonBackendFactoryInterface&&) & = delete;

  /*!
   * \brief Destroys the skeleton backend factory.
   * \pre         -
   * \context     BindingShutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  virtual ~SkeletonBackendFactoryInterface() noexcept = default;

  /*!
   * \brief   Creates the skeleton backend.
   * \details For each skeleton instance a distinct backend instance shall be created to ensure thread-safety.
   * \return Result containing a pointer to the created backend or an error. The returned pointer must not be null.
   * \error ComErrc::kGrantEnforcementError   If the request is refused by IAM.
   * \error ComErrc::kCommunicationLinkError  If there was a connection failure.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Product Private.
   * \steady FALSE
   * \spec requires true; \endspec
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_PureVirtualFunctionCanBeConst_FalsePositive
  virtual ::amsr::core::Result<BackendInterfacePtr> CreateBackend() noexcept = 0;
};

}  // namespace factory
}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_FACTORY_SKELETON_BACKEND_FACTORY_INTERFACE_H_
