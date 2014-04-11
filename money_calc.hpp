#ifndef ISOMON_MONEY_CALC_HPP
#define ISOMON_MONEY_CALC_HPP

#include "money.hpp"

namespace isomon {


template <class _Number>
struct money_calc {};

typedef money_calc<double> money_double;

template <>
struct money_calc<double>
{
  double minors;
  currency unit;

  money_calc() : minors(NAN) {}
  money_calc(double m, currency u) : minors(m * u.num_minors()), unit(u) {}
  money_calc(money m) : minors(m.total_minors()), unit(m.unit()) {}

  double value() const { return minors / unit.num_minors(); }

  #if __cplusplus >= 201103L
  explicit operator double const () { return this->value(); }
  #endif

  money_calc & operator += (double rhs) {
    this->minors += rhs * unit.num_minors();
    return *this;
  }

  money_calc & operator += (money rhs) {
    if (this->unit == rhs.unit()) {
      this->minors += rhs.total_minors();
    } else {
       minors = NAN;
       unit = ISO_XXX;
    }
    return *this;
  }

  money_calc & operator += (money_calc const& rhs) {
    if (this->unit == rhs.unit) {
      this->minors += rhs.minors;
    } else {
       minors = NAN;
       unit = ISO_XXX;
    }
    return *this;
  }

  money_calc operator + (double rhs) const {
    money_calc ret(*this);
    ret += rhs;
    return ret;
  }

  money_calc operator + (money rhs) {
    money_calc ret(*this);
    ret += rhs;
    return ret;
  }

  money_calc operator + (money_calc const& rhs) const {
    money_calc ret(*this);
    ret += rhs;
    return ret;
  }

  money_calc operator - () const {
    money_calc ret(*this);
    ret.minors = -ret.minors;
    return ret;
  }

  money_calc & operator -= (double rhs) { return *this += -rhs; }
  money_calc & operator -= (money rhs) { return *this += -rhs; }
  money_calc & operator -= (money_calc const& rhs) { return *this += -rhs; }
  money_calc operator - (double rhs) const { return *this + -rhs; }
  money_calc operator - (money rhs) const { return *this + -rhs; }
  money_calc operator - (money_calc const& rhs) const { return *this + -rhs; }

  money_calc & operator *= (double rhs) {
    this->minors *= rhs;
    return *this;
  }

  money_calc & operator /= (double rhs) {
    this->minors /= rhs;
    return *this;
  }

  money_calc operator * (double rhs) const {
    money_calc ret(*this);
    ret *= rhs;
    return ret;
  }

  money_calc operator / (double rhs) const {
    money_calc ret(*this);
    ret /= rhs;
    return ret;
  }

  bool operator > (money_calc const& mc) const {
    if (unit.is_no_currency()) return false;
    return minors > mc.minors && unit == mc.unit;
  }

  bool operator >= (money_calc const& mc) const {
    if (unit.is_no_currency()) return false;
    return minors >= mc.minors && unit == mc.unit;
  }

  bool operator < (money_calc const& mc) const { return mc > *this; }
  bool operator <= (money_calc const& mc) const { return mc >= *this; }
  bool operator > (money m) const { return *this > money_calc(m); }
  bool operator >= (money m) const { return *this >= money_calc(m); }
  bool operator < (money m) const { return *this < money_calc(m); }
  bool operator <= (money m) const { return *this <= money_calc(m); }
};

inline bool operator > (money m, money_calc<double> mc) { return mc < m; } 
inline bool operator >= (money m, money_calc<double> mc) { return mc <= m; }
inline bool operator < (money m, money_calc<double> mc) { return mc > m; }
inline bool operator <= (money m, money_calc<double> mc) { return mc >= m; }

inline bool isfinite(money_calc<double> const& mc) { 
  return std::isfinite(mc.minors);
}

inline money_calc<double> operator + (money m, double x) {
  return money_calc<double>(m) + x;
}

inline money_calc<double> operator + (double x, money m) {
  return m + x;
}

inline money_calc<double> operator + (money m, money_calc<double> mc) {
  return mc + m;
}

inline money_calc<double> operator * (money m, double x) {
  return money_calc<double>(m) * x;
}

inline money_calc<double> operator * (double x, money_calc<double> mc) {
  return mc * x;
}

inline money_calc<double> operator * (double x, money m) {
  return m * x;
}

inline money_calc<double> operator / (money m, double x) {
  return money_calc<double>(m) / x;
}

template <class _Number>
money floor(money_calc<_Number> const& mc) {
  return detail::money_cast(mc.minors, mc.unit, number_traits<_Number>::floor);
}

template <class _Number>
money ceil(money_calc<_Number> const& mc) {
  return detail::money_cast(mc.minors, mc.unit, number_traits<_Number>::ceil);
}

template <class _Number>
money trunc(money_calc<_Number> const& mc) {
  return detail::money_cast(mc.minors, mc.unit, number_traits<_Number>::trunc);
}

template <class _Number>
money round(money_calc<_Number> const& mc) {
  typedef number_traits<_Number> nt;
  return detail::money_cast(mc.minors, mc.unit, nt::roundhalfout);
}

template <class _Number>
money rounde(money_calc<_Number> const& mc) {
  typedef number_traits<_Number> nt;
  return detail::money_cast(mc.minors, mc.unit, nt::roundhalfeven);
}


} // namespace

#endif

