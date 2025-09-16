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
/*!        \file  intermediate_method_object.h
 *        \brief  Internal representation of the service method object.
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_INTERMEDIATE_METHOD_OBJECT_H_
#define LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_INTERMEDIATE_METHOD_OBJECT_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "amsr/someip_daemon_core/configuration/cfg_element.h"
#include "amsr/someip_daemon_core/configuration/configuration_types.h"

namespace amsr {
namespace someip_daemon_core {
namespace configuration {
namespace model {

/*!
 * \brief This is an intermediate representation of a method object which is filled by a parser.
 * \details The validator then takes this object and validates if mandatory elements are set or not.
 */
class MethodConfigObject final {
 public:
  /*!
   * \brief Method ID element of this service.
   */
  using MethodIdElement = CfgElement<someip_protocol::internal::MethodId>;

  /*!
   * \brief Protocol used for this method (UDP/IP or TCP/IP)
   */
  using ProtoElement = CfgElement<ConfigurationTypesAndDefs::Protocol>;

  /*!
   * \brief SOME/IP-TP element.
   */
  using SomeIpTpElement = CfgElement<ConfigurationTypesAndDefs::MethodSomeIpTp>;

  /*!
   * \brief Constructor sets the references to the elements of the structure to write to.
   * \param[in] method Reference to the method structure to write to.
   * \pre -
   * \context Init
   * \reentrant FALSE
   * \steady FALSE
   */
  explicit MethodConfigObject(ConfigurationTypesAndDefs::Method& method) noexcept
      : method_id_{method.id_}, proto_{method.proto_}, someip_tp_{method.someip_tp_} {}

  /*!
   * \brief Default destructor.
   * \steady FALSE
   */
  ~MethodConfigObject() = default;

  /*!
   * \brief Deleted copy constructor.
   */
  MethodConfigObject(MethodConfigObject const&) = delete;

  /*!
   * \brief Deleted move constructor.
   */
  MethodConfigObject(MethodConfigObject&&) = delete;

  /*!
   * \brief Deleted move assignment operator.
   */
  auto operator=(MethodConfigObject const&) & -> MethodConfigObject& = delete;

  /*!
   * \brief Move assignment is used for resetting.
   * \steady FALSE
   */
  auto operator=(MethodConfigObject&& other) & noexcept -> MethodConfigObject& = default;

  /*!
   * \brief Set the method ID.
   * \param[in] method_id The SOME/IP Method ID to be set.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetMethodId(someip_protocol::internal::MethodId const& method_id) noexcept { method_id_.SetElement(method_id); }

  /*!
   * \brief Getter for the method ID element.
   * \return Reference to the MethodId of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  MethodIdElement const& GetMethodId() const noexcept { return method_id_; }

  /*!
   * \brief Set the protocol.
   * \param[in] proto Protocol type to be set.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetProto(ConfigurationTypesAndDefs::Protocol const& proto) noexcept { proto_.SetElement(proto); }

  /*!
   * \brief Getter for the protocol.
   * \return Reference to the Protocol of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  ProtoElement const& GetProto() const noexcept { return proto_; }

  /*!
   * \brief Set the SOME/IP-TP configuration.
   * \param[in] someip_tp SOME/IP-TP configurations for method.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  void SetSomeIpTp(ConfigurationTypesAndDefs::MethodSomeIpTp const& someip_tp) noexcept {
    someip_tp_.SetElement(someip_tp);
  }

  /*!
   * \brief Getter for the SOME/IP-TP configuration.
   * \return Reference to the SOME/IP-TP configurations of this intermediate object.
   * \pre -
   * \context ANY
   * \reentrant FALSE
   * \steady FALSE
   */
  SomeIpTpElement const& GetSomeIpTp() const noexcept { return someip_tp_; }

 private:
  /*!
   * \brief The method ID element.
   */
  MethodIdElement method_id_;

  /*!
   * \brief The service method's protocol.
   */
  ProtoElement proto_;

  /*!
   * \brief SOME/IP-TP configured for this method.
   */
  SomeIpTpElement someip_tp_;
};

}  // namespace model
}  // namespace configuration
}  // namespace someip_daemon_core
}  // namespace amsr

#endif  // LIB_SOMEIP_DAEMON_CORE_INCLUDE_AMSR_SOMEIP_DAEMON_CORE_CONFIGURATION_MODEL_INTERMEDIATE_METHOD_OBJECT_H_
