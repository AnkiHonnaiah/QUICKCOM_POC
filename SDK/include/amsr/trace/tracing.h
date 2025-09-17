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
/*!        \file  amsr/trace/tracing.h
 *        \brief  API for tracing.
 *
 *         \unit  LogAndTrace::Trace::Tracing
 *
 *********************************************************************************************************************/

#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_TRACE_TRACING_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_TRACE_TRACING_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <tuple>
#include <type_traits>
#include "amsr/core/result.h"
#include "amsr/core/string_view.h"
#include "amsr/core/vector.h"
#include "amsr/envvar/envvar.h"
#include "amsr/generic/generic_error_domain.h"
#include "amsr/log/internal/logger_base.h"
#include "amsr/trace/meta_data.h"
#include "amsr/trace/tracesink.h"
#include "ara/core/vector.h"
#include "vac/container/c_string_view.h"
#include "vac/container/string_literals.h"

namespace amsr {
namespace trace {
namespace detail {
// VECTOR NC AutosarC++17_10-A12.4.1: MD_LOG_AutosarC++17_10-A12.4.1_compileTimeResolution
/*!
 * \brief  IsSupportedType::value is true if T has a supported TraceValue function, otherwise false.
 * \tparam Args Parameter pack with either 0 or >1 argument. Multiple types are not supported.
 */
template <typename... Args>
struct IsSupportedImpl : public std::false_type {};

/*!
 * \brief  IsSupportedType::value is true if T has a supported TraceValue function, otherwise false.
 * \tparam T Typename to check if supported.
 */
template <typename T>
struct IsSupportedImpl<T> {
  /*! \brief Bool representing whether a specific type is supported or not. */
  static constexpr bool const value{(std::is_same<T, std::int8_t>::value) || (std::is_same<T, std::int16_t>::value) ||
                                    (std::is_same<T, std::int32_t>::value) || (std::is_same<T, std::int64_t>::value) ||
                                    (std::is_same<T, std::uint8_t>::value) || (std::is_same<T, std::uint16_t>::value) ||
                                    (std::is_same<T, std::uint32_t>::value) ||
                                    (std::is_same<T, std::uint64_t>::value) || (std::is_same<T, double>::value) ||
                                    (std::is_same<T, float>::value) || (std::is_same<T, bool>::value) ||
                                    (std::is_same<T, amsr::core::StringView>::value)};
};

/*!
 * \brief  IsSupportedType::value is true if T has a supported TraceValue function, otherwise false.
 * \tparam Args Parameter pack type(s) to check.
 */
template <typename... Args>
struct IsSupportedType : IsSupportedImpl<Args...> {};

}  // namespace detail

/*! \brief Alias for Location. */
using Location = amsr::trace::meta::Location;
/*! \brief Alias for TraceMeta. */
using TraceMeta = amsr::trace::meta::TraceMeta<>;
/*! \brief Alias for TracePointId. */
using TracePointId = amsr::trace::meta::TracePointId;
/*! \brief Alias for TraceContextId. */
using TraceContextId = amsr::trace::meta::TraceContextId;

/*! \brief An alias for amsr::log::internal::LoggerBase. */
using LoggerBase = amsr::log::internal::LoggerBase;

/*!
 * \brief Alias for Named.
 *
 * \trace DSGN-LogAndTrace-TraceMeta
 */
template <typename T>
using Named = amsr::trace::meta::Named<T>;
/*!
 * \brief Alias for NameAndUnit.
 *
 * \trace DSGN-LogAndTrace-TraceMeta
 */
template <typename T>
using NameAndUnit = amsr::trace::meta::NameAndUnit<T>;

namespace internal {
// VECTOR NC AutosarC++17_10-A3.3.2: MD_LOG_AutosarC++17_10-A3.3.2_doNotUseNonPODTypeObjectsWithStaticStorageDuration
/*! \brief Name of the environment variable which sets the enabled contexts */
vac::container::CStringView const TRACE_ENV_VAR_NAME{"AMSR_TRACE_CONTEXTS"};

/*!
 * \brief  Checks if a specific TraceContextId is enabled.
 * \return True if the TraceContextId is enabled, false otherwise.
 * \param  item A TraceContextId for which to look in enabled_contexts.
 * \spec
 *  return true
 * \endspec
 */
bool IsEnabled(TraceContextId const& item) noexcept;
}  // namespace internal

/*!
 * \brief  Checks if the tracepoint is enabled. If it is all registered trace sinks are used.
 * \tparam Type    The type of trace, e.g. start/variable/stop.
 * \tparam T       TracePoint which must inherit from TracePoint or implement the same public methods
 *                 and fields.
 * \tparam Args... Extra args for the trace. Typically each element is created with function Arg(...).
 * \param  t       A TracePoint used to identify the source of the trace.
 * \param  l       A location with information about file and line.
 * \param  args    Extra arguments.
 * \return void.
 * \trace DSGN-LogAndTrace-Tracing
 * \spec
 *  return true
 * \endspec
 * \vpublic
 */
template <typename Type, typename T, typename... Args>
void Trace(T&& t, Location const& l, Args&&... args) noexcept {
  LoggerBase& logger_base{amsr::log::internal::LoggerBase::Get()};
  // VCA_LOGAPI_TRACEPOINT_FUNCTION_CALL
  if (internal::IsEnabled(t.GetContext())) {
    TraceMeta const meta{TraceMeta::Create(std::forward<T>(t), l, logger_base.IsTraceTimePointAcquisitionEnabled())};
    for (std::unique_ptr<TraceSink> const& sink : logger_base.GetEnabledTraceSinks()) {
      // VCA_LOGAPI_NOT_NULL_POINTER, VCA_LOGAPI_POINTER_TARGET_VERIFIED
      sink->GetTracer().Trace(Type{}, meta, std::forward<Args>(args)...);
    }
  }
}

/*!
 * \brief  Begin a trace span.
 * \tparam T       TracePoint which must inherit from TracePoint or implement the same public methods
 *                 and fields.
 * \tparam Args... Extra args for the trace. Typically each element is created with function Arg(...).
 * \param  t       A TracePoint used to identify the source of the trace.
 * \param  l       A location with information about file and line.
 * \param  args    Extra arguments.
 * \return void.
 * \spec
 *  return true
 * \endspec
 * \vpublic
 */
template <typename T, typename... Args>
void TraceStartSpan(T&& t, Location&& l, Args&&... args) noexcept {
  Trace<TraceType::SpanStart>(std::forward<T>(t), std::move(l), std::forward<Args>(args)...);
}

/*!
 * \brief  End a trace span.
 * \tparam T       TracePoint which must inherit from TracePoint or implement the same public methods
 *                 and fields.
 * \tparam Args... Extra args for the trace. Typically each element is created with function Arg(...).
 * \param  t       A TracePoint used to identify the source of the trace.
 * \param  l       A location with information about file and line.
 * \param  args    Extra arguments.
 * \return void.
 * \spec
 *  return true
 * \endspec
 * \vpublic
 */
template <typename T, typename... Args>
void TraceStopSpan(T&& t, Location&& l, Args&&... args) noexcept {
  Trace<TraceType::SpanStop>(std::forward<T>(t), std::move(l), std::forward<Args>(args)...);
}

/*!
 * \brief  Trace a variable.
 * \tparam T       TracePoint which must inherit from TracePoint or implement the same public methods
 *                 and fields.
 * \tparam Args... Extra args for the trace. Typically each element is created with function Arg(...).
 * \param  t       A TracePoint used to identify the source of the trace.
 * \param  l       A location with information about file and line.
 * \param  args    Extra arguments.
 * \return void.
 * \spec
 *  return true
 * \endspec
 * \vpublic
 */
template <typename T, typename... Args>
void TraceVariable(T&& t, Location&& l, Args&&... args) noexcept {
  Trace<TraceType::Variable>(std::forward<T>(t), std::move(l), std::forward<Args>(args)...);
}

/*!
 * \brief   RAII class to trace a span.
 * \details TraceStartSpan is called in constructor. TraceStopSpan is called in destructor.
 * \tparam  T       TracePoint which must inherit from TracePoint or implement the same public methods
 *                  and fields.
 * \tparam  Args... Extra args for the trace. Typically each element is created with function Arg(...).
 * \param   t       A TracePoint used to identify the source of the trace.
 * \param   l       A location with information about file and line.
 * \param   args    Extra arguments.
 * \return  void.
 * \spec
 *  return  true
 * \endspec
 * \vpublic
 */
template <typename T, typename... Args>
class TraceSpan final {
  /*! \brief The type of the tuple to store the Args in between ctor and dtor. */
  using TupleType = std::tuple<Args...>;
  /*! \brief The size of the tuple. Used to create the index sequence to unpack the tuple. */
  static constexpr size_t tuple_size{std::tuple_size<TupleType>::value};

