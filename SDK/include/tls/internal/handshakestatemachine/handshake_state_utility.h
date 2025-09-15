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
/*!        \file  handshake_state_utility.h
 *         \brief This file contains a collection of utility functions used by handshake states
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_HANDSHAKE_STATE_UTILITY_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_HANDSHAKE_STATE_UTILITY_H_

#include <cstddef>
#include <cstdint>
#include <functional>
#include <utility>
#include "ara/core/vector.h"

#include "tls/internal/handshakestatemachine/common_handshake_types.h"
#include "tls/internal/handshakestatemachine/handshake_state_machine_interface.h"
#include "tls/internal/messages/client_hello_message.h"
#include "tls/internal/messages/message_manager.h"
#include "tls/internal/record_protocol/cipher_suite.h"
#include "tls/internal/record_protocol/security_parameters.h"
#include "tls/public/config/psk_config.h"
#include "tls/public/tls_comm_general_types.h"
#include "tls_crypto_adapter/internal/crypto_adapter_interface.h"
#include "tls_crypto_adapter/internal/crypto_adapter_types.h"

namespace amsr {
namespace tls {
namespace internal {
namespace handshakestatemachine {
namespace utility {

/*!
 * \brief Type alias for a pair of private and public key types.
 */
using PrivatePublicKeyPair = std::pair<ara::crypto::cryp::PrivateKey::Uptrc, amsr::core::Array<std::uint8_t, 32U>>;

/*!
 * \brief Retrieves the PSK Identity with the most recent PSK identity hint from the server key exchange message
 * from the current PSK Callback interface
 * \param[in, out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
 * \param[in] ref_msg_mgr Reference to the message manager
 * \param[in] psk_config The pre-shared key configuration.
 * \return The PSK Identity stored in an STL container
 * \context ANY
 * \pre -
 * \reentrant FALSE
 * \vprivate This method is component private.
 * \trace SPEC-5506680
 */
ara::core::Vector<uint8_t> GetPSKIdentityFromServerKeyExchange(common::TlsCommFlowContext& cxt,
                                                               messages::MessageManager& ref_msg_mgr,
                                                               config::PskConfig const& psk_config) noexcept;

/*!
 * \brief Helper function to create the serialized verification data for the finished message. IE create the expected
 * hash for a finished message given the role.
 * \details Errors that are reported by functions this method calls are propagated to the caller of this method.
 * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
 * \param[in] ref_state_machine The HandshakeStateMachine used in the negotiation.
 * \param[in] ref_msg_mgr Reference to message manager which stores all messages
 * \param[in] entity The connection entity determining if it's a client or server finished message that the hash is
 * \param[in] cipher_suite The currently selected cipher suite.
 * created for.
 * \return ara::core::Vector<uint8_t> A byte vector containing the hash.
 * \context ANY
 * \pre cxt has no previous error information
 * \reentrant FALSE
 * \threadsafe FALSE
 * \trace SPEC-6561968, SPEC-6561969
 * \vprivate This method is component private.
 */
ara::core::Vector<uint8_t> CreateFinishedMessageVerifyData(common::TlsCommFlowContext& cxt,
                                                           HandshakeStateMachineInterface& ref_state_machine,
                                                           messages::MessageManager& ref_msg_mgr,
                                                           record_protocol::ConnectionEnd entity,
                                                           SupportedCipherSuites const cipher_suite) noexcept;

/*!
 * \brief This function uses accessible information from the provided ClientHelloMessage to build a serialized
 * message buffer. The verification secret is appended to the provided buffer.
 * \param[in] client_hello_message The client hello message storing information needed for hashing
 * \param[in] secret The verification secret
 * \param[in, out] buffer The message buffer for the serialized hash message
 * \vprivate This method is component private.
 * \context ANY
 * \trace SPEC-6562058
 * \pre -
 * \reentrant FALSE
 */
void SerializeClientHelloForHashCookie(messages::ClientHelloMessage const& client_hello_message,
                                       std::uint32_t const secret, ara::core::Vector<uint8_t>& buffer) noexcept;

