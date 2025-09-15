/*!********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!       \file
 *        \brief  Definition of class ReceiverManager.
 *        \unit   ZeroCopyCommon::ZeroCopyLogic::ReceiverManager
 *
 *********************************************************************************************************************/
#ifndef LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_RECEIVER_MANAGER_H_
#define LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_RECEIVER_MANAGER_H_

#include <memory>

#include "amsr/core/optional.h"
#include "vac/container/static_vector.h"

#include "amsr/zero_copy_common/logic/internal/handle.h"
#include "amsr/zero_copy_common/logic/internal/receiver.h"
#include "amsr/zero_copy_common/logic/internal/receiver_handle.h"

namespace amsr {
namespace zero_copy_common {
namespace logic {
namespace internal {

/*!
 * \brief A ReceiverManager that manages registered Receiver instances.
 * \details Manages registered Receiver instances.
 *          The maximum number of Receiver instances that can be managed is set when a ReceiverManager is created.
 *          Allows dynamically registering and removing Receiver instances.
 *          Allows access to the registered Receiver instances using the respective ReceiverHandle instances.
 * \vprivate Component Private
 */
// VCA_ZEROCOPYCOMMON_SPC_15_RECEIVER_MANAGER_STATIC_VECTOR_FUNCTION_CONTRACT
// VCA_ZEROCOPYCOMMON_SPC_15_RECEIVER_MANAGER_STATIC_VECTOR_FUNCTION_CONTRACT
class ReceiverManager final {
 public:
  /*!
   * \brief Type used for receiver ids.
   */
  using ReceiverId = ReceiverHandle::ReceiverId;

  /*!
   * \brief Type used for receiver indices.
   */
  using ReceiverIndex = ReceiverHandle::ReceiverIndex;

