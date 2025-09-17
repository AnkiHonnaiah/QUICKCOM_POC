/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2025 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  WarningDisplayExecutable/include/services/speedlimitservice/SpeedLimitService.h
 *        \brief  Header for service 'SpeedLimitService'.
 *
 *      \details
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  GENERATOR INFORMATION
 *  -------------------------------------------------------------------------------------------------------------------
 *    Generator Name: amsr_socal
 *         Commit ID: ad694a8ff64fb52ed9a0b9551cf05c10d499927a
 *********************************************************************************************************************/

#ifndef WARNINGDISPLAYEXECUTABLE_INCLUDE_SERVICES_SPEEDLIMITSERVICE_SPEEDLIMITSERVICE_H_
#define WARNINGDISPLAYEXECUTABLE_INCLUDE_SERVICES_SPEEDLIMITSERVICE_SPEEDLIMITSERVICE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/com/service_identifier.h"
#include "ara/core/string_view.h"
#include "vac/container/string_literals.h"

namespace services {
namespace speedlimitservice {

// VECTOR NC AutosarC++17_10-M7.3.6: MD_SOCAL_AutosarC++17_10-M7.3.6_usingDirectiveInHeaderFile
/*!
 * \brief Literals namespace is needed for "_sv literal.
 */
using vac::container::literals::operator""_sv;  // NOLINT(build/namespaces)

/*!
 * \brief   Representation of service 'SpeedLimitService'
 * \details Used for service-specific aspects:
 *          - Resource management
 *          - Service Identifier
 *
 * \trace SPEC-4980256
 */
class SpeedLimitService final {
 private:
  /*!
   * \brief Type-alias for easy access to StringView.
   */
  using StringViewType = ::ara::core::StringView;

  /*!
   * \brief String View for Service Identifier
   */
  static constexpr StringViewType kServiceIdentifier{"SpeedLimitService"_sv};

 public:
  /*!
   * \brief Service Identifier.
   *
   * \trace SPEC-4980252, SPEC-4980256
   */
  static constexpr ::ara::com::ServiceIdentifierType ServiceIdentifier{kServiceIdentifier};

  /*!
   * \brief Service shortname path.
   */
  static constexpr StringViewType kServiceShortNamePath{"/ServiceInterfaces/PkgSpeedLimitService/Latest/SpeedLimitService"_sv};
};

}  // namespace speedlimitservice
}  // namespace services

#endif  // WARNINGDISPLAYEXECUTABLE_INCLUDE_SERVICES_SPEEDLIMITSERVICE_SPEEDLIMITSERVICE_H_

