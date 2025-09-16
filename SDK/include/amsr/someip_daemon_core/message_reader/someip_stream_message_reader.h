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
/**        \file  someip_stream_message_reader.h
 *        \brief  Reader for reading in SOME/IP messages from a TCP stream
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_MESSAGE_READER_SOMEIP_STREAM_MESSAGE_READER_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_MESSAGE_READER_SOMEIP_STREAM_MESSAGE_READER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <array>
#include <limits>
#include <utility>

#include "amsr/someip_daemon_core/connection_manager/data_source.h"
#include "amsr/someip_daemon_core/logging/ara_com_logger.h"
#include "ara/core/optional.h"
#include "osabstraction/io/io_buffer.h"
#include "someip-protocol/internal/message.h"
#include "vac/memory/allocator/memory_buffer_allocator_interface.h"
#include "vac/memory/memory_buffer.h"

namespace amsr {
namespace someip_daemon_core {
namespace message_reader {

/*!
 * \brief SomeIpStreamMessageReader.
 * \vprivate Vector component private API
 */
class SomeIpStreamMessageReader final {
 public:
  /*!
   * \brief Result of the Read function
   * \vprivate Vector component private API
   */
  enum class ReturnCode : std::uint8_t {
    kMessageAvailable,   /*! Message available to be processed */
    kNoMessageAvailable, /*! No message available to be processed */
    kInvalidLengthField, /*! The length field of the SOME/IP message smaller than the minimum expected size */
    kBufferOverflow      /*! Not enough memory to save the SOME/IP message */
  };

  /*!
   * \brief A data buffer type.
   */
  using DataBufferPtr = vac::memory::UniqueMemoryBufferPtr<osabstraction::io::MutableIOBuffer>;
  /*!
   * \brief A data buffer allocator type.
   */
  using DataBufferAllocator = vac::memory::allocator::MemoryBufferAllocator;
  /*!
   * \brief Return structure of the Read function
   * \vprivate Vector component private API
   */
  struct SomeIpStreamMessageReadResult {
    /*!
     * \brief Contains the result of the Read operation
     */
    ReturnCode return_code{ReturnCode::kNoMessageAvailable};
    /*!
     * \brief Contains the header of the received message. Has value only if read_result = kBufferOverflow
     */
    ara::core::Optional<someip_protocol::internal::SomeIpMessageHeader> header;
    /*!
     * \brief Pointer to the received message. Valid only if read_result = kMessageAvailable
     */
    DataBufferPtr message;
  };

  /*!
   * \brief Constructor
   * \param[in] allocator            Allocator used to allocate the memory needed for the payload.
   * \context     ANY
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   */
  explicit SomeIpStreamMessageReader(DataBufferAllocator& allocator) noexcept;

  /*!
   * \brief Default destructor.
   *
   * \pre -
   * \context ANY
   * \steady  FALSE
   */
  // VECTOR NC AutosarC++17_10-A10.3.1: MD_SomeIpDaemon_AutosarC++17_10_A10.3.1_final_keyword_needed_false_positive
  // VECTOR NC AutosarC++17_10-A10.3.2: MD_SomeIpDaemon_AutosarC++17_10_A10.3.2_final_keyword_needed_false_positive
  // VECTOR NL AutosarC++17_10-A10.3.3: MD_SomeIpDaemon_AutosarC++17_10-A10.3.3_no_virtual_functions_in_final_class
  ~SomeIpStreamMessageReader() noexcept = default;

  SomeIpStreamMessageReader(SomeIpStreamMessageReader const&) = delete;
  SomeIpStreamMessageReader(SomeIpStreamMessageReader&&) = delete;
  SomeIpStreamMessageReader& operator=(SomeIpStreamMessageReader const&) = delete;
  SomeIpStreamMessageReader& operator=(SomeIpStreamMessageReader&&) = delete;

