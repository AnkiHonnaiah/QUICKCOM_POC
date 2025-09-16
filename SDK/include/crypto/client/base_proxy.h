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
/*!        \file  base_proxy.h
 *        \brief  Base class for all proxy objects.
 *
 *      \details  Base class for all client-side proxy objects. These objects are supposed to forward any call
 *                to the server side, including the arguments passed to the proxy. Therefor the base proxy will
 *                wrap and send them in a serialized message.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_BASE_PROXY_H_
#define LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_BASE_PROXY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <cstdint>
#include <memory>
#include <tuple>
#include <utility>
#include <vector>

#include "amsr/generic/singleton_wrapper.h"
#include "amsr/sec/ipc/basic_message.h"
#include "amsr/sec/ipc/message_builder.h"
#include "amsr/sec/ipc/message_deserializer.h"
#include "amsr/sec/ipc/message_serializer.h"
#include "amsr/sec/ipc/protocol.h"
#include "amsr/sec/ipc/transporter.h"
#include "crypto/client/deletion/proxy_deletion_handler.h"
#include "crypto/client/message_error_handler.h"
#include "crypto/client/out_param_updater.h"
#include "crypto/client/session_handler.h"
#include "crypto/client/type_traits.h"
#include "crypto/common/constants.h"
#include "crypto/common/util/contract.h"
#include "crypto/common/util/logger.h"
#include "vac/container/string_literals.h"

namespace crypto {
namespace client {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_CRYPTO_AutosarC++17_10-M7.3.6_sv
/*! Introduce string literal for StringView creation */
using vac::container::operator""_sv;

// VECTOR NC AutosarC++17_10-M10.1.1: MD_CRYPTO_AutosarC++17_10-M10.1.1_diamond
/*!
 * \brief Base class for all client-side proxy objects
 * \trace CREQ-Crypto-CryptoProvider_ProcessIsolation
 * \trace DSGN-Crypto-ProcessIsolation, DSGN-Crypto-Skeleton_Proxy
 * \vprivate Component Private
 */
class BaseProxy : virtual public crypto::common::Identifiable {
 public:
  // VECTOR NC AutosarC++17_10-A15.4.2: MD_CRYPTO_AutosarC++17_10-A15.4.2_noexcept_unsupported_in_used_api
  // VECTOR NC AutosarC++17_10-A15.5.1: MD_CRYPTO_AutosarC++17_10-A15.5.1_function_must_not_exit_with_an_exception
  /*!
   * \brief Destructor
   * \vprivate Component Private
   */
  ~BaseProxy() override {
    if (send_destroy_message_) {
      deletion_handler_.HandleDeletion(proxy_id_);
    }
  }

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted copy assignment operator
   * \return -
   * \vprivate Component Private
   */
  BaseProxy& operator=(BaseProxy const& /*other*/) & = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Deleted move assignment operator
   * \return -
   * \vprivate Component Private
   */
  BaseProxy& operator=(BaseProxy&& /*other*/) & noexcept = delete;

  /*!
   * \brief Constructor
   * \copydetails BaseProxy(amsr::sec::ipc::Transporter&, amsr::sec::ipc::MessageBuilder&,
   * amsr::sec::ipc::MessageSerializer&, amsr::sec::ipc::MessageDeserializer&)
   * \param[in] deletion_handler      Handler object that will be called on deletion of this object
   * \vprivate Component Private
   */
  BaseProxy(amsr::sec::ipc::Transporter& transporter, amsr::sec::ipc::MessageBuilder& message_builder,
            amsr::sec::ipc::MessageSerializer& message_serializer,
            amsr::sec::ipc::MessageDeserializer& message_deserializer,
            crypto::client::deletion::ProxyDeletionHandler const&& deletion_handler)
      : crypto::common::Identifiable(),
        logger_{common::util::kClientLoggingContext, ""_sv},
        session_handler_access_{SessionHandler::Instance()},
        transporter_{transporter},
        message_builder_{message_builder},
        message_serializer_{message_serializer},
        message_deserializer_{message_deserializer},
        deletion_handler_{deletion_handler},
        proxy_id_{},
        message_error_handler_{},
        out_param_updater_{},
        send_destroy_message_{true} {
    proxy_id_ = amsr::sec::ipc::GetProxyId(this);
  }

  /*!
   * \brief Overloaded constructor that provides the default deletion handler of type ProxyDeletionHandler
   * \param[in] transporter           Transporter interface for communication between client and server objects
   * \param[in] message_builder       Builder for messages to be sent to the server
   * \param[in] message_serializer    Serializer for serializing messages to send them via the transporter interface
   * \param[in] message_deserializer  Deserializer for deserializing responses received via the transporter interface
   * \vprivate Component Private
   */
  BaseProxy(amsr::sec::ipc::Transporter& transporter, amsr::sec::ipc::MessageBuilder& message_builder,
            amsr::sec::ipc::MessageSerializer& message_serializer,
            amsr::sec::ipc::MessageDeserializer& message_deserializer)
      : BaseProxy(transporter, message_builder, message_serializer, message_deserializer,
                  crypto::client::deletion::ProxyDeletionHandler(transporter, message_serializer)) {}

