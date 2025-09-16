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
 *        \brief  Abstracts Unix file permissions.
 *
 *********************************************************************************************************************/
#ifndef LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_FILE_PERMISSION_H_
#define LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_FILE_PERMISSION_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <fcntl.h>
#include <cstdint>

namespace amsr {
namespace stream {
namespace filestream {

/*!
 * \brief Enum encoding access rights for user, group and others.
 * \unit amsr::stream::filestream::FilePermission
 * \vprivate Product Private
 */
enum class FileRight : std::uint32_t {
  kNone = 0,
  kExecute = 1,
  kWrite = 2,
  kWriteExec = 3,
  kRead = 4,
  kReadExec = 5,
  kReadWrite = 6,
  kReadWriteExec = 7
};

/*! number of file rights, update whenever FileRight is changed */
static constexpr std::int32_t kFileRightCount{8};

/*!
 * \brief Octal number containing file permission.
 */
using FilePermission = mode_t;

/*!
 * \brief Returns a value representing the file permissions for user, group and others in octal base.
 * \param[in] user File rights of user.
 * \param[in] group File rights of group.
 * \param[in] other File rights of others.
 * \return Combined file rights.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \spec requires true; \endspec
 * \steady UNKNOWN
 * \vprivate Product Private
 *
 * \internal
 * Compute file permission in octal base.
 * \endinternal
 */
constexpr FilePermission GetFilePermission(FileRight user, FileRight group, FileRight other) {
  FilePermission const permission{(static_cast<FilePermission>(user) * 64) + (static_cast<FilePermission>(group) * 8) +
                                  static_cast<FilePermission>(other)};

  return permission;
}

/*!
 * \brief Define for commonly used file rights 0755.
 */
constexpr FilePermission kPermission0755{
    GetFilePermission(FileRight::kReadWriteExec, FileRight::kReadExec, FileRight::kReadExec)};

}  // namespace filestream
}  // namespace stream
}  // namespace amsr

#endif  // LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_FILESTREAM_FILE_PERMISSION_H_
