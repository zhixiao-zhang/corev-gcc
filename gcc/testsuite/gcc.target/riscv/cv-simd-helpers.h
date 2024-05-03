#ifndef CV_SIMD_HELPERS_H
#define CV_SIMD_HELPERS_H

#include "cv-helpers.h"

/* Macros to create signed and unsigned vectors from half-word and byte
   chunks. */

#define MK_HV(h1, h2) ((uint32_t) (((((uint32_t) h1) & 0xffff) << 16) |	\
				   ((((uint32_t) h2) & 0xffff) << 0)))
#define MK_BV(b1, b2, b3, b4) ((uint32_t) (((((uint32_t) b1) & 0xff) << 24) | \
					   ((((uint32_t) b2) & 0xff) << 16) | \
					   ((((uint32_t) b3) & 0xff) <<  8) | \
					   ((((uint32_t) b4) & 0xff) <<  0)))

/* Macros to extract elements from vectors */
#define HELEM(hv, i)  ((int16_t)  ((hv >> (i * 16)) & 0xffff))
#define UHELEM(hv, i) ((uint16_t) ((hv >> (i * 16)) & 0xffff))
#define BELEM(bv, i)  ((int8_t)  ((bv >> (i * 8)) & 0xff))
#define UBELEM(bv, i) ((uint8_t) ((bv >> (i * 8)) & 0xff))

/*******************************************************************************/
/*                                                                             */
/* Helper macros to run tests.                                                 */
/*                                                                             */
/* The naming indicates the type of each argument with 1-4 characters:         */
/* - [U]       - indicates unsigned. If missing argument is signed.            */
/* - W | H | B - indicates size (word, half-word, byte)                        */
/* - [V | S]   - indicates vector or scalar, omit if don't care.               */
/* - [V | C]   - indicates variable or literal constant, omit if don't care.   */
/*                                                                             */
/* The user must define the BUILTIN macro to name the builtin, and the OP      */
/* macro to compute the result on the host                                     */
/*                                                                             */
/*******************************************************************************/

/* Test builtins with single vector argument */
#define TEST_HVV(h1, h2) {	            \
    volatile uint32_t hv1 = MK_HV (h1, h2); \
    uint32_t r = OP (hv1);	            \
    validate (BUILTIN (hv1), r);	    \
  }

#define TEST_HVC(h1, h2) {	            \
    uint32_t r = OP (MK_HV (h1, h2));	    \
    validate (BUILTIN (MK_HV (h1, h2)), r); \
  }

#define TEST_BVV(b1, b2, b3, b4) {	            \
    volatile uint32_t bv1 = MK_BV (b1, b2, b3, b4); \
    uint32_t r = OP (bv1);                          \
    validate (BUILTIN (bv1), r);		    \
  }

#define TEST_BVC(b1, b2, b3, b4) {	            \
    uint32_t r = OP (MK_BV (b1, b2, b3, b4));       \
    validate (BUILTIN (MK_BV (b1, b2, b3, b4)), r); \
}

/* Test builtins with vector and constant */
#define TEST_HVV_WC(h1, h2, c) {	    \
    volatile uint32_t hv1 = MK_HV (h1, h2); \
    uint32_t r = OP (hv1, c);		    \
    validate (BUILTIN (hv1, c), r);	    \
  }
#define TEST_HVC_WC(h1, h2, c) {	        \
    uint32_t r = OP (MK_HV (h1, h2), c);       \
    validate (BUILTIN (MK_HV (h1, h2), c), r); \
  }
#define TEST_BVV_WC(b1, b2, b3, b4, c) {           \
    volatile uint32_t bv1 = MK_BV (b1, b2, b3, b4); \
    uint32_t r = OP (bv1, c);		            \
    validate (BUILTIN (bv1, c), r);	            \
  }
#define TEST_BVC_WC(b1, b2, b3, b4, c) {	        \
    uint32_t r = OP (MK_BV (b1, b2, b3, b4), c);       \
    validate (BUILTIN (MK_BV (b1, b2, b3, b4), c), r); \
  }

/* Macros to test builtins with two vectors */
#define TEST_HVV_HVV(h1, h2, h3, h4) {	     \
    volatile uint32_t hv1 = MK_HV (h1, h2);  \
    volatile uint32_t hv2 =  MK_HV (h3, h4); \
    uint32_t r = OP (hv1, hv2);              \
    validate (BUILTIN (hv1, hv2), r);	     \
  }

