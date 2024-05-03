/* { dg-additional-sources cv-helpers.c } */
/* { dg-do run } */
/* { dg-require-effective-target cv_bi } */
/* We need -save-temps with "dg-do run", in order to save the generated file
   We use "dg-final" with "cleanup-saved-temps" at the end to clean up. */
/* { dg-skip-if "" { *-*-* }  { "-O0" "-Og" } { "" } } */
/* { dg-options "-march=rv32i_xcvbi -mabi=ilp32 -save-temps" } */

#include "cv-helpers.h"

/* A set of exemplary run-time tests of each of the immediate branch
   instructions. */

int
foo_beqimm (int a, int x, int y)
{
  a = __builtin_expect (a, 12);
  return a != 10 ? x : y;
}

int
foo_bneimm(int a, int x, int y)
{
  a = __builtin_expect(a, 10);
  return a == 10 ? x : y;
}

int
main ()
{
  volatile int a = set_int (28);
  volatile int x = set_int (42);
  volatile int y = set_int (561);

  validate (foo_beqimm (a, x, y), 42);
  validate (foo_bneimm (a, x, y), 561);
}

/* { dg-final { scan-assembler-times "cv\\.beqimm" 2 } } */
/* { dg-final { scan-assembler-times "cv\\.bneimm" 2 } } */

/* { dg-final { cleanup-saved-temps } } */
