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
 *        \brief  Define O_CLOEXEC and F_DUPFD_CLOEXEC for PikeOS
 *      \details  The option 'CLOEXEC' is not available in PikeOS, therefore these flags need to be (dummy) defined.
 *
 *********************************************************************************************************************/
#ifndef LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_PIKEOS_H_
#define LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_PIKEOS_H_

namespace amsr {
namespace stream {
namespace filestream {

/*! Dummy definition for O_CLOEXEC as not defined for PikeOS, but needed in FileHandle::Open. */
#define O_CLOEXEC 0x0000

/*! Definition for F_DUPFD_CLOEXEC as not defined for PikeOS, but needed in FileHandle::OpenStdIoHandle. */
#define F_DUPFD_CLOEXEC F_DUPFD

}  // namespace filestream
}  // namespace stream
}  // namespace amsr

#endif  // LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_PIKEOS_H_
