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
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  amsr/trace/tracer.h
 *        \brief  Declaration of the Tracer interface.
 *
 *         \unit  LogAndTrace::Trace::Tracer
 *
 *
 *********************************************************************************************************************/

#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_TRACE_TRACER_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_TRACE_TRACER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/trace/meta_data.h"

namespace amsr {
namespace trace {

/*! \brief Alias for Location. */
using Location = meta::Location;
/*! \brief Alias for TraceMeta. */
using TraceMeta = meta::TraceMeta<>;
/*!
 * \brief Alias for Named.
 * \tparam T type of traced variable.
 */
template <typename T>
using Named = meta::Named<T>;
/*!
 * \brief Alias for NameAndUnit.
 * \tparam T type of traced variable.
 */
template <typename T>
using NameAndUnit = meta::NameAndUnit<T>;
/*!
 * \brief Alias for TraceType.
 *
 * \trace DSGN-LogAndTrace-TraceMeta
 */
using TraceType = meta::TraceType;

// VECTOR NC Metric-OO.WMC.One: MD_LOG_Metric.OO.WMC.One
/*!
 * \brief Tracer interface.
 * \trace DSGN-LogAndTrace-Tracer
 * \vpublic
 */
class Tracer {
 public:
  /*! \brief Default destructor. */
  virtual ~Tracer() noexcept = default;
  /*! \brief Default destructor. */
  Tracer() noexcept = default;

 protected:
  /*! \brief Default copy constructor. */
  Tracer(Tracer const&) noexcept = default;
  /*! \brief Default move constructor. */
  Tracer(Tracer&&) noexcept = default;
  /*!
   * \brief Default copy assignment.
   * \return
   */
  auto operator=(Tracer const&) & noexcept -> Tracer& = default;
  /*!
   * \brief Default move assignment.
   * \return
   */
  auto operator=(Tracer&&) & noexcept -> Tracer& = default;

 public:
  /*!
   * \brief  Perform a type of trace with some meta data and arguments.
   * \tparam TraceType The type of the trace.
   *                   Must be either:
   *                    - amsr::trace::TraceType::TraceType::SpanStart
   *                    - amsr::trace::TraceType::TraceType::SpanStop
   *                    - amsr::trace::TraceType::TraceType::Variable
   * \tparam Args      The type of the arguments of the trace.
   * \param  type      The type of the trace.
   * \param  meta      The meta data of the trace.
   * \param  args      The arguments of the trace.
   * \return
   * \vpublic
   */
  template <typename TraceType, typename... Args>
  void Trace(TraceType&& type, TraceMeta const& meta, Args&&... args) noexcept {
    // VCA_LOGAPI_VALID_POINTER
    this->TraceBegin(type, meta);
    this->TraceArgs(std::forward<Args>(args)...);
    // VCA_LOGAPI_VALID_POINTER
    this->TraceEnd(type, meta);
  }

 private:
  /*!
   * \brief  Perform Trace with Args.
   * \tparam Args The type of the traced arguments.
   * \param  args The traced arguments.
   * \vprivate Component Private
   */
  template <typename... Args>
  void TraceArgs(Args&&... args) noexcept {
    // VCA_LOGAPI_VALID_POINTER
    this->TraceArgBegin();
    this->TraceArgPayload(std::forward<Args>(args)...);
    // VCA_LOGAPI_VALID_POINTER
    this->TraceArgEnd();
  }

  // VECTOR NC AutosarC++17_10-M0.1.8: MD_LOG_M0.1.8_EmptyCallback
  /*!
   * \brief TraceArgs which does nothing with void as input.
   * \vprivate Component Private
   */
  static void TraceArgs(void) noexcept {}

  /*!
   * \brief  Traces one of the arguments in TraceArgs.
   * \tparam Arg1 The type of the argument to trace.
   * \tparam Args The type of the remaining arguments to trace.
   * \param  arg1 The arguments to trace.
   * \param  args The remaining arguments to trace.
   * \vprivate Component Private
   */
  template <typename Arg1, typename... Args>
  void TraceArgPayload(Arg1&& arg1, Args&&... args) noexcept {
    this->TraceArg(std::forward<Arg1>(arg1));
    // VCA_LOGAPI_VALID_POINTER
    this->TraceNextArg();
    this->TraceArgPayload(std::forward<Args>(args)...);
  }

  /*!
   * \brief  Trace the final argument of TraceArgPayload.
   * \tparam Arg Type of the final argument.
   * \param  arg The final argument.
   * \vprivate Component Private
   */
  template <typename Arg>
  void TraceArgPayload(Arg&& arg) noexcept {
    this->TraceArg(std::forward<Arg>(arg));
  }

