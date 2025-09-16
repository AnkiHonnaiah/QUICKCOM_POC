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
 *        \brief  Deinitializes the binding. The target-dependent .cpp file is generated.
 *
 *********************************************************************************************************************/
#ifndef LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_TARGET_DEPENDENT_DEINITIALIZER_H_
#define LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_TARGET_DEPENDENT_DEINITIALIZER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ipc_binding/internal/target_dependent/deinitializer_interface.h"
#include "ipc_binding_core/internal/logging/ara_com_logger.h"

namespace amsr {
namespace ipc_binding {
namespace internal {
namespace target_dependent {

/*!
 * \brief Deinitialize the binding.
 *
 * \steady FALSE
 *
 * \unit IpcBinding::Deinitializer
 */
class Deinitializer final : public DeinitializerInterface {
 public:
  /*!
   * \brief Destroy the Deinitializer.
   *
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  Deinitializer() noexcept = default;

  Deinitializer(Deinitializer&&) noexcept = delete;
  Deinitializer(Deinitializer const&) noexcept = delete;
  auto operator=(Deinitializer const&) & -> Deinitializer& = delete;
  auto operator=(Deinitializer&&) & -> Deinitializer& = delete;

  /*!
   * \brief Destroys the object.
   *
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  ~Deinitializer() noexcept final = default;  // VCA_IPCB_COMPILER_GENERATED_DESTRUCTOR

  /*!
   * \copydoc DeinitializerInterface::Deinitialize
   */
  void Deinitialize(RuntimeConfig const& runtime_config) const noexcept final;

 private:
  /*!
   * \brief Runtime service configuration type.
   */
  // VECTOR Next Line VectorC++-V0.1.5: MD_IPCBINDING_VectorC++-V0.1.5_unused_type_declaration_false_positive
  using ServiceConfig = ::amsr::ipc_binding_core::internal::runtime_configuration::ServiceConfig;

  /*!
   * \brief Runtime service configuration reference type.
   */
  // VECTOR Next Line VectorC++-V0.1.5: MD_IPCBINDING_VectorC++-V0.1.5_unused_type_declaration_false_positive
  using ServiceConfigRef = RuntimeConfig::ServiceConfigRef;

  /*!
   * \brief Type-alias for ShortnamePath.
   */
  // VECTOR Next Line VectorC++-V0.1.5: MD_IPCBINDING_VectorC++-V0.1.5_unused_type_declaration_false_positive
  using ShortnamePath = ::amsr::ipc_binding_core::internal::runtime_configuration::ShortnamePath;

  /*!
   * \brief Type alias for the user logger.
   */
  using Logger = ::amsr::ipc_binding_core::internal::logging::AraComLogger;

  /*!
   * \brief Deinitializes required service instances.
   *
   * \param[in] runtime_config  Used IPC binding runtime configuration.
   *
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void DeinitializeRequiredServiceInstances(RuntimeConfig const& runtime_config) const noexcept;

  /*!
   * \brief Deregisters SkeletonXfFactory instances from SOCAL for each configured provided service instance.
   *
   * \param[in] runtime_config  Used IPC binding runtime configuration.
   *
   * \pre         -
   * \context     Shutdown
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  void DeinitializeProvidedServiceInstances(RuntimeConfig const& runtime_config) const noexcept;

  /*!
   * \brief Logger to print out debug & error messages.
   */
  Logger logger_{::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextId,
                 ::amsr::ipc_binding_core::internal::logging::kIpcLoggerContextDescription,
                 ::amsr::core::StringView{"Deinitializer"}};
};

}  // namespace target_dependent
}  // namespace internal
}  // namespace ipc_binding
}  // namespace amsr

#endif  // LIB_IPC_BINDING_INCLUDE_IPC_BINDING_INTERNAL_TARGET_DEPENDENT_DEINITIALIZER_H_
