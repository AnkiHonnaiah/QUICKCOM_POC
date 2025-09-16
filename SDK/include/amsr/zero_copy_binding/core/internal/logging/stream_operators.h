/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file
 *         \brief  Provides stream operators for some AMSR types not equipped with a stream operator.
 *         \unit   ZeroCopyBinding::ZeroCopyLogger
 *
 *********************************************************************************************************************/

#ifndef LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_LOGGING_STREAM_OPERATORS_H_
#define LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_LOGGING_STREAM_OPERATORS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <ostream>

#include "amsr/core/instance_specifier.h"
#include "amsr/ipc_service_discovery/common/internal/service_instance_identifier/required_service_instance_identifier.h"
#include "amsr/socal/internal/internal_instance_identifier.h"
#include "amsr/zero_copy_common/memcon/internal/client/client_state.h"
#include "ara/com/instance_identifier.h"
#include "ara/log/logstream.h"

namespace ara {
namespace com {

/*!
 * \brief         Stream operator for ara::com::InstanceIdentifier.
 * \param[in,out] stream               Reference to a log stream to stream to.
 * \param[in]     instance_identifier  Instance identifier to stream to the log stream.
 * \return        A reference to the given log stream.
 * \context       ANY
 * \pre           -
 * \threadsafe    FALSE
 * \reentrant     FALSE
 * \synchronous   TRUE
 * \vprivate Component Private
 */
auto operator<<(::ara::log::LogStream& stream, InstanceIdentifier const& instance_identifier) noexcept
    -> ::ara::log::LogStream&;

/*!
 * \brief         Stream operator for ara::com::InstanceIdentifier.
 * \param[in,out] stream               Reference to an ostream to stream to.
 * \param[in]     instance_identifier  Instance identifier to stream to the ostream.
 * \return        A reference to the given ostream.
 * \context       ANY
 * \pre           -
 * \threadsafe    FALSE
 * \reentrant     FALSE
 * \synchronous   TRUE
 * \vprivate Component Private
 */
auto operator<<(std::ostream& stream, InstanceIdentifier const& instance_identifier) noexcept -> std::ostream&;

}  // namespace com

namespace core {

/*!
 * \brief         Stream operator for ara::core::InstanceSpecifier.
 * \param[in,out] stream              Reference to an ostream to stream to.
 * \param[in]     instance_specifier  Instance specifier to stream to the ostream.
 * \return        A reference to the given ostream.
 * \context       ANY
 * \pre           -
 * \threadsafe    FALSE
 * \reentrant     FALSE
 * \synchronous   TRUE
 * \vprivate Component Private
 */
auto operator<<(std::ostream& stream, InstanceSpecifier const& instance_specifier) noexcept -> std::ostream&;

}  // namespace core
}  // namespace ara

namespace amsr {
namespace socal {
namespace internal {

/*!
 * \brief         Stream operator for amsr::socal::internal::InternalInstanceIdentifier.
 * \tparam        Port                 The instance identifier port type.
 * \param[in,out] stream               Log stream to stream to.
 * \param[in]     instance_identifier  Instance identifier to stream to the log stream.
 * \return        The given log stream.
 * \context       ANY
 * \pre           -
 * \threadsafe    FALSE
 * \reentrant     FALSE
 * \synchronous   TRUE
 * \vprivate Component Private
 */
template <::amsr::socal::internal::EPortType Port>
auto operator<<(::ara::log::LogStream& stream,
                ::amsr::socal::internal::InternalInstanceIdentifier<Port> const& instance_identifier) noexcept
    -> ::ara::log::LogStream& {
  return stream << instance_identifier.ToString();  // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
}

template <::amsr::socal::internal::EPortType Port>
auto operator<<(std::ostream& stream,
                ::amsr::socal::internal::InternalInstanceIdentifier<Port> const& instance_identifier) noexcept
    -> std::ostream& {
  return stream
         << instance_identifier.ToString().ToString();  // VCA_ZEROCOPYBINDING_SLC_20_CALLING_FUNCTION_OUTSIDE_COMPONENT
}

}  // namespace internal
}  // namespace socal

namespace ipc_service_discovery {
namespace common {
namespace internal {

/*!
 * \brief         Stream operator for RequiredServiceInstanceIdentifier.
 * \param[in,out] stream               Reference to an ostream to stream to.
 * \param[in]     instance_identifier  Instance identifier to stream to the ostream.
 * \return        A reference to the given ostream.
 * \context       ANY
 * \pre           -
 * \threadsafe    FALSE
 * \reentrant     FALSE
 * \synchronous   TRUE
 * \vprivate Component Private
 */
auto operator<<(std::ostream& stream, RequiredServiceInstanceIdentifier const& instance_identifier) noexcept
    -> std::ostream&;

}  // namespace internal
}  // namespace common
}  // namespace ipc_service_discovery

namespace zero_copy_common {
namespace memcon {
namespace internal {
namespace client {

/*!
 * \brief         Stream operator for ClientState.
 * \param[in,out] stream        Reference to an ostream to stream to.
 * \param[in]     client_state  Client state to stream to the ostream.
 * \return        A reference to the given ostream.
 * \context       ANY
 * \pre           -
 * \threadsafe    FALSE
 * \reentrant     FALSE
 * \synchronous   TRUE
 * \vprivate Component Private
 */
auto operator<<(std::ostream& stream, ClientState client_state) noexcept -> std::ostream&;

}  // namespace client
}  // namespace internal
}  // namespace memcon
}  // namespace zero_copy_common

}  // namespace amsr

#endif  // LIB_ZERO_COPY_BINDING_CORE_INCLUDE_AMSR_ZERO_COPY_BINDING_CORE_INTERNAL_LOGGING_STREAM_OPERATORS_H_
