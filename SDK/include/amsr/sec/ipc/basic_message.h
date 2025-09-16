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
/*!        \file  basic_message.h
 *        \brief  Wrapper object for request and response messages.
 *
 *      \details  BaseMessage is used to hold the data required for a single method call or return value.
 *                The message contains a Task which is used to identify the method to be called, an ProxyId of the
 *                calling object, which allows to identify the calling instance, a NewProxyId which is used to register
 *                the result object of the method call for later invocation and a tuple of all content data used as
 *                arguments for the method invocation.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCOMMON_INCLUDE_AMSR_SEC_IPC_BASIC_MESSAGE_H_
#define LIB_LIBCOMMON_INCLUDE_AMSR_SEC_IPC_BASIC_MESSAGE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <tuple>
#include <utility>

#include "amsr/core/array.h"
#include "amsr/core/span.h"
#include "amsr/sec/ipc/message_object_lifetime_manager.h"
#include "amsr/sec/ipc/protocol.h"
#include "ara/crypto/common/custom_disposable.h"
#include "ara/crypto/common/mem_region.h"
#include "crypto/common/identifiable.h"

namespace amsr {
namespace sec {
namespace ipc {

// VECTOR NC AutosarC++17_10-M5.2.9: MD_CRYPTO_AutosarC++17_10-M5.2.9_reinterpret_pointer
// VECTOR NC AutosarC++17_10-A5.2.4: MD_CRYPTO_AutosarC++17_10-A5.2.4_reinterpret_cast
/*!
 * \remark Since the API enforces the passing of pure virtual types no custom getters can be offered
 *         for an object specific identification. To omit this, the object's address is used as ID.
 *         Therefore a reinterpret_cast to std::uintptr_t is needed here. The ID shall not be
 *         reinterpreted again and will be used solely for identification purposes.
 * \param[in] ptr Pointer to identifiable object
 * \return ProxyId of the identifiable object.
 * \vprivate Component Private
 */
inline auto GetProxyId(::crypto::common::Identifiable const* ptr) noexcept -> ProxyId {
  return reinterpret_cast<ProxyId>(ptr);
}

/*!
 * \brief Class for messages which are used in the IPC.
 * \tparam TaskEnumType Enumeration of detail tasks of this BasicMessage
 * \tparam ArgumentTypes Tuple of argument types contained in this BasicMessage
 * \details BasicMessages are used for the communication of the CryptoStack daemon and its clients.
 * They are always composed of at least a \c basic_task_ and a \c detail_task_.
 * The \c basic_task describes what kind of message is transmitted (for details see protocol.h).
 * The \c detail_task gives more detailed information about the operation, e.g. in the case of a function invocation
 * message this contains the function name. Additionally an \c proxy_id_ may be passed that identifies the object on
 * which the operation (e.g. function invocation) is performed. If the function returns an object this object is
 * identified by \c new_proxy_id_.
 * \vprivate Component Private
 */
template <typename TaskEnumType, typename... ArgumentTypes>
class BasicMessage {
 public:
  /*! Maximum length of supported CertChains. Needed due to restrictions of the RPC framework */
  // VECTOR NL AutosarC++17_10-M3.4.1: MD_CRYPTO_AutosarC++17_10-M3.4.1_global
  static constexpr std::size_t const kMaxNumberOfProxies{32};

