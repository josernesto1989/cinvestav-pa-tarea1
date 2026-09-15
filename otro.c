#include <stdio.h>

// 1. Detectar si estamos en arquitectura ARM (Mac M1) o Intel
#if defined(__ARM_NEON) || defined(__aarch64__)
    // Si es ARM, incluimos el traductor automático
    #include "./sse2neon.h"
    #include <stdint.h>
    uint64_t mulx_u64_portable(uint64_t a, uint64_t b, uint64_t *hi) {
    unsigned __int128 res = (unsigned __int128)a * b;
    *hi = (uint64_t)(res >> 64);
    return (uint64_t)res;
}
#else
    #include <emmintrin.h> 
#endif
union number_128{
        unsigned int f[4];
        __m128 m;
    } ;


int main() {

    union number_128{
        unsigned int integer32[4];
        unsigned long long integer64[2];
        __m128 integer128;
    } ;

    union number_128 n1, n2, n3;
    n1.integer32[0] = 4294967295;
    n1.integer32[1] = 4294967295;
    n1.integer32[2] = 0;
    n1.integer32[3] = 0;

    n2.integer32[0] = 4294967295;
    n2.integer32[1] = 4294967295;
    n2.integer32[2] = 0;
    n2.integer32[3] = 0;

    #if defined(__ARM_NEON) || defined(__aarch64__)
        n3.integer64[0] = mulx_u64_portable(n1.integer64[0], n2.integer64[0], &n3.integer64[1]);
#else
    n3.integer64[1] = _mulx_u64(n1.integer64[0],n2.integer64[0],&n3.integer64[0]);
#endif
    
    
    
    
    printf("Resultado: %llu%llu\n\n",n3.integer64[1],n3.integer64[0]);


    return 0;
}
