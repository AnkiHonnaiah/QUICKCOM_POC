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
/*!        \file
 *        \brief  Utility function for delegating unhandled ErrorCode values to a global default error handler.
 *         \unit  VaCommonLib::ErrorHandling::HandleErrors
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_VAC_LANGUAGE_HANDLE_ERRORS_H_
#define LIB_VAC_INCLUDE_VAC_LANGUAGE_HANDLE_ERRORS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <type_traits>
#include <utility>

#include "ara/core/error_code.h"
#include "vac/language/detail/callable_check.h"

namespace vac {
namespace language {

/*!
 * \brief   Interface for a global default error handler.
 * \details Subclasses may be registered as a error handler for errors which are not handled by user defined functions
 *          in HandleErrors. Use SetGlobalDefaultErrorHandler() to register a new global default error handler.
 */
class GlobalDefaultErrorHandler {
 public:
  /*!
   * \brief Destructor.
   * \spec
   *  requires true;
   * \endspec
   */
  virtual ~GlobalDefaultErrorHandler() noexcept;

  /*!
   * \brief Default constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  GlobalDefaultErrorHandler() noexcept = default;

  /*!
   * \brief  Deleted copy operator.
   * \return
   */
  GlobalDefaultErrorHandler& operator=(GlobalDefaultErrorHandler const&) = delete;

  /*!
   * \brief  Deleted move operator.
   * \return
   */
  GlobalDefaultErrorHandler& operator=(GlobalDefaultErrorHandler&&) = delete;

  /*!
   * \brief Function to overload for unhandled errors.
   * \param error Const reference of ErrorCode to be overloaded from.
   * \pre   -
   * \spec
   *  requires true;
   * \endspec
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_VAC_M9.3.3_methodCanBeDeclaredConstVirtual
  virtual void operator()(ara::core::ErrorCode const& error) = 0;

 protected:
  /*!
   * \brief Default copy constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  GlobalDefaultErrorHandler(GlobalDefaultErrorHandler const&) noexcept = default;

  /*!
   * \brief Default move constructor.
   * \spec
   *  requires true;
   * \endspec
   */
  GlobalDefaultErrorHandler(GlobalDefaultErrorHandler&&) noexcept = default;
};

// VECTOR NC AutosarC++17_10-M3.2.4: MD_VAC_M3.2.4_functionWithoutDefinition
/*!
 * \brief     Sets the global default error handler. Overrides the previous one. The initial global default error
 *            handler aborts with the message from the error.
 * \param[in] handler New global default error handler.
 * \pre       -
 * \spec
 *  requires true;
 * \endspec
 * \trace     DSGN-VaCommonLib-ErrorHandling
 */
void SetGlobalDefaultErrorHandler(GlobalDefaultErrorHandler& handler) noexcept;

// VECTOR NC AutosarC++17_10-M3.2.4: MD_VAC_M3.2.4_functionWithoutDefinition
/*!
 * \brief  Retrieves the current global default error handler.
 * \pre    -
 * \return Global default ErrorHandler.
 * \spec
 *  requires true;
 * \endspec
 */
GlobalDefaultErrorHandler& GetGlobalDefaultErrorHandler() noexcept;

/*! \brief Tag type for the else error handler.
 */
struct ElseErrorHandlerT {};

// VECTOR Next Construct AutosarC++17_10-M3.4.1: MD_VAC_M3.4.1_symbolsCanBeDeclaredLocally
/*! \brief Matcher to pass to HandleErrors for the else error handler. Can only be used as the last matcher.
 */
static constexpr ElseErrorHandlerT else_error_handler;

namespace detail {

// VECTOR Next Construct AutosarC++17_10-A12.4.1: MD_VAC_A12.4.1_noVirtualDtor
/*! \brief  Type containing a list of typed error codes matching for a single error handler.
 *  \tparam Codes The types in sequence to create ErrorList.
 */
template <typename... Codes>
class ErrorList;

// VECTOR Next Construct AutosarC++17_10-A10.2.1: MD_VAC_A10.2.1_redefinitionOfNonVirtualFunction
/*!
 * \brief  Recursive template storing a single typed error code per level.
 *         Provides a matches predicate returning whether the list matches a specific error.
 * \tparam Code The passed type to create ErrorList.
 * \tparam Codes The types in sequence to create ErrorList.
 */
template <typename Code, typename... Codes>
class ErrorList<Code, Codes...> : ErrorList<Codes...> {
 public:
  /*! \brief Stored code type
   */
  using CodeT = std::decay_t<Code>;

