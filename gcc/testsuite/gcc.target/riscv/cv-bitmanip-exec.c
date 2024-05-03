/* { dg-additional-sources cv-helpers.c } */
/* { dg-do run } */
/* { dg-require-effective-target cv_bitmanip } */
/* We need -save-temps with "dg-do run", in order to save the generated file
   We use "dg-final" with "cleanup-saved-temps" at the end to clean up. */
/* { dg-options "-march=rv32i_xcvbitmanip -mabi=ilp32 -save-temps" } */

#include "cv-helpers.h"

/* Helper macros & functions. */

#define RANGE(is2, is3) (((is3 & 0x1f) << 5) | (is2 & 0x1f))

/* Reverse to base 2 from the Aggregate Magic Algorithms
   (http://aggregate.org/MAGIC) */
static uint32_t
rev2 (uint32_t x)
{
  x = ((x & 0xaaaaaaaa) >> 1) | ((x & 0x55555555) << 1);
  x = ((x & 0xcccccccc) >> 2) | ((x & 0x33333333) << 2);
  x = ((x & 0xf0f0f0f0) >> 4) | ((x & 0x0f0f0f0f) << 4);
  x = ((x & 0xff00ff00) >> 8) | ((x & 0x00ff00ff) << 8);
  return (x >> 16) | (x << 16);
}

static uint32_t
rev4 (uint32_t x)
{
  x = ((x & 0xcccccccc) >> 2) | ((x & 0x33333333) << 2);
  x = ((x & 0xf0f0f0f0) >> 4) | ((x & 0x0f0f0f0f) << 4);
  x = ((x & 0xff00ff00) >> 8) | ((x & 0x00ff00ff) << 8);
  return (x >> 16) | (x << 16);
}

static uint32_t
rev8 (uint32_t x)
{
  x = ((x & 0xe38e38e0) >>  3) | ((x & 0x1c71c71c) <<  3);
  uint32_t mid = x & 0x000fc000;
  x = ((x & 0xfc003f00) >>  6) | ((x & 0x03f000fc) <<  6);
  x = ((x & 0xfff00000) >> 18) | ((x & 0x00003ffc) << 18);
  return (x | mid) >> 2;
}

static uint32_t
bitrev (uint32_t i, const uint8_t pts, const uint8_t radix)
{
  switch (radix)
    {
    case 0:
    case 3: return rev2 (i << pts);
    case 1: return rev4 (i << pts);
    case 2: return rev8 (i << pts);
    }
}

static uint32_t
get_bits (uint32_t val, int offset, int len)
{
  return (val >> offset) & (0xffffffff >> (31 - len));
}

static uint32_t
set_bits (uint32_t val, int offset, int len, uint32_t r)
{
  uint32_t mask = 0xffffffff >> (31 - len);
  return (r & ~(mask << offset)) | ((val & mask) << offset);
}

/* Sign extension from the Aggregate Magic Algorithms
   (http://aggregate.org/MAGIC) */
static uint32_t
sext (uint32_t  x, uint32_t bitpos)
{
  uint32_t n = 1 << bitpos;
  return (x ^ n) - n;
}

static uint32_t
zext (uint32_t  x, uint32_t bitpos)
{
  uint32_t n = 0xffffffff >> (31 - bitpos);
  return x & n;
}

/* Ones count from the Aggregate Magic Algorithms
   (http://aggregate.org/MAGIC) */
static uint32_t
ones (uint32_t x)
{
  x -= (x >> 1) & 0x55555555;
  x  = ((x >> 2) & 0x33333333) + (x & 0x33333333);
  x  = ((x >> 4) + x) & 0x0f0f0f0f;
  x += x >> 8;
  x += x >> 16;

  return x & 0x0000003f;
}

/* Leading zero count from the Aggregate Magic Algorithms
   (http://aggregate.org/MAGIC) */
static uint32_t
lzc (uint32_t x)
{
  x |= x >>  1;
  x |= x >>  2;
  x |= x >>  4;
  x |= x >>  8;
  x |= x >> 16;
  return 32 - ones (x);
}

static uint32_t
clb (uint32_t x)
{
  if (x == 0)
    return 0;

  uint32_t cnt0 = lzc (x);
  if ( cnt0 != 0)
    return cnt0 - 1;
  else
    return lzc (~x) - 1;
}

/* A set of exemplary run-time tests of each of the bit manipulation
   instructions. */

#define TEST_BITREV(i, pts, radix)					\
  validate (bitrev (i, pts, radix),					\
	    __builtin_riscv_cv_bitmanip_bitrev (i, pts, radix))

void
test_bitrev ()
{
  volatile uint32_t src = set_uint32 (0xc64a5933);

  TEST_BITREV (src, 4, 0);
  TEST_BITREV (src, 4, 1);
  TEST_BITREV (src, 4, 2);
  TEST_BITREV (src, 4, 3);
}

#define TEST_EXTRACT(val, is2, is3)					 \
  validate (__builtin_riscv_cv_bitmanip_extract (val, RANGE (is2, is3)), \
	    sext (get_bits (val, is2, is3), is3))
#define TEST_EXTRACTU(val, is2, is3)					\
  validate (__builtin_riscv_cv_bitmanip_extractu (val, RANGE (is2, is3)), \
	    zext (get_bits (val, is2, is3), is3))