/*!
 * \brief Helper function to calculate a hashed cookie based of a message buffer with any length using the SHA256
 * hashing algorithm provided by the Crypto Adapter interface.
 * \param[in, out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
 * \param[in] buffer The message to be hashed
 * \param[in] crypto_adapter A crypto adapter.
 * \return ara::core::Vector<uint8_t> The hash value stored in a byte vector
 * \context ANY
 * \pre cxt has no previous error information, buffer is not empty
 * \reentrant FALSE
 * \error TlsCommErrorCode::kTlsCryptoAdapterFailure is reported if the required hash function could not be created
 * \error TlsCommErrorCode::kTlsCryptoAdapterFailure is reported if any of the methods of the hash function
 * fails (start, update, finish).
 * \trace SPEC-6562058
 * \trace SPEC-6562060
 * \vprivate This method is component private.
 */
ara::core::Vector<uint8_t> HashCookieWithSHA256(
    common::TlsCommFlowContext& cxt, ara::core::Vector<uint8_t> const& buffer,
    tls_crypto_adapter::internal::CryptoAdapterInterface const& crypto_adapter) noexcept;

/*!
 * \brief Finds the first common Cipher suite in two priority lists of cipher suites and returns it by value.
 * \param[in] cipher_list_one First list of cipher suites
 * \param[in] cipher_list_two Second list of cipher suites
 * \return The value of the first common cipher suite, if none is found 0 is returned.
 * \vprivate This method is component private.
 * \context ANY
 * \pre -
 * \reentrant FALSE
 */
std::uint16_t FindSupportedCipherSuite(ara::core::Vector<uint16_t> const& cipher_list_one,
                                       ara::core::Vector<uint16_t> const& cipher_list_two) noexcept;

/*!
 * \brief Generates an arbitrary number of random values and returns them in a STL container
 * \param[in] num_of_values The amount of random values to generate
 * \param[in] crypto_adapter A crypto adapter.
 * \error TlsCommErrorCode::kTlsCryptoAdapterFailure is returned if the random number generation failed
 * \return STL container containing N random values
 * \vprivate This method is component private.
 * \context ANY
 * \pre -
 * \reentrant FALSE
 */
amsr::core::Result<ara::core::Vector<uint8_t>> GenerateRNG(
    size_t num_of_values, tls_crypto_adapter::internal::CryptoAdapterInterface const& crypto_adapter) noexcept;

/*!
 * \brief Checks for used extensions and returns them serialized in a STL container
 * \param[in] ref_state_machine Reference to handshake state machine
 * \param[in] ref_message_manager Reference to message manager which stores all messages
 * \return STL container containing used extensions in serialized form
 * \vprivate This method is component private.
 * \context ANY
 * \pre -
 * \reentrant FALSE
 */
ara::core::Vector<uint8_t> PrepareServerHelloExtensions(HandshakeStateMachineInterface& ref_state_machine,
                                                        messages::MessageManager& ref_message_manager) noexcept;

/*!
 * \brief Prepares outgoing messages/flight sent during entering of ServerStateClientHello
 * \param[in] config A TLS configuration.
 * \param[in] msg_mgr A reference to the message manager which stores all messages
 * \param[in] rng Reference to an array containing randomly generated values for the ServerHello message
 * \param[in] selected_cipher_suite The selected cipher suite.
 * \param[in] current_cipher_suite The currently active cipher suite instance.
 * \param[in] used_extensions The used extensions.
 * \return STL container with sequenced handshake messages
 * \vprivate This method is component private.
 * \context ANY
 * \pre -
 * \reentrant FALSE
 */
amsr::core::Result<ara::core::Vector<std::reference_wrapper<messages::HandshakeMessage>>>
PrepareFlightForServerStateClientHello(TlsCommConfigurationInformation const& config, messages::MessageManager& msg_mgr,
                                       ara::core::Vector<uint8_t> const& rng, std::uint16_t const selected_cipher_suite,
                                       record_protocol::CipherSuite const& current_cipher_suite,
                                       ara::core::Vector<uint8_t> const& used_extensions) noexcept;

