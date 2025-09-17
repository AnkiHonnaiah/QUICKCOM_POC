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
 *      \brief      Contains native message data types.
 *      \details
 *
 *********************************************************************************************************************/
#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_TYPES_MESSAGE_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_TYPES_MESSAGE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/core/byte.h"
#include "amsr/core/error_code.h"
#include "amsr/core/optional.h"
#include "amsr/core/string.h"
#include "amsr/core/variant.h"
#include "amsr/core/vector.h"
#include "amsr/log/internal/memory_pool.h"
#include "amsr/log/internal/ref.h"
#include "amsr/log/internal/utils.h"
#include "amsr/log/log_types.h"
#include "amsr/log_fwd.h"
#include "ara/core/vector.h"

#include "amsr/log/internal/types/types.h"

namespace amsr {
namespace log {
namespace internal {

/*! \brief The internal implementation of a message. */
using MessageImpl = ara::core::Vector<LogType>;

/*! \brief A message that is passed to the log sinks. */
class Message {
 private:
  /*! \brief A reference to the real underlying message. */
  Ref<MessageImpl const> message_;

 public:
  /*!
   * \brief Creates a message.
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  explicit Message(MessageImpl const& message) noexcept : message_{message} {}

  /*!
   * \brief Creates a constant start iterator.
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  auto cbegin() const noexcept -> typename MessageImpl::const_iterator {  // NOLINT(readability-identifier-naming)
    // VCA_LOGAPI_VALID_OBJECT
    return this->message_->cbegin();
  }

  /*!
   * \brief Creates a constant end iterator.
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  auto cend() const noexcept -> typename MessageImpl::const_iterator {  // NOLINT(readability-identifier-naming)
    // VCA_LOGAPI_VALID_OBJECT
    return this->message_->cend();
  }

  /*!
   * \brief Creates a constant start iterator.
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  auto begin() const noexcept -> typename MessageImpl::const_iterator {  // NOLINT(readability-identifier-naming)
    return this->cbegin();
  }

  /*!
   * \brief Creates a constant end iterator.
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  auto end() const noexcept -> typename MessageImpl::const_iterator {  // NOLINT(readability-identifier-naming)
    return this->cend();
  }

  /*!
   * \brief Checks if the message is empty.
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  auto empty() const noexcept -> bool {  // NOLINT(readability-identifier-naming)
    // VCA_LOGAPI_VALID_OBJECT
    return this->message_->empty();
  }
};

}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_TYPES_MESSAGE_H_
