/* { dg-additional-sources cv-helpers.c } */
/* { dg-do run } */
/* { dg-require-effective-target cv_simd } */
/* We need -save-temps with "dg-do run", in order to save the generated file
   We use "dg-final" with "cleanup-saved-temps" at the end to clean up. */
/* { dg-options "-march=rv32i_xcvsimd -mabi=ilp32 -save-temps" } */

#include "cv-simd-helpers.h"

/* Helper functions */
static uint32_t shuffle2_h (uint32_t v1, uint32_t flgs, uint32_t v2)
{
  uint32_t src1 = ((uint32_t) ((flgs >> 17) & 0x1)) == 1 ? v1 : v2;
  uint32_t src2 = ((uint32_t) ((flgs >>  1) & 0x1)) == 1 ? v1 : v2;
  uint32_t res = MK_HV (UHELEM (src1, (uint32_t) ((flgs >> 16) & 0x1)), \
			UHELEM (src2, (uint32_t) ((flgs >>  0) & 0x1)));
  return res;
}

static uint32_t shuffle2_b (uint32_t v1, uint32_t flgs, uint32_t v2)
{
  uint32_t src1 = ((uint32_t) ((flgs >> 26) & 0x1)) == 1 ? v1 : v2;
  uint32_t src2 = ((uint32_t) ((flgs >> 18) & 0x1)) == 1 ? v1 : v2;
  uint32_t src3 = ((uint32_t) ((flgs >> 10) & 0x1)) == 1 ? v1 : v2;
  uint32_t src4 = ((uint32_t) ((flgs >>  2) & 0x1)) == 1 ? v1 : v2;
  uint32_t res = MK_BV (UBELEM (src1, (uint32_t) ((flgs >> 24) & 0x3)), \
			UBELEM (src2, (uint32_t) ((flgs >> 16) & 0x3)), \
			UBELEM (src3, (uint32_t) ((flgs >>  8) & 0x3)), \
			UBELEM (src4, (uint32_t) ((flgs >>  0) & 0x3)));
  return res;
}

/* This run-time test does exemplary tests of each of the SIMD shuffle and
   pack instructions.
*/
int
main ()
{
  /* SIMD shuffle variants. */
#define BUILTIN __builtin_riscv_cv_simd_shuffle_h
#define OP(v1, flgs) (MK_HV (UHELEM (v1, (uint32_t) ((flgs >> 16) & 0x1)), \
			     UHELEM (v1, (uint32_t) ((flgs >>  0) & 0x1))))
  TEST_HVV_WC (65521U,   561U, 0x00010001);
  TEST_HVC_WC (  561U, 65519U, 0x00010000);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_shuffle_sci_h
#define OP(v1, flgs) (MK_HV (UHELEM (v1, (uint32_t) ((flgs >> 1) & 0x1)), \
			     UHELEM (v1, (uint32_t) ((flgs >> 0) & 0x1))))
  TEST_HVV_WC (42U, 65521U,  0x0);
  TEST_HVC_WC (42U, 65521U,  0x1);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_shuffle_b
#define OP(v1, flgs) (MK_BV (UBELEM (v1, (uint32_t) ((flgs >> 24) & 0x3)), \
                             UBELEM (v1, (uint32_t) ((flgs >> 16) & 0x3)), \
                             UBELEM (v1, (uint32_t) ((flgs >>  8) & 0x3)), \
                             UBELEM (v1, (uint32_t) ((flgs >>  0) & 0x3))))
  TEST_BVV_WC ( 17U,  32U,  36U,  72U, 0x03030303);
  TEST_BVC_WC (233U, 239U, 241U, 253U, 0x00010203);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_shuffle_sci_b