/*!
 * \brief Copies all cipher suites from source container to destination container. Destination container is emptied
 * prior to copying.
 * \param[in] source Cipher suite source container
 * \param[in, out] destination Container to store source cipher suites
 * \vprivate This method is component private.
 * \context ANY
 * \pre -
 * \reentrant FALSE
 */
void CopyCipherSuites(ara::core::Vector<uint16_t> const& source, ara::core::Vector<uint16_t>& destination) noexcept;

/*!
 * \brief Assign the source list of cipher suites to the cipher suite destination container.
 * \param[in, out] destination Container to store source cipher suites
 * \param[in] source Cipher suite source container
 * \param[in] source_size Size of cipher suite source container
 * \vprivate This method is component private.
 * \context ANY
 * \pre -
 * \reentrant FALSE
 */
void AssignCipherSuite(ara::core::Vector<uint16_t>& destination, ArrOfSuites const& source,
                       std::size_t source_size) noexcept;

/*!
 * \brief Searches for the specified cipher suite in a container of cipher suites.
 * \param[in] cipher_suites A container of cipher suites.
 * \param[in] suite The cipher suite to search for.
 * \return True if container includes the specified cipher suite.
 * \vprivate This method is component private.
 * \context ANY
 * \pre -
 * \reentrant FALSE
 */
bool FindCipherSuite(ara::core::Vector<uint16_t> const& cipher_suites, uint16_t const suite) noexcept;

/*!
 * \brief Verifies the most recent received finished message from a client
 * \details If this function is called with invalid pointer arguments amsr::core::Abort will be called
 * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
 * \param[in] ref_hsm Reference to handshake state machine
 * \param[in] ref_msg_mgr Reference to message manager
 * \param[in] cipher_suite The currently selected cipher suite.
 * \return True if verification succeeded, otherwise false.
 * \context ANY
 * \pre cxt has no previous error information
 * \reentrant FALSE
 * \vprivate This method is component private.
 */
bool VerifyClientFinishedMessage(common::TlsCommFlowContext& cxt, HandshakeStateMachineInterface& ref_hsm,
                                 messages::MessageManager& ref_msg_mgr,
                                 SupportedCipherSuites const cipher_suite) noexcept;

/*!
 * \brief Generates the key material for the different cipher suites by calling crypto lib.
 * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
 * \param[in] master_secret_container The Master Secret to use
 * \param[in] mess_mngr The MessageManager used in the negotiation.
 * \param[in] key_block_size The size of the return key_block
 * \param[in] crypto_adapter Crypto adapter to use.
 * \param[in] algorithm The hash algorithm to be used.
 * \return The vector containing the key block
 * \context ANY
 * \pre -
 * \reentrant FALSE
 * \error TlsCommErrorCode::kTlsCryptoAdapterFailure is reported if the creation of the PRF function failed
 * or if the Generate method on the PRF function failed.
 * \trace SPEC-6493130, SPEC-6493128, SPEC-6561975
 * \vprivate This method is component private.
 */
ara::core::Vector<uint8_t> GenerateKeyBlock(
    common::TlsCommFlowContext& cxt, tls_crypto_adapter::internal::MasterSecretContainer const& master_secret_container,
    messages::MessageManager& mess_mngr, std::size_t key_block_size,
    tls_crypto_adapter::internal::CryptoAdapterInterface const& crypto_adapter,
    tls_crypto_adapter::internal::EnPrfAlgorithm algorithm) noexcept;

/*!
 * \brief Generates a hash message required for a Finish message verify data from a serialized data buffer
 * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
 * \param[in] hash_in_data Serialized data buffer
 * \param[in, out] hash_out_data Reference to destination buffer for hash message
 * \param[in] hash_interface Pointer to hashing interface
 * \context ANY
 * \pre hash_interface must not be nullptr
 * \reentrant FALSE
 * \error TlsCommErrorCode::kTlsCryptoAdapterFailure is reported if an error occurred during the hashing process
 * \vprivate This method is component private.
 */
void GenerateVerifyDataHash(common::TlsCommFlowContext& cxt, ReadOnlyBufferView hash_in_data,
                            WriteableBufferView const& hash_out_data,
                            tls_crypto_adapter::internal::HashInterface* hash_interface) noexcept;

