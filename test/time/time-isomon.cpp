#include "money.hpp"
#include "money_calc.hpp"

#include <tr1/ctime>
#include <tr1/cmath>
#include <cstdlib>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace isomon;

template<class Quant, class Test>
void clockit(long rep, Quant tot, Quant final, Test t, char const* name)
{
  clock_t t0 = clock();
  for (long i=0; i < rep/10; ++i) {
    tot = t.calc(tot);
    tot = t.calc(tot);
    tot = t.calc(tot);
    tot = t.calc(tot);
    tot = t.calc(tot);
    tot = t.calc(tot);
    tot = t.calc(tot);
    tot = t.calc(tot);
    tot = t.calc(tot);
    tot = t.calc(tot);
  }
  clock_t diff = clock() - t0;

  double denom = double(rep)*double(CLOCKS_PER_SEC)/1e9;
  cout << diff/denom << "ns | " << name << " | ";
  if (t.equal(tot, final)) {
    cout << "Equal" << endl;
  } else {
    cout << tot << " != " << final << endl;
  }
}

#define CLOCKIT(rep, tot, end, func) clockit(rep, tot, end, func, #func)

struct test_double {
  bool equal(double x, double y) {
    return round(x) == round(y);
  }
};

struct test_money {
  bool equal(money x, money y) {
    return x == y;
  }
};

struct increment_double : test_double {
  double calc(double d) { return d + 0.01; }
};

struct increment_money : test_money {
  money calc(money m) { return nextafter(m); }
};

struct interest_double : test_double {
  double interest;
  interest_double(double d) : interest(d) {}

  double calc(double d) {
    double minors = round(d * 100);
    return d + tr1::trunc(minors * interest)/100.0;
  }
};

struct interest_money : test_money {
  double interest;
  interest_money(double d) : interest(d) {}

  money calc(money m) {
    m += trunc(m * interest);
    return m;
  }
};


int main(int argc, char* argv[])
{
  long megs = 10;
  if (argc > 1) {
    megs = atoi(argv[1]);
  }
  long rep = megs*1e6;

  cout << fixed;

  CLOCKIT(rep, 0.0, 0.01*rep, increment_double());

  CLOCKIT(rep, money(0,"EUR"), money(rep/100,"EUR"), increment_money());

  double dr = 0.0005/365.0;

  CLOCKIT(rep, 10000.0, 5278454643.71, interest_double(dr));

  CLOCKIT(rep, money(10000, 0, "EUR"), money(5278454643, 71, "EUR"), interest_money(dr));

  CLOCKIT(rep, -10000.0, -5278454643.71, interest_double(dr));

  CLOCKIT(rep, money(-10000, 0, "EUR"), money(-5278454643, -71, "EUR"), interest_money(dr));

  return 0;
}

