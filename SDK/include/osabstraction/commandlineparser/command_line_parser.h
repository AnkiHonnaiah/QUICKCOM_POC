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
/*!        \file
 *        \brief  OS independent implementation of a command line parser.
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_COMMANDLINEPARSER_COMMAND_LINE_PARSER_H_
#define LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_COMMANDLINEPARSER_COMMAND_LINE_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "amsr/core/abort.h"
#include "amsr/core/span.h"
#include "vac/container/c_string_view.h"

namespace osabstraction {
namespace commandlineparser {

/*!
 * \brief       Option value when iterator is invalid.
 *
 * \vprivate    Vector product internal API
 */
static constexpr std::int16_t kInvalidOption{-1};

/*!
 * \brief Provides services to parse and validate command line arguments.
 *
 * \details PSE54 and therefore not available on all supported OSs. This implementation supports:
 * Simple arguments, e.g.:
 *
 *     gcc main.c
 *
 * Short form options, e.g.:
 *
 *     gcc -v
 *
 * Short form options may require arguments, e.g.:
 *
 *     gcc -o myprog main.c
 *
 * Multiple short form options can be concatenated in one parameter, e.g.:
 *
 *     tar -xf archive.tar
 *
 * Usage:
 *
 *     CommandLineParser parser(argc, argv, OPTIONS)
 *
 * where OPTIONS is a string specifying allowed command line options where each option represented by a single letter
 * i.e. "-a", with optional ':' appended to indicate that the option has an argument. An invalid OPTIONS string leads
 * to an empty list of options. GetOption() will return kInvalidOption in this case. In debug build an invalid OPTIONS
 * string leads to an assertion.
 *
 * Example:
 *
 *     CommandLineParser parser(argc, argv, "ab:c");
 *     for(auto it: parser) {
 *       switch(it.GetOption()) {
 *         case 'a': FlagA = true; break;
 *         case 'b': ValueB = it.GetOptionArgument(); break;
 *         case 'c': FlagC = true; break;
 *         case '?': PrintUsage(); exit(); break;
 *       }
 *     }
 *
 * \unit        osabstraction::CommandLineParser
 *
 * \vprivate    Vector product internal API
 */
class CommandLineParser final {
 private:
  /*!
   * \brief       Type definition for the command line parameter.
   */
  using CommandLineParameters = ::amsr::core::Span<char const* const>;

 public:
  /*!
   * \brief         Iterator pointing to a parsed option.
   * \unit          osabstraction::CommandLineParser
   * \vprivate      Vector product internal API
   */
  class Iterator final {
   public:
    // VECTOR Next Construct VectorC++-V3.9.2: MD_OSA_V3.9.2_StringIterator
    /*!
     * \brief           Creates the iterator and starts parsing the command line parameters.
     *
     * \param[in]       parent      The reference to the parent.
     * \param[in]       it          Iterator of the command line parameters.
     * \param[in]       is_end      Marks the end iterator.
     *
     * \context         ANY
     *
     * \pre             -
     *
     * \reentrant       FALSE
     * \synchronous     TRUE
     * \threadsafe      FALSE
     * \steady          TRUE
     *
     * \vprivate        Vector component internal API
     *
     * \spec
     *   requires true;
     * \endspec
     */
    Iterator(CommandLineParser const& parent, CommandLineParameters::const_iterator it, bool is_end) noexcept;

    /*!
     * \brief           Copy constructor
     *
     * \steady          TRUE
     *
     * \vprivate        Vector product internal API
     *
     * \spec
     *   requires true;
     * \endspec
     */
    Iterator(Iterator const&) = default;

    /*!
     * \brief           Deleted default constructor
     */
    Iterator() = delete;

    /*!
     * \brief           Default move constructor
     *
     * \steady          TRUE
     *
     * \spec
     *   requires true;
     * \endspec
     */
    Iterator(Iterator&&) noexcept = default;

    /*!
     * \brief           Deleted copy assignment operator
     */
    Iterator& operator=(Iterator const&) = delete;

    /*!
     * \brief           Deleted move assignment operator
     */
    Iterator& operator=(Iterator&&) = delete;

    /*!
     * \brief           Default destructor
     *
     * \steady          TRUE
     *
     * \spec
     *   requires true;
     * \endspec
     */
    ~Iterator() noexcept = default;

    /*!
     * \brief           Equality operator.
     *
     * \param[in]       rhs   The right hand side
     *
     * \return          True if the objects are equal, false otherwise.
     *
     * \context         ANY
     *
     * \pre             -
     *
     * \reentrant       FALSE
     * \synchronous     TRUE
     * \threadsafe      FALSE
     * \steady          TRUE
     *
     * \vprivate        Vector product internal API
     *
     * \spec
     *   requires true;
     * \endspec
     */
    bool operator==(Iterator const& rhs) const noexcept {
      // VECTOR Next Line AutosarC++17_10-M0.1.2: MD_OSA_M0.1.2_dead_branch_false_positive
      return ((&parent_ == &rhs.parent_) && (command_line_parameter_ == rhs.command_line_parameter_)) &&
             (is_end_ == rhs.is_end_);
    }