/*!
 * \brief Generates random data with a PRF interface (TlsPrfSha256 or TlsPrfSha384) from provided seed data and outputs
 * the result to the passed destination buffer.
 * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
 * \param[in] ref_hsm Reference to a handshake state machine
 * \param[in] entity The entity to generate data for (Client or Server)
 * \param[in] seed_buffer Buffer with seed data to use for generating random data
 * \param[in, out] destination Output destination buffer to store generated data
 * \param[in] algorithm The hash algorithm to be used.
 * \context ANY
 * \pre -
 * \reentrant FALSE
 * \error TlsCommErrorCode::kTlsCryptoAdapterFailure is reported if an error occurred during the data generation process
 * \vprivate This method is component private.
 */
void GenerateVerifyDataPRF(common::TlsCommFlowContext& cxt, HandshakeStateMachineInterface& ref_hsm,
                           record_protocol::ConnectionEnd const entity, ReadOnlyBufferView const seed_buffer,
                           WriteableBufferView const& destination,
                           tls_crypto_adapter::internal::EnPrfAlgorithm algorithm) noexcept;

/*!
 * \brief Creates a new SecurityParameter struct from the handshake state machine and message manager
 * used in a handshake negotiation.
 * \details The created SecurityParameters are written into the state machines next_security_params_ variable.
 * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
 * \param[in] ref_state_machine The HandshakeStateMachine used in the negotiation.
 * \param[in] ref_msg_mgr The MessageManager used in the negotiation.
 * \param[in] entity The connection entity which will be written into the security parameters.
 * \context ANY
 * \pre -
 * \reentrant FALSE
 * \error TlsCommErrorCode::kTlsErrInvalidArgument is reported if cipher suites not valid
 * \trace SPEC-5506685, SPEC-6181150
 * \vprivate This method is component private.
 */
void CreateSecurityParams(common::TlsCommFlowContext& cxt, HandshakeStateMachineInterface& ref_state_machine,
                          messages::MessageManager& ref_msg_mgr, record_protocol::ConnectionEnd entity) noexcept;

/*!
 * \brief Calculates Master Secret for PSK from Crypto library to be used while creating security parameters
 * \details
 * 1. Uses psk_id_hint from received ServerKeyExchange message and gets the psk_id from Crypto Library.
 * 2. Call GenerateMasterSecretSeed.
 * 3. Calls the Crypto Lib function GenerateMasterSecretFromPSK() with the arguments mentioned above.
 * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
 * \param[in] ref_state_machine The HandshakeStateMachine used in the negotiation.
 * \param[in] ref_msg_mgr The MessageManager used in the negotiation.
 * \context ANY
 * \pre -
 * \reentrant FALSE
 * \error TlsCommErrorCode::kTlsPskIdentityNotFound is reported if the PskIdentity could not found and
 * generating the master secret key failed
 * \vprivate This method is component private.
 */
void CalculateMasterSecretPSK(common::TlsCommFlowContext& cxt, HandshakeStateMachineInterface& ref_state_machine,
                              messages::MessageManager& ref_msg_mgr) noexcept;

/*!
 * \brief Get the correct own private and partner public keys based on the EnCommRole of HandshakeStateMachine.
 * \param[in] ref_state_machine The HandshakeStateMachine used in the negotiation.
 * \param[in] ref_msg_mgr The MessageManager used in the negotiation.
 * \return A pair of own private and partner public keys.
 * \context ANY
 * \pre -
 * \reentrant FALSE
 * \vprivate This method is component private.
 */
PrivatePublicKeyPair GetOwnPrivatePartnerPublicECDHEKeys(HandshakeStateMachineInterface const& ref_state_machine,
                                                         messages::MessageManager& ref_msg_mgr) noexcept;

/*!
 * \brief Calculates Master Secret for ECDHE from Crypto library to be used while creating security parameters
 * \details
 * 1. Uses public_value from received ServerKeyExchange message and gets the private_value from DH Key Exchange.
 * 2. Call GenerateMasterSecretSeed
 * 3. Calls the Crypto Lib function GenerateMasterSecret() with the arguments mentioned above.
 * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
 * \param[in] ref_state_machine The HandshakeStateMachine used in the negotiation.
 * \param[in] ref_msg_mgr The MessageManager used in the negotiation.
 * \context ANY
 * \pre -
 * \reentrant FALSE
 * \error TlsCommErrorCode::kTlsPskIdentityNotFound is reported if the PskIdentity could not found and
 * generating the master secret key failed
 * \vprivate This method is component private.
 */
