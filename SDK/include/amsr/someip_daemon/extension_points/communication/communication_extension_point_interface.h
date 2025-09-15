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
/*!
 *         \file  communication/communication_extension_point_interface.h
 *        \brief  An interface class from which the user can derive to define custom communication extension points.
 *         \unit  SomeIpDaemon::CommunicationExtensionPoints::InterfaceClasses
 *
 *********************************************************************************************************************/
#ifndef LIB_EXTENSION_POINTS_INTERFACES_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_COMMUNICATION_COMMUNICATION_EXTENSION_POINT_INTERFACE_H_
#define LIB_EXTENSION_POINTS_INTERFACES_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_COMMUNICATION_COMMUNICATION_EXTENSION_POINT_INTERFACE_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace amsr {
namespace someip_daemon {
namespace extension_points {
namespace communication {

/*!
 * \brief An interface class from which the user can derive to define their custom communication extension points.
 * \vpublic
 * \trace SomeIpDaemon::CommunicationExtensionPoints::InterfaceClasses
 */
class CommunicationExtensionPointInterface {
 public:
  /*!
   * \brief Default constructor.
   * \steady FALSE
   */
  CommunicationExtensionPointInterface() noexcept = default;

  /*!
   * \brief Virtual destructor.
   * \details The destructor is pure virtual in order to make the class abstract.
   * \steady FALSE
   */
  virtual ~CommunicationExtensionPointInterface() noexcept = 0;

  CommunicationExtensionPointInterface(CommunicationExtensionPointInterface &&) = delete;
  CommunicationExtensionPointInterface(CommunicationExtensionPointInterface const &) = delete;
  CommunicationExtensionPointInterface &operator=(CommunicationExtensionPointInterface &&) & = delete;
  CommunicationExtensionPointInterface &operator=(CommunicationExtensionPointInterface const &) & = delete;
};

}  // namespace communication
}  // namespace extension_points
}  // namespace someip_daemon
}  // namespace amsr

#endif  // LIB_EXTENSION_POINTS_INTERFACES_INCLUDE_AMSR_SOMEIP_DAEMON_EXTENSION_POINTS_COMMUNICATION_COMMUNICATION_EXTENSION_POINT_INTERFACE_H_
