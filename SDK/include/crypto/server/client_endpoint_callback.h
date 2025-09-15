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
/*!        \file  client_endpoint_callback.h
 *        \brief  ClientEndpoint callback interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_CLIENT_ENDPOINT_CALLBACK_H_
#define LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_CLIENT_ENDPOINT_CALLBACK_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
namespace crypto {
namespace server {

// Forward declaration
class ClientEndpoint;

/*!
 * \brief ClientEndpoint callback interface.
 * \vprivate Component Private
 * \trace DSGN-Crypto-ProcessIsolation
 */
class ClientEndpointCallback {
 public:
  /*!
   * \brief Default constructor
   * \vprivate Component Private
   */
  ClientEndpointCallback() = default;

  /*!
   * \brief Destructor
   * \vprivate Component Private
   */
  virtual ~ClientEndpointCallback() noexcept = default;

  /*!
   * \brief Handle connection closed.
   * \param[in,out] caller The calling ClientEndpoint.
   * \vprivate Component Private
   */
  virtual void OnDisconnect(ClientEndpoint* caller) = 0;

 protected:
  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  ClientEndpointCallback(ClientEndpointCallback const& /*other*/) = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  ClientEndpointCallback(ClientEndpointCallback&& /*other*/) noexcept = default;

  /*!
   * \brief Default copy assignment operator
   * \return reference to this
   * \vprivate Component Private
   */
  ClientEndpointCallback& operator=(ClientEndpointCallback const& /*other*/) & = default;

  /*!
   * \brief Default move assignment operator
   * \return reference to this
   * \vprivate Component Private
   */
  ClientEndpointCallback& operator=(ClientEndpointCallback&& /*other*/) & noexcept = default;
};

}  // namespace server
}  // namespace crypto

#endif  // LIB_LIBSERVER_INCLUDE_CRYPTO_SERVER_CLIENT_ENDPOINT_CALLBACK_H_
