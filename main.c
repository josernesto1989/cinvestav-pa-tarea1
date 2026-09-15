#include <stdio.h>
#include <immintrin.h> // Contains _mulx_u64 and _addcarry_u64



typedef union {
        unsigned long long uint64[2];
}uint128;

typedef union {
        unsigned long long uint64[4];
}uint256;

uint256 mul128x128_256(uint128 A, uint128 B) {
    uint256 res;
    unsigned long long hi;
    unsigned char carry = 0;

    // Calcula productos cruzados (64x64 -> 128 bits each)
    // res0 = A.baja * B.baja
    res.uint64[0] = _mulx_u64(A.uint64[0], B.uint64[0], &hi);
    unsigned long long p0_hi = hi;

    // P1 = A.baja * B.alta
    unsigned long long p1_lo = _mulx_u64(A.uint64[0], B.uint64[1], &hi);
    unsigned long long p1_hi = hi;

    // P2 = A.alta * B.baja
    unsigned long long p2_lo = _mulx_u64(A.uint64[1], B.uint64[0], &hi);
    unsigned long long p2_hi = hi;

    // P3 = A.alta * B.alta
    unsigned long long p3_lo = _mulx_u64(A.uint64[1], B.uint64[1], &hi);
    unsigned long long p3_hi = hi;

    //acumulamos los resultados en res desde el bit 64 hasta el bit 255, usando _addcarry_u64 para manejar los carrys

    // (Bits 64..127): p0_hi + p1_lo + p2_lo
    carry = _addcarry_u64(0,     p0_hi,  p1_lo,  &res.uint64[1]);
    carry = _addcarry_u64(carry, res.uint64[1], p2_lo,  &res.uint64[1]);

    // (Bits 128..191): p1_hi + p2_hi + p3_lo + carry
    carry = _addcarry_u64(carry, p1_hi,  p2_hi,  &res.uint64[2]);
    carry = _addcarry_u64(carry, res.uint64[2], p3_lo,  &res.uint64[2]);

    // (Bits 192..255): p3_hi + carry
    _addcarry_u64(carry, p3_hi, 0, &res.uint64[3]);

    return res;
}
void print_uint256(uint256 num) {
    printf("0x%016llX%016llX%016llX%016llX\n", num.uint64[3], num.uint64[2], num.uint64[1], num.uint64[0]);
}
void print_uint128(uint128 num) {
    printf("0x%016llX%016llX\n", num.uint64[1], num.uint64[0]);
}
int main(void) {
    uint128 A;
   A.uint64[0]= 1ULL;
   A.uint64[1] = 1ULL ;
    uint128 B;
   B.uint64[0] = 2ULL;
   B.uint64[1] = 1ULL ;
        B.uint64[0]=1;
        B.uint64[1]=0;
    uint256 R = mul128x128_256(A, B);

    print_uint128(A);
    print_uint128(B);
    
    print_uint256(R);

    return 0;
}