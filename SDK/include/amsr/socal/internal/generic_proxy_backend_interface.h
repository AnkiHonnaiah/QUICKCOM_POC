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
/**        \file  generic_proxy_backend_interface.h
 *        \brief  Interface definition for the generic proxy backend.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_INTERNAL_GENERIC_PROXY_BACKEND_INTERFACE_H_
#define LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_INTERNAL_GENERIC_PROXY_BACKEND_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <set>
#include "amsr/core/string_view.h"
#include "amsr/generic/activatable_source_code.h"
#include "amsr/socal/internal/events/generic_proxy_event_backend_interface.h"
#include "amsr/socal/internal/fields/generic_proxy_field_backend_interface.h"
#include "amsr/socal/internal/methods/generic_proxy_fire_and_forget_method_backend_interface.h"
#include "amsr/socal/internal/methods/generic_proxy_method_backend_interface.h"
#include "amsr/socal/skeleton_connection_state.h"
#include "ara/core/abort.h"

namespace amsr {
namespace socal {
namespace internal {

/*!
 * \brief Interface definition for the generic proxy backend.
 */
class AMSR_LG_BETA GenericProxyBackendInterface {
 public:
  /*!
   * \brief Type alias to the generic proxy event backend interface.
   */
  using GenericProxyEventBackendInterface = ::amsr::socal::internal::events::GenericProxyEventBackendInterface;

  /*!
   * \brief Type alias to the generic proxy field backend interface.
   */
  using GenericProxyFieldBackendInterface = ::amsr::socal::internal::fields::GenericProxyFieldBackendInterface;

  /*!
   * \brief Type alias to the generic proxy fire-and-forget method backend interface.
   */
  using GenericProxyFireAndForgetMethodBackendInterface =
      ::amsr::socal::internal::methods::GenericProxyFireAndForgetMethodBackendInterface;

  /*!
   * \brief Type alias to the generic proxy method backend interface.
   */
  using GenericProxyMethodBackendInterface = ::amsr::socal::internal::methods::GenericProxyMethodBackendInterface;

  /*!
   * \brief Type alias to the StringView type.
   */
  using StringView = ::amsr::core::StringView;

  /*!
   * \brief Constructs the generic proxy backend.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  GenericProxyBackendInterface() noexcept = default;

  /*!
   * \brief Destroys the generic proxy backend.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  virtual ~GenericProxyBackendInterface() noexcept = default;

  GenericProxyBackendInterface(GenericProxyBackendInterface const&) = delete;
  GenericProxyBackendInterface(GenericProxyBackendInterface&&) = delete;
  auto operator=(GenericProxyBackendInterface const&) & -> GenericProxyBackendInterface& = delete;
  auto operator=(GenericProxyBackendInterface&&) & -> GenericProxyBackendInterface& = delete;

  /*!
   * \brief   Get the backend for the given event.
   * \details To ensure thread-safety, for each event a distinct backend instance shall be returned.
   * \param[in] short_name  The short-name of the event.
   * \return A generic event backend.
   *         The returned reference must be valid at-least until the lifetime of this generic proxy backend object.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  virtual auto GetEventBackend(StringView const& short_name) noexcept -> GenericProxyEventBackendInterface& = 0;

  /*!
   * \brief   Get the backend for the given field.
   * \details To ensure thread-safety, for each field a distinct backend instance shall be returned.
   * \param[in] short_name  The short-name of the field.
   * \return A generic field backend.
   *         The returned reference must be valid at-least until the lifetime of this generic proxy backend object.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  virtual auto GetFieldBackend(StringView const& short_name) noexcept -> GenericProxyFieldBackendInterface& = 0;

  /*!
   * \brief   Get the backend for the given fire-and-forget method.
   * \details To ensure thread-safety, for each fire-and-forget method a distinct backend instance shall be returned.
   * \param[in] short_name  The short-name of the fire-and-forget method.
   * \return A generic fire-and-forget method backend.
   *         The returned reference must be valid at-least until the lifetime of this generic proxy backend object.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  virtual auto GetFireAndForgetMethodBackend(StringView const& short_name) noexcept
      -> GenericProxyFireAndForgetMethodBackendInterface& = 0;

  /*!
   * \brief   Get the backend for the given method.
   * \details To ensure thread-safety, for each method a distinct backend instance shall be returned.
   * \param[in] short_name  The short-name of the method.
   * \return A generic method backend.
   *         The returned reference must be valid at-least until the lifetime of this generic proxy backend object.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  virtual auto GetMethodBackend(StringView const& short_name) noexcept -> GenericProxyMethodBackendInterface& = 0;

  /*!
   * \brief Get the short-names of all the configured events.
   * \return A set containing short-names of all the configured events. The returned StringView objects must be valid
   *         at-least until the lifetime of this generic proxy backend object.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  virtual auto GetAllEvents() const noexcept -> std::set<StringView> = 0;

  /*!
   * \brief Get the short-names of all the configured fields.
   * \return A set containing short-names of all the configured fields. The returned StringView objects must be valid
   *         at-least until the lifetime of this generic proxy backend object.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  virtual auto GetAllFields() const noexcept -> std::set<StringView> = 0;

  /*!
   * \brief Get the short-names of all the configured fire-and-forget methods.
   * \return A set containing short-names of all the configured fire-and-forget methods. The returned StringView objects
   *         must be valid at-least until the lifetime of this generic proxy backend object.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  virtual auto GetAllFireAndForgetMethods() const noexcept -> std::set<StringView> = 0;

  /*!
   * \brief Get the short-names of all the configured methods.
   * \return A set containing short-names of all the configured methods. The returned StringView objects must be valid
   *         at-least until the lifetime of this generic proxy backend object.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   *              FALSE against any other API of this class.
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  virtual auto GetAllMethods() const noexcept -> std::set<StringView> = 0;

  /*!
   * \brief Get the connection state between proxy and skeleton.
   * \return amsr::socal::SkeletonConnectionState.
   * \pre         -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  virtual auto ReadSkeletonConnectionState() const noexcept -> ::amsr::socal::SkeletonConnectionState {
    ::ara::core::Abort(
        "Default implementation of GenericProxyBackendInterface::ReadSkeletonConnectionState() shall never be "
        "called!");
    return ::amsr::socal::SkeletonConnectionState::kDisconnected;
  }
};

}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_GENERIC_INCLUDE_AMSR_SOCAL_INTERNAL_GENERIC_PROXY_BACKEND_INTERFACE_H_
