#ifndef CV_HELPERS_H
#define CV_HELPERS_H

#include <stdint-gcc.h>
#include <stdlib.h>

extern uint32_t set_uint32 (uint32_t val);
extern int set_int32 (int val);
extern int set_int (int val);
extern void validate (uint32_t v, const uint32_t good);

#endif	/* CV_SIMD_HELPERS_H */
