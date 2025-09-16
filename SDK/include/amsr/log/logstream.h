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
/*!
 *      \file
 *      \brief    Definition of the LogStream class.
 *
 *      \details  LogStream implementation.
 *
 *      \unit     LogAndTrace::Logging::LogStream
 *
 *********************************************************************************************************************/
#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_LOGSTREAM_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_LOGSTREAM_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include "amsr/core/array.h"
#include "amsr/core/error_code.h"
#include "amsr/core/string.h"
#include "amsr/log/internal/abort.h"
#include "amsr/log/internal/memory_pool.h"
#include "amsr/log/internal/ref.h"
#include "amsr/log/internal/to_string.h"
#include "amsr/log/internal/types/message.h"
#include "amsr/log/internal/types/meta_data.h"
#include "amsr/log/internal/types/typed_message_builder.h"
#include "amsr/log/internal/types/types.h"
#include "amsr/log/internal/utils.h"
#include "amsr/log/log_types.h"  // IWYU pragma: export
#include "amsr/log_fwd.h"        // IWYU pragma: export
#include "vac/container/string_literals.h"
#include "vac/testing/test_adapter.h"

namespace amsr {
namespace log {

namespace internal {
/*!
 * \brief Offset by one for char buffer to store NULL terminator.
 *
 * \vprivate Component private.
 */
constexpr std::uint8_t kNullTerminatorOffset{1u};

/*!
 * \brief A trait containing information on numbers.
 *
 * \tparam Type The type of number.
 * \tparam Base The conversion base.
 *
 * \vprivate Component private.
 */
template <typename Type, NumericBase kBase>
class NumberTrait final {
 public:
  /*! \brief The number of bits needed to encode the number.*/
  constexpr static std::size_t kBits{sizeof(Type) * 8u};
  /*! \brief The number of digits needed to represent the number. */
  constexpr static std::size_t kDigits{kBits / static_cast<std::size_t>(kBase)};
  /*! \brief The size of the buffer needed with trailing zero. */
  constexpr static std::size_t kBufferSize{kDigits + kNullTerminatorOffset};
  /*! \brief The type of buffer returned. */
  using Buffer = core::Array<char, kBufferSize>;
};
}  // namespace internal

// VECTOR NC Metric-OO.WMC.One: MD_LOG_Metric-OO.WMC.One_external_API
/*!
 * \brief The Class LogStream represents a log message, allowing for insert stream operators.
 * to be used for appending data.
 *
 * \vpublic
 */
class LogStream final {
  /*! \brief The type for line numbers. */
  using LineNumber = int;  // VECTOR SL AutosarC++17_10-A3.9.1: MD_LOG_AutosarC++17_10-A3.9.1_basic_type

  /*! \brief The Logger instance this LogStream is connected to. */
  internal::Ref<Logger> logger_;
  /*! \brief A single log message buffer. */
  internal::PoolShard<internal::TypedMessageBuilder> message_{};
  /*! \brief The LogLevel of the next log message. */
  LogLevel log_level_;

 public:
  /*!
   * \brief Creates a new LogStream object.
   *
   * \param logger    The logger that's the parent of this stream.
   * \param log_level The set log level for this stream.
   *
   * \vprivate Component private.
   * \spec
   *   requires true;
   * \endspec
   */
  LogStream(Logger& logger, LogLevel log_level) noexcept;

  /*!
   * \brief Delete default constructor.
   *
   * \vprivate Component private.
   */
  LogStream() = delete;

  /*!
   * \brief Delete copy constructor.
   *
   * \vprivate Component private.
   */
  LogStream(LogStream const&) = delete;
  /*!
   * \brief Delete copy assignment operator.
   *
   * \returns The assigned to object.
   *
   * \vprivate Component private.
   */
  auto operator=(LogStream const&) -> LogStream& = delete;

  /*!
   * \brief Explicit destructor
   * \spec
   *   requires true;
   * \endspec
   */
  ~LogStream() noexcept;

  /*!
   * \brief Move constructor.
   *
   * \param other The second stream.
   *
   * \vprivate Component private.
   * \spec
   *   requires true;
   * \endspec
   */
  LogStream(LogStream&& other) noexcept;

  /*!
   * \brief Move assignment operator.
   *
   * \returns The assigned to object.
   *
   * \vprivate Component private.
   */
  auto operator=(LogStream&&) -> LogStream& = delete;

  /*!
   * \brief Sends out the current log buffer and initiates a new message stream.
   * \details As defined in SWS_LOG_00039
   *
   * \trace DSGN-LogAndTrace-Flush
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   * \vpublic
   */
  void Flush() & noexcept;

