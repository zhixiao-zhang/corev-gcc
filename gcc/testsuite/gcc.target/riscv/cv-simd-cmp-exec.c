/* { dg-additional-sources cv-helpers.c } */
/* { dg-do run } */
/* { dg-require-effective-target cv_simd } */
/* We need -save-temps with "dg-do run", in order to save the generated file
   We use "dg-final" with "cleanup-saved-temps" at the end to clean up. */
/* { dg-options "-march=rv32i_xcvsimd -mabi=ilp32 -save-temps" } */

#include "cv-simd-helpers.h"

/* This run-time test does exemplary tests of each of the SIMD comparison
   instructions.  We generally offer examples of the following variants for
   each operand.

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
  /* SIMD cmpeq variants. */
#define BUILTIN __builtin_riscv_cv_simd_cmpeq_h
#define OP(v1, v2) (MK_HV ((HELEM (v1, 1) == HELEM (v2, 1)) ? 0xffff : 0x0000, \
			   (HELEM (v1, 0) == HELEM (v2, 0)) ? 0xffff : 0x0000))
  TEST_HVV_HVV (42, -19937, -19391,    561);
  TEST_HVV_HVC (42, -19937,     42, -19937);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_cmpeq_sc_h
#define OP(v1, s1) (MK_HV ((HELEM (v1, 1) == s1) ? 0xffff : 0x0000, \
			   (HELEM (v1, 0) == s1) ? 0xffff : 0x0000))
  TEST_HVV_HSV (561,  19391,      1);
  TEST_HVV_HSC (561,  19391,     -1);
  TEST_HVV_HSC (561,  19391,    561);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_cmpeq_b
#define OP(v1, v2) (MK_BV ((BELEM (v1, 3) == BELEM (v2, 3)) ? 0xff : 0x00, \
                           (BELEM (v1, 2) == BELEM (v2, 2)) ? 0xff : 0x00, \
                           (BELEM (v1, 1) == BELEM (v2, 1)) ? 0xff : 0x00, \
                           (BELEM (v1, 0) == BELEM (v2, 0)) ? 0xff : 0x00))
  TEST_BVV_BVV ( 17,  32,  36,  72, 42,  28, 27,  25);
  TEST_BVV_BVC (107, -32, 113, 127, 17, -32, 27, 127);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_cmpeq_sc_b
#define OP(v1, s1) (MK_BV ((BELEM (v1, 3) == s1) ? 0xff : 0x00, \
                           (BELEM (v1, 2) == s1) ? 0xff : 0x00, \
                           (BELEM (v1, 1) == s1) ? 0xff : 0x00, \
                           (BELEM (v1, 0) == s1) ? 0xff : 0x00))
  TEST_BVV_BSV (-17,  32, -36,  72, 28);
  TEST_BVV_BSC (-17, -32, -36, -72,  1);
  TEST_BVV_BSC (-17,  32, -36,  72, 72);
#undef OP
#undef BUILTIN

  /* SIMD cmpne variants. */
#define BUILTIN __builtin_riscv_cv_simd_cmpne_h
#define OP(v1, v2) (MK_HV ((HELEM (v1, 1) != HELEM (v2, 1)) ? 0xffff : 0x0000, \
			   (HELEM (v1, 0) != HELEM (v2, 0)) ? 0xffff : 0x0000))
  TEST_HVV_HVV (42, -19937, -19391,    561);
  TEST_HVV_HVC (42, -19937,     42, -19937);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_cmpne_sc_h
#define OP(v1, s1) (MK_HV ((HELEM (v1, 1) != s1) ? 0xffff : 0x0000, \
			   (HELEM (v1, 0) != s1) ? 0xffff : 0x0000))
  TEST_HVV_HSV (561,  19391,      1);
  TEST_HVV_HSC (561,  19391,     -1);
  TEST_HVV_HSC (561,  19391,    561);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_cmpne_b
#define OP(v1, v2) (MK_BV ((BELEM (v1, 3) != BELEM (v2, 3)) ? 0xff : 0x00, \
                           (BELEM (v1, 2) != BELEM (v2, 2)) ? 0xff : 0x00, \
                           (BELEM (v1, 1) != BELEM (v2, 1)) ? 0xff : 0x00, \
                           (BELEM (v1, 0) != BELEM (v2, 0)) ? 0xff : 0x00))
  TEST_BVV_BVV ( 17,  32,  36,  72, 42,  28, 27,  25);
  TEST_BVV_BVC (107, 109, 113, 127, 17, -32, 27, 127);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_cmpne_sc_b
