#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_NO_MAIN

#include "test-currency.hpp"
#include "test-money.hpp"
#include "test-money_calc.hpp"

#include <iostream>
#include <string>
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace boost;
using namespace boost::unit_test;


//////////////////////////////////////////////////////////////////////////
/// Functions to get boost::test initialized with test cases

#include <boost/bind.hpp>
#include <boost/range.hpp>

template<class T, size_t sz>
void add_test_cases(T (&cases)[sz], char const* test_name)
{
  for (T* p = cases; p < end(cases); ++p) {
    string name = string(test_name) + ":" + p->name();
    test_case * tc = make_test_case(bind(&T::test, *p), name);
    framework::master_test_suite().add(tc); 
  }
}

#define ADD_TEST_CASES(test_cases) add_test_cases(test_cases, #test_cases)

bool init_unit_test()
{
  // test-currency.hpp
  ADD_TEST_CASES(invalid_cases);
  ADD_TEST_CASES(invalid_num_cases);
  ADD_TEST_CASES(case_insensitive_cases);
  ADD_TEST_CASES(compare_cases);
  ADD_TEST_CASES(known_currency_cases);

  // test-money.hpp
  ADD_TEST_CASES(no_currency_cases);     
  ADD_TEST_CASES(int_round_trip_cases);     

  return true;
}

int main(int argc, char* argv[])
{
  return unit_test::unit_test_main(&init_unit_test, argc, argv);
}

