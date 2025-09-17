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
/**        \file  client_id_generator.h
 *        \brief  A class to manage the generation and release of the client Ids.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_CLIENT_ID_GENERATOR_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_CLIENT_ID_GENERATOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <bitset>
#include <utility>
#include "amsr/someip_daemon_core/client/client_id_generator_interface.h"
#include "amsr/someip_daemon_core/someip_daemon_error_code.h"
#include "ara/core/result.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace client {

/*!
 * \brief ClientIdGenerator class used for managing client Ids.
 * \tparam MaxClientIds The maximum amount of client IDs this ClientIdGenerator can assign in parallel.
 */
template <std::size_t MaxClientIds>
class ClientIdGenerator final : public ClientIdGeneratorInterface {
 public:
  /*!
   * \brief   Define default constructor.
   * \steady  FALSE
   */
  ClientIdGenerator() noexcept = default;
  /*!
   * \brief   Define default destructor.
   * \steady  FALSE
   */
  // VECTOR NC AutosarC++17_10-A10.3.1: MD_SomeIpDaemon_AutosarC++17_10_A10.3.1_final_keyword_needed_false_positive
  // VECTOR NC AutosarC++17_10-A10.3.2: MD_SomeIpDaemon_AutosarC++17_10_A10.3.2_final_keyword_needed_false_positive
  // VECTOR NL AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  ~ClientIdGenerator() noexcept = default;

  ClientIdGenerator(ClientIdGenerator const&) = delete;
  ClientIdGenerator(ClientIdGenerator&&) = delete;
  ClientIdGenerator& operator=(ClientIdGenerator const&) & = delete;
  ClientIdGenerator& operator=(ClientIdGenerator&&) & = delete;

  /*!
   * \brief Generate client Id.
   *
   * \return The generated client Id.
   * \pre       -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Initialize the return value to an overflow error
   * - Initialize the amount of loop iterations to 0 and initiate loop
   *   - While we have not iterated through the entire bitset of possible client IDs
   *   - If the client id at the current search index is not assigned
   *     - Mark the current client id as assigned
   *     - Set the assigned client ID as the return value of this API
   *     - Increment the search index
   *     - Exit the loop
   *   - Increment the loop iteration count
   *   - Increment the search index
   * - Return the allocated client Id
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  ara::core::Result<amsr::someip_protocol::internal::ClientId> GenerateClientId() noexcept override {
    ara::core::Result<amsr::someip_protocol::internal::ClientId> client_id_result{
        ara::core::Result<amsr::someip_protocol::internal::ClientId>::FromError(SomeIpDaemonErrc::client_ids_overflow)};

    // Iterate in the worst case the entire bitset. That is, a max amount of MaxClientIds iterations
    std::size_t it_count{0U};
    while (it_count < MaxClientIds) {
      if (ClientIdGenerator::client_ids_[search_index_] == 0U) {
        // Available ClientId found. Mark it as busy.
        ClientIdGenerator::client_ids_.set(search_index_);
        // Return the allocated ClientId
        client_id_result.EmplaceValue(static_cast<amsr::someip_protocol::internal::ClientId>(search_index_));

        // Increment search index and handle overflow
        IncrementSearchIndex();

        // Exit the search loop
        break;
      }

      // Increment the loop count
      ++it_count;

      // Increment search index and handle overflow
      IncrementSearchIndex();
    }

    return client_id_result;
  }

  /*!
   * \brief Release client Id.
   *
   * \param[in] client_id A SOME/IP client identifier.
   *
   * \return "true" if the client Id released successfully, and "false" otherwise.
   * \pre       -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Set the default return value as error (false)
   * - If the given client ID is within the range of allowed client IDs
   *   - If the client ID was already assigned
   *     - Mark the client ID as not assigned
   *     - Set the return value to true
   * - Return the result
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  bool ReleaseClientId(amsr::someip_protocol::internal::ClientId const client_id) noexcept override {
    bool result{false};

    // An error shall be returned if the given client id is too large
    if (client_id < MaxClientIds) {
      // Check the assignment status of this ClientId
      if (ClientIdGenerator::client_ids_[client_id] == 1U) {
        ClientIdGenerator::client_ids_.reset(client_id);
        result = true;
      }
    }
    return result;
  }

 private:
  /*!
   * \brief Increments the internal seach index and handles overflow
   * \pre       -
   * \context   App
   * \reentrant FALSE
   * \steady    FALSE
   *
   * \internal
   * - Increment the search index
   * - If the search index overflows (equals the max amount of client IDs)
   *   - Reset the search index
   * \endinternal
   */
  void IncrementSearchIndex() noexcept {
    // Increment search index and handle overflow
    ++search_index_;
    if (search_index_ == MaxClientIds) {
      search_index_ = 0;
    }
  }

  /*!
   * \brief SOME/IP client identifier allocation bit mask.
   */
  std::bitset<MaxClientIds> client_ids_{};

  /*!
   * \brief   The search index that indicates where in the bitset to start looking for the next available client ID
   * \details The goal of this variable is to always assign an increasing ClientId upon consecutive requests, even
   *          when those client IDs are released in-between.
   */
  std::size_t search_index_{0U};
};

}  // namespace client
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CLIENT_CLIENT_ID_GENERATOR_H_
