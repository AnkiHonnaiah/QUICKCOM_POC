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
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  ara/crypto/keys/entry_point.h
 *        \brief  Factory that creates or return existing single instance of the Key Storage Provider.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_KEYS_ENTRY_POINT_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_KEYS_ENTRY_POINT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/keys/key_storage_provider.h"

namespace ara {
namespace crypto {
namespace keys {

/*!
 * \brief Factory that creates or return existing single instance of the Key Storage Provider.
 * \return Shared smart pointer to loaded Key Storage Provider.
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \error SecurityErrorDomain::kRuntimeFault if the Key Storage Provider instance cannot be created.
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
ara::core::Result<KeyStorageProvider::Sptr> LoadKeyStorageProvider() noexcept;

}  // namespace keys
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_KEYS_ENTRY_POINT_H_
