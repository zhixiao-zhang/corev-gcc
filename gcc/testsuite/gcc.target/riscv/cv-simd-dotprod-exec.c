/* { dg-additional-sources cv-helpers.c } */
/* { dg-do run } */
/* { dg-require-effective-target cv_simd } */
/* We need -save-temps with "dg-do run", in order to save the generated file
   We use "dg-final" with "cleanup-saved-temps" at the end to clean up. */
/* { dg-options "-march=rv32i_xcvsimd -mabi=ilp32 -save-temps" } */

#include "cv-simd-helpers.h"

/* This run-time test does exemplary tests of each of the SIMD bit
   dot product instructions.
*/
int
main ()
{
  /* SIMD dotup variants. */
#define BUILTIN __builtin_riscv_cv_simd_dotup_h
#define OP(v1, v2) (UHELEM (v1, 0) * UHELEM (v2, 0) + \
		    UHELEM (v1, 1) * UHELEM (v2, 1))
  TEST_HVV_HVV (65521U,   561U, 42U, 65519U);
  TEST_HVV_HVC (  561U, 65519U, 42U, 65521U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_dotup_sc_h
#define OP(v1, s1) (UHELEM (v1, 0) * s1 + UHELEM (v1, 1) * s1)
  TEST_HVV_UHSV (42U, 65521U,   1U);
  TEST_HVV_HSC  (42U, 65521U,   3U);
  TEST_HVV_HSC  (42U, 65521U, 561U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_dotup_b
#define OP(v1, v2) (UBELEM (v1, 0) * UBELEM (v2, 0) + \
		    UBELEM (v1, 1) * UBELEM (v2, 1) + \
		    UBELEM (v1, 2) * UBELEM (v2, 2) + \
		    UBELEM (v1, 3) * UBELEM (v2, 3))
  TEST_BVV_BVV ( 17U,  32U,  36U,  72U, 42U,  28U, 27U,  25U);
  TEST_BVV_BVC (233U, 239U, 241U, 253U, 17U, 239U, 36U, 253U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_dotup_sc_b
#define OP(v1, s1) (UBELEM (v1, 0) * s1 + UBELEM (v1, 1) * s1 + \
		    UBELEM (v1, 2) * s1 + UBELEM (v1, 3) * s1)
  TEST_BVV_UBSV (72U, 36U, 32U, 17U, 28U);
  TEST_BVV_BSC (72U, 36U, 32U, 17U,  3U);
  TEST_BVV_BSC (72U, 36U, 32U, 17U, 72U);
#undef OP
#undef BUILTIN
#undef OP

  /* SIMD dotusp variants. */
#define BUILTIN __builtin_riscv_cv_simd_dotusp_h
#define OP(v1, v2) (UHELEM (v1, 0) * HELEM (v2, 0) + \
		    UHELEM (v1, 1) * HELEM (v2, 1))
  TEST_HVV_HVV (65521U,   561U,  42, -19937);
  TEST_HVV_HVC (  561U, 65519U, -42,  19391);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_dotusp_sc_h
#define OP(v1, s1) (UHELEM (v1, 0) * s1 + HELEM (v1, 1) * s1)
  TEST_HVV_HSV (42U, 65521U,   1);
  TEST_HVV_HSC (42U, 65521U,   3);
  TEST_HVV_HSC (42U, 65521U, 561);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_dotusp_b
#define OP(v1, v2) (UBELEM (v1, 0) * BELEM (v2, 0) + \
		    UBELEM (v1, 1) * BELEM (v2, 1) + \
		    UBELEM (v1, 2) * BELEM (v2, 2) + \
		    UBELEM (v1, 3) * BELEM (v2, 3))
  TEST_BVV_BVV ( 17U,  32U,  36U,  72U, 42,   28, 27,   25);
  TEST_BVV_BVC (233U, 239U, 241U, 253U, 17, -127, 36, -119);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_dotusp_sc_b
#define OP(v1, s1) (UBELEM (v1, 0) * s1 + BELEM (v1, 1) * s1 + \
		    UBELEM (v1, 2) * s1 + BELEM (v1, 3) * s1)
  TEST_BVV_BSV (72U, 36U, 32U, 17U, -28);
  TEST_BVV_BSC (72U, 36U, 32U, 17U,  -3);
  TEST_BVV_BSC (72U, 36U, 32U, 17U,  72);
#undef OP
#undef BUILTIN
#undef OP

  /* SIMD dotsp variants. */
