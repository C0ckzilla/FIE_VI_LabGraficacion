

#include <omp.h>
#include <stdio.h>

int main() {
    #pragma omp parallel
    printf("Hello from thread %d, nthreads %d\n", omp_get_thread_num(), omp_get_num_threads());
    return 0;
}

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

int main ()
{
    srand(time(0)); // Mueve la inicialización del generador de números aleatorios fuera del bucle

    #pragma omp parallel
    {
        int hilos = omp_get_num_threads();
        int actual = omp_get_thread_num();

        
        printf("Hola mundo desde el hilo %d de %d\n", actual, hilos);

        float arreglo[6]; // Mueve la declaración del arreglo fuera del bucle

        #pragma omp for
        for (int i = 0;  i <6; i++) {
            arreglo[i] = (float)rand() / (float)RAND_MAX;
            printf("arreglo[%d] = %f\n", i, arreglo[i]);
        }
    }
}