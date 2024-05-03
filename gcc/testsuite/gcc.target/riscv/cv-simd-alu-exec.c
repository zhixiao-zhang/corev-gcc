/* { dg-additional-sources cv-helpers.c } */
/* { dg-do run } */
/* { dg-require-effective-target cv_simd } */
/* We need -save-temps with "dg-do run", in order to save the generated file
   We use "dg-final" with "cleanup-saved-temps" at the end to clean up. */
/* { dg-options "-march=rv32i_xcvsimd -mabi=ilp32 -save-temps" } */

#include "cv-simd-helpers.h"

/* Helper functions */

/* Note that the definition of the avg operator is an arithmetic shift right,
   so the result rounds down, not towards zero (which is what divided by 2
   would do). */
static int16_t
avg_hw (int16_t a, int16_t b)
{
  return ((int16_t) (a + b)) >> 1;
}

static int8_t
avg_b (int8_t a, int8_t b)
{
  return ((int8_t) (a + b)) >> 1;
}

static uint16_t
avg_uhw (uint16_t a, uint16_t b)
{
  return ((uint16_t) (a + b)) >> 1;
}

static uint8_t
avg_ub (uint8_t a, uint8_t b)
{
  return ((uint8_t) (a + b)) >> 1;
}

static uint32_t
abs_hw (uint32_t v)
{
  uint32_t res = 0;
  for (int idx = 0; idx < 2; idx++)
    {
      int16_t el = abs ((int16_t) ((v >> (idx * 16)) & 0xffff));
      res |= ((uint32_t) el) << (idx * 16);
    }
  return res;
}

static uint32_t
abs_b (uint32_t v)
{
  uint32_t res = 0;
  for (int idx = 0; idx < 4; idx++)
    {
      int8_t el = abs ((int8_t) ((v >> (idx * 8)) & 0xff));
      res |= ((uint32_t) el) << (idx * 8);
    }
  return res;
}

/* This run-time test does exemplary tests of each of the SIMD ALU
   instructions, apart from cv.add, which is the subject of a more exhaustive
   test.

   GCC regression testing is not an exhaustive verification of the instruction
   implementation, so only a few representative builtin invocations are used
   for all execept cv.add.  We generally offer the following variants for each
   operand.

   - vector half word, arguments in variables
   - vector half word, second argument constant
   - scalar half word, arguments in variables
   - scalar half word, second argument "small" constant
   - scalar half word, second argument "large" constant
   - vector byte, arguments in variables
   - vector byte, second argument constant
   - scalar byte, arguments in variables
   - scalar byte, second argument "small" constant
   - scalar byte, second argument "large" constant

   By "small" constant, we mean a value small enough to fit in a 6-byte
   immediate (signed or unsigned as appropriate).  By "large" we mean "not
   small".
*/
int
main ()
{
  /* SIMD add variants.  These tests are more exhaustive, to provide a
     shakedown of the SIMD world. */

#define OP(v1, v2, c) (MK_HV (HELEM(v1, 1) + HELEM(v2, 1), \
			      HELEM(v1, 0) + HELEM(v2, 0)))
#define BUILTIN __builtin_riscv_cv_simd_add_h
  TEST_HVV_HVV_WC (  0,      0,      0,      0, 0);
  TEST_HVV_HVV_WC (  0,      0,      1,      1, 0);
  TEST_HVV_HVV_WC (  0,      0,     -1,     -1, 0);
  TEST_HVV_HVV_WC (  1,      1,     -1,     -1, 0);
  TEST_HVV_HVV_WC ( 42,    561,  19931,  19937, 0);
  TEST_HVV_HVV_WC ( 42,  19927,    561,  19391, 0);
  TEST_HVV_HVV_WC (-42,   -561, -19391, -19937, 0);
  TEST_HVV_HVV_WC (-42, -19937,   -561, -19391, 0);

  TEST_HVV_HVC_WC (0, 0,  0,  0, 0);
  TEST_HVV_HVC_WC (0, 0,  1,  1, 0);
  TEST_HVV_HVC_WC (0, 0, -1, -1, 0);
  TEST_HVV_HVC_WC (1, 1, -1, -1, 0);
#undef BUILTIN
#undef OP
#define OP(v1, s1) (MK_HV (HELEM(v1, 1) + s1, HELEM(v1, 0) + s1))
#define BUILTIN __builtin_riscv_cv_simd_add_sc_h
  TEST_HVV_HSV (     0,     0,      0);
  TEST_HVV_HSV (     0,     0,      1);
  TEST_HVV_HSV (     0,     0,     -1);
  TEST_HVV_HSV ( 32719, 32749,     28);
  TEST_HVV_HSV (-32719, -32749,   -32);
  TEST_HVV_HSV (    42,   -561,   561);
  TEST_HVV_HSV (   561,  19391, -1729);

  TEST_HVV_HSC (     0,      0,     0);
  TEST_HVV_HSC (     0,      0,     1);
  TEST_HVV_HSC (     0,      0,    -1);
  TEST_HVV_HSC ( 32719,  32749,    28);
  TEST_HVV_HSC (-32719, -32749,   -32);
  TEST_HVV_HSC (   -42,   -561,   561);
  TEST_HVV_HSC (   561,  19391, -1729);