  /*!
   * \brief Constructs a message describing an operation on an object.
   * \param[in] basic_task Task of the message
   * \param[in] detail_task Method task of the message
   * \param[in] proxy_id ProxyId of the calling object
   * \param[in] args Argument values to be transferred
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  BasicMessage(Task basic_task, TaskEnumType detail_task, ProxyId proxy_id, ArgumentTypes... args)
      : BasicMessage(basic_task, detail_task, proxy_id, amsr::core::Span<ProxyId>{}, args...) {}

  /*!
   * \brief Constructs a message describing an operation which does not need an object.
   * \param[in] basic_task Task of the message
   * \param[in] detail_task Method task of the message
   * \param[in] args Argument values to be transferred
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  BasicMessage(Task basic_task, TaskEnumType detail_task, ArgumentTypes... args)
      : BasicMessage(basic_task, detail_task, 0, amsr::core::Span<ProxyId>{}, args...) {}

  // VECTOR NC AutosarC++17_10-A12.1.5: MD_CRYPTO_AutosarC++17_10-A12.1.5_param_pack
  /*!
   * \brief Constructs a message describing an operation with a return value.
   * \param[in] basic_task Task of the message
   * \param[in] detail_task Detail task of the message
   * \param[in] proxy_id ProxyId of the calling object
   * \param[in] new_proxy_id NewProxyId of the result object
   * \param[in] args Argument values to be transferred
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  BasicMessage(Task basic_task, TaskEnumType detail_task, ProxyId proxy_id, ProxyId new_proxy_id, ArgumentTypes... args)
      : content_{args...},
        basic_task_{basic_task},
        detail_task_{detail_task},
        proxy_id_{proxy_id},
        new_proxy_ids_{},
        num_new_proxy_ids_{1} {
    new_proxy_ids_[0] = new_proxy_id;
  }

  /*!
   * \brief Constructs a message describing an operation with a return value.
   * \param[in] basic_task Task of the message.
   * \param[in] detail_task Detail task of the message
   * \param[in] proxy_id ProxyId of the calling object
   * \param[in] new_proxy_ids Vector of of ObjectIds containing new objects ids of the result object
   * \param[in] args Argument values to be transferred
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  // VECTOR NC VectorC++-V3.9.2: MD_CRYPTO_VectorC++-V3.9.2_built_in_types_as_function_parameters
  BasicMessage(Task basic_task, TaskEnumType detail_task, ProxyId proxy_id,
               amsr::core::Span<ProxyId const> new_proxy_ids, ArgumentTypes... args)
      : content_{args...},
        basic_task_{basic_task},
        detail_task_{detail_task},
        proxy_id_{proxy_id},
        new_proxy_ids_{},
        num_new_proxy_ids_{0} {
    static_cast<void>(
        std::for_each(new_proxy_ids.data(), new_proxy_ids.end(), [this](ProxyId const id) { AddNewProxyId(id); }));
  }

  /*!
   * \brief Constructs a message describing an operation without an object.
   * \param[in] basic_task Task of the message
   * \param[in] detail_task Detail task of the message
   * \param[in] arg_tuple Argument values to be transferred
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  BasicMessage(Task basic_task, TaskEnumType detail_task, std::tuple<ArgumentTypes...> arg_tuple)
      : content_{std::move(arg_tuple)},
        basic_task_{basic_task},
        detail_task_{detail_task},
        proxy_id_{0},
        new_proxy_ids_{},
        num_new_proxy_ids_{0} {}

  /*!
   * Default Destructor
   * \vprivate Component Private
   */
  virtual ~BasicMessage() noexcept = default;

  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  BasicMessage(BasicMessage const& /*other*/) = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  BasicMessage(BasicMessage&& /*other*/) noexcept = default;

  /*!
   * \brief Default copy assignment operator
   * \return reference to this
   * \vprivate Component Private
   */
  auto operator=(BasicMessage const& /*other*/) & -> BasicMessage& = default;

  /*!
   * \brief Default move assignment operator
   * \return reference to this
   * \vprivate Component Private
   */
  auto operator=(BasicMessage&& /*other*/) & noexcept -> BasicMessage& = default;

  /*!
   * \brief Content tuple of the message object.
   * \return content
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto Content() -> std::tuple<ArgumentTypes...>& { return content_; }

  /*!
   * \brief Content tuple of the message object.
   * \return const content
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto Content() const -> std::tuple<ArgumentTypes...> const& { return content_; }

  /*!
   * \brief Get detail task of the message which is e.g. used for method dispatching or errors on skeleton side.
   * \return Method task of the message
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto DetailTask() const -> TaskEnumType { return detail_task_; }

  /*!
   * \brief Get basic Task of the message telling the Endpoint how to process the message
   * \return Basic task of the message.
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto BasicTask() const -> Task { return basic_task_; }

  /*!
   * \brief Get NewProxyId of the message, used to register the result object.
   * \return NewProxyId stored in the message
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto NewProxyId() const noexcept -> amsr::core::Span<ProxyId const> {
    return amsr::core::Span<ProxyId const>{new_proxy_ids_.data(), num_new_proxy_ids_};
  }

  /*!
   * \brief Get ProxyId of the message, which identifies the calling object.
   * \return ProxyId stored in the message
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  auto GetProxyId() const -> ProxyId { return proxy_id_; }

  /*!
   * \brief Update the basic task of this message.
   * \param[in] task Basic task of this message
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void SetMessageTask(TaskEnumType task) { basic_task_ = task; }

  /*!
   * \brief Update the NewProxyId of this message.
   * \param[in] proxy_ids New proxy ids of this message
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void SetNewProxyIds(amsr::core::Span<ProxyId> proxy_ids) {
    num_new_proxy_ids_ = std::min(new_proxy_ids_.size(), proxy_ids.size());
    static_cast<void>(std::copy_n(proxy_ids.begin(), num_new_proxy_ids_, new_proxy_ids_.begin()));
  }

  /*!
   * \brief Sets the ProxyId of this message.
   * \param[in] id Object id of this message
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void SetObjectId(ProxyId id) { proxy_id_ = id; }

  /*!
   * \brief Add a proxy id to the new proxy ids.
   * \param[in] new_proxy_id Proxy id to add
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate Component Private
   */
  void AddNewProxyId(ProxyId new_proxy_id) noexcept {
    if (num_new_proxy_ids_ < kMaxNumberOfProxies) {
      new_proxy_ids_[num_new_proxy_ids_] = new_proxy_id;
      ++num_new_proxy_ids_;
    }
  }

