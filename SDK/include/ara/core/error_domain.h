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
/*!        \file  ara/core/error_domain.h
 *        \brief  SWS core type ara::core::ErrorDomain.
 *         \unit  VaCommonLib::ErrorHandling::ErrorDomain
 *
 *********************************************************************************************************************/

#ifndef LIB_VAC_INCLUDE_ARA_CORE_ERROR_DOMAIN_H_
#define LIB_VAC_INCLUDE_ARA_CORE_ERROR_DOMAIN_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "ara/core/string_view.h"

namespace ara {
namespace core {
namespace internal {

/*!
 * \brief Alias type for vendor- and context-specific supplementary data.
 * \vprivate Component private
 */
class SupportDataContainer final {
 public:
  /*!
   * \brief Alias type for the used string.
   */
  using StringType = char const*;

  /*!
   * \brief Type of the numerical value.
   */
  using CodeType = std::int32_t;

  /*!
   * \brief Constructor to pass in both member elements.
   */
  constexpr SupportDataContainer(CodeType c, StringType l) noexcept : code(c), location(l) {}

  /*!
   * \brief Constructor with no parameters.
   */
  constexpr SupportDataContainer() noexcept : SupportDataContainer(0, "") {}

  // VECTOR Next Construct VectorC++-V12.1.4: MD_VAC_V12-1-4_constructorShallBeDeclaredExplicitStaticCast
  /*!
   * \brief Backward compatibility constructor.
   */
  constexpr SupportDataContainer(CodeType c) noexcept : SupportDataContainer(c, "") {}

  /*!
   * \brief Default copy constructor.
   */
  constexpr SupportDataContainer(SupportDataContainer const& other) noexcept = default;

  /*!
   * \brief Default move constructor.
   */
  constexpr SupportDataContainer(SupportDataContainer&&) noexcept = default;

  /*!
   * \brief Default copy assignment.
   */
  constexpr SupportDataContainer& operator=(SupportDataContainer const&) & noexcept = default;

  /*!
   * \brief  Default move assignment.
   */
  constexpr SupportDataContainer& operator=(SupportDataContainer&&) & noexcept = default;

  // VECTOR Next Construct AutosarC++17_10-A13.5.2: MD_VAC_A13.5.2_userDefinedConversionOperatorShallBeDefinedExplicit
  /*!
   * \brief Backward compatibility conversion.
   * \returns The error code.
   */
  constexpr operator CodeType() const noexcept { return code; }

  /*!
   * \brief Convenience getter.
   * \returns A StringView containing the source location of the error (file and line number).
   */
  constexpr ara::core::StringView Location() const noexcept { return ara::core::StringView(location); }

  /*!
   * \brief Default destructor
   */
  ~SupportDataContainer() noexcept = default;

 private:
  /*!
   * \brief The error code.
   */
  CodeType code;

