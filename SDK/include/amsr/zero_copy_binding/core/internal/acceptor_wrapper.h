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
 *         \brief  Adapter class around an ipc acceptor.
 *
 *       \details  The acceptor has a templated type to be able to test the class.
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_ACCEPTOR_WRAPPER_H_
#define LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_ACCEPTOR_WRAPPER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "amsr/core/abort.h"
#include "amsr/ipc/acceptor.h"
#include "amsr/zero_copy_binding/core/internal/acceptor_wrapper_interface.h"

namespace amsr {
namespace zero_copy_binding {
namespace core {
namespace internal {

/*!
 * \brief     Acceptor implementation of the AcceptorWrapperInterface.
 * \tparam    AcceptorType  Type of the acceptor to be wrapped. In production code only the given default shall be used.
 *
 * \unit      ZeroCopyBinding::ZeroCopyBindingCore::AcceptorWrapper
 * \vprivate  Component private
 */
template <typename AcceptorType = ::amsr::ipc::Acceptor>
class AcceptorWrapper final : public AcceptorWrapperInterface {
 public:
  /*!
   * \brief       Constructor of AcceptorWrapper.
   * \param[in]   acceptor  A unique pointer to the acceptor this wrapper class shall store. Must not be nullptr.
   * \context     ANY
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   *
   * \internal
   * - Store the acceptor in the member variable.
   * \endinternal
   */
  explicit AcceptorWrapper(std::unique_ptr<AcceptorType> acceptor) noexcept
      : AcceptorWrapperInterface{},
        acceptor_{std::move(acceptor)}  // VCA_ZEROCOPYBINDING_LNG_01_VALID_RVALUE_REFERENCE_ARGUMENT
  {
    if (!acceptor_) {
      ::amsr::core::Abort("AcceptorWrapper::AcceptorWrapper(): The passed acceptor pointer must not be a nullptr.");
    }
  }

  /*!
   * \brief       Default destructor.
   */
  ~AcceptorWrapper() noexcept final = default;  // VCA_ZEROCOPYBINDING_SLC_23_MEMORY_DEALLOCATION_IN_DTOR

  AcceptorWrapper(AcceptorWrapper const&) = delete;
  auto operator=(AcceptorWrapper const&) -> AcceptorWrapper& = delete;
  AcceptorWrapper(AcceptorWrapper&&) = delete;
  auto operator=(AcceptorWrapper&&) -> AcceptorWrapper& = delete;

  /*!
   * \copydoc ::amsr::zero_copy_binding::core::internal::AcceptorWrapperInterface::Init
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Call Init() on the stored acceptor object. If s2c_buffer_size_hint is set, pass this one instead of taking the
   *   default value.
   * \endinternal
   */
  auto Init(::amsr::core::Optional<std::size_t> s2c_buffer_size_hint) noexcept -> ::ara::core::Result<void> final {
    ::ara::core::Result<void> result{};
    if (s2c_buffer_size_hint.has_value()) {
      // VCA_ZEROCOPYBINDING_LNG_03_FORWARDING_FUNCTION_ON_VALID_OBJECT
      result = acceptor_->Init(s2c_buffer_size_hint.value());
    } else {
      result = acceptor_->Init();  // VCA_ZEROCOPYBINDING_LNG_03_FORWARDING_FUNCTION_ON_VALID_OBJECT
    }
    return result;
  }

  /*!
   * \copydoc ::amsr::zero_copy_binding::core::internal::AcceptorWrapperInterface::AcceptAsync
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Call AcceptAsync() on the stored acceptor object.
   * \endinternal
   */
  auto AcceptAsync(AcceptorWrapperInterface::AcceptCompletionCallback&& callback) noexcept
      -> ::ara::core::Result<void> final {
    // VCA_ZEROCOPYBINDING_LNG_03_FORWARDING_FUNCTION_ON_VALID_OBJECT
    return acceptor_->AcceptAsync(std::move(callback));
  }

  /*!
   * \copydoc ::amsr::zero_copy_binding::core::internal::AcceptorWrapperInterface::Close
   *
   * \internal
   * - Call Close() on the stored acceptor object.
   * \endinternal
   */
  void Close() noexcept final {
    acceptor_->Close();  // VCA_ZEROCOPYBINDING_LNG_03_FORWARDING_FUNCTION_ON_VALID_OBJECT
  }

  /*!
   * \copydoc ::amsr::zero_copy_binding::core::internal::AcceptorWrapperInterface::IsInUse
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Call IsInUse() on the stored acceptor object.
   * \endinternal
   */
  auto IsInUse() const noexcept -> bool final {
    return acceptor_->IsInUse();  // VCA_ZEROCOPYBINDING_LNG_03_FORWARDING_FUNCTION_ON_VALID_OBJECT
  }

 private:
  /*!
   * \brief The pointer to the stored acceptor.
   */
  std::unique_ptr<AcceptorType> const acceptor_;
};

}  // namespace internal
}  // namespace core
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_ACCEPTOR_WRAPPER_H_
