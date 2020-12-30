

#ifndef _SECP256K1_SCALAR_REPR_
#define _SECP256K1_SCALAR_REPR_

#include <stdint.h>


typedef struct {
    uint32_t d[8];
} secp256k1_scalar;

#define SECP256K1_SCALAR_CONST(d7, d6, d5, d4, d3, d2, d1, d0) {{(d0), (d1), (d2), (d3), (d4), (d5), (d6), (d7)}}

#endif








