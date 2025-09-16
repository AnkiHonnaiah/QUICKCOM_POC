/**********************************************************************************************************************
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
/*!        \file  throw_or_terminate.h
 *        \brief  ThrowOrTerminate implementation for differentiating between throw and terminate
 *         \unit  VaCommonLib::ErrorHandling::ThrowOrTerminate
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_LANGUAGE_THROW_OR_TERMINATE_H_
#define LIB_VAC_INCLUDE_VAC_LANGUAGE_THROW_OR_TERMINATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/generic/write_std_stream.h"
#include "vac/language/compile_exceptions.h"

namespace vac {
namespace language {
namespace internal {

// VECTOR NL AutosarC++17_10-A16.0.1: MD_VAC_A16.0.1_conditionalCompilationIsOnlyAllowedForIncludeGuards
#if defined(VCAST_ENABLE)
extern "C" {
void VCAST_DUMP_COVERAGE_DATA(void);
}
#endif

/*!
 * \brief Calls std::terminate.
 * \pre   -
 * \spec
 *  requires true;
 * \endspec
 */
[[noreturn]] inline void Terminate() noexcept {
  // VECTOR Disable AutosarC++17_10-A16.0.1: MD_VAC_A16.0.1_conditionalCompilationIsOnlyAllowedForIncludeGuards
#if defined(VCAST_ENABLE)
  VCAST_DUMP_COVERAGE_DATA();
#elif defined(_BullseyeCoverage)
  static_cast<void>(cov_write());
#endif
  // VECTOR Enable AutosarC++17_10-A16.0.1
  // VECTOR Next Construct AutosarC++17_10-A15.5.2: MD_VAC_A15.5.2_terminateByDesign
  std::terminate();
}

/*!
 * \brief Prints the message on the error console and calls terminate.
 * \pre   -
 * \param message Termination message.
 * \spec
 *  requires true;
 * \endspec
 */
[[noreturn]] inline void Terminate(char const* message) noexcept {
  static_cast<void>(amsr::generic::WriteToStderr(message));
  static_cast<void>(amsr::generic::WriteToStderr("\n"));
  Terminate();
}
/*!
 * \brief   Helper template to control the behavior of ThrowOrTerminate::Execute<ExceptionType>.
 * \details throw_exceptions Execute will throw, otherwise call will terminate.
 * \tparam  throw_exceptions Flag for enabling/disabling exceptions.
 * \trace   DSGN-VaCommonLib-ErrorHandling
 */
template <bool throw_exceptions>
class ThrowOrTerminateT;

/*!
 * \brief Overload when throw_exceptions ==  true.
 */
template <>
class ThrowOrTerminateT<true> {
 public:
  /*!
   * \brief     Throws the given error_code as the given Exception type.
   * \pre       -
   * \param[in] args Arguments passed to the thrown exception.
   * \tparam    ExceptionType Exception type to be thrown. Must inherit from std::exception.
   * \tparam    Args Argument types for exception construction. Must be able to construct an ExceptionType.
   * \tparam    with_exceptions The bool type to decide compile with/without exceptions.
   * \return
   * \spec
   *  requires true;
   * \endspec
   */
  template <class ExceptionType, typename... Args, bool with_exceptions = internal::kCompileWithExceptions,
            typename = std::enable_if_t<with_exceptions>>
  [[noreturn]] static auto Execute(Args&&... args) -> void {  // COV_LIBVAC_COMPILE_WITH_NO_EXCEPTIONS
    // VECTOR Next Line AutosarC++17_10-A16.0.1: MD_VAC_A16.0.1_conditionalCompilationIsOnlyAllowedForIncludeGuards
#ifdef __EXCEPTIONS
    // VECTOR Next Line AutosarC++17_10-A15.1.1: MD_VAC_A15.1.1_onlyInstancesOfTypesDerivedFromStdExceptionShallbeThrown
    throw ExceptionType(std::forward<Args>(args)...);
#else
    // The always_false bool is here to assure the clang9 compiler that ThrowOrTerminateT<true> can not be accessed when
    // kCompileWithExceptions is false.
    static constexpr bool always_false{(sizeof...(args)) != (sizeof...(args))};
    static_assert(always_false, "This cannot be compiled with -fno-exceptions.");

#endif
  }
};