#undef BUILTIN
#undef OP
#define OP(v1, v2) (MK_BV (BELEM(v1, 3) + BELEM(v2, 3), \
			   BELEM(v1, 2) + BELEM(v2, 2), \
			   BELEM(v1, 1) + BELEM(v2, 1), \
			   BELEM(v1, 0) + BELEM(v2, 0)))
#define BUILTIN __builtin_riscv_cv_simd_add_b
  TEST_BVV_BVV (  0,   0,   0,   0,    0,    0,    0,    0);
  TEST_BVV_BVV (  0,   0,   0,   0,    1,    1,    1,    1);
  TEST_BVV_BVV (  0,   0,   0,   0,   -1,   -1,   -1,   -1);
  TEST_BVV_BVV (  1,   1,   1,   1,   -1,   -1,   -1,   -1);
  TEST_BVV_BVV ( 17,  32,  36,  72,   42,   28,   27,   25);
  TEST_BVV_BVV ( 42,  28,  27,  25,  107,  109,  113,  127);
  TEST_BVV_BVV (107, 109, 113, 127,  -17,  -32,  -36,  -72);
  TEST_BVV_BVV (-42, -28, -27, -25, -107, -109, -113, -127);

  TEST_BVV_BVC (0, 0, 0, 0,  0,  0,  0,  0);
  TEST_BVV_BVC (0, 0, 0, 0,  1,  1,  1,  1);
  TEST_BVV_BVC (0, 0, 0, 0, -1, -1, -1, -1);
  TEST_BVV_BVC (1, 1, 1, 1, -1, -1, -1, -1);
#undef BUILTIN
#undef OP
#define OP(v1, s1) (MK_BV (BELEM(v1, 3) + s1, BELEM(v1, 2) + s1, \
			   BELEM(v1, 1) + s1, BELEM(v1, 0) + s1))
#define BUILTIN __builtin_riscv_cv_simd_add_sc_b
  TEST_BVV_BSV (   0,    0,    0,    0,    0);
  TEST_BVV_BSV (   0,    0,    0,    0,    1);
  TEST_BVV_BSV (   0,    0,    0,    0,   -1);
  TEST_BVV_BSV ( 107,  109,  113,  127,   28);
  TEST_BVV_BSV (-107, -109, -113, -127,  -32);
  TEST_BVV_BSV (  42,   28,   27,   25,   42);
  TEST_BVV_BSV ( -17,  -32,  -36,  -72, -107);

  TEST_BVV_BSC (   0,    0,    0,    0,    0);
  TEST_BVV_BSC (   0,    0,    0,    0,    1);
  TEST_BVV_BSC (   0,    0,    0,    0,   -1);
  TEST_BVV_BSC ( 107,  109,  113,  127,   28);
  TEST_BVV_BSC (-107, -109, -113, -127,  -32);
  TEST_BVV_BSC (  42,   28,   27,   25,   42);
  TEST_BVV_BSC ( -17,  -32,  -36,  -72, -107);
#undef OP
#undef BUILTIN

  /* SIMD sub variants. Note that the half word vector-vector operations have
     a shift argument */
#define BUILTIN __builtin_riscv_cv_simd_sub_h
#define OP(v1, v2, c) (MK_HV (HELEM (v1, 1) - HELEM (v2, 1), \
			      HELEM (v1, 0) - HELEM (v2, 0)))
  TEST_HVV_HVV_WC (42, -19937, -19391,   561, 0);
  TEST_HVV_HVC_WC (42,  19937,    561, 19391, 0);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_sub_sc_h
#define OP(v1, s1) (MK_HV (HELEM (v1, 1) - s1, HELEM (v1, 0) - s1))
  TEST_HVV_HSV  (561,  19391,      1);
  TEST_HVV_HSC (561,  19391,     -1);
  TEST_HVV_HSC (561,  19391,    561);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_sub_b
#define OP(v1, v2) (MK_BV (BELEM (v1, 3) - BELEM (v2, 3), \
			   BELEM (v1, 2) - BELEM (v2, 2), \
			   BELEM (v1, 1) - BELEM (v2, 1), \
			   BELEM (v1, 0) - BELEM (v2, 0)))
  TEST_BVV_BVV  ( 17,  32,  36,  72,  42,  28, 27,  25);
  TEST_BVV_BVC (107, 109, 113, 127,  17, -32, 27, -72);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_sub_sc_b
#define OP(v1, s1) (MK_BV (BELEM (v1, 3) - s1, BELEM (v1, 2) - s1, \
			   BELEM (v1, 1) - s1, BELEM (v1, 0) - s1))
  TEST_BVV_BSV  (-17, -32, -36, -72, 28);
  TEST_BVV_BSC (-17, -32, -36, -72,  1);
  TEST_BVV_BSC (-17, -32, -36, -72, 42);
#undef OP
#undef BUILTIN

  /* SIMD avg variants */
#define OP(v1, v2) (MK_HV (avg_hw (HELEM (v1, 1), HELEM (v2, 1)),	\
			   avg_hw (HELEM (v1, 0), HELEM (v2, 0))))
