/* { dg-additional-sources cv-helpers.c } */
/* { dg-do run } */
/* { dg-require-effective-target cv_mem } */
/* We need -save-temps with "dg-do run", in order to save the generated file
   We use "dg-final" with "cleanup-saved-temps" at the end to clean up. */
/* { dg-options "-march=rv32i_xcvmem -mabi=ilp32 -save-temps" } */
/* { dg-skip-if "" { *-*-* } { "*" } { "-O1" } } */

#include "cv-helpers.h"

/* A set of exemplary run-time tests of each of the post-increment
   instructions. Naming of the functions is as follows

      Register-immediate with post-inc: <direction><signage><size>-ri-pi
      Register-register with post-inc:  <direction><signage><size>-rr-pi
      Register-register:                <direction><signage><size>-rr

   Where:

      direction is either "l" (load) or "s" (store)
      signage is either "u" (unsigned) or omitted (signed)
      size is either "b" (byte), "h" (half-word) or "w" (full word).
*/

int __attribute__ ((noinline))
lb_ri_pi (int8_t* a, int n)
{
  int sum = 1;

  for (int i = 0; i < n; i++)
    sum += a [i];

  return sum;
}

int __attribute__ ((noinline))
lb_rr_pi (int8_t* a, int j)
{
  int sum = 1;

  for (int i = 0; i < 4; i += j)
  {
    sum += *a;
    a += j * sizeof (a);
  }

  return sum;
}

int __attribute__ ((noinline))
lb_rr (int8_t* a, int i, int j)
{
  return a [i + j];
}

int __attribute__ ((noinline))
lbu_ri_pi (uint8_t* a, int n)
{
  int sum = 1;

  for (int i = 0; i < n; i++)
    sum += a [i];

  return sum;
}

int __attribute__ ((noinline))
lbu_rr_pi (uint8_t* a, int j)
{
  int sum = 1;

  for (int i = 0; i < 4; i += j)
  {
    sum += *a;
    a += j * sizeof (a);
  }

  return sum;
}

int __attribute__ ((noinline))
lbu_rr (uint8_t* a, int i, int j)
{
  return a [i + j];
}

int __attribute__ ((noinline))
lh_ri_pi (int16_t* a, int n)
{
  int sum = 1;

  for (int i = 0; i < n; i++)
    sum += a [i];

  return sum;
}

int __attribute__ ((noinline))
lh_rr_pi (int16_t* a, int j)
{
  int sum = 1;

  for (int i = 0; i < 4; i += j)
  {
    sum += *a;
    a += j * sizeof (a);
  }

  return sum;
}

int __attribute__ ((noinline))
lh_rr (int16_t* a, int i, int j)
{
  return a [i + j];
}

int __attribute__ ((noinline))
lhu_ri_pi (uint16_t* a, int n)
{
  int sum = 1;

  for (int i = 0; i < n; i++)
    sum += a [i];

  return sum;
}

int __attribute__ ((noinline))
lhu_rr_pi (uint16_t* a, int j)
{
  int sum = 1;

  for (int i = 0; i < 4; i += j)
  {
    sum += *a;
    a += j * sizeof (a);
  }

  return sum;
}

int __attribute__ ((noinline))
lhu_rr (uint16_t* a, int i, int j)
{
  return a [i + j];
}

int __attribute__ ((noinline))
lwu_ri_pi (uint32_t* a, int n)
{
  int sum = 1;

  for (int i = 0; i < n; i++)
    sum += a [i];

  return sum;
}

int __attribute__ ((noinline))
lwu_rr_pi (uint32_t* a, int j)
{
  int sum = 1;

  for (int i = 0; i < 4; i += j)
  {
    sum += *a;
    a += j * sizeof (a);
  }

  return sum;
}

int __attribute__ ((noinline))
lwu_rr (uint32_t* a, int i, int j)
{
  return a [i + j];
}

int
sb_ri_pi (int8_t *a, int n)
{
  int addend = 1;

  for (int i = 0; i < n; i++)
    a [i] += addend;

  return addend + a[n - 1];
}

