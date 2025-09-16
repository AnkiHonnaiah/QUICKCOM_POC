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
/*!        \file  amsr/sec/ipc/ipc_transporter_factory.h
 *        \brief  Factory for Basic IPC Transporter.
 *
 *      \details  Factory providing transporters for Basic IPC.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBIPC_INCLUDE_AMSR_SEC_IPC_IPC_TRANSPORTER_FACTORY_H_
#define LIB_LIBIPC_INCLUDE_AMSR_SEC_IPC_IPC_TRANSPORTER_FACTORY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "amsr/sec/ipc/transporter_factory.h"
#include "osabstraction/io/reactor1/reactor1.h"

namespace amsr {
namespace sec {
namespace ipc {

/*!
 * \brief Factory for Basic IPC Transporter.
 * \details Can be used to create IpcTransporters.
 * \vprivate Component Private
 */
class IpcTransporterFactory : public TransporterFactory {
 public:
  /*!
   * \brief Constructor.
   * \vprivate Component Private
   */
  IpcTransporterFactory() = default;

  /*!
   * \brief Destructor.
   * \vprivate Component Private
   */
  ~IpcTransporterFactory() noexcept override = default;

  /*!
   * \brief Explicitly-deleted copy constructor
   * \vprivate Component Private
   */
  IpcTransporterFactory(IpcTransporterFactory const& /*other*/) = delete;

  /*!
   * \brief Explicitly-deleted move constructor
   * \vprivate Component Private
   */
  IpcTransporterFactory(IpcTransporterFactory&& /*other*/) noexcept = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Explicitly-deleted copy assignment operator
   * \return -
   * \vprivate Component Private
   */
  IpcTransporterFactory& operator=(IpcTransporterFactory const& /*other*/) = delete;

  // VECTOR NC AutosarC++17_10-A10.2.1: MD_CRYPTO_AutosarC++17_10-A10.2.1_hiding_functions
  /*!
   * \brief Explicitly-deleted move assignment operator
   * \return -
   * \vprivate Component Private
   */
  IpcTransporterFactory& operator=(IpcTransporterFactory&& /*other*/) noexcept = delete;

  /*!
   * \copydoc sec::ipc::TransporterFactory::GetTransporter()
   * \vprivate Component Private
   */
  std::unique_ptr<Transporter> GetTransporter(std::unique_ptr<amsr::ipc::Connection> connection_handle) const override;
};

}  // namespace ipc
}  // namespace sec
}  // namespace amsr

#endif  // LIB_LIBIPC_INCLUDE_AMSR_SEC_IPC_IPC_TRANSPORTER_FACTORY_H_
