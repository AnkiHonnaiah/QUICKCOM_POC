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
/*!        \file  ara/com/service_identifier.h
 *        \brief  Service Identifier Type.
 *      \details  For each ServiceInterface, a unique, binding-independent, Service Identifier is generated.
 *
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_ARA_COM_SERVICE_IDENTIFIER_H_
#define LIB_SOCAL_INCLUDE_ARA_COM_SERVICE_IDENTIFIER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>
#include "ara/core/string_view.h"

namespace ara {
namespace com {

/*!
 * \brief Service Identifier Type.
 * \vpublic
 *
 * \trace SPEC-4980256
 * \unit  Socal::ServiceIdentifierType
 */
class ServiceIdentifierType final {
 public:
  /*!
   * \brief Type alias for easy access to StringView.
   * \vpublic
   */
  using StringViewType = ::ara::core::StringView;

  /*!
   * \brief Constructor.
   * \param[in] service_id  Unique service id. This shall be identical to the ServiceInterface.shortname. The underlying
   *                        string literal must be valid until the lifetime this object.
   * \pre        -
   * \context    App
   * \threadsafe TRUE for same class instance, TRUE for different instances.
   * \reentrant  TRUE for same class instance, TRUE for different instances.
   * \vprivate
   * \steady TRUE
   * \spec requires true; \endspec
   */
  explicit constexpr ServiceIdentifierType(StringViewType const& service_id) noexcept : service_id_{service_id} {}

  /*!
   * \brief Default destructor.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   TRUE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  ~ServiceIdentifierType() noexcept = default;

  /*!
   * \brief Default move constructor.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   TRUE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  ServiceIdentifierType(ServiceIdentifierType&&) noexcept = default;

  /*!
   * \brief Default copy constructor.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   TRUE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  ServiceIdentifierType(ServiceIdentifierType const&) noexcept = default;

  /*!
   * \brief Default move assignment.
   * \return *this
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  ServiceIdentifierType& operator=(ServiceIdentifierType&&) & noexcept = default;

  /*!
   * \brief Checks if the contents of the contained string view is equal to the other.
   * \param[in] other  ServiceIdentifierType to compare.
   * \return true if the contents of the contained string view is equal to the other.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   TRUE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  bool operator==(ServiceIdentifierType const& other) const noexcept;

  /*!
   * \brief Checks if the contents of the contained string view is smaller than the other.
   * \param[in] other  ServiceIdentifierType to compare.
   * \return true if the contents of the contained string view is smaller than the other.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   TRUE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  bool operator<(ServiceIdentifierType const& other) const noexcept;

  /*!
   * \brief Overloads = operator.
   * \param[in] other  ServiceIdentifierType to copy from.
   * \return *this
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  ServiceIdentifierType& operator=(ServiceIdentifierType const& other) & noexcept;

  /*!
   * \brief Get StringView representation of ServiceIdentifierType.
   * \return StringView representation of ServiceIdentifierType.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   TRUE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   */
  StringViewType ToString() const noexcept;

 private:
  /*!
   * \brief Service Id.
   */
  StringViewType service_id_;
};

/*!
 * \brief Type alias for ServiceIdentifierType, to avoid collision with binding-specific service id.
 */
using AraComServiceId = ServiceIdentifierType;

}  // namespace com
}  // namespace ara

#endif  // LIB_SOCAL_INCLUDE_ARA_COM_SERVICE_IDENTIFIER_H_
