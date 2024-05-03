/* { dg-additional-sources cv-helpers.c } */
/* { dg-do run } */
/* { dg-require-effective-target cv_simd } */
/* We need -save-temps with "dg-do run", in order to save the generated file
   We use "dg-final" with "cleanup-saved-temps" at the end to clean up. */
/* { dg-options "-march=rv32i_xcvsimd -mabi=ilp32 -save-temps" } */

#include "cv-simd-helpers.h"

/* Helper functions. */
static uint16_t
extract_hw (uint32_t v, int idx)
{
  return (v >> (idx * 16)) & 0xffff;
}

static uint8_t
extract_byte (uint32_t v, int idx)
{
  return (v >> (idx * 8)) & 0xff;
}

static uint32_t
insert_hw (uint32_t s, uint32_t v, int idx)
{
  return (v & ~(0xffff << (idx * 16)) | (s & 0xffff) << (idx * 16));
}

static uint32_t
insert_byte (uint32_t s, uint32_t v, int idx)
{
  return (v & ~(0xff << (idx * 8)) | (s & 0xff) << (idx * 8));
}

/* This run-time test does exemplary tests of each of the SIMD bit
   manipulation instructions.
*/
int
main ()
{
  /* SIMD extract variants. */
#define OP(v, i) ((int16_t) extract_hw (v, i))
#define BUILTIN __builtin_riscv_cv_simd_extract_h
  TEST_HVV_WC  (   561, 19391,      0);
  TEST_HVC_WC (-19937,    42,      1);
#undef BUILTIN
#undef OP
#define OP(v, i) ((int8_t) extract_byte (v, i))
#define BUILTIN __builtin_riscv_cv_simd_extract_b
  TEST_BVV_WC  (107, -32,  36, -127, 0);
  TEST_BVV_WC  (107, -32,  36, -127, 1);
  TEST_BVV_WC (107, -32,  36, -127, 2);
  TEST_BVV_WC (107, -32,  36, -127, 3);
#undef BUILTIN
#undef OP

  /* SIMD extractu variants. */
#define OP(v, i) ((uint16_t) extract_hw (v, i))
#define BUILTIN __builtin_riscv_cv_simd_extractu_h
  TEST_HVV_WC (  561U, 19391U,      0);
  TEST_HVC_WC (65521U,    42U,      1);
#undef BUILTIN
#undef OP
#define OP(v, i) ((uint8_t) extract_byte (v, i))
#define BUILTIN __builtin_riscv_cv_simd_extractu_b
  TEST_BVV_WC (107U, 241U, 36U, 255U, 0);
  TEST_BVV_WC (107U, 241U, 36U, 255U, 1);
  TEST_BVV_WC (107U, 241U, 36U, 255U, 2);
  TEST_BVV_WC (107U, 241U, 36U, 255U, 3);
#undef BUILTIN
#undef OP

  /* SIMD insert variants. */
#define OP(s1, v1, i) ((uint32_t) insert_hw (s1, v1, i))
#define BUILTIN __builtin_riscv_cv_simd_insert_h
  TEST_UHSV_HVV_WC (   42U,   561U, 19391U,      0);
  TEST_UHSV_HVC_WC (19937U, 65521U,    42U,      1);
  TEST_UHSC_HVV_WC (65519U,   561U, 19391U,      0);
  TEST_UHSC_HVC_WC (65521U, 65521U,    42U,      1);
#undef BUILTIN
#undef OP
#define OP(s1, v1, i) ((uint32_t) insert_byte (s1, v1, i))
#define BUILTIN __builtin_riscv_cv_simd_insert_b
  TEST_UBSV_BVV_WC ( 28U, 107U, 241U, 36U, 255U, 0);
  TEST_UBSV_BVC_WC ( 72U, 107U, 241U, 36U, 255U, 1);
  TEST_UBSC_BVV_WC (127U, 107U, 241U, 36U, 255U, 2);
  TEST_UBSC_BVC_WC (255U, 107U, 241U, 36U, 255U, 3);
#undef BUILTIN
#undef OP

  return 0;
}

/* { dg-final { scan-assembler-times "cv\\.extract\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.extract\\.b" 4 } } */

/* { dg-final { scan-assembler-times "cv\\.extractu\\.h" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.extractu\\.b" 4 } } */

/* { dg-final { scan-assembler-times "cv\\.insert\\.h" 4 } } */
/* { dg-final { scan-assembler-times "cv\\.insert\\.b" 4 } } */

/* { dg-final { cleanup-saved-temps } } */
