// VECTOR SL AutosarC++17_10-A2.9.1: MD_CRYPTO_AutosarC++17_10-A2.9.1
/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector
 * Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set
 * out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  client/type_traits.h
 *        \brief  Collection of type traits used in cryptostack client library.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_TYPE_TRAITS_H_
#define LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_TYPE_TRAITS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "amsr/sec/ipc/protocol.h"
#include "crypto/common/type_traits.h"

namespace crypto {
namespace client {

class BaseProxy;

/*!
 * \brief Typedef for Dummy type that is used for non-output-parameter that do not need to be deserialized.
 *        This holds e.g. for Identifiable objects which are proxy/skeletons and therefor not transferred via IPC.
 * \vprivate Component Private
 */
using OutParameterDummyType = amsr::sec::ipc::ProxyId;

/*!
 * \brief Checks of the given Type is derived from or a smart pointer to a class derived from crypto::client::BaseProxy
 * \tparam T Type to be checked
 * \vprivate Component Private
 */
template <typename T>
class is_proxy : public std::is_base_of<crypto::client::BaseProxy, crypto::common::raw_class_name<T>> {};

/*!
 * \copydoc is_proxy
 * \vprivate Component Private
 */
template <typename T>
class is_proxy<std::unique_ptr<T, ara::crypto::CustomDeleter>>
    : public std::is_base_of<crypto::client::BaseProxy, crypto::common::raw_class_name<T>> {};

/*!
 * \copydoc is_proxy
 * \vprivate Component Private
 */
template <typename T>
class is_proxy<std::shared_ptr<T>>
    : public std::is_base_of<crypto::client::BaseProxy, crypto::common::raw_class_name<T>> {};

/*!
 * \brief Filter for types that are used as out parameters - which means their content on server side will be sent
 * back to the client side and the original values will be updated with the ones from the response. As some values
 * cant be updated (because they are only IPC Proxies), their classes will be replaced by a dummy type which will be
 * ignored during deserialization. All of the following types are replaced by OutParameterDummyType which will not be
 * updated:
 *        - Identifiable and references to these types
 *        - Pointers need to be deserialized to update the buffer_position correctly
 * \vprivate Component Private
 */
template <class ArgumentType>
using remove_reference_and_replace_identifiables =
    typename std::conditional<!std::is_base_of<crypto::common::Identifiable,
                                               typename crypto::common::remove_uptr<
                                                   typename std::remove_reference<ArgumentType>::type>::type>::value,
                              ArgumentType, OutParameterDummyType>::type;

}  // namespace client
}  // namespace crypto

#endif  // LIB_LIBCLIENT_INCLUDE_CRYPTO_CLIENT_TYPE_TRAITS_H_
