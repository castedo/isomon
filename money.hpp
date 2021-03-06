#ifndef ISOMON_MONEY_HPP
#define ISOMON_MONEY_HPP

#include "currency.hpp"

#include <limits>
#include <ostream>
#include <iomanip>
#include <locale>
#include <tr1/cmath>

namespace isomon {


class money
{
public:
  money();
  money(int64_t major_units, int64_t minor_units, currency unit);

  static money pos_infinity(currency unit);
  static money neg_infinity(currency unit);

  double value() const;
  currency unit() const;
  int64_t total_minors() const;

  money & operator += (money rhs);
  money & operator -= (money rhs);
  money & operator *= (int32_t rhs);
  money operator - () const;
  money operator + (money rhs) const;
  money operator - (money rhs) const;
  money operator * (int32_t rhs) const;

  bool operator == (money rhs) const { return _data == rhs._data; }
  bool operator != (money rhs) const { return _data != rhs._data; }

  friend money nextafter(money m);

private:
  #ifndef DOXYGEN_SHOULD_SKIP_THIS
  void init(int64_t minors, currency unit);
  void fix_overflow();

  // don't interger multiply by anything other than int32_t
  money & operator *= (uint32_t rhs);
  money operator * (uint32_t rhs) const;
  money & operator *= (int64_t rhs);
  money operator * (int64_t rhs) const;
  money & operator *= (uint64_t rhs);
  money operator * (uint64_t rhs) const;
  #endif

  int64_t _data;
};

inline bool isfinite(money m) { return std::isfinite(m.value()); }

inline money operator * (int32_t i, money m) { return m * i; }

// Construction from floating point numbers

template <class _Number>
money floor(_Number value, currency unit);

template <class _Number>
money ceil(_Number value, currency unit);

template <class _Number>
money trunc(_Number value, currency unit);

template <class _Number>
money round(_Number value, currency unit);

template <class _Number>
money rounde(_Number value, currency unit);


inline std::ostream & operator << (std::ostream & os, money m)
{
  using namespace std;
  const moneypunct<char> & mp = use_facet<moneypunct<char> >(os.getloc());
  bool symbol_first = mp.pos_format().field[0] == moneypunct<char>::symbol;
  if (symbol_first) { os << m.unit() << ' '; }
  int num_digits = m.unit().num_digits();
  streamsize saved_digits = os.precision(num_digits);
  ios_base::fmtflags saved_flags = os.flags();
  os << fixed << m.value();
  os.flags(saved_flags);
  os.precision(saved_digits);
  if (!symbol_first) { os << ' ' << m.unit(); }
  return os;
}

// round half (towards) even, "banker's rounding"

inline long int llrounde(double x)
{
  int64_t halfout = std::llround(x);
  if (halfout % 2) { // if odd, might need to adjust
    // x is exactly halfway between two integers
    // and was rounded OUT (away from zero) to an odd number
    // we want to round IN (toward zero) to an even number instead
    if (halfout - x == 0.5) return halfout - 1;
    if (halfout - x == -0.5) return halfout + 1;
  }
  return halfout;
}

/// Number traits

template <class _Number>
struct number_traits {};

// number traits are pre-defined for type double
// define similar struct in isomon namespace for other float types

template<>
struct number_traits<double>
{
  static bool isnan(double x) { return std::isnan(x); }

  static bool isinf(double x) { return std::isinf(x); }

  static int64_t floor(double x) { return std::floor(x); }

  static int64_t ceil(double x) { return std::ceil(x); }

  static int64_t trunc(double x) { return std::tr1::trunc(x); }

  static int64_t roundhalfout(double x) { return std::llround(x); }

