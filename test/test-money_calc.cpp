#ifndef ISOMON_TEST_MONEY_CALC_HPP
#define ISOMON_TEST_MONEY_CALC_HPP

#include "money_calc.hpp"

#include <boost/test/unit_test.hpp>

using namespace std;
using namespace boost;
using namespace boost::unit_test;
using namespace isomon;


BOOST_AUTO_TEST_CASE( roundhalfout_test )
{
  money m(2.0, "USD");

  m = roundhalfout(m * 1.5);
  BOOST_CHECK_EQUAL(m.total_minors(), 300);

  money m2 = roundhalfout(m * 1.001);
  BOOST_CHECK_EQUAL(m2.total_minors(), 300);

  m2 = roundhalfout(m * 1.002);
  BOOST_CHECK_EQUAL(m2.total_minors(), 301);
}

#endif

