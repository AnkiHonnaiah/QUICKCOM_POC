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
/*!     \file   someip_binding_xf/internal/methods/skeleton_response_handler.h
 *      \brief  Handle method response.
 *      \unit   SomeIpBinding::SomeIpBindingXf::Communication::Methods::SkeletonMethodResponseHandler
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_SKELETON_RESPONSE_HANDLER_H_
#define LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_SKELETON_RESPONSE_HANDLER_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <utility>
#include "amsr/core/error_domain.h"
#include "amsr/core/string.h"
#include "amsr/socal/internal/methods/skeleton_response_handler_interface.h"
#include "amsr/someip_binding_core/internal/local_server_interface.h"
#include "amsr/someip_binding_core/internal/logging/ara_com_logger.h"
#include "amsr/someip_binding_core/internal/logging/log_builder.h"
#include "amsr/someip_binding_core/internal/methods/request.h"
#include "amsr/someip_binding_core/internal/types.h"
#include "amsr/someip_binding_xf/internal/methods/types.h"
#include "amsr/someip_binding_xf/internal/ser_deser/method_response_serializer_interface.h"
#include "ara/com/com_error_domain.h"
#include "ara/core/string_view.h"
#include "someip-protocol/internal/ap_application_error.h"
#include "someip-protocol/internal/serialization/ser_someip_header.h"
#include "someip-protocol/internal/serialization/writer.h"

namespace amsr {
namespace someip_binding_xf {
namespace internal {
namespace methods {

/*!
 * \brief   ResponseHandler class for handle method response.
 *
 * \tparam  ResponseDataType Method response data type.
 */
