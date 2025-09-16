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
/**        \file  task.h
 *        \brief  Defines Task class that models one abstract task to be implemented by a derived class.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_TASK_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_TASK_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace amsr {
namespace socal {
namespace internal {

/*!
 * \brief Models one abstract task to be implemented by a derived class. Concrete methods of a skeleton derive from this
 *        base-class. These pending requests are then stored in a global request queue for later execution.
 *
 * \unit Socal::Task
 */
class Task {
 public:
  /*!
   * \brief Name alias for key type.
   */
  using Key = void const*;

  /*!
   * \brief Constructor.
   * \param[in] key  The key associated with this task. It is used to remove the tasks later if needed. Must not be
   *                 null.
   * \pre     -
   * \context ANY
   * \steady TRUE
   * \spec requires key != nullptr; \endspec
   * \internal
   * - If the Key argument is null.
   *   - Log fatal and abort.
   * \endinternal
   */
  explicit Task(Key key) noexcept;

  /*!
   * \brief Delete default constructor.
   * \steady TRUE
   */
  Task() = delete;

  /*!
   * \brief Define destructor.
   * \steady TRUE
   * \spec requires true; \endspec
   */
  virtual ~Task() noexcept = default;

  /*!
   * \brief Delete copy assignment.
   */
  auto operator=(Task const&) & -> Task& = delete;
  /*!
   * \brief Delete move assignment.
   */
  auto operator=(Task&&) & -> Task& = delete;

  /*!
   * \brief Pure virtual method, to be realized by derived classes.
   * \steady TRUE
   */
  virtual void operator()() = 0;

 protected:
  /*!
   * \brief Define copy constructor.
   * \steady TRUE
   */
  Task(Task const&) noexcept = default;
  /*!
   * \brief Define move constructor.
   * \steady TRUE
   */
  Task(Task&&) noexcept = default;

 private:
  /*!
   * \brief Key associated to the task.
   */
  Key key_;
};

}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_TASK_H_