void CalculateMasterSecretECDHE(common::TlsCommFlowContext& cxt, HandshakeStateMachineInterface& ref_state_machine,
                                messages::MessageManager& ref_msg_mgr) noexcept;

/*!
 * \brief Generates seed used while creating master secret
 * \details Gets the random bytes from ClientHello Message and ServerHello Message and creates a seed.
 * \param[in] ref_msg_mgr A reference to the message manager
 * \context ANY
 * \pre -
 * \reentrant FALSE
 * \vprivate This method is component private.
 * \return A container with the generated seed data.
 */
ara::core::Vector<std::uint8_t> GenerateMasterSecretSeed(messages::MessageManager& ref_msg_mgr) noexcept;

/*!
 * \brief Splits the keyblock into keys and assigns them to security parameters
 * \details
 * 1. Use security parameters to determine how the keyblock should be split into keys
 * 2. Call WriteKey with the key to be written
 * \param[in,out] sec_params A reference to the security parameters
 * \param[in] key_block_iterator A iterator to the key block
 * \context ANY
 * \pre -
 * \reentrant FALSE
 * \vprivate This method is component private.
 */
void SplitKeys(record_protocol::SecurityParameters& sec_params,
               ara::core::Vector<uint8_t>::iterator key_block_iterator) noexcept;

/*!
 * \brief A helper function for writing key data to target.
 * \details Will clear and reserve target before calling insert, finally advances the key block iterator "length" steps
 * \param[in] key_block_iterator A reference to the key block iterator
 * \param[in] length The key length
 * \param[out] target The target for writing key
 * \context ANY
 * \pre -
 * \reentrant FALSE
 * \vprivate This method is component private.
 */
void WriteKey(ara::core::Vector<uint8_t>::iterator& key_block_iterator, std::uint8_t length,
              ara::core::Vector<uint8_t>& target) noexcept;

/*!
 * \brief A helper function to handle the TLS_NULL_WITH_NULL_NULL cipher suite.
 * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
 * \param[in] ref_state_machine The HandshakeStateMachine used in the negotiation.
 * \param[in] ref_msg_mgr The MessageManager used in the negotiation.
 * \param[in, out] sec_params The security parameters.
 * \context Handshake
 * \pre -
 * \reentrant FALSE
 * \vprivate This method is component private.
 */
void HandleTlsNullWithNullNull(common::TlsCommFlowContext& cxt, HandshakeStateMachineInterface& ref_state_machine,
                               messages::MessageManager& ref_msg_mgr,
                               record_protocol::SecurityParameters& sec_params) noexcept;

/*!
 * \brief A helper function to handle the TLS_PSK_WITH_NULL_SHA256 cipher suite.
 * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
 * \param[in] ref_state_machine The HandshakeStateMachine used in the negotiation.
 * \param[in] ref_msg_mgr The MessageManager used in the negotiation.
 * \param[in, out] sec_params The security parameters.
 * \context Handshake
 * \pre -
 * \reentrant FALSE
 * \vprivate This method is component private.
 */
void HandleTlsPskWithNullSha256(common::TlsCommFlowContext& cxt, HandshakeStateMachineInterface& ref_state_machine,
                                messages::MessageManager& ref_msg_mgr,
                                record_protocol::SecurityParameters& sec_params) noexcept;

/*!
 * \brief A helper function to handle the TLS_PSK_WITH_AES_128_GCM_SHA256 cipher suite.
 * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
 * \param[in] ref_state_machine The HandshakeStateMachine used in the negotiation.
 * \param[in] ref_msg_mgr The MessageManager used in the negotiation.
 * \param[in, out] sec_params The security parameters.
 * \context Handshake
 * \pre -
 * \reentrant FALSE
 * \vprivate This method is component private.
 */
