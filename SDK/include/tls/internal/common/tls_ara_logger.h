/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2020 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!
 *              \file tls_ara_logger.h
 *             \brief This file declares the types used internally when logging
 *           \details Includes a class which is a wrapper around the ara::log logger class.
 *                    The reason to add a wrapper is to isolate the dependency to ara::log and to add standard
 *                    logging information to the log record.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_COMMON_TLS_ARA_LOGGER_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_COMMON_TLS_ARA_LOGGER_H_

#include <cstdint>
#include <string>
#include "amsr/core/abort.h"
#include "amsr/core/optional.h"
#include "amsr/core/string.h"
#include "amsr/core/string_view.h"
#include "ara/log/common.h"
#include "ara/log/logger.h"
#include "ara/log/logging.h"
#include "tls/internal/common/common_general_types.h"
#include "vac/container/string_literals.h"
#include "vac/language/location.h"

namespace amsr {
namespace tls {
namespace internal {
namespace common {
/*! \brief LogMessage alias for const char pointer - May be changed to CStringView in the future */
using LogMessage = char const*;
using vac::container::literals::operator""_sv;  // VECTOR SL AutosarC++17_10-M7.3.6: MD_libseccom_M7.3.6_UsingLiterals

/*!
 * \brief Wrapper class for logging with ara::log::Logger.
 * \details A prefix with component specific information is added to the log message.
 * \vprivate This class is component private.
 */
class TlsAraLogger final {
 public:
  TlsAraLogger(TlsAraLogger const&) = delete;

  TlsAraLogger(TlsAraLogger&&) = delete;

  /*! \brief Default destructor. */
  ~TlsAraLogger() noexcept = default;

  TlsAraLogger& operator=(TlsAraLogger const&) = delete;

  TlsAraLogger& operator=(TlsAraLogger&&) = delete;

  /*!
   * \brief Indicating the module to which logging information is related
   * \vprivate This enum class is component private.
   */
  enum class EnTlsLogModule : std::uint8_t {
    kCentral,
    kHandshake,
    kRecord,
    kHandshakeMessage,
    kMessageAggregation,
    kMemoryStorage,
    kCryptoAdapter
  };

  /*!
   * \brief Converts the enumerate EnTlsLogModule value to a string value
   * \details Is used to allow the enumerate value to be printed in a log message.
   * \param[in] in_value The value that should be converted to a string
   * \return The string value
   * \context Any
   * \pre -
   * \reentrant TRUE
   * \vprivate This method is component private.
   */
  static inline amsr::core::String ModuleToString(TlsAraLogger::EnTlsLogModule const in_value) noexcept {
    amsr::core::String ret_val{};
    switch (in_value) {
      case TlsAraLogger::EnTlsLogModule::kCentral:
        ret_val = "Central";
        break;
      case TlsAraLogger::EnTlsLogModule::kHandshake:
        ret_val = "Handshake";
        break;
      case TlsAraLogger::EnTlsLogModule::kRecord:
        ret_val = "Record";
        break;
      case TlsAraLogger::EnTlsLogModule::kHandshakeMessage:
        ret_val = "HandshakeMessage";
        break;
      case TlsAraLogger::EnTlsLogModule::kMessageAggregation:
        ret_val = "MessageAggregation";
        break;
      case TlsAraLogger::EnTlsLogModule::kMemoryStorage:
        ret_val = "Memory";
        break;
      case TlsAraLogger::EnTlsLogModule::kCryptoAdapter:
        ret_val = "CryptoAdapter";
        break;
      default:
        ret_val = "[Unknown EnTlsLogModule value]";
        break;
    }
    return ret_val;
  }

  /*!
   * \brief Get the singleton instance of TlsAraLogger
   * \return a singleton instance
   * \context Any
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  static TlsAraLogger& Instance() noexcept {
    // VECTOR NL AutosarC++17_10-A3.3.2: MD_libseccom_A3.3.2_DoNotUseNonPODTypeObjectsWithStaticStorageDuration
    static TlsAraLogger singleton_instance{};
    return singleton_instance;
  }

  /*!
   * \brief Set the underlying logger used for logging.
   * \param[in, out] logger The logger which shall be used for writing logs.
   * \context Any
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void SetLogger(std::reference_wrapper<ara::log::Logger> logger) noexcept { this->logger_ = logger; }

  /*!
   * \brief Unset the underlying logger used for logging.
   * \context Any
   * \pre -
   * \reentrant FALSE
   * \vprivate This method is component private.
   */
  void UnsetLogger() noexcept { this->logger_ = {amsr::core::nullopt}; }
  //===================================================================================================================
  // LogFatalAndAbort operation