  /*!
   * \brief  Unpacks the tuple to a parameter pack and calls TraceStartSpan.
   * \param  Sequence of indices used to "unpack" the tuple.
   * \tparam Parameter pack of all the indices of the tuple.
   * \return
   */
  template <size_t... I>
  void CallTraceStartSpan(std::index_sequence<I...>) noexcept {
    TraceStartSpan(T{type}, Location{location}, std::get<I>(arguments)...);
  }
  /*!
   * \brief  Unpacks the tuple to a parameter pack and calls TraceStopSpan.
   * \param  Sequence of indices used to "unpack" the tuple.
   * \tparam Parameter pack of all the indices of the tuple.
   * \return
   */
  template <size_t... I>
  void CallTraceStopSpan(std::index_sequence<I...>) noexcept {
    TraceStopSpan(T{type}, Location{location}, std::get<I>(arguments)...);
  }

 public:
  /*! \brief Deleted  Constructor. */
  TraceSpan() = delete;

  // VECTOR NC AutosarC++17_10-A18.9.2: MD_LOG_AutosarC++17_10-A18.9.2_useStdMoveIfTheValueIsARvalueReference
  /*!
   * \brief  Construct a TraceSpan. Calls TraceStartSpan().
   * \param  t TracePoint.
   * \param  l Location of what is traced.
   * \param  args Extra arguments used in the trace.
   */
  TraceSpan(T&& t, Location&& l, Args&&... args) noexcept
      : type{std::move(t)}, location{std::move(l)}, arguments{std::make_tuple<Args...>(std::forward<Args>(args)...)} {
    CallTraceStartSpan(std::make_index_sequence<tuple_size>{});
  }

