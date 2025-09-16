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
/*!        \file  amsr/crypto/x509/entry_point.h
 *        \brief  Factory that creates instance of the x509 custom extension parser.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_AMSR_CRYPTO_X509_ENTRY_POINT_H_
#define LIB_LIBARA_INCLUDE_AMSR_CRYPTO_X509_ENTRY_POINT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/crypto/x509/x509_custom_extension_parser.h"
#include "ara/core/result.h"

namespace amsr {
namespace crypto {
namespace x509 {

/*!
 * \brief Factory that creates an instance of the X.509 Custom Extension Parser
 * \return shared smart pointer to loaded X.509 Custom Extensions Parser
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \error SecurityErrorDomain::kRuntimeFault if the X.509 Custom Parser cannot be loaded.
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
ara::core::Result<X509CustomExtensionParser::Sptr> LoadX509CustomExtensionParser() noexcept;

}  // namespace x509
}  // namespace crypto
}  // namespace amsr

#endif  // LIB_LIBARA_INCLUDE_AMSR_CRYPTO_X509_ENTRY_POINT_H_
