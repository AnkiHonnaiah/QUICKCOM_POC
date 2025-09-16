/*!********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  entry_point.h
 *        \brief  Indirection to access LoadKeyStorageProvider.
 *
 *********************************************************************************************************************/
#ifndef LIB_EXTERNAL_INCLUDE_AMSR_LIBSECCOM_EXTERNAL_LIBCRYPTO_KEYS_ENTRY_POINT_H_
#define LIB_EXTERNAL_INCLUDE_AMSR_LIBSECCOM_EXTERNAL_LIBCRYPTO_KEYS_ENTRY_POINT_H_

/*!********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/crypto/keys/entry_point.h"

namespace amsr {
namespace libseccom {
namespace external {
namespace libcrypto {

/*!
 * \brief Indirection to access LoadKeyStorageProvider.
 */
// VECTOR NC AutosarC++17_10-M7.3.6: MD_libseccom_M7.3.6_UsingLiterals
using ara::crypto::keys::LoadKeyStorageProvider;

}  // namespace libcrypto
}  // namespace external
}  // namespace libseccom
}  // namespace amsr

#endif  // LIB_EXTERNAL_INCLUDE_AMSR_LIBSECCOM_EXTERNAL_LIBCRYPTO_KEYS_ENTRY_POINT_H_