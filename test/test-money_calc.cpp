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
  money m(2, 0, "USD");

  m = round(m * 1.5);
  BOOST_CHECK_EQUAL(m.total_minors(), 300);

  money m2 = round(m * 1.001);
  BOOST_CHECK_EQUAL(m2.total_minors(), 300);

  m2 = round(m * 1.002);
  BOOST_CHECK_EQUAL(m2.total_minors(), 301);
}

BOOST_AUTO_TEST_CASE( multiply_test )
{
  money m(3, 0, "USD");
  money_calc<double> mc(1.5, "USD");

  BOOST_CHECK_EQUAL( round(m * 2.0), round(mc * 4) );
}

BOOST_AUTO_TEST_CASE( addition_test )
{
  money_calc<double> one_third_dollar( 1.0/3.0, "USD" );
  money buck(1, 0, "USD");

  BOOST_CHECK_EQUAL( buck, round(3 * one_third_dollar) );
  money_calc<double> sum = one_third_dollar + one_third_dollar;
  sum += one_third_dollar;
  BOOST_CHECK_EQUAL( buck, round(sum) );
}

BOOST_AUTO_TEST_CASE( negation_test )
{
  money thirty_three_cents(0, 33, "USD");
  money neg_thirty_three_cents(0, -33, "USD");
  money buck(1, 0, "USD");
  money_calc<double> one_third_dollar( 1.0/3.0, "USD" );
  money_calc<double> neg_one_third_dollar( -1.0/3.0, "USD" );

  BOOST_CHECK_EQUAL( -thirty_three_cents, neg_thirty_three_cents );
  BOOST_CHECK_EQUAL( buck - thirty_three_cents, money(0, 67, "USD") );
  BOOST_CHECK_EQUAL( -buck + thirty_three_cents, -money(0, 67, "USD") );
  BOOST_CHECK_EQUAL( round(-one_third_dollar), round(neg_one_third_dollar) );
}

BOOST_AUTO_TEST_CASE( mutlicurrency_nonaddition_test )
{
  money bad = money(1, 0, "USD") + money(1, 0, "EUR");
  BOOST_CHECK( bad.unit().is_no_currency() ); 
}


#endif

