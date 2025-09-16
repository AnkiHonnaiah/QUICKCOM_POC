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
 *        \brief  Defines interface for shared memory provider.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_SHARED_MEMORY_PROVIDER_INTERFACE_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_SHARED_MEMORY_PROVIDER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include <memory>

#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/memory/memory_provider_interface.h"
#include "amsr/memory/memory_types.h"

namespace amsr {
namespace memory {

/*!
 * \brief       Interface for shared memory provider.
 *
 * \details     Shared memory providers allocates shared memory objects.
 *
 * \vprivate    Vector product internal API
 */
class SharedMemoryProviderInterface : public MemoryProviderInterface {
 public:
  /*!
   * \brief       Constructs a SharedMemoryProviderInterface.
   *
   * \steady      TRUE
   *
   * \spec
   *   requires true;
   * \endspec
   *
   * \vprivate    Vector component internal API
   */
  SharedMemoryProviderInterface() noexcept = default;

  /*!
   * \brief       Virtual destructor for interface.
   *
   * \steady      TRUE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ~SharedMemoryProviderInterface() noexcept override = default;

  /*!
   * \brief       Sets the access mode of the SharedMemoryExchangeHandles created from the shared memory objects that
   *              are created from this shared memory provider.
   *
   * \details     This determines if other processes that open the SharedMemoryExchangeHandles can write to the created
   *              memory object or not. Not all OSs may support preventing other processes from getting write access to
   *              the shared memory object. On these OSs, the other process may write to the shared memory object even
   *              if the shared access mode was set to kReadOnly.
   *
   * \param[in]   shared_access_mode    kReadOnly if other processes shall not be able to open
   *                                    SharedMemoryExchangeHandles of the created shared memory object for writing,
   *                                    kReadWrite otherwise.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  void SetSharedAccessMode(AccessMode shared_access_mode) noexcept { shared_access_mode_.emplace(shared_access_mode); }

  /*!
   * \brief       Get the configured shared access mode.
   *
   * \return      Configured shared access mode or empty optional if no shared access mode was configured.
   *
   * \context     ANY
   *
   * \pre         -
   *
   * \reentrant   FALSE
   * \synchronous TRUE
   * \threadsafe  FALSE
   * \steady      TRUE
   *
   * \spec
   *   requires true;
   * \endspec
   */
  auto GetSharedAccessMode() const noexcept -> ::amsr::core::Optional<AccessMode> { return shared_access_mode_; }

 protected:
  /*!
   * \brief       Copy constructs a SharedMemoryProviderInterface.
   */
  SharedMemoryProviderInterface(SharedMemoryProviderInterface const& other) noexcept = default;

  /*!
   * \brief       Move constructs a SharedMemoryProviderInterface.
   */
  SharedMemoryProviderInterface(SharedMemoryProviderInterface&& other) noexcept = default;

  /*!
   * \brief       Copy assigns a SharedMemoryProviderInterface.
   */
  SharedMemoryProviderInterface& operator=(SharedMemoryProviderInterface const& other) & noexcept = default;

  /*!
   * \brief       Move assigns a SharedMemoryProviderInterface.
   */
  SharedMemoryProviderInterface& operator=(SharedMemoryProviderInterface&& other) & noexcept = default;

 private:
  /*!
   * \brief       Configured shared access mode. Optional is empty if the shared access mode was not configured.
   */
  ::amsr::core::Optional<AccessMode> shared_access_mode_{};
};

}  // namespace memory
}  // namespace amsr

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_AMSR_MEMORY_SHARED_MEMORY_PROVIDER_INTERFACE_H_
