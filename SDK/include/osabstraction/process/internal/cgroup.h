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
/**        \file  libosabstraction-linux/include/osabstraction/process/internal/cgroup.h
 *        \brief  Cgroup implementation for Linux.
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_OSABSTRACTION_PROCESS_INTERNAL_CGROUP_H_
#define LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_OSABSTRACTION_PROCESS_INTERNAL_CGROUP_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "amsr/core/array.h"
#include "amsr/core/string.h"
#include "osabstraction/process/process_native_types.h"
#include "osabstraction/process/resource_group.h"

namespace osabstraction {
namespace process {
namespace internal {

// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility
/*!
 * \brief     ResourceGroup class.
 *
 * \details   This class represents a Linux cgroup.
 *
 *
 * \vprivate  Vector product internal API
 */
class Cgroup final {
 public:
  /*!
   * \brief Constructs Cgroup object
   *
   * \param[in] resource_group corresponding Resource Group.
   *
   * \context           InitPhase
   *
   * \pre               -
   *
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \threadsafe        FALSE
   */
  explicit Cgroup(ResourceGroup const& resource_group) : resource_group_{resource_group} {}

  /*!
   * \brief 		Returns true if the the Process is added to the Resource Group
   *
   * \details		If the argument processId is 0 the id of the calling process is used (Linux behaviour)
   *
   * \param[in] process_id   The process ID to be added to the cgroup.
   * \param[out] error_msg error message if the operation fails.
   *
   * \context           ANY
   *
   * \pre               -
   *
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \threadsafe        FALSE
   *
   * \return    True, if adding the process is successful.
   */
  bool AddPid(ProcessId process_id, ::amsr::core::Array<char, 100>& error_msg) const;

 private:
  /*!
   * \brief Prefix for cgroup configuration files.
   */
  ResourceGroupName const cg_config_prefix_{"cgroup."};

  /*!
   * \brief Postfix for cgroup process configuration file.
   */
  ResourceGroupName const cg_config_proc_{cg_config_prefix_ + "procs"};

  /*!
   * \brief The corresponding Resource Group.
   */
  ResourceGroup const& resource_group_;

  /*!
   * \brief     Returns path to process configuration file of the cgroup
   *
   * \context           ANY
   *
   * \pre               -
   *
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \threadsafe        FALSE
   *
   * \return    Path to the file
   */
  ::amsr::core::String GetProcConfigPath() const { return (resource_group_.GetName() + "/" + cg_config_proc_); }
};

}  // namespace internal
}  // namespace process
}  // namespace osabstraction

#endif  // LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_OSABSTRACTION_PROCESS_INTERNAL_CGROUP_H_
