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
/*!        \file  identifiable.h
 *        \brief  Empty base class for safe identification of any ara::crypto object.
 *
 *      \details  In order to generate an unique identifier for any ara::crypto object, it is necessary to have
 *                a common base class for all objects. When taking the address of an object, it is not guaranteed,
 *                that the pointer to the base class of an object has the same address as the pointer to the derived
 *                class of the same object. Therefor all objects are passed as identifiable when calculating the
 *                object id (which is based on the address of the object) to make sure the address calculation is
 *                consistent.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_CRYPTO_COMMON_IDENTIFIABLE_H_
#define LIB_LIBARA_INCLUDE_CRYPTO_COMMON_IDENTIFIABLE_H_

namespace crypto {
namespace common {

/*!
 * \brief Empty base class for safe identification of any ara::crypto object
 * \vprivate Component Private
 */
class Identifiable {
 public:
  /*!
   * \brief Default constructor
   * \vprivate Component Private
   */
  Identifiable() = default;

  /*!
   * \brief Destructor
   * \vprivate Component Private
   */
  virtual ~Identifiable() noexcept = default;

  // Identifiable is not move assignable
  Identifiable& operator=(Identifiable&& /*other*/) & noexcept = delete;

 protected:
  /*!
   * \brief Default copy constructor
   * \vprivate Component Private
   */
  Identifiable(Identifiable const& /*other*/) = default;

  /*!
   * \brief Default move constructor
   * \vprivate Component Private
   */
  Identifiable(Identifiable&& /*other*/) noexcept = default;

  /*!
   * \brief Default copy assignment constructor
   * \vprivate Component Private
   */
  Identifiable& operator=(Identifiable const& /*other*/) & = default;
};

}  // namespace common
}  // namespace crypto

#endif  // LIB_LIBARA_INCLUDE_CRYPTO_COMMON_IDENTIFIABLE_H_
