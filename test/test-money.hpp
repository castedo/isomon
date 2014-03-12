#ifndef ISOMON_TEST_MONEY_HPP
#define ISOMON_TEST_MONEY_HPP

#include "money.hpp"

#include <sstream>
#include <boost/test/unit_test.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;
using namespace boost::unit_test;
using namespace isomon;

struct no_currency {
  double v;
  const char* code;

  string name() { return lexical_cast<string>(v) + code; }
  void test() {
    money m(v, code);
    BOOST_CHECK_EQUAL( m.unit().c_str(), "XXX" );
    BOOST_CHECK_EQUAL( m.unit().str(), "XXX" );
    BOOST_CHECK( m.value() != m.value() );
  }
} no_currency_cases[] = {
  {0, "AAA"}, {0, "XXX"}, {0, "ZZZ"}, {0, "___"}, {0, "$"}, {0, "$$$"}
};

struct int_round_trip {
  int64_t i;
  const char* code;

  string name() { return lexical_cast<string>(i) + code; }
  void test() {
    money m(i, code);
    BOOST_CHECK_EQUAL( m.value(), i );
    BOOST_CHECK_EQUAL( m.unit().c_str(), code );
    BOOST_CHECK_EQUAL( m.unit().str(), code );

    money m2(m.value(), m.unit());
    BOOST_CHECK_EQUAL( m2.value(), i );
    BOOST_CHECK_EQUAL( m2.unit().c_str(), code );
    BOOST_CHECK_EQUAL( m2.unit().str(), code );
  }
} int_round_trip_cases[] = {
  {0, "USD"}, {1, "EUR"}, {-1, "EUR"},
  {(1LL << 46), "USD"}, {(1LL << 53) - 2, "JPY"} 
};

BOOST_AUTO_TEST_CASE( near_zero_subtraction_test )
{
  money cent(0, 1, "USD");
  money zero(0, 0, "USD");

  BOOST_CHECK_EQUAL( cent - cent, zero );
  BOOST_CHECK_EQUAL( -cent + cent, zero );
  BOOST_CHECK_CLOSE( cent.value(), 0.01, 1e-6);
  BOOST_CHECK_CLOSE( (-cent).value(), -0.01, 1e-6);
  BOOST_CHECK_EQUAL( zero - cent, -cent);
}

BOOST_AUTO_TEST_CASE( output_test )
{
  money m(2.0, "USD");
  stringstream ss;

  ss.imbue(locale::classic());
  ss << m;
  BOOST_CHECK_EQUAL(ss.str(), "USD 2.00");

  ss.str("");
  ss.imbue(locale("de_DE"));
  ss << m;
  BOOST_CHECK_EQUAL(ss.str(), "2,00 USD");

  money yen(2, "JPY");
  ss.str("");
  ss.imbue(locale("de_DE"));
  ss << yen;
  BOOST_CHECK_EQUAL(ss.str(), "2 JPY");

  money euro(2, "EUR");
  ss.str("");
  ss.imbue(locale("en_IE"));
  ss << euro;
  BOOST_CHECK_EQUAL(ss.str(), "2.00 EUR");

  ss.str("");
  ss.imbue(locale::classic());
  ss << euro;
  BOOST_CHECK_EQUAL(ss.str(), "EUR 2.00");
}

#endif