void HandleTlsPskWithAes128GcmSha256(common::TlsCommFlowContext& cxt, HandshakeStateMachineInterface& ref_state_machine,
                                     messages::MessageManager& ref_msg_mgr,
                                     record_protocol::SecurityParameters& sec_params) noexcept;
/*!
 * \brief A helper function to handle the TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256 cipher suite.
 * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
 * \param[in] ref_state_machine The HandshakeStateMachine used in the negotiation.
 * \param[in] ref_msg_mgr The MessageManager used in the negotiation.
 * \param[in, out] sec_params The security parameters.
 * \context Handshake
 * \pre -
 * \reentrant FALSE
 * \vprivate This method is component private.
 */
void HandleTlsEcdheWithAes128GcmSha256(common::TlsCommFlowContext& cxt,
                                       HandshakeStateMachineInterface& ref_state_machine,
                                       messages::MessageManager& ref_msg_mgr,
                                       record_protocol::SecurityParameters& sec_params) noexcept;

/*!
 * \brief A helper function to handle the TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384 cipher suite.
 * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
 * \param[in] ref_state_machine The HandshakeStateMachine used in the negotiation.
 * \param[in] ref_msg_mgr The MessageManager used in the negotiation.
 * \param[in, out] sec_params The security parameters.
 * \context Handshake
 * \pre -
 * \reentrant FALSE
 * \vprivate This method is component private.
 */
void HandleTlsEcdheWithAes256GcmSha384(common::TlsCommFlowContext& cxt,
                                       HandshakeStateMachineInterface& ref_state_machine,
                                       messages::MessageManager& ref_msg_mgr,
                                       record_protocol::SecurityParameters& sec_params) noexcept;

/*!
 * \brief A helper function to handle the TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256 cipher suite.
 * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
 * \param[in] ref_state_machine The HandshakeStateMachine used in the negotiation.
 * \param[in] ref_msg_mgr The MessageManager used in the negotiation.
 * \param[in, out] sec_params The security parameters.
 * \context Handshake
 * \pre -
 * \reentrant FALSE
 * \vprivate This method is component private.
 */
void HandleTlsEcdheWithAes128CbcSha256(common::TlsCommFlowContext& cxt,
                                       HandshakeStateMachineInterface& ref_state_machine,
                                       messages::MessageManager& ref_msg_mgr,
                                       record_protocol::SecurityParameters& sec_params) noexcept;

/*!
 * \brief A helper function to handle the TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384 cipher suite.
 * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
 * \param[in] ref_state_machine The HandshakeStateMachine used in the negotiation.
 * \param[in] ref_msg_mgr The MessageManager used in the negotiation.
 * \param[in, out] sec_params The security parameters.
 * \context Handshake
 * \pre -
 * \reentrant FALSE
 * \vprivate This method is component private.
 */
void HandleTlsEcdheWithAes256CbcSha384(common::TlsCommFlowContext& cxt,
                                       HandshakeStateMachineInterface& ref_state_machine,
                                       messages::MessageManager& ref_msg_mgr,
                                       record_protocol::SecurityParameters& sec_params) noexcept;

/*!
 * \brief A helper function to handle the TLS_ECDHE_ECDSA_WITH_NULL_SHA1 cipher suite.
 * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
 * \param[in] ref_state_machine The HandshakeStateMachine used in the negotiation.
 * \param[in] ref_msg_mgr The MessageManager used in the negotiation.
 * \param[in, out] sec_params The security parameters.
 * \context Handshake
 * \pre -
 * \reentrant FALSE
 * \vprivate This method is component private.
 */
void HandleTlsEcdheWithNullSha1(common::TlsCommFlowContext& cxt, HandshakeStateMachineInterface& ref_state_machine,
                                messages::MessageManager& ref_msg_mgr,
                                record_protocol::SecurityParameters& sec_params) noexcept;

