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
/*!        \file  ara/crypto/cryp/entry_point.h
 *        \brief  Factory that creates or returns existing single instance of specific Crypto Provider.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_ENTRY_POINT_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_ENTRY_POINT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/cryp/crypto_provider.h"

namespace ara {
namespace crypto {
namespace cryp {

/*!
 * \brief Factory that creates or return existing single instance of specific Crypto Provider.
 * \details If (provider_uid == nullptr) then platform default provider should be loaded.
 * \param[in] provider_uid A globally unique identifier of required Crypto Provider.
 * \return Shared smart pointer to loaded Crypto Provider.
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \error SecurityErrorDomain::kRuntimeFault If requested Crypto Provider cannot be loaded.
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
ara::core::Result<CryptoProvider::Sptr> LoadCryptoProvider(CryptoProviderUid const* provider_uid = nullptr) noexcept;

}  // namespace cryp
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_CRYP_ENTRY_POINT_H_