 private:
  std::tuple<ArgumentTypes...> content_;                          /*!< Content of the message */
  Task basic_task_;                                               /*!< Basic Task of this message */
  TaskEnumType detail_task_;                                      /*!< Detail Task of this message */
  ProxyId proxy_id_;                                              /*!< ProxyId of the calling object */
  amsr::core::Array<ProxyId, kMaxNumberOfProxies> new_proxy_ids_; /*!< ProxyIds to register the result object */
  std::size_t num_new_proxy_ids_;                                 /*!< Number of proxy ids in new_proxy_ids_ array */
};

/*!
 * \brief Equal to comparison operator for BasicMessages.
 * \tparam TaskEnumType Enumeration of detail tasks of this BasicMessage
 * \tparam ArgumentTypes Tuple of argument types contained in this BasicMessage
 * \param[in] lhs Left hand side of the comparison
 * \param[in] rhs Right hand side of the comparison
 * \return true if \c lhs is equal to \c rhs, false otherwise
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Component Private
 */
template <typename TaskEnumType, typename... ArgumentTypes>
auto operator==(BasicMessage<TaskEnumType, ArgumentTypes...> const& lhs,
                BasicMessage<TaskEnumType, ArgumentTypes...> const& rhs) noexcept -> bool {
  bool const basic_task_eq{lhs.BasicTask() == rhs.BasicTask()};
  bool const detail_task_eq{lhs.DetailTask() == rhs.DetailTask()};
  bool const proxy_id_eq{lhs.GetProxyId() == rhs.GetProxyId()};
  bool const content_eq{lhs.Content() == rhs.Content()};

  // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
  auto const& lhs_new_proxy_ids = lhs.NewProxyId();
  // VECTOR NC AutosarC++17_10-A8.5.2: MD_CRYPTO_AutosarC++17_10-A8.5.2_auto_from_ret_type
  auto const& rhs_new_proxy_ids = rhs.NewProxyId();
  bool const new_proxy_ids_eq{std::equal(lhs_new_proxy_ids.begin(), lhs_new_proxy_ids.end(), rhs_new_proxy_ids.begin(),
                                         rhs_new_proxy_ids.end())};

  return (basic_task_eq && detail_task_eq && proxy_id_eq && new_proxy_ids_eq && content_eq);
}

/*!
 * \brief Not equal to comparison operator for BasicMessages.
 * \tparam TaskEnumType Enumeration of detail tasks of this BasicMessage
 * \tparam ArgumentTypes Tuple of argument types contained in this BasicMessage
 * \param[in] lhs Left hand side of the comparison
 * \param[in] rhs Right hand side of the comparison
 * \return true if \c lhs is not equal to \c rhs, false otherwise
 * \context ANY
 * \pre -
 * \threadsafe FALSE
 * \reentrant TRUE
 * \synchronous TRUE
 * \vprivate Component Private
 */
template <typename TaskEnumType, typename... ArgumentTypes>
auto operator!=(BasicMessage<TaskEnumType, ArgumentTypes...> const& lhs,
                BasicMessage<TaskEnumType, ArgumentTypes...> const& rhs) noexcept -> bool {
  return !(lhs == rhs);
}

}  // namespace ipc
}  // namespace sec
}  // namespace amsr

#endif  // LIB_LIBCOMMON_INCLUDE_AMSR_SEC_IPC_BASIC_MESSAGE_H_