  /*!
   * \brief Sends out the current log buffer.
   * \details As defined in SWS_LOG_00039
   * \trace DSGN-LogAndTrace-Flush
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   * \vpublic
   */
  void Flush() && noexcept;

  // VECTOR NC VectorC++-V3.9.2: MD_LOG_V3.9.2_considered_but_declined
  /*!
   * \brief Adds a location to the stream.
   *
   * \param[in] file The file location to add.
   * \param[in] line The line to add.
   * \return         A reference to the stream.
   *
   * \internal
   * - #10 If the logger is active.
   *  - #20 Then adds the file & line location.
   * - #30 Return a reference to the log stream.
   * \endinternal
   * \details As defined in SWS_LOG_00129
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   * \vpublic
   */
  auto WithLocation(core::StringView file, LineNumber line) noexcept -> LogStream&;

  /*!
   * \brief Appends boolean parameter to the internal message buffer.
   * \details As defined in SWS_LOG_00040
   *
   * \param value Value to be logged.
   * \return      The log stream.
   * \spec
   *   requires true;
   * \endspec
   *
   * \trace DSGN-LogAndTrace-DataTypes
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   * \vpublic
   */
  auto operator<<(bool value) noexcept -> LogStream&;

  /*!
   * \brief Appends unsigned char parameter to the internal message buffer.
   *
   * \param value Value to be logged.
   * \return      The log stream.
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   * \vpublic
   */
  auto operator<<(char value) noexcept -> LogStream&;

  /*!
   * \brief Appends unsigned int 8 bit parameter to the internal message buffer.
   *
   * \param value Value to be logged.
   * \return      The log stream.
   *
   * \details As defined in SWS_LOG_00041
   * \spec
   *   requires true;
   * \endspec
   * \trace DSGN-LogAndTrace-DataTypes
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   * \vpublic
   */
  auto operator<<(std::uint8_t value) noexcept -> LogStream&;

  /*!
   * \brief Appends unsigned int 16 bit parameter to the internal message buffer.
   *
   * \param value Value to be logged.
   * \return      The log stream.
   * \spec
   *   requires true;
   * \endspec
   *
   * \details As defined in SWS_LOG_00042
   * \trace DSGN-LogAndTrace-DataTypes
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   * \vpublic
   */
  auto operator<<(std::uint16_t value) noexcept -> LogStream&;

  /*!
   * \brief Appends unsigned int 32 bit parameter to the internal message buffer.
   *
   * \param value Value to be logged.
   * \return      The log stream.
   * \spec
   *   requires true;
   * \endspec
   *
   * \details As defined in SWS_LOG_00043
   * \trace DSGN-LogAndTrace-DataTypes
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   * \vpublic
   */
  auto operator<<(std::uint32_t value) noexcept -> LogStream&;

  /*!
   * \brief Appends unsigned int 64 bit parameter to the internal message buffer.
   *
   * \param value Value to be logged.
   * \return      The log stream.
   * \spec
   *   requires true;
   * \endspec
   *
   * \details As defined in SWS_LOG_00044
   * \trace DSGN-LogAndTrace-DataTypes
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   * \vpublic
   */
  auto operator<<(std::uint64_t value) noexcept -> LogStream&;

  /*!
   * \brief Appends signed int 8 bit parameter to the internal message buffer.
   *
   * \param value Value to be logged.
   * \return      The log stream.
   * \spec
   *   requires true;
   * \endspec
   *
   * \details As defined in SWS_LOG_00045
   * \trace DSGN-LogAndTrace-DataTypes
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   * \vpublic
   */
  auto operator<<(std::int8_t value) noexcept -> LogStream&;

  /*!
   * \brief Appends signed int 16 bit parameter to the internal message buffer.
   *
   * \param value Value to be logged.
   * \return      The log stream.
   * \spec
   *   requires true;
   * \endspec
   *
   * \details As defined in SWS_LOG_00046, SWS_LOG_00052
   * \trace DSGN-LogAndTrace-DataTypes
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   * \vpublic
   */
  auto operator<<(std::int16_t value) noexcept -> LogStream&;

  /*!
   * \brief Appends signed int 32 bit parameter to the internal message buffer.
   *
   * \param value Value to be logged.
   * \return      The log stream.
   * \spec
   *   requires true;
   * \endspec
   *
   * \details As defined in SWS_LOG_00047
   * \trace DSGN-LogAndTrace-DataTypes
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   * \vpublic
   */
  auto operator<<(std::int32_t value) noexcept -> LogStream&;

