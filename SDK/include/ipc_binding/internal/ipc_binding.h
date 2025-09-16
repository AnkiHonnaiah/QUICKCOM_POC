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
 *        \brief  The IPC Binding.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_IPC_BINDING_H_
#define LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_IPC_BINDING_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/generic/singleton_wrapper.h"

#include "ipc_binding/internal/life_cycle_manager.h"
#include "ipc_binding_core/internal/aracom_ipc_binding_interface.h"

namespace amsr {
namespace ipc_binding {
namespace internal {

/*!
 * \brief   Singleton that stores the binding's lifecycle manager instance.
 * \details Singleton instantiation of the lifecycle management object is done in this separate class to simplify
 *          testing.
 *
 * \unit IpcBinding::ComponentLifeCycleManagement
 */
class IpcBinding final {
 public:
  /*!
   * \brief Type-alias for the productive BindingCore main class.
   */
  using BindingCoreType = ::amsr::ipc_binding_core::internal::AraComIpcBindingInterface;

  /*!
   * \brief Get the singleton instance.
   *
   * \return Reference to the single instance of the class.
   *
   * \pre     -
   * \context ANY
   * \steady  FALSE
   */
  static auto GetInstance() noexcept -> amsr::generic::Singleton<IpcBinding>&;

  /*!
   * \brief Constructor for the IPC Binding (must not be called explicit).
   *
   * \pre     -
   * \context Init
   * \steady  FALSE
   */
  IpcBinding() noexcept;

  /*!
   * \brief Destroy the IpcBinding.
   *
   * \pre       -
   * \context   Shutdown
   * \reentrant FALSE
   * \steady    FALSE
   */
  ~IpcBinding() noexcept = default;

  IpcBinding(IpcBinding const&) noexcept = delete;
  auto operator=(IpcBinding const&) noexcept -> IpcBinding& = delete;
  IpcBinding(IpcBinding&&) noexcept = delete;
  auto operator=(IpcBinding&&) noexcept -> IpcBinding& = delete;

  /*!
   * \brief Initialize the IpcBinding.
   *
   * \return Empty result value or an error.
   *
   * \error IpcBindingErrc::json_loading_failure  If component specific JSON configuration could not be found or loaded.
   * \error IpcBindingErrc::json_parsing_failure  If component specific JSON configuration parsing failed.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto Initialize() noexcept -> amsr::core::Result<void>;

  /*!
   * \brief Deinitialize the IpcBinding.
   *
   * \return Always a positive result.
   *
   * \pre         The related reactor thread must be stopped.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto Deinitialize() noexcept -> amsr::core::Result<void>;

  /*!
   * \brief Get the IpcBindingCore main instance.
   *
   * \return Reference to the IpcBindingCore main instance.
   *
   * \pre         Binding is initialized.
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  auto GetBindingCore() noexcept -> BindingCoreType&;

 private:
  /*!
   * \brief Productive LifeCycleManager instance.
   */
  LifeCycleManager life_cycle_manager_;
};

}  // namespace internal
}  // namespace ipc_binding
}  // namespace amsr

#endif  // LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_IPC_BINDING_H_