    /*!
     * \brief           Inequality operator.
     *
     * \param[in]       rhs   The right hand side
     *
     * \return          True if the objects are not equal, false otherwise.
     *
     * \context         ANY
     *
     * \pre             -
     *
     * \reentrant       FALSE
     * \synchronous     TRUE
     * \threadsafe      FALSE
     * \steady          TRUE
     *
     * \vprivate        Vector product internal API
     *
     * \spec
     *   requires true;
     * \endspec
     */
    bool operator!=(Iterator const& rhs) const noexcept { return !(*this == rhs); }

    /*!
     * \brief           Pre-increment operator.
     *
     * \return          Itself.
     *
     * \context         ANY
     *
     * \pre             -
     *
     * \reentrant       FALSE
     * \synchronous     TRUE
     * \threadsafe      FALSE
     * \steady          TRUE
     *
     * \vprivate        Vector product internal API
     *
     * \spec
     *   requires true;
     * \endspec
     */
    Iterator& operator++() {
      ParseNextOption();  // VCA_OSA_CALL_PASS_VOID
      return *this;
    }

    // VECTOR Next Construct AutosarC++17_10-A3.9.1: MD_OSA_A3.9.1_PostfixIncrementOperator
    /*!
     * \brief           Post-increment operator.
     *
     * \return          Copy of the iterator before increment.
     *
     * \context         ANY
     *
     * \pre             -
     *
     * \reentrant       FALSE
     * \synchronous     TRUE
     * \threadsafe      FALSE
     * \steady          TRUE
     *
     * \vprivate        Vector product internal API
     *
     * \spec
     *   requires true;
     * \endspec
     */
    // NOLINTNEXTLINE(cert-dcl21-cpp) - This clang-tidy error fix contradicts with rule 'readability-const-return-type'
    Iterator operator++(int) {
      Iterator const retval{*this};
      ++(*this);
      return retval;
    }

    /*!
     * \brief           Dereference operator.
     *
     * \return          Itself.
     *
     * \context         ANY
     *
     * \pre             -
     *
     * \reentrant       FALSE
     * \synchronous     TRUE
     * \threadsafe      FALSE
     * \steady          TRUE
     *
     * \vprivate        Vector product internal API
     *
     * \spec
     *   requires true;
     * \endspec
     */
    Iterator& operator*() { return *this; }

    // VECTOR Next Construct AutosarC++17_10-A3.9.1: MD_OSA_A3.9.1_GetOption
    /*!
     * \brief           Returns the symbol for the current option.
     *
     * \return          The symbol for the current option.
     * \retval          '?'              for unknown option.
     * \retval          kInvalidOption   for invalid iterator.
     *
     * \context         ANY
     *
     * \pre             -
     *
     * \reentrant       FALSE
     * \synchronous     TRUE
     * \threadsafe      FALSE
     * \steady          TRUE
     *
     * \vprivate        Vector product internal API
     *
     * \trace           DSGN-Osab-CommandLineParser-OptionParsing
     *
     * \spec
     *   requires true;
     * \endspec
     */
    int GetOption() const noexcept;

    /*!
     * \brief           Returns the argument of the current option.
     *
     * \return          The option argument
     * \retval          nullptr    if the current option has no arguments.
     *
     * \context         ANY
     *
     * \pre             -
     *
     * \reentrant       FALSE
     * \synchronous     TRUE
     * \threadsafe      FALSE
     * \steady          TRUE
     *
     * \vprivate        Vector product internal API
     *
     * \trace           DSGN-Osab-CommandLineParser-OptionParsing
     *
     * \spec
     *   requires true;
     * \endspec
     */
    char const* GetOptionArgument() const noexcept;

   private:
    /*!
     * \brief           Parses the next option from the command line argument.
     *
     * \details         Performs the check of the next possible option and updates the iterator object.
     *
     * \context         ANY
     *
     * \pre             -
     *
     * \reentrant       FALSE
     * \threadsafe      FALSE
     */
    void ParseNextOption() noexcept;

    /*!
     * \brief           Parses the argument of an option.
     *
     * \param[in]       command_line_parameter  Current parameter being parsed.
     *
     * \context         ANY
     *
     * \pre             -
     *
     * \reentrant       FALSE
     * \threadsafe      FALSE
     *
     */
    void ParseOptionArgument(vac::container::CStringView command_line_parameter) noexcept;

    /*!
     * \brief           Advances the processing of the current parameter to the next character.
     *
     * \param[in]       command_line_parameter  Current parameter being parsed.
     *
     * \context         ANY
     *
     * \pre             -
     *
     * \reentrant       FALSE
     * \threadsafe      FALSE
     */
    void GoToNextChar(vac::container::CStringView command_line_parameter) noexcept;

    /*!
     * \brief           Sets option processing to the next parameter.
     *
     * \context         ANY
     *
     * \pre             -
     *
     * \reentrant       FALSE
     * \threadsafe      FALSE
     */
    void GoToNextParameter() noexcept;

