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
 *         \brief  Adapter class around an ipc connection.
 *
 *       \details  The connection has a templated type to be able to test the class.
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_CONNECTION_WRAPPER_H_
#define LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_CONNECTION_WRAPPER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <tuple>
#include <utility>

#include "amsr/core/abort.h"
#include "amsr/core/optional.h"
#include "amsr/generic/generic_error_domain.h"
#include "amsr/ipc/connection.h"
#include "amsr/zero_copy_binding/core/internal/connection_wrapper_interface.h"
#include "osabstraction/process/processinterface.h"

namespace amsr {
namespace zero_copy_binding {
namespace core {
namespace internal {

/*!
 * \brief     Concrete implementation of the ConnectionWrapperInterface.
 * \tparam    ConnectionType                Type of the connection to be wrapped.
 *                                          In production code only the given default shall be used.
 *                                          If the type is not ::amsr::ipc::Connection the type's conversion operator to
 *                                          ::amsr::ipc::Connection& shall guarantee that the returned reference is
 *                                          valid during an object's whole lifetime.
 * \tparam    GetOwnIntegrityLevelFunction  GetOwnIntegrityLevel() or a mocked function to be used instead.
 *                                          In production code only the given default shall be used.
 *                                          GetOwnIntegrityLevelFunction() must be a pure function.
 *
 * \unit      ZeroCopyBinding::ZeroCopyBindingCore::ConnectionWrapper
 * \vprivate  Component private
 */
template <typename ConnectionType = ::amsr::ipc::Connection,
          decltype(&::osabstraction::process::GetOwnIntegrityLevel) GetOwnIntegrityLevelFunction =
              &::osabstraction::process::GetOwnIntegrityLevel>
class ConnectionWrapper final : public ConnectionWrapperInterface {
 public:
  /*!
   * \brief       Constructor of ConnectionWrapper.
   * \param[in]   connection  A unique pointer to the connection this wrapper class shall store. Must not be nullptr.
   * \context     ANY
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \vprivate    Component private
   *
   * \internal
   * - Store the result of GetOwnIntegrityLevelInternal() to own_integrity_level_.
   * - Abort if no connection has been passed.
   * - Emplace the result of GetPeerIntegrityLevelInternal() to peer_integrity_level_.
   * \endinternal
   */
  explicit ConnectionWrapper(std::unique_ptr<ConnectionType> connection) noexcept
      : ConnectionWrapperInterface{},
        // VCA_ZEROCOPYBINDING_LNG_01_VALID_RVALUE_REFERENCE_ARGUMENT
        connection_{std::move(connection)},
        own_integrity_level_{GetOwnIntegrityLevelInternal()} {
    if (!connection_) {
      ::amsr::core::Abort(
          "ConnectionWrapper::ConnectionWrapper(): The passed connection pointer must not be a nullptr.");
    }
    peer_integrity_level_.emplace(GetPeerIntegrityLevelInternal());
  }

  /*!
   * \brief       Default destructor.
   */
  ~ConnectionWrapper() noexcept final = default;  // VCA_ZEROCOPYBINDING_SLC_23_MEMORY_DEALLOCATION_IN_DTOR

  ConnectionWrapper(ConnectionWrapper const&) = delete;
  auto operator=(ConnectionWrapper const&) -> ConnectionWrapper& = delete;
  ConnectionWrapper(ConnectionWrapper&&) = delete;
  auto operator=(ConnectionWrapper&&) -> ConnectionWrapper& = delete;

  /*!
   * \copydoc ::amsr::zero_copy_binding::core::internal::ConnectionWrapperInterface::ConnectAsync
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Prepare the connection completion callback that does the following:
   *   - Determine the peer's integrity level.
   *   - Call the user callback.
   * - Call ConnectAsync() on the stored connection object. If c2s_buffer_size_hint is set, pass this one instead of
   *   taking the default value.
   * \endinternal
   */
  auto ConnectAsync(::amsr::ipc::UnicastAddress remote_address, ConnectCompletionCallback callback,
                    ::amsr::core::Optional<std::size_t> c2s_buffer_size_hint) noexcept
      -> ::ara::core::Result<void> final {
    ::ara::core::Result<void> result{};

    // VECTOR NC AutosarC++17_10-A5.1.5: MD_ZEROCOPYBINDING_A5.1.5_lambdaNotUsedLocally
    ConnectCompletionCallback connect_compl_callback{
        // VCA_ZEROCOPYBINDING_LNG_01_VALID_RVALUE_REFERENCE_ARGUMENT
        [this, callback = std::move(callback), &peer_integrity_level = peer_integrity_level_](auto res) {
          // VCA_ZEROCOPYBINDING_LNG_03_VALID_THIS_POINTER_IN_LAMBDA
          peer_integrity_level.emplace(GetPeerIntegrityLevelInternal());
          callback(std::move(res));
        }};

    if (c2s_buffer_size_hint.has_value()) {
      // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT, VCA_ZEROCOPYBINDING_LNG_03_FORWARDING_FUNCTION_ON_VALID_OBJECT, VCA_ZEROCOPYBINDING_SPC_15_FORWARDING_FUNCTION
      result = this->connection_->ConnectAsync(remote_address, std::move(connect_compl_callback),
                                               c2s_buffer_size_hint.value());
    } else {
      // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT, VCA_ZEROCOPYBINDING_LNG_03_FORWARDING_FUNCTION_ON_VALID_OBJECT, VCA_ZEROCOPYBINDING_SPC_15_FORWARDING_FUNCTION
      result = this->connection_->ConnectAsync(remote_address, std::move(connect_compl_callback));
    }

    return result;
  }

