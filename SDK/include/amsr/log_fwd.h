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
/**     \file       log_fwd.h
 *      \brief
 *      \details
 *
 *********************************************************************************************************************/
#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_FWD_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_FWD_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/log/log_level.h"  // IWYU pragma: export
#include "amsr/log/log_mode.h"   // IWYU pragma: export

namespace amsr {
namespace log {
/*! \brief Forward declaration for 'Logger'. */
class Logger;
/*! \brief Forward declaration for 'LogStream'. */
class LogStream;

namespace internal {
/*! \brief Forward declaration for 'LoggerBase'. */
class LoggerBase;

/*! \brief Forward declaration for the logger container. */
class LoggerContainer;

namespace sink {
/*! \brief Forward declaration for 'ILogSink'. */
class ILogSink;
}  // namespace sink
}  // namespace internal
}  // namespace log
}  // namespace amsr
#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_FWD_H_