#define OP(v1, s1) (MK_BV ((BELEM (v1, 3) != s1) ? 0xff : 0x00, \
                           (BELEM (v1, 2) != s1) ? 0xff : 0x00, \
                           (BELEM (v1, 1) != s1) ? 0xff : 0x00, \
                           (BELEM (v1, 0) != s1) ? 0xff : 0x00))
  TEST_BVV_BSV (-17,  32, -36,  72, 28);
  TEST_BVV_BSC (-17, -32, -36, -72,  1);
  TEST_BVV_BSC (-17,  32, -36,  72, 72);
#undef OP
#undef BUILTIN

  /* SIMD cmpgt variants. */
#define BUILTIN __builtin_riscv_cv_simd_cmpgt_h
#define OP(v1, v2) (MK_HV ((HELEM (v1, 1) > HELEM (v2, 1)) ? 0xffff : 0x0000, \
			   (HELEM (v1, 0) > HELEM (v2, 0)) ? 0xffff : 0x0000))
  TEST_HVV_HVV (42, -19937, -19391,    561);
  TEST_HVV_HVC (42, -19937,    561, -19937);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_cmpgt_sc_h
#define OP(v1, s1) (MK_HV ((HELEM (v1, 1) > s1) ? 0xffff : 0x0000, \
			   (HELEM (v1, 0) > s1) ? 0xffff : 0x0000))
  TEST_HVV_HSV (561,  19391,      1);
  TEST_HVV_HSC (561,  19391,     -1);
  TEST_HVV_HSC (561,  19391,    561);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_cmpgt_b
#define OP(v1, v2) (MK_BV ((BELEM (v1, 3) > BELEM (v2, 3)) ? 0xff : 0x00, \
                           (BELEM (v1, 2) > BELEM (v2, 2)) ? 0xff : 0x00, \
                           (BELEM (v1, 1) > BELEM (v2, 1)) ? 0xff : 0x00, \
                           (BELEM (v1, 0) > BELEM (v2, 0)) ? 0xff : 0x00))
  TEST_BVV_BVV ( 17,  32,  36,  72, 42,  28, 27,  25);
  TEST_BVV_BVC (107, 109, 113, 127, 17, -32, 27, 127);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_cmpgt_sc_b
#define OP(v1, s1) (MK_BV ((BELEM (v1, 3) > s1) ? 0xff : 0x00, \
                           (BELEM (v1, 2) > s1) ? 0xff : 0x00, \
                           (BELEM (v1, 1) > s1) ? 0xff : 0x00, \
                           (BELEM (v1, 0) > s1) ? 0xff : 0x00))
  TEST_BVV_BSV (-17,  32, -36,  72, 28);
  TEST_BVV_BSC (-17, -32, -36, -72,  1);
  TEST_BVV_BSC (-17,  32, -36,  72, 72);
#undef OP
#undef BUILTIN

  /* SIMD cmpge variants. */
#define BUILTIN __builtin_riscv_cv_simd_cmpge_h
#define OP(v1, v2) (MK_HV ((HELEM (v1, 1) >= HELEM (v2, 1)) ? 0xffff : 0x0000, \
			   (HELEM (v1, 0) >= HELEM (v2, 0)) ? 0xffff : 0x0000))
  TEST_HVV_HVV (42, -19937, -19391,    561);
  TEST_HVV_HVC (42, -19937,    561, -19937);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_cmpge_sc_h
#define OP(v1, s1) (MK_HV ((HELEM (v1, 1) >= s1) ? 0xffff : 0x0000, \
			   (HELEM (v1, 0) >= s1) ? 0xffff : 0x0000))
  TEST_HVV_HSV (561,  19391,      1);
  TEST_HVV_HSC (561,  19391,     -1);
  TEST_HVV_HSC (561,  19391,    561);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_cmpge_b
#define OP(v1, v2) (MK_BV ((BELEM (v1, 3) >= BELEM (v2, 3)) ? 0xff : 0x00, \
                           (BELEM (v1, 2) >= BELEM (v2, 2)) ? 0xff : 0x00, \
                           (BELEM (v1, 1) >= BELEM (v2, 1)) ? 0xff : 0x00, \
                           (BELEM (v1, 0) >= BELEM (v2, 0)) ? 0xff : 0x00))
  TEST_BVV_BVV ( 17,  32,  36,  72, 42,  28, 27,  25);
  TEST_BVV_BVC (107, 109, 113, 127, 17, -32, 27, 127);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_cmpge_sc_b
