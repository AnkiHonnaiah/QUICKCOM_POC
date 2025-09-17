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
/*!        \file  message_serializer.h
 *        \brief  Serializer for messages.
 *
 *      \details  The transporter interface provides capabilities to send and receive raw data. This serializer
 *                converts the data into the corresponding message. Here only primitive types will be transferred, all
 *                other types will be mirrored using proxy objects on client and skeleton objects on server side which
 *                are connected using a unique ProxyId. Therefor object serialization will send the only the ProxyId
 *                of any object to be transferred.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCOMMON_INCLUDE_AMSR_SEC_IPC_MESSAGE_SERIALIZER_H_
#define LIB_LIBCOMMON_INCLUDE_AMSR_SEC_IPC_MESSAGE_SERIALIZER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <array>
#include <limits>
#include <memory>
#include <tuple>
#include <utility>
#include <vector>

#include "amsr/core/array.h"
#include "amsr/sec/ipc/primitive_serializer.h"
#include "ara/core/vector.h"
#include "crypto/common/util/logger.h"

namespace amsr {
namespace sec {
namespace ipc {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_CRYPTO_AutosarC++17_10-M7.3.6_sv
/*! Introduce string literal for StringView creation */
using vac::container::literals::operator"" _sv;

/*!
 * \brief Current buffer position. Wrapped in a type to enable performance tweaking if necessary.
 * \vprivate Component Private
 */
using BufferPosition = std::size_t;

// VECTOR NC Metric-OO.RFC: MD_CRYPTO_Metric-OO.RFC
/*!
 * \brief Serializer for messages.
 * \details Writes serialized messages into a buffer in order to prepare them for transmission using transporters.
 * \vprivate Component Private
 */
class MessageSerializer {
 public:
  /*!
   * \brief Constructor.
   * \param[in] primitive_serializer  Serializer for data primitives.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  explicit MessageSerializer(PrimitiveSerializer primitive_serializer) noexcept
      : primitive_serializer_(std::move(primitive_serializer)) {}

  // VECTOR NC AutosarC++17_10-A15.5.1: MD_CRYPTO_AutosarC++17_10-A15.5.1_function_must_not_exit_with_an_exception
  /*!
   * \brief Destructor
   * \vprivate Component Private
   */
  virtual ~MessageSerializer() = default;

  /*!
   * \brief Default constructor
   * \vprivate Component Private
   */
  MessageSerializer() noexcept : MessageSerializer(PrimitiveSerializer{}) {}

  // VECTOR NC AutosarC++17_10-A12.8.6: MD_CRYPTO_AutosarC++17_10-A12.8.6_only_virtual_for_mock
  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  MessageSerializer(MessageSerializer const& /*other*/) = default;

  // VECTOR NC AutosarC++17_10-A12.8.6: MD_CRYPTO_AutosarC++17_10-A12.8.6_only_virtual_for_mock
  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  MessageSerializer(MessageSerializer&& /*other*/) noexcept = default;

  // VECTOR NC AutosarC++17_10-A12.8.6: MD_CRYPTO_AutosarC++17_10-A12.8.6_only_virtual_for_mock
  /*!
   * \brief Default copy assignment operator
   * \return reference to this
   * \vprivate Component Private
   */
  MessageSerializer& operator=(MessageSerializer const& /*other*/) & = default;

  // VECTOR NC AutosarC++17_10-A12.8.6: MD_CRYPTO_AutosarC++17_10-A12.8.6_only_virtual_for_mock
  /*!
   * \brief Default move assignment operator
   * \return reference to this
   * \vprivate Component Private
   */
  MessageSerializer& operator=(MessageSerializer&& /*other*/) & noexcept = default;

