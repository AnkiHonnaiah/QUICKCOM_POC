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
/**       \file   libosabstraction-linux/include/osabstraction/io/reactor1/internal/callback_handle_types.h
 *        \brief  Constants, types and utility functions for CallbackHandles
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_OSABSTRACTION_IO_REACTOR1_INTERNAL_CALLBACK_HANDLE_TYPES_H_
#define LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_OSABSTRACTION_IO_REACTOR1_INTERNAL_CALLBACK_HANDLE_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include "osabstraction/io/reactor1/types.h"

namespace osabstraction {
namespace io {
namespace reactor1 {
namespace internal {

// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_OSA_M3.4.1_MinimizeIdentifierVisibility
/*!
 * \brief   Maximum allowed number of callbacks.
 *
 * \details The number of callbacks is limited by the bit width of the CallbackHandle index (13 bit). One entry has to
 *          be reserved for kUnblockCallbackHandle / kInvalidCallbackHandle.
 */
static constexpr std::uint64_t kMaxNumCallbacks{8191};

/*!
 * \brief   Offset of the callback entry index encoded into the CallbackHandle.
 *
 * \details The upper 13 bit of the CallbackHandle are used to encode the index of the callback entry in the internal
 *          data structure.
 */
static constexpr std::uint32_t kCallbackHandleIndexOffset{51};

/*!
 * \brief   Mask of the callback entry index encoded into the CallbackHandle.
 *
 * \details Masks the upper 13 bit of the CallbackHandle which are used to encode the index of the callback entry.
 */
static constexpr std::uint64_t kCallbackHandleIndexMask{0xFFF8000000000000};

/*!
 * \brief   Offset of the callback entry sequence number encoded into the CallbackHandle.
 *
 * \details The lower 51 bit of the CallbackHandle are used to encode the sequence number of the callback entry.
 */
static constexpr std::uint32_t kCallbackHandleSeqNumOffset{0};

/*!
 * \brief   Mask of the callback entry sequence number encoded into the CallbackHandle.
 *
 * \details Masks the lower 51 bit of the CallbackHandle which are used to encode the sequence number of the callback.
 */
static constexpr std::uint64_t kCallbackHandleSeqNumMask{0x7FFFFFFFFFFFF};

/*!
 * \brief   Handle to report an unblock event.
 *
 * \details The reserved value kInvalidCallbackHandle is used.
 */
static constexpr CallbackHandle kUnblockCallbackHandle{kInvalidCallbackHandle};

/*!
 * \brief Structure for storing the index and the sequence number of a CallbackHandle.
 */
struct CallbackHandleStruct {
  /*!
   * \brief   Index variable of the CallbackHandle.
   *
   * \details Points to the CallbackEntry where the callback is registered.
   */
  std::uint16_t index;

  /*!
   * \brief   Sequence number variable of the CallbackHandle.
   *
   * \details This is the sequence number of the CallbackEntry when the callback was registered.
   */
  std::uint64_t sequence_num;
};

/*!
 * \brief       Creates a CallbackHandleStruct from a CallbackHandle.
 *
 * \details     Decodes the index and sequence number variables encoded into a CallbackHandle and puts them into a
 *              newly created CallbackHandleStruct.
 *
 * \param[in]   handle    CallbackHandle to unpack.
 *
 * \return      Created CallbackHandleStruct.
 *
 * \context     ANY
 *
 * \pre         -
 *
 * \reentrant   TRUE
 * \synchronous TRUE
 * \threadsafe  TRUE
 */
CallbackHandleStruct UnpackCallbackHandle(CallbackHandle handle);

/*!
 * \brief       Creates a CallbackHandle from a CallbackHandleStruct.
 *
 * \details     Encodes the index and sequence number variables of a CalbackHandleStruct into a newly created
 *              CallbackHandle.
 *
 * \param[in]   handle_struct    CallbackHandleStruct to pack into a CallbackHandle.
 *
 * \return      Created CallbackHandle.
 *
 * \context     ANY
 *
 * \pre         -
 *
 * \reentrant   FALSE
 * \synchronous TRUE
 * \threadsafe  FALSE
 */
CallbackHandle PackCallbackHandle(CallbackHandleStruct handle_struct);

}  // namespace internal
}  // namespace reactor1
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_COMMON_LIBOSABSTRACTION_LINUX_INCLUDE_OSABSTRACTION_IO_REACTOR1_INTERNAL_CALLBACK_HANDLE_TYPES_H_
