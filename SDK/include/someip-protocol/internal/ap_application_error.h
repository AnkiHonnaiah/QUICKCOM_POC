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
/**        \file ap_application_error.h
 *        \brief C++ representation of the SOME/IP protocol on-wire payload format of an ApApplicationError.
 *      \details For the SOME/IP protocol an ApApplicationError is represented by an union containing a struct with all
 *               ApApplicationError related attributes (ErrorDomain, ErrorCode, SupportData, UserMessage).
 *               The usage of a wrapping union allows a simple modification / extension of the on-wire format in the
 *               future.
 *      \unit SomeIpProtocol::Serdes::ApApplicationError
 *      \complexity High metrics are expected for complex data structures due to the nature of the recursive design.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_AP_APPLICATION_ERROR_H_
#define LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_AP_APPLICATION_ERROR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/error_domain.h"
#include "ara/core/variant.h"

namespace amsr {
namespace someip_protocol {
namespace internal {
namespace serialization {

/*!
 * \brief One possible variant of ApApplicationErrorStruct.
 * \trace SPEC-8053379
 */
struct ApApplicationErrorStruct {
  /*!
   * \brief A 64-bit unsigned integer representing the ApApplicationErrorDomain. value, to which the raised
   * ApApplicationError belongs.
   */
  amsr::core::ErrorDomain::IdType error_domain_value;
  /*!
   * \brief A 32-bit signed integer representing the ApApplicationError. errorCode, which is represented on binding
   * level as amsr::core::ErrorCode::Value().
   */
  amsr::core::ErrorDomain::CodeType error_code;
  /*!
   * \brief A 32-bit signed integer representing additional (vendor specific) support data, which is represented on
   * binding level as amsr::core::ErrorCode::SupportData().
   */
  amsr::core::ErrorDomain::SupportDataType::CodeType support_data;
  /*!
   * \brief A variable length string representing a user message, which is represented on binding level as
   * amsr::core::ErrorCode::UserMessage().
   */
  amsr::core::ErrorDomain::StringType user_message;
};

/*!
 * \brief Used to model possible variants of ApApplicationError. Currently only one variant is supported.
 * \trace SPEC-8053379
 */
using ApApplicationError = ara::core::Variant<ApApplicationErrorStruct>;

}  // namespace serialization

}  // namespace internal
}  // namespace someip_protocol
}  // namespace amsr

#endif  // LIB_SOMEIP_PROTOCOL_SERDES_INCLUDE_SOMEIP_PROTOCOL_INTERNAL_AP_APPLICATION_ERROR_H_
