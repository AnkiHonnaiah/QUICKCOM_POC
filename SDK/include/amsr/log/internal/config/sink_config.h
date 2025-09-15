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
/*!        \file
 *        \brief  Contains all configuration elements that are dependent on a specific log sink.
 *      \details
 *
 *********************************************************************************************************************/

#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_SINK_CONFIG_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_SINK_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/string.h"
#include "amsr/log/internal/config/content_data.h"
#include "amsr/log/internal/map.h"

namespace amsr {
namespace log {
namespace internal {
namespace config {

/*! \brief The set of sink configuration key value pairs */
using SinkConfig = Map<core::String, ContentData>;
/*!
 * \brief   The container for all sink configurations.
 * \details Maps a sink name to its config.
 */
using SinkConfigMap = Map<core::String, SinkConfig>;

}  // namespace config
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_CONFIG_SINK_CONFIG_H_
