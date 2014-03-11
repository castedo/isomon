#ifndef ISOMON_MONEY_CALC_HPP
#define ISOMON_MONEY_CALC_HPP

#include "money.hpp"

namespace isomon {


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


} // namespace

#endif