#define BUILTIN __builtin_riscv_cv_simd_avg_h
  TEST_HVV_HVV  (42, -19937, -19391,   561);
  TEST_HVV_HVC (42,  19937,    561, 19391);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_avg_sc_h
#define OP(v1, s1) (MK_HV (avg_hw (HELEM (v1, 1), s1), \
			   avg_hw (HELEM (v1, 0), s1)))
  TEST_HVV_HSV  (561,  19391,      1);
  TEST_HVV_HSC (561,  19391,     -1);
  TEST_HVV_HSC (561,  19391,    561);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_avg_b
#define OP(v1, v2) (MK_BV (avg_b (BELEM (v1, 3), BELEM (v2, 3)),  \
			   avg_b (BELEM (v1, 2), BELEM (v2, 2)),  \
			   avg_b (BELEM (v1, 1), BELEM (v2, 1)),  \
			   avg_b (BELEM (v1, 0), BELEM (v2, 0))))
  TEST_BVV_BVV  ( 17, 32, 36, 72, 42, 28, 27, 25);
  TEST_BVV_BVC (107, 109, 113, 127, 17, -32, 27, -72);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_avg_sc_b
#define OP(v1, s1) (MK_BV (avg_b (BELEM (v1, 3), s1), \
			   avg_b (BELEM (v1, 2), s1), \
			   avg_b (BELEM (v1, 1), s1), \
			   avg_b (BELEM (v1, 0), s1)))
  TEST_BVV_BSV  (-17, -32, -36, -72, 28);
  TEST_BVV_BSC (-17, -32, -36, -72,  1);
  TEST_BVV_BSC (-17, -32, -36, -72, 42);
#undef OP
#undef BUILTIN

  /* SIMD avgu variants */
#define OP(v1, v2) (MK_HV (avg_uhw (UHELEM (v1, 1), UHELEM (v2, 1)),	\
			   avg_uhw (UHELEM (v1, 0), UHELEM (v2, 0))))
#define BUILTIN __builtin_riscv_cv_simd_avgu_h
  TEST_HVV_HVV (65521U,   561U, 42U, 65519U);
  TEST_HVV_HVC (  561U, 65519U, 42U, 65521U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_avgu_sc_h
#define OP(v1, s1) (MK_HV (avg_uhw (UHELEM (v1, 1), s1), \
			   avg_uhw (UHELEM (v1, 0), s1)))
  TEST_HVV_UHSV (42U, 65521U,   1U);
  TEST_HVV_HSC  (42U, 65521U,   3U);
  TEST_HVV_HSC  (42U, 65521U, 561U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_avgu_b
#define OP(v1, v2) (MK_BV (avg_ub (UBELEM (v1, 3), UBELEM (v2, 3)),  \
			   avg_ub (UBELEM (v1, 2), UBELEM (v2, 2)),  \
			   avg_ub (UBELEM (v1, 1), UBELEM (v2, 1)),  \
			   avg_ub (UBELEM (v1, 0), UBELEM (v2, 0))))
  TEST_BVV_BVV ( 17U,  32U,  36U,  72U, 42U,  28U, 27U,  25U);
  TEST_BVV_BVC (233U, 239U, 241U, 253U, 17U, 239U, 36U, 253U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_avgu_sc_b
#define OP(v1, s1) (MK_BV (avg_ub (UBELEM (v1, 3), s1),  \
			   avg_ub (UBELEM (v1, 2), s1),  \
			   avg_ub (UBELEM (v1, 1), s1),  \
			   avg_ub (UBELEM (v1, 0), s1)))
  TEST_BVV_UBSV (72U, 36U, 32U, 17U, 28U);
  TEST_BVV_BSC (72U, 36U, 32U, 17U,  3U);
  TEST_BVV_BSC (72U, 36U, 32U, 17U, 72U);
#undef OP
#undef BUILTIN

  /* SIMD min variants */
#define OP(v1, v2) (MK_HV ((HELEM (v1, 1) < HELEM (v2, 1)) ? \
			    HELEM (v1, 1) : HELEM (v2, 1),   \
			   (HELEM (v1, 0) < HELEM (v2, 0)) ? \
			    HELEM (v1, 0) : HELEM (v2, 0)))
#define BUILTIN __builtin_riscv_cv_simd_min_h
  TEST_HVV_HVV (42, -19937, -19391,   561);
  TEST_HVV_HVC (42,  19937,    561, 19391);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_min_sc_h
#define OP(v1, s1) (MK_HV ((HELEM (v1, 1) < s1) ? HELEM (v1, 1) : s1,	\
			   (HELEM (v1, 0) < s1) ? HELEM (v1, 0) : s1))
  TEST_HVV_HSV (561,  19391,      1);
  TEST_HVV_HSC (561,  19391,     -1);
  TEST_HVV_HSC (561,  19391,    561);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_min_b