int
sb_rr_pi (int8_t* a, int n, int j)
{
  int addend = 1;

  for (int i = 0; i < n; i += j)
  {
    *a += addend;
    a += j * sizeof (a);
  }

  return addend + *(a - j  * sizeof (a));
}

int
sb_rr (int8_t* a, int i, int j)
{
  int addend = 1;

  a [i + j] += addend;

  return addend + a[i + j];
}

int
sbu_ri_pi (uint8_t* a, int n)
{
  int addend = 1;

  for (int i = 0; i < n; i++)
    a [i] += addend;

  return addend + a[n - 1];
}

int
sbu_rr_pi (uint8_t* a, int n, int j)
{
  int addend = 1;

  for (int i = 0; i < n; i += j)
  {
    *a += addend;
    a += j * sizeof (a);
  }

  return addend + *(a - j  * sizeof (a));
}

int
sbu_rr (uint8_t* a, int i, int j)
{
  int addend = 1;

  a [i + j] += addend;

  return addend + a[i + j];
}

int
sh_ri_pi (int16_t *a, int n)
{
  int addend = 1;

  for (int i = 0; i < n; i++)
    a [i] += addend;

  return addend + a[n - 1];
}

int
sh_rr_pi (int16_t* a, int n, int j)
{
  int addend = 1;

  for (int i = 0; i < n; i += j)
  {
    *a += addend;
    a += j * sizeof (a);
  }

  return addend + *(a - j  * sizeof (a));
}

int
sh_rr (int16_t* a, int i, int j)
{
  int addend = 1;

  a [i + j] += addend;

  return addend + a[i + j];
}

int
shu_ri_pi (uint16_t* a, int n)
{
  int addend = 1;

  for (int i = 0; i < n; i++)
    a [i] += addend;

  return addend + a[n - 1];
}

int
shu_rr_pi (uint16_t* a, int n, int j)
{
  int addend = 1;

  for (int i = 0; i < n; i += j)
  {
    *a += addend;
    a += j * sizeof (a);
  }

  return addend + *(a - j  * sizeof (a));
}

int
shu_rr (uint16_t* a, int i, int j)
{
  int addend = 1;

  a [i + j] += addend;

  return addend + a[i + j];
}

int
sw_ri_pi (int32_t *a, int n)
{
  int addend = 1;

  for (int i = 0; i < n; i++)
    a [i] += addend;

  return addend + a[n - 1];
}

int
sw_rr_pi (int32_t* a, int n, int j)
{
  int addend = 1;

  for (int i = 0; i < n; i += j)
  {
    *a += addend;
    a += j * sizeof (a);
  }

  return addend + *(a - j  * sizeof (a));
}

int
sw_rr (int32_t* a, int i, int j)
{
  int addend = 1;

  a [i + j] += addend;

  return addend + a[i + j];
}

int
swu_ri_pi (uint32_t* a, int n)
{
  int addend = 1;

  for (int i = 0; i < n; i++)
    a [i] += addend;

  return addend + a[n - 1];
}

int
swu_rr_pi (uint32_t* a, int n, int j)
{
  int addend = 1;

  for (int i = 0; i < n; i += j)
  {
    *a += addend;
    a += j * sizeof (a);
  }

  return addend + *(a - j  * sizeof (a));
}

int
swu_rr (uint32_t* a, int i, int j)
{
  int addend = 1;

  a [i + j] += addend;

  return addend + a[i + j];
}

