#ifndef ISOMON_CURRENCY_DATA_H
#define ISOMON_CURRENCY_DATA_H

/** @file currency_data.h
    @brief Currency low-level C functions.
*/

// One translation unit must define ISOMON_INCLUDE_DEFINITIONS.
// If using C99, one and only one translation unit must define.
// If using C++, many translation units may define,
//   but only within one and only one module (library/executable).

#include "iso_table_data.h"

#if defined(__cplusplus)
namespace isomon {
#else
#include <stdbool.h>
#endif


/// Type for ISO numeric code values this library will accept.
/** Values returned by functions hash2isonum and code2isonum 
    should be the only soures for valid ISO numeric values for this type.
    is_isonum(int i) will return true for valid values of this type.
*/
typedef int16_t isonum_t;

/// Number of possible ISO numeric code values this library will handle.
/** ISO numeric code values are between 0 and (isonum_count - 1).
    Use macro ISOMON_ISONUM_COUNT if using C instead of C++.
*/
#if defined(__cplusplus)
const size_t isonum_count = ISOMON_ISONUM_COUNT;
#endif

#if defined(__cplusplus)
namespace data {
#endif


#ifdef ISOMON_INCLUDE_DEFINITIONS
// The C99 translation unit compiling these extern lines contains non-inlined
// function definitions for other translation units that decide not to inline.
extern bool add_currency(int16_t isonum, const char * code);
extern bool set_num_minors(int16_t isonum,
                           int16_t num_minors, int8_t num_digits);
extern bool code2hash(const char *code, int16_t * out);
extern void hash2code(int16_t hash, char *out);
extern bool hash2isonum(int16_t hash, isonum_t * out);
extern int16_t isonum2hash(isonum_t isonum);
extern bool code2isonum(const char * alpha_code, isonum_t * out);
extern const char * isonum2code(isonum_t isonum);
extern bool is_isonum(int16_t i);
extern int8_t num_minor_digits(isonum_t isonum);
extern int16_t num_minors(isonum_t isonum);

extern int32_t * lower_bound(int32_t *it, size_t count, int32_t findme);
#endif


#ifndef DOXYGEN_SHOULD_SKIP_THIS
// binary search to iterate it to first element not less than findme
// Essentially the same as std::lower_count but this is here for C99.
inline int32_t * lower_bound(int32_t *it, size_t count, int32_t findme)
{
  while (count > 0) {
    size_t probe_offset = count/2;
    if ( it[probe_offset] < findme ) {
      size_t definitely_skip_cnt = probe_offset + 1;
      it += definitely_skip_cnt;
      count -= definitely_skip_cnt;
    } else {
      count = probe_offset;
    }
  }
  return it;
}
#endif


//! Low-level C function to hash ISO currency code.
/** Perfect hash, using smallest number of bits possible (15 bits)
    for ISO 4127 alphabetic codes which are 3 latin letters.
    @param code Pointer to a null-teminated string with ISO code.
    @param out NON-NULL pointer to integer to hold hash.
    @return True iff code is a 3 Latin alphabet letter string and
            hash written to out.
*/
inline bool code2hash(const char *code, int16_t * out)
{
  if (code == 0) return false;
  int16_t hash = 0;
  const char *end = code + 3;
  while (code < end) {
    char ch = *code & 0xDF; // safely make lower cases upper case
    if (ch < 'A' || ch > 'Z') return false;
    hash <<= 5;
    hash |= ch & 0x1F;
    ++code;
  }
  if (*code != '\0') return false;
  *out = hash;
  return true;
}

/// Low-level C function to return ISO currency code from hash.
/** @param hash Hash from code2hash to convert back to ISO currency code.
    @param out NON-NULL pointer to at least 4 bytes of memory to get
    null-terminated C string of 3 letters.
*/
inline void hash2code(int16_t hash, char *out)
{
  char *p = out+3;
  *p = '\0';
  while (p != out) {
    *(--p) = 0x40 + (hash & 0x1F);
    hash >>= 5;
  }
}

/// Low-level C function to return ISO numeric from hash of alphabetic code.
/** @param hash of ISO alphabetic code as returned by code2hash.
    @param out NON-ZERO pointer to isonum_t variable to hold ISO numeric.
    @return true iff hash2code(hash) found as ISO alphabetic code
            and ISO numeric written to out.
*/
inline bool hash2isonum(int16_t hash, isonum_t * out)
{
  int32_t *it = lower_bound(g_hash_to_isonum, g_isomon_hash_count, hash << 16);
  int32_t *end = g_hash_to_isonum + g_isomon_hash_count;
  if ( it < end && hash == (*it >> 16) ) {
    *out = *it % ISOMON_ISONUM_COUNT;
    return true;
  }
  return false;
}

/// Low-level C function returning hash of ISO alphabetic code given numeric
/** @param isonum ISO numeric code: is_isonum(isonum) is true 
    @return hash of ISO alphabetic code, same as code2hash,
            which is ordered the same way as the alphabetic code
*/
inline int16_t isonum2hash(isonum_t isonum)
{
  return g_isonum_to_hash[isonum % ISOMON_ISONUM_COUNT];
}

/// Low-level C function to return ISO numeric from string.
/** @param alpha_code null-terminated string of ISO code.
    @param out NON-ZERO pointer to isonum_t variable to hold ISO numeric.
    @return true iff alpha_code found as ISO alphabetic code
            and ISO numeric written to out.
*/
inline bool code2isonum(const char * alpha_code, isonum_t * out)
{
  int16_t hash;
  return (code2hash(alpha_code, &hash) && hash2isonum(hash, out));
}

/// Low-level C function returning ISO alphabetic code given ISO numeric.
/** @param isonum ISO numeric code: is_isonum(isonum) is true
    @return ISO 3 letter alphabetic code if isonum found.
*/
inline const char * isonum2code(isonum_t isonum)
{
  return g_isonum_to_code[isonum % ISOMON_ISONUM_COUNT];
}

/// Low-level C function testing if ISO numeric found in data tables.
/**
  @param i integer which might be ISO numeric code (0 represents ISO "Nil")
  @return True if ISO numeric code found.
*/
inline bool is_isonum(int16_t i)
{
  return i >= 0 && i < ISOMON_ISONUM_COUNT && g_isonum_to_hash[i] != 0;
}

/// Low-level C function returning minor units per major unit.
/** Number of decimal digits for minors units for given ISO currency numeric code.
    @param isonum ISO numeric code: is_isonum(isonum) is true
    @return Number of decimal digits needed for minor units
    0 is returned if "currency" does not have minor units (XAU, XXX, ...)
*/
inline int8_t num_minor_digits(isonum_t isonum)
{
  size_t i = (isonum % ISOMON_ISONUM_COUNT) / 2;
  size_t scale = g_isonum_to_minor_scale[i] >> (4 * (isonum % 2));
  return g_minor_scale_to_num_digits[scale % ISOMON_MINOR_SCALE_COUNT];
}

/// Low-level C function returning minor units per major unit.
/** Number of minor units per major unit for given ISO currency numeric code.
    @param isonum ISO numeric code: is_isonum(isonum) is true
    @return Number of minor units in major unit (100 for EUR, 1 for JPY, ...)
      0 is returned if "currency" does not have minor units (XAU, XXX, ...)
*/
inline int16_t num_minors(isonum_t isonum)
{
  size_t i = (isonum % ISOMON_ISONUM_COUNT) / 2;
  size_t scale = g_isonum_to_minor_scale[i] >> (4 * (isonum % 2));
  return g_minor_scale_to_num_minors[scale % ISOMON_MINOR_SCALE_COUNT];
}


/// Add a currency to the data tables.
/** Not thread-safe. Typically this would only be called during program
    initialization.
    ISO numeric and alphabetic codes can not be re-used if already in use.
    Number of minor units per major unit default to zero. Use set_num_minors
    function to change this value for a newly added currency.
    @param isonum An ISO numeric code, real or fake, in the range
    [0, ISOMON_ISONUM_COUNT). ISOMON_ISONUM_COUNT is probably 1024.
    @param code An ISO alphabetic code that is 3 Latin letters.
    @return False iff any inputs are invalid or ISOMON_ISONUM_COUNT
    currencies have already filled up the tables.
*/
inline
bool add_currency(int16_t isonum, const char * code)
{
  if (g_isomon_hash_count >= ISOMON_ISONUM_COUNT) return false; // tables full

  if (isonum < 0 || isonum >= ISOMON_ISONUM_COUNT) return false; // bad isonum

  if (is_isonum(isonum)) return false; // numeric code already in use

  int16_t hash;
  if (!code2hash(code, &hash)) return false; // not 3 Latin letters

  int32_t *it = lower_bound(g_hash_to_isonum, g_isomon_hash_count, hash << 16);
  int32_t *end = g_hash_to_isonum + g_isomon_hash_count;
  if (it < end && hash == (*it >> 16)) return false; // alpha code in use 

  // input has been validated now start changing the tables
  ++g_isomon_hash_count;
  while (it < end) {
    *end = *(end-1);
    --end;
  }
  *it = (hash << 16) + isonum;
  hash2code(hash, g_isonum_to_code[isonum]);
  g_isonum_to_hash[isonum] = hash;
  return true;
}

/// Set number of minors per major unit for currency.
/** Not thread-safe. Typically this would only be called during program
    initialization.
    At most 16 distinct numbers of minors can be used across all currencies.
    If a currency is set to use 17th possible number of minors, this function
    will return false and fail.
    @param isonum An ISO numeric code, real or fake, in the range
    [0, ISOMON_ISONUM_COUNT). ISOMON_ISONUM_COUNT is probably 1024.
    @param num_minors Number of minor units per major unit. Use 1 if the major
    unit is the smallest unit of money (i.e major = minor). Use 0 if there is
    no smallest unit or it is not applicable.
*/
inline
bool set_num_minors(int16_t isonum, int16_t num_minors, int8_t num_digits)
{
  if (isonum < 0 || isonum >= ISOMON_ISONUM_COUNT) return false; // bad isonum
  size_t scale = ISOMON_MINOR_SCALE_COUNT - 1; // scale for num_minors == 0
  if (num_minors != 0) {
    for (scale = 0; scale < ISOMON_MINOR_SCALE_COUNT - 1; ++scale) {
      if (g_minor_scale_to_num_minors[scale] == num_minors) break;
      if (g_minor_scale_to_num_minors[scale] == 0) {
        g_minor_scale_to_num_minors[scale] = num_minors;
        break;
      }
      if (scale == ISOMON_MINOR_SCALE_COUNT-1) return false; // too many scales
    }
  }
  g_minor_scale_to_num_digits[scale] = num_digits;
  int16_t bits_to_write = 0xF << (4 * (isonum % 2));
  g_isonum_to_minor_scale[isonum / 2] &= ~bits_to_write;
  g_isonum_to_minor_scale[isonum / 2] |= scale << (4 * (isonum % 2));
  return true;
}

#ifdef __cplusplus
} // namespace isomon::data
} // namespace isomon
#endif

#endif // ISOMON_CURRENCY_DATA_H