/*!
 * \brief Overload when throw_exceptions == false.
 */
template <>
class ThrowOrTerminateT<false> {
 public:
  /*!
   * \brief Helper to cleanly discard parameter packs.
   */
  class ParameterSink final {
   public:
    /*!
     * \brief  Default contsructor. Discards the parameter pack.
     * \tparam Args Types of arguments to construct the ParameterSink.
     * \param  ... Input to construct the ParameterSink.
     * \pre    -
     * \spec
     *  requires true;
     * \endspec
     */
    template <typename... Args>
    explicit ParameterSink(Args const&...) noexcept {}

    /*!
     * \brief Deleted copy constructor.
     */
    ParameterSink(ParameterSink const&) = delete;

    /*!
     * \brief Deleted move constructor.
     */
    ParameterSink(ParameterSink&&) = delete;

    /*!
     * \brief Deleted copy assignment operator.
     * \return
     */
    ParameterSink& operator=(ParameterSink const&) = delete;

    /*!
     * \brief Deleted move assignment operator.
     * \return
     */
    ParameterSink& operator=(ParameterSink&&) = delete;

    /*!
     * \brief Default destructor.
     * \spec
     *  requires true;
     * \endspec
     */
    ~ParameterSink() noexcept = default;
  };

  // VECTOR Next Construct AutosarC++17_10-A3.9.1: MD_VAC_A3.9.1_baseTypeRequiredBySpecification
  /*!
   * \brief  Calls Terminate() with the given message.
   * \tparam ExceptionType Is ignored.
   * \param  message Error message.
   * \pre    -
   * \return
   * \spec
   *  requires true;
   * \endspec
   */
  template <class ExceptionType>
  [[noreturn]] static auto Execute(char const* message) noexcept -> void {
    Terminate(message);
  }

  /*!
   * \brief     Calls Terminate().
   * \tparam    ExceptionType Is ignored.
   * \tparam    Args Ignored arguments.
   * \param[in] args Ignored arguments.
   * \pre       -
   * \return
   * \spec
   *  requires true;
   * \endspec
   */
  template <class ExceptionType, typename... Args>
  [[noreturn]] static auto Execute(Args&&... args) noexcept -> void {
    // Omitting parameter name is not possible since doxygen enforces parameter documentation for parameter pack.
    static_cast<void>(ParameterSink{args...});
    Terminate();
  }
};
}  // namespace internal

/*!
 * \brief     Throws the specified exception or terminates when exceptions are deactivated.
 * \tparam    ExceptionType Exception type. Must inherit from std::exception.
 * \tparam    Args Argument types for exception construction. Must be able to construct an ExceptionType.
 * \param[in] args Arguments for exception construction.
 * \pre       -
 * \spec
 *  requires true;
 * \endspec
 */
template <class ExceptionType, typename... Args,
          std::enable_if_t<(std::is_constructible<ExceptionType, Args...>::value) &&
                               (std::is_base_of<std::exception, ExceptionType>::value),
                           std::int32_t> = 0>
[[noreturn]] void ThrowOrTerminate(Args&&... args) {
  /*!
   * \brief An alias for internal::ThrowOrTerminateT<..>.
   */
  using ThrowOrTerminateType = internal::ThrowOrTerminateT<internal::kCompileWithExceptions>;
  ThrowOrTerminateType::Execute<ExceptionType>(std::forward<Args>(args)...);
}

}  // namespace language
}  // namespace vac

#endif  // LIB_VAC_INCLUDE_VAC_LANGUAGE_THROW_OR_TERMINATE_H_

//  COV_JUSTIFICATION_BEGIN
//    \ID COV_LIBVAC_COMPILE_WITH_NO_EXCEPTIONS
//      \ACCEPT XX
//      \REASON The function is designed to throw an exeception and should not be used when compiling without exception
//              support. Therefore, a flag is provided to enable/disable compiling with exceptions. However, function
//              cannot be covered because a compile-time flag prevents it to be called.
//  COV_JUSTIFICATION_END
