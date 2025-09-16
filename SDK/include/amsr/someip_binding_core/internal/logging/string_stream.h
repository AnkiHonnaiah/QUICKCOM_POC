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
/*!        \file  string_stream.h
 *         \unit  SomeIpBinding::SomeIpBindingCore::Utilities
 *
 *********************************************************************************************************************/
#ifndef LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_LOGGING_STRING_STREAM_H_
#define LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_LOGGING_STRING_STREAM_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <sstream>
#include <string>

namespace amsr {
namespace someip_binding_core {
namespace internal {
namespace logging {

/*!
 * \brief Encapsulation class for standard library string stream.
 */
class StringStream final {
 public:
  /*!
   * \brief Underlying string stream type.
   */
  using StringStreamType = std::ostringstream;

  /*!
   * \brief Default constructor.
   */
  // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
  StringStream() = default;

  /*!
   * \brief Default destructor.
   */
  // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
  ~StringStream() = default;

  StringStream(StringStream const&) = delete;
  StringStream(StringStream&&) = delete;
  StringStream& operator=(StringStream const&) = delete;
  StringStream& operator=(StringStream&&) = delete;

  /*!
   * \brief Print a value in the underlying string stream.
   * \tparam The type of the value printed in the string stream.
   * \param[in] value  The value printed in the string stream.
   * \return  Reference on this string stream instance.
   * \pre -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename T>
  StringStream& operator<<(T value) noexcept {
    // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
    internal_string_stream_ << value;

    return *this;
  }

  /*!
   * \brief Getter for std::string contained in the string stream.
   * \return return contained std::string.
   * \pre -
   * \context     ANY
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \synchronous TRUE
   * \steady      FALSE
   * \spec
   *   requires true;
   * \endspec
   */
  std::string str() const;

 private:
  /*!
   * \brief Internal string stream.
   */
  // VCA_SPC_15_SOMEIPBINDING_STL_TYPE_FUNCTION
  StringStreamType internal_string_stream_{};
};

}  // namespace logging
}  // namespace internal
}  // namespace someip_binding_core
}  // namespace amsr

#endif  // LIB_SOMEIP_BINDING_CORE_INCLUDE_AMSR_SOMEIP_BINDING_CORE_INTERNAL_LOGGING_STRING_STREAM_H_
