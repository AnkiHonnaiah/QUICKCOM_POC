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
/*!        \file  instance_identifier.h
 *        \brief  Identifier of a certain instance of a service.
 *      \details  The ara::com::InstanceIdentifier is needed to distinguish different instances of the same service.
 *        \trace  SPEC-4980257
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_ARA_COM_INSTANCE_IDENTIFIER_H_
#define LIB_SOCAL_INCLUDE_ARA_COM_INSTANCE_IDENTIFIER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>
#include "ara/core/string_view.h"
#include "ara/core/vector.h"

namespace ara {
namespace com {

class InstanceIdentifier;

/*!
 * \brief Used type for a container of instance identifiers.
 * \vpublic
 *
 * \trace SPEC-8053504
 */
using InstanceIdentifierContainer = ::ara::core::Vector<InstanceIdentifier>;

/*!
 * \brief InstanceIdentifier
 * \vpublic
 *
 * \trace SPEC-4980257
 * \unit  Socal::InstanceIdentifier
 */
class InstanceIdentifier final {
 public:
  /*!
   * \brief Constructor
   * \param[in] value  Value that is used as instance ID.
   *                   The format of the string representation is specific to the binding.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   TRUE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   *
   * \trace SPEC-4980257
   */
  explicit InstanceIdentifier(::ara::core::StringView const& value) noexcept;

  /*!
   * \brief The copy constructor creates a copy from the provided InstanceIdentifier object.
   * \param[in] other  Reference to the object to be copied.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   TRUE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \spec requires true; \endspec
   * \internal
   * Steady FALSE
   * Because of the string member present in the class, the copy constructor call always allocate heap
   * memory. So this API is not allowed to call during steady-phase.
   * \endinternal
   */
  // VCA_SOCAL_VALID_COPY_OR_MOVE_CTOR_ARGUMENT, VCA_SOCAL_EXTERNAL_FUNCTION_CONTRACT
  InstanceIdentifier(InstanceIdentifier const& other) = default;

  /*!
   * \brief Default destructor.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   TRUE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  // VCA_SOCAL_COMPILER_GENERATED_DESTRUCTOR
  ~InstanceIdentifier() noexcept = default;

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
  // VCA_SOCAL_EXTERNAL_FUNCTION_CONTRACT, VCA_SOCAL_VALID_COPY_OR_MOVE_CTOR_ARGUMENT
  InstanceIdentifier(InstanceIdentifier&&) noexcept = default;

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
  // VCA_SOCAL_EXTERNAL_FUNCTION_CONTRACT
  InstanceIdentifier& operator=(InstanceIdentifier&&) & noexcept = default;

  /*!
   * \brief Overloads = operator.
   * \return *this
   * \pre         -
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \spec requires true; \endspec
   * \internal
   * Steady FALSE Because of the string member present in the class, an operator= call always allocate heap memory. So
   * this API is not allowed to call during steady-phase.
   * \endinternal
   *
   * \trace SPEC-4980257
   */
  // VCA_SOCAL_CALLING_STL_APIS
  InstanceIdentifier& operator=(InstanceIdentifier const&) & = default;

  /*!
   * \brief Get string view representation of InstanceIdentifier.
   * \return String view representation of InstanceIdentifier.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   TRUE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   *
   * \trace SPEC-4980257
   */
  ::ara::core::StringView ToString() const noexcept;

  /*!
   * \brief Get string view representation of InstanceIdentifier.
   * \return String view representation of InstanceIdentifier.
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   TRUE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   *
   * \trace SPEC-4980257
   */
  ::ara::core::StringView toString() const noexcept;

  /*!
   * \brief Overloads == operator.
   * \param[in] other  InstanceIdentifier to compare.
   * \return true if equal else false
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   TRUE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   *
   * \trace SPEC-4980257
   */
  bool operator==(InstanceIdentifier const& other) const noexcept;

  /*!
   * \brief Overloads < operator.
   * \param[in] other  InstanceIdentifier to compare.
   * \return true if *this < other else false
   * \pre         -
   * \context     App
   * \threadsafe  TRUE for same class instance, TRUE for different instances.
   * \reentrant   TRUE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \vpublic
   * \steady TRUE
   * \spec requires true; \endspec
   *
   * \trace SPEC-4980257
   */
  bool operator<(InstanceIdentifier const& other) const noexcept;

 private:
  /*!
   * \brief For non-legacy instance identifiers: String used as storage location for the string view.
   */
  std::string value_;
};

}  // namespace com
}  // namespace ara

#endif  // LIB_SOCAL_INCLUDE_ARA_COM_INSTANCE_IDENTIFIER_H_
