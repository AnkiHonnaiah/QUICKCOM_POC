/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2018 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file
 *        \brief  Defines function pointers to system functions.
 *
 *********************************************************************************************************************/
#ifndef LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_FCT_POINTERS_H_
#define LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_FCT_POINTERS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <unistd.h>  // for ssize_t
#include <cstdint>

namespace amsr {
namespace stream {
namespace filestream {

// VECTOR NC AutosarC++17_10-A3.9.1: MD_IoStream_A3.9.1_posix_api
/*!
 * References a function to open a file.
 * \vprivate Component Private
 */
using OpenFctPtr = int (*)(char const* path, int oflag, ...);

// VECTOR NC AutosarC++17_10-A3.9.1: MD_IoStream_A3.9.1_posix_api
/*!
 * References a function to close a file.
 * \vprivate Component Private
 */
using CloseFctPtr = int (*)(int file_descriptor);

// VECTOR NC AutosarC++17_10-A3.9.1: MD_IoStream_A3.9.1_posix_api
/*!
 * References a function to manipulate a file descriptor.
 * \vprivate Component Private
 */
using FcntlFctPtr = int (*)(int file_descriptor, int cmd, ...);

// VECTOR NC AutosarC++17_10-A3.9.1: MD_IoStream_A3.9.1_posix_api
// VECTOR NC AutosarC++17_10-M27.0.1: MD_IoStream_M27.0.1_posix_api
/*!
 * References a function to read from a file.
 * \vprivate Component Private
 */
using ReadFctPtr = ssize_t (*)(int file_descriptor, void* buf, size_t nbyte);

// VECTOR NC AutosarC++17_10-A3.9.1: MD_IoStream_A3.9.1_posix_api
// VECTOR NC AutosarC++17_10-M27.0.1: MD_IoStream_M27.0.1_posix_api
/*!
 * References a function to write to a file.
 * \vprivate Component Private
 */
using WriteFctPtr = ssize_t (*)(int file_descriptor, void const* buf, size_t nbyte);

// VECTOR NC AutosarC++17_10-A3.9.1: MD_IoStream_A3.9.1_posix_api
/*!
 * References a function to synchronize a file's in-core state with the storage device.
 * \vprivate Component Private
 */
using FdatasyncFctPtr = int (*)(int file_descriptor);

// VECTOR NC AutosarC++17_10-A3.9.1: MD_IoStream_A3.9.1_posix_api
// VECTOR NC AutosarC++17_10-M27.0.1: MD_IoStream_M27.0.1_posix_api
/*!
 * References a function to reposition a stream.
 * \vprivate Component Private
 */
using LseekFctPtr = off_t (*)(int file_descriptor, off_t offset, int whence);

}  // namespace filestream
}  // namespace stream
}  // namespace amsr

#endif  // LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_FCT_POINTERS_H_