  /*!
   * \brief Generic method for serializing messages with different content types.
   * \param[in] message Message containing the content tuple to be serialized.
   * \param[out] buffer Buffer to store the serialized data.
   * \return Subspan of the input buffer that contains the serialized message.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kUnsupportedFormat Serialization of message failed.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename TaskEnumClass, typename... ArgumentTypes>
  amsr::core::Result<ara::crypto::ReadOnlyMemRegion> SerializeMessage(
      BasicMessage<TaskEnumClass, ArgumentTypes...> const& message,
      ara::crypto::WritableMemRegion buffer) const noexcept {
    amsr::core::Result<ara::crypto::ReadOnlyMemRegion> result{ara::crypto::SecurityErrc::kNoError};
    BufferPosition buffer_pos{0};
    if (!SerializeTask<Task>(message.BasicTask(), buffer_pos, buffer).HasValue()) {
      logger_.LogError(AMSR_FILE, __LINE__) << "Failed to serialize basic task.";
      result.EmplaceError(ara::crypto::SecurityErrc::kUnsupportedFormat);
    } else if (!SerializeTask<TaskEnumClass>(message.DetailTask(), buffer_pos, buffer).HasValue()) {
      logger_.LogError(AMSR_FILE, __LINE__) << "Failed to serialize detail task.";
      result.EmplaceError(ara::crypto::SecurityErrc::kUnsupportedFormat);
    } else if (!primitive_serializer_.Serialize(message.GetProxyId(), buffer_pos, buffer).HasValue()) {
      logger_.LogError(AMSR_FILE, __LINE__) << "Failed to serialize object id.";
      result.EmplaceError(ara::crypto::SecurityErrc::kUnsupportedFormat);
    } else if (!primitive_serializer_.Serialize(message.NewProxyId().size(), buffer_pos, buffer).HasValue()) {
      logger_.LogError(AMSR_FILE, __LINE__) << "Failed to serialize object ids length.";
      result.EmplaceError(ara::crypto::SecurityErrc::kUnsupportedFormat);
    } else {
      for (amsr::sec::ipc::ProxyId const& proxy_id : message.NewProxyId()) {
        if (!PrimitiveSerializer::Serialize(proxy_id, buffer_pos, buffer).HasValue()) {
          logger_.LogError(AMSR_FILE, __LINE__) << "Failed to serialize object ids.";
          result.EmplaceError(ara::crypto::SecurityErrc::kUnsupportedFormat);
          break;
        }
      }

      if (result == ara::crypto::SecurityErrc::kNoError) {
        if (!ForEach(message.Content(), primitive_serializer_, buffer_pos, buffer).HasValue()) {
          result.EmplaceError(ara::crypto::SecurityErrc::kUnsupportedFormat);
        }
      }
    }

    if (result == ara::crypto::SecurityErrc::kNoError) {
      result.EmplaceValue(buffer.subspan(0, buffer_pos));
    }
    return result;
  }

  /*!
   * \brief Returns the length required for serializing a ara::crypto::ReadOnlyMemRegion.
   * \param[in] message Message containing the content tuple to be checked.
   * \return A tuple with first element being \c length in bytes required to serialize the message
   *         and the second element being the \c overhead that went to serialize the message.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \internal
   * - Calculate the \c length as the sum of the individual message parts (BasicTask, DetailTask, ProxyId, NewProxyId,
   * Content).
   * - Calculate the \c overhead as the sum of all message parts except the content.
   * \endinternal
   * \vprivate Component Private
   */
  template <typename TaskEnumClass>
  static std::tuple<std::size_t, std::size_t> GetSerializedMessageBufferLength(
      BasicMessage<TaskEnumClass, ara::crypto::ReadOnlyMemRegion&> message) noexcept {
    std::size_t length{0};
    std::size_t overhead{0};

    // Calculate length for message's BasicTask()
    length += sizeof(message.BasicTask());
    // Calculate length for message's DetailTask() */
    length += sizeof(message.DetailTask());
    // Calculate length for message's ObjectId()
    length += sizeof(message.GetProxyId());
    // Calculate length for message's NewProxyId()'s size
    length += sizeof(ara::core::Vector<amsr::sec::ipc::ProxyId>::size_type);
    // Calculate length for message's NewProxyId() content
    length += message.NewProxyId().size() * sizeof(amsr::sec::ipc::ProxyId);

    // Primitive serialization uses a std::uint32_t in front to indicate size of the serialized ReadOnlyMemRegion
    length += sizeof(std::uint32_t);

    // VECTOR NL AutosarC++17_10-M8.5.2: MD_CRYPTO_AutosarC++17_10-M8.5.2_template_type
    std::tuple<ara::crypto::ReadOnlyMemRegion&> const content_tuple{message.Content()};
    ara::crypto::ReadOnlyMemRegion const& message_content{std::get<0>(content_tuple)};
    overhead = length;
    length += message_content.size();
    return std::make_tuple(length, overhead);
  }

  /*!
   * \brief Returns the length required for serializing a ara::crypto::ReadOnlyMemRegion.
   * \param[in] message Message containing the content tuple to be checked.
   * \return A tuple with first element being \c length in bytes required to serialize the message
   *         and the second element being the \c overhead that went to serialize the message.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \internal
   * - Calculate the \c length as the sum of the indvidiual message parts (BasicTask, DetailTask, ProxyId, NewProxyId,
   * Content).
   * - Calculate the \c overhead as the sum of all message parts except the content.
   * \endinternal
   * \vprivate Component Private
   */
  template <typename TaskEnumClass>
  static std::tuple<std::size_t, std::size_t> GetSerializedMessageBufferLength(
      BasicMessage<TaskEnumClass, ara::crypto::WritableMemRegion&> message) noexcept {
    std::size_t length{0};
    std::size_t overhead{0};

    // Calculate length for message's BasicTask()
    length += sizeof(message.BasicTask());
    // Calculate length for message's DetailTask()
    length += sizeof(message.DetailTask());
    // Calculate length for message's ObjectId()
    length += sizeof(message.GetProxyId());
    // Calculate length for message's NewProxyId()'s size
    length += sizeof(ara::core::Vector<amsr::sec::ipc::ProxyId>::size_type);
    // Calculate length for message's NewProxyId() content
    length += message.NewProxyId().size() * sizeof(amsr::sec::ipc::ProxyId);

    // Primitive serialization uses a std::uint32_t in front to indicate size of the serialized ReadOnlyMemRegion
    length += sizeof(std::uint32_t);

    // VECTOR NL AutosarC++17_10-M8.5.2: MD_CRYPTO_AutosarC++17_10-M8.5.2_template_type
    std::tuple<ara::crypto::WritableMemRegion&> const content_tuple{message.Content()};
    ara::crypto::WritableMemRegion const& message_content{std::get<0>(content_tuple)};
    overhead = length;
    length += message_content.size();
    return std::make_tuple(length, overhead);
  }