  /*! \brief Default Move Constructor. */
  TraceSpan(TraceSpan&&) noexcept = default;

  /*! \brief Deleted Copy Constructor. */
  TraceSpan(TraceSpan const&) = delete;

  /*! \brief  Default Move assignment. */
  TraceSpan& operator=(TraceSpan&&) & noexcept = default;

  /*! \brief  Deleted Copy assignment. */
  TraceSpan& operator=(TraceSpan const&) & = delete;

  /*! \brief  Destructor which calls TraceStopSpan(). */
  ~TraceSpan() noexcept { CallTraceStopSpan(std::make_index_sequence<tuple_size>{}); }

 private:
  /*! \brief Tracing point used when calling TraceStartSpan/TraceStopSpan. */
  T type;
  /*! \brief Location data of where the trace is done. */
  Location location;
  /*! \brief Extra arguments to trace, stored in a tuple. */
  TupleType arguments;
};

/*!
 * \brief  Helper function to create a TraceSpan.
 * \tparam T       TracePoint which must inherit from TracePoint or implement the same public methods
 *                 and fields.
 * \tparam Args... Extra args for the trace. Typically each element is created with function Arg(...).
 * \param  t       A TracePoint used to identify the source of the trace.
 * \param  l       A location with information about file and line.
 * \param  args    Extra arguments.
 * \return A TraceSpan class.
 * \vpublic
 */
template <typename T, typename... Args>
auto MakeTraceSpan(T&& t, Location&& l, Args&&... args) noexcept -> TraceSpan<T, Args...> {
  return TraceSpan<T, Args...>(std::forward<T>(t), std::move(l), std::forward<Args>(args)...);
}

// VECTOR NC AutosarC++17_10-A7.5.1: MD_LOG_AutosarC++17_10-A7.5.1_return_input_as_reference
/*!
 * \brief   Returns a value.
 * \details If Arg is invoked with an rvalue value, dereferencing the returned reference after the rvalue
 *          has gone out of scope leads to undefined behavior.
 * \tparam T type of the traced value.
 * \param   value
 * \return  The input value
 * \vpublic
 */
template <typename T, std::enable_if_t<detail::IsSupportedType<T>::value, std::int32_t> = 0>
auto Arg(T const& value) noexcept -> T const& {
  return value;
}

/*!
 * \brief  Creates a named value.
 * \tparam T type of the traced value.
 * \param  name
 * \param  value
 * \return Named struct with name and value.
 * \vpublic
 */
template <typename T, std::enable_if_t<detail::IsSupportedType<T>::value, std::int32_t> = 0>
auto Arg(amsr::core::StringView name, T const& value) noexcept -> Named<T> {
  return Named<T>{name, value};
}

/*!
 * \brief  Creates a named value with an argument.
 * \tparam T type of the traced value.
 * \param  name
 * \param  value
 * \param  unit
 * \return NamedAndUnit struct with name, value, and unit.
 * \vpublic
 */
template <typename T, std::enable_if_t<detail::IsSupportedType<T>::value, std::int32_t> = 0>
auto Arg(amsr::core::StringView name, T const& value, amsr::core::StringView unit) noexcept -> NameAndUnit<T> {
  return NameAndUnit<T>{name, value, unit};
}

/*!
 * \brief  Creates a message field for a trace statement.
 * \tparam T type of the message.
 * \param  value
 * \return A Named<amsr::core::String> struct, containing a name and a value.
 * \vpublic
 */
template <typename T, std::enable_if_t<detail::IsSupportedType<T>::value, std::int32_t> = 0>
auto Msg(T const& value) noexcept -> Named<T> {
  return Arg("Msg", value);
}

/*!
 * \brief   Creates a message field from a raw string for a trace statement.
 * \details Creates a Named<amsr::core::String> of the raw string.
 * \param   sv StringView created from raw string.
 * \return  A Named<amsr::core::String> struct, containing a name and a value.
 * \vpublic
 */
inline auto Msg(amsr::core::StringView const&& sv) noexcept -> Named<amsr::core::StringView> { return Arg("Msg", sv); }

namespace declaration {

/*! \brief Used to declare the trace as tracing a scope. */
class AsScope {};

/*! \brief Used to declare the trace as tracing a variable. */
class AsVariable {
 public:
  /*! \brief Default Constructor. */
  AsVariable() noexcept = default;
  /*! \brief Default Copy Constructor. */
  AsVariable(AsVariable const&) = delete;
  /*! \brief Default Move Constructor. */
  AsVariable(AsVariable&&) = delete;
  /*! \brief  Default Copy assignment. */
  AsVariable& operator=(AsVariable const&) & = delete;
  /*! \brief  Default Move assignment. */
  AsVariable& operator=(AsVariable&&) & = delete;
  /*! \brief Default Destructor. */
  virtual ~AsVariable() noexcept = default;
};

}  // namespace declaration

/*!
 * \brief Declaration of a Trace.
 *
 * \trace DSGN-LogAndTrace-TraceMeta
 *
 * \vpublic
 */
class TraceDeclaration {
 public:
  /*! \brief An alias for declaration::AsVariable. */
  using AsVariable = declaration::AsVariable;
  /*! \brief An alias for declaration::AsScope. */
  using AsScope = declaration::AsScope;

  /*! \brief Default Constructor. */
  TraceDeclaration() noexcept = default;
  /*! \brief Default Move Constructor. */
  TraceDeclaration(TraceDeclaration&&) = delete;
  /*! \brief  Default Move assignment. */
  TraceDeclaration& operator=(TraceDeclaration&&) & = delete;
  /*! \brief  Default Destructor. */
  // VCA_LOGAPI_MOLE_1298
  virtual ~TraceDeclaration() noexcept = default;

 protected:
  /*! \brief Default copy Constructor. */
  TraceDeclaration(TraceDeclaration const&) noexcept = default;
  /*! \brief  Default Copy assignment. */
  TraceDeclaration& operator=(TraceDeclaration const&) & noexcept = default;
};

/*!
 * \brief Sets whether traces should contain a time point.
 * \param new_value is used to set TracingConfig::IsTimePointAcquisitionEnabled.
 * \vpublic
 */
inline void SetTimePointAcquisition(bool new_value) noexcept {
  LoggerBase& logger_base{amsr::log::internal::LoggerBase::Get()};
  logger_base.SetTraceTimePointAcquisition(new_value);
}

}  // namespace trace
}  // namespace amsr

#endif
