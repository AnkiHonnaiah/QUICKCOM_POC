/**********************************************************************************************************************
 *  COPYRIGHT
 *
 -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *
 -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *
 -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  log_source.h
 *        \brief  NativeLogSource Class
 *        \unit   osabstraction::log_source
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LOGSOURCE_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_SYSLOG_LOG_SOURCE_H_
#define LIB_LOGSOURCE_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_SYSLOG_LOG_SOURCE_H_

#include <cinttypes>
// VECTOR Next Line AutosarC++17_10-M27.0.1: MD_OSA_M27.0.1_PosixApi
#include <cstdio>
#include <regex>
#include "amsr/core/optional.h"
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/core/string_view.h"
#include "amsr/syslog/native_log_message.h"
#include "osabstraction/io/file_descriptor.h"
#include "osabstraction/io/reactor1/reactor1_interface.h"

namespace amsr {
namespace syslog {

/*!
 * \brief A source of native log messages.
 * \details If multiple instances of this class are used in the system at the same time, on some operating systems only
 *          one of them might be able to receive messages.
 *
 * \trace DSGN-Osab-LogSource-Linux
 * \vprivate Vector product internal API
 */
class NativeLogSource final {
 public:
  /*!
   * \brief A callback that will be called when a new log message is available.
   * \vprivate Vector product internal API
   */
  using EventHandler = vac::language::UniqueFunction<void(NativeLogMessage&&)>;

  /*!
   * \brief Delete copy constructor.
   */
  NativeLogSource(NativeLogSource const&) = delete;

  /*!
   * \brief Delete copy assignment.
   */
  auto operator=(NativeLogSource const&) -> NativeLogSource& = delete;

  /*!
   * \brief Delete move constructor.
   */
  NativeLogSource(NativeLogSource const&&) = delete;

  /*!
   * \brief Delete move assignment.
   */
  auto operator=(NativeLogSource const&&) -> NativeLogSource& = delete;

  /*!
   * \brief Destructor.
   *
   * \details         User has to avoid to destruct the NativeLogSource object while the internal reactor callback is
   *                  executing. This can be done by using a separate software event reactor job, or by ensuring per
   *                  design that NativeLogSource is destroyed in the same thread where Reactor::HandleEvents() is
   *                  called.
   *
   * \context         !EventHandler
   *
   * \pre             Should not be executed in the context of the user callback.
   *                  Reactor of this object should not execute this internal reactor callback while the destructor is
   *                  running. Usually this means that HandleEvents() of the reactor should not be running during the
   *                  execution of the destructor.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \vprivate Vector product internal API
   */
  ~NativeLogSource() noexcept;

  /*!
   * \brief Opens the native log source.
   *
   * \param[in]       reactor     A reactor that can be used to get notified about new messages. The reactor must exist
   *                              for the entire lifetime of the created NativeLogSource.
   * \param[in]       handler     An event handler that will be called when a new log message is available.
   * \param[in]       include_old_messages
   *                              Whether messages that have occurred in the past should be included.
   * \param[in]       filter      Names of buffer sets to be received. The meaning of the names is OS-specific. If an
   *                              empty span is passed, messages from all buffer sets will be received.
   *
   * \return          On success the created NativeLogSource.
   *
   * \error           osabstraction::OsabErrc::kResource    Insufficient resources to create the NativeLogSource.
   * \error           osabstraction::OsabErrc::kInsufficientPrivileges
   *                                                        Insufficient privileges to create the NativeLogSource.
   * \error           osabstraction::OsabErrc::kSystemEnvironmentError
   *                                                        Cannot create NativeLogSource due to an external error.
   * \error           osabstraction::OsabErrc::kUnexpected  An unexpected error ocurred.
   *
   * \context         ANY
   *
   * \pre             At least 1 slot must be available in the reactor. If include_old_messages is true, at least 2
   *                  slots must be available in the reactor.
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   *
   * \vprivate Vector product internal API
   */
  static auto Create(osabstraction::io::reactor1::Reactor1Interface& reactor, EventHandler handler,
                     bool include_old_messages, ::amsr::core::Span<::amsr::core::StringView const> filter) noexcept
      -> ::amsr::core::Result<std::unique_ptr<NativeLogSource>>;

  /*!
   * \brief Stop receiving messages.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate Vector product internal API
   */
  void Pause() noexcept;

  /*!
   * \brief Continue receiving messages.
   * \details Messages that were logged while the NativeLogSource was paused may or may not be received after resuming.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate Vector product internal API
   */
  void Resume() noexcept;

  /*!
   * \brief Changes the buffer sets to be received.
   * \details Messages that are logged during this call may be dropped even if both the old and the new filter would
   *          include the message. Messages that were logged before this call but were not yet handled may still be
   *          handled according to the old filter.
   *
   * \param[in]       filter      Names of buffer sets to be received. The meaning of the names is OS-specific. If an
   *                              empty span is passed, messages from all buffer sets will be received.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          TRUE
   *
   * \vprivate Vector product internal API
   */
  void SetFilter(::amsr::core::Span<::amsr::core::StringView const> filter) noexcept;