int
main ()
{
  int8_t data_i8 [16] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53
  };
  uint8_t data_ui8 [12] = {
    1U, 1U, 2U, 3U, 5U, 8U, 13U, 21U, 34U, 55U, 89U, 144U
  };
  int16_t data_i16 [16] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53
  };
  uint16_t data_ui16 [12] = {
    1U, 1U, 2U, 3U, 5U, 8U, 13U, 21U, 34U, 55U, 89U, 144U
  };
  int32_t data_i32 [16] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53
  };
  uint32_t data_ui32 [12] = {
    1U, 1U, 2U, 3U, 5U, 8U, 13U, 21U, 34U, 55U, 89U, 144U
  };

  validate (lb_ri_pi (data_i8, 16), 382);
  validate (lb_rr_pi (data_i8, 2), 26);
  validate (lb_rr (data_i8, 3, 4), 19);

  validate (lbu_ri_pi (data_ui8, 12), 377U);
  validate (lbu_rr_pi (data_ui8, 2), 36U);
  validate (lbu_rr (data_ui8, 3, 4), 21U);

  validate (lh_ri_pi (data_i16, 16), 382);
  validate (lh_rr_pi (data_i16, 2), 26);
  validate (lh_rr (data_i16, 3, 4), 19);

  validate (lhu_ri_pi (data_ui16, 12), 377U);
  validate (lhu_rr_pi (data_ui16, 2), 36U);
  validate (lhu_rr (data_ui16, 3, 4), 21U);

  validate (lwu_ri_pi (data_ui32, 12), 377U);
  validate (lwu_rr_pi (data_ui32, 2), 36U);
  validate (lwu_rr (data_ui32, 3, 4), 21U);

  validate (sb_ri_pi (data_i8, 16), 55);
  validate (sb_rr_pi (data_i8, 4, 2), 26);
  validate (sb_rr (data_i8, 3, 4), 22);

  validate (sbu_ri_pi (data_ui8, 12), 146U);
  validate (sbu_rr_pi (data_ui8, 4, 2), 37U);
  validate (sbu_rr (data_ui8, 3, 4), 24U);

  validate (sh_ri_pi (data_i16, 16), 55);
  validate (sh_rr_pi (data_i16, 4, 2), 26);
  validate (sh_rr (data_i16, 3, 4), 22);

  validate (shu_ri_pi (data_ui16, 12), 146U);
  validate (shu_rr_pi (data_ui16, 4, 2), 37U);
  validate (shu_rr (data_ui16, 3, 4), 24U);

  validate (sw_ri_pi (data_i32, 16), 55);
  validate (sw_rr_pi (data_i32, 4, 2), 26);
  validate (sw_rr (data_i32, 3, 4), 22);

  validate (swu_ri_pi (data_ui32, 12), 146U);
  validate (swu_rr_pi (data_ui32, 4, 2), 37U);
  validate (swu_rr (data_ui32, 3, 4), 24U);

  return 0;
}


/* { dg-final { scan-assembler-times "cv\\.lb\t\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\),\\(\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)\\),1" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.lb\t\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\),\\(\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)\\),\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.lb\t\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\),(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)\\(\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)\\)" 1 } } */

/* { dg-final { scan-assembler-times "cv\\.lbu\t\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\),\\(\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)\\),1" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.lbu\t\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\),\\(\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)\\),\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.lbu\t\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\),(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)\\(\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)\\)" 3 } } */

/* { dg-final { scan-assembler-times "cv\\.lh\t\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\),\\(\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)\\),2" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.lh\t\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\),\\(\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)\\),\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.lh\t\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\),(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)\\(\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)\\)" 2 } } */

/* { dg-final { scan-assembler-times "cv\\.lhu\t\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\),\\(\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)\\),2" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.lhu\t\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\),\\(\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)\\),\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.lhu\t\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\),(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)\\(\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)\\)" 4 } } */

/* { dg-final { scan-assembler-times "cv\\.lw\t\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\),\\(\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)\\),4" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.lw\t\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\),\\(\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)\\),\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)" 1 } } */
/* { dg-final { scan-assembler-times "cv\\.lw\t\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\),(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)\\(\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)\\)" 5 } } */

/* { dg-final { scan-assembler-times "cv\\.sb\t\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\),\\(\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)\\),1" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.sb\t\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\),\\(\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)\\),\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.sb\t\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\),(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)\\(\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)\\)" 2 } } */

/* { dg-final { scan-assembler-times "cv\\.sh\t\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\),\\(\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)\\),2" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.sh\t\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\),\\(\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)\\),\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.sh\t\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\),(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)\\(\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)\\)" 2 } } */

/* { dg-final { scan-assembler-times "cv\\.sw\t\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\),\\(\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)\\),4" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.sw\t\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\),\\(\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)\\),\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.sw\t\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\),(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)\\(\(\?\:t\[0-6\]\|a\[0-7\]\|s\[1-11\]\)\\)" 2 } } */

/* { dg-final { cleanup-saved-temps } } */
