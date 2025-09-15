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
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  server_message_deserializer.h
 *        \brief  Deserializer for server side messages and objects from ObjectProvider.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_SERVER_MESSAGE_DESERIALIZER_H_
#define LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_SERVER_MESSAGE_DESERIALIZER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>
#include <vector>

#include "amsr/core/string.h"
#include "amsr/sec/ipc/message_deserializer.h"
#include "crypto/common/type_traits.h"
#include "crypto/server/object_provider.h"

namespace crypto {
namespace server {
namespace detail {

/*!
 * \brief Implementation for ForEach.
 * \tparam Tuple Type of the tuple to operate on.
 * \tparam F Functor to apply to each element.
 * \tparam I Index sequence type
 * \param[in] tuple Tuple containing elements to apply the functor on.
 * \param[in] functor Functor to apply to each element.
 * \return void
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Component Private
 */
template <class Tuple, class F, std::size_t... I>
constexpr F ForEachImpl(Tuple&& tuple, F&& functor, std::index_sequence<I...>) noexcept {
  // VECTOR Disable AutosarC++17_10-M5.18.1: MD_CRYPTO_AutosarC++17_10-M5.18.1_comma_operator
  static_cast<void>(std::initializer_list<std::int32_t>{
      (std::forward<F>(functor)(std::get<I>(std::forward<Tuple>(tuple))), static_cast<std::int32_t>(0))...});
  // VECTOR Enable AutosarC++17_10-M5.18.1
  return functor;
}

/*!
 * \brief Implementation for GetValues.
 * \tparam ArgumentTypes Types of the Values contained in the amsr::core::Results
 * \tparam Tuple Type of the tuple containing the Results
 * \tparam I Index sequence type
 * \param[in] tuple Tuple which contains amsr::core::Results to extract the values from.
 * \return void
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Component Private
 */
// VECTOR NC AutosarC++17_10-A13.3.1: MD_CRYPTO_AutosarC++17_10-A13.3.1_overloading_templates_w_forwarding_reference
template <typename... ArgumentTypes, typename Tuple, std::size_t... I>
static auto GetValuesImpl(Tuple&& tuple, std::index_sequence<I...>) noexcept -> std::tuple<ArgumentTypes...> {
  return std::forward_as_tuple(std::get<I>(tuple).Value()...);
}

/*!
 * \brief Executes functor on each element of given tuple.
 * \tparam Tuple Type of the tuple to operate on.
 * \tparam F Functor to apply to each element.
 * \param[in] tuple Tuple containing elements to apply the functor on.
 * \param[in] functor Functor to apply to each element.
 * \return void
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Component Private
 */
template <typename Tuple, typename F>
constexpr F ForEach(Tuple&& tuple, F&& functor) noexcept {
  return ForEachImpl(std::forward<Tuple>(tuple), std::forward<F>(functor),
                     std::make_index_sequence<std::tuple_size<std::remove_reference_t<Tuple>>::value>{});
}

/*!
 * \brief Returns the Values of a tuple of amsr::core::Results.
 * \tparam ArgumentTypes Types of the Values contained in the amsr::core::Results.
 * \tparam Tuple Type of the tuple containing the Results.
 * \param[in] tuple Tuple which contains amsr::core::Results to extract the values from.
 * \return Tuple containing the extracted values of the results in \c tuple.
 * \context ANY
 * \pre -
 * \threadsafe TRUE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Component Private
 */
// VECTOR NC AutosarC++17_10-A13.3.1: MD_CRYPTO_AutosarC++17_10-A13.3.1_overloading_templates_w_forwarding_reference
template <typename... ArgumentTypes, typename Tuple>
static auto GetValues(Tuple&& tuple) noexcept -> std::tuple<ArgumentTypes...> {
  using Indices = std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>;
  return GetValuesImpl<ArgumentTypes...>(std::forward<Tuple>(tuple), Indices{});
}

}  // namespace detail

/*!
 * \brief Deserializer for server side messages
 * \vprivate Component Private
 * \trace DSGN-Crypto-Broker-Pattern
 */
class ServerMessageDeserializer {
 public:
  /*!
   * \brief Constructor
   * \param[in] primitive_deserializer Deserializer for primitives
   * \param[in] object_provider Object provider for object deserialization by object id
   * \vprivate Component Private
   */
  ServerMessageDeserializer(amsr::sec::ipc::PrimitiveDeserializer primitive_deserializer,
                            ObjectProvider& object_provider) noexcept
      : primitive_deserializer_{std::move(primitive_deserializer)}, object_provider_{object_provider} {}

  /*!
   * \brief Constructor
   * \param[in] object_provider
   * \vprivate Component Private
   */
  explicit ServerMessageDeserializer(ObjectProvider& object_provider) noexcept
      : ServerMessageDeserializer(amsr::sec::ipc::PrimitiveDeserializer{}, object_provider) {}

  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  ServerMessageDeserializer(ServerMessageDeserializer const& /*other*/) = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  ServerMessageDeserializer(ServerMessageDeserializer&& /*other*/) noexcept = default;

  /*!
   * \brief Deleted copy assignment operator
   * \return -
   * \vprivate Component Private
   */
  ServerMessageDeserializer& operator=(ServerMessageDeserializer const& /*other*/) & = delete;

  /*!
   * \brief Deleted move assignment operator
   * \return -
   * \vprivate Component Private
   */
  ServerMessageDeserializer& operator=(ServerMessageDeserializer&& /*other*/) & noexcept = delete;

