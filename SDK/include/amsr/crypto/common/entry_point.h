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
/*!        \file  amsr/crypto/common/entry_point.h
 *        \brief  Factory that creates or return existing single instance of the ASN.1 parser.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_AMSR_CRYPTO_COMMON_ENTRY_POINT_H_
#define LIB_LIBARA_INCLUDE_AMSR_CRYPTO_COMMON_ENTRY_POINT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/core/result.h"
#include "amsr/crypto/common/asn1_parser.h"

namespace amsr {
namespace crypto {
namespace common {

/*!
 * \brief Factory that creates an instance of the ASN.1 Parser
 * \return shared smart pointer to loaded ASN.1 Parser
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \error SecurityErrorDomain::kRuntimeFault if the ASN.1 Parser cannot be loaded.
 * \reentrant FALSE
 * \synchronous TRUE
 * \vpublic
 */
amsr::core::Result<Asn1Parser::Sptr> LoadAsn1Parser() noexcept;

}  // namespace common
}  // namespace crypto
}  // namespace amsr

#endif  // LIB_LIBARA_INCLUDE_AMSR_CRYPTO_COMMON_ENTRY_POINT_H_