  /*!
   * \brief Sets whether a destroy message should be sent on destruction of this object.
   * \param[in] enable Enable or disable sending a destroy message.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void SetSendDestroyMessage(bool enable) const noexcept { send_destroy_message_ = enable; }

 protected:
  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  BaseProxy(BaseProxy const& /*other*/) = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  BaseProxy(BaseProxy&& /*other*/) noexcept = default;

 public:
  /*!
   * \brief Template method for sending a request for calling a remote function to the server side.
   *        This overload is used for VOID functions that do not return a value
   *        Therefor it generates a message, serializes it and sends it.
   *        After a response was received, this is extracted and returned.
   *        If length required to serialize ReadOnlyMemRegion is larger than
   *        kTransportBufferSize, data is chunked and iteratively called.
   *        This call is BLOCKING until the response from the transporter interface is received.
   * \param[in] task_id TaskId of the action to be performed.
   * \param[in] args Argument list to be passed to the server.
   * \return Return value received from server side.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \internal
   * - Build a BasicMessage object containing the function to be called and all involved objects / parameters.
   * - Serialize this message and send it using the \c transporter_.
   * - Wait for the return message and abort on possible server side exceptions
   * - Deserialize the message
   * - Assign possible out parameters of the function back to the local objects.
   * \endinternal
   */
  template <class ReturnType, typename TaskEnumType, typename... ArgumentTypes,
            std::enable_if_t<std::is_void<ReturnType>::value, std::int32_t> = 0>
  auto CallRemoteFunctionNoException(TaskEnumType task_id, ArgumentTypes&... args) const noexcept -> ReturnType {
    amsr::sec::ipc::BasicMessage<TaskEnumType, ArgumentTypes&...> msg{
        message_builder_.BuildInvocationMessage<TaskEnumType, ArgumentTypes&...>(
            task_id, proxy_id_, std::forward<ArgumentTypes&>(args)...)};
    // Acquire mutex for thread safe transporter access
    std::lock_guard<std::recursive_mutex> const lock{transporter_.GetMutex()};
    // VECTOR NL AutosarC++17_10-M8.5.2: MD_CRYPTO_AutosarC++17_10-M8.5.2_template_type
    ara::crypto::WritableMemRegion const send_buffer{transporter_.GetMessageSendBuffer()};
    // VECTOR NL AutosarC++17_10-M8.5.2: MD_CRYPTO_AutosarC++17_10-M8.5.2_template_type
    amsr::core::Result<ara::crypto::ReadOnlyMemRegion> const message_result{
        message_serializer_.SerializeMessage<TaskEnumType, ArgumentTypes&...>(msg, send_buffer)};
    if (!message_result.HasValue()) {
      logger_.LogFatalAndAbort(AMSR_FILE, __LINE__, "Failed to serialize message."_sv);
    }

    // VECTOR NL AutosarC++17_10-M8.5.2: MD_CRYPTO_AutosarC++17_10-M8.5.2_template_type
    amsr::core::Result<ara::crypto::ReadOnlyMemRegion> send_and_receive_result{
        transporter_.SendAndReceiveSync(message_result.Value())};

    if (!send_and_receive_result.HasValue()) {
      logger_.LogFatalAndAbort(AMSR_FILE, __LINE__, "Failed to send and receive."_sv);
    }

    ara::crypto::ReadOnlyMemRegion const& return_data{send_and_receive_result.Value()};
    // For the return message: pointers to identifiables are not replaced, because the proxy object does not change.
    amsr::sec::ipc::MessageObjectLifetimeManager object_lifetime_manager{};
    amsr::sec::ipc::BasicMessage<amsr::sec::ipc::Task, remove_reference_and_replace_identifiables<ArgumentTypes>...>
        return_msg{
            message_deserializer_
                .DeserializeMessage<amsr::sec::ipc::Task, remove_reference_and_replace_identifiables<ArgumentTypes>...>(
                    return_data, object_lifetime_manager)};

    message_error_handler_.HandleErrorMessage(return_msg.BasicTask(), return_msg.DetailTask());

    // Message contains no return value, offset for out parameter assignment is zero
    std::tuple<remove_reference_and_replace_identifiables<ArgumentTypes>...>& return_msg_content{return_msg.Content()};
    std::tuple<ArgumentTypes&...> msg_content{msg.Content()};
    out_param_updater_.AssignAllOutParameters<0>(return_msg_content, msg_content);
  }

  /*!
   * \brief This overload is used for functions that return proxy objects. They need to send an object id and handle
   *        a return code that indicates proper registration of the new skeleton object
   * \copydoc CallRemoteFunctionNoException
   * \param[in] provider_reference Reference to the crypto provider proxy which owns this proxy
   * \vprivate Component Private
   */
  template <class ReturnType, typename TaskEnumType, typename... ArgumentTypes,
            std::enable_if_t<is_proxy<ReturnType>::value, std::int32_t> = 0>
  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  auto CallRemoteFunctionNoException(ara::crypto::cryp::CryptoProvider& provider_reference, TaskEnumType task_id,
                                     ArgumentTypes&... args) const noexcept -> ReturnType {
    // VECTOR NC AutosarC++17_10-A2.8.3: MD_CRYPTO_AutosarC++17_10-A2.8.3_false_positive
    // VECTOR NL AutosarC++17_10-A2.11.2: MD_CRYPTO_AutosarC++17_10-A2.11.2_using_name_not_unique
    using ProxyObjectType = crypto::common::raw_class_name<ReturnType>;
    ReturnType proxy_ptr{ProxyObjectType::Create(provider_reference, transporter_, message_builder_,
                                                 message_serializer_, message_deserializer_)};
    // Acquire mutex for thread safe transporter access
    std::lock_guard<std::recursive_mutex> const lock{transporter_.GetMutex()};
    return CallRemoteFunctionProxyInternal(std::move(proxy_ptr), task_id, args...);
  }

  /*!
   * \brief This overload is used for functions that return proxy objects. They need to send an object id and handle
   *        a return code that indicates proper registration of the new skeleton object
   * \copydoc CallRemoteFunctionNoException
   * \vprivate Component Private
   */
  template <class ReturnType, typename TaskEnumType, typename... ArgumentTypes,
            std::enable_if_t<is_proxy<ReturnType>::value, std::int32_t> = 0>
  auto CallRemoteFunctionNoException(TaskEnumType task_id, ArgumentTypes&... args) const noexcept -> ReturnType {
    // VECTOR NC AutosarC++17_10-A2.8.3: MD_CRYPTO_AutosarC++17_10-A2.8.3_false_positive
    // VECTOR NL AutosarC++17_10-A2.11.2: MD_CRYPTO_AutosarC++17_10-A2.11.2_using_name_not_unique
    using ProxyType = crypto::common::raw_class_name<ReturnType>;
    ReturnType proxy_ptr{ProxyType::Create(transporter_, message_builder_, message_serializer_, message_deserializer_)};
    // Acquire mutex for thread safe transporter access
    std::lock_guard<std::recursive_mutex> const lock{transporter_.GetMutex()};
    return CallRemoteFunctionProxyInternal(std::move(proxy_ptr), task_id, args...);
  }

  /*!
   * \brief This overload is used for functions that return Results types of proxy objects.
   * They need to send an object id and handle a return code that indicates proper registration of the new skeleton
   * object .
   * \copydoc CallRemoteFunctionNoException
   * \param[in] provider_reference Reference to the crypto provider proxy which owns this proxy.
   * \vprivate Component Private
   */
  template <class ReturnType, typename TaskEnumType, typename... ArgumentTypes,
            std::enable_if_t<is_proxy<typename crypto::common::RemoveResult<ReturnType>::type>::value &&
                                 common::IsResult<ReturnType>::value,
                             std::int32_t> = 0>
  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  auto CallRemoteFunctionNoException(ara::crypto::cryp::CryptoProvider& provider_reference, TaskEnumType task_id,
                                     ArgumentTypes&... args) const noexcept -> ReturnType {
    // VECTOR NC AutosarC++17_10-A2.8.3: MD_CRYPTO_AutosarC++17_10-A2.8.3_false_positive
    // VECTOR NL AutosarC++17_10-A2.11.2: MD_CRYPTO_AutosarC++17_10-A2.11.2_using_name_not_unique
    using ProxyType = crypto::common::raw_class_name<typename crypto::common::RemoveResult<ReturnType>::type>;
    ReturnType result{ProxyType::Create(provider_reference, transporter_, message_builder_, message_serializer_,
                                        message_deserializer_)};
    // Acquire mutex for thread safe transporter access
    std::lock_guard<std::recursive_mutex> const lock{transporter_.GetMutex()};
    return CallRemoteFunctionProxyInternal(std::move(result), task_id, args...);
  }

  /*!
   * \brief This overload is used for functions that return Results types of proxy objects.
   * They need to send an object id and handle a return code that indicates proper registration of the new skeleton
   * object
   * \copydoc CallRemoteFunctionNoException
   * \vprivate Component Private
   */
  template <class ReturnType, typename TaskEnumType, typename... ArgumentTypes,
            std::enable_if_t<is_proxy<typename crypto::common::RemoveResult<ReturnType>::type>::value &&
                                 common::IsResult<ReturnType>::value,
                             std::int32_t> = 0>
  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  auto CallRemoteFunctionNoException(TaskEnumType task_id, ArgumentTypes&... args) const noexcept -> ReturnType {
    // VECTOR NC AutosarC++17_10-A2.8.3: MD_CRYPTO_AutosarC++17_10-A2.8.3_false_positive
    // VECTOR NL AutosarC++17_10-A2.11.2: MD_CRYPTO_AutosarC++17_10-A2.11.2_using_name_not_unique
    using ProxyType = crypto::common::raw_class_name<typename crypto::common::RemoveResult<ReturnType>::type>;
    ReturnType result{ProxyType::Create(transporter_, message_builder_, message_serializer_, message_deserializer_)};
    // Acquire mutex for thread safe transporter access
    std::lock_guard<std::recursive_mutex> const lock{transporter_.GetMutex()};
    return CallRemoteFunctionProxyInternal(std::move(result), task_id, args...);
  }

  /*!
   * \brief This overload is used for functions that return copyable objects which are non void and not proxy objects.
   * \copydoc CallRemoteFunctionNoException
   * \vprivate Component Private
   */
  template <class ReturnType, typename TaskEnumType, typename... ArgumentTypes,
            std::enable_if_t<((!is_proxy<ReturnType>::value) && (!std::is_void<ReturnType>::value) &&
                              (!is_proxy<typename crypto::common::RemoveResult<ReturnType>::type>::value)),
                             std::int32_t> = 0>
  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  auto CallRemoteFunctionNoException(TaskEnumType task_id, ArgumentTypes&... args) const noexcept -> ReturnType {
    amsr::sec::ipc::BasicMessage<TaskEnumType, ArgumentTypes&...> msg{
        message_builder_.BuildInvocationMessage<TaskEnumType, ArgumentTypes&...>(
            task_id, proxy_id_, std::forward<ArgumentTypes&>(args)...)};
    // Acquire mutex for thread safe transporter access
    std::lock_guard<std::recursive_mutex> const lock{transporter_.GetMutex()};
    // VECTOR NL AutosarC++17_10-M8.5.2: MD_CRYPTO_AutosarC++17_10-M8.5.2_template_type
    ara::crypto::WritableMemRegion const send_buffer{transporter_.GetMessageSendBuffer()};
    // VECTOR NL AutosarC++17_10-M8.5.2: MD_CRYPTO_AutosarC++17_10-M8.5.2_template_type
    amsr::core::Result<ara::crypto::ReadOnlyMemRegion> const send_msg_result{
        message_serializer_.SerializeMessage<TaskEnumType, ArgumentTypes&...>(msg, send_buffer)};
    if (!send_msg_result.HasValue()) {
      logger_.LogFatalAndAbort(AMSR_FILE, __LINE__, "Failed to serialize message."_sv);
    }
    // VECTOR NL AutosarC++17_10-M8.5.2: MD_CRYPTO_AutosarC++17_10-M8.5.2_template_type
    amsr::core::Result<ara::crypto::ReadOnlyMemRegion> send_and_receive_result{
        transporter_.SendAndReceiveSync(send_msg_result.Value())};

    if (!send_and_receive_result.HasValue()) {
      logger_.LogFatalAndAbort(AMSR_FILE, __LINE__, "Failed to send and receive."_sv);
    }

    ara::crypto::ReadOnlyMemRegion const& return_data{send_and_receive_result.Value()};
    amsr::sec::ipc::MessageObjectLifetimeManager object_lifetime_manager{};
    amsr::sec::ipc::BasicMessage<amsr::sec::ipc::Task, ReturnType,
                                 remove_reference_and_replace_identifiables<ArgumentTypes>...>
        return_msg{
            message_deserializer_.DeserializeMessage<amsr::sec::ipc::Task, ReturnType,
                                                     remove_reference_and_replace_identifiables<ArgumentTypes>...>(
                return_data, object_lifetime_manager)};

    message_error_handler_.HandleErrorMessage(return_msg.BasicTask(), return_msg.DetailTask());

    // Message contains return value, offset for out parameter assignment is one
    std::tuple<ReturnType, remove_reference_and_replace_identifiables<ArgumentTypes>...> return_msg_content{
        return_msg.Content()};
    std::tuple<ArgumentTypes&...> msg_content{msg.Content()};
    out_param_updater_.AssignAllOutParameters<1>(return_msg_content, msg_content);
    ReturnType return_value{std::get<0>(return_msg.Content())};

    return return_value;
  }

  /*!
   * \brief This overload is used for functions that have a vector of proxy objects as an out parameter.
   * \remark This function only works when the out parameter is the first argument to the actual function call.
   * Also this function is only intended to be used for X509Provider::ParseCertChain and
   * X509Provider::FindCertByDnWildcard.
   * \copydoc CallRemoteFunctionNoException
   * \param[in] provider_reference Reference to the crypto provider proxy which owns this proxy.
   * \param[in] input_vector Type of vector which is used as out parameter.
   * \tparam VectorType Type of vector which is used as out parameter.
   * \vprivate Component Private
   * \internal
   * - Fill the out parameter vector with possible new object ids bounded by kMaxNumberOfProxies.
   * - Create an empty dummy vector and use it to build an invocation message.
   * - Send message (call remote function).
   * - Deserialize the message returned by the server and handle possible errors.
   * - Assing out parameters.
   * - Special treatment for the vector:
   *   - The vector returned by the server side contains only nullptr, but the size indicates how many objects were
   * created.
   *   - Assert that not more objects were created on the server side than are available.
   *   (This should already have been checked on the server side.)
   *   - Resize the out vector to match the amount of objects returned by the server.
   * - Return the actual return value.
   * \endinternal
   */
  template <class ReturnType, typename TaskEnumType, typename VectorType, typename... ArgumentTypes,
            std::enable_if_t<((!is_proxy<ReturnType>::value) && (!std::is_void<ReturnType>::value) &&
                              (!is_proxy<typename crypto::common::RemoveResult<ReturnType>::type>::value) &&
                              (common::IsVector<VectorType>::value) &&
                              (is_proxy<typename common::RemoveVector<VectorType>::type>::value)),
                             std::int32_t> = 0>
  auto CallRemoteFunctionNoException(ara::crypto::cryp::CryptoProvider& provider_reference, TaskEnumType task_id,
                                     VectorType& input_vector, ArgumentTypes&... args) const noexcept -> ReturnType {
    // VECTOR NC AutosarC++17_10-A2.8.3: MD_CRYPTO_AutosarC++17_10-A2.8.3_false_positive
    // VECTOR NL AutosarC++17_10-A2.11.2: MD_CRYPTO_AutosarC++17_10-A2.11.2_using_name_not_unique
    using ProxyType = typename common::raw_class_name<typename common::RemoveVector<VectorType>::type>;
    // Vector to hold the created proxy objects. It will be released once it is pushed to the input vector.
    ara::core::Vector<std::unique_ptr<ProxyType, ara::crypto::CustomDeleter>> tmp_vector{};
    // We need to add an empty dummy vector because the server side expects to find a vector in the argument list.
    VectorType empty_dummy_vector{};
    amsr::sec::ipc::BasicMessage<TaskEnumType, VectorType&, ArgumentTypes&...> msg{
        message_builder_.BuildInvocationMessage<TaskEnumType, VectorType&, ArgumentTypes&...>(
            task_id, proxy_id_, empty_dummy_vector, std::forward<ArgumentTypes&>(args)...)};
    // VECTOR NL AutosarC++17_10-A6.5.1: MD_CRYPTO_AutosarC++17_10-A6.5.1_not_a_container_loop
    for (size_t i{0};
         i < amsr::sec::ipc::BasicMessage<TaskEnumType, VectorType&, ArgumentTypes&...>::kMaxNumberOfProxies; ++i) {
      // VECTOR NL AutosarC++17_10-A18.5.8: MD_CRYPTO_AutosarC++17_10-A18.5.8_alloc_on_stack
      std::unique_ptr<ProxyType, ara::crypto::CustomDeleter> uptr{ProxyType::Create(
          provider_reference, transporter_, message_builder_, message_serializer_, message_deserializer_)};
      msg.AddNewProxyId(amsr::sec::ipc::GetProxyId(uptr.get()));
      tmp_vector.emplace_back(std::move(uptr));
    }

    // Acquire mutex for thread safe transporter access
    std::lock_guard<std::recursive_mutex> const lock{transporter_.GetMutex()};
    // VECTOR NL AutosarC++17_10-M8.5.2: MD_CRYPTO_AutosarC++17_10-M8.5.2_template_type
    ara::crypto::WritableMemRegion const send_buffer{transporter_.GetMessageSendBuffer()};
    // VECTOR NL AutosarC++17_10-M8.5.2: MD_CRYPTO_AutosarC++17_10-M8.5.2_template_type
    amsr::core::Result<ara::crypto::ReadOnlyMemRegion> send_msg_result{
        message_serializer_.SerializeMessage<TaskEnumType, VectorType&, ArgumentTypes&...>(msg, send_buffer)};

    if (!send_msg_result.HasValue()) {
      logger_.LogFatalAndAbort(AMSR_FILE, __LINE__, "Failed to serialize message."_sv);
    }

    // VECTOR NL AutosarC++17_10-M8.5.2: MD_CRYPTO_AutosarC++17_10-M8.5.2_template_type
    amsr::core::Result<ara::crypto::ReadOnlyMemRegion> send_and_receive_result{
        transporter_.SendAndReceiveSync(send_msg_result.Value())};

    if (!send_and_receive_result.HasValue()) {
      logger_.LogFatalAndAbort(AMSR_FILE, __LINE__, "Failed to send and receive."_sv);
    }

    ara::crypto::ReadOnlyMemRegion const& return_data{send_and_receive_result.Value()};
    amsr::sec::ipc::MessageObjectLifetimeManager object_lifetime_manager{};
    amsr::sec::ipc::BasicMessage<amsr::sec::ipc::Task, ReturnType, VectorType,
                                 remove_reference_and_replace_identifiables<ArgumentTypes>...>
        return_msg{
            message_deserializer_.DeserializeMessage<amsr::sec::ipc::Task, ReturnType, VectorType,
                                                     remove_reference_and_replace_identifiables<ArgumentTypes>...>(
                return_data, object_lifetime_manager)};

    message_error_handler_.HandleErrorMessage(return_msg.BasicTask(), return_msg.DetailTask());

    // Message contains return value, offset for out parameter assignment is one
    std::tuple<ReturnType, VectorType, remove_reference_and_replace_identifiables<ArgumentTypes>...> return_msg_content{
        return_msg.Content()};
    std::tuple<VectorType&, ArgumentTypes&...> msg_content{msg.Content()};
    out_param_updater_.AssignAllOutParameters<1>(return_msg_content, msg_content);

    std::uint64_t const result_size{std::min(tmp_vector.size(), std::get<1>(return_msg_content).size())};
    for (std::uint64_t i{0}; i < result_size; ++i) {
      input_vector.emplace_back(tmp_vector[i].release());
    }
    ReturnType const return_value{std::get<0>(return_msg_content)};

    return return_value;
  }

  /*!
   * \brief Internal template method for sending a request for calling a remote function to the server side,
   *        that returns a unique_ptr<> that cannot be copied. Therefore it generates a message, serializes it
   *        and sends it. After a response was received, this is extracted and returned.
   *        This call is BLOCKING until the response from the transporter interface is received.
   * \param[in] ptr Pointer of the proxy object to return.
   * \param[in] task_id TaskId of the action to be performed.
   * \param[in] args Argument list to be passed to the server.
   * \return unique_ptr pointing to an object that has been returned. Note: This object is likely also a proxy object.
   * \vprivate Component Private
   * \internal
   * - Build a BasicMessage object containing the function to be called and all involved objects / parameters.
   * - Serialize this message and send it using the \c transporter_.
   * - Wait for the return message and abort on possible server side exceptions
   * - Deserialize the message
   * - Assign possible out parameters of the function back to the local objects.
   * \endinternal
   */
  template <typename ReturnType, typename TaskEnumType, typename... ArgumentTypes,
            std::enable_if_t<!common::IsResult<ReturnType>::value, std::int32_t> = 0>
  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  ReturnType CallRemoteFunctionProxyInternal(ReturnType ptr, TaskEnumType task_id,
                                             ArgumentTypes&... args) const noexcept {
    amsr::sec::ipc::ProxyId const new_proxy_id{amsr::sec::ipc::GetProxyId(ptr.get())};
    amsr::sec::ipc::BasicMessage<TaskEnumType, ArgumentTypes&...> msg{
        message_builder_.BuildInvocationMessage<TaskEnumType, ArgumentTypes&...>(task_id, proxy_id_, args...)};
    msg.AddNewProxyId(new_proxy_id);

    // VECTOR NL AutosarC++17_10-M8.5.2: MD_CRYPTO_AutosarC++17_10-M8.5.2_template_type
    ara::crypto::WritableMemRegion const send_buffer{transporter_.GetMessageSendBuffer()};
    // VECTOR NL AutosarC++17_10-M8.5.2: MD_CRYPTO_AutosarC++17_10-M8.5.2_template_type
    amsr::core::Result<ara::crypto::ReadOnlyMemRegion> const send_msg_result{
        message_serializer_.SerializeMessage<TaskEnumType, ArgumentTypes&...>(msg, send_buffer)};

    if (!send_msg_result.HasValue()) {
      logger_.LogFatalAndAbort(AMSR_FILE, __LINE__, "Failed to serialize message."_sv);
    }

    // VECTOR NL AutosarC++17_10-M8.5.2: MD_CRYPTO_AutosarC++17_10-M8.5.2_template_type
    amsr::core::Result<ara::crypto::ReadOnlyMemRegion> send_and_receive_result{
        transporter_.SendAndReceiveSync(send_msg_result.Value())};

    if (!send_and_receive_result.HasValue()) {
      logger_.LogFatalAndAbort(AMSR_FILE, __LINE__, "Failed to send and receive."_sv);
    }

    ara::crypto::ReadOnlyMemRegion const& return_data{send_and_receive_result.Value()};
    logger_.LogVerbose(AMSR_FILE, __LINE__) << "Received response from server.";
    amsr::sec::ipc::MessageObjectLifetimeManager object_lifetime_manager{};
    amsr::sec::ipc::BasicMessage<amsr::sec::ipc::Task, amsr::sec::ipc::SerializationTags,
                                 remove_reference_and_replace_identifiables<ArgumentTypes>...>
        return_msg{
            message_deserializer_.DeserializeMessage<amsr::sec::ipc::Task, amsr::sec::ipc::SerializationTags,
                                                     remove_reference_and_replace_identifiables<ArgumentTypes>...>(
                return_data, object_lifetime_manager)};

    message_error_handler_.HandleErrorMessage(return_msg.BasicTask(), return_msg.DetailTask());

    // Message contains return value, offset for out parameter assignment is one
    std::tuple<amsr::sec::ipc::SerializationTags, remove_reference_and_replace_identifiables<ArgumentTypes>...>&
        return_msg_content{return_msg.Content()};
    std::tuple<ArgumentTypes&...> msg_content{msg.Content()};
    out_param_updater_.AssignAllOutParameters<1>(return_msg_content, msg_content);
    amsr::sec::ipc::SerializationTags const tag{std::get<0>(return_msg.Content())};
    ReturnType return_value{nullptr};
    if (tag == amsr::sec::ipc::SerializationTags::tag_skeleton_created) {
      static_cast<BaseProxy const*>(ptr.get())->send_destroy_message_ = true;
      return_value = std::move(ptr);
    } else if (tag == amsr::sec::ipc::SerializationTags::tag_skeleton_not_created) {
      static_cast<BaseProxy const*>(ptr.get())->send_destroy_message_ = false;
    } else {
      logger_.LogError(AMSR_FILE, __LINE__)
          << "Invalid Serialization tag received from server. Returning default value.";
    }

    return return_value;
  }

  /*!
   * \brief Internal template method for sending a request for calling a remote function to the server side,
   *        that returns a amsr::core::Result<unique_ptr<>>.
   * \param[in] input_result Pointer of the proxy object to return.
   * \param[in] task_id TaskId of the action to be performed.
   * \param[in] args Argument list to be passed to the server.
   * \return unique_ptr pointing to an object that has been returned. Note: This object is likely also a proxy object.
   * \error Error occurred on server side implementation.
   * \details This call is BLOCKING until the response from the transporter interface is received.
   * \context ANY
   * \pre \c input_result contains a valid unique pointer.
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * * \internal
   * - Build a BasicMessage object containing the function to be called and all involved objects / parameters.
   * - Serialize this message and send it using the \c transporter_.
   * - Wait for the return message and abort on possible server side exceptions
   * - Deserialize the message
   * - Assign possible out parameters of the function back to the local objects.
   * \endinternal
   */
  template <typename ReturnType, typename TaskEnumType, typename... ArgumentTypes,
            std::enable_if_t<common::IsResult<ReturnType>::value, std::int32_t> = 0>
  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  ReturnType CallRemoteFunctionProxyInternal(ReturnType input_result, TaskEnumType task_id,
                                             ArgumentTypes&... args) const noexcept {
    crypto::common::util::Contract::Precondition(
        AMSR_FILE, __LINE__, input_result.HasValue(),
        "Call RemoteFunctionProxyInternal called without a unique_ptr in input_result."_sv);
    crypto::common::util::Contract::Precondition(
        AMSR_FILE, __LINE__, input_result.Value() != nullptr,
        "Call RemoteFunctionProxyInternal called with nullptr in input_result."_sv);
    amsr::sec::ipc::BasicMessage<TaskEnumType, ArgumentTypes&...> msg{
        message_builder_.BuildInvocationMessage<TaskEnumType, ArgumentTypes&...>(task_id, proxy_id_, args...)};
    amsr::sec::ipc::ProxyId const new_proxy_id{amsr::sec::ipc::GetProxyId(input_result.Value().get())};
    msg.AddNewProxyId(new_proxy_id);

    // VECTOR NL AutosarC++17_10-M8.5.2: MD_CRYPTO_AutosarC++17_10-M8.5.2_template_type
    ara::crypto::WritableMemRegion const send_buffer{transporter_.GetMessageSendBuffer()};
    // VECTOR NL AutosarC++17_10-M8.5.2: MD_CRYPTO_AutosarC++17_10-M8.5.2_template_type
    amsr::core::Result<ara::crypto::ReadOnlyMemRegion> const send_msg_result{
        message_serializer_.SerializeMessage<TaskEnumType, ArgumentTypes&...>(msg, send_buffer)};
    if (!send_msg_result.HasValue()) {
      logger_.LogFatalAndAbort(AMSR_FILE, __LINE__, "Failed to serialize message."_sv);
    }

    // VECTOR NL AutosarC++17_10-M8.5.2: MD_CRYPTO_AutosarC++17_10-M8.5.2_template_type
    amsr::core::Result<ara::crypto::ReadOnlyMemRegion> send_and_receive_result{
        transporter_.SendAndReceiveSync(send_msg_result.Value())};

    if (!send_and_receive_result.HasValue()) {
      logger_.LogFatalAndAbort(AMSR_FILE, __LINE__, "Failed to send and receive."_sv);
    }

    ara::crypto::ReadOnlyMemRegion const& return_data{send_and_receive_result.Value()};
    logger_.LogVerbose(AMSR_FILE, __LINE__) << "Received response from server.";
    amsr::sec::ipc::MessageObjectLifetimeManager object_lifetime_manager{};
    amsr::sec::ipc::BasicMessage<amsr::sec::ipc::Task, amsr::core::Result<amsr::sec::ipc::IdentifiablePointer>,
                                 remove_reference_and_replace_identifiables<ArgumentTypes>...>
        return_msg{
            message_deserializer_
                .DeserializeMessage<amsr::sec::ipc::Task, amsr::core::Result<amsr::sec::ipc::IdentifiablePointer>,
                                    remove_reference_and_replace_identifiables<ArgumentTypes>...>(
                    return_data, object_lifetime_manager)};

    message_error_handler_.HandleErrorMessage(return_msg.BasicTask(), return_msg.DetailTask());

    // Message contains return value, offset for out parameter assignment is one
    std::tuple<amsr::core::Result<amsr::sec::ipc::IdentifiablePointer>,
               remove_reference_and_replace_identifiables<ArgumentTypes>...>& return_msg_content{return_msg.Content()};
    std::tuple<ArgumentTypes&...> msg_content{msg.Content()};
    out_param_updater_.AssignAllOutParameters<1>(return_msg_content, msg_content);
    amsr::core::Result<amsr::sec::ipc::IdentifiablePointer> raw_pointer_result{std::get<0>(return_msg.Content())};
    ReturnType result{nullptr};
    if (raw_pointer_result.HasValue()) {
      if (raw_pointer_result.Value().is_nullptr) {
        result.EmplaceValue(nullptr);
        static_cast<BaseProxy const*>(input_result.Value().get())->send_destroy_message_ = false;
      } else {
        static_cast<BaseProxy const*>(input_result.Value().get())->send_destroy_message_ = true;
        result.EmplaceValue(std::move(input_result.Value()));
      }
    } else {
      static_cast<BaseProxy const*>(input_result.Value().get())->send_destroy_message_ = false;
      result.EmplaceError(std::get<0>(return_msg.Content()).Error());
    }
    return result;
  }

  /*!
   * \brief Template method for sending a request for calling a remote function to the server side, that returns a
   *        copyable return type (= no unique pointer). Therefore it generates a message, serializes it and sends it.
   *        After a response was received, this is extracted and returned.
   *        If length required to serialize ReadOnlyMemRegion is larger than
   *        kTransportBufferSize, data is chunked and iteratively called.
   *        This call is BLOCKING until the response from the transporter interface is received.
   * \param[in] task_id TaskId of the action to be performed.
   * \param[in] in Memory to be deserialized and used for function calls.
   * \return Return value received from server side.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   * \internal
   * - Build a BasicMessage object containing the function to be called and all involved objects / parameters.
   * - Serialize this message and check its length:
   *   - If it exceeds the transport buffer size, send it in chunks using the \c transporter_.
   *   - If it fits the transport buffer size, send it in one piece using the \c transporter_.
   * - Wait for the return message and abort on possible server side exceptions
   * - Deserialize the message
   * - Assign possible out parameters of the function back to the local objects.
   * \endinternal
   */
  template <class ReturnType, typename TaskEnumType>
  auto CallRemoteFunctionWithChunking(TaskEnumType task_id, ara::crypto::ReadOnlyMemRegion& in) const noexcept
      -> ReturnType {
    ReturnType result{};
    amsr::sec::ipc::BasicMessage<TaskEnumType, ara::crypto::ReadOnlyMemRegion&> const msg{
        message_builder_.BuildInvocationMessage<TaskEnumType, ara::crypto::ReadOnlyMemRegion&>(task_id, proxy_id_, in)};

    // VECTOR NL AutosarC++17_10-M8.5.2: MD_CRYPTO_AutosarC++17_10-M8.5.2_template_type
    std::tuple<std::size_t, std::size_t> const length_details{
        message_serializer_.GetSerializedMessageBufferLength(msg)};

    std::size_t const minimum_length{std::get<0>(length_details)};
    std::size_t const msg_overhead{std::get<1>(length_details)};

    // If minimum length to fully hold the memory region as a deserialized buffer
    // greater than transport buffer size, chunk it and send it
    if (minimum_length > amsr::sec::ipc::kTransportBufferRequestSize) {
      ara::crypto::ReadOnlyMemRegion::size_type bytes_remaining{in.size()};
      ara::crypto::ReadOnlyMemRegion::size_type current_offset{0};
      ara::crypto::ReadOnlyMemRegion::size_type const max_buffer_size{
          static_cast<ara::crypto::ReadOnlyMemRegion::size_type>(amsr::sec::ipc::kTransportBufferRequestSize -
                                                                 msg_overhead)};
      while (bytes_remaining != 0) {
        // VECTOR NL AutosarC++17_10-M8.5.2: MD_CRYPTO_AutosarC++17_10-M8.5.2_template_type
        ara::crypto::ReadOnlyMemRegion current_region{in.subspan(
            current_offset, std::min<ara::crypto::ReadOnlyMemRegion::size_type>(bytes_remaining, max_buffer_size))};
        result = CallRemoteFunctionNoException<ReturnType>(task_id, current_region);
        current_offset += std::min<ara::crypto::ReadOnlyMemRegion::size_type>(bytes_remaining, max_buffer_size);
        bytes_remaining -= std::min<ara::crypto::ReadOnlyMemRegion::size_type>(bytes_remaining, max_buffer_size);
      }
    } else {
      result = CallRemoteFunctionNoException<ReturnType>(task_id, in);
    }
    return result;
  }

  /*!
   * \brief Template method for sending a request for calling a remote function to the server side, that returns a
   *        copyable return type (= no unique pointer). Therefore it generates a message, serializes it and sends it.
   *        After a response was received, this is extracted and returned.
   *        If length required to serialize ReadOnlyMemRegion is larger than
   *        kTransportBufferSize, data is chunked and iteratively called.
   *        This call is BLOCKING until the response from the transporter interface is received.
   * \param[in] task_id TaskId of the action to be performed
   * \param[in] out Memory to be deserialized and used for function calls
   * \return Return value received from server side
   * \vprivate Component Private
   * \internal
   * - Build a BasicMessage object containing the function to be called and all involved objects / parameters.
   * - Serialize this message and check its length:
   *   - If it exceeds the transport buffer size, send it in chunks using the \c transporter_.
   *   - If it fits the transport buffer size, send it in one piece using the \c transporter_.
   * - Wait for the return message and abort on possible server side exceptions
   * - Deserialize the message
   * - Assign possible out parameters of the function back to the local objects.
   * \endinternal
   */
  template <class ReturnType, typename TaskEnumType>
  auto CallRemoteFunctionWithChunking(TaskEnumType task_id, ara::crypto::WritableMemRegion& out) const noexcept
      -> ReturnType {
    ReturnType result{};
    amsr::sec::ipc::BasicMessage<TaskEnumType, ara::crypto::WritableMemRegion&> const msg{
        message_builder_.BuildInvocationMessage<TaskEnumType, ara::crypto::WritableMemRegion&>(task_id, proxy_id_,
                                                                                               out)};

    // VECTOR NL AutosarC++17_10-M8.5.2: MD_CRYPTO_AutosarC++17_10-M8.5.2_template_type
    std::tuple<std::size_t, std::size_t> const length_details{
        message_serializer_.GetSerializedMessageBufferLength(msg)};

    std::size_t const minimum_length{std::get<0>(length_details)};
    std::size_t const msg_overhead{std::get<1>(length_details)};

    // If minimum length to fully hold the memory region as a deserialized buffer
    // greater than transport buffer size, chunk it and send it
    if (minimum_length > amsr::sec::ipc::kTransportBufferRequestSize) {
      ara::crypto::WritableMemRegion::size_type bytes_remaining{out.size()};
      ara::crypto::WritableMemRegion::size_type current_offset{0};
      ara::crypto::WritableMemRegion::size_type const max_buffer_size{
          static_cast<ara::crypto::WritableMemRegion::size_type>(amsr::sec::ipc::kTransportBufferRequestSize -
                                                                 msg_overhead)};
      while (bytes_remaining != 0) {
        // VECTOR NL AutosarC++17_10-M8.5.2: MD_CRYPTO_AutosarC++17_10-M8.5.2_template_type
        ara::crypto::WritableMemRegion current_region{out.subspan(
            current_offset, std::min<ara::crypto::WritableMemRegion::size_type>(bytes_remaining, max_buffer_size))};
        result = CallRemoteFunctionNoException<ReturnType>(task_id, current_region);
        current_offset += std::min<ara::crypto::WritableMemRegion::size_type>(bytes_remaining, max_buffer_size);
        bytes_remaining -= std::min<ara::crypto::WritableMemRegion::size_type>(bytes_remaining, max_buffer_size);
      }
    } else {
      result = CallRemoteFunctionNoException<ReturnType>(task_id, out);
    }
    return result;
  }

 protected:
  /*!
   * \brief Template to cast result-wrapped unique pointers to proxy objects to result wrapped unique pointers to ara::
   * objects.
   * \tparam ReturnType Object type in ara::crypto corresponding to \c ProxyType.
   * \tparam ProxyType Type of the (input) proxy object.
   * \param[in] proxy_result Input result containing the proxy object (e.g. returned by a call to
   * CallRemoteFunction).
   * \return A result containing the casted unique pointer.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename ReturnType, typename ProxyType>
  static auto ResultUniquePointerCast(
      amsr::core::Result<std::unique_ptr<ProxyType, ara::crypto::CustomDeleter>>&& proxy_result) noexcept
      -> amsr::core::Result<std::unique_ptr<ReturnType, ara::crypto::CustomDeleter>> {
    amsr::core::Result<std::unique_ptr<ReturnType, ara::crypto::CustomDeleter>> return_result{nullptr};
    if (proxy_result.HasValue()) {
      return_result.EmplaceValue(static_cast<ReturnType*>(proxy_result.Value().release()));
    } else {
      return_result.EmplaceError(proxy_result.Error());
    }
    return return_result;
  }

  /*!
   * \brief Template to cast result-wrapped unique pointers to proxy objects to result wrapped shared pointers to ara::
   * objects.
   * \tparam ReturnType Object type in ara::crypto corresponding to \c ProxyType.
   * \tparam ProxyType Type of the
   * (input) proxy object.
   * \param[in] proxy_result Input result containing the proxy object (e.g. returned by a call to
   * CallRemoteFunction).
   * \return A result containing the casted shared pointer.
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename ReturnType, typename ProxyType>
  static auto ResultSharedPointerCast(amsr::core::Result<std::shared_ptr<ProxyType>>&& proxy_result) noexcept
      -> amsr::core::Result<std::shared_ptr<ReturnType>> {
    amsr::core::Result<std::shared_ptr<ReturnType>> return_result{nullptr};
    if (proxy_result.HasValue()) {
      return_result.EmplaceValue(std::static_pointer_cast<ReturnType>(proxy_result.Value()));
    } else {
      return_result.EmplaceError(proxy_result.Error());
    }
    return return_result;
  }

 private:
  crypto::common::util::Logger logger_;                                   /*!< Logging instance */
  amsr::generic::SingletonAccess<SessionHandler> session_handler_access_; /*!< Session handler singleton access */
  amsr::sec::ipc::Transporter& transporter_;                  /*!< Transporter for server/client communication */
  amsr::sec::ipc::MessageBuilder& message_builder_;           /*!< Message builder for generating messages */
  amsr::sec::ipc::MessageSerializer& message_serializer_;     /*!< Serializer for serialization of request messages */
  amsr::sec::ipc::MessageDeserializer& message_deserializer_; /*!< Deserializer for deserialization of responses */
  crypto::client::deletion::ProxyDeletionHandler deletion_handler_; /*!< Handler called on deletion of this proxy */
  amsr::sec::ipc::ProxyId proxy_id_;                                /*!< ProxyId of this object sent in requests */
  crypto::client::MessageErrorHandler
      message_error_handler_;                         /*!< Error handler that logs and aborts for IPC errorcode */
  crypto::client::OutParamUpdater out_param_updater_; /*!< Updater for out parameters of functions from message */

  // VECTOR NC AutosarC++17_10-A12.1.3: MD_CRYPTO_AutosarC++17_10-A12.1.3
  // session handler owns transporter, so it will always fail to send the destroy message
  mutable bool send_destroy_message_; /*!< Whether a destroy message to the server shall be sent on deletion */
};

}  // namespace client
}  // namespace crypto

#endif  // LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_BASE_PROXY_H_