  /*!
   * \copydoc ::amsr::zero_copy_binding::core::internal::ConnectionWrapperInterface::SendSync
   *
   * \internal
   * - Call SendSync() on the stored connection object, passing the message view in form of the expected data type.
   * \endinternal
   */
  auto SendSync(::amsr::core::Span<std::uint8_t const> const message_view) noexcept
      -> ::amsr::core::Result<void> final {
    std::array<osabstraction::io::ConstIOBuffer, 1U> io_buffer{{{message_view.data(), message_view.size()}}};
    ::amsr::core::Span<osabstraction::io::ConstIOBuffer> const io_buffer_view{io_buffer.data(), io_buffer.size()};

    // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT, VCA_ZEROCOPYBINDING_LNG_03_FORWARDING_FUNCTION_ON_VALID_OBJECT, VCA_ZEROCOPYBINDING_SPC_15_FORWARDING_FUNCTION
    return connection_->SendSync(io_buffer_view, nullptr);
  }

  /*!
   * \copydoc ::amsr::zero_copy_binding::core::internal::ConnectionWrapperInterface::ReceiveAsync
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Check the two arguments for validity and abort the program if they are invalid.
   * - Prepare the MessageAvailableCallback, which does the following:
   *   - Capture a MutableIOBuffer created from the memory buffer passed to ReceiveAsync().
   *   - Create a view to this buffer and return it.
   * - Prepare the ReceiveCompletionCallback, which does the following:
   *   - Capture both arguments of ReceiveAsync() by move.
   *   - If the passed result contains a value:
   *     - Create a view to the captured buffer with the size given in the result and pass it to the captured callback.
   *   - Else:
   *     - Call the captured callback with a result having the same error as the result obtained.
   * - Call ReceiveAsync() on the stored connection, passing the prepared callbacks.
   * \endinternal
   */
  auto ReceiveAsync(OnMessageReceivedCallback on_message_received_callback,
                    ::vac::memory::allocator::MemoryBufferPtr memory_buffer_ptr) noexcept
      -> ::amsr::core::Result<void> final {
    if (!on_message_received_callback) {
      ::amsr::core::Abort(
          "ConnectionWrapper::ReceiveAsync(): An invalid on message received callback has been passed.");
    }
    if (!memory_buffer_ptr) {
      ::amsr::core::Abort("ConnectionWrapper::ReceiveAsync(): An invalid memory buffer pointer has been passed.");
    }

    // The ownership of this unique pointer will be taken by the ReceiveCompletionCallback, to survive the
    // ReceiveAsync() call and the call to the MessageAvailableCallback during the receiving procedure.
    auto mutable_io_buffer_ptr(
        std::make_unique<::osabstraction::io::MutableIOBuffer>(::osabstraction::io::MutableIOBuffer{
            // VCA_ZEROCOPYBINDING_MEMORY_BUFFER_POINTER_VALID, VCA_ZEROCOPYBINDING_MEMORY_BUFFER_POINTER_VALID
            memory_buffer_ptr->GetView(0U).data(), memory_buffer_ptr->GetView(0U).size()}));

    ::amsr::ipc::MessageAvailableCallback message_available_callback{
        [mutable_io_buffer_raw_ptr = mutable_io_buffer_ptr.get()](
            // The parameter message_size is unused. When the size does not fit into the buffer an error result is
            // passed to the receive completion callback, which is forwarded to the user.
            std::size_t const& /* message_size */) -> ::amsr::core::Span<::osabstraction::io::MutableIOBuffer> {
          ::amsr::core::Span<::osabstraction::io::MutableIOBuffer> const io_buffer_view{mutable_io_buffer_raw_ptr, 1U};
          return io_buffer_view;
        }};

    // VECTOR NC AutosarC++17_10-A5.1.5: MD_ZEROCOPYBINDING_A5.1.5_lambdaNotUsedLocally
    ::amsr::ipc::ReceiveCompletionCallback receive_completion_callback{
        [
            // VCA_ZEROCOPYBINDING_LNG_01_VALID_RVALUE_REFERENCE_ARGUMENT
            on_message_received_callback = std::move(on_message_received_callback),
            // VCA_ZEROCOPYBINDING_LNG_01_VALID_RVALUE_REFERENCE_ARGUMENT, VCA_ZEROCOPYBINDING_LNG_01_VALID_RVALUE_REFERENCE_ARGUMENT
            memory_buffer_ptr = std::move(memory_buffer_ptr), mutable_io_buffer_ptr = std::move(mutable_io_buffer_ptr)](
            ::amsr::core::Result<std::size_t> receive_complete_result) {
          if (receive_complete_result.HasValue()) {
            ::amsr::core::Span<std::uint8_t const> const message_view{
                // VCA_ZEROCOPYBINDING_MEMORY_BUFFER_POINTER_VALID
                memory_buffer_ptr->GetView(0U).subspan(0U, receive_complete_result.Value())};
            on_message_received_callback(
                ::amsr::core::Result<::amsr::core::Span<std::uint8_t const>>::FromValue(message_view));
          } else {
            // VCA_ZEROCOPYBINDING_LNG_01_VALID_CONST_REFERENCE_ARGUMENT
            on_message_received_callback(::amsr::core::Result<::amsr::core::Span<std::uint8_t const>>::FromError(
                receive_complete_result.Error()));
          }
        }};

    // VCA_ZEROCOPYBINDING_LNG_01_VALID_RVALUE_REFERENCE_ARGUMENT, VCA_ZEROCOPYBINDING_SPC_15_VALID_RECEIVE_ASYNC_CALL, VCA_ZEROCOPYBINDING_LNG_03_FORWARDING_FUNCTION_ON_VALID_OBJECT
    return connection_->ReceiveAsync(std::move(message_available_callback), std::move(receive_completion_callback));
  }

