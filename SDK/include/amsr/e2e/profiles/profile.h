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
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file
 *        \brief  Contains enumeration of supported profiles by this library.
 *
 *      \details  Contains the declaration for the Profile enum class.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_PROFILE_H_
#define LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_PROFILE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

namespace amsr {
namespace e2e {
namespace profiles {

/*!
 * \brief Enumeration of supported profiles by this library.
 * \vprivate Vector Internal API
 */
enum class Profile : std::uint8_t {
  /*!
   * \brief E2E Profile 1
   */
  Profile01 = 0,
  /*!
   * \brief E2E Profile 4
   */
  Profile04 = 1,
  /*!
   * \brief E2E Profile 5
   */
  Profile05 = 2,
  /*!
   * \brief E2E Profile 6
   */
  Profile06 = 3,
  /*!
   * \brief E2E Profile 7
   */
  Profile07 = 4,
  /*!
   * \brief E2E Profile 22
   */
  Profile22 = 5,
  /*!
   * \brief E2E Profile 44
   */
  Profile44 = 6,
  /*!
   * \brief Proprietary profile
   */
  Proprietary = 7
};

}  // namespace profiles
}  // namespace e2e
}  // namespace amsr

#endif  // LIB_LIBE2E_INCLUDE_AMSR_E2E_PROFILES_PROFILE_H_
