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
/*!        \file
 *        \brief Definition and implementation of the BorrowedBitmap class.
 *        \unit ZeroCopyCommon::ZeroCopyLogic::BorrowedManager
 *
 *********************************************************************************************************************/
#ifndef LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_BORROWED_BITMAP_H_
#define LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_BORROWED_BITMAP_H_

#include <bitset>

#include "amsr/core/abort.h"

#include "amsr/zero_copy_common/logic/internal/receiver_handle.h"

namespace amsr {
namespace zero_copy_common {
namespace logic {
namespace internal {

/*!
 * \brief A BorrowedBitmap stores one bit of information (e.g. borrowed status) for N receivers and one sender.
 * \details The BorrowedBitmap is a data structure storing one bit of information for up to N receivers and one sender.
 *          The underlying implementation uses bitwise manipulation of a statically allocated 64 bit bitset which might
 *          be optimized by the compiler depending on the underlying architecture.
 *          This limits the class to a static maximum number of kReceiverLimit receivers that can be managed.
 *
 *          This can be used to e.g. store if a slot is currently borrowed by a specific receiver or the sender.
 *          Another usecase could be to e.g. store which receiver is registered in a class by setting its bit.
 *
 *          The BorrowedBitmap assumes that callers have already checked the validity of the ReceiverHandles passed
 *          into the unit. The BorrowedBitmap itself is unaware of ReceiverHandles and works solely on receiver_indices.
 *          The BorrowedBitmap performs minimum checks on these receiver_indices to avoid any out of bounds accesses.
 *
 * \vprivate Component Private
 */
class BorrowedBitmap final {
  /*!
   * \brief The size of the internally used bitmap.
   * \details We assume a size of 64 bits is reasonably fast to represent on most systems.
   *          If performance problems arise, this implementation detail can be changed.
   */
  static constexpr ReceiverHandle::ReceiverIndex kBitmapSize{64};

  /*!
   * \brief The index into the internal bitmap used for the senders borrowed status.
   * \details We use the last bit in our internal representation for the sender.
   */
  static constexpr ReceiverHandle::ReceiverIndex kSenderIndex{kBitmapSize - 1};

 public:
  /*!
   * \brief The limit on the maximum number of receivers that can be managed by this BorrowedManager.
   * \details Such a limit originates from implementation limitations of the bitmap representation used internally.
   *          Any receiver indices passed to methods need to fulfill "index < kReceiverLimit = kBitmapSize - 1".
   * \vprivate Component Private
   */
  static constexpr ReceiverHandle::ReceiverIndex kReceiverLimit{kBitmapSize - 1};  // Last bit for the sender.

  /*!
   * \brief Construct a new BorrowedBitmap with no bit set.
   * \vprivate Component Private
   *
   * \spec requires true; \endspec
   */
  // VCA_ZEROCOPYCOMMON_LNG_03_BORROWED_BITMAP_BITSET_INVALID_OBJECT
  BorrowedBitmap() noexcept = default;

  /*!
   * \brief Asserts that the given maximum receiver number fits within the static BorrowedBitmap implementation limits.
   * \param[in] max_receiver_number The requested maximum number of receivers to be checked against the static limit.
   *                                It must hold that max_receiver_number <= kReceiverLimit.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \spec requires true; \endspec
   */
  // VECTOR NL AutosarC++17_10-M0.1.8: MD_ZEROCOPYCOMMON_M0.1.8_noSideEffectInAssertFunction
  static void AssertMaxReceiverNumberWithinStaticLimits(ReceiverHandle::ReceiverIndex max_receiver_number) noexcept {
    if (max_receiver_number > kReceiverLimit) {
      ara::core::Abort("The static maximum receiver limit of BorrowedManagers is exceeded.");
    }
  }