#define BUILTIN __builtin_riscv_cv_simd_dotsp_h
#define OP(v1, v2) (HELEM (v1, 0) * HELEM (v2, 0) + \
		    HELEM (v1, 1) * HELEM (v2, 1))
  TEST_HVV_HVV (19391,    561,  42, -19937);
  TEST_HVV_HVC ( -561,  19937, -42,  19391);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_dotsp_sc_h
#define OP(v1, s1) (HELEM (v1, 0) * s1 + HELEM (v1, 1) * s1)
  TEST_HVV_HSV (42, -19937,   1);
  TEST_HVV_HSC (42, -19937,   3);
  TEST_HVV_HSC (42, -19937, 561);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_dotsp_b
#define OP(v1, v2) (BELEM (v1, 0) * BELEM (v2, 0) + \
		    BELEM (v1, 1) * BELEM (v2, 1) + \
		    BELEM (v1, 2) * BELEM (v2, 2) + \
		    BELEM (v1, 3) * BELEM (v2, 3))
  TEST_BVV_BVV ( 17, -32,   36,  -72, -42,  -28, 27,   25);
  TEST_BVV_BVC (127, 113, -109, -107,  17, -127, 36, -119);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_dotsp_sc_b
#define OP(v1, s1) (BELEM (v1, 0) * s1 + BELEM (v1, 1) * s1 + \
		    BELEM (v1, 2) * s1 + BELEM (v1, 3) * s1)
  TEST_BVV_BSV (-72, 36, -32, 17, -28);
  TEST_BVV_BSC (-72, 36, -32, 17,  -3);
  TEST_BVV_BSC (-72, 36, -32, 17,  72);
#undef OP
#undef BUILTIN
#undef OP

  /* SIMD sdotup variants. */
#define BUILTIN __builtin_riscv_cv_simd_sdotup_h
#define OP(v1, v2, w) (w + UHELEM (v1, 0) * UHELEM (v2, 0) + \
		           UHELEM (v1, 1) * UHELEM (v2, 1))
  TEST_HVV_HVV_W (65521U,   561U, 42U, 65519U, 39508041U);
  TEST_HVV_HVC_W (  561U, 65519U, 42U, 65521U, 39508041U );
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_sdotup_sc_h
#define OP(v1, s1, w) (w + UHELEM (v1, 0) * s1 + UHELEM (v1, 1) * s1)
  TEST_HVV_UHSV_W (42U, 65521U,   1U, 39508041U);
  TEST_HVV_HSC_W  (42U, 65521U,   3U, 39508041U);
  TEST_HVV_HSC_W  (42U, 65521U, 561U, 39508041U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_sdotup_b
#define OP(v1, v2, w) (w + UBELEM (v1, 0) * UBELEM (v2, 0) + \
		           UBELEM (v1, 1) * UBELEM (v2, 1) + \
		           UBELEM (v1, 2) * UBELEM (v2, 2) + \
		           UBELEM (v1, 3) * UBELEM (v2, 3))
  TEST_BVV_BVV_W ( 17U,  32U,  36U,  72U, 42U,  28U, 27U,  25U, 39508041U);
  TEST_BVV_BVC_W (233U, 239U, 241U, 253U, 17U, 239U, 36U, 253U, 39508041U);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_sdotup_sc_b
#define OP(v1, s1, w) (w + UBELEM (v1, 0) * s1 + UBELEM (v1, 1) * s1 + \
		           UBELEM (v1, 2) * s1 + UBELEM (v1, 3) * s1)
  TEST_BVV_UBSV_W (72U, 36U, 32U, 17U, 28U, 39508041U);
  TEST_BVV_BSC_W  (72U, 36U, 32U, 17U,  3U, 39508041U);
  TEST_BVV_BSC_W  (72U, 36U, 32U, 17U, 72U, 39508041U);
#undef OP
#undef BUILTIN
#undef OP

  /* SIMD sdotusp variants. */
#define BUILTIN __builtin_riscv_cv_simd_sdotusp_h
#define OP(v1, v2, w) (w + UHELEM (v1, 0) * HELEM (v2, 0) + \
		           UHELEM (v1, 1) * HELEM (v2, 1))
  TEST_HVV_HVV_W (65521U,   561U,  42, -19937, 39508041);
  TEST_HVV_HVC_W (  561U, 65519U, -42,  19391, 39508041 );
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_sdotusp_sc_h
#define OP(v1, s1, w) (w + UHELEM (v1, 0) * s1 + UHELEM (v1, 1) * s1)
  TEST_HVV_HSV_W (42U, 65521U,   1, 39508041);
  TEST_HVV_HSC_W (42U, 65521U,   3, 39508041);
  TEST_HVV_HSC_W (42U, 65521U, 561, 39508041);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_sdotusp_b
