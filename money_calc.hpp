#ifndef ISOMON_MONEY_CALC_HPP
#define ISOMON_MONEY_CALC_HPP

#include "money.hpp"

#include <tr1/cmath>

namespace isomon {

template <class _Number>
struct number_traits {};

template <class _Number>
struct money_calc {
  _Number minors;
  currency unit;

  money_calc( _Number m, currency u) : minors(m), unit(u) {}
};

template <class _Number>
money_calc<_Number> operator * (money a, _Number b) {
  return money_calc<_Number>(a.total_minors() * b, a.unit());
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
money roundhalfout(money_calc<_Number> const& mc) {
  typedef number_traits<_Number> nt;
  if (nt::isnan(mc.minors) || mc.unit.num_minors() < 1) return money();
  if (nt::isinf(mc.minors)) {
    if (mc.minors > 0) return money::pos_infinity(mc.unit);
    else return money::neg_infinity(mc.unit);
  }
  return money(0, nt::roundhalfout(mc.minors), mc.unit); 
}

template<>
struct number_traits<double>
{
  static bool isnan(double x) { return std::isnan(x); }

  static bool isinf(double x) { return std::isinf(x); }

  static int64_t floor(double x) { return std::floor(x); }

  static int64_t ceil(double x) { return std::ceil(x); }

  static int64_t trunc(double x) { return std::tr1::trunc(x); }

  static int64_t roundhalfout(double x) { return std::tr1::round(x); }

  // round half (towards) even, "banker's rounding"
  static int64_t roundhalfeven(double x) {
    int64_t halfout = roundhalfout(x);
    if (halfout % 2) { // if odd, might need to adjust
      if (x * 2 + 1 == double(2.0 * halfout)) {
        // x is exactly halfway between two integers
        // and was rounded OUT (away from zero) to an odd number
        // we want to round IN (toward zero) to an even number instead
        return (halfout < 0 ? halfout + 1 : halfout - 1);
      }
    }
    return halfout;
  }
};

} // namespace

#endif

