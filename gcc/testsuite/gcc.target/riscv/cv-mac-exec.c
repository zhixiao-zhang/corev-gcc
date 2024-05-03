/* { dg-additional-sources cv-helpers.c } */
/* { dg-do run } */
/* { dg-require-effective-target cv_mac } */
/* We need -save-temps with "dg-do run", in order to save the generated file
   We use "dg-final" with "cleanup-saved-temps" at the end to clean up. */
/* { dg-options "-march=rv32i_xcvmac -mabi=ilp32 -save-temps" } */

#include "cv-simd-helpers.h"

/* Helper macros and functions */

#define UMUL16(x, i, y, j) (((uint32_t) UHELEM (x, i)) * \
			    ((uint32_t) UHELEM (y, j)))
#define MUL16(x, i, y, j) (((int32_t) HELEM (x, i)) * \
			   ((int32_t) HELEM (y, j)))

static uint32_t
pow2u (uint8_t  n)
{
  if (0 == n)
    return 0;
  else
    return (uint32_t) (1U << (n - 1));
}

static int32_t
pow2 (int8_t  n)
{
  if (0 == n)
    return 0;
  else
    return (int32_t) (1U << (n - 1));
}

/* A set of exemplary run-time tests of each of the immediate branch
   instructions. */

static void
test_mul16 ()
{
  volatile uint32_t x, y;

  x = set_uint32 (MK_HV (42U,  561U));
  y = set_uint32 (MK_HV (28U, 1729U));

  validate (__builtin_riscv_cv_mac_muluN (x, y, 3), UMUL16 (x, 0, y, 0) >> 3);
  validate (__builtin_riscv_cv_mac_mulhhuN (x, y, 5), UMUL16 (x, 1, y, 1) >> 5);

  x = set_uint32 (MK_HV ( 42, -561));
  y = set_uint32 (MK_HV (-28, 1729));

  validate (__builtin_riscv_cv_mac_mulsN (x, y, 3), MUL16 (x, 0, y, 0) >> 3);
  validate (__builtin_riscv_cv_mac_mulhhsN (x, y, 5), MUL16 (x, 1, y, 1) >> 5);

  x = set_uint32 (MK_HV (42U,  561U));
  y = set_uint32 (MK_HV (28U, 1729U));

  validate (__builtin_riscv_cv_mac_muluRN (x, y, 3),
	    (UMUL16 (x, 0, y, 0) + pow2u (3)) >> 3);
  validate (__builtin_riscv_cv_mac_mulhhuRN (x, y, 5),
	    (UMUL16 (x, 1, y, 1) + pow2u (5)) >> 5);

  x = set_uint32 (MK_HV ( 42, -561));
  y = set_uint32 (MK_HV (-28, 1729));

  validate (__builtin_riscv_cv_mac_mulsRN (x, y, 3),
	    (MUL16 (x, 0, y, 0) + pow2 (3)) >> 3);
  validate (__builtin_riscv_cv_mac_mulhhsRN (x, y, 5),
	    (MUL16 (x, 1, y, 1) + pow2 (5)) >> 5);
}

static void
test_mac16 ()
{
  volatile uint32_t x, y, zu;
  volatile int32_t zs;

  x = set_uint32 (MK_HV ( 42U,  561U));
  y = set_uint32 (MK_HV ( 28U, 1729U));
  zu = set_uint32 (496U);

  validate (__builtin_riscv_cv_mac_macuN (x, y, zu, 3),
	    (UMUL16 (x, 0, y, 0) + zu) >> 3);
  validate (__builtin_riscv_cv_mac_machhuN (x, y, zu, 5),
	    (UMUL16 (x, 1, y, 1) + zu) >> 5);

  x = set_uint32 (MK_HV ( 42,  -561));
  y = set_uint32 (MK_HV (-28,  1729));
  zs = set_int32 (-8128);

  validate (__builtin_riscv_cv_mac_macsN (x, y, zs, 3),
	    (MUL16 (x, 0, y, 0) + zs) >> 3);
  validate (__builtin_riscv_cv_mac_machhsN (x, y, zs, 5),
	    (MUL16 (x, 1, y, 1) + zs) >> 5);

  x = set_uint32 (MK_HV (42U,  561U));
  y = set_uint32 (MK_HV (28U, 1729U));
  zu = set_uint32 (496U);

  validate (__builtin_riscv_cv_mac_macuRN (x, y, zu, 3),
	    (UMUL16 (x, 0, y, 0) + zu + pow2u (3)) >> 3);
  validate (__builtin_riscv_cv_mac_machhuRN (x, y, zu, 5),
	    (UMUL16 (x, 1, y, 1) + zu + pow2u (5)) >> 5);

  x = set_uint32 (MK_HV ( 42, -561));
  y = set_uint32 (MK_HV (-28, 1729));
  zs = set_int32 (-8128);

  validate (__builtin_riscv_cv_mac_macsRN (x, y, zs, 3),
	    (MUL16 (x, 0, y, 0) + zs + pow2 (3)) >> 3);
  validate (__builtin_riscv_cv_mac_machhsRN (x, y, zs, 5),
	    (MUL16 (x, 1, y, 1) + zs+ pow2 (5)) >> 5);
}

static void
test_mac32 ()
{
  volatile int32_t x, y, z;

  x = set_int32 (142857);
  y = set_int32 (-496);
  z = set_int32 (3355036);

  validate (__builtin_riscv_cv_mac_mac (x, y, z), z + x * y);
  validate (__builtin_riscv_cv_mac_msu (x, y, z), z - x * y);
}

int
main ()
{
  test_mul16 ();
  test_mac16 ();
  test_mac32 ();

  return 0;
}

/* { dg-final { scan-assembler-times "cv\\.mulun" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.mulhhun" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.mulsn" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.mulhhsn" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.mulurn" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.mulhhurn" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.mulsrn" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.mulhhsrn" 1 } } */

/* { dg-final { scan-assembler-times "cv\\.macun" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.machhun" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.macsn" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.machhsn" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.macurn" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.machhurn" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.macsrn" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.machhsrn" 1 } } */

/* { dg-final { scan-assembler-times "cv\\.mac\[^hsu\]" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.msu" 1 } } */

/* { dg-final { cleanup-saved-temps } } */