  /*!
   * \brief Destroys the object.
   * \vprivate Component Private
   */
  virtual ~ServerMessageDeserializer() noexcept = default;

  /*!
   * \brief Generic deserializer for all message tasks based on TaskId_t. Performs buffer length checks and
   *        calls the primitive deserializer.
   * \param[in] buffer The buffer holding the data
   * \param[out] buffer_pos Buffer position is updated to position after the deserialized value
   * \return Task with type from template argument
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kRpcInsufficientCapacity In case the buffer is too small.
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename TaskEnumClass>
  auto DeserializeTask(ara::crypto::ReadOnlyMemRegion buffer, amsr::sec::ipc::BufferPosition& buffer_pos) const noexcept
      -> amsr::core::Result<TaskEnumClass> {
    static_assert(sizeof(amsr::sec::ipc::TaskId_t) == sizeof(TaskEnumClass),
                  "Size mismatch: Size of Task must be equal to TaskId_t.");
    amsr::sec::ipc::MessageObjectLifetimeManager object_lifetime_manager{};
    return DeserializeGenericValue<amsr::sec::ipc::TaskId_t>(buffer, object_lifetime_manager, buffer_pos)
        .AndThen([](amsr::sec::ipc::TaskId_t t_id) {
          // VECTOR NL AutosarC++17_10-A7.2.1: MD_CRYPTO_AutosarC++17_10-A7.2.1
          return amsr::core::Result<TaskEnumClass>::FromValue(static_cast<TaskEnumClass>(t_id));
        });
  }

  /*!
   * Template based deserialization function.
   * \param[in] buffer Buffer containing (at least) the data to be deserialized
   * \param[in] object_lifetime_manager Buffer containing (at least) the data to be deserialized
   * \return deserialized message
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kRpcInsufficientCapacity In case the buffer is too small.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename TaskEnumClass, typename... ArgumentTypes>
  auto DeserializeMessage(ara::crypto::ReadOnlyMemRegion buffer,
                          amsr::sec::ipc::MessageObjectLifetimeManager& object_lifetime_manager) const noexcept
      -> amsr::core::Result<amsr::sec::ipc::BasicMessage<TaskEnumClass, ArgumentTypes...>> {
    // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
    auto result = amsr::core::Result<amsr::sec::ipc::BasicMessage<TaskEnumClass, ArgumentTypes...>>::FromError(
        ara::crypto::SecurityErrc::kRpcRuntimeFault);

    amsr::sec::ipc::BufferPosition buffer_pos{};
    amsr::sec::ipc::Task basic_task{};
    TaskEnumClass detail_task{};
    amsr::sec::ipc::ProxyId tmp_oid{};
    amsr::core::Span<amsr::sec::ipc::ProxyId const> tmp_noid{};

    // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
    auto const deserialize_result =
        DeserializeTask<amsr::sec::ipc::Task>(buffer, buffer_pos)
            .AndThen([this, &basic_task, &buffer, &buffer_pos](amsr::sec::ipc::Task deserialized) {
              basic_task = deserialized;
              return DeserializeTask<TaskEnumClass>(buffer, buffer_pos);
            })
            .AndThen([this, &detail_task, &buffer, &object_lifetime_manager, &buffer_pos](TaskEnumClass deserialized) {
              detail_task = deserialized;
              return DeserializeGenericValue<amsr::sec::ipc::ProxyId>(buffer, object_lifetime_manager, buffer_pos);
            })
            .AndThen(
                [this, &tmp_oid, &buffer, &object_lifetime_manager, &buffer_pos](amsr::sec::ipc::ProxyId deserialized) {
                  tmp_oid = deserialized;
                  return DeserializeGenericValue<amsr::core::Span<amsr::sec::ipc::ProxyId const>>(
                      buffer, object_lifetime_manager, buffer_pos);
                })
            .Inspect(
                [&tmp_noid](amsr::core::Span<amsr::sec::ipc::ProxyId const> deserialized) { tmp_noid = deserialized; });

    if (deserialize_result.HasValue()) {
      // VECTOR NC VectorC++-V5.0.1: MD_CRYPTO_VectorC++-V5.0.1
      // VECTOR NC AutosarC++17_10-A5.0.1: MD_CRYPTO_AutosarC++17_10-A5.0.1
      std::tuple<amsr::core::Result<ArgumentTypes>...> deserialize_results{
          DeserializeGenericValue<ArgumentTypes>(buffer, object_lifetime_manager, buffer_pos)...};

      // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
      auto merged_result = amsr::core::Result<void>::FromValue();
      static_cast<void>(detail::ForEach(deserialize_results, [&merged_result](auto&& deser_result) {
        merged_result = merged_result.And(deser_result).Drop();
      }));

      if (merged_result.HasValue()) {
        result.EmplaceValue(std::move(CreateMessage<TaskEnumClass, ArgumentTypes...>(
            basic_task, detail_task, tmp_oid, tmp_noid,
            std::forward<std::tuple<amsr::core::Result<ArgumentTypes>...>>(deserialize_results))));
      } else {
        result.EmplaceError(merged_result.Error());
      }
    } else {
      result.EmplaceError(deserialize_result.Error());
    }

    return result;
  }

 protected:
  /*!
   * \brief Create message implementation.
   * \tparam TaskEnumClass Task enumeration type.
   * \tparam ArgumentTypes Argument types parameter pack.
   * \tparam Tuple Tuple type.
   * \tparam I Index sequence used for unrolling arguments.
   * \param[in] task Task of the message.
   * \param[in] detail_task Detail task of the message.
   * \param[in] object_id ProxyId of the calling object.
   * \param[in] new_object_ids Vector of of ObjectIds containing new objects ids of the result object.
   * \param[in] tuple Argument values to be transferred.
   * \return The created message.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename TaskEnumClass, typename... ArgumentTypes, typename Tuple, std::size_t... I>
  static auto CreateMessageImpl(amsr::sec::ipc::Task task, TaskEnumClass detail_task, amsr::sec::ipc::ProxyId object_id,
                                amsr::core::Span<amsr::sec::ipc::ProxyId const> new_object_ids, Tuple&& tuple,
                                std::index_sequence<I...>) noexcept
      -> amsr::sec::ipc::BasicMessage<TaskEnumClass, ArgumentTypes...> {
    // This is necessary for template instantiations without arguments (otherwise \c deserialized_values is an unused
    // variable).
    std::tuple<ArgumentTypes...> deserialized_values{detail::GetValues<ArgumentTypes...>(tuple)};
    // VECTOR NC AutosarC++17_10-M8.5.1: MD_CRYPTO_AutosarC++17_10-M8.5.1_false_positive
    static_cast<void>(deserialized_values);
    // VECTOR NC VectorC++-V5.0.1: MD_CRYPTO_VectorC++-V5.0.1
    // VECTOR NC AutosarC++17_10-A5.0.1: MD_CRYPTO_AutosarC++17_10-A5.0.1
    return amsr::sec::ipc::BasicMessage<TaskEnumClass, ArgumentTypes...>{task, detail_task, object_id, new_object_ids,
                                                                         std::get<I>(deserialized_values)...};
  }

  /*!
   * \brief Creates a BasicMessage from given parameters containing the values inside the given tuple.
   * \tparam TaskEnumClass Task enumeration type.
   * \tparam ArgumentTypes Argument types parameter pack.
   * \tparam Tuple Tuple containing arguments.
   * \param[in] task Task of the message.
   * \param[in] detail_task Detail task of the message.
   * \param[in] object_id ProxyId of the calling object.
   * \param[in] new_object_ids Vector of of ObjectIds containing new objects ids of the result object.
   * \param[in] tuple Argument values to be transferred.
   * \return The created message.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename TaskEnumClass, typename... ArgumentTypes, typename Tuple>
  static auto CreateMessage(amsr::sec::ipc::Task task, TaskEnumClass detail_task, amsr::sec::ipc::ProxyId object_id,
                            amsr::core::Span<amsr::sec::ipc::ProxyId const> new_object_ids, Tuple&& tuple) noexcept
      -> amsr::sec::ipc::BasicMessage<TaskEnumClass, ArgumentTypes...> {
    using Indices = std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>;
    return CreateMessageImpl<TaskEnumClass, ArgumentTypes...>(task, detail_task, object_id, new_object_ids,
                                                              std::forward<Tuple>(tuple), Indices{});
  }

  /*!
   * \brief Template function for deserializing generic value by calling the appropriate PrimitiveDeserializer
   *        This function uses SFINAE to determine the correct deserialization method. It applies to
   *        \c amsr::core::StringViews.
   * \tparam ReturnType Type to be deserialized
   * \param[in] buffer The buffer holding the data
   * \param[in,out] object_lifetime_manager The lifetime manager
   * \param[out] buffer_pos Buffer position is updated to position after the deserialized value
   * \return deserialized value
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kRpcInsufficientCapacity In case the buffer is too small.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename ReturnType,
            typename std::enable_if<common::IsStringView<ReturnType>::value, std::int32_t>::type = 0>
  auto DeserializeGenericValue(ara::crypto::ReadOnlyMemRegion buffer,
                               amsr::sec::ipc::MessageObjectLifetimeManager& object_lifetime_manager,
                               amsr::sec::ipc::BufferPosition& buffer_pos) const noexcept
      -> amsr::core::Result<ReturnType> {
    // VECTOR NL AutosarC++17_10-A18.5.8: MD_CRYPTO_AutosarC++17_10-A18.5.8_alloc_on_stack
    std::unique_ptr<amsr::core::String> str{std::make_unique<amsr::core::String>()};
    return primitive_deserializer_.Deserialize(buffer, buffer_pos, *str).AndThen([&str, &object_lifetime_manager]() {
      ReturnType const str_view{&(*str)[0], str->length()};
      object_lifetime_manager.RegisterObjectForRelease(std::move(str));
      return amsr::core::Result<ReturnType>::FromValue(str_view);
    });
  }

  /*!
   * \brief Template function for deserializing generic value by calling the appropriate PrimitiveDeserializer
   *        This function uses SFINAE to determine the correct deserialization method. It applies to all value objects
   *        which are not assignable (= derived from) Identifiable
   * \tparam ReturnType Type to be deserialized
   * \param[in] buffer The buffer holding the data
   * \param[out] buffer_pos Buffer position is updated to position after the deserialized value
   * \return deserialized value
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kRpcInsufficientCapacity In case the buffer is too small.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <
      typename ReturnType,
      typename std::enable_if<(!std::is_reference<ReturnType>::value) && (!std::is_pointer<ReturnType>::value) &&
                                  (!std::is_base_of<common::Identifiable, common::raw_class_name<ReturnType>>::value) &&
                                  (!common::IsStringView<ReturnType>::value),
                              std::int32_t>::type = 0>
  auto DeserializeGenericValue(ara::crypto::ReadOnlyMemRegion buffer,
                               amsr::sec::ipc::MessageObjectLifetimeManager& /*object_lifetime_manager*/,
                               amsr::sec::ipc::BufferPosition& buffer_pos) const noexcept
      -> amsr::core::Result<ReturnType> {
    ReturnType out_value{};
    return primitive_deserializer_.Deserialize(buffer, buffer_pos, out_value).AndThen([&out_value] {
      return amsr::core::Result<ReturnType>::FromValue(out_value);
    });
  }