  /*!
   * \brief Construct a ReceiverManager.
   * \details The \p group defines the handle group that registered Receiver instances will belong to.
   *          The \p max_number_receivers is used to statically allocate the required memory.
   *          This means that the maximum number of Receiver instances cannot be changed after construction.
   * \param group Group of handles that this manager uses.
   * \param max_number_receivers Maximum number of Receivers instances that can be managed simultaneously by this
   *        instance.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  ReceiverManager(Handle::Group const& group, ReceiverIndex max_number_receivers) noexcept;

  /*!
   * \brief Check if an additional Receiver can be registered.
   * \details An additional Receiver can be registered if the maximum number of simultaneously manageable Receivers
   *          instances is not reached.
   *          The maximum number of simultaneously manageable Receiver instances is specified when a ReceiverManager is
   *          constructed.
   * \return True if an additional Receiver can be registered, false otherwise.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto CanRegister() const noexcept -> bool;

  /*!
   * \brief Check if the Receiver represented by the \p handle is registered.
   * \param handle ReceiverHandle representing a Receiver.
   * \return True if Receiver is registered, false otherwise.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto IsRegistered(ReceiverHandle const& handle) const noexcept -> bool;

  /*!
   * \brief Create a new Receiver and register it to this instance.
   * \details Create a new Receiver using the given \p free_queue_ptr and \p available_queue_ptr.
   *          The returned ReceiverHandle belongs to the handle group specified when this instance was constructed.
   *          The id of the returned ReceiverHandle is NOT guaranteed to be unique as it is ONLY used internally in the
   *          ReceiverHandle unit.
   *          The index of the returned ReceiverHandle is guaranteed to be unique for this ReceiverManager instance.
   *          It is in the range [0; max_number_receivers_).
   * \param free_queue_ptr Free queue of the Receiver.
   * \param available_queue_ptr Available queue of the Receiver.
   * \return ReceiverHandle representing the newly created and registered Receiver.
   * \context ANY
   * \pre The \p free_queue_ptr and \p available_queue_ptr must not contain nullptr.
   *      The \p free_queue_ptr and \p available_queue_ptr must point to valid objects. This cannot be checked.
   *      The \p free_queue_ptr and \p available_queue_ptr must provide valid implementations of the respective base
   *      class. This cannot be checked.
   *      It must be possible to register another Receiver to this instance. I.e. the maximum number of Receiver
   *      instances must not be reached.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto RegisterReceiver(std::unique_ptr<ReadingQueueEndInterface> free_queue_ptr,
                        std::unique_ptr<WritingQueueEndInterface> available_queue_ptr) noexcept -> ReceiverHandle;

  /*!
   * \brief Remove the registered Receiver represented by the \p handle from this instance.
   * \param handle ReceiverHandle representing the Receiver to remove.
   * \context ANY
   * \pre The Receiver represented by the \p handle must be registered to this instance.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void RemoveReceiver(ReceiverHandle const& handle) noexcept;

  /*!
   * \brief Apply a \p callable to the registered Receiver represented by the \p handle.
   * \tparam F The type of the callable to apply to the registered Receiver.
   *         The \p callable must have the signature void(Receiver const&). This property is not checked explicitly.
   * \param handle ReceiverHandle representing the Receiver to apply the callable to.
   * \param callable Callable to apply to the Receiver.
   * \context ANY
   * \pre The Receiver represented by the \p handle must be registered to this instance.
   *      The \p callable must be a valid callable (non-nullptr) respecting the contract of the Receiver passed to it.
   *      This cannot be checked.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename F>
  void ForOneReceiver(ReceiverHandle const& handle, F&& callable) const noexcept {
    // VCA_ZEROCOPYCOMMON_SLC_20_RECEIVER_MANAGER_UNDEFINED_FUNCTION
    amsr::core::Optional<Receiver> const& opt_receiver{AssertHandleRegisteredAndGetOptionalReceiver(handle)};
    Receiver const& receiver{opt_receiver.value()};
    // VCA_ZEROCOPYCOMMON_SPC_15_RECEIVER_MANAGER_CALLABLE_FUNCTION_CONTRACT
    callable(receiver);
  }

  /*!
   * \brief Apply a \p callable to the registered Receiver represented by the \p handle.
   * \tparam F The type of the callable to apply to the registered Receiver.
   *         The \p callable must have the signature void(Receiver&). This property is not checked explicitly.
   * \param handle ReceiverHandle representing the Receiver to apply the callable to.
   * \param callable Callable to apply to the Receiver.
   * \context ANY
   * \pre The Receiver represented by the \p handle must be registered to this instance.
   *      The \p callable must be a valid callable (non-nullptr) respecting the contract of the Receiver passed to it.
   *      This cannot be checked.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename F>
  void ForOneReceiver(ReceiverHandle const& handle, F&& callable) noexcept {
    // VCA_ZEROCOPYCOMMON_SLC_20_RECEIVER_MANAGER_UNDEFINED_FUNCTION
    amsr::core::Optional<Receiver>& opt_receiver{AssertHandleRegisteredAndGetOptionalReceiver(handle)};
    Receiver& receiver{opt_receiver.value()};
    // VCA_ZEROCOPYCOMMON_SPC_15_RECEIVER_MANAGER_CALLABLE_FUNCTION_CONTRACT
    callable(receiver);
  }

  /*!
   * \brief Apply a \p callable to all registered Receiver instances.
   * \tparam F The type of the callable to apply to all registered Receiver instances.
   *         The \p callable must have the signature void(Receiver&). This property is not checked explicitly.
   * \param callable Callable to apply to all registered Receiver instances.
   * \context ANY
   * \pre The \p callable must be a valid callable (non-nullptr) respecting the contract of the Receiver passed to it.
   *      This cannot be checked.
   *      The \p callable must not modify the ReceiverManager instance itself (e.g. by registering or removing a
   *      Receiver). Otherwise, undefined behavior will occur. This cannot be checked.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename F>
  void ForEachReceiver(F&& callable) noexcept {
    for (ReceiverStorage::value_type& opt_receiver : index_to_receiver_) {
      if (opt_receiver.has_value()) {
        // VCA_ZEROCOPYCOMMON_LNG_03_RECEIVER_MANAGER_OPTIONAL_INVALID_OBJECT
        Receiver& receiver{opt_receiver.value()};
        // VCA_ZEROCOPYCOMMON_SPC_15_RECEIVER_MANAGER_CALLABLE_FUNCTION_CONTRACT
        callable(receiver);
      }
    }
  }

 private:
  /*!
   * \brief Assert that the Receiver represented by the \p handle is registered and return a reference to the Optional
   *        containing the Receiver.
   * \details The returned Optional is guaranteed to be non-empty. Otherwise, abort is called in the function.
   * \param handle ReceiverHandle representing a Receiver.
   * \return Reference to the non-empty Optional containing the Receiver.
   * \context ANY
   * \pre The Receiver represented by the \p handle must be registered to this instance.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto AssertHandleRegisteredAndGetOptionalReceiver(ReceiverHandle const& handle) noexcept
      -> amsr::core::Optional<Receiver>&;

  /*!
   * \brief Assert that the Receiver represented by the \p handle is registered and return a reference to the Optional
   *        containing the Receiver.
   * \details The returned Optional is guaranteed to be non-empty. Otherwise, abort is called in the function.
   * \param handle ReceiverHandle representing a Receiver.
   * \return Reference to the non-empty Optional containing the Receiver.
   * \context ANY
   * \pre The Receiver represented by the \p handle must be registered to this instance.
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto AssertHandleRegisteredAndGetOptionalReceiver(ReceiverHandle const& handle) const noexcept
      -> amsr::core::Optional<Receiver> const&;

  /*!
   * \brief Type used for storing registered Receiver instances.
   */
  using ReceiverStorage = vac::container::StaticVector<amsr::core::Optional<Receiver>>;

  /*!
   * \brief Type used for storing unused ReceiverIndex values.
   */
  using ReceiverIndexStorage = vac::container::StaticVector<ReceiverIndex>;

  /*!
   * \brief Internal counter containing the Id that the next Receiver will get.
   */
  ReceiverId next_id_{0};

  /*!
   * \brief Handle group that registered Receiver instances of this ReceiverManager will belong to.
   */
  Handle::Group group_;

  /*!
   * \brief The maximum number of Receiver instances that can be registered to this ReceiverManager.
   */
  ReceiverIndex max_number_receivers_;

  /*!
   * \brief StaticVector storing registered Receiver instances by optionally containing a Receiver at each index.
   */
  ReceiverStorage index_to_receiver_;

  /*!
   * \brief Currently unused ReceiverIndex values of this ReceiverManager.
   */
  ReceiverIndexStorage unused_receiver_indices_;
};

}  // namespace internal
}  // namespace logic
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_RECEIVER_MANAGER_H_
