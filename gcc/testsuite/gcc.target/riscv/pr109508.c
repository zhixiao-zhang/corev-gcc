/* { dg-do compile } */
/* { dg-require-effective-target rv64 }
/* { dg-options "-mcpu=sifive-s76" } */

typedef char __attribute__((__vector_size__ (1))) V;

V v;

void
foo (void)
{
  (char) __builtin_shuffle (0 % v, (V){6}, v);
}