  /*!
   * \brief The source location where an error originated.
   * \details By using the macro AMSR_FILE_LINE in location.h, source file and line # can be retrieved.
   */
  StringType location;
};

/*!
 * \brief Equality operator for two SupportDataContainers.
 * \details Two SupportDataContainer instances compare equal if the results
 *          of their operator CodeType() and Location() functions are equal.
 * \param lhs A SupportDataContainer object to compare.
 * \param rhs A SupportDataContainer object to compare.
 * \return True if the SupportDataContainers are equal.
 * \pre    -
 * \spec
 *   requires true;
 * \endspec
 * \vprivate Component private
 */
constexpr auto operator==(SupportDataContainer const& lhs, SupportDataContainer const& rhs) noexcept -> bool {
  return (lhs.operator SupportDataContainer::CodeType() == rhs.operator SupportDataContainer::CodeType()) &&
         (lhs.Location() == rhs.Location());
}

/*!
 * \brief Equality operator for a CodeType and a SupportDataContainer.
 * \details The CodeType and SupportDataContainer instance compare equal if the CodeType and
 *          the result of the SupportDataContainer's operator CodeType() are equal.
 * \param lhs A CodeType to compare.
 * \param rhs A SupportDataContainer object to compare.
 * \return True if the CodeType and the SupportDataContainer::CodeType are equal.
 * \pre    -
 * \spec
 *   requires true;
 * \endspec
 * \vprivate Component private
 */
constexpr auto operator==(SupportDataContainer::CodeType const& lhs, SupportDataContainer const& rhs) noexcept -> bool {
  return (lhs == rhs.operator SupportDataContainer::CodeType());
}

/*!
 * \brief Equality operator for a SupportDataContainer and a CodeType.
 * \details The SupportDataContainer instance and the CodeType compare equal if the result
 *          of the SupportDataContainer's operator CodeType() and the CodeType are equal.
 * \param lhs A SupportDataContainer object to compare.
 * \param rhs A CodeType to compare.
 * \return True if the SupportDataConatiner::CodeType and the CodeType are equal.
 * \pre    -
 * \spec
 *   requires true;
 * \endspec
 * \vprivate Component private
 */
constexpr auto operator==(SupportDataContainer const& lhs, SupportDataContainer::CodeType const& rhs) noexcept -> bool {
  return (lhs.operator SupportDataContainer::CodeType() == rhs);
}

/*!
 * \brief Inequality operator for two SupportDataContainers.
 * \details Two SupportDataContainer instances compare inequal if the results of
 *          their operator CodeType() and/or Location() functions are inequal.
 * \param lhs A SupportDataContainer object to compare.
 * \param rhs A SupportDataContainer object to compare.
 * \return True if the SupportDataContainers are inequal.
 * \pre    -
 * \spec
 *   requires true;
 * \endspec
 * \vprivate Component private
 */
constexpr auto operator!=(SupportDataContainer const& lhs, SupportDataContainer const& rhs) noexcept -> bool {
  return !operator==(lhs, rhs);
}

/*!
 * \brief Inequality operator for a CodeType and a SupportDataContainer.
 * \details The CodeType and the SupportDataContainer compare inequal if the result of the
 *          SupportDataContainer's operator CodeType() and the CodeType are inequal.
 * \param lhs A CodeType to compare.
 * \param rhs A SupportDataContainer object to compare.
 * \return True if the CodeType and the SupportDataContainer are inequal.
 * \pre    -
 * \spec
 *   requires true;
 * \endspec
 * \vprivate Component private
 */
constexpr auto operator!=(SupportDataContainer::CodeType const& lhs, SupportDataContainer const& rhs) noexcept -> bool {
  return !operator==(lhs, rhs);
}

/*!
 * \brief Inequality operator for a SupportDataContainer and a CodeType.
 * \details The SupportDataContainer and the CodeType compare inequal if the CodeType and the
 *          result of the SupportDataContainer's operator CodeType() are inequal.
 * \param lhs A SupportDataContainer object to compare.
 * \param rhs A CodeType to compare.
 * \return True if the SupportDataContainer and the CodeType are inequal.
 * \pre    -
 * \spec
 *   requires true;
 * \endspec
 * \vprivate Component private
 */
constexpr auto operator!=(SupportDataContainer const& lhs, SupportDataContainer::CodeType const& rhs) noexcept -> bool {
  return !operator==(lhs, rhs);
}

}  // namespace internal

// Forward declaration.
class ErrorCode;

/*!
 * \brief   Encapsulation of an error domain.
 * \details An error domain is the controlling entity for ErrorCode's error code values, and defines the mapping of
 *          such error code values to textual representations.
 *          This class is a literal type, and subclasses are strongly advised to be literal types as well.
 * \trace   DSGN-VaCommonLib-ErrorHandling
 * \vpublic
 */
class ErrorDomain {  // VECTOR Same Line AutosarC++17_10-A12.4.2: MD_VAC_A12.4.2_classNotFinal
 public:
  /*!
   * \brief Alias type for a unique ErrorDomain identifier type.
   */
  using IdType = std::uint64_t;

  /*!
   * \brief Alias type for a domain-specific error code.
   */
  using CodeType = std::int32_t;

