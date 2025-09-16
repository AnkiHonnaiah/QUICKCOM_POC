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
/**        \file  port.h
 *        \brief  Port type
 *
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_PORT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_PORT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include "ara/core/abort.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace types {
/*!
 * \brief The class represents the port type.
 */
class Port final {
 public:
  /*!
   * \brief Constructor sets the reference to the port.
   * \param[in] port Reference to the 'port'.
   * \pre port is not zero.
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  explicit Port(std::uint16_t const port) noexcept : port_{port} {
    if (port == 0) {
      ara::core::Abort("Construct Port with value 0.");
    }
  }

  Port() = delete;

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~Port() = default;

  /*!
   * \brief Default copy constructor.
   * \steady FALSE
   */
  Port(Port const&) noexcept = default;

  Port(Port&&) noexcept = delete;
  auto operator=(Port const& other) noexcept -> Port& = delete;
  auto operator=(Port&& other) noexcept -> Port& = delete;

  /*!
   * \brief Return the value to the contained port.
   * \return Value to the contained port.
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  std::uint16_t Value() const noexcept { return port_; }

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

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_TYPES_PORT_H_
