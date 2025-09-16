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
/*!        \file
 *        \brief  Defines types used in multiple IO subcomponents of libosabstraction.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_COMMON_TYPES_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_COMMON_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

namespace osabstraction {
namespace io {

/*!
 * \brief Result of the Send() operation.
 *
 * \vprivate Vector product internal API
 */
enum class SendResult : std::uint8_t {
  /*!
   * \brief The message was sent.
   */
  kSendCompleted = 0,

  /*!
   * \brief The message could be sent. Asynchronous processing necessary to finalize.
   */
  kAsyncProcessingNecessary = 1,
};

/*!
 * \brief Result of the Receive() operation.
 *
 * \vprivate Vector product internal API
 */
enum class ReceiveResult : std::uint8_t {
  /*!
   * \brief The message has been received.
   */
  kReceiveCompleted = 0,

  /*!
   * \brief Receive not possible at the moment. Asynchronous processing necessary to finalize.
   */
  kAsyncProcessingNecessary = 1,
};

}  // namespace io
}  // namespace osabstraction

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_COMMON_TYPES_H_
