#include "currency_data.h"

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

bool pass_hashes(char A)
{
  char buf[4];
  buf[3] = '\0';
  for (char ch = A; ch < A+26; ++ch) {
    buf[0] = (buf[1] = (buf[2] = ch));
    int16_t hash;
    code2hash(buf, &hash);
    char b[4];
    hash2code(hash, b);
    if (b[3] != '\0') return false;
    char up = toupper(ch);
    if (b[0] != up || b[1] != up || b[2] != up) return false;
  }
  return true;
}

bool pass_hash_of_bad_code()
{
  char buf[4];
  buf[3] = '\0';
  for (char ch = '0'; ch <= '9'; ++ch) {
    buf[0] = (buf[1] = (buf[2] = ch));
    int16_t hash;
    if (code2hash(buf, &hash)) return false;
  }
  return true;
}

int pass_known_currency(const char *code, int num, int minors)
{
  int16_t hash;
  int16_t isonum;
  if (!(code2hash(code, &hash) && hash2isonum(hash, &isonum))) return false;
  if (isonum != num) return false;
  if (strcmp(code, isonum2code(isonum)) != 0) return false;
  if (minors != num_minors(isonum)) return false;
  return true;
}

#define DO_TEST0(func) \
    if (!func()) { printf(#func " FAILED!\n"); return false; }

#define DO_TEST1(func, a) \
    if (!func(a)) { printf(#func "(" #a ") FAILED!\n"); return false; }

#define DO_TEST2(func, a, b) \
    if (!func(a, b)) { \
      printf(#func "(" #a ", " #b ") FAILED!\n"); \
      return false; \
     }

#define DO_TEST3(func, a, b, c) \
    if (!func(a, b, c)) { \
      printf(#func "(" #a ", " #b ", " #c ") FAILED!\n"); \
      return false; \
     }

bool pass_all_unmodified()
{
  DO_TEST1(pass_hashes, 'A');
  DO_TEST1(pass_hashes, 'a');
  DO_TEST0(pass_hash_of_bad_code);
  DO_TEST3(pass_known_currency, "USD", 840, 100);
  DO_TEST3(pass_known_currency, "XXX", 999, 0);
  DO_TEST3(pass_known_currency, "EUR", 978, 100);
  DO_TEST3(pass_known_currency, "USD", 840, 100);
  DO_TEST3(pass_known_currency, "CNY", 156, 100);
  DO_TEST3(pass_known_currency, "CLP", 152, 1);
  DO_TEST3(pass_known_currency, "MXN", 484, 100);
  DO_TEST3(pass_known_currency, "GBP", 826, 100);
  DO_TEST3(pass_known_currency, "JPY", 392, 1);
  DO_TEST3(pass_known_currency, "AUD", 36, 100);
  DO_TEST3(pass_known_currency, "RUB", 643, 100);
  DO_TEST3(pass_known_currency, "SGD", 702, 100);
  DO_TEST3(pass_known_currency, "ZAR", 710, 100);
  DO_TEST3(pass_known_currency, "KWD", 414, 1000);
  DO_TEST3(pass_known_currency, "ILS", 376, 100);
  return true;
}

bool pass_all()
{
  if (!pass_all_unmodified()) return false;
  DO_TEST3(pass_known_currency, "INR", 356, 100);

  // after June 2011, half Rupee is smallest unit
  if (!set_num_minors(356, 2, 1)) return false;
  DO_TEST3(pass_known_currency, "INR", 356, 2);
  if (!pass_all_unmodified()) return false;

  if( !add_currency(1, "DOG") ) return false;
  DO_TEST3(pass_known_currency, "DOG", 1, 0);
  DO_TEST3(pass_known_currency, "INR", 356, 2);
  if (!pass_all_unmodified()) return false;
  return true;
}

int main()
{
  if (pass_all()) {
    printf("All tests passed.\n");
    return 0;
  } else {
    printf("A test FAILED!.\n");
    return 1;
  }
}

