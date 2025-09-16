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
/**     \file
 *      \brief      Contains native message data types.
 *      \details
 *
 *********************************************************************************************************************/
#ifndef LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_TYPES_TYPES_H_
#define LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_TYPES_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "amsr/core/byte.h"
#include "amsr/core/error_code.h"
#include "amsr/core/optional.h"
#include "amsr/core/string.h"
#include "amsr/core/variant.h"
#include "amsr/core/vector.h"
#include "amsr/log/internal/memory_pool.h"
#include "amsr/log/internal/ref.h"
#include "amsr/log/internal/to_string.h"
#include "amsr/log/internal/utils.h"
#include "amsr/log/log_types.h"
#include "amsr/log_fwd.h"
#include "ara/core/vector.h"

namespace amsr {
namespace log {
namespace internal {

/*! \brief The string type used. */
using StringBuffer = core::String;
/*! \brief The string shard type used. */
using StringShard = PoolShard<StringBuffer, sync::Strategy::kEmpty>;
/*! \brief The buffer type used. */
using RawBuffer = ara::core::Vector<char>;
/*! \brief The buffer shard type used. */
using RawShard = PoolShard<RawBuffer, sync::Strategy::kEmpty>;

namespace detail {
/*!
 * \brief Type used for optionally named arguments.
 *
 * \tparam T The type with an associated name.
 *           Must meet the MoveConstructible named requirement.
 */
template <typename T>
class Named final {
 public:
  /*!
   * \brief Constructs the value.
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  explicit Named(T&& d, StringShard n = {})
      : name_{std::move(n)}, data_{std::move(d)} {}  // VCA_LOGAPI_VALID_REFERENCE_ARGUMENT

  /*! \brief Delete default constructor. */
  Named() noexcept = delete;

  /*! \brief Delete copy constructor. */
  Named(Named const&) = delete;

  /*! \brief Delete copy assignment operator.*/
  auto operator=(Named const&) -> Named& = delete;

  /*! \brief Default move constructor. */
  Named(Named&&) noexcept = default;  // VCA_LOGAPI_MOLE_1298

  /*! \brief Default move assignment operator. */
  auto operator=(Named&&) & noexcept -> Named& = default;

  /*! \brief Destructor. */
  ~Named() noexcept = default;

  /*!
   * \brief Gets the name.
   */
  auto GetName() const noexcept -> StringShard::pointer { return name_.get(); }

  /*!
   * \brief Gets the data.
   */
  auto GetData() const noexcept -> T const& { return data_; }

 private:
  /*! \brief The name for the argument. */
  StringShard name_{};
  /*! \brief The real data for the argument. */
  T data_;
};

/*! \brief The base of integers. */
enum class IntBase : std::uint8_t {
  kBin,
  kDec,
  kHex,
};

/*! \brief The size of integers. */
enum class IntSize : std::uint8_t {
  kBits8 = 0,
  kBits16,
  kBits32,
  kBits64,
};

/*!
 * \brief   Gets the size of an integer.
 *
 * \tparam  T  The integer type.
 *             Must be a signed or unsigned integer type that is either 8, 16, 32, or 64 bits long.
 *
 * \return  The integer size.
 *
 *
 * \pre         -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \context     ANY
 */
template <typename T>
static auto GetIntSize(T) noexcept -> IntSize {
  IntSize size{IntSize::kBits8};
  switch (sizeof(T)) {
    case 8:
      size = IntSize::kBits64;
      break;
    case 4:
      size = IntSize::kBits32;
      break;
    case 2:
      size = IntSize::kBits16;
      break;
    default:  // case 1
      break;
  }
  return size;
}
}  // namespace detail

/*! \brief The type used for storing booleans. */
using Bool = detail::Named<bool>;
/*! \brief The (erased) type used for storing signed integers. */

class SInt final {
 public:
  /*!
   * \brief   Constructs a new SInt object.
   *
   * \tparam  T   The concrete integer type.
   *              Must be a signed or unsigned integer type that is either 8, 16, 32, or 64 bits long.
   *              Unsigned integers are casted to a 64-bit signed integer.
   *
   * \param   d   The data to store.
   * \param   n   The name to store.
   * \param   u   The unit to store.
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  template <typename T>
  explicit SInt(T d, StringShard n = {}, StringShard u = {}) noexcept
      : name_{std::move(n)},  // VCA_LOGAPI_VALID_REFERENCE_ARGUMENT
        unit_{std::move(u)},  // VCA_LOGAPI_VALID_REFERENCE_ARGUMENT
        data_{static_cast<std::int64_t>(d)},
        num_bytes_{detail::GetIntSize(d)} {}

  /*! \brief Delete default constructor. */
  SInt() noexcept = delete;

