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
 *        \brief  Implementation of GenericMemoryWrapperFactory.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_GENERIC_LAYER_INCLUDE_IPC_BINDING_GENERIC_LAYER_INTERNAL_MEMORY_WRAPPER_GENERIC_MEMORY_WRAPPER_FACTORY_INTERFACE_H_
#define LIB_IPC_BINDING_GENERIC_LAYER_INCLUDE_IPC_BINDING_GENERIC_LAYER_INTERNAL_MEMORY_WRAPPER_GENERIC_MEMORY_WRAPPER_FACTORY_INTERFACE_H_

#include <memory>

#include "ipc_binding_generic_layer/internal/memory_wrapper/generic_memory_wrapper.h"

namespace amsr {
namespace ipc_binding_generic_layer {
namespace internal {

/*!
 * \brief Interface for GenericMemoryWrapperFactory.
 *
 * \unit IpcBinding::IpcBindingGenericLayer::GenericProxyEvent
 */
class AMSR_LG_BETA GenericMemoryWrapperFactoryInterface {
 public:
  /*!
   * \brief Default constructor.
   *
   * \pre        -
   * \context    App
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady     FALSE
   */
  GenericMemoryWrapperFactoryInterface() noexcept = default;

  /*!
   * \brief Default destructor.
   *
   * \pre        -
   * \context    App
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady     FALSE
   */
  virtual ~GenericMemoryWrapperFactoryInterface() noexcept = default;

  GenericMemoryWrapperFactoryInterface(GenericMemoryWrapperFactoryInterface const&) = delete;
  GenericMemoryWrapperFactoryInterface(GenericMemoryWrapperFactoryInterface&&) = delete;
  auto operator=(GenericMemoryWrapperFactoryInterface const&) -> GenericMemoryWrapperFactoryInterface& = delete;
  auto operator=(GenericMemoryWrapperFactoryInterface&&) -> GenericMemoryWrapperFactoryInterface& = delete;

  /*!
   * \brief Creates a GenericMemoryWrapper instance and returns it.
   *
   * \return Unique pointer to GenericMemoryWrapper instance.
   *
   * \pre        -
   * \context    App
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady     TRUE
   */
  virtual auto Create() noexcept -> amsr::UniquePtr<GenericMemoryWrapperInterface> = 0;
};

}  // namespace internal
}  // namespace ipc_binding_generic_layer
}  // namespace amsr

#endif  // LIB_IPC_BINDING_GENERIC_LAYER_INCLUDE_IPC_BINDING_GENERIC_LAYER_INTERNAL_MEMORY_WRAPPER_GENERIC_MEMORY_WRAPPER_FACTORY_INTERFACE_H_
