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
 *        \brief  Interface for GenericMemoryWrapperFactory.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_GENERIC_LAYER_INCLUDE_IPC_BINDING_GENERIC_LAYER_INTERNAL_MEMORY_WRAPPER_GENERIC_MEMORY_WRAPPER_FACTORY_H_
#define LIB_IPC_BINDING_GENERIC_LAYER_INCLUDE_IPC_BINDING_GENERIC_LAYER_INTERNAL_MEMORY_WRAPPER_GENERIC_MEMORY_WRAPPER_FACTORY_H_

#include <memory>

#include "ipc_binding_generic_layer/internal/memory_wrapper/generic_memory_wrapper_factory_interface.h"

namespace amsr {
namespace ipc_binding_generic_layer {
namespace internal {

/*!
 * \brief Factory for creating GenericMemoryWrapper's.
 *
 * \unit IpcBinding::IpcBindingGenericLayer::GenericProxyEvent
 */
class AMSR_LG_BETA GenericMemoryWrapperFactory : public GenericMemoryWrapperFactoryInterface {
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
  GenericMemoryWrapperFactory() noexcept = default;

  /*!
   * \brief Default destructor.
   *
   * \pre        -
   * \context    App
   * \threadsafe FALSE
   * \reentrant  FALSE
   * \steady     FALSE
   */
  ~GenericMemoryWrapperFactory() noexcept override = default;

  GenericMemoryWrapperFactory(GenericMemoryWrapperFactory const&) = delete;
  GenericMemoryWrapperFactory(GenericMemoryWrapperFactory&&) = delete;
  auto operator=(GenericMemoryWrapperFactory const&) -> GenericMemoryWrapperFactory& = delete;
  auto operator=(GenericMemoryWrapperFactory&&) -> GenericMemoryWrapperFactory& = delete;

  /*!
   * \copydoc amsr::ipc_binding_generic_layer::internal::GenericMemoryWrapperFactoryInterface::Create
   */
  auto Create() noexcept -> amsr::UniquePtr<GenericMemoryWrapperInterface> final;
};

}  // namespace internal
}  // namespace ipc_binding_generic_layer
}  // namespace amsr

#endif  // LIB_IPC_BINDING_GENERIC_LAYER_INCLUDE_IPC_BINDING_GENERIC_LAYER_INTERNAL_MEMORY_WRAPPER_GENERIC_MEMORY_WRAPPER_FACTORY_H_