#define OP(v1, v2, w) (w + UBELEM (v1, 0) * BELEM (v2, 0) + \
		           UBELEM (v1, 1) * BELEM (v2, 1) + \
		           UBELEM (v1, 2) * BELEM (v2, 2) + \
		           UBELEM (v1, 3) * BELEM (v2, 3))
  TEST_BVV_BVV_W ( 17U,  32U,  36U,  72U, 42,   28, 27,   25, 39508041);
  TEST_BVV_BVC_W (233U, 239U, 241U, 253U, 17, -127, 36, -119, 39508041);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_sdotusp_sc_b
#define OP(v1, s1, w) (w + UBELEM (v1, 0) * s1 + UBELEM (v1, 1) * s1 + \
		           UBELEM (v1, 2) * s1 + UBELEM (v1, 3) * s1)
  TEST_BVV_BSV_W (72U, 36U, 32U, 17U, -28, 39508041);
  TEST_BVV_BSC_W (72U, 36U, 32U, 17U,   3, 39508041);
  TEST_BVV_BSC_W (72U, 36U, 32U, 17U,  72, 39508041);
#undef OP
#undef BUILTIN
#undef OP

  /* SIMD sdotsp variants. */
#define BUILTIN __builtin_riscv_cv_simd_sdotsp_h
#define OP(v1, v2, w) (w + HELEM (v1, 0) * HELEM (v2, 0) + \
		           HELEM (v1, 1) * HELEM (v2, 1))
  TEST_HVV_HVV_W (19391,   561,  42, -19937, 39508041);
  TEST_HVV_HVC_W ( -561, 19937, -42,  19391, 39508041);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_sdotsp_sc_h
#define OP(v1, s1, w) (w + HELEM (v1, 0) * s1 + HELEM (v1, 1) * s1)
  TEST_HVV_HSV_W (42U, -19937,   1, 39508041);
  TEST_HVV_HSC_W (42U, -19937,   3, 39508041);
  TEST_HVV_HSC_W (42U, -19937, 561, 39508041);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_sdotsp_b
#define OP(v1, v2, w) (w + BELEM (v1, 0) * BELEM (v2, 0) + \
		           BELEM (v1, 1) * BELEM (v2, 1) + \
		           BELEM (v1, 2) * BELEM (v2, 2) + \
		           BELEM (v1, 3) * BELEM (v2, 3))
  TEST_BVV_BVV_W ( 17, -32,   36,  -72, -42,  -28, 27,   25, 39508041);
  TEST_BVV_BVC_W (127, 113, -109, -107,  17, -127, 36, -119, 39508041);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_sdotsp_sc_b
#define OP(v1, s1, w) (w + BELEM (v1, 0) * s1 + BELEM (v1, 1) * s1 + \
		           BELEM (v1, 2) * s1 + BELEM (v1, 3) * s1)
  TEST_BVV_BSV_W (-72, 36, -32, 17, -28, 39508041);
  TEST_BVV_BSC_W (-72, 36, -32, 17,   3, 39508041);
  TEST_BVV_BSC_W (-72, 36, -32, 17,  72, 39508041);
#undef OP
#undef BUILTIN

  return 0;
}

/* { dg-final { scan-assembler-times "cv\\.dotup\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.dotup\\.sc\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.dotup\\.sci\\.h" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.dotup\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.dotup\\.sc\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.dotup\\.sci\\.b" 1 } } */

/* { dg-final { scan-assembler-times "cv\\.dotusp\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.dotusp\\.sc\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.dotusp\\.sci\\.h" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.dotusp\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.dotusp\\.sc\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.dotusp\\.sci\\.b" 1 } } */

/* { dg-final { scan-assembler-times "cv\\.dotsp\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.dotsp\\.sc\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.dotsp\\.sci\\.h" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.dotsp\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.dotsp\\.sc\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.dotsp\\.sci\\.b" 1 } } */

/* { dg-final { scan-assembler-times "cv\\.sdotup\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.sdotup\\.sc\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.sdotup\\.sci\\.h" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.sdotup\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.sdotup\\.sc\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.sdotup\\.sci\\.b" 1 } } */

/* { dg-final { scan-assembler-times "cv\\.sdotusp\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.sdotusp\\.sc\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.sdotusp\\.sci\\.h" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.sdotusp\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.sdotusp\\.sc\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.sdotusp\\.sci\\.b" 1 } } */

/* { dg-final { scan-assembler-times "cv\\.sdotsp\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.sdotsp\\.sc\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.sdotsp\\.sci\\.h" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.sdotsp\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.sdotsp\\.sc\\.b" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.sdotsp\\.sci\\.b" 1 } } */

/* { dg-final { cleanup-saved-temps } } */
