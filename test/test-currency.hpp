#ifndef ISOMON_TEST_CURRENCY_HPP
#define ISOMON_TEST_CURRENCY_HPP

#include "currency.hpp"

#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;
using namespace boost::unit_test;
using namespace isomon;

struct invalid {
  char const* code;

  string name() { return code; }
  void test() {
    currency c(code);
    BOOST_CHECK_EQUAL( c.isonum(), 999 );
    BOOST_CHECK_EQUAL( c.str(), "XXX" );
    BOOST_CHECK_EQUAL( c.c_str(), "XXX" );
  }
} invalid_cases[] = {
  {"AAA"}, {"ZZZ"}, {"$"}, {"A"}, {"BC"}, {"ABCD"}, {"#'@;"}
};

struct invalid_nums {
  int16_t num;

  string name() { return lexical_cast<string>(num); }
  void test() {
    currency c(num);
    BOOST_CHECK_EQUAL( c.isonum(), 999 );
    BOOST_CHECK_EQUAL( c.str(), "XXX" );
    BOOST_CHECK_EQUAL( c.c_str(), "XXX" );
  }
} invalid_num_cases[] = {
  {-1}, {-999}, {1}, {1000}, {12345}
};

struct case_insensitive {
  char const* code;

  string name() { return code; }
  void test() {
    currency c(code);
    string up(code);
    to_upper(up);
    BOOST_CHECK_EQUAL( c.str(), up );
    currency cup(up);
    BOOST_CHECK_EQUAL( c, cup );
  }
} case_insensitive_cases[] = {
  {"usd"}, {"uSd"}, {"gBp"}, {"EuR"}, {"jpy"}, {"cHf"}
};

struct compare {
  currency less;
  currency more;

  string name() { return less.str() + more.str(); }
  void test() {
    BOOST_CHECK( less < more );
  }
} compare_cases[] = {
  {ISO_USD, ISO_XXX}, {ISO_JPY, ISO_USD}, {ISO_EUR, ISO_USD}, {ISO_GBP, ISO_JPY}
};

struct known_currency {
  char const* code; int16_t num; int num_minors;

  string name() { return code; }
  void test() {
    currency c(code);
    BOOST_CHECK_EQUAL( c.c_str(), code );
    BOOST_CHECK_EQUAL( c.str(), code );
    BOOST_CHECK_EQUAL( c.isonum(), num );
    BOOST_CHECK_EQUAL( c.num_minors(), num_minors );
    currency ci(num);
    BOOST_CHECK_EQUAL( ci.c_str(), code );
    BOOST_CHECK_EQUAL( ci.str(), code );
    BOOST_CHECK_EQUAL( ci.isonum(), num );
    BOOST_CHECK_EQUAL( ci.num_minors(), num_minors );
  }
} known_currency_cases[] = {
  {"XXX", 999, 0},
  {"EUR", 978, 100},
  {"USD", 840, 100},
  {"CNY", 156, 100},
  {"CLP", 152, 1},
  {"MXN", 484, 100},
  {"GBP", 826, 100},
  {"JPY", 392, 1},
  {"AUD", 36, 100},
  {"RUB", 643, 100},
  {"SGD", 702, 100},
  {"ZAR", 710, 100},
  {"INR", 356, 100},
  {"KWD", 414, 1000},
  {"ILS", 376, 100}
};

#endif

