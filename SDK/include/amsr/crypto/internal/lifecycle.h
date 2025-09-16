/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  libclient/include/amsr/crypto/internal/lifecycle.h
 *        \brief  Implementation of component initialization and deinitialization functions.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCLIENT_INCLUDE_AMSR_CRYPTO_INTERNAL_LIFECYCLE_H_
#define LIB_LIBCLIENT_INCLUDE_AMSR_CRYPTO_INTERNAL_LIFECYCLE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"

namespace amsr {
namespace crypto {
namespace internal {

/*!
 * \brief Initializes the Crypto module for application usage.
 *
 * \trace CREQ-Crypto-Initialization
 * \vprivate Product Private
 */
amsr::core::Result<void> InitializeComponent() noexcept;

/*!
 * \brief Deinitializes the Crypto module during shutdown of a client application.
 * \trace CREQ-Crypto-Initialization
 * \vprivate Product Private
 */
amsr::core::Result<void> DeinitializeComponent() noexcept;

/*!
 * \brief Query the initialization status of the component
 * \return \c true if the component is initialized \c false otherwise
 *
 * \trace CREQ-Crypto-Initialization
 * \vprivate Product Private
 */
bool IsComponentInitialized() noexcept;

}  // namespace internal
}  // namespace crypto
}  // namespace amsr

#endif  // LIB_LIBCLIENT_INCLUDE_AMSR_CRYPTO_INTERNAL_LIFECYCLE_H_
