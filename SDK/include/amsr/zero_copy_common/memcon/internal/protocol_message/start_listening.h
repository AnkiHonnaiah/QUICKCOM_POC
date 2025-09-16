/*!********************************************************************************************************************
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
 *        \brief StartListening message.
 *        \unit ZeroCopyCommon::MemCon::ProtocolMessage
 *
 *********************************************************************************************************************/
#ifndef LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_PROTOCOL_MESSAGE_START_LISTENING_H_
#define LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_PROTOCOL_MESSAGE_START_LISTENING_H_

#include "amsr/zero_copy_common/memcon/internal/protocol_message/protocol_message.h"

namespace amsr {
namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace protocol_message {

/*!
 * \brief StartListening message.
 * \vprivate Component internal.
 */
class StartListening : public ProtocolMessage<StartListening> {
 public:
  /*!
   * \brief Create with explicit values.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr StartListening() noexcept : ProtocolMessage{ProtocolMessage::Id::kStartListening} {}

  /*!
   * \brief Compare messages against another.
   * \param[in] other ProtocolMessage to compare against.
   * \return True if equal, false if different.
   * \context ANY
   * \pre -
   *
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component internal.
   */
  constexpr auto operator==(StartListening const& other) const noexcept -> bool {
    static_cast<void>(other);
    return true;
  }
};

}  // namespace protocol_message
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common
}  // namespace amsr

#endif  // LIB_MEMCON_INCLUDE_AMSR_ZERO_COPY_COMMON_MEMCON_INTERNAL_PROTOCOL_MESSAGE_START_LISTENING_H_