void
test_extract ()
{
  volatile uint32_t src = set_uint32 (0xc64a5933);
  volatile uint32_t len1 = set_uint32 (11);
  volatile uint32_t len2 = set_uint32 (10);

  TEST_EXTRACT (src, 5, len1);
  TEST_EXTRACT (src, 5, len2);
  TEST_EXTRACT (src, 5, 9);
  TEST_EXTRACT (src, 5, 10);

  src = 0xc64a5933;
  len1 = 11;
  len2 = 10;

  TEST_EXTRACTU (src, 5, len1);
  TEST_EXTRACTU (src, 5, len2);
  TEST_EXTRACTU (src, 5, 9);
  TEST_EXTRACTU (src, 5, 10);
}

#define TEST_INSERT(val, is2, is3, res)					     \
  validate (__builtin_riscv_cv_bitmanip_insert (val, RANGE (is2, is3), res), \
	    set_bits (val, is2, is3, res))

void
test_insert ()
{
  volatile uint32_t src = set_uint32 (0xc64a5933);
  volatile uint32_t len1 = set_uint32 (11);
  volatile uint32_t len2 = set_uint32 (10);
  volatile uint32_t res;

  res = set_uint32 (0x216b244b);
  TEST_INSERT (src, 5, len1, res);
  res = set_uint32 (0x216b244b);
  TEST_INSERT (src, 5, len2, res);
  res = set_uint32 (0x216b244b);
  TEST_INSERT (src, 5, len1, 0x216b244b);
  res = set_uint32 (0x216b244b);
  TEST_INSERT (src, 5, len2, 0x216b244b);
  res = set_uint32 (0x216b244b);
  TEST_INSERT (src, 5, 9, res);
  res = set_uint32 (0x216b244b);
  TEST_INSERT (src, 5, 10, res);
  res = set_uint32 (0x216b244b);
  TEST_INSERT (src, 5, 9, 0x216b244b);
  res = set_uint32 (0x216b244b);
  TEST_INSERT (src, 5, 10, 0x216b244b);
}

#define TEST_BCLR(val, is2, is3)					\
  validate (__builtin_riscv_cv_bitmanip_bclr (val, RANGE (is2, is3)),	\
	    val & ~((0xffffffff >> (31 - is3)) << is2))

void
test_bclr ()
{
  volatile uint32_t src = set_uint32 (0xc64a5933);
  volatile uint32_t len1 = set_uint32 (11);
  volatile uint32_t len2 = set_uint32 (10);

  TEST_BCLR (src, 5, len1);
  TEST_BCLR (src, 5, len2);
  TEST_BCLR (src, 5, 11);
  TEST_BCLR (src, 5, 10);
}

#define TEST_BSET(val, is2, is3)					\
  validate (__builtin_riscv_cv_bitmanip_bset (val, RANGE (is2, is3)),	\
	    val | ((0xffffffff >> (31 - is3)) << is2))

void
test_bset ()
{
  volatile uint32_t src = set_uint32 (0xc64a5933);
  volatile uint32_t len1 = set_uint32 (11);
  volatile uint32_t len2 = set_uint32 (10);

  TEST_BSET (src, 5, len1);
  TEST_BSET (src, 5, len2);
  TEST_BSET (src, 5, 11);
  TEST_BSET (src, 5, 10);
}

void
test_bit_info ()
{
  volatile uint32_t src = set_uint32 (0xc64a5933);

  validate (__builtin_riscv_cv_bitmanip_ff1 (src), ones ((src & -src) - 1));
  validate (__builtin_riscv_cv_bitmanip_fl1 (src), 31 - lzc (src));
  validate (__builtin_riscv_cv_bitmanip_clb (src), clb (src));
  validate (__builtin_riscv_cv_bitmanip_cnt (src), ones (src));
}

void
test_ror ()
{
  volatile uint32_t src = set_uint32 (0xc64a5933);
  volatile uint32_t rot = 7;

  validate (__builtin_riscv_cv_bitmanip_ror (src, rot),
	    (src >> rot) | (src << (32 -rot)));
}

int
main ()
{
  test_bitrev ();

  test_extract ();
  test_insert ();

  test_bclr ();
  test_bset ();

  test_bit_info ();

  test_ror ();

  return 0;
}

/* { dg-final { scan-assembler-times "cv\\.bitrev" 4 } } */

/* { dg-final { scan-assembler-times "cv\\.extract\[^ur\]" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.extractr" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.extractu\[^r\]" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.extractur" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.insert\[^r\]" 4 } } */
/* { dg-final { scan-assembler-times "cv\\.insertr" 4 } } */

/* Need to scan for just bclr */
/* { dg-final { scan-assembler-times "cv\\.bclrr" 2 } } */
/* Need to scan for just bset */
/* { dg-final { scan-assembler-times "cv\\.bsetr" 2 } } */

/* { dg-final { scan-assembler-times "cv\\.ff1" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.fl1" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.clb" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.cnt" 1 } } */

/* { dg-final { scan-assembler-times "cv\\.ror" 1 } } */

/* { dg-final { cleanup-saved-temps } } */
