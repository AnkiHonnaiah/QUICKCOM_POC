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
#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_TYPES_TYPED_MESSAGE_BUILDER_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_TYPES_TYPED_MESSAGE_BUILDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include <utility>

#include "amsr/log/internal/types/message.h"
#include "amsr/log/internal/types/meta_data.h"

namespace amsr {
namespace log {
namespace internal {
/*! \brief The initial size for each dynamic pool element. */
constexpr static std::size_t kInitialBufferSize{128ul};

/*! \brief Contains everything to build a single log message. */
// VCA_LOGAPI_MOLE_1298
class TypedMessageBuilder {
 public:
  /*! \brief The memory pool type used for strings. */
  using StringPool = MemoryPool<StringBuffer, sync::Strategy::kEmpty>;
  /*! \brief The memory pool type used for raw buffers. */
  using RawPool = MemoryPool<RawBuffer, sync::Strategy::kEmpty>;

 private:
  /*! \brief The memory pool used for strings. */
  StringPool string_pool_{1,
                          // VECTOR NC AutosarC++17_10-A15.4.2: MD_LOG_AutosarC++17_10-A15.4.2
                          [](StringBuffer& name) noexcept {
                            name.reserve(kInitialBufferSize);
                            name.clear();
                          }};  // namespace internal

  /*! \brief The memory pool used for raw buffers. */
  RawPool raw_pool_{
      0,
      // VECTOR NC AutosarC++17_10-A15.4.2: MD_LOG_AutosarC++17_10-A15.4.2
      // VECTOR NC AutosarC++17_10-A15.5.3: MD_LOG_AutosarC++17_10-A15.5.3
      [](RawBuffer& raw) noexcept {
        // VCA_LOGAPI_CHECKED_CONTRACT
        raw.reserve(kInitialBufferSize);
        raw.clear();
      }  // namespace log
  };     // namespace amsr
  /*! \brief The assembled log message data. */
  MessageImpl message_{};
  /*! \brief The assembled log message meta data. */
  MetaDataImpl meta_data_{};

 public:
  /*!
   * \brief Default constructs the message builder.
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_LOG_AutosarC++17_10-A15.4.2
  TypedMessageBuilder() noexcept {
    constexpr static std::size_t kInitialPoolSize{10ul};
    // VCA_LOGAPI_CHECKED_CONTRACT
    this->message_.reserve(kInitialPoolSize);
  }

  /*!
   * \brief Gets the message data object.
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  auto GetLogData() noexcept -> MessageImpl& { return this->message_; }

  /*!
   * \brief Gets the wrapped message data object.
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  auto GetLogData() const noexcept -> Message { return Message{this->message_}; }

  /*!
   * \brief Sets the message meta data object.
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  void SetMetaData(MetaDataImpl&& meta_data) noexcept { this->meta_data_ = std::move(meta_data); }

  /*!
   * \brief Gets the message meta data object.
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  auto GetMetaData() noexcept -> MetaDataImpl& { return this->meta_data_; }

  /*!
   * \brief Gets the wrapped message meta data object.
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  auto GetMetaData() const noexcept -> MetaData { return MetaData{this->meta_data_}; }

  /*!
   * \brief Gets a string from the pool.
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  auto GetNewString() noexcept -> StringShard { return this->string_pool_.Get(); }

  /*!
   * \brief Gets a raw buffer from the pool.
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  auto GetNewRawBuffer() noexcept -> RawShard { return this->raw_pool_.Get(); }

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_LOG_AutosarC++17_10-M9.3.3_stable_api
  // VECTOR NC AutosarC++17_10-A7.5.1: MD_LOG_AutosarC++17_10-A7.5.1_return_input_as_reference
  /*!
   * \brief Inserts a value into the message data object.
   *
   * \tparam T Type to be inserted.
   *           Can be an arbitrary type.
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  template <typename T>
  auto InsertData(T const& t) noexcept -> T const& {
    return t;
  }

  /*!
   * \brief Inserts a string view into the message data object.
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  auto InsertData(core::StringView sv) noexcept -> StringShard {
    StringShard shard{this->GetNewString()};
    // VCA_LOGAPI_VALID_OBJECT
    shard->assign(sv.data(), sv.size());
    return shard;
  }

  /*!
   * \brief Inserts a raw buffer into the message data object.
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  auto InsertData(LogRawBuffer const& buffer) noexcept -> RawShard {
    using vac::container::literals::operator""_sv;

    RawShard shard{this->GetNewRawBuffer()};
    AssertDebug(buffer.size <= static_cast<std::size_t>(std::numeric_limits<std::ptrdiff_t>::max()),
                "Messages longer than ptrdiff type are not supported."_sv);
    // VCA_LOGAPI_VALID_OBJECT
    shard->assign(buffer.buffer, std::next(buffer.buffer, static_cast<std::ptrdiff_t>(buffer.size)));
    return shard;
  }

  /*!
   * \brief Clears the message object for reuse.
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  void Clear() noexcept {
    // VCA_LOGAPI_CHECKED_CONTRACT
    this->message_.clear();
    this->meta_data_ = MetaDataImpl{};
  }
};
}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_TYPES_TYPED_MESSAGE_BUILDER_H_