 private:
  /*!
   * \brief Opens the native log source.
   *
   * \param[in]       reactor     A reactor that can be used to get notified about new messages. The reactor must exist
   *                              for the entire lifetime of this object.
   * \param[in]       handler     An event handler that will be called when a new log message is available.
   * \param[in]       include_old_messages
   *                              Whether messages that have occurred in the past should be included.
   * \param[in]       filter      Names of buffer sets to be received. The meaning of the names is OS-specific. If an
   *                              empty span is passed, messages from all buffer sets will be received.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  NativeLogSource(osabstraction::io::reactor1::Reactor1Interface& reactor, EventHandler handler,
                  bool include_old_messages, ::amsr::core::Span<::amsr::core::StringView const> filter);

  /*!
   * \brief Handles the end of the log file stream being reached.
   *
   * \return A log message marking the switch from log file to socket.
   *
   * \context         OnReactorEvent
   *
   * \pre             reading_old_messages_ == true
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto HandleEndOfLogFile() noexcept -> NativeLogMessage;

  /*!
   * \brief Maps a given native severity to our severities.
   *
   * \param[in]       native_severity
   *                              A severity given by syslog.
   *
   * \return          A severity.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   */
  static Severity MapSeverity(std::uint8_t priority) noexcept;

  /*!
   * \brief Called from the reactor to handle a received log message.
   *
   * \context         Reactor1Interface::HandleEvents
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   * \steady          FALSE
   */
  void OnReactorEvent() noexcept;

  /*!
   * \brief Parses a syslog message in RFC5424 format.
   *
   * \param[in]       message     The received RFC5424 message.
   *
   * \return          A result containing the parsed message.
   *
   * \error           osabstraction::OsabErrc::kProtocolError   The message is invalid.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   */
  auto ParseMessage(::amsr::core::StringView message) const noexcept -> ::amsr::core::Result<NativeLogMessage>;

  /*!
   * \brief Converts a string to a number.
   *
   * \param[in]       string      A string to be converted to a number.
   *
   * \return          An optional that contains the parsed number if successful, else an empty optional.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      TRUE
   */
  static auto ParseNumber(amsr::core::String const& string) noexcept -> ::amsr::core::Optional<std::intmax_t>;

  /*!
   * \brief Starts receiving messages on the syslog server.
   *
   * \error           osabstraction::OsabErrc::kResource    Insufficient resources.
   * \error           osabstraction::OsabErrc::kInsufficientPrivileges
   *                                                        Insufficient privileges.
   * \error           osabstraction::OsabErrc::kSystemEnvironmentError
   *                                                        Cannot start syslog server due to an external error.
   * \error           osabstraction::OsabErrc::kUnexpected  An unexpected error ocurred.
   *
   * \context         ANY
   *
   * \pre             -
   *
   * \reentrant       FALSE
   * \synchronous     TRUE
   * \threadsafe      FALSE
   */
  auto StartServer() noexcept -> ::amsr::core::Result<void>;

  /*!
   * \brief Protects internal state.
   */
  std::mutex mutex_{};

  /*!
   * \brief The reactor that will be used by the user to wait for messages.
   */
  osabstraction::io::reactor1::Reactor1Interface& reactor_;

  /*!
   * \brief The user callback.
   */
  EventHandler handler_;

  /*!
   * \brief File descriptor for the socket of the syslog server.
   */
  osabstraction::io::FileDescriptor socket_fd_{};

  /*!
   * \brief A read stream for the log file.
   */
  std::FILE* log_file_stream_{};

  /*!
   * \brief A regular expression matching log messages in RFC5424 format.
   */
  std::regex const rfc5424_pattern_{
      R"(<(\d+)>1 ([^ ]+) [^ ]+ ([^ ]+) ([^ ]+) ([^ ]+) (-|(?:\[(?:\\]|[^\]])*])+)(?: +(.*))?\n?)"};

  /*!
   * \brief The callback handle for the reactor software event.
   */
  osabstraction::io::reactor1::CallbackHandle software_event_handle_{
      osabstraction::io::reactor1::kInvalidCallbackHandle};

  /*!
   * \brief The callback handle for the socket.
   */
  osabstraction::io::reactor1::CallbackHandle socket_callback_handle_{
      osabstraction::io::reactor1::kInvalidCallbackHandle};

  /*!
   * \brief Whether messages from the log file are currently being read.
   */
  bool reading_old_messages_;

  /*!
   * \brief Whether the syslog server has started receiving messages.
   */
  bool server_started_{false};

  /*!
   * \brief Whether receiving messages is paused.
   */
  bool paused_{false};
};

}  // namespace syslog
}  // namespace amsr

/*!
 * \exclusivearea   ::amsr::syslog::NativeLogSource::mutex_
 * Protects the access to the internal state.
 * \protects        ::amsr::syslog::NativeLogSource::paused_
 *                  ::amsr::syslog::NativeLogSource::reading_old_messages_
 * \usedin          ::amsr::syslog::NativeLogSource::~NativeLogSource
 *                  ::amsr::syslog::NativeLogSource::OnReactorEvent
 *                  ::amsr::syslog::NativeLogSource::Pause
 *                  ::amsr::syslog::NativeLogSource::Resume
 * \exclude         -
 * \length          MEDIUM
 * \endexclusivearea
 */

#endif  // LIB_LOGSOURCE_LIBOSABSTRACTION_LINUX_INCLUDE_AMSR_SYSLOG_LOG_SOURCE_H_