#define TEST_HVV_HVC(h1, h2, h3, h4) {	         \
    volatile uint32_t hv1 = MK_HV (h1, h2);	 \
    uint32_t r = OP (hv1, MK_HV (h3, h4));       \
    validate (BUILTIN (hv1, MK_HV (h3, h4)), r); \
  }

#define TEST_BVV_BVV(b1, b2, b3, b4, b5, b6, b7, b8) { \
    volatile uint32_t bv1 = MK_BV (b1, b2, b3, b4);    \
    volatile uint32_t bv2 = MK_BV (b5, b6, b7, b8);    \
    uint32_t r = OP (bv1, bv2);			       \
    validate (BUILTIN (bv1, bv2), r);		       \
  }

#define TEST_BVV_BVC(b1, b2, b3, b4, b5, b6, b7, b8) {   \
    volatile uint32_t bv1 = MK_BV (b1, b2, b3, b4);      \
    uint32_t r = OP (bv1, MK_BV (b5, b6, b7, b8));	 \
    validate (BUILTIN (bv1, MK_BV (b5, b6, b7, b8)), r); \
}

/* Macros to test builtins with a vector and a scalar */
#define TEST_HVV_HSV(h1, h2, s1) {	    \
    volatile uint32_t hv1 = MK_HV (h1, h2); \
    volatile int16_t  hs1 = s1;		    \
    uint32_t r = OP (hv1, s1);              \
    validate (BUILTIN (hv1, hs1), r);	    \
  }

#define TEST_HVV_UHSV(h1, h2, s1) {	    \
    volatile uint32_t hv1 = MK_HV (h1, h2); \
    volatile uint16_t hs1 = s1;		    \
    uint32_t r = OP (hv1, s1);              \
    validate (BUILTIN (hv1, hs1), r);	    \
  }

#define TEST_HVV_HSC(h1, h2, s1) {	    \
    volatile uint32_t hv1 = MK_HV (h1, h2); \
    uint32_t r = OP (hv1, s1);              \
    validate (BUILTIN (hv1, s1), r);	    \
  }

#define TEST_BVV_BSV(b1, b2, b3, b4, s1) {	    \
    volatile uint32_t bv1 = MK_BV (b1, b2, b3, b4); \
    volatile int8_t   bs1 = s1;			    \
    uint32_t r = OP (bv1, s1);                      \
    validate (BUILTIN (bv1, bs1), r);		    \
  }

#define TEST_BVV_UBSV(b1, b2, b3, b4, s1) {	    \
    volatile uint32_t bv1 = MK_BV (b1, b2, b3, b4); \
    volatile uint8_t  bs1 = s1;			    \
    uint32_t r = OP (bv1, s1);                      \
    validate (BUILTIN (bv1, bs1), r);		    \
  }

#define TEST_BVV_BSC(b1, b2, b3, b4, s1) {	    \
    volatile uint32_t bv1 = MK_BV (b1, b2, b3, b4); \
    uint32_t r = OP (bv1, s1);                      \
    validate (BUILTIN (bv1, s1), r);		    \
  }

/* Macros to test builtins with two vectors and a constant */
#define TEST_HVV_HVV_WC(h1, h2, h3, h4, c1) { \
    volatile uint32_t hv1 = MK_HV (h1, h2);   \
    volatile uint32_t hv2 =  MK_HV (h3, h4);  \
    uint32_t r = OP (hv1, hv2, c1);           \
    validate (BUILTIN (hv1, hv2, c1), r);     \
  }

#define TEST_HVV_HVC_WC(h1, h2, h3, h4, c1) {        \
    volatile uint32_t hv1 = MK_HV (h1, h2);          \
    uint32_t r = OP (hv1, MK_HV (h3, h4), c1);	     \
    validate (BUILTIN (hv1, MK_HV (h3, h4), c1), r); \
  }

/* Test builtins with unsigned scalar, vector and constant */
#define TEST_UHSV_HVV_WC(s1, h1, h2, c1) {  \
    volatile uint16_t s1var = s1;           \
    volatile uint32_t hv1 = MK_HV (h1, h2); \
    uint32_t r = OP (s1, hv1, c1);	    \
    validate (BUILTIN (s1var, hv1, c1), r); \
  }
#define TEST_UHSV_HVC_WC(s1, h1, h2, c1) {	       \
    volatile uint16_t s1var = s1;                      \
    uint32_t r = OP (s1, MK_HV (h1, h2), c1);	       \
    validate (BUILTIN (s1var, MK_HV (h1, h2), c1), r); \
  }
