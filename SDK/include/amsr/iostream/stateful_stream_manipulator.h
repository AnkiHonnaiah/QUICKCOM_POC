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
 *        \brief   Manipulates stream properties.
 *
 *********************************************************************************************************************/
#ifndef LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_STATEFUL_STREAM_MANIPULATOR_H_
#define LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_STATEFUL_STREAM_MANIPULATOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/iostream/output_stream.h"
#include "amsr/iostream/stateful_output_stream.h"
#include "vac/language/function_ref.h"

namespace amsr {
namespace stream {

// VECTOR NC Metric-OO.WMC.One: MD_IoStream_Metric-OO.WMC.One_interface
/*!
 * \brief Manipulates stream properties.
 * \tparam T type of manipulator function.
 * \unit amsr::stream::StatefulStreamManipulator
 * \vprivate Product Private
 */
template <class T>
class StatefulStreamManipulator final {
 public:
  /*!
   * \brief Constructs instance of class.
   * \param[in] manipulator Function reference which manipulates the stream.
   * \param[in] manipulator_arg Arguments for manipulator.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  StatefulStreamManipulator(vac::language::FunctionRef<StatefulOutputStream&(StatefulOutputStream&, T)> manipulator,
                            T manipulator_arg)
      : function_{manipulator}, function_argument_{manipulator_arg} {}

  // StatefulStreamManipulator is not copy constructible because a stream is not copy constructible.
  StatefulStreamManipulator(StatefulStreamManipulator const& stream) = delete;

  // StatefulStreamManipulator is not copy assignable because a stream is not copy assignable.
  StatefulStreamManipulator& operator=(StatefulStreamManipulator const& stream) & = delete;

  /*!
   * \brief Move constructs instance of class.
   * \param[in] stream Instance of StatefulStreamManipulator
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  StatefulStreamManipulator(StatefulStreamManipulator&& stream) noexcept = default;

  // vac::language::FunctionRef is not move assignable
  StatefulStreamManipulator& operator=(StatefulStreamManipulator&& stream) & noexcept = delete;

  /*!
   * \brief Destructs instance.
   * \context ANY
   * \pre -
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  ~StatefulStreamManipulator() noexcept = default;

  /*!
   * \brief Get manipulator function.
   * \tparam T type of function argument.
   * \return Reference to the function.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  vac::language::FunctionRef<StatefulOutputStream&(StatefulOutputStream&, T)> const& GetFunction() const noexcept {
    return function_;
  }

  /*!
   * \brief Get arguments of manipulator function.
   * \tparam T type of argument.
   * \return The argument.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \spec requires true; \endspec
   * \vprivate Product Private
   * \steady UNKNOWN
   */
  T GetFunctionArgument() const noexcept { return function_argument_; }

 private:
  /*! Manipulator function. */
  vac::language::FunctionRef<StatefulOutputStream&(StatefulOutputStream&, T)> function_;
  /*! Manipulator function argument. */
  T function_argument_;
};

/*!
 * \brief Changes stream properties as required by manipulator.
 * \details Overloaded inserter.
 * \param[in] sos The stateful output stream for which the properties of the underlying stream should be modified.
 * \param[in] manipulator The stream manipulator.
 * \return The same reference of stream.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \spec requires true; \endspec
 * \steady UNKNOWN
 * \vprivate Product Private
 */
template <class T>
StatefulOutputStream& operator<<(StatefulOutputStream& sos, StatefulStreamManipulator<T> const& manipulator) noexcept {
  // VCA_IOSTREAM_STREAM_REF, VCA_IOSTREAM_STREAM_MANIPULATOR
  return manipulator.GetFunction()(sos, manipulator.GetFunctionArgument());
}

/*!
 * \brief Applicator to change fill character in underlying stream.
 * \param[in] c Fill character.
 * \return Stream manipulator to change fill character.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \spec requires true; \endspec
 * \vprivate Product Private
 * \steady UNKNOWN
 */
StatefulStreamManipulator<char> FillChar(char c);

/*!
 * \brief Applicator to change width in underlying stream.
 * \param[in] output_width Width for input/output.
 * \return Stream manipulator to change width.
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant FALSE
 * \spec requires true; \endspec
 * \vprivate Product Private
 * \steady UNKNOWN
 */
StatefulStreamManipulator<std::uint8_t> Width(std::uint8_t output_width);

}  // namespace stream
}  // namespace amsr

#endif  // LIB_IOSTREAM_INCLUDE_AMSR_IOSTREAM_STATEFUL_STREAM_MANIPULATOR_H_