#define OP(v1, v2) (MK_BV ((BELEM (v1, 3) < BELEM (v2, 3)) ? \
			   BELEM (v1, 3) : BELEM (v2, 3),    \
			   (BELEM (v1, 2) < BELEM (v2, 2)) ? \
			   BELEM (v1, 2) : BELEM (v2, 2),    \
			   (BELEM (v1, 1) < BELEM (v2, 1)) ? \
			   BELEM (v1, 1) : BELEM (v2, 1),    \
			   (BELEM (v1, 0) < BELEM (v2, 0)) ? \
			   BELEM (v1, 0) : BELEM (v2, 0)))
  TEST_BVV_BVV  ( 17, 32, 36, 72, 42, 28, 27, 25);
  TEST_BVV_BVC (107, 109, 113, 127, 17, -32, 27, -72);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_min_sc_b
#define OP(v1, s1) (MK_BV ((BELEM (v1, 3) < s1) ? BELEM (v1, 3) : s1,	\
			   (BELEM (v1, 2) < s1) ? BELEM (v1, 2) : s1,	\
			   (BELEM (v1, 1) < s1) ? BELEM (v1, 1) : s1,	\
			   (BELEM (v1, 0) < s1) ? BELEM (v1, 0) : s1 ))
  TEST_BVV_BSV  (-17, -32, -36, -72, 28);
  TEST_BVV_BSC (-17, -32, -36, -72,  1);
  TEST_BVV_BSC (-17, -32, -36, -72, 42);
#undef OP
#undef BUILTIN

  /* SIMD minu variants */
#define BUILTIN __builtin_riscv_cv_simd_minu_h
#define OP(v1, v2) (MK_HV ((UHELEM (v1, 1) < UHELEM (v2, 1)) ?	\
			   UHELEM (v1, 1) : UHELEM (v2, 1),    \
			   (UHELEM (v1, 0) < UHELEM (v2, 0)) ? \
			   UHELEM (v1, 0) : UHELEM (v2, 0)))
  TEST_HVV_HVV (65521U,   561U, 42U, 65519U);
  TEST_HVV_HVC (  561U, 65519U, 42U, 65521U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_minu_sc_h
#define OP(v1, s1) (MK_HV ((UHELEM (v1, 1) < s1) ? UHELEM (v1, 1) : s1, \
			   (UHELEM (v1, 0) < s1) ? UHELEM (v1, 0) : s1))
  TEST_HVV_UHSV (42U, 65521U,   1U);
  TEST_HVV_HSC (42U, 65521U,   3U);
  TEST_HVV_HSC (42U, 65521U, 561U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_minu_b
#define OP(v1, v2) (MK_BV ((UBELEM (v1, 3) < UBELEM (v2, 3)) ?	\
			   UBELEM (v1, 3) : UBELEM (v2, 3),    \
			   (UBELEM (v1, 2) < UBELEM (v2, 2)) ? \
			   UBELEM (v1, 2) : UBELEM (v2, 2),    \
			   (UBELEM (v1, 1) < UBELEM (v2, 1)) ? \
			   UBELEM (v1, 1) : UBELEM (v2, 1),    \
			   (UBELEM (v1, 0) < UBELEM (v2, 0)) ? \
			   UBELEM (v1, 0) : UBELEM (v2, 0)))
  TEST_BVV_BVV ( 17U,  32U,  36U,  72U, 42U,  28U, 27U,  25U);
  TEST_BVV_BVC (233U, 239U, 241U, 253U, 17U, 239U, 36U, 253U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_minu_sc_b
#define OP(v1, s1) (MK_BV ((UBELEM (v1, 3) < s1) ? UBELEM (v1, 3) : s1, \
                           (UBELEM (v1, 2) < s1) ? UBELEM (v1, 2) : s1, \
                           (UBELEM (v1, 1) < s1) ? UBELEM (v1, 1) : s1, \
                           (UBELEM (v1, 0) < s1) ? UBELEM (v1, 0) : s1 ))
  TEST_BVV_UBSV (72U, 36U, 32U, 17U, 28U);
  TEST_BVV_BSC (72U, 36U, 32U, 17U,  3U);
  TEST_BVV_BSC (72U, 36U, 32U, 17U, 72U);
#undef OP
#undef BUILTIN

  /* SIMD max variants */
#define BUILTIN __builtin_riscv_cv_simd_max_h
#define OP(v1, v2) (MK_HV ((HELEM (v1, 1) > HELEM (v2, 1)) ? \
			   HELEM (v1, 1) : HELEM (v2, 1),    \
			   (HELEM (v1, 0) > HELEM (v2, 0)) ? \
			   HELEM (v1, 0) : HELEM (v2, 0)))
  TEST_HVV_HVV  (42, -19937, -19391,   561);
  TEST_HVV_HVC (42,  19937,    561, 19391);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_max_sc_h
#define OP(v1, s1) (MK_HV ((HELEM (v1, 1) > s1) ? HELEM (v1, 1) : s1,	\
			   (HELEM (v1, 0) > s1) ? HELEM (v1, 0) : s1))
  TEST_HVV_HSV  (561,  19391,      1);
  TEST_HVV_HSC (561,  19391,     -1);
  TEST_HVV_HSC (561,  19391,    561);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_max_b
