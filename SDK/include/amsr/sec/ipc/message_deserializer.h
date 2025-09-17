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
/*!        \file  message_deserializer.h
 *        \brief  Deserializer for messages.
 *
 *      \details  The transporter interface provides capabilities to send and receive raw data. This deserializer
 *                converts the data from the corresponding message. Here only primitive types will be transferred, all
 *                other types will be mirrored using proxy objects on client and skeleton objects on server side which
 *                are connected using a unique ProxyId. Therefor object deserialization receives an ProxyId and
 *                performs a lookup in an object database provided by the ObjectProvider interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCOMMON_INCLUDE_AMSR_SEC_IPC_MESSAGE_DESERIALIZER_H_
#define LIB_LIBCOMMON_INCLUDE_AMSR_SEC_IPC_MESSAGE_DESERIALIZER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>
#include <vector>

#include "amsr/core/string.h"
#include "amsr/core/string_view.h"
#include "amsr/sec/ipc/basic_message.h"
#include "amsr/sec/ipc/primitive_deserializer.h"
#include "amsr/sec/ipc/primitive_serializer.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/common/trusted_container.h"
#include "ara/crypto/cryp/domain_parameters.h"
#include "ara/crypto/cryp/key.h"
#include "ara/crypto/keys/key_slot_content_props.h"
#include "ara/crypto/keys/key_slot_prototype_props.h"
#include "ara/crypto/keys/user_permissions.h"
#include "vac/language/remove_shared_ptr.h"

namespace amsr {
namespace sec {
namespace ipc {

/*!
 * \brief  Deserializer for messages
 * \details The transporter interface provides capabilities to send and receive raw data. This deserializer
 * converts the data from the corresponding message. Here only primitive types will be transferred, all
 * other types will be mirrored using proxy objects on client and skeleton objects on server side which
 * are connected using a unique ProxyId. Therefor object deserialization receives an ProxyId and
 * performs a lookup in an object database provided by the ObjectProvider interface.
 * \vprivate Component Private
 */
class MessageDeserializer {
 public:
  /*!
   * \brief Constructor.
   * \param[in] primitive_deserializer  Deserializer for primitive data types.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  explicit MessageDeserializer(PrimitiveDeserializer const&& primitive_deserializer)
      : primitive_deserializer_{primitive_deserializer} {}

  /*!
   * \brief Destructor
   * \vprivate Component Private
   */
  virtual ~MessageDeserializer() noexcept = default;

  /*!
   * \brief Default constructor
   * \vprivate Component Private
   */
  MessageDeserializer() = default;

  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  MessageDeserializer(MessageDeserializer const& /*other*/) = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  MessageDeserializer(MessageDeserializer&& /*other*/) noexcept = default;

  /*!
   * \brief Default copy assignment operator
   * \return reference to this
   * \vprivate Component Private
   */
  MessageDeserializer& operator=(MessageDeserializer const& /*other*/) & = default;

  /*!
   * \brief Default move assignment operator
   * \return reference to this
   * \vprivate Component Private
   */
  MessageDeserializer& operator=(MessageDeserializer&& /*other*/) & noexcept = default;

