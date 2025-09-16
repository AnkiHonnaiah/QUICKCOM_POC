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
 *  ----------------------------------------------------------------------------------------------------------------*/
/**        \file  handle_type.h
 *        \brief  Specifies the handle type for a specific service instance.
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_HANDLE_TYPE_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_HANDLE_TYPE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/socal/internal/internal_instance_identifier.h"
#include "ara/com/instance_identifier.h"

namespace amsr {
namespace socal {

/*!
 * \brief Type name alias for RequiredInstanceIdentifier.
 */
using RequiredInstanceIdentifier = ::amsr::socal::internal::RequiredInstanceIdentifier;

/*!
 * \brief Type name alias for ProvidedInstanceIdentifier.
 */
using ProvidedInstanceIdentifier = ::amsr::socal::internal::ProvidedInstanceIdentifier;
/*!
 * \brief Specifies handle type.
 *
 * \trace SPEC-4980259, SPEC-4663477
 * \unit  Socal::Proxy::HandleType
 */
class HandleType {
 public:
  /*!
   * \brief  Creates a find service handle type encapsulating the provided and required instance IDs.
   * \remark Internal use only.
   * \param[in] required_instance_id  The required instance ID.
   * \param[in] provided_instance_id  The provided instance ID.
   * \pre      -
   * \context  ANY
   * \vprivate Vector component internal API.
   * \steady FALSE
   * \spec requires true; \endspec
   */
  explicit HandleType(RequiredInstanceIdentifier const& required_instance_id,
                      ProvidedInstanceIdentifier const& provided_instance_id) noexcept;

  // VECTOR NC AutosarC++17_10-A12.8.6: MD_SOCAL_AutosarC++17_10-A12.8.6_Shall_be_declared_protected
  /*!
   * \brief The copy constructor creates a copy from the provided HandleType object.
   * \param[in] other  Reference to the object to be copied.
   * \steady FALSE
   * \spec requires true; \endspec
   *
   * \trace SPEC-8053505
   */
  // VCA_SOCAL_CALLING_NON_STATIC_FUNCTION_CONSTRUCTOR
  // VCA_SOCAL_COPY_CTOR_VALID_OBJECT
  // VCA_SOCAL_ACCESS_IS_POSSIBLY_OUT_OF_BOUND_FALSE_POSITIVE
  // VCA_SOCAL_POSSIBLE_USE_AFTER_FREE_FALSE_POSITIVE
  // VCA_SOCAL_ACCESSING_ABSOLUTE_ADDRESS_FALSE_POSITIVE
  HandleType(HandleType const& other) = default;

  /*!
   * \brief Destructor
   * \steady FALSE
   * \spec requires true; \endspec
   */
  virtual ~HandleType() noexcept = default;  // VCA_SOCAL_COMPILER_GENERATED_DESTRUCTOR

  // VECTOR NC AutosarC++17_10-A12.8.6: MD_SOCAL_AutosarC++17_10-A12.8.6_Shall_be_declared_protected
  /*!
   * \brief Use default copy assignment.
   * \return HandleType& reference to this.
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   *
   * \trace SPEC-8053505
   */
  HandleType& operator=(HandleType const&) & = default;

  // VECTOR NC AutosarC++17_10-A12.8.6: MD_SOCAL_AutosarC++17_10-A12.8.6_Shall_be_declared_protected
  /*!
   * \brief Use default move constructor.
   * \steady FALSE
   * \spec requires true; \endspec
   *
   * \trace SPEC-8053506
   */
  // VCA_SOCAL_ACCESS_IS_POSSIBLY_OUT_OF_BOUND_FALSE_POSITIVE
  // VCA_SOCAL_ACCESSING_ABSOLUTE_ADDRESS_FALSE_POSITIVE
  // VCA_SOCAL_CALLING_NON_STATIC_FUNCTION_CONSTRUCTOR
  // VCA_SOCAL_POSSIBLE_USE_AFTER_FREE_FALSE_POSITIVE
  HandleType(HandleType&&) noexcept = default;

  // VECTOR NC AutosarC++17_10-A12.8.6: MD_SOCAL_AutosarC++17_10-A12.8.6_Shall_be_declared_protected
  /*!
   * \brief Use default move assignment operator.
   * \return HandleType& reference to this.
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   *
   * \trace SPEC-8053506
   */
  HandleType& operator=(HandleType&&) & noexcept = default;

  /*!
   * \brief Returns a reference to the provided instance identifier in ara::com::InstanceIdentifier type.
   * \return Reference to an instance identifier.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   TRUE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   *
   * \trace SPEC-4980259
   */
  ::ara::com::InstanceIdentifier const& GetInstanceId() const noexcept;

  /*!
   * \brief Returns a reference to the required instance identifier.
   * \return Reference to the required instance identifier.
   * \pre      -
   * \context  ANY
   * \vprivate
   * \steady FALSE
   * \spec requires true; \endspec
   */
  RequiredInstanceIdentifier const& GetRequiredInstanceId() const noexcept;
  /*!
   * \brief Returns a reference to the provided instance identifier.
   * \return Reference to the required instance identifier.
   * \pre      -
   * \context  ANY
   * \vprivate
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ProvidedInstanceIdentifier const& GetProvidedInstanceId() const noexcept;

  /*!
   * \brief Overloads == operator.
   * \param[in] other  The HandleType to compare against.
   * \return True if *this == other else false
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   TRUE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   *
   * \trace SPEC-4980259
   */
  bool operator==(HandleType const& other) const noexcept;

  /*!
   * \brief Overloads < operator.
   * \param[in] other  The HandleType to compare against.
   * \return True if *this < other else false
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   TRUE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady FALSE
   * \spec requires true; \endspec
   *
   * \trace SPEC-4980259
   */
  bool operator<(HandleType const& other) const noexcept;

 private:
  /*!
   * \brief The required instance identifier assigned to this handle.
   */
  RequiredInstanceIdentifier required_instance_id_;

  /*!
   * \brief The provided instance identifier assigned to this handle..
   */
  ProvidedInstanceIdentifier provided_instance_id_;
};

}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_HANDLE_TYPE_H_