  /*! \brief Delete copy constructor. */
  SInt(SInt const&) = delete;

  /*! \brief Delete copy assignment operator.*/
  auto operator=(SInt const&) -> SInt& = delete;

  /*! \brief Default move constructor. */
  SInt(SInt&&) noexcept = default;  // VCA_LOGAPI_MOLE_1298

  /*! \brief Default move assignment operator. */
  auto operator=(SInt&&) & noexcept -> SInt& = default;

  /*! \brief Default destructor. */
  ~SInt() noexcept = default;

  /*!
   * \brief Gets the name.
   */
  auto GetName() const noexcept -> StringShard::pointer { return name_.get(); }

  /*!
   * \brief Returns the unit.
   */
  auto GetUnit() const noexcept -> StringShard::pointer { return unit_.get(); }

  /*!
   * \brief Gets the value.
   */
  auto GetData() const noexcept -> std::int64_t { return data_; }

  /*!
   * \brief Gets the size.
   */
  auto GetNumBytes() const noexcept -> detail::IntSize { return num_bytes_; }

 private:
  /*! \brief The name for the argument. */
  StringShard name_{};
  /*! \brief The unit for the argument. */
  StringShard unit_{};
  /*! \brief The real data for the argument. */
  std::int64_t data_{};
  /*! \brief The number of bytes for the argument. */
  detail::IntSize num_bytes_{};
};

/*! \brief The (erased) type used for storing unsigned integers. */
class UInt final {
 public:
  /*!
   * \brief   Constructs a new UInt object.
   *
   * \tparam  T   The concrete integer type.
   *              Must be a signed or unsigned integer type that is either 8, 16, 32, or 64 bits long.
   *              Signed integers are casted to 64-bit unsigned integers.
   *
   * \param   d   The data to store.
   * \param   n   The name to store.
   * \param   u   The unit to store.
   *
   * \pre         -
   * \spec
   *   requires true;
   * \endspec
   * \threadsafe  FALSE
   * \reentrant   FALSE
   * \context     ANY
   */
  template <typename T>
  // VECTOR NC AutosarC++17_10-A12.1.5: MD_LOG_AutosarC++17_10-A12.1.5
  explicit UInt(T d, StringShard n = {}, StringShard u = {}) noexcept
      : name_{std::move(n)},  // VCA_LOGAPI_VALID_REFERENCE_ARGUMENT
        unit_{std::move(u)},  // VCA_LOGAPI_VALID_REFERENCE_ARGUMENT
        data_{static_cast<std::uint64_t>(d)},
        num_bytes_{detail::GetIntSize(d)} {}

  /*!
   * \brief   Constructs a new UInt object with hex/bin base.
   *
   * \tparam  T       The underlying integer type.
   *                  Must meet the template requirements of IntLogWrapper<T, kBase> and GetIntSize<T>(T).
   * \tparam  kBase   The base to use.
   *                  Shall be an enumeration of amsr::log::internal::NumericBase.
   *
   * \param   d       The integer wrapper value to store.
   * \param   n       The name to store.
   * \param   u       The unit to store.
   * \spec
   *   requires true;
   * \endspec
   */
  template <typename T, NumericBase kBase>
  explicit UInt(IntLogWrapper<T, kBase> d, StringShard n = {}, StringShard u = {}) noexcept
      : name_{std::move(n)},  // VCA_LOGAPI_VALID_REFERENCE_ARGUMENT
        unit_{std::move(u)},  // VCA_LOGAPI_VALID_REFERENCE_ARGUMENT
        data_{static_cast<std::uint64_t>(d.value)},
        num_bytes_{detail::GetIntSize(d.value)},
        base_{(kBase == NumericBase::kBinBase) ? detail::IntBase::kBin : detail::IntBase::kHex} {}