  /*!
   * \brief Alias type for vendor- and context-specific supplementary data.
   */
  using SupportDataType = internal::SupportDataContainer;

  /*!
   * \brief Alias type for the used string.
   * \vprivate
   */
  using StringType = char const*;

  /*!
   * \brief Default copy constructor deleted.
   * \vpublic
   */
  ErrorDomain(ErrorDomain const&) = delete;

  /*!
   * \brief Default move constructor deleted.
   * \vpublic
   */
  ErrorDomain(ErrorDomain&&) = delete;  // VECTOR Same Line AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_deletedFunction

  /*!
   * \brief  Default copy assignment operator deleted.
   * \return
   */
  ErrorDomain& operator=(ErrorDomain const&) & = delete;

  /*!
   * \brief  Default move assignment operator deleted.
   * \return
   */
  // VECTOR Next Line AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_deletedFunction
  ErrorDomain& operator=(ErrorDomain&&) & = delete;

  /*!
   * \brief   Return the name of this error domain.
   * \pre     -
   * \returns The name as a null-terminated string, never nullptr.
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  virtual StringType Name() const noexcept = 0;

  /*!
   * \brief     Return a textual representation of the given error code.
   * \details   The return value is undefined if the errorCode did not originate from this error domain.
   * \param[in] error_code The domain-specific error code.
   * \pre       -
   * \returns   The text as a null-terminated string, never nullptr.
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  virtual StringType Message(CodeType error_code) const noexcept = 0;

  /*!
   * \brief  Throws the given errorCode as Exception.
   * \param  error_code Error code to be thrown.
   * \pre    -
   * \remark If the code is compiled without exceptions, this function call will terminate.
   * \spec
   *   requires true;
   * \endspec
   * \trace  DSGN-VaCommonLib-ErrorHandling
   * \vpublic
   */
  [[noreturn]] virtual void ThrowAsException(ErrorCode const& error_code) const noexcept(false) = 0;

  /*!
   * \brief   Return the unique domain identifier.
   * \pre     -
   * \returns The identifier.
   * \spec
   *   requires true;
   * \endspec
   * \trace   DSGN-VaCommonLib-ErrorHandling
   * \vpublic
   */
  constexpr IdType Id() const noexcept { return id_; }

  /*!
   * \brief   Compare for equality with another ErrorDomain instance.
   * \details Two ErrorDomain instances compare equal when their identifiers (returned by Id()) are equal.
   * \param   other The other instance.
   * \spec
   *   requires true;
   * \endspec
   * \pre     -
   * \returns True if other is equal to *this, false otherwise.
   * \vpublic
   */
  constexpr bool operator==(ErrorDomain const& other) const noexcept { return id_ == other.id_; }

  /*!
   * \brief   Compare for non-equality with another ErrorDomain instance.
   * \param   other The other instance.
   * \pre     -
   * \returns True if other is not equal to *this, false otherwise.
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr bool operator!=(ErrorDomain const& other) const noexcept { return !operator==(other); }

 protected:
  /*!
   * \brief     Construct a new instance with the given identifier.
   * \details   Identifiers are expected to be system-wide unique.
   * \param[in] id The unique identifier.
   * \pre       -
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  constexpr explicit ErrorDomain(IdType id) noexcept : id_(id) {}

  // VECTOR Next Construct AutosarC++17_10-A15.5.1: MD_VAC_A15.5.1_explicitNoexceptIfAppropriate
  /*!
   * \brief   Destructor.
   * \details This dtor is non-virtual (and trivial) so that this class can be a literal type. While this class has
   *          virtual functions, no polymorphic destruction is needed.
   * \vpublic
   * \spec
   *   requires true;
   * \endspec
   */
  ~ErrorDomain() noexcept = default;

 private:
  /*! \brief Id of the error domain. */
  IdType const id_;
};

}  // namespace core
}  // namespace ara

#endif  // LIB_VAC_INCLUDE_ARA_CORE_ERROR_DOMAIN_H_