  /*!
   * \brief Appends signed int 64 bit parameter to the internal message buffer.
   *
   * \param value Value to be logged.
   * \return      The log stream.
   * \spec
   *   requires true;
   * \endspec
   *
   * \details As defined in SWS_LOG_00048
   * \trace DSGN-LogAndTrace-DataTypes
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   * \vpublic
   */
  auto operator<<(std::int64_t value) noexcept -> LogStream&;

  /*!
   * \brief Appends float 32 bit parameter to the internal message buffer.
   *
   * \param value Value to be logged.
   * \return      The log stream.
   * \spec
   *   requires true;
   * \endspec
   *
   * \details As defined in SWS_LOG_00049
   * \trace DSGN-LogAndTrace-DataTypes
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   * \vpublic
   */
  auto operator<<(float value) noexcept -> LogStream&;

  /*!
   * \brief Appends float 64 bit parameter to the internal message buffer.
   *
   * \param value Value to be logged.
   * \return      The log stream.
   * \spec
   *   requires true;
   * \endspec
   *
   * \details As defined in SWS_LOG_00050
   * \trace DSGN-LogAndTrace-DataTypes
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   * \vpublic
   */
  auto operator<<(double value) noexcept -> LogStream&;

  /*!
   * \brief Appends null terminated ASCII string to the internal message buffer.
   *
   * \param value Value to be logged.
   * \return      The log stream.
   * \spec
   *   requires true;
   * \endspec
   *
   * \deprecated Will be removed in future AUTOSAR version.
   *
   * \details As defined in SWS_LOG_00051
   * \trace DSGN-LogAndTrace-DataTypes
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   * \vpublic
   */
  auto operator<<(char const* const value) noexcept -> LogStream&;

  /*!
   * \brief Appends an error code to the log stream.
   *
   * \param value The value to be logged.
   * \return      The passed log stream.
   * \spec
   *   requires true;
   * \endspec
   *
   * \details In contrast to the SWS does not contain the integral error code.
   *
   * \details As defined in SWS_LOG_00124
   * \trace DSGN-LogAndTrace-DataTypes
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   * \vpublic
   */
  auto operator<<(core::ErrorCode const& value) noexcept -> LogStream&;

  /*!
   * \brief Appends formatted hexadecimal parameter to the internal message buffer.
   *
   * \tparam T      The underlying type of the input.
   *                T must meet the type requirements of
   *                amsr::log::internal::UInt::UInt<T, kBase>(IntLogWrapper<T, kBase>, StringShard, StringShard)
   *                and amsr::log::IntLogWrapper<T, kBase>.
   * \tparam kBase  The base that's used for formatting.
   *                Must be a value defined in the enum amsr::log::internal::NumericBase.
   * \param wrapper The value to be logged.
   * \return        The log stream.
   * \spec
   *   requires true;
   * \endspec
   *
   * \details As defined in SWS_LOG_00053
   * \details As defined in SWS_LOG_00054
   * \details As defined in SWS_LOG_00055
   * \details As defined in SWS_LOG_00056
   * \details As defined in SWS_LOG_00057
   * \details As defined in SWS_LOG_00058
   * \details As defined in SWS_LOG_00059
   * \details As defined in SWS_LOG_00060
   *
   * \trace DSGN-LogAndTrace-NumberFormat
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   * \vpublic
   */
  template <typename T, internal::NumericBase kBase>
  auto operator<<(IntLogWrapper<T, kBase> const& wrapper) noexcept -> LogStream& {
    static_assert(not std::is_signed<T>::value, "IntLogWrapper cannot be used for signed values.");
    return IfPresent([wrapper](internal::TypedMessageBuilder& sm) {
      // VCA_LOGAPI_CHECKED_CONTRACT
      sm.GetLogData().emplace_back(internal::UInt{Clone(wrapper)});
    });
  }

  /*!
   * \brief Appends  binary data to the internal message buffer.
   *
   * \param value Value to be logged.
   * \return      The log stream.
   * \spec
   *   requires true;
   * \endspec
   *
   * \details As defined in SWS_LOG_00061
   * \trace DSGN-LogAndTrace-NumberFormat
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   * \vpublic
   */
  auto operator<<(LogRawBuffer const& value) noexcept -> LogStream&;

  /*!
   * \brief Appends string view to the internal message buffer.
   *
   * \param value Value to be logged.
   * \return      The log stream.
   * \spec
   *   requires true;
   * \endspec
   *
   * \details As defined in SWS_LOG_00062
   * \trace DSGN-LogAndTrace-DataTypes
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   * \vpublic
   */
  auto operator<<(core::StringView const value) noexcept -> LogStream&;

