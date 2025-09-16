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
 *        \brief  Defines a SafeIPC integrity level type.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_INTEGRITY_LEVEL_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_INTEGRITY_LEVEL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>

namespace amsr {
namespace ipc {

/*!
 * \brief   Integrity level of a process.
 *
 * \details The integrity level of an IPC peer process can be queried with SafeIPC.
 */
enum class IntegrityLevel : std::uint32_t {
  /*!
   * \brief   QM integrity level.
   */
  kQm = 0,
  /*!
   * \brief   ASIL A integrity level.
   */
  kAsilA = 1,
  /*!
   * \brief   ASIL B integrity level.
   */
  kAsilB = 2,
  /*!
   * \brief   ASIL C integrity level.
   */
  kAsilC = 3,
  /*!
   * \brief   ASIL D integrity level.
   */
  kAsilD = 4
};

}  // namespace ipc
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_IPC_INTEGRITY_LEVEL_H_