  /*!
   * \copydoc ::amsr::zero_copy_binding::core::internal::ConnectionWrapperInterface::Close
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Call Close() on the stored connection object.
   * \endinternal
   */
  void Close() noexcept final {
    // VCA_ZEROCOPYBINDING_SPC_15_FORWARDING_FUNCTION, VCA_ZEROCOPYBINDING_LNG_03_FORWARDING_FUNCTION_ON_VALID_OBJECT
    this->connection_->Close();
  };

  /*!
   * \copydoc ::amsr::zero_copy_binding::core::internal::ConnectionWrapperInterface::CheckIsOpen
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Call CheckIsOpen() on the stored connection object.
   * \endinternal
   */
  auto CheckIsOpen() const noexcept -> bool final {
    // VCA_ZEROCOPYBINDING_SPC_15_FORWARDING_FUNCTION, VCA_ZEROCOPYBINDING_LNG_03_FORWARDING_FUNCTION_ON_VALID_OBJECT
    return this->connection_->CheckIsOpen();
  };

  /*!
   * \copydoc ::amsr::zero_copy_binding::core::internal::ConnectionWrapperInterface::IsInUse
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Call IsInUse() on the stored connection object.
   * \endinternal
   */
  auto IsInUse() const noexcept -> bool final {
    // VCA_ZEROCOPYBINDING_SPC_15_FORWARDING_FUNCTION, VCA_ZEROCOPYBINDING_LNG_03_FORWARDING_FUNCTION_ON_VALID_OBJECT
    return this->connection_->IsInUse();
  };

  /*!
   * \copydoc ::amsr::zero_copy_binding::core::internal::ConnectionWrapperInterface::GetPeerIdentity
   */
  auto GetPeerIdentity() const noexcept -> ::ara::core::Result<::amsr::ipc::Credentials> final {
    // VCA_ZEROCOPYBINDING_SPC_15_FORWARDING_FUNCTION, VCA_ZEROCOPYBINDING_LNG_03_FORWARDING_FUNCTION_ON_VALID_OBJECT
    return this->connection_->GetPeerIdentity();
  }

  /*!
   * \copydoc ::amsr::zero_copy_binding::core::internal::ConnectionWrapperInterface::GetPeerIntegrityLevel
   */
  auto GetPeerIntegrityLevel() const noexcept -> ::ara::core::Result<::amsr::ipc::IntegrityLevel> final {
    return peer_integrity_level_.value();
  }

