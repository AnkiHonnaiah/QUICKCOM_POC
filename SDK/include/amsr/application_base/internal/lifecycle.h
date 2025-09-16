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
/**     \file       lifecycle.h
 *      \brief      The Application Base lifecycle.
 *
 *********************************************************************************************************************/
#ifndef LIB_APPLICATION_BASE_INCLUDE_AMSR_APPLICATION_BASE_INTERNAL_LIFECYCLE_H_
#define LIB_APPLICATION_BASE_INCLUDE_AMSR_APPLICATION_BASE_INTERNAL_LIFECYCLE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/application_base/initialization_error_domain.h"
#include "amsr/application_base/internal/app_base.h"

namespace amsr {
namespace application_base {
namespace internal {
/*!
 * \brief     Initializes the ApplicationBase framework for application usage.
 *
 * \return    Nothing on success
 *
 * \error     InitializationErrc::kWrongSequence
 *            InitializeComponent() is called in a wrong sequence (e.g. twice consecutively)
 *
 * \note      Calling any amsr::application_base::internal functions before calling this function
 *            may have unforseen consequences.
 *
 * todo (vseverinsso): Trace CREQ/DSGN feature (AVI-3033).
 *
 * \vprivate  Component private.
 */
inline auto InitializeComponent() noexcept -> Result<void> {
  Result<void> result{};

  if (not detail::GetSingleton().IsInitialized()) {
    detail::GetSingleton().Create();
    static_cast<void>(GetInstance()->Initialize());
  } else {
    result.EmplaceError(MakeErrorCode(
        InitializationErrc::kWrongSequence, {},
        "amsr::application_base::internal::InitializeComponent: ApplicationBase singleton is already initalized."));
  }

  return result;
}
/*!
 * \brief     Deinitializes the ApplicationBase framework
 *
 * \return    Nothing on success
 *
 * \error     InitializationErrc::kWrongSequence
 *            DeinitializeComponent() is called in a wrong sequence (e.g. DeinitializeComponent()
 *            is called prior to InitializeComponent()).
 *
 * \note      Calling any amsr::application_base::internal functions after calling this function
 *            may have unforeseen consequences.
 *
 * todo (vseverinsso): Trace CREQ/DSGN feature (AVI-3033).
 *
 * \vprivate  Component private.
 */
inline auto DeinitializeComponent() noexcept -> Result<void> {
  Result<void> result{};

  if (detail::GetSingleton().IsInitialized()) {
    static_cast<void>(GetInstance()->Deinitialize());
    detail::GetSingleton().Destroy();
  } else {
    result.EmplaceError(MakeErrorCode(
        InitializationErrc::kWrongSequence, {},
        "amsr::application_base::internal::DeinitializeComponent: ApplicationBase singleton is not initalized."));
  }

  return result;
}

/*!
 * \brief     Gets the initialization state of logging.
 *
 * \return    True in case the component is initialized and ready for use.
 *
 * \spec
 *   requires true;
 * \endspec
 *
 * \vprivate  Component private.
 */
inline auto IsComponentInitialized() noexcept -> bool { return detail::GetSingleton().IsInitialized(); }

}  // namespace internal
}  // namespace application_base
}  // namespace amsr

#endif  // LIB_APPLICATION_BASE_INCLUDE_AMSR_APPLICATION_BASE_INTERNAL_LIFECYCLE_H_