  /*!
   * \brief Writes a fatal log message
   * \param[in] module The module related to this log
   * \param[in] log_message Log message (const char pointer)
   * \param[in] file_name Filename of the file creating the log entry (const char pointer)
   * \param[in] function_name Name of the function creating the log entry (const char pointer)
   * \param[in] line_number The line number of the log message
   * \param[in] flow_context Pointer to the flow context object ("nullptr" by default)
   * \context Any
   * \pre -
   * \reentrant FALSE
   * \vprivate Vector API Component private.
   */
  [[noreturn]] void LogFatalAndAbort(EnTlsLogModule const module, LogMessage log_message, FileName const file_name,
                                     FunctionName const function_name, LineNumber const line_number,
                                     TlsCommFlowContext const* const flow_context = nullptr) const noexcept {
    amsr::core::String const module_str{ModuleToString(module)};
    amsr::core::String const line_number_str{std::to_string(line_number.value_)};

    amsr::core::String final_message{};

    final_message.append("[");
    final_message.append(module_str);
    final_message.append("]");

    final_message.append("[");
    final_message.append(file_name.value_);
    final_message.append(":");
    final_message.append(function_name.value_);
    final_message.append(":");
    final_message.append(line_number_str);
    final_message.append("]");

    if (flow_context != nullptr) {
      final_message.append("[");
      final_message.append(flow_context->uid);
      final_message.append(":");
      final_message.append(flow_context->message_flow_name);
      if (flow_context->has_error) {
        final_message.append(":error");
      }
      final_message.append("]");
    }

    final_message.append("[");
    final_message.append(log_message);
    final_message.append("]");

    if (logger_.has_value()) {
      logger_->get().LogFatal(
          [&final_message](ara::log::LogStream& stream) noexcept { stream << amsr::core::StringView{final_message}; });
    }

    // "logger_.LogFatal()" does not abort internally.
    // Thus, the message needs to be passed to "amsr::core::Abort()" as well.
    // Moreover, "amsr::core::Abort()" only uses "std::cerr" as log sink.
    // So, if "logger_.LogFatal()" is supposed to write to a file, "amsr::core::Abort()" will still
    // write to "std::cerr".

    amsr::core::Abort(final_message.c_str());
  }
  //===================================================================================================================
  // LogError operation

  /*!
   * \brief Writes an error log message
   * \param[in] module The module related to this log
   * \param[in] log_message Log message (const char pointer)
   * \param[in] file_name Filename of the file creating the log entry (const char pointer)
   * \param[in] function_name Name of the function creating the log entry (const char pointer)
   * \param[in] line_number The line number of the log message
   * \param[in] flow_context Pointer to the flow context object ("nullptr" by default)
   * \context Any
   * \pre -
   * \reentrant FALSE
   * \vprivate Vector API Component private.
   */
  void LogError(EnTlsLogModule const module, LogMessage log_message, FileName const file_name,
                FunctionName const function_name, LineNumber const line_number,
                TlsCommFlowContext const* const flow_context = nullptr) const noexcept {
    if (logger_.has_value()) {
      logger_->get().LogError([&module, &log_message, &file_name, &function_name, &line_number,
                               &flow_context](ara::log::LogStream& stream) noexcept {
        amsr::core::String const module_str{ModuleToString(module)};
        amsr::core::StringView const module_sv{module_str};
        amsr::core::StringView const file_name_sv{file_name.value_};
        amsr::core::StringView const function_name_sv{function_name.value_};
        amsr::core::StringView const log_message_sv{log_message};

        stream << "["_sv << module_sv << "]"_sv;
        stream << "["_sv << file_name_sv << ":"_sv << function_name_sv << ":"_sv << line_number.value_ << "]"_sv;

        if (flow_context != nullptr) {
          amsr::core::StringView const uid_sv{amsr::core::StringView(flow_context->uid)};
          amsr::core::StringView const flow_name_sv{amsr::core::StringView(flow_context->message_flow_name)};
          stream << "["_sv << uid_sv << ":"_sv << flow_name_sv;
          if (flow_context->has_error) {
            stream << ":error"_sv;
          }
          stream << "]"_sv;
        }

        stream << "["_sv << log_message_sv << "]"_sv;
      });
    }
  }
  //===================================================================================================================
  // LogWarn operations