#define TEST_UHSC_HVV_WC(s1, h1, h2, c1) {  \
    volatile uint32_t hv1 = MK_HV (h1, h2); \
    uint32_t r = OP (s1, hv1, c1);	    \
    validate (BUILTIN (s1, hv1, c1), r);    \
  }
#define TEST_UHSC_HVC_WC(s1, h1, h2, c1) {	    \
    uint32_t r = OP (s1, MK_HV (h1, h2), c1);	    \
    validate (BUILTIN (s1, MK_HV (h1, h2), c1), r); \
  }
#define TEST_UBSV_BVV_WC(s1, b1, b2, b3, b4, c1) {  \
    volatile uint16_t s1var = s1;           \
    volatile uint32_t bv1 = MK_BV (b1, b2, b3, b4); \
    uint32_t r = OP (s1, bv1, c1);	    \
    validate (BUILTIN (s1var, bv1, c1), r); \
  }
#define TEST_UBSV_BVC_WC(s1, b1, b2, b3, b4, c1) {	       \
    volatile uint16_t s1var = s1;                      \
    uint32_t r = OP (s1, MK_BV (b1, b2, b3, b4), c1);	       \
    validate (BUILTIN (s1var, MK_BV (b1, b2, b3, b4), c1), r); \
  }
#define TEST_UBSC_BVV_WC(s1, b1, b2, b3, b4, c1) {  \
    volatile uint32_t bv1 = MK_BV (b1, b2, b3, b4); \
    uint32_t r = OP (s1, bv1, c1);	    \
    validate (BUILTIN (s1, bv1, c1), r);    \
  }
#define TEST_UBSC_BVC_WC(s1, b1, b2, b3, b4, c1) {	    \
    uint32_t r = OP (s1, MK_BV (b1, b2, b3, b4), c1);	    \
    validate (BUILTIN (s1, MK_BV (b1, b2, b3, b4), c1), r); \
  }

/* Macros to test builtins with two vectors and a variable or constant*/
#define TEST_HVV_HVV_W(h1, h2, h3, h4, w) { \
    volatile uint32_t hv1 = MK_HV (h1, h2); \
    volatile uint32_t hv2 = MK_HV (h3, h4); \
    uint32_t r = OP (hv1, hv2, w);	    \
    validate (BUILTIN (hv1, hv2, w), r);    \
  }

#define TEST_HVV_HVC_W(h1, h2, h3, h4, w) {	    \
    volatile uint32_t hv1 = MK_HV (h1, h2);	    \
    uint32_t r = OP (hv1, MK_HV (h3, h4), w);	    \
    validate (BUILTIN (hv1, MK_HV (h3, h4), w), r); \
  }

#define TEST_BVV_BVV_W(b1, b2, b3, b4, b5, b6, b7, b8, w) { \
    volatile uint32_t bv1 = MK_BV (b1, b2, b3, b4);         \
    volatile uint32_t bv2 = MK_BV (b5, b6, b7, b8);         \
    uint32_t r = OP (bv1, bv2, w);			    \
    validate (BUILTIN (bv1, bv2, w), r);		    \
  }

#define TEST_BVV_BVC_W(b1, b2, b3, b4, b5, b6, b7, b8, w) { \
    volatile uint32_t bv1 = MK_BV (b1, b2, b3, b4);         \
    uint32_t r = OP (bv1, MK_BV (b5, b6, b7, b8), w);	    \
    validate (BUILTIN (bv1, MK_BV (b5, b6, b7, b8), w), r); \
}

/* Macros to test builtins with a vector, a scalar and a variable or constant */
#define TEST_HVV_HSV_W(h1, h2, s1, w) {     \
    volatile uint32_t hv1 = MK_HV (h1, h2); \
    volatile int16_t  hs1 = s1;		    \
    uint32_t r = OP (hv1, s1, w);	    \
    validate (BUILTIN (hv1, hs1, w), r);    \
  }

#define TEST_HVV_UHSV_W(h1, h2, s1, w) {    \
    volatile uint32_t hv1 = MK_HV (h1, h2); \
    volatile uint16_t hs1 = s1;		    \
    uint32_t r = OP (hv1, s1, w);	    \
    validate (BUILTIN (hv1, hs1, w), r);    \
  }

