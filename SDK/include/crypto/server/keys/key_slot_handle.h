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
/*!        \file  key_slot_handle.h
 *        \brief  Class representing a key slot handle.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_KEYS_KEY_SLOT_HANDLE_H_
#define LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_KEYS_KEY_SLOT_HANDLE_H_
/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "key_slot.h"

namespace crypto {
namespace server {
namespace keys {

/*!
 * \brief Internal key slot handle representation.
 * \vprivate Component Private
 */
class KeySlotHandle final {
 public:
  /*!
   * \brief Construct a default KeySlotHandle.
   * \vprivate Component Private
   */
  KeySlotHandle() noexcept;

  /*!
   * \brief Construct a new KeySlotHandle instance.
   * \param[in] key_slot The key slot to be represented by this handle
   * \vprivate Component Private
   */
  explicit KeySlotHandle(std::weak_ptr<KeySlot> key_slot) noexcept;

  /*!
   * \brief Destroy this KeySlotHandle instance.
   * \vprivate Component Private
   */
  ~KeySlotHandle() noexcept = default;

  /*!
   * \brief Copy construct a new KeySlotHandle instance.
   * \vprivate Component Private
   */
  KeySlotHandle(KeySlotHandle const& /*other*/) noexcept = default;

  /*!
   * \brief Move construct a new KeySlotHandle instance.
   * \vprivate Component Private
   */
  KeySlotHandle(KeySlotHandle&& /*other*/) noexcept = default;

  /*!
   * \brief Default copy assignment operator
   * \return reference to this.
   * \vprivate Component Private
   */
  KeySlotHandle& operator=(KeySlotHandle const& /*other*/) & noexcept = default;

  /*!
   * \brief Default move assignment operator
   * \return reference to this.
   * \vprivate Component Private
   */
  KeySlotHandle& operator=(KeySlotHandle&& /*other*/) & noexcept = default;

  /*!
   * \brief Indicates if the key slot this handle refers to is still valid & unchanged.
   * \return True if the handle can still be used, false otherwise
   * \vprivate Component Private
   */
  auto IsValid() const noexcept -> bool;

  /*!
   * \brief Get the slot number this handle is referring to.
   * \return Slot number
   * \vprivate Component Private
   */
  auto GetSlotNumber() const noexcept -> KeySlot::SlotNumber;

  // VECTOR NC AutosarC++17_10-A11.3.1: MD_CRYPTO_AutosarC++17_10-A11.3.1_comparators
  /*! Friend declaration for equal to comparison operator */
  friend auto operator==(KeySlotHandle const& lhs, KeySlotHandle const& rhs) noexcept -> bool;

 private:
  KeySlot::SlotNumber slot_number_; /*!< Slot number of the actual key slot */
  std::weak_ptr<KeySlot> key_slot_; /*!< Weak pointer to the actual key slot */
};

/*!
 * \brief Is equal comparison operator.
 * \param[in] lhs Left hand side of the comparison
 * \param[in] rhs Right hand side of the comparison
 * \return True iff both objects are identical, false otherwise
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Component Private
 */
auto operator==(KeySlotHandle const& lhs, KeySlotHandle const& rhs) noexcept -> bool;

/*!
 * \brief Is not equal comparison operator.
 * \param[in] lhs Left hand side of the comparison
 * \param[in] rhs Right hand side of the comparison
 * \return True iff both objects are not equal, false otherwise
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Component Private
 */
auto operator!=(KeySlotHandle const& lhs, KeySlotHandle const& rhs) noexcept -> bool;

/*!
 * \brief Less than comparison operator.
 * \param[in] lhs Left hand side of the comparison
 * \param[in] rhs Right hand side of the comparison
 * \return True iff \c lhs < \c rhs, false otherwise
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Component Private
 */
auto operator<(KeySlotHandle const& lhs, KeySlotHandle const& rhs) noexcept -> bool;

}  // namespace keys
}  // namespace server
}  // namespace crypto

#endif  //  LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_KEYS_KEY_SLOT_HANDLE_H_
