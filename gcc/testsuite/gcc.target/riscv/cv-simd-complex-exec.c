/* { dg-additional-sources cv-helpers.c } */
/* { dg-do run } */
/* { dg-require-effective-target cv_simd } */
/* We need -save-temps with "dg-do run", in order to save the generated file
   We use "dg-final" with "cleanup-saved-temps" at the end to clean up. */
/* { dg-options "-march=rv32i_xcvsimd -mabi=ilp32 -save-temps" } */

#include "cv-simd-helpers.h"

/* Helper functions */
static uint32_t
cplxmul_r (uint32_t v1, uint32_t v2, uint32_t v3, int shft)
{
  int32_t p = (HELEM (v1, 0) * HELEM (v2, 0) - HELEM (v1, 1) * HELEM (v2, 1))
    >> (15 + shft);
  return MK_HV (HELEM (v3, 1), p);
}

static uint32_t
cplxmul_i (uint32_t v1, uint32_t v2, uint32_t v3, int shft)
{
  int32_t p = (HELEM (v1, 0) * HELEM (v2, 1) + HELEM (v1, 1) * HELEM (v2, 0))
    >> (15 + shft);
  return MK_HV (p, HELEM (v3, 0));
}

static uint32_t
add_sh (uint32_t v1, uint32_t v2, int shft)
{
  int16_t r1 = (HELEM(v1, 1) + HELEM(v2, 1)) & 0xffff;
  int16_t r2 = (HELEM(v1, 0) + HELEM(v2, 0)) & 0xffff;
  int32_t res = MK_HV (r1 >> shft, r2 >> shft);
  return res;
}

static uint32_t
sub_sh (uint32_t v1, uint32_t v2, int shft)
{
  int16_t r1 = (HELEM(v1, 1) - HELEM(v2, 1)) & 0xffff;
  int16_t r2 = (HELEM(v1, 0) - HELEM(v2, 0)) & 0xffff;
  int32_t res = MK_HV (r1 >> shft, r2 >> shft);
  return res;
}

/* This run-time test does exemplary tests of each of the SIMD complex
   arithmetic instructions.
*/
int
main ()
{
  /* SIMD complex multiplication variants. */
#define BUILTIN __builtin_riscv_cv_simd_cplxmul_r
#define OP(v1, v2, v3, shft) (cplxmul_r (v1, v2, v3, shft))
  TEST_HVV_HVV_HVV_W (32719, 561, 561, -32749, 42, -1729, 0);
  TEST_HVV_HVV_HVV_W (32719, 561, 561, -32749, 42, -1729, 1);
  TEST_HVV_HVV_HVV_W (32719, 561, 561, -32749, 42, -1729, 2);
  TEST_HVV_HVV_HVV_W (32719, 561, 561, -32749, 42, -1729, 3);
#undef OP
#undef BUILTIN
#define BUILTIN __builtin_riscv_cv_simd_cplxmul_i
#define OP(v1, v2, v3, shft) (cplxmul_i (v1, v2, v3, shft))
  TEST_HVV_HVV_HVV_W (32719, 561, 561, -32749, 42, -1729, 0);
  TEST_HVV_HVV_HVV_W (32719, 561, 561, -32749, 42, -1729, 1);
  TEST_HVV_HVV_HVV_W (32719, 561, 561, -32749, 42, -1729, 2);
  TEST_HVV_HVV_HVV_W (32719, 561, 561, -32749, 42, -1729, 3);
#undef OP
#undef BUILTIN

  /* SIMD complex conjugate. */
#define BUILTIN __builtin_riscv_cv_simd_cplxconj
#define OP(v1) (MK_HV (-HELEM (v1, 1), HELEM (v1, 0)))
  TEST_HVV (561, -32749);
  TEST_HVC ( 42,  -1729);
#undef OP
#undef BUILTIN

    /* SIMD matrix rotation by j */
#define BUILTIN __builtin_riscv_cv_simd_subrotmj
#define OP(v1, v2, shft) (MK_HV (((HELEM (v2, 0) - HELEM (v1, 0)) & 0xffff) \
				 >> shft,                                   \
				 ((HELEM (v1, 1) - HELEM (v2, 1)) & 0xffff) \
				 >> shft))
  TEST_HVV_HVV_W (561, -32749, 42, -1729, 0);
  TEST_HVV_HVV_W (561, -32749, 42, -1729, 1);
  TEST_HVV_HVC_W (561, -32749, 42, -1729, 2);
  TEST_HVV_HVC_W (561, -32749, 42, -1729, 3);
#undef OP
#undef BUILTIN

  /* SIMD add/sub variants for complex numbers */
#define OP(v1, v2, shft) (add_sh (v1, v2, shft))
#define BUILTIN __builtin_riscv_cv_simd_add_h
  TEST_HVV_HVV_WC ( 42,  19927,    561,  19391, 1);
  TEST_HVV_HVC_WC (-42,   -561, -19391, -19937, 1);
  TEST_HVV_HVV_WC ( 42,  19927,    561,  19391, 2);
  TEST_HVV_HVC_WC (-42,   -561, -19391, -19937, 2);
  TEST_HVV_HVV_WC ( 42,  19927,    561,  19391, 3);
  TEST_HVV_HVC_WC (-42,   -561, -19391, -19937, 3);
#undef BUILTIN
#undef OP
#define OP(v1, v2, shft) (sub_sh (v1, v2, shft))
#define BUILTIN __builtin_riscv_cv_simd_sub_h
  TEST_HVV_HVV_WC ( 42,  19927,    561,  19391, 1);
  TEST_HVV_HVC_WC (-42,   -561, -19391, -19937, 1);
  TEST_HVV_HVV_WC ( 42,  19927,    561,  19391, 2);
  TEST_HVV_HVC_WC (-42,   -561, -19391, -19937, 2);
  TEST_HVV_HVV_WC ( 42,  19927,    561,  19391, 3);
  TEST_HVV_HVC_WC (-42,   -561, -19391, -19937, 3);
#undef BUILTIN
#undef OP

  return 0;
}

/* { dg-final { scan-assembler-times "cv\\.cplxmul\\.r\[^\\.\]" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.cplxmul\\.r\\.div2" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.cplxmul\\.r\\.div4" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.cplxmul\\.r\\.div8" 1 } } */

/* { dg-final { scan-assembler-times "cv\\.cplxmul\\.i\[^\\.\]" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.cplxmul\\.i\\.div2" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.cplxmul\\.i\\.div4" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.cplxmul\\.i\\.div8" 1 } } */

/* { dg-final { scan-assembler-times "cv\\.cplxconj" 2 } } */

/* { dg-final { scan-assembler-times "cv\\.subrotmj\[^\\.\]" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.subrotmj\\.div2" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.subrotmj\\.div4" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.subrotmj\\.div8" 1 } } */

/* { dg-final { scan-assembler-times "cv\\.add\\.div2" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.add\\.div4" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.add\\.div8" 2 } } */

/* { dg-final { scan-assembler-times "cv\\.sub\\.div2" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.sub\\.div4" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.sub\\.div8" 2 } } */

/* { dg-final { cleanup-saved-temps } } */
