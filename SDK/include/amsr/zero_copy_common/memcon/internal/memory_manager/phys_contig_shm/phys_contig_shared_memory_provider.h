/*!********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!       \file
 *        \brief Alias to PhysicallyContiguousSharedMemoryProvider by OsAbstraction.
 *        \unit  ZeroCopyCommon::MemCon::MemoryManager
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_MEMORY_MANAGER_PHYS_CONTIG_SHM_PHYS_CONTIG_SHARED_MEMORY_PROVIDER_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_MEMORY_MANAGER_PHYS_CONTIG_SHM_PHYS_CONTIG_SHARED_MEMORY_PROVIDER_H_

#include <memory>

#include "amsr/memory/memory_provider_interface.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace memory_manager {
namespace phys_contig_shm {

/*!
 * \brief       Helper function to get a PhysicallyContiguousSharedMemoryProvider with read-only access for other
 *              processes.
 * \details     Each SharedMemoryExchangeHandle which is created from a Readable- or ReadWritableMemoryInterface
 *              given by this provider will set the remote's process access mode to "read-only". Not all
 *              OSs may support preventing other processes from getting write access to the shared memory object. On
 *              these OSs, the other process may write to the shared memory object even if the shared access mode was
 *              set to "read-only". This is not checked by the implementation.
 * \return      Unique instance of a PhysicallyContiguousSharedMemoryProvider implementing MemoryProviderInterface.
 * \context     ANY
 * \pre         -
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \vprivate    Component private
 * \spec
 *   requires true;
 * \endspec
 */
auto GetMemoryProvider() noexcept -> std::unique_ptr<::amsr::memory::MemoryProviderInterface>;

}  // namespace phys_contig_shm
}  // namespace memory_manager
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_MEMORY_MANAGER_PHYS_CONTIG_SHM_PHYS_CONTIG_SHARED_MEMORY_PROVIDER_H_
