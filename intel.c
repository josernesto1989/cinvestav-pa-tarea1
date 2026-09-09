#include <stdio.h>

// 1. Detectar si estamos en arquitectura ARM (Mac M1) o Intel
#if defined(__ARM_NEON) || defined(__aarch64__)
    // Si es ARM, incluimos el traductor automático
    // Nota: Necesitas descargar "sse2neon.h" y ponerlo en tu proyecto
    #include "./sse2neon.h"
#else
    // Si es Intel tradicional, usamos la cabecera nativa
    #include <emmintrin.h> 
#endif

int main() {
    // Arreglos de prueba (alineados o estándar)
    float array1[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    float array2[4] = {5.0f, 6.0f, 7.0f, 8.0f};
    float resultado[4];

    // 2. Cargar los datos en registros SIMD (Intel __m128)
    // sse2neon mapea __m128 internamente a float32x4_t de ARM NEON
    __m128 v1 = _mm_loadu_ps(array1);
    __m128 v2 = _mm_loadu_ps(array2);

    // 3. Realizar la suma en paralelo
    // En M1, esto se convertirá automáticamente en la instrucción 'fadd' de NEON
    __m128 v_res = _mm_add_ps(v1, v2);

    // 4. Almacenar el resultado de vuelta en memoria
    _mm_storeu_ps(resultado, v_res);

    // Imprimir el resultado
    printf("Resultado: %.1f, %.1f, %.1f, %.1f\n", 
            resultado[0], resultado[1], resultado[2], resultado[3]);

    return 0;
}
