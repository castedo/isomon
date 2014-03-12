#ifndef ISOMON_MONEY_CALC_HPP
#define ISOMON_MONEY_CALC_HPP

#include "money.hpp"

namespace isomon {


template <class _Number>
struct money_calc {};

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

  money_calc & operator += (money_calc const& rhs) {
    if (this->unit == rhs.unit) {
      this->minors += rhs.minors;
    } else {
       minors = NAN;
       unit = ISO_XXX;
    }
    return *this;
  }

  money_calc operator + (money_calc const& rhs) const {
    money_calc ret(*this);
    ret += rhs;
    return ret;
  }

  money_calc & operator *= (double rhs) {
    this->minors *= rhs;
    return *this;
  }

  money_calc operator * (double rhs) const {
    money_calc ret(*this);
    ret *= rhs;
    return ret;
  }
};

inline bool isfinite(money_calc<double> const& mc) { 
  return std::isfinite(mc.minors);
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

template <class _Number>
money trunc(money_calc<_Number> const& mc) {
  typedef number_traits<_Number> nt;
  if (nt::isnan(mc.minors) || mc.unit.num_minors() < 1) return money();
  if (nt::isinf(mc.minors)) {
    if (mc.minors > 0) return money::pos_infinity(mc.unit);
    else return money::neg_infinity(mc.unit);
  }
  return money(0, nt::trunc(mc.minors), mc.unit); 
}

template <class _Number>
money round(money_calc<_Number> const& mc) {
  typedef number_traits<_Number> nt;
  if (nt::isnan(mc.minors) || mc.unit.num_minors() < 1) return money();
  if (nt::isinf(mc.minors)) {
    if (mc.minors > 0) return money::pos_infinity(mc.unit);
    else return money::neg_infinity(mc.unit);
  }
  return money(0, nt::roundhalfout(mc.minors), mc.unit); 
}


} // namespace

#endif

