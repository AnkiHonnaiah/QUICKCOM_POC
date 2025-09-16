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
/*!        \file  reactor_constructor_interface.h
 *        \brief  Interface for the ReactorConstructor
 *
 *********************************************************************************************************************/
#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_REACTOR_CONSTRUCTOR_INTERFACE_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_REACTOR_CONSTRUCTOR_INTERFACE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "osabstraction/io/reactor1/reactor1.h"

namespace amsr {
namespace socal {
namespace internal {

/*!
 * \brief Interface for the ReactorConstructor.
 */
class ReactorConstructorInterface {
 public:
  /*!
   * \brief Default constructor.
   * \steady FALSE
   */
  ReactorConstructorInterface() = default;

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  virtual ~ReactorConstructorInterface() = default;

  /*!
   * \brief Delete the default copy constructor.
   */
  ReactorConstructorInterface(ReactorConstructorInterface const&) = delete;

  /*!
   * \brief Delete the default move constructor.
   */
  ReactorConstructorInterface(ReactorConstructorInterface&&) = delete;

  /*!
   * \brief Delete the default copy assignment operator.
   */
  ReactorConstructorInterface& operator=(ReactorConstructorInterface const&) & = delete;

  /*!
   * \brief Delete the default move assignment operator.
   */
  ReactorConstructorInterface& operator=(ReactorConstructorInterface&&) & = delete;

  /*!
   * \brief Get a reference to the constructed reactor.
   * \return Reference to the constructed reactor. The returned pointer shall not be a nullptr.
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  virtual osabstraction::io::reactor1::Reactor1& GetReactor() noexcept = 0;

  /*!
   * \brief Call the Preconstruct method of Reactor1.
   * \param[in] num_of_callbacks  Number of callbacks that the reactor should be able to handle. Valid range: 0 to 8191.
   * \return A Result that contains a ConstructionToken.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  // VECTOR NL AutosarC++17_10-M9.3.3: MD_SOCAL_AutosarC++17_10-M9.3.3_PureVirtualFunctionCanBeConst_FalsePositive
  virtual ::amsr::core::Result<osabstraction::io::reactor1::Reactor1::ConstructionToken> Preconstruct(
      std::uint16_t const num_of_callbacks) const noexcept = 0;

  /*!
   * \brief Create and emplace a Reactor1 object.
   * \param[in] token  A ConstructionToken to be used for constructing a Reactor object.
   * \pre         -
   * \context     Init
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  virtual void emplace(osabstraction::io::reactor1::Reactor1::ConstructionToken&& token) noexcept = 0;

  /*!
   * \brief Call the HandleEvents method of the stored Reactor1.
   * \param[in] timeout  Time to wait for events in nanoseconds.
   * \return A Result object that contains the result of HandleEvents.
   * \pre         -
   * \context     ANY Only one thread at a time, not from a callback of the stored Reactor1 object.
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady TRUE
   */
  virtual ::amsr::core::Result<osabstraction::io::reactor1::UnblockReason> HandleEvents(
      std::chrono::nanoseconds const timeout) noexcept = 0;
};

}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_REACTOR_CONSTRUCTOR_INTERFACE_H_