  /*!
   * \brief Template function for deserializing generic value references. It will deserialize the object and register
   *        it to the MessageObjectLifetimeManager who will free it when the message is freed.
   *        This function uses SFINAE to determine the correct deserialization method. It applies to all references to
   *        objects which are not assignable (= derived from) Identifiable
   * \tparam ReturnType Type to be deserialized
   * \param[in] buffer The buffer holding the data
   * \param[in] object_lifetime_manager lifetime manager for heap allocated objects
   * \param[out] buffer_pos Buffer position is updated to position after the deserialized value
   * \return deserialized value
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kRpcInsufficientCapacity In case the buffer is too small.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename ReturnType,
            typename std::enable_if<
                (std::is_reference<ReturnType>::value) &&
                    (!std::is_base_of<common::Identifiable, common::raw_class_name<ReturnType>>::value) &&
                    ((!common::IsVector<
                         typename std::remove_const<typename std::remove_reference<ReturnType>::type>::type>::value) ||
                     std::is_assignable<
                         ara::crypto::cryp::CryptoProvider::ContainedObjectsList&,
                         typename std::remove_const<typename std::remove_reference<ReturnType>::type>::type>::value) &&
                    (!common::is_identifiable_uptr_rvalue_reference<ReturnType>::value),
                std::int32_t>::type = 0>
  auto DeserializeGenericValue(ara::crypto::ReadOnlyMemRegion buffer,
                               amsr::sec::ipc::MessageObjectLifetimeManager& object_lifetime_manager,
                               amsr::sec::ipc::BufferPosition& buffer_pos) const noexcept
      -> amsr::core::Result<ReturnType> {
    // VECTOR NL AutosarC++17_10-A2.8.3: MD_CRYPTO_AutosarC++17_10-A2.8.3_false_positive
    using TypeName = typename std::remove_const<typename std::remove_reference<ReturnType>::type>::type;
    // VECTOR NL AutosarC++17_10-A18.5.8: MD_CRYPTO_AutosarC++17_10-A18.5.8_alloc_on_stack
    std::unique_ptr<TypeName> out_value_uptr{std::make_unique<TypeName>()};
    TypeName* const out_value_ptr{out_value_uptr.get()};
    return primitive_deserializer_.Deserialize(buffer, buffer_pos, *out_value_ptr)
        .AndThen([&out_value_ptr, &out_value_uptr, &object_lifetime_manager]() {
          object_lifetime_manager.RegisterObjectForRelease(std::move(out_value_uptr));
          return amsr::core::Result<ReturnType>::FromValue(*out_value_ptr);
        });
  }

  /*!
   * \brief Template function for deserializing generic value pointers. It will deserialize the object and register
   *        it to the MessageObjectLifetimeManager who will free it when the message is freed.
   *        This function uses SFINAE to determine the correct deserialization method. It applies to all pointers to
   *        objects which are not assignable (= derived from) Identifiable
   * \tparam ReturnType Type to be deserialized
   * \param[in] buffer The buffer holding the data
   * \param[in] object_lifetime_manager lifetime manager for heap allocated objects
   * \param[out] buffer_pos Buffer position is updated to position after the deserialized value
   * \return deserialized value
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kRpcInsufficientCapacity In case the buffer is too small.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <
      typename ReturnType,
      typename std::enable_if<std::is_pointer<ReturnType>::value &&
                                  (!std::is_base_of<common::Identifiable, common::raw_class_name<ReturnType>>::value),
                              std::int32_t>::type = 0>
  auto DeserializeGenericValue(ara::crypto::ReadOnlyMemRegion buffer,
                               amsr::sec::ipc::MessageObjectLifetimeManager& object_lifetime_manager,
                               amsr::sec::ipc::BufferPosition& buffer_pos) const noexcept
      -> amsr::core::Result<ReturnType> {
    amsr::sec::ipc::SerializationTags tag{};
    return primitive_deserializer_.Deserialize(buffer, buffer_pos, tag)
        .AndThen([this, &buffer, &object_lifetime_manager, &buffer_pos, &tag]() {
          // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
          auto inner_result = amsr::core::Result<void>::FromValue();
          ReturnType ret{nullptr};
          if (tag != amsr::sec::ipc::SerializationTags::tag_nullptr) {
            // VECTOR NL AutosarC++17_10-A2.8.3: MD_CRYPTO_AutosarC++17_10-A2.8.3_false_positive
            using PureTypeName = typename std::remove_pointer<ReturnType>::type;
            // VECTOR NL AutosarC++17_10-A18.5.8: MD_CRYPTO_AutosarC++17_10-A18.5.8_alloc_on_stack
            std::unique_ptr<PureTypeName> out_value_uptr{std::make_unique<PureTypeName>()};
            ReturnType out_value_ptr{out_value_uptr.get()};
            inner_result = primitive_deserializer_.Deserialize(buffer, buffer_pos, *out_value_ptr);
            object_lifetime_manager.RegisterObjectForRelease(std::move(out_value_uptr));
            ret = std::move(out_value_ptr);
          }
          return (inner_result.HasValue()) ? amsr::core::Result<ReturnType>::FromValue(ret)
                                           : amsr::core::Result<ReturnType>::FromError(inner_result.Error());
        });
  }

  /*!
   * \brief Template function for deserializing identifiable pointers. It will request the object from the
   *        ObjectProvider.
   *        This function uses SFINAE to determine the correct deserialization method. It applies to all references to
   *        objects which are assignable (= derived from) Identifiable
   * \tparam ReturnType Type to be deserialized
   * \param[in] buffer The buffer holding the data
   * \param[out] buffer_pos Buffer position is updated to position after the deserialized value
   * \return deserialized value
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kRpcInsufficientCapacity In case the buffer is too small.
   * \error ara::crypto::SecurityErrc::kRpcRuntimeFault If the given object does not match the requested type.
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <
      typename ReturnType,
      typename std::enable_if<(std::is_pointer<ReturnType>::value) &&
                                  (std::is_base_of<common::Identifiable, common::raw_class_name<ReturnType>>::value),
                              std::int32_t>::type = 0>
  auto DeserializeGenericValue(ara::crypto::ReadOnlyMemRegion buffer,
                               amsr::sec::ipc::MessageObjectLifetimeManager const& /*object_lifetime_manager*/,
                               amsr::sec::ipc::BufferPosition& buffer_pos) const noexcept
      -> amsr::core::Result<ReturnType> {
    amsr::sec::ipc::SerializationTags tag{};
    return primitive_deserializer_.Deserialize(buffer, buffer_pos, tag).AndThen([this, &buffer, &buffer_pos, &tag]() {
      // VECTOR NC AutosarC++17_10-A18.5.8: MD_CRYPTO_AutosarC++17_10-A18.5.8_alloc_on_stack
      ReturnType out_ptr{nullptr};
      // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
      auto inner_result = amsr::core::Result<void>::FromValue();
      if (tag != amsr::sec::ipc::SerializationTags::tag_nullptr) {
        amsr::sec::ipc::ProxyId proxy_id{};
        inner_result =
            amsr::sec::ipc::PrimitiveDeserializer::Deserialize(buffer, buffer_pos, proxy_id)
                // VECTOR NL AutosarC++17_10-A5.1.8: MD_CRYPTO_AutosarC++17_10-A5.1.8_nested_lambdas
                .AndThen([this, &proxy_id, &out_ptr]() -> amsr::core::Result<void> {
                  // VECTOR NC AutosarC++17_10-A18.5.8: MD_CRYPTO_AutosarC++17_10-A18.5.8_alloc_on_stack
                  std::shared_ptr<common::raw_class_name<ReturnType>> out_sptr{nullptr};

                  // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
                  auto const obj_result = object_provider_.GetObject(proxy_id, out_sptr);

                  if (obj_result.HasValue()) {
                    out_ptr = out_sptr.get();
                  }

                  return obj_result;
                })
                // VECTOR NL AutosarC++17_10-A5.1.8: MD_CRYPTO_AutosarC++17_10-A5.1.8_nested_lambdas
                .OrElse([this, &proxy_id, &out_ptr](amsr::core::ErrorCode const&) -> amsr::core::Result<void> {
                  // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
                  auto obj_ref_result =
                      object_provider_.GetObjectReference<common::raw_class_name<ReturnType>>(proxy_id);

                  if (obj_ref_result.HasValue()) {
                    out_ptr = &obj_ref_result.Value();
                  }

                  return obj_ref_result.Drop();
                });
      }

      return (inner_result.HasValue()) ? amsr::core::Result<ReturnType>::FromValue(out_ptr)
                                       : amsr::core::Result<ReturnType>::FromError(inner_result.Error());
    });
  }

