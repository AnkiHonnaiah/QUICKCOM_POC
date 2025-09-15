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
/*!        \file  ara/crypto/x509/entry_point.h
 *        \brief  Factory that creates or return existing single instance of the X.509 Provider.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_X509_ENTRY_POINT_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_X509_ENTRY_POINT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/core/result.h"
#include "x509_provider.h"

namespace ara {
namespace crypto {
namespace x509 {

/*!
 * \brief Factory that creates or return existing single instance of the X.509 Provider.
 * \details X.509 Provider should use the default Crypto Provider for hashing and signature verification!
 *          Therefore when you load the X.509 Provider, in background it loads the default Crypto Provider too.
 * \return shared smart pointer to loaded X.509 Provider
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \error SecurityErrorDomain::kRuntimeFault if the X.509 Provider cannot be loaded.
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
ara::core::Result<X509Provider::Sptr> LoadX509Provider() noexcept;

}  // namespace x509
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_X509_ENTRY_POINT_H_