  /*!
   * \brief Generic deserializer for all message tasks based on TaskId_t. Performs buffer length checks and
   *        calls the primitive deserializer.
   * \param[in] buffer The buffer holding the data
   * \param[out] buffer_pos Buffer position is updated to position after the deserialized value
   * \return Task with type from template argument.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename TaskEnumClass>
  TaskEnumClass DeserializeTask(ara::crypto::ReadOnlyMemRegion buffer, BufferPosition& buffer_pos) const noexcept {
    static_assert(sizeof(TaskId_t) == sizeof(TaskEnumClass), "Size mismatch: Size of Task must be equal to TaskId_t.");
    MessageObjectLifetimeManager const object_lifetime_manager{};
    TaskId_t const tmp{DeserializeGenericValue<TaskId_t>(buffer, object_lifetime_manager, buffer_pos)};
    // VECTOR NC AutosarC++17_10-A7.2.1: MD_CRYPTO_AutosarC++17_10-A7.2.1
    return static_cast<TaskEnumClass>(tmp);
  }

  /*!
   * Template based deserialization function.
   * \param[in] buffer Buffer containing (at least) the data to be deserialized.
   * \param[in] object_lifetime_manager Buffer containing (at least) the data to be deserialized.
   * \return deserialized message.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename TaskEnumClass, typename... ArgumentTypes>
  auto DeserializeMessage(ara::crypto::ReadOnlyMemRegion buffer,
                          MessageObjectLifetimeManager& object_lifetime_manager) const noexcept
      -> BasicMessage<TaskEnumClass, ArgumentTypes...> {
    BufferPosition buffer_pos{0};

    Task const basic_task{DeserializeTask<Task>(buffer, buffer_pos)};
    TaskEnumClass const detail_task{DeserializeTask<TaskEnumClass>(buffer, buffer_pos)};
    amsr::sec::ipc::ProxyId const tmp_oid{
        DeserializeGenericValue<amsr::sec::ipc::ProxyId>(buffer, object_lifetime_manager, buffer_pos)};
    ara::core::Vector<amsr::sec::ipc::ProxyId> const& tmp_noid{
        DeserializeGenericValue<ara::core::Vector<amsr::sec::ipc::ProxyId>>(buffer, object_lifetime_manager,
                                                                            buffer_pos)};

    // VECTOR NC VectorC++-V5.0.1: MD_CRYPTO_VectorC++-V5.0.1
    // VECTOR NL AutosarC++17_10-A5.0.1: MD_CRYPTO_AutosarC++17_10-A5.0.1
    BasicMessage<TaskEnumClass, ArgumentTypes...> const message{
        basic_task, detail_task, tmp_oid, tmp_noid,
        DeserializeGenericValue<ArgumentTypes>(buffer, object_lifetime_manager, buffer_pos)...};

    return message;
  }

 protected:
  /*!
   * \brief Template function for deserializing generic value. This template uses SFINAE and handles all non-void
   * amsr::core::Result<T> types.
   * \tparam ReturnType Type to be deserialized.
   * \param[in] buffer The buffer holding the data.
   * \param[out] buffer_pos Buffer position is updated to position after the deserialized value.
   * \return deserialized value.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <
      typename ReturnType,
      typename std::enable_if<(::crypto::common::IsResult<ReturnType>::value) &&
                                  (!std::is_void<typename ::crypto::common::RemoveResult<ReturnType>::type>::value),
                              std::int32_t>::type = 0>
  ReturnType DeserializeGenericValue(ara::crypto::ReadOnlyMemRegion buffer,
                                     MessageObjectLifetimeManager const& /*object_lifetime_manager*/,
                                     BufferPosition& buffer_pos) const noexcept {
    // VECTOR NL AutosarC++17_10-A2.8.3: MD_CRYPTO_AutosarC++17_10-A2.8.3_false_positive
    using ResultValueType = typename ::crypto::common::RemoveResult<ReturnType>::type; /*!< Type of the value. */
    ReturnType out_value{ResultValueType{}};

    static_cast<void>(primitive_deserializer_.Deserialize(buffer, buffer_pos, out_value));

    return out_value;
  }

  /*!
   * \brief Template function for deserializing generic value. This template uses SFINAE and handles all non-void
   * amsr::core::Optional<T> types.
   * \tparam ReturnType Type to be deserialized.
   * \param[in] buffer The buffer holding the data.
   * \param[out] buffer_pos Buffer position is updated to position after the deserialized value.
   * \return deserialized value.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename ReturnType,
            typename std::enable_if<::crypto::common::IsOptional<ReturnType>::value, std::int32_t>::type = 0>
  static auto DeserializeGenericValue(ara::crypto::ReadOnlyMemRegion buffer,
                                      MessageObjectLifetimeManager const& /*object_lifetime_manager*/,
                                      BufferPosition& buffer_pos) noexcept -> ReturnType {
    // VECTOR NL AutosarC++17_10-A2.8.3: MD_CRYPTO_AutosarC++17_10-A2.8.3_false_positive
    using OptionalValueType = typename ::crypto::common::RemoveOptional<ReturnType>::type; /*!< Type of the value. */
    ReturnType out_value{OptionalValueType{}};

    static_cast<void>(PrimitiveDeserializer::Deserialize(buffer, buffer_pos, out_value));

    return out_value;
  }

  /*!
   * \brief Template function for deserializing generic value. This template uses SFINAE and handles
   * amsr::core::StringView types.
   * \tparam ReturnType Type to be deserialized.
   * \param[in] buffer The buffer holding the data.
   * \param[in] object_lifetime_manager lifetime manager for heap allocated objects.
   * \param[out] buffer_pos Buffer position is updated to position after the deserialized value.
   * \return deserialized value.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename ReturnType,
            typename std::enable_if<::crypto::common::IsStringView<ReturnType>::value, std::int32_t>::type = 0>
  ReturnType DeserializeGenericValue(ara::crypto::ReadOnlyMemRegion buffer,
                                     MessageObjectLifetimeManager& object_lifetime_manager,
                                     BufferPosition& buffer_pos) const noexcept {
    std::unique_ptr<amsr::core::String> str{std::make_unique<amsr::core::String>()};

    static_cast<void>(primitive_deserializer_.Deserialize(buffer, buffer_pos, *str.get()));
    amsr::core::StringView const view{&(*str)[0], str->length()};
    object_lifetime_manager.RegisterObjectForRelease(std::move(str));

    return view;
  }

  /*!
   * \brief Template function for deserializing generic value. This function is not intended to do anything but it is
   * used as proxy that forwards all calls to the correct function in PrimitiveDeserializer.
   * \details This template uses SFINAE and handles all value objects (=not a reference and not a pointer).
   * \tparam ReturnType Type to be deserialized.
   * \param[in] buffer The buffer holding the data.
   * \param[out] buffer_pos Buffer position is updated to position after the deserialized value.
   * \return deserialized value.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <
      typename ReturnType,
      typename std::enable_if<((!std::is_reference<ReturnType>::value) && (!std::is_pointer<ReturnType>::value)) &&
                                  ((!::crypto::common::IsResult<ReturnType>::value) ||
                                   (std::is_void<typename ::crypto::common::RemoveResult<ReturnType>::type>::value)) &&
                                  (!::crypto::common::IsOptional<ReturnType>::value) &&
                                  (!::crypto::common::IsStringView<ReturnType>::value),
                              std::int32_t>::type = 0>
  ReturnType DeserializeGenericValue(ara::crypto::ReadOnlyMemRegion buffer,
                                     MessageObjectLifetimeManager const& /*object_lifetime_manager*/,
                                     BufferPosition& buffer_pos) const noexcept {
    // VECTOR NL AutosarC++17_10-M8.5.2: MD_CRYPTO_AutosarC++17_10-M8.5.2_template_type
    typename std::remove_const<ReturnType>::type out_value{};

    static_cast<void>(primitive_deserializer_.Deserialize(buffer, buffer_pos, out_value));

    return out_value;
  }

  /*!
   * \brief Template function for deserializing generic value. This function is not intended to do anything but it is
   * used as proxy that forwards all calls to the correct function in PrimitiveDeserializer.
   * \details This template uses SFINAE and handles all references to objects.
   * \tparam ReturnType Type to be deserialized.
   * \param[in] buffer The buffer holding the data.
   * \param[in] object_lifetime_manager lifetime manager for heap allocated objects.
   * \param[out] buffer_pos Buffer position is updated to position after the deserialized value.
   * \return deserialized value.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename ReturnType, typename std::enable_if<std::is_reference<ReturnType>::value, std::int32_t>::type = 0>
  ReturnType DeserializeGenericValue(ara::crypto::ReadOnlyMemRegion buffer,
                                     MessageObjectLifetimeManager& object_lifetime_manager,
                                     BufferPosition& buffer_pos) const noexcept {
    // VECTOR NL AutosarC++17_10-A2.8.3: MD_CRYPTO_AutosarC++17_10-A2.8.3_false_positive
    using PureTypeName = typename std::remove_const<typename std::remove_reference<ReturnType>::type>::type;
    // VECTOR NL AutosarC++17_10-A18.5.8: MD_CRYPTO_AutosarC++17_10-A18.5.8_alloc_on_stack
    std::unique_ptr<PureTypeName> out_value_uptr{std::make_unique<PureTypeName>()};
    PureTypeName* const out_value_ptr{out_value_uptr.get()};
    static_cast<void>(primitive_deserializer_.Deserialize(buffer, buffer_pos, *out_value_ptr));
    object_lifetime_manager.RegisterObjectForRelease(std::move(out_value_uptr));
    return *out_value_ptr;
  }

  /*!
   * \brief Template function for deserializing generic value.
   * This template is used for pointers to identifiables on the client side.
   * Because the pointer is passed by value and the proxy object does never change, we don't need to parse the returned.
   * \details This template uses SFINAE and handles all pointers to objects.
   * \tparam ReturnType Type to be deserialized.
   * \param[in] buffer The buffer holding the data.
   * \param[in] object_lifetime_manager lifetime manager for heap allocated objects.
   * \param[out] buffer_pos Buffer position is updated to position after the deserialized value.
   * \return deserialized value.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \internal
   * - Deserialize leading tag
   * - Depending on the tag:
   *   - Advance buffer by length of the pointer
   *   - Don't further advance buffer for nullptrs
   * \endinternal
   */
  template <typename ReturnType, typename std::enable_if<(std::is_pointer<ReturnType>::value) &&
                                                             (::crypto::common::is_identifiable<ReturnType>::value),
                                                         std::int32_t>::type = 0>
  // VECTOR NC AutosarC++17_10-A8.4.4: MD_CRYPTO_AutosarC++17_10-A8.4.4_in_out_param
  ReturnType DeserializeGenericValue(ara::crypto::ReadOnlyMemRegion buffer,
                                     MessageObjectLifetimeManager const& object_lifetime_manager,
                                     BufferPosition& buffer_pos) const noexcept {
    static_cast<void>(object_lifetime_manager);
    SerializationTags tag{};
    static_cast<void>(primitive_deserializer_.Deserialize(buffer, buffer_pos, tag));
    ReturnType const ret{nullptr};
    if (tag != SerializationTags::tag_nullptr) {
      buffer_pos += sizeof(ProxyId);
    }
    return ret;
  }

  /*!
   * \brief Template function for deserializing generic value. This function is not intended to do anything but it is
   * used as proxy that forwards all calls to the correct function in PrimitiveDeserializer.
   * This template is used on the client side for pointers to non-identifiable objects.
   * \details This template uses SFINAE and handles all pointers to objects.
   * \tparam ReturnType Type to be deserialized.
   * \param[in] buffer The buffer holding the data.
   * \param[in] object_lifetime_manager lifetime manager for heap allocated objects.
   * \param[out] buffer_pos Buffer position is updated to position after the deserialized value.
   * \return deserialized value.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \internal
   *  - Deserialize leading tag (1 byte)
   *  - Check if tag indicates a nullptr
   *  - If yes: return nulltptr
   *  - If not: deserialize the object and return it
   * \endinternal
   */
  template <typename ReturnType, typename std::enable_if<(std::is_pointer<ReturnType>::value) &&
                                                             (!::crypto::common::is_identifiable<ReturnType>::value),
                                                         std::int32_t>::type = 0>
  ReturnType DeserializeGenericValue(ara::crypto::ReadOnlyMemRegion buffer,
                                     MessageObjectLifetimeManager& object_lifetime_manager,
                                     BufferPosition& buffer_pos) const noexcept {
    // VECTOR NL AutosarC++17_10-A2.8.3: MD_CRYPTO_AutosarC++17_10-A2.8.3_false_positive
    using TypeName = typename std::remove_pointer<ReturnType>::type;
    SerializationTags tag{};
    static_cast<void>(primitive_deserializer_.Deserialize(buffer, buffer_pos, tag));
    ReturnType ret{nullptr};
    if (tag != SerializationTags::tag_nullptr) {
      // VECTOR NL AutosarC++17_10-A18.5.8: MD_CRYPTO_AutosarC++17_10-A18.5.8_alloc_on_stack
      std::unique_ptr<TypeName> out_value_uptr{std::make_unique<TypeName>()};
      ReturnType out_value_ptr{out_value_uptr.get()};
      static_cast<void>(primitive_deserializer_.Deserialize(buffer, buffer_pos, *out_value_ptr));
      object_lifetime_manager.RegisterObjectForRelease(std::move(out_value_uptr));
      ret = std::move(out_value_ptr);
    }
    return ret;
  }

 private:
  PrimitiveDeserializer primitive_deserializer_; /*!< Deserializer for data primitives */
};

}  // namespace ipc
}  // namespace sec
}  // namespace amsr

#endif  // LIB_LIBCOMMON_INCLUDE_AMSR_SEC_IPC_MESSAGE_DESERIALIZER_H_
