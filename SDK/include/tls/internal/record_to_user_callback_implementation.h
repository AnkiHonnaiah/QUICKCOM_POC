
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
/*!
 *             \file  record_to_user_callback_implementation.h
 *            \brief  Implementation of the RecordToUserCallbackInterface.
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_TO_USER_CALLBACK_IMPLEMENTATION_H_
#define LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_TO_USER_CALLBACK_IMPLEMENTATION_H_

#include "tls/internal/record_protocol/record_protocol_interfaces.h"

namespace amsr {
namespace tls {
namespace internal {
/*!
 * \brief Implementation of the RecordToUserCallbackInterface
 */
template <typename ComProvider>
class RecordToUserCallbackImpl final : public record_protocol::RecordToTlsUserCallbackInterface {
 public:
  /*!
   * \brief Constructor for the Record Protocol callback implementation
   * \param[in] instance_owner Reference to the parent of this callback
   * \context ANY
   * \threadsafe FALSE
   * \reentrant FALSE
   * \pre -
   */
  explicit RecordToUserCallbackImpl(ComProvider& instance_owner) noexcept
      : RecordToTlsUserCallbackInterface(), instance_owner_{instance_owner} {}

  /*! \brief Not copy constructible. */
  RecordToUserCallbackImpl(RecordToUserCallbackImpl const&) = delete;

  /*! \brief Not move constructible. */
  RecordToUserCallbackImpl(RecordToUserCallbackImpl&&) = delete;

  /*! \brief Not copy assignable. */
  RecordToUserCallbackImpl& operator=(RecordToUserCallbackImpl const&) = delete;

  /*! \brief Not move assignable. */
  RecordToUserCallbackImpl& operator=(RecordToUserCallbackImpl&&) = delete;

  /*! \brief Destructor override for the RecordProtocol callback implementation. Default destructor. */
  ~RecordToUserCallbackImpl() final = default;
  /*!
   * \internal
   * \brief Call the Tls Extensions function of user callback
   * \param[in] buffer A read only buffer view
   * \endinternal
   */
  void OnSendToTransport(ReadOnlyBufferView buffer) noexcept final {
    instance_owner_.GetPtrToUserCallback()->WriteToTransport(buffer);
  }

  /*!
   * \internal
   * \brief Call the Tls Extensions function of user callback
   * \param[in] buffer A read only buffer view
   * \endinternal
   */
  void OnSendToCommParty(ReadOnlyBufferView buffer) noexcept final {
    instance_owner_.GetPtrToUserCallback()->WriteToCommParty(buffer);
  }

 private:
  /*! \brief A reference to a TlsClientCommProvider instance */
  ComProvider& instance_owner_;
};

}  // namespace internal
}  // namespace tls
}  // namespace amsr

#endif  // LIB_LIBTLS_INCLUDE_TLS_INTERNAL_RECORD_TO_USER_CALLBACK_IMPLEMENTATION_H_
