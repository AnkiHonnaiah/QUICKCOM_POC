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
/*!        \file  base_skeleton_impl.h
 *        \brief  Base implementation for server side skeleton objects.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_BASE_SKELETON_IMPL_H_
#define LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_BASE_SKELETON_IMPL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include <memory>
#include <tuple>
#include <utility>

#include "amsr/core/string.h"
#include "amsr/sec/ipc/basic_message.h"
#include "amsr/sec/ipc/message_builder.h"
#include "amsr/sec/ipc/message_serializer.h"
#include "amsr/sec/ipc/protocol.h"
#include "amsr/sec/ipc/transporter.h"
#include "ara/core/vector.h"
#include "ara/crypto/common/mem_region.h"
#include "crypto/common/type_traits.h"
#include "crypto/common/util/contract.h"
#include "crypto/common/util/logger.h"
#include "crypto/server/base_skeleton.h"
#include "crypto/server/server_message_deserializer.h"
#include "crypto/server/skeleton/skeleton_builder.h"

namespace crypto {
namespace server {
/*!
 * Base class for server side skeleton objects
 * \tparam ImplementationType   Type of the Real implementation that shall receive the forwarded calls
 * \tparam TaskEnumType         Type of the Enum class that is used for method dispatching
 * \vprivate Component Private
 * \trace DSGN-Crypto-ProcessIsolation, DSGN-Crypto-Skeleton_Proxy
 */
template <typename ImplementationType, typename TaskEnumType>
class BaseSkeletonImpl : public BaseSkeleton {
 public:
  /*! Type alias for commonly used result */
  using ReadOnlyMemResult = amsr::core::Result<ara::crypto::ReadOnlyMemRegion>;

  /*!
   * \brief Creates a new BaseSkeletonImpl
   * \param[in] implementation  The object to forward the method invocations to
   * \param[in] object_registration Registration for newly created objects
   * \param[in] skeleton_registration Registration and provider for BaseSkeleton objects
   * \param[in] transporter     Transporter interface for client/server communication
   * \param[in] deserializer    Deserializer for deserializing the received data into messages
   * \param[in] serializer      Serializer for serializing the message containing return value
   * \param[in] message_builder Builder for messages containing the return value
   * \param[in] skeleton_builder Builder for creating skeletons required if new objects are created as result of
   * method invocation
   * \param[in] implementation_registered If the implementation object has been registered to the
   * object_registration This should generally be true except for CrpytoProvider KeyStorageProvider and X509Provider
   * objects
   * \vprivate Component Private
   */
  BaseSkeletonImpl(ImplementationType& implementation, crypto::server::ObjectRegistration& object_registration,
                   crypto::server::BaseSkeletonRegistration& skeleton_registration,
                   amsr::sec::ipc::Transporter& transporter, crypto::server::ServerMessageDeserializer& deserializer,
                   amsr::sec::ipc::MessageSerializer& serializer, amsr::sec::ipc::MessageBuilder& message_builder,
                   crypto::server::skeleton::SkeletonBuilder& skeleton_builder,
                   bool implementation_registered = true) noexcept
      : BaseSkeleton(),
        implementation_{implementation},
        implementation_registered_{implementation_registered},
        logger_{common::util::kServerLoggingContext, ""_sv},
        object_registration_{object_registration},
        base_skeleton_registration_{skeleton_registration},
        transporter_{transporter},
        deserializer_{deserializer},
        current_e_uid_{},
        serializer_{serializer},
        message_builder_{message_builder},
        skeleton_builder_{skeleton_builder} {}

  /*!
   * \brief Destructor
   * \vprivate Component Private
   */
  ~BaseSkeletonImpl() noexcept override { UnregisterObject(); }

  /*!
   * \brief Unregister function called by the destructor, will initiate the unregistration of the implementation
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void UnregisterObject() noexcept {
    if (implementation_registered_) {
      object_registration_.UnregisterInstance(implementation_);
    }
  }

  /*!
   * \copydoc crypto::server::BaseSkeleton::ProcessMessage
   * \details the properties of this method (thread-safety, reentrant, synchronous)
   *          depend on the underlying implementation.
   * \context ANY
   * \pre -
   * \vprivate Component Private
   */
  auto ProcessMessage(ara::crypto::ReadOnlyMemRegion input_buffer, ara::crypto::WritableMemRegion output_buffer,
                      amsr::sec::ipc::UserId e_uid) noexcept -> ReadOnlyMemResult override {
    amsr::sec::ipc::MessageObjectLifetimeManager object_lifetime_manager{};
    current_e_uid_ = e_uid;
    return deserializer_.DeserializeMessage<TaskEnumType>(input_buffer, object_lifetime_manager)
        .AndThen([this, &input_buffer, &output_buffer](amsr::sec::ipc::BasicMessage<TaskEnumType> message_task) {
          TaskEnumType const detail_task{message_task.DetailTask()};
          return DispatchMethod(detail_task, input_buffer, output_buffer);
        });
  }

 protected:
  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  BaseSkeletonImpl(BaseSkeletonImpl const& /*other*/) = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  BaseSkeletonImpl(BaseSkeletonImpl&& /*other*/) noexcept = default;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Default copy assignment operator
   * \return reference to this
   * \vprivate Component Private
   */
  BaseSkeletonImpl& operator=(BaseSkeletonImpl const& /*other*/) & = default;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Default move assignment operator
   * \return reference to this
   * \vprivate Component Private
   */
  BaseSkeletonImpl& operator=(BaseSkeletonImpl&& /*other*/) & noexcept = default;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_cannot_be_made_const_false_positive
  /*!
   * \brief Method dispatcher for this skeleton. Calls the correct function for the taskid of the message.
   * \param[in] task Task to dispatch the message to the correct implementation method
   * \param[in] input_buffer Incoming message to dispatch and process
   * \param[out] output_buffer Output message to return the message
   * \return Subspan of the output buffer that contains the response message
   * \details the properties of this method (thread-safety, reentrant, synchronous, errors)
   *          depend on the underlying implementation.
   * \context ANY
   * \pre -
   * \vprivate Component Private
   */
  virtual ReadOnlyMemResult DispatchMethod(TaskEnumType task, ara::crypto::ReadOnlyMemRegion input_buffer,
                                           ara::crypto::WritableMemRegion output_buffer) noexcept = 0;

