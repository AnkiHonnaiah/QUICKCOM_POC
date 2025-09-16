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
 *         \brief  The Zero-Copy Binding.
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_INCLUDE_AMSR_ZERO_COPY_BINDING_ZERO_COPY_BINDING_H_
#define LIB_ZERO_COPY_BINDING_INCLUDE_AMSR_ZERO_COPY_BINDING_ZERO_COPY_BINDING_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/core/optional.h"
#include "amsr/generic/singleton_wrapper.h"
#include "amsr/zero_copy_binding/core/internal/access_control/access_control_default.h"
#include "amsr/zero_copy_binding/core/internal/initialize_state_machine.h"
#include "amsr/zero_copy_binding/core/internal/logging/zero_copy_logger.h"
#include "amsr/zero_copy_binding/internal/life_cycle_manager.h"

namespace amsr {
namespace zero_copy_binding {

/*!
 * \brief The Zero-Copy Binding.
 * \unit  ZeroCopyBinding::ZeroCopyBinding::ZeroCopyBindingLifeCycle
 */
class ZeroCopyBinding final : public core::internal::InitializeStateMachine {
 public:
  /*!
   * \brief Shortcut alias for AccessControlInterface.
   */
  using AccessControlInterface = core::internal::access_control::AccessControlInterface;

  /*!
   * \brief       Get the Zero-Copy Binding Singleton. If it does not exist yet, it will be created.
   *
   * \return      A singleton of the Zero-Copy Binding.
   * \details
   * \context     App
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vpublic
   */
  static auto GetInstance() noexcept -> ::amsr::generic::Singleton<ZeroCopyBinding>&;

  /*!
   * \brief       Set the user-defined access control implementation.
   * \details     The user can define a custom access control implementation, that has to be set after
   *              ZeroCopyBinding is initialized.
   * \param[in]   access_control Valid pointer to a user-defined access control implementation.
   * \pre         Initialize() must have been called before.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vpublic
   */
  void SetAccessControl(
      std::unique_ptr<core::internal::access_control::AccessControlInterface> access_control) noexcept;

  /*!
   * \brief       Reset the user-defined access control implementation to the default implementation.
   * \pre         Initialize() must have been called before.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vpublic
   */
  void ResetAccessControl() noexcept;

  /*!
   * \brief       Construct a new Zero-Copy Binding object.
   *
   * \context     Init
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component private
   */
  ZeroCopyBinding() noexcept;

  ZeroCopyBinding(ZeroCopyBinding const&) = delete;
  auto operator=(ZeroCopyBinding const&) -> ZeroCopyBinding& = delete;
  ZeroCopyBinding(ZeroCopyBinding&&) = delete;
  auto operator=(ZeroCopyBinding&&) -> ZeroCopyBinding& = delete;

  /*!
   * \brief       Default destructor of the Zero-Copy Binding.
   *
   * \context     Shutdown
   * \pre         The object must be in a deinitialized state.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate Component private
   */
  ~ZeroCopyBinding() noexcept = default;  // VCA_ZEROCOPYBINDING_SLC_20_BASE_CLASS_METHOD

 private:
  /*!
   * \brief        Initializer function to setup the Zero-Copy Binding.
   *
   * \context      Init
   * \pre          The object must not be initialized yet.
   * \threadsafe   FALSE
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \vprivate     Component private
   */
  void OnInitialize() noexcept final;

  /*!
   * \brief        Deinitializer function of the Zero-Copy Binding.
   *
   * \context      Shutdown
   * \pre          Initialize() must have been called before.
   * \threadsafe   FALSE
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \vprivate     Component private
   */
  void OnDeinitialize() noexcept final;

  /*!
   * \brief The Logger.
   */
  core::internal::logging::ZeroCopyLogger& logger_;

  /*!
   * \brief The LifeCycleManager instance.
   */
  ::amsr::core::Optional<internal::LifeCycleManager> life_cycle_manager_{};
};

}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_INCLUDE_AMSR_ZERO_COPY_BINDING_INTERNAL_ZERO_COPY_BINDING_H_