  /*!
   * \brief     Constructor
   * \param[in] code Code to construct ErrorList.
   * \param[in] error_codes Error code to construct ErrorList.
   * \pre       -
   * \spec
   *  requires true;
   * \endspec
   */
  explicit constexpr ErrorList(CodeT code, Codes&&... error_codes) noexcept
      : ErrorList<Codes...>(std::move(error_codes)...), code_(code) {}

  /*!
   * \brief  Returns whether this list matches the error
   * \param  error Error to be examined.
   * \pre    -
   * \return True if error match the code, otherwise false.
   * \spec
   *  requires true;
   * \endspec
   */
  constexpr bool matches(ara::core::ErrorCode const& error) const noexcept {
    return (error == code_) || (ErrorList<Codes...>::matches(error));  // VCA_VAC_VALID_ARGUMENT
  }

  /*! \brief One code in the list. */
  CodeT const code_;
};

// VECTOR Next Construct AutosarC++17_10-A12.4.1: MD_VAC_A12.4.1_noVirtualDtor
/*! \brief Recursion tail which matches nothing.
 */
template <>
class ErrorList<> {
 public:
  /*!
   * \brief  Recursion tail which matches nothing.
   * \pre    -
   * \return False.
   * \spec
   *  requires true;
   * \endspec
   */
  constexpr static bool matches(ara::core::ErrorCode const&) noexcept { return false; }
};

/*! \brief Returns whether a type is ErrorList, default false.
 */
template <typename>
struct IsErrorList : std::false_type {};

/*! \brief  Returns whether a type is ErrorList, specialization for true.
 *  \tparam Args The input parameter types of the callable.
 */
template <typename... Args>
struct IsErrorList<ErrorList<Args...>> : std::true_type {};

/*! \brief  Template providing matches predicate depending on input type.
 *  \tparam T The type of the callable.
 */
template <typename T, bool = std::is_enum<std::decay_t<T>>::value, bool = IsErrorList<std::decay_t<T>>::value,
          bool = std::is_same<std::decay_t<T>, ElseErrorHandlerT>::value>
class ErrorMatcher;

/*! \brief  Matcher predicate for a single ErrorCode.
 *  \tparam EnumT Predicate type for ErrorMatcher.
 */
template <typename EnumT>
class ErrorMatcher<EnumT, true, false, false> {
 public:
  /*!
   * \brief     Matcher predicate for a single ErrorCode.
   * \param     error ErrorCode to be examined.
   * \param     errc Prediction of the ErrorCode.
   * \pre       -
   * \return    True if ErrorCode matches the prediction, otherwise false.
   * \spec
   *  requires true;
   * \endspec
   */
  static bool matches(ara::core::ErrorCode const& error, EnumT errc) noexcept {
    return error == errc;  // Comment to force line-break. See TAR-22835.
  }
};

/*! \brief  Matcher predicate for an ErrorList.
 *  \tparam ErrorListT Predicate type for ErrorMatcher.
 */
template <typename ErrorListT>
class ErrorMatcher<ErrorListT, false, true, false> {
 public:
  /*!
   * \brief  Matcher predicate for an ErrorList.
   * \param  error ErrorCode to be examined.
   * \param  error_list Prediction of the ErrorCode.
   * \pre    -
   * \return True if ErrorCode matches the prediction, otherwise false.
   * \spec
   *  requires true;
   * \endspec
   */
  static bool matches(ara::core::ErrorCode const& error, ErrorListT const& error_list) noexcept {
    return error_list.matches(error);
  }
};

/*! \brief  Matcher predicate for a user provided else error handler.
 *  \tparam ElseT Predicate type for ErrorMatcher.
 */
template <typename ElseT>
class ErrorMatcher<ElseT, false, false, true> {
 public:
  /*!
   * \brief  Else error handler is only allowed as the last link and always matches.
   * \pre    -
   * \return True.
   * \spec
   *  requires true;
   * \endspec
   */
  constexpr static bool matches(ara::core::ErrorCode const&, ElseT) noexcept { return true; }
};

/*! \brief  Template which applies an error handler if a matcher matches.
 *  \tparam Args The input parameter types of the callable.
 */
template <typename... Args>
class HandleErrorsAux;

/*! \brief Applies global default error handler when no user provided matcher matches.
 */
template <>
class HandleErrorsAux<> {
 public:
  /*!
   * \brief Calls the global default error handler when no user provided matcher matches.
   * \param error The ErrorCode to be handled.
   * \pre   -
   * \spec
   *  requires true;
   * \endspec
   */
  static void handle(ara::core::ErrorCode const& error) noexcept {
    GetGlobalDefaultErrorHandler()(error);  // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
  }
};

// VECTOR Next Construct AutosarC++17_10-A18.9.2: MD_VAC_A18.9.2_useStdMoveIfTheValueIsARvalueReference
/*!
 * \brief  Uses two types from the input list, a matcher and an error handler.
 * \tparam T matcher type which is an Enum, ErrorList, or ElseErrorHandlerT.
 * \tparam F error handler a callable accepting an ErrorCode const reference.
 * \tparam Args The types of arguments of the callable.
 */
template <typename T, typename F, typename... Args>
class HandleErrorsAux<T, F, Args...> {
 public:
  /*!
   * \brief      Handles an error using provided handler if matcher matches, otherwise forwards to next pair of matcher
   *             and handler.
   * \details    If the provided handler callable is a nullptr and the matcher matches then the default
   *             error handler is called instead
   * \param      error The error to be handled.
   * \param      errc Matcher.
   * \param[in]  f The error handler callable.
   * \param[in]  args A list of matcher and handler pairs.
   * \pre        -
   * \spec
   *  requires true;
   * \endspec
   */
  static void handle(ara::core::ErrorCode const& error, T const& errc, F&& f, Args&&... args) noexcept {
    static_assert((!std::is_same<std::decay_t<T>, ElseErrorHandlerT>::value) || (0 == sizeof...(args)),
                  "ElseErrorHandler can only be used as the last handler!");
    if (detail::ErrorMatcher<T>::matches(error, errc)) {
      bool const is_null_ptr{IsCallableNullPtr(f)};
      if (is_null_ptr) {
        GetGlobalDefaultErrorHandler()(error);  // VCA_VAC_VALID_FUNC_CALL_COMPILE_TIME_CHECK
      } else {
        std::forward<F>(f)(error);  // VCA_VAC_FUNC_FULFILL_CONTRACT_RUN_TIME_CHECK
      }
    } else {
      HandleErrorsAux<Args...>::handle(error, std::forward<Args>(args)...);  // VCA_VAC_VALID_ARGUMENT
    }
  }
};
}  // namespace detail