    /*!
     * \brief           Stops parsing and sets current option to kInvalidOption.
     *
     *
     * \context         Any
     *
     * \pre             -
     *
     * \reentrant       FALSE
     * \synchronous     TRUE
     * \threadsafe      FALSE
     */
    void StopParsing() noexcept;

    /*!
     * \brief           Back reference to the parent object.
     */
    CommandLineParser const& parent_;

    /*!
     * \brief           Iterator pointing to a command line parameter
     */
    CommandLineParameters::const_iterator command_line_parameter_;

    /*!
     * \brief           Current position in the string referenced by the current command line parameter.
     */
    std::size_t str_pos_;

    // VECTOR Next Construct AutosarC++17_10-A3.9.1: MD_OSA_A3.9.1_GetOption
    /*!
     * \brief           Symbol for the current option, '?' for unknown option or kInvalidOption for invalid iterator.
     */
    int cur_opt_;

    /*!
     * \brief           C-string of the possible argument for the current option. nullptr if there is none.
     */
    char const* cur_argument_{nullptr};

    /*!
     * \brief           Marks the end iterator.
     */
    bool is_end_{false};
  };  // End of class Iterator.

  // VECTOR Next Construct AutosarC++17_10-A3.9.1: MD_OSA_A3.9.1_CommandLineParser
  // VECTOR Next Construct VectorC++-V3.9.2: MD_OSA_V3.9.2_CommandLineParser
  /*!
   *
   * \brief             Creates the command line parser and validates the given command line arguments.
   *
   * \param[in]         argc             Number of command line arguments.
   * \param[in]         argv             Array containing the actual command line arguments.
   *                                     Must be a valid array. Each entry of the array must be a valid c-string.
   *                                     Number of entries must be (argc+1) where last entry is nullptr.
   *                                     The referenced data must not be changed for the lifetime of this class.
   * \param[in]         allowed_options  String which specifies which options are allowed and which options require an
   *                                     argument.
   *                                     Must be a valid c-string. Must only contain alphabetic characters
   *                                     and ':' to mark the previous alphabetic character to be an option
   *                                     with a parameter.
   *                                     The referenced data must not be changed for the lifetime of this class.
   *
   * \context           ANY
   *
   * \pre               -
   *
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \threadsafe        FALSE
   *
   * \vprivate          Vector product internal API
   */
  CommandLineParser(int argc, char const* const* argv, char const* allowed_options);

  /*!
   * \brief             Deleted default constructor
   */
  CommandLineParser() = delete;

  /*!
   * \brief             Copy constructor
   *
   * \vprivate          Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  CommandLineParser(CommandLineParser const&) = default;

  /*!
   * \brief             Deleted move constructor
   */
  CommandLineParser(CommandLineParser&&) = delete;

  /*!
   * \brief             Deleted copy assignment operator
   */
  CommandLineParser& operator=(CommandLineParser const&) = delete;

  /*!
   * \brief             Deleted move assignment operator
   */
  CommandLineParser& operator=(CommandLineParser&&) = delete;

  /*!
   * \brief           Default destructor
   *
   * \spec
   *   requires true;
   * \endspec
   */
  ~CommandLineParser() noexcept = default;

  /*!
   *
   * \brief             Returns an iterator initialized to the first actual option after name of executable.
   *
   * \return            Iterator to the begin of the option list.
   *
   * \context           ANY
   *
   * \pre               -
   *
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \threadsafe        FALSE
   *
   * \vprivate          Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  Iterator begin() const noexcept { return Iterator{*this, command_line_parameters_.cbegin(), false}; }

  /*!
   * \brief             Returns an iterator to the end of the option list.
   *
   * \return            Iterator to the end of the option list.
   *
   * \context           ANY
   *
   * \pre               -
   *
   * \reentrant         FALSE
   * \synchronous       TRUE
   * \threadsafe        FALSE
   *
   * \vprivate          Vector product internal API
   *
   * \spec
   *   requires true;
   * \endspec
   */
  Iterator end() const noexcept { return Iterator{*this, command_line_parameters_.cend(), true}; }

 private:
  /*!
   * \brief             Checks the passed option list string.
   *
   * \details           Program execution is aborted if the option list is invalid.
   *
   *
   *
   * \context           ANY
   *
   * \pre               valid options list
   *
   * \reentrant         FALSE
   * \threadsafe        FALSE
   *
   * \trace             DSGN-Osab-CommandLineParser-OptionsListValidation
   */
  void ValidateOptionList() noexcept;

  /*!
   * \brief             Command line parameters starting from the first parameter after the program name.
   */
  CommandLineParameters command_line_parameters_;

  /*!
   * \brief             The allowed option string.
   */
  vac::container::CStringView options_list_;
};

}  // namespace commandlineparser
}  // namespace osabstraction

#endif  // LIB_COMMON_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_COMMANDLINEPARSER_COMMAND_LINE_PARSER_H_
