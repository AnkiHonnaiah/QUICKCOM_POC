// VECTOR SL AutosarC++17_10-A2.9.1: MD_CRYPTO_AutosarC++17_10-A2.9.1
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
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  access_control_config.h
 *        \brief  Cryptostack access control configuration abstraction.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_CONFIG_ACCESS_CONTROL_CONFIG_H_
#define LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_CONFIG_ACCESS_CONTROL_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/core/result.h"
#include "amsr/core/string.h"
#include "amsr/ipc/credentials.h"
#include "ara/core/map.h"
#include "ara/core/vector.h"
#include "ara/crypto/keys/elementary_types.h"
#include "crypto/common/data/data_container.h"
#include "crypto/common/data/json_keys.h"
#include "crypto/server/constants.h"

namespace crypto {
namespace server {
namespace config {

/*!
 * \brief Operation modes in context of access control.
 * \vprivate Product Private
 */
enum class Operation : std::uint8_t {
  kNone = 0b00000000,      /*!< Restricted access */
  kRead = 0b00000001,      /*!< Read operation. */
  kWrite = 0b00000010,     /*!< Write operation. */
  kReadWrite = 0b00000011, /*!< Read/Write operation. */
};

/*!
 * \brief Bitwise and operator.
 * \param[in] lhs Left hand side of the bitwise and operation
 * \param[in] rhs Right hand side of the bitwise and operation
 * \return The bitwise and of \c lhs and \c rhs
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Component Private
 */
constexpr auto operator&(Operation lhs, Operation rhs) -> Operation {
  return static_cast<Operation>(static_cast<std::uint8_t>(lhs) & static_cast<std::uint8_t>(rhs));
}

// VECTOR NC AutosarC++17_10-A8.4.4: MD_CRYPTO_AutosarC++17_10-A8.4.4_in_out_param
// VECTOR NC AutosarC++17_10-A12.8.7: MD_CRYPTO_AutosarC++17_10-A12.8.7_bitwise_assignment_overload_false_positive
/*!
 * \brief Bitwise and assignment operator.
 * \param[in] lhs Left hand side of the bitwise and assignment operation
 * \param[in] rhs Right hand side of the bitwise and assignment operation
 * \return The bitwise and of \c lhs and \c rhs
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Component Private
 */
constexpr auto operator&=(Operation& lhs, Operation rhs) -> Operation& {
  // VECTOR Next Line AutosarC++17_10-A4.5.1: MD_CRYPTO_AutosarC++17_10-A4.5.1_enum_operand
  lhs = lhs & rhs;
  return lhs;
}

/*!
 * \brief Bitwise or operator.
 * \param[in] lhs Left hand side of the bitwise or operation
 * \param[in] rhs Right hand side of the bitwise or operation
 * \return The bitwise or of \c lhs and \c rhs
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Component Private
 */
constexpr auto operator|(Operation lhs, Operation rhs) noexcept -> Operation {
  return static_cast<Operation>(static_cast<std::uint8_t>(lhs) | static_cast<std::uint8_t>(rhs));
}

// VECTOR NC AutosarC++17_10-A8.4.4: MD_CRYPTO_AutosarC++17_10-A8.4.4_in_out_param
// VECTOR NC AutosarC++17_10-A12.8.7: MD_CRYPTO_AutosarC++17_10-A12.8.7_bitwise_assignment_overload_false_positive
/*!
 * \brief Bitwise or assignment operator.
 * \param[in] lhs Left hand side of the bitwise or assignment operation
 * \param[in] rhs Right hand side of the bitwise or assignment operation
 * \return The bitwise or of \c lhs and \c rhs
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Component Private
 */
constexpr auto operator|=(Operation& lhs, Operation rhs) noexcept -> Operation& {
  // VECTOR Next Line AutosarC++17_10-A4.5.1: MD_CRYPTO_AutosarC++17_10-A4.5.1_enum_operand
  lhs = lhs | rhs;
  return lhs;
}

/*!
 * \brief Comparison operator.
 * \param[in] lhs Left hand side of the comparison
 * \param[in] rhs Right hand side of the comparison
 * \return True if both objects are identical, false otherwise
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Component Private
 */
constexpr auto operator==(Operation const lhs, Operation const rhs) noexcept -> bool {
  return static_cast<std::uint8_t>(lhs) == static_cast<std::uint8_t>(rhs);
}

/*!
 * \brief Access control configuration.
 * \vprivate Component Private
 */
class AccessControlConfig final {
 public:
  /*!
   * \brief Deserialize the given DataContainer to retrieve the access control configuration.
   * \param[in] serial Serialized configuration.
   * \return void.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kUnsupportedFormat If the configuration data is malformed.
   * \reentrant FALSE
   * \synchronous FALSE
   * \vprivate Component Private
   */
  auto Deserialize(crypto::common::data::DataContainer const& serial) noexcept -> amsr::core::Result<void>;

  /*!
   * \brief Check if a valid access control configuration is present.
   * \return Whether the underlying access control configuration is empty.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous FALSE
   * \vprivate Component Private
   */
  auto IsEmpty() const noexcept -> bool;

  /*!
   * \brief Get the allowed operation for a given slot number and user.
   * \param[in] user_id The user ID for which the allowed operation shall be checked.
   * \param[in] slot_number The slot number for which the allowed operation shall be checked.
   * \return The operation that is allowed for the given user on the given slot.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous FALSE
   * \vprivate Component Private
   */
  auto GetAllowedOperation(amsr::ipc::Credentials user_id, ara::crypto::keys::SlotNumber slot_number) noexcept
      -> Operation;

 private:
  /*!
   * \brief Deserialize the given DataContainer to retrieve the access control configuration.
   * \param[in] restrictions Serialized restrictions.
   * \return Container of deserialized restrictions
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kUnsupportedFormat If the configuration data is malformed.
   * \reentrant FALSE
   * \synchronous FALSE
   */
  static auto DeserializeRestrictions(
      ara::core::Vector<crypto::common::data::DataContainer> const& restrictions) noexcept
      -> amsr::core::Result<ara::core::Vector<std::pair<ara::crypto::keys::SlotNumber, Operation>>>;

  ara::core::Map<amsr::ipc::Credentials, ara::core::Vector<std::pair<ara::crypto::keys::SlotNumber, Operation>>>
      access_control_{}; /*!< The access control configuration */
};

}  // namespace config
}  // namespace server
}  // namespace crypto

#endif  // LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_CONFIG_ACCESS_CONTROL_CONFIG_H_
