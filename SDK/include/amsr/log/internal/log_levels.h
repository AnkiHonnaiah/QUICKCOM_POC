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
/**     \file       log_levels.h
 *      \brief
 *      \details
 *
 *      \unit       LogAndTrace::Logging::LogLevels
 *
 *********************************************************************************************************************/
#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_LOG_LEVELS_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_LOG_LEVELS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <atomic>
#include "amsr/core/string.h"
#include "amsr/log/internal/config/logger_config.h"
#include "amsr/log/internal/filter_result.h"
#include "amsr/log/internal/map.h"
#include "amsr/log/log_level.h"
#include "amsr/log_fwd.h"  // IWYU pragma: export

namespace amsr {
namespace log {
namespace internal {
/*!
 * \brief The set of log levels that are contained for each sink.
 * \details No threadsafe write access. Reading is possible at all times.
 * When writing both the threshold and levels have to be updated.
 * When reading inconsistencies might happen during an update, but do not break the system.
 */
class LogLevels {
  /*! \brief The sink string type. */
  using Sink = core::String;
  /*! \brief The atomic log level. */
  using AtomicLevel = std::atomic<LogLevel>;
  /*! \brief The set of levels for every configured sink. */
  Map<Sink, AtomicLevel> levels_{};
  /*! \brief The threshold to use for checking the levels_ map. */
  AtomicLevel threshold_;
  /*! \brief The default threshold to use when the sinks do not match. */
  AtomicLevel default_;

 public:
  /*!
   * \brief Constructs a new Log Level object.
   * \spec
   *   requires true;
   * \endspec
   */
  explicit LogLevels(LogLevel level) noexcept : threshold_{level}, default_{level} {}

  /*!
   * \brief Sets the Level for the sink.
   *
   * \tparam Key  The type of sink string.
   * \param key   The sink.
   * \param level The value to set.
   * \spec
   *   requires true;
   * \endspec
   */
  void SetLevel(core::StringView key, LogLevel level) noexcept {
    Drop(this->levels_.InsertOrAssign(key, level));
    if (level >= this->threshold_) {
      this->threshold_ = level;
    } else {
      // We might have to set the threshold to the new 'max'.
      this->RefreshThreshold();
    }
  }

  /*!
   * \brief Sets the levels for all sinks.
   *
   * \param level The new level for this logger.
   * \spec
   *   requires true;
   * \endspec
   */
  void SetLevel(LogLevel level) noexcept {
    // VCA_LOGAPI_CHECKED_CONTRACT
    this->levels_.clear();
    this->threshold_ = level;
    this->default_ = level;
  }

  /*!
   * \brief Sets the Levels for each configured sink.
   *
   * \param config The logger configuration.
   * \spec
   *   requires true;
   * \endspec
   */
  void SetLevels(config::LoggerConfig const& config) noexcept {
    if (config.log_level.has_value()) {
      this->default_ = *config.log_level;
    }
    this->threshold_ = this->default_.load();
    // VCA_LOGAPI_VALID_OBJECT
    this->SetLevels(config.sinks);
  }

  /*!
   * \brief Sets the Levels for each configured sink.
   *
   * \param levels The configured sink configs.
   * \spec
   *   requires true;
   * \endspec
   */
  void SetLevels(Map<Sink, LogLevel> const& levels) noexcept {
    // VCA_LOGAPI_CHECKED_CONTRACT
    this->levels_.clear();
    // VCA_LOGAPI_VALID_OBJECT
    for (std::pair<Sink const, LogLevel> const& kv : levels) {
      // VCA_LOGAPI_VALID_OBJECT
      Drop(this->levels_.InsertOrAssign(kv.first, kv.second));
    }
    this->RefreshThreshold();
  }

  /*!
   * \brief Checks if the log level is even relevant.
   *
   * \param candidate The level to check.
   * \return          True in case the level is relevant.
   * \spec
   *   requires true;
   * \endspec
   */
  auto IsRelevant(LogLevel candidate) const noexcept -> bool {
    return LogLevels::IsRelevant(candidate, this->threshold_);
  }

  /*!
   * \brief Checks if the log level is even relevant.
   *
   * \param sink      Sink to check for logger specific override setting.
   * \param candidate The level to check against either logger specific override level
   *                  or Logger default threshold.
   * \return          True in case the log level is relevant, i.e, if candidate
   *                  is lower than sink or logger thresholds.
   * \spec
   *   requires true;
   * \endspec
   */
  auto IsRelevant(core::StringView sink, LogLevel candidate) const noexcept -> FilterResult {
    FilterResult ret_val{FilterResult::FromLoggerFilter(LogLevels::IsRelevant(candidate, this->threshold_))};
    if (ret_val.IsEnabled()) {
      // VCA_LOGAPI_CHECKED_CONTRACT
      MapIter<Sink, AtomicLevel> const it{this->levels_.find(sink)};
      if (it != this->levels_.end()) {
        // VCA_LOGAPI_VALID_OBJECT
        ret_val = FilterResult::FromSinkFilter(LogLevels::IsRelevant(candidate, it->second));
      } else {
        ret_val = FilterResult::FromLoggerFilter(LogLevels::IsRelevant(candidate, this->default_));
      }
    }
    return ret_val;
  }

 private:
  /*!
   * \brief Recalculates the threshold for relevancy.
   * \spec
   *   requires true;
   * \endspec
   */
  void RefreshThreshold() noexcept {
    using KV = Map<Sink, AtomicLevel>::value_type;
    MapIter<Sink, AtomicLevel> const max{
        // VCA_LOGAPI_CHECKED_CONTRACT
        std::max_element(this->levels_.cbegin(), this->levels_.cend(), [](KV const& lhs, KV const& rhs) {
          LogLevel const l{lhs.second.load()};
          LogLevel const r{rhs.second.load()};
          return l < r;
        })};
    // VCA_LOGAPI_VALID_OBJECT
    LogLevel const lhs{max->second.load()};
    LogLevel const rhs{this->default_.load()};
    if (lhs != rhs) {
      this->threshold_ = std::max(lhs, rhs);
    }
  }

  /*!
   * \brief Checks if the log level is even relevant.
   *
   * \param candidate The log level to check.
   * \param reference The log level to check against.
   * \return          True in case the level is allowed and relevant in relation to reference level.
   * \spec
   *   requires true;
   * \endspec
   */
  static auto IsRelevant(LogLevel candidate, LogLevel reference) noexcept -> bool {
    return (candidate != LogLevel::kOff) && (candidate <= reference);
  }
};

}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_LOG_LEVELS_H_
