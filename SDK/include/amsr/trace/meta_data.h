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
/*!     \file     amsr/trace/meta_data.h
 *      \brief    Contains meta data for a Trace.
 *      \details
 *
 *      \unit     LogAndTrace::Trace::TraceMetaData
 *
 *********************************************************************************************************************/
#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_TRACE_META_DATA_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_TRACE_META_DATA_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <string>
#include "amsr/core/string.h"
#include "amsr/core/string_view.h"
#include "vac/language/location.h"

namespace amsr {
namespace trace {
namespace meta {

/*!
 * \brief A trace point.
 * \vpublic
 */
struct TracePointId {
  /*!
   * \brief The ID of the TracePoint.
   */
  amsr::core::StringView trace_point_id_;
};

/*!
 * \brief A tracing TraceContext.
 * \vpublic
 */
struct TraceContextId {
  /*!
   * \brief The ID of the TraceContext.
   */
  amsr::core::StringView trace_context_id_;
};

/*!
 * \brief A location of a trace statement.
 * \vpublic
 */
struct Location {
  /*!
   * \brief The file from where a trace statement is made.
   */
  amsr::core::StringView file_;
  /*!
   * \brief The line from where a trace statement is made.
   */
  std::int32_t line_{};
};

/*!
 * \brief  Metadata for a trace.
 * \tparam Clock the type of clock to save time stamps with. Defaulted to std::chrono::system_clock.
 *         Must implement the TrivialClock named requirement.
 * \vpublic
 */
template <typename Clock = std::chrono::system_clock>
class TraceMeta {
  /*! \brief Type used for timestamps. */
  using TimeStampClock = Clock;
  /*! \brief TracePointId of a given trace point. */
  TracePointId point_id_;
  /*! \brief The context which is being traced. */
  TraceContextId context_id_;
  /*! \brief The location of the trace. */
  Location location_;
  /*!
   * \brief The timestamp of the creation of the TraceMeta.
   *        Enabled through: TracingConfig::enable_time_point_acquisition.
   */
  std::chrono::time_point<TimeStampClock> time_;

 public:
  /*!
   * \brief  Get the trace point Id.
   * \return Const reference to point_id.
   */
  auto GetTracePointId() const noexcept -> TracePointId const& { return point_id_; }

  /*!
   * \brief  Get the trace context id.
   * \return Const reference to context_id.
   */
  auto GetTraceContextId() const noexcept -> TraceContextId const& { return context_id_; }

  /*!
   * \brief  Get the location.
   * \return Const reference to location.
   */
  auto GetLocation() const noexcept -> Location const& { return location_; }

  /*!
   * \brief  Get the time.
   * \return Const reference to time.
   */
  auto GetTime() const noexcept -> std::chrono::time_point<TimeStampClock> const& { return time_; }

  /*!
   * \brief  Create a TraceMeta from a TracePoint and a Location.
   * \tparam TracePoint a type of tracepoint. Must contain static method GetTracepoint() and GetContext().
   * \param  location The location where the trace is called.
   * \param  acquire_time_point Store a timestamp when TraceMeta is created.
   * \return A constructed TraceMeta.
   * \vpublic
   */
  template <typename TracePoint>
  static auto Create(TracePoint&&, Location location, bool acquire_time_point) noexcept -> TraceMeta {
    TraceMeta trace;
    trace.point_id_ = TracePoint::GetTracepoint();
    trace.context_id_ = TracePoint::GetContext();
    trace.location_ = location;
    if (acquire_time_point) {
      trace.time_ = TimeStampClock::now();
    }
    return trace;
  }
};

/*!
 * \brief Contains a number that has a name & unit.
 * \tparam T Type of value contained.
 * \vprivate Component Private
 */
template <typename T>
struct NameAndUnit {
  /*! \brief The name of the log item. */
  amsr::core::StringView name{};
  /*! \brief The value of the log item. */
  std::reference_wrapper<T const> value;
  /*! \brief The unit of the log item. */
  amsr::core::StringView unit{};
};

/*!
 * \brief Contains a value and a name.
 * \tparam T Type of value contained.
 * \vprivate Component Private
 */
template <typename T>
struct Named {
  /*! \brief The name of the log item. */
  amsr::core::StringView name{};
  /*! \brief The value of the log item. */
  std::reference_wrapper<T const> value;
};

/*!
 * \brief Three supported TraceTypes.
 * \vprivate Component Private
 */
struct TraceType final {
  /*! \brief Type used for overload resolution so the correct Trace function is called in Tracer. */
  struct SpanStart final {};
  /*! \brief Type used for overload resolution so the correct Trace function is called in Tracer. */
  struct SpanStop final {};
  /*! \brief Type used for overload resolution so the correct Trace function is called in Tracer. */
  struct Variable final {};
};

}  // namespace meta
}  // namespace trace
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_TRACE_META_DATA_H_
