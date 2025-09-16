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
/*!        \file  message_builder.h
 *        \brief  Wrapper class to generate messages for various various argument types. Wraps instantiation and
 *                initialization of message content.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCOMMON_INCLUDE_AMSR_SEC_IPC_MESSAGE_BUILDER_H_
#define LIB_LIBCOMMON_INCLUDE_AMSR_SEC_IPC_MESSAGE_BUILDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <memory>
#include <tuple>
#include <utility>

#include "amsr/sec/ipc/basic_message.h"
#include "amsr/sec/ipc/protocol.h"

namespace amsr {
namespace sec {
namespace ipc {

/*!
 * \brief Wrapper class to generate messages for various various argument types.
 * \details Allows building messages for IPC by providing several functions for creating \c
 * BasicMessage objects (see basic_message.h for details).
 * \vprivate Component Private
 */
class MessageBuilder final {
 public:
  /*!
   * \brief Default constructor
   * \vprivate Component Private
   */
  MessageBuilder() = default;

  /*!
   * \brief Default Destructor
   * \vprivate Component Private
   */
  ~MessageBuilder() noexcept = default;

  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  MessageBuilder(MessageBuilder const& /*other*/) = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  MessageBuilder(MessageBuilder&& /*other*/) noexcept = default;

  /*!
   * \brief Default copy assignment operator
   * \return reference to this
   * \vprivate Component Private
   */
  MessageBuilder& operator=(MessageBuilder const& /*other*/) & = default;

  /*!
   * \brief Default move assignment operator
   * \return reference to this
   * \vprivate Component Private
   */
  MessageBuilder& operator=(MessageBuilder&& /*other*/) & noexcept = default;

  /*!
   * \brief Method for creating an instance of the message and initializing the content tuple using the arguments.
   * \param[in] task_id Task of the message to be performed by the recipient.
   * \param[in] proxy_id ProxyId of the calling object.
   * \param[in] args argument list containing the values for the content tuple.
   * \return Message containing TaskId, ObjectID, NewObjectID = 0 and a content tuple containing the given values.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  template <typename TaskEnumType, typename... ArgumentTypes>
  static auto BuildInvocationMessage(TaskEnumType task_id, ProxyId proxy_id, ArgumentTypes... args) noexcept
      -> BasicMessage<TaskEnumType, ArgumentTypes...> {
    BasicMessage<TaskEnumType, ArgumentTypes...> param_value{Task::Invocation, task_id, proxy_id, args...};
    return param_value;
  }

  /*!
   * \brief Method for creating a return message which does not contain any object id (not required).
   * \tparam ReturnValueType Return type of the method which will be added as first element in the message.
   * \tparam ArgumentTypes Template argument containing all argument types to be added to the message.
   * \param[in] args Instance pack containing the argument values.
   * \return Message containing the arguments and the return value.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename ReturnValueType, typename... ArgumentTypes,
            std::enable_if_t<std::is_void<ReturnValueType>::value, std::int32_t> = 0>
  static auto BuildReturnMessage(std::tuple<ArgumentTypes...> args) noexcept -> BasicMessage<Task, ArgumentTypes...> {
    BasicMessage<Task, ArgumentTypes...> param_value{Task::Return, Task::Return, args};
    return param_value;
  }

  /*!
   * \brief Method for creating a return message which does not contain any object id (not required).
   * \tparam ReturnValueType Return type of the method which will be added as first element in the message.
   * \tparam ArgumentTypes Template argument containing all argument types to be added to the message.
   * \param[in] return_value Instance containing the return value of type ReturnValueType.
   * \param[in] args Instance pack containing the argument values.
   * \return Message containing the arguments and the return value.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename ReturnValueType, typename... ArgumentTypes,
            std::enable_if_t<!std::is_void<ReturnValueType>::value, std::int32_t> = 0>
  static auto BuildReturnMessage(ReturnValueType return_value, std::tuple<ArgumentTypes...> args) noexcept
      -> BasicMessage<Task, ReturnValueType, ArgumentTypes...> {
    std::tuple<ReturnValueType, ArgumentTypes...> const result_tuple{
        // VECTOR NL AutosarC++17_10-A18.9.3: MD_CRYPTO_AutosarC++17_10-A18.9.3
        std::tuple_cat(std::tuple<ReturnValueType>(std::move(return_value)), args)};
    BasicMessage<Task, ReturnValueType, ArgumentTypes...> param_value{Task::Return, Task::Return, result_tuple};
    return param_value;
  }

  /*!
   * \brief Method for creating an instance of the message and initializing the content tuple using the arguments.
   * \param[in] handshake_step Handshake step to be sent.
   * \param[in] args argument list containing the values for the content tuple.
   * \return Message containing TaskId, ObjectID, NewObjectID = 0 and a content tuple containing the given values.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  template <typename... ArgumentTypes>
  static auto BuildHandshakeMessage(Handshake handshake_step, ArgumentTypes... args) noexcept
      -> BasicMessage<Handshake, ArgumentTypes...> {
    BasicMessage<Handshake, ArgumentTypes...> param_value{Task::Handshake, handshake_step, args...};

    return param_value;
  }

  /*!
   * \brief Method for creating a message to indicate a destructor call.
   * \param[in] proxy_id of the object to be destroyed.
   * \return Message with basic task Destroy used for destruction of objects on server side.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static BasicMessage<Task> BuildDestroyMessage(ProxyId proxy_id) noexcept {
    BasicMessage<Task> param_value{Task::Destroy, Task::Destroy, proxy_id};
    return param_value;
  }

  /*!
   * \brief Method for creating an error message to indicate an exception or error.
   * \param[in] error Error code for the result message.
   * \return Message containing the basic task "Error" and the provided error code as detail task.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  static BasicMessage<ara::crypto::SecurityErrc> BuildErrorMessage(ara::crypto::SecurityErrc error) noexcept {
    BasicMessage<ara::crypto::SecurityErrc> param_value{Task::Error, error};
    return param_value;
  }
};

}  // namespace ipc
}  // namespace sec
}  // namespace amsr

#endif  // LIB_LIBCOMMON_INCLUDE_AMSR_SEC_IPC_MESSAGE_BUILDER_H_
