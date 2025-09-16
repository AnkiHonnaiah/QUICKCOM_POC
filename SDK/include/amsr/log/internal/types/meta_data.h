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
/**     \file
 *      \brief      Contains meta data for a message.
 *      \details
 *
 *********************************************************************************************************************/
#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_TYPES_META_DATA_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_TYPES_META_DATA_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/array.h"
#include "amsr/log/internal/log_levels.h"
#include "amsr/log/internal/time.h"
#include "amsr/log/internal/types/types.h"
#include "amsr/log/internal/utils.h"
#include "amsr/log_fwd.h"  // IWYU pragma: keep

namespace amsr {
namespace log {
namespace internal {

/*! \brief A location for a message. */
struct Location {
  /*! \brief The file from which a message stems. */
  String file;
  /*! \brief The line from which a message stems. */
  UInt line;
};

/*! \brief The internal implementation for meta data. */
struct MetaDataImpl {
  /*! \brief The std::high_resolution_clock timestamp of the message. */
  HighResTimePoint time{};
  /*! \brief The severity of the message. */
  LogLevel severity{LogLevel::kFatal};
  /*! \brief The logger from which the message stems. */
  Logger const* logger{nullptr};
  /*! \brief The location from which a message is sent. */
  Optional<Location> location{};
  /*! \brief The timestamp of the message */
  TimestampTimePoint time_point{};
};

/*! \brief The type used for DLT Ids. */
using Id = core::Array<char, 4>;

/*! \brief The meta data that is passed to the log sinks. */
class MetaData {
  /*! \brief The reference to the real meta data object. */
  Ref<MetaDataImpl const> impl_;

 public:
  /*!
   * \brief Create the object using the internal data.
   *
   * \param impl The internal implementation to use.
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  explicit MetaData(MetaDataImpl const& impl) noexcept : impl_{impl} {}

  /*!
   * \brief Gets the timestamp of std::chrono::high_resolution_clock from the meta data.
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  auto GetTime() const noexcept -> HighResTimePoint { return this->impl_->time; }

  /*!
   * \brief Gets the timestamp of the timestamp clock from the meta data.
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  auto GetTimePoint() const noexcept -> TimestampTimePoint { return this->impl_->time_point; }

  /*!
   * \brief Gets the severity from the meta data.
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  auto GetSeverity() const noexcept -> LogLevel { return this->impl_->severity; }

  /*!
   * \brief Checks if the log level is enabled for the passed log sink.
   *
   * \param sink The sink to check.
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  auto IsSinkEnabled(core::StringView sink) const noexcept -> FilterResult;

  /*!
   * \brief Gets the application ID.
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  auto GetApplicationId() const noexcept -> Id;

  /*!
   * \brief Gets the context for the message.
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  auto GetContext() const noexcept -> core::StringView;

  /*!
   * \brief Gets the location from the meta data.
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  auto GetLocation() const noexcept -> Optional<Ref<Location const>>;
};
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_TYPES_META_DATA_H_