 private:
  /*!
   * \brief Template (internal recursion) for calling a given function with each element of an std::tuple.
   * \param[in] tuple The tuple to be iterated.
   * \param[in] serializer PrimitiveSerializer to serialize the individual values with.
   * \param[out] buffer_pos buffer position.
   * \param[out] buffer buffer to write the serialized message to.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kRuntimeFault Runtime error.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename Tuple, std::size_t... Indices>
  amsr::core::Result<void> ForEachImpl(Tuple&& tuple, std::index_sequence<Indices...>,
                                       PrimitiveSerializer const& serializer, BufferPosition& buffer_pos,
                                       ara::crypto::WritableMemRegion buffer) const noexcept {
    amsr::core::Result<void> result{ara::crypto::SecurityErrc::kNoError};
    static_cast<void>(buffer);
    amsr::core::Array<bool, std::tuple_size<std::remove_reference_t<Tuple>>::value> serial_success{
        serializer.Serialize(std::get<Indices>(std::forward<Tuple>(tuple)), buffer_pos, buffer).HasValue()...};

    for (bool const success : serial_success) {
      if (!success) {
        logger_.LogError(AMSR_FILE, __LINE__) << "Failed to serialize argument.";
        result.EmplaceError(ara::crypto::SecurityErrc::kRuntimeFault);
        break;
      }
    }

    if (result == ara::crypto::SecurityErrc::kNoError) {
      result.EmplaceValue();
    }

    return result;
  }

  /*!
   * \brief Template for calling a given function with each element of an std::tuple.
   * \param[in] tuple The tuple to be iterated.
   * \param[in] serializer PrimitiveSerializer to serialize the individual values with.
   * \param[out] buffer_pos buffer position.
   * \param[out] buffer buffer to write the serialized message to.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kRuntimeFault Runtime error.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename Tuple>
  amsr::core::Result<void> ForEach(Tuple&& tuple, PrimitiveSerializer const& serializer, BufferPosition& buffer_pos,
                                   ara::crypto::WritableMemRegion buffer) const noexcept {
    constexpr std::size_t N{std::tuple_size<std::remove_reference_t<Tuple>>::value};
    return ForEachImpl(std::forward<Tuple>(tuple), std::make_index_sequence<N>{}, serializer, buffer_pos, buffer);
  }

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_function_cannot_be_made_static_false_positive
  /*!
   * \brief Generic function to serialize all kinds of Task enums based on TaskId_t.
   * \param[in] value Value to be serialized.
   * \param[in,out] buffer_pos Current position of the buffer, will be updated after serialization.
   * \param[out] buffer Output buffer to store serialized data.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kRuntimeFault Runtime error.
   * \error ara::crypto::SecurityErrc::kInsufficientCapacity Buffer too small to hold serialized message.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename TaskEnumClass>
  amsr::core::Result<void> SerializeTask(TaskEnumClass const value, BufferPosition& buffer_pos,
                                         ara::crypto::WritableMemRegion buffer) const noexcept {
    amsr::core::Result<void> result{ara::crypto::SecurityErrc::kRuntimeFault};
    if ((buffer.size() - buffer_pos) < sizeof(value)) {
      logger_.LogError(AMSR_FILE, __LINE__)
          << "Serialization error: Buffer too small to hold serialized message of required type.";
      result.EmplaceError(ara::crypto::SecurityErrc::kInsufficientCapacity);
    } else {
      TaskId_t const task_value{static_cast<TaskId_t>(value)};
      static_assert(sizeof(TaskId_t) == sizeof(TaskEnumClass), "Size of TaskId_t does not match size of provided Task");
      result = PrimitiveSerializer::Serialize(task_value, buffer_pos, buffer);
    }

    return result;
  }

  PrimitiveSerializer primitive_serializer_; /*!< Serializer for primitive values */
  ::crypto::common::util::Logger logger_{::crypto::common::util::kCommonLoggingContext,
                                         ""_sv}; /*!< Logging instance. */
};

}  // namespace ipc
}  // namespace sec
}  // namespace amsr

#endif  // LIB_LIBCOMMON_INCLUDE_AMSR_SEC_IPC_MESSAGE_SERIALIZER_H_
