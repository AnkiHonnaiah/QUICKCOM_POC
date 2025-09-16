/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2020 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!       \file   alert_message.h
 *        \brief  Class describing a TLS/DTLS Alert message
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_ALERT_MESSAGE_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_ALERT_MESSAGE_H_

#include <cstdint>
#include "ara/core/vector.h"

#include "tls/internal/common/common_general_types.h"

namespace amsr {
namespace tls {
namespace internal {
namespace messages {

/*!
 * \brief An enum containing the different alert descriptions
 * \trace SPEC-6561846, SPEC-6561828
 */
enum class AlertDescription : std::uint8_t {
  kCloseNotify = 0U,
  kUnexpectedMessage = 10U,
  kBadRecordMac = 20U,
  kDecryptionFailedRESERVED = 21U,
  kRecordOverflow = 22U,
  kDecompressionFailure = 30U,
  kHandshakeFailure = 40U,
  kNoCertificateRESERVED = 41U,
  kBadCertificate = 42U,
  kUnsupportedCertificate = 43U,
  kCertificateRevoked = 44U,
  kCertificateExpired = 45U,
  kCertificateUnknown = 46U,
  kIllegalParameter = 47U,
  kUnknownCa = 48U,
  kAccessDenied = 49U,
  kDecodeError = 50U,
  kDecryptError = 51U,
  kExportRestrictionRESERVED = 60U,
  kProtocolVersion = 70U,
  kInsufficientSecurity = 71U,
  kInternalError = 80U,
  kUserCanceled = 90U,
  kNoRenegotiation = 100U,
  kUnsupportedExtension = 110U,
  kUnknownPskIdentity = 115U
};

/*!
 * \brief An enum describing the different alert levels.
 * \trace SPEC-6561828
 */
enum class AlertLevel : std::uint8_t { kWarning = 1U, kFatal = 2U };

/*!
 * \brief Class describing a TLS/DTLS Alert Message.
 * \vprivate This class is component private.
 */
class AlertMessage final {
 public:
  /*!
   * \brief Creates a new AlertMessage object
   */
  AlertMessage() noexcept = default;

  /*!
   * \brief Destructor
   */
  ~AlertMessage() = default;

  AlertMessage(AlertMessage const&) = delete;
  AlertMessage(AlertMessage&&) = delete;
  AlertMessage& operator=(AlertMessage const&) = delete;
  AlertMessage& operator=(AlertMessage&&) = delete;

  /*!
   * \brief Get the alert description of this alert message.
   * \return AlertDescription
   * \context Any
   * \pre -
   * \reentrant TRUE
   * \trace DSGN-SecCom-HandleAlertProtocolMessages, SPEC-6561828
   * \vprivate This method is component private.
   */
  AlertDescription GetAlertDescription() const noexcept { return alert_description_; }

  /*!
   * \brief Set the alert description of this alert message.
   * \param[in] alert_description The alert description
   * \context Any
   * \pre -
   * \reentrant TRUE
   * \trace DSGN-SecCom-HandleAlertProtocolMessages, SPEC-6561828
   * \vprivate This method is component private.
   */
  void SetAlertDescription(AlertDescription alert_description) noexcept { alert_description_ = alert_description; }

  /*!
   * \brief Get the alert level of this alert message.
   * \return AlertLevel
   * \context Any
   * \pre -
   * \reentrant TRUE
   * \trace DSGN-SecCom-HandleAlertProtocolMessages, SPEC-6561828
   * \vprivate This method is component private.
   */
  AlertLevel GetAlertLevel() const noexcept { return alert_level_; }

  /*!
   * \brief Set the alert level of this alert message.
   * \param[in] alert_level The alert level
   * \context Any
   * \pre -
   * \reentrant TRUE
   * \trace DSGN-SecCom-HandleAlertProtocolMessages, SPEC-6561828
   * \vprivate This method is component private.
   */
  void SetAlertLevel(AlertLevel alert_level) noexcept { alert_level_ = alert_level; }

  /*!
   * \brief Serialize AlertLevel and AlertDescription into uint8_t array.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \return The serialized message
   * \context Any
   * \pre buffer must be of size kAlertMessageSize
   * \error TlsCommErrorCode::kTlsErrSerialize is reported if buffer size does not match alert size
   * \reentrant TRUE
   * \trace DSGN-SecCom-HandleAlertProtocolMessages, SPEC-6561828
   * \vprivate This method is component private.
   */
  ara::core::Vector<uint8_t> Serialize(common::TlsCommFlowContext const& cxt) const noexcept;

  /*!
   * \brief Deserialize into AlertLevel and AlertDescription.
   * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
   * \param[in] buffer The buffer to deserialize
   * \context Any
   * \pre -
   * \error TlsCommErrorCode::kTlsErrDeserialize is reported if buffer size does not match alert size or
   * invalid alert level/description
   * \reentrant FALSE
   * \trace DSGN-SecCom-HandleAlertProtocolMessages, SPEC-6561828
   * \vprivate This method is component private.
   */
  void Deserialize(common::TlsCommFlowContext& cxt, ara::core::Vector<uint8_t>& buffer) noexcept;

  /*!
   * \brief Reset AlertLevel and AlertDescription to default.
   * \context Any
   * \pre -
   * \reentrant FALSE
   * \trace DSGN-SecCom-HandleAlertProtocolMessages, SPEC-6561828
   * \vprivate This method is component private.
   */
  void Cleanup() noexcept;

  /*!
   * \brief Check if byte is a valid alert level.
   * \param[in] byte The byte to check
   * \return true if the byte is a valid alert level, false otherwise
   * \context Any
   * \pre -
   * \reentrant TRUE
   */
  static bool IsValidAlertLevel(std::uint8_t byte) noexcept;

  /*!
   * \brief Check if byte is a valid alert description.
   * \param[in] byte The byte to check
   * \return true if the byte is a valid alert description, false otherwise
   * \context Any
   * \pre -
   * \reentrant TRUE
   */
  static bool IsValidAlertDescription(std::uint8_t byte) noexcept;

 private:
  /*!
   * \brief Instantiate variable for AlertDescription.
   */
  AlertDescription alert_description_{AlertDescription::kCloseNotify};

  /*!
   * \brief Instantiate variable for AlertLevel.
   */
  AlertLevel alert_level_{AlertLevel::kWarning};

  /*! \brief Friend test declarations */
  FRIEND_TEST(Cdd__MessageManager, Cleanup);

  /*! \brief Friend test declarations */
  FRIEND_TEST(UT__MessageManager, Cleanup);
};

}  // namespace messages
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_MESSAGES_ALERT_MESSAGE_H_