#define OP(v1, s1) (MK_BV ((BELEM (v1, 3) >= s1) ? 0xff : 0x00, \
                           (BELEM (v1, 2) >= s1) ? 0xff : 0x00, \
                           (BELEM (v1, 1) >= s1) ? 0xff : 0x00, \
                           (BELEM (v1, 0) >= s1) ? 0xff : 0x00))
  TEST_BVV_BSV (-17,  32, -36,  72, 28);
  TEST_BVV_BSC (-17, -32, -36, -72,  1);
  TEST_BVV_BSC (-17,  32, -36,  72, 72);
#undef OP
#undef BUILTIN

  /* SIMD cmplt variants. */
#define BUILTIN __builtin_riscv_cv_simd_cmplt_h
#define OP(v1, v2) (MK_HV ((HELEM (v1, 1) < HELEM (v2, 1)) ? 0xffff : 0x0000, \
			   (HELEM (v1, 0) < HELEM (v2, 0)) ? 0xffff : 0x0000))
  TEST_HVV_HVV (42, -19937, -19391,    561);
  TEST_HVV_HVC (42, -19937,    561, -19937);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_cmplt_sc_h
#define OP(v1, s1) (MK_HV ((HELEM (v1, 1) < s1) ? 0xffff : 0x0000, \
			   (HELEM (v1, 0) < s1) ? 0xffff : 0x0000))
  TEST_HVV_HSV (561,  19391,      1);
  TEST_HVV_HSC (561,  19391,     -1);
  TEST_HVV_HSC (561,  19391,    561);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_cmplt_b
#define OP(v1, v2) (MK_BV ((BELEM (v1, 3) < BELEM (v2, 3)) ? 0xff : 0x00, \
                           (BELEM (v1, 2) < BELEM (v2, 2)) ? 0xff : 0x00, \
                           (BELEM (v1, 1) < BELEM (v2, 1)) ? 0xff : 0x00, \
                           (BELEM (v1, 0) < BELEM (v2, 0)) ? 0xff : 0x00))
  TEST_BVV_BVV ( 17,  32,  36,  72, 42,  28, 27,  25);
  TEST_BVV_BVC (107, 109, 113, 127, 17, -32, 27, 127);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_cmplt_sc_b
#define OP(v1, s1) (MK_BV ((BELEM (v1, 3) < s1) ? 0xff : 0x00, \
                           (BELEM (v1, 2) < s1) ? 0xff : 0x00, \
                           (BELEM (v1, 1) < s1) ? 0xff : 0x00, \
                           (BELEM (v1, 0) < s1) ? 0xff : 0x00))
  TEST_BVV_BSV (-17,  32, -36,  72, 28);
  TEST_BVV_BSC (-17, -32, -36, -72,  1);
  TEST_BVV_BSC (-17,  32, -36,  72, 72);
#undef OP
#undef BUILTIN

  /* SIMD cmple variants. */
#define BUILTIN __builtin_riscv_cv_simd_cmple_h
#define OP(v1, v2) (MK_HV ((HELEM (v1, 1) <= HELEM (v2, 1)) ? 0xffff : 0x0000, \
			   (HELEM (v1, 0) <= HELEM (v2, 0)) ? 0xffff : 0x0000))
  TEST_HVV_HVV (42, -19937, -19391,    561);
  TEST_HVV_HVC (42, -19937,    561, -19937);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_cmple_sc_h
#define OP(v1, s1) (MK_HV ((HELEM (v1, 1) <= s1) ? 0xffff : 0x0000, \
			   (HELEM (v1, 0) <= s1) ? 0xffff : 0x0000))
  TEST_HVV_HSV (561,  19391,      1);
  TEST_HVV_HSC (561,  19391,     -1);
  TEST_HVV_HSC (561,  19391,    561);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_cmple_b
