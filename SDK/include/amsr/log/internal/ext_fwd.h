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
 *      \brief      Contains forward declarations from used libraries.
 *      \details
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_EXT_FWD_H_
#define LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_EXT_FWD_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
namespace ara {
namespace core {
/*!
 * \brief Forward declaration for error code.
 */
class ErrorCode;
}  // namespace core
}  // namespace ara

namespace amsr {
namespace stream {
/*!
 * \brief Forward declaration for stateful output streams.
 */
class StatefulOutputStream;
}  // namespace stream
}  // namespace amsr

namespace osabstraction {
namespace io {
struct ConstIOBuffer;
}
}  // namespace osabstraction

#endif  // LIB_COMMON_INCLUDE_AMSR_LOG_INTERNAL_EXT_FWD_H_
