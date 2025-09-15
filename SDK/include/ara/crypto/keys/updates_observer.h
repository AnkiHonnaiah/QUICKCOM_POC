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
/*!        \file  ara/crypto/keys/updates_observer.h
 *        \brief  Updates Observer interface definition.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRYPTO_KEYS_UPDATES_OBSERVER_H_
#define LIB_LIBARA_INCLUDE_ARA_CRYPTO_KEYS_UPDATES_OBSERVER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/crypto/common/std_api.h"
#include "ara/crypto/keys/elementary_types.h"

namespace ara {
namespace crypto {
namespace keys {

/*!
 * \brief Definition of an "updates observer" interface.
 * \details The "updates observer" interface should be implemented by a consumer application, if a software developer
 *          would like to get notifications about the slots' content update events.
 */
class UpdatesObserver : public std::enable_shared_from_this<UpdatesObserver> {
 public:
  /*!
   * \brief Shared smart pointer of the interface.
   * \vpublic
   */
  using Sptr = std::shared_ptr<UpdatesObserver>;

  // Attention! Deviation: Not mentioned in the AUTOSAR SWS, should be declared to satisfy AutosarC++17_10-A12.0.1
  /*!
   * \brief Constructor.
   * \vpublic
   */
  UpdatesObserver() noexcept = default;

  /*!
   * \brief Destructor.
   * \vpublic
   */
  virtual ~UpdatesObserver() noexcept = default;

  // VECTOR NC AutosarC++17_10-M9.3.3: MD_CRYPTO_AutosarC++17_10-M9.3.3_AUTOSAR_API
  /*!
   * \brief Notification method that should be called if content of specified slots was changed.
   * \param[in] updated_slots List of monitored slots that were updated after opening (for reading).
   * \context ANY
   * \pre -
   * \threadsafe TRUE
   * \reentrant FALSE
   * \synchronous TRUE
   * \vpublic
   */
  virtual void OnUpdate(TransactionScope const& updated_slots) noexcept = 0;

  // Attention! Deviation: These special member functions are declared protected to satisfy AutosarC++17_10-A12.8.6
 protected:
  // Attention! Deviation: Not mentioned in the AUTOSAR SWS, should be declared to satisfy AutosarC++17_10-A12.0.1
  /*!
   * \brief Copy constructor.
   * \vpublic
   */
  UpdatesObserver(UpdatesObserver const& /*other*/) noexcept = default;

  // Attention! Deviation: Not mentioned in the AUTOSAR SWS, should be declared to satisfy AutosarC++17_10-A12.0.1
  /*!
   * \brief Copy assignment operator.
   * \return A reference to the object
   * \vpublic
   */
  UpdatesObserver& operator=(UpdatesObserver const& /*other*/) & noexcept = default;

  // Attention! Deviation: Not mentioned in the AUTOSAR SWS, should be declared to satisfy AutosarC++17_10-A12.0.1
  /*!
   * \brief Move constructor.
   * \vpublic
   */
  UpdatesObserver(UpdatesObserver&& /*other*/) noexcept = default;

  // Attention! Deviation: Not mentioned in the AUTOSAR SWS, should be declared to satisfy AutosarC++17_10-A12.0.1
  /*!
   * \brief Move assignment.
   * \return A reference to the object
   * \vpublic
   */
  UpdatesObserver& operator=(UpdatesObserver&& /*other*/) & noexcept = default;
};

}  // namespace keys
}  // namespace crypto
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRYPTO_KEYS_UPDATES_OBSERVER_H_
