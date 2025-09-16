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
 *         \brief  The interface of the acceptor wrapper used in the skeleton event manager.
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_ACCEPTOR_WRAPPER_INTERFACE_H_
#define LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_ACCEPTOR_WRAPPER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>

#include "amsr/core/optional.h"
#include "amsr/ipc/acceptor.h"
#include "amsr/ipc/ipc_types.h"
#include "ara/core/result.h"

namespace amsr {
namespace zero_copy_binding {
namespace core {
namespace internal {

/*!
 * \brief     The interface of the acceptor wrapper.
 * \details   The interface is used in the factory and hence should not be templated.
 *
 * \unit      ZeroCopyBinding::ZeroCopyBindingCore::AcceptorWrapper
 * \vprivate  Component private
 */
class AcceptorWrapperInterface {
 protected:
  /*!
   * \brief The type of the accept completion call back.
   */
  using AcceptCompletionCallback = ::amsr::ipc::AcceptCompletionCallback;

 public:
  /*!
   * \brief       Default constructor.
   */
  AcceptorWrapperInterface() noexcept = default;

  /*!
   * \brief       Default destructor.
   */
  virtual ~AcceptorWrapperInterface() noexcept = 0;

  AcceptorWrapperInterface(AcceptorWrapperInterface const&) = delete;
  auto operator=(AcceptorWrapperInterface const&) -> AcceptorWrapperInterface& = delete;
  AcceptorWrapperInterface(AcceptorWrapperInterface&&) = delete;
  auto operator=(AcceptorWrapperInterface&&) -> AcceptorWrapperInterface& = delete;

  /*!
   * \brief       Calls Init() on the stored acceptor object.
   * \param[in]   s2c_buffer_size_hint  Optional parameter that indicates the shared memory buffer size (in bytes).
   * \error       See CDD of the wrapped acceptor class.
   * \context     ANY
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  virtual auto Init(::amsr::core::Optional<std::size_t> s2c_buffer_size_hint) noexcept -> ::ara::core::Result<void> = 0;

  /*!
   * \brief       Calls AcceptAsync() on the stored acceptor object.
   * \param[in]   callback  Callback to notify about the completion of the accept operation.
   * \error       See CDD of the wrapped acceptor class.
   * \context     ANY
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous FALSE
   * \vprivate    Component private
   */
  virtual auto AcceptAsync(AcceptCompletionCallback&& callback) noexcept -> ::ara::core::Result<void> = 0;

  /*!
   * \brief       Calls Close() on the stored acceptor object.
   * \context     Not timer_manager::HandleTimerExpiry() if a timer manager is used.
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  virtual void Close() noexcept = 0;

  /*!
   * \brief       Calls IsInUse() on the stored acceptor object.
   * \return      true if the acceptor is initialized or any provided callback is currently executing, false otherwise.
   * \context     ANY
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  virtual auto IsInUse() const noexcept -> bool = 0;
};

inline AcceptorWrapperInterface::~AcceptorWrapperInterface() noexcept = default;

}  // namespace internal
}  // namespace core
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_ACCEPTOR_WRAPPER_INTERFACE_H_
