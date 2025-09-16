/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  transporter_factory.h
 *        \brief  Interface for TransporterFactory.
 *
 *      \details  Interface for factory providing event driven transporters.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBIPC_INCLUDE_AMSR_SEC_IPC_TRANSPORTER_FACTORY_H_
#define LIB_LIBIPC_INCLUDE_AMSR_SEC_IPC_TRANSPORTER_FACTORY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/ipc/connection.h"
#include "amsr/sec/ipc/transporter.h"

namespace amsr {
namespace sec {
namespace ipc {

/*!
 * \brief Interface for TransporterProvider.
 * \details Interface for a factory which creates asynchronous transporters.
 * \vprivate Component Private
 */
class TransporterFactory {
 public:
  /*!
   * \brief Destructor.
   * \vprivate Component Private
   */
  virtual ~TransporterFactory() noexcept = default;

  /*!
   * \brief Default constructor
   * \vprivate Component Private
   */
  TransporterFactory() = default;

  /*!
   * \brief Get a transporter for a connection handle.
   * \param[in] connection_handle The connection to be managed by the transporter.
   * \return An instance implementing the transporter interface.
   * \trace DSGN-Crypto-ProcessIsolation
   * \vprivate Component Private
   */
  virtual std::unique_ptr<Transporter> GetTransporter(
      std::unique_ptr<amsr::ipc::Connection> connection_handle) const = 0;

 protected:
  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  TransporterFactory(TransporterFactory const& /*other*/) = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  TransporterFactory(TransporterFactory&& /*other*/) noexcept = default;

  /*!
   * \brief Default copy assignment operator
   * \return reference to this
   * \vprivate Component Private
   */
  TransporterFactory& operator=(TransporterFactory const& /*other*/) & = default;

  /*!
   * \brief Default move assignment operator
   * \return reference to this
   * \vprivate Component Private
   */
  TransporterFactory& operator=(TransporterFactory&& /*other*/) & noexcept = default;
};

}  // namespace ipc
}  // namespace sec
}  // namespace amsr

#endif  // LIB_LIBIPC_INCLUDE_AMSR_SEC_IPC_TRANSPORTER_FACTORY_H_
