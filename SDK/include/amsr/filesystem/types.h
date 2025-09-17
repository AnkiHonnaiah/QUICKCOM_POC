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
/**        \file  libosabstraction-linux/include/amsr/filesystem/types.h
 *        \brief  Declares the filesystem related types.
 *        \unit   osabstraction::filesystem_utils
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_FILESYSTEM_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_FILESYSTEM_TYPES_H_
#define LIB_FILESYSTEM_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_FILESYSTEM_TYPES_H_

#include <cstdint>
#include "amsr/filesystem/path.h"

namespace amsr {
namespace filesystem {
/*!
 * \brief Strong type, internal only
 */
struct PermissionType {
  /*!
   * \brief contains the permission bits
   */
  std::uint32_t val;
};

/*!
 * \brief Strong type for PikeOs and Linux file types as determined by d_type
 */
struct OsFType {
  /*!
   * \brief contains type
   */
  std::uint8_t val;
};

/*!
 * \brief Type of a file system object.
 */
enum class FileType : std::uint8_t { kNone, kRegular, kDirectory, kOther };

class FileStatus;  // Forward declaration

// VECTOR Next Construct Metric-OO.WMC.One: MD_OSA_Metric-OO.WMC.One_Permission
/*!
 * \brief Class to represent the permission of an file system object.
 */
class Permission final {
 public:
  /*!
   * \brief Default constructor.
   * \steady          TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  Permission() noexcept = default;

  /*!
   * \brief Copy constructor.
   * \steady          TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  Permission(Permission const&) noexcept = default;

  /*!
   * \brief Move constructor.
   * \steady          TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  Permission(Permission&&) noexcept = default;

  /*!
   * \brief Copy assignment.
   * \param other The permission to copy.
   * \return Reference to itself.
   * \steady          TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator=(Permission const& other) & noexcept -> Permission& = default;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_OSA_A15.5.1_explicit_noexcept_falsepositive
  /*!
   * \brief Move assignment.
   * \param other The permission to move.
   * \return Reference to itself.
   * \steady          TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator=(Permission&& other) & noexcept -> Permission& = default;

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_OSA_A15.5.1_explicit_noexcept_falsepositive
  /*!
   * \brief Destructor.
   * \steady          TRUE
   * \spec
   *   requires true;
   * \endspec
   */
  ~Permission() noexcept = default;

  /*!
   * \brief Construct a new Permission from a PermissionType.
   *
   * \details     Should not be used by User, only for the constexpr
   *
   * \param       type  Value.
   *
   * \pre         -
   *
   * \context     ANY
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   *
   * \vprivate    Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr explicit Permission(PermissionType type) : permissions_{type.val} {};

  /*!
   * \brief       Returns whether *this is a specific Permission.
   *
   * \details     e.g kPermOwnerRead.
   *
   * \param       rhs  Right side of the comparison.
   *
   * \return      true if correct, false otherwise.
   *
   * \pre         -
   *
   * \context     ANY
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   *
   * \vprivate    Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator==(Permission const& rhs) const noexcept -> bool;

  /*!
   * \brief       Returns whether *this is not a specific Permission.
   *
   * \details     e.g kPermOwnerRead.
   *
   * \param       rhs  Right side of the comparison.
   *
   * \return      true if correct, false otherwise.
   *
   * \pre         -
   *
   * \context     ANY
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   *
   * \vprivate    Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator!=(Permission const& rhs) const noexcept -> bool;

  /*!
   * \brief       Used to set the permissions
   *
   * \param       rhs  Permission to set
   *
   * \return      Reference to changed Permission .
   *
   * \pre         -
   *
   * \context     ANY
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   *
   * \vprivate    Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator|=(Permission const& rhs) & noexcept -> Permission&;
  /*!
   * \brief       Used to set the permissions
   *
   * \param       rhs  Permission to set
   *
   * \return      New changed Permission.
   *
   * \pre         -
   *
   * \context     ANY
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   *
   * \vprivate    Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  auto operator|(Permission const& rhs) const noexcept -> Permission;

  /*!
   * \brief       Returns whether *this contains a specific Permission.
   *
   * \param       perm  Permission to check if contained
   *
   * \return      true if correct, false otherwise
   *
   * \pre         -
   *
   * \context     ANY
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   *
   * \vprivate    Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  auto Contains(Permission const& perm) const noexcept -> bool;

  /*!
   * \brief       Determine Permission
   *
   * \return      true if correct, false otherwise
   *
   * \pre         -
   *
   * \context     ANY
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   *
   * \vprivate    Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  auto IsOwnerReadable() const noexcept -> bool;

  /*!
   * \brief       Determine Permission
   *
   * \return      true if correct, false otherwise
   *
   * \pre         -
   *
   * \context     ANY
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   *
   * \vprivate    Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  auto IsOwnerWritable() const noexcept -> bool;

  /*!
   * \brief       Determine Permission
   *
   * \return      true if correct, false otherwise
   *
   * \pre         -
   *
   * \context     ANY
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   *
   * \vprivate    Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  auto IsOwnerExecutable() const noexcept -> bool;

  /*!
   * \brief       Determine Permission
   *
   * \return      true if correct, false otherwise
   *
   * \pre         -
   *
   * \context     ANY
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   *
   * \vprivate    Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  auto IsGroupReadable() const noexcept -> bool;

  /*!
   * \brief       Determine Permission
   *
   * \return      true if correct, false otherwise
   *
   * \pre         -
   *
   * \context     ANY
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   *
   * \vprivate    Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  auto IsGroupWritable() const noexcept -> bool;

  /*!
   * \brief       Determine Permission
   *
   * \return      true if correct, false otherwise
   *
   * \pre         -
   *
   * \context     ANY
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   *
   * \vprivate    Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  auto IsGroupExecutable() const noexcept -> bool;

  /*!
   * \brief       Determine Permission
   *
   * \return      true if correct, false otherwise
   *
   * \pre         -
   *
   * \context     ANY
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   *
   * \vprivate    Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  auto IsOthersReadable() const noexcept -> bool;

  /*!
   * \brief       Determine Permission
   *
   * \return      true if correct, false otherwise
   *
   * \pre         -
   *
   * \context     ANY
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   *
   * \vprivate    Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  auto IsOthersWritable() const noexcept -> bool;

  /*!
   * \brief       Determine Permission
   *
   * \return      true if correct, false otherwise
   *
   * \pre         -
   *
   * \context     ANY
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   *
   * \vprivate    Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  auto IsOthersExecutable() const noexcept -> bool;

 private:
  /*!
   * \brief the set permissions.
   */
  PermissionType permissions_{};  // None
};