#define OP(v1, v2) (MK_BV ((BELEM (v1, 3) <= BELEM (v2, 3)) ? 0xff : 0x00, \
                           (BELEM (v1, 2) <= BELEM (v2, 2)) ? 0xff : 0x00, \
                           (BELEM (v1, 1) <= BELEM (v2, 1)) ? 0xff : 0x00, \
                           (BELEM (v1, 0) <= BELEM (v2, 0)) ? 0xff : 0x00))
  TEST_BVV_BVV ( 17,  32,  36,  72, 42,  28, 27,  25);
  TEST_BVV_BVC (107, 109, 113, 127, 17, -32, 27, 127);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_cmple_sc_b
#define OP(v1, s1) (MK_BV ((BELEM (v1, 3) <= s1) ? 0xff : 0x00, \
                           (BELEM (v1, 2) <= s1) ? 0xff : 0x00, \
                           (BELEM (v1, 1) <= s1) ? 0xff : 0x00, \
                           (BELEM (v1, 0) <= s1) ? 0xff : 0x00))
  TEST_BVV_BSV (-17,  32, -36,  72, 28);
  TEST_BVV_BSC (-17, -32, -36, -72,  1);
  TEST_BVV_BSC (-17,  32, -36,  72, 72);
#undef OP
#undef BUILTIN
#undef OP

  /* SIMD cmpgtu variants. */
#define BUILTIN __builtin_riscv_cv_simd_cmpgtu_h
#define OP(v1, v2) (MK_HV ((UHELEM (v1, 1) > UHELEM (v2, 1)) ? \
		           0xffff : 0x0000,		       \
			   (UHELEM (v1, 0) > UHELEM (v2, 0)) ? \
                           0xffff : 0x0000))
  TEST_HVV_HVV (42U, 65521U, 65519U,   561U);
  TEST_HVV_HVC (42U, 65521U,   561U, 65521U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_cmpgtu_sc_h
#define OP(v1, s1) (MK_HV ((UHELEM (v1, 1) > s1) ? 0xffff : 0x0000, \
			   (UHELEM (v1, 0) > s1) ? 0xffff : 0x0000))
  TEST_HVV_HSV (561U, 19391U,     1U);
  TEST_HVV_HSC (561U, 19391U,     3U);
  TEST_HVV_HSC (561U, 19391U,   561U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_cmpgtu_b
#define OP(v1, v2) (MK_BV ((UBELEM (v1, 3) > UBELEM (v2, 3)) ? 0xff : 0x00, \
                           (UBELEM (v1, 2) > UBELEM (v2, 2)) ? 0xff : 0x00, \
                           (UBELEM (v1, 1) > UBELEM (v2, 1)) ? 0xff : 0x00, \
                           (UBELEM (v1, 0) > UBELEM (v2, 0)) ? 0xff : 0x00))
  TEST_BVV_BVV ( 17U,  32U,  36U,  72U, 42U,  28U, 27U,  25U);
  TEST_BVV_BVC (107, 109, 113, 127U, 17U, 239U, 27U, 127U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_cmpgtu_sc_b
#define OP(v1, s1) (MK_BV ((UBELEM (v1, 3) > s1) ? 0xff : 0x00, \
                           (UBELEM (v1, 2) > s1) ? 0xff : 0x00, \
                           (UBELEM (v1, 1) > s1) ? 0xff : 0x00, \
                           (UBELEM (v1, 0) > s1) ? 0xff : 0x00))
  TEST_BVV_BSV (233U,  32U, 241U,  72U, 28U);
  TEST_BVV_BSC (233U, 239U, 241U, 255U,  1U);
  TEST_BVV_BSC (233U,  32U, 241U,  72U, 72U);
#undef OP
#undef BUILTIN

  /* SIMD cmpgeu variants. */
