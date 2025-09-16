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
/*!        \file  libclient/include/amsr/crypto/in_process_crypto_provider.h
 *        \brief  Forward declaration of the CreateInProcessCryptoProvider function.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCLIENT_INCLUDE_AMSR_CRYPTO_IN_PROCESS_CRYPTO_PROVIDER_H_
#define LIB_LIBCLIENT_INCLUDE_AMSR_CRYPTO_IN_PROCESS_CRYPTO_PROVIDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/crypto/cryp/crypto_provider.h"

namespace amsr {
namespace crypto {
/*!
 * \brief Create a CryptoProvider for in-process use.
 * \details This function has to be implemented by the library which implements the CryptoProvider.
 * \return Shared pointer to the CryptoProvider intended for in-process use.
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \synchronous TRUE
 * \vprivate Product Private
 */
// VECTOR NL AutosarC++17_10-M3.2.4: MD_CRYPTO_AutosarC++17_10-M3.2.4_type_without_definition_false_positive
auto CreateInProcessCryptoProvider() noexcept -> ara::crypto::cryp::CryptoProvider::Sptr;

}  // namespace crypto
}  // namespace amsr

#endif  // LIB_LIBCLIENT_INCLUDE_AMSR_CRYPTO_IN_PROCESS_CRYPTO_PROVIDER_H_
