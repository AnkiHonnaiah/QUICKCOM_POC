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
/**     \file
 *      \brief      Contains forward declarations for message data types.
 *      \details
 *
 *********************************************************************************************************************/
#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_TYPES_TYPES_FWD_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_TYPES_TYPES_FWD_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
namespace amsr {
namespace log {
namespace internal {
/*! \brief Forward declaration for messages. */
class Message;
/*!\brief Forward declaration for meta data. */
class MetaData;

namespace config {
/*! \brief Forward declaration for the logging config. */
struct LoggingConfig;
}  // namespace config
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_TYPES_TYPES_FWD_H_
