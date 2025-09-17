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
 *        \brief  A work unit for the thread pool.
 *
 *********************************************************************************************************************/
#ifndef LIB_VATHREAD_INCLUDE_AMSR_THREAD_WORK_UNIT_H_
#define LIB_VATHREAD_INCLUDE_AMSR_THREAD_WORK_UNIT_H_

#include <memory>

namespace amsr {
namespace thread {

/*!
 * \brief This class represents a unit of work that can be submitted to the ThreadPool.
 * \trace DSGN-VectorThreadLibrary-WorkUnit
 * \vpublic
 */
class WorkUnit {
 public:
  /*!
   * \brief Construct a WorkUnit.
   * \context               ANY
   * \pre                   -
   * \threadsafe            TRUE
   * \reentrant             FALSE
   * \synchronous           TRUE
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  WorkUnit() noexcept = default;

  /*!
   * \brief Destruct a WorkUnit.
   * \context               ANY
   * \pre                   -
   * \threadsafe            FALSE
   * \reentrant             FALSE
   * \synchronous           TRUE
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  virtual ~WorkUnit() noexcept = default;
  /*!
   * \brief Method to execute the WorkUnit.
   * \context               Thread of a ThreadPool.
   * \pre                   -
   * \threadsafe            FALSE
   * \reentrant             FALSE
   * \synchronous           TRUE
   * \vpublic
   */
  virtual void Run() noexcept = 0;

 protected:
  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_THREAD_A15.5.1_NoExceptFunctionDeleted
  /*!
   * \brief Copy constructor deleted.
   */
  WorkUnit(WorkUnit const& work_unit) noexcept = delete;
  /*!
   * \brief Copy assign deleted.
   */
  WorkUnit& operator=(WorkUnit const& work_unit) & noexcept = delete;
  /*!
   * \brief Move construct a WorkUnit.
   * \param[in,out] work_unit The WorkUnit to move from.
   * \context               ANY
   * \pre                   -
   * \threadsafe            TRUE for different work_unit
   * \reentrant             FALSE
   * \synchronous           TRUE
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  WorkUnit(WorkUnit&& work_unit) noexcept = default;
  /*!
   * \brief Move assign a WorkUnit.
   * \param[in,out] work_unit The WorkUnit to move from.
   * \return                Reference to itself.
   * \context               ANY
   * \pre                   -
   * \threadsafe            TRUE for different work_unit
   * \reentrant             FALSE
   * \synchronous           TRUE
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  WorkUnit& operator=(WorkUnit&& work_unit) & noexcept = default;
};

}  // namespace thread
}  // namespace amsr

#endif  // LIB_VATHREAD_INCLUDE_AMSR_THREAD_WORK_UNIT_H_