  static int64_t roundhalfeven(double x) { return llrounde(x); }
};

/////////////////////////////////////////////////////////////////////

namespace detail {

const int64_t CURRENCY_BITS = 0x3ffLL; // low 10 bits
const int64_t POS_INF_MINORS = (1LL << 53) - 1; // 2^53 - 1
const int64_t NEG_INF_MINORS = -(POS_INF_MINORS + 1); // - 2^53

template <class _Number>
money money_cast(_Number minors, currency unit, int64_t (*rounder)(_Number) )
{
  typedef number_traits<_Number> nt;
  if (nt::isnan(minors) || unit.num_minors() < 1) return money();
  if (nt::isinf(minors)) {
    if (minors > 0) return money::pos_infinity(unit);
    else return money::neg_infinity(unit);
  }
  return money(0, rounder(minors), unit); 
}

inline
std::pair<int64_t, uint32_t> safe_multiply(int64_t a, int32_t b)
// returns ret == pair( a*b >> 32, a*b & 2^32 - 1 ) without overflows
// where (a*b == ret.first << 32 + ret.second) in 96 bit math
{
  int64_t hi = (a >> 32) * b;
  int64_t lo = (a & 0xFFFFFFFFLL) * b;
  hi += (lo >> 32);
  return std::pair<int64_t, uint32_t>(hi, lo & 0xFFFFFFFFLL);
}

} // namespace isomon::detail

inline void money::init(int64_t minors, currency unit) {
  if (unit.num_minors() < 1) {
    _data = ISO_XXX;
  } else {
    _data = std::max( detail::NEG_INF_MINORS,
                      std::min(detail::POS_INF_MINORS, minors) );
    _data <<= 10;
    _data |= 0x3FF & unit.isonum();
  }
}

inline money::money() : _data(ISO_XXX) {}

inline money::money(int64_t majors, int64_t minors, currency unit) {
  init( majors * unit.num_minors() + minors, unit );
}

inline money money::pos_infinity(currency unit) {
  money ret;
  if (unit.num_minors() > 0) {
    ret._data = unit.isonum() | (detail::POS_INF_MINORS << 10);
  }
  return ret;
}

inline money money::neg_infinity(currency unit) {
  money ret;
  if (unit.num_minors() > 0) {
    ret._data = unit.isonum() | (detail::NEG_INF_MINORS << 10);
  }
  return ret;
}

inline double money::value() const {
  int64_t minors = _data >> 10;
  if (minors == detail::NEG_INF_MINORS) {
    return -std::numeric_limits<double>::infinity();
  } else if (minors == detail::POS_INF_MINORS) {
    return std::numeric_limits<double>::infinity();
  }
  return double(minors)/unit().num_minors();
}

inline currency money::unit() const {
  return currency(0x3FF & _data);
}

inline int64_t money::total_minors() const {
   return _data >> 10;
}

inline money & money::operator += (money rhs) {
  int64_t diff_bits = (_data ^ rhs._data);
  if (detail::CURRENCY_BITS & diff_bits) {
    _data = ISO_XXX;
  } else {
    _data += ~detail::CURRENCY_BITS & rhs._data;
    int64_t neg_if_sign_changed = (_data ^ rhs._data);
    int64_t neg_if_overflow = neg_if_sign_changed & ~diff_bits;
    if (neg_if_overflow < 0) fix_overflow();
  }
  return *this;
}

inline money & money::operator -= (money rhs) {
  return *this += -rhs;
}

inline money & money::operator *= (int32_t rhs) {
  std::pair<int64_t, uint32_t> p = detail::safe_multiply(total_minors(), rhs);
  int64_t minors;
  if (p.first < -(1LL << 31)) {
    minors = detail::NEG_INF_MINORS;
  } else if (p.first >= (1LL << 31)) {
    minors = detail::POS_INF_MINORS;
  } else {
    minors = (p.first << 32) + p.second;
  }
  minors = std::max( detail::NEG_INF_MINORS,
                     std::min(detail::POS_INF_MINORS, minors) );
  _data = (minors << 10) | (0x3FF & _data);
  return *this;
}

inline money money::operator - () const {
  return money(0, -this->total_minors(), this->unit());
}

inline money money::operator + (money rhs) const {
  money ret(*this);
  return ret += rhs;
}

inline money money::operator - (money rhs) const {
  money ret(*this);
  return ret -= rhs;
}

inline money money::operator * (int rhs) const {
  money ret = *this;
  return ret *= rhs;
}

inline void money::fix_overflow()
{
  int64_t minors = (_data < 0 ? detail::POS_INF_MINORS
                           : detail::NEG_INF_MINORS);
  _data &= detail::CURRENCY_BITS; // clear the minors bits
  _data |= (minors << 10);
}

inline money nextafter(money m)
{
  money ret;
  ret._data = m._data + (1LL << 10);
  int64_t neg_if_overflow = ~m._data & ret._data;
  if (neg_if_overflow < 0) {
    ret._data &= detail::CURRENCY_BITS;
    ret._data |= (detail::POS_INF_MINORS << 10);
  }
  return ret;
}

template <class _Number>
money floor(_Number value, currency unit)
{
  _Number minors = value * unit.num_minors();
  return detail::money_cast(minors, unit, number_traits<_Number>::floor);
}

template <class _Number>
money ceil(_Number value, currency unit)
{
  _Number minors = value * unit.num_minors();
  return detail::money_cast(minors, unit, number_traits<_Number>::ceil);
}

template <class _Number>
money trunc(_Number value, currency unit)
{
  _Number minors = value * unit.num_minors();
  return detail::money_cast(minors, unit, number_traits<_Number>::trunc);
}

template <class _Number>
money round(_Number value, currency unit)
{
  typedef number_traits<_Number> nt;
  _Number minors = value * unit.num_minors();
  return detail::money_cast(minors, unit, nt::roundhalfout);
}

template <class _Number>
money rounde(_Number value, currency unit)
{
  typedef number_traits<_Number> nt;
  _Number minors = value * unit.num_minors();
  return detail::money_cast(minors, unit, nt::roundhalfeven);
}


} // namespace isomon

#endif