/*!
 * \brief   Handles an error with an error handler selected using the first positive matcher.
 * \details If no matcher matches, the error will be routed to the global default error handler. To specify a local
 *          default error handler, use the any-matcher tag vac::language::else_error_handler with your handler as the
 *          last parameters.
 *          Example: HandleErrors(res.Error(),
 *                                PosixErrc::invalid_argument, [](ErrorCode const&) {},
 *                                ErrorList(PosixErrc::argument_out_of_domain, PosixErrc::argument_list_too_long),
 *                                [](ErrorCode const&) {});
 *          Example: HandleErrors(res.Error(),
 *                                PosixErrc::invalid_argument, [](ErrorCode const&) {},
 *                                else_error_handler, [](ErrorCode const&) { handleMyElseCase(); });
 * \param     error error to be handled.
 * \param[in] args a list of matcher and handler pairs, where a matcher is an Enum, ErrorList, or ElseErrorHandlerT.
 * \tparam    Args Args List of matcher and handler type pairs, where matcher is an Enum, ErrorList, or
 *            ElseErrorHandlerT.
 * \pre       -
 * \spec
 *  requires true;
 * \endspec
 * \trace     DSGN-VaCommonLib-ErrorHandling
 */
template <typename... Args>
void HandleErrors(ara::core::ErrorCode const& error, Args&&... args) noexcept {
  detail::HandleErrorsAux<Args...>::handle(error, std::forward<Args>(args)...);
}

/*!
 * \brief     Produces an ErrorList which matches all of its contained error codes.
 * \tparam    Args enum types of error codes.
 * \param[in] args enum values of error codes.
 * \pre       -
 * \return    error list type.
 * \spec
 *  requires true;
 * \endspec
 */
template <typename... Args>
constexpr auto ErrorList(Args&&... args) noexcept -> detail::ErrorList<Args...> {
  return detail::ErrorList<Args...>{std::forward<Args>(args)...};
}

}  // namespace language
}  // namespace vac

#endif  // LIB_VAC_INCLUDE_VAC_LANGUAGE_HANDLE_ERRORS_H_