#define TEST_HVV_HSC_W(h1, h2, s1, w) {     \
    volatile uint32_t hv1 = MK_HV (h1, h2); \
    uint32_t r = OP (hv1, s1, w);	    \
    validate (BUILTIN (hv1, s1, w), r);	    \
  }

#define TEST_BVV_BSV_W(b1, b2, b3, b4, s1, w) {     \
    volatile uint32_t bv1 = MK_BV (b1, b2, b3, b4); \
    volatile int8_t   bs1 = s1;			    \
    uint32_t r = OP (bv1, s1, w);		    \
    validate (BUILTIN (bv1, bs1, w), r);	    \
  }

#define TEST_BVV_UBSV_W(b1, b2, b3, b4, s1, w) {    \
    volatile uint32_t bv1 = MK_BV (b1, b2, b3, b4); \
    volatile uint8_t  bs1 = s1;			    \
    uint32_t r = OP (bv1, s1, w);		    \
    validate (BUILTIN (bv1, bs1, w), r);	    \
  }

#define TEST_BVV_BSC_W(b1, b2, b3, b4, s1, w) {     \
    volatile uint32_t bv1 = MK_BV (b1, b2, b3, b4); \
    uint32_t r = OP (bv1, s1, w);		    \
    validate (BUILTIN (bv1, s1, w), r);	    \
  }

/* Test builtins with vector, constant of variable and vector */
#define TEST_HVV_W_HVV(h1, h2, c, h3, h4) { \
    volatile uint32_t hv1 = MK_HV (h1, h2); \
    volatile uint32_t hv2 = MK_HV (h3, h4); \
    uint32_t r = OP (hv1, c, hv2);	    \
    validate (BUILTIN (hv1, c, hv2), r);    \
  }
#define TEST_HVC_W_HVV(h1, h2, c, h3, h4) {	    \
    volatile uint32_t hv2 = MK_HV (h3, h4);         \
    uint32_t r = OP (MK_HV (h1, h2), c, hv2);       \
    validate (BUILTIN (MK_HV (h1, h2), c, hv2), r); \
  }
#define TEST_BVV_W_BVV(b1, b2, b3, b4, c, b5, b6, b7, b8) { \
    volatile uint32_t bv1 = MK_BV (b1, b2, b3, b4);         \
    volatile uint32_t bv2 = MK_BV (b5, b6, b7, b8);         \
    uint32_t r = OP (bv1, c, bv2);			    \
    validate (BUILTIN (bv1, c, bv2), r);		    \
  }
#define TEST_BVC_W_BVV(b1, b2, b3, b4, c, b5, b6, b7, b8) { \
    volatile uint32_t bv2 = MK_BV (b5, b6, b7, b8);         \
    uint32_t r = OP (MK_BV (b1, b2, b3, b4), c, bv2);	    \
    validate (BUILTIN (MK_BV (b1, b2, b3, b4), c, bv2), r); \
  }

/* Macros to test builtins with three vectors */
#define TEST_BVV_BVV_BVV(b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12) { \
    volatile uint32_t bv1 = MK_BV (b1, b2,  b3,  b4);                         \
    volatile uint32_t bv2 = MK_BV (b5, b6,  b7,  b8);                         \
    volatile uint32_t bv3 = MK_BV (b9, b10, b11, b12);                        \
    uint32_t r = OP (bv1, bv2, bv3);			                      \
    validate (BUILTIN (bv1, bv2, bv3), r);			              \
  }

#define TEST_BVV_BVC_BVV(b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12) { \
    volatile uint32_t bv1 = MK_BV (b1, b2, b3, b4);                           \
    volatile uint32_t bv3 = MK_BV (b9, b10, b11, b12);                        \
    uint32_t r = OP (bv1, MK_BV (b5, b6, b7, b8), bv3);			      \
    validate (BUILTIN (bv1, MK_BV (b5, b6, b7, b8), bv3), r);		      \
}

/* Macro to test builtins with three vectors and a value. */
#define TEST_HVV_HVV_HVV_W(h1, h2, h3, h4, h5, h6, w) {	\
    volatile uint32_t hv1 = MK_HV (h1, h2);             \
    volatile uint32_t hv2 = MK_HV (h3, h4);             \
    volatile uint32_t hv3 = MK_HV (h5, h6);             \
    uint32_t r = OP (hv1, hv2, hv3, w);	                \
    validate (BUILTIN (hv1, hv2, hv3, w), r);           \
  }

#endif	/* CV_SIMD_HELPERS_H */