  /*!
   * \brief Writes a warning log message
   * \param[in] module The module related to this log
   * \param[in] log_message Log message (const char pointer)
   * \param[in] file_name Filename of the file creating the log entry (const char pointer)
   * \param[in] function_name Name of the function creating the log entry (const char pointer)
   * \param[in] line_number The line number of the log message
   * \param[in] flow_context Pointer to the flow context object ("nullptr" by default)
   * \context Any
   * \pre -
   * \reentrant FALSE
   * \vprivate Vector API Component private.
   */
  void LogWarn(EnTlsLogModule const module, LogMessage log_message, FileName const file_name,
               FunctionName const function_name, LineNumber const line_number,
               TlsCommFlowContext const* const flow_context = nullptr) const noexcept {
    if (logger_.has_value()) {
      logger_->get().LogWarn([&module, &log_message, &file_name, &function_name, &line_number,
                              &flow_context](ara::log::LogStream& stream) noexcept {
        amsr::core::String const module_str{ModuleToString(module)};
        amsr::core::StringView const module_sv{module_str};
        amsr::core::StringView const file_name_sv{file_name.value_};
        amsr::core::StringView const function_name_sv{function_name.value_};
        amsr::core::StringView const log_message_sv{log_message};

        stream << "["_sv << module_sv << "]"_sv;
        stream << "["_sv << file_name_sv << ":"_sv << function_name_sv << ":"_sv << line_number.value_ << "]"_sv;

        if (flow_context != nullptr) {
          amsr::core::StringView const uid_sv{amsr::core::StringView(flow_context->uid)};
          amsr::core::StringView const flow_name_sv{amsr::core::StringView(flow_context->message_flow_name)};
          stream << "["_sv << uid_sv << ":"_sv << flow_name_sv;
          if (flow_context->has_error) {
            stream << ":error"_sv;
          }
          stream << "]"_sv;
        }

        stream << "["_sv << log_message_sv << "]"_sv;
      });
    }
  }
  //===================================================================================================================
  // LogInfo operations

  /*!
   * \brief Writes an info log message
   * \param[in] module The module related to this log
   * \param[in] log_message Log message (const char pointer)
   * \param[in] file_name Filename of the file creating the log entry (const char pointer)
   * \param[in] function_name Name of the function creating the log entry (const char pointer)
   * \param[in] line_number The line number of the log message
   * \param[in] flow_context Pointer to the flow context object ("nullptr" by default)
   * \context Any
   * \pre -
   * \reentrant FALSE
   * \vprivate Vector API Component private.
   */
  void LogInfo(EnTlsLogModule const module, LogMessage log_message, FileName const file_name,
               FunctionName const function_name, LineNumber const line_number,
               TlsCommFlowContext const* const flow_context = nullptr) const noexcept {
    if (logger_.has_value()) {
      logger_->get().LogInfo([&module, &log_message, &file_name, &function_name, &line_number,
                              &flow_context](ara::log::LogStream& stream) noexcept {
        amsr::core::String const module_str{ModuleToString(module)};
        amsr::core::StringView const module_sv{module_str};
        amsr::core::StringView const file_name_sv{file_name.value_};
        amsr::core::StringView const function_name_sv{function_name.value_};
        amsr::core::StringView const log_message_sv{log_message};

        stream << "["_sv << module_sv << "]"_sv;
        stream << "["_sv << file_name_sv << ":"_sv << function_name_sv << ":"_sv << line_number.value_ << "]"_sv;

        if (flow_context != nullptr) {
          amsr::core::StringView const uid_sv{amsr::core::StringView(flow_context->uid)};
          amsr::core::StringView const flow_name_sv{amsr::core::StringView(flow_context->message_flow_name)};
          stream << "["_sv << uid_sv << ":"_sv << flow_name_sv;
          if (flow_context->has_error) {
            stream << ":error"_sv;
          }
          stream << "]"_sv;
        }

        stream << "["_sv << log_message_sv << "]"_sv;
      });
    }
  }
  //===================================================================================================================
  // LogDebug operations

