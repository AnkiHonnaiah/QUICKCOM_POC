/**********************************************************************************************************************
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
 *        \brief  Implementation of the HandshakeToUserCallbackInterface.
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKE_TO_USER_CALLBACK_IMPLEMENTATION_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKE_TO_USER_CALLBACK_IMPLEMENTATION_H_

#include <memory>
#include <utility>

#include "tls/internal/handshakestatemachine/handshake_to_user_callback_interface.h"
#include "tls/public/diagnostics_interface.h"

namespace amsr {
namespace tls {
namespace internal {

/*!
 *\brief Implementation of the HandshakeToUserCallbackInterface.
 */
template <typename ComProvider>
class HandshakeToUserCallbackImpl final : public handshakestatemachine::HandshakeToUserCallbackInterface {
 public:
  /*!
   * \brief Constructor for the Handshake protocol callback implementation
   * \param[in] instance_owner Reference to the parent of this callback
   * \param[in] diagnostic_handler Reference to the DiagnosticsInterface instance
   * \context ANY
   */
  explicit HandshakeToUserCallbackImpl(ComProvider& instance_owner,
                                       std::unique_ptr<amsr::tls::DiagnosticsInterface> diagnostic_handler) noexcept
      : HandshakeToUserCallbackInterface(),
        instance_owner_{instance_owner},
        diagnostic_handler_{std::move(diagnostic_handler)} {}

  /*! \brief Destructor override for the Handshake callback implementation. Default destructor. */
  ~HandshakeToUserCallbackImpl() final = default;

  HandshakeToUserCallbackImpl(HandshakeToUserCallbackImpl const&) = delete;
  HandshakeToUserCallbackImpl(HandshakeToUserCallbackImpl&&) = delete;
  HandshakeToUserCallbackImpl& operator=(HandshakeToUserCallbackImpl const&) = delete;
  HandshakeToUserCallbackImpl& operator=(HandshakeToUserCallbackImpl&&) = delete;

  /*!
   * \internal
   * - Set the current connection state to connected
   * - Notify the application about connection
   * \endinternal
   */
  void OnConnected() noexcept final {
    instance_owner_.SetCurrentState(EnTlsCommRoleState::kRoleStateConnected);
    instance_owner_.GetPtrToUserCallback()->OnConnected();
  }

  /*!
   * \internal
   * - Check if the pointer to the user callback interface is not null
   *   - Set the current connection state to not connected
   *   - Call the on disconnected function of user callback
   * - Otherwise,
   *   - Abort the process and Log a fatal with message indicating that pointer to user callback interface is null
   * \endinternal
   */
  void OnDisconnected() noexcept final {
    instance_owner_.SetCurrentState(EnTlsCommRoleState::kRoleStateNotConnected);
    instance_owner_.GetPtrToUserCallback()->OnDisconnected(
        static_cast<amsr::core::StringView>(instance_owner_.GetFlowContext().error_string),
        instance_owner_.GetFlowContext().error_code);
  }

  /*!
   * \internal
   * - Set the current connection state to performing a parallel handshake
   * \endinternal
   */
  void OnHandshakeStarted() noexcept final {
    instance_owner_.SetCurrentState(EnTlsCommRoleState::kRoleStatePerformingHandshake);
  }

  /*!
   * \brief Report the Tls Extensions
   * \param[in] extensions A vector of TlsExtensionData
   * \internal
   * - Check if the pointer to the user callback interface is not null
   * - Call the Tls Extensions function of user callback
   * \endinternal
   */
  void ReportTlsExtensions(ara::core::Vector<TlsExtensionData> extensions) noexcept final {
    if (diagnostic_handler_) {
      diagnostic_handler_->ReportTlsExtensions(extensions);
    }
  }

 private:
  /*! \brief A reference to a TlsClientCommProvider instance */
  ComProvider& instance_owner_;

  /*! \brief DiagnosticsInterface instance */
  std::unique_ptr<amsr::tls::DiagnosticsInterface> diagnostic_handler_;
};

}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKE_TO_USER_CALLBACK_IMPLEMENTATION_H_
