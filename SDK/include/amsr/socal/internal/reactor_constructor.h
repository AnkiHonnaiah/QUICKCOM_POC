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
/*!        \file    reactor_constructor.h
 *        \brief    Wrapper class to construct a reactor instance.
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#ifndef LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_REACTOR_CONSTRUCTOR_H_
#define LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_REACTOR_CONSTRUCTOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/optional.h"
#include "amsr/socal/internal/reactor_constructor_interface.h"

namespace amsr {
namespace socal {
namespace internal {

/*!
 * \brief Wrapper class to construct a reactor.
 *
 * \unit Socal::Runtime::ReactorConstructor
 */
// VCA_SOCAL_FREEING_STATICALLY_ALLOCATED_MEMORY
class ReactorConstructor final : public ReactorConstructorInterface {
 public:
  /*!
   * \brief Get a reference to the constructed reactor.
   * \return Reference to the constructed reactor.
   * \pre         -
   * \context     ANY
   * \threadsafe  TRUE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady FALSE
   */
  osabstraction::io::reactor1::Reactor1& GetReactor() noexcept final;

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
  ::amsr::core::Result<osabstraction::io::reactor1::Reactor1::ConstructionToken> Preconstruct(
      std::uint16_t const num_of_callbacks) const noexcept final;

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
  void emplace(osabstraction::io::reactor1::Reactor1::ConstructionToken&& token) noexcept final;

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
  ::amsr::core::Result<osabstraction::io::reactor1::UnblockReason> HandleEvents(
      std::chrono::nanoseconds const timeout) noexcept final;

 private:
  /*!
   * \brief   Reactor instance.
   * \details This will always hold a value after successful construction of the ReactorConstructor.
   */
  ::amsr::core::Optional<osabstraction::io::reactor1::Reactor1> reactor_{};
};

}  // namespace internal
}  // namespace socal
}  // namespace amsr

#endif  // LIB_SOCAL_INCLUDE_AMSR_SOCAL_INTERNAL_REACTOR_CONSTRUCTOR_H_