  /*!
   * \brief Writes a debug log message
   * \param[in] module The module related to this log
   * \param[in] log_message Log message (const char pointer)
   * \param[in] file_name Filename of the file creating the log entry (const char pointer)
   * \param[in] function_name Name of the function creating the log entry (const char pointer)
   * \param[in] line_number The line number of the log message
   * \param[in] flow_context Pointer to the flow context object ("nullptr" by default)
   * \context Any
   * \pre -
   * \reentrant FALSE
   * \vprivate Vector API Component private.
   */
  void LogDebug(EnTlsLogModule const module, LogMessage log_message, FileName const file_name,
                FunctionName const function_name, LineNumber const line_number,
                TlsCommFlowContext const* const flow_context = nullptr) const noexcept {
    if (logger_.has_value()) {
      logger_->get().LogDebug([&module, &log_message, &file_name, &function_name, &line_number,
                               &flow_context](ara::log::LogStream& stream) noexcept {
        amsr::core::String const module_str{ModuleToString(module)};
        amsr::core::StringView const module_sv{module_str};
        amsr::core::StringView const file_name_sv{file_name.value_};
        amsr::core::StringView const function_name_sv{function_name.value_};
        amsr::core::StringView const log_message_sv{log_message};

        stream << "["_sv << module_sv << "]"_sv;
        stream << "["_sv << file_name_sv << ":"_sv << function_name_sv << ":"_sv << line_number.value_ << "]"_sv;

        if (flow_context != nullptr) {
          amsr::core::StringView const uid_sv{amsr::core::StringView(flow_context->uid)};
          amsr::core::StringView const flow_name_sv{amsr::core::StringView(flow_context->message_flow_name)};
          stream << "["_sv << uid_sv << ":"_sv << flow_name_sv;
          if (flow_context->has_error) {
            stream << ":error"_sv;
          }
          stream << "]"_sv;
        }

        stream << "["_sv << log_message_sv << "]"_sv;
      });
    }
  }
  //===================================================================================================================
  // LogVerbose operations

  /*!
   * \brief Writes a verbose log message
   * \param[in] module The module related to this log
   * \param[in] log_message Log message (const char pointer)
   * \param[in] file_name Filename of the file creating the log entry (const char pointer)
   * \param[in] function_name Name of the function creating the log entry (const char pointer)
   * \param[in] line_number The line number of the log message
   * \param[in] flow_context Pointer to the flow context object ("nullptr" by default)
   * \context Any
   * \pre -
   * \reentrant FALSE
   * \vprivate Vector API Component private.
   */
  void LogVerbose(EnTlsLogModule const module, LogMessage log_message, FileName const file_name,
                  FunctionName const function_name, LineNumber const line_number,
                  TlsCommFlowContext const* const flow_context = nullptr) const noexcept {
    if (logger_.has_value()) {
      logger_->get().LogVerbose([&module, &log_message, &file_name, &function_name, &line_number,
                                 &flow_context](ara::log::LogStream& stream) noexcept {
        amsr::core::String const module_str{ModuleToString(module)};
        amsr::core::StringView const module_sv{module_str};
        amsr::core::StringView const file_name_sv{file_name.value_};
        amsr::core::StringView const function_name_sv{function_name.value_};
        amsr::core::StringView const log_message_sv{log_message};

        stream << "["_sv << module_sv << "]"_sv;
        stream << "["_sv << file_name_sv << ":"_sv << function_name_sv << ":"_sv << line_number.value_ << "]"_sv;

        if (flow_context != nullptr) {
          amsr::core::StringView const uid_sv{amsr::core::StringView(flow_context->uid)};
          amsr::core::StringView const flow_name_sv{amsr::core::StringView(flow_context->message_flow_name)};
          stream << "["_sv << uid_sv << ":"_sv << flow_name_sv;
          if (flow_context->has_error) {
            stream << ":error"_sv;
          }
          stream << "]"_sv;
        }

        stream << "["_sv << log_message_sv << "]"_sv;
      });
    }
  }
  //===================================================================================================================
  /*!
   * \brief Checks if messages of specified logging level will make it to the log or not. Used to prevent useless
   * assembly of log messages especially for performance critical code sections.
   * \param[in] log_level Log level to check for
   * \context Any
   * \pre -
   * \return Status of the check
   * \reentrant FALSE
   * \vprivate Vector API Component private.
   */
  bool IsLogLevelSupported(ara::log::LogLevel log_level) const noexcept {
    bool status{false};

    if (logger_.has_value()) {
      status = logger_->get().IsEnabled(log_level);
    }

    return status;
  }
  //===================================================================================================================
 private:
  /*!
   * \brief Constructor for the log wrapper class.
   * \details Is private to secure the singleton pattern
   * \context Any
   * \pre -
   * \reentrant FALSE
   */
  TlsAraLogger() : logger_{amsr::core::nullopt} {}

  /*! \brief The actual wrapped ara::log::Logger used for logging. */
  amsr::core::Optional<std::reference_wrapper<ara::log::Logger>> logger_;
};

}  // namespace common
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_COMMON_TLS_ARA_LOGGER_H_
