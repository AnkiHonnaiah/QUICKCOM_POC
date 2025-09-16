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
/**     \file       ipc_message_type.h
 *      \brief
 *      \details
 *
 *      \unit       LogAndTrace::Ipc::IpcMessageType
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_COMMON_INCLUDE_AMSR_LOG_INTERNAL_MESSAGE_IPC_MESSAGE_TYPE_H_
#define LIB_IPC_COMMON_INCLUDE_AMSR_LOG_INTERNAL_MESSAGE_IPC_MESSAGE_TYPE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

namespace amsr {
namespace log {
namespace internal {
/*!
 * \brief Contains the type of IPC message
 */
enum class IpcMessageType : std::uint8_t {
  kRegisterApp = 0x0,
  kRegisterContext = 0x1,
  kNonVerbose = 0x2,
  kVerbose = 0x3,
  kSetLogLevel = 0x4
};
}  // namespace internal
}  // namespace log
}  // namespace amsr
#endif  // LIB_IPC_COMMON_INCLUDE_AMSR_LOG_INTERNAL_MESSAGE_IPC_MESSAGE_TYPE_H_
