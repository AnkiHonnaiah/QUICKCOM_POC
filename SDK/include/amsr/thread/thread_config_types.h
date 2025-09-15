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
 *        \brief  Configuration structure for a thread.
 *
 *********************************************************************************************************************/
#ifndef LIB_VATHREAD_INCLUDE_AMSR_THREAD_THREAD_CONFIG_TYPES_H_
#define LIB_VATHREAD_INCLUDE_AMSR_THREAD_THREAD_CONFIG_TYPES_H_

#include "amsr/core/optional.h"
#include "amsr/core/string.h"
#include "amsr/core/string_view.h"
#include "amsr/thread/common.h"
#include "ara/core/vector.h"

namespace amsr {
namespace thread {

/*!
 * \brief Type for thread scheduling priority.
 */
using SchedulingPriority = std::uint8_t;

/*!
 * \brief Type for thread nice value.
 */
using NiceValue = std::int32_t;

/*!
 * \brief The supported scheduling policies.
 */
enum class SchedulingPolicy : std::int32_t {
  kOther = SCHED_OTHER,  /*!< OS-specific policy. */
  kFifo = SCHED_FIFO,    /*!< Real-time: First in-first out (FIFO) scheduling policy. */
  kRoundRobin = SCHED_RR /*!< Real-time: Round robin scheduling policy. */
};

/*!
 * \brief The class that provides the fields that are supported for the configuration of a thread.
 * \trace DSGN-VectorThreadLibrary-ThreadConfiguration
 * \vpublic
 */
class ThreadConfig final {
 public:
  /*!
   * \brief Constructor for ThreadConfig.
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  ThreadConfig() noexcept = default;

  /*!
   * \brief Destructor for ThreadConfig.
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  ~ThreadConfig() noexcept = default;

  /*!
   * \brief Default Copy constructor.
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  ThreadConfig(ThreadConfig const&) = default;

  /*!
   * \brief Default Copy assignment.
   * \return Reference to itself.
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  ThreadConfig& operator=(ThreadConfig const&) & noexcept = default;

  /*!
   * \brief Default Move constructor.
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  ThreadConfig(ThreadConfig&&) noexcept = default;  // VCA_THREAD_MOLE_1298, VCA_THREAD_EXT_CALL_CTOR

  /*!
   * \brief Default Move assignment.
   * \return Reference to itself.
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  ThreadConfig& operator=(ThreadConfig&&) & noexcept = default;

  /*!
   * \brief           Common constructor for ThreadConfig class.
   * \param[in]       thread_stack_size  Stack size of the thread.
   * \param[in]       identifier         ThreadConfiguration Identifier.
   * \param[in]       sched_priority     Scheduling priority of the thread.
   * \param[in]       sched_policy       Scheduling policy of the thread.
   * \param[in]       nice_value         nice value of the thread.
   * \vprivate Vector product internal API
   */
  ThreadConfig(ara::core::String identifier, ara::core::Optional<SchedulingPriority> sched_priority,
               ara::core::Optional<SchedulingPolicy> sched_policy, ara::core::Optional<NiceValue> nice_value,
               std::size_t thread_stack_size) noexcept
      : stack_size_in_bytes_{thread_stack_size},
        identifier_{std::move(identifier)},
        sched_priority_{sched_priority},
        sched_policy_{sched_policy},
        nice_value_{nice_value} {}

  /*!
   * \brief             Getter for threadconfiguration identifier.
   * \return            Threadconfiguration identifier.
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   * \internal
   *  - Return identifier.
   * \endinternal
   */
  ara::core::String GetIdentifier() const noexcept { return identifier_; }  // COV_THREAD_TOOLINGISSUE
  // Covered by: UT__ThreadConfigurationAPI, ThreadConfig_Ctor

  /*!
   * \brief             Getter for threadconfiguration scheduling priority.
   * \return            Threadconfiguration scheduling priority.
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   * \internal
   *  - Return scheduling priority.
   * \endinternal
   */
  ara::core::Optional<SchedulingPriority> GetSchedulingPriority() const noexcept {
    return sched_priority_;
  }  // COV_THREAD_TOOLINGISSUE
  // Covered by: UT__ThreadConfigurationAPI, ThreadConfig_Ctor

  /*!
   * \brief             Getter for threadconfiguration scheduling policy.
   * \return            Threadconfiguration scheduling policy.
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   * \internal
   *  - Return scheduling policy.
   * \endinternal
   */
  ara::core::Optional<SchedulingPolicy> GetSchedulingPolicy() const noexcept {
    return sched_policy_;
  }  // COV_THREAD_TOOLINGISSUE
     // Covered by: UT__ThreadConfigurationAPI, ThreadConfig_Ctor

  /*!
   * \brief             Getter for threadconfiguration nice value.
   * \return            Threadconfiguration nice value.
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   * \internal
   *  - Return nice value.
   * \endinternal
   */
  ara::core::Optional<NiceValue> GetNiceValue() const noexcept {
    return nice_value_;
  }  // COV_THREAD_TOOLINGISSUE
     // Covered by: UT__ThreadConfigurationAPI, ThreadConfig_Ctor

  /*!
   * \brief             Getter for threadconfiguration stack size.
   * \return            Threadconfiguration stack size.
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   * \internal
   *  - Return stack size.
   * \endinternal
   */
  std::size_t GetStackSize() const noexcept {
    return stack_size_in_bytes_;
  }  // COV_THREAD_TOOLINGISSUE
     // Covered by: UT__ThreadConfigurationAPI, ThreadConfig_Ctor

 private:
  /*!
   * \brief The stack size in bytes to start a thread with.
   * \details The stack size must be a multiple of the page size. The default value is set to kDefaultThreadStackSize.
   */
  std::size_t stack_size_in_bytes_{kDefaultThreadStackSize};

  /*!
   * \brief The identifier of thread configuration.
   * \details This is a unique identifier for thread configuration. It equals SHORT-NAME of BswThreadConfiguration node.
   */
  amsr::core::String identifier_{};

  /*!
   * \brief The thread scheduling priority.
   * \details OS dependent value. The default value (empty optional) means that scheduling priority will be inherited
   * from parent.
   */
  amsr::core::Optional<SchedulingPriority> sched_priority_{};

  /*!
   * \brief The thread scheduling policy.
   * \details OS dependent value. The default value is inherited from parent.
   */
  amsr::core::Optional<SchedulingPolicy> sched_policy_{};

  /*!
   * \brief The thread nice value.
   * \details Only relevant on Linux
   */
  amsr::core::Optional<NiceValue> nice_value_{};
};

namespace internal {
/*!
 * \brief The thread configurations list.
 */
using ThreadConfigList = ara::core::Vector<amsr::thread::ThreadConfig>;

}  // namespace internal

}  // namespace thread
}  // namespace amsr

#endif  // LIB_VATHREAD_INCLUDE_AMSR_THREAD_THREAD_CONFIG_TYPES_H_

// COV_JUSTIFICATION_BEGIN
// \ID COV_THREAD_TOOLINGISSUE
//   \ACCEPT XX
//   \REASON The branch is already covered by a test case but VCast does not show correct coverage measurements.
// COV_JUSTIFICATION_END
