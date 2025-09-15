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
/**        \file  subscribe_eventgroup_nack_entry_interpreter.h
 *        \brief  -
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_ENTRIES_SUBSCRIBE_EVENTGROUP_NACK_ENTRY_INTERPRETER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_ENTRIES_SUBSCRIBE_EVENTGROUP_NACK_ENTRY_INTERPRETER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/entries/service_discovery_subscribe_eventgroup_nack_entry.h"
#include "amsr/someip_daemon_core/service_discovery/message/options/option_interpreter_interface.h"
#include "ara/core/optional.h"
#include "someip-protocol/internal/someip_posix_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace service_discovery {
namespace message {
namespace entries {

/*!
 * \brief Interpretation of SOME/IP subscribe eventgroup nack messages
 */
class SubscribeEventgroupNAckEntryInterpreter final {
 public:
  SubscribeEventgroupNAckEntryInterpreter() = delete;
  ~SubscribeEventgroupNAckEntryInterpreter() = delete;
  SubscribeEventgroupNAckEntryInterpreter(SubscribeEventgroupNAckEntryInterpreter const &) = delete;
  SubscribeEventgroupNAckEntryInterpreter(SubscribeEventgroupNAckEntryInterpreter &&) = delete;
  SubscribeEventgroupNAckEntryInterpreter &operator=(SubscribeEventgroupNAckEntryInterpreter const &) & = delete;
  SubscribeEventgroupNAckEntryInterpreter &operator=(SubscribeEventgroupNAckEntryInterpreter &&) & = delete;
  /*!
   * \brief Interprets a Subscribe Eventgroup Nack Entry
   * \param[in] entry   The service discovery entry
   * \param[in] options The service discovery options container
   * \return A result with the interpreted offer service entry or containing an error.
   *
   * \retval SdOptionInterpretationError::kBadIndexing        Not possible to access an option from the given index
   * \retval SdOptionInterpretationError::kMalformedAddress   The address of an option can't be interpreted
   * \retval SdOptionInterpretationError::kUnknownOptionType  A referenced option type is unknown
   * \retval SdOptionInterpretationError::kNotAllowedOption   A referenced option type is not allowed and not
   *                                                          discardable
   * \retval SdEntryInterpretationError::kWrongEntryType      The passed entry doesn't correspond to an
   *                                                          OfferServiceEntry
   *
   * \pre         -
   * \context Network
   * \steady FALSE
   * \synchronous TRUE
   * \trace SPEC-10144606
   * \trace SPEC-10144608
   */
  static ara::core::Result<message::entries::SubscribeEventgroupNAckEntry> Interpret(
      message::entries::ServiceDiscoveryEntry const &entry,
      message::options::ServiceDiscoveryOptionContainer const &options);

 private:
  /*!
   * \brief Specific implementation of the option interpreter interface for SubscribeEventgroupNack entries
   */
  class OptionInterpreter : public options::OptionInterpreterInterface {
   public:
    /*!
     * \brief Default constructor.
     * \context Network
     * \steady FALSE
     */
    OptionInterpreter() noexcept = default;

    /*!
     * \brief Default destructor.
     * \context Network
     * \steady FALSE
     */
    ~OptionInterpreter() noexcept override = default;

    OptionInterpreter(OptionInterpreter const &) = delete;
    OptionInterpreter(OptionInterpreter &&) = delete;
    OptionInterpreter &operator=(OptionInterpreter const &) & = delete;
    OptionInterpreter &operator=(OptionInterpreter &&) & = delete;

   private:
    /*!
     * \brief Interprets an options::IpEndpointOption containing a TCP endpoint
     * \param[in] endpoint  The endpoint
     *
     * \return Always an error, as endpoints can't be referenced by NACK entries
     *
     * \retval SdOptionInterpretationError::kNotAllowedOption     The entry endpoint is not
     *                                                            allowed by the SOME/IP SD protocol
     *
     * \pre         -
     * \context Network
     * \steady FALSE
     * \synchronous TRUE
     */
    ara::core::Result<void> OnTcpEndpointOption(options::IpEndpointOption const &endpoint) override;
    /*!
     * \brief Interprets an options::IpEndpointOption containing a UDP endpoint
     * \param[in] endpoint  The endpoint
     *
     * \return Always an error, as endpoints can't be referenced by NACK entries
     *
     * \retval SdOptionInterpretationError::kNotAllowedOption     The entry endpoint is not
     *                                                            allowed by the SOME/IP SD protocol
     *
     * \pre         -
     * \context Network
     * \steady FALSE
     * \synchronous TRUE
     */
    ara::core::Result<void> OnUdpEndpointOption(options::IpEndpointOption const &endpoint) override;
  };

  /*!
   * \brief Checks correctness of an entry id
   * \param[in] entry_id The entry id
   * \return True if the entry id is correct
   *
   * \pre         -
   * \context Network
   * \steady FALSE
   * \synchronous TRUE
   */
  static bool CheckEntryId(entries::EventgroupEntryId const entry_id);
};

}  // namespace entries
}  // namespace message
}  // namespace service_discovery
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_SERVICE_DISCOVERY_MESSAGE_ENTRIES_SUBSCRIBE_EVENTGROUP_NACK_ENTRY_INTERPRETER_H_
