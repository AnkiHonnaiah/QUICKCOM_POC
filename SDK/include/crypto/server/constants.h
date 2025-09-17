// VECTOR SL AutosarC++17_10-A2.9.1: MD_CRYPTO_AutosarC++17_10-A2.9.1
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
/*!        \file  crypto/server/constants.h
 *        \brief  Defines several constants used in the scope of the server.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_CONSTANTS_H_
#define LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_CONSTANTS_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "vac/container/c_string_view.h"
#include "vac/container/string_literals.h"

namespace crypto {
namespace server {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_CRYPTO_AutosarC++17_10-M7.3.6_sv
/*! Introduce string literal for StringView creation */
using vac::container::literals::operator""_sv;

/*! Default path for the daemon configuration file. */
static constexpr amsr::core::StringView const kDefaultDaemonConfigPath{"./etc/daemon_config.json"_sv};

/*! Default path for the access control configuration file. */
static constexpr amsr::core::StringView const kDefaultAccessControlConfigPath{"./etc/access_control.json"_sv};

/*! Default path for the certificate storage root */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_CRYPTO_AutosarC++17_10-M3.4.1_global
static constexpr vac::container::CStringView const kDefaultCertStoreRoot{"."_sv};

}  // namespace server
}  // namespace crypto

#endif  // LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_CONSTANTS_H_
