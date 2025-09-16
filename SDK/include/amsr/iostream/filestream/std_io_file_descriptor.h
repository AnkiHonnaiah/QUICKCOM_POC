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
/*!        \file
 *        \brief  Provide enum for standard IO file descriptor.
 *
 *********************************************************************************************************************/
#ifndef LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_STD_IO_FILE_DESCRIPTOR_H_
#define LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_STD_IO_FILE_DESCRIPTOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <unistd.h>

namespace amsr {
namespace stream {
namespace filestream {

/*!
 * \brief Define for type of Posix file handles.
 * \vprivate Component Private
 */
using PosixStdFileHandleType = int;  // VECTOR SL AutosarC++17_10-A3.9.1: MD_IoStream_A3.9.1_posix_api

/*!
 * \brief Enum encoding access to standard I/O file handles.
 * \vprivate Component Private
 */
enum class StdIoFileDescriptor : PosixStdFileHandleType { StdOut = STDOUT_FILENO, StdErr = STDERR_FILENO };

}  // namespace filestream
}  // namespace stream
}  // namespace amsr

#endif  // LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_STD_IO_FILE_DESCRIPTOR_H_
