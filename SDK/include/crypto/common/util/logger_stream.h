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
/*!        \file  logger_stream.h
 *        \brief  Defines a wrapper for the ara::log::LogStream class to prevent logging after it is deinitialized.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_UTIL_LOGGER_STREAM_H_
#define LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_UTIL_LOGGER_STREAM_H_

/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/core/optional.h"
#include "amsr/log/internal/lifecycle.h"
#include "amsr/loguti/stream.h"
#include "ara/log/logging.h"

namespace crypto {
namespace common {
namespace util {

/*!
 * \brief Wraps the ara::log::LogStream class to prevent logging after it is deinitialized.
 * \details This implementation is necessary because the deinitialization interface amsr::core::Deserialize() was
 * introduced without support for the CryptoStack deinitialization. Thus, on client-side the SessionHandler is not
 * guaranteed to be shut down before the logging is deinitialized. This might lead to logging access in an invalid
 * state. As workaround, only this stream wrapper is accessed when logging. It internally checks if the calls can be
 * forwarded to the real logging implementation. As soon as the CryptoStack deinitialization is integrated in
 * amsr::core::Deinitialize(), this wrapper becomes obsolete.
 * \vprivate Product Private
 */
class LoggerStream final {
 private:
  amsr::core::Optional<ara::log::LogStream> stream_; /*!< Wrapped logging stream. */

 public:
  /*!
   * \brief Constructs the object.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  LoggerStream() noexcept : stream_{} {}

  /*!
   * \brief Constructs the object.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  ~LoggerStream() noexcept = default;

  // The same stream should not be accessed in multiple instances.
  LoggerStream(LoggerStream const& stream) noexcept = delete;

  /*!
   * \brief Constructs the object by move.
   * \param[in] stream Stream to move
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  LoggerStream(LoggerStream&& stream) noexcept = default;

  /*!
   * \brief Wraps the given stream.
   * \param[in] stream Stream to wrap
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  void SetStream(ara::log::LogStream&& stream) noexcept { stream_.emplace(std::move(stream)); }

  // The same stream should not be accessed in multiple instances.
  LoggerStream& operator=(LoggerStream const& stream) noexcept = delete;

  /*!
   * \brief Assigns the object by move.
   * \param[in] stream Stream to move.
   * \return The stream.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  LoggerStream& operator=(LoggerStream&& stream) & noexcept {
    if (stream.stream_.has_value()) {
      stream_.emplace(std::move(stream.stream_.value()));
      stream.stream_.reset();
    } else {
      stream_.reset();
    }

    return *this;
  }

  /*!
   * \brief Logs the value using the wrapped stream.
   * \tparam T Type of the value to log
   * \param[in] value Value to log
   * \return The stream.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Product Private
   */
  template <typename T>
  LoggerStream& operator<<(T value) noexcept {
    if (stream_.has_value()) {
      stream_.value() << value;
    }

    return *this;
  }
};

}  // namespace util
}  // namespace common
}  // namespace crypto

#endif  // LIB_LIBCOMMON_INCLUDE_CRYPTO_COMMON_UTIL_LOGGER_STREAM_H_
