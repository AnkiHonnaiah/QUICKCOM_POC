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
/*!        \file   someip_daemon_lite/required_instance_id.h
 *         \brief  Required instance ID type.
 *         \unit   SomeIpDaemon::CommunicationExtensionPoints::RequiredServiceInstance
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_REQUIRED_INSTANCE_ID_H_
#define LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_REQUIRED_INSTANCE_ID_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_lite/data_types.h"
#include "amsr/someip_daemon_lite/internal/constants.h"

namespace amsr {
namespace someip_daemon_lite {

/*!
 * \brief Required instance ID.
 * \vpublic
 */
class RequiredInstanceId final {
 public:
  /*!
   * \brief Constructor.
   * \vpublic
   * \param[in] instance_id ID of the required instance.
   * \pre -
   * \context     ANY
   * \reentrant   FALSE
   * \threadsafe  FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \internal
   * - Construct from the passed-in argument.
   * \endinternal
   */
  explicit RequiredInstanceId(InstanceId const instance_id) noexcept : instance_id_{instance_id} {}

  /*!
   * \brief Destructor.
   * \vpublic
   * \steady FALSE
   */
  ~RequiredInstanceId() noexcept = default;

  RequiredInstanceId(RequiredInstanceId const&) = delete;
  RequiredInstanceId(RequiredInstanceId&&) = delete;
  RequiredInstanceId& operator=(RequiredInstanceId const&) = delete;
  RequiredInstanceId& operator=(RequiredInstanceId&&) = delete;

  /*!
   * \brief Check if the internal value represents all IDs.
   * \vpublic
   * \return "true" if internal instance ID is of the special value to represent
   *         all IDs and "false" otherwise.
   * \context     ANY
   * \reentrant   TRUE
   * \threadsafe  TRUE
   * \synchronous TRUE
   * \steady      FALSE
   * \internal
   * - Check if the passed-in ID represents all IDs or not and return the status.
   * \endinternal
   */
  bool IsAll() const noexcept { return (instance_id_ == internal::kInstanceIdAll); }

  /*!
   * \brief Get the internal instance ID.
   * \vpublic
   * \return The internal instance ID.
   * \pre -
   * \context     ANY
   * \reentrant   TRUE
   * \threadsafe  TRUE
   * \synchronous TRUE
   * \steady      FALSE
   * \internal
   * - Return the internal instance ID.
   * \endinternal
   */
  InstanceId Value() const noexcept { return instance_id_; }

 private:
  /*!
   * \brief Internal instance ID.
   */
  std::uint16_t const instance_id_;
};

}  // namespace someip_daemon_lite
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_LITE_INCLUDE_AMSR_SOMEIP_DAEMON_LITE_REQUIRED_INSTANCE_ID_H_
