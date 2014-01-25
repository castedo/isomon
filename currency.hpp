#ifndef ISOMON_CURRENCY_HPP
#define ISOMON_CURRENCY_HPP

/** @file currency.hpp
    @brief C++ header for currency class
*/

#include "currency_data.h"

#include <string>
#include <ostream>
#include <istream>

namespace isomon {

/// ISO 4217 currency
class currency
{
public:

  /// Contruct ISO "no currency" code "XXX" (999)
  currency() : num_(ISO_XXX) {}

  /// Construct from 3 letter ISO character code
  currency(std::string const& code) { init(code.c_str()); }

  /// Construct from 3 letter ISO character code
  currency(char const* code) { init(code); }

  /// Construct from int if ISO numeric code, otherwise default value
  currency(int16_t i) : num_(data::is_isonum(i) ? i : ISO_XXX) {}

  /// True if this is not special ISO "no currency" value "XXX" (999)
  bool is_currency() { return num_ != ISO_XXX; }

  /// True if this is special ISO "no currency" value "XXX" (999)
  bool is_no_currency() { return num_ == ISO_XXX; }

  /// return ISO 3-character code
  std::string str() const { return std::string(data::isonum2code(num_)); }

  /// return ISO 3-character code as null-terminated C string
  char const* c_str() const { return data::isonum2code(num_); }

  /// return ISO numeric code
  int16_t isonum() const { return num_; }

  /// return number of decimal digits for minor units,
  //! or return 0 if not applicable or unknown
  int32_t num_digits() const { return data::num_minor_digits(num_); }

  /// return number of minor units per major unit,
  //! or return 0 if not applicable or unknown
  int32_t num_minors() const { return data::num_minors(num_); }

  bool operator == (currency rhs) const { return num_ == rhs.num_; }
  bool operator != (currency rhs) const { return num_ != rhs.num_; }

  /// Ordering is the same as the ordering of ISO alphabetic code.
  bool operator < (currency rhs) const {
    return data::isonum2hash(num_) < data::isonum2hash(rhs.num_);
  }
  bool operator <= (currency rhs) const {
    return data::isonum2hash(num_) <= data::isonum2hash(rhs.num_);
  }
  bool operator > (currency rhs) const {
    return data::isonum2hash(num_) > data::isonum2hash(rhs.num_);
  }
  bool operator >= (currency rhs) const {
    return data::isonum2hash(num_) >= data::isonum2hash(rhs.num_);
  }

  friend std::istream & operator >> (std::istream & is, currency & rhs);

private:
  #ifndef DOXYGEN_SHOULD_SKIP_THIS
  void init(char const* code) {
    if (!data::code2isonum(code, &num_)) { num_ = ISO_XXX; }
  }
  #endif

  isonum_t num_;
};

/// Output ISO alphabetic code (3 Latin letters)
inline std::ostream & operator << (std::ostream & os, currency rhs) {
  return os << rhs.c_str();
}

/// Read ISO alphabetic code (3 Latin letters)
inline std::istream & operator >> (std::istream & is, currency & rhs) {
  char code[4];
  is >> std::ws >> code[0] >> code[1] >> code[2];
  code[3] = '\0';
  if (!is || !data::code2isonum(code, &rhs.num_)) {
    is.setstate(std::ios_base::failbit);
  }
  return is;
}

} // namespace isomon

#endif // ISOMON_CURRENCY_HPP

