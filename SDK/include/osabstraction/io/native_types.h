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
 *        \brief  Operating system specific types and constexpr.
 *
 *      \details  All types and literal values are mapped to operating system specific values.
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_OSABSTRACTION_IO_NATIVE_TYPES_H_
#define LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_OSABSTRACTION_IO_NATIVE_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <sys/uio.h>  // struct iovec
#include <climits>    // IOV_MAX

#include <cstddef>

namespace osabstraction {
namespace io {

/*!
 * \brief    Native type for IO operation on POSIX systems.
 *
 * \details  POSIX systems use the type "int" for socket descriptors and file descriptors.
 *
 * \vprivate pes internal API
 */
using NativeHandle = int;  // VECTOR SL AutosarC++17_10-A3.9.1: MD_OSA_A3.9.1_PosixApi

/*!
 * \brief Invalid file descriptor / socket descriptor.
 */
constexpr NativeHandle kInvalidNativeHandle{-1};

/*!
 * \brief Defines the maximum size of an IO Buffer array.
 *
 * \details
 * Any array that is larger then the size cannot be handled by the operating system.
 */
constexpr std::size_t const kMaxIOBufferArraySize{IOV_MAX};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility_public_constant
/*!
 * \brief Defines the size limit for a single buffer.
 *
 * \details
 * Any buffer that exceeds this limit cannot be handle by a single read/write operation.
 */
constexpr std::size_t const kMaxIOBufferSize{SSIZE_MAX};

}  // namespace io
}  // namespace osabstraction

#endif  // LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_OSABSTRACTION_IO_NATIVE_TYPES_H_