 protected:
  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_function_cannot_be_made_static_false_positive
  // VECTOR NC AutosarC++17_10-A13.3.1: MD_CRYPTO_AutosarC++17_10-A13.3.1_overloading_templates_w_forwarding_reference
  /*!
   * \brief Dummy template which should never be called during normal operation
   * \tparam ReturnValueType Type of the argument to process, specialization for "unique_ptr" types
   * \param[in] return_value ReturnValueType - Parameter unused.
   * \param[in] new_proxy_ids Object to process - Parameter unused.
   * \param[out] out_buffer Buffer to write the result message into - Parameter unused.
   * \param[in] arguments Type of the argument to process - Parameter unused.
   * \return subspan of the buffer that contains the message
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \error ara::crypto::SecurityErrc::kRpcRuntimeFault Received multiple ProxyIds but function doesn't return multiple.
   * identifiable objects.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename ReturnValueType, typename... ArgumentTypes>
  auto ProcessReturnValue(ReturnValueType return_value, amsr::core::Span<amsr::sec::ipc::ProxyId const> new_proxy_ids,
                          ara::crypto::WritableMemRegion out_buffer,
                          std::tuple<ArgumentTypes...> const& arguments) noexcept -> ReadOnlyMemResult {
    static_cast<void>(return_value);
    static_cast<void>(new_proxy_ids);
    static_cast<void>(out_buffer);
    static_cast<void>(arguments);
    logger_.LogError(AMSR_FILE, __LINE__)
        << "Received multiple ProxyIds but function doesn't return multiple identifiable objects.";
    return ReadOnlyMemResult{ara::crypto::SecurityErrc::kRpcRuntimeFault};
  }

  // VECTOR NC AutosarC++17_10-A13.3.1: MD_CRYPTO_AutosarC++17_10-A13.3.1_overloading_templates_w_forwarding_reference
  /*!
   * \brief Method for processing the return value of the function call and creating the serialized message
   *        that will be returned to the caller proxy. This variant is used for functions which create multiple new
   *        proxy objects in an out vector.
   * \tparam ReturnValueType  Type of the argument to process, specialization for "unique_ptr" types
   * \param[in] return_value  ReturnValueType
   * \param[in] new_proxy_ids Object to process
   * \param[out] out_buffer   Buffer to write the result message into
   * \param[in] arguments     Type of the argument to process
   * \return subspan of the buffer that contains the message
   * \pre Length of the result vector (first element in \c arguments) must not be longer than \c new_proxy_ids.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kRpcInvalidInputSize Maximum length of output vector exceeded.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Runtime error.
   * \error ara::crypto::SecurityErrc::kUnsupportedFormat Buffer too small to hold serialized message of required type.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename ReturnValueType, typename VectorType, typename... ArgumentTypes,
            std::enable_if_t<(common::IsVector<typename std::remove_reference<VectorType>::type>::value) &&
                                 (common::is_identifiable<typename common::RemoveVector<
                                      typename std::remove_reference<VectorType>::type>::type>::value),
                             std::int32_t> = 0>
  auto ProcessReturnValue(ReturnValueType return_value, amsr::core::Span<amsr::sec::ipc::ProxyId const> new_proxy_ids,
                          ara::crypto::WritableMemRegion out_buffer,
                          std::tuple<VectorType, ArgumentTypes...> const& arguments) noexcept -> ReadOnlyMemResult {
    using ElementType = common::raw_class_name<typename common::RemoveVector<
        typename std::remove_reference<VectorType>::type>::type>; /*!< Element type in vector. */
    ReadOnlyMemResult result{ara::crypto::SecurityErrc::kRuntimeFault};
    VectorType& out_vector{std::get<0>(arguments)};
    if (out_vector.size() > new_proxy_ids.size()) {
      logger_.LogError(AMSR_FILE, __LINE__) << "Maximum length of output vector exceeded.";
      result.EmplaceError(ara::crypto::SecurityErrc::kRpcInvalidInputSize);
    } else {
      for (size_t i{0}; i < out_vector.size(); i++) {
        // VECTOR NL AutosarC++17_10-A5.2.3: MD_CRYPTO_AutosarC++17_10-A5.2.3_const_cast
        ElementType* const raw_pointer{out_vector[i].get()};
        base_skeleton_registration_.RegisterBaseSkeleton(new_proxy_ids[i],
                                                         skeleton_builder_.MakeSkeleton(*raw_pointer));
        object_registration_.RegisterInstance(new_proxy_ids[i], std::move(out_vector[i]));
      }
      amsr::sec::ipc::BasicMessage<amsr::sec::ipc::Task, ReturnValueType, VectorType, ArgumentTypes...> const
          response_message{message_builder_.BuildReturnMessage(return_value, arguments)};
      result = serializer_.SerializeMessage(response_message, out_buffer);
    }
    return result;
  }

  // VECTOR NC AutosarC++17_10-A13.3.1: MD_CRYPTO_AutosarC++17_10-A13.3.1_overloading_templates_w_forwarding_reference
  /*!
   * \brief Method for processing the return value of the function call and creating the serialized message
   *        that will be returned to the caller proxy. This variant processes amsr::core::Result<std::unique_ptr<T>>
   *        and amsr::core::Result<std::shared_ptr<T>> results.
   * \tparam ReturnValueType Type of the argument to process, specialization for "unique_ptr" types
   * \param[in] return_value  ReturnValueType
   * \param[in] new_proxy_id Object to process
   * \param[out] out_buffer   Buffer to write the result message into
   * \param[in] arguments     Type of the argument to process
   * \return subspan of the buffer that contains the message
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kRuntimeFault Runtime error.
   * \error ara::crypto::SecurityErrc::kUnsupportedFormat Buffer too small to hold serialized message of required type.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <
      typename ReturnValueType, typename... ArgumentTypes,
      std::enable_if_t<common::IsResult<ReturnValueType>::value &&
                           (common::is_unique_pointer<typename common::RemoveResult<ReturnValueType>::type>::value ||
                            common::is_shared_pointer<typename common::RemoveResult<ReturnValueType>::type>::value),
                       std::int32_t> = 0>
  auto ProcessReturnValue(ReturnValueType&& return_value, amsr::sec::ipc::ProxyId new_proxy_id,
                          ara::crypto::WritableMemRegion out_buffer,
                          std::tuple<ArgumentTypes...> const& arguments) noexcept -> ReadOnlyMemResult {
    // VECTOR NL AutosarC++17_10-A2.8.3: MD_CRYPTO_AutosarC++17_10-A2.8.3_false_positive
    using PointerType = typename common::RemoveResult<ReturnValueType>::type;
    // VECTOR NL AutosarC++17_10-A2.8.3: MD_CRYPTO_AutosarC++17_10-A2.8.3_false_positive
    using ValueType = typename common::classname_without_smart_ptr<PointerType>;

    ReadOnlyMemResult result{ara::crypto::SecurityErrc::kRuntimeFault};
    amsr::core::Result<ValueType*> real_result{nullptr};

    if (return_value.HasValue()) {
      if (return_value.Value() != nullptr) {
        // use shared_ptr instead unique_ptr to be able to copy and register it
        // VECTOR NL AutosarC++17_10-A5.2.3: MD_CRYPTO_AutosarC++17_10-A5.2.3_const_cast
        real_result.EmplaceValue(const_cast<ValueType*>(return_value.Value().get()));
        // VECTOR NL AutosarC++17_10-A5.2.3: MD_CRYPTO_AutosarC++17_10-A5.2.3_const_cast
        ValueType* const non_const_object{const_cast<ValueType*>(real_result.Value())};
        base_skeleton_registration_.RegisterBaseSkeleton(new_proxy_id,
                                                         skeleton_builder_.MakeSkeleton(*non_const_object));
        object_registration_.RegisterInstance(new_proxy_id, std::move(return_value.Value()));
      }
    } else {
      real_result.EmplaceError(return_value.Error());
    }

    amsr::sec::ipc::BasicMessage<amsr::sec::ipc::Task, amsr::core::Result<ValueType*>, ArgumentTypes...> const
        response_message{message_builder_.BuildReturnMessage(real_result, arguments)};
    result = serializer_.SerializeMessage(response_message, out_buffer);

    return result;
  }

  // VECTOR NC AutosarC++17_10-A13.3.1: MD_CRYPTO_AutosarC++17_10-A13.3.1_overloading_templates_w_forwarding_reference
  /*!
   * \brief Method for processing the return value of the function call and creating the serialized message
   *        that will be returned to the caller proxy. This variant processes simple return types.
   * \tparam ReturnValueType Type of the argument to process, specialization for "simple" types
   * \param[in] return_value  ReturnValueType
   * \param[in] new_proxy_id Object to process - Parameter unused.
   * \param[out] out_buffer   Buffer to write the result message into
   * \param[in] arguments     Type of the argument to process
   * \return subspan of the buffer that contains the message
   * \context ANY
   * \pre -
   * \threadsafe TRUE out_buffer must not be accessed in another thread.
   * \error ara::crypto::SecurityErrc::kUnsupportedFormat Buffer too small to hold serialized message of required type.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename ReturnValueType, typename... ArgumentTypes,
            std::enable_if_t<
                (!std::is_void<ReturnValueType>::value) && (!common::is_unique_pointer<ReturnValueType>::value) &&
                    (!common::is_shared_pointer<ReturnValueType>::value) &&
                    (!(std::is_reference<ReturnValueType>::value && common::is_identifiable<ReturnValueType>::value)) &&
                    (!std::is_same<ReturnValueType, char const*>::value) &&
                    (!common::is_shared_pointer<typename common::RemoveResult<ReturnValueType>::type>::value),
                std::int32_t> = 0>
  auto ProcessReturnValue(ReturnValueType return_value, amsr::sec::ipc::ProxyId new_proxy_id,
                          ara::crypto::WritableMemRegion out_buffer,
                          std::tuple<ArgumentTypes...> arguments) const noexcept -> ReadOnlyMemResult {
    static_cast<void>(new_proxy_id);
    // VECTOR NL AutosarC++17_10-A2.8.3: MD_CRYPTO_AutosarC++17_10-A2.8.3_false_positive
    using NonRefReturnValueType = typename std::remove_reference<ReturnValueType>::type;
    // VECTOR Disable AutosarC++17_10-A5.0.1: MD_CRYPTO_AutosarC++17_10-A5.0.1
    amsr::sec::ipc::BasicMessage<amsr::sec::ipc::Task, NonRefReturnValueType, ArgumentTypes...> const response_message{
        message_builder_.BuildReturnMessage<NonRefReturnValueType>(return_value, std::move(arguments))};
    // VECTOR Enable AutosarC++17_10-A5.0.1: MD_CRYPTO_AutosarC++17_10-A5.0.1
    return serializer_.SerializeMessage(response_message, out_buffer);
  }

  // VECTOR NC AutosarC++17_10-A13.3.1: MD_CRYPTO_AutosarC++17_10-A13.3.1_overloading_templates_w_forwarding_reference
  /*!
   * \brief Method for processing the return value of the function call and creating the serialized message
   *        that will be returned to the caller proxy. This variant processes char const*.
   * \tparam ReturnValueType Type of the argument to process, specialization for "simple" types
   * \param[in] return_value  ReturnValueType
   * \param[in] new_proxy_id Object to process - Parameter unused.
   * \param[out] out_buffer   Buffer to write the result message into
   * \param[in] arguments     Type of the argument to process
   * \return subspan of the buffer that contains the message
   * \context ANY
   * \pre -
   * \threadsafe TRUE out_buffer must not be accessed in another thread.
   * \error ara::crypto::SecurityErrc::kUnsupportedFormat Buffer too small to hold serialized message of required type.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename ReturnValueType, typename... ArgumentTypes,
            std::enable_if_t<std::is_same<ReturnValueType, char const*>::value, std::int32_t> = 0>
  auto ProcessReturnValue(ReturnValueType return_value, amsr::sec::ipc::ProxyId new_proxy_id,
                          ara::crypto::WritableMemRegion out_buffer,
                          std::tuple<ArgumentTypes...> arguments) const noexcept -> ReadOnlyMemResult {
    static_cast<void>(new_proxy_id);
    amsr::sec::ipc::BasicMessage<amsr::sec::ipc::Task, amsr::core::String, ArgumentTypes...> const response_message{
        message_builder_.BuildReturnMessage<amsr::core::String>(amsr::core::String(return_value),
                                                                std::move(arguments))};
    return serializer_.SerializeMessage(response_message, out_buffer);
  }

  // VECTOR NC AutosarC++17_10-A13.3.1: MD_CRYPTO_AutosarC++17_10-A13.3.1_overloading_templates_w_forwarding_reference
  /*!
   * \brief Method for processing the return value of the function call and creating the serialized message
   *        that will be returned to the caller proxy. This variant processes void return types.
   * \tparam ReturnValueType Type of the argument to process, specialization for "simple" types
   * \param[in] new_proxy_id Object to process - Parameter unused.
   * \param[out] out_buffer   Buffer to write the result message into
   * \param[in] arguments     Type of the argument to process
   * \return subspan of the buffer that contains the message
   * \context ANY
   * \pre -
   * \threadsafe TRUE out_buffer must not be accessed in another thread.
   * \error ara::crypto::SecurityErrc::kUnsupportedFormat Buffer too small to hold serialized message of required type.
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename ReturnValueType, typename... ArgumentTypes,
            std::enable_if_t<std::is_void<ReturnValueType>::value, std::int32_t> = 0>
  auto ProcessReturnValue(amsr::sec::ipc::ProxyId new_proxy_id, ara::crypto::WritableMemRegion out_buffer,
                          std::tuple<ArgumentTypes...> const& arguments) const noexcept -> ReadOnlyMemResult {
    amsr::sec::ipc::BasicMessage<amsr::sec::ipc::Task, ArgumentTypes...> const response_message{
        message_builder_.BuildReturnMessage<void>(arguments)};
    static_cast<void>(new_proxy_id);
    return serializer_.SerializeMessage(response_message, out_buffer);
  }

  // VECTOR NC AutosarC++17_10-A13.3.1: MD_CRYPTO_AutosarC++17_10-A13.3.1_overloading_templates_w_forwarding_reference
  /*!
   * \brief Method for processing the return value of the function call and creating the serialized message
   *        that will be returned to the caller proxy. This variant processes unique_ptr and shared_ptr return values
   * and will register the object to the object manager.
   * \tparam ReturnValueType Type of the argument to process, specialization for "unique_ptr" types
   * \param[in] new_object  ReturnValueType
   * \param[in] new_proxy_id Object to process
   * \param[out] out_buffer   Buffer to write the result message into
   * \param[in] arguments     Type of the argument to process
   * \return subspan of the buffer that contains the message
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kRuntimeFault Runtime error.
   * \error ara::crypto::SecurityErrc::kUnsupportedFormat Buffer too small to hold serialized message of required type.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename ReturnValueType, typename... ArgumentTypes,
            std::enable_if_t<((common::is_unique_pointer<ReturnValueType>::value) ||
                              (common::is_shared_pointer<ReturnValueType>::value)),
                             std::int32_t> = 0>
  auto ProcessReturnValue(ReturnValueType new_object, amsr::sec::ipc::ProxyId new_proxy_id,
                          ara::crypto::WritableMemRegion out_buffer,
                          std::tuple<ArgumentTypes...> const& arguments) noexcept -> ReadOnlyMemResult {
    // VECTOR NL AutosarC++17_10-A2.8.3: MD_CRYPTO_AutosarC++17_10-A2.8.3_false_positive
    using RawReturnValueType = common::classname_without_smart_ptr<ReturnValueType>;
    ReadOnlyMemResult result{ara::crypto::SecurityErrc::kRuntimeFault};
    if (new_object != nullptr) {
      // VECTOR NL AutosarC++17_10-A5.2.3: MD_CRYPTO_AutosarC++17_10-A5.2.3_const_cast
      RawReturnValueType* const non_const_object{const_cast<RawReturnValueType*>(new_object.get())};
      base_skeleton_registration_.RegisterBaseSkeleton(new_proxy_id, skeleton_builder_.MakeSkeleton(*non_const_object));
      object_registration_.RegisterInstance(new_proxy_id, std::move(new_object));
      amsr::sec::ipc::BasicMessage<amsr::sec::ipc::Task, amsr::sec::ipc::SerializationTags, ArgumentTypes...> const
          response_message{
              message_builder_.BuildReturnMessage(amsr::sec::ipc::SerializationTags::tag_skeleton_created, arguments)};
      result = serializer_.SerializeMessage(response_message, out_buffer);
    } else {
      amsr::sec::ipc::BasicMessage<amsr::sec::ipc::Task, amsr::sec::ipc::SerializationTags, ArgumentTypes...> const
          response_message{message_builder_.BuildReturnMessage(
              amsr::sec::ipc::SerializationTags::tag_skeleton_not_created, arguments)};
      result = serializer_.SerializeMessage(response_message, out_buffer);
    }
    return result;
  }

  // VECTOR NC AutosarC++17_10-A13.3.1: MD_CRYPTO_AutosarC++17_10-A13.3.1_overloading_templates_w_forwarding_reference
  /*!
   * \brief Method for processing the return value of the function call and creating the serialized message
   *        that will be returned to the caller proxy. This variant processes reference return values and will
   * register the object as reference on the object registration.
   * \details This does not affect the life time of the object
   * \tparam ReturnValueType Type of the argument to process, specialization for "unique_ptr" types
   * \param[in] new_object    ReturnValueType
   * \param[in] new_proxy_id  Object to process
   * \param[out] out_buffer   Buffer to write the result message into
   * \param[in] arguments     Type of the argument to process
   * \return subspan of the buffer that contains the message
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \error ara::crypto::SecurityErrc::kUnsupportedFormat Buffer too small to hold serialized message of required type.
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename ReturnValueType, typename... ArgumentTypes,
            std::enable_if_t<((std::is_reference<ReturnValueType>::value &&
                               std::is_base_of<common::Identifiable, common::raw_class_name<ReturnValueType>>::value)),
                             std::int32_t> = 0>
  auto ProcessReturnValue(ReturnValueType new_object, amsr::sec::ipc::ProxyId new_proxy_id,
                          ara::crypto::WritableMemRegion out_buffer,
                          std::tuple<ArgumentTypes...> const& arguments) noexcept -> ReadOnlyMemResult {
    // VECTOR NL AutosarC++17_10-A2.8.3: MD_CRYPTO_AutosarC++17_10-A2.8.3_false_positive
    using RawRetValueType = typename std::add_lvalue_reference<
        typename std::remove_const<typename std::remove_reference<ReturnValueType>::type>::type>::type;

    // VECTOR NL AutosarC++17_10-A5.2.3: MD_CRYPTO_AutosarC++17_10-A5.2.3_const_cast
    RawRetValueType non_const_object{const_cast<RawRetValueType>(new_object)};
    base_skeleton_registration_.RegisterBaseSkeleton(new_proxy_id, skeleton_builder_.MakeSkeleton(non_const_object));
    object_registration_.RegisterInstance(new_proxy_id, non_const_object);
    amsr::sec::ipc::BasicMessage<amsr::sec::ipc::Task, amsr::sec::ipc::SerializationTags, ArgumentTypes...> const
        response_message{
            message_builder_.BuildReturnMessage(amsr::sec::ipc::SerializationTags::tag_skeleton_created, arguments)};
    return serializer_.SerializeMessage(response_message, out_buffer);
  }

  // VECTOR NC AutosarC++17_10-M7.1.2: MD_CRYPTO_AutosarC++17_10-M7.1.2_compilation_error
  /*!
   * \brief Template implementation for expanding std::tuple and calling callback method with the expanded values as
   * arguments.
   * Example: Given "void foo(string, int, bool)" and "Tuple<string, int bool> t"
   * This call is not possible: foo(t)
   * The template expands the tuple and calls the function: apply_from_tuple(foo, t) will generate:
   * foo(std::get<0>(t), std::get<1>(t), std::get<2>(t)) and return the return value of the call to foo
   *
   * \tparam ReturnType     Return type of the function being called
   * \tparam Function       The function type being called
   * \tparam ArgumentTuple  Content types of the tuple containing all arguments to call the function with
   * \param[in] function        Function to be called
   * \param[in] argument_tuple  Arguments to call the function with
   * \return                Return value of function
   * \details the properties of this method (thread-safety, exceptions and exception-safety, reentrant, synchronous)
   *          depend on the underlying implementation.
   * \context ANY
   * \pre -
   * \vprivate Component Private
   */
  template <typename ReturnType, typename Function, typename ArgumentTuple, std::size_t... S>
  ReturnType CallFunctionWithArgsFromTupleInternal(Function&& function, ArgumentTuple&& argument_tuple,
                                                   std::index_sequence<S...>) const noexcept {
    // VECTOR Disable AutosarC++17_10-A5.0.1: MD_CRYPTO_AutosarC++17_10-A5.0.1
    return (implementation_.*function)(std::get<S>(std::forward<ArgumentTuple>(argument_tuple))...);
    // VECTOR Enable AutosarC++17_10-A5.0.1
  }

  /*!
   *
   * \tparam ReturnType     Return type of the function being called
   * \tparam Function       The function type being called
   * \tparam ArgumentTuple  Content types of the tuple containing all arguments to call the function with
   * \param[in] function        Function to be called
   * \param[in] argument_tuple  Arguments to call the function with
   * \return                Return value of function
   * \details the properties of this method (thread-safety, exceptions and exception-safety, reentrant, synchronous)
   *          depend on the underlying implementation.
   * \context ANY
   * \pre -
   * \vprivate Component Private
   */
  template <typename ReturnType, typename Function, typename ArgumentTuple>
  ReturnType CallFunctionWithArgumentsFromTuple(Function&& function, ArgumentTuple&& argument_tuple) const noexcept {
    std::size_t constexpr tuple_size{std::tuple_size<typename std::remove_reference<ArgumentTuple>::type>::value};
    return (CallFunctionWithArgsFromTupleInternal<ReturnType>(std::forward<Function>(function),
                                                              std::forward<ArgumentTuple>(argument_tuple),
                                                              std::make_index_sequence<tuple_size>()));
  }

  /*!
   * \brief Template for calling the "real" implementation of the provided function. This method performs several
   * tasks:
   * (1) Deserialize the input data into the Argument types of the provided function
   * (2) Call the function provided as argument with the implementor-object as first argument and the argument values
   * that have been deserialized and extracted from the tuple
   * (3) Accept the return value of the method and extract the raw pointer if applicable (for smart-pointers)
   * (4) Create a result message for the return value and serialize it
   * NOTE: This function is overloaded for and const   non-const functions
   * \tparam ReturnType Return type of the function
   * \tparam ArgumentTypes Types of the expected arguments
   * \param[in] function    Function with qualifier const to be called on the implementation.
   * \param[in] input_buffer Incoming message to dispatch and process
   * \param[out] output_buffer Output message to return the message
   * \return The serialized message containing the return value of the method.
   * \details the properties of this method (thread-safety, reentrant, synchronous)
   *          depend on the underlying implementation.
   * \context ANY
   * \pre -
   * \error ara::crypto::SecurityErrc::kRpcInvalidArgument Received multiple ProxyIds but function needs 0 or 1.
   * \error ara::crypto::SecurityErrc::kRuntimeFault Runtime error.
   * \error ara::crypto::SecurityErrc::kUnsupportedFormat Buffer too small to hold serialized message of required type.
   * \vprivate Component Private
   */
  template <typename ReturnType, typename ClassType, typename... ArgumentTypes,
            std::enable_if_t<((!std::is_void<ReturnType>::value) && (!common::is_unique_pointer<ReturnType>::value) &&
                              (!common::is_unique_pointer<typename common::RemoveResult<ReturnType>::type>::value) &&
                              (!common::is_shared_pointer<typename common::RemoveResult<ReturnType>::type>::value)),
                             std::int32_t> = 0>
  auto CallImplementation(ReturnType (ClassType::*function)(ArgumentTypes...) const,
                          ara::crypto::ReadOnlyMemRegion input_buffer,
                          ara::crypto::WritableMemRegion output_buffer) noexcept -> ReadOnlyMemResult {
    amsr::sec::ipc::MessageObjectLifetimeManager object_lifetime_manager{};

    return deserializer_.DeserializeMessage<TaskEnumType, ArgumentTypes...>(input_buffer, object_lifetime_manager)
        .AndThen(
            [this, &function, &output_buffer](amsr::sec::ipc::BasicMessage<TaskEnumType, ArgumentTypes...>&& message) {
              // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
              auto result = ReadOnlyMemResult::FromError(ara::crypto::SecurityErrc::kRpcInvalidArgument);
              // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
              auto const new_proxy_ids = message.NewProxyId();
              std::tuple<ArgumentTypes...>& msg_content{message.Content()};

              ReturnType const return_value{CallFunctionWithArgumentsFromTuple<ReturnType>(function, msg_content)};

              if (new_proxy_ids.size() == 1) {  // exactly one (identifiable) object returned
                amsr::sec::ipc::ProxyId const proxy_id{new_proxy_ids[0]};
                result = ProcessReturnValue<ReturnType>(return_value, proxy_id, output_buffer, msg_content);
              } else if (new_proxy_ids.size() > 1) {  // multiple identifiable objects returned (e.g. in out parameters)
                // should not happen in this template
                // VECTOR NL AutosarC++17_10-A5.1.8: MD_CRYPTO_AutosarC++17_10-A5.1.8_false_positive
                logger_.LogError(AMSR_FILE, __LINE__) << "Received multiple new proxy_ids but function needs 0 or 1.";
              } else {  // no identifiable object returned
                result =
                    ProcessReturnValue<ReturnType>(return_value, amsr::sec::ipc::ProxyId{}, output_buffer, msg_content);
              }
              return result;
            });
  }

  /*!
   * \copydoc CallImplementation
   * \param[in] function Function without qualifier const to be called on the implementation.
   * \vprivate Component Private
   */
  template <typename ReturnType, typename ClassType, typename... ArgumentTypes,
            std::enable_if_t<((!std::is_void<ReturnType>::value) && (!common::is_unique_pointer<ReturnType>::value) &&
                              ((!common::IsResult<ReturnType>::value) ||
                               (!(common::is_unique_pointer<typename common::RemoveResult<ReturnType>::type>::value ||
                                  common::is_shared_pointer<typename common::RemoveResult<ReturnType>::type>::value)))),
                             std::int32_t> = 0>
  auto CallImplementation(ReturnType (ClassType::*function)(ArgumentTypes...),
                          ara::crypto::ReadOnlyMemRegion input_buffer,
                          ara::crypto::WritableMemRegion output_buffer) noexcept -> ReadOnlyMemResult {
    amsr::sec::ipc::MessageObjectLifetimeManager object_lifetime_manager{};

    return deserializer_.DeserializeMessage<TaskEnumType, ArgumentTypes...>(input_buffer, object_lifetime_manager)
        .AndThen([this, &function,
                  &output_buffer](amsr::sec::ipc::BasicMessage<TaskEnumType, ArgumentTypes...>&& message) {
          // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
          auto result = ReadOnlyMemResult::FromError(ara::crypto::SecurityErrc::kRpcInvalidArgument);
          // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
          auto const new_proxy_ids = message.NewProxyId();
          std::tuple<ArgumentTypes...>& msg_content{message.Content()};

          ReturnType return_value{CallFunctionWithArgumentsFromTuple<ReturnType>(function, msg_content)};

          if (new_proxy_ids.size() == 1) {  // exactly one (identifiable) object returned
            amsr::sec::ipc::ProxyId const proxy_id{new_proxy_ids[0]};
            result = ProcessReturnValue<ReturnType>(std::move(return_value), proxy_id, output_buffer,
                                                    std::forward<std::tuple<ArgumentTypes...>>(msg_content));
          } else if (new_proxy_ids.size() >
                     1) {  // potentially multiple identifiable objects returned (e.g. in out parameters)
            result = ProcessReturnValue<ReturnType>(std::move(return_value), new_proxy_ids, output_buffer,
                                                    std::forward<std::tuple<ArgumentTypes...>>(msg_content));
          } else {  // no identifiable object returned
            result = ProcessReturnValue<ReturnType>(std::move(return_value), amsr::sec::ipc::ProxyId{}, output_buffer,
                                                    std::forward<std::tuple<ArgumentTypes...>>(msg_content));
          }

          return result;
        });
  }

  /*!
   * \copydoc CallImplementation
   * \param[in] function Function without qualifier const to be called on the implementation.
   * \vprivate Component Private
   */
  template <typename ReturnType, typename ClassType, typename... ArgumentTypes,
            std::enable_if_t<common::is_unique_pointer<ReturnType>::value && common::is_identifiable<ReturnType>::value,
                             std::int32_t> = 0>
  auto CallImplementation(ReturnType (ClassType::*function)(ArgumentTypes...),
                          ara::crypto::ReadOnlyMemRegion input_buffer,
                          ara::crypto::WritableMemRegion output_buffer) noexcept -> ReadOnlyMemResult {
    amsr::sec::ipc::MessageObjectLifetimeManager object_lifetime_manager{};

    return deserializer_.DeserializeMessage<TaskEnumType, ArgumentTypes...>(input_buffer, object_lifetime_manager)
        .AndThen([this, &function,
                  &output_buffer](amsr::sec::ipc::BasicMessage<TaskEnumType, ArgumentTypes...>&& message) {
          // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
          auto result = ReadOnlyMemResult::FromError(ara::crypto::SecurityErrc::kRpcInvalidArgument);
          // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
          auto const new_proxy_ids = message.NewProxyId();
          std::tuple<ArgumentTypes...>& msg_content{message.Content()};

          if (new_proxy_ids.size() != 1) {
            // Exactly one new proxy_id should be received since we return a unique pointer to an identifiable
            // VECTOR NL AutosarC++17_10-A5.1.8: MD_CRYPTO_AutosarC++17_10-A5.1.8_false_positive
            logger_.LogError(AMSR_FILE, __LINE__) << "Received multiple or 0 object ids but function needs exactly 1.";
          } else {
            amsr::sec::ipc::ProxyId const new_proxy_id{new_proxy_ids[0]};
            ReturnType return_value{CallFunctionWithArgumentsFromTuple<ReturnType>(function, msg_content)};
            result = ProcessReturnValue<ReturnType>(std::move(return_value), new_proxy_id, output_buffer, msg_content);
          }
          return result;
        });
  }

  /*!
   * \copydoc CallImplementation
   * \param[in] function Function without qualifier const to be called on the implementation.
   * \vprivate Component Private
   */
  template <typename ReturnType, typename ClassType, typename... ArgumentTypes,
            std::enable_if_t<common::IsResult<ReturnType>::value &&
                                 (common::is_unique_pointer<typename common::RemoveResult<ReturnType>::type>::value ||
                                  common::is_shared_pointer<typename common::RemoveResult<ReturnType>::type>::value),
                             std::int32_t> = 0>
  auto CallImplementation(ReturnType (ClassType::*function)(ArgumentTypes...),
                          ara::crypto::ReadOnlyMemRegion input_buffer,
                          ara::crypto::WritableMemRegion output_buffer) noexcept -> ReadOnlyMemResult {
    amsr::sec::ipc::MessageObjectLifetimeManager object_lifetime_manager{};

    return deserializer_.DeserializeMessage<TaskEnumType, ArgumentTypes...>(input_buffer, object_lifetime_manager)
        .AndThen([this, &function,
                  &output_buffer](amsr::sec::ipc::BasicMessage<TaskEnumType, ArgumentTypes...>&& message) {
          // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
          auto result = ReadOnlyMemResult::FromError(ara::crypto::SecurityErrc::kRpcInvalidArgument);
          // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
          auto const new_proxy_ids = message.NewProxyId();
          std::tuple<ArgumentTypes...>& msg_content{message.Content()};

          if (new_proxy_ids.size() != 1) {
            // Exactly one new proxy_id should be received since we return a unique pointer to an identifiable.
            // VECTOR NL AutosarC++17_10-A5.1.8: MD_CRYPTO_AutosarC++17_10-A5.1.8_false_positive
            logger_.LogError(AMSR_FILE, __LINE__) << "Received multiple or 0 object ids but function needs exactly 1.";
          } else {
            ReturnType return_value{CallFunctionWithArgumentsFromTuple<ReturnType>(function, msg_content)};
            result =
                ProcessReturnValue<ReturnType>(std::move(return_value), new_proxy_ids[0], output_buffer, msg_content);
          }
          return result;
        });
  }

  /*!
   * \copydoc CallImplementation
   * \param[in] function Function with qualifier const to be called on the implementation.
   * \vprivate Component Private
   */
  template <typename ReturnType, typename ClassType, typename... ArgumentTypes,
            std::enable_if_t<common::IsResult<ReturnType>::value &&
                                 (common::is_unique_pointer<typename common::RemoveResult<ReturnType>::type>::value ||
                                  common::is_shared_pointer<typename common::RemoveResult<ReturnType>::type>::value),
                             std::int32_t> = 0>
  auto CallImplementation(ReturnType (ClassType::*function)(ArgumentTypes...) const,
                          ara::crypto::ReadOnlyMemRegion input_buffer,
                          ara::crypto::WritableMemRegion output_buffer) noexcept -> ReadOnlyMemResult {
    amsr::sec::ipc::MessageObjectLifetimeManager object_lifetime_manager{};
    return deserializer_.DeserializeMessage<TaskEnumType, ArgumentTypes...>(input_buffer, object_lifetime_manager)
        .AndThen([this, &function,
                  &output_buffer](amsr::sec::ipc::BasicMessage<TaskEnumType, ArgumentTypes...>&& message) {
          // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
          auto result = ReadOnlyMemResult::FromError(ara::crypto::SecurityErrc::kRpcInvalidArgument);
          // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
          auto const new_proxy_ids = message.NewProxyId();
          std::tuple<ArgumentTypes...>& msg_content{message.Content()};

          if (new_proxy_ids.size() != 1) {
            // Exactly one new proxy_id should be received since we return a unqiue pointer to an identifiable.
            // VECTOR NL AutosarC++17_10-A5.1.8: MD_CRYPTO_AutosarC++17_10-A5.1.8_false_positive
            logger_.LogError(AMSR_FILE, __LINE__) << "Received multiple or 0 object ids but function needs exactly 1.";
          } else {
            ReturnType return_value{CallFunctionWithArgumentsFromTuple<ReturnType>(function, msg_content)};
            result =
                ProcessReturnValue<ReturnType>(std::move(return_value), new_proxy_ids[0], output_buffer, msg_content);
          }
          return result;
        });
  }

  /*!
   * \copydoc CallImplementation
   * \param[in] function Function with return type void and qualifier const to be called on the implementation.
   * \vprivate Component Private
   */
  template <typename ReturnType, typename ClassType, typename... ArgumentTypes,
            std::enable_if_t<std::is_void<ReturnType>::value, std::int32_t> = 0>
  auto CallImplementation(ReturnType (ClassType::*function)(ArgumentTypes...) const,
                          ara::crypto::ReadOnlyMemRegion input_buffer,
                          ara::crypto::WritableMemRegion output_buffer) const noexcept -> ReadOnlyMemResult {
    amsr::sec::ipc::MessageObjectLifetimeManager object_lifetime_manager{};
    return deserializer_.DeserializeMessage<TaskEnumType, ArgumentTypes...>(input_buffer, object_lifetime_manager)
        .AndThen(
            [this, &function, &output_buffer](amsr::sec::ipc::BasicMessage<TaskEnumType, ArgumentTypes...>&& message) {
              std::tuple<ArgumentTypes...>& msg_content{message.Content()};
              CallFunctionWithArgumentsFromTuple<ReturnType>(function, msg_content);
              // No ProxyId because function returns void
              return ProcessReturnValue<ReturnType>(amsr::sec::ipc::ProxyId{}, output_buffer, msg_content);
            });
  }

  /*!
   * \copydoc CallImplementation
   * \param[in] function Function return type void and without qualifier const to be called on the implementation.
   * \vprivate Component Private
   */
  template <typename ReturnType, typename ClassType, typename... ArgumentTypes,
            std::enable_if_t<std::is_void<ReturnType>::value, std::int32_t> = 0>
  auto CallImplementation(ReturnType (ClassType::*function)(ArgumentTypes...),
                          ara::crypto::ReadOnlyMemRegion input_buffer,
                          ara::crypto::WritableMemRegion output_buffer) const noexcept -> ReadOnlyMemResult {
    amsr::sec::ipc::MessageObjectLifetimeManager object_lifetime_manager{};
    return deserializer_.DeserializeMessage<TaskEnumType, ArgumentTypes...>(input_buffer, object_lifetime_manager)
        .AndThen(
            [this, &function, &output_buffer](amsr::sec::ipc::BasicMessage<TaskEnumType, ArgumentTypes...>&& message) {
              std::tuple<ArgumentTypes...>& msg_content{message.Content()};
              // empty new object id because return type is void
              amsr::sec::ipc::ProxyId const proxy_id{};
              CallFunctionWithArgumentsFromTuple<ReturnType>(function, msg_content);
              return ProcessReturnValue<ReturnType>(proxy_id, output_buffer, msg_content);
            });
  }

  /*!
   * \copydoc CallImplementation
   * \param[in] allowed_uid UserId of the user which is allowed to perform this operation (e.g. trust master or ca
   * connector).
   * \details Checks if current_e_uid matches allowed_uid before calling the implementation.
   * \vprivate Component Private
   * \trace CREQ-Crypto-X509Provider_X509_Privileged_Applications
   */
  template <typename ReturnType, typename ClassType, typename... ArgumentTypes,
            std::enable_if_t<common::IsResult<ReturnType>::value, std::int32_t> = 0>
  auto CallRestrictedImplementation(ReturnType (ClassType::*function)(ArgumentTypes...),
                                    ara::crypto::ReadOnlyMemRegion input_buffer,
                                    ara::crypto::WritableMemRegion output_buffer,
                                    amsr::sec::ipc::UserId allowed_uid) noexcept -> ReadOnlyMemResult {
    ReadOnlyMemResult result{ara::crypto::SecurityErrc::kRuntimeFault};
    if (current_e_uid_ == allowed_uid) {
      result = CallImplementation(function, input_buffer, output_buffer);
    } else {
      amsr::sec::ipc::MessageObjectLifetimeManager object_lifetime_manager{};
      result =
          deserializer_.DeserializeMessage<TaskEnumType, ArgumentTypes...>(input_buffer, object_lifetime_manager)
              .AndThen([this, &output_buffer](amsr::sec::ipc::BasicMessage<TaskEnumType, ArgumentTypes...>&& message) {
                ReturnType const return_value{ara::crypto::SecurityErrc::kAccessViolation};
                std::tuple<ArgumentTypes...>& msg_content{message.Content()};
                // empty new object id because return type is void
                amsr::sec::ipc::ProxyId const proxy_id{};
                return ProcessReturnValue<ReturnType>(return_value, proxy_id, output_buffer, msg_content);
              });
    }
    return result;
  }

 protected:
  /*!
   * \brief Getter for the contained implementation instance
   * \return Object with the real implementation instance
   */
  // VECTOR NL AutosarC++17_10-M9.3.1: MD_CRYPTO_AutosarC++17_10-M9.3.1
  ImplementationType& GetImplementation() const noexcept { return implementation_; }

  /*!
   * \brief Getter for the contained deserializer
   * \return Deserializer
   */
  // VECTOR NL AutosarC++17_10-M9.3.1: MD_CRYPTO_AutosarC++17_10-M9.3.1
  crypto::server::ServerMessageDeserializer& GetDeserializer() const noexcept { return deserializer_; }

 private:
  ImplementationType& implementation_; /*!< Object with the real implementation to be called */
  bool implementation_registered_; /*!< Indicates whether the implementation object has been registered in the object
                                      registration */
  common::util::Logger logger_;    /*!< Logging instance */
  crypto::server::ObjectRegistration& object_registration_;              /*!< Register of objects */
  crypto::server::BaseSkeletonRegistration& base_skeleton_registration_; /*!< Register of base skeletons */
  amsr::sec::ipc::Transporter& transporter_;                             /*!< Transport interface for communication */
  crypto::server::ServerMessageDeserializer& deserializer_;              /*!< Deserializer for messages */
  // VECTOR NC AutosarC++17_10-A12.1.3: MD_CRYPTO_AutosarC++17_10-A12.1.3
  amsr::sec::ipc::UserId current_e_uid_;                        /*!< Current user id */
  amsr::sec::ipc::MessageSerializer& serializer_;               /*!< Serializer for result messages */
  amsr::sec::ipc::MessageBuilder& message_builder_;             /*!< MessageBuilder for creating result messages */
  crypto::server::skeleton::SkeletonBuilder& skeleton_builder_; /*!< Builder for skeleton objects */
};

}  // namespace server
}  // namespace crypto

#endif  // LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_BASE_SKELETON_IMPL_H_