/*!
 * \brief Class to represent the type and permission of a file system object.
 */
class FileStatus final {
 public:
  /*!
   * \brief Construct a new FileStatus from its type and permission.
   *
   * \param       ftype  Type of file system object.
   * \param       perms  Permission of the file system object.
   *
   * \pre         -
   *
   * \context     ANY
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   *
   * \vprivate    Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  FileStatus(FileType ftype, Permission perms) : type_{ftype}, permissions_{perms} {}

  /*!
   * \brief Return the type
   *
   * \return      FileType
   *
   * \pre         -
   *
   * \context     ANY
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   *
   * \vprivate    Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  auto Type() const -> FileType { return type_; }

  /*!
   * \brief Return the permissions
   *
   * \return      Permissions
   *
   * \pre         -
   *
   * \context     ANY
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   *
   * \vprivate    Vector product internal API
   * \spec
   *   requires true;
   * \endspec
   */
  auto Permissions() const -> Permission { return permissions_; }

 private:
  /*!
   * \brief FileType representing the type of the file system object.
   */
  FileType type_{};
  /*!
   * \brief Permission representing the permissions of the file system object.
   */
  Permission permissions_{};
};

// VECTOR NC AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility_public_constant
/*!
 * \brief Constant containing None permission
 */
static constexpr Permission kPermNone{PermissionType{0}};
// VECTOR NC AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility_public_constant
/*!
 * \brief Constant containing Read permission for Owner only
 */
static constexpr Permission kPermOwnerRead{PermissionType{0x100U}};  // Octal: 0400
// VECTOR NC AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility_public_constant
/*!
 * \brief Constant containing Write permission for Owner only
 */
static constexpr Permission kPermOwnerWrite{PermissionType{0x80U}};  // Octal: 0200
// VECTOR NC AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility_public_constant
/*!
 * \brief Constant containing Execute permission for Owner only
 */
static constexpr Permission kPermOwnerExec{PermissionType{0x40U}};  // Octal: 0100
// VECTOR NC AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility_public_constant
/*!
 * \brief Constant containing All permissions for Owner only
 */
static constexpr Permission kPermOwnerAll{PermissionType{0x1C0U}};  // Octal: 0700
// VECTOR NC AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility_public_constant
/*!
 * \brief Constant containing Read permission for Group only
 */
static constexpr Permission kPermGroupRead{PermissionType{0x20U}};  // Octal: 040
// VECTOR NC AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility_public_constant
/*!
 * \brief Constant containing Write permission for Group only
 */
static constexpr Permission kPermGroupWrite{PermissionType{0x10U}};  // Octal : 020
// VECTOR NC AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility_public_constant
/*!
 * \brief Constant containing Execute permission for Group only
 */
static constexpr Permission kPermGroupExec{PermissionType{0x8U}};  // Octal : 010
// VECTOR NC AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility_public_constant
/*!
 * \brief Constant containing Alls permissions for Group only
 */
static constexpr Permission kPermGroupAll{PermissionType{0x38U}};  // Octal : 070
// VECTOR NC AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility_public_constant
/*!
 * \brief Constant containing Read permission for Others only
 */
static constexpr Permission kPermOthersRead{PermissionType{0x4U}};  // Octal : 04
// VECTOR NC AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility_public_constant
/*!
 * \brief Constant containing Write permission for Others only
 */
static constexpr Permission kPermOthersWrite{PermissionType{0x2U}};  // Octal : 02
// VECTOR NC AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility_public_constant
/*!
 * \brief Constant containing Execute permission for Others only
 */
static constexpr Permission kPermOthersExec{PermissionType{0x1U}};  // Octal : 01
// VECTOR NC AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility_public_constant
/*!
 * \brief Constant containing All permissions for Others only
 */
static constexpr Permission kPermOthersAll{PermissionType{0x7U}};  // Octal : 07
// VECTOR NC AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility_public_constant
/*!
 * \brief Constant containing All permissions
 */
static constexpr Permission kPermAll{PermissionType{0x1FFU}};  // Octal : 0777

}  // namespace filesystem
}  // namespace amsr

#endif  // LIB_FILESYSTEM_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_FILESYSTEM_TYPES_H_
