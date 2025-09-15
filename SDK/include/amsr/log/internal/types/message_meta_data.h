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
 *      \brief      Contains meta data for message stripped of access to internal data structures.
 *      \details
 *
 *********************************************************************************************************************/
#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_TYPES_MESSAGE_META_DATA_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_TYPES_MESSAGE_META_DATA_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/log/internal/types/meta_data.h"

namespace amsr {
namespace log {
namespace internal {

/*! \brief The type used for DLT Ids. */
using Id = core::Array<char, 4>;

/*! \brief The meta data that is passed to the log sinks. */
class MessageMetaData {
  /*! \brief The reference to the real meta data object. */
  Ref<MetaData const> meta_;

 public:
  /*!
   * \brief Create the object using the internal data.
   *
   * \param meta The internal implementation to use.
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  explicit MessageMetaData(MetaData const& meta) noexcept : meta_{meta} {}

  /*!
   * \brief Gets the timestamp of std::chrono::high_resolution_clock from the meta data.
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  auto GetTime() const noexcept -> HighResTimePoint {
    // VCA_LOGAPI_VALID_OBJECT, VCA_LOGAPI_MEMBER_REF_VALUE
    return this->meta_->GetTime();
  }

  /*!
   * \brief Gets the timestamp of the timestamp clock from the meta data.
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  auto GetTimePoint() const noexcept -> TimestampTimePoint {
    // VCA_LOGAPI_VALID_OBJECT, VCA_LOGAPI_MEMBER_REF_VALUE
    return this->meta_->GetTimePoint();
  }

  /*!
   * \brief Gets the severity from the meta data.
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  auto GetSeverity() const noexcept -> LogLevel { return this->meta_->GetSeverity(); }

  /*!
   * \brief Gets the application ID.
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  auto GetApplicationId() const noexcept -> Id {
    // VCA_LOGAPI_VALID_OBJECT, VCA_LOGAPI_MEMBER_REF_VALUE
    return this->meta_->GetApplicationId();
  };

  /*!
   * \brief Gets the context for the message.
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  auto GetContext() const noexcept -> core::StringView {
    // VCA_LOGAPI_VALID_OBJECT, VCA_LOGAPI_MEMBER_REF_VALUE
    return this->meta_->GetContext();
  };

  /*!
   * \brief Gets the location from the meta data.
   *
   * \pre         -
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  auto GetLocation() const noexcept -> Optional<Ref<Location const>> {
    // VCA_LOGAPI_VALID_OBJECT, VCA_LOGAPI_MEMBER_REF_VALUE
    return this->meta_->GetLocation();
  };
};
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_TYPES_MESSAGE_META_DATA_H_