#define OP(v1, v2) (MK_BV ((BELEM (v1, 3) > BELEM (v2, 3)) ?	\
			   BELEM (v1, 3) : BELEM (v2, 3),  \
			   (BELEM (v1, 2) > BELEM (v2, 2)) ?	\
			   BELEM (v1, 2) : BELEM (v2, 2),  \
			   (BELEM (v1, 1) > BELEM (v2, 1)) ?	\
			   BELEM (v1, 1) : BELEM (v2, 1),  \
			   (BELEM (v1, 0) > BELEM (v2, 0)) ?	\
			   BELEM (v1, 0) : BELEM (v2, 0)))
  TEST_BVV_BVV  ( 17, 32, 36, 72, 42, 28, 27, 25);
  TEST_BVV_BVC (107, 109, 113, 127, 17, -32, 27, -72);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_max_sc_b
#define OP(v1, s1) (MK_BV ((BELEM (v1, 3) > s1) ? BELEM (v1, 3) : s1,	\
                           (BELEM (v1, 2) > s1) ? BELEM (v1, 2) : s1,	\
                           (BELEM (v1, 1) > s1) ? BELEM (v1, 1) : s1,	\
                           (BELEM (v1, 0) > s1) ? BELEM (v1, 0) : s1 ))
  TEST_BVV_BSV  (-17, -32, -36, -72, 28);
  TEST_BVV_BSC (-17, -32, -36, -72,  1);
  TEST_BVV_BSC (-17, -32, -36, -72, 42);
#undef OP
#undef BUILTIN

  /* SIMD maxu variants */
#define OP(v1, v2) (MK_HV ((UHELEM (v1, 1) > UHELEM (v2, 1)) ? \
			   UHELEM (v1, 1) : UHELEM (v2, 1),    \
			   (UHELEM (v1, 0) > UHELEM (v2, 0)) ? \
			   UHELEM (v1, 0) : UHELEM (v2, 0)))
#define BUILTIN __builtin_riscv_cv_simd_maxu_h
  TEST_HVV_HVV (65521U,   561U, 42U, 65519U);
  TEST_HVV_HVC (  561U, 65519U, 42U, 65521U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_maxu_sc_h
#define OP(v1, s1) (MK_HV ((UHELEM (v1, 1) > s1) ? UHELEM (v1, 1) : s1, \
			   (UHELEM (v1, 0) > s1) ? UHELEM (v1, 0) : s1))
  TEST_HVV_UHSV (42U, 65521U,   1U);
  TEST_HVV_HSC (42U, 65521U,   3U);
  TEST_HVV_HSC (42U, 65521U, 561U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_maxu_b
#define OP(v1, v2) (MK_BV ((UBELEM (v1, 3) > UBELEM (v2, 3)) ? \
			   UBELEM (v1, 3) : UBELEM (v2, 3),    \
			   (UBELEM (v1, 2) > UBELEM (v2, 2)) ? \
			   UBELEM (v1, 2) : UBELEM (v2, 2),    \
			   (UBELEM (v1, 1) > UBELEM (v2, 1)) ? \
			   UBELEM (v1, 1) : UBELEM (v2, 1),    \
			   (UBELEM (v1, 0) > UBELEM (v2, 0)) ? \
			   UBELEM (v1, 0) : UBELEM (v2, 0)))
  TEST_BVV_BVV ( 17U,  32U,  36U,  72U, 42U,  28U, 27U,  25U);
  TEST_BVV_BVC (233U, 239U, 241U, 253U, 17U, 239U, 36U, 253U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_maxu_sc_b
#define OP(v1, s1) (MK_BV ((UBELEM (v1, 3) > s1) ? UBELEM (v1, 3) : s1, \
                           (UBELEM (v1, 2) > s1) ? UBELEM (v1, 2) : s1, \
                           (UBELEM (v1, 1) > s1) ? UBELEM (v1, 1) : s1, \
                           (UBELEM (v1, 0) > s1) ? UBELEM (v1, 0) : s1 ))
  TEST_BVV_UBSV (72U, 36U, 32U, 17U, 28U);
  TEST_BVV_BSC (72U, 36U, 32U, 17U,  3U);
  TEST_BVV_BSC (72U, 36U, 32U, 17U, 72U);
#undef OP
#undef BUILTIN

  /* SIMD srl variants. */
#define OP(v1, v2) (MK_HV (UHELEM (v1, 1) >> (UHELEM (v2, 1) & 0xf), \
		           UHELEM (v1, 0) >> (UHELEM (v2, 0) & 0xf)))