  /*!
   * \copydoc ::amsr::zero_copy_binding::core::internal::ConnectionWrapperInterface::GetOwnIntegrityLevel
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - Return the result of the free GetOwnIntegrityLevel() function from the osabstraction.
   * \endinternal
   */
  auto GetOwnIntegrityLevel() const noexcept -> ::ara::core::Result<::amsr::ipc::IntegrityLevel> final {
    return own_integrity_level_;
  }

  /*!
   * \copydoc ::amsr::zero_copy_binding::core::internal::ConnectionWrapperInterface::GetConnection
   */
  auto GetConnection() noexcept -> ::amsr::ipc::Connection& final {
    // VCA_ZEROCOPYBINDING_LNG_02_VALID_CONNECTION_REFERENCE
    return static_cast<::amsr::ipc::Connection&>(*connection_);
  }

 private:
  /*!
   * \brief       Wrapper function around GetOwnIntegrityLevelFunction() forwarding its results.
   * \details     Used in the constructor to initialize the own_integrity_level_ member. Using the static variable
   *              directly in GetOwnIntegrityLevel() requires a mutex to be locked on some systems.
   * \return      The result of GetOwnIntegrityLevelFunction().
   * \context     ANY
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vprivate    Component private
   */
  static auto GetOwnIntegrityLevelInternal() -> ::ara::core::Result<::amsr::ipc::IntegrityLevel> {
    // VECTOR NC AutosarC++17_10-A3.3.2: MD_ZEROCOPYBINDING_A3.3.2_staticIntegrityLevelVariable
    static ::ara::core::Result<::amsr::ipc::IntegrityLevel> const kOwnIntegrityLevel{GetOwnIntegrityLevelFunction()};
    return kOwnIntegrityLevel;
  }

  /*!
   * \brief       Determines the peer's integrity level.
   * \details     Used in the constructor or the connect completion callback to initialize the peer_integrity_level_
   *              member.
   * \context     ANY
   * \pre         -
   * \threadsafe  TRUE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \vprivate    Component private
   *
   * \internal
   * - Set result to the default error.
   * - For each integrity level:
   *   - Retrieve result from CheckPeerIntegrityLevel().
   *   - If result contains an error return the error.
   *   - If the method returned 'false' return the previously stored integrity level.
   *   - Else store the current integrity level and continue.
   * \endinternal
   */
  auto GetPeerIntegrityLevelInternal() const noexcept -> ::ara::core::Result<::amsr::ipc::IntegrityLevel> {
    static constexpr std::array<::amsr::ipc::IntegrityLevel, 5U> kIntegrityLevelList{
        ::amsr::ipc::IntegrityLevel::kQm, ::amsr::ipc::IntegrityLevel::kAsilA, ::amsr::ipc::IntegrityLevel::kAsilB,
        ::amsr::ipc::IntegrityLevel::kAsilC, ::amsr::ipc::IntegrityLevel::kAsilD};
    auto result(::ara::core::Result<::amsr::ipc::IntegrityLevel>::FromError(::amsr::generic::GenErrc::kRuntime));
    for (::amsr::ipc::IntegrityLevel const integrity_level : kIntegrityLevelList) {
      // VCA_ZEROCOPYBINDING_SPC_15_GENERAL_JUSTIFICATION, VCA_ZEROCOPYBINDING_LNG_03_FORWARDING_FUNCTION_ON_VALID_OBJECT
      ::ara::core::Result<bool> const check_result{this->connection_->CheckPeerIntegrityLevel(integrity_level)};
      if (!check_result.HasValue()) {
        result.EmplaceError(check_result.Error());
        break;
      }
      if (!check_result.Value()) {
        break;
      }
      result.EmplaceValue(integrity_level);
    }
    return result;
  }

  /*!
   * \brief The stored connection.
   */
  std::unique_ptr<ConnectionType> const connection_;

  /*!
   * \brief The result of GetOwnIntegrityLevelFunction(). Stored as member to save system calls.
   */
  ::ara::core::Result<::amsr::ipc::IntegrityLevel> const own_integrity_level_;

  /*!
   * \brief The result of GetPeerIntegrityLevelInternal(). Stored as member to be also available after the connection
   *        broke down.
   */
  ::amsr::core::Optional<::ara::core::Result<::amsr::ipc::IntegrityLevel>> peer_integrity_level_{::amsr::core::nullopt};
};

}  // namespace internal
}  // namespace core
}  // namespace zero_copy_binding
}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_CONNECTION_WRAPPER_H_
