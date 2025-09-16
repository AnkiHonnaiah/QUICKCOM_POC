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
 *        \brief  Runtime implementation.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_RUNTIME_H_
#define LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_RUNTIME_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/generic/singleton_wrapper.h"
#include "amsr/socal/internal/runtime.h"

#include "ipc_binding/internal/runtime_interface.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"

namespace amsr {
namespace ipc_binding {
namespace internal {

/*!
 * \brief   Productive runtime implementation for the RuntimeInterface.
 * \details This productive RuntimeInterface implementation is accessing the Socal runtime.
 *
 * \steady FALSE
 *
 * \unit IpcBinding::Runtime
 */
class Runtime final : public RuntimeInterface {
 public:
  /*!
   * \brief Construct a runtime.
   *
   * \pre       -
   * \context   Init
   * \reentrant FALSE
   * \steady    FALSE
   */
  Runtime() = default;

  /*!
   * \brief Destroy the Runtime.
   *
   * \pre       -
   * \context   Shutdown
   * \reentrant FALSE
   * \steady    FALSE
   */
  ~Runtime() noexcept final = default;  // VCA_IPCB_MOLE_1298

  Runtime(Runtime const&) noexcept = delete;
  auto operator=(Runtime const&) noexcept -> Runtime& = delete;
  Runtime(Runtime&&) noexcept = delete;
  auto operator=(Runtime&&) noexcept -> Runtime& = delete;

  /*!
   * \copydoc ::amsr::ipc_binding::internal::RuntimeInterface::GetSteadyTimerManager()
   */
  auto GetSteadyTimerManager() noexcept -> amsr::steady_timer::TimerManagerInterface& final;

  /*!
   * \copydoc ::amsr::ipc_binding::internal::RuntimeInterface::GetReactor()
   */
  auto GetReactor() noexcept -> osabstraction::io::reactor1::Reactor1& final;

  /*!
   * \brief Map an InstanceIdentifier to the corresponding P-Port InstanceSpecifier.
   *
   * \param[in] instance_specifier   The instance specifier which will be the key for the new entry.
   * \param[in] instance_identifier  The instance identifier which will be the value for the new entry. Expected format:
   *                                 "<binding_name>:<binding specific instance ID>".
   *
   * \pre         The same InstanceIdentifier and InstanceSpecifier combination must have not been mapped earlier.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void MapProvidedInstanceSpecifierToInstanceId(amsr::core::InstanceSpecifier const& instance_specifier,
                                                ara::com::InstanceIdentifier const& instance_identifier) noexcept final;

  /*!
   * \brief Map an InstanceIdentifier to the corresponding R-Port InstanceSpecifier.
   *
   * \param[in] instance_specifier   The instance specifier which will be the key for the new entry.
   * \param[in] instance_identifier  The instance identifier which will be the value for the new entry. Expected format:
   *                                 "<binding_name>:<binding specific instance ID>".
   *
   * \pre         The same InstanceIdentifier and InstanceSpecifier combination must have not been mapped earlier.
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void MapRequiredInstanceSpecifierToInstanceId(amsr::core::InstanceSpecifier const& instance_specifier,
                                                ara::com::InstanceIdentifier const& instance_identifier) noexcept final;

  /*!
   * \brief Removes the given InstanceIdentifier from the provided instance specifier table.
   *
   * \param[in] instance_specifier   The instance specifier which will be the key for the entry to be removed.
   * \param[in] instance_identifier  The instance identifier which will be the value for the entry to be removed.
   *
   * \pre         The InstanceIdentifier and InstanceSpecifier combination must have been mapped earlier.
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void RemoveProvidedInstanceSpecifierEntry(amsr::core::InstanceSpecifier const& instance_specifier,
                                            ara::com::InstanceIdentifier const& instance_identifier) noexcept final;

  /*!
   * \brief Removes the given InstanceIdentifier from the required instance specifier table.
   *
   * \param[in] instance_specifier   The instance specifier which will be the key for the entry to be removed.
   * \param[in] instance_identifier  The instance identifier which will be the value for the entry to be removed.
   *
   * \pre         The InstanceIdentifier and InstanceSpecifier combination must have been mapped earlier.
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void RemoveRequiredInstanceSpecifierEntry(amsr::core::InstanceSpecifier const& instance_specifier,
                                            ara::com::InstanceIdentifier const& instance_identifier) noexcept final;

 private:
  /*!
   * \brief Type alias for the user logger.
   */
  using Logger = ::amsr::ipc_binding_core::internal::logging::AraComLogger;

  /*!
   * \brief Logger
   */
  Logger logger_{::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextId,
                 ::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextDescription,
                 ::amsr::core::StringView{"Runtime"}};

  /*!
   * \brief   Socal runtime instance.
   * \details De-initializing Socal is not possible until this object is destroyed. So this member is added here to make
   *          sure IpcBinding is deinitialized before Socal.
   */
  ::amsr::generic::SingletonAccess<::amsr::socal::internal::Runtime> socal_runtime_{
      ::amsr::socal::internal::Runtime::getSingletonInstance().GetAccess()};
};

}  // namespace internal
}  // namespace ipc_binding
}  // namespace amsr

#endif  // LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_RUNTIME_H_