#define BUILTIN __builtin_riscv_cv_simd_srl_h
  TEST_HVV_HVV  (42, -19937, 15U,   0U);
  TEST_HVV_HVC (42,  19937, 32U,   1U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_srl_sc_h
#define OP(v1, s1) (MK_HV (UHELEM (v1, 1) >> (s1 & 0xf), \
			   UHELEM (v1, 0) >> (s1 & 0xf)))
  TEST_HVV_HSV  (561,  19391,      1U);
  TEST_HVV_HSC (561,  19391,      7U);
  TEST_HVV_HSC (561,  19391,     64U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_srl_b
#define OP(v1, v2) (MK_BV (UBELEM (v1, 3) >> (UBELEM (v2, 3) & 0xf), \
                           UBELEM (v1, 2) >> (UBELEM (v2, 2) & 0xf), \
                           UBELEM (v1, 1) >> (UBELEM (v2, 1) & 0xf), \
			   UBELEM (v1, 0) >> (UBELEM (v2, 0) & 0xf)))
  TEST_BVV_BVV  ( 17,  32,  36,  72,  0U, 1U, 4U,  7U);
  TEST_BVV_BVC (107, 109, 113, 127,  0U, 1U, 7U, 16U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_srl_sc_b
#define OP(v1, s1) (MK_BV (UBELEM (v1, 3) >> (s1 & 0xf), \
                           UBELEM (v1, 2) >> (s1 & 0xf), \
                           UBELEM (v1, 1) >> (s1 & 0xf), \
			   UBELEM (v1, 0) >> (s1 & 0xf)))
  TEST_BVV_BSV  (-17, -32, -36, -72,  1U);
  TEST_BVV_BSC (-17, -32, -36, -72,  7U);
  TEST_BVV_BSC (-17, -32, -36, -72, 64U);
#undef OP
#undef BUILTIN

  /* SIMD sra variants. */
#define OP(v1, v2) (MK_HV (HELEM (v1, 1) >> (HELEM (v2, 1) & 0xf), \
		           HELEM (v1, 0) >> (HELEM (v2, 0) & 0xf)))
#define BUILTIN __builtin_riscv_cv_simd_sra_h
  TEST_HVV_HVV  (42, -19937, 15U,   0U);
  TEST_HVV_HVC (42,  19937, 32U,   1U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_sra_sc_h
#define OP(v1, s1) (MK_HV (HELEM (v1, 1) >> (s1 & 0xf), \
			   HELEM (v1, 0) >> (s1 & 0xf)))
  TEST_HVV_HSV  (561,  19391,      1U);
  TEST_HVV_HSC (561,  19391,      7U);
  TEST_HVV_HSC (561,  19391,     64U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_sra_b
#define OP(v1, v2) (MK_BV (BELEM (v1, 3) >> (BELEM (v2, 3) & 0xf), \
                           BELEM (v1, 2) >> (BELEM (v2, 2) & 0xf), \
                           BELEM (v1, 1) >> (BELEM (v2, 1) & 0xf), \
			   BELEM (v1, 0) >> (BELEM (v2, 0) & 0xf)))
  TEST_BVV_BVV  ( 17,  32,  36,  72,  0U, 1U, 4U,  7U);
  TEST_BVV_BVC (107, 109, 113, 127,  0U, 1U, 7U, 16U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_sra_sc_b
#define OP(v1, s1) (MK_BV (BELEM (v1, 3) >> (s1 & 0xf), \
                           BELEM (v1, 2) >> (s1 & 0xf), \
                           BELEM (v1, 1) >> (s1 & 0xf), \
			   BELEM (v1, 0) >> (s1 & 0xf)))
  TEST_BVV_BSV  (-17, -32, -36, -72,  1U);
  TEST_BVV_BSC (-17, -32, -36, -72,  7U);
  TEST_BVV_BSC (-17, -32, -36, -72, 64U);
#undef OP
#undef BUILTIN

  /* SIMD sll variants. */
#define OP(v1, v2) (MK_HV (UHELEM (v1, 1) << (UHELEM (v2, 1) & 0xf), \
		           UHELEM (v1, 0) << (UHELEM (v2, 0) & 0xf)))
#define BUILTIN __builtin_riscv_cv_simd_sll_h
  TEST_HVV_HVV (42, -19937, 15U,   0U);
  TEST_HVV_HVC (42,  19937, 32U,   1U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_sll_sc_h
#define OP(v1, s1) (MK_HV (UHELEM (v1, 1) << (s1 & 0xf), \
			   UHELEM (v1, 0) << (s1 & 0xf)))
  TEST_HVV_HSV  (561,  19391,      1U);
  TEST_HVV_HSC (561,  19391,      7U);
  TEST_HVV_HSC (561,  19391,     64U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_sll_b
#define OP(v1, v2) (MK_BV (UBELEM (v1, 3) << (UBELEM (v2, 3) & 0xf), \
                           UBELEM (v1, 2) << (UBELEM (v2, 2) & 0xf), \
                           UBELEM (v1, 1) << (UBELEM (v2, 1) & 0xf), \
			   UBELEM (v1, 0) << (UBELEM (v2, 0) & 0xf)))
  TEST_BVV_BVV  ( 17,  32,  36,  72,  0U, 1U, 4U,  7U);
  TEST_BVV_BVC (107, 109, 113, 127,  0U, 1U, 7U, 16U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_sll_sc_b
#define OP(v1, s1) (MK_BV (UBELEM (v1, 3) << (s1 & 0xf), \
                           UBELEM (v1, 2) << (s1 & 0xf), \
                           UBELEM (v1, 1) << (s1 & 0xf), \
			   UBELEM (v1, 0) << (s1 & 0xf)))
  TEST_BVV_BSV  (-17, -32, -36, -72,  1U);
  TEST_BVV_BSC (-17, -32, -36, -72,  7U);
  TEST_BVV_BSC (-17, -32, -36, -72, 64U);