  /*! \brief Delete default constructor. */
  UInt() noexcept = delete;

  /*! \brief Delete copy constructor. */
  UInt(UInt const&) = delete;

  /*! \brief Delete copy assignment operator.*/
  auto operator=(UInt const&) -> UInt& = delete;

  /*! \brief Default move constructor. */
  UInt(UInt&&) noexcept = default;  // VCA_LOGAPI_MOLE_1298

  /*! \brief Default move assignment operator. */
  auto operator=(UInt&&) & noexcept -> UInt& = default;

  /*! \brief Default destructor. */
  ~UInt() noexcept = default;

  /*!
   * \brief Gets the name.
   */
  auto GetName() const noexcept -> StringShard::pointer { return name_.get(); }

  /*!
   * \brief Gets the unit.
   */
  auto GetUnit() const noexcept -> StringShard::pointer { return unit_.get(); }

  /*!
   * \brief Gets the value.
   */
  auto GetData() const noexcept -> std::uint64_t { return data_; }

  /*!
   * \brief Gets the size.
   */
  auto GetNumBytes() const noexcept -> detail::IntSize { return num_bytes_; }

  /*!
   * \brief Gets the integer base.
   */
  auto GetBase() const noexcept -> detail::IntBase { return base_; }

 private:
  /*! \brief The name for the argument. */
  StringShard name_{};
  /*! \brief The unit for the argument. */
  StringShard unit_{};
  /*! \brief The real data for the argument. */
  std::uint64_t data_{};
  /*! \brief The number of bytes for the argument. */
  detail::IntSize num_bytes_{};
  /*! \brief The base for the integer. */
  detail::IntBase base_{detail::IntBase::kDec};
};

/*! \brief The (erased) type used for storing floating point numbers. */
class Float final {
 public:
  /*!
   * \brief Create Float element from float.
   * \spec
   *   requires true;
   * \endspec
   */
  // VECTOR NC AutosarC++17_10-A12.1.5: MD_LOG_AutosarC++17_10-A12.1.5
  explicit Float(float value, StringShard n = {}, StringShard u = {}) noexcept
      : name_{std::move(n)},  // VCA_LOGAPI_VALID_REFERENCE_ARGUMENT
        unit_{std::move(u)},  // VCA_LOGAPI_VALID_REFERENCE_ARGUMENT
        data_{static_cast<double>(value)} {}
  /*!
   * \brief Create Float element from double.
   * \spec
   *   requires true;
   * \endspec
   */
  explicit Float(double value, StringShard n = {}, StringShard u = {}) noexcept
      : name_{std::move(n)},  // VCA_LOGAPI_VALID_REFERENCE_ARGUMENT
        unit_{std::move(u)},  // VCA_LOGAPI_VALID_REFERENCE_ARGUMENT
        data_{value},
        is_double_{true} {}

  /*! \brief Delete default constructor. */
  Float() noexcept = delete;

  /*! \brief Delete copy constructor. */
  Float(Float const&) = delete;

  /*! \brief Delete copy assignment operator.*/
  auto operator=(Float const&) -> Float& = delete;

  /*! \brief Default move constructor. */
  Float(Float&&) noexcept = default;  // VCA_LOGAPI_MOLE_1298

  /*! \brief Default move assignment operator. */
  auto operator=(Float&&) & noexcept -> Float& = default;

  /*! \brief Destructor. */
  ~Float() noexcept = default;

