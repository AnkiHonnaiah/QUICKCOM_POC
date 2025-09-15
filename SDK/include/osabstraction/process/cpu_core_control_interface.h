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
/*!        \file  libosabstraction-posix/include/osabstraction/process/cpu_core_control_interface.h
 *        \brief  Data types to work with CPU affinity of a process.
 *        \unit osabstraction::Process_Linux
 *        \unit osabstraction::Process_QNX
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_PROCESS_CPU_CORE_CONTROL_INTERFACE_H_
#define LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_PROCESS_CPU_CORE_CONTROL_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <bitset>
#include <cstdint>

namespace osabstraction {
namespace process {

/*!
 * \brief   Data type for a core ID.
 *
 * \details Identifies a CPU core by a unique number. IDs are 0 for the first core, 1 for the second core, ...
 */
using CoreID = std::uint8_t;

/*!
 * \brief Number of supported CPU cores.
 */
constexpr std::uint32_t const kMaxNumberOfCpus{256};

/*!
 * \brief   A selection of CPU cores.
 *
 * \details Each bit of the mask stores the information if the respective core is selected (bit set) or not (bit clear).
 *          The bits are assigned to the CPU cores via the ID numbers of the cores, the bit 0x1 of the mask represents
 *          the CPU core with the ID 0, the bit 0x2 of the mask represents the CPU core with the ID 1, ...
 *
 * \trace   DSGN-Osab-Process-ProcessCpuAffinity
 */
using CpuAffinity = std::bitset<kMaxNumberOfCpus>;

/*!
 * \brief           Checks whether the CPU affinity is valid for the system (at least one configured CPU exists on the
 *                  system).
 *
 * \note            Empty CPU affinity is considered valid.
 *
 * \param[in]       cpu_affinity   The CPU affinity to check.
 *
 * \return          true if in non-empty CPU affinity set there is at least one set CPU thta exists in the system,
 *                  false otherwise.
 *
 * \context         ANY
 *
 * \pre             -
 *
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \threadsafe      FALSE
 * \steady          TRUE
 *
 * \vprivate        Vector component internal API
 *
 * \spec
 *   requires true;
 * \endspec
 */
bool IsCpuAffinityValid(CpuAffinity const& cpu_affinity) noexcept;

}  // namespace process
}  // namespace osabstraction

#endif  // LIB_COMMON_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_PROCESS_CPU_CORE_CONTROL_INTERFACE_H_