#define BUILTIN __builtin_riscv_cv_simd_cmpgeu_h
#define OP(v1, v2) (MK_HV ((UHELEM (v1, 1) >= UHELEM (v2, 1)) ? \
		           0xffff : 0x0000,			\
			   (UHELEM (v1, 0) >= UHELEM (v2, 0)) ? \
                           0xffff : 0x0000))
  TEST_HVV_HVV (42U,  65521U,  65519U,    561U);
  TEST_HVV_HVC (42U,  65521U,    561U,  65521U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_cmpgeu_sc_h
#define OP(v1, s1) (MK_HV ((UHELEM (v1, 1) >= s1) ? 0xffff : 0x0000, \
			   (UHELEM (v1, 0) >= s1) ? 0xffff : 0x0000))
  TEST_HVV_HSV (561U,  19391U,      1U);
  TEST_HVV_HSC (561U,  19391U,     3U);
  TEST_HVV_HSC (561U,  19391U,    561U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_cmpgeu_b
#define OP(v1, v2) (MK_BV ((UBELEM (v1, 3) >= UBELEM (v2, 3)) ? 0xff : 0x00, \
                           (UBELEM (v1, 2) >= UBELEM (v2, 2)) ? 0xff : 0x00, \
                           (UBELEM (v1, 1) >= UBELEM (v2, 1)) ? 0xff : 0x00, \
                           (UBELEM (v1, 0) >= UBELEM (v2, 0)) ? 0xff : 0x00))
  TEST_BVV_BVV ( 17U,  32U,  36U,  72U, 42U,  28U, 27U,  25U);
  TEST_BVV_BVC (107, 109, 113, 127U, 17U, 239U, 27U, 127U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_cmpgeu_sc_b
#define OP(v1, s1) (MK_BV ((UBELEM (v1, 3) >= s1) ? 0xff : 0x00, \
                           (UBELEM (v1, 2) >= s1) ? 0xff : 0x00, \
                           (UBELEM (v1, 1) >= s1) ? 0xff : 0x00, \
                           (UBELEM (v1, 0) >= s1) ? 0xff : 0x00))
  TEST_BVV_BSV (233U,  32U, 241U,  72U, 28U);
  TEST_BVV_BSC (233U, 239U, 241U, 255U,  1U);
  TEST_BVV_BSC (233U,  32U, 241U,  72U, 72U);
#undef OP
#undef BUILTIN

  /* SIMD cmpltu variants. */
#define BUILTIN __builtin_riscv_cv_simd_cmpltu_h
#define OP(v1, v2) (MK_HV ((UHELEM (v1, 1) < UHELEM (v2, 1)) ? \
		           0xffff : 0x0000,		       \
			   (UHELEM (v1, 0) < UHELEM (v2, 0)) ? \
                           0xffff : 0x0000))
  TEST_HVV_HVV (42U,  65521U,  65519U,    561U);
  TEST_HVV_HVC (42U,  65521U,    561U,  65521U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_cmpltu_sc_h
#define OP(v1, s1) (MK_HV ((UHELEM (v1, 1) < s1) ? 0xffff : 0x0000, \
			   (UHELEM (v1, 0) < s1) ? 0xffff : 0x0000))
  TEST_HVV_HSV (561U,  19391U,      1U);
  TEST_HVV_HSC (561U,  19391U,     3U);
  TEST_HVV_HSC (561U,  19391U,    561U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_cmpltu_b
#define OP(v1, v2) (MK_BV ((UBELEM (v1, 3) < UBELEM (v2, 3)) ? 0xff : 0x00, \
                           (UBELEM (v1, 2) < UBELEM (v2, 2)) ? 0xff : 0x00, \
                           (UBELEM (v1, 1) < UBELEM (v2, 1)) ? 0xff : 0x00, \
                           (UBELEM (v1, 0) < UBELEM (v2, 0)) ? 0xff : 0x00))
  TEST_BVV_BVV ( 17U,  32U,  36U,  72U, 42U,  28U, 27U,  25U);
  TEST_BVV_BVC (107U, 109U, 113U, 127U, 17U, 239U, 27U, 127U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_cmpltu_sc_b
#define OP(v1, s1) (MK_BV ((UBELEM (v1, 3) < s1) ? 0xff : 0x00, \
                           (UBELEM (v1, 2) < s1) ? 0xff : 0x00, \
                           (UBELEM (v1, 1) < s1) ? 0xff : 0x00, \
                           (UBELEM (v1, 0) < s1) ? 0xff : 0x00))
  TEST_BVV_BSV (233U,  32U, 241U,  72U, 28U);
  TEST_BVV_BSC (233U, 239U, 241U, 255U,  1U);
  TEST_BVV_BSC (233U,  32U, 241U,  72U, 72U);
#undef OP
#undef BUILTIN

  /* SIMD cmpleu variants. */