#undef OP
#undef BUILTIN

  /* SIMD or variants */
#define OP(v1, v2) (MK_HV (UHELEM (v1, 1) | UHELEM (v2, 1), \
			   UHELEM (v1, 0) | UHELEM (v2, 0)))
#define BUILTIN __builtin_riscv_cv_simd_or_h
  TEST_HVV_HVV (0xfff1, 0x0231, 0x002a, 0xffef);
  TEST_HVV_HVC (0x0231, 0xffef, 0x002a, 0xfff1);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_or_sc_h
#define OP(v1, s1) (MK_HV (UHELEM (v1, 1) | s1, UHELEM (v1, 0) | s1))
  TEST_HVV_HSV (0x002a, 0xfff1, 0x0001);
  TEST_HVV_HSC (0x002a, 0xfff1, 0x0003);
  TEST_HVV_HSC (0x002a, 0xfff1, 0x0231);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_or_b
#define OP(v1, v2) (MK_BV (UBELEM (v1, 3) | UBELEM (v2, 3), \
			   UBELEM (v1, 2) | UBELEM (v2, 2), \
			   UBELEM (v1, 1) | UBELEM (v2, 1), \
			   UBELEM (v1, 0) | UBELEM (v2, 0)))
  TEST_BVV_BVV (0x11, 0x20, 0x24, 0x48, 0x2a, 0x1c, 0x1b, 0x19);
  TEST_BVV_BVC (0xe9, 0xef, 0xf1, 0xfd, 0x11, 0xef, 0x24, 0xfd);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_or_sc_b
#define OP(v1, s1) (MK_BV (UBELEM (v1, 3) | s1, UBELEM (v1, 2) | s1, \
			   UBELEM (v1, 1) | s1, UBELEM (v1, 0) | s1))
  TEST_BVV_BSV (0x48, 0x24, 0x20, 0x11, 0x1c);
  TEST_BVV_BSC (0x48, 0x24, 0x20, 0x11, 0x03);
  TEST_BVV_BSC (0x48, 0x24, 0x20, 0x11, 0x48);
#undef OP
#undef BUILTIN

  /* SIMD xor variants */
#define OP(v1, v2) (MK_HV (UHELEM (v1, 1) ^ UHELEM (v2, 1), \
			   UHELEM (v1, 0) ^ UHELEM (v2, 0)))
#define BUILTIN __builtin_riscv_cv_simd_xor_h
  TEST_HVV_HVV (0xfff1, 0x0231, 0x002a, 0xffef);
  TEST_HVV_HVC (0x0231, 0xffef, 0x002a, 0xfff1);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_xor_sc_h
#define OP(v1, s1) (MK_HV (UHELEM (v1, 1) ^ s1, UHELEM (v1, 0) ^ s1))
  TEST_HVV_HSV (0x002a, 0xfff1, 0x0001);
  TEST_HVV_HSC (0x002a, 0xfff1, 0x0003);
  TEST_HVV_HSC (0x002a, 0xfff1, 0x0231);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_xor_b
#define OP(v1, v2) (MK_BV (UBELEM (v1, 3) ^ UBELEM (v2, 3), \
			   UBELEM (v1, 2) ^ UBELEM (v2, 2), \
			   UBELEM (v1, 1) ^ UBELEM (v2, 1), \
			   UBELEM (v1, 0) ^ UBELEM (v2, 0)))
  TEST_BVV_BVV (0x11, 0x20, 0x24, 0x48, 0x2a, 0x1c, 0x1b, 0x19);
  TEST_BVV_BVC (0xe9, 0xef, 0xf1, 0xfd, 0x11, 0xef, 0x24, 0xfd);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_xor_sc_b
#define OP(v1, s1) (MK_BV (UBELEM (v1, 3) ^ s1, UBELEM (v1, 2) ^ s1, \
			   UBELEM (v1, 1) ^ s1, UBELEM (v1, 0) ^ s1))
  TEST_BVV_BSV (0x48, 0x24, 0x20, 0x11, 0x1c);
  TEST_BVV_BSC (0x48, 0x24, 0x20, 0x11, 0x03);
  TEST_BVV_BSC (0x48, 0x24, 0x20, 0x11, 0x48);
#undef OP
#undef BUILTIN

  /* SIMD and variants */
#define OP(v1, v2) (MK_HV (UHELEM (v1, 1) & UHELEM (v2, 1), \
			   UHELEM (v1, 0) & UHELEM (v2, 0)))
#define BUILTIN __builtin_riscv_cv_simd_and_h
  TEST_HVV_HVV (0xfff1, 0x0231, 0x002a, 0xffef);
  TEST_HVV_HVC (0x0231, 0xffef, 0x002a, 0xfff1);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_and_sc_h
#define OP(v1, s1) (MK_HV (UHELEM (v1, 1) & s1, UHELEM (v1, 0) & s1))
  TEST_HVV_HSV (0x002a, 0xfff1, 0x0001);
  TEST_HVV_HSC (0x002a, 0xfff1, 0x0003);
  TEST_HVV_HSC (0x002a, 0xfff1, 0x0231);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_and_b