  /*!
   * \brief Returns the name of the Float.
   *
   * \return name_ as a raw pointer.
   */
  auto GetName() const noexcept -> StringShard::pointer { return name_.get(); }

  /*!
   * \brief Returns the unit of the Float.
   *
   * \return unit_ as a raw pointer.
   */
  auto GetUnit() const noexcept -> StringShard::pointer { return unit_.get(); }

  /*!
   * \brief Returns the value of the Float.
   */
  auto GetData() const noexcept -> double { return data_; }

  /*!
   * \brief Checks if the Float is a double.
   *
   * \return true if the Float was created from a double. Returns false if it was created from a float.
   */
  auto IsDouble() const noexcept -> bool { return is_double_; }

 private:
  /*! \brief The name for the argument. */
  StringShard name_{};
  /*! \brief The unit for the argument. */
  StringShard unit_{};
  /*! \brief The real data for the argument. */
  double data_{};
  /*! \brief Store if it's really double or a float. */
  bool is_double_{false};
};

/*! \brief The type used for storing strings. */
using String = detail::Named<StringShard>;

/*! \brief The type used for storing raw buffers. */
using Raw = detail::Named<RawShard>;

/*! \brief The type used for storing error codes. */
using NamedError = detail::Named<core::ErrorCode>;

/*! \brief The set of possible types. */
using LogType = core::Variant<Bool, SInt, UInt, Float, String, Raw, NamedError>;

/*!
 * \brief   Visits the LogType variant.
 *
 * \tparam  Visitor   The visitor type that visits.
 *                    Must implement overloads to operator() which accpets one argument of any of
 *                    the types in the variant.
 *
 * \tparam  Types     The types in the LogType variant.
 *                    Must be 7 types.
 *
 * \param   visitor   A callable that can be called with any type in the variant.
 * \param   v         The variant.
 *
 * \spec
 *   requires true;
 * \endspec
 */
template <typename Visitor, typename... Types, typename std::enable_if_t<sizeof...(Types) == 7, std::uint8_t> = 0>
// VECTOR NC AutosarC++17_10-M0.3.1: MD_LOG_AutosarC++17_10-M0.3.1_cant_be_nullptr
void Visit(Visitor&& visitor, core::Variant<Types...> const& v) noexcept {
  switch (v.index()) {
    case 0:
      visitor.operator()(*core::get_if<0>(&v));
      break;
    case 1:
      visitor.operator()(*core::get_if<1>(&v));  // VCA_LOGAPI_VALID_REFERENCE_ARGUMENT
      break;
    case 2:
      visitor.operator()(*core::get_if<2>(&v));  // VCA_LOGAPI_VALID_REFERENCE_ARGUMENT
      break;
    case 3:
      visitor.operator()(*core::get_if<3>(&v));  // VCA_LOGAPI_VALID_REFERENCE_ARGUMENT
      break;
    case 4:
      visitor.operator()(*core::get_if<4>(&v));  // VCA_LOGAPI_VALID_REFERENCE_ARGUMENT
      break;
    case 5:                                      // NOLINT(cppcoreguidelines-avoid-magic-numbers)
                                                 // VCA_LOGAPI_VALID_REFERENCE_ARGUMENT
      visitor.operator()(*core::get_if<5>(&v));  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
      break;
    default:
      // VCA_LOGAPI_VALID_OBJECT
      visitor.operator()(*core::get_if<6>(&v));  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
      break;
  }
}

/*! \brief Gets the mapping from loggable type to a storable type. */
template <typename T>
struct GetType;

/*! \brief Maps bool to Bool. */
template <>
struct GetType<bool> {
  /*! \brief The output type. */
  using Type = Bool;
};

/*! \brief Maps uint8_t to UInt. */
template <>
struct GetType<std::uint8_t> {
  /*! \brief The output type. */
  using Type = UInt;
};

/*! \brief Maps uint16_t to UInt. */
template <>
struct GetType<std::uint16_t> {
  /*! \brief The output type. */
  using Type = UInt;
};
/*! \brief Maps uint32_t to UInt. */
template <>
struct GetType<std::uint32_t> {
  /*! \brief The output type. */
  using Type = UInt;
};
/*! \brief Maps uint64_t to UInt. */
template <>
struct GetType<std::uint64_t> {
  /*! \brief The output type. */
  using Type = UInt;
};
/*! \brief Maps int8_t to SInt. */
template <>
struct GetType<std::int8_t> {
  /*! \brief The output type. */
  using Type = SInt;
};

/*! \brief Maps int16_t to SInt. */
template <>
struct GetType<std::int16_t> {
  /*! \brief The output type. */
  using Type = SInt;
};

/*! \brief Maps int32_t to SInt. */
template <>
struct GetType<std::int32_t> {
  /*! \brief The output type. */
  using Type = SInt;
};

/*! \brief Maps int64_t to SInt. */
template <>
struct GetType<std::int64_t> {
  /*! \brief The output type. */
  using Type = SInt;
};

/*! \brief Maps float to Float. */
template <>
struct GetType<float> {
  /*! \brief The output type. */
  using Type = Float;
};

/*! \brief Maps double to Float. */
template <>
struct GetType<double> {
  /*! \brief The output type. */
  using Type = Float;
};

/*! \brief Maps StringView to String. */
template <>
struct GetType<core::StringView> {
  /*! \brief The output type. */
  using Type = String;
};

/*! \brief Maps LogRawBuffer to RawBuffer. */
template <>
struct GetType<amsr::log::LogRawBuffer> {
  /*! \brief The output type. */
  using Type = Raw;
};

/*! \brief Maps ErrorCode to NamedError. */
template <>
struct GetType<core::ErrorCode> {
  /*! \brief The output type. */
  using Type = NamedError;
};

/*!
 * \brief  Maps IntLogWrapper to UInt.
 * \tparam T     The type for IntLogWrapper.
 * \tparam kBase The base for IntLogWrapper.
 */
template <typename T, NumericBase kBase>
struct GetType<IntLogWrapper<T, kBase>> {
  /*! \brief The output type. */
  using Type = UInt;
};

/*!
 * \brief Converts bools to a loggable type.
 *
 * \param b The input to be converted.
 * \return  The converted value
 *
 * \pre         -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \context     ANY
 */
inline auto ConvertToLoggable(bool b) -> bool { return b; }

/*!
 * \brief Converts ints to a loggable type.
 *
 * \param i The input to be converted.
 * \return  The converted value
 *
 * \pre         -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \context     ANY
 */
inline auto ConvertToLoggable(std::int8_t i) -> std::int8_t { return i; }

/*!
 * \brief Converts ints to a loggable type.
 *
 * \param i The input to be converted.
 * \return  The converted value
 *
 * \pre         -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \context     ANY
 */
inline auto ConvertToLoggable(std::int16_t i) -> std::int16_t { return i; }

/*!
 * \brief Converts ints to a loggable type.
 *
 * \param i The input to be converted.
 * \return  The converted value
 *
 * \pre         -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \context     ANY
 */
inline auto ConvertToLoggable(std::int32_t i) -> std::int32_t { return i; }

/*!
 * \brief Converts ints to a loggable type.
 *
 * \param i The input to be converted.
 * \return  The converted value
 *
 * \pre         -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \context     ANY
 */
inline auto ConvertToLoggable(std::int64_t i) -> std::int64_t { return i; }

/*!
 * \brief Converts ints to a loggable type.
 *
 * \param i The input to be converted.
 * \return  The converted value
 *
 * \pre         -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \context     ANY
 */
inline auto ConvertToLoggable(std::uint8_t i) -> std::uint8_t { return i; }

/*!
 * \brief Converts ints to a loggable type.
 *
 * \param i The input to be converted.
 * \return  The converted value
 *
 * \pre         -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \context     ANY
 */
inline auto ConvertToLoggable(std::uint16_t i) -> std::uint16_t { return i; }

/*!
 * \brief Converts ints to a loggable type.
 *
 * \param i The input to be converted.
 * \return  The converted value
 *
 * \pre         -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \context     ANY
 */
inline auto ConvertToLoggable(std::uint32_t i) -> std::uint32_t { return i; }

/*!
 * \brief Converts ints to a loggable type.
 *
 * \param i The input to be converted.
 * \return  The converted value
 *
 * \pre         -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \context     ANY
 */
inline auto ConvertToLoggable(std::uint64_t i) -> std::uint64_t { return i; }

/*!
 * \brief Converts floats to a loggable type.
 *
 * \param f The input to be converted.
 * \return  The converted value
 *
 * \pre         -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \context     ANY
 */
inline auto ConvertToLoggable(float f) -> float { return f; }

/*!
 * \brief Converts doubles to a loggable type.
 *
 * \param f The input to be converted.
 * \return  The converted value
 *
 * \pre         -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \context     ANY
 */
inline auto ConvertToLoggable(double f) -> double { return f; }

/*!
 * \brief Converts characters to a loggable type.
 *
 * \param ch The input to be converted.
 * \return   The converted value
 *
 * \pre         -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \context     ANY
 */
inline auto ConvertToLoggable(char const& ch) -> core::StringView { return core::StringView{&ch, 1}; }

/*!
 * \brief Converts strings to a loggable type.
 *
 * \param s The input to be converted.
 * \return  The converted value
 *
 * \pre         Must be zero terminated.
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \context     ANY
 */
inline auto ConvertToLoggable(char const* s) -> core::StringView { return core::StringView{s}; }

/*!
 * \brief Converts strings to a loggable type.
 *
 * \param sv The input to be converted.
 * \return   The converted value

 * \pre         -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \context     ANY
 */
inline auto ConvertToLoggable(core::StringView sv) -> core::StringView { return sv; }

/*!
 * \brief Converts raw buffers to a loggable type.
 *
 * \param buf The input to be converted.
 * \return    The converted value
 *
 * \pre         -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \context     ANY
 */
inline auto ConvertToLoggable(amsr::log::LogRawBuffer buf) -> amsr::log::LogRawBuffer { return buf; }

/*!
 * \brief Converts log levels to a loggable type.
 *
 * \param level The input to be converted.
 * \return      The converted value
 *
 * \pre         -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \context     ANY
 */
inline auto ConvertToLoggable(amsr::log::LogLevel level) -> core::StringView { return GetSeverityString(level); }

/*!
 * \brief Converts error codes to a loggable type.
 *
 * \param ec The input to be converted.
 * \return   The converted value
 *
 * \pre         -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \context     ANY
 */
inline auto ConvertToLoggable(core::ErrorCode ec) -> core::ErrorCode { return ec; }

/*!
 * \brief Converts hex/bin integers to a loggable type.
 *
 * \tparam T      The underlying integer type for an IntLogWrapper.
 *                Must comply to the template parameter requirements of amsr::log::internal::IntLogWrapper<T, kBase>
 * \tparam kBase  The base for an IntLogWrapper.
 *                Must comply to the template parameter requirements of amsr::log::internal::IntLogWrapper<T, kBase>
 *
 * \param i The input to be converted.
 * \return  The converted value
 *
 * \pre         -
 * \spec
 *   requires true;
 * \endspec
 * \threadsafe  FALSE
 * \reentrant   FALSE
 * \context     ANY
 */
// VECTOR NC AutosarC++17_10-A15.4.4: MD_LOG_AutosarC++17_10-A15.4.4
template <typename T, NumericBase kBase>
inline auto ConvertToLoggable(IntLogWrapper<T, kBase> i) -> IntLogWrapper<T, kBase> {
  return i;
}

}  // namespace internal
}  // namespace log
}  // namespace amsr

#endif  // LIB_AMSR_LOG_INCLUDE_AMSR_LOG_INTERNAL_TYPES_TYPES_H_
