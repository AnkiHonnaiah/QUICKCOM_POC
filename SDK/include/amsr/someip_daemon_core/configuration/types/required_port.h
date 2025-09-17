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
/**        \file  required_port.h
 *        \brief  Required port type
 *
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_REQUIRED_PORT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_REQUIRED_PORT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace types {
/*!
 * \brief The class represents the required port type.
 */
class RequiredPort final {
 public:
  /*!
   * \brief Constructor sets the reference to the port.
   * \param[in] port Reference to the 'port'.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  explicit RequiredPort(std::uint16_t const port) noexcept : port_{port} {}

  RequiredPort() = delete;

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~RequiredPort() = default;

  /*!
   * \brief Default copy constructor.
   * \steady FALSE
   */
  RequiredPort(RequiredPort const&) noexcept = default;

  /*!
   * \brief Deleted move constructor.
   */
  RequiredPort(RequiredPort&&) noexcept = delete;

  /*!
   * \brief Deleted copy assignment operator.
   */
  auto operator=(RequiredPort const& other) noexcept -> RequiredPort& = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(RequiredPort&& other) noexcept -> RequiredPort& = delete;

  /*!
   * \brief Check whether the port object is assigned dynamically.
   * \pre -
   * \return True if the port is assigned dynamically and false otherwise.
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  bool IsDynamic() const noexcept { return port_ == 0; }

  /*!
   * \brief Return the value to the contained port.
   * \return Value to the contained port.
   * \pre  IsDynamic() is false
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  std::uint16_t Value() const noexcept { return port_; }

  /*!
   * \brief Compare operator.
   *
   * \param[in] other the PORT object to compare with.
   * \return true if all the parameters are the same otherwise false.
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  bool operator==(RequiredPort const& other) const noexcept {
    return ((IsDynamic()) && (other.IsDynamic())) ||
           (((!IsDynamic()) && (!other.IsDynamic())) && (Value() == other.Value()));
  }

 private:
  /*!
   * \brief The port.
   */
  std::uint16_t const port_;
};

}  // namespace types
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_REQUIRED_PORT_H_
