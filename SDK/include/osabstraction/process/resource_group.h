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
/**        \file  libosabstraction-posix/include/osabstraction/process/resource_group.h
 *        \brief  Resource Group implementation for Linux.
 *        \unit osabstraction::Process_Linux
 *        \unit osabstraction::Process_QNX
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_PROCESS_RESOURCE_GROUP_H_
#define LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_PROCESS_RESOURCE_GROUP_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>
#include "amsr/core/result.h"
#include "amsr/core/string.h"

namespace osabstraction {
namespace process {

/*!
 * \brief Resource Group name which is the path to the cgroup to be used on Linux or the name of Adaptive Partition
 *        on QNX.
 */
using ResourceGroupName = ::amsr::core::String;

/*!
 * \brief     ResourceGroup class.
 *
 * \details   This class represents a Resource Group.
 *
 * \vprivate  Vector product internal API
 */
class ResourceGroup final {
 public:
  /*!
   * \brief     Creates a ResourceGroup object from a resource group name.
   *
   * \param[in] path   Path of the resource group/cgroup.
   *
   * \context           InitPhase
   *
   * \pre               -
   *
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \threadsafe        FALSE
   * \steady            TRUE
   *
   * \return    The created resource group.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  static ::amsr::core::Result<ResourceGroup> Create(ResourceGroupName path) noexcept;

  /*!
   * \brief   Returns the name of the resource group
   *
   * \details The name for a cgroup is the path to the cgroup on Linux or the name of the Adaptive Partition on QNX.
   *
   * \context           ANY
   *
   * \pre               -
   *
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \threadsafe        FALSE
   *
   * \return  Resource Group Name.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ResourceGroupName GetName() const noexcept { return path_; }  // VCA_OSA_EXT_CALL_CTOR

  /*!
   * \brief Default copy constructor.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ResourceGroup(ResourceGroup const&) noexcept = default;

  /*!
   * \brief Destructor.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ~ResourceGroup() noexcept = default;  // VCA_OSA_MOLE_1298

  /*!
   * \brief   Default copy operator.
   *
   * \return  This ResourceGroup object.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ResourceGroup& operator=(ResourceGroup const&) & noexcept = default;

  /*!
   * \brief Move constructs a resource group.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ResourceGroup(ResourceGroup&&) noexcept = default;

  /*!
   * \brief   Move assigns a resource group.
   *
   * \return  This ResourceGroup object.
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ResourceGroup& operator=(ResourceGroup&&) & noexcept = default;

 private:
  /*!
   * \brief     Constructs Resource Group object.
   *
   * \param[in] path String that represents name of the resource group.
   *
   * \context           InitPhase
   *
   * \pre               -
   *
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \threadsafe        FALSE
   */
  explicit ResourceGroup(ResourceGroupName&& path) noexcept : path_{std::move(path)} {}

  /*!
   * \brief     Stores the path of the resource group.
   */
  ResourceGroupName path_{};
};

}  // namespace process
}  // namespace osabstraction

#endif  // LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_PROCESS_RESOURCE_GROUP_H_