  /*!
   * \brief  Traces a value contained in a Named.
   * \tparam Arg type of the value contained in the named.
   * \param  arg the Named object.
   * \vprivate Component Private
   */
  template <typename Arg>
  void TraceArg(meta::Named<Arg> const& arg) noexcept {
    // VCA_LOGAPI_VALID_POINTER
    this->TraceName(arg.name);
    // VCA_LOGAPI_VALID_POINTER
    this->TraceValue(arg.value);
  }

  /*!
   * \brief  Trace a value contained in a NameAndUnit.
   * \tparam Arg The type of the value contained in the NameAndUnit.
   * \param  arg The NameAndUnit object.
   * \vprivate Component Private
   */
  template <typename Arg>
  void TraceArg(meta::NameAndUnit<Arg> const& arg) noexcept {
    // VCA_LOGAPI_VALID_POINTER
    this->TraceName(arg.name);
    // VCA_LOGAPI_VALID_POINTER
    this->TraceValue(arg.value);
    // VCA_LOGAPI_VALID_POINTER
    this->TraceUnit(arg.unit);
  }

  /*!
   * \brief Pure virtual function for beginning a trace at the start of a trace span.
   * \vpublic
   */
  virtual void TraceBegin(TraceType::SpanStart const&, TraceMeta const&) noexcept = 0;
  /*!
   * \brief Pure virtual function for ending a trace at the start of a trace span.
   * \vpublic
   */
  virtual void TraceEnd(TraceType::SpanStart const&, TraceMeta const&) noexcept = 0;

  /*!
   * \brief Pure virtual function for beginning a trace at the end of a trace span.
   * \vpublic
   */
  virtual void TraceBegin(TraceType::SpanStop const&, TraceMeta const&) noexcept = 0;
  /*!
   * \brief Pure virtual function for ending a trace at the end of a trace span.
   * \vpublic
   */
  virtual void TraceEnd(TraceType::SpanStop const&, TraceMeta const&) noexcept = 0;

  /*!
   * \brief Pure virtual function for beginning a trace of a variable.
   * \vpublic
   */
  virtual void TraceBegin(TraceType::Variable const&, TraceMeta const&) noexcept = 0;
  /*!
   * \brief Pure virtual function for ending a trace of a variable.
   * \vpublic
   */
  virtual void TraceEnd(TraceType::Variable const&, TraceMeta const&) noexcept = 0;

  /*!
   * \brief Begin tracing an Arg.
   * \vpublic
   */
  virtual void TraceArgBegin() noexcept = 0;
  /*!
   * \brief End tracing an Arg.
   * \vpublic
   */
  virtual void TraceArgEnd() noexcept = 0;
  /*!
   * \brief Trace the next argument.
   * \vpublic
   */
  virtual void TraceNextArg() const noexcept = 0;
  /*!
   * \brief Trace a name.
   * \param name The name of an arg to trace.
   * \vpublic
   */
  virtual void TraceName(amsr::core::StringView name) noexcept = 0;
  /*!
   * \brief Trace a unit.
   * \param unit The unit of an arg to trace.
   * \vpublic
   */
  virtual void TraceUnit(amsr::core::StringView unit) noexcept = 0;

  /*!
   * \brief Trace a value.
   * \vpublic
   */
  virtual void TraceValue(std::int8_t) noexcept = 0;
  /*!
   * \brief Trace a value.
   * \vpublic
   */
  virtual void TraceValue(std::int16_t) noexcept = 0;
  /*!
   * \brief Trace a value.
   * \vpublic
   */
  virtual void TraceValue(std::int32_t) noexcept = 0;
  /*!
   * \brief Trace a value.
   * \vpublic
   */
  virtual void TraceValue(std::int64_t) noexcept = 0;
  /*!
   * \brief Trace a value.
   * \vpublic
   */
  virtual void TraceValue(std::uint8_t) noexcept = 0;
  /*!
   * \brief Trace a value.
   * \vpublic
   */
  virtual void TraceValue(std::uint16_t) noexcept = 0;
  /*!
   * \brief Trace a value.
   * \vpublic
   */
  virtual void TraceValue(std::uint32_t) noexcept = 0;
  /*!
   * \brief Trace a value.
   * \vpublic
   */
  virtual void TraceValue(std::uint64_t) noexcept = 0;
  /*!
   * \brief Trace a value.
   * \vpublic
   */
  virtual void TraceValue(float) noexcept = 0;
  /*!
   * \brief Trace a value.
   * \vpublic
   */
  virtual void TraceValue(double) noexcept = 0;
  /*!
   * \brief Trace a value.
   * \vpublic
   */
  virtual void TraceValue(bool) noexcept = 0;
  /*!
   * \brief Trace a value.
   * \vpublic
   */
  virtual void TraceValue(amsr::core::StringView) noexcept = 0;
};

}  // namespace trace
}  // namespace amsr

#endif