  /*!
   * \brief Appends log level parameter to the internal message buffer.
   *
   * \param value Value to be logged.
   * \return      The log stream.
   * \spec
   *   requires true;
   * \endspec
   *
   * \details As defined in SWS_LOG_00063
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   * \vpublic
   */
  auto operator<<(LogLevel value) & noexcept -> LogStream&;

  // VECTOR Disable AutosarC++17_10-M5.2.12: MD_LOG_AutosarC++17_10-M5.2.12
  /*!
   * \brief Logs a single argument without a description.
   *
   * \tparam T      Any supported loggable type according to DSGN-LogAndTrace-DataTypes,
   *                DSGN-LogAndTrace-NumberFormat and DSGN-LogAndTrace-CustomDataTypes.
   * \param[in] arg The argument to log.
   * \return        The log stream.
   * \spec
   *   requires true;
   * \endspec
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   */
  // VECTOR NC AutosarC++17_10-A13.3.1: MD_LOG_AutosarC++17_10-A13.3.1
  template <typename T>
  auto Arg(T const& arg) & noexcept -> LogStream& {
    return *this << arg;
  }

  /*!
   * \brief Logs a single argument with a description.
   *
   * \tparam T       Any supported loggable type according to DSGN-LogAndTrace-DataTypes, CREQ-LogAndTrace-HexAndBin.
   *                 std::string is not supported.
   *
   * \param[in] name The name of the argument. Must not be empty.
   * \param[in] arg  The argument to log.
   *
   * \return         The log stream.
   *
   * \pre         The name must not be an empty string.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   *
   * \internal
   *  - #10 If the logger is active.
   *   - #20 the description & argument are written.
   *  - #30 Then a reference to the logger is returned.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A13.3.1: MD_LOG_AutosarC++17_10-A13.3.1
  template <typename T>
  auto Arg(core::StringView name, T const& arg) & noexcept -> LogStream& {
    using vac::container::literals::operator""_sv;

    static_assert(not std::is_same<T, std::string>::value, "std::string is not supported!");
    internal::Assert(not name.empty(), "The name of 'Arg' call cannot be empty"_sv);

    return IfPresent([name, &arg](internal::TypedMessageBuilder& sm) {
      // VECTOR NC AutosarC++17_10-A2.11.2: MD_LOG_AutosarC++17_10-A2.11.2_reused_name
      using Payload = typename internal::GetType<decltype(internal::ConvertToLoggable(arg))>::Type;
      internal::MessageImpl& msg{sm.GetLogData()};
      internal::StringShard name_s{sm.InsertData(name)};
      // VECTOR NC AutosarC++17_10-A8.5.2: MD_LOG_AutosarC++17_10-A8.5.2_complicated_and_irrelevant_type
      // VECTOR NC AutosarC++17_10-A7.1.5: MD_LOG_AutosarC++17_10-A7.1.5_complicated_and_irrelevant_type
      // VCA_LOGAPI_VALID_REFERENCE_ARGUMENT
      auto arg_val = sm.InsertData(internal::ConvertToLoggable(arg));
      // VCA_LOGAPI_CHECKED_CONTRACT
      msg.emplace_back(Payload{std::move(arg_val), std::move(name_s)});
    });
  }

  /*!
   * \brief Logs a single number with description and unit.
   *
   * \tparam T       The loggable integer or floating point number.
   * \param[in] name The name of the argument.
   * \param[in] arg  The argument to log.
   * \param[in] unit The unit for the argument.
   * \return         The log stream.
   *
   * \pre         The name and unit must not be empty strings.
   *
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   *
   * \internal
   *  - #10 If the logger is active.
   *   - #20 the description, argument & unit are written.
   *  - #30 Then a reference to the logger is returned.
   * \endinternal
   */
  // VECTOR NC AutosarC++17_10-A13.3.1: MD_LOG_AutosarC++17_10-A13.3.1
  template <typename T>
  auto Arg(core::StringView name, T const& arg, core::StringView unit) & noexcept -> LogStream& {
    using vac::container::literals::operator""_sv;

    static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value || internal::IsIntLogWrapper<T>::value,
                  "Units are available only for numeric types!");
    internal::Assert(not name.empty(), "The name of 'Arg' call cannot be empty"_sv);
    internal::Assert(not unit.empty(), "The unit of 'Arg' call cannot be empty"_sv);

