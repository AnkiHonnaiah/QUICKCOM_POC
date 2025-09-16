/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!         \file  record_protocol_containers.h
 *         \brief  This file includes definitions of general container types used by RecordProtocol.
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_RECORD_PROTOCOL_CONTAINERS_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_RECORD_PROTOCOL_CONTAINERS_H_

#include <cstdint>  // uint8_t, uint16_t, uint64_t

#include "tls/internal/common/common_general_types.h"

namespace amsr {
namespace tls {
namespace internal {
namespace record_protocol {
// ============================================= General Types =============================================

/*!
 * \brief A version struct containing the major and minor versions of a TLS or DTLS record.
 * \vprivate This class is component private.
 */
class ProtocolVersion final {
 public:
  /*!
   * \brief Construct a new object; will set the version numbers to match TLS 1.2.
   */
  ProtocolVersion() noexcept = default;

  /*!
   * \brief Default copy assignment operator.
   * \return The protocol version itself.
   */
  ProtocolVersion(ProtocolVersion const&) = default;

  /*!
   * \brief Default move assignment operator.
   * \return The protocol version itself.
   */
  ProtocolVersion(ProtocolVersion&&) noexcept = default;

  /*! \brief Default destructor. */
  ~ProtocolVersion() noexcept = default;

  /*! \brief Default copy assignment operator. */
  auto operator=(ProtocolVersion const&) & -> ProtocolVersion& = default;

  /*! \brief Default move assignment operator. */
  auto operator=(ProtocolVersion&&) & noexcept -> ProtocolVersion& = default;

  /*!
   * \brief Construct a new Protocol Version object, automatically selecting the version number for
   * TLS or DTLS as defined by the tls_type argument.
   * \param[in] tls_type The TLS type to use, which determines the version number.
   * \reentrant TRUE
   */
  explicit ProtocolVersion(common::TlsType tls_type) noexcept;

  /*!
   * \brief Construct a new Protocol Version object.
   * \param[in] major The major version to use.
   * \param[in] minor The minor version to use.
   * \reentrant TRUE
   */
  ProtocolVersion(std::uint8_t major, std::uint8_t minor) noexcept;

  /*!
   * \brief Get the major version number.
   * \return uint8_t the major version number
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate This method is component private.
   */
  auto GetMajorVersion() const noexcept -> std::uint8_t;

  /*!
   * \brief Get the minor version number.
   * \return uint8_t the minor version number
   * \context ANY
   * \pre -
   * \threadsafe FALSE
   * \reentrant TRUE
   * \synchronous TRUE
   * \vprivate This method is component private.
   */
  auto GetMinorVersion() const noexcept -> std::uint8_t;

  /*!
   * \brief Overloaded equality operator, to check if two TLS versions are identical.
   * \param[in] other The ProtocolVersion to compare this one to.
   * \return true If the versions are identical. otherwise, false.
   * \vprivate This method is component private.
   * \reentrant FALSE
   */
  auto operator==(ProtocolVersion const& other) const noexcept -> bool {
    return (other.major_version_ == major_version_) && (other.minor_version_ == minor_version_);
  }

  /*!
   * \brief Less than comparison function for TLS Protocol Versions.
   * \param[in] other The ProtocolVersion to compare this one to.
   * \return true if this major and minor are less than other, otherwise false.
   * \reentrant FALSE
   */
  auto TlsLessThanCmp(ProtocolVersion const& other) const noexcept -> bool {
    bool to_return{major_version_ < other.major_version_};
    if ((!to_return) && (major_version_ == other.major_version_)) {
      to_return = minor_version_ < other.minor_version_;
    }
    return to_return;
  }

 private:
  /*!
   * \brief The major version number, defaults to 3 which is the version number of TLS.
   */
  std::uint8_t major_version_{common::kTlsMajorVersion};

  /*!
   * \brief The minor version number, defaults to 3 which is the version number of TLS.
   */
  std::uint8_t minor_version_{common::kTlsMinorVersion};
};

/*!
 * \brief This struct holds context information about the current state of the RecordProtocol.
 * \details Instances of this type should only retain information for as long as it takes to process one record.
 *          For each new received record a new context should be created or a reused context
 *          should be updated to the current state of the owning RecordProtocol.
 * \vprivate This struct is component private.
 */
struct RecordProtocolContext {
  /*!
   * \brief Flag to determine if the protocol is TLS or DTLS.
   */
  bool isTls_{false};

  /*!
   * \brief This flag indicates if the RecordProtocol has established an association.
   */
  bool is_connected_{false};

  /*!
   * \brief The current write epoch. Only used for DTLS records.
   */
  std::uint16_t current_write_epoch_{0U};

  /*!
   * \brief The current read epoch. Only used for DTLS records.
   */
  std::uint16_t current_read_epoch_{0U};

  /*!
   * \brief The current sliding window bitmap. Only used for DTLS records.
   */
  std::uint64_t sliding_window_bitmap_{0U};

  /*!
   * \brief The current right side of the sliding window. Only used for DTLS records.
   */
  std::uint64_t sliding_window_right_{0U};

  /*!
   * \brief The current implicit read sequence number. Only used for TLS records.
   */
  std::uint64_t current_implicit_read_sequence_{0U};

  /*!
   * \brief The current implicit write sequence number. Only used for TLS records.
   */
  std::uint64_t current_implicit_write_sequence_{0U};

  /*!
   * \brief The current explicit sequence number. Only used for DTLS records.
   */
  std::uint64_t current_explicit_sequence_{0U};
};

}  // namespace record_protocol
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_PROTOCOL_RECORD_PROTOCOL_CONTAINERS_H_