  /*!
   * \brief Assert that the given receiver index fits within the static BorrowedBitmap implementation limits.
   * \details If this method does not abort, it is save to index into the bitmap_ member (post condition).
   * \param[in] receiver_index The receiver index to be checked against the static limit.
   *                           It must hold that receiver_index < kReceiverLimit.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \spec requires true; \endspec
   */
  // VECTOR NL AutosarC++17_10-M0.1.8: MD_ZEROCOPYCOMMON_M0.1.8_noSideEffectInAssertFunction
  static void AssertReceiverIndexWithinStaticLimits(ReceiverHandle::ReceiverIndex receiver_index) noexcept {
    if (receiver_index >= kReceiverLimit) {
      ara::core::Abort("The static maximum receiver limit of BorrowedManagers is exceeded.");
    }
  }

  /*!
   * \brief Check if the sender bit is set.
   * \return True, if the sender bit is set, false otherwise.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \spec requires true; \endspec
   */
  auto IsSenderSet() const noexcept -> bool { return bitmap_[kSenderIndex]; }

  /*!
   * \brief Set the sender bit.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \spec requires true; \endspec
   */
  void SetSender() noexcept { bitmap_[kSenderIndex] = true; }

  /*!
   * \brief Clear the sender bit.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \spec requires true; \endspec
   */
  void ClearSender() noexcept { bitmap_[kSenderIndex] = false; }

  /*!
   * \brief Check if the receiver bit is set for the given receiver index.
   * \param[in] receiver_index The index of the receiver to be checked.
   *                           It must hold that receiver_index < kReceiverLimit.
   * \return True, if the receiver bit is set, false otherwise.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \spec requires true; \endspec
   */
  auto IsReceiverSet(ReceiverHandle::ReceiverIndex receiver_index) const noexcept -> bool {
    AssertReceiverIndexWithinStaticLimits(receiver_index);
    return bitmap_[receiver_index];
  }

  /*!
   * \brief Set the receiver bit for the given receiver index.
   * \param[in] receiver_index The index of the receiver to be set.
   *                           It must hold that receiver_index < kReceiverLimit.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \spec requires true; \endspec
   */
  void SetReceiver(ReceiverHandle::ReceiverIndex receiver_index) noexcept {
    AssertReceiverIndexWithinStaticLimits(receiver_index);
    bitmap_[receiver_index] = true;
  }

  /*!
   * \brief Clear the receiver bit for the given receiver index.
   * \param[in] receiver_index The index of the receiver to be cleared.
   *                           It must hold that receiver_index < kReceiverLimit.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \spec requires true; \endspec
   */
  void ClearReceiver(ReceiverHandle::ReceiverIndex receiver_index) noexcept {
    AssertReceiverIndexWithinStaticLimits(receiver_index);
    bitmap_[receiver_index] = false;
  }

  /*!
   * \brief Check if any bit (any receiver or the sender) is set.
   * \return True, if any bit is set, false otherwise.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \spec requires true; \endspec
   */
  auto IsAnySet() const noexcept -> bool { return bitmap_.any(); }

  /*!
   * \brief Perform a bitwise-and operation between this and the given BorrowedBitmap.
   * \details Does not modify this or the other BorrowedBitmap, but returns a new BorrowedBitmap.
   * \param[in] other The other BorrowedBitmap to perform the bitwise-and.
   * \return A new BorrowedBitmap with the result of the bitwise-and.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   *
   * \spec requires true; \endspec
   */
  auto BitwiseAnd(BorrowedBitmap const& other) const noexcept -> BorrowedBitmap {
    BorrowedBitmap result{};
    result.bitmap_ = bitmap_;
    result.bitmap_ &= other.bitmap_;
    return result;
  }

 private:
  /*!
   * \brief The actual data structure holding the bitwise information.
   * \details For now, we use a bitset. If there are problems on any OS, this can be switched for raw bit manipulation.
   *          The default state for the bitmap is every bit un-set (which is the bitsets default constructor).
   */
  // VCA_ZEROCOPYCOMMON_LNG_03_BORROWED_BITMAP_BITSET_INVALID_OBJECT
  // VCA_ZEROCOPYCOMMON_SPC_15_BORROWED_BITMAP_BITSET_FUNCTION_CONTRACT
  std::bitset<kBitmapSize> bitmap_{};
};

}  // namespace internal
}  // namespace logic
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_ZERO_COPY_LOGIC_INCLUDE_AMSR_ZERO_COPY_COMMON_LOGIC_INTERNAL_BORROWED_BITMAP_H_