/*!
 * \brief A helper function to generate and split the key block.
 * \param[in,out] cxt TlsCommFlowContext used for tracing internal logic flow and error reporting
 * \param[in] ref_state_machine The HandshakeStateMachine used in the negotiation.
 * \param[in] ref_msg_mgr The MessageManager used in the negotiation.
 * \param[in, out] sec_params The security parameters.
 * \param[in] algorithm The hash algorithm to be used.
 * \context Handshake
 * \pre cxt shall be error free.
 * \reentrant FALSE
 * \vprivate This method is component private.
 */
void CreateWriteReadKeys(common::TlsCommFlowContext& cxt, HandshakeStateMachineInterface& ref_state_machine,
                         messages::MessageManager& ref_msg_mgr, record_protocol::SecurityParameters& sec_params,
                         tls_crypto_adapter::internal::EnPrfAlgorithm algorithm) noexcept;

/*!
 * \brief Helper for generating the master secret for PSK.
 * \param[in] crypto_adapter A crypto adapter to use.
 * \param[in] uuid The pre-shared key UUID.
 * \param[in] seed Seeding data.
 * \param[out] destination The destination buffer to fill with the generated master secret.
 * \param[in] algorithm The hash algorithm to be used.
 * \context Internal
 * \pre -
 * \threadsafe False
 * \reentrant False
 * \synchronous True
 * \return An error code providing information about success or failure of the operation.
 * \vprivate This method is component private.
 */
tls_crypto_adapter::internal::CryptoAdapterErrorCode GenerateMasterSecretFromPSK(
    tls_crypto_adapter::internal::CryptoAdapterInterface& crypto_adapter, ara::crypto::Uuid const& uuid,
    ReadOnlyBufferView seed, tls_crypto_adapter::internal::MasterSecretContainer& destination,
    tls_crypto_adapter::internal::EnPrfAlgorithm algorithm) noexcept;

/*!
 * \brief It's a delegated call for generating the master secret for ECDHE.
 * \param[in] crypto_adapter A crypto adapter to use.
 * \param[in] own_private_key Our own private key which is used for generating pre-master secret.
 * \param[in] partner_public_key The client's public key which is used for generating pre-master secret.
 * \param[in] seed Seeding data.
 * \param[out] destination The destination buffer to fill with the generated master secret.
 * \param[in] algorithm The hash algorithm to be used.
 * \context Internal
 * \pre -
 * \threadsafe False
 * \reentrant False
 * \synchronous True
 * \return An error code providing information about success or failure of the operation.
 * \vprivate This method is component private.
 */
tls_crypto_adapter::internal::CryptoAdapterErrorCode GenerateMasterSecretFromECDHE(
    tls_crypto_adapter::internal::CryptoAdapterInterface& crypto_adapter,
    ara::crypto::cryp::PrivateKey::Uptrc own_private_key, ara::crypto::ReadOnlyMemRegion partner_public_key,
    ReadOnlyBufferView seed, tls_crypto_adapter::internal::MasterSecretContainer& destination,
    tls_crypto_adapter::internal::EnPrfAlgorithm algorithm) noexcept;

/*!
 * \brief Deserialize the extensions and use them for communication.
 * \param[in] cxt TlsCommFlowContext to log contextual information
 * \param[out] extensions_checked boolean value for extension checked
 * \param[in] ref_state_machine The HandshakeStateMachine used in the negotiation.
 * \param[in] extensions The range of bytes containing the extensions values.
 * \context ANY
 * \pre -
 * \reentrant FALSE
 * \vprivate This method is component private.
 * \threadsafe FALSE
 */
void ObtainExtensionData(common::TlsCommFlowContext& cxt, bool& extensions_checked,
                         HandshakeStateMachineInterface& ref_state_machine,
                         amsr::core::Span<std::uint8_t const> extensions) noexcept;

/*!
 * \brief Prepare and set ClientHello Extensions if any Certificate-Based Cipher Suite is used.
 * \param[in] client_hello_msg Reference to ClientHelloMessage.
 * \context ANY
 * \pre -
 * \reentrant FALSE
 */
void SetCertificateRelatedClientHelloExtensionsIfNeeded(messages::ClientHelloMessage& client_hello_msg) noexcept;

}  // namespace utility
}  // namespace handshakestatemachine
}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_HANDSHAKESTATEMACHINE_HANDSHAKE_STATE_UTILITY_H_
