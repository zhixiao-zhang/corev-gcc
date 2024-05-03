/* { dg-do compile } */

/* Helper code that is external to the main tests, so we can handle variables
   which are guaranteed not to be folded in by optimization and functions
   which we don't want inlined, so debugging tests is easier. */

#include <stdint-gcc.h>
#include <stdlib.h>

/* Functions which return their argument, so values can be set opaquely. */
uint32_t
set_uint32 (uint32_t val)
{
  return val;
}

int32_t
set_int32 (int32_t val)
{
  return val;
}

int
set_int (int val)
{
  return val;
}

/* abort if v doesn't hold "good" */
void
validate (uint32_t v, const uint32_t good)
{
  if (v != good)
    abort ();
}