    return IfPresent([name, unit, &arg](internal::TypedMessageBuilder& sm) {
      using Payload = typename internal::GetType<decltype(internal::ConvertToLoggable(arg))>::Type;
      internal::MessageImpl& msg{sm.GetLogData()};
      internal::StringShard name_s{sm.InsertData(name)};
      internal::StringShard unit_s{sm.InsertData(unit)};
      // VECTOR NC AutosarC++17_10-A8.5.2: MD_LOG_AutosarC++17_10-A8.5.2_complicated_and_irrelevant_type
      // VECTOR NC AutosarC++17_10-A7.1.5: MD_LOG_AutosarC++17_10-A7.1.5_complicated_and_irrelevant_type
      auto arg_val = sm.InsertData(internal::ConvertToLoggable(arg));
      // VCA_LOGAPI_CHECKED_CONTRACT
      msg.emplace_back(Payload{std::move(arg_val), std::move(name_s), std::move(unit_s)});
    });
  }

  /*!
   * \brief Logs a single argument.
   *
   * \tparam Args    Any supported loggable type and optionally its name/unit.
   *                 Must meet the type requirements of the matching lvalue overload of
   *                 amsr::log::LogStream::Arg that depends on the number of arguments passed.
   * \param[in] args The arguments to log.
   * \return         The log stream.
   *
   * \details     Forwards the arguments to the lvalue overloads.
   * \pre         The name and unit must not be empty strings.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \context     ANY
   */
  // VECTOR NC AutosarC++17_10-A13.3.1: MD_LOG_AutosarC++17_10-A13.3.1
  template <typename... Args>
  auto Arg(Args&&... args) && noexcept -> LogStream {
    this->Arg(std::forward<Args>(args)...);
    return std::move(*this);
  }

  // VECTOR Enable AutosarC++17_10-M5.2.12

 private:
  /*!
   * \brief Executes the given Callable with the value of this instance if nonempty.
   *
   * \param f The Callable.
   * \returns The stream itself.
   * \spec
   *   requires true;
   * \endspec
   */
  auto IfPresent(FunctionRef<void(internal::TypedMessageBuilder&)> f) & noexcept -> LogStream&;

  /*!
   * \brief Wrapper for IfPresent for UInt, SInt and Float types.
   *
   * \tparam LogT   The internal types amsr::log::internal::UInt,
   *                amsr::log::internal::SInt or amsr::log::internal::Float.
   * \tparam T      Any supported loggable type that meets the type
   *                requirement of the constructor of the chosen type
   *                of LogT.
   *
   * \param value   The value to log.
   * \returns       The stream itself.
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename LogT, typename T>
  auto EmplaceNumericalIfPresent(T value) noexcept -> LogStream&;

  /*!
   * \brief Internal helper to flush the stream.
   * \spec
   *   requires true;
   * \endspec
   */
  void FlushInternal() noexcept;

  /*!
   * \brief Sets up the log stream for usage.
   * \spec
   *   requires true;
   * \endspec
   */
  void Setup() noexcept;

  /*! \brief Friend declaration for testing. */
  FRIEND_TEST(UT__LogStream, LogStreamOperator);
  /*! \brief Friend declaration for testing. */
  FRIEND_TEST(UT__LogStream, FlushEmptyMessage);
  /*! \brief Friend declaration for testing. */
  FRIEND_TEST(UT__LogStream, WithLocation);
  /*! \brief Friend declaration for testing. */
  FRIEND_TEST(UT__LogStream, MultipleStreams);
  /*! \brief Friend declaration for testing. */
  FRIEND_TEST(UT__LogStream, MovedStream);
  /*! \brief Friend declaration for testing. */
  FRIEND_TEST(UT__LogStream, EmptyStream);
};

/*!
 * \brief The logging adapter for custom types.
 *
 * \tparam T    Any supported loggable type.
 *              Any custom type T that has a defined overload for amsr::log::operator<<(amsr::log::LogStream&, T).
 * \param ls    The log stream to use.
 * \param value The value to log.
 * \return      The log stream that was passed.
 *
 * \note This overload is used only if the log stream is a rvalue (i.e. the first <<)
 *       and T does not have an overload for amsr::log::LogStream::operator<<(T).
 *
 * \pre         -
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \synchronous TRUE
 * \context     ANY
 */
template <typename T>
auto operator<<(LogStream&& ls, T&& value) noexcept -> LogStream& {
  // VECTOR NC AutosarC++17_10-A4.5.1: MD_LOG_AutosarC++17_10-A4.5.1_shift_op_false_positive
  // VCA_LOGAPI_CHECKED_CONTRACT
  return ls << std::forward<T>(value);
}
}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_LOGSTREAM_H_