template <typename ResponseDataType>
class SkeletonResponseHandler final
    : public ::amsr::socal::internal::methods::SkeletonResponseHandlerInterface<ResponseDataType> {
 public:
  /*!
   * \brief Application Error.
   */
  using ApApplicationError = ::amsr::someip_protocol::internal::serialization::ApApplicationError;
  /*!
   * \brief Serializer interface with ResponseDataType.
   */
  using ResponseSerializer =
      ::amsr::someip_binding_xf::internal::ser_deser::MethodResponseSerializerInterface<ResponseDataType>;

  /*!
   * \brief Serializer interface with application error.
   */
  using ApplicationErrorSerializer =
      ::amsr::someip_binding_xf::internal::ser_deser::MethodResponseSerializerInterface<ApApplicationError>;

  /*!
   * \brief Type alias for the concrete memory buffer type.
   */
  using BufferPtrType = ::amsr::someip_binding_core::internal::MemoryBufferPtr;

  /*!
   * \brief Writer.
   */
  using WriterType = ::amsr::someip_protocol::internal::serialization::Writer;

  /*!
   * \brief An alias to the map of errordomain.
   */
  using ApplicationErrorMap =
      std::map<amsr::core::ErrorDomain::IdType, ::amsr::someip_binding_xf::internal::methods::ApplicationErrorDomain>;

  /*!
   * \brief Type alias for SkeletonResponseHandlerInterface.
   */
  using SkeletonResponseHandlerInterface =
      ::amsr::socal::internal::methods::SkeletonResponseHandlerInterface<ResponseDataType>;

  SkeletonResponseHandler() = delete;
  SkeletonResponseHandler(SkeletonResponseHandler const&) = delete;
  SkeletonResponseHandler(SkeletonResponseHandler&&) = delete;
  SkeletonResponseHandler& operator=(SkeletonResponseHandler const&) = delete;
  SkeletonResponseHandler& operator=(SkeletonResponseHandler&&) = delete;

  /*!
   * \brief Constructor.
   * \param[in] instance_id           Instance ID.
   * \param[in] request_header        Header of the corresponding SOME/IP method request (used to create the response
                                      header).
   * \param[in] server                Server interface type.
   * \param[in] serializer            Serializer for the method response payload.
   * \param[in] app_error_serializer  Serializer for application error response.
   * \param[in] allocator             Allocator for memory used to serialize/send positive response or application
                                      error.
   * \param[in] application_error_map Application errors referenced by this method.
   * \param[in] requester_address     Contains the IP address and port of the sender of the method request.
   * \pre         -
   * \context     Reactor
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   */
  SkeletonResponseHandler(
      ::amsr::someip_protocol::internal::InstanceId const instance_id,
      ::amsr::someip_protocol::internal::SomeIpMessageHeader const& request_header,
      ::amsr::someip_binding_core::internal::LocalServerInterface& server, ResponseSerializer& serializer,
      ApplicationErrorSerializer& app_error_serializer,
      amsr::someip_binding_core::internal::MemoryBufferAllocator& allocator,
      ApplicationErrorMap const& application_error_map,
      ::amsr::someip_binding_core::internal::methods::RequesterAddress const& requester_address) noexcept
      : SkeletonResponseHandlerInterface{},
        instance_id_{instance_id},
        request_header_{request_header},
        server_{server},
        response_serializer_{serializer},
        app_error_serializer_{app_error_serializer},
        allocator_{allocator},
        // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
        application_error_map_{application_error_map},
        requester_address_{requester_address} {}

  /*!
   * \brief Destructor.
   *
   * \pre The user must call one of the three APIs HandleMethodResponse, HandleApplicationError, HandleErrorResponse.
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   TRUE
   * \synchronous TRUE
   * \steady      TRUE
   */
  // VCA_SLC_23_SOMEIPBINDING_DEFAULT_CTOR_DTOR_MOLE_1298
  ~SkeletonResponseHandler() noexcept final {
    if (!response_sent_) {
      logger_.LogFatalAndAbort(
          [this](::amsr::someip_binding_core::internal::logging::StringStream& s) {
            s << "The user must call one of the three APIs HandleMethodResponse, HandleApplicationError, "
                 "HandleErrorResponse. Address of response_sent_: ";
            s << response_sent_;
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

 private:
  /*!
   * \brief Create and send a serialized method response for a processed method request.
   *
   * \param[in] response  The method result received.
   *
   * \pre  The user did not call one of the three APIs HandleMethodResponse, HandleApplicationError,
   *       HandleErrorResponse, yet.
   * \context          App
   * \threadsafe       FALSE for same class instance, TRUE for different instances.
   * \reentrant        FALSE for same class instance, TRUE for different instances.
   * \synchronous      TRUE
   * \steady           TRUE
   * \exceptionsafety  STRONG No undefined behavior or side effects.
   * \vprivate Product Private
   *
   * \internal
   * - Prepare the serialized response message from the given header and payload.
   * - Serialize the Header.
   * - Serialize the payload.
   * - Send the response message via server manager.
   * \endinternal
   */
  void HandleMethodResponse(ResponseDataType const&& response) noexcept final {
    if (response_sent_) {
      logger_.LogFatalAndAbort(
          [](::amsr::someip_binding_core::internal::logging::StringStream& s) {
            s << "One of the three APIs HandleMethodResponse, HandleApplicationError, "
                 "HandleErrorResponse was already called.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    response_sent_ = true;
    // Copy the method request SOME/IP header & set message type to kResponse.
    ::amsr::someip_protocol::internal::SomeIpMessageHeader response_header{request_header_};
    response_header.message_type_ = ::amsr::someip_protocol::internal::SomeIpMessageType::kResponse;

    // Calculate the required size for the response data
    // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
    std::size_t const payload_size{response_serializer_.GetRequiredBufferSize(response)};
    // Calculate response header length
    response_header.length_ = ::amsr::someip_protocol::internal::kMinimumPayloadLength +
                              static_cast<someip_protocol::internal::LengthField>(payload_size);
    std::size_t const alloc_size{::amsr::someip_protocol::internal::kHeaderSize + payload_size};
    // Allocate memory for method response message
    // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
    amsr::core::Result<BufferPtrType> packet{allocator_.Allocate(alloc_size)};
    if (packet.HasValue()) {
      // VECTOR NC AutosarC++17_10-M0.3.1: MD_SOMEIPBINDING_AutosarC++17_10-M0.3.1_ptr_null_at_dereference_fp
      // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
      ::amsr::someip_binding_core::internal::MemoryBuffer::MemoryBufferView const body_view{
          // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
          packet.Value()->GetView(0U)};
      WriterType writer{body_view};

      // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
      ::amsr::someip_protocol::internal::serialization::SerializeSomeIpMessageHeaderObject(writer, response_header);
      // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
      response_serializer_.Serialize(writer, response);
      // VCA_SOMEIPBINDING_SERVER_MANAGER_OBJECT_LIFECYCLE
      server_.SendMethodResponse(std::move(packet.Value()), requester_address_);
    } else {
      logger_.LogFatalAndAbort(
          [](::amsr::someip_binding_core::internal::logging::StringStream& s) { s << "Failed to allocate buffer."; },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief Create and send an ApplicationError method response.
   *
   * \details In case of ApApplicationErrors provided which are not configured, The application will abort when the
   *          forwarded error is received on client application.
   *
   * \param[in]  error_code The ApApplicationError error code returned by the skeleton implementation. Only configured
   *             ApApplicationErrors must be provided.
   *
   * \pre The user did not call one of the three APIs HandleMethodResponse, HandleApplicationError,
   *      HandleErrorResponse, yet.
   *
   * \context          App
   * \threadsafe       FALSE for same class instance, TRUE for different instances.
   * \reentrant        FALSE for same class instance, TRUE for different instances.
   * \synchronous      TRUE
   * \steady           TRUE
   * \vprivate Product Private
   *
   * \internal
   * - Prepare the application error response message header for the variant ApApplicationError.
   * - Send the application error response message for the corresponding method request.
   * - Abort in case of an unexpected error is received.
   * \endinternal
   */
  void HandleApplicationError(::amsr::core::ErrorCode const& error_code) noexcept final {
    if (response_sent_) {
      logger_.LogFatalAndAbort(
          [](::amsr::someip_binding_core::internal::logging::StringStream& s) {
            s << "The user should call one of the three APIs HandleMethodResponse, HandleApplicationError, "
                 "HandleErrorResponse.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    response_sent_ = true;
    if (IsErrorCodeReferenced(application_error_map_, error_code)) {
      // Copy the method request SOME/IP header & set message type to kError.
      ::amsr::someip_protocol::internal::SomeIpMessageHeader response_header{request_header_};
      response_header.message_type_ = ::amsr::someip_protocol::internal::SomeIpMessageType::kError;

      // [SWS_CM_10312] The Return Code shall be set to E_NOT_OK (0x01) in case the ClientServerOperation raised one
      // of the possible ApApplicationErrors referenced by the ClientServerOperation in role possibleError.
      response_header.return_code_ = ::amsr::someip_protocol::internal::SomeIpReturnCode::kNotOk;
      ara::core::StringView const error_code_string_view{error_code.UserMessage()};
      amsr::core::String const error_code_string{error_code_string_view};
      ::amsr::someip_protocol::internal::serialization::ApApplicationErrorStruct const error_struct{
          error_code.Domain().Id(), error_code.Value(), error_code.SupportData(), error_code_string.c_str()};
      ::amsr::someip_protocol::internal::serialization::ApApplicationError const ap_application_error{error_struct};

      // Calculate the required size for the response data
      // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
      std::size_t const payload_size{app_error_serializer_.GetRequiredBufferSize(ap_application_error)};
      // Calculate response header length
      response_header.length_ = ::amsr::someip_protocol::internal::kMinimumPayloadLength +
                                static_cast<someip_protocol::internal::LengthField>(payload_size);
      std::size_t const alloc_size{::amsr::someip_protocol::internal::kHeaderSize + payload_size};
      // Allocate memory for application error message
      // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
      amsr::core::Result<BufferPtrType> packet{allocator_.Allocate(alloc_size)};
      if (packet.HasValue()) {
        // VECTOR NC AutosarC++17_10-M0.3.1: MD_SOMEIPBINDING_AutosarC++17_10-M0.3.1_ptr_null_at_dereference_fp
        ::amsr::someip_binding_core::internal::MemoryBuffer::MemoryBufferView const body_view{
            // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
            packet.Value()->GetView(0U)};
        WriterType writer{body_view};

        // VCA_SOMEIPBINDING_TRIVIAL_FUNCTION_CONTRACT
        ::amsr::someip_protocol::internal::serialization::SerializeSomeIpMessageHeaderObject(writer, response_header);
        // VCA_SOMEIPBINDING_POSSIBLY_CALLING_NULLPTR_METHOD_CALL_ON_REF
        app_error_serializer_.Serialize(writer, ap_application_error);

        // VCA_SOMEIPBINDING_SERVER_MANAGER_OBJECT_LIFECYCLE
        server_.SendMethodResponse(std::move(packet.Value()), requester_address_);
      } else {
        logger_.LogFatalAndAbort(
            [](::amsr::someip_binding_core::internal::logging::StringStream& s) {
              s << "Failed to allocate memory buffer.";
            },
            static_cast<char const*>(__func__), __LINE__);
      }
    } else {
      logger_.LogFatalAndAbort(
          [this, &error_code](::amsr::someip_binding_core::internal::logging::StringStream& s) {
            s << "Cannot send response with not-referenced application error: ";
            // VCA_SOMEIPBINDING_PASSING_REFERENCE
            ::amsr::someip_binding_core::internal::logging::LogBuilder::LogSomeIpHeader(s, request_header_);
            s << ", Instance ID: 0x";
            s << std::hex;
            s << instance_id_;
            s << ", with error: ";
            s << error_code.Message();
            s << std::dec;
          },
          static_cast<char const*>(__func__), __LINE__);
    }
  }

  /*!
   * \brief Serialize and transmit an error response.
   *
   * \param[in] error_code The error code returned due to failure in method request processing.
   *                       ara::com::ComErrc::kMethodArgumentsNotRetrieved (deserialization error) or
   *                       ara::com::ComErrc::kErrorNotOk (service was stopped or method scheduling error).
   *
   * \pre The user did not call one of the three APIs HandleMethodResponse, HandleApplicationError,
   * HandleErrorResponse, yet.
   *
   * \context     App
   * \threadsafe  FALSE for same class instance, TRUE for different instances.
   * \reentrant   FALSE for same class instance, TRUE for different instances.
   * \synchronous TRUE
   * \steady      TRUE
   * \vprivate Product Private
   *
   * \internal
   * - Transmit a error response if a deserialization error occurred.
   * - Otherwise, transmit a error response if Method scheduling fails or Service is not available.
   * - Abort in case of an unexpected error is received.
   * \endinternal
   */
  void HandleErrorResponse(::amsr::core::ErrorCode const& error_code) noexcept final {
    if (response_sent_) {
      logger_.LogFatalAndAbort(
          [](::amsr::someip_binding_core::internal::logging::StringStream& s) {
            s << "The user should call one of the three APIs HandleMethodResponse, HandleApplicationError, "
                 "HandleErrorResponse.";
          },
          static_cast<char const*>(__func__), __LINE__);
    }
    response_sent_ = true;
    amsr::core::ErrorDomain::IdType const received_error_domain_id{error_code.Domain().Id()};
    if (received_error_domain_id != ara::com::internal::GetComDomain().Id()) {
      logger_.LogFatalAndAbort(
          [this, &error_code](::amsr::someip_binding_core::internal::logging::StringStream& s) {
            s << "Unexpected error received for method request: ";
            // VCA_SOMEIPBINDING_PASSING_REFERENCE
            ::amsr::someip_binding_core::internal::logging::LogBuilder::LogSomeIpHeader(s, request_header_);
            s << ", Instance ID: 0x";
            s << std::hex;
            s << instance_id_;
            s << ", with error: ";
            s << error_code.Message();
            s << std::dec;
          },
          static_cast<char const*>(__func__), __LINE__);
    } else {
      // VECTOR NC AutosarC++17_10-A7.2.1: MD_SOMEIPBINDING_AutosarC++17_10-A7.2.1_outside_range_of_enumerator
      ::ara::com::ComErrc const error_code_value{static_cast<::ara::com::ComErrc>(error_code.Value())};
      ::amsr::someip_protocol::internal::SomeIpReturnCode return_code{
          ::amsr::someip_protocol::internal::SomeIpReturnCode::kNotOk};
      switch (error_code_value) {
        case ::ara::com::ComErrc::kMethodArgumentsNotRetrieved: {
          // Deserialization failed
          return_code = ::amsr::someip_protocol::internal::SomeIpReturnCode::kMalformedMessage;
          break;
        }
        case ::ara::com::ComErrc::kErrorNotOk: {
          // Request could not be scheduled.
          // kNotOk
          break;
        }
        default: {
          logger_.LogFatalAndAbort(
              [this, &error_code](::amsr::someip_binding_core::internal::logging::StringStream& s) {
                s << "Unexpected error received for method request: ";
                // VCA_SOMEIPBINDING_PASSING_REFERENCE
                ::amsr::someip_binding_core::internal::logging::LogBuilder::LogSomeIpHeader(s, request_header_);
                s << ", Instance ID: 0x";
                s << std::hex;
                s << instance_id_;
                s << ", with error: ";
                s << error_code.Message();
                s << std::dec;
              },
              static_cast<char const*>(__func__), __LINE__);
          break;
        }
      }

      // VCA_SOMEIPBINDING_SERVER_MANAGER_OBJECT_LIFECYCLE
      server_.SendErrorResponse(static_cast<::amsr::someip_protocol::internal::ReturnCode>(return_code),
                                request_header_, requester_address_);
    }
  }

  /*!
   * \brief Checks if the error code is contained in the applications error map
   *
   * \param[in]   application_errors Application error domain map.
   * \param[in]   error_code         The ApApplicationError error code returned by the skeleton implementation. Only
   *                                 configured ApApplicationErrors must be provided.
   * \return      true if configured, otherwise false.
   *
   * \pre   -
   * \context     App
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      TRUE
   * \spec
   *   requires true;
   * \endspec
   *
   * \internal
   * - check if the application error is configured or not for this method.
   * - Return true if configured, otherwise false.
   * \endinternal
   */
  static bool IsErrorCodeReferenced(ApplicationErrorMap const& application_errors,
                                    ::amsr::core::ErrorCode const& error_code) noexcept {
    bool validation_result{false};
    typename ApplicationErrorMap::const_iterator const it_domain{application_errors.find(error_code.Domain().Id())};
    if (it_domain != application_errors.cend()) {
      // VCA_SOMEIPBINDING_KEY_FOUND_ITERATOR_OBJECT
      if (it_domain->second.error_codes.find(error_code.Value()) != it_domain->second.error_codes.cend()) {
        validation_result = true;
      }
    }

    return validation_result;
  }

  /*!
   * \brief Type alias for logger.
   */
  using Logger = ::amsr::someip_binding_core::internal::logging::AraComLogger;

  /*!
   * \brief Instance ID.
   */
  ::amsr::someip_protocol::internal::InstanceId const instance_id_;

  /*!
   * \brief Header of the corresponding SOME/IP method request.
   */
  ::amsr::someip_protocol::internal::SomeIpMessageHeader const request_header_;

  /*!
   * \brief Server manager interface.
   */
  ::amsr::someip_binding_core::internal::LocalServerInterface& server_;

  /*!
   * \brief Response serializer.
   */
  ResponseSerializer& response_serializer_;

  /*!
   * \brief Application error serializer.
   */
  ApplicationErrorSerializer& app_error_serializer_;

  /*!
   * \brief Used memory buffer allocator.
   */
  amsr::someip_binding_core::internal::MemoryBufferAllocator& allocator_;

  /*!
   * \brief Application error map.
   */
  ApplicationErrorMap const application_error_map_;

  /*!
   * \brief Requester address.
   */
  ::amsr::someip_binding_core::internal::methods::RequesterAddress requester_address_;
  /*!
   * \brief Flag for check API's call.
   */
  bool response_sent_{false};

  /*!
   * \brief Method logger for printing debug and error messages.
   * The concrete method will set the prefix for identification.
   */
  Logger logger_{::amsr::someip_binding_core::internal::logging::kSomeIpLoggerContextId,
                 ::amsr::someip_binding_core::internal::logging::kSomeIpLoggerContextDescription,
                 ::amsr::core::StringView{"ResponseHandler"}};
};

}  // namespace methods
}  // namespace internal
}  // namespace someip_binding_xf
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_TRANSFORMATION_LAYER_INCLUDE_AMSR_SOMEIP_BINDING_XF_INTERNAL_METHODS_SKELETON_RESPONSE_HANDLER_H_