  /*!
   * \brief Tries to read a single message from a socket.
   * \param[in] data_source A data source from which a single message will be read.
   * \return The result of the read operation
   * \context     Network
   * \reentrant   FALSE
   * \vprivate    Vector component private API
   * \synchronous FALSE
   * \steady      TRUE
   */
  ara::core::Result<struct SomeIpStreamMessageReadResult> Read(connection_manager::TcpDataSource* data_source);

 private:
  /*!
   * \brief Internal state of the reader.
   */
  enum class State : std::uint8_t {
    kReadHeader, /*! Reading the SOME/IP message header */
    kReadBody,   /*! Reading the SOME/IP message body */
    kSkipBody    /*! Reading and discarding the SOME/IP message body */
  };

  /*!
   * \brief The length of the junk buffer to discard unused read data.
   */
  static constexpr std::size_t kJunkBufferLength{1024U};
  /*!
   * \brief A data buffer for SOME/IP header.
   */
  using SomeIpHeaderBuffer = std::array<std::uint8_t, someip_protocol::internal::kHeaderSize>;
  /*!
   * \brief A data buffer to discard unused read data.
   */
  using JunkBuffer = std::array<std::uint8_t, kJunkBufferLength>;

  /*!
   * \brief A helper function for reading in a message's header.
   * \param[in,out] data_source A data source from which the header will be read in.
   * \return bool indicating whether the header has been completely read in or not
   * \context     Network
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   */
  ara::core::Result<bool> ReadHeader(connection_manager::TcpDataSource* data_source);
  /*!
   * \brief A helper function for reading in a message's body.
   * \param[in,out] data_source A data source from the body will be read in.
   * \return bool indicating whether the body has been completely read in or not
   * \context     Network
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   */
  ara::core::Result<bool> ReadBody(connection_manager::TcpDataSource* data_source);
  /*!
   * \brief A helper function for reading in and discarding a message's body.
   * \param[in,out] data_source A data source from the body will be read in.
   * \return bool indicating whether the body has been completely skipped or not
   * \context     Network
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   */
  ara::core::Result<bool> SkipBody(connection_manager::TcpDataSource* data_source);
  /*!
   * \brief A helper function for changing the state of the state machine
   * \param[in] state The state to change to
   * \context     ANY
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   */
  void ChangeState(State const state);
  /*!
   * \brief Performs deserialization of the SOME/IP header.
   * \return The deserialized SOME/IP header.
   * \context     ANY
   * \reentrant   FALSE
   * \synchronous FALSE
   * \steady      TRUE
   */
  someip_protocol::internal::SomeIpMessageHeader DeserializeHeader() const;
  /*!
   * \brief A data buffer allocator.
   */
  DataBufferAllocator& allocator_;

  /*!
   * \brief Our logger.
   */
  someip_daemon_core::logging::AraComLogger logger_{someip_daemon_core::logging::kSomeIpLoggerContextId,
                                                    someip_daemon_core::logging::kSomeIpLoggerContextDescription,
                                                    ara::core::StringView{"SomeIpStreamMessageReader"}};
  /*!
   * \brief Current state of the reader.
   */
  State state_{State::kReadHeader};
  /*!
   * \brief Number of bytes read of the header so far.
   */
  std::size_t bytes_read_header_{};
  /*!
   * \brief Number of bytes of the message body read so far.
   */
  std::size_t bytes_read_body_{};
  /*!
   * \brief A buffer for a SOME/IP header.
   */
  SomeIpHeaderBuffer header_buffer_{};
  /*!
   * \brief A de-serialized someip header
   */
  someip_protocol::internal::SomeIpMessageHeader someip_header_{};
  /*!
   * \brief Stores message being read.
   */
  DataBufferPtr buffer_{nullptr};
  /*!
   * \brief A buffer for discarding read bytes.
   */
  static JunkBuffer junk_buffer_;
};

}  // namespace message_reader
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_MESSAGE_READER_SOMEIP_STREAM_MESSAGE_READER_H_
