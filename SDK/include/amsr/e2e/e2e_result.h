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
/*!        \file  e2e_result.h
 *        \brief  Result type used for the Transformer helper class.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBE2E_INCLUDE_AMSR_E2E_E2E_RESULT_H_
#define LIB_LIBE2E_INCLUDE_AMSR_E2E_E2E_RESULT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/e2e/e2e_types.h"

namespace amsr {
namespace e2e {

/*!
 * \brief Class providing the E2ECheckStatus and E2EState result.
 * \unit amsr::e2e::Result
 * \vprivate Vector Internal API
 * \trace SPEC-12100823
 */
class Result final {
 public:
  /*!
   * \brief Constructor of result sets the state of the state machine and the status from the check.
   * \param[in] state        State of the state machine
   * \param[in] check_status Most recent check status sample
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  Result(amsr::e2e::state_machine::State state, amsr::e2e::state_machine::CheckStatus check_status) noexcept
      : sm_state_{state}, check_status_{check_status} {}

  /*!
   * \brief Receive the latest check status.
   * \return Latest check status
   * \context ANY
   * \pre -
   * \threadsafe   TRUE
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       TRUE
   */
  amsr::e2e::state_machine::CheckStatus GetCheckStatus() const noexcept { return check_status_; }

  /*!
   * \brief Get the latest state from the STM based on the latest check.
   * \return Latest State Machine state
   * \context ANY
   * \pre -
   * \threadsafe   TRUE
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady       TRUE
   */
  amsr::e2e::state_machine::State GetState() const noexcept { return sm_state_; }

  /*!
   * \brief Compare two results.
   * \param[in] rhs The other result object to compare to.
   * \return true if state and check status; false if one of them differ.
   * \context ANY
   * \pre -
   * \threadsafe   TRUE
   * \reentrant    FALSE
   * \synchronous  TRUE
   * \steady TRUE
   * \vprivate Vector Internal API
   */
  inline bool operator==(Result const& rhs) const noexcept {
    return ((sm_state_ == rhs.sm_state_) && (check_status_ == rhs.check_status_));
  }

 private:
  /*!
   * \brief The E2EState of the result
   */
  amsr::e2e::state_machine::State sm_state_;
  /*!
   * \brief The E2ECheckStatus of the result
   */
  amsr::e2e::state_machine::CheckStatus check_status_;
};

/*!
 * \brief Type-alias for compatibility due to consistency issues in the COM SWS.
 */
using E2EResult = Result;

}  // namespace e2e
}  // namespace amsr

#endif  // LIB_LIBE2E_INCLUDE_AMSR_E2E_E2E_RESULT_H_