#define OP(v1, flgs) (MK_BV (UBELEM (v1, (uint32_t) ((flgs >> 6) & 0x3)), \
                             UBELEM (v1, (uint32_t) ((flgs >> 4) & 0x3)), \
                             UBELEM (v1, (uint32_t) ((flgs >> 2) & 0x3)), \
                             UBELEM (v1, (uint32_t) ((flgs >> 0) & 0x3))))
  TEST_BVV_WC (72U, 36U, 32U, 17U, 0x00);
  TEST_BVC_WC (72U, 36U, 32U, 17U, 0x39);
  TEST_BVV_WC (72U, 36U, 32U, 17U, 0x40);
  TEST_BVC_WC (72U, 36U, 32U, 17U, 0x79);
  TEST_BVV_WC (72U, 36U, 32U, 17U, 0x80);
  TEST_BVC_WC (72U, 36U, 32U, 17U, 0xb9);
  TEST_BVV_WC (72U, 36U, 32U, 17U, 0xc0);
  TEST_BVC_WC (72U, 36U, 32U, 17U, 0xf9);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_shuffle2_h
#define OP(v1, flgs, v2) (shuffle2_h (v1, flgs, v2))
  TEST_HVV_W_HVV (65521U,   561U, 0x00010001,    42U, 65519U);
  TEST_HVC_W_HVV (  561U, 65519U, 0x00030000, 65521U,    42U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_shuffle2_b
#define OP(v1, flgs, v2) (shuffle2_b (v1, flgs, v2))
  TEST_BVV_W_BVV ( 17U,  32U,  36U,  72U, 0x07070707, 233U, 239U, 241U, 253U);
  TEST_BVC_W_BVV (233U, 239U, 241U, 253U, 0x04010603,  17U,  32U,  36U,  72U);
#undef OP
#undef BUILTIN

  /* SIMD pack variants. */
#define BUILTIN __builtin_riscv_cv_simd_packhi_h
#define OP(v1, v2) (MK_HV (UHELEM (v1, 1), UHELEM (v2, 1)))
  TEST_HVV_HVV (65521U,   561U,    42U, 65519U);
  TEST_HVV_HVC (  561U, 65519U, 65521U,    42U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_packlo_h
#define OP(v1, v2) (MK_HV (UHELEM (v1, 0), UHELEM (v2, 0)))
  TEST_HVV_HVV (65521U,   561U,    42U, 65519U);
  TEST_HVV_HVC (  561U, 65519U, 65521U,    42U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_packhi_b
#define OP(v1, v2, v3) (MK_BV (UBELEM (v1, 0), UBELEM (v2, 0), \
			       UBELEM (v3, 1), UBELEM (v3, 0)))
  TEST_BVV_BVV_BVV ( 17U, 32U, 36U, 72U, 233U, 239U, 241U, 253U, 72U, 36U,
		     30U,  17U);
  TEST_BVV_BVC_BVV ( 17U, 32U, 36U, 72U, 233U, 239U, 241U, 253U, 72U, 36U,
		     30U,  17U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_packlo_b
#define OP(v1, v2, v3) (MK_BV (UBELEM (v3, 3), UBELEM (v3, 2), \
			       UBELEM (v1, 0), UBELEM (v2, 0)))
  TEST_BVV_BVV_BVV ( 17U, 32U, 36U, 72U, 233U, 239U, 241U, 253U, 72U, 36U,
		     30U,  17U);
  TEST_BVV_BVC_BVV ( 17U, 32U, 36U, 72U, 233U, 239U, 241U, 253U, 72U, 36U,
		     30U,  17U);
#undef OP
#undef BUILTIN

  return 0;
}

/* { dg-final { scan-assembler-times "cv\\.shuffle\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.shuffle\\.sci\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.shuffle\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.shufflei0\\.sci\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.shufflei1\\.sci\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.shufflei2\\.sci\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.shufflei3\\.sci\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.shuffle2\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.shuffle2\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.pack\[^\\.lh\]" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.pack\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.packlo\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.packhi\\.b" 2 } } */

/* { dg-final { cleanup-saved-temps } } */
