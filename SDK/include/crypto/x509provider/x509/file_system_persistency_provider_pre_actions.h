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
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  file_system_persistency_provider_pre_actions.h
 *        \brief  Defines the interface of the certificate store pre-actions extension.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_PERSISTENCY_PROVIDER_PRE_ACTIONS_H_
#define LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_PERSISTENCY_PROVIDER_PRE_ACTIONS_H_

/*********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/core/result.h"
#include "amsr/core/span.h"
#include "amsr/core/string_view.h"

namespace crypto {
namespace x509provider {
namespace x509 {
/*!
 * \brief         Performs the actions necessary prior to reading a certificate file from the filesystem such as
 * checking the group identifier and potentially verifying the integrity and/or authenticity of the file data.
 * \param[in]     item_identifier Identifier of the certificate item.
 * \param[in]     group_identifier Group identifier of the group that the certificate item belongs to.
 * \param[in]     data The data that the certificate file currently contains.
 * \return        The (possibly) altered data originating from the file identified by \c group_identifier
 * and \c item_identifier.
 * \context       ANY
 * \pre           -
 * \threadsafe    FALSE
 * \error         ara::crypto::SecurityErrc::kResourceFault if a failure during the checks on \c data occured.
 * \error         ara::crypto::SecurityErrc::kAccessViolation if the successful checks in this function lead to the
 * conclusion that \c data is invalid (e.g. due to an invalid signature).
 * \error         ara::crypto::SecurityErrc::kUnsupported if the passed \c group_identifier is restricted or invalid.
 * \error         ara::crypto::SecurityErrc::kRuntimeFault if any other error occured.
 * \reentrant     FALSE
 * \synchronous   TRUE
 * \vprivate      Component Private
 * \trace         CREQ-Crypto-CertificateFilesystemPersistency
 */
auto CertificatePreRead(amsr::core::StringView item_identifier, amsr::core::StringView group_identifier,
                        amsr::core::Span<std::uint8_t const> data) noexcept
    -> amsr::core::Result<amsr::core::Span<std::uint8_t const>>;

/*!
 * \brief         Performs the actions necessary prior to writing a certificate file to the filesystem such as checking
 * the group identifier and potentially verifying the integrity and/or authenticity of the data to write.
 * \param[in]     item_identifier Identifier of the certificate item.
 * \param[in]     group_identifier Group identifier of the group that the certificate item belongs to.
 * \param[in]     data The data to write to the certificate file.
 * \return        The (possibly) altered data to write to the file identified by \c group_identifier and \c
 *                item_identifier.
 * \context       ANY
 * \pre           -
 * \threadsafe    FALSE
 * \error         ara::crypto::SecurityErrc::kResourceFault if a failure during the checks on \c data occured.
 * \error         ara::crypto::SecurityErrc::kUnsupported if the passed \c group_identifier is restricted or invalid.
 * \error         ara::crypto::SecurityErrc::kRuntimeFault if any other error occured.
 * \reentrant     FALSE
 * \synchronous   TRUE
 * \vprivate      Component Private
 * \trace         CREQ-Crypto-CertificateFilesystemPersistency
 */
auto CertificatePreWrite(amsr::core::StringView item_identifier, amsr::core::StringView group_identifier,
                         amsr::core::Span<std::uint8_t const> data) noexcept
    -> amsr::core::Result<amsr::core::Span<std::uint8_t const>>;

/*!
 * \brief         Performs the actions necessary prior to moving a certificate from a location on the filesystem to
 * another such as checking the group identifiers and potentially verifying the integrity and/or authenticity of the
 * data to move.
 * \param[in]     item_identifier Identifier of the certificate item.
 * \param[in]     from_group_identifier Group identifier of the group that the certificate item currently belongs to.
 * \param[in]     to_group_identifier Group identifier of the group that the certificate item shall be moved to.
 * \param[in]     data The data that the certificate file currently contains.
 * \return        The data (possibly altered by this function) originating from the file identified
 * by \c from_group_identifier and \c item_identifier to use for writing to the new location.
 * \context       ANY
 * \pre           -
 * \threadsafe    FALSE
 * \error         ara::crypto::SecurityErrc::kResourceFault if a failure during the checks on \c data occured.
 * \error         ara::crypto::SecurityErrc::kAccessViolation if the successful checks in this function lead to the
 * conclusion that \c data is invalid (e.g. due to an invalid signature).
 * \error         ara::crypto::SecurityErrc::kUnsupported if the passed \c to_group_identifier is restricted or
 * invalid.
 * \error         ara::crypto::SecurityErrc::kRuntimeFault if any other error occured.
 * \reentrant     FALSE
 * \synchronous   TRUE
 * \vprivate      Component Private
 * \trace         CREQ-Crypto-CertificateFilesystemPersistency
 */
auto CertificatePreMove(amsr::core::StringView item_identifier, amsr::core::StringView from_group_identifier,
                        amsr::core::StringView to_group_identifier, amsr::core::Span<std::uint8_t const> data) noexcept
    -> amsr::core::Result<amsr::core::Span<std::uint8_t const>>;

/*!
 * \brief         Performs the actions necessary prior to removing a certificate file from the filesystem.
 * \param[in]     item_identifier Identifier of the certificate item.
 * \param[in]     group_identifier Group identifier of the group that the certificate item belongs to.
 * \context       ANY
 * \pre           -
 * \threadsafe    FALSE
 * \error         ara::crypto::SecurityErrc::kAccessViolation if the successful checks in this function lead to the
 * conclusion that the file to remove is invalid.
 * \error         ara::crypto::SecurityErrc::kUnsupported if the passed \c to_group_identifier is restricted or
 * invalid.
 * \error         ara::crypto::SecurityErrc::kRuntimeFault if any other error occured.
 * \reentrant     FALSE
 * \synchronous   TRUE
 * \vprivate      Component Private
 * \trace         CREQ-Crypto-CertificateFilesystemPersistency
 */
auto CertificatePreRemove(amsr::core::StringView item_identifier, amsr::core::StringView group_identifier) noexcept
    -> amsr::core::Result<void>;
}  // namespace x509
}  // namespace x509provider
}  // namespace crypto

#endif  // LIB_LIBX509PROVIDER_INCLUDE_CRYPTO_X509PROVIDER_X509_PERSISTENCY_PROVIDER_PRE_ACTIONS_H_
