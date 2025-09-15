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
 *        \brief  This file contains implementation of MetaDataManager class.
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_META_DATA_MANAGER_META_DATA_MANAGER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_META_DATA_MANAGER_META_DATA_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/configuration.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"
#include "amsr/someip_daemon_core/connection_manager/meta_data_manager/systemclock_timestamp_generator.h"
#include "someip-protocol/internal/pdu_message.h"
#include "someip-protocol/internal/someip_message.h"

namespace amsr {
namespace someip_daemon_core {
namespace connection_manager {
namespace meta_data_manager {

/*!
 * \brief           Responsible for managing the meta data in Some/IP and PDU messages
 * \vprivate        Vector component internal API
 */
class MetaDataManager {
 public:
  /*!
   * \brief           Delete default constructor
   * \steady          TRUE
   */
  MetaDataManager() = delete;
  /*!
   * \brief           Constructor of MetaDataManager
   * \vprivate
   * \param[in]       config
   *                  A pointer to SOME/IP daemon configuration.
   * \param[in]       time_stamp_generator_ptr
   *                  A pointer to a time stamp generator.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \reentrant       FALSE
   * \steady          TRUE
   */
  MetaDataManager(configuration::Configuration const* config,
                  SystemClockTimeStampGenerator* time_stamp_generator_ptr) noexcept;

  /*!
   * \brief           Generate meta data if it is configured to be used for this SOME/IP message in the daemon
   *                  configuration
   * \param[in,out]   someip_message
   *                  SomeIP message with meta data to be filled in.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \trace           DSGN-SomeIpDaemon-MetaData
   * \steady          TRUE
   */
  void GenerateMetaDataIfConfigured(someip_protocol::internal::SomeIpMessage& someip_message) const noexcept;

  /*!
   * \brief           Generate meta data if it is configured to be used for this PDU message in the daemon configuration
   * \param[in,out]   pdu_message
   *                  PDU message with meta data to be filled in.
   * \context         ANY
   * \pre             -
   * \threadsafe      TRUE, for different this pointer
   * \trace           DSGN-SomeIpDaemon-MetaData
   * \steady          TRUE
   */
  void GenerateMetaDataIfConfigured(someip_protocol::internal::PduMessage& pdu_message) const noexcept;

  /*!
   * \brief           Virtual default destructor
   * \steady          TRUE
   */
  virtual ~MetaDataManager() noexcept = default;

  /*!
   * \brief           Delete move constructor
   * \steady          TRUE
   */
  MetaDataManager(MetaDataManager&&) = delete;

  /*!
   * \brief           Delete copy constructor
   * \steady          TRUE
   */
  MetaDataManager(MetaDataManager const&) = delete;

  /*!
   * \brief           Delete move assignment
   * \steady          TRUE
   */
  MetaDataManager& operator=(MetaDataManager&&) = delete;

  /*!
   * \brief           Delete copy assignment
   * \steady          TRUE
   */
  MetaDataManager& operator=(MetaDataManager const&) = delete;

 private:
  /*!
   * \brief           A pointer to SOME/IP daemon configuration
   */
  configuration::Configuration const* config_;

  /*!
   * \brief           A pointer to time stamp generator
   */
  SystemClockTimeStampGenerator* time_stamp_generator_;
};

}  // namespace meta_data_manager
}  // namespace connection_manager
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONNECTION_MANAGER_META_DATA_MANAGER_META_DATA_MANAGER_H_
