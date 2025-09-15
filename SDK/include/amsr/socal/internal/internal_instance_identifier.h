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
/**        \file  internal/internal_instance_identifier.h
 *        \brief  Contains strong type definitions for provided and required service instance identifiers.
 *********************************************************************************************************************/

#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_INTERNAL_INSTANCE_IDENTIFIER_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_INTERNAL_INSTANCE_IDENTIFIER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/string_view.h"
#include "ara/com/instance_identifier.h"

namespace amsr {
namespace socal {
namespace internal {

/*!
 * \brief Instance identifier port types:
 *        - KRequired: Required instance identifier.
 *        - KProvided: Provided instance identifier.
 */
enum class EPortType : std::uint8_t { kRequired = 0U, kProvided = 1U };

/*!
 * \brief InternalInstanceIdentifier.
 * \tparam Port  Type of service instance, allowed values: [kRequired, kProvided].
 *
 * \unit Socal::InternalInstanceIdentifier
 */
template <EPortType Port>
class InternalInstanceIdentifier final {
 public:
  /*!
   * \brief Constructor
   * \param[in] value  Value that is used as instance ID.
   *                   The format of the string representation is specific to the binding.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  explicit InternalInstanceIdentifier(::amsr::core::StringView const& value) noexcept;

  /*!
   * \brief Creates an internal instance identifier based on an ara::com::InstanceIdentifier.
   * \param[in] id  Valid ara::com::InstanceIdentifier.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  explicit InternalInstanceIdentifier(::ara::com::InstanceIdentifier const& id) noexcept;

  /*!
   * \brief The copy constructor creates a copy from the InternalInstanceIdentifier  object.
   * \param[in] other  Reference to the object to be copied.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  // VCA_SOCAL_COPY_CTOR_VALID_OBJECT
  InternalInstanceIdentifier(InternalInstanceIdentifier<Port> const& other) = default;

  /*!
   * \brief Default destructor.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ~InternalInstanceIdentifier() noexcept = default;  // VCA_SOCAL_COMPILER_GENERATED_DESTRUCTOR

  /*!
   * \brief Default move constructor.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  // VCA_SOCAL_COPY_CTOR_VALID_OBJECT
  InternalInstanceIdentifier(InternalInstanceIdentifier<Port>&&) noexcept = default;

  /*!
   * \brief Default move assignment.
   * \return *this
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  InternalInstanceIdentifier<Port>& operator=(InternalInstanceIdentifier<Port>&&) & noexcept = default;

  /*!
   * \brief Overloads = operator.
   * \return *this
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  // VCA_SOCAL_VALID_RETURN_FROM_ASSIGNMENT_OPERATOR, VCA_SOCAL_VALID_MOVE_CONSTRUCTION
  InternalInstanceIdentifier<Port>& operator=(InternalInstanceIdentifier<Port> const&) & = default;

  /*!
   * \brief Get string view representation of InternalInstanceIdentifier.
   * \return String view representation of InternalInstanceIdentifier.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ::amsr::core::StringView ToString() const noexcept;

  /*!
   * \brief Get ara::com::InstanceIdentifier representation of InternalInstanceIdentifier.
   * \return ara::com::InstanceIdentifier representation of InternalInstanceIdentifier.
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  ::ara::com::InstanceIdentifier const& GetAraComInstanceIdentifier() const noexcept;

  /*!
   * \brief Overloads == operator.
   * \param[in] other  InternalInstanceIdentifier  to compare.
   * \return true if equal else false
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  bool operator==(InternalInstanceIdentifier<Port> const& other) const noexcept;

  /*!
   * \brief Overloads < operator.
   * \param[in] other  InternalInstanceIdentifier  to compare.
   * \return true if *this < other else false
   * \pre         -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady FALSE
   * \spec requires true; \endspec
   */
  bool operator<(InternalInstanceIdentifier<Port> const& other) const noexcept;

 private:
  /*!
   * \brief Service instance ID as ara::com::InstanceIdentifier.
   */
  ::ara::com::InstanceIdentifier ara_com_instance_id_;
};

/*!
 * \brief Required instance identifier.
 */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_SOCAL_AutosarC++17_10_M3.4.1_Type_can_be_declared_locally_in_primary_file
using RequiredInstanceIdentifier = InternalInstanceIdentifier<EPortType::kRequired>;

/*!
 * \brief Provided instance identifier.
 */
// VECTOR NL AutosarC++17_10-M3.4.1: MD_SOCAL_AutosarC++17_10_M3.4.1_Type_can_be_declared_locally_in_primary_file
using ProvidedInstanceIdentifier = InternalInstanceIdentifier<EPortType::kProvided>;

}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_INTERNAL_INSTANCE_IDENTIFIER_H_