#define OP(v1, v2) (MK_BV (UBELEM (v1, 3) & UBELEM (v2, 3), \
			   UBELEM (v1, 2) & UBELEM (v2, 2), \
			   UBELEM (v1, 1) & UBELEM (v2, 1), \
			   UBELEM (v1, 0) & UBELEM (v2, 0)))
  TEST_BVV_BVV (0x11, 0x20, 0x24, 0x48, 0x2a, 0x1c, 0x1b, 0x19);
  TEST_BVV_BVC (0xe9, 0xef, 0xf1, 0xfd, 0x11, 0xef, 0x24, 0xfd);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_and_sc_b
#define OP(v1, s1) (MK_BV (UBELEM (v1, 3) & s1, UBELEM (v1, 2) & s1, \
			   UBELEM (v1, 1) & s1, UBELEM (v1, 0) & s1))
  TEST_BVV_BSV (0x48, 0x24, 0x20, 0x11, 0x1c);
  TEST_BVV_BSC (0x48, 0x24, 0x20, 0x11, 0x03);
  TEST_BVV_BSC (0x48, 0x24, 0x20, 0x11, 0x48);
#undef OP
#undef BUILTIN

  /* SIMD abs variants */
#define BUILTIN __builtin_riscv_cv_simd_abs_h
#define OP(v1) (abs_hw (v1))
  TEST_HVV (0xfff1, 0x0231);
  TEST_HVC (0x0231, 0xffef);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_abs_b
#define OP(v1) (abs_b (v1))
  TEST_BVV (0x11, 0x20, 0x24, 0x48);
  TEST_BVC (0xe9, 0xef, 0xf1, 0xfd);
#undef BUILTIN
#undef OP

  return 0;
}

/* { dg-final { scan-assembler-times "cv\\.add\\.h" 12 } } */
/* { dg-final { scan-assembler-times "cv\\.add\\.sc\\.h" 9 } } */
/* { dg-final { scan-assembler-times "cv\\.add\\.sci\\.h" 5 } } */
/* { dg-final { scan-assembler-times "cv\\.add\\.b" 12 } } */
/* { dg-final { scan-assembler-times "cv\\.add\\.sc\\.b" 9 } } */
/* { dg-final { scan-assembler-times "cv\\.add\\.sci\\.b" 5 } } */

/* { dg-final { scan-assembler-times "cv\\.sub\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.sub\\.sc\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.sub\\.sci\\.h" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.sub\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.sub\\.sc\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.sub\\.sci\\.b" 1 } } */

/* { dg-final { scan-assembler-times "cv\\.avg\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.avg\\.sc\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.avg\\.sci\\.h" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.avg\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.avg\\.sc\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.avg\\.sci\\.b" 1 } } */

/* { dg-final { scan-assembler-times "cv\\.avgu\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.avgu\\.sc\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.avgu\\.sci\\.h" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.avgu\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.avgu\\.sc\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.avgu\\.sci\\.b" 1 } } */

/* { dg-final { scan-assembler-times "cv\\.min\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.min\\.sc\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.min\\.sci\\.h" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.min\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.min\\.sc\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.min\\.sci\\.b" 1 } } */

/* { dg-final { scan-assembler-times "cv\\.minu\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.minu\\.sc\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.minu\\.sci\\.h" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.minu\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.minu\\.sc\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.minu\\.sci\\.b" 1 } } */

/* { dg-final { scan-assembler-times "cv\\.max\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.max\\.sc\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.max\\.sci\\.h" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.max\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.max\\.sc\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.max\\.sci\\.b" 1 } } */

/* { dg-final { scan-assembler-times "cv\\.maxu\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.maxu\\.sc\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.maxu\\.sci\\.h" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.maxu\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.maxu\\.sc\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.maxu\\.sci\\.b" 1 } } */

/* { dg-final { scan-assembler-times "cv\\.srl\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.srl\\.sc\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.srl\\.sci\\.h" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.srl\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.srl\\.sc\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.srl\\.sci\\.b" 1 } } */

/* { dg-final { scan-assembler-times "cv\\.sra\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.sra\\.sc\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.sra\\.sci\\.h" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.sra\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.sra\\.sc\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.sra\\.sci\\.b" 1 } } */

/* { dg-final { scan-assembler-times "cv\\.sll\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.sll\\.sc\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.sll\\.sci\\.h" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.sll\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.sll\\.sc\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.sll\\.sci\\.b" 1 } } */

/* { dg-final { scan-assembler-times "cv\\.or\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.or\\.sc\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.or\\.sci\\.h" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.or\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.or\\.sc\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.or\\.sci\\.b" 1 } } */

/* { dg-final { scan-assembler-times "cv\\.xor\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.xor\\.sc\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.xor\\.sci\\.h" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.xor\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.xor\\.sc\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.xor\\.sci\\.b" 1 } } */

/* { dg-final { scan-assembler-times "cv\\.and\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.and\\.sc\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.and\\.sci\\.h" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.and\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.and\\.sc\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.and\\.sci\\.b" 1 } } */

/* { dg-final { scan-assembler-times "cv\\.abs\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.abs\\.b" 2 } } */

/* { dg-final { cleanup-saved-temps } } */
