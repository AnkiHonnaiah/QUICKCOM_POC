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
 *        \brief   Defines the thread running state values.
 *
 *********************************************************************************************************************/
#ifndef LIB_VATHREAD_INCLUDE_AMSR_THREAD_THREAD_RUNNING_STATE_H_
#define LIB_VATHREAD_INCLUDE_AMSR_THREAD_THREAD_RUNNING_STATE_H_

namespace amsr {
namespace thread {

/*!
 * \brief Thread running states.
 * \vpublic
 */
enum class ThreadRunningState : std::uint8_t {
  kUninitialized = 1, /*!< Not started. */
  kRunning = 2,       /*!< Running. */
  kCompleted = 3,     /*!< Completed. */
};

}  // namespace thread
}  // namespace amsr

#endif  // LIB_VATHREAD_INCLUDE_AMSR_THREAD_THREAD_RUNNING_STATE_H_
