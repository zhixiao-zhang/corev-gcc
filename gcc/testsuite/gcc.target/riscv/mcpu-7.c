/* { dg-do compile } */
/* Verify -mtune has higher priority than -mcpu for pipeline model .  */
/* { dg-options "-mcpu=sifive-s21 -mtune=sifive-u74 -fdump-rtl-sched2-details -march=rv32i -mabi=ilp32" } */
/* We don't generate have the sched2 pass for low optimization levels:
   -O0 -O1 and -Og */
/* { dg-skip-if "" { *-*-* }  { "-O0" "-O1" "-Og" } { "" } } */
/* { dg-final { scan-rtl-dump "simple_return\[ \]+:sifive_7_B" "sched2" } } */

int main()
{
  return 0;
}