  /*!
   * \brief Template function for deserializing value shared pointers. It will request the object from the
   *        ObjectProvider.
   *        This function uses SFINAE to determine the correct deserialization method. It applies to all references to
   *        objects which are assignable (= derived from) Identifiable
   * \tparam ReturnType Type to be deserialized
   * \param[in] buffer The buffer holding the data
   * \param[out] buffer_pos Buffer position is updated to position after the deserialized value
   * \return deserialized value
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kRpcInsufficientCapacity In case the buffer is too small.
   * \error ara::crypto::SecurityErrc::kRpcRuntimeFault If the given object does not match the requested type.
   * \error ara::crypto::SecurityErrc::kRpcUnknownObjectIdentifier if no Reference for this proxy_id has been
   * registered.
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <
      typename ReturnType,
      typename std::enable_if<(std::is_reference<ReturnType>::value) &&
                                  (std::is_base_of<common::Identifiable, common::raw_class_name<ReturnType>>::value &&
                                   (!common::is_identifiable_uptr_rvalue_reference<ReturnType>::value)),
                              std::int32_t>::type = 0>
  auto DeserializeGenericValue(ara::crypto::ReadOnlyMemRegion buffer,
                               amsr::sec::ipc::MessageObjectLifetimeManager& /*object_lifetime_manager*/,
                               amsr::sec::ipc::BufferPosition& buffer_pos) const noexcept
      -> amsr::core::Result<ReturnType> {
    // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
    auto result = amsr::core::Result<ReturnType>::FromError(ara::crypto::SecurityErrc::kRpcRuntimeFault);
    amsr::sec::ipc::ProxyId proxy_id{};
    // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
    auto const deserialize_result = amsr::sec::ipc::PrimitiveDeserializer::Deserialize(buffer, buffer_pos, proxy_id);

    if (deserialize_result.HasValue()) {
      // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
      auto object_ref_result = object_provider_.GetObjectReference<ReturnType>(proxy_id);

      if (object_ref_result.HasValue()) {
        result.EmplaceValue(object_ref_result.Value());
      } else {
        // VECTOR NC AutosarC++17_10-A18.5.8: MD_CRYPTO_AutosarC++17_10-A18.5.8_alloc_on_stack
        std::shared_ptr<common::raw_class_name<ReturnType>> out_value_sptr{nullptr};
        // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
        auto const object_result = object_provider_.GetObject(proxy_id, out_value_sptr);
        if ((object_result.HasValue()) && (out_value_sptr != nullptr)) {
          result.EmplaceValue(*out_value_sptr);
        }
      }
    }
    return result;
  }

  /*!
   * \brief Template function for deserializing value shared pointers. It will request the object from the
   *        ObjectProvider.
   *        This function uses SFINAE to determine the correct deserialization method. It applies to all
   *        shared_pointers to objects which are assignable (= derived from) Identifiable
   * \tparam ReturnType Type to be deserialized
   * \param[in] buffer The buffer holding the data
   * \param[out] buffer_pos Buffer position is updated to position after the deserialized value
   * \return deserialized value
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kRpcInsufficientCapacity In case the buffer is too small.
   * \error ara::crypto::SecurityErrc::kRpcRuntimeFault If the given object does not match the requested type.
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <
      typename ReturnType,
      typename std::enable_if<std::is_assignable<std::shared_ptr<common::Identifiable const>, ReturnType>::value &&
                                  (!common::is_identifiable_uptr_rvalue_reference<ReturnType>::value),
                              std::int32_t>::type = 0>
  auto DeserializeGenericValue(ara::crypto::ReadOnlyMemRegion buffer,
                               amsr::sec::ipc::MessageObjectLifetimeManager& /*object_lifetime_manager*/,
                               amsr::sec::ipc::BufferPosition& buffer_pos) const noexcept
      -> amsr::core::Result<ReturnType> {
    amsr::sec::ipc::SerializationTags tag{};
    return primitive_deserializer_.Deserialize(buffer, buffer_pos, tag).AndThen([this, &buffer, &buffer_pos, &tag]() {
      // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
      auto inner_result = amsr::core::Result<void>::FromValue();
      ReturnType ret{nullptr};
      if (tag != amsr::sec::ipc::SerializationTags::tag_nullptr) {
        // VECTOR NC AutosarC++17_10-A18.5.8: MD_CRYPTO_AutosarC++17_10-A18.5.8_alloc_on_stack
        std::shared_ptr<common::raw_class_name<ReturnType>> out_value_sptr{nullptr};
        amsr::sec::ipc::ProxyId proxy_id{};
        inner_result = amsr::sec::ipc::PrimitiveDeserializer::Deserialize(buffer, buffer_pos, proxy_id)
                           // VECTOR NL AutosarC++17_10-A5.1.8: MD_CRYPTO_AutosarC++17_10-A5.1.8_nested_lambdas
                           .AndThen([this, &proxy_id, &out_value_sptr]() {
                             return object_provider_.GetObject(proxy_id, out_value_sptr);
                           })
                           // VECTOR NL AutosarC++17_10-A5.1.8: MD_CRYPTO_AutosarC++17_10-A5.1.8_nested_lambdas
                           .Inspect([&ret, &out_value_sptr]() { ret = out_value_sptr; });
      }
      return (inner_result.HasValue()) ? amsr::core::Result<ReturnType>::FromValue(ret)
                                       : amsr::core::Result<ReturnType>::FromError(inner_result.Error());
    });
  }

  /*!
   * \brief Template function for deserializing vectors of std::unique_ptr<common::Identifiable> which are used
   * as in-parameters. It will request the objects from the ObjectProvider.
   * \tparam ReturnType Type to be deserialized
   * \param[in] buffer The buffer holding the data
   * \param[out] object_lifetime_manager MessageObjectLifetimeManager to deregister object.
   * \param[out] buffer_pos Buffer position is updated to position after the deserialized value
   * \return deserialized value
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kRpcInsufficientCapacity In case the buffer is too small.
   * \error ara::crypto::SecurityErrc::kRpcRuntimeFault If the given object does not match the requested type.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <
      typename ReturnType,
      typename std::enable_if<
          common::IsVector<typename std::remove_const<typename std::remove_reference<ReturnType>::type>::type>::value &&
              (std::is_reference<ReturnType>::value &&
               std::is_const<typename std::remove_reference<ReturnType>::type>::value) &&
              common::is_unique_pointer<typename common::RemoveVector<
                  typename std::remove_const<typename std::remove_reference<ReturnType>::type>::type>::type>::value &&
              std::is_base_of<common::Identifiable,
                              typename common::remove_uptr<typename common::RemoveVector<typename std::remove_const<
                                  typename std::remove_reference<ReturnType>::type>::type>::type>::type>::value,
          std::int32_t>::type = 0>
  auto DeserializeGenericValue(ara::crypto::ReadOnlyMemRegion buffer,
                               amsr::sec::ipc::MessageObjectLifetimeManager& object_lifetime_manager,
                               amsr::sec::ipc::BufferPosition& buffer_pos) const noexcept
      -> amsr::core::Result<ReturnType> {
    std::size_t len{0};

    return amsr::sec::ipc::PrimitiveDeserializer::CheckSize(sizeof(std::uint32_t), buffer, buffer_pos)
        .AndThen([&buffer, &buffer_pos, &len]() {
          return amsr::sec::ipc::PrimitiveDeserializer::Deserialize(buffer, buffer_pos, len);
        })
        .AndThen([this, &buffer, &object_lifetime_manager, &buffer_pos, &len]() {
          // VECTOR NC AutosarC++17_10-A2.8.3: MD_CRYPTO_AutosarC++17_10-A2.8.3_false_positive
          // VECTOR NL AutosarC++17_10-A2.11.2: MD_CRYPTO_AutosarC++17_10-A2.11.2_using_name_not_unique
          using VectorType = typename std::remove_const<typename std::remove_reference<ReturnType>::type>::type;
          // VECTOR NL AutosarC++17_10-A18.5.8: MD_CRYPTO_AutosarC++17_10-A18.5.8_alloc_on_stack
          std::shared_ptr<VectorType> const vector_ptr{std::make_shared<VectorType>()};
          amsr::sec::ipc::SerializationTags tag{};
          amsr::sec::ipc::ProxyId proxy_id{};
          // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
          auto inner_result = amsr::core::Result<void>::FromValue();

          // VECTOR NL AutosarC++17_10-A6.5.1: MD_CRYPTO_AutosarC++17_10-A6.5.1_not_a_container_loop
          for (std::size_t i{0}; i < len; ++i) {
            // VECTOR NC AutosarC++17_10-A2.8.3: MD_CRYPTO_AutosarC++17_10-A2.8.3_false_positive
            // VECTOR NL AutosarC++17_10-A2.11.2: MD_CRYPTO_AutosarC++17_10-A2.11.2_using_name_not_unique
            using ValueType = typename common::remove_uptr<typename common::RemoveVector<
                typename std::remove_const<typename std::remove_reference<ReturnType>::type>::type>::type>::type;
            // VECTOR NC AutosarC++17_10-A18.5.8: MD_CRYPTO_AutosarC++17_10-A18.5.8_alloc_on_stack
            // VECTOR NC AutosarC++17_10-A12.8.3: MD_CRYPTO_AutosarC++17_10-A12.8.3_false_positive
            std::unique_ptr<ValueType, ara::crypto::CustomDeleter> uptr{nullptr};

            inner_result = amsr::sec::ipc::PrimitiveDeserializer::CheckSize(sizeof(amsr::sec::ipc::SerializationTags),
                                                                            buffer, buffer_pos)
                               // VECTOR NL AutosarC++17_10-A5.1.8: MD_CRYPTO_AutosarC++17_10-A5.1.8_nested_lambdas
                               .AndThen([this, &buffer, &buffer_pos, &tag]() {
                                 return primitive_deserializer_.Deserialize(buffer, buffer_pos, tag);
                               });

            if (inner_result.HasValue()) {
              if (tag != amsr::sec::ipc::SerializationTags::tag_nullptr) {
                // VECTOR NL AutosarC++17_10-A18.5.8: MD_CRYPTO_AutosarC++17_10-A18.5.8_alloc_on_stack
                std::shared_ptr<ValueType> sptr{nullptr};
                inner_result =
                    amsr::sec::ipc::PrimitiveDeserializer::CheckSize(sizeof(amsr::sec::ipc::ProxyId), buffer,
                                                                     buffer_pos)
                        // VECTOR NL AutosarC++17_10-A5.1.8: MD_CRYPTO_AutosarC++17_10-A5.1.8_nested_lambdas
                        .AndThen([&buffer, &buffer_pos, &proxy_id]() {
                          return amsr::sec::ipc::PrimitiveDeserializer::Deserialize(buffer, buffer_pos, proxy_id);
                        })
                        // VECTOR NL AutosarC++17_10-A5.1.8: MD_CRYPTO_AutosarC++17_10-A5.1.8_nested_lambdas
                        .AndThen([this, &proxy_id, &sptr]() { return object_provider_.GetObject(proxy_id, sptr); });

                if (inner_result.HasValue()) {
                  uptr = std::move(std::unique_ptr<ValueType, ara::crypto::CustomDeleter>{sptr.get()});
                } else {
                  break;
                }
              }
              // VECTOR NC AutosarC++17_10-A12.8.3: MD_CRYPTO_AutosarC++17_10-A12.8.3_false_positive
              if (uptr != nullptr) {
                vector_ptr->emplace_back(std::move(uptr));
              }
            } else {
              break;
            }
          }

          object_lifetime_manager.RegisterObjectForRelease(vector_ptr);
          return (inner_result.HasValue()) ? amsr::core::Result<ReturnType>::FromValue(*vector_ptr)
                                           : amsr::core::Result<ReturnType>::FromError(inner_result.Error());
        });
  }

  /*!
   * \brief Template function for deserializing vectors of std::unique_ptr<common::Identifiable> which are used
   * as out-parameters. It will request the objects from the ObjectProvider.
   * \tparam ReturnType Type to be deserialized
   * \param[in] buffer The buffer holding the data
   * \param[out] object_lifetime_manager MessageObjectLifetimeManager to deregister object.
   * \param[out] buffer_pos Buffer position is updated to position after the deserialized value
   * \return deserialized value
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kRpcInsufficientCapacity In case the buffer is too small.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename ReturnType,
            typename std::enable_if<
                common::IsVector<typename std::remove_reference<ReturnType>::type>::value &&
                    (std::is_reference<ReturnType>::value && (!std::is_const<ReturnType>::value)) &&
                    common::is_unique_pointer<
                        typename common::RemoveVector<typename std::remove_reference<ReturnType>::type>::type>::value &&
                    std::is_base_of<common::Identifiable,
                                    typename common::remove_uptr<typename common::RemoveVector<
                                        typename std::remove_reference<ReturnType>::type>::type>::type>::value,
                std::int32_t>::type = 0>
  static auto DeserializeGenericValue(ara::crypto::ReadOnlyMemRegion buffer,
                                      amsr::sec::ipc::MessageObjectLifetimeManager& object_lifetime_manager,
                                      amsr::sec::ipc::BufferPosition& buffer_pos) noexcept
      -> amsr::core::Result<ReturnType> {
    // VECTOR NC AutosarC++17_10-A2.8.3: MD_CRYPTO_AutosarC++17_10-A2.8.3_false_positive
    // VECTOR NL AutosarC++17_10-A2.11.2: MD_CRYPTO_AutosarC++17_10-A2.11.2_using_name_not_unique
    using VectorType = typename std::remove_reference<ReturnType>::type;
    // VECTOR NL AutosarC++17_10-A18.5.8: MD_CRYPTO_AutosarC++17_10-A18.5.8_alloc_on_stack
    std::shared_ptr<VectorType> result{std::make_shared<VectorType>()};
    std::size_t len{0};

    return amsr::sec::ipc::PrimitiveDeserializer::Deserialize(buffer, buffer_pos, len)
        .AndThen([&object_lifetime_manager, &result]() {
          object_lifetime_manager.RegisterObjectForRelease(result);
          return amsr::core::Result<ReturnType>::FromValue(*result);
        });
  }

  /*!
   * \brief Template function for deserializing vectors of std::unique_ptr<common::Identifiable> which are used
   * as out-parameters. It will request the objects from the ObjectProvider.
   * \tparam ReturnType Type to be deserialized
   * \param[in] buffer The buffer holding the data
   * \param[out] object_lifetime_manager MessageObjectLifetimeManager to deregister object.
   * \param[out] buffer_pos Buffer position is updated to position after the deserialized value
   * \return deserialized value
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kRpcInsufficientCapacity In case the buffer is too small.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <
      typename ReturnType,
      typename std::enable_if<
          common::IsVector<typename std::remove_const<typename std::remove_reference<ReturnType>::type>::type>::value &&
              std::is_assignable<
                  ara::core::Vector<ara::crypto::ReadOnlyMemRegion>&,
                  typename std::remove_const<typename std::remove_reference<ReturnType>::type>::type>::value,
          std::int32_t>::type = 0>
  static auto DeserializeGenericValue(ara::crypto::ReadOnlyMemRegion buffer,
                                      amsr::sec::ipc::MessageObjectLifetimeManager& object_lifetime_manager,
                                      amsr::sec::ipc::BufferPosition& buffer_pos) noexcept
      -> amsr::core::Result<ReturnType> {
    std::size_t len{0};
    return amsr::sec::ipc::PrimitiveDeserializer::CheckSize(sizeof(std::size_t), buffer, buffer_pos)
        .AndThen([&buffer, &buffer_pos, &len]() {
          return amsr::sec::ipc::PrimitiveDeserializer::Deserialize(buffer, buffer_pos, len);
        })
        .AndThen([&buffer, &buffer_pos, &len]() {
          // VECTOR NC AutosarC++17_10-A2.8.3: MD_CRYPTO_AutosarC++17_10-A2.8.3_false_positive
          // VECTOR NL AutosarC++17_10-A2.11.2: MD_CRYPTO_AutosarC++17_10-A2.11.2_using_name_not_unique
          using VectorType = typename std::remove_const<typename std::remove_reference<ReturnType>::type>::type;
          // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
          // VECTOR NL AutosarC++17_10-A18.5.8: MD_CRYPTO_AutosarC++17_10-A18.5.8_alloc_on_stack
          auto inner_result =
              amsr::core::Result<std::shared_ptr<VectorType>>::FromValue(std::make_shared<VectorType>());
          // VECTOR NL AutosarC++17_10-A6.5.1: MD_CRYPTO_AutosarC++17_10-A6.5.1_not_a_container_loop
          for (std::size_t i{0}; i < len; ++i) {
            std::size_t mem_len{0};
            // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
            auto deser_result =
                amsr::sec::ipc::PrimitiveDeserializer::Deserialize(buffer, buffer_pos, mem_len)
                    // VECTOR NL AutosarC++17_10-A5.1.8: MD_CRYPTO_AutosarC++17_10-A5.1.8_nested_lambdas
                    .AndThen([&mem_len, &buffer, &buffer_pos]() {
                      return amsr::sec::ipc::PrimitiveDeserializer::CheckSize(mem_len, buffer, buffer_pos);
                    });
            if (deser_result.HasValue()) {
              inner_result.Value()->emplace_back(ara::crypto::ReadOnlyMemRegion{buffer.subspan(buffer_pos, mem_len)});
            } else {
              deser_result.EmplaceError(deser_result.Error());
              break;
            }
          }
          return inner_result;
        })
        .AndThen([&object_lifetime_manager](auto&& result) {
          object_lifetime_manager.RegisterObjectForRelease(result);
          return amsr::core::Result<ReturnType>::FromValue(*result);
        });
  }

 private:
  amsr::sec::ipc::PrimitiveDeserializer primitive_deserializer_; /*!< Deserializer for primitive data types. */
  crypto::server::ObjectProvider& object_provider_;              /*!< Storage for deserialized objects. */
};

}  // namespace server
}  // namespace crypto

#endif  // LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_SERVER_MESSAGE_DESERIALIZER_H_
