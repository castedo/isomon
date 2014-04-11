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
  const char* code;

  string name() { return code; }
  void test() {
    money m(0, 0, code);
    BOOST_CHECK_EQUAL( m.unit().c_str(), "XXX" );
    BOOST_CHECK_EQUAL( m.unit().str(), "XXX" );
    BOOST_CHECK( m.value() != m.value() );
  }
} no_currency_cases[] = {
  {"AAA"}, {"XXX"}, {"ZZZ"}, {"___"}, {"$"}, {"$$$"}
};

struct int_round_trip {
  int64_t i;
  const char* code;

  string name() { return lexical_cast<string>(i) + code; }
  void test() {
    money m(i, 0, code);
    BOOST_CHECK_EQUAL( m.value(), i );
    BOOST_CHECK_EQUAL( m.unit().c_str(), code );
    BOOST_CHECK_EQUAL( m.unit().str(), code );

    money m2(m.value(), 0, m.unit());
    BOOST_CHECK_EQUAL( m2.value(), i );
    BOOST_CHECK_EQUAL( m2.unit().c_str(), code );
    BOOST_CHECK_EQUAL( m2.unit().str(), code );
  }
} int_round_trip_cases[] = {
  {0, "USD"}, {1, "EUR"}, {-1, "EUR"},
  {(1LL << 46), "USD"}, {(1LL << 53) - 2, "JPY"} 
};

BOOST_AUTO_TEST_CASE( integer_multipy_test )
{
  money zero(0, 0, "USD");
  money cent(0, 1, "USD");
  money two_cent(0, 2, "USD");

  BOOST_CHECK_EQUAL( two_cent, cent * 2 );
  BOOST_CHECK_EQUAL( zero, cent * 0 );
  BOOST_CHECK_EQUAL( -two_cent, cent * -2 );
}

void check_safe_multiply(int64_t a, int32_t b,
                         int64_t expect_hi, uint32_t expect_lo)
{
  pair<int64_t, uint32_t> got = isomon::detail::safe_multiply(a, b);
  BOOST_CHECK_EQUAL( expect_hi, got.first ); 
  BOOST_CHECK_EQUAL( expect_lo, got.second ); 
}

BOOST_AUTO_TEST_CASE( safe_multiply_test )
{
  int64_t max64 = numeric_limits<int64_t>::max();
  int64_t min64 = numeric_limits<int64_t>::min();
  int64_t max32 = numeric_limits<int32_t>::max();
  int64_t min32 = numeric_limits<int32_t>::min();
  check_safe_multiply(0, 0, 0, 0);
  check_safe_multiply(1, 1, 0, 1);
  check_safe_multiply(-1, -1, 0, 1);
  check_safe_multiply(1, -1, -1, uint32_t(-1));
  check_safe_multiply(-1, 1, -1, uint32_t(-1));
  check_safe_multiply(min64, min32, 1LL << 62, 0);
  check_safe_multiply(min64, max32, -(1LL << 62) + (1LL << 31), 0);
  check_safe_multiply(max64, min32, -(1LL << 62), 1LL << 31);
  check_safe_multiply(max64, max32, (1LL<<62) - (1LL<<31) - 1, (1LL<<31) + 1);
}

BOOST_AUTO_TEST_CASE( big_integer_multipy_test )
{
  int64_t mucho = 1;
  mucho <<= 52;
  money i_want(0, mucho, "USD");
  money too_much(0, 2*mucho, "USD");

  BOOST_CHECK( isfinite(i_want) );
  BOOST_CHECK( !isfinite(too_much) );
  BOOST_CHECK( !isfinite(i_want * 2) );
  BOOST_CHECK( !isfinite(i_want * -2) );
  BOOST_CHECK( (i_want * 2).value() > 0 );
  BOOST_CHECK( (i_want * -2).value() < 0 );
  BOOST_CHECK( !isfinite(i_want * (1 << 31)) );
  BOOST_CHECK( !isfinite(i_want * (-1 << 31)) );
}

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
  money m(2, 0, "USD");
  stringstream ss;

  ss.imbue(locale::classic());
  ss << m;
  BOOST_CHECK_EQUAL(ss.str(), "USD 2.00");

  ss.str("");
  ss.imbue(locale("de_DE"));
  ss << m;
  BOOST_CHECK_EQUAL(ss.str(), "2,00 USD");

  money yen(2, 0, "JPY");
  ss.str("");
  ss.imbue(locale("de_DE"));
  ss << yen;
  BOOST_CHECK_EQUAL(ss.str(), "2 JPY");

  money euro(2, 0, "EUR");
  ss.str("");
  ss.imbue(locale("en_IE"));
  ss << euro;
  BOOST_CHECK_EQUAL(ss.str(), "2.00 EUR");

  ss.str("");
  ss.imbue(locale::classic());
  ss << euro;
  BOOST_CHECK_EQUAL(ss.str(), "EUR 2.00");
}

BOOST_AUTO_TEST_CASE( rounding_test )
{
  money zero(0, 0, "USD");
  money cent(0, 1, "USD");
  money two(0, 2, "USD");
  money three(0, 3, "USD");

  BOOST_CHECK_EQUAL( cent, round(0.005, "USD") );
  BOOST_CHECK_EQUAL( zero, rounde(0.005, "USD") );
  BOOST_CHECK_EQUAL( two, round(0.015, "USD") );
  BOOST_CHECK_EQUAL( two, rounde(0.015, "USD") );
  BOOST_CHECK_EQUAL( three, round(0.025, "USD") );
  BOOST_CHECK_EQUAL( two, rounde(0.025, "USD") );

  BOOST_CHECK_EQUAL( -cent, round(-0.005, "USD") );
  BOOST_CHECK_EQUAL( zero, rounde(-0.005, "USD") );
  BOOST_CHECK_EQUAL( -two, round(-0.015, "USD") );
  BOOST_CHECK_EQUAL( -two, rounde(-0.015, "USD") );
  BOOST_CHECK_EQUAL( -three, round(-0.025, "USD") );
  BOOST_CHECK_EQUAL( -two, rounde(-0.025, "USD") );
}

BOOST_AUTO_TEST_CASE( six_fifths_rounding_test )
{
  double not_really_two = (1.2 - 1) * 10;

  BOOST_CHECK( not_really_two < 2 );
  BOOST_CHECK_EQUAL( not_really_two, 0x1.FFFFFFFFFFFFEp+0 );

  money two(2, 0, "USD");
  money one_99(1, 99, "USD");

  BOOST_CHECK_EQUAL( two, round(not_really_two, "USD") );
  BOOST_CHECK_EQUAL( two, rounde(not_really_two, "USD") );
  BOOST_CHECK_EQUAL( two, ceil(not_really_two, "USD") );
  BOOST_CHECK_EQUAL( one_99, trunc(not_really_two, "USD") );
  BOOST_CHECK_EQUAL( one_99, floor(not_really_two, "USD") );
}

#endif

