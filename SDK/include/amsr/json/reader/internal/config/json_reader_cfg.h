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
/**       \file     json_reader_cfg.h
 *        \brief    Configuration data of vaJson.
 *
 *        \details  Contains the application specific configuration.
 *
 *********************************************************************************************************************/

#ifndef LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_CONFIG_JSON_READER_CFG_H_
#define LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_CONFIG_JSON_READER_CFG_H_

// VECTOR Disable AutosarC++17_10-M3.4.1: MD_JSON_AutosarC++17_10-M3.4.1_top_level_constant

namespace amsr {
namespace json {
namespace internal {
namespace config {
/*!
 * \brief Size of the key buffer.
 */
constexpr static std::size_t kKeyBufferSize{1024};

/*!
 * \brief Size of the string buffer.
 */
constexpr static std::size_t kStringBufferSize{1024};
}  // namespace config
}  // namespace internal
}  // namespace json
}  // namespace amsr

// VECTOR Enable AutosarC++17_10-M3.4.1

#endif  // LIB_VAJSON_INCLUDE_AMSR_JSON_READER_INTERNAL_CONFIG_JSON_READER_CFG_H_