#define BUILTIN __builtin_riscv_cv_simd_cmpleu_h
#define OP(v1, v2) (MK_HV ((UHELEM (v1, 1) <= UHELEM (v2, 1)) ? \
		           0xffff : 0x0000,			\
			   (UHELEM (v1, 0) <= UHELEM (v2, 0)) ? \
                           0xffff : 0x0000))
  TEST_HVV_HVV (42U,  65521U,  65519U,    561U);
  TEST_HVV_HVC (42U,  65521U,    561U,  65521U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_cmpleu_sc_h
#define OP(v1, s1) (MK_HV ((UHELEM (v1, 1) <= s1) ? 0xffff : 0x0000, \
			   (UHELEM (v1, 0) <= s1) ? 0xffff : 0x0000))
  TEST_HVV_HSV (561U,  19391U,      1U);
  TEST_HVV_HSC (561U,  19391U,     3U);
  TEST_HVV_HSC (561U,  19391U,    561U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_cmpleu_b
#define OP(v1, v2) (MK_BV ((UBELEM (v1, 3) <= UBELEM (v2, 3)) ? 0xff : 0x00, \
                           (UBELEM (v1, 2) <= UBELEM (v2, 2)) ? 0xff : 0x00, \
                           (UBELEM (v1, 1) <= UBELEM (v2, 1)) ? 0xff : 0x00, \
                           (UBELEM (v1, 0) <= UBELEM (v2, 0)) ? 0xff : 0x00))
  TEST_BVV_BVV ( 17U,  32U,  36U,  72U, 42U,  28U, 27U,  25);
  TEST_BVV_BVC (107U, 109U, 113U, 127, 17U, 239U, 27, 127);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_cmpleu_sc_b
#define OP(v1, s1) (MK_BV ((UBELEM (v1, 3) <= s1) ? 0xff : 0x00, \
                           (UBELEM (v1, 2) <= s1) ? 0xff : 0x00, \
                           (UBELEM (v1, 1) <= s1) ? 0xff : 0x00, \
                           (UBELEM (v1, 0) <= s1) ? 0xff : 0x00))
  TEST_BVV_BSV (233U,  32U, 241U,  72U, 28);
  TEST_BVV_BSC (233U, 239U, 241U, 255U,  1U);
  TEST_BVV_BSC (233U,  32U, 241U,  72U, 72U);
#undef OP
#undef BUILTIN

  return 0;
}

/* { dg-final { scan-assembler-times "cv\\.cmpeq\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpeq\\.sc\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpeq\\.sci\\.h" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpeq\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpeq\\.sc\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpeq\\.sci\\.b" 1 } } */

/* { dg-final { scan-assembler-times "cv\\.cmpne\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpne\\.sc\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpne\\.sci\\.h" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpne\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpne\\.sc\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpne\\.sci\\.b" 1 } } */

/* { dg-final { scan-assembler-times "cv\\.cmpgt\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpgt\\.sc\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpgt\\.sci\\.h" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpgt\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpgt\\.sc\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpgt\\.sci\\.b" 1 } } */

/* { dg-final { scan-assembler-times "cv\\.cmpge\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpge\\.sc\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpge\\.sci\\.h" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpge\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpge\\.sc\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpge\\.sci\\.b" 1 } } */

/* { dg-final { scan-assembler-times "cv\\.cmplt\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmplt\\.sc\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmplt\\.sci\\.h" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.cmplt\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmplt\\.sc\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmplt\\.sci\\.b" 1 } } */

/* { dg-final { scan-assembler-times "cv\\.cmple\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmple\\.sc\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmple\\.sci\\.h" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.cmple\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmple\\.sc\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmple\\.sci\\.b" 1 } } */

/* { dg-final { scan-assembler-times "cv\\.cmpgtu\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpgtu\\.sc\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpgtu\\.sci\\.h" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpgtu\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpgtu\\.sc\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpgtu\\.sci\\.b" 1 } } */

/* { dg-final { scan-assembler-times "cv\\.cmpgeu\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpgeu\\.sc\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpgeu\\.sci\\.h" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpgeu\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpgeu\\.sc\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpgeu\\.sci\\.b" 1 } } */

/* { dg-final { scan-assembler-times "cv\\.cmpltu\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpltu\\.sc\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpltu\\.sci\\.h" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpltu\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpltu\\.sc\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpltu\\.sci\\.b" 1 } } */

/* { dg-final { scan-assembler-times "cv\\.cmpleu\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpleu\\.sc\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpleu\\.sci\\.h" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpleu\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpleu\\.sc\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.cmpleu\\.sci\\.b" 1 } } */

/* { dg-final { cleanup-saved-temps } } */
